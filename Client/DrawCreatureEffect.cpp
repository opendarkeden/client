//#define __3D_IMAGE_OBJECT__					// by sonee
//----------------------------------------------------------------------
// MTopViewDraw.cpp
//----------------------------------------------------------------------
//
// 복잡한 Draw 함수 부분만 따로 빼놓는다.
// 작업하기가 영 불편해서 
//----------------------------------------------------------------------
#include "Client_PCH.h"
#pragma warning(disable:4786)

#include "DX3D.h"
#include <math.h>
#include <list>
#include <stdio.h>
#include <fstream.h>
#include "MZone.h"
#include "MCreature.h"
#include "MFakeCreature.h"
#include "MPlayer.h"
#include "MItem.h"
#include "MImageObject.h"
#include "MAnimationObject.h"
#include "MShadowAnimationObject.h"
#include "assert.h"

#include "MMovingEffect.h"
#include "MScreenEffect.h"
#include "MTopView.h"
#include "MItemTable.h"
#include "MCreatureTable.h"
#include "MWeather.h"
#include "CMessageArray.h"
#include "DXLib.h"
#include "SP.h"
#include "FL2.h"
//#include "2D.h"
#include "ClientConfig.h"
#include "VS_UI_BASE.h"
#include "VS_UI_Mouse_Pointer.h"
#include "UIFunction.h"
#include "AddonDef.h"
#include "MZoneTable.h"
#include "MGameTime.h"
#include "UserOption.h"
#include "UserInformation.h"
#include "SpriteIDDef.h"
#include "MGameStringTable.h"
//#include "MZoneInfo.h"
#include "MObjectSelector.h"
#include "MPortal.h"
#include "EffectSpriteTypeDef.h"
#include "MScreenEffectManager.h"
#include "MEffectSpriteTypeTable.h"
#include "Profiler.h"
//#include "MFileDef.h"
#include "Properties.h"
#include "MEventManager.h"
#include "DebugInfo.h"
#include "MItemOptionTable.h"
#include "Client.h"
#include "SkillDef.h"
#include "vs_ui_item.h"
#include "MTopViewDraw.inl"

#ifdef OUTPUT_DEBUG
	extern int g_WipeValue;
	extern int g_WipeStep;
#endif

extern int defaultSkinColor;
extern int defaultCoatColor;
extern int defaultTrouserColor;

#define	SHIFT_SNIPPING			2

namespace {
	POINT pointTemp;
	RECT rect;
};

int GetAdvancementPartFromItemClass( ITEM_CLASS itemClass, TYPE_FRAMEID frameID );
int ConvAdvancementSlayerActionFromSlayerAction( int CurAction, MCreatureWear* pCreatureWear );

void	MTopView::DrawFadeOut( POINT *pPoint, MCreature* pCreature, int action, int direction, int frame )
{
	WORD clothes;
	BYTE clothesType;
	MCreatureWear*	pCreatureWear = (MCreatureWear*)pCreature;

	//-----------------------------------------------------------
	// 좌우 잔상
	//-----------------------------------------------------------
	for (int i=0; i<ADDON_MAX; i++)
	{
		// Creature의 현재 방향에 따라서...
		// 옷을 출력해주는 순서가 다를 수 있다.
		clothesType = MCreatureWear::s_AddonOrder[pCreature->GetDirection()][i];
		
		// i번째 종류의 옷을 입고 있다면 출력해 준다.
		const MCreatureWear::ADDON_INFO& addonInfo = pCreatureWear->GetAddonInfo(clothesType);
		
		if (addonInfo.bAddon)
		{
			clothes = addonInfo.FrameID;
			
			FRAME_ARRAY &FA = m_AddonFPK[clothes][action][direction];
			
			// 있는 동작인 경우
			if (FA.GetSize() > frame)
			{
				CFrame &Frame = FA[frame];					
				int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
				int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
				int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
				
				
				CIndexSprite* pSprite = &m_AddonSPK[ sprite ];					
				
				// 복장Sprite가 초기화 되지 않은 경우
				//							if (pSprite->IsNotInit())
				//							{								
				//								LoadFromFileAddonSPK( clothes, action );
				//								//continue;
				//							}
				
				pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
				pointTemp.y = pPoint->y + cy;// + pCreature->GetSY();
				
				//---------------------------------------- 
				// 캐릭터 선택 사각형 영역 설정
				//---------------------------------------- 	
				rect.left	= pointTemp.x;
				rect.top	= pointTemp.y;
				rect.right	= rect.left + pSprite->GetWidth();
				rect.bottom = rect.top + pSprite->GetHeight();
				pCreature->AddScreenRect( &rect );
				
				int colorSet1 = addonInfo.ColorSet1, colorSet2 = addonInfo.ColorSet2;
				if(colorSet2 == UNIQUE_ITEM_COLOR || colorSet2 == QUEST_ITEM_COLOR)
				{
					// 유니크 아이템이거나 퀘스트 아이템이면
					//								
					//								if(colorSet2 == QUEST_ITEM_COLOR)
					//									colorSet2 = MItem::GetQuestItemColorset();
					//								else
					//									colorSet2 = MItem::GetUniqueItemColorset();									
					
					colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
					if(addonInfo.ItemClass != ITEM_CLASS_COAT && addonInfo.ItemClass != ITEM_CLASS_TROUSER)
						colorSet1 = colorSet2;
				}
				
				CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
				
				//-----------------------------------------------------------
				// 좌우 잔상
				//-----------------------------------------------------------
				POINT pointTemp2;
				
				//CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_NET );
				//CIndexSprite::SetEffect( CIndexSprite::EFFECT_NET );
				
				// 왼쪽
				pointTemp2.x = pointTemp.x - 25;
				pointTemp2.y = pointTemp.y;
				m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 2);
				
				// 오른쪽
				pointTemp2.x = pointTemp.x + 25;
				pointTemp2.y = pointTemp.y;
				m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 2);
				
				// 가운데							
				//m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 1);	
				//m_pSurface->BltIndexSprite(&pointTemp, pSprite);	
			}
		}
	}
}


void	MTopView::DrawFadeOutForACSlayer( POINT *pPoint, MCreature* pCreature, int action, int direction, int frame )
{
	int clothes;
	BYTE clothesType;
	MCreatureWear*	pCreatureWear = (MCreatureWear*)pCreature;
/*
	action = ConvAdvancementSlayerActionFromSlayerAction( action, dynamic_cast< MCreatureWear* >(pCreature) );

	if( action == -1 )
		return;
	else
		action -= ADVANCEMENT_ACTION_START;
*/
	CCreatureFramePack& slayerFPK = pCreature->IsMale() ? m_AdvancementSlayerManFPK : m_AdvancementSlayerWomanFPK;
	CIndexSpritePack& addonISPK = pCreature->IsMale() ? m_AdvancementSlayerManSPK : m_AdvancementSlayerWomanSPK;


	//-----------------------------------------------------------
	// 좌우 잔상
	//-----------------------------------------------------------
	for (int i=0; i<AC_ADDON_MAX; i++)
	{
		// Creature의 현재 방향에 따라서...
		// 옷을 출력해주는 순서가 다를 수 있다.
		clothesType = MCreatureWear::s_ACAddonOrder[ direction ][ i ];
		
		// i번째 종류의 옷을 입고 있다면 출력해 준다.
		const MCreatureWear::ADDON_INFO& addonInfo = pCreatureWear->GetACAddonInfo(clothesType);
		
		if (addonInfo.bAddon || i == AC_ADDON_BODY )
		{
			clothes = GetAdvancementPartFromItemClass( addonInfo.ItemClass, addonInfo.FrameID );

			if( clothes == -1 )
				continue;
			
			FRAME_ARRAY &FA = slayerFPK[clothes][action][direction];
			
			// 있는 동작인 경우
			if (FA.GetSize() > frame)
			{
				CFrame &Frame = FA[frame];					
				int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
				int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
				int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
				
				
				CIndexSprite* pSprite = &addonISPK[ sprite ];					
				
				// 복장Sprite가 초기화 되지 않은 경우
				//							if (pSprite->IsNotInit())
				//							{								
				//								LoadFromFileAddonSPK( clothes, action );
				//								//continue;
				//							}
				
				pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
				pointTemp.y = pPoint->y + cy;// + pCreature->GetSY();
				
				//---------------------------------------- 
				// 캐릭터 선택 사각형 영역 설정
				//---------------------------------------- 	
				rect.left	= pointTemp.x;
				rect.top	= pointTemp.y;
				rect.right	= rect.left + pSprite->GetWidth();
				rect.bottom = rect.top + pSprite->GetHeight();
				pCreature->AddScreenRect( &rect );
				
				int colorSet1 = addonInfo.ColorSet1, colorSet2 = addonInfo.ColorSet2;
				if(colorSet2 == UNIQUE_ITEM_COLOR || colorSet2 == QUEST_ITEM_COLOR)
				{
					// 유니크 아이템이거나 퀘스트 아이템이면
					//								
					//								if(colorSet2 == QUEST_ITEM_COLOR)
					//									colorSet2 = MItem::GetQuestItemColorset();
					//								else
					//									colorSet2 = MItem::GetUniqueItemColorset();									
					
					colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
					if(addonInfo.ItemClass != ITEM_CLASS_COAT && addonInfo.ItemClass != ITEM_CLASS_TROUSER)
						colorSet1 = colorSet2;
				}
				
				CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
				
				//-----------------------------------------------------------
				// 좌우 잔상
				//-----------------------------------------------------------
				POINT pointTemp2;
				
				//CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_NET );
				//CIndexSprite::SetEffect( CIndexSprite::EFFECT_NET );
				
				// 왼쪽
				pointTemp2.x = pointTemp.x - 25;
				pointTemp2.y = pointTemp.y;
				m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 2);
				
				// 오른쪽
				pointTemp2.x = pointTemp.x + 25;
				pointTemp2.y = pointTemp.y;
				m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 2);
				
				// 가운데							
				//m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 1);	
				//m_pSurface->BltIndexSprite(&pointTemp, pSprite);	
			}
		}
	}
}

void	MTopView::DrawAfterSlayer(POINT* pPoint, MCreature* pCreature, int action, int direction, int frame )
{
	WORD clothes;
	BYTE clothesType;
	MCreatureWear*	pCreatureWear = (MCreatureWear*)pCreature;

	//-----------------------------------------------------------
	// 좌우 잔상
	//-----------------------------------------------------------
	for (int i=0; i<ADDON_MAX; i++)
	{
		// Creature의 현재 방향에 따라서...
		// 옷을 출력해주는 순서가 다를 수 있다.
		clothesType = MCreatureWear::s_AddonOrder[pCreature->GetDirection()][i];
		
		// i번째 종류의 옷을 입고 있다면 출력해 준다.
		const MCreatureWear::ADDON_INFO& addonInfo = pCreatureWear->GetAddonInfo(clothesType);
		
		if (addonInfo.bAddon)
		{
			clothes = addonInfo.FrameID;
			
			FRAME_ARRAY &FA = m_AddonFPK[clothes][action][direction];
			
			// 있는 동작인 경우
			if (FA.GetSize() > frame)
			{
				CFrame &Frame = FA[frame];					
				int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
				int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
				int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
				
				
				CIndexSprite* pSprite = &m_AddonSPK[ sprite ];					
				
				// 복장Sprite가 초기화 되지 않은 경우
				//							if (pSprite->IsNotInit())
				//							{								
				//								LoadFromFileAddonSPK( clothes, action );
				//								//continue;
				//							}
				
				pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
				pointTemp.y = pPoint->y + cy;// + pCreature->GetSY();
				
				//---------------------------------------- 
				// 캐릭터 선택 사각형 영역 설정
				//---------------------------------------- 	
				rect.left	= pointTemp.x;
				rect.top	= pointTemp.y;
				rect.right	= rect.left + pSprite->GetWidth();
				rect.bottom = rect.top + pSprite->GetHeight();
				pCreature->AddScreenRect( &rect );
				
				int colorSet1 = addonInfo.ColorSet1, colorSet2 = addonInfo.ColorSet2;
				if(colorSet2 == UNIQUE_ITEM_COLOR || colorSet2 == QUEST_ITEM_COLOR)
				{
					// 유니크 아이템이거나 퀘스트 아이템이면
					//								
					//								if(colorSet2 == QUEST_ITEM_COLOR)
					//									colorSet2 = MItem::GetQuestItemColorset();
					//								else
					//									colorSet2 = MItem::GetUniqueItemColorset();									
					
					colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
					if(addonInfo.ItemClass != ITEM_CLASS_COAT && addonInfo.ItemClass != ITEM_CLASS_TROUSER)
						colorSet1 = colorSet2;
				}
				
				CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
				
				//-----------------------------------------------------------
				// 좌우 잔상
				//-----------------------------------------------------------
				POINT pointTemp2;
				
				int iX[8] =  { 15 , 15 , 0 , -15 , -15 , -15  , 0  , 15 } ;
				int iX1[8] = { 30 , 30 , 0 , -30 , -30 , -30  , 0  , 30 } ;

				int iY[8] =  { 0 , -15 , -15  , -15   , 0  , 15   , 15  , 15 } ;
				int iY1[8] = { 0 , -30 , -30  , -30   , 0  , 30   , 30  , 30 } ;


				// 왼쪽
				pointTemp2.x = pointTemp.x + iX[direction];
				pointTemp2.y = pointTemp.y + iY[direction];
				m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 1);
				
				// 오른쪽
				pointTemp2.x = pointTemp.x + iX1[direction];
				pointTemp2.y = pointTemp.y + iY1[direction];
				m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 2);
			}
		}
	}
}

