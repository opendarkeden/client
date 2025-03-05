//----------------------------------------------------------------------
// MFakeCreature.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "ClientDef.h"
#include "MFakeCreature.h"
#include "MTopView.h"
#include "DebugInfo.h"
#include "MPlayer.h"
#include "SkillDef.h"
#include "UserInformation.h"
#include "vs_ui.h"
extern DWORD g_CurrentTime;
extern bool			HasEffectStatusSummonSylph( MCreature* pCreature );
extern void			RemoveEffectStatusSummonSylph( MCreature* pCreature );
extern	LONG g_SECTOR_WIDTH;
extern	LONG g_SECTOR_HEIGHT;
extern	LONG g_SECTOR_WIDTH_HALF;
extern	LONG g_SECTOR_HEIGHT_HALF;
extern	LONG g_SECTOR_SKIP_PLAYER_LEFT;
extern	LONG g_SECTOR_SKIP_PLAYER_UP;
extern	LONG g_TILESURFACE_SECTOR_WIDTH;
extern	LONG g_TILESURFACE_SECTOR_HEIGHT;
extern	LONG g_TILESURFACE_SECTOR_OUTLINE_RIGHT;
extern	LONG g_TILESURFACE_SECTOR_OUTLINE_DOWN;
extern	LONG g_TILESURFACE_WIDTH;
extern	LONG g_TILESURFACE_HEIGHT;
extern	LONG g_TILESURFACE_OUTLINE_RIGHT;
extern	LONG g_TILESURFACE_OUTLINE_DOWN;
BYTE GetCreatureActionCountMax( const MCreature* pCreature, int action );

extern POINT GetNextTileByDirection(int TileX, int TileY, byte Dir);
#define MoveNextPosition()					\
											\
		MovePosition( m_NextX, m_NextY );	\
		int dir = m_Direction;				\
											\
		SetAction( m_MoveAction );			\
		m_Direction = m_NextDirection;		\
		m_DirectionMove = m_NextDirection;	\
		ActionMove();						\
		m_Direction = dir;					\
											\
		m_NextX = SECTORPOSITION_NULL;		\
		m_NextY = SECTORPOSITION_NULL;		\
		m_NextDirection = DIRECTION_NULL;

#define	ActionMoveNextPosition()					\
		if (!m_bFastMove)							\
		{											\
			if (m_NextX != SECTORPOSITION_NULL		\
				&& m_NextDirection != SECTORPOSITION_NULL)	\
			{										\
				MoveNextPosition();					\
			}										\
			else if (m_NextAction==m_MoveAction)	\
			{										\
				SetAction( m_NextAction );			\
				ActionMove();						\
			}										\
		}
//----------------------------------------------------------------------
// static
//----------------------------------------------------------------------
DWORD MFakeCreature::m_FakeID = 0;

//----------------------------------------------------------------------
// Get FakeID
//----------------------------------------------------------------------
DWORD
MFakeCreature::GetFakeID()
{
	// 서버에서는 10000부터 objectID를 사용한다..고 가정하고...
	// 냠냠.... 하드 코딩~~ 케케..	
	MCreature *pFakeCreature = NULL;

	do
	{
		m_FakeID++;
		if (m_FakeID==10000)
		{
			m_FakeID = 0;
		}
		pFakeCreature = g_pZone->GetFakeCreature(m_FakeID);

	} while(pFakeCreature != NULL);


	return m_FakeID;
}

//----------------------------------------------------------------------
// 
// constructor / destructor
//
//----------------------------------------------------------------------
MFakeCreature::MFakeCreature()
{
	m_FakeCreatureType = FAKE_CREATURE_STAND;

	m_MoveType = CREATURE_FAKE_NO_BLOCK;

	m_FakeX = 0;
	m_FakeY = 0;
	m_Delay = 0;
	m_OwnerID = OBJECTID_NULL;
	m_bTraceFlag = false;

	m_ID = GetFakeID();

	m_pPetItem = NULL;

	// 목적지	
	m_DestX = SECTORPOSITION_NULL;
	m_DestY = SECTORPOSITION_NULL;

	// 다음 목적지
	m_NextDestX = SECTORPOSITION_NULL;
	m_NextDestY = SECTORPOSITION_NULL;

	// 못 갔던 곳..
	m_BlockDestX = SECTORPOSITION_NULL;
	m_BlockDestY = SECTORPOSITION_NULL;

	m_nextMoveTime = timeGetTime()+(rand()%8+3)*1000;
	m_PatrolCount = rand()%5+1;

	m_TurretDelay = 0;
	m_WildWolf_Mode = 0;
	m_IsEatCorps = false;
}

MFakeCreature::~MFakeCreature()
{
	if(m_pPetItem != NULL)
	{
		TYPE_OBJECTID id = GetOwnerID();
		if(g_pPlayer != NULL && id != g_pPlayer->GetID())
		{
			delete m_pPetItem;
			m_pPetItem = NULL;
		}
	}

	// remove priority queue 
	RemoveNodes();

	// remove list
	m_listDirection.clear();

}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Set Fake Creature
//----------------------------------------------------------------------
void				
MFakeCreature::SetFakeCreatureType(FAKE_CREATURE_TYPE fct)
{ 
	m_FakeCreatureType = fct; 

	switch (fct)
	{
		//----------------------------------------------
		// 흐릿한 한번의 동작 후에 사라짐
		//----------------------------------------------
		case FAKE_CREATURE_FAST_MOVE_ACTION :		// [새기술]
		case FAKE_CREATURE_FADE_ACTION :
		case FAKE_CREATURE_BRIGHTNESS :
			SetFade();
		break;

		//----------------------------------------------
		// 포탈로 들어가는 경우
		//----------------------------------------------
		case FAKE_CREATURE_TO_PORTAL :
			SetCutHeightFrom( 0, 80, 5 );
		break;

		//----------------------------------------------
		// 빙글빙글
		//----------------------------------------------
		case FAKE_CREATURE_TURNING_MOVE :
			SetFade();
			break;
		case FAKE_CREATURE_ROCKET:
			break;
	}
}

//----------------------------------------------------------------------
// Set ActionResult  [새기술]
//----------------------------------------------------------------------
// Creature에 넣어야 되는데.. 컴파일 시간을 줄이기 위해서 -_-;;
//----------------------------------------------------------------------
/*
MActionResult*		
MFakeCreature::SetActionResult(MActionResult* pResult)
{
	if (m_pActionResult != NULL)
	{		
		// 이미 있던 결과를 처리해준다.	
		// [ TEST CODE ]
		// 결과 처리..
		//
		// (!) m_pActionResult값이 Execute에서 변할 수 있어서 
		//		저장했다가 지워줘야 한다.
		MActionResult* pOldResult = m_pActionResult;

		m_pActionResult = NULL;

		pOldResult->Execute();

		delete pOldResult;			
	}

	// 결과를 설정
	m_pActionResult = pActionResult;
}
*/

