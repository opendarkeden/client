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

#ifdef PLATFORM_WINDOWS
#include "DX3D.h"
#endif
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

extern int defaultSkinColor;
extern int defaultCoatColor;
extern int defaultTrouserColor;

namespace {
	POINT pointTemp;
	RECT rect;
};

extern int GetAdvancementPartFromItemClass( ITEM_CLASS itemClass , TYPE_FRAMEID frameID);
void	MTopView::DrawDivineGuidanceSlayerCharacter( 
	POINT *pPoint, MCreature* pCreature, int direction )
{
	MCreatureWear*	pCreatureWear = (MCreatureWear*)pCreature;
	
	// Creature의 Action에 맞는 add-on을 출력한다.
	//action = pCreature->GetAction();
	const int tempFrame = 30;
	
	WORD clothes;
	BYTE clothesType;
	
	for(int k = 0; k < 2; k++)
	{
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
				
				FRAME_ARRAY &FA = m_AddonFPK[clothes][ACTION_DRAINED][(direction+g_CurrentFrame/2+k)%8];
				
				// 있는 동작인 경우
				if (FA.GetSize() > tempFrame)
				{
					CFrame &Frame = FA[tempFrame];					
					int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
					int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
					int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
					
					
					CIndexSprite* pSprite = &m_AddonSPK[ sprite ];					
					
					pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
					pointTemp.y = pPoint->y + cy-(*g_pCreatureTable)[pCreature->GetCreatureType()].Height;// + pCreature->GetSY();
					
					//---------------------------------------- 
					// 정상적인 캐릭터 출력
					//---------------------------------------- 
					int colorSet1 = addonInfo.ColorSet1, colorSet2 = addonInfo.ColorSet2;
					if(colorSet2 == UNIQUE_ITEM_COLOR || colorSet2 == QUEST_ITEM_COLOR)
					{
						colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
						if(addonInfo.ItemClass != ITEM_CLASS_COAT && addonInfo.ItemClass != ITEM_CLASS_TROUSER)
							colorSet1 = colorSet2;
					}
					
					CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
					
					m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 2-k);
				}
			}
		}
	}
}

void	MTopView::DrawDivineGuidanceAdvancementClassSlayerCharacter( POINT *pPoint, MCreature* pCreature, int direction )
{
	MCreatureWear*	pCreatureWear = (MCreatureWear*)pCreature;
	
	// Creature의 Action에 맞는 add-on을 출력한다.
	//action = pCreature->GetAction();
	const int tempFrame = 30;
	
	WORD clothes;
	BYTE clothesType;
	
	CCreatureFramePack& slayerFPK = pCreature->IsMale() ? m_AdvancementSlayerManFPK : m_AdvancementSlayerWomanFPK;
	CIndexSpritePack& addonISPK = pCreature->IsMale() ? m_AdvancementSlayerManSPK : m_AdvancementSlayerWomanSPK;
			
	for(int k = 0; k < 2; k++)
	{
		for (int i=0; i<ADDON_MAX; i++)
		{
			// Creature의 현재 방향에 따라서...
			// 옷을 출력해주는 순서가 다를 수 있다.
			clothesType = MCreatureWear::s_ACAddonOrder[pCreature->GetDirection()][i];
			
			// i번째 종류의 옷을 입고 있다면 출력해 준다.
			const MCreatureWear::ADDON_INFO& addonInfo = pCreatureWear->GetACAddonInfo(clothesType);
			
			if (addonInfo.bAddon)
			{
				clothes = GetAdvancementPartFromItemClass( addonInfo.ItemClass, addonInfo.FrameID );
				
				FRAME_ARRAY &FA = slayerFPK[clothes][ACTION_ADVANCEMENT_SLAYER_DRAINED-ADVANCEMENT_ACTION_START][(direction+g_CurrentFrame/2+k)%8];
				
				
				// 있는 동작인 경우
				if (FA.GetSize() > tempFrame)
				{
					CFrame &Frame = FA[tempFrame];					
					int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
					int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
					int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
					
					
					CIndexSprite* pSprite = &addonISPK[ sprite ];					
					
					pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
					pointTemp.y = pPoint->y + cy-(*g_pCreatureTable)[pCreature->GetCreatureType()].Height;// + pCreature->GetSY();
					
					//---------------------------------------- 
					// 정상적인 캐릭터 출력
					//---------------------------------------- 
					int colorSet1 = addonInfo.ColorSet1, colorSet2 = addonInfo.ColorSet2;
					if(colorSet2 == UNIQUE_ITEM_COLOR || colorSet2 == QUEST_ITEM_COLOR)
					{
						colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
						if(addonInfo.ItemClass != ITEM_CLASS_COAT && addonInfo.ItemClass != ITEM_CLASS_TROUSER)
							colorSet1 = colorSet2;
					}
					
					CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
					
					m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 2-k);
				}
			}
		}
	}
}

