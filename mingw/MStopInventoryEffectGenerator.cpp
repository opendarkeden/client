//----------------------------------------------------------------------
// MStopInventoryEffectGenerator.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MStopInventoryEffectGenerator.h"
#include "MScreenEffect.h"
#include "MTopView.h"
#include "MScreenEffectManager.h"
#include "UIFunction.h"
#include "MInventory.h"
#include "EffectSpriteTypeDef.h"
#include "MEffectSpriteTypeTable.h"
#include "DebugInfo.h"
#include "VS_UI_GameCommon.h"

//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE

extern MScreenEffectManager*	g_pInventoryEffectManager;

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
//MStopInventoryEffectGenerator	g_StopInventoryEffectGenerator;

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
bool
MStopInventoryEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	int est = egInfo.effectSpriteType;

	int invenX = egInfo.x1;
	int invenY = egInfo.y1;
	
	POINT grid_plus = { 0, 0 };
	
//	if (est==EFFECTSPRITETYPE_IDENTIFY_1x1
//		|| est==EFFECTSPRITETYPE_ENCHANT_1x1)
//	{
		const MItem* pItem = g_pInventory->GetItem( invenX, invenY );

		//---------------------------------------------------------------
		// 아이템이 있으면 크기에 따라서 이펙트가 바뀐다.
		//---------------------------------------------------------------
		if (pItem!=NULL)
		{
			grid_plus.x = (pItem->GetGridWidth()-1)*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X/2;
			grid_plus.y = (pItem->GetGridHeight()-1)*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y/2;
//			const int gridMax = 4;
//			const int estIdentify[gridMax] =
//			{
//				EFFECTSPRITETYPE_IDENTIFY_1x1,		// 0
//				EFFECTSPRITETYPE_IDENTIFY_1x3,		// 1
//				EFFECTSPRITETYPE_IDENTIFY_2x2,		// 2
//				EFFECTSPRITETYPE_IDENTIFY_2x3,		// 3
//			};
//
//			const int estEnchant[gridMax] =
//			{
//				EFFECTSPRITETYPE_ENCHANT_1x1,		// 0
//				EFFECTSPRITETYPE_ENCHANT_1x3,		// 1
//				EFFECTSPRITETYPE_ENCHANT_2x2,		// 2
//				EFFECTSPRITETYPE_ENCHANT_2x3,		// 3
//			};
//
//			int gridX = pItem->GetGridWidth();
//			int gridY = pItem->GetGridHeight();
//
//			// 카카...
//			int gridValue = (gridX-1)*2 + (gridY==3);
//
//			if (gridValue < gridMax)
//			{
//				if (est==EFFECTSPRITETYPE_IDENTIFY_1x1)
//				{
//					est = estIdentify[gridValue];
//				}
//				else //if (frameID==EFFECTSPRITETYPE_ENCHANT_1x1
//				{
//					est = estEnchant[gridValue];
//				}
//				
//			}
		}
//	}
//*/
	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[est].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[est].FrameID;


	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);

	MScreenEffect*	pEffect;
	//---------------------------------------------
	// Effect 생성
	//---------------------------------------------
	pEffect = new MScreenEffect(bltType);

	pEffect->SetFrameID( frameID, maxFrame );	

	// 기준점 설정..(!!!!!!!!!!)
	//---------------------------------------------
	// 현재 inventory의 위치...
	//---------------------------------------------
	POINT point;
	
	point = UI_GetInventoryPosition();
	pEffect->SetScreenBasis(point.x, point.y);

	// SetPosition을 먼저 해줘야 한다. 안에서 Map좌표로 변환하는거 때매 삑사리난다.
	pEffect->SetPosition( egInfo.x1, egInfo.y1 );

	//---------------------------------------------
	// 화면에서 Effect의 위치
	//---------------------------------------------
	point = UI_GetInventoryGridPosition(egInfo.x1, egInfo.y1);	
	pEffect->SetScreenPosition(point.x+grid_plus.x, point.y+grid_plus.y);


	pEffect->SetStepPixel(0);		// 실제로 움직이지는 않지만, 다음 Effect를 위해서 대입해준다.
	pEffect->SetCount( egInfo.count, egInfo.linkCount );			// 지속되는 Frame

	// 방향 설정
	pEffect->SetDirection( 0 );

	// 위력
	pEffect->SetPower(egInfo.power);

	// 빛의 밝기
	//pEffect->SetLight( light );

	// Inventory에 추가한다.
	
	//if (
	g_pInventoryEffectManager->AddEffect( pEffect );
	//)
	{
		// 다음 Effect 생성 정보
		pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );	
	}

	return true;
}
