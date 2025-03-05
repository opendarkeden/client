//----------------------------------------------------------------------
// MHelicopterManager.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MHelicopterManager.h"
#include "MChaseEffect.h"
#include "MZone.h"
#include "MTopView.h"
#include "MPlayer.h"
#include "EffectSpriteTypeDef.h"
#include "SoundDef.h"
#include "MZoneSoundManager.h"
#include "MEffectSpriteTypeTable.h"
#include "UIFunction.h"

//----------------------------------------------------------------------
// define
//----------------------------------------------------------------------
#define PIXEL_HELICOPTER_MOVE		20

//----------------------------------------------------------------------
//
//						MHelicopter
//
//----------------------------------------------------------------------
MHelicopter::MHelicopter()
{
	m_CreatureID = OBJECTID_NULL;
	m_EffectID = OBJECTID_NULL;
	m_pSound = NULL;
}

MHelicopter::~MHelicopter()
{
	Release();
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void
MHelicopter::Release()
{
	if (g_pZone!=NULL)
	{
		g_pZone->RemoveEffect( m_EffectID );
	}

	if (m_pSound!=NULL)
	{
		delete m_pSound;
		m_pSound = NULL;
	}

	m_CreatureID = OBJECTID_NULL;
	m_EffectID = OBJECTID_NULL;
}

//----------------------------------------------------------------------
// Set ChaseCreature
//----------------------------------------------------------------------
// 하나의 creature를 계속 추적하는 Effect를 생성한다.
//----------------------------------------------------------------------
bool			
MHelicopter::SetChaseCreature(TYPE_OBJECTID id, int x0, int y0)
{
	// player인 경우 따로 체크하는게 나을수도 있겠는데..
	MCreature* pCreature = g_pZone->GetCreature( id );

	if (pCreature!=NULL)
	{
		m_CreatureID = id;

		//-----------------------------------------------------
		// 헬기 소리 출력
		//-----------------------------------------------------
		if (m_pSound!=NULL)
		{
			delete m_pSound;
		}
		m_pSound = new ZONESOUND_NODE(SOUND_WORLD_PROPELLER);
		m_pSound->Play( x0, y0, true );	// loop
		
		//-----------------------------------------------------
		// Creture의 좌표로 목표좌표를 설정한다.
		//-----------------------------------------------------
		int cx, cy, cz;
		cx = g_pTopView->MapToPixelX( pCreature->GetX() );
		cy = g_pTopView->MapToPixelY( pCreature->GetY() );
		cz = 0;//pCreature->GetZ();

		// test코드.. 위에서 아래로 사바사바 내려오기..
		int effectSpriteType = EFFECTSPRITETYPE_SUMMON_HELICOPTER;
		int direction = 2;
		int step = PIXEL_HELICOPTER_MOVE;
		int x0	= cx + 500;
		int	y0	= cy - 300;
		int z0	= cz;

		BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[effectSpriteType].BltType;
		TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[effectSpriteType].FrameID;		
		

		MChaseEffect* pEffect = new MChaseEffect(bltType);	

		// Effect ID를 저장해둔다.
		m_EffectID = pEffect->GetID();
		
		int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);
		
		pEffect->SetFrameID( frameID, maxFrame );		// 0번 Effect, Max 3 Frame					

		// 발사 위치 Pixel좌표	
		pEffect->SetPixelPosition( x0, y0, z0 );

		pEffect->SetDirection( direction );
		
		// 목표 Creature
		pEffect->SetTraceCreatureID( id );
		pEffect->SetStepPixel( step );
		

		// 지속되는 Frame (목표가 있다면 별로 관계 없음 - -;)
		//pEffect->SetCount( egInfo.count, egInfo.linkCount );

		// 위력
		//pEffect->SetPower(egInfo.power);

		// 빛의 밝기
		//pEffect->SetLight( light );

		g_pZone->AddEffect( pEffect );

		return true;
	}

	DEBUG_ADD_FORMAT("MHelicopter::SetChase-No Such Creature id=%d", id);

	Release();

	return false;
}

//----------------------------------------------------------------------
// Update Sound
//----------------------------------------------------------------------
void			
MHelicopter::UpdateSound(int x, int y)
{
	if (m_pSound!=NULL)
	{
		m_pSound->Play( x, y, true );
	}
}

//----------------------------------------------------------------------
//
//						MHelicopterManager
//
//----------------------------------------------------------------------
MHelicopterManager::MHelicopterManager()
{
}