void	MTopView::DrawDivineGuidanceVampireCharacter( 
			POINT *pPoint, MCreature* pCreature, int direction, int body )
{
	static int tempGuidance = 0;
	tempGuidance = (g_CurrentFrame)%20;
	const int tempFrame = 30;
	

	for(int k = 0; k < 2; k++)
	{
		FRAME_ARRAY &FA = m_CreatureFPK[body][ACTION_DRAINED][(direction+g_CurrentFrame/2+k)%8];
		
		if (FA.GetSize() > tempFrame)
		{
			CFrame& Frame =	FA[tempFrame];
			int sprite = Frame.GetSpriteID(),	//m_CreatureFPK[body][action][direction][frame].GetSpriteID(),
				cx		= Frame.GetCX(),	//m_CreatureFPK[body][action][direction][frame].GetCX(),
				cy		= Frame.GetCY();	//m_CreatureFPK[body][action][direction][frame].GetCY();
			
			// 좌표 보정
			pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
			pointTemp.y = pPoint->y + cy-(*g_pCreatureTable)[pCreature->GetCreatureType()].Height;// + pCreature->GetSY();
			
			CIndexSprite* pSprite = &m_CreatureSPK[ sprite ];
			
			int colorSet1 = pCreature->GetBodyColor1();
			int colorSet2 = pCreature->GetBodyColor2();
			
			if( colorSet1 == QUEST_ITEM_COLOR || colorSet1 == UNIQUE_ITEM_COLOR )
				colorSet1 = MItem::GetSpecialColorItemColorset( colorSet1 );
			
			if( colorSet2 == QUEST_ITEM_COLOR || colorSet2 == UNIQUE_ITEM_COLOR )
				colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
			
			CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
			
			pointTemp.y += abs(10-tempGuidance);
			m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 2-k);
		}
	}
}

