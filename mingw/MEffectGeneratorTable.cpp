//----------------------------------------------------------------------
// MEffectGeneratorTable.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MEffectGeneratorTable.h"
#include "MAttackCreatureEffectGenerator.h"
#include "MAttackZoneEffectGenerator.h"
#include "MAttackZoneParabolaEffectGenerator.h"
#include "MStopZoneEffectGenerator.h"
#include "MFirePiecingEffectGenerator.h"
#include "MRisingEffectGenerator.h"
#include "MFallingEffectGenerator.h"
#include "MAttackCreatureParabolaEffectGenerator.h"
#include "MAttachCreatureEffectGenerator.h"
#include "MAttachCreatureEffectGroundGenerator.h"
#include "MStopZoneCrossEffectGenerator.h"
#include "MStopZoneXEffectGenerator.h"
#include "MStopZoneRectEffectGenerator.h"
#include "MRippleZoneEffectGenerator.h"
#include "MRippleZoneWideEffectGenerator.h"
#include "MStopZoneRhombusEffectGenerator.h"
#include "MStopInventoryEffectGenerator.h"
#include "MMultipleFallingEffectGenerator.h"
#include "MStopZoneMultipleEffectGenerator.h"
#include "MAttachZoneEffectGenerator.h"
#include "MStopZoneRandomEffectGenerator.h"
#include "MStopZoneWallEffectGenerator.h"
#include "MRippleZonePixelEffectGenerator.h"
#include "MStopZoneSelectableEffectGenerator.h"
#include "MAttachZoneSelectableEffectGenerator.h"
#include "MSpreadOutEffectGenerator.h"
#include "MBloodyWallEffectGenerator.h"
#include "MBloodyWaveEffectGenerator.h"
#include "MAttackCreatureHomingEffectGenerator.h"
#include "MAroundZoneEffectGenerator.h"
#include "MStopZoneEmptyCrossEffectGenerator.h"
#include "MStopZoneEmptyRectEffectGenerator.h"
#include "MStopZoneEmptyHorizontalWallEffectGenerator.h"
#include "MStopZoneEmptyVerticalEffectGenerator.h"
#include "MAttackZoneBombEffectGenerator.h"
#include "MAttachCreatureOrbitEffectGenerator.h"
#include "MMeteorDropEffectGenerator.h"
#include "MBloodyBreakerEffectGenerator.h"
#include "MAttachZoneAroundEffectGenerator.h"
#include "MAttackZoneRectEffectGenerator.h"
#include "MFollowPathEffectGenerator.h"

#include "MEffect.h"
#include "MSoundTable.h"
#include "MEffectSpriteTypeTable.h"

#include "MTopView.h"
#include "ClientDef.h"

#include "DebugInfo.h"
#include "SkillDef.h"
#include "EffectSpriteTypeDef.h"
#include "MSkipEffectGenerator.h"
#include "MPlayingWithFireEffectGenerator.h"
//#define	new			DEBUG_NEW

#ifdef OUTPUT_DEBUG
	#define OUTPUT_DEBUG_EFFECT_GENERATOR
#endif


//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
MEffectGeneratorTable*	g_pEffectGeneratorTable = NULL;

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
MEffectGeneratorTable::MEffectGeneratorTable()
{
	for (int i=0; i<MAX_EFFECTGENERATORID; i++)
	{
		m_pEffectGenerator[i] = NULL;
	}	
}