void	MTopView::DrawAfterACSlayer(POINT* pPoint, MCreature* pCreature, int action, int direction, int frame )
{
	int clothes;
	BYTE clothesType;
	MCreatureWear*	pCreatureWear = (MCreatureWear*)pCreature;

	CCreatureFramePack& slayerFPK = pCreature->IsMale() ? m_AdvancementSlayerManFPK : m_AdvancementSlayerWomanFPK;
	CIndexSpritePack& addonISPK = pCreature->IsMale() ? m_AdvancementSlayerManSPK : m_AdvancementSlayerWomanSPK;

	//-----------------------------------------------------------
	// UP / DOWN  잔상
	//-----------------------------------------------------------
	for (int i=0; i<AC_ADDON_MAX; i++)
	{
		// Creature의 현재 방향에 따라서...
		// 옷을 출력해주는 순서가 다를 수 있다.
		clothesType = MCreatureWear::s_ACAddonOrder[ direction ][ i ];
		
		// i번째 종류의 옷을 입고 있다면 출력해 준다.
		const MCreatureWear::ADDON_INFO& addonInfo = pCreatureWear->GetACAddonInfo(clothesType);
		
		if (addonInfo.bAddon || i == AC_ADDON_BODY )
		{
			clothes = GetAdvancementPartFromItemClass( addonInfo.ItemClass, addonInfo.FrameID );

			if( clothes == -1 )
				continue;
			
			FRAME_ARRAY &FA = slayerFPK[clothes][action][direction];
			
			// 있는 동작인 경우
			if (FA.GetSize() > frame)
			{
				CFrame &Frame = FA[frame];					
				int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
				int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
				int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
				
				
				CIndexSprite* pSprite = &addonISPK[ sprite ];					
				
				// 복장Sprite가 초기화 되지 않은 경우
				//							if (pSprite->IsNotInit())
				//							{								
				//								LoadFromFileAddonSPK( clothes, action );
				//								//continue;
				//							}
				
				pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
				pointTemp.y = pPoint->y + cy;// + pCreature->GetSY();
				
				//---------------------------------------- 
				// 캐릭터 선택 사각형 영역 설정
				//---------------------------------------- 	
				rect.left	= pointTemp.x;
				rect.top	= pointTemp.y;
				rect.right	= rect.left + pSprite->GetWidth();
				rect.bottom = rect.top + pSprite->GetHeight();
				pCreature->AddScreenRect( &rect );
				
				int colorSet1 = addonInfo.ColorSet1, colorSet2 = addonInfo.ColorSet2;
				if(colorSet2 == UNIQUE_ITEM_COLOR || colorSet2 == QUEST_ITEM_COLOR)
				{
					colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
					if(addonInfo.ItemClass != ITEM_CLASS_COAT && addonInfo.ItemClass != ITEM_CLASS_TROUSER)
						colorSet1 = colorSet2;
				}
				CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
				//-----------------------------------------------------------
				// UP / DOWN 잔상
				//-----------------------------------------------------------
				POINT pointTemp2;
				
				int iX[8] =  { 15 , 15 , 0 , -15 , -15 , -15  , 0  , 15 } ;
				int iX1[8] = { 30 , 30 , 0 , -30 , -30 , -30  , 0  , 30 } ;

				int iY[8] =  { 0 , -15 , -15  , -15   , 0  , 15   , 15  , 15 } ;
				int iY1[8] = { 0 , -30 , -30  , -30   , 0  , 30   , 30  , 30 } ;

				// UP
				pointTemp2.x = pointTemp.x + iX[direction];
				pointTemp2.y = pointTemp.y + iY[direction];
				m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 1);
				
				// DOWN
				pointTemp2.x = pointTemp.x + iX1[direction];
				pointTemp2.y = pointTemp.y + iY1[direction];
				m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 2);

			}
		}
	}
}

void	MTopView::DrawAfterVampire(POINT* pPoint, MCreature* pCreature, int action, int direction, int frame , int body )
{

	//------------------------------------------------
	// 관 출력정보
	//------------------------------------------------
	int casketValue = pCreature->GetCasketCount();
	int creature_type = pCreature->GetCreatureType();
	bool bInCasket = pCreature->IsInCasket();
	bool bCasketOnly = (bInCasket && casketValue==0);
	
	
	//----------------------------------------------------
	// 완전히 관만 출력하는 경우는 몸 출력을 안한다.
	//----------------------------------------------------
	if (!bCasketOnly)
	{
		FRAME_ARRAY& FA = m_CreatureFPK[body][action][direction];
		
		if (FA.GetSize() > frame)
		{
			CFrame& Frame =	FA[frame];
			int sprite = Frame.GetSpriteID(),	//m_CreatureFPK[body][action][direction][frame].GetSpriteID(),
				cx		= Frame.GetCX(),	//m_CreatureFPK[body][action][direction][frame].GetCX(),
				cy		= Frame.GetCY();	//m_CreatureFPK[body][action][direction][frame].GetCY();
			
			// 좌표 보정
			pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
			pointTemp.y = pPoint->y + cy;// + pCreature->GetSY();
			
			CIndexSprite* pSprite = &m_CreatureSPK[ sprite ];
			
			//---------------------------------------- 
			// 캐릭터 선택 사각형 영역 설정
			//---------------------------------------- 	
			rect.left	= pointTemp.x;
			rect.top	= pointTemp.y;
			rect.right	= rect.left + pSprite->GetWidth();
			rect.bottom = rect.top + pSprite->GetHeight();
			//if(frameindex==0 )
			pCreature->SetScreenRect( &rect );
			
			// Player가 추적중인 Character 표시
			//if (g_pPlayer->GetTraceCreatureID()==pCreature->GetID())			
			
			//CIndexSprite::SetUsingColorSet(c1%MAX_COLORSET, c2%MAX_COLORSET);
			
			//-----------------------------------------------------------
			// CutHeight
			//-----------------------------------------------------------
			bool bCutHeight = pCreature->IsCutHeight();
			int previousClipBottom;
			if (bCutHeight)
			{
				previousClipBottom = m_pSurface->GetClipBottom();
				m_pSurface->SetClipBottom( rect.bottom - TILE_Y_HALF );
				
				pointTemp.y += pCreature->GetCutHeightCount() - TILE_Y_HALF;
			}
			
			//-----------------------------------------------------------
			// 투명 상태 출력 			
			//-----------------------------------------------------------
			// vampire
			if (pCreature->IsFakeDie())
			{
				WORD colorSet = pCreature->GetAttachEffectColor();
				
				if( pCreature->GetCreatureType() == CREATURETYPE_BAT && pCreature->GetBatColor() != 0xFFFF)
					colorSet = pCreature->GetBatColor();
				
				//---------------------------------------- 
				// 캐릭터 전체 색깔이 바뀌는 경우					
				//---------------------------------------- 
				if (colorSet < MAX_COLORSET)
				{
					// IndexSprite에도 EFFECT...넣고...
					// 스르륵~ 단계적으로 나타나게/없어지게 해야됨.. - -;
					m_pSurface->BltIndexSpriteColorSet(&pointTemp, pSprite, colorSet);
				}
				else
				{
					int colorSet1 = pCreature->GetBodyColor1();
					int colorSet2 = pCreature->GetBodyColor2();
					
					if( colorSet1 == QUEST_ITEM_COLOR || colorSet1 == UNIQUE_ITEM_COLOR )
						colorSet1 = MItem::GetSpecialColorItemColorset( colorSet1 );
					
					if( colorSet2 == QUEST_ITEM_COLOR || colorSet2 == UNIQUE_ITEM_COLOR )
						colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
					
					CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
					
					int netValue = pCreature->GetFakeDieCount()/5;
					
					CSpriteSurface::s_Value1 = netValue;
					
					CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_NET );
					CIndexSprite::SetEffect( CIndexSprite::EFFECT_NET );
					
					m_pSurface->BltIndexSpriteEffect(&pointTemp, pSprite);
					
				}
			}
			else if (pCreature->IsInvisible())
			{
				WORD colorSet = pCreature->GetAttachEffectColor();
				
				if( pCreature->GetCreatureType() == CREATURETYPE_BAT && pCreature->GetBatColor() != 0xFFFF)
					colorSet = pCreature->GetBatColor();
				
				//---------------------------------------- 
				// 캐릭터 전체 색깔이 바뀌는 경우					
				//---------------------------------------- 
				if (colorSet < MAX_COLORSET)
				{
					// IndexSprite에도 EFFECT...넣고...
					// 스르륵~ 단계적으로 나타나게/없어지게 해야됨.. - -;
					m_pSurface->BltIndexSpriteColorSet(&pointTemp, pSprite, colorSet);
				}
				else
				{
					int colorSet1 = pCreature->GetBodyColor1();
					int colorSet2 = pCreature->GetBodyColor2();
					
					if( colorSet1 == QUEST_ITEM_COLOR || colorSet1 == UNIQUE_ITEM_COLOR )
						colorSet1 = MItem::GetSpecialColorItemColorset( colorSet1 );
					
					if( colorSet2 == QUEST_ITEM_COLOR || colorSet2 == UNIQUE_ITEM_COLOR )
						colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
					
					CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
					
					int wipeValue = pCreature->GetInvisibleCount();
					
					if (wipeValue==0)
					{
						m_pSurface->BltIndexSprite(&pointTemp, pSprite);							
					}
					else if (wipeValue==64)
					{
						CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_SIMPLE_OUTLINE );
						CIndexSprite::SetEffect( CIndexSprite::EFFECT_SIMPLE_OUTLINE );
						
						m_pSurface->BltIndexSpriteEffect(&pointTemp, pSprite);
					}
					else
					{
						CSpriteSurface::s_Value1 = wipeValue;
						
						CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_WIPE_OUT );
						CIndexSprite::SetEffect( CIndexSprite::EFFECT_WIPE_OUT );
						
						m_pSurface->BltIndexSpriteEffect(&pointTemp, pSprite);
						
						//CSpriteSurface::s_Value1 = wipeValue/10+1;
						
						//CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_NET );
						//CIndexSprite::SetEffect( CIndexSprite::EFFECT_NET );
						
						//m_pSurface->BltIndexSpriteEffect(&pointTemp, pSprite);
					}							
				}
			}					
			else
			{	
				//-----------------------------------------------------------
				// 빠르게 움직이는 중
				//-----------------------------------------------------------
				if (pCreature->IsFastMove() || pCreature->IsBloodyZenith())
				{
					int colorSet1 = pCreature->GetBodyColor1();
					int colorSet2 = pCreature->GetBodyColor2();
					
					if( colorSet1 == QUEST_ITEM_COLOR || colorSet1 == UNIQUE_ITEM_COLOR )
						colorSet1 = MItem::GetSpecialColorItemColorset( colorSet1 );
					
					if( colorSet2 == QUEST_ITEM_COLOR || colorSet2 == UNIQUE_ITEM_COLOR )
						colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
					
					
					CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
					
					POINT pointTemp2;
					
					//CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_NET );
					//CIndexSprite::SetEffect( CIndexSprite::EFFECT_NET );
					
					int fastMoveShadowMax = 2;
					
					if(creature_type == CREATURETYPE_WILD_WOLF)
						fastMoveShadowMax = 6;
					if(pCreature->IsBloodyZenith())
					{
						fastMoveShadowMax = pCreature->GetBloodyZenithCount();
						pCreature->UpdateBloodyZenith();
					}
					for(int fastMoveShadow = fastMoveShadowMax; fastMoveShadow > 0; fastMoveShadow--)
					{
						// 1 pixel skip
						pointTemp2.x = pointTemp.x - (pCreature->GetCX() >> 1)*fastMoveShadow;
						pointTemp2.y = pointTemp.y - (pCreature->GetCY() >> 1)*fastMoveShadow;
						if(pCreature->IsBloodyZenith())
						{
							pointTemp2.y -= sin((double)max(0, pCreature->GetMoveCount()-fastMoveShadow)*3.141592/(double)pCreature->GetMoveCountMax())*100;
						}
						
						//CSpriteSurface::s_Value1 = 1;
						//m_pSurface->BltIndexSpriteEffect(&pointTemp2, pSprite);
						m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, min(2, fastMoveShadow));
					}
					
				}
				
				if(pCreature->IsBloodyZenith())
				{
					pointTemp.y -= sin((double)pCreature->GetMoveCount()*3.141592/(double)pCreature->GetMoveCountMax())*100;
				}
				WORD colorSet = pCreature->GetAttachEffectColor();
				
				if( pCreature->GetCreatureType() == CREATURETYPE_BAT && pCreature->GetBatColor() != 0xFFFF)
					colorSet = pCreature->GetBatColor();
				
				//---------------------------------------- 
				// 캐릭터 전체 색깔이 바뀌는 경우					
				//---------------------------------------- 
				if (colorSet < MAX_COLORSET)
				{
					m_pSurface->BltIndexSpriteColorSet(&pointTemp, pSprite, colorSet);
				}
				//---------------------------------------- 
				// 정상적인 색깔 출력
				//---------------------------------------- 					
				else
				{				
					int colorSet1 = pCreature->GetBodyColor1();
					int colorSet2 = pCreature->GetBodyColor2();
					
					if( colorSet1 == QUEST_ITEM_COLOR || colorSet1 == UNIQUE_ITEM_COLOR )
						colorSet1 = MItem::GetSpecialColorItemColorset( colorSet1 );
					
					if( colorSet2 == QUEST_ITEM_COLOR || colorSet2 == UNIQUE_ITEM_COLOR )
						colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
					
					CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
					
					int shadowCount = pCreature->GetShadowCount();
					
					//----------------------------------------
					// Shadow출력
					//----------------------------------------
					if (shadowCount)
					{
						bool bDrawShadowFirst = (direction<=4) || creature_type == 764 ||  creature_type == 765;
						if (!bDrawShadowFirst)
						{
							if (pCreature->IsFade())
							{
								m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 1);
							}
							else
							{
								m_pSurface->BltIndexSprite(&pointTemp, pSprite);
							}
						}
						
						// 0, 1, 2, 3, 4, 5
						const int darkbits[6] =
						{
							0, 1, 1, 2, 2, 3
						};
						
						POINT pointTemp2;
						
						// 이동이면 좌표도 바뀐다
						int cxStep = 0, cyStep = 0, cxInc =0 , cyInc = 0;
						if (action==ACTION_MOVE)
						{
							cxInc = -pCreature->GetCX();
							cyInc = -pCreature->GetCY();
						}
						
						
						int faNum = FA.GetSize();
						for (int f=1; f<=shadowCount; f++)
						{
							int frame_f = frame-f;
							
							//---------------------------------------- 
							// f frame 전
							//---------------------------------------- 
							if (frame_f < 0)
							{
								frame_f += faNum;
							}
							
							CFrame &Frame_f = FA[frame_f];
							int sprite_f	= Frame_f.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
							
							CIndexSprite* pSprite_f = &m_CreatureSPK[ sprite_f ];
							
							//									if (pSprite_f->IsNotInit())
							//									{
							//										LoadFromFileCreatureActionSPK( body, action );
							//									}
							
							int cx_f		= Frame_f.GetCX();
							int cy_f		= Frame_f.GetCY();
							
							pointTemp2.x = pPoint->x + cx_f + cxStep;
							pointTemp2.y = pPoint->y + cy_f + cyStep;
							cxStep += cxInc;
							cyStep += cyInc;
							
							m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite_f, darkbits[f]);																		
						}
						
						if (bDrawShadowFirst)
						{
							if (pCreature->IsFade())
							{
								m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 1);
							}
							else
							{
								m_pSurface->BltIndexSprite(&pointTemp, pSprite);
							}
						}
					}
					//----------------------------------------
					// 몸만 찍기
					//----------------------------------------
					else
					{
						POINT pointTemp2;
				
						int iX[8] =  { 15 , 15 , 0 , -15 , -15 , -15  , 0  , 15 } ;
						int iX1[8] = { 30 , 30 , 0 , -30 , -30 , -30  , 0  , 30 } ;

						int iY[8] =  { 0 , -15 , -15  , -15   , 0  , 15   , 15  , 15 } ;
						int iY1[8] = { 0 , -30 , -30  , -30   , 0  , 30   , 30  , 30 } ;


						// 왼쪽
						pointTemp2.x = pointTemp.x + iX[direction];
						pointTemp2.y = pointTemp.y + iY[direction];
						m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 1);
						
						// 오른쪽
						pointTemp2.x = pointTemp.x + iX1[direction];
						pointTemp2.y = pointTemp.y + iY1[direction];
						m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 2);
					}
				}
			}

			if (bCutHeight)
			{
				m_pSurface->SetClipBottom( previousClipBottom );
			}

			// 2004, 10, 28, sobeit add start  - 몬스터 킬 퀘스트 해당 몬스터에 표시.
			if(pCreature->IsAlive() && (g_CurrentFrame&0x04)&&UI_IsMonsterKillQuest_Monster((*g_pCreatureTable)[pCreature->GetCreatureType()].SpriteTypes[0]))
			//if(pCreature->IsAlive() && (g_CurrentFrame&0x04))
			{
				UI_ShowTargetArrow(pPoint->x + 12 ,pPoint->y- pCreature->GetHeight()-20);
			}
		// 2004, 10, 28, sobeit add end


		}
	}
		
	//----------------------------------------------------
	// 관 출력
	//----------------------------------------------------
	if (bInCasket)
	{
		// 그림자 때문에 *2해야한다.
		int casketID = SPRITEID_CASKET_1 + pCreature->GetCasketType()*2;
		
		if (casketID < m_EtcSPK.GetSize())
		{
			CSprite* pSprite = &m_EtcSPK[casketID];
			
			if (pSprite!=NULL)
			{
				// tile의 중심에서 세운다.
				pointTemp.x = pPoint->x + TILE_X_HALF - (pSprite->GetWidth()>>1);
				pointTemp.y = pPoint->y + TILE_Y - pSprite->GetHeight();
				
				//---------------------------------------- 
				// 캐릭터 선택 사각형 영역 설정
				//---------------------------------------- 	
				rect.left	= pointTemp.x;
				rect.top	= pointTemp.y;
				rect.right	= rect.left + pSprite->GetWidth();
				rect.bottom = rect.top + pSprite->GetHeight();
				//if(frameindex==0 )
				pCreature->SetScreenRect( &rect );
				
				if (casketValue==0)
				{
					m_pSurface->BltSprite(&pointTemp, pSprite);						
				}
				else if (casketValue==64)
				{
					CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_SIMPLE_OUTLINE );
					
					m_pSurface->BltSpriteEffect(&pointTemp, pSprite);
				}
				else
				{
					CSpriteSurface::s_Value1 = casketValue;
					
					CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_WIPE_OUT );
					
					m_pSurface->BltSpriteEffect(&pointTemp, pSprite);
				}
			}
		}
	}
}