void	MTopView::DrawDivineGuidanceAdvancementClassVampireCharacter( 
			POINT *pPoint, MCreature* pCreature, int direction, int body )
{
	static int tempGuidance = 0;
	tempGuidance = (g_CurrentFrame)%20;
	const int tempFrame = 30;

	CCreatureFramePack& advanceVampireFPK = pCreature->IsMale() ? m_AdvancementVampireManFPK : m_AdvancementVampireWomanFPK;
	CIndexSpritePack& advanceVampireSPK = pCreature->IsMale() ? m_AdvancementVampireManSPK : m_AdvancementVampireWomanSPK;	

	for(int k = 0; k < 2; k++)
	{
		FRAME_ARRAY &FA = advanceVampireFPK[0][ACTION_ADVANCEMENT_DRAINED-ADVANCEMENT_ACTION_START][(direction+g_CurrentFrame/2+k)%8];
		
		if (FA.GetSize() > tempFrame)
		{
			CFrame& Frame =	FA[tempFrame];
			int sprite = Frame.GetSpriteID(),	//m_CreatureFPK[body][action][direction][frame].GetSpriteID(),
				cx		= Frame.GetCX(),	//m_CreatureFPK[body][action][direction][frame].GetCX(),
				cy		= Frame.GetCY();	//m_CreatureFPK[body][action][direction][frame].GetCY();
			
			// 좌표 보정
			pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
			pointTemp.y = pPoint->y + cy-(*g_pCreatureTable)[pCreature->GetCreatureType()].Height;// + pCreature->GetSY();
			
			CIndexSprite* pSprite = &advanceVampireSPK[ sprite ];
			
			int colorSet1 = pCreature->GetBodyColor1();
			int colorSet2 = pCreature->GetBodyColor2();
			
			if( colorSet1 == QUEST_ITEM_COLOR || colorSet1 == UNIQUE_ITEM_COLOR )
				colorSet1 = MItem::GetSpecialColorItemColorset( colorSet1 );
			
			if( colorSet2 == QUEST_ITEM_COLOR || colorSet2 == UNIQUE_ITEM_COLOR )
				colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
			
			CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
			
			pointTemp.y += abs(10-tempGuidance);
			m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 2-k);
		}
	}
}

void	MTopView::DrawDivineGuidanceOustersCharacter( 
			POINT *pPoint, MCreature* pCreature, int direction )
{
	//  챠크람이 있다
	MCreatureWear *pCreatureWear = (MCreatureWear *)pCreature;
	const int tempFrame = 30;
	
	const MCreatureWear::ADDON_INFO& addonInfoChakram = pCreatureWear->GetAddonInfo(ADDON_RIGHTHAND);
	bool bChakram = addonInfoChakram.bAddon && addonInfoChakram.ItemClass == ITEM_CLASS_OUSTERS_CHAKRAM;
	
	int tempAction = ACTION_DRAINED;
	
	// 몸을 찍고 챠크람을 찍는다
	const MCreatureWear::ADDON_INFO& addonInfo = pCreatureWear->GetAddonInfo(ADDON_COAT);
	const MCreatureWear::ADDON_INFO& bootsAddonInfo = pCreatureWear->GetAddonInfo(ADDON_TROUSER);
	
	for(int k = 0; k < 2; k++)
	{
		if (addonInfo.bAddon && !pCreatureWear->IsGhost(1))
		{
			int clothes = addonInfo.FrameID;			
			
			FRAME_ARRAY &FA = m_OustersFPK[clothes][tempAction][(direction+g_CurrentFrame/2+k)%8];
			
			// 있는 동작인 경우
			if (FA.GetSize() > tempFrame)
			{
				CFrame &Frame = FA[tempFrame];					
				int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
				int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
				int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
				
				CIndexSprite* pSprite = &m_OustersSPK[ sprite ];					
				
				pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
				pointTemp.y = pPoint->y + cy-(*g_pCreatureTable)[pCreature->GetCreatureType()].Height;// + pCreature->GetSY();
				
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
				
				m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 2-k);
				
			}
		}
		
		if (bChakram && !pCreatureWear->IsGhost(2))
		{
			int clothes = addonInfoChakram.FrameID;
			
			FRAME_ARRAY &FA = m_OustersFPK[clothes][tempAction][(direction+g_CurrentFrame/2+k)%8];
			
			// 있는 동작인 경우
			if (FA.GetSize() > tempFrame)
			{
				CFrame &Frame = FA[tempFrame];					
				int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
				int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
				int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
				
				CIndexSprite* pSprite = &m_OustersSPK[ sprite ];					
				
				pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
				pointTemp.y = pPoint->y + cy-(*g_pCreatureTable)[pCreature->GetCreatureType()].Height;// + pCreature->GetSY();
				
				int colorset1, colorset2;
				colorset1 = addonInfoChakram.ColorSet1;
				colorset2 = addonInfoChakram.ColorSet2;
				
				if( colorset1 == QUEST_ITEM_COLOR || colorset1 == UNIQUE_ITEM_COLOR )
					colorset1 = MItem::GetSpecialColorItemColorset( colorset1 );
				if( colorset2 == QUEST_ITEM_COLOR || colorset2 == UNIQUE_ITEM_COLOR )
					colorset2 = MItem::GetSpecialColorItemColorset( colorset2 );
				CIndexSprite::SetUsingColorSet( colorset1, colorset2 );
				
				m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 2-k);
			}
		}
	}
}