MHelicopterManager::~MHelicopterManager()
{
	Release();
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void
MHelicopterManager::Release()
{
	CTypeMap2<MHelicopter>::Release();
}

//----------------------------------------------------------------------
// Update
//----------------------------------------------------------------------
void
MHelicopterManager::Update()
{
	iterator iHelicopter = begin();

	while (iHelicopter != end())
	{
		MHelicopter* pHelicopter = iHelicopter->second;

		bool bDeleteHelicopter = false;
	
		//-----------------------------------------------------
		// 헬기와 관련된 Effect 찾기
		//-----------------------------------------------------
		MEffect* pEffect = g_pZone->GetEffect( pHelicopter->GetEffectID() );

		if (pEffect==NULL)
		{
			bDeleteHelicopter = true;			
		}
		else
		{
			if (pEffect->GetEffectType()==MEffect::EFFECT_CHASE)
			{
				MChaseEffect *pChaseEffect = (MChaseEffect*)pEffect;

				//-----------------------------------------------------
				// 소리 나는 위치를 수정한다.
				//-----------------------------------------------------
				pHelicopter->UpdateSound( pEffect->GetX(), pEffect->GetY() );

				//-----------------------------------------------------
				// Player가 부른 헬기인 경우
				//-----------------------------------------------------
				if (pChaseEffect->GetTraceCreatureID()==g_pPlayer->GetID())
				{
					if (pChaseEffect->IsChaseOver())
					{
						// WayPoint 선택하는 UI를 띄워야 한다.
						if (!UI_IsRunningSelectWayPoint())
						{
							UI_RunSelectWayPoint();							
						}
					}
				}

				//-----------------------------------------------------
				// 완전히 화면 밖으로 사라진 경우
				//-----------------------------------------------------
				else if (pChaseEffect->GetTraceCreatureID()==OBJECTID_NULL
					&& pChaseEffect->IsChaseOver())
				{
					bDeleteHelicopter = true;					
				}		
				
			}
		}

		//---------------------------------------------
		// 이번에꺼 지울까 말까?
		//---------------------------------------------
		if (bDeleteHelicopter)
		{
			delete pHelicopter;

			// 임시로 저장해두고 지운다.
			iterator iTemp = iHelicopter;
			iHelicopter ++;

			erase( iTemp );
		}
		else
		{
			iHelicopter ++;
		}
	}
}

//----------------------------------------------------------------------
// Add Helicopter
//----------------------------------------------------------------------
bool
MHelicopterManager::AddHelicopter(MHelicopter* pHelicopter)
{
	TYPE_OBJECTID creatureID = pHelicopter->GetCreatureID();

	iterator iHelicopter = find( creatureID );

	// 이미 있다면 기존에걸 지워버린다.
	if (iHelicopter != end())
	{
		delete iHelicopter->second;

		erase( iHelicopter );
	}

	insert(TYPE_MAP::value_type(creatureID, pHelicopter));

	return true;
}


//----------------------------------------------------------------------
// Get Helicopter
//----------------------------------------------------------------------
MHelicopter*
MHelicopterManager::GetHelicopter(TYPE_OBJECTID creatureID) const
{
	const_iterator iHelicopter = find( creatureID );

	if (iHelicopter == end())
	{
		return NULL;
	}
	
	return iHelicopter->second;
}

//----------------------------------------------------------------------
// Remove Helicopter Soon
//----------------------------------------------------------------------
void
MHelicopterManager::RemoveHelicopterSoon(TYPE_OBJECTID creatureID)
{
	iterator iHelicopter = find( creatureID );

	if (iHelicopter != end())
	{
		delete iHelicopter->second;

		erase( iHelicopter );
	}	
}

//----------------------------------------------------------------------
// Remove Helicopter
//----------------------------------------------------------------------
// 화면에서 사라져가는 모습을 보여준다.
//----------------------------------------------------------------------
void
MHelicopterManager::RemoveHelicopter(TYPE_OBJECTID creatureID)
{
	DEBUG_ADD_FORMAT("RemoveHelicopter. cid=%d", creatureID);
	iterator iHelicopter = find( creatureID );

	if (iHelicopter != end())
	{
		MHelicopter* pHelicopter = iHelicopter->second;

		MEffect* pEffect = g_pZone->GetEffect( pHelicopter->GetEffectID() );

		if (pEffect==NULL)
		{
			//-----------------------------------------------------
			// Effect가 없는 경우.. (캐릭터가 사라졌거나.. 뭐 그런거다)
			//-----------------------------------------------------
			delete pHelicopter;

			erase( iHelicopter );
		}
		else
		{
			if (pEffect->GetEffectType()==MEffect::EFFECT_CHASE)
			{				
				MChaseEffect *pChaseEffect = (MChaseEffect*)pEffect;

				if (pChaseEffect->GetTraceCreatureID()!=OBJECTID_NULL)
				{
					// 더 이상 따라가지는 않는다.
					pChaseEffect->SetTraceCreatureID( OBJECTID_NULL );

					int x = pEffect->GetPixelX() - 1000;
					int y = pEffect->GetPixelY() + 600;
					pChaseEffect->SetTarget(x, y, 0, PIXEL_HELICOPTER_MOVE);
				}
			}
		}
	}	
}