void	MTopView::DrawAfterACVampire(POINT* pPoint, MCreature* pCreature, int action, int direction, int frame )
{
	CCreatureFramePack& VampireFPK = pCreature->IsMale() ? m_AdvancementVampireManFPK : m_AdvancementVampireWomanFPK;
	CIndexSpritePack& addonISPK = pCreature->IsMale() ? m_AdvancementVampireManSPK : m_AdvancementVampireWomanSPK;

	//------------------------------------------------
	// 관 출력정보
	//------------------------------------------------
	int casketValue = pCreature->GetCasketCount();
	int creature_type = pCreature->GetCreatureType();
	bool bInCasket = pCreature->IsInCasket();
	bool bCasketOnly = (bInCasket && casketValue==0);
	
	
	//action = GetAdvancementVampireActionFromVampireAction(action , pCreature) ; 
	//if( action == -1 )
	//	return;
	//else
	
	//action -= ADVANCEMENT_ACTION_START;
	
	//CCreatureFramePack& advanceVampireFPK = pCreature->IsMale() ? m_AdvancementVampireManFPK : m_AdvancementVampireWomanFPK;
	//CIndexSpritePack& advanceVampireSPK = pCreature->IsMale() ? m_AdvancementVampireManSPK : m_AdvancementVampireWomanSPK;

	
	//----------------------------------------------------
	// 완전히 관만 출력하는 경우는 몸 출력을 안한다.
	//----------------------------------------------------
	if (!bCasketOnly)
	{
		FRAME_ARRAY& FA = VampireFPK[0][action][direction];
		
		if (FA.GetSize() > frame)
		{
			CFrame& Frame =	FA[frame];
			int sprite = Frame.GetSpriteID(),	//m_CreatureFPK[body][action][direction][frame].GetSpriteID(),
				cx		= Frame.GetCX(),	//m_CreatureFPK[body][action][direction][frame].GetCX(),
				cy		= Frame.GetCY();	//m_CreatureFPK[body][action][direction][frame].GetCY();
			
			// 좌표 보정
			pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
			pointTemp.y = pPoint->y + cy;// + pCreature->GetSY();
			
			CIndexSprite* pSprite = &addonISPK[ sprite ];
			
			//---------------------------------------- 
			// 캐릭터 선택 사각형 영역 설정
			//---------------------------------------- 	
			rect.left	= pointTemp.x;
			rect.top	= pointTemp.y;
			rect.right	= rect.left + pSprite->GetWidth();
			rect.bottom = rect.top + pSprite->GetHeight();
			//if(frameindex==0 )
			pCreature->SetScreenRect( &rect );
			//-----------------------------------------------------------
			// CutHeight
			//-----------------------------------------------------------
			bool bCutHeight = pCreature->IsCutHeight();
			int previousClipBottom;
			if (bCutHeight)
			{
				previousClipBottom = m_pSurface->GetClipBottom();
				m_pSurface->SetClipBottom( rect.bottom - TILE_Y_HALF );
				
				pointTemp.y += pCreature->GetCutHeightCount() - TILE_Y_HALF;
			}
			
			//-----------------------------------------------------------
			// 투명 상태 출력 			
			//-----------------------------------------------------------
			// vampire
			if (pCreature->IsFakeDie())
			{
				WORD colorSet = pCreature->GetAttachEffectColor();
				
				if( pCreature->GetCreatureType() == CREATURETYPE_BAT && pCreature->GetBatColor() != 0xFFFF)
					colorSet = pCreature->GetBatColor();
				
				//---------------------------------------- 
				// 캐릭터 전체 색깔이 바뀌는 경우					
				//---------------------------------------- 
				if (colorSet < MAX_COLORSET)
				{
					m_pSurface->BltIndexSpriteColorSet(&pointTemp, pSprite, colorSet);
				}
				else
				{
					int colorSet1 = pCreature->GetBodyColor1();
					int colorSet2 = pCreature->GetBodyColor2();
					
					if( colorSet1 == QUEST_ITEM_COLOR || colorSet1 == UNIQUE_ITEM_COLOR )
						colorSet1 = MItem::GetSpecialColorItemColorset( colorSet1 );
					
					if( colorSet2 == QUEST_ITEM_COLOR || colorSet2 == UNIQUE_ITEM_COLOR )
						colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
					
					CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
					
					int netValue = pCreature->GetFakeDieCount()/5;
					
					CSpriteSurface::s_Value1 = netValue;
					
					CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_NET );
					CIndexSprite::SetEffect( CIndexSprite::EFFECT_NET );
					
					m_pSurface->BltIndexSpriteEffect(&pointTemp, pSprite);
					
				}
			}
			else if (pCreature->IsInvisible())
			{
				WORD colorSet = pCreature->GetAttachEffectColor();
				
				if( pCreature->GetCreatureType() == CREATURETYPE_BAT && pCreature->GetBatColor() != 0xFFFF)
					colorSet = pCreature->GetBatColor();
				
				//---------------------------------------- 
				// 캐릭터 전체 색깔이 바뀌는 경우					
				//---------------------------------------- 
				if (colorSet < MAX_COLORSET)
				{
					// IndexSprite에도 EFFECT...넣고...
					// 스르륵~ 단계적으로 나타나게/없어지게 해야됨.. - -;
					m_pSurface->BltIndexSpriteColorSet(&pointTemp, pSprite, colorSet);
				}
				else
				{
					int colorSet1 = pCreature->GetBodyColor1();
					int colorSet2 = pCreature->GetBodyColor2();
					
					if( colorSet1 == QUEST_ITEM_COLOR || colorSet1 == UNIQUE_ITEM_COLOR )
						colorSet1 = MItem::GetSpecialColorItemColorset( colorSet1 );
					
					if( colorSet2 == QUEST_ITEM_COLOR || colorSet2 == UNIQUE_ITEM_COLOR )
						colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
					
					CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
					
					int wipeValue = pCreature->GetInvisibleCount();
					
					if (wipeValue==0)
					{
						m_pSurface->BltIndexSprite(&pointTemp, pSprite);							
					}
					else if (wipeValue==64)
					{
						CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_SIMPLE_OUTLINE );
						CIndexSprite::SetEffect( CIndexSprite::EFFECT_SIMPLE_OUTLINE );
						
						m_pSurface->BltIndexSpriteEffect(&pointTemp, pSprite);
					}
					else
					{
						CSpriteSurface::s_Value1 = wipeValue;
						
						CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_WIPE_OUT );
						CIndexSprite::SetEffect( CIndexSprite::EFFECT_WIPE_OUT );
						
						m_pSurface->BltIndexSpriteEffect(&pointTemp, pSprite);
					}							
				}
			}					
			else
			{	
				//-----------------------------------------------------------
				// 빠르게 움직이는 중
				//-----------------------------------------------------------
				if (pCreature->IsFastMove() || pCreature->IsBloodyZenith())
				{
					int colorSet1 = pCreature->GetBodyColor1();
					int colorSet2 = pCreature->GetBodyColor2();
					
					if( colorSet1 == QUEST_ITEM_COLOR || colorSet1 == UNIQUE_ITEM_COLOR )
						colorSet1 = MItem::GetSpecialColorItemColorset( colorSet1 );
					
					if( colorSet2 == QUEST_ITEM_COLOR || colorSet2 == UNIQUE_ITEM_COLOR )
						colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
					
					
					CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
					
					POINT pointTemp2;
					
					int fastMoveShadowMax = 2;
					if(pCreature->IsBloodyZenith())
					{
						fastMoveShadowMax = pCreature->GetBloodyZenithCount();
						pCreature->UpdateBloodyZenith();
					}
					for(int fastMoveShadow = fastMoveShadowMax; fastMoveShadow > 0; fastMoveShadow--)
					{
						// 1 pixel skip
						pointTemp2.x = pointTemp.x - (pCreature->GetCX() >> 1)*fastMoveShadow;
						pointTemp2.y = pointTemp.y - (pCreature->GetCY() >> 1)*fastMoveShadow;
						if(pCreature->IsBloodyZenith())
						{
							pointTemp2.y -= sin((double)max(0, pCreature->GetMoveCount()-fastMoveShadow)*3.141592/(double)pCreature->GetMoveCountMax())*100;
						}
						
						m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, min(2, fastMoveShadow));
					}
					
				}
				
				if(pCreature->IsBloodyZenith())
				{
					pointTemp.y -= sin((double)pCreature->GetMoveCount()*3.141592/(double)pCreature->GetMoveCountMax())*100;
				}
				WORD colorSet = pCreature->GetAttachEffectColor();
				
				if( pCreature->GetCreatureType() == CREATURETYPE_BAT && pCreature->GetBatColor() != 0xFFFF)
					colorSet = pCreature->GetBatColor();
				
				//---------------------------------------- 
				// 캐릭터 전체 색깔이 바뀌는 경우					
				//---------------------------------------- 
				if (colorSet < MAX_COLORSET)
				{
					m_pSurface->BltIndexSpriteColorSet(&pointTemp, pSprite, colorSet);
				}
				//---------------------------------------- 
				// 정상적인 색깔 출력
				//---------------------------------------- 					
				else
				{				
					int colorSet1 = pCreature->GetBodyColor1();
					int colorSet2 = pCreature->GetBodyColor2();
					
					if( colorSet1 == QUEST_ITEM_COLOR || colorSet1 == UNIQUE_ITEM_COLOR )
						colorSet1 = MItem::GetSpecialColorItemColorset( colorSet1 );
					
					if( colorSet2 == QUEST_ITEM_COLOR || colorSet2 == UNIQUE_ITEM_COLOR )
						colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
					
					CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
					
					int shadowCount = pCreature->GetShadowCount();
					
					//----------------------------------------
					// Shadow출력
					//----------------------------------------
					if (shadowCount)
					{
						bool bDrawShadowFirst = (direction<=4) || creature_type == 764 ||  creature_type == 765;
						if (!bDrawShadowFirst)
						{
							if (pCreature->IsFade())
							{
								m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 1);
							}
							else
							{
								m_pSurface->BltIndexSprite(&pointTemp, pSprite);
							}
						}
	
						// 0, 1, 2, 3, 4, 5
						const int darkbits[6] =
						{
							0, 1, 1, 2, 2, 3
						};
						
						POINT pointTemp2;
						
						// 이동이면 좌표도 바뀐다
						int cxStep = 0, cyStep = 0, cxInc =0 , cyInc = 0;
						if (action==ACTION_MOVE)
						{
							cxInc = -pCreature->GetCX();
							cyInc = -pCreature->GetCY();
						}
						
						
						int faNum = FA.GetSize();
						for (int f=1; f<=shadowCount; f++)
						{
							int frame_f = frame-f;
							
							//---------------------------------------- 
							// f frame 전
							//---------------------------------------- 
							if (frame_f < 0)
							{
								frame_f += faNum;
							}
							
							CFrame &Frame_f = FA[frame_f];
							int sprite_f	= Frame_f.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
							
							CIndexSprite* pSprite_f = &m_CreatureSPK[ sprite_f ];
							
							int cx_f		= Frame_f.GetCX();
							int cy_f		= Frame_f.GetCY();
							
							pointTemp2.x = pPoint->x + cx_f + cxStep;
							pointTemp2.y = pPoint->y + cy_f + cyStep;
							cxStep += cxInc;
							cyStep += cyInc;
							
							m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite_f, darkbits[f]);																		
	
						}
						
						if (bDrawShadowFirst)
						{
							if (pCreature->IsFade())
							{
								m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 1);
							}
							else
							{
								m_pSurface->BltIndexSprite(&pointTemp, pSprite);
							}
	
						}
					}
					//----------------------------------------
					// 몸만 찍기
					//----------------------------------------
					else
					{
						POINT pointTemp2;
				
						int iX[8] =  { 15 , 15 , 0 , -15 , -15 , -15  , 0  , 15 } ;
						int iX1[8] = { 30 , 30 , 0 , -30 , -30 , -30  , 0  , 30 } ;

						int iY[8] =  { 0 , -15 , -15  , -15   , 0  , 15   , 15  , 15 } ;
						int iY1[8] = { 0 , -30 , -30  , -30   , 0  , 30   , 30  , 30 } ;


						// UP
						pointTemp2.x = pointTemp.x + iX[direction];
						pointTemp2.y = pointTemp.y + iY[direction];
						m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 1);
						
						// DOWN
						pointTemp2.x = pointTemp.x + iX1[direction];
						pointTemp2.y = pointTemp.y + iY1[direction];
						m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 2);
						
					}
				}
			}
	
			if (bCutHeight)
			{
				m_pSurface->SetClipBottom( previousClipBottom );
			}
	
			// 2004, 10, 28, sobeit add start  - 몬스터 킬 퀘스트 해당 몬스터에 표시.
			if(pCreature->IsAlive() && (g_CurrentFrame&0x04)&&UI_IsMonsterKillQuest_Monster((*g_pCreatureTable)[pCreature->GetCreatureType()].SpriteTypes[0]))
			//if(pCreature->IsAlive() && (g_CurrentFrame&0x04))
			{
				UI_ShowTargetArrow(pPoint->x + 12 ,pPoint->y- pCreature->GetHeight()-20);
			}
		// 2004, 10, 28, sobeit add end
		}
	
		bool bWeapon = true;
	
		bool bBat = pCreature->GetCreatureType() == CREATURETYPE_BAT;
		bool bWolf = pCreature->GetCreatureType() == CREATURETYPE_WOLF;
		bool bWerWolf = pCreature->GetCreatureType() == CREATURETYPE_WER_WOLF;
	
		if( bWeapon && !bBat && !bWolf && !bWerWolf )
		{
	
			FRAME_ARRAY& FAWEAPON = VampireFPK[1][action][direction];
			if (FAWEAPON.GetSize() > frame)
			{
				CFrame& Frame =	FAWEAPON[frame];
				int sprite = Frame.GetSpriteID(),	//m_CreatureFPK[body][action][direction][frame].GetSpriteID(),
					cx		= Frame.GetCX(),	//m_CreatureFPK[body][action][direction][frame].GetCX(),
					cy		= Frame.GetCY();	//m_CreatureFPK[body][action][direction][frame].GetCY();
				
				// 좌표 보정
				pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
				pointTemp.y = pPoint->y + cy;// + pCreature->GetSY();
				
				CIndexSprite* pSprite = &addonISPK[ sprite ];
				
				//---------------------------------------- 
				// 캐릭터 선택 사각형 영역 설정
				//---------------------------------------- 	
				rect.left	= pointTemp.x;
				rect.top	= pointTemp.y;
				rect.right	= rect.left + pSprite->GetWidth();
				rect.bottom = rect.top + pSprite->GetHeight();
	//				if(frameindex==0 )
	//					pCreature->SetScreenRect( &rect );
				//-----------------------------------------------------------
				// CutHeight
				//-----------------------------------------------------------
				bool bCutHeight = pCreature->IsCutHeight();
				int previousClipBottom;
				if (bCutHeight)
				{
					previousClipBottom = m_pSurface->GetClipBottom();
					m_pSurface->SetClipBottom( rect.bottom - TILE_Y_HALF );
					
					pointTemp.y += pCreature->GetCutHeightCount() - TILE_Y_HALF;
				}
				
				//-----------------------------------------------------------
				// 투명 상태 출력 			
				//-----------------------------------------------------------
				// vampire
				if (pCreature->IsFakeDie())
				{
					WORD colorSet = pCreature->GetAttachEffectColor();
					
					//---------------------------------------- 
					// 캐릭터 전체 색깔이 바뀌는 경우					
					//---------------------------------------- 
					if (colorSet < MAX_COLORSET)
					{
						m_pSurface->BltIndexSpriteColorSet(&pointTemp, pSprite, colorSet);
					}
					else
					{
						int netValue = pCreature->GetFakeDieCount()/5;
						
						CSpriteSurface::s_Value1 = netValue;
						
						CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_NET );
						CIndexSprite::SetEffect( CIndexSprite::EFFECT_NET );
						
						m_pSurface->BltIndexSpriteEffect(&pointTemp, pSprite);
	
					}
				}
				else if (pCreature->IsInvisible())
				{
					WORD colorSet = pCreature->GetAttachEffectColor();
					
					if( pCreature->GetCreatureType() == CREATURETYPE_BAT && pCreature->GetBatColor() != 0xFFFF)
						colorSet = pCreature->GetBatColor();
					
					//---------------------------------------- 
					// 캐릭터 전체 색깔이 바뀌는 경우					
					//---------------------------------------- 
					if (colorSet < MAX_COLORSET)
					{
						// IndexSprite에도 EFFECT...넣고...
						// 스르륵~ 단계적으로 나타나게/없어지게 해야됨.. - -;
						m_pSurface->BltIndexSpriteColorSet(&pointTemp, pSprite, colorSet);
					}
					else
					{
						int wipeValue = pCreature->GetInvisibleCount();
						
						if (wipeValue==0)
						{
							m_pSurface->BltIndexSprite(&pointTemp, pSprite);							
						}
						else if (wipeValue==64)
						{
							CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_SIMPLE_OUTLINE );
							CIndexSprite::SetEffect( CIndexSprite::EFFECT_SIMPLE_OUTLINE );
							
							m_pSurface->BltIndexSpriteEffect(&pointTemp, pSprite);
						}
						else
						{
							CSpriteSurface::s_Value1 = wipeValue;
							
							CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_WIPE_OUT );
							CIndexSprite::SetEffect( CIndexSprite::EFFECT_WIPE_OUT );
							
							m_pSurface->BltIndexSpriteEffect(&pointTemp, pSprite);
						}
					}
				}					
				else
				{	
					//-----------------------------------------------------------
					// 빠르게 움직이는 중
					//-----------------------------------------------------------
					if (pCreature->IsFastMove() || pCreature->IsBloodyZenith())
					{
						POINT pointTemp2;
						
						int fastMoveShadowMax = 2;
						if(pCreature->IsBloodyZenith())
						{
							fastMoveShadowMax = pCreature->GetBloodyZenithCount();
							pCreature->UpdateBloodyZenith();
						}
						for(int fastMoveShadow = fastMoveShadowMax; fastMoveShadow > 0; fastMoveShadow--)
						{
							// 1 pixel skip
							pointTemp2.x = pointTemp.x - (pCreature->GetCX() >> 1)*fastMoveShadow;
							pointTemp2.y = pointTemp.y - (pCreature->GetCY() >> 1)*fastMoveShadow;
							if(pCreature->IsBloodyZenith())
							{
								pointTemp2.y -= sin((double)max(0, pCreature->GetMoveCount()-fastMoveShadow)*3.141592/(double)pCreature->GetMoveCountMax())*100;
							}
							
							m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, min(2, fastMoveShadow));
						}
						
					}
					
					if(pCreature->IsBloodyZenith())
					{
						pointTemp.y -= sin((double)pCreature->GetMoveCount()*3.141592/(double)pCreature->GetMoveCountMax())*100;
					}
					WORD colorSet = pCreature->GetAttachEffectColor();
					
					//---------------------------------------- 
					// 캐릭터 전체 색깔이 바뀌는 경우					
					//---------------------------------------- 
					if (colorSet < MAX_COLORSET)
					{
						m_pSurface->BltIndexSpriteColorSet(&pointTemp, pSprite, colorSet);
					}
					//---------------------------------------- 
					// 정상적인 색깔 출력
					//---------------------------------------- 					
					else
					{				
						int shadowCount = pCreature->GetShadowCount();
						
						//----------------------------------------
						// Shadow출력
						//----------------------------------------
						if (shadowCount)
						{
							bool bDrawShadowFirst = (direction<=4) || creature_type == 764 ||  creature_type == 765;
							if (!bDrawShadowFirst)
							{
								if (pCreature->IsFade())
								{
									m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 1);
								}
								else
								{
									m_pSurface->BltIndexSprite(&pointTemp, pSprite);
								}
							}
							
							// 0, 1, 2, 3, 4, 5
							const int darkbits[6] =
							{
								0, 1, 1, 2, 2, 3
							};
							
							POINT pointTemp2;
							
							// 이동이면 좌표도 바뀐다
							int cxStep = 0, cyStep = 0, cxInc =0 , cyInc = 0;
							if (action==ACTION_MOVE)
							{
								cxInc = -pCreature->GetCX();
								cyInc = -pCreature->GetCY();
							}
							
							
							int faNum = FA.GetSize();
							for (int f=1; f<=shadowCount; f++)
							{
								int frame_f = frame-f;
								
								//---------------------------------------- 
								// f frame 전
								//---------------------------------------- 
								if (frame_f < 0)
								{
									frame_f += faNum;
								}
								
								CFrame &Frame_f = FA[frame_f];
								int sprite_f	= Frame_f.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
								
								CIndexSprite* pSprite_f = &m_CreatureSPK[ sprite_f ];
								
								int cx_f		= Frame_f.GetCX();
								int cy_f		= Frame_f.GetCY();
								
								pointTemp2.x = pPoint->x + cx_f + cxStep;
								pointTemp2.y = pPoint->y + cy_f + cyStep;
								cxStep += cxInc;
								cyStep += cyInc;
								
								m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite_f, darkbits[f]);																		
							}
							
							if (bDrawShadowFirst)
							{
								if (pCreature->IsFade())
								{
									m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 1);
								}
								else
								{
									m_pSurface->BltIndexSprite(&pointTemp, pSprite);
								}
							}
						}
						//----------------------------------------
						// 몸만 찍기
						//----------------------------------------
						else
						{
							POINT pointTemp2;
				
							int iX[8] =  { 15 , 15 , 0 , -15 , -15 , -15  , 0  , 15 } ;
							int iX1[8] = { 30 , 30 , 0 , -30 , -30 , -30  , 0  , 30 } ;

							int iY[8] =  { 0 , -15 , -15  , -15   , 0  , 15   , 15  , 15 } ;
							int iY1[8] = { 0 , -30 , -30  , -30   , 0  , 30   , 30  , 30 } ;


							// UP
							pointTemp2.x = pointTemp.x + iX[direction];
							pointTemp2.y = pointTemp.y + iY[direction];
							m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 1);
							
							// DOWN
							pointTemp2.x = pointTemp.x + iX1[direction];
							pointTemp2.y = pointTemp.y + iY1[direction];
							m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 2);
						}
					}
				}
			}
		}
	}
		
	//----------------------------------------------------
	// 관 출력
	//----------------------------------------------------
	if (bInCasket)
	{
		// 그림자 때문에 *2해야한다.
		int casketID = SPRITEID_CASKET_1 + pCreature->GetCasketType()*2;
		
		if (casketID < m_EtcSPK.GetSize())
		{
			CSprite* pSprite = &m_EtcSPK[casketID];
			
			if (pSprite!=NULL)
			{
				// tile의 중심에서 세운다.
				pointTemp.x = pPoint->x + TILE_X_HALF - (pSprite->GetWidth()>>1);
				pointTemp.y = pPoint->y + TILE_Y - pSprite->GetHeight();
				
				//---------------------------------------- 
				// 캐릭터 선택 사각형 영역 설정
				//---------------------------------------- 	
				rect.left	= pointTemp.x;
				rect.top	= pointTemp.y;
				rect.right	= rect.left + pSprite->GetWidth();
				rect.bottom = rect.top + pSprite->GetHeight();
				//if(frameindex==0 )
				pCreature->SetScreenRect( &rect );
				
				if (casketValue==0)
				{
					m_pSurface->BltSprite(&pointTemp, pSprite);						
				}
				else if (casketValue==64)
				{
					CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_SIMPLE_OUTLINE );
					
					m_pSurface->BltSpriteEffect(&pointTemp, pSprite);
				}
				else
				{
					CSpriteSurface::s_Value1 = casketValue;
					
					CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_WIPE_OUT );
					
					m_pSurface->BltSpriteEffect(&pointTemp, pSprite);
				}
			}
		}
	}

}