void	MTopView::DrawDivineGuidanceAdvancementClassOustersCharacter( 
			POINT *pPoint, MCreature* pCreature, int direction )
{
	//  챠크람이 있다
	MCreatureWear *pCreatureWear = (MCreatureWear *)pCreature;
	const int tempFrame = 30;
	
	const MCreatureWear::ADDON_INFO& addonInfoChakram = pCreatureWear->GetAddonInfo(ADDON_RIGHTHAND);
	bool bChakram = addonInfoChakram.bAddon && addonInfoChakram.ItemClass == ITEM_CLASS_OUSTERS_CHAKRAM;
	
	int tempAction = ACTION_ADVANCEMENT_OUSTERS_DRAINED-ADVANCEMENT_ACTION_START;
	const MCreatureWear::ADDON_INFO& addonInfo = pCreatureWear->GetAddonInfo(ADDON_COAT);
	const MCreatureWear::ADDON_INFO& bootsAddonInfo = pCreatureWear->GetAddonInfo(ADDON_TROUSER);
	
	for(int k = 0; k < 2; k++)
	{
		if (addonInfo.bAddon && !pCreatureWear->IsGhost(1))
		{
			int clothes = 1;
			
			FRAME_ARRAY &FA = m_AdvancementOustersFPK[clothes][tempAction][(direction+g_CurrentFrame/2+k)%8];
			
			// 있는 동작인 경우
			if (FA.GetSize() > tempFrame)
			{
				CFrame &Frame = FA[tempFrame];					
				int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
				int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
				int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
				
				CIndexSprite* pSprite = &m_AdvancementOustersSPK[ sprite ];					
				
				pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
				pointTemp.y = pPoint->y + cy-(*g_pCreatureTable)[pCreature->GetCreatureType()].Height;// + pCreature->GetSY();
				
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
				
				m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 2-k);
				
			}
		}
		
		if (bChakram && !pCreatureWear->IsGhost(2))
		{
			int clothes = 0;
			
			FRAME_ARRAY &FA = m_AdvancementOustersFPK[clothes][tempAction][(direction+g_CurrentFrame/2+k)%8];
			
			// 있는 동작인 경우
			if (FA.GetSize() > tempFrame)
			{
				CFrame &Frame = FA[tempFrame];					
				int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
				int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
				int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
				
				CIndexSprite* pSprite = &m_AdvancementOustersSPK[ sprite ];					
				
				pointTemp.x = pPoint->x + cx;// + pCreature->GetSX();
				pointTemp.y = pPoint->y + cy-(*g_pCreatureTable)[pCreature->GetCreatureType()].Height;// + pCreature->GetSY();
				
				int colorset1, colorset2;
				colorset1 = addonInfoChakram.ColorSet1;
				colorset2 = addonInfoChakram.ColorSet2;
				
				if( colorset1 == QUEST_ITEM_COLOR || colorset1 == UNIQUE_ITEM_COLOR )
					colorset1 = MItem::GetSpecialColorItemColorset( colorset1 );
				if( colorset2 == QUEST_ITEM_COLOR || colorset2 == UNIQUE_ITEM_COLOR )
					colorset2 = MItem::GetSpecialColorItemColorset( colorset2 );
				CIndexSprite::SetUsingColorSet( colorset1, colorset2 );
				
				m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 2-k);
			}
		}
	}
}