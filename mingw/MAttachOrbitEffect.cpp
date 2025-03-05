//----------------------------------------------------------------------
// MAttachOrbitEffect.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MAttachOrbitEffect.h"
#include "MTopView.h"
#include "MEffectSpriteTypeTable.h"
#include "MathTable.h"

//----------------------------------------------------------------------
// Static
//----------------------------------------------------------------------
POINT	
MAttachOrbitEffect::s_OrbitPosition[MAX_EFFECT_ORBIT_TYPE][MAX_EFFECT_ORBIT_STEP];

//----------------------------------------------------------------------
// Init OrbitPosition
//----------------------------------------------------------------------
void		
MAttachOrbitEffect::InitOrbitPosition()
{
	int radOrbitStep			= MathTable::MAX_ANGLE/(MAX_EFFECT_ORBIT_STEP/2);
	const int pixelOrbitWidthHalf[MAX_EFFECT_ORBIT_TYPE]	= { TILE_X*2, TILE_X, TILE_X/2};
	const int pixelOrbitHeightHalf[MAX_EFFECT_ORBIT_TYPE]	= { TILE_Y*2, TILE_Y, TILE_Y/2};

	for (int type = 0; type < MAX_EFFECT_ORBIT_TYPE; type++)
	{
		if(type == 2)
			radOrbitStep			= MathTable::MAX_ANGLE/(MAX_EFFECT_ORBIT_STEP);

		for (int i=0; i<MAX_EFFECT_ORBIT_STEP; i++)
		{
			int rad = radOrbitStep*i;
			
			s_OrbitPosition[type][i].x = (MathTable::FCos(rad)*pixelOrbitWidthHalf[type])>>16;
			s_OrbitPosition[type][i].y = (MathTable::FSin(rad)*pixelOrbitHeightHalf[type])>>16;
		}
	}
}

//----------------------------------------------------------------------
// 
// constructor/destructor
//
//----------------------------------------------------------------------

MAttachOrbitEffect::MAttachOrbitEffect(TYPE_EFFECTSPRITETYPE type, DWORD last, int orbit_type, int orbit_step, DWORD linkCount)
: MAttachEffect( type, last, linkCount )
{
	m_bRun = true;
	m_Type = orbit_type;

	if(orbit_step == -1)
		m_OrbitStep = rand()%MAX_EFFECT_ORBIT_STEP;
	else
		m_OrbitStep = orbit_step%MAX_EFFECT_ORBIT_STEP;
}

MAttachOrbitEffect::~MAttachOrbitEffect()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Update
//----------------------------------------------------------------------
bool
MAttachOrbitEffect::Update()
{	
	bool bUpdate = MAttachEffect::Update();

	if (bUpdate && m_bRun == true)
	{
		NextOrbitStep();
	}

	return bUpdate;
}