void	MTopView::DrawAfterOusters(POINT* pPoint, MCreature* pCreature, int action, int direction, int frame )
{
	
	//  챠크람이 있다
	MCreatureWear *pCreatureWear = (MCreatureWear *)pCreature;
	
	const MCreatureWear::ADDON_INFO& addonInfoChakram = pCreatureWear->GetAddonInfo(ADDON_RIGHTHAND);
	bool bChakram = addonInfoChakram.bAddon && addonInfoChakram.ItemClass == ITEM_CLASS_OUSTERS_CHAKRAM;
	
	int tempAction = action;
	if(bChakram == false)
	{
		switch(tempAction)
		{
		case ACTION_STAND:
			tempAction = ACTION_OUSTERS_STAND;
			break;
			
		case ACTION_MOVE:
			tempAction = ACTION_OUSTERS_MOVE;
			break;
		}
	}
	
	// 몸을 찍고 챠크람을 찍는다
	const MCreatureWear::ADDON_INFO& addonInfo = pCreatureWear->GetAddonInfo(ADDON_COAT);
	const MCreatureWear::ADDON_INFO& bootsAddonInfo = pCreatureWear->GetAddonInfo(ADDON_TROUSER);
	

	if (addonInfo.bAddon && !pCreatureWear->IsGhost(1))
	{
		//				DEBUG_ADD("[DrawCreature] DrawCloth");
		int clothes = addonInfo.FrameID;
		
		
		FRAME_ARRAY &FA = m_OustersFPK[clothes][tempAction][direction];
		
		// 있는 동작인 경우
		if (FA.GetSize() > frame)
		{
			CFrame &Frame = FA[frame];					
			int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
			int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
			int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
			
			CIndexSprite* pSprite = &m_OustersSPK[ sprite ];					
			
			pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
			pointTemp.y = pPoint->y + cy;// + pCreature->GetSY();
			
			//---------------------------------------- 
			// 캐릭터 선택 사각형 영역 설정
			//---------------------------------------- 	
			rect.left	= pointTemp.x;
			rect.top	= pointTemp.y;
			rect.right	= rect.left + pSprite->GetWidth();
			rect.bottom = rect.top + pSprite->GetHeight();
			pCreature->AddScreenRect( &rect );
			
			//---------------------------------------- 
			// 캐릭터 부분 색깔이 바뀌는 경우
			//---------------------------------------- 
			if (addonInfo.bEffectColor)	//colorSet < MAX_COLORSET)
			{
				m_pSurface->BltIndexSpriteColorSet(&pointTemp, pSprite, 
					addonInfo.EffectColorSet);
			}
			//---------------------------------------- 
			// 정상적인 캐릭터 출력
			//---------------------------------------- 
			else
			{
				CIndexSprite::SetUsingColorSetOnly( 0, pCreature->GetBodyColor1() );	// 머리색
				
				int Colorset;
				
				Colorset = addonInfo.ColorSet2;
				
				if( Colorset == QUEST_ITEM_COLOR || Colorset == UNIQUE_ITEM_COLOR )
					Colorset = MItem::GetSpecialColorItemColorset( Colorset );
				CIndexSprite::SetUsingColorSetOnly( 1, Colorset );	// 옷색
				
				if(bChakram == false)	// 리스틀릿을 차고 있으면
				{
					if(addonInfoChakram.bAddon)
					{
						Colorset = addonInfoChakram.ColorSet2;
						if (Colorset == QUEST_ITEM_COLOR||Colorset == UNIQUE_ITEM_COLOR )
							Colorset = MItem::GetSpecialColorItemColorset( Colorset );
						
						CIndexSprite::SetUsingColorSetOnly( 2, Colorset );	// 팔
					}
					else
					{
						Colorset = defaultCoatColor;
						if (Colorset == QUEST_ITEM_COLOR||Colorset == UNIQUE_ITEM_COLOR )
							Colorset = MItem::GetSpecialColorItemColorset( Colorset );
						
						CIndexSprite::SetUsingColorSetOnly( 2, Colorset );	// 팔
					}
				}
				else
				{
					Colorset = defaultCoatColor;
					if(addonInfoChakram.bAddon)
						Colorset = addonInfoChakram.ColorSet2;
					if (Colorset == QUEST_ITEM_COLOR||Colorset == UNIQUE_ITEM_COLOR )
						Colorset = MItem::GetSpecialColorItemColorset( Colorset );
					
					CIndexSprite::SetUsingColorSetOnly( 2, Colorset );	// 팔
					
				}
				
				if(bootsAddonInfo.bAddon)
				{
					Colorset = bootsAddonInfo.ColorSet2;
					if (Colorset == QUEST_ITEM_COLOR||Colorset == UNIQUE_ITEM_COLOR )
						Colorset = MItem::GetSpecialColorItemColorset( Colorset );
					
					CIndexSprite::SetUsingColorSetOnly( 3, Colorset );	// 부츠색
				}
				else
				{
					Colorset = defaultCoatColor ;
					if (Colorset == QUEST_ITEM_COLOR||Colorset == UNIQUE_ITEM_COLOR )
						Colorset = MItem::GetSpecialColorItemColorset( Colorset );
					
					CIndexSprite::SetUsingColorSetOnly( 3, Colorset );	// 팔
				}
				
				POINT pointTemp2;
				
				int iX[8] =  { 15 , 15 , 0 , -15 , -15 , -15  , 0  , 15 } ;
				int iX1[8] = { 30 , 30 , 0 , -30 , -30 , -30  , 0  , 30 } ;

				int iY[8] =  { 0 , -15 , -15  , -15   , 0  , 15   , 15  , 15 } ;
				int iY1[8] = { 0 , -30 , -30  , -30   , 0  , 30   , 30  , 30 } ;


				// UP
				pointTemp2.x = pointTemp.x + iX[direction];
				pointTemp2.y = pointTemp.y + iY[direction];
				m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 1);
				
				// DOWN
				pointTemp2.x = pointTemp.x + iX1[direction];
				pointTemp2.y = pointTemp.y + iY1[direction];
				m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 2);
			}			
		}
	}
	
	if (bChakram && !pCreatureWear->IsGhost(2))
	{
		//				DEBUG_ADD("[DrawCreature] DrawChakram");
		int clothes = addonInfoChakram.FrameID;
		
		FRAME_ARRAY &FA = m_OustersFPK[clothes][tempAction][direction];
		
		// 있는 동작인 경우
		if (FA.GetSize() > frame)
		{
			//					DEBUG_ADD_FORMAT("[DrawCreature] FA.GetSize() > frame %d %d", FA.GetSize(), frame);
			CFrame &Frame = FA[frame];					
			int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
			int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
			int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
			
			CIndexSprite* pSprite = &m_OustersSPK[ sprite ];					
			
			pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
			pointTemp.y = pPoint->y + cy;// + pCreature->GetSY();
			
			//---------------------------------------- 
			// 캐릭터 부분 색깔이 바뀌는 경우
			//---------------------------------------- 
			if (addonInfoChakram.bEffectColor)	//colorSet < MAX_COLORSET)
			{
				m_pSurface->BltIndexSpriteColorSet(&pointTemp, pSprite, 
					addonInfoChakram.EffectColorSet);
			}
			//---------------------------------------- 
			// 정상적인 캐릭터 출력
			//---------------------------------------- 
			else
			{
				int colorset1, colorset2;
				colorset1 = addonInfoChakram.ColorSet1;
				colorset2 = addonInfoChakram.ColorSet2;
				
				if( colorset1 == QUEST_ITEM_COLOR || colorset1 == UNIQUE_ITEM_COLOR )
					colorset1 = MItem::GetSpecialColorItemColorset( colorset1 );
				if( colorset2 == QUEST_ITEM_COLOR || colorset2 == UNIQUE_ITEM_COLOR )
					colorset2 = MItem::GetSpecialColorItemColorset( colorset2 );
				CIndexSprite::SetUsingColorSet( colorset1, colorset2 );
			
				POINT pointTemp2;
				
				int iX[8] =  { 15 , 15 , 0 , -15 , -15 , -15  , 0  , 15 } ;
				int iX1[8] = { 30 , 30 , 0 , -30 , -30 , -30  , 0  , 30 } ;

				int iY[8] =  { 0 , -15 , -15  , -15   , 0  , 15   , 15  , 15 } ;
				int iY1[8] = { 0 , -30 , -30  , -30   , 0  , 30   , 30  , 30 } ;


				// UP
				pointTemp2.x = pointTemp.x + iX[direction];
				pointTemp2.y = pointTemp.y + iY[direction];
				m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 1);
				
				// DOWN
				pointTemp2.x = pointTemp.x + iX1[direction];
				pointTemp2.y = pointTemp.y + iY1[direction];
				m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 2);

			}						
			
		}
	}
}