MEffectGeneratorTable::~MEffectGeneratorTable()
{
	Release();
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Init - 한 번만 불려야 한다.
//----------------------------------------------------------------------
void
MEffectGeneratorTable::Init()
{
	Release();

	AddEffectGenerator( new MAttackCreatureEffectGenerator );
	AddEffectGenerator( new MAttackZoneEffectGenerator );	
	AddEffectGenerator( new MAttackZoneParabolaEffectGenerator );	
	AddEffectGenerator( new MStopZoneEffectGenerator );
	AddEffectGenerator( new MFirePiecingEffectGenerator );
	AddEffectGenerator( new MRisingEffectGenerator );
	AddEffectGenerator( new MFallingEffectGenerator );
	AddEffectGenerator( new MAttackCreatureParabolaEffectGenerator );
	AddEffectGenerator( new MAttachCreatureEffectGenerator );
	AddEffectGenerator( new MAttachCreatureEffectGroundGenerator );
	AddEffectGenerator( new MStopZoneCrossEffectGenerator );
	AddEffectGenerator( new MStopZoneXEffectGenerator );
	AddEffectGenerator( new MStopZoneRectEffectGenerator );
	AddEffectGenerator( new MRippleZoneEffectGenerator );
	AddEffectGenerator( new MRippleZoneWideEffectGenerator );
	AddEffectGenerator( new MStopZoneRhombusEffectGenerator );
	AddEffectGenerator( new MStopInventoryEffectGenerator );
	AddEffectGenerator( new MMultipleFallingEffectGenerator );
	AddEffectGenerator( new MStopZoneMultipleEffectGenerator );
	AddEffectGenerator( new MAttachZoneEffectGenerator );
	AddEffectGenerator( new MStopZoneRandomEffectGenerator );
	AddEffectGenerator( new MStopZoneWallEffectGenerator );
	AddEffectGenerator( new MRippleZonePixelEffectGenerator );
	AddEffectGenerator( new MStopZoneSelectableEffectGenerator );
	AddEffectGenerator( new MAttachZoneSelectableEffectGenerator );
	AddEffectGenerator( new MSpreadOutEffectGenerator );
	AddEffectGenerator( new MBloodyWallEffectGenerator );
	AddEffectGenerator( new MBloodyWaveEffectGenerator );
	AddEffectGenerator( new MAttackCreatureHomingEffectGenerator );
	AddEffectGenerator( new MAroundZoneEffectGenerator );
	AddEffectGenerator( new MStopZoneEmptyCrossEffectGenerator );
	AddEffectGenerator( new MStopZoneEmptyRectEffectGenerator );
	AddEffectGenerator( new MStopZoneEmptyHorizontalWallEffectGenerator );
	AddEffectGenerator( new MStopZoneEmptyVerticalWallEffectGenerator );
	AddEffectGenerator( new MAttackZoneBombEffectGenerator );
	AddEffectGenerator( new MAttachCreatureOrbitEffectGenerator );	
	AddEffectGenerator( new MMeteorDropEffectGenerator );	
	AddEffectGenerator( new MBloodyBreakerEffectGenerator );
	AddEffectGenerator( new MAttachZoneAroundEffectGenerator );
	AddEffectGenerator( new MAttackZoneRectEffectGenerator );
	AddEffectGenerator( new MFollowPathEffectGenerator );
	AddEffectGenerator( new MSkipEffectGenerator );
	AddEffectGenerator( new MPlayingWithFireEffectGenerator );
}	

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void
MEffectGeneratorTable::Release()
{
	for (int i=0; i<MAX_EFFECTGENERATORID; i++)
	{
		if (m_pEffectGenerator[i] != NULL)
		{
			//#ifdef DEBUG_NEW
			//	operator delete (m_pEffectGenerator[i], __FILE__, __LINE__);
			//#else
				delete m_pEffectGenerator[i];
			//#endif

		}
	}	
}

//----------------------------------------------------------------------
// m_pEffectGenerator에 MEffectGenerator*를 추가한다.
//----------------------------------------------------------------------
void
MEffectGeneratorTable::AddEffectGenerator(MEffectGenerator* pEffectGenerator)
{
	// 없는 경우에 추가하고..
	if (m_pEffectGenerator[ pEffectGenerator->GetID() ] == NULL)
	{
		m_pEffectGenerator[ pEffectGenerator->GetID() ] = pEffectGenerator;
	}
	// 이미 다른게 있다면... 추가하지 않는다.
	else
	{
		delete pEffectGenerator;
	}
}


//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
// 최초 Effect생성 
//----------------------------------------------------------------------
void
MEffectGeneratorTable::Generate(	
	int x0, int y0, int z0,
	BYTE direction,	
	BYTE power,
	TYPE_ACTIONINFO nActionInfo,
	MEffectTarget* pEffectTarget,
	BYTE temp1, BYTE temp2)
{	
	#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
		DEBUG_ADD_FORMAT("EGT-Generate. ai=%d, target=0x%X", nActionInfo, pEffectTarget);
	#endif

	if (nActionInfo >= g_pActionInfoTable->GetSize())
	{
		#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
			DEBUG_ADD_FORMAT("Exceed Max ActionInfo(%d/%d)", nActionInfo, g_pActionInfoTable->GetSize());
		#endif

		return;
	}
	
	//------------------------------------------------------------
	// EffectTarget이 없거나..
	// 정보의 개수가 다르면...
	// 뭔가(?) 잘못됐다는 얘기다..	
	//------------------------------------------------------------
	if (pEffectTarget==NULL)
	{
		#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
			DEBUG_ADD_FORMAT("EGT-Generate OK0. ai=%d, target=0x%X", nActionInfo, pEffectTarget);
		#endif

		return;
	}
	else if ((*g_pActionInfoTable)[ nActionInfo ].GetSize()!=pEffectTarget->GetMaxPhase())
	{
		// 목표 완료시킨다.
		//while (!pEffectTarget->IsEnd())
		//{
		//	pEffectTarget->NextPhase();
		//}

		//------------------------------------------------------------
		// 목표가 없는 Target이다.. 지운다.
		//------------------------------------------------------------
		if (pEffectTarget->IsExistResult())
		{
			pEffectTarget->GetResult()->Release();			
		}
		delete pEffectTarget;
		pEffectTarget = NULL;

		#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
			DEBUG_ADD_FORMAT("EGT-Generate OK1. ai=%d, target=0x%X", nActionInfo, pEffectTarget);
		#endif

		return;
	}

	#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
		DEBUG_ADD_FORMAT("et=0x%X, targetID=%d, targetPhase=%d", pEffectTarget, (int)pEffectTarget->GetEffectID(), pEffectTarget->GetMaxPhase());
	#endif
	
	//------------------------------------------------------------
	// Effect생성 정보가 없으면...
	// 생성될 Effect가 없으니.. 결과를 바로 처리하면 된다.
	//------------------------------------------------------------
	bool bGenerated = false;
	if ((*g_pActionInfoTable)[ nActionInfo ].GetSize()==0)
	{
		// 결과 바로 처리
		pEffectTarget->SetResultTime();
	}	
	else
	{
		//------------------------------------------------------------
		// 현재 EFFECT_INFO_NODE 
		//------------------------------------------------------------
		// 대체로 0이지만.. main node부터 시작하는 경우도 있다.
		// 그러므로 --> pEffectTarget->GetCurrentPhase()
		//------------------------------------------------------------
		ACTION_INFO_NODE&			info	= (*g_pActionInfoTable)[ nActionInfo ][pEffectTarget->GetCurrentPhase()];
		TYPE_EFFECTSPRITETYPE		EffectSpriteType = info.EffectSpriteType;
		
		if(nActionInfo == MAGIC_ACID_TOUCH || nActionInfo == MAGIC_BLOODY_NAIL )
		{
			if(power == 2)
			{
				if( nActionInfo == MAGIC_ACID_TOUCH )
				{
					EffectSpriteType = EFFECTSPRITETYPE_ACID_TOUCH;
				}
					else
				{
					EffectSpriteType = EFFECTSPRITETYPE_BLOODY_NAIL;
				}				
			}
		}

		//------------------------------------------------------------
		// 첫번째 목표
		//------------------------------------------------------------
		int targetX = pEffectTarget->GetX();
		int targetY = pEffectTarget->GetY();
		int targetZ = pEffectTarget->GetZ();
		int targetID = pEffectTarget->GetID();
		
		
		DWORD count;
		
		//------------------------------------------------------------
		// Effect의 이번 node의 지속 시간 결정
		//------------------------------------------------------------
		// bDelayNode이고 설정된 delay가 있는 경우
		//		//main node이고 pEffectTarget의 DelayFrame이 0이 아닌 경우
		//------------------------------------------------------------
		if (info.bDelayNode
			//(*g_pActionInfoTable)[ nActionInfo ].HasMainNode()
			//&& pEffectTarget->GetCurrentPhase()==(*g_pActionInfoTable)[ nActionInfo ].GetMainNode()
			&& pEffectTarget->GetDelayFrame()!=0)
		{
			count = pEffectTarget->GetDelayFrame();
		}
		else
		{
			count = info.Count;
		}


		//------------------------------------------------------------
		// 다음 목표
		//------------------------------------------------------------
		pEffectTarget->NextPhase();

		
		//------------------------------------------------------------
		// EffectGenerator가 생성되지 않은 경우
		//------------------------------------------------------------
		if (info.EffectGeneratorID >= MAX_EFFECTGENERATORID
			|| m_pEffectGenerator[ info.EffectGeneratorID ]==NULL)
		{
			// 목표 완료시킨다.
			//while (!pEffectTarget->IsEnd())
			//{
				//pEffectTarget->NextPhase();
			//}
			//------------------------------------------------------------
			// 목표가 없는 Target이다.. 지운다.
			//------------------------------------------------------------
			if (pEffectTarget->IsExistResult())
			{
				pEffectTarget->GetResult()->Release();				
			}
			delete pEffectTarget;
			pEffectTarget = NULL;

			#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
				DEBUG_ADD_FORMAT("[Error] EGT-Generate id=%d, ai=%d, target=0x%X", info.EffectGeneratorID, nActionInfo, pEffectTarget);
			#endif	
			
			return;
		}

		//------------------------------------------------------------
		//
		// EffectGenerator에서 Effect를 생성하게 한다.
		//
		//------------------------------------------------------------
		EFFECTGENERATOR_INFO egInfo;

		egInfo.nActionInfo			= nActionInfo;
		egInfo.pEffectTarget		= pEffectTarget->IsEnd()? NULL : pEffectTarget;
		egInfo.effectSpriteType		= EffectSpriteType;//info.EffectSpriteType;
		egInfo.x0					= x0;
		egInfo.y0					= y0;
		egInfo.z0					= z0;
		egInfo.x1					= targetX;
		egInfo.y1					= targetY;
		egInfo.z1					= targetZ;
		egInfo.creatureID			= targetID;
		egInfo.direction			= direction;
		egInfo.step					= info.Step;
		egInfo.count				= count;
		egInfo.linkCount			= info.LinkCount;
		egInfo.power				= power;
		egInfo.pPreviousEffect		= NULL;
		egInfo.temp1				= temp1;
		egInfo.temp2				= temp2;

		#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
			DEBUG_ADD_FORMAT("et=0x%X, targetID=%d, targetPhase2=%d", pEffectTarget, (int)pEffectTarget->GetEffectID(), pEffectTarget->GetMaxPhase());
		#endif

		//------------------------------------------------------------
		// Generate의 return값은 egInfo.pEffectTarget이 
		// 내부에서 다른 effect로 link되었는가를 의미한다.
		//------------------------------------------------------------
		#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
			DEBUG_ADD_FORMAT("Generate before. id=%d", info.EffectGeneratorID);
		#endif

		if (m_pEffectGenerator[ info.EffectGeneratorID ]->Generate( egInfo ))
		{
			#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
				DEBUG_ADD("Generate true");
			#endif

			//------------------------------------------------------------
			// 기술의 단계에 맞는 sound를 출력해준다.
			//------------------------------------------------------------
			if (info.SoundID != SOUNDID_NULL)
			{
				//g_Sound.Play( g_SoundTable[info.SoundID].pDSBuffer );
				POINT point = MTopView::PixelToMap(x0, y0);
				PlaySound( info.SoundID, false, point.x, point.y );
			}

			//------------------------------------------------------------
			// 결과를 처리해야하는 시점인가? 
			//------------------------------------------------------------
			if (info.bResultTime)
			{
				// 결과 처리 시간이 지났다고 체크한다.
				// 나중에 결과가 생기면 바로 처리하게 된다.
				pEffectTarget->SetResultTime();
			}

			#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
				DEBUG_ADD_FORMAT("et=0x%X, targetID=%d, targetPhase3=%d", pEffectTarget, (int)pEffectTarget->GetEffectID(), pEffectTarget->GetMaxPhase());
			#endif

			bGenerated = true;
		}
		else
		{
			#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
				DEBUG_ADD("Generate false");
			#endif

			pEffectTarget->SetResultTime();
		}
	}

	//------------------------------------------------------------
	// 결과 처리
	//------------------------------------------------------------
	if (pEffectTarget->IsResultTime() || pEffectTarget->IsEnd())
	{	
		#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
			DEBUG_ADD("Check Result");
		#endif

		// 결과가 있으면 처리해야 한다.
		if (pEffectTarget->IsExistResult())
		{
			MActionResult* pResult = pEffectTarget->GetResult();
			pEffectTarget->SetResultNULL();

			#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
				DEBUG_ADD_FORMAT("ExecuteResult. et=0x%X, targetID=%d, targetPhase4=%d", pEffectTarget, (int)pEffectTarget->GetEffectID(), pEffectTarget->GetMaxPhase());
			#endif

			// 결과 실행
			pResult->Execute();
			
			// 메모리 제거
			delete pResult;			
		}

		#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
			else
			{
				DEBUG_ADD("NO Result");
			}
		#endif
		
		//------------------------------
		// [ TEST CODE ]
		//------------------------------
		// Player의 진행중이던 EffectTarget은 끝났다고 설정한다.
		// 왜 하필 Player인가?
		// 다른 캐릭터도 이 함수(Generate)를 쓴다.
		g_pPlayer->RemoveEffectTarget( pEffectTarget->GetEffectID() );

		//if (!bGenerated)
		//{
		//	delete pEffectTarget;
		//}
	}
	
	//------------------------------------------------------------
	// 더 이상 목표가 필요없는 마지막 단계인 경우..
	// effect가 사라질때 지워지게 한다.
	//------------------------------------------------------------
	if (//pEffectTarget->IsEnd() && 
		!bGenerated || pEffectTarget->IsEnd() )
	{
		#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
			DEBUG_ADD("delete EffectTarget");
		#endif

			if (pEffectTarget->IsExistResult())
			{
				MActionResult* pResult = pEffectTarget->GetResult();
				pEffectTarget->SetResultNULL();
				
#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
				DEBUG_ADD_FORMAT("ExecuteResult. et=0x%X, targetID=%d, targetPhase4=%d", pEffectTarget, (int)pEffectTarget->GetEffectID(), pEffectTarget->GetMaxPhase());
#endif
				
				// 결과 실행
				pResult->Execute();
				
				// 메모리 제거
				delete pResult;			
			}

		delete pEffectTarget;	
		pEffectTarget = NULL;
	}

	#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
		DEBUG_ADD_FORMAT("EGT-Generate OK. ai=%d, target=0x%X", nActionInfo, pEffectTarget);
	#endif
}

//----------------------------------------------------------------------
// GenerateNext
//----------------------------------------------------------------------
// 연결된 다음 Effect 생성
//----------------------------------------------------------------------
void	 
MEffectGeneratorTable::GenerateNext( MEffect* pEffect )
{
	#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
		DEBUG_ADD_FORMAT("EGT-GenerateNext. target=0x%X", pEffect->GetEffectTarget());
	#endif

	if (pEffect==NULL)
	{
		return;
	}

	//------------------------------------------------------------
	// pEffect의 정보를 참고해서 Generate한다.
	//------------------------------------------------------------
	MEffectTarget* pEffectTarget = pEffect->GetEffectTarget();
	
	//------------------------------------------------------------
	// [ 필살!~ ] 다음 Effect로 Link가 된 Effect의 
	//						EffectTarget을 제거한다.
	//
	// pEffectTarget는 Generate()에서 다음 Effect로 전달되거나
	//							이번 loop에서 delete되어야 한다.
	//------------------------------------------------------------
	pEffect->SetEffectTargetNULL();

	//------------------------------------------------------------
	// 목표가 없으면..
	//------------------------------------------------------------
	if (pEffectTarget==NULL)
	{
		#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
			DEBUG_ADD("return-pEffectTarget NULL");
		#endif

		return;
	}

	///*
	// 으흠 - -;
	// 완전히 끝난걸 의미한다.
	if (pEffectTarget->IsEnd())
	{
		#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
			DEBUG_ADD("EffectTarget End - Check Result0");
		#endif

		// 결과가 있으면 처리해야 한다.
 		if (pEffectTarget->IsExistResult())
		{
			MActionResult* pResult = pEffectTarget->GetResult();

			pEffectTarget->SetResultNULL();

			#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
				DEBUG_ADD("Execute Result0");
			#endif

			// 결과 실행
			pResult->Execute();
			
			// 메모리 제거
			delete pResult;			
		}
		#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
			else		
			{			
				DEBUG_ADD("NO Result0");				
			}
		#endif
		
		// Player의 진행중이던 EffectTarget은 끝났다고 설정한다.
		g_pPlayer->RemoveEffectTarget( pEffectTarget->GetEffectID() );

		#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
			DEBUG_ADD("delete EffectTarget0");
		#endif

		delete pEffectTarget;	// 음(!) 이게 맞나??
		//pEffect->SetEffectTargetNULL();

		#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
			DEBUG_ADD("GenerateNext OK0");
		#endif

		return;
	}
	//*/


	// 몇번째 Info냐 하면...
	int nActionInfoNode = pEffectTarget->GetCurrentPhase();		

	
	ACTION_INFO_NODE&			info	= (*g_pActionInfoTable)[ pEffect->GetActionInfo() ][ nActionInfoNode ];

	int targetX = pEffectTarget->GetX();
	int targetY = pEffectTarget->GetY();
	int targetZ = pEffectTarget->GetZ();
	int targetID = pEffectTarget->GetID();
	
	DWORD count;
	
	//------------------------------------------------------------
	// Effect의 이번 node의 지속 시간 결정
	//------------------------------------------------------------
	// bDelayNode이고 설정된 delay가 있는 경우
	//		//main node이고 pEffectTarget의 DelayFrame이 0이 아닌 경우
	//------------------------------------------------------------
	if (info.bDelayNode
		//(*g_pActionInfoTable)[ nActionInfo ].HasMainNode()
		//&& pEffectTarget->GetCurrentPhase()==(*g_pActionInfoTable)[ nActionInfo ].GetMainNode()
		&& pEffectTarget->GetDelayFrame()!=0)
	{
		count = pEffectTarget->GetDelayFrame();
	}
	else
	{
		count = info.Count;
	}

	//------------------------------------------------------------
	// 다음 목표
	//------------------------------------------------------------
	pEffectTarget->NextPhase();

	//------------------------------------------------------------
	// EffectGenerator가 생성되지 않은 경우
	//------------------------------------------------------------
	if (info.EffectGeneratorID >= MAX_EFFECTGENERATORID
		|| m_pEffectGenerator[ info.EffectGeneratorID ]==NULL)
	{
		#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
			DEBUG_ADD_FORMAT("[Error] EGT-Generate id=%d, target=0x%X", info.EffectGeneratorID, pEffectTarget);
		#endif	

		// 목표 완료시킨다.
		//while (!pEffectTarget->IsEnd())
		//{
		//	pEffectTarget->NextPhase();
		//}

		if (pEffectTarget->IsExistResult())
		{
			pEffectTarget->GetResult()->Release();			
		}

		delete pEffectTarget;
		//pEffect->SetEffectTargetNULL();	// 이거 주석 되었던 이유가 뭘까.. -> 위에서 해주네 - -;

		return;
	}

	//------------------------------------------------------------
	//
	// EffectGenerator에서 Effect를 생성하게 한다.
	//
	//------------------------------------------------------------
	EFFECTGENERATOR_INFO egInfo;

	egInfo.nActionInfo			= pEffect->GetActionInfo();
	
	// 마지막꺼면 target을 넘겨주지 않는다.
	egInfo.pEffectTarget		= pEffectTarget->IsEnd()? NULL : pEffectTarget;

	egInfo.effectSpriteType		= info.EffectSpriteType;
	egInfo.x0					= pEffect->GetPixelX();
	egInfo.y0					= pEffect->GetPixelY();
	egInfo.z0					= pEffect->GetPixelZ();
	egInfo.x1					= targetX;
	egInfo.y1					= targetY;
	egInfo.z1					= targetZ;
	egInfo.creatureID			= targetID;
	egInfo.direction			= pEffect->GetDirection();
	egInfo.step					= info.Step;
	egInfo.count				= count;
	egInfo.linkCount			= info.LinkCount;
	egInfo.power				= pEffect->GetPower();
	egInfo.pPreviousEffect		= pEffect;
	egInfo.temp1				= 0;
	egInfo.temp2				= 0;

	// Generate
	bool bGenerated = false;

	//------------------------------------------------------------
	// Generate의 return값은 egInfo.pEffectTarget이 
	// 내부에서 다른 effect로 link되었는가를 의미한다.
	//------------------------------------------------------------
	#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
		DEBUG_ADD_FORMAT("Generate before. id=%d", info.EffectGeneratorID);
	#endif	

	if (m_pEffectGenerator[ info.EffectGeneratorID ]->Generate( egInfo ))
	{
		#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
			DEBUG_ADD("Generate OK");
		#endif	

		//------------------------------------------------------------
		// 기술의 단계에 맞는 sound를 출력해준다.
		//------------------------------------------------------------
		if (info.SoundID != SOUNDID_NULL)
		{
			//g_Sound.Play( g_SoundTable[info.SoundID].pDSBuffer );
			POINT point = MTopView::PixelToMap(pEffect->GetPixelX(), pEffect->GetPixelY());
			PlaySound( info.SoundID, false, point.x, point.y );
		}

		//------------------------------------------------------------
		// 결과를 처리해야하는 시점인가? 
		//------------------------------------------------------------
		if (info.bResultTime)
		{
			// 결과 처리 시간이 지났다고 체크한다.
			// 나중에 결과가 생기면 바로 처리하게 된다.
			pEffectTarget->SetResultTime();
		}

		bGenerated = true;
	}
	else
	{
		#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
			DEBUG_ADD("Generate false");
		#endif

		pEffectTarget->SetResultTime();
	}

	//------------------------------------------------------------
	// 결과 처리
	//------------------------------------------------------------
	if (pEffectTarget->IsResultTime() 
		|| pEffectTarget->IsEnd()
		|| !bGenerated)
	{
		#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
			DEBUG_ADD("Check Result");
		#endif

		// 결과가 있으면 처리해야 한다.
 		if (pEffectTarget->IsExistResult())
		{
			MActionResult* pResult = pEffectTarget->GetResult();

			pEffectTarget->SetResultNULL();

			#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
				DEBUG_ADD("Execute Result");
			#endif

			// 결과 실행
			pResult->Execute();
			
			// 메모리 제거
			delete pResult;
			
			//pEffectTarget
		}
		#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
			else
			{
				DEBUG_ADD("NO Result");
			}
		#endif
		
		// Player의 진행중이던 EffectTarget은 끝났다고 설정한다.
		g_pPlayer->RemoveEffectTarget( pEffectTarget->GetEffectID() );

		//delete pEffectTarget;
		//pEffect->SetEffectTargetNULL();
	}

	//------------------------------------------------------------
	// 더 이상 목표가 필요없는 마지막 단계인 경우..
	//------------------------------------------------------------
	// end이면 Generate()에서 pEffectTarget을 넘겨주지 않으므로
	// 여기서 지워야 한다.
	if (pEffectTarget->IsEnd() || !bGenerated)
	{
		#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
			DEBUG_ADD("delete EffectTarget");
		#endif
			
		if (pEffectTarget->IsExistResult())
		{
			MActionResult* pResult = pEffectTarget->GetResult();
			
			pEffectTarget->SetResultNULL();
			
#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
			DEBUG_ADD("Execute Result");
#endif
			
			// 결과 실행
			pResult->Execute();
			
			// 메모리 제거
			delete pResult;
		}

		delete pEffectTarget;
		//pEffect->SetEffectTargetNULL();
	}	

	#ifdef OUTPUT_DEBUG_EFFECT_GENERATOR
		DEBUG_ADD("GenerateNext OK");
	#endif
}


