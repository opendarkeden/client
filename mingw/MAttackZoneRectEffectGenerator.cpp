//----------------------------------------------------------------------
// MAttackZoneRectEffectGenerator.cpp
//----------------------------------------------------------------------
// 가운데서부터 8방향으로 이동하게 만든다.
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MAttackZoneRectEffectGenerator.h"
#include "MLinearEffect.h"
#include "MTopView.h"
#include "MZone.h"
#include "EffectSpriteTypeDef.h"
#include "MEffectSpriteTypeTable.h"


bool
MAttackZoneRectEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	int est = egInfo.effectSpriteType;
	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[est].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[est].FrameID;
	int currentPhase = egInfo.pEffectTarget != NULL ?egInfo.pEffectTarget->GetCurrentPhase() : -1;
	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);

	// 시작 좌표


	if( currentPhase == 2 )
	{
		// 첫번째면 8방향을 생성한다.
		int sx = egInfo.x1;
		int sy = egInfo.y1;
		int sz = egInfo.z1;

		for(int i=0;i<8;i++)
		{
			MLinearEffect *pEffect  = new MLinearEffect(bltType);
			pEffect->SetFrameID( frameID, maxFrame );
			pEffect->SetPixelPosition( sx, sy, 0 );
			int tx,ty;

			tx = g_pTopView->PixelToMapX( sx );
			ty = g_pTopView->PixelToMapY( sy );


			if( i == 0 || i == 1 || i == 7 )
				tx --;
			else
			if( i == 3 || i == 4 || i == 5 )
				tx ++;
			
			if( i == 1 || i == 2 || i == 3 )
				ty ++;
			else
			if( i == 5 || i == 6 || i == 7 )
				ty --;

			tx = g_pTopView->MapToPixelX( tx );
			ty = g_pTopView->MapToPixelY( ty );
			
			pEffect->SetTarget(tx,ty,0, egInfo.step );
			pEffect->SetDirection( i );
			pEffect->SetCount(egInfo.count, egInfo.linkCount);
			pEffect->SetPower(egInfo.power);
			
			if(g_pZone->AddGroundEffect( pEffect ) && i ==0 )
			{
				pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget);
			} else
			{
				pEffect->SetLink( egInfo.nActionInfo, NULL);
			}
		}

	} else
	{
		// 첫번째가 아니면

		int sx = egInfo.x0;
		int sy = egInfo.y0;
		int sz = egInfo.z0;


		int tx,ty;

		tx = egInfo.x1;
		ty = egInfo.y1;

		MLinearEffect* pEffect = new MLinearEffect(bltType);
		
		pEffect->SetFrameID( frameID, maxFrame );
		pEffect->SetPixelPosition( sx,sy,0 );
		pEffect->SetTarget(tx,ty,sz,egInfo.step);
		pEffect->SetDirection( egInfo.direction);
		pEffect->SetCount( egInfo.count,  egInfo.linkCount );
		pEffect->SetPower( egInfo.power);
		
		if(g_pZone->AddGroundEffect( pEffect ) )
		{
			pEffect->SetLink(egInfo.nActionInfo, egInfo.pEffectTarget );
		} else
		{
			pEffect->SetLink( egInfo.nActionInfo, NULL);
		}
	}
		
	return true;	
}