void	MTopView::DrawAfterACOusters(POINT* pPoint, MCreature* pCreature, int action, int direction, int frame )
{
	MCreatureWear*	pCreatureWear = (MCreatureWear*)pCreature;

	CCreatureFramePack& OustersFPK = m_AdvancementOustersFPK ;
	CIndexSpritePack&   addonISPK  = m_AdvancementOustersSPK ;

	const MCreatureWear::ADDON_INFO& addonInfoChakram = pCreatureWear->GetAddonInfo(ADDON_RIGHTHAND);
	bool bChakram = addonInfoChakram.bAddon && addonInfoChakram.ItemClass == ITEM_CLASS_OUSTERS_CHAKRAM;
	
	int tempAction = action;
	if(bChakram == false)
	{
		switch(tempAction)
		{
		case ACTION_STAND:
			tempAction = ACTION_OUSTERS_STAND;
			break;
			
		case ACTION_MOVE:
			tempAction = ACTION_OUSTERS_MOVE;
			break;
		}
	}
	
	// 몸을 찍고 챠크람을 찍는다
	const MCreatureWear::ADDON_INFO& addonInfo = pCreatureWear->GetAddonInfo(ADDON_COAT);
	const MCreatureWear::ADDON_INFO& bootsAddonInfo = pCreatureWear->GetAddonInfo(ADDON_TROUSER);

	if (addonInfo.bAddon && !pCreatureWear->IsGhost(1))
	{
		//				DEBUG_ADD("[DrawCreature] DrawCloth");
		int clothes = addonInfo.FrameID;
		
		
		FRAME_ARRAY &FA = OustersFPK[clothes][tempAction][direction];
		
		// 있는 동작인 경우
		if (FA.GetSize() > frame)
		{
			CFrame &Frame = FA[frame];					
			int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
			int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
			int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
			
			CIndexSprite* pSprite = &addonISPK[ sprite ];					
			
			pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
			pointTemp.y = pPoint->y + cy;// + pCreature->GetSY();
			
			//---------------------------------------- 
			// 캐릭터 선택 사각형 영역 설정
			//---------------------------------------- 	
			rect.left	= pointTemp.x;
			rect.top	= pointTemp.y;
			rect.right	= rect.left + pSprite->GetWidth();
			rect.bottom = rect.top + pSprite->GetHeight();
			pCreature->AddScreenRect( &rect );
			
			//---------------------------------------- 
			// 캐릭터 부분 색깔이 바뀌는 경우
			//---------------------------------------- 
			if (addonInfo.bEffectColor)	//colorSet < MAX_COLORSET)
			{
				m_pSurface->BltIndexSpriteColorSet(&pointTemp, pSprite, 
					addonInfo.EffectColorSet);
			}
			//---------------------------------------- 
			// 정상적인 캐릭터 출력
			//---------------------------------------- 
			else
			{
				CIndexSprite::SetUsingColorSetOnly( 0, pCreature->GetBodyColor1() );	// 머리색
				
				int Colorset;
				
				Colorset = addonInfo.ColorSet2;
				
				if( Colorset == QUEST_ITEM_COLOR || Colorset == UNIQUE_ITEM_COLOR )
					Colorset = MItem::GetSpecialColorItemColorset( Colorset );
				CIndexSprite::SetUsingColorSetOnly( 1, Colorset );	// 옷색
				
				if(bChakram == false)	// 리스틀릿을 차고 있으면
				{
					if(addonInfoChakram.bAddon)
					{
						Colorset = addonInfoChakram.ColorSet2;
						if (Colorset == QUEST_ITEM_COLOR||Colorset == UNIQUE_ITEM_COLOR )
							Colorset = MItem::GetSpecialColorItemColorset( Colorset );
						
						CIndexSprite::SetUsingColorSetOnly( 2, Colorset );	// 팔
					}
					else
					{
						Colorset = defaultCoatColor;
						if (Colorset == QUEST_ITEM_COLOR||Colorset == UNIQUE_ITEM_COLOR )
							Colorset = MItem::GetSpecialColorItemColorset( Colorset );
						
						CIndexSprite::SetUsingColorSetOnly( 2, Colorset );	// 팔
					}
				}
				else
				{
					Colorset = defaultCoatColor;
					if(addonInfoChakram.bAddon)
						Colorset = addonInfoChakram.ColorSet2;
					if (Colorset == QUEST_ITEM_COLOR||Colorset == UNIQUE_ITEM_COLOR )
						Colorset = MItem::GetSpecialColorItemColorset( Colorset );
					
					CIndexSprite::SetUsingColorSetOnly( 2, Colorset );	// 팔
					
				}
				
				if(bootsAddonInfo.bAddon)
				{
					Colorset = bootsAddonInfo.ColorSet2;
					if (Colorset == QUEST_ITEM_COLOR||Colorset == UNIQUE_ITEM_COLOR )
						Colorset = MItem::GetSpecialColorItemColorset( Colorset );
					
					CIndexSprite::SetUsingColorSetOnly( 3, Colorset );	// 부츠색
				}
				else
				{
					Colorset = defaultCoatColor ;
					if (Colorset == QUEST_ITEM_COLOR||Colorset == UNIQUE_ITEM_COLOR )
						Colorset = MItem::GetSpecialColorItemColorset( Colorset );
					
					CIndexSprite::SetUsingColorSetOnly( 3, Colorset );	// 팔
				}
				
				POINT pointTemp2;
				int iX[8] =  { 15 , 15 , 0 , -15 , -15 , -15  , 0  , 15 } ;
				int iX1[8] = { 30 , 30 , 0 , -30 , -30 , -30  , 0  , 30 } ;

				int iY[8] =  { 0 , -15 , -15  , -15   , 0  , 15   , 15  , 15 } ;
				int iY1[8] = { 0 , -30 , -30  , -30   , 0  , 30   , 30  , 30 } ;

				

				// 왼쪽
				pointTemp2.x = pointTemp.x + iX[direction];
				pointTemp2.y = pointTemp.y + iY[direction];
				m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 1);
				
				// 오른쪽
				pointTemp2.x = pointTemp.x + iX1[direction];
				pointTemp2.y = pointTemp.y + iY1[direction];
				m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 2);
			}			
		}
	}
	
	if (bChakram && !pCreatureWear->IsGhost(2))
	{
		//				DEBUG_ADD("[DrawCreature] DrawChakram");
		int clothes = addonInfoChakram.FrameID;
		
		FRAME_ARRAY &FA = OustersFPK[clothes][tempAction][direction];
		
		// 있는 동작인 경우
		if (FA.GetSize() > frame)
		{
			//					DEBUG_ADD_FORMAT("[DrawCreature] FA.GetSize() > frame %d %d", FA.GetSize(), frame);
			CFrame &Frame = FA[frame];					
			int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
			int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
			int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
			
			CIndexSprite* pSprite = &addonISPK[ sprite ];					
			
			pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
			pointTemp.y = pPoint->y + cy;// + pCreature->GetSY();
			
			//---------------------------------------- 
			// 캐릭터 부분 색깔이 바뀌는 경우
			//---------------------------------------- 
			if (addonInfoChakram.bEffectColor)	//colorSet < MAX_COLORSET)
			{
				m_pSurface->BltIndexSpriteColorSet(&pointTemp, pSprite, 
					addonInfoChakram.EffectColorSet);
			}
			//---------------------------------------- 
			// 정상적인 캐릭터 출력
			//---------------------------------------- 
			else
			{
				int colorset1, colorset2;
				colorset1 = addonInfoChakram.ColorSet1;
				colorset2 = addonInfoChakram.ColorSet2;
				
				if( colorset1 == QUEST_ITEM_COLOR || colorset1 == UNIQUE_ITEM_COLOR )
					colorset1 = MItem::GetSpecialColorItemColorset( colorset1 );
				if( colorset2 == QUEST_ITEM_COLOR || colorset2 == UNIQUE_ITEM_COLOR )
					colorset2 = MItem::GetSpecialColorItemColorset( colorset2 );
				CIndexSprite::SetUsingColorSet( colorset1, colorset2 );

				POINT pointTemp2;
				int iX[8] =  { 15 , 15 , 0 , -15 , -15 , -15  , 0  , 15 } ;
				int iX1[8] = { 30 , 30 , 0 , -30 , -30 , -30  , 0  , 30 } ;

				int iY[8] =  { 0 , -15 , -15  , -15   , 0  , 15   , 15  , 15 } ;
				int iY1[8] = { 0 , -30 , -30  , -30   , 0  , 30   , 30  , 30 } ;


				// 왼쪽
				pointTemp2.x = pointTemp.x + iX[direction];
				pointTemp2.y = pointTemp.y + iY[direction];
				m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 1);
				
				// 오른쪽
				pointTemp2.x = pointTemp.x + iX1[direction];
				pointTemp2.y = pointTemp.y + iY1[direction];
				m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 2);
			}						
			
		}

	}
}