//----------------------------------------------------------------------
// Action
//----------------------------------------------------------------------
void
MFakeCreature::Action()
{
	if( m_Delay > 0 )
	{
		m_Delay --;
		return;
	}

	//--------------------------------------------------------
	// 채팅 String 어둡게 할 시간
	//--------------------------------------------------------
	if (m_NextChatFadeTime < g_CurrentTime)
	{
		FadeChatString();		
	}
	//--------------------------------------------------------
	// Status 상태 변화	- HP, MP 변화
	//--------------------------------------------------------
	UpdateStatus();	
	
	//--------------------------------------------------------
	// 특수한 효과들
	//--------------------------------------------------------
	UpdateInvisible();
	UpdateCasket();
	UpdateCutHeight();
	UpdateTurning();

	//--------------------------------------------------------
	// Fake
	//--------------------------------------------------------
	UpdateFake();

	UpdateGhost();
	
	switch(GetFakeCreatureType())
	{
		case FAKE_CREATURE_TRACE:
			KeepTraceCreature();
			break;
		case FAKE_CREATURE_GHOST:
			KeepWanderCreature();
			break;
		case FAKE_CREATURE_ROCKET:
			KeepRocketLuncher();
			break;
		case FAKE_CREATURE_WILD_WOLF:
			KeepWildWolfAttack();
			break;

	}
	//--------------------------------------------------------
	// 방향 전환을 자연스럽게
	//--------------------------------------------------------
	ChangeNearDirection();
	//m_CurrentDirection = m_Direction;

	
	BOOL bSlayer = IsSlayer();

	if (m_nNextUsedActionInfo!=ACTIONINFO_NULL)
	{		
		// affectUsedActionInfo를 실행하고 나면
		// NextActionInfo가 바뀔 수가 있다.			
		TYPE_ACTIONINFO nextUsedActionInfo = m_nNextUsedActionInfo;
		TYPE_ACTIONINFO nextNextUsedActionInfo = ACTIONINFO_NULL;

		//--------------------------------------------------------
		// 이전에 사용 중인 기술이 있다면... 바로 적용 시킨다.
		//--------------------------------------------------------
		if (m_nUsedActionInfo!=ACTIONINFO_NULL)
		{	
			m_nNextUsedActionInfo = ACTIONINFO_NULL;

			// castingEffect가 아직 출력 안됐으면 출력시킨다.
			if (GetActionInfoCastingStartFrame(m_nUsedActionInfo) >= m_ActionCount)
			{
				AttachCastingEffect( m_nUsedActionInfo, TRUE );
			}

			AffectUsedActionInfo( m_nUsedActionInfo );
			m_nUsedActionInfo	= ACTIONINFO_NULL;

			nextNextUsedActionInfo = m_nNextUsedActionInfo;
		}

		m_nUsedActionInfo = nextUsedActionInfo;
		m_nNextUsedActionInfo = nextNextUsedActionInfo;

		int actionInfoAction = GetActionInfoAction(m_nUsedActionInfo);

		// 맞는 동작이면.. 피 튀김..
		if (actionInfoAction==ACTION_DAMAGED)
		{
			if //(m_Action!=ACTION_STAND 
				//&& (!IsSlayer() || IsSlayer && m_Action!=ACTION_SLAYER_MOTOR_STAND)
				//&& 
				(GetActionInfoCastingStartFrame(m_nUsedActionInfo) < m_ActionCount)
			{
				AttachCastingEffect( m_nUsedActionInfo, TRUE );

				AffectUsedActionInfo( m_nUsedActionInfo );
		
				m_nUsedActionInfo = ACTIONINFO_NULL;					
			}
		}

		// 2001.05.21 추가
		// 기술 동작에서 ACTION_STAND는 보여주지 않는다.
		if (actionInfoAction!=ACTION_STAND)
		{
			SetAction( actionInfoAction );
		}
	}
	//--------------------------------------------------------
	// 기억된 다음 행동... 
	//--------------------------------------------------------
	else if (m_bNextAction 
			|| m_NextAction!=ACTION_STAND 
			&& (!bSlayer || bSlayer && m_NextAction != ACTION_SLAYER_MOTOR_STAND)
			&& m_NextAction!=m_MoveAction)
	{
		SetAction( m_NextAction );
	}	 

	//--------------------------------------------------------
	// 마비 됐을 때..
	//--------------------------------------------------------
	if (HasEffectStatus(EFFECTSTATUS_CURSE_PARALYSIS)
		|| HasEffectStatus(EFFECTSTATUS_EXPLOSION_WATER)
		|| (HasEffectStatus(EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS) && !IsSlayer())
		|| HasEffectStatus(EFFECTSTATUS_GLACIER))
	{			
		//--------------------------------------------------------
		// 기술을 사용하려는 경우
		//--------------------------------------------------------
		if (m_nUsedActionInfo!=ACTIONINFO_NULL)
		{
			AffectUsedActionInfo(m_nUsedActionInfo);
			m_nUsedActionInfo	= ACTIONINFO_NULL;
		}
	}
	//--------------------------------------------------------
	// 일반적인 경우
	//--------------------------------------------------------
	else
	{
		//--------------------------------------------------------
		// Action 처리
		//--------------------------------------------------------
		if (m_ActionCount < m_ActionCountMax)
		{		
			//--------------------------------------------------------
			// 기술을 사용하려는 경우
			//--------------------------------------------------------
			if (m_nUsedActionInfo!=ACTIONINFO_NULL)
			{
				ActionEffect();
			}
			//--------------------------------------------------------
			// 행동..
			//--------------------------------------------------------
			else
			{
				// 적절한 Action 수행
				//switch (m_Action)
				if (m_Action==ACTION_STAND 
					|| IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND)
				{
					m_ActionCount++;
				}

				/*
				else if (m_Action==ACTION_MOVE
					|| IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_MOVE)
				{
						// 걷는 동작은 따로 처리한다.
						//ActionMove();
				}
				*/
				
				//	default :
				else 
				{
					ActionEffect();
				}
			}
		}
		
		//--------------------------------------------------------
		// Action이 끝난 경우...
		//--------------------------------------------------------
		if (m_ActionCount>=m_ActionCountMax)
		{
			//--------------------------------------------------------
			// 살아 있는 경우만
			//--------------------------------------------------------
			if (m_bAlive)
			{
				//--------------------------------------------------------
				// 목표 타일에 도착한 상태에서..
				// 계속 서 있거나.. 다 걸은 경우는.. 
				// 제자리에서 흔들거리는 모습을 표현해준다.
				//--------------------------------------------------------
				if (m_MoveCount==m_MoveCountMax)
				{		
					//#ifdef CONNECT_SERVER
					//--------------------------------------------------------
					// 다음 이동이 없다면.. 정지시킨다.
					//--------------------------------------------------------
					AffectMoveBuffer();

					BOOL bSlayer = IsSlayer();

					if (m_bNextAction 
						|| m_NextAction!=ACTION_STAND 
						&& (!bSlayer || bSlayer && m_NextAction!=ACTION_SLAYER_MOTOR_STAND))
					{
						m_Action = ((m_MoveDevice==MOVE_DEVICE_RIDE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND);
						SetAction( m_NextAction );
					}	 
					else
					{
						SetAction( (m_MoveDevice==MOVE_DEVICE_RIDE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND );				
					}
					//#else
					//	SetAction( m_MoveAction );				
					//#endif
				}
				//--------------------------------------------------------
				// 걷다가 다른 action을 보여줬거나 계속 걷던 중이다.
				// 다시 걷는다.
				//--------------------------------------------------------
				else 
				{			
					m_Action = m_MoveAction;				
					
					ActionMove();

					if (m_MoveCount>=m_MoveCountMax)
					{
						// 다음 이동할 곳이 있으면..
//						if (m_NextX != SECTORPOSITION_NULL)
						if (m_NextX != SECTORPOSITION_NULL
							&& m_NextDirection != DIRECTION_NULL)
						{		
//							DEBUG_ADD_FORMAT("[ Apply Buffering Move ] [ID=%d] Current(%d, %d) Dir(%d) --> Next(%d, %d) Dir(%d)", 
//															m_ID,
//															m_X, m_Y, m_Direction, 
//															m_NextX,m_NextY, m_NextDirection);
//							
//							MovePosition( m_NextX, m_NextY );
//							m_Direction = m_NextDirection;					
//							
//							// 이동하게 한다.
//							SetAction( m_MoveAction );
//
//							m_NextX = SECTORPOSITION_NULL;
//							m_NextY = SECTORPOSITION_NULL;
//							m_NextDirection = DIRECTION_NULL;
							MovePosition( m_NextX, m_NextY );
							int dir = m_Direction;

							SetAction( m_MoveAction );
							m_Direction = m_NextDirection;
							m_DirectionMove = m_NextDirection;
							ActionMove();
							m_Direction = dir;

							m_NextX = SECTORPOSITION_NULL;
							m_NextY = SECTORPOSITION_NULL;
							m_NextDirection = DIRECTION_NULL;
						}
						else
						{
							//#ifdef CONNECT_SERVER

							//--------------------------------------------------------
							// 다음 이동이 없다면.. 정지시킨다.
							//--------------------------------------------------------
							AffectMoveBuffer();

							BOOL bSlayer = IsSlayer();

							if (m_bNextAction 
								|| m_NextAction!=ACTION_STAND 
								&& (!bSlayer || bSlayer && m_NextAction!=ACTION_SLAYER_MOTOR_STAND))
							{
								m_Action = ((m_MoveDevice==MOVE_DEVICE_RIDE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND);
								SetAction( m_NextAction );
							}	 
							else
							{
								SetAction( (m_MoveDevice==MOVE_DEVICE_RIDE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND );				
							}
							//#else
							//	SetAction( m_MoveAction );				
							//#endif
						}
					}
				}
			}
		}
	}
}

//----------------------------------------------------------------------
// Set FakeCreature FastMoveAction [새기술]
//----------------------------------------------------------------------
void
MFakeCreature::SetFakeCreatureFastMoveAction(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY,
											 TYPE_ACTIONINFO skillID, 
											 TYPE_OBJECTID targetID)
{
	FastMovePosition(sX, sY);

	m_nBasicActionInfo = m_nSpecialActionInfo = skillID;
	m_TraceID = targetID;
}


//----------------------------------------------------------------------
// Set FakePosition
//----------------------------------------------------------------------
void				
MFakeCreature::SetFakePosition(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)	
{ 
	m_FakeX = sX; 
	m_FakeY = sY; 

	switch (m_FakeCreatureType)
	{
		//----------------------------------------------
		// 빙글빙글
		//----------------------------------------------
		case FAKE_CREATURE_TURNING_MOVE :
		{
			//int actionCountMax = 2 * (*g_pCreatureTable)[m_CreatureType].GetActionCount( m_Action );
			int actionCountMax = 2 * GetCreatureActionCountMax( this, m_Action );

			if (actionCountMax==0)
			{
				actionCountMax = 1;
			}

			// 목표 200, 현재 100
			// gapX = 200 - 100 = 100
			// m_cX = 100 / c
			int gapX = m_FakeX - m_X;
			int gapY = m_FakeY - m_Y;
			float step = actionCountMax;
			m_FakeCX = gapX*TILE_X / step;
			m_FakeCY = gapY*TILE_Y / step;

			m_sX = 0;
			m_sY = 0;
			m_FakeSX = 0.0f;
			m_FakeSY = 0.0f;
		}
		break;
	}
}

//----------------------------------------------------------------------
// Update Fake
//----------------------------------------------------------------------
void
MFakeCreature::UpdateFake()
{
	switch (m_FakeCreatureType)
	{
		//----------------------------------------------
		// 빙글빙글
		//----------------------------------------------
		case FAKE_CREATURE_TURNING_MOVE :
		{
			m_Direction = ((m_Direction+1) & 0x07);

			if (m_X!=m_FakeX || m_Y!=m_FakeY)
			{			
				m_FakeSX += m_FakeCX;
				m_FakeSY += m_FakeCY;

				int sX = m_FakeSX;
				int sY = m_FakeSY;

				// 한 타일 좌표를 넘게 이동했으면..
				if (abs(sX) >= TILE_X || abs(sY) >= TILE_Y)
				{
					// 새로운 map좌표 계산
					int mapX = m_X;
					int mapY = m_Y;
					int	pixelX = MTopView::MapToPixelX(mapX);
					int	pixelY = MTopView::MapToPixelY(mapY);

					pixelX += sX;
					pixelY += sY;

					m_X = MTopView::PixelToMapX(pixelX);
					m_Y = MTopView::PixelToMapY(pixelY);

					sX %= TILE_X;
					sY %= TILE_Y;

					m_FakeSX = sX;
					m_FakeSY = sY;
				}
			}			
			else
			{
				m_FakeSX = 0;
				m_FakeSY = 0;
				m_FakeCX = 0;
				m_FakeCY = 0;
			}
		}
		break;
	}
}

//----------------------------------------------------------------------
// IsFakeEnd
//----------------------------------------------------------------------
bool
MFakeCreature::IsFakeEnd()
{
	switch (m_FakeCreatureType)
	{
		case FAKE_CREATURE_FADE_ACTION :
			return (m_Action==ACTION_STAND
					|| m_ActionCount>=m_ActionCountMax);
		break;

		case FAKE_CREATURE_TO_PORTAL :
			return m_CutHeightCount >= m_CutHeightCountLast;
		break;

		case FAKE_CREATURE_TURNING_MOVE :
			return m_X==m_FakeX 
					&& m_Y==m_FakeY 
					&& m_ActionCount>=m_ActionCountMax;
		break;

		// [새기술]
		case FAKE_CREATURE_FAST_MOVE_ACTION :
			return !m_bFastMove
					&& m_nUsedActionInfo==ACTIONINFO_NULL
					&& m_nNextUsedActionInfo==ACTIONINFO_NULL
					&& (m_Action==ACTION_STAND
						|| m_ActionCount>=m_ActionCountMax);
		break;
		case FAKE_CREATURE_ROCKET:
			{
				MCreature *pCreature = g_pZone->GetCreature(m_TraceID);	
//			if(pCreature != NULL)
//			{
//				if(GetX() == pCreature->GetX() && GetY() == pCreature->GetY())
//				{
//					ExecuteActionInfoFromMainNode(RESULT_TEMP_SKILL_PLASMA_ROCKET_LAUNCHER,pCreature->GetX(), pCreature->GetY(), 0,pCreature->GetDirection(),	0,	
//							pCreature->GetX(), pCreature->GetY(), 0, 10, NULL, false);			
//					return true;
//				}
//			}
				if( timeGetTime() > m_nextMoveTime || NULL == pCreature)
				{
					if(pCreature != NULL)
						ExecuteActionInfoFromMainNode(SKILL_CLIENT_PLASMA_ROCKET_LAUNCHER_BLOW,pCreature->GetX(), pCreature->GetY(), 0,pCreature->GetDirection(),	0,	
								pCreature->GetX(), pCreature->GetY(), 0, 10, NULL, false);			
					return true;
				}
			}
			break;
		case FAKE_CREATURE_WILD_WOLF:
			{
				if(WILD_WOLF_MODE_REMOVE == m_WildWolf_Mode && m_PatrolCount > 30)
					return true;
			}
			break;
		case FAKE_CREATURE_DRAGON_TORNADO:
			{
				POINT	firstSector, endSector;
				firstSector.x = g_pPlayer->GetX() + g_SECTOR_SKIP_PLAYER_LEFT-1; 
				firstSector.y = g_pPlayer->GetY() + g_SECTOR_SKIP_PLAYER_UP-1;
				endSector.x	  = firstSector.x+g_SECTOR_WIDTH+2;
				endSector.y   = firstSector.y+g_SECTOR_HEIGHT+2;
				// 화면 밖으로 가믄 없애자..일단..
				if(!IsInSector(firstSector.x, firstSector.y, endSector.x, endSector.y))
					return true;
			}
			break;
	}

	return false;
}

//----------------------------------------------------------------------
// Packet Special Action To Other [새기술]
//----------------------------------------------------------------------
void		
MFakeCreature::PacketSpecialActionToOther(TYPE_ACTIONINFO nActionInfo, TYPE_OBJECTID id, MActionResult* pActionResult)
{
	MCreature::PacketSpecialActionToOther(nActionInfo, id, pActionResult);

	switch (m_FakeCreatureType)
	{
		case FAKE_CREATURE_FAST_MOVE_ACTION :
		{		
			int dir = GetDirectionToPosition( m_TraceX, m_TraceY );
			SetDirection( dir );
			SetCurrentDirection( dir );
		}
		break;
	}
}

void
MFakeCreature::KeepTraceCreature()
{
	MCreature *pCreature = g_pZone->GetCreature(m_TraceID);

	if(pCreature != NULL)
	{
		UpdateTurret();

		int distance = max(abs(pCreature->GetX()-m_X), abs(pCreature->GetY()-m_Y));
		if(distance > 6)
		{
			m_FinalTurretDirect = 0xFF;
			RemoveNodes();
			m_bTraceFlag = true;
			SetFakeCreatureFastMoveAction(pCreature->GetX(), pCreature->GetY(), ACTIONINFO_NULL, pCreature->GetID());
		}
		else
		if(distance > 3)
		{
			m_FinalTurretDirect = 0xFF;
			m_bTraceFlag = true;
			RemoveNodes();
			SetNextDestination(pCreature->GetX(), pCreature->GetY());
			// 하드하드 ㅋㅋㅋ -ㅅ-
			if(m_Action == ACTION_VAMPIRE_DRAIN)
			{
				if(m_ActionCount < m_ActionCountMax-13)
					m_ActionCount = m_ActionCountMax-13;
				
				SetNextActionToMove();
			}
			else
			{
				if( m_Action != ACTION_MOVE )
					SetAction(m_MoveAction);
			}
		}

		static BYTE lastAction = ACTION_STAND;
		if(lastAction != ACTION_STAND && m_Action == ACTION_STAND)
		{
			// 멈췄다
			m_bTraceFlag = false;
		}
		lastAction = m_Action;

		if(m_bFastMove == false && timeGetTime() >= m_nextMoveTime)
		{
			if(m_PatrolCount <= 0)
			{
				MPetItem *pPetItem = GetPetItem();
				if(pPetItem != NULL)
				{
					switch(pPetItem->GetItemType())
					{
					case 1:
					case 2:
						m_nextMoveTime = timeGetTime()+(rand()%5+3)*1000;
						m_PatrolCount = rand()%5+3;
						SetAction(ACTION_VAMPIRE_DRAIN);
						break;

					case 3:
						m_PatrolCount = rand()%3+2;
						m_nextMoveTime = timeGetTime()+(rand()%3+2)*1000;
						ExecuteActionInfoFromMainNode(
							SKILL_CLIENT_TANK_SMOKE,										// 사용 기술 번호
							
							0, 0, 0,
							DIRECTION_DOWN, // 사용 방향
							
							GetID(),												// 목표에 대한 정보
							0, 0, 0, 
							
							0,													// 기술의 (남은) 지속 시간		
							
							NULL,
							
							false);	
						break;
					}
				}
			}
			else if(m_Action == ACTION_STAND && m_PatrolCount > 0 && GetPetItem() != NULL)
			{
				MPetItem *pPetItem = GetPetItem();

				if(GetCreatureType() == 702 || GetCreatureType() == 703 || GetCreatureType() == 704)
				{
					SetTurretFinalDirection( rand()/(RAND_MAX/32) );
					m_nextMoveTime = timeGetTime()+(rand()%3+2)*1000;
					m_PatrolCount--;
				}
				else
				{
					m_nextMoveTime = timeGetTime()+(rand()%5+3)*1000;
					m_PatrolCount--;
					int minDistance = 1;

					if(pPetItem != NULL)
					{
					int petLevel = GetPetItem()->GetNumber();

					if(petLevel >= 30)
						minDistance = 2;
					else if(petLevel >= 20)
						minDistance = 1;
					}

					POINT p = CaculatePetPosition(pCreature->GetX(), pCreature->GetY(), 2, minDistance, GetMoveType(), GetX(), GetY());
					SetNextDestination(p.x, p.y);
					SetAction( m_MoveAction );
				}
			}
		}


//		else if(distance <=1)
//			m_bTraceFlag = false;
//
//		if(m_bTraceFlag)
//		{		
//			BYTE dir = GetDirectionToPosition(pCreature->GetX(), pCreature->GetY());
//			TYPE_SECTORPOSITION x = m_X, y = m_Y;
//			GetPositionToDirection(x, y, dir);
//			PacketMove(x, y, dir);
//			SetNextDestination(pCreature->GetX(), pCreature->GetY());
//			SetAction( m_MoveAction );
//
//		}
	}
	else
	{
		SetStop();
	}
}

//----------------------------------------------------------------------
// Set Next Destination
//----------------------------------------------------------------------
// 다음에 이동할 목표 위치를 결정한다.
// 목표위치가 갈 수 없는 곳인 경우... 어떻게 할 것인가?
//----------------------------------------------------------------------
bool	
MFakeCreature::SetNextDestination(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)
{ 
	// 추적 중이 아니면서
	// 이미 목표위치에 있는 경우는 길찾기 필요없다.
	if (m_bTraceFlag==false && sX==m_X && sY==m_Y)
	{
		return false;
	}	

	// 잘못된 값
	// 이전에 못 갔던 곳..
	if (sX==SECTORPOSITION_NULL || sY==SECTORPOSITION_NULL
		|| sX==m_BlockDestX && sY==m_BlockDestY)
	{
		return false;
	}

	if(m_bTraceFlag == false)
	{
		BOOL bCanStand = m_pZone->CanMove(m_MoveType, sX,sY);
		if(!bCanStand)
		{
			extern POINT g_DirectionValue[MAX_DIRECTION];
			
			POINT MovePoint;

			if( sY > GetY() )
				MovePoint.y = -1;
			else if( sY < GetY() )
				MovePoint.y = 1;
			else
				MovePoint.y = 0;

			if( sX > GetX() )
				MovePoint.x = -1;
			else if( sX < GetX() )
				MovePoint.x = 1;
			else
				MovePoint.x = 0;

			int limit = 20;	// 유효값 20번내에 못찾으면 포기

			while( bCanStand == FALSE && !( GetX() == sX && GetY() == sY ) &&
				sX >= 0 && sY >= 0 
				&& sX < m_pZone->GetWidth() && sY < m_pZone->GetHeight()
				&& limit-- > 0
				)
			{
				if( GetX() != sX )sX += MovePoint.x;
				if( GetY() != sY )sY += MovePoint.y;
				bCanStand = m_pZone->CanMove( m_MoveType, sX,sY );
			};

			if( bCanStand == FALSE || GetX() == sX && GetY() == sY)
				return false;
		}
	}

	
	m_NextDestX = sX; 
	m_NextDestY = sY; 

//	SetDestination(m_NextDestX, m_NextDestY);

	return true;
}

//----------------------------------------------------------------------
// Get NextDestination()
//----------------------------------------------------------------------
void
MFakeCreature::GetNextDestination(POINT &dest)
{
	dest.x = m_NextDestX;
	dest.y = m_NextDestY;
}

//----------------------------------------------------------------------
// Get NextPosition()
//----------------------------------------------------------------------
// 길찾기에 의해서 정해진 길들 중에서
// 바로 다음에 가야할 Sector에 대한 좌표를 넘겨준다.
//
// '현재위치'에서 '다음 방향'에 대해 적절한 변화를 더해준다.
//----------------------------------------------------------------------
bool
MFakeCreature::GetNextPosition(POINT &next)
{
	// 갈 길이 정해지지 않은 경우
	if (m_listDirection.empty())
	{
		//next.x = SECTORPOSITION_NULL;
		//next.y = SECTORPOSITION_NULL;
		return false;
	}

	
	// 갈 길이 정해진 경우, 다음 좌표를 return한다.

	// 현재 위치에서 다음 방향에 대해서 목표 설정
	next.x = m_X;
	next.y = m_Y;
	
	switch (m_listDirection.front())
	{
		case DIRECTION_LEFTDOWN		: next.x--;	next.y++;	break;
		case DIRECTION_RIGHTUP		: next.x++;	next.y--;	break;
		case DIRECTION_LEFTUP		: next.x--;	next.y--;	break;
		case DIRECTION_RIGHTDOWN	: next.x++;	next.y++;	break;
		case DIRECTION_LEFT			: next.x--;				break;
		case DIRECTION_DOWN			:			next.y++;	break;
		case DIRECTION_UP			:			next.y--;	break;
		case DIRECTION_RIGHT		: next.x++;				break;
	}

	return true;
}


//----------------------------------------------------------------------
// Set Destination(sX, sY)
//----------------------------------------------------------------------
// 목표위치를 지정하는 순간에 목표위치까지의
// Best Path를 결정해서 m_listDirection에 저장해둔다.
//----------------------------------------------------------------------
bool
MFakeCreature::SetDestination(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)
{
	if(HasEffectStatus(EFFECTSTATUS_SLEEP)
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		)
		return false;

	// message : find path
	//DEBUG_ADD_FORMAT("........Find Path........");				

	#ifdef OUTPUT_DEBUG_PLAYER_ACTION
		DEBUG_ADD_FORMAT("SetDest(%d, %d)", sX, sY);
	#endif

	//-------------------------------------------------------
	// 살아있지 않은 경우..
	//-------------------------------------------------------
	if (!m_bAlive)
	{
		#ifdef OUTPUT_DEBUG_PLAYER_ACTION
			DEBUG_ADD("dead");
		#endif
		return false;
	}

	//-------------------------------------------------------
	// if not empty, delete all elements
	//-------------------------------------------------------
	if (!m_listDirection.empty())
	{
		m_listDirection.clear();
	}

	//-------------------------------------------------------
	// Zone의 영역 밖이면 check 안한다.
	//-------------------------------------------------------
	if (sX<0 || sY<0 
		|| sX>=m_pZone->GetWidth() || sY>=m_pZone->GetHeight()
		)
	{
		// Zone영역 밖이면 check 안 할 경우
		//m_ActionCount = m_ActionCountMax;
		m_MoveCount = m_MoveCountMax;
		m_DestX = SECTORPOSITION_NULL;
		m_DestY = SECTORPOSITION_NULL;

		#ifdef OUTPUT_DEBUG_PLAYER_ACTION
			DEBUG_ADD("outOfZone or Sanctuary");
		#endif

		return false;

		// 현재 진행 방향으로 이동하게 하는 경우
		/*
		POINT position = g_pTopView->GetSelectedSector(g_x, g_y);		
		BYTE direction = GetDirectionToPosition(position.x, position.y);		

		// 방향에 따른 위치를 결정한다.		
		MCreature::GetNextPosition(direction, position);
		
		sX = position.x;
		sY = position.y;
		
		// 현재 진행 방향이 Zone을 벗어나는 경우
		if (sX<0 || sY<0 
			|| sX>=m_pZone->GetWidth() || sY>=m_pZone->GetHeight()) 
		{
			return false;
		}
		*/
	}

	//-------------------------------------------------------
	// 추적 중일 경우, 이미 사정 거리에 있을 때...
	//-------------------------------------------------------
	if (m_bTraceFlag == true
		&& max(abs(m_TraceX-m_X), abs(m_TraceY-m_Y))<=1)
	{
		//m_ActionCount = m_ActionCountMax;
		m_MoveCount = m_MoveCountMax;

		m_DestX = SECTORPOSITION_NULL;
		m_DestY = SECTORPOSITION_NULL;
	
		#ifdef OUTPUT_DEBUG_PLAYER_ACTION
			DEBUG_ADD("already in dist");
		#endif

		return false;
	}

	/*
	// priority queue test code
	ofstream file("test.txt", ios::app);
	CString str;

	m_pqDNodes.push( new DNode(0,0,0,1) );
	m_pqDNodes.push( new DNode(0,0,0,4) );
	m_pqDNodes.push( new DNode(0,0,0,3) );
	m_pqDNodes.push( new DNode(0,0,0,8) );
	m_pqDNodes.push( new DNode(0,0,0,2) );
	m_pqDNodes.push( new DNode(0,0,0,4) );
	m_pqDNodes.push( new DNode(0,0,0,7) );

	while (!m_pqDNodes.empty())
	{
		DNode*	pDNode = m_pqDNodes.top();
		m_pqDNodes.pop();

		str.Format("Pop : (%d,%d) , [%d], dist=%d", pDNode->x, pDNode->y, pDNode->direction, pDNode->distance);
		
		file << str << endl;
	}

	return;
	*/

	//--------------------------------------------------------------
	// 갈 수 있는 곳이면
	//--------------------------------------------------------------
	// Best First Search
	//--------------------------------------------------------------
	BOOL	bCanStand = m_pZone->CanMove(m_MoveType, sX,sY);

#ifdef __EXPO_CLIENT__
	bCanStand |= (BOOL)g_UserInformation.Invisible;
#endif

	//--------------------------------------------------------------
	// 갈 수 있는 곳이거나
	// 추적중이면 Best Search~로 길을 찾는다.
	//--------------------------------------------------------------
	if (bCanStand || m_bTraceFlag == true) 
	{		
		//-------------------------------------------------------
		// 목표위치를 저장
		//-------------------------------------------------------
		m_DestX = sX;
		m_DestY = sY;

		//-------------------------------------------------------
		// priority queue를 초기화한다.
		//-------------------------------------------------------
		RemoveNodes();	

		//-------------------------------------------------------
		// visited를 모두 false로 해두고
		// 현재위치부터 방문하지 않은 곳들을 방문하기 시작한다.
		//-------------------------------------------------------
		POINT firstSector = g_pTopView->GetFirstSector();
		//int x0 = m_DestX - g_pClientConfig->MAX_FINDPATH_WIDTH, 
		//	x1 = m_DestX + g_pClientConfig->MAX_FINDPATH_WIDTH+1, 
		//	y0 = m_DestY - g_pClientConfig->MAX_FINDPATH_HEIGHT,
		//	y1 = m_DestY + g_pClientConfig->MAX_FINDPATH_HEIGHT+1;
		int x0 = firstSector.x - 1, 
			x1 = firstSector.x + g_SECTOR_WIDTH + 5, 
			y0 = firstSector.y - 1,
			y1 = firstSector.y + g_SECTOR_HEIGHT + 5;

		if (x0 < 0)						
			x0 = 0;
		if (x1 > g_pZone->GetWidth())	
			x1 = g_pZone->GetWidth();
		
		if (y0 < 0)				
			y0 = 0;
		if (y1 > g_pZone->GetHeight())	
			y1 = g_pZone->GetHeight();
		
//		int size = x1-x0;	// byte수
		for (int i=y0; i<y1; i++)
		{			
			for (int j=x0; j<x1; j++)
			{
				//m_ppVisited[i][j] = false;
				g_pZone->UnSetVisitedFlag(j, i);
			}
//			memset( m_ppVisited[i]+x0, false, size );
		}
			

		//-------------------------------------------------------
		//
		// 현재 위치
		//
		//-------------------------------------------------------

		//-------------------------------------------------------
		// 목적지를 향한 방향부터 길찾기를 한다.		
		//-------------------------------------------------------
		static BYTE FirstDirection = GetDirectionToPosition(m_DestX, m_DestY);
		static TYPE_SECTORPOSITION previousDestX = SECTORPOSITION_NULL;
		static TYPE_SECTORPOSITION previousDestY = SECTORPOSITION_NULL;

		//-------------------------------------------------------
		// 방금 전의 길찾기 했던거랑 목표 위치가 다른 경우에만 
		// 목적지를 향한 방향을 설정한다.
		//-------------------------------------------------------
		if (previousDestX!=m_DestX || previousDestY!=m_DestY)
		{
			FirstDirection = GetDirectionToPosition(m_DestX, m_DestY);
			previousDestX = m_DestX;
			previousDestY = m_DestY;
		}
		//-------------------------------------------------------
		// 같은 목표를 향해 나아가는 경우는 
		// 현재 Player의 방향부터 살펴본다.
		//-------------------------------------------------------
		else
		{
			FirstDirection = m_Direction;
		}
		
		DNode*	currentDNode = new DNode(
									m_X,
									m_Y,
									FirstDirection,
									CalculateDistance(m_X, m_Y),
									0
									);
	
		//-------------------------------------------------------
		// listDNodes에 넣어서 나중에 지워준다.
		//-------------------------------------------------------
		m_listDNodes.push_back( currentDNode );

		currentDNode->SetParent(NULL);
		m_pqDNodes.push(currentDNode);	
		
		POINT	next;
		int		dist;
		bool	bFound = false;		// 아직 도달하지 않았다는 의미

		while (!bFound && !m_pqDNodes.empty())
		{
			//-------------------------------------------------------
			// 제일 괜찮은(!) Node를 골라서(priority queue이다)
			// 어디로 가야할지에 대해서 계산해본다.
			//-------------------------------------------------------
			DNode* pDNode = m_pqDNodes.top();
			m_pqDNodes.pop();

			//-------------------------------------------------------
			// 완전히 도착한 경우
			//-------------------------------------------------------
			if (pDNode->distance==0) 
			{
				m_pqDNodes.push(pDNode);
				bFound = true;
				break;
			}
			
			//-------------------------------------------------------
			// 사정 거리 이내로 접근하기
			//-------------------------------------------------------
			if (m_bTraceFlag == true && 
				max(abs(pDNode->x-m_DestX), abs(pDNode->y-m_DestY))<=1)
			{
				m_pqDNodes.push(pDNode);
				bFound = true;
				break;
			}			

			int Direction = pDNode->direction;

			//-------------------------------------------------------
			// 8방향을 모두 check
			//-------------------------------------------------------
			for (int i=0; i<8; Direction++, i++)
			{		
				if (Direction==8) Direction=0;

				next.x = pDNode->x;
				next.y = pDNode->y;

				switch (Direction)
				{
					case DIRECTION_LEFTDOWN		: next.x--;	next.y++;	break;
					case DIRECTION_RIGHTUP		: next.x++;	next.y--;	break;
					case DIRECTION_LEFTUP		: next.x--;	next.y--;	break;
					case DIRECTION_RIGHTDOWN	: next.x++;	next.y++;	break;
					case DIRECTION_LEFT			: next.x--;				break;
					case DIRECTION_DOWN			: next.y++;				break;
					case DIRECTION_UP			: next.y--;				break;
					case DIRECTION_RIGHT		: next.x++;				break;
				}

				//-------------------------------------------------------
				// Zone의 영역 밖이면 check 안한다.
				//-------------------------------------------------------
				if (next.x<0 || next.y<0 
					|| next.x>=m_pZone->GetWidth() || next.y>=m_pZone->GetHeight()) continue;

				//-------------------------------------------------------
				// 갔던 곳이면 안 간다.
				//-------------------------------------------------------
				if (g_pZone->IsVisitedFlag(next.x, next.y)) continue;

				// 한 화면을 넘어가는 경우는 check하지 않는다.				
				if (next.x<x0 || next.y<y0 
					|| next.x>=x1 || next.y>=y1) continue;

				//-------------------------------------------------------
				// 갈 수 있으면 pqueue에 넣는다.
				//-------------------------------------------------------
				if (m_pZone->CanMove(m_MoveType, next.x, next.y))
				{				
					g_pZone->SetVisitedFlag(next.x, next.y);

					//-------------------------------------------------------
					// Node를 만들어서 priority queue에 추가한다.
					//-------------------------------------------------------
					// 거리 계산
					dist = CalculateDistance(next.x, next.y);

					// Node를 만들어서 추가
					DNode*	nextDNode = new DNode(
											next.x, 
											next.y, 
											Direction,
											dist,
											pDNode->step + 1);
					
				
					//-------------------------------------------------------
					// listDNodes에 넣어서 나중에 지워준다.
					//-------------------------------------------------------
					m_listDNodes.push_back( nextDNode );
					
					//CString str;
					//str.Format("NewNode : (%d,%d)-->(%d,%d) , [%d], dist=%d", next.x, next.y, m_Dest.x, m_Dest.y, i, dist);

					nextDNode->SetParent(pDNode);
					m_pqDNodes.push(nextDNode);					
				}

			
			}
		}

		//-------------------------------------------------------
		// 찾았을 경우 길을 정해야 한다.
		//-------------------------------------------------------
		if (bFound)
		{				
			//-------------------------------------------------------
			// m_pqDNodes로부터 list를 채운다. (parent를 따라가야한다)
			//-------------------------------------------------------

			// 목표위치
			DNode* currentNode = m_pqDNodes.top();
			m_pqDNodes.pop();
			
			while (currentNode->pParent!=NULL)
			{
				m_listDirection.insert( m_listDirection.begin(), currentNode->direction);
				currentNode = currentNode->pParent;			
			}

			RemoveNodes();

			m_BlockDestX = SECTORPOSITION_NULL;
			m_BlockDestY = SECTORPOSITION_NULL;
		}
		//-------------------------------------------------------
		// 길이 없을 경우 정지한다.
		//-------------------------------------------------------
		else
		{
			// 추적 중지
			if (m_bTraceFlag == true)				
			{
				m_MoveCount = m_MoveCountMax;

				m_bTraceFlag = false;

				// 2001.7.31 추가
				m_nUsedActionInfo = ACTIONINFO_NULL;
				m_nNextUsedActionInfo = ACTIONINFO_NULL;
			}

			m_BlockDestX = m_DestX;
			m_BlockDestY = m_DestY;

			m_DestX = SECTORPOSITION_NULL;
			m_DestY = SECTORPOSITION_NULL;			
		}
	}
	// 갈 수 없는 곳
	else 
	{
		//m_ActionCount = m_ActionCountMax;
		m_MoveCount = m_MoveCountMax;

		// 이번에 갈 수 없었던 곳 설정
		//m_BlockDestX = m_DestX;
		//m_BlockDestY = m_DestY;

		m_DestX = SECTORPOSITION_NULL;
		m_DestY = SECTORPOSITION_NULL;		

		#ifdef OUTPUT_DEBUG_PLAYER_ACTION
			DEBUG_ADD("no way!");
		#endif

		return false;
	}

	/*
	else
	// 이 부분에 약간의 오류가 있는 듯 함!! -_-;;;;
	//--------------------------------------------------------------
	// 아예 갈 수 없는 곳이면..
	//--------------------------------------------------------------
	// Player위치에서 목표지점까지 직선?으로 걸어가다가 
	// 장애물이 생기면 멈춘다.
	// (m_X, m_Y)  --->  (sX, sY)
	//--------------------------------------------------------------		
	{
		int	stepX = sX - m_X,
			stepY = sY - m_Y,
			x = m_X,
			y = m_Y,
			signX, signY;

		//-------------------------------------------------------
		// if not empty, delete all elements
		//-------------------------------------------------------
		if (!m_listDirection.empty())
		{
			m_listDirection.clear();
		}
			
		
		while (1)
		{
			// 부호에 따라서..
			signX = (stepX==0)? 0 : (stepX<0)? -1 : 1;
			signY = (stepY==0)? 0 : (stepY<0)? -1 : 1;

			x += signX;
			y += signY;
				
			// 갈 수 있는 곳이면 list에 넣는다.
			if (m_pZone->CanMove(x,y))
			{				
				m_listDirection.insert( m_listDirection.end(), DetermineDirection(stepX, stepY));
			}
			else 
			{
				// x,y 한칸 전의 위치를 목표지점으로 정한다.
				m_DestX = x - signX;
				m_DestY = y - signY;
				break;
			}	
			
			// 한 칸 움직였음을 표시
			stepX -= signX;
			stepY -= signY;
		}				
		
		return;
	}
	*/

	#ifdef OUTPUT_DEBUG_PLAYER_ACTION
		DEBUG_ADD("find way");
	#endif

	return true;
}

//----------------------------------------------------------------------
// Get Destination()
//----------------------------------------------------------------------
void
MFakeCreature::GetDestination(POINT &dest)
{
	dest.x = m_DestX;
	dest.y = m_DestY;
}

//----------------------------------------------------------------------
// priority queue인 m_pqDNodes를 지운다.
//----------------------------------------------------------------------
void
MFakeCreature::RemoveNodes()
{
	while (!m_pqDNodes.empty())
	{
		m_pqDNodes.pop();
	}


	// temp를 지운다.
	DNODE_LIST::iterator iNode = m_listDNodes.begin();

	while (iNode != m_listDNodes.end())
	{
		DNode* pNode = *iNode;
		delete pNode;
		iNode++;		
	}

	m_listDNodes.clear();
}

//----------------------------------------------------------------------
// (x,y)로 부터 m_Dest(X,Y)까지의 거리를 구한다.
//----------------------------------------------------------------------
// 가로,세로,대각선 모두 1이므로 가장 큰 값이 거리이고
// 목표와 같은 좌표가 있는게 더 좋으므로 아닌 경우 +1이 되도록 한다.
//----------------------------------------------------------------------
int
MFakeCreature::CalculateDistance(int x, int y)
{

	int xd = abs(x-m_DestX),
		yd = abs(y-m_DestY);	 

	int plus = !(m_DestX==x) + !(m_DestY==y);
		
	if (xd>yd)
	{
		return xd*xd + plus;
	}
	else return yd*yd + plus;

	return 0;
}

void
MFakeCreature::SetStop()
{
	// 길찾기 제거
	m_listDirection.clear(); 

	// 추적 중지	
	m_bTraceFlag = false;
	//m_fNextTrace	= FLAG_TRACE_NULL;

	// Action 중지
	m_sX=0; 
	m_sY=0;

	// 다음 동작도 없앰
	m_NextX = SECTORPOSITION_NULL;
	m_NextY = SECTORPOSITION_NULL;

	// 목표 제거
	m_DestX			= SECTORPOSITION_NULL; 
	m_DestY			= SECTORPOSITION_NULL; 
	m_NextDestX		= SECTORPOSITION_NULL; 
	m_NextDestY		= SECTORPOSITION_NULL; 

	m_bFastMove = false;
	m_bKnockBack = 0;	

	//------------------------------------------------
	// 기존의 MoveBuffer를 다 지움
	//------------------------------------------------
	AffectMoveBufferAll();
	//ReleaseMoveBuffer();

	// 2001.11.8 - 정지할때 좌표 보정 확실히..
	ActionMoveNextPosition();

	// Action 중지
	m_sX=0; 
	m_sY=0;

	int action = ACTION_STAND;
	m_Action		= action;
	m_ActionCount	= m_ActionCountMax; 
	m_MoveCount		= m_MoveCountMax;

	// 다음 동작도 없앰
	m_bNextAction = false;
	m_NextX = SECTORPOSITION_NULL;
	m_NextY = SECTORPOSITION_NULL;

	
	//m_nNextUsedActionInfo = ACTIONINFO_NULL;

}

//----------------------------------------------------------------------
// Move
//----------------------------------------------------------------------
//
// 현재의 방향(m_CurrentDirection)으로 한 Frame이동한다.
//
// 한 Sector를 이동하는게 아니고 Sector와 Sector사이를 이동하는 중간의
// 한 Frame을 이동하는 것이다. 
//
// Sector --> Sector에서 몇 Frame을 이동할까??가 문제인데...
//    = cX,cY에 한번에 이동하는 pixel단위를 대입하고
//    = sX,sY에 한번에 이동하는 pixel단위를 입력하면 된다.
//
// Move()에서는
//    cX,cY가 0이면 다시 cX,cY, sX,sY 값을 생성하고
//    0이 아니면 sX
//----------------------------------------------------------------------
void	
MFakeCreature::ActionMovePet()
{
	#ifdef OUTPUT_DEBUG_PLAYER_ACTION
		DEBUG_ADD("ActionMove");
	#endif

	int moveTimesTemp = 0;
	if (m_MoveDevice==MOVE_DEVICE_WALK)
	{
		moveTimesTemp = (*g_pCreatureTable)[m_CreatureType].MoveTimes;
	}
	else if (m_MoveDevice==MOVE_DEVICE_RIDE)
	{
		moveTimesTemp = (*g_pCreatureTable)[m_CreatureType].MoveTimesMotor;
	}
	else if (m_MoveDevice==MOVE_DEVICE_SUMMON_SYLPH || HasEffectStatusSummonSylph( dynamic_cast<MCreature*>(this) ))
	{
		moveTimesTemp = (*g_pCreatureTable)[m_CreatureType].MoveTimesMotor;
	}

	//----------------------------------------------------
	//
	// 다음 Sector에 도달한 경우	
	//
	//----------------------------------------------------
	// m_listDirection에서 방향을 하나 읽어와서 
	// check해보고 움직인다.
	//----------------------------------------------------
	if (m_MoveCount%moveTimesTemp==0)//m_sX==0 && m_sY==0)
	{
		TYPE_SECTORPOSITION	x, y;

		//-------------------------------------------------------------
		// fast move인 경우는 nextX, nextY에 좌표가 있다.
		//-------------------------------------------------------------
		if (m_bFastMove)
		{
			x = m_NextX;
			y = m_NextY;

			m_NextX = SECTORPOSITION_NULL;					

			//m_CurrentDirection = 
			m_Direction = GetDirectionToPosition( x, y );
		}
		//-------------------------------------------------------------
		// 일반적인 움직임
		//-------------------------------------------------------------
		else
		{
			//----------------------------------------------------
			// 목표위치가 없으면 움직이지 않는다.
			//----------------------------------------------------
			if (m_DestX==SECTORPOSITION_NULL && m_NextDestX==SECTORPOSITION_NULL) 
			{
				DEBUG_ADD("### Don't need to move any more - Move Stop! ###");
				
				m_MoveCount = m_MoveCountMax;
				return;
			}

				//sprintf(g_pDebugMessage->GetCurrent(), "ListSize = %d", m_listDirection.size());
			
			//----------------------------------------------------
			//
			//
			//                 추적 중이면..
			//
			//
			//----------------------------------------------------		
			//----------------------------------------------------	
			// Creature 추적 중이면..
			// Creature가 사라지거나
			// 추적 목표좌표가 바뀔 수 있으므로 check해준다.
			//----------------------------------------------------	
			if (m_bTraceFlag == true) 
			{
				// 추적하는 Creature의 정보를 읽어온다.
				MCreature*	pCreature = m_pZone->GetCreature( m_TraceID );				

				//GET_DYING_CREATURE( pCreature, m_TraceID );

				//-------------------------------------------------------
				// 추적하는 Creature가 사라졌을 경우 --> 추적 중지
				// 내가 Slayer인 경우는 Darkness안에 들어간 캐릭을 쫓아갈 수 없다.					
				//-------------------------------------------------------								
				if (pCreature==NULL )
				{
					// 추적 중지
					m_bTraceFlag = false;
						
					// 멈춘다.
					SetStop();					
					return;			
				}
			
				//-------------------------------------------------------			
				// 추적할려는 Creature의 좌표가 달라졌을 경우
				//-------------------------------------------------------			
				if (pCreature->GetX()!=m_TraceX ||
					pCreature->GetY()!=m_TraceY)
				{										
					m_NextDestX = pCreature->GetX();
					m_NextDestY = pCreature->GetY();
					// Z는 관계없다.
				}

				m_TraceX	= pCreature->GetX();
				m_TraceY	= pCreature->GetY();
				m_TraceZ	= pCreature->GetZ();
			}
			//----------------------------------------------------
			// 길찾기 Buffering했던걸 설정한다.
			//----------------------------------------------------
			if (m_NextDestX!=SECTORPOSITION_NULL)
			{
				// 길찾기
				//SetDestination(m_NextDestX, m_NextDestY);
				m_DestX		= m_NextDestX;	// TEST
				m_DestY		= m_NextDestY;	// TEST
				m_NextDestX = SECTORPOSITION_NULL;
				m_NextDestY = SECTORPOSITION_NULL;			
			}

			//----------------------------------------------------
			// 길찾기 : 한 Tile 이동시마다 길찾기를 한다.
			//----------------------------------------------------
			if (m_DestX!=SECTORPOSITION_NULL)
				SetDestination(m_DestX, m_DestY);

			//----------------------------------------------------
			// Server에서 검증받기 위해서 Send
			//----------------------------------------------------
			//SetAction( m_MoveAction );

			
			//----------------------------------------------------
			// 갈 길이 없는 경우
			//----------------------------------------------------
			if (m_listDirection.empty())
			{
				//--------------------------------------------
				// 목적지에 도달한 경우..
				//--------------------------------------------
				if (m_DestX==m_X && m_DestY==m_Y)
				{
					m_DestX = SECTORPOSITION_NULL;
					m_DestY = SECTORPOSITION_NULL;

					#ifdef OUTPUT_DEBUG_PLAYER_ACTION
						DEBUG_ADD("dest reached");
					#endif

					return;
				}

				#ifdef OUTPUT_DEBUG_PLAYER_ACTION
					DEBUG_ADD("no way");
				#endif

				return;
			}		

			
			//----------------------------------------------------	
			//
			// 방향대로 움직인다.
			//
			//----------------------------------------------------			
			//----------------------------------------------------
			// m_listDirection에서 방향을 하나 읽어온다.
			//----------------------------------------------------
			m_Direction = m_listDirection.front();
			m_listDirection.pop_front();	
				
			//----------------------------------------------------
			// 다음에 길찾기를 해야할 경우...
			//----------------------------------------------------
			if (m_NextDestX!=SECTORPOSITION_NULL)
			{
				//m_NextAction = m_MoveAction;
				SetNextAction( m_MoveAction );
			}

			//----------------------------------------------------
			// 읽어온 방향으로 한 Sector를 진행하고
			// cX,cY, sX,sY를 다시 지정해줘야 한다.
			//----------------------------------------------------

			//----------------------------------------------------
			// 이동 가능한지 check
			//----------------------------------------------------
			x = m_X;
			y = m_Y;

			switch (m_Direction)
			{
				case DIRECTION_LEFTDOWN		: x--;	y++;	break;
				case DIRECTION_RIGHTUP		: x++;	y--;	break;
				case DIRECTION_LEFTUP		: x--;	y--;	break;
				case DIRECTION_RIGHTDOWN	: x++;	y++;	break;
				case DIRECTION_LEFT			: x--;			break;
				case DIRECTION_DOWN			:		y++;	break;
				case DIRECTION_UP			:		y--;	break;
				case DIRECTION_RIGHT		: x++;			break;
			}
		}

		//--------------------------------------------------------------------
		// 움직일 수 있을 경우		
		//--------------------------------------------------------------------
		if (m_pZone->CanMove(m_MoveType, x,y)
			|| m_bFastMove		// Fast Move인 경우는 무조건 이동 가능..
			
			)
		{		
			//---------------------------------------------------------------
			// Fast Move인 경우..
			//---------------------------------------------------------------
			if (m_bFastMove)
			{
				// nothing
			}
			//---------------------------------------------------------------
			// 일반적인 움직임 --> 검증 필요
			//---------------------------------------------------------------
			else
			{
			}

			//-----------------------
			// 시야 바꿔주기
			//-----------------------
			//m_pZone->MoveLightSight(m_X, m_Y, m_LightSight, m_Direction);
			//m_pZone->UnSetLightSight(m_X, m_Y, m_LightSight);
			//m_pZone->SetLightSight(x, y, m_LightSight);

			// 이전 좌표 기억
			TYPE_SECTORPOSITION ox = m_X;
			TYPE_SECTORPOSITION oy = m_Y;

			// zone의 sector의 정보를 바꿔준다.
			MovePosition( x, y );
			
			//CheckInDarkness();

			//--------------------------------------------
			// 5 Frame이 있다고 할 경우
			//--------------------------------------------
			//
			// [1] UP,DOWN,LEFT,RIGHT일 때,
			//
			//     X변화 : 0 16 32 48 64   (+-16)
			//     Y변화 : 0  8 16 24 32   (+-8)
			// 
			//
			// [2] 대각선(LEFTUP,LEFTDOWN,RIGHTUP,RIGHTDOWN)으로 움직일때,
			//
			//     X변화 : 0  8 16 24 32   (+-8)
			//     Y변화 : 0  4  8 12 16   (+-4)
			//
			//--------------------------------------------
			// sX,sY : 움직여야할 전체 pixel(한 TILE)
			// cX,cY : 이동하는 단위 pixel
			//--------------------------------------------		
			//--------------------------------------------
			// Fast Move 인 경우
			//--------------------------------------------

			if (m_bFastMove)
			{
				// 직선 거리로 움직일때의 pixel거리
				// ex) 왼쪽 2 --> 1 : ( 2 - 1 ) * TILE_X
				//     위쪽 2 --> 1 : ( 2 - 1 ) * TILE_Y
				m_sX = (ox - m_X) * TILE_X;
				m_sY = (oy - m_Y) * TILE_Y;
				
				//------------------------------------------------
				// 실제 한 타일 이동 속도의 반의 속도에 목적지까지 이동한다.
				//------------------------------------------------
				// 한 타일 이동할때의 Frame 수
				int moveTimes_div_2 = (*g_pCreatureTable)[m_CreatureType].MoveTimes >> 1;
				
				m_cX = -m_sX / moveTimes_div_2;
				m_cY = -m_sY / moveTimes_div_2;

				m_MoveCount += 2;

				if (m_MoveCount > m_MoveCountMax)
				{
					m_MoveCount = m_MoveCountMax;
				}
			}
			//--------------------------------------------	
			// 정상 이동
			//--------------------------------------------
			else
			{	
				m_DirectionMoved = m_Direction;

				m_sX = m_sXTable[m_DirectionMoved];
				m_sY = m_sYTable[m_DirectionMoved];

				//????????????????????????????????????????????????????????
				//???                                                  ???
				//???   한 Frame을 이동...해야하는가?? 말아야 하는가   ???
				//???                                                  ???
				//????????????????????????????????????????????????????????
				int moveTimes_1;
				
				#ifdef OUTPUT_DEBUG
					if (m_CreatureType >= g_pCreatureTable->GetSize())
					{
						DEBUG_ADD_FORMAT("[Error] CreatureType is exceed MAX. id=%d, type=%d", m_ID, m_CreatureType);
						
						SetStop();

						return;
					}
				#endif

				if (m_MoveDevice==MOVE_DEVICE_WALK)
				{
					moveTimes_1 = (*g_pCreatureTable)[m_CreatureType].MoveTimes-1;
				}
				else if (m_MoveDevice==MOVE_DEVICE_RIDE)
				{
					moveTimes_1 = (*g_pCreatureTable)[m_CreatureType].MoveTimesMotor-1;
				}
				else if (m_MoveDevice==MOVE_DEVICE_SUMMON_SYLPH || HasEffectStatusSummonSylph( dynamic_cast<MCreature*>(this) ))
				{
					moveTimes_1 = (*g_pCreatureTable)[m_CreatureType].MoveTimesMotor-1;
				}
				else
				{					
					DEBUG_ADD_FORMAT("[Error] What is MoveType of this Player??? id=%d", m_ID);							
					
					SetStop();

					return;
				}

				m_MoveTableCount = 0;
				m_cX = m_cXTable[moveTimes_1][m_DirectionMoved][m_MoveTableCount%moveTimes_1];
				m_cY = m_cYTable[moveTimes_1][m_DirectionMoved][m_MoveTableCount%moveTimes_1];
			}

			m_sX += m_cX;
			m_sY += m_cY;

			if( m_MoveCount == 0 )
				m_NextMoveCount = 1;//(*g_pCreatureTable)[m_CreatureType].MoveRatio;
			else
				m_NextMoveCount += 1;//(*g_pCreatureTable)[m_CreatureType].MoveRatio;

			m_MoveCount++;

			// 다음에 이동할 count를 지정한다.

			//------------------------------------------------
			// 캐릭터의 MoveAction에 맞는 Sound를 출력해준다.
			//------------------------------------------------
			TYPE_SOUNDID soundID = (*g_pCreatureTable)[m_CreatureType].GetActionSound( m_MoveAction );

			if (soundID!=SOUNDID_NULL)
			{
				PlaySound( soundID,
							false,
							m_X,
							m_Y);
			}
		}	
		//--------------------------------------------
		// 움직일 수 없을 경우		
		//--------------------------------------------
		// 길찾기를 다시 해준다.
		else
		{
			m_NextDestX = m_DestX;
			m_NextDestY = m_DestY;
			m_listDirection.empty();


			DEBUG_ADD("다른 캐릭터에 의해서 Block됐음.");					
		}

		// 다음 Sector까지 움직인다.
		if (!m_listDirection.empty())
		{
			//m_NextAction = m_MoveAction;
			SetNextAction( m_MoveAction );		
		}
	}
	//----------------------------------------------------
	//
	// 아직 다음 Sector까지 덜 도달한 경우
	//
	//----------------------------------------------------
	else
	{
		// 한 Frame을 이동한다.
		/*
		switch (m_MoveDevice)
		{
			case MOVE_DEVICE_NULL :			
				m_sX += m_cXTableDeviceNull[m_Direction][m_ActionCount];
				m_sY += m_cYTableDeviceNull[m_Direction][m_ActionCount];
			break;

			case MOVE_DEVICE_MOTOR1 :						
				m_sX += m_cXTableDeviceMotor1[m_Direction][m_ActionCount];
				m_sY += m_cYTableDeviceMotor1[m_Direction][m_ActionCount];
			break;
		}
		*/

		#ifdef OUTPUT_DEBUG_PLAYER_ACTION
			DEBUG_ADD("going");
		#endif

		// 이동할 count가 됐을때만 이동한다.
		if (m_MoveCount>=m_NextMoveCount)
		{			
			//--------------------------------------------
			// Fast Move 인 경우
			//--------------------------------------------
			if (m_bFastMove)
			{
				// 2001.8.22 추가 - 냠냠
				if (m_sX!=0)	m_sX += m_cX;
				if (m_sY!=0)	m_sY += m_cY;		

				m_MoveCount += 2;

				if (m_MoveCount > m_MoveCountMax)
				{
					m_MoveCount = m_MoveCountMax;
				}

				// 다음에 이동할 count를 지정한다.
				m_NextMoveCount += 1;//(*g_pCreatureTable)[m_CreatureType].MoveRatio;

			}
			//--------------------------------------------
			// 일반적인 이동의 경우
			//--------------------------------------------
			else
			{
				int moveTimes_1;
				
				if (m_MoveDevice==MOVE_DEVICE_WALK)
				{
					moveTimes_1 = (*g_pCreatureTable)[m_CreatureType].MoveTimes-1;
				}
				else if (m_MoveDevice==MOVE_DEVICE_RIDE)
				{
					moveTimes_1 = (*g_pCreatureTable)[m_CreatureType].MoveTimesMotor-1;
				}
				else if (m_MoveDevice==MOVE_DEVICE_SUMMON_SYLPH || HasEffectStatusSummonSylph( dynamic_cast<MCreature*>(this)))
				{
					moveTimes_1 = (*g_pCreatureTable)[m_CreatureType].MoveTimesMotor-1;
				}
				else
				{
					DEBUG_ADD_FORMAT("[Error] What is MoveType of this Creature??? id=%d", m_ID);					
					SetStop();					
					return;
				}

				m_MoveTableCount++;
				m_cX = m_cXTable[moveTimes_1][m_DirectionMoved][m_MoveTableCount%moveTimes_1];
				m_cY = m_cYTable[moveTimes_1][m_DirectionMoved][m_MoveTableCount%moveTimes_1];

				// 2001.8.22 추가 - 냠냠
				if (m_sX!=0)	m_sX += m_cX;
				if (m_sY!=0)	m_sY += m_cY;		

				// 다음에 이동할 count를 지정한다.
				m_NextMoveCount += 1;//(*g_pCreatureTable)[m_CreatureType].MoveRatio;

				m_MoveCount++;
			}
		}	
		else
		{
			m_MoveCount++;
		}
		
		//m_ActionCount++;
		// frame을 바꿔준다. 마지막 Frame까지 갔으면 0번째 Frame으로 바꾼다.
		//if (++m_ActionCount == (*m_pFrames)[m_Action][m_Direction].GetCount())
		//	m_ActionCount = 0;

		//if (m_ActionCount == (m_ActionCountMax>>1))
		//if (m_MoveCount == (m_MoveCountMax>>1))
		//{				
		//	SetLightSight( m_LightSight );
		//}
	}

	#ifdef OUTPUT_DEBUG_PLAYER_ACTION
		DEBUG_ADD("moveok");
	#endif
}


void MFakeCreature::ActionMove()
{
	if(GetOwnerID() != OBJECTID_NULL)
		ActionMovePet();
	else
		MCreature::ActionMove();
}

void	
MFakeCreature::SetZone(MZone* pZone)
{
	// base class functions
	MCreature::SetZone(pZone);
}

void
MFakeCreature::SetOwnerID(TYPE_OBJECTID id)
{
	m_OwnerID = id;
}

POINT
MFakeCreature::CaculatePetPosition(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, int distance, int distanceMin, MOVE_TYPE moveType, TYPE_SECTORPOSITION petX, TYPE_SECTORPOSITION petY)
{
	POINT p = {sX, sY};

	if(g_pZone == NULL)
	{
		return p;
	}

	const int x0 = sX-distance;
	const int x1 = sX+distance;
	const int y0 = sY-distance;
	const int y1 = sY+distance;

	std::vector<POINT> vMovePoint;

	for (int i=y0; i<y1; i++)
	{			
		for (int j=x0; j<x1; j++)
		{
			if(g_pZone->CanMove(moveType, j, i) && j != sX && i != sY && max(abs(i-petY), abs(j-petX)) >= distanceMin)
			{
				POINT tempPoint = {j, i};
				vMovePoint.push_back(tempPoint);
			}
		}
	}

	if(!vMovePoint.empty())
	{
		p = vMovePoint[rand()%vMovePoint.size()];
	}

	return p;
}

void
MFakeCreature::SyncTurretDirection()
{
	m_TurretDirect = ((GetDirection()+6)%8)*4;
	m_FinalTurretDirect = 0xFF;
}

const BYTE
MFakeCreature::GetTurretDirection() const
{
	return m_TurretDirect;
}

void
MFakeCreature::SetTurretFinalDirection(BYTE d)
{
	m_FinalTurretDirect = d;
}

void
MFakeCreature::UpdateTurret()
{
	if( (GetCreatureType() == 702 || GetCreatureType() == 703 || GetCreatureType() == 704) )
	{
		if( GetTurretDelay() > 0 )
			m_TurretDelay--;
		
		bool bSetFinalDirect = false;

		TYPE_OBJECTID ownerID = GetOwnerID();

		BYTE finalDirect = 0;
		
		MCreature *pOwnerCreature = g_pZone->GetCreature( ownerID );

		if( pOwnerCreature != NULL )
		{
			TYPE_OBJECTID targetID = pOwnerCreature->GetStatus( MODIFY_LAST_TARGET );

			MCreature *pTargetCreature = g_pZone->GetCreature( targetID );

			if( pTargetCreature != NULL )
			{
				BYTE direct = GetDirectionToPosition( pTargetCreature->GetX(), pTargetCreature->GetY() );

				bSetFinalDirect = true;
				finalDirect = ((direct+6)%8)*4;
			}
		}
		
		if( bSetFinalDirect == false )
		{
			finalDirect = ((GetDirection()+6)%8)*4;
			if( m_FinalTurretDirect != 0xFF )
				finalDirect = m_FinalTurretDirect;
		}
			
		if( finalDirect != m_TurretDirect )
		{
			if( ((finalDirect - m_TurretDirect +32)%32) < 16 )
			{
				m_TurretDirect = (m_TurretDirect + 1)%32;
			}
			else
			{
				m_TurretDirect = (m_TurretDirect - 1 +32)%32;
			}
		}
	}
}
//2004, 03, 22 sobeit add start
void 
MFakeCreature::KeepWanderCreature()
{
	// 유령이 아니믄 리턴
	if(GetCreatureType() != CREATURETYPE_GHOST)
		return;
	POINT	firstSector, endSector,NextMove;
	firstSector.x = g_pPlayer->GetX() + g_SECTOR_SKIP_PLAYER_LEFT-1;// +4 테스트
	firstSector.y = g_pPlayer->GetY() + g_SECTOR_SKIP_PLAYER_UP-1;
	endSector.x	  = firstSector.x+g_SECTOR_WIDTH+2;
	endSector.y   = firstSector.y+g_SECTOR_HEIGHT+2;
	// 화면 안에 있는 유령 이믄..
	if(IsInSector(firstSector.x, firstSector.y, endSector.x, endSector.y))
	{
		NextMove.x = GetX(); NextMove.y = GetY();
		switch (m_Direction)
		{
			case DIRECTION_LEFTDOWN		: NextMove.x--;	NextMove.y++;	break;
			case DIRECTION_RIGHTUP		: NextMove.x++;	NextMove.y--;	break;
			case DIRECTION_LEFTUP		: NextMove.x--;	NextMove.y--;	break;
			case DIRECTION_RIGHTDOWN	: NextMove.x++;	NextMove.y++;	break;
			case DIRECTION_LEFT			: NextMove.x--;				break;
			case DIRECTION_DOWN			:			NextMove.y++;	break;
			case DIRECTION_UP			:			NextMove.y--;	break;
			case DIRECTION_RIGHT		: NextMove.x++;				break;
		}

		if(	!m_pZone->CanMove(m_MoveType, NextMove.x,NextMove.y))
			m_Direction = (m_Direction + 3 + rand()%3)%8;
		
		if( m_Action != ACTION_MOVE )
		{
			if(0xff == m_DirectionMove)
				m_Direction = rand()%8;
			SetAction(ACTION_MOVE);
		}
		m_DirectionMove = m_Direction;
	}
	else
	{
		if(ACTION_MOVE == g_pPlayer->GetAction())
		{
			//SetStop();
			SetAction(ACTION_STAND);
			m_DirectionMove = 0xff;
		}
		else
		{
			if(abs(GetX()-g_pPlayer->GetX())>10 ||abs(GetY()-g_pPlayer->GetY())>15 )
			{
				//SetStop();
				SetAction(ACTION_STAND);
				return;
			}
			if(IsInSector(firstSector.x-1, firstSector.y, firstSector.x-1, endSector.y))
			{// 왼쪽 벽
				m_Direction = 3 + rand()%3;
				m_DirectionMove = m_Direction;
				SetAction(ACTION_MOVE);
			}
			else if(IsInSector(endSector.x+1, firstSector.y, endSector.x+1, endSector.y))
			{// 오른쪽 벽
				m_Direction = (7 + rand()%3)%8;
				m_DirectionMove = m_Direction;
				SetAction(ACTION_MOVE);
			}
			else if(IsInSector(firstSector.x, firstSector.y-1, endSector.x, firstSector.y-1))
			{// 위쪽 벽
				m_Direction = 1 + rand()%3;
				m_DirectionMove = m_Direction;
				SetAction(ACTION_MOVE);
			}
			else if(IsInSector(firstSector.x, endSector.y+1, endSector.x, endSector.y+1))
			{// 아래쪽 벽
				m_Direction = 5 + rand()%3;
				m_DirectionMove = m_Direction;
				SetAction(ACTION_MOVE);
			}
			else 
				//SetStop();
				SetAction(ACTION_STAND);
			
		}
	}


}
bool
MFakeCreature::IsInSector(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, TYPE_SECTORPOSITION eX, TYPE_SECTORPOSITION eY)
{
	if (GetX() >= sX && GetX() <= eX
		&& GetY() >= sY && GetY() <= eY)
	{
		return true;
	}
	return false;
}
//2004, 03, 22 sobeit add end

// 2004, 11, 8, sobeit add start
void 
MFakeCreature::KeepRocketLuncher()
{
	if(GetCreatureType() != CREATURETYPE_ROCKET_LUNCHER)
		return;

	if(IsGhost(0xff))
		return;
	MCreature *pCreature = g_pZone->GetCreature(m_TraceID);	
	if(pCreature != NULL)
	{
		int distance = max(abs(pCreature->GetX()-m_X), abs(pCreature->GetY()-m_Y));
//		// 처음 접근할때까지만 추적하고 ( 걍있는 변수 m_PatrolCount 써야지..ㅎㅎ)
		if(distance > 1)
		{
			m_DirectionMove = m_Direction = m_CurrentDirection = GetDirectionToPosition(pCreature->GetX(), pCreature->GetY());
			m_DestX = pCreature->GetX();
			m_DestY = pCreature->GetY();
			m_PatrolCount = m_CurrentDirection;
		}
//		// 타겟주위를 맴돈다..? -_-;
		else
		{
			if(m_DestX == GetX() && m_DestY == GetY())
			{
				m_PatrolCount ++;
				m_DestX = pCreature->GetX(); m_DestY = pCreature->GetY();
				switch (m_PatrolCount % 8)
				{
					case DIRECTION_LEFTDOWN		: m_DestX--;	m_DestY++;	break;
					case DIRECTION_RIGHTUP		: m_DestX++;	m_DestY--;	break;
					case DIRECTION_LEFTUP		: m_DestX--;	m_DestY--;	break;
					case DIRECTION_RIGHTDOWN	: m_DestX++;	m_DestY++;	break;
					case DIRECTION_LEFT			: m_DestX--;				break;
					case DIRECTION_DOWN			:			m_DestY++;	break;
					case DIRECTION_UP			:			m_DestY--;	break;
					case DIRECTION_RIGHT		: m_DestX++;				break;
				}
			}
			m_DirectionMove = m_Direction = m_CurrentDirection = GetDirectionToPosition(m_DestX, m_DestY);
		}
	}

	if( m_Action != ACTION_MOVE )
		SetAction(m_MoveAction);

			

}
// 2004, 11, 8, sobeit add end

void MFakeCreature::SetFakeMoveOnce(BYTE Dir)
{
	m_DirectionMove = m_Direction = m_CurrentDirection = Dir%8;
	SetAction(m_MoveAction);
}

void
MFakeCreature::KeepWildWolfAttack()
{
	if(g_pZone == NULL)
		return;
	
	if(m_PatrolCount++ < 4 ) return;
	if(m_IsEatCorps)
	{
		if((GetX() == m_DestX && GetY() ==m_DestY) || m_WildWolf_Mode == 0 ) 
		{
			switch(m_WildWolf_Mode)
			{
				case WILD_WOLF_MODE_ATTACK_1st:
				case WILD_WOLF_MODE_ATTACK_2nd:
				case WILD_WOLF_MODE_ATTACK_3rd:
				{
					int TempDir = MTopView::GetDirectionToPosition(GetX(), GetY(), m_DestX, m_DestY);
					POINT TargetPoint = GetNextTileByDirection(m_DestX, m_DestY, TempDir);
					m_bTraceFlag = true;
					SetCurrentDirection(TempDir);
					SetFakeCreatureFastMoveAction(TargetPoint.x, TargetPoint.y, ACTIONINFO_NULL, NULL);	
					//SetNextDestination(TargetPoint.x, TargetPoint.y);
					m_DestX = TargetPoint.x;
					m_DestY = TargetPoint.y;
					m_WildWolf_Mode ++;
					m_DirectionMove = TempDir;
					if( m_Action != ACTION_MOVE )
						SetAction(ACTION_MOVE);
					m_PatrolCount = 0;
				}
				break;
				case WILD_WOLF_MODE_MOVE_1st:
				case WILD_WOLF_MODE_MOVE_2st:
				{
					int TempDir = (GetDirection()+4)%8;
					SetDirection(TempDir);
					SetAction(ACTION_VAMPIRE_DRAIN);
					m_PatrolCount = 0;
					m_WildWolf_Mode = WILD_WOLF_MODE_REMOVE;
				}
				break;
			}
			
		}
	}
	else
	{
		MCreature *pCreature = g_pZone->GetCreature(m_TraceID);
		if(NULL != pCreature)
		{
			if((GetX() == m_DestX && GetY() ==m_DestY) || m_WildWolf_Mode == 0 )
			{
				switch(m_WildWolf_Mode)
				{
				case WILD_WOLF_MODE_ATTACK_1st:
				case WILD_WOLF_MODE_ATTACK_2nd:
				case WILD_WOLF_MODE_ATTACK_3rd:
					{
						int TempDir = MTopView::GetDirectionToPosition(GetX(), GetY(), pCreature->GetX(), pCreature->GetY());
						POINT TargetPoint = GetNextTileByDirection(pCreature->GetX(), pCreature->GetY(), TempDir);
						m_bTraceFlag = true;
						SetCurrentDirection(TempDir);
						SetFakeCreatureFastMoveAction(TargetPoint.x, TargetPoint.y, ACTIONINFO_NULL, pCreature->GetID());	
						//SetNextDestination(TargetPoint.x, TargetPoint.y);
						m_WildWolf_Mode ++;
						m_DestX = TargetPoint.x;
						m_DestY = TargetPoint.y;
						m_DirectionMove = TempDir;
						if( m_Action != ACTION_MOVE )
							SetAction(ACTION_MOVE);
						m_PatrolCount = 0;
					}
					break;
				case WILD_WOLF_MODE_MOVE_1st:
				case WILD_WOLF_MODE_MOVE_2st:
					{
						int TempDir = MTopView::GetDirectionToPosition(GetX(), GetY(), pCreature->GetX(), pCreature->GetY());
						TempDir = (TempDir +7)%8;
						POINT TargetPoint = GetNextTileByDirection(GetX(), GetY(), TempDir );
						m_bTraceFlag = true;
						SetCurrentDirection(TempDir);
						SetFakeCreatureFastMoveAction(TargetPoint.x, TargetPoint.y, ACTIONINFO_NULL, pCreature->GetID());	
						//SetNextDestination(TargetPoint.x, TargetPoint.y);
						m_WildWolf_Mode ++;
						m_DestX = TargetPoint.x;
						m_DestY = TargetPoint.y;
						m_DirectionMove = TempDir;
						if( m_Action != ACTION_MOVE )
							SetAction(ACTION_MOVE);
						m_PatrolCount = 0;
					}
					break;
				}
				
			}
		}
		else
		{
			m_WildWolf_Mode = WILD_WOLF_MODE_REMOVE;
		}
		
	}
}

