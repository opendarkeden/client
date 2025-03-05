//----------------------------------------------------------------------
// MAttachEffect.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include <math.h>
#include "MMovingEffect.h"
#include "MAttachEffect.h"
#include "MTopView.h"
#include "MEffectSpriteTypeTable.h"

//----------------------------------------------------------------------
// 
// constructor/destructor
//
//----------------------------------------------------------------------

MAttachEffect::MAttachEffect(TYPE_EFFECTSPRITETYPE type, DWORD last, DWORD linkCount)
: MMovingEffect( (type < g_pEffectSpriteTypeTable->GetSize()? (*g_pEffectSpriteTypeTable)[type].BltType : BLT_EFFECT) )
{
	// class종류
	//m_EffectType	= EFFECT_ATTACH;

	// 따라가는 Creature ID
	m_CreatureID	= OBJECTID_NULL;

	m_bEffectSprite = true;

	// EffectSprite 종류
	m_EffectSpriteType	= type;
	
	// 끝나는 시간 = 현재 시간 + 지속 시간
	// 2004, 12, 9, sobeit add start - 정말 무한?
	if(last == 0xFFFF)
		m_EndFrame			= 0xFFFFFFFF;
	else
	// 2004, 12, 9, sobeit add end
		m_EndFrame			= g_CurrentFrame + last - 1;
	
	// 특별히 색깔 바뀌는 부위 없음...
	// 전체가 바뀐다는걸 의미하기도 한다. - -;
	m_bEffectColorPart = ADDON_NULL;

	// linkCount
	if (linkCount==MAX_LINKCOUNT)
	{
		m_EndLinkFrame = m_EndFrame;
	}
	else
	{
		m_EndLinkFrame = g_CurrentFrame + linkCount - 1;
	}

	//-------------------------------------------------------
	// 제대로 된 type인 경우.. 아닌 경우는 색깔이거나 뭐..그렇다 - -;
	//-------------------------------------------------------
	if (type < g_pEffectSpriteTypeTable->GetSize())
	{
		TYPE_FRAMEID	frameID = (*g_pEffectSpriteTypeTable)[type].FrameID;
		BYTE			maxFrame;
		
		// BLT_NORMAL		
		BLT_TYPE bltType = (*g_pEffectSpriteTypeTable)[type].BltType;
		maxFrame = g_pTopView->GetMaxEffectFrame(bltType, frameID);		

		SetFrameID( frameID, maxFrame );

		// default로 Effect의 밝기를 지정한다.
		if (m_BltType == BLT_EFFECT)
		{
			m_Light = g_pTopView->m_EffectAlphaFPK[m_FrameID][m_Direction][m_CurrentFrame].GetLight();
		}
		else
		{
			m_Light = 0;
		}
	}
	else
	{
		SetFrameID( 0, 1 );
		m_Light = 0;
	}
}

MAttachEffect::~MAttachEffect()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Move
//----------------------------------------------------------------------
// 매 순간마다 StepX~Z가 달라진다.
//----------------------------------------------------------------------
void				
MAttachEffect::SetAttachCreatureID(TYPE_OBJECTID id)
{ 
	MCreature* pCreature = g_pZone->GetCreature( id );

	if (pCreature==NULL)
	{
		pCreature = g_pZone->GetFakeCreature( id );

		if (pCreature==NULL)
		{
			MItem* pItem = g_pZone->GetItem( id );

			if (pItem!=NULL
				&& pItem->GetItemClass()==ITEM_CLASS_CORPSE)
			{
				pCreature = ((MCorpse*)pItem)->GetCreature();
			}			
		}
	}

	SetAttachCreature( pCreature );
}

//----------------------------------------------------------------------
// TraceCreature
//----------------------------------------------------------------------
bool
MAttachEffect::SetAttachCreature(MCreature* pCreature)
{	
	// Creature가 사라졌을 경우..
	if (pCreature == NULL)
	{
		m_EndFrame = 0;
		return false;	
	}

	m_CreatureID = pCreature->GetID(); 

	// 현재의 좌표를 읽어온다.
	POINT point;// = MTopView::MapToPixel(pCreature->GetX(), pCreature->GetY());
	//point.x += pCreature->GetSX();
	//point.y += pCreature->GetSY();
	point.x = pCreature->GetPixelX();
	point.y = pCreature->GetPixelY();

	// 새로운 좌표 설정	
	m_PixelX = point.x;
	m_PixelY = point.y;
	m_PixelZ = pCreature->GetZ();

	//--------------------------------
	// Sector 좌표를 맞춘다.
	//--------------------------------
	AffectPosition();

	// 좌표
	//#ifdef	OUTPUT_DEBUG
	//	sprintf(g_pDebugMessage->GetCurrent(), "Set AttachEffect : Type=%d, Light=%d, (%d, %d)", pEffect->GetFrameID(), m_Light, m_X, m_Y);	
	//	g_pDebugMessage->Next();
	//#endif

	return true;
}

//----------------------------------------------------------------------
// Update
//----------------------------------------------------------------------
bool
MAttachEffect::Update()
{	
	if (g_CurrentFrame < m_EndFrame)
	{	
		// Frame을 바꿔준다.
		NextFrame();

		// Counter를 하나 줄인다.
		//m_Count--;

		if (m_BltType == BLT_EFFECT)
		{
			m_Light = g_pTopView->m_EffectAlphaFPK[m_FrameID][m_Direction][m_CurrentFrame].GetLight();
		}

		return true;
	}

	// 캐릭터 좌표에 붙는다. 
	// 다음 Effect로 연결하기 위해서..좌표를 설정한다.
	//AttachCreature();

	return false;
}