void	MTopView::DrawFastMove(POINT* pPoint, MCreature* pCreature, int action, int direction, int frame )
{
	WORD clothes;
	BYTE clothesType;
	MCreatureWear*	pCreatureWear = (MCreatureWear*)pCreature;
	
	for (int i=0; i<ADDON_MAX; i++)
	{
		// Creature의 현재 방향에 따라서...
		// 옷을 출력해주는 순서가 다를 수 있다.
		clothesType = MCreatureWear::s_AddonOrder[pCreature->GetDirection()][i];
		
		// i번째 종류의 옷을 입고 있다면 출력해 준다.
		const MCreatureWear::ADDON_INFO& addonInfo = pCreatureWear->GetAddonInfo(clothesType);
		
		if (addonInfo.bAddon)
		{
			clothes = addonInfo.FrameID;
			
			FRAME_ARRAY &FA = m_AddonFPK[clothes][action][direction];
			
			// 있는 동작인 경우
			if (FA.GetSize() > frame)
			{
				CFrame &Frame = FA[frame];					
				int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
				int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
				int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
				
				
				CIndexSprite* pSprite = &m_AddonSPK[ sprite ];					
				
				
				pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
				pointTemp.y = pPoint->y + cy;// + pCreature->GetSY();
				
				//---------------------------------------- 
				// 캐릭터 선택 사각형 영역 설정
				//---------------------------------------- 	
				rect.left	= pointTemp.x;
				rect.top	= pointTemp.y;
				rect.right	= rect.left + pSprite->GetWidth();
				rect.bottom = rect.top + pSprite->GetHeight();
				pCreature->AddScreenRect( &rect );
				
				int colorSet1 = addonInfo.ColorSet1, colorSet2 = addonInfo.ColorSet2;
				if(colorSet2 == UNIQUE_ITEM_COLOR || colorSet2 == QUEST_ITEM_COLOR)
				{
					//								// 유니크 아이템이면
					//								if(colorSet2 == QUEST_ITEM_COLOR)
					//									colorSet2 = MItem::GetQuestItemColorset();
					//								else
					//									colorSet2 = MItem::GetUniqueItemColorset();									
					colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
					if(addonInfo.ItemClass != ITEM_CLASS_COAT && addonInfo.ItemClass != ITEM_CLASS_TROUSER)
						colorSet1 = colorSet2;
				}
				
				CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
				
				//-----------------------------------------------------------
				// 빠르게 움직이는 중
				//-----------------------------------------------------------
				POINT pointTemp2;
				
				//CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_NET );
				//CIndexSprite::SetEffect( CIndexSprite::EFFECT_NET );
				
				// 2 pixel skip
				pointTemp2.x = pointTemp.x - pCreature->GetCX();
				pointTemp2.y = pointTemp.y - pCreature->GetCY();
				
				//CSpriteSurface::s_Value1 = 3;
				//m_pSurface->BltIndexSpriteEffect(&pointTemp2, pSprite);
				m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 2);
				
				// 1 pixel skip
				pointTemp2.x = pointTemp.x - (pCreature->GetCX() >> 1);
				pointTemp2.y = pointTemp.y - (pCreature->GetCY() >> 1);
				
				//CSpriteSurface::s_Value1 = 1;
				//m_pSurface->BltIndexSpriteEffect(&pointTemp2, pSprite);
				m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 1);
				
				//---------------------------------------- 
				// 캐릭터 부분 색깔이 바뀌는 경우
				//---------------------------------------- 
				//if (addonInfo.bEffectColor)	//colorSet < MAX_COLORSET)
				//{
				//	m_pSurface->BltIndexSpriteColorSet(&pointTemp, pSprite, 
				//									addonInfo.EffectColorSet);
				//}
				//---------------------------------------- 
				// 정상적인 캐릭터 출력
				//---------------------------------------- 
				//else
				{								
					// [새기술] - fake는 fast move일때 무조건 어둡게..
					if (pCreature->IsFakeCreature())
					{
						MFakeCreature *pFakeCreature = dynamic_cast<MFakeCreature*>(pCreature);
						if(pFakeCreature->GetFakeCreatureType() == MFakeCreature::FAKE_CREATURE_BRIGHTNESS )
							m_pSurface->BltIndexSpriteBrightness(&pointTemp, pSprite, 32);
						else
							m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 1);
					}
					else
					{
						m_pSurface->BltIndexSprite(&pointTemp, pSprite);							
					}						
					
					// 정상적인 출력
					//CIndexSprite::SetUsingColorSet( addonInfo.ColorSet1, addonInfo.ColorSet2 );
					//m_pSurface->BltIndexSprite(&pointTemp, pSprite);
				}
				
			}
		}
	}
}

void	MTopView::DrawFastMoveForACSlayer(POINT* pPoint, MCreature* pCreature, int action, int direction, int frame )
{
	int clothes;
	BYTE clothesType;
	MCreatureWear*	pCreatureWear = (MCreatureWear*)pCreature;
/*	
	action = ConvAdvancementSlayerActionFromSlayerAction( action, dynamic_cast< MCreatureWear* >(pCreature) );

	if( action == -1 )
		return;
	else
		action -= ADVANCEMENT_ACTION_START;
*/
	CCreatureFramePack& slayerFPK = pCreature->IsMale() ? m_AdvancementSlayerManFPK : m_AdvancementSlayerWomanFPK;
	CIndexSpritePack& addonISPK = pCreature->IsMale() ? m_AdvancementSlayerManSPK : m_AdvancementSlayerWomanSPK;


	//-----------------------------------------------------------
	// 좌우 잔상
	//-----------------------------------------------------------
	for (int i=0; i<AC_ADDON_MAX; i++)
	{
		// Creature의 현재 방향에 따라서...
		// 옷을 출력해주는 순서가 다를 수 있다.
		clothesType = MCreatureWear::s_ACAddonOrder[ direction ][ i ];
		
		// i번째 종류의 옷을 입고 있다면 출력해 준다.
		const MCreatureWear::ADDON_INFO& addonInfo = pCreatureWear->GetACAddonInfo(clothesType);
		
		if (addonInfo.bAddon || i == AC_ADDON_BODY )
		{
			clothes = GetAdvancementPartFromItemClass( addonInfo.ItemClass, addonInfo.FrameID );

			if( clothes == -1 )
				continue;
			
			FRAME_ARRAY &FA = slayerFPK[clothes][action][direction];
			
			// 있는 동작인 경우
			if (FA.GetSize() > frame)
			{
				CFrame &Frame = FA[frame];					
				int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
				int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
				int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
				
				
				CIndexSprite* pSprite = &addonISPK[ sprite ];					
				
				
				pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
				pointTemp.y = pPoint->y + cy;// + pCreature->GetSY();
				
				//---------------------------------------- 
				// 캐릭터 선택 사각형 영역 설정
				//---------------------------------------- 	
				rect.left	= pointTemp.x;
				rect.top	= pointTemp.y;
				rect.right	= rect.left + pSprite->GetWidth();
				rect.bottom = rect.top + pSprite->GetHeight();
				pCreature->AddScreenRect( &rect );
				
				int colorSet1 = addonInfo.ColorSet1, colorSet2 = addonInfo.ColorSet2;
				if(colorSet2 == UNIQUE_ITEM_COLOR || colorSet2 == QUEST_ITEM_COLOR)
				{
					//								// 유니크 아이템이면
					//								if(colorSet2 == QUEST_ITEM_COLOR)
					//									colorSet2 = MItem::GetQuestItemColorset();
					//								else
					//									colorSet2 = MItem::GetUniqueItemColorset();									
					colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
					if(addonInfo.ItemClass != ITEM_CLASS_COAT && addonInfo.ItemClass != ITEM_CLASS_TROUSER)
						colorSet1 = colorSet2;
				}
				
				CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
				
				//-----------------------------------------------------------
				// 빠르게 움직이는 중
				//-----------------------------------------------------------
				POINT pointTemp2;
				
				//CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_NET );
				//CIndexSprite::SetEffect( CIndexSprite::EFFECT_NET );
				
				// 2 pixel skip
				pointTemp2.x = pointTemp.x - pCreature->GetCX();
				pointTemp2.y = pointTemp.y - pCreature->GetCY();
				
				//CSpriteSurface::s_Value1 = 3;
				//m_pSurface->BltIndexSpriteEffect(&pointTemp2, pSprite);
				m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 2);
				
				// 1 pixel skip
				pointTemp2.x = pointTemp.x - (pCreature->GetCX() >> 1);
				pointTemp2.y = pointTemp.y - (pCreature->GetCY() >> 1);
				
				//CSpriteSurface::s_Value1 = 1;
				//m_pSurface->BltIndexSpriteEffect(&pointTemp2, pSprite);
				m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, 1);
				
				//---------------------------------------- 
				// 캐릭터 부분 색깔이 바뀌는 경우
				//---------------------------------------- 
				//if (addonInfo.bEffectColor)	//colorSet < MAX_COLORSET)
				//{
				//	m_pSurface->BltIndexSpriteColorSet(&pointTemp, pSprite, 
				//									addonInfo.EffectColorSet);
				//}
				//---------------------------------------- 
				// 정상적인 캐릭터 출력
				//---------------------------------------- 
				//else
				{								
					// [새기술] - fake는 fast move일때 무조건 어둡게..
					if (pCreature->IsFakeCreature())
					{
						MFakeCreature *pFakeCreature = dynamic_cast<MFakeCreature*>(pCreature);
						if(pFakeCreature->GetFakeCreatureType() == MFakeCreature::FAKE_CREATURE_BRIGHTNESS )
							m_pSurface->BltIndexSpriteBrightness(&pointTemp, pSprite, 32);
						else
							m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 1);
					}
					else
					{
						m_pSurface->BltIndexSprite(&pointTemp, pSprite);							
					}						
					
					// 정상적인 출력
					//CIndexSprite::SetUsingColorSet( addonInfo.ColorSet1, addonInfo.ColorSet2 );
					//m_pSurface->BltIndexSprite(&pointTemp, pSprite);
				}
				
			}
		}
	}
}

