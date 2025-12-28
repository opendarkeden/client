//----------------------------------------------------------------------
// MBloodyBreakerEffectGenerator.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MBloodyBreakerEffectGenerator.h"
#include "MEffect.h"
#include "MTopView.h"
#include "MZone.h"
#include "EffectSpriteTypeDef.h"
#include "MEffectSpriteTypeTable.h"

#include "DebugInfo.h"

const char bloody_breaker_center_x = 6;
const char bloody_breaker_center_y = 6;
char bloody_breaker_map[8][13][13] = {0, };

struct tempBreaker
{
	int x;
	int y;
	BYTE center;
};

void	MakeMap(BYTE dic, std::vector<tempBreaker> &v_cp, int p)
{
	const char width[7] = { 0, 0, 1, 1, 2, 2, 2 };
	
    POINT mask[8];
    mask[0].x = -1;
    mask[0].y =  0;
    mask[1].x = -1;
    mask[1].y =  1;
    mask[2].x =  0;
    mask[2].y =  1;
    mask[3].x =  1;
    mask[3].y =  1;
    mask[4].x =  1;
    mask[4].y =  0;
    mask[5].x =  1;
    mask[5].y = -1;
    mask[6].x =  0;
    mask[6].y = -1;
    mask[7].x = -1;
    mask[7].y = -1;	
    
	int l = 0;
	for ( int i = 1; i <= 6; i++ )
	{
		int x = 0;
		int y = 0;
		
		for ( int j = 0; j <= width[i]; j++ )
		{
			x = mask[dic].x * i;
			y = mask[dic].y * i;				
			
			
			if ( j == 0)
			{
				if( p == i )
				{
					tempBreaker pt= { x,y,1};
					v_cp.push_back(pt);
				}
			}
			else
			{
				int left  = ( (dic&0x1) == 0 ? (( dic + 2 )&0x7) : (( dic + 3 )&0x7) );
				int right = ( (dic&0x1) == 0 ? (( dic + 6 )&0x7) : (( dic + 5 )&0x7) );
				
				int xl = x + ( mask[left].x * j );
				int yl = y + ( mask[left].y * j );
				
				int xr = x + ( mask[right].x * j );
				int yr = y + ( mask[right].y * j );
				
				if( p == i )
				{
					tempBreaker pt = { xl,yl,0};					
					v_cp.push_back(pt);
					pt.x = xr;
					pt.y = yr;
					v_cp.push_back(pt);
				}
			}
		}
	}
}


bool
MBloodyBreakerEffectGenerator::Generate( const EFFECTGENERATOR_INFO& egInfo )
{
	int est = egInfo.effectSpriteType;
	
	BLT_TYPE		bltType = (*g_pEffectSpriteTypeTable)[est].BltType;
	TYPE_FRAMEID	frameID	= (*g_pEffectSpriteTypeTable)[est].FrameID;
	bool			repeatFrame	= (*g_pEffectSpriteTypeTable)[est].RepeatFrame;
	int maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);	
		
	//int currentPhase = (egInfo.pEffectTarget==NULL? lastPhase +1 : egInfo.pEffectTarget->GetCurrentPhase());

	int currentPhase = egInfo.pEffectTarget != NULL ?egInfo.pEffectTarget->GetCurrentPhase() : -1;

	unsigned char currentDirection = egInfo.direction;
	std::vector<tempBreaker> v_cp;

	MakeMap(currentDirection, v_cp, currentPhase );

	if(v_cp.empty())
		return false;

	TYPE_SECTORPOSITION	tX, tY;
	tX = g_pTopView->PixelToMapX(egInfo.x0);
	tY = g_pTopView->PixelToMapY(egInfo.y0);

	
	for(int i=0;i<v_cp.size();i++)
	{
		MEffect *pEffect = new MEffect (bltType);

		pEffect->SetFrameID( frameID, maxFrame );
		int tempy =0, tempx =0;
		if(currentPhase != -1)
		{
			if(currentDirection == 7 || currentDirection == 6 || currentDirection == 5)
				tempy+=(currentPhase-1);
			if(currentDirection == 1 || currentDirection == 2 || currentDirection == 3)
				tempy-=(currentPhase-1);
			if(currentDirection == 0 || currentDirection == 1 || currentDirection == 7)
				tempx+=(currentPhase-1);
			if(currentDirection == 5 || currentDirection == 4 || currentDirection == 3)
				tempx-=(currentPhase-1);
		}
		pEffect->SetPosition(v_cp[i].x+tX+tempx,v_cp[i].y+tY+tempy);
		pEffect->SetZ(egInfo.z0);
		pEffect->SetStepPixel(egInfo.step);
		pEffect->SetCount(egInfo.count, egInfo.linkCount);
		pEffect->SetDirection( egInfo.direction );
		pEffect->SetPower(egInfo.power);
		pEffect->SetMulti( true );

		if(g_pZone->AddEffect( pEffect ))
		{
			if(v_cp[i].center == 1 && egInfo.pEffectTarget != NULL) 
			{			
				pEffect->SetLink( egInfo.nActionInfo, egInfo.pEffectTarget );
				
			} else
			{
				pEffect->SetLink( egInfo.nActionInfo, NULL );
			}
		}
		if (est>=EFFECTSPRITETYPE_BLOODY_WALL_1
			&& est<=EFFECTSPRITETYPE_BLOODY_WALL_3)
		{
			if (++est > EFFECTSPRITETYPE_BLOODY_WALL_3)
			{
				est = EFFECTSPRITETYPE_BLOODY_WALL_1;
			}
		}
		frameID	= (*g_pEffectSpriteTypeTable)[est].FrameID;
		maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);	
	}
	
	return true;
}
