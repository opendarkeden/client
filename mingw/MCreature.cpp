//----------------------------------------------------------------------
// MCreature.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MZone.h"
#include "MObject.h"
#include "MCreature.h"
#include "ClientDef.h"
#include "MTopView.h"
#include "MActionInfoTable.h"
#include "MEffectGeneratorTable.h"
#include "MCreatureTable.h"
#include "MSoundTable.h"
#include "MEffectStatusTable.h"
#include "FL2.h"
#include "ClientConfig.h"
#include "ServerInfo.h"
#include "DebugInfo.h"
#include "SkillDef.h"
#include "MLevelNameTable.h"
#include "EffectSpriteTypeDef.h"
#include "SoundDef.h"
#include "MParty.h"
#include "PacketFunction.h"
#include "MonsterNameTable.h"
#include "MEffectSpriteTypeTable.h"
#include "MAttachOrbitEffect.h"
#include "UserInformation.h"
//#include "RequestClientPlayerManager.h"
//#include "packet\Rpackets\CRRequest.h"
#include "VS_UI_base.h"
#include "MSkillManager.h"	// [새기술3]
#include "UIFunction.h"
#include "MEventManager.h"
#include "PacketFunction2.h"
#include "RankBonusTable.h"
#include "RankBonusDef.h"
#include "MTestDef.h"
#include "MTimeItemManager.h"
#include "MinTr.h"
#include "MFakeCreature.h"
#include "SystemAvailabilities.h"
#include "MGameStringTable.h"
#include "vs_ui.h"
#include "Client.h"

#define ATTACHEFFECTCOLOR_NULL	0xFFFF

extern DWORD	g_CurrentTime;
extern DWORD	g_CurrentFrame;

extern int					g_nZoneLarge;
extern int					g_nZoneSmall;
extern bool					g_bZonePlayerInLarge;
extern WORD					g_ZoneCreatureColorSet;

#ifdef OUTPUT_DEBUG	
	bool g_bCheckError = false;	// 테스트를 위해서 임시로..
#endif

//#define	new			DEBUG_NEW
#define				MAX_DARKNESS_COUNT		13

#ifdef __METROTECH_TEST__
	extern bool g_bLight;
#endif

//----------------------------------------------------------------------
// Get ArmageddonSprite
//----------------------------------------------------------------------
// 이거도 member function으로 넣어야 되는데.. 컴파일 시간때문에 일단 -_-;
//----------------------------------------------------------------------
//TYPE_EFFECTSPRITETYPE
//GetArmageddonSprite(MCreature* pCreature)
//{
//	DWORD aHP = pCreature->GetArmageddon();
//
//	if (aHP > 300)
//	{
//		return EFFECTSPRITETYPE_ARMAGEDDON_BACK_0_2;
//	}
//	else if (aHP > 200)
//	{
//		return EFFECTSPRITETYPE_ARMAGEDDON_BACK_1_2;
//	}
//	else if (aHP > 100)
//	{
//		return EFFECTSPRITETYPE_ARMAGEDDON_BACK_2_2;
//	}
//	
//	return EFFECTSPRITETYPE_ARMAGEDDON_BACK_3_2;	
//}
//
//int
//IsChangedArmageddonSprite(DWORD previousHP, DWORD currentHP)
//{
//	if (previousHP > 300)
//	{
//		if (currentHP <= 300)
//		{
//			return 1;
//		}
//
//		return 0;
//	}
//	else if (previousHP > 200)
//	{
//		if (currentHP <= 200)
//		{
//			return 2;
//		}
//
//		return 0;
//	}
//	else if (previousHP > 100)
//	{
//		if (currentHP <= 100)
//		{
//			return 3;
//		}
//
//		return 0;
//	}
//	
//	if (currentHP == 0)
//	{
//		return 4;
//	}
//	
//	return 0;
//}

//----------------------------------------------------------------------
// define
//----------------------------------------------------------------------
// 걍 함수로 만들어야 되는데.. 헤더바꾸고 컴파일하기 싫어서 임시로 - -;
//DEBUG_ADD_FORMAT("[ Apply Buffering Move ] [ID=%d] Current(%d, %d) Dir(%d) --> Next(%d, %d) Dir(%d)",	\
//															m_ID,												\
//															m_X, m_Y, m_Direction,								\
//															m_NextX,m_NextY, m_NextDirection);					\
		
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

#define SetServerPositionWithBlock(sX, sY)							\
		{															\
			CLASS_TYPE classType = GetClassType();					\
																	\
			if (classType!=CLASS_PLAYER && classType!=CLASS_FAKE	\
				&& (m_ServerX!=sX || m_ServerY!=sY))				\
			{														\
				m_pZone->UnSetServerBlock(m_MoveType, m_ServerX, m_ServerY);	\
																	\
				m_ServerX = sX;										\
				m_ServerY = sY;										\
																	\
				if (m_bAlive)										\
				{													\
					m_pZone->SetServerBlock(m_MoveType, sX, sY);	\
				}													\
			}														\
		}

//----------------------------------------------------------------------
// 한번에 이동하는 pixel에 대한 정보 
// ACTIONCOUNT_MOVE만큼 sXTable의 방향에 더해주면 sXTable값이 0이 된다.
// 8방향에 방향마다 Move수만큼..
//----------------------------------------------------------------------
/*
int MCreature::m_cXTableFrame8[8][8] = 
{
	{ -6, -6, -6, -6, -6, -6, -6, -6 },		// LEFT
	{ -6, -6, -6, -6, -6, -6, -6, -6 },		// LEFTDOWN
	{  0,  0,  0,  0,  0,  0,  0,  0 },		// DOWN
	{  6,  6,  6,  6,  6,  6,  6,  6 },		// RIGHTDOWN
	{  6,  6,  6,  6,  6,  6,  6,  6 },		// RIGHT
	{  6,  6,  6,  6,  6,  6,  6,  6 },		// RIGHTUP
	{  0,  0,  0,  0,  0,  0,  0,  0 },		// UP
	{ -6, -6, -6, -6, -6, -6, -6, -6 }		// LEFTUP
};

int MCreature::m_cXTableFrame8[8][8] = 
{
	{  0,  0,  0,  0,  0,  0,  0,  0 },		// LEFT
	{  3,  3,  3,  3,  3,  3,  3,  3 },		// LEFTDOWN
	{  3,  3,  3,  3,  3,  3,  3,  3 },		// DOWN
	{  3,  3,  3,  3,  3,  3,  3,  3 },		// RIGHTDOWN
	{  0,  0,  0,  0,  0,  0,  0,  0 },		// RIGHT
	{ -3, -3, -3, -3, -3, -3, -3, -3 },		// RIGHTUP
	{ -3, -3, -3, -3, -3, -3, -3, -3 },		// UP
	{ -3, -3, -3, -3, -3, -3, -3, -3 }		// LEFTUP
};

int MCreature::m_cXTableFrame4[8][4] = 
{
	{ -12, -12, -12, -12 },		// LEFT
	{ -12, -12, -12, -12 },		// LEFTDOWN
	{   0,   0,   0,   0 },		// DOWN
	{  12,  12,  12,  12 },		// RIGHTDOWN
	{  12,  12,  12,  12 },		// RIGHT
	{  12,  12,  12,  12 },		// RIGHTUP
	{   0,   0,   0,   0 },		// UP
	{ -12, -12, -12, -12 }		// LEFTUP
};

int MCreature::m_cXTableFrame4[8][4] = 
{
	{  0,  0,  0,  0 },		// LEFT
	{  6,  6,  6,  6 },		// LEFTDOWN
	{  6,  6,  6,  6 },		// DOWN
	{  6,  6,  6,  6 },		// RIGHTDOWN
	{  0,  0,  0,  0 },		// RIGHT
	{ -6, -6, -6, -6 },		// RIGHTUP
	{ -6, -6, -6, -6 },		// UP
	{ -6, -6, -6, -6 }		// LEFTUP
};
*/

//------------------------------------------------------------------
// Slayer action 속도
//------------------------------------------------------------------
int MCreature::s_SlayerActionSpeed[ACTION_MAX_SLAYER][3] =
{
	{ ACTION_STAND, ACTION_STAND, ACTION_STAND },
	{ ACTION_MOVE, ACTION_MOVE, ACTION_MOVE },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_MAGIC, ACTION_MAGIC, ACTION_MAGIC },
	{ ACTION_DAMAGED, ACTION_DAMAGED, ACTION_DAMAGED },
	{ ACTION_DRAINED, ACTION_DRAINED, ACTION_DRAINED },
	{ ACTION_DIE, ACTION_DIE, ACTION_DIE },

	{ ACTION_SLAYER_GUN_SR_SLOW, ACTION_SLAYER_GUN_SR, ACTION_SLAYER_GUN_SR_FAST },
	{ ACTION_SLAYER_GUN_AR_SLOW, ACTION_SLAYER_GUN_AR, ACTION_SLAYER_GUN_AR_FAST },
	{ ACTION_SLAYER_SWORD_SLOW, ACTION_SLAYER_SWORD, ACTION_SLAYER_SWORD_FAST },
	{ ACTION_SLAYER_BLADE_SLOW, ACTION_SLAYER_BLADE, ACTION_SLAYER_BLADE_FAST },
	{ ACTION_SLAYER_THROW_WEAPON, ACTION_SLAYER_THROW_WEAPON, ACTION_SLAYER_THROW_WEAPON },
	{ ACTION_SLAYER_MOTOR_MOVE, ACTION_SLAYER_MOTOR_MOVE, ACTION_SLAYER_MOTOR_MOVE },
	{ ACTION_SLAYER_GUN_SG_SLOW, ACTION_SLAYER_GUN_SG, ACTION_SLAYER_GUN_SG_FAST },
	{ ACTION_SLAYER_GUN_SMG_SLOW, ACTION_SLAYER_GUN_SMG, ACTION_SLAYER_GUN_SMG_FAST },
	{ ACTION_SLAYER_SWORD_2_SLOW, ACTION_SLAYER_SWORD_2, ACTION_SLAYER_SWORD_2_FAST },
	{ ACTION_SLAYER_BLADE_2_SLOW, ACTION_SLAYER_BLADE_2, ACTION_SLAYER_BLADE_2_FAST },
	{ ACTION_SLAYER_THROW_POTION, ACTION_SLAYER_THROW_POTION, ACTION_SLAYER_THROW_POTION },
	{ ACTION_SLAYER_MOTOR_STAND, ACTION_SLAYER_MOTOR_STAND, ACTION_SLAYER_MOTOR_STAND },
};

//------------------------------------------------------------------
// vampire action 속도
//------------------------------------------------------------------
int MCreature::s_VampireActionSpeed[ACTION_MAX_SLAYER][3] = //ACTION_MAX_VAMPIRE][3] =
{
	{ ACTION_STAND, ACTION_STAND, ACTION_STAND },
	{ ACTION_MOVE, ACTION_MOVE, ACTION_MOVE },
	{ ACTION_VAMPIRE_ATTACK_SLOW, ACTION_ATTACK, ACTION_VAMPIRE_ATTACK_FAST },
	{ ACTION_MAGIC, ACTION_MAGIC, ACTION_MAGIC },
	{ ACTION_DAMAGED, ACTION_DAMAGED, ACTION_DAMAGED },
	{ ACTION_DRAINED, ACTION_DRAINED, ACTION_DRAINED },
	{ ACTION_DIE, ACTION_DIE, ACTION_DIE },

	{ ACTION_VAMPIRE_DRAIN, ACTION_VAMPIRE_DRAIN, ACTION_VAMPIRE_DRAIN },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },	
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK }
};

//------------------------------------------------------------------
// ousters action 속도
//------------------------------------------------------------------
int MCreature::s_OustersActionSpeed[ACTION_MAX_SLAYER][3] = //ACTION_MAX_OUSTERS][3] =
{
	{ ACTION_STAND, ACTION_STAND, ACTION_STAND },
	{ ACTION_MOVE, ACTION_MOVE, ACTION_MOVE },
	{ ACTION_OUSTERS_ATTACK_SLOW, ACTION_ATTACK, ACTION_OUSTERS_ATTACK_FAST },
	{ ACTION_MAGIC, ACTION_MAGIC, ACTION_MAGIC },
	{ ACTION_DAMAGED, ACTION_DAMAGED, ACTION_DAMAGED },
	{ ACTION_DRAINED, ACTION_DRAINED, ACTION_DRAINED },
	{ ACTION_DIE, ACTION_DIE, ACTION_DIE },
	
	{ ACTION_OUSTERS_STAND, ACTION_OUSTERS_STAND, ACTION_OUSTERS_STAND },
	{ ACTION_OUSTERS_MOVE, ACTION_OUSTERS_MOVE, ACTION_OUSTERS_MOVE },	
	{ ACTION_OUSTERS_CHAKRAM_SLOW, ACTION_OUSTERS_CHAKRAM, ACTION_OUSTERS_CHAKRAM_FAST },
	{ ACTION_OUSTERS_MAGIC_ATTACK, ACTION_OUSTERS_MAGIC_ATTACK, ACTION_OUSTERS_MAGIC_ATTACK },
	{ ACTION_OUSTERS_DRAIN, ACTION_OUSTERS_DRAIN, ACTION_OUSTERS_DRAIN },
	{ ACTION_OUSTERS_FAST_MOVE_STAND, ACTION_OUSTERS_FAST_MOVE_STAND, ACTION_OUSTERS_FAST_MOVE_STAND },
	{ ACTION_OUSTERS_FAST_MOVE_MOVE, ACTION_OUSTERS_FAST_MOVE_MOVE, ACTION_OUSTERS_FAST_MOVE_MOVE },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK },
	{ ACTION_ATTACK, ACTION_ATTACK, ACTION_ATTACK }
};

//----------------------------------------------------------------------
// 한 Tile 이동의 Pixel수
//----------------------------------------------------------------------
int MCreature::m_sXTable[MAX_DIRECTION] = 
{
	TILE_X, TILE_X, 0, -TILE_X, -TILE_X, -TILE_X, 0, TILE_X
};

int MCreature::m_sYTable[MAX_DIRECTION] = 
{
	0, -TILE_Y, -TILE_Y, -TILE_Y, 0, TILE_Y, TILE_Y, TILE_Y
};

int *MCreature::m_cXTable[MAX_FRAME_MOVE][MAX_DIRECTION] = { NULL, };
int *MCreature::m_cYTable[MAX_FRAME_MOVE][MAX_DIRECTION] = { NULL, };

//----------------------------------------------------------------------
// 방향에 따른 변화값
//----------------------------------------------------------------------
POINT g_DirectionValue[MAX_DIRECTION] =
{
	{ -1, 0 },
	{ -1, 1 },
	{ 0, 1 },
	{ 1, 1 },
	{ 1, 0 },
	{ 1, -1 },
	{ 0, -1 },
	{ -1, -1 }
};

bool			IsCreatureMove( MCreature *pCreature )
{
	if( pCreature == NULL )
		return false;

	BYTE Action = pCreature->GetAction();
	if( Action == ACTION_MOVE )
		return true;

	if( pCreature->IsSlayer() )
	{		
		if( Action == ACTION_SLAYER_MOTOR_MOVE )
			return true;
	} else
	if( pCreature->IsVampire() )
	{				
	} else
	if( pCreature->IsOusters() )
	{
		if( Action == ACTION_OUSTERS_MOVE || Action == ACTION_OUSTERS_FAST_MOVE_MOVE )
			return true;
	}

	return false;
}

int ConvAdvancementSlayerActionFromSlayerAction( int CurAction, MCreatureWear* pCreatureWear );
int ConvAdvancementOustersActionFromOusterAction( int CurAction, bool bChakram );
int GetAdvancementVampireActionFromVampireAction( int CurAction, const MCreature* pCreature );

BYTE GetCreatureActionCountMax( const MCreature* pCreature, int action )
{
	bool bBat = pCreature->GetCreatureType() == CREATURETYPE_BAT;
	bool bWolf = pCreature->GetCreatureType() == CREATURETYPE_WOLF;
	bool bWerWolf = pCreature->GetCreatureType() == CREATURETYPE_WER_WOLF;
	// 2005, 1, 7, sobeit add start
	bool bInstallTurret = pCreature->HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET);
	// 2005, 1, 7, sobeit add end
	if( !pCreature->IsAdvancementClass() || g_pTopView == NULL || bBat || bWolf || bWerWolf || bInstallTurret)
	{
		return (*g_pCreatureTable)[pCreature->GetCreatureType()].GetActionCount( action );
	}

	BYTE* refActionMaxTable = NULL;
	switch( pCreature->GetRace() )
	{
	case RACE_SLAYER :
		action = ConvAdvancementSlayerActionFromSlayerAction( action, (MCreatureWear*)pCreature );
		refActionMaxTable = g_AdvanceSlayerActionMaxCount;
		break;
		
	case RACE_VAMPIRE :
		action = GetAdvancementVampireActionFromVampireAction( action, pCreature );
		refActionMaxTable = g_AdvanceVampireActionMaxCount;
		break;
		
	case RACE_OUSTERS :
		{
			MCreatureWear* pCreatureWear = (MCreatureWear*)pCreature;
			const MCreatureWear::ADDON_INFO& addonInfoChakram = pCreatureWear->GetAddonInfo(ADDON_RIGHTHAND);
			bool bChakram = addonInfoChakram.bAddon && addonInfoChakram.ItemClass == ITEM_CLASS_OUSTERS_CHAKRAM;
			
			action = ConvAdvancementOustersActionFromOusterAction( action, bChakram );
			refActionMaxTable = g_AdvanceOustersActionMaxCount;
		}
		break;
	}
	
	action -= ADVANCEMENT_ACTION_START;

	if( refActionMaxTable != NULL && action >= 0 )
		return refActionMaxTable[ action ];
	
	return 0;
}

bool			IsCreatureActionAttack( MCreature *pCreature )
{
	if( pCreature == NULL )
		return false;

	BYTE Action = pCreature->GetAction();
	
	if( Action == ACTION_ATTACK || Action == ACTION_MAGIC )
		return true;

	if( pCreature->IsSlayer() )
	{
		if ( Action == ACTION_SLAYER_GUN_SR || Action == ACTION_SLAYER_GUN_AR ||
			Action == ACTION_SLAYER_SWORD || Action == ACTION_SLAYER_BLADE ||
			Action == ACTION_SLAYER_THROW_WEAPON || Action == ACTION_SLAYER_GUN_SG ||
			Action == ACTION_SLAYER_GUN_SMG || Action == ACTION_SLAYER_SWORD_2 ||
			Action == ACTION_SLAYER_BLADE_2	|| Action == ACTION_SLAYER_THROW_POTION	||
			(Action >= ACTION_SLAYER_GUN_SR_SLOW && Action <= ACTION_SLAYER_BLADE_2_FAST )
			)
			return true;
	} 
	else if( pCreature->IsOusters() )
	{
		if ( Action == ACTION_OUSTERS_CHAKRAM || Action == ACTION_OUSTERS_MAGIC_ATTACK ||
			Action == ACTION_OUSTERS_ATTACK_SLOW || Action == ACTION_OUSTERS_ATTACK_FAST ||
			Action == ACTION_OUSTERS_CHAKRAM_SLOW || Action == ACTION_OUSTERS_CHAKRAM_FAST )
			return true;
	}
	else if (pCreature->IsVampire() )
	{		
		if( Action == ACTION_VAMPIRE_ATTACK_FAST || Action == ACTION_VAMPIRE_ATTACK_SLOW)
			return true;
	}

	return false;
}

bool			HasEffectStatusSummonSylph( MCreature* pCreature )
{
	if( 
		pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH ) ||
		pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_GREEN ) ||
		pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_ORANGE ) ||
		pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_RED ) ||
		pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_YELLOW ) ||
		pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_WHITE ) ||
		pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_BROWN ) ||
		pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_LILAC ) ||
		pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_BLACK ) 
		)
		return true;

	return false;
}

void			RemoveEffectStatusSummonSylph( MCreature* pCreature )
{
	pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH );
	pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_GREEN );
	pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_ORANGE );
	pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_RED );
	pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_YELLOW );
	pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_WHITE );
	pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_BROWN );
	pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_LILAC );
	pCreature->HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH_BLACK );
}

//----------------------------------------------------------------------
// Init MoveTable
//----------------------------------------------------------------------
// cX,cYtable을 초기화한다.
// [maxFrame종류8][8방향][maxFrame개 만큼의 frame들]
//----------------------------------------------------------------------
void
MCreature::InitMoveTable()
{
	for (int maxFrame=0; maxFrame<MAX_FRAME_MOVE; maxFrame++)
	{
		int maxFrame_plus_1	= maxFrame + 1;

		for (int d=0; d<MAX_DIRECTION; d++)
		{			
			// maxFrame개 만큼의 frame들
			if (m_cXTable[maxFrame][d]!=NULL)
			{
				delete [] m_cXTable[maxFrame][d];			
			}
			m_cXTable[maxFrame][d] = new int [maxFrame_plus_1];

			if (m_cYTable[maxFrame][d]!=NULL)
			{
				delete [] m_cYTable[maxFrame][d];				
			}
			m_cYTable[maxFrame][d] = new int [maxFrame_plus_1];
			
												// maxFrame이 8일 경우..
			int totalX	= -m_sXTable[d];				// 48
			int totalY	= -m_sYTable[d];				// 24

			int		pX	= 0, pY = 0;			// 바로전의 x,y값
			float	fX	= 0, fY = 0;			// 현재까지 더한 x,y값
			float	cX	= (float)totalX / maxFrame_plus_1;	// 6
			float	cY	= (float)totalY / maxFrame_plus_1;	// 3

			// 각 Frame을 초기화 시킨다.
			// maxFrame개를 더하면 totalX, totalY가 되어야 한다.			
			for (int f=0; f<maxFrame; f++)
			{
				// 현재까지 더한 x, y값
				fX += cX;
				fY += cY;

				// 현재 값에서 이전 값을 빼서 차이를 구한다.
				// fX를 반올림 하는것도 괜찮지 싶다.
				m_cXTable[maxFrame][d][f] = fX - pX;
				m_cYTable[maxFrame][d][f] = fY - pY;

				// 현재의 값을 기억해둔다.
				pX = fX;
				pY = fY;
			}

			m_cXTable[maxFrame][d][maxFrame] = totalX - pX;
			m_cYTable[maxFrame][d][maxFrame] = totalY - pY;
		}		
	}	
}	

//----------------------------------------------------------------------
// Release MoveTable
//----------------------------------------------------------------------
// cX,cYTable을 제거한다.	
//----------------------------------------------------------------------
void
MCreature::ReleaseMoveTable()
{
	for (int maxFrame=0; maxFrame<MAX_FRAME_MOVE; maxFrame++)
	{
		for (int d=0; d<MAX_DIRECTION; d++)
		{
			if (m_cXTable[maxFrame][d]!=NULL)
			{
				delete [] m_cXTable[maxFrame][d];
				m_cXTable[maxFrame][d] = NULL;
			}

			if (m_cYTable[maxFrame][d]!=NULL)
			{
				delete [] m_cYTable[maxFrame][d];
				m_cYTable[maxFrame][d] = NULL;
			}
		}
	}
}

		
//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
MCreature::MCreature()
{
	m_PersonalShopOpenTime = 0;
	m_HeadSkin = 0;
	m_DelayLastAction = 0;
	m_ObjectType	= TYPE_CREATURE;

	m_CreatureType	= CREATURETYPE_NULL;

	m_CreatureFrameIDs.Init(0);
	m_bExistBody	= FALSE;

	m_MoveType = CREATURE_GROUND;

	// 움직이는 방법과 그에 따른 이동 단위
	m_MoveDevice = MOVE_DEVICE_WALK;
	m_MoveAction = ACTION_MOVE;

	//m_ppCXTable = m_cXTableDeviceNull;
	//m_ppCYTable = m_cYTableDeviceNull;

	m_ColorBody1 = 0;
	m_ColorBody2 = 0;
	m_ChangeColorSet = ATTACHEFFECTCOLOR_NULL;	// 기본적으로 사용하지 않는다.

	m_ID = OBJECTID_NULL;

	m_pZone = NULL;

	m_X = SECTORPOSITION_NULL;
	m_Y = SECTORPOSITION_NULL;
	
	m_sX = 0;	
	m_sY = 0;

	m_cX = 0;
	m_cY = 0;

	m_Action = ACTION_STAND;				// Action에 대한 ID
	m_Direction = DIRECTION_LEFTDOWN;			// 바라봐야할 방향
	m_DirectionMove = m_Direction;
	m_DirectionMoved = m_Direction;
	m_CurrentDirection = DIRECTION_LEFTDOWN;	// 현재바라보는 방향

	//  행동	
	m_NextAction		= ACTION_STAND;


	m_ActionCount		= 0;				// Current Frame
	m_ActionCountMax	= 0;

	m_MoveCount			= 0;
	m_MoveCountMax		= 0;
	m_NextMoveCount		= 0;

	// 높이
	m_Z		= 0;
	
	//-------------------------------------------------------
	// 다음 움직일 방향	
	//-------------------------------------------------------
	m_NextX			= SECTORPOSITION_NULL;
	m_NextY			= SECTORPOSITION_NULL;
	m_NextDirection	= DIRECTION_NULL;

	//-------------------------------------------------------
	// 마지막으로 검증된 Sector의 좌표
	//-------------------------------------------------------
	m_ServerX	= SECTORPOSITION_NULL;
	m_ServerY	= SECTORPOSITION_NULL;


	//-------------------------------------------------------
	// Chatting String
	//-------------------------------------------------------
	m_ChatString = new char* [g_pClientConfig->MAX_CHATSTRING];
	for (int i=0; i<g_pClientConfig->MAX_CHATSTRING; i++)
	{
		m_ChatString[i] = new char [g_pClientConfig->MAX_CHATSTRINGLENGTH_PLUS1];
	}
	
	m_ChatStringCurrent = 0;
	for (i=0; i<g_pClientConfig->MAX_CHATSTRING; i++)
	{
		m_ChatString[i][0] = NULL;
	}
	m_ChatColor = RGB_WHITE;//0xFFFF;
	m_OriChatColor = RGB_WHITE;

	m_ChatTime = 0;

	//-------------------------------------------------------
	// 기술 종류
	//-------------------------------------------------------
	m_nBasicActionInfo		= SKILL_ATTACK_MELEE;
	m_nSpecialActionInfo	= ACTIONINFO_NULL;
	m_nUsedActionInfo		= ACTIONINFO_NULL;

	//-------------------------------------------------------
	// 캐릭터에 붙어 있는 Effect종류
	//-------------------------------------------------------
	m_bAttachEffect = new bool [(*g_pEffectSpriteTypeTable).GetSize()];
	for (i=0; i<(*g_pEffectSpriteTypeTable).GetSize(); i++)
	{
		m_bAttachEffect[i] = false;
	}

	m_bEffectStatus = new bool [(*g_pEffectStatusTable).GetSize()];
	for (i=0; i<(*g_pEffectStatusTable).GetSize(); i++)
	{
		m_bEffectStatus[i] = false;
	}

	m_AttachEffectColor = ATTACHEFFECTCOLOR_NULL;

//	m_MaxEffectLight = 0;	// 최고 밝기

	//-------------------------------------------------------
	// 목표에 대한 정보
	//-------------------------------------------------------
	SetTraceID( OBJECTID_NULL );
	m_TraceX				= SECTORPOSITION_NULL;
	m_TraceY				= SECTORPOSITION_NULL;
	m_TraceZ				= 0;

	// 빛나는 Effect들
	//m_nAlphaEffect = 0;

	// 최근에 채팅 String이 추가된 시점
	m_NextChatFadeTime = g_CurrentTime;

	m_pActionResult = NULL;

	m_pName = NULL;

	m_bAlive = true;

	m_bNextAction = false;
		
	m_nNextUsedActionInfo = ACTIONINFO_NULL;

	m_ItemCount = 0;

	m_EffectDelayFrame = 0;

	m_InvisibleCount = 0;
	m_InvisibleStep = 0;

	//-------------------------------------------------------
	// HP
	//-------------------------------------------------------
	MStatus::ClearStatus();

	SetStatus( MODIFY_CURRENT_HP, 100 );
	SetStatus( MODIFY_MAX_HP, 100 );
	SetStatus( MODIFY_EFFECT_STAT, EFFECTSTATUS_NULL );

	m_LevelName = 0;

	m_GuildNumber = -1;
	m_nUnionGuildID = 0;

	m_bFastMove = false;
	m_bKnockBack = 0;

	m_RepeatCount = 0;


	m_WeaponSpeed = WEAPON_SPEED_NORMAL;

	
	m_bHasHead = true;

	m_RecoveryHPAmount = 0; 
	m_RecoveryHPTimes = 0;
	m_RecoveryHPNextTime = 0;
	m_RecoveryHPDelayTime = 0;	
	m_RecoveryMPAmount = 0; 
	m_RecoveryMPTimes = 0;
	m_RecoveryMPNextTime = 0;
	m_RecoveryMPDelayTime = 0;	
	//m_RecoveryPart = MODIFY_CURRENT_HP;

	m_NextBloodingTime  = 0;

	m_DelayActionInfo = ACTIONINFO_NULL;

	m_bMale = true;

	m_ShadowCount = 0;

	m_bInCasket = false;
	m_CasketCount = 0;
	m_CasketType = 0;

	m_bCutHeight = false;
	m_CutHeightCount = 0;
	m_CutHeightCountInc = 0;

	m_bStopBloodDrain = 0;
	m_bStopAbsorbSoul = 0;

	// -1이면 darkness에 있는게 아니다.
	m_DarknessCount = -1;
	m_DarknessCountInc = 0;

	m_bPlayerParty = false;

	m_bHallu = false;
//	m_HalluName = 0;
	m_HalluName = rand()%g_pMonsterNameTable->GetLastNameSize();

	m_RegenDelayTime = 0;
	m_RegenNextTime = 0;
	m_RegenAmount = 0;
	m_RegenBonusDelayTime = 0;
	m_RegenBonusNextTime = 0;
	m_RegenBonusAmount = 0;

	m_Competence = 3;

	m_bTurning = false;
	m_TurningCount = 0;

	m_bCauseCriticalWounds = false;
	m_CauseCriticalWoundsCount = 0;

	m_bBloodyZenith = false;
	m_BloodyZenithCount = 0;

	m_bGunShotGuidance = false;
	m_GunShotGuidanceCount = 0;
	m_fGhost = 0;
	m_GhostCount = 0;

	m_bFakeDie = false;
	m_FakeDieCount = 0;
	m_GradeActionInfo = 0;
	m_TraceTimer = 0;
	m_BatColorSet = 0xFFFF;

	m_PetID = OBJECTID_NULL;
	m_ElementalID = OBJECTID_NULL;

	m_bSiegeWar = 0;

	m_bBurningSolCount = 0;
	m_bBurningSol = false;
	m_current_sprite_index = 0;
	m_Input_Chat_Flag = false;
	m_IsSkipMotorStand = false;

	m_Persnal_flag = false;

	m_MasterEffectType = 0;
}

MCreature::~MCreature()
{
	//----------------------------------------
	// 펫 제거
	//----------------------------------------
	if(GetPetID() != OBJECTID_NULL && g_pZone != NULL)
	{
		g_pZone->RemoveFakeCreature(GetPetID());
	}

	//----------------------------------------
	// 정령 제거
	//----------------------------------------
	if(GetElementalID() != OBJECTID_NULL && g_pZone != NULL)
	{
		g_pZone->RemoveFakeCreature(GetElementalID());
	}

	ClearAttachEffect();

	//-------------------------------------------------------
	// 이름 제거
	//-------------------------------------------------------
	if (m_pName!=NULL)
	{
		delete [] m_pName;
		m_pName = NULL;
	}

	//-------------------------------------------------------
	// Chatting String
	//-------------------------------------------------------
	if (m_ChatString != NULL)
	{
		for (int i=0; i<g_pClientConfig->MAX_CHATSTRING; i++)
		{
			delete [] m_ChatString[i];
		}

		delete [] m_ChatString;		
		m_ChatString = NULL;
	}

	//-------------------------------------------------------
	// effectspritetype
	//-------------------------------------------------------
	if (m_bAttachEffect != NULL)
	{
		delete [] m_bAttachEffect;
		m_bAttachEffect = NULL;
	}

	if (m_bEffectStatus != NULL)
	{
		delete [] m_bEffectStatus;
		m_bEffectStatus = NULL;
	}

	if (m_pActionResult!=NULL)
	{
		delete m_pActionResult;
		m_pActionResult = NULL;
	}
	
	ReleaseMoveBuffer();
}

bool IsCreatureTypeVampirePC(int type)
{
	return type == CREATURETYPE_VAMPIRE_MALE1 ||
		type == CREATURETYPE_VAMPIRE_MALE2 ||
		type == CREATURETYPE_VAMPIRE_MALE3 ||
		type == CREATURETYPE_VAMPIRE_FEMALE1 ||
		type == CREATURETYPE_VAMPIRE_FEMALE2 ||
		type == CREATURETYPE_VAMPIRE_FEMALE3 ||
		type == CREATURETYPE_WER_WOLF	||	
				// add by Coffee 2006.11.24  藤속뱁痢饑쮸近蹶
		type == CREATURETYPE_VAMPIRE_FEMALE4 ||
		type == CREATURETYPE_VAMPIRE_MALE4	||
		//add by viva
		type == CREATURETYPE_VAMPIRE_MALE5	||
		type == CREATURETYPE_VAMPIRE_FEMALE5	||
		type == CREATURETYPE_VAMPIRE_MALE6	||
		type == CREATURETYPE_VAMPIRE_FEMALE6;

}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Set Creature Type
//----------------------------------------------------------------------
void	
MCreature::SetCreatureType(TYPE_CREATURETYPE type)
{ 
	if (type < (*g_pCreatureTable).GetSize())
	{

	}
	else
	{
		DEBUG_ADD_FORMAT("[Error] CreatureType is Wrong. type=%d", (int)type);
		
		type = 0;
	}
//	type = 812;//add by viva
	m_CreatureType		= type;   

	m_ActionCount		= 0;
	m_ActionCountMax	= 0; 
	m_MoveCount			= 0;
	m_MoveCountMax		= 0; 
	m_Action		= (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND;
	m_sX=0; 
	m_sY=0;
	
	// 다음 동작도 없앰
	m_bNextAction = false;
	m_NextX = SECTORPOSITION_NULL;
	m_NextY = SECTORPOSITION_NULL;

//	int spriteType = (*g_pCreatureTable)[m_CreatureType].SpriteTypes[0];
//
//	m_CreatureFrameID	= (*g_pCreatureSpriteTable)[spriteType].FrameID;
//	m_bExistBody		= m_CreatureFrameID!=FRAMEID_NULL;

		//by csm
	int spriteType;
	m_CreatureFrameIDs.Init((*g_pCreatureTable)[m_CreatureType].SpriteTypes.GetSize());
	for(int framecnt = 0;  framecnt < (*g_pCreatureTable)[m_CreatureType].SpriteTypes.GetSize(); framecnt++ )
	{
		spriteType = (*g_pCreatureTable)[m_CreatureType].SpriteTypes[framecnt];
		m_CreatureFrameIDs[framecnt]	= (*g_pCreatureSpriteTable)[spriteType].FrameID;
	}
	
	
	m_bExistBody		= m_CreatureFrameIDs[0]!=FRAMEID_NULL;


	
	// 색깔 - 몹인 경우만.. 설정해준다.
	if(!IsCreatureTypeVampirePC(m_CreatureType))
	{
		m_ColorBody1		= (*g_pCreatureTable)[m_CreatureType].ColorSet;
		m_ColorBody2		= m_ColorBody1;
	}
	
	// 어둡게 찍기 제거
	m_bFade				= false;

	// 원래 크리쳐 타입이 바뀔때 이펙트를 날리도록 했다.
	// 이는 슬레이어<->뱀파이어의 변신시만 하도록 하쟈
	// 붙은 effect 제거
//	ClearAttachEffect();

	//by csm
	spriteType = (*g_pCreatureTable)[m_CreatureType].SpriteTypes[0];
	//---------------------------------------------------------------------------
	// 기술 설정
	//---------------------------------------------------------------------------
	// 기본 공격 동작이 없는 경우..
	if (!(*g_pCreatureSpriteTable)[spriteType].IsPlayerSprite()
		//&& (*g_pCreatureTable)[m_CreatureType].GetActionCount( ACTION_ATTACK )==0)
		&& GetCreatureActionCountMax( this, ACTION_ATTACK )==0)
	{
		m_nBasicActionInfo = ACTIONINFO_NULL;
	}
	else
	{
		m_nBasicActionInfo = SKILL_ATTACK_MELEE;
	}

//	m_nSpecialActionInfo	= ACTIONINFO_NULL;
	m_nUsedActionInfo		= ACTIONINFO_NULL;

	//m_WeaponSpeed			= WEAPON_SPEED_NORMAL;

	//---------------------------------------------------------------------------
	// 완전히 색깔이 바뀌는 몹인 경우
	//---------------------------------------------------------------------------
	int changeColor = (*g_pCreatureTable)[m_CreatureType].ChangeColorSet;
	if (changeColor < MAX_COLORSET)
	{
		m_ChangeColorSet = changeColor;
		m_AttachEffectColor = changeColor;
	}
	else
	{
		// ATTACHEFFECTCOLOR_NULL이면 정상적인 색깔이다.

		if ((*g_pCreatureSpriteTable)[spriteType].IsMonsterSprite())
		{
			m_ChangeColorSet = g_ZoneCreatureColorSet;//ATTACHEFFECTCOLOR_NULL;
			m_AttachEffectColor = g_ZoneCreatureColorSet;//ATTACHEFFECTCOLOR_NULL;
		}
		else
		{
			m_ChangeColorSet = ATTACHEFFECTCOLOR_NULL;
			m_AttachEffectColor = ATTACHEFFECTCOLOR_NULL;
		}
	}

	//---------------------------------------------------------------------------
	// 기본(?)적으로 성별 설정..
	// 늑대나 박쥐인 경우는 creaturetype으로 성별을 알 수가 없어서리..
	//---------------------------------------------------------------------------
	if( m_CreatureType != CREATURETYPE_WER_WOLF )
		m_bMale = (*g_pCreatureTable)[m_CreatureType].bMale;

	//---------------------------------------------------------------------------
	// 몸 잔상
	//---------------------------------------------------------------------------
	m_ShadowCount = (*g_pCreatureTable)[m_CreatureType].ShadowCount;

	m_bInCasket = false;
	m_CasketCount = 0;

	//---------------------------------------------------------------------------
	// Effect 제거
	//---------------------------------------------------------------------------
	if (HasEffectStatus(EFFECTSTATUS_TRANSFORM_TO_BAT)
		&& m_CreatureType!=CREATURETYPE_BAT)
	{
		RemoveEffectStatus(EFFECTSTATUS_TRANSFORM_TO_BAT);
	}

	if (HasEffectStatus(EFFECTSTATUS_TRANSFORM_TO_WOLF)
		&& m_CreatureType!=CREATURETYPE_WOLF)
	{
		RemoveEffectStatus(EFFECTSTATUS_TRANSFORM_TO_WOLF);
	}

	if (HasEffectStatus(EFFECTSTATUS_TRANSFORM_TO_WERWOLF)
		&& m_CreatureType != CREATURETYPE_WER_WOLF)
	{
		RemoveEffectStatus(EFFECTSTATUS_TRANSFORM_TO_WERWOLF);
	}

	// 2004, 9, 13, sobeit add start - 총슬 130 스킬
	if (HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET)
		&& m_CreatureType != CREATURETYPE_INSTALL_TURRET)
	{
		RemoveEffectStatus(EFFECTSTATUS_INSTALL_TURRET);
	}
	// 2004, 9, 13, sobeit add end - 총슬 130 스킬
	
	// Weapon Speed를 다시 설정해준다.
	int attackSpeed = m_Status[MODIFY_ATTACK_SPEED];

	// 0이면 값이 설정이 안됐다고 가정한다. 특히, 몹인 경우이다.
	if (attackSpeed!=0)	
	{
		SetWeaponSpeed( attackSpeed );
	}

	CheckRegen();
}

//----------------------------------------------------------------------
// Set BodyColor1
//----------------------------------------------------------------------
void		
MCreature::SetBodyColor1(WORD set)		
{ 
	m_ColorBody1 = set; 
}

//----------------------------------------------------------------------
// Set BodyColor2
//----------------------------------------------------------------------
void		
MCreature::SetBodyColor2(WORD set)		
{ 
	m_ColorBody2 = set; 

	if (m_CreatureType==CREATURETYPE_VAMPIRE_OPERATOR
		|| m_CreatureType==CREATURETYPE_SLAYER_OPERATOR
		|| m_CreatureType==CREATURETYPE_OUSTERS_OPERATOR)
	{
		m_ColorBody1 = set;
	}
}

//----------------------------------------------------------------------
// Clear AttachEffect
//----------------------------------------------------------------------
void
MCreature::ClearAttachEffect()
{
	//-------------------------------------------------------
	// Creature에 붙어 있는 Effect를 메모리에서 제거한다.
	//-------------------------------------------------------
	ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

	while (iEffect != m_listEffect.end())
	{
		MEffect* pEffect = *iEffect;
		delete pEffect;
		
		iEffect++;
	}

	m_listEffect.clear();

	//-------------------------------------------------------
	// 몸에 붙는 Effect
	//-------------------------------------------------------
	iEffect = m_listGroundEffect.begin();

	while (iEffect != m_listGroundEffect.end())
	{
		MEffect* pEffect = *iEffect;
		delete pEffect;
		
		iEffect++;
	}

	m_listGroundEffect.clear();

	//-------------------------------------------------------
	// 체크 값 제거
	//-------------------------------------------------------
	if (g_pEffectSpriteTypeTable!=NULL)
	{
		for (int i=0; i<(*g_pEffectSpriteTypeTable).GetSize(); i++)
		{
			m_bAttachEffect[i] = false;
		}
	}

	if (g_pEffectStatusTable!=NULL)
	{
		for (int i=0; i<(*g_pEffectStatusTable).GetSize(); i++)
		{
			m_bEffectStatus[i] = false;
		}	
	}

	UnSetHalluCreature();
}
void
MCreature::RemoveGlacierEffect()
{
	SetStop();
	StopCauseCriticalWounds();
	ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

	bool re = false;
	while (iEffect != m_listEffect.end())
	{
		MAttachEffect* pEffect = *iEffect;
		TYPE_EFFECTSPRITETYPE Type = pEffect->GetEffectSpriteType();
		if(	EFFECTSPRITETYPE_GDR_GLACIER_SLAYER_START == Type ||
			EFFECTSPRITETYPE_GDR_GLACIER_SLAYER_LOOP == Type ||
			EFFECTSPRITETYPE_GDR_GLACIER_SLAYER_END == Type ||
			EFFECTSPRITETYPE_GDR_GLACIER_VAMPIRE_START == Type ||
			EFFECTSPRITETYPE_GDR_GLACIER_VAMPIRE_LOOP == Type ||
			EFFECTSPRITETYPE_GDR_GLACIER_VAMPIRE_END == Type ||
			EFFECTSPRITETYPE_GDR_GLACIER_OUSTERS_START == Type ||
			EFFECTSPRITETYPE_GDR_GLACIER_OUSTERS_LOOP == Type ||
			EFFECTSPRITETYPE_GDR_GLACIER_OUSTERS_END == Type 

			||EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_2 == Type 
			||EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_1 == Type 
			||EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_3 == Type 
			||EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_4 == Type 
			)
		{
			// 메모리 제거
			delete pEffect;

			ATTACHEFFECT_LIST::iterator dEffect = iEffect;
			iEffect--;

			// list에서 제거
			m_listEffect.erase( dEffect );

			m_bAttachEffect[Type] = false;
	
			re = true;
		}
		
		iEffect++;

	}
	m_bEffectStatus[EFFECTSTATUS_GLACIER]  = false;
}

void
MCreature::Remove_Curse_Paralsis_Effect()
{
	ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();
	while (iEffect != m_listEffect.end())
	{
		MAttachEffect* pEffect = *iEffect;
		TYPE_EFFECTSPRITETYPE Type = pEffect->GetEffectSpriteType();
		if(	EFFECTSPRITETYPE_PURPLE_WALL_2 == Type )
		{
			pEffect->SetCount( 0, 0 );
		}
		iEffect++;
	}
}

//----------------------------------------------------------------------
// Remove EffectStatus
//----------------------------------------------------------------------
// 특정한 EffectSpriteType을 제거한다.
//----------------------------------------------------------------------
bool
MCreature::RemoveEffectStatus(EFFECTSTATUS status)
{
	DEBUG_ADD_FORMAT("[RemoveEffectStatus] id=%d, status=%d", m_ID, (int)status);
	
	if (status>=g_pEffectStatusTable->GetSize())
	{
		DEBUG_ADD_FORMAT("[Error]EffectStatus exceed Max : %d", (int)status);
		
		return false;
	}

	if (!m_bEffectStatus[status])
	{
		// [새기술7]
		DEBUG_ADD_FORMAT("NotEffectStatus(%d)", (int)status);
		return false;
	}

	m_bEffectStatus[status] = false;

	bool bUseEffectSprite = (*g_pEffectStatusTable)[status].bUseEffectSprite;

	int pTypes = (*g_pEffectStatusTable)[status].EffectSpriteType;
	//------------------------------------------------------------
	// effectStatus에 따라서.
	//------------------------------------------------------------
	switch (status)
	{
		case EFFECTSTATUS_SUMMON_SYLPH_GREEN :
		case EFFECTSTATUS_SUMMON_SYLPH_ORANGE :
		case EFFECTSTATUS_SUMMON_SYLPH_RED :
		case EFFECTSTATUS_SUMMON_SYLPH_YELLOW :
		case EFFECTSTATUS_SUMMON_SYLPH_WHITE :
		case EFFECTSTATUS_SUMMON_SYLPH_BROWN :
		case EFFECTSTATUS_SUMMON_SYLPH_LILAC :
		case EFFECTSTATUS_SUMMON_SYLPH_BLACK :
		case EFFECTSTATUS_SUMMON_SYLPH:
			SetMoveDevice(MOVE_DEVICE_WALK);
			if(IsAdvancementClass() && status==EFFECTSTATUS_SUMMON_SYLPH)
			{
				pTypes = EFFECTSPRITETYPE_OUSTERS_FASTMOVE_LOOP;
				ExecuteActionInfoFromMainNode(SKILL_CLIENT_ADVANCEMENT_SUMMON_SYLPH_END,
					GetX(), GetY(), 0,GetDirection(),	GetID(),	
					GetX(), GetY(), 0, 8, NULL, false);
			}
		break;
		case EFFECTSTATUS_BIKE_CRASH:
			SetMoveDevice(MOVE_DEVICE_WALK);
		break;

		//------------------------------------------------------------
		// 마비 풀릴 때
		//------------------------------------------------------------
		case EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS :
		case EFFECTSTATUS_EXPLOSION_WATER :
			if (m_bAlive)
			{
				SetStop();
				StopCauseCriticalWounds();
	#ifdef OUTPUT_DEBUG					
			DEBUG_ADD_FORMAT("[CCW] Stop");
	#endif
			}			
		break;

		case EFFECTSTATUS_CURSE_PARALYSIS :
			if (m_bAlive)
			{
				SetStop();			
				Remove_Curse_Paralsis_Effect();
			}			
		break;

		//------------------------------------------------------------
		// 마비 풀릴 때
		//------------------------------------------------------------
		// add by Coffee 2007-3-21
		case EFFECTSTATUS_SATELLITE_BOMB_AIM :
		// end	
		case EFFECTSTATUS_GUN_SHOT_GUIDANCE_AIM :
			if (m_bAlive)
			{
				SetStop();
				StopGunShotGuidance();
	#ifdef OUTPUT_DEBUG					
			DEBUG_ADD_FORMAT("[GSG] Stop");
	#endif
			}			
		break;

		//------------------------------------------------------------
		// EFFECTSTATUS_SUMMON_CASKET [새기술]
		//------------------------------------------------------------
		case EFFECTSTATUS_CASKET :
			// 바로 invisible로 만든다.
			RemoveCasket();
		break;


		//------------------------------------------------------------
		// Invisible 제거
		//------------------------------------------------------------
		case EFFECTSTATUS_INVISIBILITY :
		case EFFECTSTATUS_SNIPPING_MODE :	
			// invisible을 제거한다.
			SetVisible();
		break;

		//------------------------------------------------------------
		// COMA 제거
		//------------------------------------------------------------
		case EFFECTSTATUS_COMA :
			SetAlive();
			if(IsVampire())
			{
				ExecuteActionInfoFromMainNode(
							BLOOD_RESURRECT,										// 사용 기술 번호
						
							m_X, m_Y, 0,
							(int)m_Direction,														// 사용 방향
							
							m_ID,												// 목표에 대한 정보
							m_X, m_Y, 0,
							
							2*16, //5*16, 
							
							NULL,
							
							false,
							
							(int)m_sX, (int)m_sY);	
			}
		break;

		case EFFECTSTATUS_GHOST:
			if (!(*g_pCreatureTable)[m_CreatureType].bFlyingCreature)	// 박쥐인 경우
				SetGroundCreature();
			break;
		case EFFECTSTATUS_WILL_OF_LIFE :
			if( IsVampire() )
				CheckRegen();
			break;

		case EFFECTSTATUS_GLACIER :
			RemoveGlacierEffect();
			break;
		case EFFECTSTATUS_SIEGE_DEFENDERL:
		case EFFECTSTATUS_SIEGE_REINFORCE:
		case EFFECTSTATUS_SIEGE_ATTACKER_1:
		case EFFECTSTATUS_SIEGE_ATTACKER_2:
		case EFFECTSTATUS_SIEGE_ATTACKER_3:
		case EFFECTSTATUS_SIEGE_ATTACKER_4:
		case EFFECTSTATUS_SIEGE_ATTACKER_5:
			SetSiegeWar(0);
			break;
//		case EFFECTSTATUS_BURNING_SOL_CHARGE_1:
//		case EFFECTSTATUS_BURNING_SOL_CHARGE_2:
//		case EFFECTSTATUS_BURNING_SOL_CHARGE_3:
//		case EFFECTSTATUS_BURNING_SOL_CHARGE_4:
//			StopBurningSol();
//			break;
		case EFFECTSTATUS_INSTALL_TURRET:
			ExecuteActionInfoFromMainNode(RESULT_SKILL_INSTALL_TURRET,GetX(), GetY(), 0,0,GetID()	,	
				GetX(), GetY(), 0, 0, NULL, false);
			SetAction( ACTION_STAND );
			SetInstallTurretCount(0);
			SetInstallTurretDirect(2);
			SetDirection( 2 );
			SetCurrentDirection( 2 );
			break;

		case EFFECTSTATUS_GRAND_MASTER_SLAYER:
		case EFFECTSTATUS_GRAND_MASTER_VAMPIRE:
		case EFFECTSTATUS_GRAND_MASTER_OUSTERS:
		case EFFECTSTATUS_GRAND_MASTER_SLAYER_130:
		case EFFECTSTATUS_GRAND_MASTER_SLAYER_150:
		case EFFECTSTATUS_GRAND_MASTER_VAMPIRE_130:
		case EFFECTSTATUS_GRAND_MASTER_VAMPIRE_150:
		case EFFECTSTATUS_GRAND_MASTER_OUSTERS_130:
		case EFFECTSTATUS_GRAND_MASTER_OUSTERS_150:
			{
				pTypes = GetMasterEffectType(status);
			}
			break;
	}


	//------------------------------------------------------------
	//
	//			EffectSprite로 표현하는 경우
	//
	//------------------------------------------------------------
	if (bUseEffectSprite)
	{
		int numTypes = 1;
//		int* pTypes = NULL;
		
//		if (status==EFFECTSTATUS_SPIRIT_GUARD_1 || 
//			status==EFFECTSTATUS_SPIRIT_GUARD_2 || 
//			status==EFFECTSTATUS_SPIRIT_GUARD_3 || 
//			status==EFFECTSTATUS_SPIRIT_GUARD_4)
//		{
//			numTypes = EFFECTSTATUS_SPIRIT_GUARD_4 - EFFECTSTATUS_SPIRIT_GUARD_1 +1;
//
//			pTypes = new int[numTypes];
//
//			for(int i = 0; i < numTypes; i++)
//				pTypes[i] = EFFECTSPRITETYPE_SPIRIT_GUARD;
//		}
//		else
//		if (status==EFFECTSTATUS_ARMAGEDDON)
//		{			
//			numTypes = 4;
//			pTypes = new int[numTypes];
//
//			pTypes[0] = EFFECTSPRITETYPE_ARMAGEDDON_BACK_0_2;
//			pTypes[1] = EFFECTSPRITETYPE_ARMAGEDDON_BACK_1_2;
//			pTypes[2] = EFFECTSPRITETYPE_ARMAGEDDON_BACK_2_2;
//			pTypes[3] = EFFECTSPRITETYPE_ARMAGEDDON_BACK_3_2;
//		}
//		else
		{
			numTypes = 1;
//			pTypes = new int[numTypes];

			//pTypes[0] = (*g_pEffectStatusTable)[status].EffectSpriteType;
			
		}		

		//for (int i=0; i<numTypes; i++)
		{
		//	TYPE_EFFECTSPRITETYPE type = pTypes[i];
			TYPE_EFFECTSPRITETYPE type = pTypes;
			TYPE_EFFECTSPRITETYPE type2 = EFFECTSPRITETYPE_NULL;

			if(type == EFFECTSPRITETYPE_FAST_MOVE_STOP)
				type2 = EFFECTSPRITETYPE_FAST_MOVE_FLY;
			
			if( type >= EFFECTSPRITETYPE_SUMMON_SYLPH_GREEN_STOP &&
				type <= EFFECTSPRITETYPE_SUMMON_SYLPH_BLACK_STOP &&
				((type - EFFECTSPRITETYPE_SUMMON_SYLPH_GREEN_STOP)&0x1) == 0
				)
				type2 = type - 1;

			//-------------------------------------------------------
			// Creature에 붙어 있는 Effect중에서
			// EffectSpriteType이 type인 것을 제거한다.
			//-------------------------------------------------------
			if (type < g_pEffectSpriteTypeTable->GetSize())
			{
				if (IsFemale()
					&& (*g_pEffectSpriteTypeTable)[type].FemaleEffectSpriteType!=EFFECTSPRITETYPE_NULL)
				{
					type = (*g_pEffectSpriteTypeTable)[type].FemaleEffectSpriteType;
				}
				

				if (m_bAttachEffect[type])
				{
					//-------------------------------------------------------
					// 몸에 붙은거
					//-------------------------------------------------------
					ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

					bool re = false;
					while (iEffect != m_listEffect.end())
					{
						MAttachEffect* pEffect = *iEffect;
						
						//-------------------------------------------------------
						// 같은 type을 찾는다.
						//-------------------------------------------------------
						if (pEffect->IsEffectSprite() 
							&& (pEffect->GetEffectSpriteType() == type || type2 != EFFECTSPRITETYPE_NULL &&
							type2 == pEffect->GetEffectSpriteType()) )
						{
							// 메모리 제거
							delete pEffect;

							ATTACHEFFECT_LIST::iterator dEffect = iEffect;
							iEffect--;

							// list에서 제거
							m_listEffect.erase( dEffect );

							m_bAttachEffect[type] = false;
					
							re = true;
						}
						
						iEffect++;

					}

					if(re)
					{
					//	delete [] pTypes;
						return true;
					}

					//-------------------------------------------------------
					// 바닥에 붙은 거
					//-------------------------------------------------------
					iEffect = m_listGroundEffect.begin();

					while (iEffect != m_listGroundEffect.end())
					{
						MAttachEffect* pEffect = *iEffect;
						
						//-------------------------------------------------------
						// 같은 type을 찾는다.
						//-------------------------------------------------------
						if (pEffect->IsEffectSprite() && pEffect->GetEffectSpriteType() == type
							|| type2 != EFFECTSPRITETYPE_NULL && type2 == pEffect->GetEffectSpriteType() )
						{
							// 메모리 제거
							delete pEffect;

							// list에서 제거
							m_listGroundEffect.erase( iEffect );

							m_bAttachEffect[type] = false;
					
						//	delete [] pTypes;
							return true;
						}
						
						iEffect++;
					}
				}
			}
		}

	//	delete [] pTypes;
	}
	//------------------------------------------------------------
	//
	//			EffectColor로 표현하는 경우
	//
	//------------------------------------------------------------
	else
	{
		WORD colorSet = (*g_pEffectStatusTable)[status].EffectColor;

		//-------------------------------------------------------
		// Creature에 붙어 있는 Effect중에서
		// EffectColor가 color인 것을 제거한다.
		//-------------------------------------------------------
		ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

		while (iEffect != m_listEffect.end())
		{
			MAttachEffect* pEffect = *iEffect;
			
			//-------------------------------------------------------
			// 같은 type을 찾는다.
			//-------------------------------------------------------
			if (pEffect->IsEffectColor() && pEffect->GetEffectColor() == colorSet)
			{
				// 메모리 제거
				delete pEffect;

				// list에서 제거
				m_listEffect.erase( iEffect );

				return true;
			}
			
			iEffect++;
		}		

		// GroundEffect는 없다고 본다.
	}
	
	return false;
}

//----------------------------------------------------------------------
// Add EffectStatus
//----------------------------------------------------------------------
// 
// 특별한 상태를 나타내는 effect
//
// Creature에 달라붙어서 표현되는 Effect 표현
//
// 중첩을 허용하지 않으므로.. 
// 아직 붙어서 표현되지 않은 Effect만 새로 추가한다.
//
// 그러나, 시간 확장을 해야하는 경우가 생긴다.
//----------------------------------------------------------------------
bool
MCreature::AddEffectStatus(enum EFFECTSTATUS status, DWORD delayFrame)
{
	DEBUG_ADD_FORMAT("[AddEffectStatus] id=%d, status=%d, delay=%d", m_ID, (int)status, delayFrame);
//	if( status == EFFECTSTATUS_VIEW_HP )
//	{
//		int a = 0;
//	}
	
	//----------------------------------------------------------
	// 이미 죽은 경우
	//----------------------------------------------------------
	if (IsDead() && status != EFFECTSTATUS_COMA && 
		!(
		GetCreatureType() >= 371 && GetCreatureType() <= 376 || GetCreatureType() >= 560 && GetCreatureType() <= 563) && 
		GetCreatureType() != 482 && GetCreatureType() != 650 && !(GetCreatureType() >= 526 && GetCreatureType() <= 549) &&
		GetCreatureType() != 670 && GetCreatureType() != 672 && GetCreatureType() != 673
		)	// 성물인 경우는 이펙트 붙임
	{
		DEBUG_ADD("Don't AddEffectStatus : Already Dead");
		
		return false;
	}

	if (status>=g_pEffectStatusTable->GetSize())
	{
		DEBUG_ADD_FORMAT("[Error]EffectStatus exceed Max : %d", (int)status);
		
		return false;
	}

	int eCount = 0; // 이펙트의 개수
	int orbit_type = 0;

	//------------------------------------------------------------
	// effectStatus에 따라서.
	//------------------------------------------------------------
	MEffect::EFFECT_TYPE eType = MEffect::EFFECT_ATTACH;
	TYPE_EFFECTSPRITETYPE type = (*g_pEffectStatusTable)[status].EffectSpriteType;
	switch (status)
	{
	case EFFECTSTATUS_SUMMON_SYLPH_GREEN :
	case EFFECTSTATUS_SUMMON_SYLPH_ORANGE :
	case EFFECTSTATUS_SUMMON_SYLPH_RED :
	case EFFECTSTATUS_SUMMON_SYLPH_YELLOW :
	case EFFECTSTATUS_SUMMON_SYLPH_WHITE :
	case EFFECTSTATUS_SUMMON_SYLPH_BROWN :
	case EFFECTSTATUS_SUMMON_SYLPH_LILAC :
	case EFFECTSTATUS_SUMMON_SYLPH_BLACK :
	case EFFECTSTATUS_SUMMON_SYLPH:
		{
			SetMoveDevice(MOVE_DEVICE_SUMMON_SYLPH);
			/********  Edit By sonic 혼뇜침쥣랗瘻빈긴헷槻벎********/
			
			if(IsAdvancementClass() && status==EFFECTSTATUS_SUMMON_SYLPH)
			{
				type = EFFECTSPRITETYPE_OUSTERS_FASTMOVE_LOOP;
				ExecuteActionInfoFromMainNode(SKILL_CLIENT_ADVANCEMENT_SUMMON_SYLPH_START,GetX(), GetY(), 0,GetDirection(),	GetID(),	
				GetX(), GetY(), 0, 8, NULL, false);
			}
			/********End *******************************************/
		}
		break;

		//------------------------------------------------------------
		// EFFECTSTATUS_INVISIBILITY
		//------------------------------------------------------------
		case EFFECTSTATUS_INVISIBILITY :
			// 바로 invisible로 만든다.
			SetInvisibleSoon();
		break;

		//------------------------------------------------------------
		// EFFECTSTATUS_SUMMON_CASKET [새기술]
		//------------------------------------------------------------
		case EFFECTSTATUS_CASKET :
			// 바로 invisible로 만든다.
			AddCasketSoon(0);
		break;

		//------------------------------------------------------------
		// EFFECTSTATUS_GUN_SHOT_GUIDANCE_AIM
		//------------------------------------------------------------

		// add by Coffee 2007-3-21
		case EFFECTSTATUS_SATELLITE_BOMB_AIM :
		// end		
		case EFFECTSTATUS_GUN_SHOT_GUIDANCE_AIM :
#ifdef OUTPUT_DEBUG					
				DEBUG_ADD_FORMAT("[GSG] Set");
#endif
//				AffectMoveBufferAll();
//				ActionMoveNextPosition();
			if(!IsVampire())
			{
				SetAction((*g_pActionInfoTable)[m_nBasicActionInfo].GetAction());
				m_ActionCount = 0;
				SetGunShotGuidance(delayFrame);
			}
			else
			{
				return false;
			}
			break;

		//------------------------------------------------------------
		// EFFECTSTATUS_CURSE_PARALYSIS
		//------------------------------------------------------------
		case EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS :
#ifdef OUTPUT_DEBUG					
				DEBUG_ADD_FORMAT("[CCW] Set");
#endif
//				AffectMoveBufferAll();
//				ActionMoveNextPosition();
			// CauseCriticalWounds인 경우 슬레이어는 AddEffect를 하지 않는다. 이펙트 붙이지 않고 데미지만
			// 2005, 1, 18, sobeit modify start // g_pZone->GetID() == 4002 추가 - 퀘스트 cause_critical_wounds걸려있는 슬레이어 npc
			if(!IsSlayer() || GetCreatureType() == 793) 
			// 2005, 1, 18, sobeit modify end
			{
				SetAction(ACTION_DRAINED);
				m_ActionCount = 0;
				SetCauseCriticalWounds(delayFrame);
			}
			else
			{
				SetAction(ACTION_DAMAGED);
				return false;
			}
			break;

		case EFFECTSTATUS_EXPLOSION_WATER:
			SetAction(ACTION_DRAINED);
			m_ActionCount = 0;
			SetCauseCriticalWounds(delayFrame);
			break;
			
		case EFFECTSTATUS_CURSE_PARALYSIS :
			// 다 이동시킨다.
			AffectMoveBufferAll();
			
			ActionMoveNextPosition();
		break;

		case EFFECTSTATUS_FIRE_ELEMENTAL:
			if(GetClassType() == CLASS_FAKE)
			{
				delayFrame = 0xFFFF;
				eType = MEffect::EFFECT_ATTACH_ORBIT;
				eCount = 1;
				orbit_type = 2;
			}
			else
			{
				AddOustersElemental( this, 699, status );
				delayFrame = 0;
			}
			break;

		case EFFECTSTATUS_FIRE_ELEMENTAL_ATTACK:
			if(GetClassType() == CLASS_FAKE)
			{
//				ClearAttachEffect();
				delayFrame = 0;
				ExecuteActionInfoFromMainNode(
							SKILL_CLIENT_FIRE_ELEMENTAL_ATTACK,										// 사용 기술 번호
						
							m_X, m_Y, 0,
							(int)m_Direction,														// 사용 방향
							
							m_ID,												// 목표에 대한 정보
							m_X, m_Y, 0,
							
							0xFFFF, //5*16, 
							
							NULL,
							
							false,
							
							(int)m_sX, (int)m_sY);	
			}
			else
			{
				MFakeCreature *pFakeCreature = (MFakeCreature *)g_pZone->GetFakeCreature(GetElementalID());
				if(pFakeCreature != NULL)
				{
					pFakeCreature->AddEffectStatus(status, 0xFFFF);
				}
				return false;
			}
			break;

		case EFFECTSTATUS_WATER_ELEMENTAL:
			if(GetClassType() == CLASS_FAKE)
			{
				eType = MEffect::EFFECT_ATTACH_ORBIT;
				eCount = 1;
				orbit_type = 2;
				delayFrame = 0xFFFF;
			}
			else
			{
				AddOustersElemental( this, 699, status );
				delayFrame = 0;
			}
			break;

		case EFFECTSTATUS_WATER_ELEMENTAL_HEAL:
			if(GetClassType() == CLASS_FAKE)
			{
//				ClearAttachEffect();
				delayFrame = 0;
				ExecuteActionInfoFromMainNode(
							SKILL_CLIENT_WATER_ELEMENTAL_HEAL,										// 사용 기술 번호
						
							m_X, m_Y, 0,
							(int)m_Direction,														// 사용 방향
							
							m_ID,												// 목표에 대한 정보
							m_X, m_Y, 0,
							
							0xFFFF, //5*16, 
							
							NULL,
							
							false,
							
							(int)m_sX, (int)m_sY);	
			}
			else
			{
				MFakeCreature *pFakeCreature = (MFakeCreature *)g_pZone->GetFakeCreature(GetElementalID());
				if(pFakeCreature != NULL)
				{
					pFakeCreature->AddEffectStatus(status, 0xFFFF);
				}
				return false;
			}
			break;

		//------------------------------------------------------------
		// EFFECTSTATUS_COMA
		//------------------------------------------------------------
		case EFFECTSTATUS_COMA :
			SetDead();
		break;

		case EFFECTSTATUS_FIRE_ELEMENTAL_DAMAGED:
			{
				TYPE_EFFECTSPRITETYPE type = (*g_pEffectStatusTable)[status].EffectSpriteType;
				if(type < g_pEffectSpriteTypeTable->GetSize())
				{
					TYPE_FRAMEID	frameID = (*g_pEffectSpriteTypeTable)[type].FrameID;
					BYTE			maxFrame;
					
					// BLT_NORMAL		
					BLT_TYPE bltType = (*g_pEffectSpriteTypeTable)[type].BltType;
					maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);		

					delayFrame = maxFrame;
				}
			}
			break;

		case EFFECTSTATUS_WATER_ELEMENTAL_HEALED:
			{
				TYPE_EFFECTSPRITETYPE type = (*g_pEffectStatusTable)[status].EffectSpriteType;
				if(type < g_pEffectSpriteTypeTable->GetSize())
				{
					TYPE_FRAMEID	frameID = (*g_pEffectSpriteTypeTable)[type].FrameID;
					BYTE			maxFrame;
					
					// BLT_NORMAL		
					BLT_TYPE bltType = (*g_pEffectSpriteTypeTable)[type].BltType;
					maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);		

					delayFrame = maxFrame;
				}
			}
			break;

		//------------------------------------------------------------
		// EFFECTSTATUS_ARMAGEDDON
		//------------------------------------------------------------
		case EFFECTSTATUS_ARMAGEDDON :
		case EFFECTSTATUS_GROUND_ELEMENTAL_CENTER:
			delayFrame = 0xFFFF;		// 무한..
		break;

		//------------------------------------------------------------
		// 성물 관련
		//------------------------------------------------------------
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_GREGORI :         // 151			
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_NEMA :         // 152				
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_LEGIOS :         // 153				
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_MIHOLE :         // 154				
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_AROSA :         // 155				
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_ARMEGA :         // 156				
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_INI :         // 157				
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_JAVE :         // 158				
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_CHASPA :         // 159				
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_CONCILIA :         // 160				
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_KIRO :         // 161				
		case EFFECTSTATUS_HAS_BLOOD_BIBLE_HILLEL :         // 162						
		case EFFECTSTATUS_SHRINE_GUARD_GREGORI :				// 163				
		case EFFECTSTATUS_SHRINE_GUARD_NEMA :				// 164				
		case EFFECTSTATUS_SHRINE_GUARD_LEGIOS :				// 165				
		case EFFECTSTATUS_SHRINE_GUARD_MIHOLE :				// 166				
		case EFFECTSTATUS_SHRINE_GUARD_AROSA :				// 167				
		case EFFECTSTATUS_SHRINE_GUARD_ARMEGA :				// 168				
		case EFFECTSTATUS_SHRINE_GUARD_INI :				// 169				
		case EFFECTSTATUS_SHRINE_GUARD_JAVE :				// 170				
		case EFFECTSTATUS_SHRINE_GUARD_CHASPA :				// 171				
		case EFFECTSTATUS_SHRINE_GUARD_CONCILIA :				// 172				
		case EFFECTSTATUS_SHRINE_GUARD_KIRO :				// 173				
		case EFFECTSTATUS_SHRINE_GUARD_HILLEL :				// 174						
		case EFFECTSTATUS_SHRINE_HOLY_GREGORI :				// 175				
		case EFFECTSTATUS_SHRINE_HOLY_NEMA :				// 176				
		case EFFECTSTATUS_SHRINE_HOLY_LEGIOS :				// 177				
		case EFFECTSTATUS_SHRINE_HOLY_MIHOLE :				// 178				
		case EFFECTSTATUS_SHRINE_HOLY_AROSA :				// 179				
		case EFFECTSTATUS_SHRINE_HOLY_ARMEGA :				// 180				
		case EFFECTSTATUS_SHRINE_HOLY_INI :				// 181				
		case EFFECTSTATUS_SHRINE_HOLY_JAVE :				// 182				
		case EFFECTSTATUS_SHRINE_HOLY_CHASPA :				// 183				
		case EFFECTSTATUS_SHRINE_HOLY_CONCILIA :			// 184				
		case EFFECTSTATUS_SHRINE_HOLY_KIRO :			// 185				
		case EFFECTSTATUS_SHRINE_HOLY_HILLEL :			// 186				
		case EFFECTSTATUS_SHRINE_SHIELD:
		case EFFECTSTATUS_SLAYER_RELIC:
//		case EFFECTSTATUS_DROP_BLOOD_BIBLE:			
		case EFFECTSTATUS_VAMPIRE_RELIC:
			delayFrame = 0xFFFF;		// 무한..
			break;

		//------------------------------------------------------------
		// EFFECTSTATUS_CURE_CRITICAL_WOUNDS
		//------------------------------------------------------------
		case EFFECTSTATUS_CURE_CRITICAL_WOUNDS :
			eType = MEffect::EFFECT_ATTACH_ORBIT;
			eCount = 1;
			orbit_type = 0;
			break;
			
		case EFFECTSTATUS_SPIRIT_GUARD_1 :
			eType = MEffect::EFFECT_ATTACH_ORBIT;
			eCount = 1;
			orbit_type = 1;
			break;
			
		case EFFECTSTATUS_SPIRIT_GUARD_2 :
			eType = MEffect::EFFECT_ATTACH_ORBIT;
			eCount = 2;
			orbit_type = 1;
			break;
			
		case EFFECTSTATUS_SPIRIT_GUARD_3 :
			eType = MEffect::EFFECT_ATTACH_ORBIT;
			eCount = 3;
			orbit_type = 1;
			break;
			
		case EFFECTSTATUS_SPIRIT_GUARD_4 :
			eType = MEffect::EFFECT_ATTACH_ORBIT;
			eCount = 4;
			orbit_type = 1;
			break;

		case EFFECTSTATUS_HALLUCINATION:
			{
				if(this == g_pPlayer)
				{
					MEvent event;
					event.eventID = EVENTID_HALLUCINATION;
					event.eventType = EVENTTYPE_EFFECT;
					event.eventFlag = EVENTFLAG_FADE_SCREEN;
					event.eventDelay = delayFrame/g_pClientConfig->FPS*1000;
					event.parameter1 = EFFECTSTATUS_HALLUCINATION;
					event.parameter2 = (30 << 16) | (0 << 8) | 30;	// alpha depth, r, g, b
					g_pEventManager->AddEvent(event);
				}
			}
			break;

		case EFFECTSTATUS_GHOST:
			SetFlyingCreature();
			break;

		case EFFECTSTATUS_LEVELUP_SLAYER:
			delayFrame = 30;
			PlaySound(SOUND_SLAYER_ENCHANT_PFA);
			break;

		case EFFECTSTATUS_DISTANCE_BLITZ:
			delayFrame = 10;
			break;

		case EFFECTSTATUS_LEVELUP_VAMPIRE:
			delayFrame = 24;
			PlaySound(SOUND_SLAYER_ENCHANT_PFA);
			break;

		case EFFECTSTATUS_WATER_SHIELD:
			delayFrame = 12;
			break;

		case EFFECTSTATUS_WARP_BLOOD_BIBLE_FROM_ME :// 195 내가 가지고있던 피의성서가 워프될때 머리에 찍어주는-_- 이펙트			
		case EFFECTSTATUS_WARP_BLOOD_BIBLE_FROM_TILE :		// 196 땅에 떨어져있던 피의성서가 워프될때 그 타일에 찍어주는 이펙트
		case EFFECTSTATUS_SHRINE_GUARD_WARP :			// 187		수호성단에서 워프될때
		case EFFECTSTATUS_SHRINE_HOLY_WARP :			// 188		성지성단에서 워프될때
		case EFFECTSTATUS_WARP_BLOOD_BIBLE_SLAYER:
		case EFFECTSTATUS_WARP_BLOOD_BIBLE_VAMPIRE:
			delayFrame = 14;				
			break;
		case EFFECTSTATUS_WILL_OF_LIFE :
			delayFrame = 84;			
			break;
		case EFFECTSTATUS_ILLUSION_OF_AVENGE :
			delayFrame = 32;
			break;
		case EFFECTSTATUS_DENIAL_MAGIC_DAMAGED :
			delayFrame = 14;
			break;
		case EFFECTSTATUS_BACK_STAB :
		case EFFECTSTATUS_BACK_STAB_2 :
			delayFrame = 12;
			break;
		case EFFECTSTATUS_FIRE_OF_SOUL_STONE :
			delayFrame = 16;
			break;
		case EFFECTSTATUS_SAND_OF_SOUL_STONE :
			delayFrame = 10;
			break;
		case EFFECTSTATUS_HAS_FLAG :
			delayFrame = 0xFFFF;
			break;
		case EFFECTSTATUS_FLAG_INSERT :
			delayFrame = 0xFFFF;
			break;
		case EFFECTSTATUS_STONE_SKIN_DAMAGED :
			delayFrame = 8;
			break;
		case EFFECTSTATUS_HAS_SWEEPER :                       // 292
		case EFFECTSTATUS_HAS_SWEEPER_2 :                     // 293
		case EFFECTSTATUS_HAS_SWEEPER_3 :                     // 294
		case EFFECTSTATUS_HAS_SWEEPER_4 :                     // 295
		case EFFECTSTATUS_HAS_SWEEPER_5 :                     // 296
		case EFFECTSTATUS_HAS_SWEEPER_6 :                     // 297
		case EFFECTSTATUS_HAS_SWEEPER_7 :                     // 298
		case EFFECTSTATUS_HAS_SWEEPER_8 :                     // 299
		case EFFECTSTATUS_HAS_SWEEPER_9 :                     // 300
		case EFFECTSTATUS_HAS_SWEEPER_10 :                    // 301
		case EFFECTSTATUS_HAS_SWEEPER_11 :                    // 302
		case EFFECTSTATUS_HAS_SWEEPER_12 :                    // 303
		case EFFECTSTATUS_KEEP_SWEEPER :                      // 304
		case EFFECTSTATUS_KEEP_SWEEPER_2 :                    // 305
		case EFFECTSTATUS_KEEP_SWEEPER_3 :                    // 306
		case EFFECTSTATUS_KEEP_SWEEPER_4 :                    // 307
		case EFFECTSTATUS_KEEP_SWEEPER_5 :                    // 308
		case EFFECTSTATUS_KEEP_SWEEPER_6 :                    // 309
		case EFFECTSTATUS_KEEP_SWEEPER_7 :                    // 310
		case EFFECTSTATUS_KEEP_SWEEPER_8 :                    // 311
		case EFFECTSTATUS_KEEP_SWEEPER_9 :                    // 312
		case EFFECTSTATUS_KEEP_SWEEPER_10 :                   // 313
		case EFFECTSTATUS_KEEP_SWEEPER_11 :                   // 314
		case EFFECTSTATUS_KEEP_SWEEPER_12 :                   // 315			
		case EFFECTSTATUS_SLAYER_REGEN_ZONE :                 // 316
		case EFFECTSTATUS_VAMPIRE_REGEN_ZONE :                // 317
		case EFFECTSTATUS_OUSTERS_REGEN_ZONE :                // 318
		case EFFECTSTATUS_DEFAULT_REGEN_ZONE :                // 319
		case EFFECTSTATUS_SLAYER_TRYING_1 :                   // 320
		case EFFECTSTATUS_SLAYER_TRYING_2 :                   // 321
		case EFFECTSTATUS_SLAYER_TRYING_3 :                   // 322
		case EFFECTSTATUS_VAMPIRE_TRYING_1 :                  // 323
		case EFFECTSTATUS_VAMPIRE_TRYING_2 :                  // 324
		case EFFECTSTATUS_VAMPIRE_TRYING_3 :                  // 325
		case EFFECTSTATUS_OUSTERS_TRYING_1 :                  // 326
		case EFFECTSTATUS_OUSTERS_TRYING_2 :                  // 327
		case EFFECTSTATUS_OUSTERS_TRYING_3 :                  // 328
			delayFrame = 0xFFFF;
			break;
		case EFFECTSTATUS_CAPTURE_REGEN_ZONE :                // 331
			delayFrame = 36;
			break;

		// 2004, 6 ,12 sobeit add start - 130,150 마스터 이펙트시 그전 마스터 이펙트 지워준다.
		case EFFECTSTATUS_GRAND_MASTER_SLAYER:
		case EFFECTSTATUS_GRAND_MASTER_VAMPIRE:
		case EFFECTSTATUS_GRAND_MASTER_OUSTERS:
		case EFFECTSTATUS_GRAND_MASTER_SLAYER_130:
		case EFFECTSTATUS_GRAND_MASTER_SLAYER_150:
		case EFFECTSTATUS_GRAND_MASTER_VAMPIRE_130:
		case EFFECTSTATUS_GRAND_MASTER_VAMPIRE_150:
		case EFFECTSTATUS_GRAND_MASTER_OUSTERS_130:
		case EFFECTSTATUS_GRAND_MASTER_OUSTERS_150:
			{
				delayFrame = 0xFFFF;
				type = GetMasterEffectType(status);
				if(0 == type || IsDead()) 
					return false;
			}
			break;
		// 2004, 6 ,12 sobeit add end - 130,150 마스터 이펙트시 그전 마스터 이펙트 지워준다.
//		case EFFECTSTATUS_GDR_FLOATING:
//			//SetAction(ACTION_DRAINED);
//		//	m_MoveAction = ACTION_DRAINED;
//			if(GetCreatureType() == 717) // 질드레 일때
//			{ 
//				
//			}
//			break;
		case EFFECTSTATUS_BLINDNESS:
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_BLINDNESS,GetX(), GetY(), 0,GetDirection(),	GetID(),	
								GetX(), GetY(), 0, delayFrame, NULL, false);
			break;	
		case EFFECTSTATUS_GLACIER:
			if(IsAlive() /*&& !HasEffectStatus(EFFECTSTATUS_GLACIER)*/)
			{
				
				SetAction(ACTION_DRAINED);
				m_ActionCount = 0;
				SetCauseCriticalWounds(delayFrame);
			//	if(!HasEffectStatus(EFFECTSTATUS_GLACIER))
				{
					if(IsSlayer())
						ExecuteActionInfoFromMainNode(SKILL_CLIENT_GDR_GLACIER_SLAYER,GetX(), GetY(), 0,GetDirection(),	GetID(),	
										GetX(), GetY(), 0, max(0, delayFrame-20), NULL, false);
					else if(IsVampire())
						ExecuteActionInfoFromMainNode(SKILL_CLIENT_GDR_GLACIER_VAMPIRE,GetX(), GetY(), 0,GetDirection(),	GetID(),	
										GetX(), GetY(), 0, max(0, delayFrame-20), NULL, false);
					else if(IsOusters())
						ExecuteActionInfoFromMainNode(SKILL_CLIENT_GDR_GLACIER_OUSTERS,GetX(), GetY(), 0,GetDirection(),	GetID(),	
										GetX(), GetY(), 0, max(0, delayFrame-20), NULL, false);
				}
			}
		//	m_bEffectStatus[status] = true;
		//	return true;
			break;
		case EFFECTSTATUS_SUMMON_GDR:
			delayFrame = 34;
			break;
//		case EFFECTSTATUS_ICICLE_DROP:
//			delayFrame = 16;
//			break;
//		case EFFECTSTATUS_ICICLE_AUGER:
//			delayFrame = 14;
//			break;
//		case EFFECTSTATUS_ICICLE_LARGE_AUGER:
//			delayFrame = 12;
//			break;
		
		case EFFECTSTATUS_GOLD_MEDAL:
			delayFrame = 21;
			break;
		case EFFECTSTATUS_SIEGE_DEFENDERL:
		case EFFECTSTATUS_SIEGE_REINFORCE:
		case EFFECTSTATUS_SIEGE_ATTACKER_1:
		case EFFECTSTATUS_SIEGE_ATTACKER_2:
		case EFFECTSTATUS_SIEGE_ATTACKER_3:
		case EFFECTSTATUS_SIEGE_ATTACKER_4:
		case EFFECTSTATUS_SIEGE_ATTACKER_5:
			SetSiegeWar(status - EFFECTSTATUS_SIEGE_DEFENDERL + 1);
			break;
		case EFFECTSTATUS_REPUTO_FACTUM:
			delayFrame = 0;
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_HIT_REPUTO_FACTUM, GetX(), GetY(), 0,GetDirection(),	GetID(),	
								GetX(), GetY(), 0, 8, NULL, false);
			break;
		case EFFECTSTATUS_BURNING_SOL_CHARGE_1:
			SetAction(ACTION_SLAYER_BLADE);
			SetBurningSol(0);
			break;
		case EFFECTSTATUS_BURNING_SOL_CHARGE_2:
//			if(HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1))
//			{
//				MCreature::RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1);
//			}
//			else
			if(!HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1))
			{
				SetAction(ACTION_SLAYER_BLADE);
				SetBurningSol(0);
			}
			
			break;
		case EFFECTSTATUS_BURNING_SOL_CHARGE_3:
//			if(HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2))
//			{
//				MCreature::RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1);
//				MCreature::RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2);
//			}
//			else
			if(!HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1)&&
				!HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2))
			{
				SetAction(ACTION_SLAYER_BLADE);
				SetBurningSol(0);
			}
			break;
		case EFFECTSTATUS_BURNING_SOL_CHARGE_4:
//			if(HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_3))
//			{
//				MCreature::RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1);
//				MCreature::RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2);
//				MCreature::RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_3);
//			}
//			else
			if(!HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1)&&
				!HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2)&&
				!HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_3))
			{
				SetAction(ACTION_SLAYER_BLADE);
				SetBurningSol(0);
			}
			break;
		case EFFECTSTATUS_INSTALL_TURRET: // 총슬 130 인스톨 터렛
			{
				SetAction( ACTION_STAND );
				SetInstallTurretCount(0);
				SetInstallTurretDirect(2);
//				SetCreatureType( CREATURETYPE_INSTALL_TURRET );
//				SetGroundCreature();
//				
				SetDirection( 2 );
				SetCurrentDirection( 2 );
//				//--------------------------------------------------
//				// 늑대로 변신하는 결과
//				//--------------------------------------------------
//				MActionResult* pResult = new MActionResult;
//
//				pResult->Add( new MActionResultNodeChangeCreatureType( GetID(), CREATURETYPE_INSTALL_TURRET ) );

//					
//				ExecuteActionInfoFromMainNode(
//					RESULT_SKILL_INSTALL_TURRET,										// 사용 기술 번호
//				
//					GetX(), GetY(), 0,
//					GetDirection(),														// 사용 방향
//					
//					OBJECTID_NULL,												// 목표에 대한 정보
//					GetX(), GetY(), 0, 
//					
//					0,													// 기술의 (남은) 지속 시간		
//					
//					NULL, //NULL,
//					
//					false);			// 기술 첨부터 시작한다.
			}
			break;
			// 2004, 10, 25, sobeit add start
		case EFFECTSTATUS_DRAGON_EYES:
			delayFrame = 0xffff;
			break;
			// 2004, 10, 25, sobeit add end

		case EFFECTSTATUS_SET_AFIRE_ATTACK:
			delayFrame = 20;
			break;

		case EFFECTSTATUS_ICE_HORIZON:
				delayFrame = 22;
			break;
			
		case EFFECTSTATUS_FIERCE_FLAME:
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_FIERCE_FLAME,GetX(), GetY(), 0,GetDirection(),	GetID(),	
					GetX(), GetY(), 0, delayFrame, NULL, false);
			break;
		// add by Coffee 2007-5-3 삽랬	
		case EFFECTSTATUS_DUMMY_DRAKE:
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_DUMMY_DRAKE,GetX(), GetY(), 0,GetDirection(),	GetID(),	
					GetX(), GetY(), 0, delayFrame, NULL, false);
			break;
		case EFFECTSTATUS_HYDRO_CONVERGENCE: //彊랬
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_HYDRO_CONVERGENCE,GetX(), GetY(), 0,GetDirection(),	GetID(),	
					GetX(), GetY(), 0, delayFrame, NULL, false);
			break;
		case EFFECTSTATUS_HETER_CHAKRAM: //츌濫
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_SKILL_DUMMY_DRAKE,GetX(), GetY(), 0,GetDirection(),	GetID(),	
					GetX(), GetY(), 0, delayFrame, NULL, false);
			break;	
		case EFFECTSTATUS_BLOOD_CURSE: // 俱沂뱁세콘
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_BLOOD_CURSE,GetX(), GetY(), 0,GetDirection(),	GetID(),	
					GetX(), GetY(), 0, delayFrame, NULL, false);
			break;
		// end 	
		case EFFECTSTATUS_PASSING_HEAL:
				delayFrame = 20;
			break;
		case EFFECTSTATUS_DONATION_200501:
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_CANDLE,GetX(), GetY(), 0,GetDirection(),	GetID(),	
								GetX(), GetY(), 0, delayFrame, NULL, false);
			break;	
			
	}

	m_bEffectStatus[status] = true;

	bool bUseEffectSprite = (*g_pEffectStatusTable)[status].bUseEffectSprite;	

	//------------------------------------------------------------
	//
	//			EffectSprite로 표현하는 경우
	//
	//------------------------------------------------------------		
	if (bUseEffectSprite)
	{
		if( (status == EFFECTSTATUS_GRAND_MASTER_OUSTERS ||
			status == EFFECTSTATUS_GRAND_MASTER_SLAYER ||
			status == EFFECTSTATUS_GRAND_MASTER_VAMPIRE ) &&
			!g_pSystemAvailableManager->IsAvailableGrandMasterEffectSystem() )
			return false;

		DEBUG_ADD("[Use EffectSprite]");
		
		//TYPE_EFFECTSPRITETYPE type = (*g_pEffectStatusTable)[status].EffectSpriteType;

		if (type>=g_pEffectSpriteTypeTable->GetSize())//EFFECTSPRITETYPE_NULL)
		{
			DEBUG_ADD_FORMAT("[Error]EffectSpriteType of EffectStatus exceed Max : %d", (int)type);
			
			return false;
		}

		if (IsFemale()
			&& (*g_pEffectSpriteTypeTable)[type].FemaleEffectSpriteType!=EFFECTSPRITETYPE_NULL)
		{
			type = (*g_pEffectSpriteTypeTable)[type].FemaleEffectSpriteType;
		}

//		//------------------------------------------------------------
//		// EFFECTSTATUS_ARMAGEDDON
//		//------------------------------------------------------------
//		if (status==EFFECTSTATUS_ARMAGEDDON)
//		{
//			type = GetArmageddonSprite( this );			
//		}

		TYPE_FRAMEID	frameID = (*g_pEffectSpriteTypeTable)[type].FrameID;

		if (frameID==FRAMEID_NULL)
		{
			DEBUG_ADD("[Error]FrameID of EffectSpriteType is NULL");
			
			return false;
		}

		//------------------------------------------------------------
		// 이미 있는 경우 
		//------------------------------------------------------------
		if (m_bAttachEffect[type])
		{		
			ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

			//------------------------------------------------------
			// 몸에 붙는거
			//------------------------------------------------------
			// 같은 Effect를 찾는다. 
			while (iEffect != m_listEffect.end())
			{
				MAttachEffect*	pEffect = *iEffect;
				
				// 같은 type을 찾는다.
				if (pEffect->IsEffectSprite() && pEffect->GetEffectSpriteType() == type)
				{
					// 추가할려는 것이 더 늦게 끝날 경우에만 시간을 확장한다.
					if (g_CurrentFrame+delayFrame > pEffect->GetEndFrame()) 
					{
						pEffect->SetCount( delayFrame );
					}

					return true;
				}

				iEffect++;
			}

			//------------------------------------------------------
			// 바닥에 붙는 거
			//------------------------------------------------------
			iEffect = m_listGroundEffect.begin();

			// 같은 Effect를 찾는다. 
			while (iEffect != m_listGroundEffect.end())
			{
				MAttachEffect*	pEffect = *iEffect;
				
				// 같은 type을 찾는다.
				if (pEffect->IsEffectSprite() && pEffect->GetEffectSpriteType() == type)
				{
					// 추가할려는 것이 더 늦게 끝날 경우에만 시간을 확장한다.
					if (g_CurrentFrame+delayFrame > pEffect->GetEndFrame()) 
					{
						pEffect->SetCount( delayFrame );
					}

					return true;
				}

				iEffect++;
			}

			return false;
		}

		//------------------------------------------------------------
		// 없는 경우 추가한다.
		//------------------------------------------------------------
		MAttachEffect*	pEffect = NULL;
		
		if (eType==MEffect::EFFECT_ATTACH_ORBIT)
		{
			for(int i = 0; i < eCount; i++)
			{
				int effectPosition = 0;
				if( eCount > 1 )
				{
					effectPosition = (MAX_EFFECT_ORBIT_STEP/(eCount*2))*i;
				}
				else
				{
					effectPosition = -1;
				}
				pEffect = new MAttachOrbitEffect(type, delayFrame, orbit_type, effectPosition);
				
				pEffect->SetAttachCreature( this );
				
				// 빛나는 Effect의 개수를 증가?
				if (pEffect->GetBltType()==BLT_EFFECT)
				{
					// 최초로 추가되는 빛나는Effect이면 시야를 추가시킨다.
					//if (m_nAlphaEffect==0)
					{
						pEffect->SetPosition( m_X, m_Y );
						//g_pZone->SetLight(m_X, m_Y, pEffect->GetLight());
					}
					
					//m_nAlphaEffect ++;		
				}
				
				//------------------------------------------------------------
				// 바닥에 붙는 거라면..
				//------------------------------------------------------------
				if ((*g_pEffectStatusTable)[status].bAttachGround)
				{
					m_listGroundEffect.push_back( pEffect );
				}
				//------------------------------------------------------------
				// 몸에 붙는 거..
				//------------------------------------------------------------
				else
				{
					m_listEffect.push_back( pEffect );
				}
				
				m_bAttachEffect[type] = true;
				
			}
		}
		else
		{
			pEffect = new MAttachEffect(type, delayFrame);
			
			pEffect->SetAttachCreature( this );
			
			// 빛나는 Effect의 개수를 증가?
			if (pEffect->GetBltType()==BLT_EFFECT)
			{
				// 최초로 추가되는 빛나는Effect이면 시야를 추가시킨다.
				//if (m_nAlphaEffect==0)
				{
					pEffect->SetPosition( m_X, m_Y );
					//g_pZone->SetLight(m_X, m_Y, pEffect->GetLight());
				}
				
				//m_nAlphaEffect ++;		
			}
			
			//------------------------------------------------------------
			// 바닥에 붙는 거라면..
			//------------------------------------------------------------
			if ((*g_pEffectStatusTable)[status].bAttachGround)
			{
				m_listGroundEffect.push_back( pEffect );
			}
			//------------------------------------------------------------
			// 몸에 붙는 거..
			//------------------------------------------------------------
			else
			{
				m_listEffect.push_back( pEffect );
			}
			
			m_bAttachEffect[type] = true;
			
		}
	

		// Set LightSight
		/*
		#ifdef	OUTPUT_DEBUG
			sprintf(g_pDebugMessage->GetCurrent(), "NewEffect : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), pEffect->GetLight(), pEffect->GetX(), pEffect->GetY());	
			g_pDebugMessage->Next();
		#endif
		*/
	}
	//------------------------------------------------------------
	//
	//			EffectColor로 표현하는 경우
	//
	//------------------------------------------------------------
//	else
	if((*g_pEffectStatusTable)[status].EffectColor != 0xFFFF)
	{
		DEBUG_ADD("[Use EffectColor]");
		
		WORD colorSet = (*g_pEffectStatusTable)[status].EffectColor;

		if (colorSet>=MAX_COLORSET)
		{
			DEBUG_ADD_FORMAT("[Error]EffectColor of EffectStatus exceed Max : ", (int)colorSet);
			
			return false;
		}

		//------------------------------------------------------------
		// 이미 있는지 체크한다.
		// 색깔로 체크해야되므로.. list를 다 검색해야 한다. T_T;
		//------------------------------------------------------------
		DEBUG_ADD_FORMAT("listEffect size = %d", m_listEffect.size());
		
		ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

		//----------------------------------------------------------------
		// 몸에 붙는거
		//----------------------------------------------------------------
		// 같은 Effect를 찾는다. 
		while (iEffect != m_listEffect.end())
		{
			MAttachEffect*	pEffect = *iEffect;
			
			// 같은 색깔을 찾는다.
			if (pEffect->IsEffectColor() && pEffect->GetEffectColor() == colorSet)
			{
				// 추가할려는 것이 더 늦게 끝날 경우에만 시간을 확장한다.
				if (g_CurrentFrame+delayFrame > pEffect->GetEndFrame()) 
				{
					pEffect->SetCount( delayFrame );
				}

				return true;
			}

			iEffect++;
		}

		//----------------------------------------------------------------
		// 바닥에 붙는거
		//----------------------------------------------------------------
		// 없다고 본다.

		DEBUG_ADD("[Create New AttachEffect]");
		
		//------------------------------------------------------------
		// 없는 경우 추가한다.
		//------------------------------------------------------------
		MAttachEffect*	pEffect = new MAttachEffect(EFFECTSPRITETYPE_NULL, delayFrame);

		DEBUG_ADD("[Create New AttachEffect OK]");
		
		pEffect->SetAttachCreature( this );

		DEBUG_ADD("[Set AttachCreature]");
		
		// 색깔 설정
		pEffect->SetEffectColor( colorSet );

		ADDON	part = (*g_pEffectStatusTable)[status].EffectColorPart;

		//------------------------------------------------------------
		// 특정한 부위만 색깔이 바뀌는 경우도 있다.
		//------------------------------------------------------------
		//if (part<ADDON_MAX)
		//{
			pEffect->SetEffectColorPart( part );
		//}

		DEBUG_ADD("[if (pEffect->GetBltType()==BLT_EFFECT)]");
		
		// 빛나는 Effect의 개수를 증가?
		if (pEffect->GetBltType()==BLT_EFFECT)
		{
			// 최초로 추가되는 빛나는Effect이면 시야를 추가시킨다.
			//if (m_nAlphaEffect==0)
			{
				pEffect->SetPosition( m_X, m_Y );
				//g_pZone->SetLight(m_X, m_Y, pEffect->GetLight());
			}

			//m_nAlphaEffect ++;		
		}

		//------------------------------------------------------
		// 색깔 변하는거는 그냥 몸에 붙이는거에 추가한다.
		//------------------------------------------------------
		m_listEffect.push_back( pEffect );				
	}

	DEBUG_ADD("[AddEffectStatus - OK]");
	
	return true;
}

//----------------------------------------------------------------------
// Set Name - 캐릭터 이름 설정
//----------------------------------------------------------------------
void	
MCreature::SetName(const char* pName)
{
	if (m_pName!=NULL)
	{
		delete [] m_pName;
	}

	m_pName = new char [strlen(pName) + 1];

	strcpy(m_pName, pName);
}

//----------------------------------------------------------------------
// Is Stop
//----------------------------------------------------------------------
BOOL	
MCreature::IsStop()
{ 
	return m_MoveCount>=m_MoveCountMax 
			&& m_ActionCount>=GetActionCountMax() 
			|| m_Action==ACTION_STAND && m_MoveAction==ACTION_MOVE
			|| m_Action==ACTION_SLAYER_MOTOR_STAND && m_MoveAction==ACTION_SLAYER_MOTOR_MOVE
			|| m_Action==ACTION_OUSTERS_FAST_MOVE_STAND && m_MoveAction==ACTION_OUSTERS_FAST_MOVE_MOVE;
}

//----------------------------------------------------------------------
// Set Server Position
//----------------------------------------------------------------------
void			
MCreature::SetServerPosition(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)	
{ 
	//------------------------------------------------
	// 기존의 MoveBuffer를 다 지움
	//------------------------------------------------
	ReleaseMoveBuffer();

	SetServerPositionWithBlock(sX, sY);
}

//----------------------------------------------------------------------
// Is Slayer Character
//----------------------------------------------------------------------
// slayer 그림을 사용했나?
//----------------------------------------------------------------------
bool			
MCreature::IsSlayerCharacter() const
{
	return (*g_pCreatureTable)[m_CreatureType].SpriteTypes[0] <= 1;
	//return m_CreatureType==CREATURETYPE_SLAYER_MALE
	//	||  m_CreatureType==CREATURETYPE_SLAYER_FEMALE;
}

//----------------------------------------------------------------------
// 움직이는 방법과 그에 따른 이동단위를 결정한다.
//----------------------------------------------------------------------
void		
MCreature::SetMoveDevice(MOVE_DEVICE md)
{	
	//--------------------------------------------------------
	// 모두 적용
	//--------------------------------------------------------
	AffectMoveBufferAll();

	//--------------------------------------------------
	// Slayer인 경우만 변경이 가능하다.
	//--------------------------------------------------
	if (IsSlayer())
	{
		m_MoveDevice = md; 

//		SetAction(ACTION_STAND);
		switch (md)
		{
			case MOVE_DEVICE_WALK :
			{
				m_MoveAction = ACTION_MOVE;

				if (m_Action==ACTION_SLAYER_MOTOR_STAND)
				{
					SetAction( ACTION_STAND );
				}				
			}
			break;

			case MOVE_DEVICE_RIDE :
			{
				m_MoveAction = ACTION_SLAYER_MOTOR_MOVE;

				if (m_Action==ACTION_STAND)
				{
					SetAction( ACTION_SLAYER_MOTOR_STAND );
				}
			}
			break;
		}	
	}
	//--------------------------------------------------
	// 아닌 경우..
	//--------------------------------------------------
	else if( IsOusters() )
	{
		m_MoveDevice = md; 
		if(m_MoveDevice==MOVE_DEVICE_SUMMON_SYLPH)
		{	
			m_MoveAction = ACTION_OUSTERS_FAST_MOVE_MOVE;
		}
		else
		{
			m_MoveAction = ACTION_MOVE;
		}
		
		SetAction(ACTION_STAND);
	} else
	{
		m_MoveDevice = MOVE_DEVICE_WALK;
		m_MoveAction = ACTION_MOVE;
	}
}

//----------------------------------------------------------------------
// Creature의 위치를 Set하고 움직이던 동작을 멈춘다.
//----------------------------------------------------------------------
void	
MCreature::SetPosition(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)	
{ 
	//------------------------------------------------
	// 기존의 MoveBuffer를 다 지움
	//------------------------------------------------
	ReleaseMoveBuffer();

	if (m_X!=SECTORPOSITION_NULL
		&& m_Y!=SECTORPOSITION_NULL)
	{
		MovePosition( x, y );
	}
	else
	{
		m_X	= x; 
		m_Y	= y; 	
	}

	m_sX = 0;
	m_sY = 0;
	m_ActionCount		= 0;
	m_ActionCountMax	= 0;
	m_MoveCount			= 0;
	m_MoveCountMax		= 0;

	CheckInDarkness();	
}

//----------------------------------------------------------------------
// Set Weapon Speed
//----------------------------------------------------------------------
void				
MCreature::SetWeaponSpeed(int speed)
{
	//-----------------------------------------------------------
	// 박쥐나 늑대면 공격속도 따로 없당..
	//-----------------------------------------------------------
	if (m_CreatureType==CREATURETYPE_BAT ||
		m_CreatureType==CREATURETYPE_WOLF ||
		m_CreatureType==CREATURETYPE_WER_WOLF)
	{
		m_WeaponSpeed = WEAPON_SPEED_NORMAL;
	}
	//-----------------------------------------------------------
	// 공격 속도 계산..
	//-----------------------------------------------------------
	else
	{
		if (speed <= 33)
		{
			m_WeaponSpeed = WEAPON_SPEED_SLOW;
		}
		else if (speed <= 66)
		{
			m_WeaponSpeed = WEAPON_SPEED_NORMAL;
		}
		else
		{
			m_WeaponSpeed = WEAPON_SPEED_FAST;
		}
	}
	if(GetClassType() == CLASS_PLAYER)
	{
		UI_SetWeaponSpeed(speed, m_WeaponSpeed);
	}
}

//----------------------------------------------------------------------
// Creature를 멈추게 한다.
//----------------------------------------------------------------------
void
MCreature::SetStop()
{ 
	
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

	int action = (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_MOVE : ACTION_STAND;
	if( IsOusters() && HasEffectStatusSummonSylph( this ))
	{
		if (action==ACTION_STAND)
		{
			action = ACTION_OUSTERS_FAST_MOVE_STAND;
		} 
		else if( action == ACTION_MOVE )
		{
			action = ACTION_OUSTERS_FAST_MOVE_MOVE;
		}
	}
	m_Action		= action;
	// 2004, 11, 3, sobeit modify start - m_ActionCount를 바뀐 액션의 맥스 카운트로 세팅 했음 - 아우스터즈 고스트 버그 땜시..ㅡㅡ;
	m_ActionCount	= GetActionCountMax(); 
	//m_ActionCount	= (*g_pCreatureTable)[m_CreatureType].GetActionCount( m_Action );
	// 2004, 11, 3, sobeit modify end
	m_MoveCount		= m_MoveCountMax;

	// 다음 동작도 없앰
	m_bNextAction = false;
	m_NextX = SECTORPOSITION_NULL;
	m_NextY = SECTORPOSITION_NULL;

	
	//m_nNextUsedActionInfo = ACTIONINFO_NULL;
}

//----------------------------------------------------------------------
// Get PixelX - Creature의 PixelX좌표
//----------------------------------------------------------------------
int			
MCreature::GetPixelX() const
{ 
	return MTopView::MapToPixelX(m_X) + m_sX;
}

//----------------------------------------------------------------------
// Get PixelY - Creature의 PixelY좌표
//----------------------------------------------------------------------
int			
MCreature::GetPixelY() const			
{ 
	return MTopView::MapToPixelY(m_Y) + m_sY;// + ((m_MoveType == CREATURE_FLYING)<<2);
}

//----------------------------------------------------------------------
// Set Level Name
//----------------------------------------------------------------------
void				
MCreature::SetLevelName(int ln)
{
	// slayer이면 기술 레벨에 따라서...

	// vampire이면 레벨에 따라서?

	// 
	m_LevelName = rand() % g_pLevelNameTable->GetSize();
}

//----------------------------------------------------------------------
// Get Level Name
//----------------------------------------------------------------------
const char*			
MCreature::GetLevelName() const
{
	return (*g_pLevelNameTable)[m_LevelName].GetString();
}

//----------------------------------------------------------------------
// Move Position
//----------------------------------------------------------------------
// Zone에서 (m_X, m_Y) Sector에 있는 
// this Creature를 (x, y)로 옮겨야 한다.
//----------------------------------------------------------------------
bool
MCreature::MovePosition(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)
{
	if (m_pZone==NULL)
	{
		return false;
	}

	// Creature Type에 따라서 다르다.			
	switch (m_MoveType)
	{
		case CREATURE_UNDERGROUND : 
			if (m_pZone->MoveUndergroundCreature(this, m_X, m_Y, x, y))
			{
				// Effect가 있을 경우
				/*
				if (m_nAlphaEffect)
				{
					// 시야를 바꿔준다.
					g_pZone->UnSetLightSight(m_X, m_Y, 0);
					// 시야를 바꿔준다.
					g_pZone->SetLightSight(x, y, 0);
				}
				*/

				// 이동했으면 새로운 좌표 설정
				m_X	= x;
				m_Y	= y;		
				
				CheckInDarkness();
				
				return true;
			}
		break;

		case CREATURE_GROUND : 
			if (m_pZone->MoveGroundCreature(this, m_X, m_Y, x, y))
			{
				// Effect가 있을 경우
				/*
				if (m_nAlphaEffect)
				{
					// 시야를 바꿔준다.
					g_pZone->UnSetLightSight(m_X, m_Y, 0);
					// 시야를 바꿔준다.
					g_pZone->SetLightSight(x, y, 0);
				}
				*/

				// 이동했으면 새로운 좌표 설정
				m_X	= x;
				m_Y	= y;

				CheckInDarkness();

				return true;
			}
		break;

		case CREATURE_FLYING : 
			if (m_pZone->MoveFlyingCreature(this, m_X, m_Y, x, y))
			{
				// Effect가 있을 경우
				/*
				if (m_nAlphaEffect)
				{
					// 시야를 바꿔준다.
					g_pZone->UnSetLightSight(m_X, m_Y, 0);
					// 시야를 바꿔준다.
					g_pZone->SetLightSight(x, y, 0);
				}
				*/

				// 이동했으면 새로운 좌표 설정
				m_X	= x;
				m_Y	= y;

				CheckInDarkness();

				return true;
			}
		break;

		// fake는 그냥 움직인다.
		case CREATURE_FAKE_NO_BLOCK :
		case CREATURE_FAKE_UNDERGROUND :
		case CREATURE_FAKE_GROUND :
		case CREATURE_FAKE_FLYING :
			m_X = x;
			m_Y = y;
		return true;
		break;
	}

	return false;
}

//----------------------------------------------------------------------
// (x,y)에서 방향으로 이동한 위치를 얻는다.
//----------------------------------------------------------------------
void
MCreature::GetPositionToDirection(TYPE_SECTORPOSITION &x, TYPE_SECTORPOSITION &y, BYTE direction)
{
	switch (direction)
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

//----------------------------------------------------------------------
// Add Effect
//----------------------------------------------------------------------
// Creature에 달라붙어서 표현되는 Effect 표현
//
// 중첩을 허용하지 않으므로.. 
// 아직 붙어서 표현되지 않은 Effect만 새로 추가한다.
//
// 그러나, 시간 확장을 해야하는 경우가 생긴다.
//----------------------------------------------------------------------
MAttachEffect*		
MCreature::CreateAttachEffect(TYPE_EFFECTSPRITETYPE type, 
							  DWORD delayFrame, 
							  DWORD linkDelayFrame, 
							  BOOL bGroundEffect,
							  MEffect::EFFECT_TYPE eType)
{
	DEBUG_ADD_FORMAT("CreateAttachEffect. type=%d, delayf=%d", type, delayFrame);

	// 죽은 경우... delay를 없앤다. 
	if (!m_bAlive)
	{
		// - -;;
		delayFrame = min( 10, delayFrame );
	}
	bool	IsMulti = false;
	int		orbitType = 0;
	// 2004, 08, 05 sobeit add start - 성문, 길드 타워 파편튀기
	if(GetCreatureType() >= 726 && GetCreatureType() <= 729)
	{	
		if(type < EFFECTSPRITETYPE_CASTLE_GATE_DUST_1 ||
		   type > EFFECTSPRITETYPE_CASTLE_GATE_DAMAGED) // 성문 관련 이펙트가 아니면 무저건 파편튀기..ㅎㅎ
		{
			int currentHP		= max(0,int(GetHP()));
			int frame = 4 -(currentHP*5/GetMAX_HP());
			
			if(m_NickNameType != frame) // 변수 추가하긴 그렇고..성문은 m_NickNameType을 안쓰기 때문에 요기서 Temporary하게 쓰자-_-; 
			{
				m_NickNameType = frame;
				if(frame<1) frame = 1;
				type = EFFECTSPRITETYPE_CASTLE_GATE_DUST_1 + rand()%(5-frame);
				delayFrame = 32;
			}
			else
			{
				if(frame<1) frame = 1;
				type = EFFECTSPRITETYPE_CASTLE_GATE_PIECE_1 + rand()%(5-frame);
				delayFrame = 14;
			}
			// 성문 타격 이펙트 인데.. 없는게 나을듯..ㅎ
//			ExecuteActionInfoFromMainNode(SKILL_CLIENT_CASTLE_GATE_DAMAGED,GetX(), GetY(), 0,GetDirection(),	GetID(),	
//				GetX(), GetY(), 0, 8, NULL, false);
		}
		else
		{
			if(type == EFFECTSPRITETYPE_CASTLE_GATE_LARGE_DUST)
				delayFrame = 44;
			else if(type == EFFECTSPRITETYPE_CASTLE_GATE_DAMAGED)
				delayFrame = 8;
			else 
				delayFrame = 32;
		}
	}
	else if(GetCreatureType() == 734) // 길드 타워
	{
		if(type < EFFECTSPRITETYPE_CASTLE_GATE_DAMAGED ||
		   type > EFFECTSPRITETYPE_GUILD_TOWER_PROTECT) // 길드 타워관련 이펙트가 아니면 무저건 파편튀기..ㅎㅎ
		{
			type = EFFECTSPRITETYPE_CASTLE_GATE_DAMAGED;
			delayFrame = 8;
//			ExecuteActionInfoFromMainNode(SKILL_CLIENT_GUILD_TOWER_PROTECT,GetX(), GetY(), 0,GetDirection(),	GetID(),	
//				GetX(), GetY(), 0, 32, NULL, false);
		}
		else
		{
			delayFrame = 32;
		}
	}
//	else if(GetCreatureType() == 723) // 각성 질드레
//	{
//		if(type == EFFECTSPRITETYPE_GDR_DEAD)
//			delayFrame = 218;
//
//	}
	else
	{
//		if(GetCreatureType() == 734&&type == EFFECTSPRITETYPE_GUILD_TOWER_DESTROY) // 길드 타워
//			delayFrame = 32;
	// 2004, 08, 05 sobeit add end - 성문, 길드 타워 파편튀기

		//------------------------------------------------
		// 피인 경우는 위치를 random하게
		//------------------------------------------------
		if (type==EFFECTSPRITETYPE_BLOOD_GUN_1_1
			|| type==EFFECTSPRITETYPE_BLOOD_GUN_2_1
			|| type==EFFECTSPRITETYPE_BLOOD_GUN_3_1
			|| type==EFFECTSPRITETYPE_GREEN_BLOOD_GUN_1_1
			|| type==EFFECTSPRITETYPE_GREEN_BLOOD_GUN_2_1
			|| type==EFFECTSPRITETYPE_GREEN_BLOOD_GUN_3_1)
		{
			// 키에 따라서.. 100넘으면... (골레머)
			if ((*g_pCreatureTable)[m_CreatureType].Height > 100)
			{
				// type + 0~6
				type += rand()%7;
			}
			else
			{
				// type + 1~6
				type += rand()%6 + 1;
			}		
		}

		//------------------------------------------------
		// 피인 경우는 위치를 random하게
		//------------------------------------------------
		if (type==EFFECTSPRITETYPE_BULLET_OF_LIGHT_START_1)
		{
			// 키에 따라서.. 100넘으면... (골레머)
			if ((*g_pCreatureTable)[m_CreatureType].Height > 100)
			{
				// type + 0~6
				type += (rand()%7)*2;
			}
			else
			{
				// type + 1~6
				type += (rand()%6)*2;
			}		
		}
		//------------------------------------------------
		// 칼 맞은 피 (세로)
		//------------------------------------------------
		if (type==EFFECTSPRITETYPE_BLOOD_VERTICAL_1 
			|| type==EFFECTSPRITETYPE_BLOOD_HORIZONTAL_1)
		{
			if (rand()%2)
			{
				type = EFFECTSPRITETYPE_BLOOD_VERTICAL_1 + rand()%4;
			}
			else
			{
				type = EFFECTSPRITETYPE_BLOOD_HORIZONTAL_1 + rand()%3;
			}		
		}
		if (type==EFFECTSPRITETYPE_GREEN_BLOOD_VERTICAL_1 
			|| type==EFFECTSPRITETYPE_GREEN_BLOOD_HORIZONTAL_1)
		{
			if (rand()%2)
			{
				type = EFFECTSPRITETYPE_GREEN_BLOOD_VERTICAL_1 + rand()%4;
			}
			else
			{
				type = EFFECTSPRITETYPE_GREEN_BLOOD_HORIZONTAL_1 + rand()%3;
			}		
		}
		//------------------------------------------------
		// 산에 맞은 smoke인 경우
		//------------------------------------------------
		else if (type==EFFECTSPRITETYPE_SMOKE)
		{
			type += rand()%5;
		}
		
		//------------------------------------------------
		// Typhoon back [새기술]
		//------------------------------------------------
		else if (type==EFFECTSPRITETYPE_TYPHOON_BACK)
		{	
			SetTurning( 16 );
		}


		if (type>=g_pEffectSpriteTypeTable->GetSize())//EFFECTSPRITETYPE_NULL)
		{
			DEBUG_ADD_FORMAT("[Error]EffectSpriteType of EffectStatus exceed Max : ", (int)type);
			
			return NULL;
		}

		//------------------------------------------------
		// 여자용 EffectSpriteType을 사용하는 경우
		//------------------------------------------------
		if (IsFemale()
			&& (*g_pEffectSpriteTypeTable)[type].FemaleEffectSpriteType!=EFFECTSPRITETYPE_NULL)
		{
			type = (*g_pEffectSpriteTypeTable)[type].FemaleEffectSpriteType;

			if (type>=g_pEffectSpriteTypeTable->GetSize())//EFFECTSPRITETYPE_NULL)
			{
				DEBUG_ADD_FORMAT("[Error]EffectSpriteType of EffectStatus exceed Max : ", (int)type);
				
				return NULL;
			}
		}
	

		// 공격 속도에 따른 이펙트를 다르게 구별한다.
		switch( type )
		{
		case EFFECTSPRITETYPE_FLOURISH_NORMAL :
		case EFFECTSPRITETYPE_FLOURISH_NORMAL_2 :
		case EFFECTSPRITETYPE_FLOURISH_NORMAL_3 :
		case EFFECTSPRITETYPE_NEW_BLOODY_NAIL_FEMALE_NORMAL :
		case EFFECTSPRITETYPE_NEW_ACID_TOUCH_FEMALE_NORMAL :	
		case EFFECTSPRITETYPE_NEW_BLOODY_NAIL_MALE_NORMAL :
		case EFFECTSPRITETYPE_NEW_ACID_TOUCH_MALE_NORMAL :	
		case EFFECTSPRITETYPE_REDIANCE_MALE_NORMAL_ATTACK_NORMAL :
		case EFFECTSPRITETYPE_REDIANCE_FEMALE_NORMAL_ATTACK_NORMAL :
		case EFFECTSPRITETYPE_TALON_OF_CROW_MALE_NORMAL :
		case EFFECTSPRITETYPE_TALON_OF_CROW_FEMALE_NORMAL :
			IsMulti = true;
			switch( GetWeaponSpeed() )
			{
			case WEAPON_SPEED_SLOW :
				delayFrame = 15;
				++type;
				break;
			case WEAPON_SPEED_NORMAL :
				delayFrame = 13;
				break;
			case WEAPON_SPEED_FAST :
				delayFrame = 11;
				--type;			
				break;
			}
			break;
		case EFFECTSPRITETYPE_REDIANCE_MALE_NORMAL :
		case EFFECTSPRITETYPE_REDIANCE_FEMALE_NORMAL :
			
	//	case EFFECTSPRITETYPE_THROW_CHACRAM_21:
	//	case EFFECTSPRITETYPE_THROW_CHACRAM_23:
	//	case EFFECTSPRITETYPE_THROW_CHACRAM_25:
			IsMulti = true;
			switch( GetWeaponSpeed() )
			{
			case WEAPON_SPEED_SLOW :
				delayFrame = 25;
				++type;
				break;
			case WEAPON_SPEED_NORMAL :
				delayFrame = 23;
				break;
			case WEAPON_SPEED_FAST :
				delayFrame = 21;
				--type;			
				break;
			}
	//		if(type == EFFECTSPRITETYPE_THROW_CHACRAM_23
	//			|| type == EFFECTSPRITETYPE_THROW_CHACRAM_21
	//			|| type == EFFECTSPRITETYPE_THROW_CHACRAM_25)
	//		{
	//			SetGhost(2, delayFrame-2);
	//			IsMulti = true;
	//		}
			break;

		case EFFECTSPRITETYPE_THROW_CHACRAM_21:
		case EFFECTSPRITETYPE_THROW_CHACRAM_23:
		case EFFECTSPRITETYPE_THROW_CHACRAM_25:
			IsMulti = true;
			switch( GetWeaponSpeed() )
			{
			case WEAPON_SPEED_SLOW :
				delayFrame = 25;
				type = EFFECTSPRITETYPE_THROW_CHACRAM_25;
				break;
			case WEAPON_SPEED_NORMAL :
				delayFrame = 23;
				type = EFFECTSPRITETYPE_THROW_CHACRAM_23;
				break;
			case WEAPON_SPEED_FAST :
				delayFrame = 21;
				type = EFFECTSPRITETYPE_THROW_CHACRAM_21;			
				break;
			}
			//SetGhost(2, delayFrame);
			break;

		case EFFECTSPRITETYPE_THROW_CHACRAM_21_MASTERY:
		case EFFECTSPRITETYPE_THROW_CHACRAM_23_MASTERY:
		case EFFECTSPRITETYPE_THROW_CHACRAM_25_MASTERY:
			IsMulti = true;
			switch( GetWeaponSpeed() )
			{
			case WEAPON_SPEED_SLOW :
				delayFrame = 25;
				type = EFFECTSPRITETYPE_THROW_CHACRAM_25_MASTERY;
				break;
			case WEAPON_SPEED_NORMAL :
				delayFrame = 23;
				type = EFFECTSPRITETYPE_THROW_CHACRAM_23_MASTERY;
				break;
			case WEAPON_SPEED_FAST :
				delayFrame = 21;
				type = EFFECTSPRITETYPE_THROW_CHACRAM_21_MASTERY;
				break;
			}
			SetGhost(2, delayFrame);
			break;

		
		case EFFECTSPRITETYPE_DISTANCE_BLITZ_12:
		case EFFECTSPRITETYPE_DISTANCE_BLITZ_3:
		case EFFECTSPRITETYPE_DISTANCE_BLITZ_4:
		case EFFECTSPRITETYPE_DISTANCE_BLITZ_5:
		case EFFECTSPRITETYPE_DISTANCE_BLITZ_6:
//		case EFFECTSPRITETYPE_THROW_CHACRAM_21:
//		case EFFECTSPRITETYPE_THROW_CHACRAM_23:
//		case EFFECTSPRITETYPE_THROW_CHACRAM_25:
//		case EFFECTSPRITETYPE_THROW_CHACRAM_21_MASTERY:
//		case EFFECTSPRITETYPE_THROW_CHACRAM_23_MASTERY:
//		case EFFECTSPRITETYPE_THROW_CHACRAM_25_MASTERY:
			{
				delayFrame = 25;
				SetGhost(2, delayFrame-2);
				IsMulti = true;
			}
			break;

		case EFFECTSPRITETYPE_PIERCING_FRONT :
		case EFFECTSPRITETYPE_PIERCING_BACK :
		case EFFECTSPRITETYPE_JABBING_VEIN_REAR_1 :
		case EFFECTSPRITETYPE_JABBING_VEIN_REAR_2 :
		case EFFECTSPRITETYPE_JABBING_VEIN_REAR_3 :
		case EFFECTSPRITETYPE_JABBING_VEIN_REAR_4 :
		case EFFECTSPRITETYPE_JABBING_VEIN_FRONT_1 :
		case EFFECTSPRITETYPE_JABBING_VEIN_FRONT_2 :
		case EFFECTSPRITETYPE_JABBING_VEIN_FRONT_3 :
		case EFFECTSPRITETYPE_JABBING_VEIN_FRONT_4 :

		case EFFECTSPRITETYPE_GREAT_RUFFIAN_2_AXE_THROW:
		case EFFECTSPRITETYPE_GREAT_RUFFIAN_2_AXE_THROW_SHADOW:
			IsMulti = true;
			break;

		case EFFECTSPRITETYPE_SUMMON_FIRE_ELEMENTAL:
		case EFFECTSPRITETYPE_SUMMON_FIRE_ELEMENTAL_ATTACK:
		case EFFECTSPRITETYPE_SUMMON_WATER_ELEMENTAL:
		case EFFECTSPRITETYPE_SUMMON_WATER_ELEMENTAL_HEAL:
			orbitType = 2;
			break;
		
		}

		if( type >= EFFECTSPRITETYPE_LAR_SLASH_MALE_FAST && type <= EFFECTSPRITETYPE_LAR_SLASH_FEMALE_SLOW )
			delayFrame = 0xFFFF;
	}	
	TYPE_FRAMEID	frameID = (*g_pEffectSpriteTypeTable)[type].FrameID;

	if (frameID==FRAMEID_NULL)
	{
		DEBUG_ADD("[Error]FrameID of EffectSpriteType is NULL");
		
		return NULL;
	}


	//------------------------------
	// 이미 있는 경우 
	//------------------------------
	if (m_bAttachEffect[type] && !IsMulti)
	{	
		//------------------------------------------------------
		// 몸에 붙은거 체크
		//------------------------------------------------------
		ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

		// 같은 Effect를 찾는다. 
		while (iEffect != m_listEffect.end())
		{
			MAttachEffect*	pEffect = *iEffect;
			
			// 같은 type을 찾는다.
			if (pEffect->IsEffectSprite() && pEffect->GetEffectSpriteType() == type)
			{
				// 추가할려는 것이 더 늦게 끝날 경우에만 시간을 확장한다.
				if (g_CurrentFrame+delayFrame > pEffect->GetEndFrame()) 
				{
					pEffect->SetCount( delayFrame );
				}

				DEBUG_ADD("find same AttachEffect");
				return pEffect;
			}

			iEffect++;
		}

		//------------------------------------------------------
		// 바닥에 붙은거 체크
		//------------------------------------------------------
		iEffect = m_listGroundEffect.begin();

		// 같은 Effect를 찾는다. 
		while (iEffect != m_listGroundEffect.end())
		{
			MAttachEffect*	pEffect = *iEffect;
			
			// 같은 type을 찾는다.
			if (pEffect->IsEffectSprite() && pEffect->GetEffectSpriteType() == type)
			{
				// 추가할려는 것이 더 늦게 끝날 경우에만 시간을 확장한다.
				if (g_CurrentFrame+delayFrame > pEffect->GetEndFrame()) 
				{
					pEffect->SetCount( delayFrame );
				}

				DEBUG_ADD("find same GroundAttachEffect");
				return pEffect;
			}

			iEffect++;
		}

		return NULL;
	}

	//------------------------------
	// 없는 경우 추가한다.
	//------------------------------
	MAttachEffect*	pEffect = NULL;
	
	if (eType==MEffect::EFFECT_ATTACH_ORBIT)
	{
		pEffect = new MAttachOrbitEffect(type, delayFrame, orbitType, 0, linkDelayFrame);
	}
	else	// EFFECT_ATTACH
	{
		pEffect = new MAttachEffect(type, delayFrame, linkDelayFrame);
	}

	pEffect->SetAttachCreatureID( m_ID );

	// 빛나는 Effect의 개수를 증가?
	if (pEffect->GetBltType()==BLT_EFFECT)
	{
		// 최초로 추가되는 빛나는Effect이면 시야를 추가시킨다.
		//if (m_nAlphaEffect==0)
		{
			pEffect->SetPosition( m_X, m_Y );
			//g_pZone->SetLight(m_X, m_Y, pEffect->GetLight());
		}

		//m_nAlphaEffect ++;		
	}

	//------------------------------------------------------
	// 바닥에 붙는거
	//------------------------------------------------------
	if (bGroundEffect)
	{
		m_listGroundEffect.push_back( pEffect );
	}
	//------------------------------------------------------
	// 몸에 붙는 거
	//------------------------------------------------------
	else
	{
		m_listEffect.push_back( pEffect );
	}

	m_bAttachEffect[type] = true;

	// Set LightSight
	DEBUG_ADD_FORMAT("NewEffect : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), pEffect->GetLight(), pEffect->GetX(), pEffect->GetY());			
	
	return pEffect;
}

//----------------------------------------------------------------------
// Update Effect
//----------------------------------------------------------------------
// Creature에 붙은 모든 Effect의 Frame을 바꿔주고..
// 끝나는게 있으면 list와 memory에서 삭제한다.
//----------------------------------------------------------------------
void		
MCreature::UpdateAttachEffect()
{
	ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();
	ATTACHEFFECT_LIST::iterator iEffectTemp;

	// 밝기는 0
	//m_MaxEffectLight = 0;

	//---------------------------------------------------------------------
	// 캐릭터 색깔 바꾸는 색..
	//---------------------------------------------------------------------
	// NULL값이당.. 외부에서 체크해야함..
	m_AttachEffectColor = m_ChangeColorSet;	//ATTACHEFFECTCOLOR_NULL;
	int bShowColor =  (HasEffectStatus(EFFECTSTATUS_CURSE_PARALYSIS)
						|| g_CurrentFrame % g_pClientConfig->FRAME_DRAW_ORIGINAL_SPRITE);	// 몇 frame마다 한번씩은 원래 색깔을 보여준다.
	int numColors = 0;
	
	//---------------------------------------------------------------------
	// 모든 Effect를 Update한다.
	//---------------------------------------------------------------------
	while (iEffect != m_listEffect.end())
	{
		MAttachEffect*	pEffect = *iEffect;
		
		int x		= pEffect->GetX();
		int y		= pEffect->GetY();
		int light = pEffect->GetLight();		
		bool		bErase = false;

		//---------------------------------------------------------------------
		// Update
		//---------------------------------------------------------------------
		// message : find path
		//#ifdef	OUTPUT_DEBUG
			//sprintf(g_pDebugMessage->GetCurrent(), "Before UpdateEffect : Type=%d", pEffect->GetFrameID());	
			//g_pDebugMessage->Next();
		//#endif


		// 빠른 이동일 경우 이동과 정지의 이펙트가 다르다.... 하드코딩..ㅡ.ㅜ
		if( pEffect->GetEffectSpriteType() == EFFECTSPRITETYPE_FAST_MOVE_FLY ||
			pEffect->GetEffectSpriteType() == EFFECTSPRITETYPE_FAST_MOVE_STOP ||
			(
			pEffect->GetEffectSpriteType() == EFFECTSPRITETYPE_SUMMON_SYLPH_GREEN_FLY &&
			pEffect->GetEffectSpriteType() == EFFECTSPRITETYPE_SUMMON_SYLPH_BLACK_STOP
			)
			)
		{		
			BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_FAST_MOVE_STOP].BltType;
			TYPE_EFFECTSPRITETYPE		EffectSpriteType = pEffect->GetEffectSpriteType();
			bool			bNewSummonSylphEffect = false;
			bool			bNewSummonSylphEffectStop = false;
			
			if( EffectSpriteType >= EFFECTSPRITETYPE_SUMMON_SYLPH_GREEN_FLY &&
				EffectSpriteType <= EFFECTSPRITETYPE_SUMMON_SYLPH_BLACK_STOP )
			{
				bNewSummonSylphEffect = true;

				if( (EffectSpriteType - EFFECTSPRITETYPE_SUMMON_SYLPH_GREEN_FLY) & 0x1 )
					bNewSummonSylphEffectStop = true;
			}
			
			if( bNewSummonSylphEffect && !bNewSummonSylphEffectStop )
				bltType = (*g_pEffectSpriteTypeTable)[EffectSpriteType + 1].BltType;

			if( GetAction() == ACTION_OUSTERS_FAST_MOVE_STAND && (
				EffectSpriteType != EFFECTSPRITETYPE_FAST_MOVE_STOP ||
				( bNewSummonSylphEffect && !bNewSummonSylphEffectStop )
				)
				)
			{
				TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_FAST_MOVE_STOP].FrameID;
				
				if( bNewSummonSylphEffect )
					frameID = (*g_pEffectSpriteTypeTable)[ EffectSpriteType + 1 ].FrameID;

				int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);			
			
				pEffect->SetEffectSprite( EFFECTSPRITETYPE_FAST_MOVE_STOP );
				if( bNewSummonSylphEffect )
					pEffect->SetEffectSprite( EffectSpriteType + 1 );
				pEffect->SetFrameID( frameID, maxFrame );
			}
			else
			if( GetAction() == ACTION_OUSTERS_FAST_MOVE_MOVE && 
				( pEffect->GetEffectSpriteType() != EFFECTSPRITETYPE_FAST_MOVE_FLY ||
				( bNewSummonSylphEffect && bNewSummonSylphEffectStop )
			  )
			  )
			{
				TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[EFFECTSPRITETYPE_FAST_MOVE_FLY].FrameID;
				if( bNewSummonSylphEffect )
					frameID = (*g_pEffectSpriteTypeTable)[ EffectSpriteType - 1 ].FrameID;

				int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);			

				pEffect->SetEffectSprite( EFFECTSPRITETYPE_FAST_MOVE_FLY );

				if( bNewSummonSylphEffect )
					pEffect->SetEffectSprite( EffectSpriteType - 1 );
				pEffect->SetFrameID( frameID, maxFrame );
			}
		}
		else if( pEffect->GetEffectSpriteType() >= EFFECTSPRITETYPE_LAR_SLASH_MALE_FAST && pEffect->GetEffectSpriteType() <= EFFECTSPRITETYPE_LAR_SLASH_FEMALE_SLOW )
		{
			if(!(GetAction() == ACTION_SLAYER_SWORD_2 || GetAction() == ACTION_SLAYER_SWORD_2_SLOW || GetAction() == ACTION_SLAYER_SWORD_2_FAST ) 
				&&	m_RepeatCount <= 0 )
			{
				pEffect->SetCount(0);
			}
		}
		else if (pEffect->GetEffectSpriteType() >= EFFECTSPRITETYPE_REGEN_TOWER_THROW_LIGHT_START &&
			pEffect->GetEffectSpriteType() <= EFFECTSPRITETYPE_REGEN_TOWER_THROW_LIGHT_END &&
			!HasEffectStatus( EFFECTSTATUS_TRYING )
			)
		{
			bErase = true;
		}
		

		if (bErase == false && pEffect->Update())
		{
			// 흡영인 경우에는 방향을 바꾸지 않는다.
			if( pEffect->GetEffectSpriteType() != EFFECTSPRITETYPE_ABSORB_SOUL &&
				pEffect->GetEffectSpriteType() != EFFECTSPRITETYPE_PIERCING_FRONT &&
				pEffect->GetEffectSpriteType() != EFFECTSPRITETYPE_PIERCING_BACK )
				pEffect->SetDirection( m_CurrentDirection );

			// 최고 밝기를 가진 Effect의 밝기를 저장한다.
//			if (m_MaxEffectLight < pEffect->GetLight())
//			{
//				m_MaxEffectLight = pEffect->GetLight();
//			}

			// 위치 설정
			pEffect->SetPosition(m_X, m_Y);

			//---------------------------------------------------------------------
			// 위치가 바뀌었거나
			// 빛의 크기(시야)가 바뀐 경우
			//---------------------------------------------------------------------
			if (x!=pEffect->GetX() || y!=pEffect->GetY()
				|| light != pEffect->GetLight())
			{				
				// 시야 바꾸기
//				g_pZone->UnSetLight(x, y, light);
//				g_pZone->SetLight(pEffect->GetX(), pEffect->GetY(), pEffect->GetLight());

				/*
				// UnSetLightSight
				#ifdef	OUTPUT_DEBUG
					sprintf(g_pDebugMessage->GetCurrent(), "UnSetLightSight : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), light, x,y);	
					g_pDebugMessage->Next();
				#endif

				// SetLightSight
				#ifdef	OUTPUT_DEBUG
					sprintf(g_pDebugMessage->GetCurrent(), "SetLightSight : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), pEffect->GetLight(), pEffect->GetX(), pEffect->GetY());	
					g_pDebugMessage->Next();
				#endif
				*/
			}

			//---------------------------------------------------------------------
			// 캐릭터 색깔 바꾸는 Effect이면
			// 하나를 선택해야 한다.
			//---------------------------------------------------------------------
			if (bShowColor && pEffect->IsEffectColor())
			{
				numColors++;
				//if (m_AttachEffectColor == ATTACHEFFECTCOLOR_NULL || rand()%numColors==0)
				if (m_AttachEffectColor == m_ChangeColorSet || rand()%numColors==0)
				{
					m_AttachEffectColor = pEffect->GetEffectColor();
				}
			}

			//-----------------------------------------------
			//
			// 이 Effect가 끝나기 전에 LinkCount에 의해서
			// 다음 연결되는 Effect가 있으면 생성해야 한다.
			//
			// 현재Frame이 EndLinkFrame을 넘어간 경우
			//
			//-----------------------------------------------
			if (g_CurrentFrame >= pEffect->GetEndLinkFrame()
				&& pEffect->GetLinkSize() != 0)
			{
				// GenerateNext에서 
				// pEffect의 EffectTarget을 NULL로 만들어주기 때문에
				// 여기서 지울 필요 없다.
				g_pEffectGeneratorTable->GenerateNext( pEffect );

				// pEffect는 여전히 존재해야 하므로 지우면 안된다.
			}

			// 제대로 된 경우
			iEffect++;
		}
		//---------------------------------------------------------------------
		// 시간이 다 돼서 끝나는 경우
		//---------------------------------------------------------------------
		else
		{
			
			bool bUseEffectSprite = pEffect->IsEffectSprite();

			// flag제거
			if (bUseEffectSprite)
			{
				m_bAttachEffect[pEffect->GetEffectSpriteType()] = false;	// flag제거
			}
		
			//---------------------------------------------------------------------
			//
			// 다음 연결되는 Effect가 있으면 생성해야 한다.
			//
			//---------------------------------------------------------------------
//			// 2004, 8, 16, sobeit add start test - 도슬 130 스킬
//			if(pEffect->GetEffectSpriteType() == EFFECTSPRITETYPE_BURNING_SOL_CHARGING)
//			{
//				// 	아직 차징 중이면 날라가는게 나오면 안된다..^^
//			}
//			// 2004, 8, 16, sobeit add end test - 도슬 130 스킬
			if (pEffect->GetLinkSize() != 0)
			{				
				(*g_pEffectGeneratorTable).GenerateNext( pEffect );
			}

			// 빛나는 Effect면 시야를 사라지게 해야한다.
			//if (pEffect->GetBltType()==BLT_EFFECT)
			{
				//m_nAlphaEffect --;

				// 모든 빛나는Effect가 사라졌으면 시야를 제거시킨다.
				//if (m_nAlphaEffect==0)
				{
//					g_pZone->UnSetLight(x, y, light);

					// UnSet LightSight
					/*
					#ifdef	OUTPUT_DEBUG
						sprintf(g_pDebugMessage->GetCurrent(), "RemoveEffect : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), light, x,y);	
						g_pDebugMessage->Next();
					#endif
					*/
				}
			}

			DEBUG_ADD_FORMAT("[DeleteAttachEffect] id=%d, esType=%d", m_ID, pEffect->GetEffectSpriteType());
			
			// memory삭제, list삭제						
			delete pEffect;						// memory제거
			
			// list에서 삭제하기 위해서.. 임시로 저장
			iEffectTemp = iEffect;

			iEffect++;
			m_listEffect.erase( iEffectTemp );	// list에서 제거
		}		
	}


	//---------------------------------------------------------------------
	// 바닥에 붙는 effect
	//---------------------------------------------------------------------
	iEffect = m_listGroundEffect.begin();
	
	//---------------------------------------------------------------------
	// 모든 Effect를 Update한다.
	//---------------------------------------------------------------------
	while (iEffect != m_listGroundEffect.end())
	{
		MAttachEffect*	pEffect = *iEffect;
		
		int x		= pEffect->GetX();
		int y		= pEffect->GetY();
		int light = pEffect->GetLight();		

		//---------------------------------------------------------------------
		// Update
		//---------------------------------------------------------------------
		// message : find path
		//#ifdef	OUTPUT_DEBUG
			//sprintf(g_pDebugMessage->GetCurrent(), "Before UpdateEffect : Type=%d", pEffect->GetFrameID());	
			//g_pDebugMessage->Next();
		//#endif

		if (pEffect->Update())
		{
			// 최고 밝기를 가진 Effect의 밝기를 저장한다.
//			if (m_MaxEffectLight < pEffect->GetLight())
//			{
//				m_MaxEffectLight = pEffect->GetLight();
//			}

			// 위치 설정
			pEffect->SetPosition(m_X, m_Y);

			//---------------------------------------------------------------------
			// 위치가 바뀌었거나
			// 빛의 크기(시야)가 바뀐 경우
			//---------------------------------------------------------------------
			if (x!=pEffect->GetX() || y!=pEffect->GetY()
				|| light != pEffect->GetLight())
			{				
				// 시야 바꾸기
//				g_pZone->UnSetLight(x, y, light);
//				g_pZone->SetLight(pEffect->GetX(), pEffect->GetY(), pEffect->GetLight());

				/*
				// UnSetLightSight
				#ifdef	OUTPUT_DEBUG
					sprintf(g_pDebugMessage->GetCurrent(), "UnSetLightSight : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), light, x,y);	
					g_pDebugMessage->Next();
				#endif

				// SetLightSight
				#ifdef	OUTPUT_DEBUG
					sprintf(g_pDebugMessage->GetCurrent(), "SetLightSight : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), pEffect->GetLight(), pEffect->GetX(), pEffect->GetY());	
					g_pDebugMessage->Next();
				#endif
				*/
			}

			//---------------------------------------------------------------------
			// 캐릭터 색깔 바꾸는 Effect이면
			// 하나를 선택해야 한다.
			//---------------------------------------------------------------------
			/*
			// 바닥에 깔리는 거는 색깔 바꾸는게 없다.
			if (bShowColor && pEffect->IsEffectColor())
			{
				numColors++;
				if (m_AttachEffectColor == ATTACHEFFECTCOLOR_NULL || rand()%numColors==0)
				{
					m_AttachEffectColor = pEffect->GetEffectColor();
				}
			}
			*/
			//-----------------------------------------------
			//
			// 이 Effect가 끝나기 전에 LinkCount에 의해서
			// 다음 연결되는 Effect가 있으면 생성해야 한다.
			//
			// 현재Frame이 EndLinkFrame을 넘어간 경우
			//
			//-----------------------------------------------
			if (g_CurrentFrame >= pEffect->GetEndLinkFrame()
				&& pEffect->GetLinkSize() != 0)
			{
				// GenerateNext에서 
				// pEffect의 EffectTarget을 NULL로 만들어주기 때문에
				// 여기서 지울 필요 없다.
				g_pEffectGeneratorTable->GenerateNext( pEffect );

				// pEffect는 여전히 존재해야 하므로 지우면 안된다.
			}

			// 제대로 된 경우
			iEffect++;
		}
		//---------------------------------------------------------------------
		// 시간이 다 돼서 끝나는 경우
		//---------------------------------------------------------------------
		else
		{
			bool bUseEffectSprite = pEffect->IsEffectSprite();

			// flag제거
			if (bUseEffectSprite)
			{
				m_bAttachEffect[pEffect->GetEffectSpriteType()] = false;	// flag제거
			}
		
			//---------------------------------------------------------------------
			//
			// 다음 연결되는 Effect가 있으면 생성해야 한다.
			//
			//---------------------------------------------------------------------
			if (pEffect->GetLinkSize() != 0)
			{				
				(*g_pEffectGeneratorTable).GenerateNext( pEffect );
			}

			// 빛나는 Effect면 시야를 사라지게 해야한다.
			//if (pEffect->GetBltType()==BLT_EFFECT)
			{
				//m_nAlphaEffect --;

				// 모든 빛나는Effect가 사라졌으면 시야를 제거시킨다.
				//if (m_nAlphaEffect==0)
				{
//					g_pZone->UnSetLight(x, y, light);

					// UnSet LightSight
					/*
					#ifdef	OUTPUT_DEBUG
						sprintf(g_pDebugMessage->GetCurrent(), "RemoveEffect : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), light, x,y);	
						g_pDebugMessage->Next();
					#endif
					*/
				}
			}

			
			DEBUG_ADD_FORMAT("[DeleteAttachEffect] id=%d, esType=%d", m_ID, pEffect->GetEffectSpriteType());
			
			// memory삭제, list삭제						
			delete pEffect;						// memory제거
			
			// list에서 삭제하기 위해서.. 임시로 저장
			iEffectTemp = iEffect;

			iEffect++;
			m_listGroundEffect.erase( iEffectTemp );	// list에서 제거
		}		
	}
}

//----------------------------------------------------------------------
// UnSet LightSight AttachEffect
//----------------------------------------------------------------------
// Creature에 붙은 모든 Effect의 시야를 삭제한다.
//----------------------------------------------------------------------
/*
void		
MCreature::UnSetLightSightAttachEffect()
{
	ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();
	
	// 모든 Effect를 Update한다.
	while (iEffect != m_listEffect.end())
	{
		MAttachEffect*	pEffect = *iEffect;
		
		// 시야 삭제
//		g_pZone->UnSetLight(pEffect->GetX(), pEffect->GetY(), pEffect->GetLight());

		// 다음 꺼
		iEffect++;
	}
}
*/

//----------------------------------------------------------------------
// Set LightSight AttachEffect
//----------------------------------------------------------------------
// Creature에 붙은 모든 Effect의 시야를 추가한다.
//----------------------------------------------------------------------
/*
void		
MCreature::SetLightSightAttachEffect()
{
	ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();
	
	// 모든 Effect를 Update한다.
	while (iEffect != m_listEffect.end())
	{
		MAttachEffect*	pEffect = *iEffect;
		
		// 시야 삭제
//		g_pZone->SetLight(pEffect->GetX(), pEffect->GetY(), pEffect->GetLight());

		// 다음 꺼
		iEffect++;
	}
}
*/


//----------------------------------------------------------------------
// Set Dead
//----------------------------------------------------------------------
// 죽는 actionInfo를 보여주고 죽는다.
//----------------------------------------------------------------------
void
MCreature::SetDead()
{
	// 살아 있는 경우만 죽인다. -_-;;
	if (m_bAlive)
	{	
//		if(HasEffectStatus(EFFECTSTATUS_HIDE_TO_ATTACKER))
//		{
//			if(GetCreatureType() == 738) // 플래져 스테이션
//				ExecuteActionInfoFromMainNode(SKILL_PLEASURE_EXPLOSION,GetX(), GetY(), 0,GetDirection(),	GetID(),	
//								GetX(), GetY(), 0, 20, NULL, false);			
//			else if(GetCreatureType() == 739) // 랜드 마인
//				ExecuteActionInfoFromMainNode(SKILL_LAND_MINE_EXPLOSION,GetX(), GetY(), 0,GetDirection(),	GetID(),	
//								GetX(), GetY(), 0, 0, NULL, false);	
//			else if(GetCreatureType() == 740) // 클레이 모어
//				ExecuteActionInfoFromMainNode(SKILL_CLAYMORE_EXPLOSION,GetX(), GetY(), 0,GetDirection(),	GetID(),	
//								GetX(), GetY(), 0, 20, NULL, false);	
//		}
		//if(HasEffectStatus( EFFECTSTATUS_SUMMON_SYLPH ) )
		if(GetCreatureType() == 723) // 각성 질드레
			ExecuteActionInfoFromMainNode(SKILL_CLIENT_GDR_DEAD, GetX(), GetY(), 0,GetDirection(),	GetID(),	
								GetX(), GetY(), 0, 20, NULL, false);			

		if(HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET))
		{
			RemoveEffectStatus(EFFECTSTATUS_INSTALL_TURRET);
			SetAction( ACTION_STAND );
		}

		if( HasEffectStatusSummonSylph( this ) )
		{
			RemoveEffectStatusSummonSylph( this );
			SetAction( ACTION_STAND );
		}
		if(HasEffectStatus(EFFECTSTATUS_GLACIER))
		{ 
			RemoveEffectStatus( EFFECTSTATUS_GLACIER );
			SetAction(ACTION_STAND);
			ClearAttachEffect();
		}
		StopDrain();		
		StopAbsorb();

		//-------------------------------------------------------`
		// server block좌표 없애주기
		//-------------------------------------------------------
		m_pZone->UnSetServerBlock(m_MoveType, m_ServerX, m_ServerY);
	
		// 2002.3.22 추가
		SetGroundCreature();

		MovePosition( m_ServerX, m_ServerY );
		SetStop();

		// 다른 결과를 적용시킨다.
		AffectUsedActionInfo(m_nUsedActionInfo);		
		m_nUsedActionInfo	= ACTIONINFO_NULL;
		m_nSpecialActionInfo = ACTIONINFO_NULL;

		// 죽을 때의 ActionInfo	
		m_nNextUsedActionInfo	= (*g_pCreatureTable)[m_CreatureType].DeadActionInfo;	

		MActionResultNode* pActionResultNode = CreateActionResultNode(this, m_nNextUsedActionInfo);
		if(pActionResultNode != NULL)
		{
			if(m_pActionResult != NULL)
			{
				delete m_pActionResult;
				m_pActionResult = NULL;
			}
			
			m_pActionResult = new MActionResult;
			
			m_pActionResult->Add(pActionResultNode);
		}

			
		
		if (m_nNextUsedActionInfo!=ACTIONINFO_NULL)
		{		
			// 죽을 때의 동작
			SetNextAction( GetActionInfoAction(m_nNextUsedActionInfo) );

			// 나한테 하는 동작
			SetTraceID( m_ID );
			m_TraceX				= m_X;
			m_TraceY				= m_Y;
			m_TraceZ				= 0;
		}

		//-------------------------------------------------------
		// 보이게 하기
		//-------------------------------------------------------
		SetVisibleSoon();

		// HP
		StopRecoveryHP();
		StopRecoveryMP();
		SetStatus( MODIFY_CURRENT_HP, 0 );

		m_Z = 0;


		// 죽었다.
		m_bAlive = false;	
		m_bInCasket = false;

		//-------------------------------------------------------
		// Effect 제거
		//-------------------------------------------------------
		ClearAttachEffect();

		//-------------------------------------------------------
		// Effect상태들 제거
		//-------------------------------------------------------
		if (g_pEffectStatusTable!=NULL)
		{
			for (int e=0; e<g_pEffectStatusTable->GetSize(); e++)
			{
				if (e!=EFFECTSTATUS_BLOOD_DRAIN)
				{
					RemoveEffectStatus( (EFFECTSTATUS)e );
				}
			}
		}

		// 다크니스 벗어나기..
		m_DarknessCount = -1;
		m_DarknessCountInc = 0;

	}	
}

//----------------------------------------------------------------------
// Set Corpse
//----------------------------------------------------------------------
// 죽어 있는 상태로 만든다. --> 바로 시체로..
//----------------------------------------------------------------------
void
MCreature::SetCorpse()
{	
	//-------------------------------------------------------
	// server block좌표 없애주기
	//-------------------------------------------------------
	m_pZone->UnSetServerBlock(m_MoveType, m_ServerX, m_ServerY);

	// 정지
	SetStop();

	// 죽는 모습
	ACTIONINFO deadAI = (enum ACTIONINFO)(*g_pCreatureTable)[m_CreatureType].DeadActionInfo;
	
	// 죽을 때의 동작
	SetAction( GetActionInfoAction(deadAI) );

	// 2002.3.22 추가
	SetGroundCreature();
	
	// 동작 모두 수행..
	m_ActionCount	= GetActionCountMax(); 
	m_MoveCount		= m_MoveCountMax;
	
	// 죽었다.
	m_bAlive = false;

	// 다크니스 벗어나기..
	m_DarknessCount = -1;
	m_DarknessCountInc = 0;
}

//----------------------------------------------------------------------
// Set Alive
//----------------------------------------------------------------------
void
MCreature::SetAlive()
{
	// 살아난다. 
	m_bAlive = true;

	m_bHasHead = true;

	SetAction( ACTION_STAND );

	//-------------------------------------------------------
	// Effect상태들 제거
	//-------------------------------------------------------
	/*
	if (g_pEffectStatusTable!=NULL)
	{
		for (int e=0; e<g_pEffectStatusTable->GetSize(); e++)
		{
			RemoveEffectStatus( (EFFECTSTATUS)e );
		}
	}
	*/

	if ((*g_pCreatureTable)[m_CreatureType].bFlyingCreature)	// 박쥐인 경우에는 SetGroundCreature했던거 때매 다시 m_MoveType를 바꿔준다
		m_MoveType = CREATURE_FLYING;

	if (m_MoveType == CREATURE_FLYING)
	{
		m_Z=(*g_pCreatureTable)[m_CreatureType].FlyingHeight;
	}
}

//----------------------------------------------------------------------
// Set NextAction
//----------------------------------------------------------------------
// 다음에 할 행동을 설정한다.
//----------------------------------------------------------------------
void			
MCreature::SetNextAction(BYTE action)
{
	m_NextAction = action;
}

//----------------------------------------------------------------------
// Set NextAction to Move
//----------------------------------------------------------------------
// 다음 action에는 move한다.
//----------------------------------------------------------------------
void	
MCreature::SetNextActionToMove()
{
	m_NextAction = m_MoveAction;
}

//----------------------------------------------------------------------
// Set CutHeight From
//----------------------------------------------------------------------
void				
MCreature::SetCutHeightFrom(int startCount, int lastCount, int inc)
{
	m_bCutHeight = true;
	m_CutHeightCountLast = lastCount;
	m_CutHeightCount = startCount;
	m_CutHeightCountInc = inc;
}

//----------------------------------------------------------------------
// Update CutHeight
//----------------------------------------------------------------------
void				
MCreature::UpdateCutHeight()
{
	if (m_bCutHeight)
	{
		m_CutHeightCount += m_CutHeightCountInc;

		if (m_CutHeightCountInc < 0 && m_CutHeightCount <= m_CutHeightCountLast)
		{
			m_bCutHeight = false;
			m_CutHeightCount = 0;
			m_CutHeightCountInc = 0;
		}

		else if (m_CutHeightCountInc > 0 && m_CutHeightCount >= m_CutHeightCountLast)
		{
			m_bCutHeight = false;
			m_CutHeightCount = m_CutHeightCountLast;
			m_CutHeightCountInc = 0;
		}	
		
		// 빙빙 돌린다.
		m_Direction = ((m_Direction+1) & 0x07);		

		if( GetCreatureType() == 702 || GetCreatureType() == 703 || GetCreatureType() == 704 )
		{
			MFakeCreature *pFakeCreature = (MFakeCreature *)this;
			pFakeCreature->SyncTurretDirection();
		}
	}
}

//----------------------------------------------------------------------
// Update Turning
//----------------------------------------------------------------------
void				
MCreature::UpdateTurning()
{
	if (m_bTurning)
	{
		if (--m_TurningCount <= 0)
		{
			m_bTurning = false;
		}
		else
		{
			// 빙빙 돌린다.
			m_Direction = ((m_Direction+1) & 0x07);
		}
	}
}

//----------------------------------------------------------------------
// Update CauseCriticalWounds
//----------------------------------------------------------------------
void				
MCreature::UpdateCauseCriticalWounds()
{
	if(m_bCauseCriticalWounds)
	{
		if(IsDead())
		{
			StopCauseCriticalWounds();
			SetStop();
			return;
		}
		if(--m_CauseCriticalWoundsCount < 0)
		{
			StopCauseCriticalWounds();
			SetStop();
		}
		if(m_CauseCriticalWoundsCount < 10 && m_CauseCriticalWoundsCount >= 0)
			m_ActionCount = m_CauseCriticalWoundsCount;
		else if(m_ActionCount < 10)
			m_ActionCount++;

		if(m_CreatureType == 793) // 슬레이어 제물 -_-;
			m_ActionCount = 20;

	#ifdef OUTPUT_DEBUG					
		DEBUG_ADD_FORMAT("[UpdateCCW] bCCW : %d CCWCount : %d Action : %d ActionCount : %d", m_bCauseCriticalWounds, m_CauseCriticalWoundsCount, m_Action, m_ActionCount);
	#endif
	}
}

//----------------------------------------------------------------------
// Update Ghost
//----------------------------------------------------------------------
void				
MCreature::UpdateGhost()
{
//	DEBUG_ADD_FORMAT("[UpdateGhost] %s, %d", m_fGhost?"true":"false", m_GhostCount);
	if(m_fGhost)
	{
		if(--m_GhostCount < 0)
		{
			StopGhost();
			//SetStop();
		}
	}
}

bool MCreature::IsBloodyZenith()
{
//	bool bActionAttack = 
//	{
//		m_Action == ACTION_ATTACK ||
//		m_Action == ACTION_VAMPIRE_ATTACK_FAST ||
//		m_Action == ACTION_VAMPIRE_ATTACK_SLOW
//	};

	return m_bBloodyZenith;//  && bActionAttack; 
}

int MCreature::GetBloodyZenithCount()
{
	return m_BloodyZenithCount;
}

//----------------------------------------------------------------------
// Update BloodyZenith
//----------------------------------------------------------------------
void				
MCreature::UpdateBloodyZenith()
{
	if(IsBloodyZenith() && !IsFastMove())
	{
		if(--m_BloodyZenithCount < 0)
		{
			StopBloodyZenith();
//			char TempBuffer[256];
//			sprintf(TempBuffer,"블러디 제니스 끝 a:%d, n:%d, n:%d, z:%d,s:%d", m_nUsedActionInfo, m_NextAction, m_nNextUsedActionInfo,MAGIC_BLOODY_ZENITH,SKILL_SET_AFIRE);
//			
//			g_pSystemMessage->Add(TempBuffer);
//			SetStop();
		}
	}
}

//----------------------------------------------------------------------
// Update GunShotGuidance
//----------------------------------------------------------------------
void				
MCreature::UpdateGunShotGuidance()
{
	if(m_bGunShotGuidance)
	{
		if(IsDead())
		{
			StopGunShotGuidance();
			SetStop();
			return;
		}
		if(--m_GunShotGuidanceCount < 0)
		{
			StopGunShotGuidance();
			SetStop();
		}
		if(m_GunShotGuidanceCount < 8 && m_GunShotGuidanceCount >= 0)
			m_ActionCount = m_GunShotGuidanceCount;
		else if(m_ActionCount < 8)
			m_ActionCount++;
		
	#ifdef OUTPUT_DEBUG					
		DEBUG_ADD_FORMAT("[UpdateCCW] bCCW : %d CCWCount : %d Action : %d ActionCount : %d", m_bGunShotGuidance, m_GunShotGuidanceCount, m_Action, m_ActionCount);
	#endif
	}
}

//----------------------------------------------------------------------
// Set FakeDie
//----------------------------------------------------------------------
void				
MCreature::SetFakeDie()
{
	m_bFakeDie = true;
	m_FakeDieCount = 0;
}

//----------------------------------------------------------------------
// Update CauseCriticalWounds
//----------------------------------------------------------------------
void				
MCreature::UpdateFakeDie()
{
	if(m_bFakeDie)
	{
		++m_FakeDieCount;
//		if( < 0)
//		{
//			StopFakeDie();
//		}
	}
}

//----------------------------------------------------------------------
// Set Invisible
//----------------------------------------------------------------------
void				
MCreature::SetInvisible()
{
	m_InvisibleCount = 0;
	m_InvisibleStep = 1;
}

//----------------------------------------------------------------------
// Set Invisible
//----------------------------------------------------------------------
void				
MCreature::SetInvisibleSoon()
{
	m_InvisibleCount = 64;
	m_InvisibleStep = 0;
}

//----------------------------------------------------------------------
// Set Visible
//----------------------------------------------------------------------
void				
MCreature::SetVisible()
{
	m_InvisibleCount = 64;
	m_InvisibleStep = -1;
}

//----------------------------------------------------------------------
// Set Visible
//----------------------------------------------------------------------
void				
MCreature::SetVisibleSoon()
{
	m_InvisibleCount = 0;
	m_InvisibleStep = 0;
}

//----------------------------------------------------------------------
// Set SameBody
//----------------------------------------------------------------------
void		
MCreature::SetSameBody(const MCreature* pCreature)
{
	SetCreatureType( pCreature->GetCreatureType() );

	if (GetClassType()!=CLASS_FAKE)
	{
		m_MoveType = pCreature->m_MoveType;
	}

//	if(pCreature->IsAdvancementClass())
//		SetStatus( MODIFY_ADVANCEMENT_CLASS_LEVEL, pCreature->GetAdvancementClassLevel() );
	m_Z	= pCreature->m_Z;

	m_ColorBody1 = pCreature->m_ColorBody1;
	m_ColorBody2 = pCreature->m_ColorBody2;
}

//----------------------------------------------------------------------
// Set Action
//----------------------------------------------------------------------
// 한 Action마다 Frame수가 다를 것이다.. 
// 이거는 file로 빼서 정보를 두는게 나을 것인데
// FRAME_PACK에서 읽어와야 한다.
//----------------------------------------------------------------------
void
MCreature::SetAction(BYTE action)
{ 	
//	if ( m_nUsedActionInfo == MAGIC_BLOODY_ZENITH && m_Action == ACTION_ATTACK &&
//		m_ActionCount >= m_ActionCountMax && GetActionDelay() > 0)
//	{
//		SetActionDelay( GetActionDelay() - 1 );
//		m_ActionCount = m_ActionCountMax - 2;
//		return;
//	}

	// 무조건 꺼준다.
	m_bFastMove = false;

	if (!m_bAlive
		&& action!=ACTION_DIE)
	{		
		return;
	}
	if(HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET) && action != ACTION_STAND && action != ACTION_ATTACK)
		return;

	
	if(GetCreatureType() == 782 && action == ACTION_ATTACK)
		action = ACTION_VAMPIRE_DRAIN;
//	if(action>ACTION_OUSTERS_MAGIC_ATTACK)
//		int i = 0;
	//-----------------------------------------------------------------
	// 현재 모습의 action에 맞는 경우. 2001.10.5
	//-----------------------------------------------------------------
	if (action < GetActionMax())
	{
		
		//-----------------------------------------------------
		// 오토바이 타고 있을때
		//-----------------------------------------------------
		if (m_MoveDevice==MOVE_DEVICE_RIDE)
		{
			if (IsSlayer())
			{
				if (action==ACTION_STAND)
				{
					action = ACTION_SLAYER_MOTOR_STAND;
				}
				else if (action==ACTION_MOVE)
				{
					action = ACTION_SLAYER_MOTOR_MOVE;
				}
			}
		} else
			//-----------------------------------------------------
			// 걸어다닐때
			//-----------------------------------------------------
		{
			if (IsSlayer())
			{
				if (action==ACTION_SLAYER_MOTOR_STAND)
				{
					action = ACTION_STAND;
				}
				else if (action==ACTION_SLAYER_MOTOR_MOVE)
				{
					action = ACTION_MOVE;
				}
			}
		}
		if( IsOusters() && (HasEffectStatusSummonSylph( this ) || m_MoveDevice==MOVE_DEVICE_SUMMON_SYLPH))
		{
			if (action==ACTION_STAND)
			{
				action = ACTION_OUSTERS_FAST_MOVE_STAND;
			} else
			if( action == ACTION_MOVE )
			{
				action = ACTION_OUSTERS_FAST_MOVE_MOVE;
			}
		}
				
		m_bNextAction = false;
			
		//------------------------------------------------
		// Move인 경우
		//------------------------------------------------
		if (action==m_MoveAction)//ACTION_MOVE || action==ACTION_SLAYER_MOTOR_MOVE)
		{
			// 그냥 서있는 경우면... 끝동작으로 만든다.
			// Action이 끝났다고 표시해주기 위해서..
			if (m_Action==ACTION_STAND 
				|| IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND)
			{			
				m_ActionCount = 0;
				m_ActionCountMax = 0;			
			}		

			// Action이 끝난 경우만 Action을 Move로 한다.
			// 아닌 경우는.. Action이 끝나고 Move로 바뀐다.
			if (m_ActionCount>=GetActionCountMax())
			{
				m_Action = action;
			}		

			// 정지
			m_sX = 0;
			m_sY = 0;

			// Server에서 검증된 위치로 이동시킨다.
			//MovePosition( m_ServerX, m_ServerY );
			
			// [ TEST CODE ]
			// Action을 없앤다.
			// --> 안 없애면.. Action이 끝나고 Move가 수행된다. 
			// 그런데, Move가 안돼서 좌표가 빗나갈 확률이 있다. 흠!!
			//m_ActionCount = 0;
			//m_ActionCountMax = 0;

			m_RepeatCount = 0;

			// Move 첫 동작
			m_MoveCount = 0;
			m_NextMoveCount = 0;
			
			//m_MoveCountMax = (*g_pCreatureTable)[m_CreatureType].GetActionCount( action );
			m_MoveCountMax = GetCreatureActionCountMax( this, action );
				//g_pTopView->m_CreatureFPK[(*g_pCreatureTable)[m_CreatureType].FrameID]
				//							[action][m_CurrentDirection].GetSize();//ACTIONCOUNT_MOVE;

		}
		//------------------------------------------------
		// Action인 경우
		//------------------------------------------------
		else
		{
			//------------------------------------------------
			// extreme은 특정 동작에서 풀린다.
			//------------------------------------------------
//			if (HasEffectStatus(EFFECTSTATUS_EXTREME))
//			{
//				if (action==ACTION_MAGIC
//					|| action==ACTION_DIE
//					|| action==ACTION_VAMPIRE_DRAIN
//					|| action==ACTION_DRAINED)
//				{
//					RemoveEffectStatus(EFFECTSTATUS_EXTREME);
//				}
//			}

			if (// 오토바이 타고 있을때는 action 안 보여준다.
				m_MoveDevice==MOVE_DEVICE_RIDE && action!=ACTION_SLAYER_MOTOR_STAND
				// damaged인 경우..
				|| action==ACTION_DAMAGED 						
						&& (// 공격받는 중이 아니거나 정지 동작이 아니면 공격 받는 모습 표현 안 한다
							m_Action!=ACTION_DAMAGED && m_Action!=ACTION_STAND && m_Action!=ACTION_MOVE
						//&& (// 흡혈 당하는 동작 중이거나 
						//	m_Action==ACTION_DRAINED
							// 뱀파이어일때, 흡혈하는 동작 중에는 damaged를 안 보여준다.
						//	|| IsVampire() && m_Action==ACTION_VAMPIRE_DRAIN
							)
				)
			{
				m_NextAction = (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND;
				return;
			}

			
			// 2005, 1, 6, sobeit add start - motor ride move 하던중 stop 가 날라오면 한번은 무시..
			if (IsAdvancementClass() && m_MoveDevice==MOVE_DEVICE_RIDE && action == ACTION_SLAYER_MOTOR_STAND&& m_Action==ACTION_SLAYER_MOTOR_MOVE)
			{
				if(!m_IsSkipMotorStand)
				{
					m_NextAction = (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND;
					m_IsSkipMotorStand = true;
					return;
				}
				else
					m_IsSkipMotorStand = false;
			}
			// 2005, 1, 6, sobeit add end
			m_ActionCountMax = GetCreatureActionCountMax( this, action );//(*g_pCreatureTable)[m_CreatureType].GetActionCount( action );

			if (m_ActionCountMax==0)
			{
				// 동작이 없는 경우
				m_Action = (m_MoveDevice==MOVE_DEVICE_WALK)? ACTION_STAND : ACTION_SLAYER_MOTOR_STAND;
				m_ActionCount = 0;		
				m_RepeatCount = 0;
				//m_ActionCountMax = (*g_pCreatureTable)[m_CreatureType].GetActionCount( m_Action );
				m_ActionCountMax = GetCreatureActionCountMax( this, m_Action );
				
				// 밑에 해놨길래 일단 해놓는데.. 뭔지 몰겠다 - -; 2002.1.10
				m_nSpecialActionInfo = ACTIONINFO_NULL;
			}
			else
			{
				m_Action = action;			

				//---------------------------------------------
				// action을 반복하는 경우
				//---------------------------------------------
				// 반복 action의 시작부터 보여준다.
				// 그러나, 다음 반복 packet이 언제 올지 모르기 때문에..
				// ActionCounMax는 무조건 끝~frame으로 정한다.
				//---------------------------------------------
//				BOOL bSlayer = IsSlayer();

				if (m_RepeatCount!=0	//m_Action == action			
					&& action!=ACTION_STAND
					&& (!IsSlayer() || IsSlayer() && action!=ACTION_SLAYER_MOTOR_STAND)
					//&& (*g_pActionInfoTable)[m_nUsedActionInfo].IsUseRepeatFrame())
					)
				{
					// count 설정..
					
					m_ActionCount = GetActionInfoRepeatStartFrame( m_nSpecialActionInfo );
//					char Msg[128];
//					sprintf(Msg, "%d" , m_ActionCount);
//					g_pSystemMessage->Add(Msg);	
				}
				//---------------------------------------------
				// 일반적인 경우..
				//---------------------------------------------
				else
				{			
					// count 설정..
					m_ActionCount = 0;		
					m_RepeatCount = 0;

					// 2004, 11, 12, sobeit modify start - block
					// 이거 뭐지? 왜 해놨는지 모르겠네.. - -;; 2002.1.10
				//	m_nSpecialActionInfo = ACTIONINFO_NULL;	
					// 2004, 11, 12, sobeit modify end - block
				}				
			}
		}
	}
	// 2004, 11, 12, sobeit add start - block
	if(m_Action == ACTION_STAND)	
		m_nSpecialActionInfo = ACTIONINFO_NULL;	
	// 2004, 11, 12, sobeit add end - block

	// 동작을 시작할때 부가적으로 붙는 Effect 들.
	// 어딘가로 빼야 하는데.-_-
	if( HasEffectStatus( EFFECTSTATUS_REDIANCE ) )
	{
		if( 
			( m_Action == ACTION_SLAYER_SWORD_2 || m_Action == ACTION_SLAYER_SWORD_2_SLOW || m_Action == ACTION_SLAYER_SWORD_2_FAST ) 
			)
		{
			MAttachEffect *pEffect = CreateAttachEffect( EFFECTSPRITETYPE_REDIANCE_MALE_NORMAL, 0, 0 );
			if( pEffect != NULL )
			{
				pEffect->SetDirection( GetDirection() );
				pEffect->SetLink( ACTIONINFO_NULL, NULL );							
				pEffect->SetAttachCreatureID( GetID() );
			}
		} 
		else if(
			( m_Action == ACTION_SLAYER_SWORD || m_Action == ACTION_SLAYER_SWORD_FAST || m_Action == ACTION_SLAYER_SWORD_SLOW ) )
		{
			MAttachEffect *pEffect = CreateAttachEffect( EFFECTSPRITETYPE_REDIANCE_MALE_NORMAL_ATTACK_NORMAL, 0, 0 );
			if( pEffect != NULL )
			{
				pEffect->SetDirection( GetDirection() );
				pEffect->SetLink( ACTIONINFO_NULL, NULL );							
				pEffect->SetAttachCreatureID( GetID() );
			}
		}				
	}

//	// 2004, 9, 8, sobeit add start - 도슬 130 스킬 관련
//	else if(HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1)||
//			HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2)||
//			HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_3)||
//			HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_4)
//			)
//	{
//		if(m_Action == ACTION_SLAYER_BLADE)
//		{
//			if(m_ActionCount>4)
//				m_ActionCount = 5;
//		}
//		else
//			m_Action = ACTION_SLAYER_BLADE;
//	
//	}
//	// 2004, 9, 8, sobeit add end - 도슬 130 스킬 관련
	m_NextAction = (m_MoveAction==ACTION_SLAYER_MOTOR_MOVE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND;
}

//----------------------------------------------------------------------
// Clear Chat String
//----------------------------------------------------------------------
void
MCreature::ClearChatString()
{
	// Chatting String
	m_ChatStringCurrent = 0;
	for (int i=0; i<g_pClientConfig->MAX_CHATSTRING; i++)
	{
		m_ChatString[i][0] = NULL;
	}
	m_HeadSkin = 0;

//	if( CurPernalShop() == 1 && m_NextChatFadeTime != 0 )
	if( CurPernalShop() == 1 && GetInputChatting() == true)
	{
		SetInputChatting(false);
		
		if( m_PersnalMessage.empty() )
			SetPersnalString( (*g_pGameStringTable)[UI_STRING_MESSAGE_PERSNAL_DEFAULT_MESSGE].GetString() );
		else
			SetPersnalString( (char*)m_PersnalMessage.c_str(), g_pUserOption->ChattingColor );
	}
}

void		
MCreature::SetPersnalString(char *str, COLORREF color)
{
	if (str==NULL)
	{
		DEBUG_ADD("[SetPersnalString] str == NULL");
		return;
	}

	DEBUG_ADD_FORMAT("[SetPersnalString] %s", str);

//if(CurPernalShop() == 0 )
	ClearChatString();

	int startIndex = 0,
		endIndex = 0;

	m_NextChatFadeTime = 0;
	SetInputChatting(false);

	DEBUG_ADD("[SetPersnalString] before while");
	int len = strlen(str);
	while (endIndex < len)
	{
		endIndex = startIndex + g_pClientConfig->MAX_CHATSTRING_LENGTH;

		// 크리스마스 트리용 하드 코딩
		bool bTree = false;
		char *find = strchr(str+startIndex, '\n'); 
		if(find != NULL)
		{
			*find = '\0';
			endIndex = strlen(str+startIndex)+startIndex;
			bTree = true;
		}
		
		// len이 짜를만한 길이도 안되면...
		if (endIndex >= len)
		{
			endIndex = len;

			// 일정 개수의 String을 copy한다.
			strcpy(m_ChatString[m_ChatStringCurrent], str+startIndex);					
		}
		else
		{
			// g_pClientConfig->MAX_CHATSTRING_LENGTH개를 짜를 수 없는가?
			if (!g_PossibleStringCut(str, endIndex))
			{
				endIndex --;
			}			
			
			// startIndex ~ endIndex-1 까지 strcpy
			char	*pSource=str+startIndex, 
					*pDest	=m_ChatString[m_ChatStringCurrent];
			for (int i=startIndex; i<endIndex; i++)
			{
				*pDest++ = *pSource++;
			}
			*pDest = NULL;

			if(bTree)
			{
				endIndex++;
			}
		}

		// 다음 줄..
		m_ChatStringCurrent++;
		if (m_ChatStringCurrent==g_pClientConfig->MAX_CHATSTRING) m_ChatStringCurrent=0;

		// index를 바꾼다.
		startIndex = endIndex;
	}
	
	// 채팅 String이 Delay될 시간을 지정해준다.	
//	m_NextChatFadeTime = g_CurrentTime + g_pClientConfig->DELAY_CHATSTRING_KEEP;
//	m_NextChatFadeTime = g_CurrentTime + 0xFFFFFFFF;

	// 채팅 색깔
//	m_ChatColor = RGB(50, 50, 200);//RGB_WHITE;//0xFFFF;
//	m_OriChatColor = m_ChatColor;
	
	m_ChatColor = color;
	m_OriChatColor |= 0xFF000000;
//BYTE b = (m_OriChatColor & 0xFF000000) >> 24;
	m_ChatTime = g_CurrentFrame;

//	if(CurPernalShop() == 0 )
//		ClearChatString();

	DEBUG_ADD("[SetPersnalString] Excute ok");
	for(int i = 0; i < m_ChatStringCurrent; i++)
	{
		DEBUG_ADD_FORMAT("%s", m_ChatString[i]);
	}
	DEBUG_ADD("[SetPersnalString] Excute ok 2");
}
//----------------------------------------------------------------------
// Set ChatString
//----------------------------------------------------------------------
// String을 추가한다. 끝에~..
//----------------------------------------------------------------------
void		
MCreature::SetChatString(char *str, COLORREF color)
{
	if (str==NULL)
	{
		DEBUG_ADD("[SetChatString] str == NULL");
		return;
	}

//	if(CurPernalShop() == 1)
//		return;

	DEBUG_ADD_FORMAT("[SetChatString] %s", str);

	if(g_pUserOption->ChatWhite)
		color = RGB_WHITE;

	SetInputChatting(false);
	ClearChatString();
	SetInputChatting(true);



	int startIndex = 0,
		endIndex = 0;

//	size_t cur = -1;
//	std::string temp;
//	temp = str;;
//	
//	while((cur= temp.find("\n")) != -1)
//	{
//		temp.replace(temp.begin() + cur , temp.begin() + cur+2, "\0");
//	}
//
//	str = (char*)temp.c_str();

	// 크리스마스 트리용 하드코딩
	char *szTreeFrom = NULL;
	if( GetCreatureType() == 482 || GetCreatureType() == 650 )
	{
		if(GetCreatureType() == 482)
		{
			DEBUG_ADD("[SetChatString] Tree!!!");
			
			char *find = strchr(str, '%');
			if(find == NULL)
			{
				DEBUG_ADD("[SetChatString] find == NULL");
				
				return;
			}
			
			*find = '\0';
			
			strcpy(m_ChatString[m_ChatStringCurrent], "Dear. ");
			strcat(m_ChatString[m_ChatStringCurrent], str);					
			
			m_ChatStringCurrent++;
			
			str = find+1;
		}
		char *find2 = strchr(str, '%');
		if(find2 == NULL)
		{
			DEBUG_ADD("[SetChatString] find2 == NULL");

			return;
		}

		*find2 = '\0';

		szTreeFrom = find2+1;
	}    
	

	int len = strlen(str);
	
	if(strstr(str, "배철숩니다") != NULL)
	{
		m_HeadSkin = 215;
	}
	else if(strstr(str, "해골바가지") != NULL)
	{
		m_HeadSkin = 149;
	}
	else if(strstr(str, "늑대인간") != NULL)
	{
		m_HeadSkin = 397;
	}
	else if(strstr(str, "닭대가리") != NULL)
	{
		m_HeadSkin = 221;
	}
	else if(strstr(str, "가면라이더") != NULL)
	{
		m_HeadSkin = 316;
	}


	// 한 문장의 String을 적절한 길이로 잘라준다~~
	DEBUG_ADD("[SetChatString] before while");
	while (endIndex < len)
	{
		endIndex = startIndex + g_pClientConfig->MAX_CHATSTRING_LENGTH;

		// 크리스마스 트리용 하드 코딩
		bool bTree = false;
		if(GetCreatureType() == 482 || GetCreatureType() == 650 )
		{
			char *find = strchr(str+startIndex, '\n'); 
			if(find != NULL)
			{
				*find = '\0';
				endIndex = strlen(str+startIndex)+startIndex;
				bTree = true;
			}
		}
		
		// len이 짜를만한 길이도 안되면...
		if (endIndex >= len)
		{
			endIndex = len;

			// 일정 개수의 String을 copy한다.
			strcpy(m_ChatString[m_ChatStringCurrent], str+startIndex);					
		}
		else
		{
			// g_pClientConfig->MAX_CHATSTRING_LENGTH개를 짜를 수 없는가?
			if (!g_PossibleStringCut(str, endIndex))
			{
				endIndex --;
			}			
			
			// startIndex ~ endIndex-1 까지 strcpy
			char	*pSource=str+startIndex, 
					*pDest	=m_ChatString[m_ChatStringCurrent];
			for (int i=startIndex; i<endIndex; i++)
			{
				*pDest++ = *pSource++;
			}
			*pDest = NULL;

			if(bTree)
			{
				endIndex++;
			}
		}

		// 다음 줄..
		m_ChatStringCurrent++;
		if (m_ChatStringCurrent==g_pClientConfig->MAX_CHATSTRING) m_ChatStringCurrent=0;

		// index를 바꾼다.
		startIndex = endIndex;
	}
	DEBUG_ADD("[SetChatString] while ok");

	// 크리스마스 트리용 하드코딩
	if(GetCreatureType() == 482 || GetCreatureType() == 650)
	{
		DEBUG_ADD("[SetChatString] Tree code");

		int len = strlen(szTreeFrom) + 6;

		char szTemp[512];
		memset(szTemp, ' ', g_pClientConfig->MAX_CHATSTRING_LENGTH);

		strcpy(szTemp + max(g_pClientConfig->MAX_CHATSTRING_LENGTH - len, 0), "From. ");
		strcat(szTemp, szTreeFrom);

		strcpy(m_ChatString[m_ChatStringCurrent], szTemp);

		// 다음 줄..
		m_ChatStringCurrent++;

		DEBUG_ADD("[SetChatString] Tree code ok");
	}
	

	// 채팅 String이 Delay될 시간을 지정해준다.	
	m_NextChatFadeTime = g_CurrentTime + g_pClientConfig->DELAY_CHATSTRING_KEEP;

	// 채팅 색깔
	m_ChatColor = color;//RGB_WHITE;//0xFFFF;
	m_OriChatColor = m_ChatColor;
	
	// 크리스마스 트리용 하드 코딩
	if(GetCreatureType() == 482 || GetCreatureType() == 650)
	{
		m_NextChatFadeTime = 0;
	}
	else
	{
		m_OriChatColor |= 0xFF000000;
	}

	// 말한 시간 설정
	m_ChatTime = g_CurrentFrame;
	

	DEBUG_ADD("[SetChatString] Excute ok");
	for(int i = 0; i < m_ChatStringCurrent; i++)
	{
		DEBUG_ADD_FORMAT("%s", m_ChatString[i]);
	}
	DEBUG_ADD("[SetChatString] Excute ok 2");
}

//----------------------------------------------------------------------
// Fade ChatString
//----------------------------------------------------------------------
// ChatString을 어둡게 한다.
//----------------------------------------------------------------------
void		
MCreature::FadeChatString()
{
	/*
	m_ChatString[m_ChatStringCurrent][0] = NULL;	

	m_ChatStringCurrent++;
	if (m_ChatStringCurrent==g_pClientConfig->MAX_CHATSTRING) m_ChatStringCurrent=0;

	// 채팅 String이 Delay될 시간을 지정해준다.	
	m_NextChatFadeTime = g_CurrentTime + DELAY_FADECHATSTRING;
	*/

//	MCreature* pCreature = g_pZone->GetCreature(GetID());
//	if(CurPernalShop() == 1 && m_NextChatFadeTime+3000 >= g_CurrentTime)
//	{
//		if(pCreature->GetPersnalShopMessage().length()>0)
//			pCreature->SetPersnalString((char*)g_pPlayer->GetPersnalShopMessage().c_str(),g_pUserOption->ChattingColor);
//		else
//			pCreature->SetPersnalString((*g_pGameStringTable)[UI_STRING_MESSAGE_PERSNAL_DEFAULT_MESSGE].GetString(),g_pUserOption->ChattingColor);
//	}
//	else if(CurPernalShop() == 1)
//	{
//		return;
//	}

//	if (m_NextChatFadeTime >= g_CurrentTime || ( CurPernalShop() == 1 && m_NextChatFadeTime == 0 ) )
	if (m_NextChatFadeTime >= g_CurrentTime || ( CurPernalShop() == 1 && GetInputChatting() == false ) )
		return;

	BYTE b = (m_OriChatColor & 0xFF000000) >> 24;

	BYTE color[3] ={ m_OriChatColor & 0xFF, (m_OriChatColor & 0xFF00)>>8, (m_OriChatColor & 0xFF0000)>>16};


	if (b>=g_pClientConfig->MIN_CHATSTRING_COLOR256)
	{
		// 크리스마스 트리용 하드코딩
		if(GetCreatureType() == 482 || GetCreatureType() == 650)
		{
			b -= g_pClientConfig->VALUE_CHATSTRING_FADE*4;
		}
		else
		{
			b -= g_pClientConfig->VALUE_CHATSTRING_FADE;
			if(b > 0)
			{
				color[0] = color[0]*b/0xFF;
				color[1] = color[1]*b/0xFF;
				color[2] = color[2]*b/0xFF;
			}

		}
	}
	else
	{
		// 크리스마스 트리용 하드코딩
		if(GetCreatureType() == 482 || GetCreatureType() == 650)
		{
			const int treeColorMax = 18;
			static COLORREF treeColor[treeColorMax] = 
			{
				RGB(255, 150, 150),
				RGB(150, 255, 150),
				RGB(150, 150, 255),
				RGB(255, 255, 150),
				RGB(150, 255, 255),
				RGB(255, 150, 255),

				RGB(255, 200, 150),
				RGB(200, 255, 150),
				RGB(200, 150, 255),
				RGB(255, 255, 200),
				RGB(200, 255, 255),
				RGB(255, 200, 255),

				RGB(255, 150, 200),
				RGB(150, 255, 200),
				RGB(150, 200, 255),

				RGB(255, 200, 200),
				RGB(200, 255, 200),
				RGB(200, 200, 255),
			};

			// 채팅 색깔
			COLORREF color = 0;
			
			do
			{
				color = treeColor[rand()%treeColorMax];
			}while(color == (m_OriChatColor&0x00FFFFFF));
			
			m_ChatColor = color;//RGB_WHITE;//0xFFFF;
			m_OriChatColor = m_ChatColor;
			m_OriChatColor |= 0xFF000000;
			if(GetCreatureType() == 650 )
				m_NextChatFadeTime = g_CurrentTime + 0xffffff;
			
			return;
		}
		else
		{
			ClearChatString();
		}
	}

	m_NextChatFadeTime = g_CurrentTime + g_pClientConfig->DELAY_CHATSTRING_FADE;
	
	m_ChatColor = RGB(color[0], color[1], color[2]);
	m_OriChatColor = (m_OriChatColor&0xFFFFFF) | (b<<24);
}


//----------------------------------------------------------------------
// Get ChatString
//----------------------------------------------------------------------
// 0 ~ g_pClientConfig->MAX_CHATSTRING-1
// 0이 가장 오래된 String이고 g_pClientConfig->MAX_CHATSTRING-1이 가장 최근에 것으로
// return해야 한다.
//----------------------------------------------------------------------
const char*	
MCreature::GetChatString(BYTE i)
{ 
	//                          i   = 실제로 return되어야 하는 값
	//m_ChatStringCurrent - (3-[0]) = m_ChatStringCurrent;
	//m_ChatStringCurrent - (3-[1]) = m_ChatStringCurrent - 2;
	//m_ChatStringCurrent - (3-[2]) = m_ChatStringCurrent - 1;

	BYTE gap = g_pClientConfig->MAX_CHATSTRING - i;

	if (gap > m_ChatStringCurrent)
	{
		return m_ChatString[m_ChatStringCurrent + g_pClientConfig->MAX_CHATSTRING - gap];
	}
	
	return m_ChatString[m_ChatStringCurrent - gap];	
}

//----------------------------------------------------------------------
// Check AffectStatus
//----------------------------------------------------------------------
// pItem을 this Creature가 사용할 수 있는지의 여부를 체크해서 설정한다.
//----------------------------------------------------------------------
void	
MCreature::CheckAffectStatus(MItem* pItem)
{
	if(pItem->GetItemClass() == ITEM_CLASS_PET_ITEM)
	{
		int timeGap = (timeGetTime() - ((MPetItem *)pItem)->GetUpdateTime())/1000/60;
		
		if(pItem->GetCurrentDurability() <= timeGap )
		{
			pItem->UnSetAffectStatus();
			return;
		}
	}

	//-----------------------------------------------------
	// Slayer
	//-----------------------------------------------------
	switch(GetRace())
	{
	case RACE_SLAYER:
		if (pItem->IsSlayerItem())
		{
			//-----------------------------------------------------
			// 슬레이어인 경우는 STR, DEX, INT체크
			//-----------------------------------------------------
			int reqSTR = pItem->GetRequireSTR();
			int reqDEX = pItem->GetRequireDEX();
			int reqINT = pItem->GetRequireINT();
			int reqSUM = pItem->GetRequireSUM();
			
			int str = GetSTR();
			int dex = GetDEX();
			int inte = GetINT();
			int sum = str+dex+inte;
			
			//-----------------------------------------------------
			// 제한 수치가 없거나 
			// 특정 수치를 하나 만족하는 경우..
			//-----------------------------------------------------
			if (
				//-----------------------------------------------------
				// 성별 검사
				//-----------------------------------------------------
				(pItem->IsGenderForAll() 
				|| pItem->IsGenderForMale() && IsMale()
				|| pItem->IsGenderForFemale() && IsFemale()
				)
				
				//-----------------------------------------------------
				// 필요능력치가 없거나..
				//-----------------------------------------------------
				&& (
				
				//-----------------------------------------------------
				// 있는경우.. 다 만족시켜야 한다.
				//-----------------------------------------------------
				(reqSTR==0 || reqSTR!=0 && str >= reqSTR)
				&&	(reqDEX==0 || reqDEX!=0 && dex >= reqDEX)
				&&	(reqINT==0 || reqINT!=0 && inte >= reqINT)
				&&	(reqSUM==0 || reqSUM!=0 && sum >= reqSUM)
				)					
				)
			{
				pItem->SetAffectStatus();
			}
			else
			{
				pItem->UnSetAffectStatus();
			}
			
			if((pItem->IsGenderForAll() 
				|| pItem->IsGenderForMale() && IsMale()
				|| pItem->IsGenderForFemale() && IsFemale()) &&
				pItem->IsQuestItem() )
			{
				pItem->SetAffectStatus();
			}
		}
		else
		{
			//-----------------------------------------------------
			// 타종족 아이템 사용 불가
			//-----------------------------------------------------
			pItem->UnSetAffectStatus();
		}
		break;

	case RACE_VAMPIRE:
		if (pItem->IsVampireItem())
		{
			int reqLevel =  pItem->GetRequireLevel();

			//-----------------------------------------------------
			// 뱀파이어인 경우는 level로 체크한다.
			//-----------------------------------------------------
			if (
					//-----------------------------------------------------
					// 성별 검사
					//-----------------------------------------------------
					(pItem->IsGenderForAll() 
						|| pItem->IsGenderForMale() && IsMale()
						|| pItem->IsGenderForFemale() && IsFemale()
					)
					&&
					(reqLevel==0 || reqLevel!=0 && GetLEVEL() >= reqLevel)
				)
			{
				pItem->SetAffectStatus();
			}
			else
			{
				pItem->UnSetAffectStatus();
			}
			if((pItem->IsGenderForAll() 
				|| pItem->IsGenderForMale() && IsMale()
				|| pItem->IsGenderForFemale() && IsFemale()) &&
				pItem->IsQuestItem()  )
			{
				pItem->SetAffectStatus();
			}
			

			//-----------------------------------------------------
			// item 종류에 따라서 skill 체크한다.
			//-----------------------------------------------------
			// g_pSkillAvailable은 player의 skill정보이다.
			// 어차피 다른 creature가 CheckAffectStatus()를 사용할 일은 없지만.. --;
			// 일단은 item level체크로만 끝내도록 한다...
			/*
			switch (pItem->GetItemClass())
			{
				//-----------------------------------------------------
				// Portal Item
				//-----------------------------------------------------
				case ITEM_CLASS_VAMPIRE_PORTAL_ITEM :
					if (((MVampirePortalItem*)pItem)->IsMarked())
					{
						if (g_pSkillAvailable->find( MAGIC_BLOODY_TUNNEL ) != g_pSkillAvailable->end())
						{
							pItem->SetAffectStatus();
						}
						else
						{
							pItem->UnSetAffectStatus();
						}
					}
					else
					{
						if (g_pSkillAvailable->find( MAGIC_BLOODY_MARK ) != g_pSkillAvailable->end())
						{
							pItem->SetAffectStatus();
						}
						else
						{
							pItem->UnSetAffectStatus();
						}
					}
				break;

				//-----------------------------------------------------
				// Vampire ETC
				//-----------------------------------------------------
				case ITEM_CLASS_VAMPIRE_ETC :
					if (pItem->GetItemType()==0)
					{
						if (g_pSkillAvailable->find( MAGIC_TRANSFORM_TO_WOLF ) != g_pSkillAvailable->end())
						{
							pItem->SetAffectStatus();
						}
						else
						{
							pItem->UnSetAffectStatus();
						}
					} 
					else if (pItem->GetItemType()==1)
					{
						if (g_pSkillAvailable->find( MAGIC_TRANSFORM_TO_BAT ) != g_pSkillAvailable->end())
						{
							pItem->SetAffectStatus();
						}
						else
						{
							pItem->UnSetAffectStatus();
						}
					}
				break;
			}
			*/
		}
		else
		{
			//-----------------------------------------------------
			// 타종족 아이템 사용 불가
			//-----------------------------------------------------
			pItem->UnSetAffectStatus();
		}
		break;

	case RACE_OUSTERS:
		if (pItem->IsOustersItem())
		{
			int reqLevel =  pItem->GetRequireLevel();

			//-----------------------------------------------------
			// 슬레이어인 경우는 STR, DEX, INT체크
			//-----------------------------------------------------
			int reqSTR = pItem->GetRequireSTR();
			int reqDEX = pItem->GetRequireDEX();
			int reqINT = pItem->GetRequireINT();
			int reqSUM = pItem->GetRequireSUM();
			
			int str = GetSTR();
			int dex = GetDEX();
			int inte = GetINT();
			int sum = str+dex+inte;

			//-----------------------------------------------------
			// 아우스터즈인 경우도-_- level로 체크한다.
			//-----------------------------------------------------
			if (
				(reqLevel==0 || reqLevel!=0 && GetLEVEL() >= reqLevel)
				&&	(reqSTR==0 || reqSTR!=0 && str >= reqSTR)
				&&	(reqDEX==0 || reqDEX!=0 && dex >= reqDEX)
				&&	(reqINT==0 || reqINT!=0 && inte >= reqINT)
				&&	(reqSUM==0 || reqSUM!=0 && sum >= reqSUM)
				)
			{
				pItem->SetAffectStatus();
			}
			else
			{
				pItem->UnSetAffectStatus();
			}
			if(	pItem->IsQuestItem() )
			{
				pItem->SetAffectStatus();
			}

		}
		else
		{
			//-----------------------------------------------------
			// 타종족 아이템 사용 불가
			//-----------------------------------------------------
			pItem->UnSetAffectStatus();
		}
		break;
	}
}

//----------------------------------------------------------------------
// Set Direction To Creature
//----------------------------------------------------------------------
// 다른 Creature를 향해서 바라본다.
//----------------------------------------------------------------------
void	
MCreature::SetDirectionToPosition(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)
{
	// 같은 좌표라면.. 현재 방향으로 둔다.
	if (m_X==sX && m_Y==sY)
	{
		m_Direction = m_CurrentDirection;
	}
	else
	{
		m_Direction = MTopView::GetDirectionToPosition(m_X, m_Y, sX, sY);
	}
}

//----------------------------------------------------------------------
// Change Near Direction
//----------------------------------------------------------------------
// m_CurrentDirection을 m_Direction으로 자연스럽게 바꾼다.
// 그러나, 현재 움직이는 방향(m_DirectionMoved)이 우선이다.
//----------------------------------------------------------------------
bool	
MCreature::ChangeNearDirection()
{
	int toDir;
	
	// action을 보여주는 중이면.. 방향 설정
	if (m_ActionCount<GetActionCountMax())
	{
		toDir = m_Direction;
	}
	// 움직이는 중이면 m_DirectionMoved
	// 단, KnockBack이나 FastMove인 경우에는... 방향 무시..
	else if (m_MoveCount>0 && m_MoveCount<m_MoveCountMax
		&& !m_bKnockBack
		&& !m_bFastMove)		
	{
		toDir = m_DirectionMoved;
	}
	// 아니면.. m_Direction
	else
	{
		toDir = m_Direction;
	}

	// 같으면 바꿀 필요가 없다.
	if (m_CurrentDirection==toDir)
	{
		return false;
	}

	// 다르면.. 
	// 왼쪽으로 돌거나 오른쪽으로 돌게 해야한다.

	// 양수로 만들기 위해서.. 글고.. 속도를 위해서.. &를 사용했다.
	int gap = (((m_CurrentDirection+MAX_DIRECTION) - toDir) & 0x07);//% MAX_DIRECTION;

	// 4, 7
	if (gap==4)
	{
		// 왼쪽이나 오른쪽으로 random하게 돈다.
		if (rand() & 0x01)
		{
			m_CurrentDirection ++;			
		}
		else
		{
			m_CurrentDirection += 7;			
		}
	}
	// 
	else if (gap<4)
	{
		m_CurrentDirection += 7;		
	}
	else
	{
		m_CurrentDirection ++;		
	}

	// 0~7을 유지하도록 한다.
	m_CurrentDirection &= 0x07;

	return true;	
}

//----------------------------------------------------------------------
// Get Counter Direction
//----------------------------------------------------------------------
// 진행 방향의 반대 방향을 구한다.
// 음 이거 static으로 바꾸든지 d를 없애든지 해야는데... - -
//----------------------------------------------------------------------
int
MCreature::GetCounterDirection( int d )
{
	// 반대방향
	d += 4;

	// 0~7까지만 되도록 한다.
	while (d < 8)
	{
		d -= 8;
	}

	return d;
}

//----------------------------------------------------------------------
// Get Direction To Creature
//----------------------------------------------------------------------
// 현재 위치(m_X,m_Y)에서 (sX,sY)를 향해서 바라보는 방향을 결정한다.
//----------------------------------------------------------------------
BYTE	
MCreature::GetDirectionToPosition(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY)
{
	// 같은 좌표라면.. 현재 방향으로 둔다.
	if (m_X==sX && m_Y==sY)
	{
		return m_CurrentDirection;
	}

	return MTopView::GetDirectionToPosition(m_X, m_Y, sX, sY);
}

//----------------------------------------------------------------------
// Get NextPosition()
//----------------------------------------------------------------------
// 현재 Creature의 위치에서 direction방향으로 이동한 경우의 위치
//----------------------------------------------------------------------
void
MCreature::GetNextPosition(BYTE direction, POINT &next)
{
	// 현재 위치에서 다음 방향에 대해서 목표 설정
	next.x = m_X;
	next.y = m_Y;
	
	switch (direction)
	{
		case DIRECTION_LEFTDOWN		: next.x--;	next.y++;	break;
		case DIRECTION_RIGHTUP		: next.x++;	next.y--;	break;
		case DIRECTION_LEFTUP		: next.x--;	next.y--;	break;
		case DIRECTION_RIGHTDOWN	: next.x++;	next.y++;	break;
		case DIRECTION_LEFT			: next.x--;			break;
		case DIRECTION_DOWN			:		next.y++;	break;
		case DIRECTION_UP			:		next.y--;	break;
		case DIRECTION_RIGHT		: next.x++;			break;
	}
}

//----------------------------------------------------------------------
// Get NextPosition()
//----------------------------------------------------------------------
// 현재 Creature의 위치에서 m_CurrentDirection방향으로 이동한 경우의 위치
//----------------------------------------------------------------------
/*
void
MCreature::GetNextPosition(POINT &next)
{
	// 현재 위치에서 다음 방향에 대해서 목표 설정
	next.x = m_X;
	next.y = m_Y;
	
	switch (m_CurrentDirection)
	{
		case DIRECTION_LEFTDOWN		: next.x--;	next.y++;	break;
		case DIRECTION_RIGHTUP		: next.x++;	next.y--;	break;
		case DIRECTION_LEFTUP		: next.x--;	next.y--;	break;
		case DIRECTION_RIGHTDOWN	: next.x++;	next.y++;	break;
		case DIRECTION_LEFT			: next.x--;			break;
		case DIRECTION_DOWN			:		next.y++;	break;
		case DIRECTION_UP			:		next.y--;	break;
		case DIRECTION_RIGHT		: next.x++;			break;
	}
}
*/

//----------------------------------------------------------------------
// Get NextPosition()
//----------------------------------------------------------------------
// 현재 Creature의 위치에서 m_CurrentDirection방향으로 이동한 경우의 위치
//----------------------------------------------------------------------
void
MCreature::GetNextPosition(TYPE_SECTORPOSITION &sX, TYPE_SECTORPOSITION &sY)
{
	// 현재 위치에서 다음 방향에 대해서 목표 설정
	sX = m_X;
	sY = m_Y;
	
	switch (m_Direction)
	{
		case DIRECTION_LEFTDOWN		: sX--;	sY++;	break;
		case DIRECTION_RIGHTUP		: sX++;	sY--;	break;
		case DIRECTION_LEFTUP		: sX--;	sY--;	break;
		case DIRECTION_RIGHTDOWN	: sX++;	sY++;	break;
		case DIRECTION_LEFT			: sX--;			break;
		case DIRECTION_DOWN			:		sY++;	break;
		case DIRECTION_UP			:		sY--;	break;
		case DIRECTION_RIGHT		: sX++;			break;
	}
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
MCreature::ActionMove()
{
	if( GetCreatureType() == 672 )
		return;

	if( HasEffectStatus( EFFECTSTATUS_TRYING ) )
	{
		MZone::EFFECT_MAP::const_iterator itr = g_pZone->GetGroundEffects();
		bool bFind = false;

		for(int i = 0; i < g_pZone->GetGroundEffectCount(); itr++,i++ )
		{
			const MEffect* pEffect = itr->second;

			if( g_pPlayer->GetX() == pEffect->GetX() &&
				g_pPlayer->GetY() == pEffect->GetY() &&
				pEffect->GetFrameID() == EFFECTSPRITETYPE_REGEN_TOWER_GROUND )
			{
				bFind = true;
				break;
			}
		}
		if( bFind == false )
			RemoveEffectStatus( EFFECTSTATUS_TRYING );
	}

	// 목적 Sector에 도달한 경우	
	if (m_MoveCount==0)//m_sX==0 && m_sY==0)// && m_ActionCount==0)
	{
		// 현재 방향으로 한 Sector를 진행하고
		// cX,cY, sX,sY를 다시 지정해줘야 한다.

		//--------------------------------------------
		// 이동 가능한지 check
		//--------------------------------------------
		TYPE_SECTORPOSITION	x, y;

		//--------------------------------------------
		// fast move인 경우는 nextX, nextY에 좌표가 있다.
		//--------------------------------------------
		if (m_bFastMove)
		{
			x = m_NextX;
			y = m_NextY;

			m_NextX = SECTORPOSITION_NULL;					

			//m_CurrentDirection = 
			m_DirectionMove = m_Direction = GetDirectionToPosition( x, y );
		}
		else
		{
			// 현재 방향으로 움직였을 때의 좌표를 결정한다.
			x = m_X; 
			y = m_Y;
			GetPositionToDirection( x, y, m_DirectionMove );
		}

		//--------------------------------------------
		// 움직일 수 있을 경우
		//--------------------------------------------
		//
		// 움직일 수 없더라도 움직여야 한다.
		// Server에서 날아온 정보이기 때문에..
		// 
		//--------------------------------------------
		// [ TEST CODE ]
		/*
		#ifdef	OUTPUT_DEBUG			
			if (!m_pZone->CanMove(m_MoveType, x,y))
			{
				DEBUG_ADD_FORMAT("[Can't Move] But Go! [ID=%d] (%d,%d) --> (%d,%d)",
										m_ID,
										m_X, m_Y,
										x, y);
				
			}
		#endif
		*/

		//#ifdef	OUTPUT_DEBUG
			TYPE_SECTORPOSITION ox = m_X;
			TYPE_SECTORPOSITION oy = m_Y;
		//#endif
		
		{
			// zone의 sector의 정보를 바꿔준다.
			// m_X와 m_Y도 설정해준다.
			if (MovePosition( x, y ))
			{
				#ifdef OUTPUT_DEBUG
					DEBUG_ADD_FORMAT("[Move] [ID=%d] (%d,%d) --> (%d,%d)",
									m_ID,
									ox, oy,
									m_X, m_Y);					

					if (m_DirectionMove >= 8)
					{
						DEBUG_ADD_FORMAT("[Error] Direction exceed MAX : dir=%d", (int)m_DirectionMove);
						return;
					}
				#endif
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

					// 2005, 1, 5, sobeit add start
					if(HasEffectStatus(EFFECTSTATUS_BIKE_CRASH))
					{
//						moveTimes_div_2 = moveTimes_div_2/2;
						m_MoveCountMax = m_MoveCountMax*2;
					}
					// 2005, 1, 5, sobeit add end

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
					m_DirectionMoved = m_DirectionMove;
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
					else if (m_MoveDevice==MOVE_DEVICE_SUMMON_SYLPH || HasEffectStatusSummonSylph( this ))
					{
						moveTimes_1 = (*g_pCreatureTable)[m_CreatureType].MoveTimesMotor-1;
					}
					else
					{
						DEBUG_ADD_FORMAT("[Error] What is MoveType of this Creature??? id=%d", m_ID);
						
						SetStop();

						return;
					}

					m_MoveTableCount = 0;
					m_cX = m_cXTable[moveTimes_1][m_DirectionMoved][m_MoveTableCount];
					m_cY = m_cYTable[moveTimes_1][m_DirectionMoved][m_MoveTableCount];

					//m_ActionCount++;
					m_MoveCount++;
				}
				
				m_sX += m_cX;
				m_sY += m_cY;	
				
				// 다음에 이동할 count를 지정한다.
				// frame만 바뀌는 경우와
				// frame이 바뀌고 이동하는 경우가 있다.
				m_NextMoveCount = (*g_pCreatureTable)[m_CreatureType].MoveRatio;				

				//------------------------------------------------
				// 캐릭터의 MoveAction에 맞는 Sound를 출력해준다.
				//------------------------------------------------
				TYPE_SOUNDID soundID = (*g_pCreatureTable)[m_CreatureType].GetActionSound( m_MoveAction );

				if (soundID!=SOUNDID_NULL  )
				{
					PlaySound( soundID,
								false,
								m_X,
								m_Y);
				}

				DEBUG_ADD("[Move] OK");				
			}
			// 못 움직인 경우
			else
			{
				// 멈춤
				SetAction( ACTION_STAND );	
			}			
		}
		//--------------------------------------------
		// 움직일 수 없을 경우
		//--------------------------------------------
	}
	// 아직 다음 Sector까지 덜 도달한 경우
	else
	{
		// 한 Frame을 이동한다.
		/*
		switch (m_MoveDevice)
		{
			case MOVE_DEVICE_NULL :			
				m_sX += m_cXTableDeviceNull[m_CurrentDirection][m_ActionCount];
				m_sY += m_cYTableDeviceNull[m_CurrentDirection][m_ActionCount];
			break;

			case MOVE_DEVICE_MOTOR1 :						
				m_sX += m_cXTableDeviceMotor1[m_CurrentDirection][m_ActionCount];
				m_sY += m_cYTableDeviceMotor1[m_CurrentDirection][m_ActionCount];
			break;
		}
		*/
		//m_sX += m_cX;
		//m_sY += m_cY;

		// 이동할 count가 됐을때만 이동한다.
		if (m_MoveCount>=m_NextMoveCount)
		{			
			//--------------------------------------------
			// Fast Move 인 경우
			//--------------------------------------------
			if (m_bFastMove)
			{
				m_sX += m_cX;
				m_sY += m_cY;

				m_MoveCount += 2;

				if (m_MoveCount > m_MoveCountMax)
				{
					m_MoveCount = m_MoveCountMax;
				}

				// 다음에 이동할 count를 지정한다.
				m_NextMoveCount += (*g_pCreatureTable)[m_CreatureType].MoveRatio;

				//----------------------------------------------------------
				// fast move 후에 한번 때리기.. - -
				// Flash Sliding
				//----------------------------------------------------------
				if (m_MoveCount+1 >= m_MoveCountMax
					&& m_nNextUsedActionInfo!=m_nBasicActionInfo)
				{
					MCreature* pCreature = g_pZone->GetCreature( m_TraceID );

					if (pCreature!=NULL 
						&& m_nSpecialActionInfo != MAGIC_RAPID_GLIDING 
						&& m_nSpecialActionInfo != SKILL_TELEPORT 
						&& m_nSpecialActionInfo != SKILL_CHARGING_ATTACK 
						&& !HasEffectStatus(EFFECTSTATUS_BIKE_CRASH))
					{
						//---------------------------------------------------
						// 상대가 맞는 모습
						//---------------------------------------------------
						MActionResult* pResult = new MActionResult;
						
						int ActionInfo = GetBasicActionInfo();//m_nBasicActionInfo;

						if(m_nSpecialActionInfo == SKILL_BLITZ_SLIDING || m_nSpecialActionInfo == SKILL_BLAZE_WALK )
						{
							ActionInfo = SKILL_BLITZ_SLIDING_ATTACK;

							if( m_nSpecialActionInfo == SKILL_BLAZE_WALK )
								ActionInfo = SKILL_BLAZE_WALK_ATTACK;
							
							SetDirection( MTopView::GetDirectionToPosition( GetX(), GetY(), pCreature->GetX(), pCreature->GetY() ) );
						}
						else if(m_nSpecialActionInfo == SKILL_SET_AFIRE)
						{
							ActionInfo = RESULT_SKILL_SET_AFIRE;
						}
//						if(m_nSpecialActionInfo == MAGIC_BLOODY_ZENITH)
//						{
//							SetBloodyZenith(16);
//						}						

						// 결과 표현
						SetDirection( MTopView::GetDirectionToPosition( GetX(), GetY(), pCreature->GetX(), pCreature->GetY() ) );
						pResult->Add( new MActionResultNodeActionInfo( 
														ActionInfo,
														m_ID,
														pCreature->GetID(), 
														pCreature->GetX(),
														pCreature->GetY(),
														0 ) 
										);	
					
						PacketSpecialActionToOther( ActionInfo, 
													m_TraceID, 
													pResult );						
					}
				}
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
				else if (m_MoveDevice==MOVE_DEVICE_SUMMON_SYLPH || HasEffectStatusSummonSylph( this ))
				{
					moveTimes_1 = (*g_pCreatureTable)[m_CreatureType].MoveTimesMotor-1;
				} 
				else
				{
					DEBUG_ADD_FORMAT("[Error] MoveTimes Error. MCreature.cpp Lines:%d",__LINE__);
					return;
				}

				m_MoveTableCount++;
				if(m_MoveTableCount > moveTimes_1)
					m_MoveTableCount = moveTimes_1;
				
				m_cX = m_cXTable[moveTimes_1][m_DirectionMoved][m_MoveTableCount];
				m_cY = m_cYTable[moveTimes_1][m_DirectionMoved][m_MoveTableCount];

				m_sX += m_cX;
				m_sY += m_cY;

				// 다음에 이동할 count를 지정한다.
				m_NextMoveCount += (*g_pCreatureTable)[m_CreatureType].MoveRatio;

				m_MoveCount++;
			}
		
		}
		else
		{
			//m_ActionCount++;
			m_MoveCount++;
		}	


		// frame을 바꿔준다. 마지막 Frame까지 갔으면 0번째 Frame으로 바꾼다.
		//if (++m_ActionCount == (*m_pFrames)[m_Action][m_CurrentDirection].GetCount())
		//	m_ActionCount = 0;
	}
}

//----------------------------------------------------------------------
// Attach CastingEffect
//----------------------------------------------------------------------
void	
MCreature::AttachCastingEffect(TYPE_ACTIONINFO nUsedActionInfo, BOOL bForceAttach)
{
	DEBUG_ADD_FORMAT("[CheckError] AttachCastingEffect(%d, %d)", nUsedActionInfo, bForceAttach);	
	#ifdef OUTPUT_DEBUG					
		if (g_bCheckError)
		{
			DEBUG_ADD_FORMAT("[CheckError] AttachCastingEffect(%d, %d)", nUsedActionInfo, bForceAttach);
		}
	#endif

	if (nUsedActionInfo!=ACTIONINFO_NULL)
	{
		int		castingStartFrame = GetActionInfoCastingStartFrame(nUsedActionInfo);
		BOOL	bCastingEffect	= bForceAttach || (castingStartFrame==m_ActionCount);

		//-------------------------------------------------------------
		// Casting Effect
		//-------------------------------------------------------------
		if (bCastingEffect)
		{
			if (GetActionInfoActionEffectSpriteType(nUsedActionInfo) != EFFECTSPRITETYPE_NULL)
			{
				int castingFrames = GetActionInfoCastingFrames(nUsedActionInfo);
			
				// 설정이 안 돼 있으면.. action 동작에 맞춘다.
				if (castingFrames==0)
				{
					int actionInfoAction = GetActionInfoAction(nUsedActionInfo);
						
					//-------------------------------------------------
					// 오토바이를 타고 있는 경우
					//-------------------------------------------------				
					if (m_MoveDevice==MOVE_DEVICE_RIDE)
					{
						//castingFrames = (*g_pCreatureTable)[m_CreatureType].GetActionCount( actionInfoAction );					
						m_ActionCountMax = GetCreatureActionCountMax( this, actionInfoAction );
					}					
					else
					{	
						//-------------------------------------------------
						// 흡혈 동작중에 공격받은 경우
						//-------------------------------------------------
						if (actionInfoAction==ACTION_DAMAGED
							//&& (// 흡혈 당하는 동작 중이거나 
								//m_Action==ACTION_DRAINED
								// 뱀파이어일때, 흡혈하는 동작 중에는 damaged를 안 보여준다.
								//|| IsVampire() && m_Action==ACTION_VAMPIRE_DRAIN
							//	)
							)
						{
							//castingFrames = (*g_pCreatureTable)[m_CreatureType].GetActionCount( ACTION_DAMAGED );
							castingFrames = GetCreatureActionCountMax( this, ACTION_DAMAGED );
						}
						else
						{
							castingFrames = (int)GetActionCountMax() - (int)castingStartFrame;

							// 임시로..
							// 여기서 음수가 나오는 경우.. 어케 하지.. - -;
							if (castingFrames < 0)
							{
								castingFrames = 16;
							}
						}
					}					
				}

				TYPE_EFFECTSPRITETYPE effectSpriteType = GetActionInfoActionEffectSpriteType(nUsedActionInfo);				

				MCreature* targetCreature = NULL;

				//-------------------------------------------------
				// casting이 누구한테 붙는가?
				//-------------------------------------------------
				if(nUsedActionInfo != ACTIONINFO_NULL)
				{
					if ((*g_pActionInfoTable)[nUsedActionInfo].IsCastingEffectToSelf())
					{
						targetCreature = this;
					}
					else
					{
						targetCreature = g_pZone->GetCreature( m_TraceID );					
					}
				}


				if (targetCreature!=NULL)
				{
					// [ TEST CODE ]
					// Creature에게 붙이는 Effect를 생성해서 pointer를 넘겨받는다.
					MAttachEffect* pEffect = targetCreature->CreateAttachEffect( 
																effectSpriteType, 
																//(*g_pActionInfoTable)[nUsedActionInfo].GetDelay() 
																castingFrames,
																castingFrames
															);
		
					if (pEffect != NULL)
					{
						pEffect->SetDirection( targetCreature->GetDirection() );

						pEffect->SetLink( nUsedActionInfo, NULL );

						// 붙어야 하는 캐릭터
						pEffect->SetAttachCreatureID( targetCreature->GetID() );
						
						#ifdef OUTPUT_DEBUG
						
							if (m_MoveDevice==MOVE_DEVICE_RIDE)
							{
								
							}
							else
							{	
								//-------------------------------------------------
								// 흡혈 동작중에 공격받은 경우
								//-------------------------------------------------
								if (GetActionInfoAction(nUsedActionInfo)==ACTION_DAMAGED)
								{
									DEBUG_ADD_FORMAT("[ATTACH] damaged. id=%d, ai=%d, est=%d, frames=%d", m_ID, nUsedActionInfo, (int)effectSpriteType, (int)castingFrames);
									DEBUG_ADD_FORMAT("Action[%d]=%d/%d, Move=%d/%d, Frame=%d", m_Action, m_ActionCount, GetActionCountMax(), m_MoveCount, m_MoveCountMax, GetFrame());

									g_bCheckError = true;
								}
							}
						#endif
					}
				}
			}

			//-------------------------------------------------------------
			// Casting ActionInfo 표현
			//-------------------------------------------------------------
			TYPE_ACTIONINFO castingActionInfo = (*g_pActionInfoTable)[nUsedActionInfo].GetCastingActionInfo();
			if (castingActionInfo!=ACTIONINFO_NULL)
			{
				#ifdef OUTPUT_DEBUG
					
					DEBUG_ADD_FORMAT("AffectUsedActionInfo(%d)", (int)castingActionInfo);
					
					AffectUsedActionInfo(castingActionInfo);					

					DEBUG_ADD("castingActionInfo OK");

				#else

					AffectUsedActionInfo(castingActionInfo);

				#endif
			}

			//-------------------------------------------------------------
			// casting 동작에 맞는 sound
			//-------------------------------------------------------------
			PlaySound( GetCastingSoundID(nUsedActionInfo) ,
						false,
						m_X, m_Y);
		}
	}

	#ifdef OUTPUT_DEBUG					
		if (g_bCheckError)
		{
			DEBUG_ADD_FORMAT("[CheckError] AttachCastingEffect OK~~");
		}
	#endif
}

//----------------------------------------------------------------------
// Set Underground Creature
//----------------------------------------------------------------------
void	
MCreature::SetUndergroundCreature()		
{ 
	if (m_pZone!=NULL)
	{
		m_pZone->UnSetServerBlock(m_MoveType, m_ServerX, m_ServerY);
	}

	m_MoveType = CREATURE_UNDERGROUND; 
	m_Z = 0;

	CheckRegen();
}

//----------------------------------------------------------------------
// Set Ground Creature
//----------------------------------------------------------------------
void	
MCreature::SetGroundCreature()				
{ 
	if (m_pZone!=NULL)
	{
		m_pZone->UnSetServerBlock(m_MoveType, m_ServerX, m_ServerY);
	}

	m_MoveType = CREATURE_GROUND; 
	m_Z=0; 

	CheckRegen();
}

//----------------------------------------------------------------------
// Set Flying Creature
//----------------------------------------------------------------------
void	
MCreature::SetFlyingCreature()				
{ 
	if (m_pZone!=NULL)
	{
		m_pZone->UnSetServerBlock(m_MoveType, m_ServerX, m_ServerY);
	}

	m_MoveType = CREATURE_FLYING; 
	m_Z = (*g_pCreatureTable)[m_CreatureType].FlyingHeight; 
	
	CheckRegen();
}

//----------------------------------------------------------------------
// Action Effect
//----------------------------------------------------------------------
void	
MCreature::ActionEffect()
{	
	//-------------------------------------------------------------
	// 사용 기술이 정해져 있는 경우..
	//-------------------------------------------------------------
	if (m_nUsedActionInfo!=ACTIONINFO_NULL)
	{
		BOOL	bActionStand	= (m_Action==ACTION_STAND || IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND || IsOusters() && m_Action==ACTION_OUSTERS_FAST_MOVE_STAND);
		int		actionCountMax_1 = GetActionCountMax()-1;
		BOOL	bStartAction	= (m_ActionCount==0);
		BOOL	bEndAction		= (m_ActionCount==actionCountMax_1) || bActionStand;
		int		StartFrame		= (*g_pActionInfoTable)[m_nUsedActionInfo].GetStartFrame( m_WeaponSpeed );

		// Effect가 시작되는 경우는..
		// (1) StartFrame인 경우
		// (2) 마지막 ActionFrame인 경우
		BOOL bStartEffect = m_ActionCount==StartFrame || 
							StartFrame >= GetActionCountMax() && bEndAction;

		bStartAction |= (*g_pActionInfoTable)[m_nUsedActionInfo].IsUseRepeatFrame()
					&& m_RepeatCount!=0
					&& m_ActionCount == GetActionInfoRepeatStartFrame( m_nUsedActionInfo );

			
		//BOOL	bCastingEffect	= GetActionInfoCastingStartFrame(m_nUsedActionInfo)==m_ActionCount;

		//-------------------------------------------------------------
		// 시작 동작
		//-------------------------------------------------------------
		if (bStartAction)
		{
			//------------------------------------------------
			// 기술의 동작에 맞는 sound를 출력해준다.
			//------------------------------------------------
			//g_Sound.Play( g_SoundTable[(*g_pActionInfoTable)[m_nUsedActionInfo].GetSoundID()].pDSBuffer );
			//PlaySound( (*g_pActionInfoTable)[m_nUsedActionInfo].GetSoundID() ,
			//			false,
			//			m_X, m_Y);

			//------------------------------------------------
			// 캐릭터의 Action에 맞는 Sound를 출력해준다.
			//------------------------------------------------
			PlaySound( 
				(*g_pCreatureTable)[m_CreatureType].GetActionSound( 
					GetActionInfoAction(m_nUsedActionInfo)
				),
				false,
				m_X, m_Y
			);
		}

		//-------------------------------------------------------------
		// Casting Effect
		//-------------------------------------------------------------
		AttachCastingEffect( m_nUsedActionInfo );	

		#ifdef OUTPUT_DEBUG					
			if (g_bCheckError)
			{
				DEBUG_ADD("[CheckError] ActionEffect");
			}
		#endif


		//-------------------------------------------------------------
		// 정지 동작이면..
		// 바로 effect를 보여준다.
		//-------------------------------------------------------------
		if (bActionStand)
		{
			m_ActionCount = GetActionCountMax()-1;
		}

		//-------------------------------------------------------------
		// 기술을 보여줘야 하는 경우
		//-------------------------------------------------------------
		if (bStartEffect)
		{
			AffectUsedActionInfo(m_nUsedActionInfo);
			m_nUsedActionInfo	= ACTIONINFO_NULL;
		}
	}
	else
	{
		//if (m_ActionCount==m_ActionCountMax-1)		
		{
			//--------------------------------------------------------
			// Action끝
			//--------------------------------------------------------	
			//m_Action			= ACTION_STAND;
		}
	}

	m_ActionCount++;

	//--------------------------------------------------------	
	// 임시로 흡혈동작 중지하는거..
	//--------------------------------------------------------	
	if (m_bStopBloodDrain)
	{
		StopBloodDrain();
	}
	if (m_bStopAbsorbSoul)
	{
		StopAbsorbSoul();
	}

	#ifdef OUTPUT_DEBUG					
		if (g_bCheckError)
		{
			DEBUG_ADD("[CheckError] ActionEffect OK");
		}
	#endif
}

//----------------------------------------------------------------------
// Affect Used ActionInfo
//----------------------------------------------------------------------
// 기술 사용한걸 표현한다.
//----------------------------------------------------------------------
void
MCreature::AffectUsedActionInfo(TYPE_ACTIONINFO nUsedActionInfo)
{
	bool	bMonster = !IsPlayerOnly();
	bool	SpecialID = false;
	#ifdef OUTPUT_DEBUG					
		if (g_bCheckError)
		{
			DEBUG_ADD_FORMAT("[CheckError] AffectUsedActionInfo(%d)", nUsedActionInfo);
		}
	#endif

	if (nUsedActionInfo==ACTIONINFO_NULL)
	{
		#ifdef OUTPUT_DEBUG					
			if (g_bCheckError)
			{
				DEBUG_ADD("[CheckError] AffectUsedActionInfo NULL -> return");
			}
		#endif

		return;
	}

	DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d %d",__LINE__,nUsedActionInfo);

	if ((*g_pActionInfoTable)[nUsedActionInfo].GetSize()!=0)
	{
		POINT point;	

		//--------------------------------------------------------
		// Casting ActionInfo인가?
		//--------------------------------------------------------
		BOOL bCastingAction = (*g_pActionInfoTable)[nUsedActionInfo].IsCastingAction();
		DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);


		//--------------------------------------------------------
		// 목표 위치 Pixel좌표
		//--------------------------------------------------------
		point = MTopView::MapToPixel(m_TraceX, m_TraceY);
		//point.x += m_sX;
		//point.y += m_sY;
		//point.x += TILE_X_HALF;

		//--------------------------------------------------------
		//
		//					Effect 목표 설정
		//
		//--------------------------------------------------------
		MEffectTarget* pEffectTarget = NULL;

		//--------------------------------------------------------
		// casting action인 경우
		//--------------------------------------------------------
		if (bCastingAction)
		{
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			pEffectTarget = new MEffectTarget( (*g_pActionInfoTable)[nUsedActionInfo].GetSize() );
			pEffectTarget->NewEffectID();		
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);

			if( nUsedActionInfo == CASTING_FIRE || nUsedActionInfo == CASTING_WATER ||
				nUsedActionInfo == CASTING_GROUND || nUsedActionInfo == CASTING_FIRE2 ||
				nUsedActionInfo == CASTING_GROUND2 || nUsedActionInfo == CASTING_WATER2 )
			{
				pEffectTarget->Set( point.x, point.y, m_TraceZ, GetID() );
			} else
			{
				
				pEffectTarget->Set( point.x, point.y, m_TraceZ, m_TraceID );				
			}
			pEffectTarget->SetDelayFrame( m_EffectDelayFrame );

			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);

			// castingAction에서는 결과처리가 필요없다.
		}
		//--------------------------------------------------------
		// casting action이 아닌.. 실제 기술인 경우..
		//--------------------------------------------------------
		else
		{
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			pEffectTarget = new MEffectTarget( (*g_pActionInfoTable)[nUsedActionInfo].GetSize() );
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d %d",__LINE__,(*g_pActionInfoTable)[nUsedActionInfo].GetSize());

			pEffectTarget->NewEffectID();
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d %d",__LINE__,m_nUsedActionInfo);

			//pEffectTarget->Set( point.x, point.y, m_TraceZ, m_TraceID );
			
//			if(m_nUsedActionInfo == MAGIC_ACID_TOUCH || m_nUsedActionInfo == MAGIC_BLOODY_NAIL 
//				|| m_nUsedActionInfo == SKILL_FLOURISH || m_nUsedActionInfo == SKILL_ABSORB_SOUL)
			if(m_nUsedActionInfo != ACTIONINFO_NULL && (*g_pActionInfoTable)[m_nUsedActionInfo].IsAttachSelf() )
			{
				SpecialID = true;
//				if(!bMonster)
				{
					DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
					pEffectTarget->Set( point.x, point.y, m_TraceZ, GetID() );					
				}
//				else
//					pEffectTarget->Set( point.x, point.y, m_TraceZ, m_TraceID );

				DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			}
			else
			{
				DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
				pEffectTarget->Set( point.x, point.y, m_TraceZ, m_TraceID );
			}
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);

			//--------------------------------------------------------
			// 지속 시간 설정
			//--------------------------------------------------------

			if (m_DelayActionInfo==nUsedActionInfo)
			{
				DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
				pEffectTarget->SetDelayFrame( m_EffectDelayFrame );
				m_DelayActionInfo	= ACTIONINFO_NULL;
				m_EffectDelayFrame	= 0;
				DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			}
			
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			// 결과 설정 : 결과 처리는 EffectGenerator에 맡긴다.
			pEffectTarget->SetResult( m_pActionResult );
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			
			// 설정된 결과를 없게 한다.
			m_pActionResult = NULL;
		}

		//--------------------------------------------------------
		//
		//					시작 위치를 결정한다.
		//
		//--------------------------------------------------------
		int x,y,z, direction;

		DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
		//--------------------------------------------------------
		// User 위치에서 시작하는 경우
		//--------------------------------------------------------
		if ((*g_pActionInfoTable)[nUsedActionInfo].IsStartUser())
		{
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			point = MTopView::MapToPixel(m_X, m_Y);
			point.x += m_sX;
			point.y += m_sY;
			//point.x += TILE_X_HALF;

			x			= point.x;
			y			= point.y;
			z			= m_Z;//+60;			
		}
		//--------------------------------------------------------
		// Target 위치에서 시작하는 경우
		//--------------------------------------------------------
		else if ((*g_pActionInfoTable)[nUsedActionInfo].IsStartTarget())
		{
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			x			= point.x;
			y			= point.y;
			z			= m_Z;//+60;			
		}

		//--------------------------------------------------------
		// 공중에서 시작하는 경우
		//--------------------------------------------------------
		if ((*g_pActionInfoTable)[nUsedActionInfo].IsStartSky())
		{
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			z	= m_Z + (*g_pActionInfoTable)[nUsedActionInfo].GetValue();

			direction	= DIRECTION_DOWN;
		}
		//--------------------------------------------------------
		// 지상에서 시작하는 경우
		//--------------------------------------------------------
		else
		{
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
			direction	= m_Direction;
		}

		//--------------------------------------------------------
		//
		//                   Effect생성		
		//
		//--------------------------------------------------------
		
		DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
		(*g_pEffectGeneratorTable).Generate(
				x,y,z,				// 시작 위치
				direction, 		// 방향
		//		1,					// power
				SpecialID ? bMonster ? 2 : 1 : 1,
				nUsedActionInfo,		//	ActionInfoTable종류,
				pEffectTarget,		// 목표 정보
				GetActionGrade()	
		);
		DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
		ClearActionGrade();		

		DEBUG_ADD_FORMAT("[Creature::AffectUsedActionInfo] ai=%d, id=%d", nUsedActionInfo, m_ID);		
	}
	else
	{
		//------------------------------------------------------------
		// 결과를 처리해야하는 시점인가? - 당연하다고 본다 *_*;
		//------------------------------------------------------------				
		DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
		if (m_pActionResult != NULL)
		{					
			// (!) m_pActionResult값이 Execute에서 변할 수 있어서 
			//		저장했다가 지워줘야 한다.
			MActionResult* pOldResult = m_pActionResult;

			m_pActionResult = NULL;
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);

			pOldResult->Execute();
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);

			delete pOldResult;	
			DEBUG_ADD_FORMAT("[AffectUsedActionInfo] %d",__LINE__);
		}				
	}

	//--------------------------------------------------------
	// Action끝
	//--------------------------------------------------------	
	//m_Action			= ACTION_STAND;	


	#ifdef OUTPUT_DEBUG					
		if (g_bCheckError)
		{
			DEBUG_ADD_FORMAT("[CheckError] AffectUsedActionInfo OKOK ~~");
		}
	#endif
}

//----------------------------------------------------------------------
// Affect Buffered ActionInfo
//----------------------------------------------------------------------
// NextUsedActionInfo가 있다면
// UsedActionInfo --> NextUsedActionInfo를 모두 적용한다.
//
// UsedActionInfo만 있는 경우는 처리 안한다.
//----------------------------------------------------------------------
void
MCreature::AffectBufferedActionInfo()
{
	DEBUG_ADD_FORMAT("[AffectBufferedActionInfo] ai=%d, nai=%d, action=%d-%d/%d", m_nUsedActionInfo, m_nNextUsedActionInfo, (int)m_Action, (int)m_ActionCount, (int)GetActionCountMax());
	
	//--------------------------------------------------------
	// 다음에 사용할려는 기술이 있다면... 바로 적용 시킨다.
	//--------------------------------------------------------
	if (m_nNextUsedActionInfo!=ACTIONINFO_NULL)
	{	
		TYPE_ACTIONINFO nextActionInfo = m_nNextUsedActionInfo;
		DEBUG_ADD("[AffectBufferedActionInfo] Into Here");
		
		//--------------------------------------------------------
		// 현재 사용해야할 기술이 있는 경우 
		//--------------------------------------------------------
		if (m_nUsedActionInfo!=ACTIONINFO_NULL)
		{
			DEBUG_ADD("[AffectBufferedActionInfo] AttachCastingEffect" );
			
			m_nNextUsedActionInfo = ACTIONINFO_NULL;
		
			// m_nUsedActionInfo 적용
			// --> 내부에서 m_nNextUsedActionInfo가 바뀔 수 있다.

	// 이 경우는 무조건(!) CastingEffect를 붙여야 한다.
			AttachCastingEffect( m_nUsedActionInfo, TRUE );

			#ifdef OUTPUT_DEBUG					
				if (g_bCheckError)
				{
					DEBUG_ADD("[CheckError] AffectBufferedActionInfo");
				}
			#endif

			AffectUsedActionInfo( m_nUsedActionInfo );

			#ifdef OUTPUT_DEBUG					
				if (g_bCheckError)
				{
					DEBUG_ADD("[CheckError] AffectUsedActionInfo OK");
				}
			#endif

			m_nUsedActionInfo	= ACTIONINFO_NULL;			

			// affect하고 나서.. 사용해야할 기술이 생긴다면..
			if (m_nNextUsedActionInfo!=ACTIONINFO_NULL)
			{	
				#ifdef OUTPUT_DEBUG					
					if (g_bCheckError)
					{
						DEBUG_ADD_FORMAT("[CheckError] AttachCastingEffect( %d )", m_nNextUsedActionInfo);

						AttachCastingEffect( m_nNextUsedActionInfo, TRUE );

						DEBUG_ADD_FORMAT("[CheckError] AffectUsedActionInfo( %d )", m_nNextUsedActionInfo);

						AffectUsedActionInfo( m_nNextUsedActionInfo );

						DEBUG_ADD_FORMAT("[CheckError] m_nNextUsedActionInfo!=ACTIONINFO_NULL OK");
					}
					else
					{
						AttachCastingEffect( m_nNextUsedActionInfo, TRUE );
						AffectUsedActionInfo( m_nNextUsedActionInfo );
					}
				#else
					AttachCastingEffect( m_nNextUsedActionInfo, TRUE );
					AffectUsedActionInfo( m_nNextUsedActionInfo );
				#endif
			}
		}
		
		//--------------------------------------------------------
		// 다음에 사용해야할 기술 적용
		//--------------------------------------------------------
		m_nNextUsedActionInfo = ACTIONINFO_NULL;

		// --> 내부에서 m_nNextUsedActionInfo가 바뀔 수 있다.
		DEBUG_ADD("~~~~~zzzz");
		AttachCastingEffect( nextActionInfo, TRUE );
		DEBUG_ADD("~~~~~zzzz2");
		AffectUsedActionInfo( nextActionInfo );
			
		// affect하고 나서.. 사용해야할 기술이 생긴다면..
		if (m_nNextUsedActionInfo!=ACTIONINFO_NULL)
		{	
			DEBUG_ADD("~~~~~zzzz3");
			AttachCastingEffect( m_nNextUsedActionInfo, TRUE );
			AffectUsedActionInfo( m_nNextUsedActionInfo );
		}

		m_nUsedActionInfo = ACTIONINFO_NULL;
		
		m_nNextUsedActionInfo = ACTIONINFO_NULL;

		#ifdef OUTPUT_DEBUG					
			if (g_bCheckError)
			{
				DEBUG_ADD("[CheckError] AffectBufferedActionInfo OKOK -_-;");
			}
		#endif
	}
	//--------------------------------------------------------
	// 현재 사용해야할 기술만 있는 경우인가?
	//--------------------------------------------------------
	else if (m_nUsedActionInfo!=ACTIONINFO_NULL)
	{
		DEBUG_ADD("[AFFE] USEDACTIONINFO != NULL ");
		m_nNextUsedActionInfo = ACTIONINFO_NULL;
	
		// m_nUsedActionInfo 적용
		// --> 내부에서 m_nNextUsedActionInfo가 바뀔 수 있다.

		// 이 경우는 무조건(!) CastingEffect를 붙여야 한다.
		DEBUG_ADD("[AFFE] CASTING EFF");
		AttachCastingEffect( m_nUsedActionInfo, TRUE );
		DEBUG_ADD("[AFFE] AFFECT USED");
		AffectUsedActionInfo( m_nUsedActionInfo );

		m_nUsedActionInfo	= ACTIONINFO_NULL;			

		// affect하고 나서.. 사용해야할 기술이 생긴다면..
		if (m_nNextUsedActionInfo!=ACTIONINFO_NULL)
		{			
			DEBUG_ADD("[AFFE] ZZZZZZ ");
			AttachCastingEffect( m_nNextUsedActionInfo, TRUE );
			AffectUsedActionInfo( m_nNextUsedActionInfo );
		}	

		//--------------------------------------------------------
		// 다음에 사용해야할 기술 적용
		//--------------------------------------------------------
		m_nUsedActionInfo = ACTIONINFO_NULL;
		m_nNextUsedActionInfo = ACTIONINFO_NULL;
	}
}

//----------------------------------------------------------------------
// Is Recovery HP
//----------------------------------------------------------------------
inline BOOL				
MCreature::IsRecoveryHP()	
{ 
	return m_RecoveryHPTimes > 0;
	//return m_RecoveryPart==MODIFY_CURRENT_HP; 
}

//----------------------------------------------------------------------
// Is Recovery MP
//----------------------------------------------------------------------
inline BOOL				
MCreature::IsRecoveryMP()	
{ 
	return m_RecoveryMPTimes > 0;
	//return m_RecoveryPart==MODIFY_CURRENT_MP;
}		

//----------------------------------------------------------------------
// Set Recovery
//----------------------------------------------------------------------
// amount만큼 times번씩 delay마다 회복한다.
//----------------------------------------------------------------------
void				
MCreature::SetRecoveryHP(int amount, int times, DWORD delay)
{ 
	if (times > 0)
	{
		// 바로 한번 회복

		if(! IsRecoveryHP() )					// 현재 HP 가 차고 있지 않으면
		{
			SetStatus( MODIFY_CURRENT_HP, GetHP()+amount );
			m_RecoveryHPTimes = times - 1; 
		} else
			m_RecoveryHPTimes = times;

		m_RecoveryHPAmount = amount; 
		m_RecoveryHPDelayTime = delay;
		//m_RecoveryHPPart = MODIFY_CURRENT_HP;

		// 다음 회복할 시간 설정
		m_RecoveryHPNextTime = g_CurrentTime + m_RecoveryHPDelayTime;
	}
}

//----------------------------------------------------------------------
// Set Recovery
//----------------------------------------------------------------------
// amount만큼 times번씩 delay마다 회복한다.
//----------------------------------------------------------------------
void				
MCreature::SetRecoveryMP(int amount, int times, DWORD delay)
{ 
	// 2004, 9, 3, sobeit add start - 플레져 스테이션에 맞으면 MP가 안찬다...-_-
	if(HasEffectStatus(EFFECTSTATUS_PLEASURE_EXPLOSION))
		return;
	// 2004, 9, 3, sobeit add end
	if (times > 0)
	{
		// 바로 한번 회복
		if(! IsRecoveryMP() )
		{
			SetStatus( MODIFY_CURRENT_MP, GetMP()+amount );
			m_RecoveryMPTimes = times-1; 
		} else
			m_RecoveryMPTimes = times; 

		m_RecoveryMPAmount = amount; 		
		m_RecoveryMPDelayTime = delay;
		//m_RecoveryMPPart = MODIFY_CURRENT_MP;

		// 다음 회복할 시간 설정
		m_RecoveryMPNextTime = g_CurrentTime + m_RecoveryMPDelayTime;
	}
}

//----------------------------------------------------------------------
// Check Drop Blood
//----------------------------------------------------------------------
// HP 낮을 때.. 피 흘리기
//----------------------------------------------------------------------
void				
MCreature::CheckDropBlood()
{
	if (!HasEffectStatus(EFFECTSTATUS_COMA))
	{
		//----------------------------------------------------------
		// 피 흘릴 시간이 되었는지(-_-;) 체크..
		//----------------------------------------------------------
		if (g_pUserOption->BloodDrop 
			&& g_CurrentTime > m_NextBloodingTime)
		{
			int currentHP = m_Status[MODIFY_CURRENT_HP];
			int maxHP = m_Status[MODIFY_MAX_HP];

			int percentHP = ((maxHP==0)? 0 : currentHP*100 / maxHP);

			//----------------------------------------------------------
			// HP 30% 이하일때 피 흘린다.
			//----------------------------------------------------------
			if (percentHP <= g_pClientConfig->BLOOD_DROP_HP_PERCENT)
			{
				ExecuteActionInfoFromMainNode(
							BLOOD_DROP_GROUND,										// 사용 기술 번호
						
							m_X, m_Y, 0,
							(int)m_Direction,														// 사용 방향
							
							m_ID,												// 목표에 대한 정보
							m_X, m_Y, 0,
							
							80, //5*16, 
							
							NULL,
							
							false,
							
							(int)m_sX, (int)m_sY);	
			}

			//----------------------------------------------------------
			// 다음 피 흘리기 가능한 시간 설정. 
			// 좀 늦게 체크해도 관계없으므로 피 흘리거나 말거나..시간 확장
			//----------------------------------------------------------
			DWORD timeGap = 300 //+ 1000	//g_pClientConfig->BLOOD_DROP_GAP_TIME 
							+ (percentHP<<2) * rand()%20;//g_pClientConfig->BLOOD_DROP_RANDOM_TIME;

			//----------------------------------------------------------
			// 흡혈 당한 상태이면 피를 좀 덜 흘린다.
			//----------------------------------------------------------
			if (HasEffectStatus(EFFECTSTATUS_BLOOD_DRAIN))
			{
				// 5배 정도 덜 흘린다.
				m_NextBloodingTime = g_CurrentTime + timeGap*5;
			}
			else
			{			
				m_NextBloodingTime = g_CurrentTime + timeGap;
			}
		}
	}
}

//----------------------------------------------------------------------
// Add Casket Soon
//----------------------------------------------------------------------
void				
MCreature::AddCasketSoon(int type)
{
	m_bInCasket = true;
	m_CasketCount = 0;
	m_CasketCountInc = 0;
	m_CasketType = type;

	//SetInvisibleSoon();

	// [새기술3] -_-; virtual로 하는게 나을텐데.. 
	if (GetClassType()==CLASS_PLAYER)
	{		
		g_pSkillAvailable->SetAvailableSkills();
	}

	// [새기술7]
	m_bEffectStatus[EFFECTSTATUS_CASKET] = true;
	
	CheckRegen();

	if(g_pPlayer == this)	// Player인 경우 관소환 중에는 옷 못갈아 입는다.
	{
		UI_LockGear();
	}
}

//----------------------------------------------------------------------
// Remove Casket Soon
//----------------------------------------------------------------------
void				
MCreature::RemoveCasketSoon()
{
	m_bInCasket = false;
	m_CasketCount = 64;
	m_CasketCountInc = 0;

	//SetVisibleSoon();

	// [새기술7]
	m_bEffectStatus[EFFECTSTATUS_CASKET] = false;
	
	CheckRegen();

	if(g_pPlayer == this)	// Player인 경우 관소환 중에는 옷 못갈아 입는걸 푼다.
	{
		UI_UnlockGear();;
	}
}

//----------------------------------------------------------------------
// Add Casket
//----------------------------------------------------------------------
// 관이 서서히 생긴다.
//----------------------------------------------------------------------
void				
MCreature::AddCasket(int type)
{
	AddCasketSoon(type);
//	m_bInCasket = true;
	m_CasketCount = 64;
	m_CasketCountInc = -2;
//	m_CasketType = type;
//
//	//SetInvisible();
//
//	// [새기술3] -_-; virtual로 하는게 나을텐데.. 
//	if (GetClassType()==CLASS_PLAYER)
//	{		
//		g_pSkillAvailable->SetAvailableSkills();
//	}
//
//	// [새기술7]
//	m_bEffectStatus[EFFECTSTATUS_CASKET] = true;
//	
//	CheckRegen();
}

//----------------------------------------------------------------------
// Remove Casket
//----------------------------------------------------------------------
// 관이 서서히 사라진다.
//----------------------------------------------------------------------
void				
MCreature::RemoveCasket()
{
	RemoveCasketSoon();
	m_bInCasket = true;		// 64되면 false로 바꿔야 한다.
	m_CasketCount = 0;
	m_CasketCountInc = 2;

	//SetVisible();

//	// [새기술7]
//	m_bEffectStatus[EFFECTSTATUS_CASKET] = false;
//
//	CheckRegen();
}

//----------------------------------------------------------------------
// Update Casket
//----------------------------------------------------------------------
void				
MCreature::UpdateCasket()
{
	if (m_bInCasket)
	{		
		int incValue = m_CasketCountInc;

		if (m_CasketCount < 48)
		{
			if (m_CasketCount < 32)
			{
				incValue *= 3;
			}
			else
			{
				incValue *= 2;
			}
		}
		m_CasketCount += incValue;
	
		if (m_CasketCount<0)
		{
			m_CasketCount = 0;
			m_CasketCountInc = 0;	
			
			CheckRegen();
		}
		else if (m_CasketCount>64)
		{
			m_bInCasket = false;
			m_CasketCount = 64;
			m_CasketCountInc = 0;

			// [새기술3] - -;;
			if (GetClassType()==CLASS_PLAYER)
			{		
				g_pSkillAvailable->SetAvailableSkills();
			}

			CheckRegen();
		}		
	}
}

//----------------------------------------------------------------------
// Update Status
//----------------------------------------------------------------------
// 일단은.. HP / MP 변화
//----------------------------------------------------------------------
void
MCreature::UpdateStatus()
{
	//--------------------------------------------------------
	// coma 상태에서는 HP를 올리지 않는다.
	//--------------------------------------------------------
	if (!m_bAlive 
		|| HasEffectStatus(EFFECTSTATUS_COMA))
	{
		return;
	}

	bool bChangedHP = false;

	//--------------------------------------------------------
	// HP 회복
	//--------------------------------------------------------
	if (m_RecoveryHPTimes > 0 
		&& g_CurrentTime >= m_RecoveryHPNextTime)
	{		
		m_RecoveryHPTimes--;

		// 회복
		SetStatus( MODIFY_CURRENT_HP, GetStatus(MODIFY_CURRENT_HP)+m_RecoveryHPAmount );

		bChangedHP = true;
		
		// 다음 회복할 시간 설정
		m_RecoveryHPNextTime += m_RecoveryHPDelayTime;		
	}

	//--------------------------------------------------------
	// MP 회복
	//--------------------------------------------------------
	if (m_RecoveryMPTimes > 0 
		&& g_CurrentTime >= m_RecoveryMPNextTime)
	{		
		m_RecoveryMPTimes--;

		// 회복
		SetStatus( MODIFY_CURRENT_MP, GetStatus(MODIFY_CURRENT_MP)+m_RecoveryMPAmount );

		// 다음 회복할 시간 설정
		m_RecoveryMPNextTime += m_RecoveryMPDelayTime;		
	}

	//--------------------------------------------------------
	// 뱀파이어인 경우 자동 회복
	//--------------------------------------------------------
	if (m_RegenAmount > 0)
	{
		//--------------------------------------------------------
		// 기본적인 회복
		//--------------------------------------------------------
		if (g_CurrentTime >= m_RegenNextTime)
		{		
			// [새기술4] mephisto 걸리면 HP regen 안된다.

			//if ( !m_bEffectStatus[EFFECTSTATUS_MEPHISTO] || !m_bEffectStatus[EFFECTSTATUS_MEPHISTO]&&IsInCasket() )
			{
				// 관속에 있을 경우 은데미지부터 회복한다.
				if( IsInCasket() )
				{
					DWORD SilverDamage = GetStatus( MODIFY_SILVER_DAMAGE );
					DWORD Value = m_RegenAmount;
					
					if( SilverDamage > 0 )
					{
						if( SilverDamage < Value )
							Value = SilverDamage;
						
						SetStatus( MODIFY_SILVER_DAMAGE, SilverDamage - Value );
					} else
					{
						SetStatus( MODIFY_CURRENT_HP, GetStatus( MODIFY_CURRENT_HP ) + Value );
					}
				}
				else if( !m_bEffectStatus[EFFECTSTATUS_MEPHISTO] )
					SetStatus( MODIFY_CURRENT_HP, GetStatus(MODIFY_CURRENT_HP)+m_RegenAmount );

				bChangedHP = true;
			}

			m_RegenNextTime += m_RegenDelayTime;		
			

			//--------------------------------------------------------
			// 부가적인 회복 - -;
			//--------------------------------------------------------
			if (g_CurrentTime >= m_RegenBonusNextTime)
			{		
				if( GetStatus( MODIFY_SILVER_DAMAGE ) > 0 && IsInCasket() )
					SetStatus( MODIFY_SILVER_DAMAGE, max( 0, GetStatus( MODIFY_SILVER_DAMAGE ) - m_RegenBonusAmount ) );
				else
					SetStatus( MODIFY_CURRENT_HP, GetStatus(MODIFY_CURRENT_HP)+m_RegenBonusAmount );

				//bChangedHP = true;

				m_RegenBonusNextTime += m_RegenBonusDelayTime;		
			}
		}		
	}

	//--------------------------------------------------------
	// 파티인 경우는 HP 다시 설정해준다.
	//--------------------------------------------------------
	if (bChangedHP)
	{	
		if (IsPlayerParty())
		{
			PARTY_INFO* pInfo = g_pParty->GetMemberInfo( GetName() );
				
			// 좌표를 수정해준다.
			if (pInfo!=NULL)
			{
				pInfo->HP = m_Status[MODIFY_CURRENT_HP];
			}
		}
	}

}

//----------------------------------------------------------------------
// Update Invisible
//----------------------------------------------------------------------
void
MCreature::UpdateInvisible()
{
	if (m_InvisibleStep)
	{
		m_InvisibleCount += m_InvisibleStep;

		if (m_InvisibleCount>=64 || m_InvisibleCount<=0)
		{
			m_InvisibleStep = 0;
		}
	}
}

//----------------------------------------------------------------------
// 다음 동작을 취한다.
//----------------------------------------------------------------------
void
MCreature::Action()
{	
	#ifdef OUTPUT_DEBUG						
		g_bCheckError = false;


		//if (rand()%8==5)
		{
		//	PlaySound( rand()%g_pSoundTable->GetSize() );
		}
	#endif

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
	
//	// 2004, 08, 05 sobeit add start
//	if( GetCreatureType() == 726 ||  GetCreatureType() == 727 || GetCreatureType() == 728 || GetCreatureType() == 729) // 성문
//		return; // 밑으로 내려가 봐야 별로 할게 없다.
//	// 2004, 08, 05 sobeit add end

	//--------------------------------------------------------
	// 특수한 효과들
	//--------------------------------------------------------
//	if(UpDateInstallTurret())
//		return;
	UpDateInstallTurret();

	UpdateInvisible();
	UpdateCasket();
	UpdateCutHeight();
	UpdateTurning();
	UpdateCauseCriticalWounds();
	UpdateGhost();
//	UpdateBloodyZenith();
	UpdateGunShotGuidance();
	UpdateFakeDie();
	UpdateBurningSol();

	UpdateBikeCrash();
	if( !HasEffectStatus( EFFECTSTATUS_FIRE_ELEMENTAL ) && !HasEffectStatus( EFFECTSTATUS_WATER_ELEMENTAL ) && GetElementalID() != OBJECTID_NULL && g_pZone != NULL)
	{
		g_pZone->RemoveFakeCreature(GetElementalID());
	}
	
	if( GetCreatureType() == 672 )
	{
		if( HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_2 ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_3 ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_4 ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_5 ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_6 ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_7 ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_8 ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_9 ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_10 ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_11 ) ||
			HasEffectStatus( EFFECTSTATUS_KEEP_SWEEPER_12 )
			)
		{
			if( GetAction() != ACTION_MOVE )
			{
				m_Action = ACTION_MOVE;				
			}
			if(GetActionCountMax() != 0)
			{
				m_ActionCount = g_CurrentFrame % GetActionCountMax();
			}
		}
		else
		{
			m_Action = ACTION_STAND;
			m_ActionCount = 0;
		}
		
		return;
	} 
	else if( GetCreatureType() == 675 )
	{
		m_Action = ACTION_MAGIC;
		m_ActionCount = 0;		
	}
	//UpdateDarkness();
	
	//--------------------------------------------------------
	// 방향 전환을 자연스럽게
	//--------------------------------------------------------
	if (!ChangeNearDirection())
	{
			//--------------------------------------------------------
			// 방향을 바꿀 필요가 없던 경우에..
			// 심심할때마다 한번씩 방향 바꿔주기.. - -;
			//--------------------------------------------------------			
			if (// Player가 아니고
//				m_CreatureType >= 4
				!IsCreatureTypeVampirePC(m_CreatureType)
				&& m_CreatureType != CREATURETYPE_SLAYER_OPERATOR
				&& m_CreatureType != CREATURETYPE_VAMPIRE_OPERATOR
				&& m_CreatureType != CREATURETYPE_OUSTERS_OPERATOR
				&& m_CreatureType != CREATURETYPE_SLAYER_MALE
				&& m_CreatureType != CREATURETYPE_SLAYER_FEMALE
				&& m_CreatureType != CREATURETYPE_OUSTERS
				// 공중바토리가 아니고
				&& m_CreatureType != 431
				// NPC가 아니고... (방향별로 그림이 다 없어서리 -_-;)
				&& !IsNPC()
				// 살아 있고..
				&& m_bAlive
				// 정지상태이고
				&& m_Action==ACTION_STAND
				// 움직일곳이 없고
				&& m_listMoveBuffer.size()==0
				// 정지동작의 끝에..
				&& m_ActionCount>=GetActionCountMax()-1
				// random하게.. - -;
				&& (rand() % 5)==0
				&& !HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET)
				)
			{
				m_Direction = ((int)m_Direction + 8 + rand()%5-2) % 8;
			}
	}
	//m_CurrentDirection = m_Direction;

	//--------------------------------------------------------
	// 무슨 effect가 걸려있다면 2배 느리게 움직인다.
	//--------------------------------------------------------
	if (HasEffectStatus( EFFECTSTATUS_HAS_SLAYER_RELIC ) || HasEffectStatus( EFFECTSTATUS_HAS_VAMPIRE_RELIC ) || 
		HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_GREGORI )|| HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_NEMA ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_LEGIOS ) || HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_MIHOLE ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_AROSA )  || HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_ARMEGA ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_INI )    || HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_JAVE ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_CHASPA ) || HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_CONCILIA ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_KIRO )   || HasEffectStatus( EFFECTSTATUS_HAS_BLOOD_BIBLE_HILLEL ) ||
		HasEffectStatus( EFFECTSTATUS_AMBER_OF_GUARD ) || HasEffectStatus( EFFECTSTATUS_AMBER_OF_AVENGER ) ||
		HasEffectStatus( EFFECTSTATUS_AMBER_OF_IMMORTAL ) || HasEffectStatus( EFFECTSTATUS_AMBER_OF_CURSE ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_CASTLE_SYMBOL_5 ) || HasEffectStatus( EFFECTSTATUS_HAS_CASTLE_SYMBOL_6 ) ||
		IsCreatureMove( this ) && HasEffectStatus( EFFECTSTATUS_ICE_FIELD_TO_CREATURE ) ||
		IsCreatureMove( this ) && HasEffectStatus( EFFECTSTATUS_FROZEN_ARMOR_TO_ENEMY ) ||
		IsCreatureActionAttack( this ) && (	HasEffectStatus( EFFECTSTATUS_ICE_OF_SOUL_STONE ) || (IsInFuryOfGnome() && !IsOusters()/*(IsSlayer() || IsVampire())*/)	) ||
		IsCreatureMove( this ) && HasEffectStatus( EFFECTSTATUS_JABBING_VEIN ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER ) || HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_2 ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_3 ) || HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_4 ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_5 ) || HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_6 ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_7 ) || HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_8 ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_9 ) || HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_10 ) ||
		HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_11 ) || HasEffectStatus( EFFECTSTATUS_HAS_SWEEPER_12 )
		)
	{
		if (g_CurrentFrame & 0x01)
		{
			return;
		}
	}

//	BOOL bSlayer = IsSlayer();

	//--------------------------------------------------------
	// KnockBack 처리 2001.10.9
	//--------------------------------------------------------
	if (m_bKnockBack > 0)
	{
		m_sX += m_cX;
		m_sY += m_cY;

		// knockBack 다x 됐으면 정지동작.
		if (--m_bKnockBack==0)
		{
			m_sX = 0;
			m_sY = 0;

			SetAction( ACTION_STAND );

			return;
		}			
	}
	//--------------------------------------------------------
	// FastMove중에는 action을 취하지 않는다. 
	//--------------------------------------------------------
	else if (!m_bFastMove)
	{
		if (m_nNextUsedActionInfo!=ACTIONINFO_NULL)
		{	
			int bOldNextAction = m_NextAction;

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

			// 2001.9.30일 추가
			if (m_nUsedActionInfo!=ACTIONINFO_NULL)
			{
				int actionInfoAction = GetActionInfoAction(m_nUsedActionInfo);

				// 맞는 동작이면.. 피 튀김..
				// 2002.3.19 주석 제거.. 아무 동작이나 일단 이펙트 보여주자
				//if (actionInfoAction==ACTION_DAMAGED)
				{
					if //(m_Action!=ACTION_STAND 
						//&& (!IsSlayer() || IsSlayer && m_Action!=ACTION_SLAYER_MOTOR_STAND)
						//&& 
						(GetActionInfoCastingStartFrame(m_nUsedActionInfo) <= m_ActionCount)
					{
						AttachCastingEffect( m_nUsedActionInfo, TRUE );

						AffectUsedActionInfo( m_nUsedActionInfo );
				
						// 소리 출력
						PlaySound( 
							(*g_pCreatureTable)[m_CreatureType].GetActionSound( 
								GetActionInfoAction(m_nUsedActionInfo)
							),
							false,
							m_X, m_Y
						);

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
			// 이동할려다가 맞거나해서 다른 action을 실행하게 되는 경우
			// 이동이 안되는 경우가 있었다.
			// 대책 --> NextAction을 기억했다가 이동이면.. 이동시키기
			// 2001.9.24
			//--------------------------------------------------------
			if (bOldNextAction==m_MoveAction)
			{
				SetNextAction( m_MoveAction );
			}	 			
		}
		//--------------------------------------------------------
		// 기억된 다음 행동... 
		//--------------------------------------------------------
		else if (m_bNextAction 
				|| m_NextAction!=ACTION_STAND 
				&& (!IsOusters() || IsOusters() && m_NextAction!=ACTION_OUSTERS_FAST_MOVE_STAND)
				&& (!IsSlayer() || IsSlayer() && m_NextAction!=ACTION_SLAYER_MOTOR_STAND))
		{
			SetAction( m_NextAction );
		}	 
	}

	//--------------------------------------------------------
	// 마비 됐을 때..
	//--------------------------------------------------------
	if(HasEffectStatus(EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS) && !IsSlayer())
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
	else
	if (HasEffectStatus(EFFECTSTATUS_CURSE_PARALYSIS)
		|| HasEffectStatus(EFFECTSTATUS_EXPLOSION_WATER)
		|| HasEffectStatus(EFFECTSTATUS_GLACIER)
		)
	{			
		//--------------------------------------------------------
		// 기술을 사용하려는 경우
		//--------------------------------------------------------
		if (m_nUsedActionInfo!=ACTIONINFO_NULL)
		{
			AffectUsedActionInfo(m_nUsedActionInfo);
			m_nUsedActionInfo	= ACTIONINFO_NULL;
		}

		if (m_ActionCount>=GetActionCountMax())
		{			
			//--------------------------------------------------------
			// 목표 타일에 도착한 상태에서..
			// 계속 서 있거나.. 다 걸은 경우는.. 
			// 제자리에서 흔들거리는 모습을 표현해준다.
			//--------------------------------------------------------
			if (m_MoveCount>=m_MoveCountMax)
			{		
				BOOL bSlayer = IsSlayer();

				if (m_bNextAction 
					|| m_NextAction!=ACTION_STAND 
					&& (!IsOusters() || IsOusters() && m_NextAction!=ACTION_OUSTERS_FAST_MOVE_STAND)
					&& (!bSlayer || bSlayer && m_NextAction!=ACTION_SLAYER_MOTOR_STAND))
				{
					m_Action = ((m_MoveDevice==MOVE_DEVICE_RIDE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND);
					if(IsOusters() && HasEffectStatusSummonSylph( this ))
						m_Action = ACTION_OUSTERS_FAST_MOVE_STAND;

					SetAction( m_NextAction );
				}	 
				else
				{
					SetAction( ACTION_STAND );
				}
			}
		}
	}
	//--------------------------------------------------------
	// 일반적인 경우
	//--------------------------------------------------------
	else
	{
		//--------------------------------------------------------
		// 피 흘리는거 체크
		//--------------------------------------------------------
		CheckDropBlood();

		//--------------------------------------------------------
		// Action 처리
		//--------------------------------------------------------
		if (m_ActionCount < GetActionCountMax())
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
		// 살아 있는 경우만
		//--------------------------------------------------------
		if (m_bAlive)
		{
			//--------------------------------------------------------
			// Action이 끝난 경우...
			//--------------------------------------------------------
			if (m_ActionCount>=GetActionCountMax())
			{			
				//--------------------------------------------------------
				// 목표 타일에 도착한 상태에서..
				// 계속 서 있거나.. 다 걸은 경우는.. 
				// 제자리에서 흔들거리는 모습을 표현해준다.
				//--------------------------------------------------------
				if (m_MoveCount>=m_MoveCountMax)
				{		
					//#ifdef CONNECT_SERVER
					//--------------------------------------------------------
					// 다음 이동이 없다면.. 정지시킨다.
					//--------------------------------------------------------
					AffectMoveBuffer();

					BOOL bSlayer = IsSlayer();

					if (m_bNextAction 
						|| m_NextAction!=ACTION_STAND 
						&& (!IsOusters() || IsOusters() && m_NextAction!=ACTION_OUSTERS_FAST_MOVE_STAND)
						&& (!bSlayer || bSlayer && m_NextAction!=ACTION_SLAYER_MOTOR_STAND))
					{
						m_Action = ((m_MoveDevice==MOVE_DEVICE_RIDE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND);
						if(IsOusters() && HasEffectStatusSummonSylph( this ))
							m_Action = ACTION_OUSTERS_FAST_MOVE_STAND;
						SetAction( m_NextAction );
					}	 
					else
					{
						SetAction( ACTION_STAND );				
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
						if (m_NextX != SECTORPOSITION_NULL
							&& m_NextDirection != DIRECTION_NULL)
						{		
							MoveNextPosition();
						}
						else
						{
							//#ifdef CONNECT_SERVER

							//--------------------------------------------------------
							// 다음 이동
							//--------------------------------------------------------
							AffectMoveBuffer();

							BOOL bSlayer = IsSlayer();

							if (m_bNextAction 
								|| m_NextAction!=ACTION_STAND 
								&& (!IsOusters() || IsOusters() && m_NextAction!=ACTION_OUSTERS_FAST_MOVE_STAND )
								&& (!bSlayer || bSlayer && m_NextAction!=ACTION_SLAYER_MOTOR_STAND))
							{
								m_Action = ((m_MoveDevice==MOVE_DEVICE_RIDE)? ACTION_SLAYER_MOTOR_STAND : ACTION_STAND);
								if(IsOusters() && HasEffectStatusSummonSylph( this ))
									m_Action = ACTION_OUSTERS_FAST_MOVE_STAND;
								SetAction( m_NextAction );
							}	 
							else
							{
								SetAction( ACTION_STAND );				
							}
							//#else
							//	SetAction( m_MoveAction );				
							//#endif
						}
					}
				}
			}
			//--------------------------------------------------------
			// Action을 보여주는 중이라도.. 
			// 이동할 꺼리?가 있으면 이동도 시켜준다. 2001.10.10
			//--------------------------------------------------------
			else
			{		
				/*
				// 2001.11.8 - 주석처리.. 보기 안 좋다나.. - -;;
				if (// KnockBack이나 FastMove가 아닌 경우
					!m_bFastMove && m_bKnockBack==0	&& m_MoveCount>0)
				{
					if (m_MoveCount<m_MoveCountMax)
					{
						ActionMove();
					}
					else if (m_MoveCount>=m_MoveCountMax)
					{
						// 다음 이동할 곳이 있으면..
						if (m_NextX != SECTORPOSITION_NULL)
						{		
							MoveNextPosition();
						}
					}
				}
				*/
			}
		}
	}

	#ifdef OUTPUT_DEBUG					
		if (g_bCheckError)
		{
			DEBUG_ADD_FORMAT("[CheckError] Action OK");
		}
	#endif		
}

//----------------------------------------------------------------------
// Packet Move (x, y, d)
//----------------------------------------------------------------------
// this Creature는 Move를 받았으므로
//
// //0. Player와의 좌표 충돌을 고려해서 움직여준다.
//
// (x,y)에서 d방향으로 한 칸 움직인다.
//----------------------------------------------------------------------
void		
MCreature::PacketMove(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y, BYTE direction)
{
//	// 2004, 9, 16, sobeit add start - 터렛 방향 바꾸기, 뺄까..ㅡㅡ;
//	if(HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET))
//	{
//		SetCurrentDirection(direction);
//		return;
//	}
//	// 2004, 9, 16, sobeit add end
	//---------------------------------------------------
	// 새로운 이동 정보를 buffer에 추가시킨다.
	//---------------------------------------------------
	MoveNode* pNode = new MoveNode;
	pNode->x = x;
	pNode->y = y;
	pNode->direction = direction;

	if (IsPlayerParty())
	{
		PARTY_INFO* pInfo = g_pParty->GetMemberInfo( GetName() );		
			
		// 좌표를 수정해준다.
		if (pInfo!=NULL)
		{
			pInfo->zoneID = (g_bZonePlayerInLarge?g_nZoneLarge : g_nZoneSmall);
			pInfo->zoneX = x;
			pInfo->zoneY = y;
		}
	}

	//---------------------------------------------------
	// Server좌표는 바로 적용시킨다.
	//---------------------------------------------------
	///*
	int oldX = m_X;
	int oldY = m_Y;

	m_X = x;
	m_Y = y;

	POINT next;
	GetNextPosition( direction, next );

	m_X = oldX;
	m_Y = oldY;

	// server좌표 기억
	// 2001.8.6
	// 2002.3.22 다시 살렸다.
	//m_ServerX	= next.x;
	//m_ServerY	= next.y;
	
	SetServerPositionWithBlock( next.x, next.y );
	//*/

	m_listMoveBuffer.push_back( pNode );

	//-----------------------------------------------------------
	// 정지된 상태이면 바로 적용한다.
	//-----------------------------------------------------------
	if (m_Action==ACTION_STAND 
		|| IsSlayer() && m_Action==ACTION_SLAYER_MOTOR_STAND)
	{
		AffectMoveBuffer();
	}
	else
	{
		//---------------------------------------------------
		// 한계를 넘는 MoveBuffer는 모두 적용시켜버린다.
		//---------------------------------------------------
		while (m_listMoveBuffer.size() > g_pClientConfig->MAX_CREATURE_MOVE_BUFFER)
		{
			AffectMoveBuffer();
		}		
	}
}

//----------------------------------------------------------------------
// Release MoveBuffer
//----------------------------------------------------------------------
// 기존의 MoveBuffer를 다 지움
//----------------------------------------------------------------------
void
MCreature::ReleaseMoveBuffer()
{
	MOVENODE_LIST::iterator iNode = m_listMoveBuffer.begin();

	while (iNode != m_listMoveBuffer.end())
	{
		MoveNode* pNode = *iNode;

		delete pNode;

		iNode++;
	}

	m_listMoveBuffer.clear();
}

//----------------------------------------------------------------------
// Affect MoveBuffer All
//----------------------------------------------------------------------
// 모든 MoveBuffer를 적용시킨다.
//----------------------------------------------------------------------
void
MCreature::AffectMoveBufferAll()
{
	while (m_listMoveBuffer.size() != 0)
	{
		AffectMoveBuffer();
	}
}

//----------------------------------------------------------------------
// Affect Move Buffer
//----------------------------------------------------------------------
// Buffering된 다음의 이동 정보를 하나 설정한다.
//----------------------------------------------------------------------
bool	
MCreature::AffectMoveBuffer()
{
	//-------------------------------------------
	// 이동할 정보가 없는 경우
	//-------------------------------------------
	if (m_listMoveBuffer.size()==0)
	{
		return false;
	}

	//-------------------------------------------
	// 젤 앞에껄 읽어온다.
	//-------------------------------------------
	MoveNode* pNode = m_listMoveBuffer.front();
	m_listMoveBuffer.pop_front();

	//-------------------------------------------
	// 값 설정
	//-------------------------------------------
	// 원래는 PacketMove()에 있던건데
	// move buffering 추가하면서 cut & paste - -;;
	// 잘 돌아갈려나..
	//-------------------------------------------
	int x = pNode->x;
	int y = pNode->y;
	int direction = pNode->direction;

	delete pNode;

	// 죽은 경우
	if (!m_bAlive)
	{
		return false;
	}


	DEBUG_ADD_FORMAT("AffectMoveBuffer : [ID=%d] From(%d,%d) Direction(%d)", m_ID, x,y, direction);

	// 이전 좌표를 기억해둔다.		
	int oldX = m_X;
	int oldY = m_Y;

	// 임시로.. 새 좌표를 설정
	m_X = x;
	m_Y = y;	// m_X=x; m_Y=y;  and.. Etc...
		
	// Server에서 날아온 방향으로 움직인 좌표를 구한다.
	POINT next;
	GetNextPosition( direction, next );

	// 영역 넘어가는지 체크 . 2001.10.7
	if (next.x < 0 
		|| next.y < 0 
		|| next.x >= g_pZone->GetWidth() 
		|| next.y >= g_pZone->GetHeight()) 
	{
		m_X = oldX;
		m_Y = oldY;

		return false;
	}

	// server좌표 기억
	// 2001.8.6
	// 주석 2002.3.22... PacketMove로 옮겼다.
	//SetServerPositionWithBlock( next.x, next.y );	

	// 원래 좌표로 되돌린다.
	m_X = oldX;
	m_Y = oldY;

	//-----------------------------------------------------
	// 갈 수 없는 곳인 경우...
	//-----------------------------------------------------
	// Server에서 잘못된 정보가 왔다.. - -;;
	// timing문제라고 할까?
	#ifdef OUTPUT_DEBUG
		//if (!m_pZone->CanMove(m_MoveType, next.x, next.y))
		{
		//	DEBUG_ADD_FORMAT("[ Collide ] Creature[ID=%d] Can't Move to (%d,%d) - But, Go!", m_ID, next.x, next.y);				
			
			// 그래도 움직여보자~
			//return;
		}
	#endif

	//--------------------------------------------------
	// Server에서 검증된 좌표를 설정한다.
	//--------------------------------------------------
	//m_ServerX = x;//next.x;
	//m_ServerY = y;//next.y;

	// 아직 이전 정보가 update되지 않았으면...
	if (m_NextX != SECTORPOSITION_NULL)
		// && m_NextY != SECTORPOSITION_NULL
	{
		// message
		DEBUG_ADD_FORMAT("[ SlowUpdate ] [ID=%d] : (%d, %d) --> (%d, %d),  Player=(%d, %d)", 
							m_ID, 
							m_X, m_Y, 
							m_NextX, m_NextY, 
							g_pPlayer->GetX(), g_pPlayer->GetY());			
		
		// 이전 정보를 바로 update시킨다.
		MovePosition(m_NextX, m_NextY);
		m_Direction = m_NextDirection;
		m_DirectionMove = m_NextDirection;

		// (m_X, m_Y)에서 m_CurrentDirection으로 이동시킨다.
		SetNextAction( m_MoveAction );

		m_NextX = x;
		m_NextY = y;
		m_NextDirection = direction;	

		return true;
	}

	//--------------------------------------------------
	// Player가 이미 갈 자리에 있다면....
	//--------------------------------------------------
	// 못 간다~~
	/*
	if (g_pPlayer->GetX()==next.x && g_pPlayer->GetY()==next.y)
	{
		// 방향만 설정..
		//pCreature->SetDirection( pGCMove->getDir() );	
		
		// 이전에 있던 좌표에서 움직이려고 한 경우..
		if (oldX==x && oldY==y)
		{
			// 그냥 서 있으면 된다.
		}
		else
		{
			// 이전에 있던 좌표(oldX, oldY)에서 새좌표(x,y)로 이동한다.
			m_X = oldX;
			m_Y = oldY;

			MovePosition( x, y );			

			// 새 좌표에서 방향(direction)으로 움직여야 하지만
			// 그냥 새 좌표에 서 있어야 한다.
		}

		// message		
		#ifdef	OUTPUT_DEBUG
			sprintf(g_pDebugMessage->GetCurrent(), "[Can't Move] : Player(%d, %d) Already Exist... ", g_pPlayer->GetX(), g_pPlayer->GetY());
			g_pDebugMessage->Next();
		#endif
	}
	*/
	//--------------------------------------------------
	// 목적지에 이미 가 있는 경우는 움직일 필요없다.
	//--------------------------------------------------		
	//else 
	if (m_X==next.x && m_Y==next.y)
	{	
		// message
		DEBUG_ADD_FORMAT("[ Stand ] [ID=%d] : Already Reached (%d, %d) ,  Player=(%d, %d)", m_ID, oldX, oldY, g_pPlayer->GetX(), g_pPlayer->GetY());						
	}
	//--------------------------------------------------
	// 목적지에 가 있지 않은 경우 --> 움직인다.
	//--------------------------------------------------
	else
	{		
		//--------------------------------------------------
		// 이전에 있던 위치에서 움직이는 것이면.. 
		//--------------------------------------------------
		if (m_X==x && m_Y==y)
		{
			// 움직이는 동작이 끝난 상태이면..
			if (m_MoveCount>=m_MoveCountMax)
			{
				DEBUG_ADD_FORMAT("[ MoveSetting ] [ID=%d] From(%d, %d) to Direction(%d) ,  Player=(%d, %d)", m_ID, x,y, direction, g_pPlayer->GetX(), g_pPlayer->GetY());						
				
				m_Direction = direction;
				m_DirectionMove = direction;
				SetNextAction( m_MoveAction );
				//m_bNextAction = true;
			}
			// 아직 움직이고 있는 중이면..
			else
			{
				DEBUG_ADD_FORMAT("[ MoveBuffering ][ID=%d] Current(%d, %d), Next(%d, %d) to Direction(%d)", m_ID, m_X, m_Y, x,y, direction);						
				
				m_NextX = x;
				m_NextY = y;
				m_NextDirection = direction;
			}
		}
		//--------------------------------------------------
		// jump하게 되는 경우에...
		//--------------------------------------------------
		// 이전에 있던 Creature의 위치를 제거한다.
		else
		{
			// 이전에 있던 좌표(m_X, m_Y)에서 새좌표(x,y)로 이동한다.			
			MovePosition( x, y );			
			
			// 새 좌표에서 방향(direction)으로 움직여야 한다.
			m_Direction = direction;
			m_DirectionMove = direction;
			m_NextDirection = direction;	// 2001.11.6
			SetNextAction( m_MoveAction );

			// message
			DEBUG_ADD_FORMAT("[ Jump ] [ID=%d] : (%d, %d) --> (%d, %d),  Player=(%d, %d)", m_ID, m_X, m_Y, next.x, next.y, g_pPlayer->GetX(), g_pPlayer->GetY());
		}			
		
		// 방향을 정하고 움직이도록 한다.
		//m_CurrentDirection = direction;
		//SetNextAction( m_MoveAction );
	}	

	return true;
}

//----------------------------------------------------------------------
// Get ActionInfo's Action
//----------------------------------------------------------------------
// nActionInfo에 적절한 ACTION을 찾는다.
//----------------------------------------------------------------------
int
MCreature::GetActionInfoAction(TYPE_ACTIONINFO nActionInfo, bool IsSelfAction)
{
	// 2001.9.30
	if (nActionInfo>=g_pActionInfoTable->GetSize())
	{
		return 0;
	}

	int action;
	
	
	// 2004, 11, 23, sobeit add start - set afire
//	if(nActionInfo == SKILL_SET_AFIRE && !IsFastMove())
//		return ACTION_ATTACK;
	// 2004, 11, 23, sobeit add end
	//-------------------------------------------------------------
	// 기본 action의 적용을 받는가?
	//-------------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsAffectCurrentWeaponAction())
	{
		action = (*g_pActionInfoTable)[m_nBasicActionInfo].GetAction();
	}
	else
	{
		action = (*g_pActionInfoTable)[nActionInfo].GetAction();	
	}

	if(action == ACTION_MAGIC && IsAdvancementClass())
	{
		if(!(*g_pActionInfoTable)[nActionInfo].IsTargetSelf() && !IsSelfAction)
		{
			if(IsSlayer())
				return ACTION_ADVANCEMENT_SLAYER_MAGIC_ATTACK;
			else if(IsOusters())
				return ACTION_ADVANCEMENT_OUSTERS_MAGIC_ATTACK;
			else 
				return ACTION_ADVANCEMENT_MAGIC_ATTACK;

		}
	}
	//-------------------------------------------------------------
	// 무기 속도에 따라서 action이 달라진다.
	//-------------------------------------------------------------
	if (IsSlayer())
	{
		if (m_CreatureType==CREATURETYPE_SLAYER_OPERATOR)
		{
			if (action>ACTION_DAMAGED
				|| action==ACTION_ATTACK)
			{
				return ACTION_ATTACK;
			}
		}
		
		//-----------------------------------------------------
		// Lightning Hand는 무조건 ACTION_SLAYER_SWORD_2이다. - -;d
		//-----------------------------------------------------
		if (nActionInfo==SKILL_LIGHTNING_HAND || nActionInfo == SKILL_LARSLASH )
		{
			return ACTION_SLAYER_SWORD_2;
		} 
		else
		if (nActionInfo == SKILL_BLITZ_SLIDING_ATTACK)
		{
			return ACTION_SLAYER_SWORD_2_FAST;
		} else
		if ( nActionInfo==SKILL_BLAZE_WALK_ATTACK )
		{
			return ACTION_SLAYER_BLADE_2_FAST;
		}
		else 
		if( nActionInfo == SKILL_PLASMA_ROCKET_LAUNCHER )
		{
			return s_SlayerActionSpeed[action][1];
		}
		return s_SlayerActionSpeed[action][m_WeaponSpeed];
	} 
	else if( IsOusters() )
	{
		if(nActionInfo == SKILL_DISTANCE_BLITZ)
			return action;

		// test code
//		if(nActionInfo == SKILL_FATAL_SNICK || nActionInfo == SKILL_FATAL_SNICK_MASTERY)
//			action = ACTION_OUSTERS_CHAKRAM;
//		
		if(m_CreatureType==CREATURETYPE_OUSTERS)
			return s_OustersActionSpeed[action][m_WeaponSpeed];		
	}

	if (m_CreatureType==CREATURETYPE_WOLF)		
	{
		if (action==ACTION_ATTACK)
		{
			return ACTION_ATTACK;
		}
	}

	if (m_CreatureType==CREATURETYPE_VAMPIRE_OPERATOR || m_CreatureType==CREATURETYPE_SLAYER_OPERATOR || m_CreatureType==CREATURETYPE_OUSTERS_OPERATOR)
	{
		if (action>ACTION_DAMAGED
			|| action==ACTION_ATTACK)
		{
			return ACTION_ATTACK;
		}		
	}
	// 2004, 7, 27 sobeit add start
	if (m_CreatureType==717 || m_CreatureType==721 || m_CreatureType==723) // 질드레, 질드레 분신, 각성 질드레
	{
		if(action <ACTION_MAX_OUSTERS)
			return action;
	}
	// 2004, 7, 27 sobeit add end
	if(m_CreatureType == 755 || m_CreatureType == 756 || m_CreatureType == 757 || m_CreatureType == 758 ||
		m_CreatureType == 759 || m_CreatureType == 760 || m_CreatureType == 761 || m_CreatureType == 762 || m_CreatureType == 763)
	{
		if(action==ACTION_ATTACK)
		{
			if (rand()%2)
				action = ACTION_MAGIC;
			else
				action = ACTION_ATTACK;
		}
	}
	

//	if (IsCreatureTypeVampirePC(m_CreatureType))
//		return s_VampireActionSpeed[action][m_WeaponSpeed];	


	return s_VampireActionSpeed[action][m_WeaponSpeed];	
//	return action;
}

//----------------------------------------------------------------------
// Get ActionInfo's CastingStartFrame
//----------------------------------------------------------------------
// nActionInfo에 적절한 CastingStartFrame을 찾는다.
//----------------------------------------------------------------------
int			
MCreature::GetActionInfoCastingStartFrame(TYPE_ACTIONINFO nActionInfo)
{
	// 2001.9.30
	if (nActionInfo>=g_pActionInfoTable->GetSize())
	{
		return 0;
	}

	//-------------------------------------------------------------
	// 기본 action의 적용을 받는가?
	//-------------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsAffectCurrentWeaponCastingStartFrame())
	{
//		if(nActionInfo == SKILL_PLASMA_ROCKET_LAUNCHER)
//		{
//			switch(m_nBasicActionInfo)
//			{
//			case ACTION_SLAYER_GUN_SR :
//			case ACTION_SLAYER_GUN_SR_SLOW :
//			case ACTION_SLAYER_GUN_SR_FAST :
//				return 7;
//			case ACTION_SLAYER_GUN_AR:
//			case ACTION_SLAYER_GUN_AR_SLOW:
//			case ACTION_SLAYER_GUN_AR_FAST:
//				return 4;
//			case ACTION_SLAYER_GUN_SG:
//			case ACTION_SLAYER_GUN_SG_SLOW:
//			case ACTION_SLAYER_GUN_SG_FAST:
//				return 4;
//			case ACTION_SLAYER_GUN_SMG:
//			case ACTION_SLAYER_GUN_SMG_SLOW:
//			case ACTION_SLAYER_GUN_SMG_FAST:
//				return 4;
//			}
//		}
		return (*g_pActionInfoTable)[m_nBasicActionInfo].GetCastingStartFrame( m_WeaponSpeed );
	}
	
	return (*g_pActionInfoTable)[nActionInfo].GetCastingStartFrame( m_WeaponSpeed );	
}

//----------------------------------------------------------------------
// Get ActionInfo's RepeatStartFrame
//----------------------------------------------------------------------
// nActionInfo에 적절한 RepeatStartFrame을 찾는다.
//----------------------------------------------------------------------
int			
MCreature::GetActionInfoRepeatStartFrame(TYPE_ACTIONINFO nActionInfo)
{
	// 2001.9.30
	if (nActionInfo>=g_pActionInfoTable->GetSize())
	{
		return GetActionCountMax();
	}

	int repeat;

	//-------------------------------------------------------------
	// 기본 action의 적용을 받는가?
	//-------------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsAffectCurrentWeaponCastingStartFrame())
	{
//		if(nActionInfo == SKILL_PLASMA_ROCKET_LAUNCHER)
//		{
//			switch(m_nBasicActionInfo)
//			{
//			case ACTION_SLAYER_GUN_SR :
//			case ACTION_SLAYER_GUN_SR_SLOW :
//			case ACTION_SLAYER_GUN_SR_FAST :
//				return 7;
//			case ACTION_SLAYER_GUN_AR:
//			case ACTION_SLAYER_GUN_AR_SLOW:
//			case ACTION_SLAYER_GUN_AR_FAST:
//				return 4;
//			case ACTION_SLAYER_GUN_SG:
//			case ACTION_SLAYER_GUN_SG_SLOW:
//			case ACTION_SLAYER_GUN_SG_FAST:
//				return 4;
//			case ACTION_SLAYER_GUN_SMG:
//			case ACTION_SLAYER_GUN_SMG_SLOW:
//			case ACTION_SLAYER_GUN_SMG_FAST:
//				return 4;
//			}
//		}
		if (m_nBasicActionInfo < g_pActionInfoTable->GetSize())
		{
			repeat = (*g_pActionInfoTable)[m_nBasicActionInfo].GetRepeatStartFrame( m_WeaponSpeed );
		}
		else
		{
			repeat = GetActionCountMax();
		}
	}
	else
	{
		repeat = (*g_pActionInfoTable)[nActionInfo].GetRepeatStartFrame( m_WeaponSpeed );
	}

	
	if (repeat==-1)
	{
		return 0;
	}

	return repeat;
}
		
//----------------------------------------------------------------------
// Get ActionInfo's RepeatEndFrame
//----------------------------------------------------------------------
// nActionInfo에 적절한 RepeatEndFrame을 찾는다.
//----------------------------------------------------------------------
int			
MCreature::GetActionInfoRepeatEndFrame(TYPE_ACTIONINFO nActionInfo)
{
	// 2001.9.30
	if (nActionInfo>=g_pActionInfoTable->GetSize())
	{
		return GetActionCountMax();
	}

	int repeat;
	//-------------------------------------------------------------
	// 기본 action의 적용을 받는가?
	//-------------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsAffectCurrentWeaponCastingStartFrame())
	{
//		if(nActionInfo == SKILL_PLASMA_ROCKET_LAUNCHER)
//		{
//			switch(m_nBasicActionInfo)
//			{
//			case ACTION_SLAYER_GUN_SR :
//			case ACTION_SLAYER_GUN_SR_SLOW :
//			case ACTION_SLAYER_GUN_SR_FAST :
//				return 17;
//			case ACTION_SLAYER_GUN_AR:
//			case ACTION_SLAYER_GUN_AR_SLOW:
//			case ACTION_SLAYER_GUN_AR_FAST:
//				return 14;
//			case ACTION_SLAYER_GUN_SG:
//			case ACTION_SLAYER_GUN_SG_SLOW:
//			case ACTION_SLAYER_GUN_SG_FAST:
//				return 14;
//			case ACTION_SLAYER_GUN_SMG:
//			case ACTION_SLAYER_GUN_SMG_SLOW:
//			case ACTION_SLAYER_GUN_SMG_FAST:
//				return 14;
//			}
//		}
		if (m_nBasicActionInfo < g_pActionInfoTable->GetSize())
		{
			repeat = (*g_pActionInfoTable)[m_nBasicActionInfo].GetRepeatEndFrame( m_WeaponSpeed );		
		}
		else
		{
			repeat = GetActionCountMax();
		}
	}
	else
	{
		repeat = (*g_pActionInfoTable)[nActionInfo].GetRepeatEndFrame( m_WeaponSpeed );
	}

	if (repeat==-1)
	{
		return GetCreatureActionCountMax( this, m_Action );//(*g_pCreatureTable)[m_CreatureType].GetActionCount( m_Action );
	}

	return repeat;
}

//----------------------------------------------------------------------
// Get ActionInfo's CastingFrames
//----------------------------------------------------------------------
// nActionInfo에 적절한 CastingFrames을 찾는다.
//----------------------------------------------------------------------
int			
MCreature::GetActionInfoCastingFrames(TYPE_ACTIONINFO nActionInfo)
{
	// 2001.9.30
	if (nActionInfo>=g_pActionInfoTable->GetSize())
	{
		return 0;
	}

	//-------------------------------------------------------------
	// 기본 action의 적용을 받는가?
	//-------------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsAffectCurrentWeaponCastingFrames())
	{
		return (*g_pActionInfoTable)[m_nBasicActionInfo].GetCastingFrames( m_WeaponSpeed );
	}
	
	return (*g_pActionInfoTable)[nActionInfo].GetCastingFrames( m_WeaponSpeed );	
}

//----------------------------------------------------------------------
// Get ActionInfo's Delay
//----------------------------------------------------------------------
// nActionInfo에 적절한 Delay를 찾는다.
//----------------------------------------------------------------------
int
MCreature::GetActionInfoDelay(TYPE_ACTIONINFO nActionInfo)
{
	// 2001.9.30
	if (nActionInfo>=g_pActionInfoTable->GetSize())
	{
		return 0;
	}


//	if(nActionInfo == SKILL_FATAL_SNICK || nActionInfo == SKILL_FATAL_SNICK_MASTERY)
//	{
//		switch( GetWeaponSpeed() )
//		{
//		case WEAPON_SPEED_SLOW :
//			return 25 * 1000 / 16;
//		case WEAPON_SPEED_NORMAL :
//			return 23 * 1000 / 16;
//		case WEAPON_SPEED_FAST :
//			return 21 * 1000 / 16;
//		}
//	} 
	//-------------------------------------------------------------
	// 기본 action의 적용을 받는가?
	//-------------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsAffectCurrentWeaponDelay())
	{
		return (*g_pActionInfoTable)[m_nBasicActionInfo].GetDelay();
	}
	
	return (*g_pActionInfoTable)[nActionInfo].GetDelay();	
}

//----------------------------------------------------------------------
// Get Casting SoundID
//----------------------------------------------------------------------
// nActionInfo에 적절한 ACTION을 찾는다.
//----------------------------------------------------------------------
TYPE_SOUNDID
MCreature::GetCastingSoundID(TYPE_ACTIONINFO nActionInfo)
{
	//-------------------------------------------------------------
	// 기본 무기의 적용을 받는가?
	//-------------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsAffectCurrentWeaponSound())
	{
		return (*g_pActionInfoTable)[m_nBasicActionInfo].GetSoundID();
	}
	
	return (*g_pActionInfoTable)[nActionInfo].GetSoundID();	
}

//----------------------------------------------------------------------
// Get ActionInfo ActionEffectSpriteType
//----------------------------------------------------------------------
// nActionInfo에 적절한 ActionEffectSpriteType을 찾는다.
//----------------------------------------------------------------------
int
MCreature::GetActionInfoActionEffectSpriteType(TYPE_ACTIONINFO nActionInfo)
{
	// 2001.9.30
	if (nActionInfo>=g_pActionInfoTable->GetSize())
	{
		return 0;
	}

	int useActionInfo;


	//-------------------------------------------------------------
	// 기본 무기의 적용을 받는가?
	//-------------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsAffectCurrentWeaponActionEffectSpriteType())
	{
		useActionInfo = GetBasicActionInfo(); //m_nBasicActionInfo;
	}
	else
	{
		useActionInfo = nActionInfo;
	}

	// plus만큼 더한다.
	// 임시 땜빵~~ 그러나.. 앞으로도 고치지 않겠지 - -;
	useActionInfo += (*g_pActionInfoTable)[nActionInfo].GetAffectCurrentWeaponActionInfoPlus();


	TYPE_EFFECTSPRITETYPE re_est = EFFECTSPRITETYPE_NULL;

	//------------------------------------------------------------
	// 남자인 경우
	//------------------------------------------------------------
	if (IsMale())
	{
		re_est = (*g_pActionInfoTable)[useActionInfo].GetActionEffectSpriteType();
		if(nActionInfo == SKILL_BULLET_OF_LIGHT)
		{
			switch(re_est)
			{
			case EFFECTSPRITETYPE_GUN_AR_MALE:
				re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_AR_MALE;
				break;

			case EFFECTSPRITETYPE_GUN_SMG_MALE:
				re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_SMG_MALE;
				break;

			case EFFECTSPRITETYPE_GUN_SR_MALE:
				re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_SR_MALE;
				break;

			case EFFECTSPRITETYPE_GUN_SG_MALE:
				re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_SG_MALE;
				break;
			}
		} 
		else
		if(nActionInfo == SKILL_JABBING_VEIN)
		{
			switch(re_est)
			{
			case EFFECTSPRITETYPE_GUN_AR_MALE :
				re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_AR_MALE;
				break;
			case EFFECTSPRITETYPE_GUN_SMG_MALE :
				re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_SMG_MALE;
				break;
			case EFFECTSPRITETYPE_GUN_SR_MALE :
				re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_SR_MALE;
				break;
			case EFFECTSPRITETYPE_GUN_SG_MALE :
				re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_SG_MALE;
				break;
			}
		}
		return re_est;
	}
	
	//------------------------------------------------------------
	// 여자용 Effect check
	//------------------------------------------------------------	
	TYPE_EFFECTSPRITETYPE effectSpriteTypeFemale = (*g_pActionInfoTable)[useActionInfo].GetActionEffectSpriteTypeFemale();

	if (effectSpriteTypeFemale!=EFFECTSPRITETYPE_NULL)
	{
		re_est = effectSpriteTypeFemale;
		if(nActionInfo == SKILL_BULLET_OF_LIGHT)
		{
			switch(re_est)
			{
			case EFFECTSPRITETYPE_GUN_AR_FEMALE:
				re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_AR_FEMALE;
				break;

			case EFFECTSPRITETYPE_GUN_SMG_FEMALE:
				re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_SMG_FEMALE;
				break;

			case EFFECTSPRITETYPE_GUN_SR_FEMALE:
				re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_SR_FEMALE;
				break;

			case EFFECTSPRITETYPE_GUN_SG_FEMALE:
				re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_SG_FEMALE;
				break;
			}
		} 
		else
		if(nActionInfo == SKILL_JABBING_VEIN)
		{
			switch(re_est)
			{
			case EFFECTSPRITETYPE_GUN_AR_FEMALE :
				re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_AR_FEMALE;
				break;
			case EFFECTSPRITETYPE_GUN_SMG_FEMALE :
				re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_SMG_FEMALE;
				break;
			case EFFECTSPRITETYPE_GUN_SR_FEMALE :
				re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_SR_FEMALE;
				break;
			case EFFECTSPRITETYPE_GUN_SG_FEMALE :
				re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_SG_FEMALE;
				break;
			}
		}
		return re_est;
	}
	
	// 없는 경우는 남자꺼 사용..
	re_est = (*g_pActionInfoTable)[useActionInfo].GetActionEffectSpriteType();
	if(nActionInfo == SKILL_BULLET_OF_LIGHT)
	{
		switch(re_est)
		{
		case EFFECTSPRITETYPE_GUN_AR_MALE:
			re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_AR_MALE;
			break;
			
		case EFFECTSPRITETYPE_GUN_SMG_MALE:
			re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_SMG_MALE;
			break;
			
		case EFFECTSPRITETYPE_GUN_SR_MALE:
			re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_SR_MALE;
			break;
			
		case EFFECTSPRITETYPE_GUN_SG_MALE:
			re_est = EFFECTSPRITETYPE_BULLET_OF_LIGHT_SG_MALE;
			break;
		}
	} 
	else if(nActionInfo == SKILL_JABBING_VEIN)
	{
		switch(re_est)
		{
		case EFFECTSPRITETYPE_GUN_AR_MALE :
			re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_AR_MALE;
			break;
		case EFFECTSPRITETYPE_GUN_SMG_MALE :
			re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_SMG_MALE;
			break;
		case EFFECTSPRITETYPE_GUN_SR_MALE :
			re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_SR_MALE;
			break;
		case EFFECTSPRITETYPE_GUN_SG_MALE :
			re_est = EFFECTSPRITETYPE_JABBING_VEIN_GUN_SG_MALE;
			break;
		}
	}
	return re_est;	
}


//----------------------------------------------------------------------
// Packet SpecialAction Result
//----------------------------------------------------------------------
// this Creature는 Damage를 받았으므로
//
// 0. 모든 동작을 중지하고..
// 1. Action --> Damage받는 동작
// 2. Damage 표시
//----------------------------------------------------------------------
void		
MCreature::PacketSpecialActionResult(TYPE_ACTIONINFO nResultActionInfo, TYPE_OBJECTID id, TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, BYTE temp)
{
	//--------------------------------------------------
	// 죽었으면 return
	//--------------------------------------------------
	/*
	// 여기서 return해버리면 죽을때 맞는 이펙트가 안 보인다.
	if (!m_bAlive)
	{
		// 죽었을때는 죽는 effect말고는 보여주면 안된다.
		if (nResultActionInfo!=(*g_pCreatureTable)[m_CreatureType].DeadActionInfo)
			return;
	}
	*/

	//----------------------------------------------------------------------
	// 결과 action이 아닌 경우..  - 의미가 있나?? 흠..
	//----------------------------------------------------------------------
	if (nResultActionInfo < (*g_pActionInfoTable).GetMinResultActionInfo()
		// fast move, knockback중에는 action을 보여주지 않는다... // 2001.10.9
		|| m_bFastMove || m_bKnockBack || nResultActionInfo == ACTIONINFO_NULL)
	{
		return;
	}


	//----------------------------------------------------------------------
	// 방어막 효과.. - -;
	//----------------------------------------------------------------------
	if (GetActionInfoAction( nResultActionInfo )==ACTION_DAMAGED)
	{
		// 죽었을때 맞는 동작 안 보여주기..
		// 타일 기술때문에 맞기도 하더라는.. - -;
		// 2002.3.9
		if (!m_bAlive)
		{
			return;
		}

		//if (HasEffectStatus(EFFECTSTATUS_AURA_PRISM))
		{
		//	nResultActionInfo = REFLECT_AURA_PRISM;
		}
		//else 
			if (HasEffectStatus(EFFECTSTATUS_AURA_SHIELD))
		{
			nResultActionInfo = REFLECT_AURA_PRISM;//REFLECT_AURA_SHIELD;
		}

//		#ifndef CONNECT_SERVER
//			if (HasEffectStatus(EFFECTSTATUS_ARMAGEDDON))
//			{				
//				int hp = GetArmageddonHP()-30;
//				hp = max(0, hp);
//
//				SetStatus( MODIFY_ARMAGEDDON_HP, hp );
//			}
//		#endif
	}


	// 이동을 중지한다.
	////m_sX = 0;	
	////m_sY = 0;	
	//SetStop();

	//m_ActionCount = 0; 
	//------------------------------------------------------
	// 이미 NextUsedACtionInfo가 있는 경우 체크
	//------------------------------------------------------
	AffectBufferedActionInfo();
	
	ActionMoveNextPosition();

	// nResultActionInfo에 해당하는 ActionInfo를 찾아야 한다.
	// 원래ActionInfo + MIN_RESULT_ACTIONINFO를 하면 된다.
	m_nNextUsedActionInfo	= nResultActionInfo; // +(*g_pActionInfoTable).GetMinResultActionInfo()

	if( temp != 0)
		SetActionGrade( temp );

	if (m_bAlive)
	{
		BYTE nextAction = GetActionInfoAction(m_nNextUsedActionInfo);
		SetNextAction( nextAction );

		if (nextAction!=ACTION_STAND)
		{
			m_bNextAction = true;
		}
	}

	// 자신에게 사용?..
	SetTraceID( id );
	m_TraceX	= sX;
	m_TraceY	= sY;

	// Test Code
	/*
	if (rand()%2)
		SetChatString( "Shit!" );
	else
		SetChatString( "Oops!" );
	*/
}


//----------------------------------------------------------------------
// Packet AttackNormal (x,y, d)
//----------------------------------------------------------------------
// this Creature는 Attack 동작을 취한다.
//
// 0. 모든 동작을 중지하고..
// 1. Action Attack
//----------------------------------------------------------------------
void		
MCreature::PacketAttackNormal(TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, BYTE direction)
{
	//--------------------------------------------------
	// 죽었으면 return
	//--------------------------------------------------
	if (!m_bAlive)
		return;

	// 이동을 중지한다.
	m_sX = 0;	
	m_sY = 0;
	
	// server에서 날아온~ 정보 설정..
	//--------------------------------------------------
	// Player가 이미 그 자리에 있다면....
	//--------------------------------------------------
	// 못 간다~~
	/*
	if (g_pPlayer->GetX()==sX && g_pPlayer->GetY()==sY)
	{
		// message
		#ifdef	OUTPUT_DEBUG
			sprintf(g_pDebugMessage->GetCurrent(), "[Can't Move To Attack] : Player(%d, %d) Already Exist... ", g_pPlayer->GetX(), g_pPlayer->GetY());
			g_pDebugMessage->Next();
		#endif

		// 현재 Player가 있는 위치로 바라본다.
		SetDirectionToPosition( g_pPlayer->GetX(), g_pPlayer->GetY() );
	}	
	else
	*/
	{
		//--------------------------------------------------
		// 다른 Creature가 이미 있다면?
		//--------------------------------------------------
		/*
		bool bIsExistCreature;
		switch (m_MoveType)
		{
			case CREATURE_UNDERGROUND : 
				bIsExistCreature = m_pZone->GetSector(x,y).IsExistUndergroundCreature();
			break;

			case CREATURE_GROUND : 
				bIsExistCreature = m_pZone->GetSector(x,y).IsExistGroundCreature();
			break;

			case CREATURE_FLYING : 
				bIsExistCreature = m_pZone->GetSector(x,y).IsExistFlyingCreature();
			break;
		}
		*/	

		// 현재 좌표와 같은 경우.. 움직일 필요가 없다.
		if (m_X==sX && m_Y==sY)
		{
		}
		// 좌표가 다르면.. 일단 움직여주고 Attack해야 한다.
		else
		{
			// 이동
			MovePosition(sX, sY);
			
			// message
			DEBUG_ADD_FORMAT("[ Move To Attack ] : (%d, %d) --> (%d, %d),  Player=(%d, %d)", m_X, m_Y, sX, sY, g_pPlayer->GetX(), g_pPlayer->GetY());								
		}	

		// Server에서 날아온 정보 설정..		
		m_Direction = direction;
	}		

	m_ActionCount = 0; 

	// 공격하는 motion
	m_nNextUsedActionInfo	= SKILL_ATTACK_MELEE;
	SetNextAction( GetActionInfoAction(m_nNextUsedActionInfo) );	
}

//----------------------------------------------------------------------
// Packet SpecialAction To Sector
//----------------------------------------------------------------------
// this Creature는 nActionInfo에 따른 동작을 취한다.
//
// 0. 모든 동작을 중지하고..
// 1. SpecialAction
//----------------------------------------------------------------------
void		

MCreature::PacketSpecialActionToSector(TYPE_ACTIONINFO nActionInfo, TYPE_SECTORPOSITION sX, TYPE_SECTORPOSITION sY, MActionResult* pActionResult)
{
	DEBUG_ADD_FORMAT("PacketSpecialActionToSector ai : %d", nActionInfo );
	//--------------------------------------------------
	// 죽었으면 return
	//--------------------------------------------------
	if (!m_bAlive || (
		nActionInfo>=(*g_pActionInfoTable).GetMinResultActionInfo() &&
		!(nActionInfo>=STEP_SKILL_FLOURISH_2 && nActionInfo <MAGIC_ENCHANT_OPTION_NULL )
		)
		)
	{
		//DEBUG_ADD("delete?");
		if (pActionResult!=NULL)
		{	
			DEBUG_ADD("delete");
			delete pActionResult;
		}
//		DEBUG_ADD("return");
		return;
	}

	//------------------------------------------------------
	// Sector에 사용하는 기술이 맞는 경우
	//------------------------------------------------------
	//DEBUG_ADD("fuck");
	if ((*g_pActionInfoTable)[nActionInfo].IsTargetZone() ||
		nActionInfo == SKILL_TRANSFUSION || nActionInfo == SKILL_PIERCING || nActionInfo == SKILL_VIOLENT_PHANTOM 
		|| nActionInfo == SKILL_ICE_LANCE_MASTERY || nActionInfo == SKILL_ICE_LANCE
		|| nActionInfo == SKILL_DESTRUCTION_SPEAR_MASTERY || nActionInfo == SKILL_DESTRUCTION_SPEAR
		|| nActionInfo == SKILL_SPIT_STREAM || nActionInfo == SKILL_ROTTEN_APPLE || nActionInfo == SKILL_VAMPIRE_INNATE_DEADLY_CLAW
		)
	{
	//	DEBUG_ADD("hmhm");
		//---------------------------------------------------------------
		// 냠냠.. 2001.10.28
		//---------------------------------------------------------------
		AffectMoveBufferAll();
	//	DEBUG_ADD("3");

		ActionMoveNextPosition();
	//	DEBUG_ADD("2");

		// 이동을 중지한다.
	//	m_sX = 0;	
	//	m_sY = 0;
		if (nActionInfo==m_nSpecialActionInfo && m_nSpecialActionInfo!=ACTIONINFO_NULL)
		{
			m_RepeatCount ++;
		}
		else
		{
			m_RepeatCount = 0;
			m_ActionCount = 0;
		}

		m_nSpecialActionInfo = nActionInfo;
		//------------------------------------------------------
		// 이미 NextUsedACtionInfo가 있는 경우 체크
		//------------------------------------------------------
		AffectBufferedActionInfo();
	//	DEBUG_ADD("1");

		// Server에서 날아온 정보 설정..	
		m_nNextUsedActionInfo	= nActionInfo;
		m_TraceX		= sX;
		m_TraceY		= sY;
		m_TraceZ		= 0;	// z값은.. 의미가 있을까..
		// 2004, 11, 4, sobeit modify start - 그레이트 러피언 땜에...
		if(GetCreatureType() == 764 || GetCreatureType() == 765)
		{
			SetTraceID( GetID() );
			if(nActionInfo == SKILL_CHAIN_THROWING_AXE)
				SetShadowCount(3);
		}
		else
			SetTraceID( OBJECTID_NULL );
		// 2004, 11, 4, sobeit modify end

		
		// 목표를 향해서 바라본다.
		//SetDirectionToPosition(sX, sY);
		
	//	m_ActionCount = 0;

		// [ TEST CODE ]	
	//	DEBUG_ADD("0");
		BYTE nextAction = GetActionInfoAction(nActionInfo);

	//	DEBUG_ADD("9");
		SetNextAction( nextAction );
	//	DEBUG_ADD("8");

		if (nextAction!=ACTION_STAND)
		{
			m_bNextAction = true;
		}

		//---------------------------------------------------------------
		// 이미 표현해야할 결과가 있는 경우..
		//---------------------------------------------------------------
	//	DEBUG_ADD("7");
		if (m_pActionResult != NULL)
		{
			DEBUG_ADD("Execute Old Result");

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

	//	DEBUG_ADD("6");
		// 결과를 설정
		m_pActionResult = pActionResult;

	//	DEBUG_ADD("5");
	}
	//------------------------------------------------------
	// 아닌 경우
	//------------------------------------------------------
	else
	{
		if (pActionResult!=NULL)
		{	
			delete pActionResult;
		}
		DEBUG_ADD_FORMAT("[Skill : %s] Target is Not Sector", (*g_pActionInfoTable)[nActionInfo].GetName());					
	}
}

//----------------------------------------------------------------------
// Packet SpecialAction To Other
//----------------------------------------------------------------------
// this Creature는 nActionInfo에 따른 동작을 취한다.
//
// 0. 모든 동작을 중지하고..
// 1. SpecialAction
//----------------------------------------------------------------------
void		
MCreature::PacketSpecialActionToOther(TYPE_ACTIONINFO nActionInfo, TYPE_OBJECTID id, MActionResult* pActionResult)
{
	//--------------------------------------------------
	// 죽었으면 return
	//--------------------------------------------------
	if (!m_bAlive || 
		nActionInfo>=(*g_pActionInfoTable).GetMinResultActionInfo() &&
		!( nActionInfo >= STEP_SKILL_FLOURISH_2 && nActionInfo < MAGIC_ENCHANT_OPTION_NULL )
		)
	{
		if (pActionResult!=NULL)
		{	
			delete pActionResult;
		}
		return;
	}
	//------------------------------------------------------
	// 타인에게 사용하는 기술이 맞는 경우
	//------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsTargetOther())
	{
		// 대상이 되는 creature를 얻는다.
		MCreature* pCreature = g_pZone->GetCreature( id );
		
		if (pCreature==NULL)
		{
			// 그런 creature가 없을 경우
			DEBUG_ADD_FORMAT("[Skill : %s] There's no such creature(%d)", (*g_pActionInfoTable)[nActionInfo].GetName(), id);

			if (pActionResult!=NULL)
			{	
				delete pActionResult;
			}
			
			return;
		}

		//---------------------------------------------------------------
		// 냠냠.. 2001.10.28
		//---------------------------------------------------------------
		AffectMoveBufferAll();

		ActionMoveNextPosition();

		// 이동을 중지한다.
		//m_sX = 0;	
		//m_sY = 0;

		//------------------------------------------------------
		// 이전 동작이랑 같은 경우
		// 2001.7.23 추가
		//------------------------------------------------------
		if (nActionInfo==m_nSpecialActionInfo && m_nSpecialActionInfo!=ACTIONINFO_NULL)
		{
			m_RepeatCount ++;
		}
		else
		{
			m_RepeatCount = 0;
			m_ActionCount = 0;
		}

		m_nSpecialActionInfo = nActionInfo;

		//------------------------------------------------------
		// 이미 NextUsedACtionInfo가 있는 경우 체크
		//------------------------------------------------------
		AffectBufferedActionInfo();
				
		// Server에서 날아온 정보 설정..	
		m_nNextUsedActionInfo	= nActionInfo;
		m_TraceX		= pCreature->GetX();
		m_TraceY		= pCreature->GetY();
		m_TraceZ		= pCreature->GetZ();
		SetTraceID ( id );

		// 목표를 향해서 바라본다.		
		//SetDirectionToPosition(pCreature->GetX(), pCreature->GetY());
		
		//m_ActionCount = 0;

		// [ TEST CODE ]	
		BYTE nextAction = GetActionInfoAction(nActionInfo);
		SetNextAction( nextAction );

		if (nextAction!=ACTION_STAND)
		{
			m_bNextAction = true;
		}

		//---------------------------------------------------------------
		// 이미 표현해야할 결과가 있는 경우..
		//---------------------------------------------------------------		
		if (m_pActionResult != NULL)
		{
			DEBUG_ADD("Execute Old Result");

			// 이미 있던 결과를 처리해준다.	
			// [ TEST CODE ]
			// 결과 처리..			
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
	//------------------------------------------------------
	// 아닌 경우
	//------------------------------------------------------
	else
	{
		if (pActionResult!=NULL)
		{	
			delete pActionResult;
		}

		DEBUG_ADD_FORMAT("[Skill : %s] Target is Not Other", (*g_pActionInfoTable)[nActionInfo].GetName());							
	}
}

//----------------------------------------------------------------------
// Packet SpecialAction To InventoryItem
//----------------------------------------------------------------------
// casting동작만 보여주게 된다.
//----------------------------------------------------------------------
void		
MCreature::PacketSpecialActionToInventoryItem(TYPE_ACTIONINFO nActionInfo)
{
	//--------------------------------------------------
	// 죽었으면 return
	//--------------------------------------------------
	if (!m_bAlive || nActionInfo>=(*g_pActionInfoTable).GetMinResultActionInfo())
		return;

	//------------------------------------------------------
	// 타인에게 사용하는 기술이 맞는 경우
	//------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsTargetItem())
	{		
		//------------------------------------------------------
		// 이미 NextUsedACtionInfo가 있는 경우 체크
		//------------------------------------------------------
		AffectBufferedActionInfo();
				
		// Server에서 날아온 정보 설정..	
		m_nNextUsedActionInfo	= nActionInfo;
		m_TraceX		= m_X;
		m_TraceY		= m_Y;
		m_TraceZ		= m_Z;
		SetTraceID( m_ID );

		BYTE nextAction = GetActionInfoAction(nActionInfo);
		SetNextAction( nextAction );

		if (nextAction!=ACTION_STAND)
		{
			m_bNextAction = true;
		}

		//---------------------------------------------------------------
		// 이미 표현해야할 결과가 있는 경우..
		//---------------------------------------------------------------
		if (m_pActionResult != NULL)
		{
			DEBUG_ADD("Execute Old Result");

			// 이미 있던 결과를 처리해준다.	
			// [ TEST CODE ]
			// 결과 처리..			
			// (!) m_pActionResult값이 Execute에서 변할 수 있어서 
			//		저장했다가 지워줘야 한다.
			MActionResult* pOldResult = m_pActionResult;

			m_pActionResult = NULL;

			pOldResult->Execute();

			delete pOldResult;	
		}
	}
	//------------------------------------------------------
	// 아닌 경우
	//------------------------------------------------------
	else
	{
		DEBUG_ADD_FORMAT("[Skill : %s] Target is Not Item", (*g_pActionInfoTable)[nActionInfo].GetName());							
	}
}

//----------------------------------------------------------------------
// Packet SpecialAction To Nobody
//----------------------------------------------------------------------
//
// 대상이 Zone에 없는 경우...
//
//----------------------------------------------------------------------
void		
MCreature::PacketSpecialActionToNobody(TYPE_ACTIONINFO nActionInfo, TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)
{
	//--------------------------------------------------
	// 죽었으면 return
	//--------------------------------------------------
	if (!m_bAlive || nActionInfo>=(*g_pActionInfoTable).GetMinResultActionInfo())
		return;

	//------------------------------------------------------
	// 타인에게 사용하는 기술이 맞는 경우
	//------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsTargetOther())
	{
		//---------------------------------------------------------------
		// 냠냠.. 2001.10.28
		//---------------------------------------------------------------
		AffectMoveBufferAll();

		ActionMoveNextPosition();

		//------------------------------------------------------
		// 이미 NextUsedACtionInfo가 있는 경우 체크
		//------------------------------------------------------
		AffectBufferedActionInfo();
				
		// Server에서 날아온 정보 설정..	
		m_nNextUsedActionInfo	= nActionInfo;
		m_TraceX		= x;
		m_TraceY		= y;
		m_TraceZ		= 0;
		SetTraceID( OBJECTID_NULL );

		// 목표를 향해서 바라본다.		
		//SetDirectionToPosition(pCreature->GetX(), pCreature->GetY());
		
		//m_ActionCount = 0;

		// [ TEST CODE ]	
		BYTE nextAction = GetActionInfoAction(nActionInfo);
		SetNextAction( nextAction );

		if (nextAction!=ACTION_STAND)
		{
			m_bNextAction = true;
		}
	}
	//------------------------------------------------------
	// 아닌 경우
	//------------------------------------------------------
	else
	{
		DEBUG_ADD_FORMAT("[Skill : %s] Target is Not Other", (*g_pActionInfoTable)[nActionInfo].GetName());	
	}
}

//----------------------------------------------------------------------
// Packet SpecialAction To Self
//----------------------------------------------------------------------
// this Creature는 nActionInfo에 따른 동작을 취한다.
//
// 0. 모든 동작을 중지하고..
// 1. SpecialAction
//----------------------------------------------------------------------
void		
MCreature::PacketSpecialActionToSelf(TYPE_ACTIONINFO nActionInfo, MActionResult* pActionResult)
{
	//--------------------------------------------------
	// 죽었으면 return
	//--------------------------------------------------
	if (!m_bAlive || nActionInfo>=(*g_pActionInfoTable).GetMinResultActionInfo() || nActionInfo == ACTIONINFO_NULL)
	{
		if (pActionResult!=NULL)
		{	
			delete pActionResult;
		}
		return;
	}

	//------------------------------------------------------
	// 자신한테 사용하는 기술이 맞는 경우
	//------------------------------------------------------
	if ((*g_pActionInfoTable)[nActionInfo].IsTargetSelf())
	{
		//---------------------------------------------------------------
		// 냠냠.. 2001.10.28
		//---------------------------------------------------------------
		AffectMoveBufferAll();

		ActionMoveNextPosition();

		// 이동을 중지한다.
	//	m_sX = 0;	
	//	m_sY = 0;
		//------------------------------------------------------
		// 이미 NextUsedACtionInfo가 있는 경우 체크
		//------------------------------------------------------
		AffectBufferedActionInfo();
				
		// Server에서 날아온 정보 설정..	
		m_nNextUsedActionInfo	= nActionInfo;
		m_TraceX		= m_X;
		m_TraceY		= m_Y;
		m_TraceZ		= m_Z;	// z값은.. 의미가 있을까..
		SetTraceID( m_ID ) ;
		
	//	m_ActionCount = 0;

		// [ TEST CODE ]	
		BYTE nextAction = GetActionInfoAction(nActionInfo, true);
		SetNextAction( nextAction );

		if (nextAction!=ACTION_STAND)
		{
			m_bNextAction = true;
		}

		//---------------------------------------------------------------
		// 이미 표현해야할 결과가 있는 경우..
		//---------------------------------------------------------------
		if (m_pActionResult != NULL)
		{
			DEBUG_ADD("Execute Old Result");

			// 이미 있던 결과를 처리해준다.	
			// [ TEST CODE ]
			// 결과 처리..			
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
	//------------------------------------------------------
	// 아닌 경우
	//------------------------------------------------------
	else
	{
		if (pActionResult!=NULL)
		{	
			delete pActionResult;
		}
		DEBUG_ADD_FORMAT("[Skill : %s] Target is Not Self", (*g_pActionInfoTable)[nActionInfo].GetName());							
	}
}

//----------------------------------------------------------------------
// Set Status
//----------------------------------------------------------------------
void	
MCreature::SetStatus(DWORD n, DWORD value)
{
	if(n >= MAX_MODIFY)
		return;

	DWORD oldValue = m_Status[n];

	m_Status[n] = value; 

	switch (n)
	{
		//--------------------------------------------------
		// Guild ID 바뀜
		//--------------------------------------------------
		case MODIFY_GUILDID :
		{
			m_Status[n] = value;
			SetGuildNumber( value );
		}
		break;
		case MODIFY_UNIONID :
		{
			SetUnionGuildID( value );
		}
		break;

		// 2004, 10, 19, sobeit add start

		// 2004, 10, 19, sobeit add end
		//--------------------------------------------------
		// 성향 바뀜
		//--------------------------------------------------
		case MODIFY_ALIGNMENT :			// 성향
		{
			//--------------------------------------------------
			// -10000 ~ 10000을 0~4로 바꾼다.
			//--------------------------------------------------
			value = ConvertAlignment( value );
			m_Status[n] = value;	
		}
		break;

		//--------------------------------------------------
		// HP 바뀌는 경우
		//--------------------------------------------------
		case MODIFY_CURRENT_HP :
			{
				const int localValue = min(GetMAX_HP()-GetSilverDamage(), value);
				AddHPModify( localValue - oldValue );

				// max를 안 넘게
				m_Status[n] = localValue;	

				// 파티인 경우는 HP 다시 설정해준다.
				if (IsPlayerParty())
				{
					PARTY_INFO* pInfo = g_pParty->GetMemberInfo( GetName() );
						
					// 좌표를 수정해준다.
					if (pInfo!=NULL)
					{
						pInfo->HP = localValue;
					}
				}
			}
		break;

		//--------------------------------------------------
		// MaxHP 바뀌는 경우
		//--------------------------------------------------
		case MODIFY_MAX_HP :
			// 파티인 경우는 HP 다시 설정해준다.
			if (IsPlayerParty())
			{
				PARTY_INFO* pInfo = g_pParty->GetMemberInfo( GetName() );
					
				// 좌표를 수정해준다.
				if (pInfo!=NULL)
				{
					pInfo->MaxHP = value;
				}
			}
		break;

//		//--------------------------------------------------
//		// Armageddon HP 바뀌는 경우
//		//--------------------------------------------------
//		case MODIFY_ARMAGEDDON_HP :
//		{
//			int changeValue = IsChangedArmageddonSprite(oldValue, value);
//
//			if (HasEffectStatus(EFFECTSTATUS_ARMAGEDDON)
//				&& changeValue)
//			{
//				RemoveEffectStatus(EFFECTSTATUS_ARMAGEDDON);
//
//				// 다시 추가				
//				if (value!=0)
//				{
//					AddEffectStatus(EFFECTSTATUS_ARMAGEDDON, 0xFFFF);
//				}
//
//				// 부서지는 부분 표시
//				int skillID = ARMAGEDDON_CRASH_1 + (changeValue-1);
//				
//				//--------------------------------------------------
//				// 부서지는 Effect
//				//--------------------------------------------------		
//				ExecuteActionInfoFromMainNode(
//					skillID,
//				
//					m_X, m_Y, 0,
//					m_Direction,
//					
//					m_ID,												// 목표에 대한 정보
//					m_X, m_Y, 0, 
//					
//					0,													// 기술의 (남은) 지속 시간		
//					
//					NULL,
//					
//					true);
//			}
//		}
//		break;
	}
}

//----------------------------------------------------------------------
// Change To Slayer
//----------------------------------------------------------------------
// slayer로 변신하는데..
// 이미 slayer이면.. return false
//----------------------------------------------------------------------
bool	
MCreature::ChangeToSlayer()
{
	//-----------------------------------------------------
	// 이미 slayer이면..
	//-----------------------------------------------------
	if (IsSlayer())
	{
		return false;
	}

	ClearAttachEffect();

	//-----------------------------------------------------
	// 정보 설정
	//-----------------------------------------------------
	SetMoveDevice( MOVE_DEVICE_WALK );
	SetBasicActionInfo( SKILL_ATTACK_MELEE );

	//-----------------------------------------------------
	// 정지 시킨다.
	//-----------------------------------------------------
	SetStop();

	//-----------------------------------------------------
	// 남, 여 ?
	//-----------------------------------------------------
	if (IsMale())
	{
		SetCreatureType( CREATURETYPE_SLAYER_MALE );
	}
	else
	{
		SetCreatureType( CREATURETYPE_SLAYER_FEMALE );
	}

	//-----------------------------------------------------
	// 번쩍
	//-----------------------------------------------------
	//g_pTopView->SetFadeStart(1, 31, 6,  5,5,31);

	return true;
}

//----------------------------------------------------------------------
// Change To Vampire
//----------------------------------------------------------------------
// vampire로 변신하는데..
// 이미 vampire이면.. return false
//----------------------------------------------------------------------
bool
MCreature::ChangeToVampire()
{
	//-----------------------------------------------------
	// 이미 vampire이면..
	//-----------------------------------------------------
	if (IsVampire())
	{
		return false;
	}

	ClearAttachEffect();

	DEBUG_ADD("MCreature::ChangeToVampire - RemoveEffectStatus OK");
	
	//-----------------------------------------------------
	// 정보 설정
	//-----------------------------------------------------
	SetMoveDevice( MOVE_DEVICE_WALK );
	SetBasicActionInfo( SKILL_ATTACK_MELEE );

	//-----------------------------------------------------
	// 정지 시킨다.
	//-----------------------------------------------------
	SetStop();

	//-----------------------------------------------------
	// 남, 여 ?
	//-----------------------------------------------------
	if (IsMale())
	{
		// 변신 소리
		PlaySound(SOUND_VAMPIRE_CHANGE_MALE, 
					false,
					m_X,
					m_Y);

		SetCreatureType( CREATURETYPE_VAMPIRE_MALE1 );
	}
	else
	{
		// 변신 소리
		PlaySound(SOUND_VAMPIRE_CHANGE_FEMALE, 
					false,
					m_X,
					m_Y);

		SetCreatureType( CREATURETYPE_VAMPIRE_FEMALE1 );
	}

	//-----------------------------------------------------
	// 복장 제거
	//-----------------------------------------------------
	// 안해도 된다.
	//for (int i=0; i<ADDON_MAX; i++)
	//{
	//	ClearAddonInfo( i );
	//}

	//-----------------------------------------------------
	// 번쩍
	//-----------------------------------------------------
	//g_pTopView->SetFadeStart(1, 31, 6,  31,0,0);

	DEBUG_ADD("MCreature::ChangeToVampire OK");
	
	return true;
}

//----------------------------------------------------------------------
// Fast Move Position
//----------------------------------------------------------------------
// (m_X, m_Y) --> (x, y)
//----------------------------------------------------------------------
bool		
MCreature::FastMovePosition(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y,bool server)
{
	//if (m_X!=x || m_Y!=y)
	{
		//------------------------------------------------
		// 정지
		//------------------------------------------------
		SetStop();

		m_nNextUsedActionInfo = ACTIONINFO_NULL;
		m_bNextAction = false;
		
		//------------------------------------------------
		// fast move 설정
		//------------------------------------------------	
		SetAction( m_MoveAction );

		//MovePosition( x, y );

		m_NextX = x;
		m_NextY = y;
		SetServerPositionWithBlock( x, y );

		// SetAction하고 나서 설정해줘야 한다.
		m_bFastMove = true;

		return true;
	}

	// 어쨋든 서버좌표는 검증됐다고 본다.
	//m_ServerX = x;
	//m_ServerY = y;

	return false;
}

//----------------------------------------------------------------------
// KnockBackPosition
//----------------------------------------------------------------------
bool
MCreature::KnockBackPosition(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y, BYTE Action)
{
	//if (m_X!=x || m_Y!=y)
	{
		// 뜨아 이거 bool아닌뎅.. m_b로 시작하네 - -; 컴파일 다시 하기 귀차나서  --;

		if( Action == 0 )
		{
			//int knockBackCount = (*g_pCreatureTable)[m_CreatureType].GetActionCount( ACTION_DAMAGED );
			int knockBackCount = GetCreatureActionCountMax( this, ACTION_DAMAGED );
			
			if (knockBackCount<=0)
			{
				// 맞기 동작이 없당 - -;
				return false;
			}
			
			//------------------------------------------------
			// 정지
			//------------------------------------------------
			SetStop();
			
			m_nNextUsedActionInfo = ACTIONINFO_NULL;
			m_bNextAction = false;

			//------------------------------------------------
			// KnockBack 동작으로 damaged를 설정
			//------------------------------------------------	
			SetAction( ACTION_DAMAGED );
			
			//------------------------------------------------
			// 캐릭터의 Action에 맞는 Sound를 출력해준다.
			//------------------------------------------------
			TYPE_SOUNDID soundID = (*g_pCreatureTable)[m_CreatureType].GetActionSound( ACTION_DAMAGED );
			
			if (soundID!=SOUNDID_NULL)
			{
				PlaySound( soundID,
					false,
					m_X,
					m_Y);
			}
			m_bKnockBack = knockBackCount;
		}
		else
		{
			m_bKnockBack = 3;
			AddEffectStatus( EFFECTSTATUS_FADE_OUT, 8 );
		}
		
		
		//------------------------------------------------	
		// sx, sy로 좌표 보정
		//
		// 현재위치(m_X, m_Y)에서 KnockBack위치(x, y)로 이동.
		// 
		//------------------------------------------------	
		int oldX = m_X;
		int oldY = m_Y;
		int newX = x;
		int newY = y;
		POINT	oldPoint = g_pTopView->MapToPixel(oldX, oldY);
		POINT	newPoint = g_pTopView->MapToPixel(newX, newY);
		
		// 이동해야할 pixel수 계산
		int gapX = newPoint.x - oldPoint.x; //+ m_sX);
		int gapY = newPoint.y - oldPoint.y; //+ m_sY);

		m_sX = -gapX;
		m_sY = -gapY;

		// 한번에 이동해야할 pixel수 --> (m_cX, m_cY)에 넣어둔다.
		m_cX = gapX / m_bKnockBack;
		m_cY = gapY / m_bKnockBack;

		
		// 좌표가 달라졌을 경우만..
		if (newX!=oldX || newY!=oldY)
		{
			//------------------------------------------------	
			// KnockBack되는 방향 설정
			// 새좌표에서 예전좌표를 바라보는 방향
			//------------------------------------------------	
			m_Direction = MTopView::GetDirectionToPosition(newX, newY, oldX, oldY);
		
			//------------------------------------------------	
			// 좌표는 바로 이동.
			//------------------------------------------------			
			MovePosition( x, y );
		}

		SetServerPositionWithBlock( x, y );

		if( Action == 0 )
		{
			//------------------------------------------------			
			// 몸에 붙는 피
			//------------------------------------------------			
			MAttachEffect* pEffect = NULL;
			if(g_pUserInformation->GoreLevel == false)
			{
				pEffect = CreateAttachEffect( EFFECTSPRITETYPE_GREEN_BLOOD, 6, 6);		
			}
			else
			{
				pEffect = CreateAttachEffect( EFFECTSPRITETYPE_BLOOD, 6, 6);		
			}
			
			if (pEffect != NULL)
			{
				pEffect->SetDirection( m_Direction );
				pEffect->SetAttachCreatureID( m_ID );
			}						
			
			//------------------------------------------------			
			// 바닥에 흘리는 피 3방울(-_-;)
			//------------------------------------------------			
			if (g_pUserOption->BloodDrop)
			{
				int num = rand()%3 + 3;
				for (int i=0; i<num; i++)
				{
					ExecuteActionInfoFromMainNode(
						BLOOD_CRITICAL_HIT,										// 사용 기술 번호
						
						oldX, oldY, 0,
						(int)m_Direction,														// 사용 방향
						
						m_ID,												// 목표에 대한 정보
						oldX, oldY, 0,
						
						50 + rand()%30, //5*16, 
						
						NULL,
						
						false);	
				}
			}
		}
		SetServerPosition( x,y);

		return true;
	}

	// 어쨋든 서버좌표는 검증됐다고 본다.
	//m_ServerX = x;
	//m_ServerY = y;

	return false;
}

//----------------------------------------------------------------------
// Stop Fast Move
//----------------------------------------------------------------------
void				
MCreature::StopFastMove()
{
	m_sX = 0;
	m_sY = 0;
	m_bFastMove = false;

	// 2001.8.10에 추가
	// 컥..근데 이 함수 쓰이지도 않는군.. - -;;
	m_MoveCount = m_MoveCountMax;
}

//----------------------------------------------------------------------
// Find Enemy
//----------------------------------------------------------------------
int
MCreature::FindEnemy()
{
	// 지금은 MPlayer에서만 사용한다.
	// 미래를 위해서...
	return OBJECTID_NULL;
}

//----------------------------------------------------------------------
// Set ShadowCount
//----------------------------------------------------------------------
void				
MCreature::SetShadowCount(int n)
{ 
	// 5개까지.. - -;
	m_ShadowCount = min(5, n); 
}

//----------------------------------------------------------------------
// Stop Blood Drain
//----------------------------------------------------------------------
// m_bStopBloodDrain이면 이걸 호출해야 한다.
//----------------------------------------------------------------------
void	
MCreature::StopBloodDrain()
{
	if (m_Action==ACTION_VAMPIRE_DRAIN
		&& IsVampire()
		&& GetActionCountMax()>60)
	{
		if( m_ActionCount>9 && m_ActionCount<70 && GetCreatureType() == CREATURETYPE_WER_WOLF )
		{
			m_ActionCount = 70;
			UnSetStopBloodDrain();
		} else
		if (m_ActionCount>9 && m_ActionCount<60)
		{
			m_ActionCount = 60;

			UnSetStopBloodDrain();
		}	
	}
	else
	{
		//UnSetStopBloodDrain();
	}
}

//----------------------------------------------------------------------
// Stop Absorb Soul
//----------------------------------------------------------------------
// m_bStopAbsorbSoul이면 이걸 호출해야 한다.
//----------------------------------------------------------------------
void	
MCreature::StopAbsorbSoul()
{
	if (m_Action==ACTION_OUSTERS_DRAIN
		&& IsOusters()
		&& GetActionCountMax()>30)
	{
		if (m_ActionCount>9 && m_ActionCount<30)
		{
			m_ActionCount = 30;

			ATTACHEFFECT_LIST::iterator itr = m_listEffect.begin();
			ATTACHEFFECT_LIST::iterator endItr = m_listEffect.end();

			while(itr != endItr)
			{
				MAttachEffect*	pEffect = *itr;
				
				if(pEffect->GetEffectSpriteType() == EFFECTSPRITETYPE_ABSORB_SOUL)
				{
					bool bUseEffectSprite = pEffect->IsEffectSprite();
					
					// flag제거
					if (bUseEffectSprite)
					{
						m_bAttachEffect[pEffect->GetEffectSpriteType()] = false;	// flag제거
					}

					delete pEffect;
					m_listEffect.erase(itr);
					break;
				}
				itr++;
			}
			
			UnSetStopAbsorbSoul();
		}	
	}
	else
	{
		//UnSetStopBloodDrain();
	}
}

//----------------------------------------------------------------------
// Update Darkness
//----------------------------------------------------------------------
// 단순히 count만 바꿔준다.
//----------------------------------------------------------------------
void	
MCreature::UpdateDarkness()
{
	if (!IsVampire() || IsVampire() && g_pZone->GetID() == 3001
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		)
	{
		if (m_DarknessCountInc==0)
		{
			if (m_ActionCount>=GetActionCountMax()-1)
			{
				CheckInDarkness();
			}
		}
		else
		{
			m_DarknessCount += m_DarknessCountInc;

			// darkness에 들어가는 경우
			if (m_DarknessCount < 0)
			{
				m_DarknessCount = 0;
				m_DarknessCountInc = 0;
			}
			// darkness에서 빠져나오는 경우
			else if (m_DarknessCount >= MAX_DARKNESS_COUNT)
			{
				m_DarknessCount = -1;
				m_DarknessCountInc = 0;
			}
		}
	}
	else
	{
		// Slayer가 아닌 경우는 Darkness랑 관계없다.
		// 확인용
		m_DarknessCount = -1;
		m_DarknessCountInc = 0;
	}
}

//----------------------------------------------------------------------
// Check In Darkness
//----------------------------------------------------------------------
// darkness가 나타나고/사라지는 순간
//            player가 움직이는 순간을 체크하면 된다.
//
// player가 아니므로  시야 관계없당.
//----------------------------------------------------------------------
void
MCreature::CheckInDarkness()
{
	if (g_pZone!=NULL)
	{
		//if (IsSlayer())
		{
			//-------------------------------------------------------
			// Darkness에 들어왔는지 체크
			//-------------------------------------------------------
			// 서버 좌표보다는 현재 좌표가 보기에 좋다.. ㅋㅋ..
			if (m_X >=0 && m_X < g_pZone->GetWidth()
				&& m_Y >= 0 && m_Y < g_pZone->GetHeight())
			{
				const MSector& sector = g_pZone->GetSector( m_X, m_Y );

				if (m_bAlive && (sector.HasDarkness()))
				{
					// darkness에 있는 경우
					//PlaceInDarkness();
					m_DarknessCount = MAX_DARKNESS_COUNT;
					m_DarknessCountInc = 0;
				}
				else
				{		
					// darkness는 사라졌다.
					//PlaceNotInDarkness();
					m_DarknessCount = -1;
					m_DarknessCountInc = 0;
				}
			}
		}
	}
}

BOOL
MCreature::IsInGroundElemental() const
{
	if (g_pZone!=NULL)
	{
		//if (IsSlayer())
		{
			//-------------------------------------------------------
			// Darkness에 들어왔는지 체크
			//-------------------------------------------------------
			// 서버 좌표보다는 현재 좌표가 보기에 좋다.. ㅋㅋ..
			if (m_X >=0 && m_X < g_pZone->GetWidth()
				&& m_Y >= 0 && m_Y < g_pZone->GetHeight())
			{
				const MSector& sector = g_pZone->GetSector( m_X, m_Y );

				if (m_bAlive && sector.HasGroundElemental())
				{
					return true;
				}
			}
		}
	}
	return false;
}

BOOL
MCreature::IsInFuryOfGnome() const
{
	if (g_pZone!=NULL)
	{
		//if (IsSlayer())
		{
			//-------------------------------------------------------
			// Fury Of Gnome에 들어왔는지 체크
			//-------------------------------------------------------
			// 서버 좌표보다는 현재 좌표가 보기에 좋다.. ㅋㅋ..
			if (m_X >=0 && m_X < g_pZone->GetWidth()
				&& m_Y >= 0 && m_Y < g_pZone->GetHeight())
			{
				const MSector& sector = g_pZone->GetSector( m_X, m_Y );

				if (m_bAlive && sector.HasFuryOfGnome())
				{
					return true;
				}
			}
		}
	}
	return false;
}

//----------------------------------------------------------------------
// Place In Darkness
//----------------------------------------------------------------------
// player는 darkness에 있다.
//----------------------------------------------------------------------
void
MCreature::PlaceInDarkness(bool IsBlindness)
{
	if (
		  (
				(IsBlindness || (!IsVampire() || IsVampire() && g_pZone->GetID() == 3001))
				
				&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
			)
		  
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		)
	{
		// darkness에 있지 않던 경우
		if (m_DarknessCount < 0)
		{
			m_DarknessCount = MAX_DARKNESS_COUNT;		
			m_DarknessCountInc = -1;
		}	
		else if (m_DarknessCount > 0)
		{
			m_DarknessCountInc = -1;
		}
		else if (m_DarknessCount==0)
		{
			m_DarknessCountInc = 0;
		}
	}
}

//----------------------------------------------------------------------
// Move In Darkness
//----------------------------------------------------------------------
// player는 darkness에 있지 않다.
//----------------------------------------------------------------------
void
MCreature::PlaceNotInDarkness()
{
	if (!IsVampire() || IsVampire() && g_pZone->GetID() == 3001
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		)
	{
		// darkness에 있던 경우
		if (m_DarknessCount >= 0 && m_DarknessCount < MAX_DARKNESS_COUNT)
		{
			m_DarknessCountInc = 1;
		}	
	}
}

//----------------------------------------------------------------------
// Show In Darkness
//----------------------------------------------------------------------
// darkness에 있지 않아서 잘 보이거나
// darkness에 있더라도 거리에 의해서 보이는가?
//----------------------------------------------------------------------
BOOL	
MCreature::ShowInDarkness(int sX, int sY) const
{

//	DEBUG_ADD_FORMAT("[ShowInDarkness] g_bLight %d", g_bLight);
	// 이미 darkness에 있는 경우
	if (//!(m_CreatureType >= 526 && m_CreatureType <= 549 || m_CreatureType >= 371 && m_CreatureType <= 376 || m_CreatureType >= 560 && m_CreatureType <= 563) &&
//		!IsNPC() &&
		m_DarknessCount >= 0 &&
		(!HasEffectStatus( EFFECTSTATUS_LIGHTNESS ) || g_pZone->GetID() == 3001 )
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
	)
	{
		// darknessCount안에는 볼 수 있다.
		if ((max(abs(m_X-sX), abs(m_Y-sY))) <= m_DarknessCount)
		{
			return TRUE;
		}

		return FALSE;
	}

	return TRUE;
}

//----------------------------------------------------------------------
// Set HalluCreature
//----------------------------------------------------------------------
// hallu인 경우... 어떤 몹으로 보여지게 된다.
//----------------------------------------------------------------------
void				
MCreature::SetHalluCreature(TYPE_CREATURETYPE type)
{
	// NPC는 Hallu걸려도 제대로 보인다.
	if (IsNPC())
	{
		m_bHallu = false;
		return;
	}

	m_HalluCreatureType = type;
	
	m_HalluCreatureFrameID	= (*g_pCreatureSpriteTable)[(*g_pCreatureTable)[type].SpriteTypes[0]].FrameID;
	
	// 색깔 - 몹인 경우만.. 설정해준다.
	m_HalluColorBody	= (*g_pCreatureTable)[m_CreatureType].ColorSet;

//	m_HalluName = rand()%g_pMonsterNameTable->GetLastNameSize();

	m_bHallu = true;
}

//----------------------------------------------------------------------
// Determine HalluAction
//----------------------------------------------------------------------
void			
MCreature::DetermineHalluActionFrame()
{
	//------------------------------------------------------
	// 적절한 action을 선택한다.
	//------------------------------------------------------
	if (m_Action <= ACTION_DIE)
	{
		m_HalluAction = m_Action;
	}
	else if (m_Action==ACTION_SLAYER_MOTOR_STAND)
	{
		m_HalluAction = ACTION_STAND;
	}
	else if (m_Action==ACTION_SLAYER_MOTOR_MOVE)
	{
		m_HalluAction = ACTION_MOVE;
	}
	else
	{
		m_HalluAction = ACTION_ATTACK;
	}

	BYTE frame = GetFrame();

	//------------------------------------------------------
	// 동작의 frame 수를 결정한다.
	//------------------------------------------------------
	int countMax = (*g_pCreatureTable)[m_HalluCreatureType].GetActionCount( m_HalluAction );

	if (countMax==0)
	{
		// 동작이 없다면..
		if (m_HalluAction!=ACTION_ATTACK)
		{
			m_HalluAction = ACTION_ATTACK;

			countMax = (*g_pCreatureTable)[m_HalluCreatureType].GetActionCount( m_HalluAction );
		}

		// 동작이 없다면 정지..
		if (countMax==0)
		{
			m_HalluAction = ACTION_STAND;

			countMax = (*g_pCreatureTable)[m_HalluCreatureType].GetActionCount( m_HalluAction );						
		}
	}
	
	if (countMax==0)	// 비교 비교.. - -;
	{
		m_HalluFrame = 0;
	}
	else
	{
		m_HalluFrame = frame % countMax;
	}
}

//----------------------------------------------------------------------
// Get HalluName
//----------------------------------------------------------------------
const char*			
MCreature::GetHalluName() const
{ 
	if( strncmp( GetName(), (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetString(), (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetLength() ) == 0 )
//		strstr(GetName(), "운영자") != NULL)
		return GetName();
	return g_pMonsterNameTable->GetLastName(m_HalluName);
}

//----------------------------------------------------------------------
// Set Regen
//----------------------------------------------------------------------
void				
MCreature::SetRegen(int amount, DWORD delay)
{ 
	if(this == g_pPlayer && IsVampire() && g_pPlayer->GetCreatureType() != CREATURETYPE_BAT)
	{
		const int dex = g_pPlayer->GetBASIC_DEX();
		if(dex < 51)
			;
		else if(dex > 50 && dex < 121)
			amount += 1;
		else if(dex > 120 && dex < 191)
			amount += 2;
		else if(dex > 190 && dex < 261)
			amount += 3;
		else if(dex > 260 && dex < 331)
			amount += 4;
		else if(dex > 330 && dex < 391)
			amount +=5;
		else if(dex > 390 && dex < 451)
			amount +=6;
		else if(dex > 450)
			amount +=7;

		if((*g_pRankBonusTable)[RANK_BONUS_URANUS_BLESS].GetStatus() == RankBonusInfo::STATUS_LEARNED)
			amount += (*g_pRankBonusTable)[RANK_BONUS_URANUS_BLESS].GetPoint();

		if( HasEffectStatus( EFFECTSTATUS_WILL_OF_LIFE ) )
			amount += (5+ ( GetLEVEL() / 7 ) );		
	}
	m_RegenAmount = amount; 
	
	m_RegenDelayTime = delay; 
	m_RegenNextTime = g_CurrentTime; 
}

//----------------------------------------------------------------------
// Set Regen Bonus
//----------------------------------------------------------------------
void				
MCreature::SetRegenBonus(int amount, DWORD delay)
{ 
	m_RegenBonusAmount = amount; 
	m_RegenBonusDelayTime = delay; 
	m_RegenBonusNextTime = g_CurrentTime; 
}

//----------------------------------------------------------------------
// CheckRegen - Regen설정
//----------------------------------------------------------------------
void
MCreature::CheckRegen()
{
	//-------------------------------------------------------
	// 뱀파이어가 아니면 Regen하지 않는다.
	//-------------------------------------------------------
	if (!IsVampire())
	{
		SetRegen( 0, 0 );
		return;
	}

	//-------------------------------------------------------
	// Burrow상태
	//-------------------------------------------------------
	if (IsUndergroundCreature())
	{
		SetRegen( g_pClientConfig->REGEN_AMOUNT_BURROW, 1000 );
		return;
	}

	//-------------------------------------------------------
	// Casket 상태
	//-------------------------------------------------------
	if (IsInCasket())
	{
		SetRegen( g_pClientConfig->REGEN_AMOUNT_CASKET, 1000 );
		return;
	}

	//-------------------------------------------------------
	// Creature Type에 따라서..
	//-------------------------------------------------------
	switch (m_CreatureType)
	{
		//---------------------------------------------------------------
		// 뱀파이어, 늑대
		//---------------------------------------------------------------
		case CREATURETYPE_VAMPIRE_MALE1 :
		case CREATURETYPE_VAMPIRE_FEMALE1 :
		case CREATURETYPE_VAMPIRE_MALE2 :
		case CREATURETYPE_VAMPIRE_FEMALE2 :
		case CREATURETYPE_VAMPIRE_MALE3 :
		case CREATURETYPE_VAMPIRE_FEMALE3 :
		case CREATURETYPE_VAMPIRE_OPERATOR :
		case CREATURETYPE_WOLF :
		case CREATURETYPE_WER_WOLF :
		// add by coffee 2006.11.24  
		case CREATURETYPE_VAMPIRE_FEMALE4:
		case CREATURETYPE_VAMPIRE_MALE4:
		//add by viva
		case CREATURETYPE_VAMPIRE_MALE5:
		case CREATURETYPE_VAMPIRE_FEMALE5:
		case CREATURETYPE_VAMPIRE_MALE6:
		case CREATURETYPE_VAMPIRE_FEMALE6:
		// add end
			SetRegen( g_pClientConfig->REGEN_AMOUNT_VAMPIRE, 1000 );	// 2씩 1초마다			
		break;

		//---------------------------------------------------------------
		// 딴 경우..
		//---------------------------------------------------------------
		default :
			SetRegen( 0, 0 );	// regen 안한다.		
	}
}

//----------------------------------------------------------------------
// 나를 흡혈하고 있던 Creature의 흡혈동작을 멈춘다
//----------------------------------------------------------------------
void 
MCreature::StopDrain()
{
	if(m_Action == ACTION_DRAINED)
	{
		MCreature *pCreature = g_pZone->GetCreature(GetDrainCreatureID());
		if(pCreature != NULL)
		{
			pCreature->StopBloodDrain();
		}
	}
}

//----------------------------------------------------------------------
// 나를 흡영하고 있던 Creature의 흡영동작을 멈춘다
//----------------------------------------------------------------------
void 
MCreature::StopAbsorb()
{
	if(m_Action == ACTION_DRAINED)
	{
		MCreature *pCreature = g_pZone->GetCreature(GetAbsorbCreatureID());
		if(pCreature != NULL)
		{
			pCreature->StopAbsorb();
		}
	}
}

void
MCreature::AddHPModify(const int modify)
{
	m_HPModifyList.push_back(HPModify(modify, GetTickCount()));
	if(m_HPModifyList.size() > g_pClientConfig->HPModifyListMax)m_HPModifyList.pop_front(); 
}

void 
MCreature::SetTraceID(TYPE_OBJECTID id)
{
	m_TraceID = id;	
}

TYPE_ACTIONINFO		MCreature::GetBasicActionInfo() const
{ 	
	return m_nBasicActionInfo; 
}

int MCreature::GetHeadSkin() const

{
	return m_HeadSkin;
}


bool MCreature::UpdateBurningSol()
{
	if(m_bBurningSol)
	{
		if(IsDead())
		{
			StopBurningSol();
			SetStop();
			return false ;
		}
		else if(HasEffectStatus(EFFECTSTATUS_CURSE_PARALYSIS))
		{
			StopBurningSol();
			SetStop();
			RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1);
			RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2);
			RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_3);
			RemoveEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_4);
			return false;
		}
		int bCheckFrame = 0;
		if(!IsAdvancementClass())
			bCheckFrame = IsMale()?7:11;
		else
			bCheckFrame = 7;

		m_ActionCount = m_bBurningSolCount;

		m_bBurningSolCount++;
		if(	HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_1)||
			HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_2)||
			HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_3)||
			HasEffectStatus(EFFECTSTATUS_BURNING_SOL_CHARGE_4))
		{
			if(m_bBurningSolCount>=bCheckFrame)
				m_bBurningSolCount = bCheckFrame;
		}
		if(m_ActionCount>=GetActionCountMax())
			StopBurningSol();
		return true;
	#ifdef OUTPUT_DEBUG					
		DEBUG_ADD_FORMAT("[UpdateBurningSol] ActionCount : %d", m_bBurningSolCount);
	#endif
	}
	return false;
}
void MCreature::InstallTurretStopAttack()
{
	m_nUsedActionInfo = ACTIONINFO_NULL;
	if(m_Action == ACTION_ATTACK)
	{
		if(m_ActionCount<14)
			m_ActionCount = 54 - m_ActionCount;
		else if(m_ActionCount<20)
			m_ActionCount = 20;
		m_ActionCountMax = 54;
		PlaySound( SOUND_SLAYER_TURRET_FIRE_END );
		
//		if(m_ActionCount<28)
//			m_ActionCount = 75 - m_ActionCount;
//		else if(m_ActionCount<42)
//			m_ActionCount = 42;
//		m_ActionCountMax = 75;
	}
}
bool MCreature::UpDateInstallTurret()
{
	if(HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET))
	{
		if(IsAlive() && (m_Action == ACTION_STAND))
		{
			if (m_bInstallTurretDirect != m_Direction)
			{
				if( ((m_bInstallTurretDirect - m_Direction + 8)%8) < 4 )
				{
					m_bInstallTurretCount = ((m_bInstallTurretCount-1)+16)%16;
				}
				else
				{
					m_bInstallTurretCount = (m_bInstallTurretCount+1)%16;
				}

				if(!(m_bInstallTurretCount&0x01))
					m_bInstallTurretDirect = ((m_bInstallTurretCount /2) +2 )%8;

			}
			else
			{
				if(!(g_CurrentFrame%100))
					SetDirection((m_bInstallTurretDirect + 2+ rand()%5)%8);
			}
			return true;
		}
		else if( m_Action == ACTION_ATTACK)
		{
			if(m_ActionCount<20)
				m_ActionCount = 20;
	
			m_ActionCountMax = 54;
		}

//		
		SetInstallTurretDirect(m_CurrentDirection);
		SetInstallTurretCount(((m_CurrentDirection+6)%8)*2);

	}
	return false;

}

BYTE		MCreature::GetActionCountMax() const
{ 
	//if( IsAdvancementClass() )
	//	return GetCreatureActionCountMax(( this, GetAction() );
	return m_ActionCountMax; 
}
// 2005, 1, 5, sobeit add start
void		MCreature::UpdateBikeCrash()
{
	if(HasEffectStatus(EFFECTSTATUS_BIKE_CRASH))
	{
		int direction = (GetDirection() + 1)%8;
		int TempDir = GetInstallTurretDirect();
		
		if(GetAction() == ACTION_SLAYER_MOTOR_STAND)
		{
			if(direction == ((TempDir+3)%8))
			{
				ExecuteActionInfoFromMainNode(SKILL_CLIENT_BIKE_CRASH, GetX(), GetY(), 0, GetDirection(),	GetID(),	
					GetX(), GetY(), 0, 5000, NULL, false);	
			}
			SetDirection(direction);
			SetCurrentDirection(direction);
			if(direction == TempDir)
			{
				m_bInstallTurretCount ++;
				if(m_bInstallTurretCount == 3)
				{
					RemoveEffectStatus(EFFECTSTATUS_BIKE_CRASH);
					if(g_pPlayer->GetID() == GetID())
						g_pPlayer->SetDelay(0);
				}
			}
		}
//		else
//		{
//			if(direction < ((TempDir+3)%8))
//			{
//				SetDirection(direction);
//				SetCurrentDirection(direction);
//			}
//		}
	}
	
}
// 2005, 1, 5, sobeit add end	

void 
MCreature::RemoveCauseCriticalWoundsEffect()
{
	SetStop();
	StopCauseCriticalWounds();
	ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();

	bool re = false;
	while (iEffect != m_listEffect.end())
	{
		MAttachEffect* pEffect = *iEffect;
		TYPE_EFFECTSPRITETYPE Type = pEffect->GetEffectSpriteType();
		if(	
			EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_2 == Type 
			||EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_1 == Type 
			||EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_3 == Type 
			||EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_4 == Type 
			)
		{
			// 메모리 제거
			delete pEffect;

			ATTACHEFFECT_LIST::iterator dEffect = iEffect;
			iEffect--;

			// list에서 제거
			m_listEffect.erase( dEffect );

			m_bAttachEffect[Type] = false;
	
			re = true;
		}
		
		iEffect++;

	}
	iEffect = m_listGroundEffect.begin();

	while (iEffect != m_listGroundEffect.end())
	{
		MAttachEffect* pEffect = *iEffect;
		TYPE_EFFECTSPRITETYPE Type = pEffect->GetEffectSpriteType();
		if(	
			EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_2 == Type 
			||EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_1 == Type 
			||EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_3 == Type 
			||EFFECTSPRITETYPE_CAUSE_CRITICAL_WOUND_4 == Type 
			)
		{
			// 메모리 제거
			delete pEffect;

			ATTACHEFFECT_LIST::iterator dEffect = iEffect;
			iEffect--;

			// list에서 제거
			m_listGroundEffect.erase( dEffect );

			m_bAttachEffect[Type] = false;
	
			re = true;
		}
		
		iEffect++;

	}
	m_bEffectStatus[EFFECTSTATUS_CAUSE_CRITICAL_WOUNDS]  = false;

}

// 2005, 1, 27, sobeit add start- max action count;
int
MCreature::GetActionMax() const
{
	if(!IsAdvancementClass())
		 return (*g_pCreatureTable)[m_CreatureType].GetActionMax(); 

	// 승직 캐릭터 일때
	if(IsSlayer())
		return ACTION_ADVANCEMENT_SLAYER_MAX/* - ACTION_ADVANCEMENT_SLAYER_STOP_SWORD*/;
	else if(IsVampire())
		return ACTION_ADVANCEMENT_MAX/* - ACTION_ADVANCEMENT_STOP*/;
	else if(IsOusters())
		return ACTION_ADVANCEMENT_OUSTERS_MAX/* - ACTION_ADVANCEMENT_OUSTERS_CHAKRAM_STOP*/;

	return 0;
}

// 2005, 1, 27, sobeit add end

// 2005, 2, 22, sobeit add start
int 
MCreature::GetMasterEffectType(DWORD Status)
{
	TYPE_EFFECTSPRITETYPE type = (*g_pEffectStatusTable)[Status].EffectSpriteType;
	int MarketType = GetMasterEffectType();
	
	//if(MarketType>3) return 0;
	
	switch(Status)
	{
		case EFFECTSTATUS_GRAND_MASTER_SLAYER:
			{
				if(MarketType) 
					type =  EFFECTSPRITETYPE_MARKET_MASTER_SLAYER_100_FEAR + MarketType - 1;
			}
			break;
		case EFFECTSTATUS_GRAND_MASTER_VAMPIRE:
			{
				if(MarketType) 
					type =  EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_100_FEAR + MarketType - 1;
			}
			break;
		case EFFECTSTATUS_GRAND_MASTER_OUSTERS:
			{
				if(MarketType) 
					type =  EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_100_FEAR + MarketType - 1;
			}
			break;
			
		case EFFECTSTATUS_GRAND_MASTER_SLAYER_130:
			{
				RemoveEffectStatus(EFFECTSTATUS_GRAND_MASTER_SLAYER);
				if(IsDead()) return 0; //죽은넘한텐 이펙트 무시
				if(MarketType) 
					type =  EFFECTSPRITETYPE_MARKET_MASTER_SLAYER_130_FEAR + MarketType - 1;
			}
			break;
		case EFFECTSTATUS_GRAND_MASTER_SLAYER_150:
			{
				RemoveEffectStatus(EFFECTSTATUS_GRAND_MASTER_SLAYER_130);
				if(IsAdvancementClass())
				{
					switch(MarketType)
					{
					//case 0:
					case 1:
					case 2:
					case 3:
						type =  EFFECTSPRITETYPE_MARKET_MASTER_SLAYER_150_FEAR + MarketType - 1;
					    break;
					case 4:
						type =  EFFECTSPRITETYPE_MARKET_MASTER_SLAYER_150_ADVANCE_NEW_4;
						break;
					}
					// 승직 캐릭터는 Market Master Effect 적용 하지 말라고 해서 주석 처리 함..
					// 나중에 또 말 바꾸면 주석 풀어주면 됨.
//					if(MarketType) 
//						type =  EFFECTSPRITETYPE_MARKET_MASTER_SLAYER_150_ADVANCE_FEAR + MarketType - 1;
//					else
					// edit By Sonic 2006.10.28 혼뇜랗瘻빈실�ド爵溝캣須寫㏏�

					//	type = EFFECTSPRITETYPE_ADVANCEMENT_MASTER_SLAYER;
// 					if(MarketType)
// 						type =  EFFECTSPRITETYPE_MARKET_MASTER_SLAYER_150_FEAR + MarketType - 1;
					// end  By Sonic 2006.10.28
				}
				else
				{
					if(MarketType) 
						type =  EFFECTSPRITETYPE_MARKET_MASTER_SLAYER_150_FEAR + MarketType - 1;
				}
			}
			break;
		case EFFECTSTATUS_GRAND_MASTER_VAMPIRE_130:
			{
				RemoveEffectStatus(EFFECTSTATUS_GRAND_MASTER_VAMPIRE);
				if(MarketType) 
					type =  EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_130_FEAR + MarketType - 1;
			}
			break;
		case EFFECTSTATUS_GRAND_MASTER_VAMPIRE_150:
			{
				RemoveEffectStatus(EFFECTSTATUS_GRAND_MASTER_VAMPIRE_130);
				if(IsAdvancementClass())
				{
					switch(MarketType)
					{
					//case 0:
					case 1:
					case 2:
					case 3:
						type =  EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_150_FRONT_FEAR + MarketType - 1;
					    break;
					case 4:
						type =  EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_150_ADVANCE_NEW_4;
						break;	
					}
					// 승직 캐릭터는 Market Master Effect 적용 하지 말라고 해서 주석 처리 함..
					// 나중에 또 말 바꾸면 주석 풀어주면 됨.
//					if(MarketType) 
//						type =  EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_150_FRONT_ADVANCE_FEAR + MarketType - 1;
//					else
						// edit by sonic 2006.10.28   혼뇜랗瘻빈였꼇돕�爵矩璪狼バ㏏�
							//type =  EFFECTSPRITETYPE_ADVANCEMENT_MASTER_VAMPIRE_BACK;
// 							if(MarketType)
// 								type =  EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_150_FRONT_FEAR + MarketType - 1;
						// end by sonic 2006.10.28
						
				}
				else
				{
					if(MarketType) 
						type =  EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_150_FRONT_FEAR + MarketType - 1;
				}
			}
			break;
		case EFFECTSTATUS_GRAND_MASTER_OUSTERS_130:
			{
				RemoveEffectStatus(EFFECTSTATUS_GRAND_MASTER_OUSTERS);
				if(MarketType) 
					type =  EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_130_FEAR + MarketType - 1;
			}
			break;
		case EFFECTSTATUS_GRAND_MASTER_OUSTERS_150:
			{
				RemoveEffectStatus(EFFECTSTATUS_GRAND_MASTER_OUSTERS_130);
	
				if(IsAdvancementClass())
				{
					switch(MarketType)
					{
					//case 0:
					case 1:
					case 2:
					case 3:
						type =  EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_150_FEAR + MarketType - 1;
					    break;
					case 4:
						type =  EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_150_ADVANCE_NEW_4;
						break;	
					}	
					// 승직 캐릭터는 Market Master Effect 적용 하지 말라고 해서 주석 처리 함..
					// 나중에 또 말 바꾸면 주석 풀어주면 됨.
//					if(MarketType) 
//						type =  EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_150_ADVANCE_FEAR + MarketType - 1;
//					else
					// edit by sonic 2006.10.28  錦攣랗瘻빈실�セ璪狼ノ奸�
						//type =  EFFECTSPRITETYPE_ADVANCEMENT_MASTER_OUSTER;
// 					if(MarketType)
// 					type =  EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_150_FEAR + MarketType - 1;

					// end by sonic 2006.10.28
				}
				else
				{
					if(MarketType) 
						type =  EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_150_FEAR + MarketType - 1;
				}

			}
			break;
	}
	
	return type;
}

void 
MCreature::ChangeMasterEffectType(int MarketEffect)
{
	int status = 0;

	if(IsSlayer())
	{
		if(HasEffectStatus(EFFECTSTATUS_GRAND_MASTER_SLAYER))		status = EFFECTSTATUS_GRAND_MASTER_SLAYER;
		else if(HasEffectStatus(EFFECTSTATUS_GRAND_MASTER_SLAYER_130))	status = EFFECTSTATUS_GRAND_MASTER_SLAYER_130;
		else if(HasEffectStatus(EFFECTSTATUS_GRAND_MASTER_SLAYER_150))	status = EFFECTSTATUS_GRAND_MASTER_SLAYER_150;
	}
	else if(IsVampire())
	{
		if(HasEffectStatus(EFFECTSTATUS_GRAND_MASTER_VAMPIRE))		status = EFFECTSTATUS_GRAND_MASTER_VAMPIRE;
		else if(HasEffectStatus(EFFECTSTATUS_GRAND_MASTER_VAMPIRE_130))	status = EFFECTSTATUS_GRAND_MASTER_VAMPIRE_130;
		else if(HasEffectStatus(EFFECTSTATUS_GRAND_MASTER_VAMPIRE_150))	status = EFFECTSTATUS_GRAND_MASTER_VAMPIRE_150;
	}
	else if(IsOusters())
	{
		if(HasEffectStatus(EFFECTSTATUS_GRAND_MASTER_OUSTERS))		status = EFFECTSTATUS_GRAND_MASTER_OUSTERS;
		else if(HasEffectStatus(EFFECTSTATUS_GRAND_MASTER_OUSTERS_130))	status = EFFECTSTATUS_GRAND_MASTER_OUSTERS_130;
		else if(HasEffectStatus(EFFECTSTATUS_GRAND_MASTER_OUSTERS_150))	status = EFFECTSTATUS_GRAND_MASTER_OUSTERS_150;
	}

	if( 0 == status )
		return;
		
	RemoveEffectStatus((EFFECTSTATUS)status);

	SetMasterEffectType(MarketEffect);

	AddEffectStatus((EFFECTSTATUS)status, 0xffff);
	
		
//	ATTACHEFFECT_LIST::iterator iEffect = m_listEffect.begin();
//
//	bool re = false;
//	while (iEffect != m_listEffect.end())
//	{
//		MAttachEffect* pEffect = *iEffect;
//		TYPE_EFFECTSPRITETYPE Type = pEffect->GetEffectSpriteType();
//		if(	
//			Type >= EFFECTSPRITETYPE_GRAND_MASTER_ALPHA_VAMPIRE_100_NEW && Type <= EFFECTSPRITETYPE_GRAND_MASTER_ALPHA_OUSTERS_150_NEW
//			|| Type >= EFFECTSPRITETYPE_ADVANCEMENT_MASTER_VAMPIRE_BACK && Type <= EFFECTSPRITETYPE_ADVANCEMENT_MASTER_OUSTER
//			|| Type >= EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_100_FEAR && Type <= EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_150_ADVANCE_HOPE
//			)
//		{
//			// 메모리 제거
//			delete pEffect;
//
//			ATTACHEFFECT_LIST::iterator dEffect = iEffect;
//			iEffect--;
//
//			// list에서 제거
//			m_listEffect.erase( dEffect );
//
//			m_bAttachEffect[Type] = false;
//	
//			re = true;
//		}
//		
//		iEffect++;
//
//	}
//	iEffect = m_listGroundEffect.begin();
//
//	while (iEffect != m_listGroundEffect.end())
//	{
//		MAttachEffect* pEffect = *iEffect;
//		TYPE_EFFECTSPRITETYPE Type = pEffect->GetEffectSpriteType();
//		if(	
//			Type >= EFFECTSPRITETYPE_GRAND_MASTER_ALPHA_VAMPIRE_100_NEW && Type <= EFFECTSPRITETYPE_GRAND_MASTER_ALPHA_OUSTERS_150_NEW
//			|| Type >= EFFECTSPRITETYPE_ADVANCEMENT_MASTER_VAMPIRE_BACK && Type <= EFFECTSPRITETYPE_ADVANCEMENT_MASTER_OUSTER
//			|| Type >= EFFECTSPRITETYPE_MARKET_MASTER_VAMPIRE_100_FEAR && Type <= EFFECTSPRITETYPE_MARKET_MASTER_OUSTERS_150_ADVANCE_HOPE
//			)
//		{
//			// 메모리 제거
//			delete pEffect;
//
//			ATTACHEFFECT_LIST::iterator dEffect = iEffect;
//			iEffect--;
//
//			// list에서 제거
//			m_listGroundEffect.erase( dEffect );
//
//			m_bAttachEffect[Type] = false;
//	
//			re = true;
//		}
//		
//		iEffect++;
//
//	}
//	m_bEffectStatus[Status]  = false;

}
// 2005, 2, 22, sobeit add end