void	MTopView::DrawInvisible(POINT* pPoint, MCreature* pCreature, int action, int direction, int frame )
{
	WORD clothes;
	BYTE clothesType;
	MCreatureWear*	pCreatureWear = (MCreatureWear*)pCreature;
		
	for (int i=0; i<ADDON_MAX; i++)
	{
		// Creature의 현재 방향에 따라서...
		// 옷을 출력해주는 순서가 다를 수 있다.
		clothesType = MCreatureWear::s_AddonOrder[pCreature->GetDirection()][i];
		
		// i번째 종류의 옷을 입고 있다면 출력해 준다.
		const MCreatureWear::ADDON_INFO& addonInfo = pCreatureWear->GetAddonInfo(clothesType);
		
		if (addonInfo.bAddon)
		{
			clothes = addonInfo.FrameID;
	
			FRAME_ARRAY &FA = m_AddonFPK[clothes][action][direction];
			
			// 있는 동작인 경우
			if (FA.GetSize() > frame)
			{
				CFrame &Frame = FA[frame];					
				int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
				int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
				int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
				
				
				CIndexSprite* pSprite = &m_AddonSPK[ sprite ];					
				
				
				pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
				pointTemp.y = pPoint->y + cy;// + pCreature->GetSY();
				
				//---------------------------------------- 
				// 캐릭터 선택 사각형 영역 설정
				//---------------------------------------- 	
				rect.left	= pointTemp.x;
				rect.top	= pointTemp.y;
				rect.right	= rect.left + pSprite->GetWidth();
				rect.bottom = rect.top + pSprite->GetHeight();
				pCreature->AddScreenRect( &rect );
				
				//---------------------------------------- 
				// 캐릭터 부분 색깔이 바뀌는 경우
				//---------------------------------------- 
				//if (addonInfo.bEffectColor)	//colorSet < MAX_COLORSET)
				//{
				//	m_pSurface->BltIndexSpriteColorSet(&pointTemp, pSprite, 
				//									addonInfo.EffectColorSet);
				//}
				//---------------------------------------- 
				// 정상적인 캐릭터 출력
				//---------------------------------------- 
				//else
				{
					int colorSet1 = addonInfo.ColorSet1, colorSet2 = addonInfo.ColorSet2;
					if(colorSet2 == UNIQUE_ITEM_COLOR || colorSet2 == QUEST_ITEM_COLOR)
					{
						// 유니크 아이템이면
						//									if(colorSet2 == QUEST_ITEM_COLOR)
						//										colorSet2 = MItem::GetQuestItemColorset();
						//									else
						//										colorSet2 = MItem::GetUniqueItemColorset();										
						
						colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
						
						if(addonInfo.ItemClass != ITEM_CLASS_COAT && addonInfo.ItemClass != ITEM_CLASS_TROUSER)
							colorSet1 = colorSet2;
					}
					
					CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
					
					int wipeValue = pCreature->GetInvisibleCount();
					
					if (wipeValue==0)
					{
						m_pSurface->BltIndexSprite(&pointTemp, pSprite);							
					}
					else if (wipeValue==64)
					{
						//CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_SIMPLE_OUTLINE );
						//CIndexSprite::SetEffect( CIndexSprite::EFFECT_SIMPLE_OUTLINE );
						
						//m_pSurface->BltIndexSpriteEffect(&pointTemp, pSprite);
						
						// slayer의 invisible인 snipping
						m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, SHIFT_SNIPPING);
					}
					else
					{
						POINT pointTemp2 = pointTemp;
						// slayer의 invisible인 snipping
						m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, SHIFT_SNIPPING);
						
						CSpriteSurface::s_Value1 = wipeValue;
						
						CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_WIPE_OUT );
						CIndexSprite::SetEffect( CIndexSprite::EFFECT_WIPE_OUT );
						
						m_pSurface->BltIndexSpriteEffect(&pointTemp, pSprite);
						
						//CSpriteSurface::s_Value1 = wipeValue/10+1;
						
						//CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_NET );
						//CIndexSprite::SetEffect( CIndexSprite::EFFECT_NET );
						
						//m_pSurface->BltIndexSpriteEffect(&pointTemp, pSprite);
					}
				}						
				
				// 정상적인 출력
				//CIndexSprite::SetUsingColorSet( addonInfo.ColorSet1, addonInfo.ColorSet2 );
				//m_pSurface->BltIndexSprite(&pointTemp, pSprite);
				
			}
		}
	}
}

void	MTopView::DrawInvisibleForACSlayer(POINT* pPoint, MCreature* pCreature, int action, int direction, int frame )
{
	int clothes;
	BYTE clothesType;
	MCreatureWear*	pCreatureWear = (MCreatureWear*)pCreature;
/*		
	action = ConvAdvancementSlayerActionFromSlayerAction( action, dynamic_cast< MCreatureWear* >(pCreature) );

	if( action == -1 )
		return;
	else
		action -= ADVANCEMENT_ACTION_START;
*/
	CCreatureFramePack& slayerFPK = pCreature->IsMale() ? m_AdvancementSlayerManFPK : m_AdvancementSlayerWomanFPK;
	CIndexSpritePack& addonISPK = pCreature->IsMale() ? m_AdvancementSlayerManSPK : m_AdvancementSlayerWomanSPK;


	//-----------------------------------------------------------
	// 좌우 잔상
	//-----------------------------------------------------------
	for (int i=0; i<AC_ADDON_MAX; i++)
	{
		// Creature의 현재 방향에 따라서...
		// 옷을 출력해주는 순서가 다를 수 있다.
		clothesType = MCreatureWear::s_ACAddonOrder[ direction ][ i ];
		
		// i번째 종류의 옷을 입고 있다면 출력해 준다.
		const MCreatureWear::ADDON_INFO& addonInfo = pCreatureWear->GetACAddonInfo(clothesType);
		
		if (addonInfo.bAddon || AC_ADDON_BODY == i )
		{
			clothes = GetAdvancementPartFromItemClass( addonInfo.ItemClass , addonInfo.FrameID);

			if( clothes == -1 )
				continue;
	
			FRAME_ARRAY &FA = slayerFPK[clothes][action][direction];
			
			// 있는 동작인 경우
			if (FA.GetSize() > frame)
			{
				CFrame &Frame = FA[frame];					
				int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
				int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
				int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
				
				
				CIndexSprite* pSprite = &addonISPK[ sprite ];					
				
				
				pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
				pointTemp.y = pPoint->y + cy;// + pCreature->GetSY();
				
				//---------------------------------------- 
				// 캐릭터 선택 사각형 영역 설정
				//---------------------------------------- 	
				rect.left	= pointTemp.x;
				rect.top	= pointTemp.y;
				rect.right	= rect.left + pSprite->GetWidth();
				rect.bottom = rect.top + pSprite->GetHeight();
				pCreature->AddScreenRect( &rect );
				
				//---------------------------------------- 
				// 캐릭터 부분 색깔이 바뀌는 경우
				//---------------------------------------- 
				//if (addonInfo.bEffectColor)	//colorSet < MAX_COLORSET)
				//{
				//	m_pSurface->BltIndexSpriteColorSet(&pointTemp, pSprite, 
				//									addonInfo.EffectColorSet);
				//}
				//---------------------------------------- 
				// 정상적인 캐릭터 출력
				//---------------------------------------- 
				//else
				{
					int colorSet1 = addonInfo.ColorSet1, colorSet2 = addonInfo.ColorSet2;
					if(colorSet2 == UNIQUE_ITEM_COLOR || colorSet2 == QUEST_ITEM_COLOR)
					{
						// 유니크 아이템이면
						//									if(colorSet2 == QUEST_ITEM_COLOR)
						//										colorSet2 = MItem::GetQuestItemColorset();
						//									else
						//										colorSet2 = MItem::GetUniqueItemColorset();										
						
						colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
						
						if(addonInfo.ItemClass != ITEM_CLASS_COAT && addonInfo.ItemClass != ITEM_CLASS_TROUSER)
							colorSet1 = colorSet2;
					}
					
					CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
					
					int wipeValue = pCreature->GetInvisibleCount();
					
					if (wipeValue==0)
					{
						m_pSurface->BltIndexSprite(&pointTemp, pSprite);							
					}
					else if (wipeValue==64)
					{
						//CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_SIMPLE_OUTLINE );
						//CIndexSprite::SetEffect( CIndexSprite::EFFECT_SIMPLE_OUTLINE );
						
						//m_pSurface->BltIndexSpriteEffect(&pointTemp, pSprite);
						
						// slayer의 invisible인 snipping
						m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, SHIFT_SNIPPING);
					}
					else
					{
						POINT pointTemp2 = pointTemp;
						// slayer의 invisible인 snipping
						m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite, SHIFT_SNIPPING);
						
						CSpriteSurface::s_Value1 = wipeValue;
						
						CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_WIPE_OUT );
						CIndexSprite::SetEffect( CIndexSprite::EFFECT_WIPE_OUT );
						
						m_pSurface->BltIndexSpriteEffect(&pointTemp, pSprite);
						
						//CSpriteSurface::s_Value1 = wipeValue/10+1;
						
						//CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_NET );
						//CIndexSprite::SetEffect( CIndexSprite::EFFECT_NET );
						
						//m_pSurface->BltIndexSpriteEffect(&pointTemp, pSprite);
					}
				}						
				
				// 정상적인 출력
				//CIndexSprite::SetUsingColorSet( addonInfo.ColorSet1, addonInfo.ColorSet2 );
				//m_pSurface->BltIndexSprite(&pointTemp, pSprite);
				
			}
		}
	}
}

void	MTopView::DrawWeaponFadeOut(POINT* pPoint, MCreature* pCreature, int action, int direction, int frame )
{
	WORD clothes;
	BYTE clothesType;
	MCreatureWear*	pCreatureWear = (MCreatureWear*)pCreature;
	
	for (int i=0; i<ADDON_MAX; i++)
	{
		// Creature의 현재 방향에 따라서...
		// 옷을 출력해주는 순서가 다를 수 있다.
		clothesType = MCreatureWear::s_AddonOrder[pCreature->GetDirection()][i];
		
		// i번째 종류의 옷을 입고 있다면 출력해 준다.
		const MCreatureWear::ADDON_INFO& addonInfo = pCreatureWear->GetAddonInfo(clothesType);
		
		if (addonInfo.bAddon)
		{
			clothes = addonInfo.FrameID;
			
			FRAME_ARRAY &FA = m_AddonFPK[clothes][action][direction];
			
			// 있는 동작인 경우
			if (FA.GetSize() > frame)
			{
				CFrame &Frame = FA[frame];					
				int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
				int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
				int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
				
				
				CIndexSprite* pSprite = &m_AddonSPK[ sprite ];					
				
				
				pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
				pointTemp.y = pPoint->y + cy;// + pCreature->GetSY();
				
				//---------------------------------------- 
				// 캐릭터 선택 사각형 영역 설정
				//---------------------------------------- 	
				rect.left	= pointTemp.x;
				rect.top	= pointTemp.y;
				rect.right	= rect.left + pSprite->GetWidth();
				rect.bottom = rect.top + pSprite->GetHeight();
				pCreature->AddScreenRect( &rect );
				
				
				//------------------------------------------------------
				// 잔상 추가
				//------------------------------------------------------
				if (clothesType==ADDON_RIGHTHAND)
				{			
					// 0, 1, 2, 3, 4, 5
					const int darkbits[6] =
					{
						0, 1, 1, 2, 2, 3
					};
					
					POINT pointTemp2;
					
					int maxShadow = min( frame, 5 );
					
					for (int f=1; f<=maxShadow; f++)
					{
						int colorSet1 = addonInfo.ColorSet1, colorSet2 = addonInfo.ColorSet2;
						
						if(colorSet2 == QUEST_ITEM_COLOR || colorSet2 == UNIQUE_ITEM_COLOR )										
						{
							colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
							if(addonInfo.ItemClass != ITEM_CLASS_COAT && addonInfo.ItemClass != ITEM_CLASS_TROUSER)
								colorSet1 = colorSet2;
							
						}
						
						CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
						
						//---------------------------------------- 
						// f frame 전
						//---------------------------------------- 
						CFrame &Frame_f = FA[frame-f];					
						int sprite_f	= Frame_f.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
						
						CIndexSprite* pSprite_f = &m_AddonSPK[ sprite_f ];					
						
						int cx_f		= Frame_f.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
						int cy_f		= Frame_f.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
						
						pointTemp2.x = pPoint->x + cx_f;// + pCreature->GetSX();
						pointTemp2.y = pPoint->y + cy_f;// + pCreature->GetSY();
						
						if( pCreature->HasEffectStatus( EFFECTSTATUS_REDIANCE ) )
							m_pSurface->BltIndexSpriteColorSet(&pointTemp2, pSprite_f, addonInfo.EffectColorSet);
						else
							m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite_f, darkbits[f]);
						
					}								
				}
				
				//---------------------------------------- 
				// 캐릭터 부분 색깔이 바뀌는 경우
				//---------------------------------------- 
				if (addonInfo.bEffectColor)	//colorSet < MAX_COLORSET)
				{
					m_pSurface->BltIndexSpriteColorSet(&pointTemp, pSprite, 
						addonInfo.EffectColorSet);
				}
				//---------------------------------------- 
				// 정상적인 캐릭터 출력
				//---------------------------------------- 
				else
				{
					int colorSet1 = addonInfo.ColorSet1, colorSet2 = addonInfo.ColorSet2;
					if(colorSet2 == UNIQUE_ITEM_COLOR || colorSet2 == QUEST_ITEM_COLOR)
					{
						
						colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
						if(addonInfo.ItemClass != ITEM_CLASS_COAT && addonInfo.ItemClass != ITEM_CLASS_TROUSER)
							colorSet1 = colorSet2;
					}
					
					CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
					
					// 어둡게 찍기
					if (pCreature->IsFade())
					{
						m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 1);
					}
					else
					{
						m_pSurface->BltIndexSprite(&pointTemp, pSprite);							
					}
				}						
				
				// 정상적인 출력
				//CIndexSprite::SetUsingColorSet( addonInfo.ColorSet1, addonInfo.ColorSet2 );
				//m_pSurface->BltIndexSprite(&pointTemp, pSprite);
				
			}
		}
	}
}

void	MTopView::DrawWeaponFadeOutForACSlayer(POINT* pPoint, MCreature* pCreature, int action, int direction, int frame )
{
	int clothes;
	BYTE clothesType;
	MCreatureWear*	pCreatureWear = (MCreatureWear*)pCreature;
	/*
	action = ConvAdvancementSlayerActionFromSlayerAction( action, dynamic_cast< MCreatureWear* >(pCreature) );

	if( action == -1 )
		return;
	else
		action -= ADVANCEMENT_ACTION_START;
		*/

	CCreatureFramePack& slayerFPK = pCreature->IsMale() ? m_AdvancementSlayerManFPK : m_AdvancementSlayerWomanFPK;
	CIndexSpritePack& addonISPK = pCreature->IsMale() ? m_AdvancementSlayerManSPK : m_AdvancementSlayerWomanSPK;


	//-----------------------------------------------------------
	// 좌우 잔상
	//-----------------------------------------------------------
	for (int i=0; i<AC_ADDON_MAX; i++)
	{
		// Creature의 현재 방향에 따라서...
		// 옷을 출력해주는 순서가 다를 수 있다.
		clothesType = MCreatureWear::s_ACAddonOrder[ direction ][ i ];
		
		// i번째 종류의 옷을 입고 있다면 출력해 준다.
		const MCreatureWear::ADDON_INFO& addonInfo = pCreatureWear->GetACAddonInfo(clothesType);
		
		if (addonInfo.bAddon || AC_ADDON_BODY == i )
		{
			clothes = GetAdvancementPartFromItemClass( addonInfo.ItemClass , addonInfo.FrameID);

			if( clothes == -1 )
				continue;
			
			FRAME_ARRAY &FA = slayerFPK[clothes][action][direction];
			
			// 있는 동작인 경우
			if (FA.GetSize() > frame)
			{
				CFrame &Frame = FA[frame];					
				int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
				int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
				int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
				
				
				CIndexSprite* pSprite = &addonISPK[ sprite ];					
				
				
				pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
				pointTemp.y = pPoint->y + cy;// + pCreature->GetSY();
				
				//---------------------------------------- 
				// 캐릭터 선택 사각형 영역 설정
				//---------------------------------------- 	
				rect.left	= pointTemp.x;
				rect.top	= pointTemp.y;
				rect.right	= rect.left + pSprite->GetWidth();
				rect.bottom = rect.top + pSprite->GetHeight();
				pCreature->AddScreenRect( &rect );
				
				
				//------------------------------------------------------
				// 잔상 추가
				//------------------------------------------------------
				if (clothesType==ADDON_RIGHTHAND)
				{			
					// 0, 1, 2, 3, 4, 5
					const int darkbits[6] =
					{
						0, 1, 1, 2, 2, 3
					};
					
					POINT pointTemp2;
					
					int maxShadow = min( frame, 5 );
					
					for (int f=1; f<=maxShadow; f++)
					{
						int colorSet1 = addonInfo.ColorSet1, colorSet2 = addonInfo.ColorSet2;
						
						if(colorSet2 == QUEST_ITEM_COLOR || colorSet2 == UNIQUE_ITEM_COLOR )										
						{
							colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
							if(addonInfo.ItemClass != ITEM_CLASS_COAT && addonInfo.ItemClass != ITEM_CLASS_TROUSER)
								colorSet1 = colorSet2;
							
						}
						
						CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
						
						//---------------------------------------- 
						// f frame 전
						//---------------------------------------- 
						CFrame &Frame_f = FA[frame-f];					
						int sprite_f	= Frame_f.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
						
						CIndexSprite* pSprite_f = &addonISPK[ sprite_f ];					
						
						int cx_f		= Frame_f.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
						int cy_f		= Frame_f.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
						
						pointTemp2.x = pPoint->x + cx_f;// + pCreature->GetSX();
						pointTemp2.y = pPoint->y + cy_f;// + pCreature->GetSY();
						
						if( pCreature->HasEffectStatus( EFFECTSTATUS_REDIANCE ) )
							m_pSurface->BltIndexSpriteColorSet(&pointTemp2, pSprite_f, addonInfo.EffectColorSet);
						else
							m_pSurface->BltIndexSpriteDarkness(&pointTemp2, pSprite_f, darkbits[f]);
						
					}								
				}
				
				//---------------------------------------- 
				// 캐릭터 부분 색깔이 바뀌는 경우
				//---------------------------------------- 
				if (addonInfo.bEffectColor)	//colorSet < MAX_COLORSET)
				{
					m_pSurface->BltIndexSpriteColorSet(&pointTemp, pSprite, 
						addonInfo.EffectColorSet);
				}
				//---------------------------------------- 
				// 정상적인 캐릭터 출력
				//---------------------------------------- 
				else
				{
					int colorSet1 = addonInfo.ColorSet1, colorSet2 = addonInfo.ColorSet2;
					if(colorSet2 == UNIQUE_ITEM_COLOR || colorSet2 == QUEST_ITEM_COLOR)
					{
						
						colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
						if(addonInfo.ItemClass != ITEM_CLASS_COAT && addonInfo.ItemClass != ITEM_CLASS_TROUSER)
							colorSet1 = colorSet2;
					}
					
					CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
					
					// 어둡게 찍기
					if (pCreature->IsFade())
					{
						m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 1);
					}
					else
					{
						m_pSurface->BltIndexSprite(&pointTemp, pSprite);							
					}
				}						
				
				// 정상적인 출력
				//CIndexSprite::SetUsingColorSet( addonInfo.ColorSet1, addonInfo.ColorSet2 );
				//m_pSurface->BltIndexSprite(&pointTemp, pSprite);
				
			}
		}
	}
}

void	MTopView::DrawCentauroTurret( POINT* pPoint, MCreature* pCreature, int action, int direction, int frame , int body)
{
	MFakeCreature *pFakeCreature = (MFakeCreature *)pCreature;
	BYTE direct = pFakeCreature->GetTurretDirection();
	
	FRAME_ARRAY& FA = m_CreatureFPK[body][ACTION_VAMPIRE_DRAIN][2];
	
	CFrame& Frame =	FA[direct];
	int sprite = Frame.GetSpriteID(),	//m_CreatureFPK[body][action][direction][frame].GetSpriteID(),
		cx		= Frame.GetCX(),	//m_CreatureFPK[body][action][direction][frame].GetCX(),
		cy		= Frame.GetCY();	//m_CreatureFPK[body][action][direction][frame].GetCY();

	if( pCreature->GetCreatureType() == 702 )
	{
		POINT pointGap[8] = 
		{
			{ 11, 6 },	// 좌
			{ 8, 2 },	// 좌하
			{ 0, 0 },	// 하
			{ -7, 2 },	// 우하
			{ -10, 6 },	// 우
			{ -7, 10 },	// 우상
			{ 0, 12 },	// 상
			{ 8, 10 },	// 좌상
		};
		cx += pointGap[pFakeCreature->GetDirection()].x;
		cy += pointGap[pFakeCreature->GetDirection()].y;
	}
	
	if( pFakeCreature->GetTurretDelay() > 0 )
	{
		extern POINT g_DirectionValue[MAX_DIRECTION];
		BYTE direct = (pFakeCreature->GetTurretDirection()/4+2)%8;
		cx -= g_DirectionValue[direct].x*pFakeCreature->GetTurretDelay()*2;
		cy -= g_DirectionValue[direct].y*pFakeCreature->GetTurretDelay()*2;
	}
	
	// 좌표 보정
	pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
	pointTemp.y = pPoint->y + cy;// + pCreature->GetSY();
	
	int colorSet1 = pCreature->GetBodyColor1();
	int colorSet2 = pCreature->GetBodyColor2();
	
	if( colorSet1 == QUEST_ITEM_COLOR || colorSet1 == UNIQUE_ITEM_COLOR )
		colorSet1 = MItem::GetSpecialColorItemColorset( colorSet1 );
	
	if( colorSet2 == QUEST_ITEM_COLOR || colorSet2 == UNIQUE_ITEM_COLOR )
		colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
	
	CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );

	CIndexSprite* pSprite = &m_CreatureSPK[ sprite ];

	//m_pSurface->BltIndexSprite(&pointTemp, pSprite);
	if(m_SelectCreatureID == pCreature->GetID())
		m_SOM.Add( pointTemp.x, pointTemp.y, pSprite );
	else
		m_pSurface->BltIndexSprite(&pointTemp, pSprite);
}

void	MTopView::DrawInstallTurret( POINT *pPoint, MCreature* pCreature, int action, int direction, int frame, int frameindex )
{
	//	frame = pCreature->GetInstallTurretCount();
	if(ACTION_STAND == action)
	{
		direction = 2;
		frame = pCreature->GetInstallTurretCount();
	}
	FRAME_ARRAY& FA = m_CreatureFPK[222][action][direction];
	
	if (FA.GetSize() > frame)
	{			
		CFrame&	Frame = FA[frame];
		
		int sprite = Frame.GetSpriteID(),	//m_CreatureFPK[body][action][direction][frame].GetSpriteID(),
			cx		= Frame.GetCX(),	//m_CreatureFPK[body][action][direction][frame].GetCX(),
			cy		= Frame.GetCY();	//m_CreatureFPK[body][action][direction][frame].GetCY();
		
		CIndexSprite* pSprite = &m_CreatureSPK[ sprite ];
		
		pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
		pointTemp.y = pPoint->y + cy;// + pCreature->GetSY();			
		
		//---------------------------------------- 		
		// 캐릭터 선택 사각형 영역 설정
		//---------------------------------------- 	
		rect.left	= pointTemp.x;
		rect.top	= pointTemp.y;
		rect.right	= rect.left + pSprite->GetWidth();
		rect.bottom = rect.top + pSprite->GetHeight();
		if(frameindex ==0 )
			pCreature->SetScreenRect( &rect );
		
		if(m_SelectCreatureID == pCreature->GetID())
		{
			WORD colorSet = pCreature->GetAttachEffectColor();
			m_SOM.Add( pointTemp.x, pointTemp.y, pSprite, colorSet );
		}
		else
			m_pSurface->BltIndexSprite(&pointTemp, pSprite);
		
	}
}


