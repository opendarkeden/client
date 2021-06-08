// VS_UI_Description.cpp

#include "VS_UI_Description.h"
#include "VS_UI_GameCommon.h"
#include "VS_UI_Shop.h"
#include "VS_UI_mouse_pointer.h"

#include "MPriceManager.h"
#include "MItemOptionTable.h"
#include "UserOption.h"
#include "ClientConfig.H"

#include "MGameStringTable.H"
#include "MZoneTable.h"
#include "MTimeItemManager.h"

#ifdef _LIB
#include "ClientFunction.h"
#endif

enum 
{
	SMALL_FONT_Y_GAP = 16,
	DEFAULT_BOX_WIDTH = 200,
	SIDE_GAP = 14,
	SIDE_GAP_HALF = 7,
	NORMAL_FONT_Y_GAP = 18,
	NAME_STRING_LEN = 100, // 100 ÀÌÇÏ°ÚÁö?
	VALUE_GAP = 5,
};

#define ITEM_NAME_NORMAL_COLOR			RGB_WHITE
#define ITEM_NAME_OPTION_COLOR			RGB_YELLOW
#define ITEM_ENAME_NORMAL_COLOR			RGB(200,200,200)
#define ITEM_ENAME_OPTION_COLOR			RGB(200,200,100)
#define ITEM_NAME_NORMAL_SHADE			RGB(100,100,100)
#define ITEM_NAME_OPTION_SHADE			RGB(100,100,0)
#define ITEM_ENAME_RARE_COLOR			RGB(15<<3,15<<3,31<<3)
#define ITEM_DESC_RGB					RGB(192, 192, 255)

//-----------------------------------------------------------------------------
// _Item_Description_Show
//
// 
//-----------------------------------------------------------------------------
void	_Item_Description_Show(Rect rect, void * void_ptr, long left, long right)
{
	COLORREF ITEM_NAME_RARE_OPTION_SHADE = RGB(0,0,31<<3);
	MItem * p_item = (MItem *)void_ptr;
	if(p_item == NULL)
		return;

//#ifdef _LIB // Client¿¡¼­´Â 3D¿ëÀ¸·Î »ç¿ëÇÒ ¼ö ÀÖ´Ù. -.-
	RECT _rect;
	
	SetRect(&_rect, rect.x, rect.y, rect.x+rect.w, rect.y+rect.h);
	DrawAlphaBox(&_rect, 0, 0, 0, g_pUserOption->ALPHA_DEPTH);
	
//	SetRect(&_rect, rect.x, rect.y, rect.x+rect.w, rect.y+rect.h); // -.- TransparentBlt16()¿¡¼­ °ªÀ» º¯°æÇÔ.. ¾Æ´Ï ÀÌ·²¼ö°¡!
	gpC_base->m_p_DDSurface_back->DrawRect(&_rect, LIGHT_MAGENTA);

//#endif

//	RECT dest_rect;
//	S_SURFACEINFO	dest_surface;

//#ifndef _LIB
//	if (gpC_base->m_DDSurface_offscreen.Lock())
//	{
//		S_SURFACEINFO src_surface;
//		S_RECT src_rect;
//
//		SetSurfaceInfo(&src_surface, gpC_base->m_DDSurface_offscreen.GetDDSD());
//
//		//SetRect(src_rect, 0, 0, rect.w, rect.h);
//		//filledRect(&src_surface, &src_rect, BLACK);
//#endif
//		if (gpC_base->m_p_DDSurface_back->Lock()) // Descriptor common background¸¦ Ãâ·ÂÇÑ´Ù.
//		{
//			SetSurfaceInfo(&dest_surface, gpC_base->m_p_DDSurface_back->GetDDSD());
//			SetRect(dest_rect, rect.x, rect.y, rect.w, rect.h);

//#ifndef _LIB
//			//TransparentBlt16(&dest_surface, &src_surface, &dest_rect, &src_rect);
//			filledRect(&dest_surface, &dest_rect, BLACK);
//#endif
//			rectangle(&dest_surface, dest_rect.x, dest_rect.y, dest_rect.x+dest_rect.w-1, dest_rect.y+dest_rect.h-1, LIGHT_MAGENTA);

//			gpC_base->m_p_DDSurface_back->Unlock();
//		}

//#ifndef _LIB
//		gpC_base->m_DDSurface_offscreen.Unlock();
//	}
//#endif

	//
	// show contents
	//
	int px = rect.x+SIDE_GAP_HALF;
	int py = rect.y+SIDE_GAP_HALF;
//	gpC_base->m_item_name_pi.text_color = ITEM_NAME_NORMAL_RGB;

	// nameÀÌ Á¦ÀÏ ±æ ¼ö ÀÖ±â ¶§¹®¿¡ nameÀ» ±âÁØÀ¸·Î Á¤ÇÑ´Ù.
	char sz_name[NAME_STRING_LEN];
	char sz_ename[NAME_STRING_LEN];
	sz_name[0] = '\0'; // for strcat()
	sz_ename[0] = '\0'; // for strcat()
	
	COLORREF name_color = ITEM_NAME_NORMAL_COLOR;
	COLORREF ename_color = ITEM_ENAME_NORMAL_COLOR;
	COLORREF shadow_color = ITEM_NAME_NORMAL_SHADE;

#ifndef __ENGLISH_VERSION__
	strcat(sz_ename, "(");
#endif
	// identifyµÈ ¾ÆÀÌÅÛÀÎ °æ¿ì.. by sigi
	if (g_pTimeItemManager->IsExist( p_item->GetID() ) )
	{
		strcat( sz_name, (*g_pGameStringTable)[UI_STRING_MESSAGE_QUEST_HAN].GetString() );
		strcat(sz_name, " "); // add 'space'
		strcat( sz_ename,(*g_pGameStringTable)[UI_STRING_MESSAGE_QUEST_ENG].GetString() );
		strcat(sz_ename, " "); // add 'space'
	}

	if (p_item->IsIdentified())
	{
		if(p_item->IsUniqueItem() || p_item->IsQuestItem() )
		{
			WORD temp_color;
			if( p_item->IsQuestItem() )
				temp_color = CIndexSprite::ColorSet[MItem::GetQuestItemColorset()][15];
			else
				temp_color = CIndexSprite::ColorSet[MItem::GetUniqueItemColorset()][15];				
			name_color = RGB(CDirectDraw::Red(temp_color)<<3, CDirectDraw::Green(temp_color)<<3, CDirectDraw::Blue(temp_color)<<3);
			ename_color = name_color;
			
			if( p_item->IsQuestItem() )
				temp_color = CIndexSprite::ColorSet[MItem::GetQuestItemColorset()][29];			
			else
				temp_color = CIndexSprite::ColorSet[MItem::GetUniqueItemColorset()][29];				
			shadow_color = RGB(CDirectDraw::Red(temp_color)<<3, CDirectDraw::Green(temp_color)<<3, CDirectDraw::Blue(temp_color)<<3);
		}
		else
			if (!p_item->IsEmptyItemOptionList() && p_item->GetItemClass() != MItem::ITEM_CLASS_COUPLE_RING &&
				p_item->GetItemClass() != MItem::ITEM_CLASS_VAMPIRE_COUPLE_RING)
		{
				//add by sonic 2006.10.28 ÏÔÊ¾Èý¸öÊôÐÔ×°±¸Ãû×Ö
			//for(int i=0;i<min(2,p_item->GetItemOptionListCount());i++)
			for(int i=0;i<min(3,p_item->GetItemOptionListCount());i++)
			{
				if(p_item->GetItemOptionName(i)!=NULL)
				{
					strcat(sz_name, p_item->GetItemOptionName(i));
					strcat(sz_name, " "); // add 'space'
				}
				if(p_item->GetItemOptionEName(i)!=NULL)
				{
					strcat(sz_ename, p_item->GetItemOptionEName(i));
					strcat(sz_ename, " "); // add 'space'
				}
			}
			if(p_item->GetItemOptionListCount() == 2 )			// Rare ¾ÆÀÌÅÛÀÎ °æ¿ì
			{
				name_color = g_pClientConfig->COLOR_NAME_ITEM_RARE_OPTION;
				shadow_color = ITEM_NAME_RARE_OPTION_SHADE;
				ename_color = ITEM_ENAME_RARE_COLOR;
			} 
			// add by Sonic 2006.10.28 Ôö¼ÓÏÔÊ¾ÈýÊôÐÔ×°±¸ÎªºìÉ«
			else if(p_item->GetItemOptionListCount() > 2)
			{
				name_color = g_pClientConfig->COLOR_NAME_VAMPIRE; //Red
				shadow_color = 0;//g_pClientConfig->COLOR_NAME_VAMPIRE; //Red
				ename_color = g_pClientConfig->COLOR_NAME_VAMPIRE; //Red
			}
			// end by Sonic 2006.10.28 Ôö¼ÓÏÔÊ¾ÈýÊôÐÔ×°±¸ÎªºìÉ«
			else
			{
				name_color = ITEM_NAME_OPTION_COLOR;
				shadow_color = ITEM_NAME_OPTION_SHADE;
				ename_color = ITEM_ENAME_OPTION_COLOR;
			}			
		}
	}

	strcat(sz_name, p_item->GetName());
	strcat(sz_ename, p_item->GetEName());
#ifndef __ENGLISH_VERSION__
	strcat(sz_ename, ")");
#endif 

	if(!p_item->IsIdentified())
	{
		for(int i = 0; i < strlen(sz_name); i++)
			sz_name[i] = '?';
		
		for(i = 0; i < strlen(sz_ename); i++)
			sz_ename[i] = '?';
	}

	char sz_buf[50];

	g_FL2_GetDC();
	
#ifndef __ENGLISH_VERSION__
	// name
	// optionÀÌ ºÙ¾îÀÖÀ¸¸é ´Ù¸¥ »öÀ¸·Î Ãâ·ÂÇÑ´Ù.
	g_PrintColorStr(px+1, py+1, sz_name, gpC_base->m_item_name_pi, shadow_color);
	g_PrintColorStr(px, py, sz_name, gpC_base->m_item_name_pi, name_color);
	py += NORMAL_FONT_Y_GAP;
	g_PrintColorStr(px+1, py+1, sz_ename, gpC_base->m_item_desc_pi, shadow_color);
	g_PrintColorStr(px, py, sz_ename, gpC_base->m_item_desc_pi, ename_color);
	py += SMALL_FONT_Y_GAP+5;
#else
	// optionÀÌ ºÙ¾îÀÖÀ¸¸é ´Ù¸¥ »öÀ¸·Î Ãâ·ÂÇÑ´Ù.
	g_PrintColorStr(px+1, py+1, sz_ename, gpC_base->m_item_name_pi, shadow_color);
	g_PrintColorStr(px, py, sz_ename, gpC_base->m_item_name_pi, name_color);
	py += SMALL_FONT_Y_GAP+5;
#endif


	int vx;

	const COLORREF required_rgb = RGB(255, 128, 64);

	// identifyµÈ ¾ÆÀÌÅÛÀÎ °æ¿ì.. by sigi
//	if (p_item->IsIdentified())
	{
		// weight
//		vx = g_PrintColorStr(px, py, "Weight", gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
//		sprintf(sz_buf, "%d", p_item->GetWeight());
//		g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);

//		py += SMALL_FONT_Y_GAP;

		// gun °è¿­
//		if(p_item->IsGunItem())
		{

			switch(p_item->GetItemClass())
			{
			case MItem::ITEM_CLASS_SWORD:
				wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_SWORD].GetString());
				break;
				
			case MItem::ITEM_CLASS_BLADE:
				wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_BLADE].GetString());
				break;
				
			case MItem::ITEM_CLASS_CROSS:
				wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_CROSS].GetString());
				break;
				
			case MItem::ITEM_CLASS_MACE:
				wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_MACE].GetString());
				break;
				
			case MItem::ITEM_CLASS_MINE:
				wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_MINE].GetString());
				break;
				
			case MItem::ITEM_CLASS_BOMB:
				wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_BOMB].GetString());
				break;
				
			case MItem::ITEM_CLASS_BOMB_MATERIAL:
				if(p_item->GetItemType() < 5)
					wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_BOMB_MATERIAL].GetString());
				else
					wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_MINE_MATERIAL].GetString());
				break;
				
			case MItem::ITEM_CLASS_SG:
				wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_SG].GetString());
				break;
				
			case MItem::ITEM_CLASS_SMG:
				wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_SMG].GetString());
				break;
				
			case MItem::ITEM_CLASS_AR:
				wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_AR].GetString());
				break;
				
			case MItem::ITEM_CLASS_SR:
				wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_SR].GetString());
				break;
				
			default:
				wsprintf(sz_buf, "");
				break;
			}
			
			if(strlen(sz_buf) > 0)
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_CLASS].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, required_rgb);
				
				py += SMALL_FONT_Y_GAP;
			}
		}
		
		// Identified ºÐ¸®
		if(p_item->IsIdentified())
		{
			// Durability
			if (p_item->GetMaxDurability() != -1&&!p_item->IsUniqueItem()&& !p_item->IsQuestItem() &&
				p_item->GetItemClass()!=MItem::ITEM_CLASS_VAMPIRE_AMULET &&
				p_item->GetItemClass()!=MItem::ITEM_CLASS_COUPLE_RING &&
				p_item->GetItemClass()!=MItem::ITEM_CLASS_VAMPIRE_COUPLE_RING 
				)
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DURABILITY].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "%d/%d", p_item->GetCurrentDurability(), p_item->GetMaxDurability());				
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
				py += SMALL_FONT_Y_GAP;
			}
			// Silvering
			if (p_item->GetSilverMax() != -1)
			{
				vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_SILVERING].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "%d/%d", p_item->GetSilver(), p_item->GetSilverMax());
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
				py += SMALL_FONT_Y_GAP;
			}			
			// Damage
			if (p_item->GetMaxDamage() != -1) // damage°¡ ÀÖ´Â°¡?
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DAMAGE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "%d~%d", p_item->GetMinDamage(), p_item->GetMaxDamage());				
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_RED);
				py += SMALL_FONT_Y_GAP;
			}			
			// critical hit
			if (p_item->GetCriticalHit() != -1)//Å©¸®Æ®ÄÃ È÷Æ®°¡ ÀÖ´Â°¡?
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_CRITICALHIT].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "%d", p_item->GetCriticalHit());
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_RED);
				py += SMALL_FONT_Y_GAP;
			}		
			// Defense
			if (p_item->GetDefenseValue() != -1)
			{
				vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DEFENSE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "%d", p_item->GetDefenseValue());
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_GREEN);				
				py += SMALL_FONT_Y_GAP;
			}			
			// Protection
			if (p_item->GetProtectionValue() != -1)
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_PROTECTION].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "%d", p_item->GetProtectionValue());
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_GREEN);				
				py += SMALL_FONT_Y_GAP;
			}			
			// TOHIT
			if (p_item->GetToHit() != -1)
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ACCURACY].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "%d", p_item->GetToHit());
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_GREEN);				
				py += SMALL_FONT_Y_GAP;
			}			
			// Heal point
			if (p_item->GetHealPoint() != -1)
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_HP].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "+%d", p_item->GetHealPoint());
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
				py += SMALL_FONT_Y_GAP;
			}			
			// Mana point
			if (p_item->GetManaPoint() != -1)
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_MP].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "+%d", p_item->GetManaPoint());
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
				py += SMALL_FONT_Y_GAP;
			}
			
			// Attack range
			if (p_item->GetReach() != -1 && p_item->GetReach() != 1)
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_RANGE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "%d %s", p_item->GetReach(),(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_TILE_PIECE].GetString());
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
				py += SMALL_FONT_Y_GAP;
			}			
			// Bullet
			if (p_item->GetMagazineSize() != -1)
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_MAGAZINE_NUM].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "%d %s", p_item->GetMagazineSize(),(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_MAGAZINE_COUNT].GetString());
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
				py += SMALL_FONT_Y_GAP;
			}
			// Pocket size
			if (p_item->GetPocketNumber() != -1)
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_POCKET_NUM].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "%d %s", p_item->GetPocketNumber(),(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_NUMBER].GetString());
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
				py += SMALL_FONT_Y_GAP;
			}			
			// Pile size
			if (p_item->IsPileItem() == true)
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ITEM_NUM].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "%d %s", p_item->GetNumber(),(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_NUMBER].GetString());
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);			
				py += SMALL_FONT_Y_GAP;
			}
			
			// Charge size
			if (p_item->IsChargeItem() == true)
			{
				vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_LEFT_NUM].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "%d/%d", p_item->GetNumber(), p_item->GetMaxNumber());
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
				py += SMALL_FONT_Y_GAP;
			}
		} else
		{
			// Durability
			if (p_item->GetMaxDurability() != -1&&!p_item->IsUniqueItem()&&p_item->GetItemClass()!=MItem::ITEM_CLASS_VAMPIRE_AMULET && !p_item->IsQuestItem())
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DURABILITY].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "???/???");				
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
				py += SMALL_FONT_Y_GAP;
			}
			// Silvering
			if (p_item->GetSilverMax() != -1)
			{
				vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_SILVERING].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "???/???");				
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
				py += SMALL_FONT_Y_GAP;
			}
			
			// Damage
			if (p_item->GetMaxDamage() != -1) // damage°¡ ÀÖ´Â°¡?
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DAMAGE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "???~???");				
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_RED);
				py += SMALL_FONT_Y_GAP;
			}
			
			// critical hit
			if (p_item->GetCriticalHit() != -1)//Å©¸®Æ®ÄÃ È÷Æ®°¡ ÀÖ´Â°¡?
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_CRITICALHIT].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "???");				
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_RED);
				py += SMALL_FONT_Y_GAP;
			}
			
			// Defense
			if (p_item->GetDefenseValue() != -1)
			{
				vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DEFENSE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "???");				
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_GREEN);				
				py += SMALL_FONT_Y_GAP;
			}
			
			// Protection
			if (p_item->GetProtectionValue() != -1)
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_PROTECTION].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "???");				
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_GREEN);				
				py += SMALL_FONT_Y_GAP;
			}
			
			// TOHIT
			if (p_item->GetToHit() != -1)
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ACCURACY].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "???", p_item->GetToHit());				
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_GREEN);				
				py += SMALL_FONT_Y_GAP;
			}
			
			// Heal point
			if (p_item->GetHealPoint() != -1)
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_HP].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "+???");
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
				py += SMALL_FONT_Y_GAP;
			}
			
			// Mana point
			if (p_item->GetManaPoint() != -1)
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_MP].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "+???");				
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
				py += SMALL_FONT_Y_GAP;
			}
			
			// Attack range
			if (p_item->GetReach() != -1 && p_item->GetReach() != 1)
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_RANGE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "??? %s",(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_TILE_PIECE].GetString());				
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
				py += SMALL_FONT_Y_GAP;
			}
			
			// Bullet
			if (p_item->GetMagazineSize() != -1)
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_MAGAZINE_NUM].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "??? %s",(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_MAGAZINE_COUNT].GetString());				
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
				py += SMALL_FONT_Y_GAP;
			}
			
			// Pocket size
			if (p_item->GetPocketNumber() != -1)
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_POCKET_NUM].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "??? %s", (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_NUMBER].GetString());				
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
				py += SMALL_FONT_Y_GAP;
			}			
			// Pile size
			if (p_item->IsPileItem() == true)
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ITEM_NUM].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "??? %s",(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_NUMBER].GetString());				
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);			
				py += SMALL_FONT_Y_GAP;
			}
			
			// Charge size
			if (p_item->IsChargeItem() == true)
			{
				vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_LEFT_NUM].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "???/???");				
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
				py += SMALL_FONT_Y_GAP;
			}
		}

		/*// Durability
		if (p_item->GetMaxDurability() != -1&&!p_item->IsUniqueItem()&&p_item->GetItemClass()!=MItem::ITEM_CLASS_VAMPIRE_AMULET)
		{
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DURABILITY].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			if(p_item->IsIdentified())
				sprintf(sz_buf, "%d/%d", p_item->GetCurrentDurability(), p_item->GetMaxDurability());
			else
				sprintf(sz_buf, "???/???");
			
			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
			
			py += SMALL_FONT_Y_GAP;
		}
		
		// Silvering
		if (p_item->GetSilverMax() != -1)
		{
			vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_SILVERING].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			if(p_item->IsIdentified())
				sprintf(sz_buf, "%d/%d", p_item->GetSilver(), p_item->GetSilverMax());
			else
				sprintf(sz_buf, "???/???");
			
			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
			
			py += SMALL_FONT_Y_GAP;
		}
		
		// Damage
		if (p_item->GetMaxDamage() != -1) // damage°¡ ÀÖ´Â°¡?
		{
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DAMAGE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			if(p_item->IsIdentified())
				sprintf(sz_buf, "%d~%d", p_item->GetMinDamage(), p_item->GetMaxDamage());
			else
				sprintf(sz_buf, "???~???");
			
			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_RED);
			py += SMALL_FONT_Y_GAP;
		}
		
		// critical hit
		if (p_item->GetCriticalHit() != -1)//Å©¸®Æ®ÄÃ È÷Æ®°¡ ÀÖ´Â°¡?
		{
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_CRITICALHIT].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			if(p_item->IsIdentified())
				sprintf(sz_buf, "%d", p_item->GetCriticalHit());
			else
				sprintf(sz_buf, "???");
			
			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_RED);
			py += SMALL_FONT_Y_GAP;
		}
		
		
		// Defense
		if (p_item->GetDefenseValue() != -1)
		{
			vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DEFENSE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			if(p_item->IsIdentified())
				sprintf(sz_buf, "%d", p_item->GetDefenseValue());
			else
				sprintf(sz_buf, "???");
			
			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_GREEN);
			
			py += SMALL_FONT_Y_GAP;
		}
		
		// Protection
		if (p_item->GetProtectionValue() != -1)
		{
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_PROTECTION].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			if(p_item->IsIdentified())
				sprintf(sz_buf, "%d", p_item->GetProtectionValue());
			else
				sprintf(sz_buf, "???");
			
			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_GREEN);

			py += SMALL_FONT_Y_GAP;
		}

		// TOHIT
		if (p_item->GetToHit() != -1)
		{
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ACCURACY].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			if(p_item->IsIdentified())
				sprintf(sz_buf, "%d", p_item->GetToHit());
			else
				sprintf(sz_buf, "???", p_item->GetToHit());

			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_GREEN);

			py += SMALL_FONT_Y_GAP;
		}

		// Heal point
		if (p_item->GetHealPoint() != -1)
		{
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_HP].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			if(p_item->IsIdentified())
				sprintf(sz_buf, "+%d", p_item->GetHealPoint());
			else
				sprintf(sz_buf, "+???");
			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);

			py += SMALL_FONT_Y_GAP;
		}

		// Mana point
		if (p_item->GetManaPoint() != -1)
		{
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_MP].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			if(p_item->IsIdentified())
				sprintf(sz_buf, "+%d", p_item->GetManaPoint());
			else
				sprintf(sz_buf, "+???");

			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);

			py += SMALL_FONT_Y_GAP;
		}

		// Attack range
		if (p_item->GetReach() != -1 && p_item->GetReach() != 1)
		{
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_RANGE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			if(p_item->IsIdentified())
				sprintf(sz_buf, "%d %s", p_item->GetReach(),(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_TILE_PIECE].GetString());
			else
				sprintf(sz_buf, "??? %s",(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_TILE_PIECE].GetString());

			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);

			py += SMALL_FONT_Y_GAP;
		}

		// Bullet
		if (p_item->GetMagazineSize() != -1)
		{
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_MAGAZINE_NUM].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			if(p_item->IsIdentified())
				sprintf(sz_buf, "%d %s", p_item->GetMagazineSize(),(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_MAGAZINE_COUNT].GetString());
			else
				sprintf(sz_buf, "??? %s",(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_MAGAZINE_COUNT].GetString());

			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
			py += SMALL_FONT_Y_GAP;
		}

		// Pocket size
		if (p_item->GetPocketNumber() != -1)
		{
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_POCKET_NUM].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			if(p_item->IsIdentified())
				sprintf(sz_buf, "%d %s", p_item->GetPocketNumber(),(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_NUMBER].GetString());
			else
				sprintf(sz_buf, "??? %s", (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_NUMBER].GetString());

			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);

			py += SMALL_FONT_Y_GAP;
		}
		
		// Pile size
		if (p_item->IsPileItem() == true)
		{
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ITEM_NUM].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			if(p_item->IsIdentified())
				sprintf(sz_buf, "%d %s", p_item->GetNumber(),(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_NUMBER].GetString());
			else				
				sprintf(sz_buf, "??? %s",(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_NUMBER].GetString());

			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);			
			py += SMALL_FONT_Y_GAP;
		}
		
		// Charge size
		if (p_item->IsChargeItem() == true)
		{
			vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_LEFT_NUM].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			if(p_item->IsIdentified())
				sprintf(sz_buf, "%d/%d", p_item->GetNumber(), p_item->GetMaxNumber());
			else
				sprintf(sz_buf, "???/???");

			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
			
			py += SMALL_FONT_Y_GAP;
		}*/
		if (p_item->IsIdentified() && p_item->GetItemClass() == MItem::ITEM_CLASS_VAMPIRE_PORTAL_ITEM)
		{
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ARRIVAL_LOCATION].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			MVampirePortalItem *temp_item = (MVampirePortalItem *)p_item;

			if(temp_item->IsMarked())
			{
				sprintf(sz_buf, "%s", g_pZoneTable->Get(temp_item->GetZoneID())->Name.GetString());
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
				py += SMALL_FONT_Y_GAP;
				
				sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ZONEINFO_XY].GetString(), temp_item->GetZoneX(), temp_item->GetZoneY());
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
				py += SMALL_FONT_Y_GAP;
			}
			else
			{
				sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_NOT_EXIST].GetString());
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
				py += SMALL_FONT_Y_GAP;
			}
		}
		// Add option
		if(p_item->IsIdentified())
		{			
			/*if (p_item->GetItemOptionListCount() != 0)
			{
				int optionCount = p_item->GetItemOptionListCount();
				ITEMOPTION_INFO& optionInfo = (*g_pItemOptionTable)[option];
				
				char pPartName[20];
				strcpy(pPartName, ITEMOPTION_INFO::ITEMOPTION_PARTNAME[optionInfo.Part]); // OptionÀÌ Àû¿ëµÇ´Â ¼öÄ¡ÀÇ ÀÌ¸§
				if(p_item->IsVampireItem() && strstr(pPartName,"MP") != NULL)
					*strstr(pPartName,"MP") = 'H';
				
				BYTE PlusPoint	= optionInfo.PlusPoint; // Àû¿ëµÇ´Â ¼öÄ¡
				
				if (pPartName)
				{
					fOptionCheck=TRUE;
					vx = g_PrintColorStr(px, py, "¿É¼Ç : ", gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
					if(optionInfo.Part == ITEMOPTION_INFO::PART_DURABILITY)
					{
						sprintf(sz_buf, "%s +%d", pPartName, PlusPoint-100);
						strcat(sz_buf, "%");
					}
					else
						sprintf(sz_buf, "%s +%d", pPartName, PlusPoint);
					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_GOLD);//RGB_YELLOW);
				}
				py += SMALL_FONT_Y_GAP;
			}*/
			BOOL fOptionCheck=FALSE;
			if(!p_item->IsEmptyItemOptionList())
			{			
				const std::list<TYPE_ITEM_OPTION> &optionList=p_item->GetItemOptionList();
				std::list<TYPE_ITEM_OPTION>::const_iterator itr=optionList.begin();				
				
				while(itr != optionList.end() && *itr < g_pItemOptionTable->GetSize())
				{
					ITEMOPTION_INFO& optionInfo=(*g_pItemOptionTable)[*itr];
					char pPartName[20];
#ifdef __ENGLISH_VERSION__
					strcpy(pPartName,g_pItemOptionTable->ITEMOPTION_PARTENAME[optionInfo.Part].GetString());
#else
					strcpy(pPartName,g_pItemOptionTable->ITEMOPTION_PARTNAME[optionInfo.Part].GetString());
#endif
					
					if(p_item->IsVampireItem() && strstr(pPartName,"MP") != NULL)
						*strstr(pPartName,"MP") = 'H';
					
					BYTE PlusPoint	= optionInfo.PlusPoint; // Àû¿ëµÇ´Â ¼öÄ¡
					
					if (pPartName)
					{
						if(!fOptionCheck)
						{
							vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_OPTION].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
							fOptionCheck=TRUE;
						}
								
						if(optionInfo.Part == ITEMOPTION_TABLE::PART_DURABILITY)
						{
							sprintf(sz_buf, "%s +%d", pPartName, PlusPoint-100);
							strcat(sz_buf, "%");
						}
						else
						if(
							optionInfo.Part >= ITEMOPTION_TABLE::PART_STR_TO_DEX &&
							optionInfo.Part <= ITEMOPTION_TABLE::PART_INT_TO_DEX)
						{
							sprintf(sz_buf, "%s %d", pPartName, PlusPoint);
							strcat(sz_buf, "%");
						}
						else
							sprintf(sz_buf, "%s +%d", pPartName, PlusPoint);
						g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_GOLD);//RGB_YELLOW);
					}
					py += SMALL_FONT_Y_GAP;
					itr++;
				}
			}
			//if(!(*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].DefaultOptionList.empty())
			if(!p_item->IsEmptyItemDefaultOption())
			{
				const std::list<TYPE_ITEM_OPTION> &DefaultOptionList = p_item->GetItemDefaultOptionList();
				std::list<TYPE_ITEM_OPTION>::const_iterator itr=DefaultOptionList.begin();
								
				while(itr != DefaultOptionList.end())
				{
					ITEMOPTION_INFO& optionInfo=(*g_pItemOptionTable)[*itr];
					char pPartName[20];
#ifdef __ENGLISH_VERSION__
					strcpy(pPartName,g_pItemOptionTable->ITEMOPTION_PARTENAME[optionInfo.Part].GetString());					
#else
					strcpy(pPartName,g_pItemOptionTable->ITEMOPTION_PARTNAME[optionInfo.Part].GetString());					
#endif
					if(p_item->IsVampireItem() && strstr(pPartName,"MP") != NULL)
						*strstr(pPartName,"MP") = 'H';
					
					BYTE PlusPoint	= optionInfo.PlusPoint; // Àû¿ëµÇ´Â ¼öÄ¡
					
					if (pPartName)
					{
						if(!fOptionCheck)
						{
							vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_OPTION].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
							fOptionCheck=TRUE;
						}
						if(optionInfo.Part == ITEMOPTION_TABLE::PART_DURABILITY)
						{
							sprintf(sz_buf, "%s +%d", pPartName, PlusPoint-100);
							strcat(sz_buf, "%");
						}
						else
						if(
							optionInfo.Part >= ITEMOPTION_TABLE::PART_STR_TO_DEX &&
							optionInfo.Part <= ITEMOPTION_TABLE::PART_INT_TO_DEX)
						{
							sprintf(sz_buf, "%s %d", pPartName, PlusPoint);
							strcat(sz_buf, "%");
						}
						else
							sprintf(sz_buf, "%s +%d", pPartName, PlusPoint);
						g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_GOLD);//RGB_YELLOW);
					}
					py += SMALL_FONT_Y_GAP;
					itr++;
				}				
			}
		}
		else
		{
			sprintf(sz_buf,"%s???????",(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_OPTION].GetString());
			vx = g_PrintColorStr(px, py, sz_buf, gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			
			py += SMALL_FONT_Y_GAP;
		}
		
		bool bl_required = false;

		if(p_item->GetItemClass() != MItem::ITEM_CLASS_COUPLE_RING && p_item->GetItemClass() != MItem::ITEM_CLASS_VAMPIRE_COUPLE_RING &&
			!p_item->IsQuestItem())
		{
			
			if(p_item->IsIdentified())
			{
				// Required
				if(p_item->IsSlayerItem())
				{
					char sz_and[] = ",";
					if (p_item->GetRequireSTR() > 0)
					{
						if(!bl_required)
							vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
						else
							vx = px + g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi.hfont);
						bl_required = true;
						
						vx = g_PrintColorStr(vx, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_STR].GetString(), gpC_base->m_item_desc_pi, required_rgb);
						sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE_STAT].GetString(), p_item->GetRequireSTR());
						vx = g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_GREEN);
						
						if (p_item->GetRequireDEX() > 0 ||
							p_item->GetRequireINT() > 0 ||
							//					p_item->GetRequireLevel() > 0 ||
							(p_item->GetRequireSUM() > 0 && p_item->GetRequireSTR() < p_item->GetRequireSUM()))
							g_PrintColorStr(vx+3, py, sz_and, gpC_base->m_item_desc_pi, required_rgb);
						
						py += SMALL_FONT_Y_GAP;
					}
					
					if (p_item->GetRequireDEX() > 0)
					{
						if(!bl_required)
							vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
						else
							vx = px + g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi.hfont);
						bl_required = true;
						
						vx = g_PrintColorStr(vx, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_DEX].GetString(), gpC_base->m_item_desc_pi, required_rgb);
						sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE_STAT].GetString(), p_item->GetRequireDEX());
						vx = g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_GREEN);
						
						if (p_item->GetRequireINT() > 0 ||
							//					p_item->GetRequireLevel() > 0 ||
							(p_item->GetRequireSUM() > 0 && p_item->GetRequireDEX() < p_item->GetRequireSUM()))
							g_PrintColorStr(vx+3, py, sz_and, gpC_base->m_item_desc_pi, required_rgb);
						
						py += SMALL_FONT_Y_GAP;
					}
					
					if (p_item->GetRequireINT() > 0)
					{
						if(!bl_required)
							vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
						else
							vx = px + g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi.hfont);
						bl_required = true;
						
						vx = g_PrintColorStr(vx, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_ENG_INT].GetString(), gpC_base->m_item_desc_pi, required_rgb);
						sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE_STAT].GetString(), p_item->GetRequireINT());
						vx = g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_GREEN);
						
						if (//p_item->GetRequireLevel() > 0 ||
							(p_item->GetRequireSUM() > 0 && p_item->GetRequireINT() < p_item->GetRequireSUM()))
							g_PrintColorStr(vx, py, sz_and, gpC_base->m_item_desc_pi, required_rgb);
						
						py += SMALL_FONT_Y_GAP;
					}
					
					if (p_item->GetRequireSUM() > 0 &&
						p_item->GetRequireSUM() > p_item->GetRequireSTR() &&
						p_item->GetRequireSUM() > p_item->GetRequireDEX() &&
						p_item->GetRequireSUM() > p_item->GetRequireINT() )
					{
						if(!bl_required)
							vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
						else
							vx = px + g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi.hfont);
						bl_required = true;
						
						vx = g_PrintColorStr(vx, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ALL_STAT_SUM].GetString(), gpC_base->m_item_desc_pi, required_rgb);
						sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE_STAT].GetString(), p_item->GetRequireSUM());
						vx = g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_GREEN);
						
						//				if (p_item->GetRequireLevel() > 0)
						//					g_PrintColorStr(vx+3, py, sz_and, gpC_base->m_item_desc_pi, required_rgb);
						
						py += SMALL_FONT_Y_GAP;
					}
				}
				else if(p_item->IsVampireItem())
				{
					if (p_item->GetRequireLevel() > 0)
					{
						//			if(!bl_required)
						vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
						//			else
						//				vx = px + g_GetStringWidth("Required", gpC_base->m_item_desc_pi.hfont);
						bl_required = true;
						
						vx = g_PrintColorStr(vx, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_LEVEL].GetString(), gpC_base->m_item_desc_pi, required_rgb);
						sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE_STAT].GetString(), p_item->GetRequireLevel());
						vx = g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_GREEN);
						
						py += SMALL_FONT_Y_GAP;
					}
				}
				
				if(!p_item->IsVampireItem() && gbl_vampire_interface)
				{
					if(!bl_required)
						vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
					else
						vx = px + g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi.hfont);
					bl_required = true;
					
					vx = g_PrintColorStr(vx, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ONLY_SLAYER].GetString(), gpC_base->m_item_desc_pi, required_rgb);
					py += SMALL_FONT_Y_GAP;
				}
				else if(!p_item->IsSlayerItem() && !gbl_vampire_interface)
				{
					if(!bl_required)
						vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
					else
						vx = px + g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi.hfont);
					bl_required = true;
					
					vx = g_PrintColorStr(vx, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ONLY_VAMPIRE].GetString(), gpC_base->m_item_desc_pi, required_rgb);
					py += SMALL_FONT_Y_GAP;
				}
				
				if (p_item->IsGenderForMale())
				{
					if(!bl_required)
						vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
					else
						vx = px + g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi.hfont);
					bl_required = true;
					
					vx = g_PrintColorStr(vx, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ONLY_MALE].GetString(), gpC_base->m_item_desc_pi, required_rgb);
					py += SMALL_FONT_Y_GAP;
				}
				else if (p_item->IsGenderForFemale())
				{
					if(!bl_required)
						vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
					else
						vx = px + g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi.hfont);
					bl_required = true;
					
					vx = g_PrintColorStr(vx, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ONLY_FEMALE].GetString(), gpC_base->m_item_desc_pi, required_rgb);
					py += SMALL_FONT_Y_GAP;
				}
			}
			else
			{
				sprintf(sz_buf,"%s????????",(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString());
				vx = g_PrintColorStr(px, py, sz_buf, gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				py += SMALL_FONT_Y_GAP;
			}
		}
	}

//	if(p_item->GetItemClass() != MItem::ITEM_CLASS_EVENT_GIFT_BOX)
	{
		
		// Price
		if (gbl_sell_running == true)
		{
			assert(gbl_buy_running == false);
			
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_PRICE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			
			if(p_item->IsPileItem() && p_item->GetNumber() >= 1)
			{
				int price = max( 0, g_pPriceManager->GetItemPrice(p_item, MPriceManager::PC_TO_NPC) );
				
				if( g_pTimeItemManager->IsExist( p_item->GetID() ) )
					price = 50;

				sprintf(sz_buf, "%d", price*max(0,p_item->GetNumber()));
				std::string sstr = sz_buf;
				for(int i = 3; i <= 13; i += 4)
					if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
				sprintf(sz_buf, "$%s", sstr.c_str());
				vx = g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
				sprintf(sz_buf, "(%dx%d)", price,max(0, p_item->GetNumber()));
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, required_rgb);
			}
			else 
			{
				sprintf(sz_buf, "%d", g_pTimeItemManager->IsExist(p_item->GetID()) ? 50 : max( 0, g_pPriceManager->GetItemPrice(p_item, MPriceManager::PC_TO_NPC)) );
				std::string sstr = sz_buf;
				for(int i = 3; i <= 13; i += 4)
					if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
				sprintf(sz_buf, "$%s", sstr.c_str());
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
			}
			
			py += SMALL_FONT_Y_GAP;
		}
		
		if (gbl_buy_running == true && !g_pTimeItemManager->IsExist( p_item->GetID() ) )
		{
			assert(gbl_sell_running == false);			
			// check mysterious
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_PRICE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			if(C_VS_UI_SHOP::m_shop_type == MShop::SHOP_EVENT_STAR && left)
			{
				STAR_ITEM_PRICE price;
				g_pPriceManager->GetItemPrice(p_item, price);
				
				if(price.type != -1 && price.number > 0)
				{
					wsprintf(sz_buf, "%s %d%s", (*g_pItemTable)[MItem::ITEM_CLASS_EVENT_STAR][price.type].HName.GetString(), max( 0, price.number) , (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_NUMBER].GetString());
				}
				else
				{
					wsprintf(sz_buf, (*g_pGameStringTable)[STRING_MESSAGE_CANNOT_BUY].GetString());
				}
			}
			else
			{
//				sprintf(sz_buf, "%d", g_pPriceManager->GetItemPrice(p_item, MPriceManager::NPC_TO_PC, C_VS_UI_SHOP::m_bl_mysterious_tab));
				if(p_item->IsPileItem() && p_item->GetNumber() >= 1)
				{
					int price = max( 0, g_pPriceManager->GetItemPrice(p_item, MPriceManager::NPC_TO_PC) );
					sprintf(sz_buf, "%d", price*p_item->GetNumber());
					std::string sstr = sz_buf;
					for(int i = 3; i <= 13; i += 4)
						if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
					sprintf(sz_buf, "$%s", sstr.c_str());
					vx = g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
					sprintf(sz_buf, "(%dx%d)", price, max(p_item->GetNumber(),0) );
					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, required_rgb);
				}
				else 
				{
					sprintf(sz_buf, "%d", max( 0, g_pPriceManager->GetItemPrice(p_item, MPriceManager::NPC_TO_PC) ));
					std::string sstr = sz_buf;
					for(int i = 3; i <= 13; i += 4)
						if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
					sprintf(sz_buf, "$%s", sstr.c_str());
					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
				}
			}
			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
			
			py += SMALL_FONT_Y_GAP;
		}
		
		if (gbl_repair_running == true && !g_pTimeItemManager->IsExist( p_item->GetID() ) )
		{
			assert(gbl_buy_running == false);
			assert(gbl_sell_running == false);			
			if(p_item->IsChargeItem())
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_CHARGE_PRICE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			else
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REPAIR_PRICE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			sprintf(sz_buf, "%d", max( 0, g_pPriceManager->GetItemPrice(p_item, MPriceManager::REPAIR) ));
			std::string sstr = sz_buf;
			for(int i = 3; i <= 13; i += 4)
				if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
			sprintf(sz_buf, "$%s", sstr.c_str());
			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
			
			py += SMALL_FONT_Y_GAP;
		}
		
		if (gbl_silvering_running == true)
		{
			assert(gbl_buy_running == false);
			assert(gbl_sell_running == false);
			assert(gbl_repair_running == false);			
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_SILVERING_PRICE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			sprintf(sz_buf, "%d", max( 0, g_pPriceManager->GetItemPrice(p_item, MPriceManager::SILVERING)));
			std::string sstr = sz_buf;
			for(int i = 3; i <= 13; i += 4)
				if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
			sprintf(sz_buf, "$%s", sstr.c_str());
			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
			
			py += SMALL_FONT_Y_GAP;
		}
		
	}

//	MString temp_string = (*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].Description;
	if((*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].Description.GetLength() != 0)
		g_PrintColorStr(px, py, (*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].Description.GetString(), gpC_base->m_item_desc_pi, required_rgb);
	
	if(gpC_mouse_pointer->GetPickUpItem() != NULL && gpC_mouse_pointer->GetPickUpItem()->GetItemClass() == MItem::ITEM_CLASS_EVENT_STAR && p_item->IsGearItem())
	{
		const std::list<TYPE_ITEM_OPTION> &optionList = p_item->GetItemOptionList();
		std::list<TYPE_ITEM_OPTION>::const_iterator itr = optionList.begin();	
		
		bool bCanUpgrade = false;
	
		if(p_item->IsUniqueItem() || p_item->IsQuestItem())
		{
			bCanUpgrade = false;
		} else			
		if(gpC_mouse_pointer->GetPickUpItem()->GetItemType() > 7)
		{
			if(p_item->GetItemOptionListCount() == 0)
				bCanUpgrade = true;
		} else
		{
			while(itr!=optionList.end())
			{
				if((*g_pItemOptionTable)[*itr].UpgradeOptionType != 0)
				{
					bCanUpgrade = true;
					break;
				}
				itr++;
			}
		}
		if(p_item->GetItemOptionListCount()>1)
			bCanUpgrade = false;

		if(bCanUpgrade)
		{
			if(gpC_mouse_pointer->GetPickUpItem()->GetNumber() != 1)
				g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_ONLY_PICK_UP_ITEM_ONE].GetString(), gpC_base->m_item_desc_pi, RGB(150, 150, 255));
			else
				g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CAN_ENCHANT].GetString(), gpC_base->m_item_desc_pi, RGB(150, 150, 255));
		}
		else
			g_PrintColorStr(px, py, (*g_pGameStringTable)[STRING_MESSAGE_ITEM_TO_ITEM_IMPOSIBLE].GetString(), gpC_base->m_item_desc_pi, RGB(255, 150, 150));

		py += SMALL_FONT_Y_GAP;
	}
	
	if(g_pTimeItemManager->IsExist( p_item->GetID() ) )
	{
		char	temp[256] = {0,};	
		bool	bContinue = false;
		TYPE_OBJECTID objectID = p_item->GetID();		
		
		int tempInt;
		vx=px;
		
		if( g_pTimeItemManager->GetDay( objectID ) == 0 && g_pTimeItemManager->GetHour (objectID ) ==0 &&
			g_pTimeItemManager->GetMinute( objectID ) == 0 )
		{
			if( g_pTimeItemManager->GetSecond( objectID ) == 0 )
			{
				strcpy(temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_EXPIRED_ITEM].GetString() );
			} else
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_LEFT_TIME].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB );
				strcat(temp, " ");
				char tempSecond[10];
				wsprintf(tempSecond, (*g_pGameStringTable)[UI_STRING_MESSAGE_SECOND].GetString() , g_pTimeItemManager->GetSecond( objectID ) );
				strcat(temp, tempSecond );
			}
		} else
		{
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_LEFT_TIME].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB );
			memset( temp, 0, sizeof( temp ) );

			if((tempInt = g_pTimeItemManager->GetDay( objectID ))>0  || bContinue )
			{
				char tempDay[30] = {0,};
				strcat(temp, " ");
				wsprintf(tempDay, (*g_pGameStringTable)[UI_STRING_MESSAGE_DAY].GetString() , tempInt );
				strcat ( temp, tempDay );
				bContinue = true;
			}
			if((tempInt = g_pTimeItemManager->GetHour( objectID ))>0 || bContinue)
			{
				char tempDay[30] = {0,};
				strcat(temp, " ");
				wsprintf(tempDay, (*g_pGameStringTable)[UI_STRING_MESSAGE_HOUR].GetString() , tempInt );
				strcat ( temp, tempDay );
				bContinue = true;
			}
			if((tempInt = g_pTimeItemManager->GetMinute( objectID ))>0  || bContinue)
			{
				char tempDay[30] = {0,};
				strcat(temp, " ");
				wsprintf(tempDay, (*g_pGameStringTable)[UI_STRING_MESSAGE_MINUTE].GetString() , tempInt );
				strcat ( temp, tempDay );
				bContinue = true;
			}			
		}		
		g_PrintColorStr(vx, py, temp,gpC_base->m_item_desc_pi,RGB(255,255,255));
		py += SMALL_FONT_Y_GAP;
	}
	
	g_FL2_ReleaseDC();
}

//-----------------------------------------------------------------------------
// _Skill_Description_Show
//
// 
//-----------------------------------------------------------------------------
void	_Skill_Description_Show(Rect rect, void * void_ptr, long left, long right)
{
//#ifdef _LIB // Client¿¡¼­´Â 3D¿ëÀ¸·Î »ç¿ëÇÒ ¼ö ÀÖ´Ù. -.-
	RECT _rect;
	SetRect(&_rect, rect.x, rect.y, rect.x+rect.w, rect.y+rect.h);
	DrawAlphaBox(&_rect, 0, 0, 0, g_pUserOption->ALPHA_DEPTH);
//#endif

//	S_RECT dest_rect;
//	S_SURFACEINFO	dest_surface;

//#ifndef _LIB
//	if (gpC_base->m_DDSurface_offscreen.Lock())
//	{
//		S_SURFACEINFO src_surface;
//		S_RECT src_rect;
//
//		SetSurfaceInfo(&src_surface, gpC_base->m_DDSurface_offscreen.GetDDSD());
//
//		//SetRect(src_rect, 0, 0, rect.w, rect.h);
//		//filledRect(&src_surface, &src_rect, BLACK);
//#endif
//		if (gpC_base->m_p_DDSurface_back->Lock()) // Descriptor common background¸¦ Ãâ·ÂÇÑ´Ù.
//		{
//			SetSurfaceInfo(&dest_surface, gpC_base->m_p_DDSurface_back->GetDDSD());
//			SetRect(dest_rect, rect.x, rect.y, rect.w, rect.h);
//
//#ifndef _LIB
//			//TransparentBlt16(&dest_surface, &src_surface, &dest_rect, &src_rect);
//			filledRect(&dest_surface, &dest_rect, BLACK);
//#endif
			SetRect(&_rect, rect.x, rect.y, rect.x+rect.w, rect.y+rect.h); // -.- TransparentBlt16()¿¡¼­ °ªÀ» º¯°æÇÔ.. ¾Æ´Ï ÀÌ·²¼ö°¡!
			gpC_base->m_p_DDSurface_back->DrawRect(&_rect, LIGHT_MAGENTA);
//			rectangle(&dest_surface, dest_rect.x, dest_rect.y, dest_rect.x+dest_rect.w-1, dest_rect.y+dest_rect.h-1, LIGHT_MAGENTA);
//
//			gpC_base->m_p_DDSurface_back->Unlock();
//		}
//
//#ifndef _LIB
//		gpC_base->m_DDSurface_offscreen.Unlock();
//	}
//#endif

	//
	// show contents
	//
	int px = rect.x+SIDE_GAP_HALF;
	int py = rect.y+SIDE_GAP_HALF;

	char sz_buf[50];

	g_FL2_GetDC();
	// name
	// optionÀÌ ºÙ¾îÀÖÀ¸¸é ´Ù¸¥ »öÀ¸·Î Ãâ·ÂÇÑ´Ù.
	g_PrintColorStr(px, py, (*g_pSkillInfoTable)[left].GetName(), gpC_base->m_item_name_pi, RGB_GRAY);

	py += NORMAL_FONT_Y_GAP;
	int vx;

	if(left == SKILL_LOVE_CHAIN)
	{
		vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_LOVE_CHAIN].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
		
		g_FL2_ReleaseDC();
		return;
	}

	if (!gbl_vampire_interface && !(*g_pSkillInfoTable)[left].IsPassive())
	{
		// level
		sprintf(sz_buf,"%s: ",(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_LEVEL].GetString());
		vx = g_PrintColorStr(px, py, sz_buf, gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
		sprintf(sz_buf, "%d", (*g_pSkillInfoTable)[left].GetExpLevel());
		g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);

		py += SMALL_FONT_Y_GAP;
	}

	// MP
	if(!(*g_pSkillInfoTable)[left].IsPassive())
	{
		if(gbl_vampire_interface)
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CONSUME_HP].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
		else
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CONSUME_MP].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);

		if( left == SKILL_WILL_OF_LIFE )
			sprintf(sz_buf, "%d",5 + (g_char_slot_ingame.level / 7));
		else
			sprintf(sz_buf, "%d", (*g_pSkillInfoTable)[left].GetMP());
		g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
	}
	else
	{
		if(left >= SKILL_HOLYLAND_BLOOD_BIBLE_ARMEGA && left <= SKILL_HOLYLAND_BLOOD_BIBLE_CHASPA)
			g_PrintColorStr(px, py, (*g_pGameStringTable)[STRING_MESSAGE_BLOOD_BIBLE_BONUS_ARMEGA+(left-SKILL_HOLYLAND_BLOOD_BIBLE_ARMEGA)].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
		else
			g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_HAN_PASSIVE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
	}

	//py += SMALL_FONT_Y_GAP;

	g_FL2_ReleaseDC();
}

//-----------------------------------------------------------------------------
// _BloodBible_Description_Show
//
// 
//-----------------------------------------------------------------------------
void	_BloodBible_Description_Show(Rect rect, void * void_ptr, long left, long right)
{
	S_BLOOD_BIBLE_DESCRIPTION *desc = (S_BLOOD_BIBLE_DESCRIPTION*)void_ptr;
	COLORREF color = (COLORREF) right;
	RECT _rect;
	SetRect(&_rect, rect.x, rect.y, rect.x+rect.w, rect.y+rect.h);

	DrawAlphaBox(&_rect, 0, 0, 0, g_pUserOption->ALPHA_DEPTH);
	SetRect(&_rect, rect.x, rect.y, rect.x+rect.w, rect.y+rect.h); // -.- TransparentBlt16()¿¡¼­ °ªÀ» º¯°æÇÔ.. ¾Æ´Ï ÀÌ·²¼ö°¡!
	gpC_base->m_p_DDSurface_back->DrawRect(&_rect, LIGHT_MAGENTA);

	int px = rect.x+SIDE_GAP_HALF;
	int py = rect.y+SIDE_GAP_HALF;


	g_FL2_GetDC();
	// name
	// optionÀÌ ºÙ¾îÀÖÀ¸¸é ´Ù¸¥ »öÀ¸·Î Ãâ·ÂÇÑ´Ù.
	
	if(desc->sz_name != NULL)
	{
		int vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_NAME].GetString() , gpC_base->m_item_name_pi, RGB_YELLOW);
		g_PrintColorStr(vx, py, desc->sz_name , gpC_base->m_item_name_pi, RGB_YELLOW);
		py += NORMAL_FONT_Y_GAP;
	}

	if(desc->sz_place != NULL)
	{		
		int vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_DESC_POSITION].GetString(),gpC_base->m_item_desc_pi,ITEM_DESC_RGB);
		g_PrintColorStr(vx, py, desc->sz_place , gpC_base->m_item_desc_pi, RGB_WHITE);
		py += SMALL_FONT_Y_GAP;
	}
	
	if(desc->sz_shrine != NULL)
	{		
		int tempColor;
		int vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_DESC_SHRINE_RACE].GetString(), gpC_base->m_item_desc_pi,ITEM_DESC_RGB);
		if(strcmp(desc->sz_shrine, (*g_pGameStringTable)[STRING_MESSAGE_SLAYER].GetString()) == 0)
			tempColor = RGB(23,107,255);
		else
			tempColor = RGB_RED;

		g_PrintColorStr(vx, py, desc->sz_shrine , gpC_base->m_item_desc_pi, tempColor);
		py += SMALL_FONT_Y_GAP;
	}
	
	if(desc->sz_player != NULL)
	{		
		int vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_DESC_PLAYER].GetString(),gpC_base->m_item_desc_pi,ITEM_DESC_RGB);
		g_PrintColorStr(vx, py, desc->sz_player , gpC_base->m_item_desc_pi, MAGENTA);
		py += SMALL_FONT_Y_GAP;
	}
	
	if(desc->sz_option != NULL)
	{
		int vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_OPTION].GetString(),gpC_base->m_item_desc_pi,ITEM_DESC_RGB);
		g_PrintColorStr(vx, py, desc->sz_option , gpC_base->m_item_desc_pi, RGB_GOLD);
		py += SMALL_FONT_Y_GAP;		
	}

	if(desc->sz_status != NULL)
	{
		int vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_DESC_STATUS].GetString(),gpC_base->m_item_desc_pi,ITEM_DESC_RGB);
		g_PrintColorStr(vx, py, desc->sz_status , gpC_base->m_item_desc_pi, color);
		py += SMALL_FONT_Y_GAP;
	}
	
	g_FL2_ReleaseDC();
}


//-----------------------------------------------------------------------------
// _Strings_Description_Show
//
// 
//-----------------------------------------------------------------------------
void	_Strings_Description_Show(Rect rect, void * void_ptr, long left, long right)
{
	LPSTR* ptr = (LPSTR *)void_ptr;

//#ifdef _LIB // Client¿¡¼­´Â 3D¿ëÀ¸·Î »ç¿ëÇÒ ¼ö ÀÖ´Ù. -.-
	RECT _rect;
	SetRect(&_rect, rect.x, rect.y, rect.x+rect.w, rect.y+rect.h);
	DrawAlphaBox(&_rect, 0, 0, 0, g_pUserOption->ALPHA_DEPTH);
//#endif
//
//	S_RECT dest_rect;
//	S_SURFACEINFO	dest_surface;
//
//#ifndef _LIB
//	if (gpC_base->m_DDSurface_offscreen.Lock())
//	{
//		S_SURFACEINFO src_surface;
//		S_RECT src_rect;
//
//		SetSurfaceInfo(&src_surface, gpC_base->m_DDSurface_offscreen.GetDDSD());
//
//		//SetRect(src_rect, 0, 0, rect.w, rect.h);
//		//filledRect(&src_surface, &src_rect, BLACK);
//#endif
//		if (gpC_base->m_p_DDSurface_back->Lock()) // Descriptor common background¸¦ Ãâ·ÂÇÑ´Ù.
//		{
//			SetSurfaceInfo(&dest_surface, gpC_base->m_p_DDSurface_back->GetDDSD());
//			SetRect(dest_rect, rect.x, rect.y, rect.w, rect.h);
//
//#ifndef _LIB
//			//TransparentBlt16(&dest_surface, &src_surface, &dest_rect, &src_rect);
//			filledRect(&dest_surface, &dest_rect, BLACK);
//#endif
			SetRect(&_rect, rect.x, rect.y, rect.x+rect.w, rect.y+rect.h); // -.- TransparentBlt16()¿¡¼­ °ªÀ» º¯°æÇÔ.. ¾Æ´Ï ÀÌ·²¼ö°¡!
			gpC_base->m_p_DDSurface_back->DrawRect(&_rect, LIGHT_MAGENTA);
//			rectangle(&dest_surface, dest_rect.x, dest_rect.y, dest_rect.x+dest_rect.w-1, dest_rect.y+dest_rect.h-1, LIGHT_MAGENTA);
//
//			gpC_base->m_p_DDSurface_back->Unlock();
//		}
//
//#ifndef _LIB
//		gpC_base->m_DDSurface_offscreen.Unlock();
//	}
//#endif

	//
	// show contents
	//
	int px = rect.x+SIDE_GAP_HALF;
	int py = rect.y+SIDE_GAP_HALF;

	g_FL2_GetDC();

	for(int i = 0; i < left; i++)
	{
		g_PrintColorStr(px, py, ptr[i], gpC_base->m_item_name_pi, RGB_WHITE);
		py += NORMAL_FONT_Y_GAP;
	}

	g_FL2_ReleaseDC();
}

//-----------------------------------------------------------------------------
// _Info_Description_Show
//
// 
//-----------------------------------------------------------------------------
void	_Info_Description_Show(Rect rect, void * void_ptr, long left, long right)
{
//#ifdef _LIB // Client¿¡¼­´Â 3D¿ëÀ¸·Î »ç¿ëÇÒ ¼ö ÀÖ´Ù. -.-
	RECT _rect;
	SetRect(&_rect, rect.x, rect.y, rect.x+rect.w, rect.y+rect.h);
	DrawAlphaBox(&_rect, 0, 0, 0, g_pUserOption->ALPHA_DEPTH);
//#endif
//
//	S_RECT dest_rect;
//	S_SURFACEINFO	dest_surface;
//
//#ifndef _LIB
//	if (gpC_base->m_DDSurface_offscreen.Lock())
//	{
//		S_SURFACEINFO src_surface;
//		S_RECT src_rect;
//
//		SetSurfaceInfo(&src_surface, gpC_base->m_DDSurface_offscreen.GetDDSD());
//
//		//SetRect(src_rect, 0, 0, rect.w, rect.h);
//		//filledRect(&src_surface, &src_rect, BLACK);
//#endif
//		if (gpC_base->m_p_DDSurface_back->Lock()) // Descriptor common background¸¦ Ãâ·ÂÇÑ´Ù.
//		{
//			SetSurfaceInfo(&dest_surface, gpC_base->m_p_DDSurface_back->GetDDSD());
//			SetRect(dest_rect, rect.x, rect.y, rect.w, rect.h);
//
//#ifndef _LIB
//			//TransparentBlt16(&dest_surface, &src_surface, &dest_rect, &src_rect);
//			filledRect(&dest_surface, &dest_rect, BLACK);
//#endif
			SetRect(&_rect, rect.x, rect.y, rect.x+rect.w, rect.y+rect.h); // -.- TransparentBlt16()¿¡¼­ °ªÀ» º¯°æÇÔ.. ¾Æ´Ï ÀÌ·²¼ö°¡!
			gpC_base->m_p_DDSurface_back->DrawRect(&_rect, LIGHT_MAGENTA);
//			rectangle(&dest_surface, dest_rect.x, dest_rect.y, dest_rect.x+dest_rect.w-1, dest_rect.y+dest_rect.h-1, LIGHT_MAGENTA);
//
//			gpC_base->m_p_DDSurface_back->Unlock();
//		}
//
//#ifndef _LIB
//		gpC_base->m_DDSurface_offscreen.Unlock();
//	}
//#endif

	//
	// show contents
	//
	int px = rect.x+SIDE_GAP_HALF;
	int py = rect.y+SIDE_GAP_HALF;

	if(left == 0)left = RGB_WHITE;
	g_PrintColorStr(px, py, (LPSTR)void_ptr, gpC_base->m_item_name_pi, left);
	py += NORMAL_FONT_Y_GAP;
}

//-----------------------------------------------------------------------------
// _SkillTree_Description_Show
//
// 
//-----------------------------------------------------------------------------
void	_SkillTree_Description_Show(Rect rect, void * void_ptr, long left, long right)
{
//#ifdef _LIB // Client¿¡¼­´Â 3D¿ëÀ¸·Î »ç¿ëÇÒ ¼ö ÀÖ´Ù. -.-
	RECT _rect;
	SetRect(&_rect, rect.x, rect.y, rect.x+rect.w, rect.y+rect.h);
	DrawAlphaBox(&_rect, 0, 0, 0, g_pUserOption->ALPHA_DEPTH);
//#endif
//
//	S_RECT dest_rect;
//	S_SURFACEINFO	dest_surface;
//
//#ifndef _LIB
//	if (gpC_base->m_DDSurface_offscreen.Lock())
//	{
//		S_SURFACEINFO src_surface;
//		S_RECT src_rect;
//
//		SetSurfaceInfo(&src_surface, gpC_base->m_DDSurface_offscreen.GetDDSD());
//
//		//SetRect(src_rect, 0, 0, rect.w, rect.h);
//		//filledRect(&src_surface, &src_rect, BLACK);
//#endif
//		if (gpC_base->m_p_DDSurface_back->Lock()) // Descriptor common background¸¦ Ãâ·ÂÇÑ´Ù.
//		{
//			SetSurfaceInfo(&dest_surface, gpC_base->m_p_DDSurface_back->GetDDSD());
//			SetRect(dest_rect, rect.x, rect.y, rect.w, rect.h);
//
//#ifndef _LIB
//			//TransparentBlt16(&dest_surface, &src_surface, &dest_rect, &src_rect);
//			filledRect(&dest_surface, &dest_rect, BLACK);
//#endif
			SetRect(&_rect, rect.x, rect.y, rect.x+rect.w, rect.y+rect.h); // -.- TransparentBlt16()¿¡¼­ °ªÀ» º¯°æÇÔ.. ¾Æ´Ï ÀÌ·²¼ö°¡!
			gpC_base->m_p_DDSurface_back->DrawRect(&_rect, LIGHT_MAGENTA);
//			rectangle(&dest_surface, dest_rect.x, dest_rect.y, dest_rect.x+dest_rect.w-1, dest_rect.y+dest_rect.h-1, LIGHT_MAGENTA);
//
//			gpC_base->m_p_DDSurface_back->Unlock();
//		}
//
//#ifndef _LIB
//		gpC_base->m_DDSurface_offscreen.Unlock();
//	}
//#endif

	//
	// show contents
	//
	int px = rect.x+SIDE_GAP_HALF;
	int py = rect.y+SIDE_GAP_HALF;

	char sz_buf[50];

	g_FL2_GetDC();
	// name
	// optionÀÌ ºÙ¾îÀÖÀ¸¸é ´Ù¸¥ »öÀ¸·Î Ãâ·ÂÇÑ´Ù.
	g_PrintColorStr(px, py, (*g_pSkillInfoTable)[left].GetName(), gpC_base->m_item_name_pi, RGB_GRAY);

	py += NORMAL_FONT_Y_GAP;
	int vx;

	MSkillDomain::SKILLSTATUS status = (*g_pSkillManager)[*(SKILLDOMAIN *)void_ptr].GetSkillStatus((ACTIONINFO)left);

	// level
	{
		vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_REQUIRE_LEVEL].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
		if((*g_pSkillInfoTable)[left].GetLearnLevel() >= 0)
		{
			sprintf(sz_buf, "%d", (*g_pSkillInfoTable)[left].GetLearnLevel());
			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
		}
		py += SMALL_FONT_Y_GAP;
	}
	if(status==MSkillDomain::SKILLSTATUS_LEARNED)
	{
		if (!gbl_vampire_interface && !(*g_pSkillInfoTable)[left].IsPassive())
		{
			sprintf(sz_buf,"%s: ",(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_LEVEL].GetString());
			vx = g_PrintColorStr(px, py, sz_buf, gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			sprintf(sz_buf, "%d", (*g_pSkillInfoTable)[left].GetExpLevel());
			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
			py += SMALL_FONT_Y_GAP;
		}
	}
	

	// MP
	if(!(*g_pSkillInfoTable)[left].IsPassive())
	{
		if(gbl_vampire_interface)
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CONSUME_HP].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
		else
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CONSUME_MP].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);

		sprintf(sz_buf, "%d", (*g_pSkillInfoTable)[left].GetMP());
		g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
	}
	else
	{
		//g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_HAN_PASSIVE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
		if(left >= SKILL_HOLYLAND_BLOOD_BIBLE_ARMEGA && left <= SKILL_HOLYLAND_BLOOD_BIBLE_CHASPA)
		{
			g_PrintColorStr(px, py, (*g_pGameStringTable)[STRING_MESSAGE_BLOOD_BIBLE_BONUS_ARMEGA+left-SKILL_HOLYLAND_BLOOD_BIBLE_ARMEGA].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
		}
		else
			g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_HAN_PASSIVE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);

	}
	py += SMALL_FONT_Y_GAP;

//	assert(status != MSkillDomain::SKILLSTATUS_NULL);

	// »õ·Î¿î skillÀ» ¹è¿ï ¼ö ÀÖ´Â »óÅÂÀÎÁö Ã¼Å©ÇØ¾ßÇÔ... by sigi
	if(status==MSkillDomain::SKILLSTATUS_LEARNED)
	{
		if(!gbl_vampire_interface)
		{
			if((*g_pSkillInfoTable)[(ACTIONINFO)left].IsPassive())
			{
//				g_PrintColorStr(px, py, "Passive", gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			}
			else
			{
				int domain = (*g_pSkillInfoTable)[(ACTIONINFO)left].DomainType;
				int max_exp = (*g_pSkillInfoTable)[(ACTIONINFO)left].maxExp;
				int exp = (*g_pSkillInfoTable)[(ACTIONINFO)left].GetSkillExp();
				int exp_percent;
				if(max_exp > 0)
					exp_percent = (exp % (max_exp/100))*100/(max_exp/100);
				else exp_percent = 0;

				if(exp == max_exp)exp_percent = 100;

				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_EXP].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				wsprintf(sz_buf, "%d", exp_percent);
				strcat(sz_buf, "%");
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
				py += SMALL_FONT_Y_GAP;

				if(domain >= 0)
				{
					int domain_grade = 	max((*g_pSkillManager)[domain].GetDomainLevel()/25,
						(*g_pSkillInfoTable)[(ACTIONINFO)left].GetLearnLevel()/25);
					//int learn_level_grade = (*g_pSkillInfoTable)[(ACTIONINFO)left].GetLearnLevel()/25;
										
//					int skill_grade = (*g_pSkillInfoTable)[(ACTIONINFO)left].GetSkillStep() - SKILL_STEP_APPRENTICE;					
//					if(domain_grade != SKILL_STEP_GRAND_MASTER - SKILL_STEP_APPRENTICE && domain_grade - skill_grade >= 0 && domain_grade - skill_grade <= 2)
					
					if(domain_grade>= 0 && domain_grade< 3)
					{
						vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_LIMIT_LEVEL].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
						switch(domain_grade)
						{
						case 0:
							g_PrintColorStr(vx, py, "49", gpC_base->m_item_desc_pi, RGB_YELLOW);
							break;
							
						case 1:
							g_PrintColorStr(vx, py, "74", gpC_base->m_item_desc_pi, RGB_YELLOW);
							break;
							
						case 2:
							g_PrintColorStr(vx, py, "99", gpC_base->m_item_desc_pi, RGB_YELLOW);
							break;
						}
					}
				}
			}
		}
	}
	else// if ((*g_pSkillManager)[*(SKILLDOMAIN *)void_ptr].HasNewSkill())
	{
//		switch (status)
//		{
//			default: // ÀÌÁ¦ ¹è¿ï ¼ö ÀÖ´Ù.

//#ifdef _DEBUG
//		(*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].SetDomainLevel(21);
//#endif
					int domain_level;
					if(gbl_vampire_interface)
						domain_level = (*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].GetDomainLevel();
					else
					{
						if((*g_pSkillInfoTable)[left].DomainType >= 0)
							domain_level = (*g_pSkillManager)[(*g_pSkillInfoTable)[left].DomainType].GetDomainLevel();
						else domain_level = 0;
					}

				if((*g_pSkillInfoTable)[left].GetLearnLevel() <= domain_level && (*g_pSkillInfoTable)[left].GetLearnLevel() >= 0)
					g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_CAN_LEARN_SKILL].GetString(), gpC_base->m_item_desc_pi, RGB_GREEN);
				else
//				break;

//			case MSkillDomain::SKILLSTATUS_OTHER: // ¹è¿ï ¼ö ¾ø´Ù.
				//g_PrintColorStr(px, py, "\"Cannot learn this skill yet\"", gpC_base->m_item_desc_pi, RGB_RED);
					g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_CANNOT_LEARN_SKILL].GetString(), gpC_base->m_item_desc_pi, RGB_RED);
//				break;
//		}
	}
//	else if (status==MSkillDomain::SKILLSTATUS_OTHER		// by sigi
//			|| status==MSkillDomain::SKILLSTATUS_NEXT)
//				g_PrintColorStr(px, py, "\"¾ÆÁ÷ ¹è¿ï ¼ö ¾ø½À´Ï´Ù.\"", gpC_base->m_item_desc_pi, RGB_RED);				
	
	g_FL2_ReleaseDC();
	
}

//-----------------------------------------------------------------------------
// _Help_Description_Show
//
//
//-----------------------------------------------------------------------------
void	_Help_Description_Show(Rect rect, void * void_ptr, long left, long right)
{
//#ifdef _LIB // Client¿¡¼­´Â 3D¿ëÀ¸·Î »ç¿ëÇÒ ¼ö ÀÖ´Ù. -.-
	RECT _rect;
	SetRect(&_rect, rect.x, rect.y, rect.x+rect.w, rect.y+rect.h);
	DrawAlphaBox(&_rect, 0, 0, 0, g_pUserOption->ALPHA_DEPTH);
//#endif
//
//	S_RECT dest_rect;
//	S_SURFACEINFO	dest_surface;
//
//#ifndef _LIB
//	if (gpC_base->m_DDSurface_offscreen.Lock())
//	{
//		S_SURFACEINFO src_surface;
////		S_RECT src_rect;
//
//		SetSurfaceInfo(&src_surface, gpC_base->m_DDSurface_offscreen.GetDDSD());
//
//		//SetRect(src_rect, 0, 0, rect.w, rect.h);
//		//filledRect(&src_surface, &src_rect, BLACK);
//#endif
//		if (gpC_base->m_p_DDSurface_back->Lock()) // Descriptor common background¸¦ Ãâ·ÂÇÑ´Ù.
//		{
//			SetSurfaceInfo(&dest_surface, gpC_base->m_p_DDSurface_back->GetDDSD());
//			SetRect(dest_rect, rect.x, rect.y, rect.w, rect.h);
//
//#ifndef _LIB
//			//TransparentBlt16(&dest_surface, &src_surface, &dest_rect, &src_rect);
//			filledRect(&dest_surface, &dest_rect, BLACK);
//#endif
			SetRect(&_rect, rect.x, rect.y, rect.x+rect.w, rect.y+rect.h); // -.- TransparentBlt16()¿¡¼­ °ªÀ» º¯°æÇÔ.. ¾Æ´Ï ÀÌ·²¼ö°¡!
			gpC_base->m_p_DDSurface_back->DrawRect(&_rect, LIGHT_MAGENTA);
//			rectangle(&dest_surface, dest_rect.x, dest_rect.y, dest_rect.x+dest_rect.w-1, dest_rect.y+dest_rect.h-1, LIGHT_MAGENTA);
//
//			gpC_base->m_p_DDSurface_back->Unlock();
//		}
//
//#ifndef _LIB
//		gpC_base->m_DDSurface_offscreen.Unlock();
//	}
//#endif

	//
	// show contents
	//
	int px = rect.x+SIDE_GAP_HALF;
	int py = rect.y+SIDE_GAP_HALF;

	g_FL2_GetDC();

	g_PrintColorStr(px, py, ((S_DEFAULT_HELP_STRING *)void_ptr)->sz_main_str, gpC_base->m_item_name_pi, RGB_WHITE);
	py += NORMAL_FONT_Y_GAP;

	if (((S_DEFAULT_HELP_STRING *)void_ptr)->sz_main_str2 != NULL)
	{
		if(right)
			g_PrintColorStr(px, py, ((S_DEFAULT_HELP_STRING *)void_ptr)->sz_main_str2, gpC_base->m_item_desc_pi, right);
		else
			g_PrintColorStr(px, py, ((S_DEFAULT_HELP_STRING *)void_ptr)->sz_main_str2, gpC_base->m_item_desc_pi, RGB(200, 200, 200));
		
		py += SMALL_FONT_Y_GAP;
	}

	if (((S_DEFAULT_HELP_STRING *)void_ptr)->sz_sub_str != NULL)
	{
		if(right)
			g_PrintColorStr(px, py, ((S_DEFAULT_HELP_STRING *)void_ptr)->sz_sub_str, gpC_base->m_item_desc_pi, right);
		else
			g_PrintColorStr(px, py, ((S_DEFAULT_HELP_STRING *)void_ptr)->sz_sub_str, gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
	}

	g_FL2_ReleaseDC();
}

//-----------------------------------------------------------------------------
// _Item_Description_Calculator
//
// void_ptr = MItem *
//-----------------------------------------------------------------------------
void _Item_Description_Calculator(void (*fp_show)(Rect, void *, long, long), int x, int y, void * void_ptr, long left, long right)
{
	char sz_buf[256]={0,};
	Rect rect;
	
	//////////////////////////////// start calculation
	MItem * p_item = (MItem *)void_ptr;
	if (p_item == NULL)
		return;

	int line_count = 2;//2; // default: name, weight -> ºüÁü

#ifdef __ENGLISH_VERSION__
	line_count--;
#endif
	
	if( g_pTimeItemManager->IsExist( p_item->GetID() ) )
		line_count++;

	// identifyµÈ ¾ÆÀÌÅÛÀÎ °æ¿ì.. by sigi
//	if (p_item->IsIdentified())
	{
		if ( p_item->IsIdentified())
		{
			if (!p_item->IsEmptyItemOptionList())
				line_count+=p_item->GetItemOptionListCount();
		}
		else
		{
			line_count++;
		}
		
		line_count+=(*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].DefaultOptionList.size();
			
		if(p_item->GetItemClass() == MItem::ITEM_CLASS_SWORD || 
			p_item->GetItemClass() == MItem::ITEM_CLASS_BLADE || 
			p_item->GetItemClass() == MItem::ITEM_CLASS_CROSS || 
			p_item->GetItemClass() == MItem::ITEM_CLASS_MACE || 
			p_item->GetItemClass() == MItem::ITEM_CLASS_MINE || 
			p_item->GetItemClass() == MItem::ITEM_CLASS_BOMB || 
			p_item->GetItemClass() == MItem::ITEM_CLASS_BOMB_MATERIAL || 
			p_item->IsGunItem())
			line_count++;
		if (p_item->GetMaxDamage() != -1)
			line_count++;
		if (p_item->GetCriticalHit() != -1)
			line_count++;
		if (p_item->GetMaxDurability() != -1 &&!p_item->IsUniqueItem()&& p_item->GetItemClass() != MItem::ITEM_CLASS_VAMPIRE_AMULET &&
			p_item->GetItemClass() != MItem::ITEM_CLASS_COUPLE_RING && p_item->GetItemClass() != MItem::ITEM_CLASS_VAMPIRE_COUPLE_RING &&
			!p_item->IsQuestItem())
			line_count++;
		if (p_item->GetProtectionValue() != -1)
			line_count++;
		if (p_item->GetDefenseValue() != -1)
			line_count++;
		if (p_item->GetToHit() != -1)
			line_count++;
		if (p_item->GetHealPoint() != -1)
			line_count++;
		if (p_item->GetManaPoint() != -1)
			line_count++;
		if (p_item->GetSilverMax() != -1)
			line_count++;
		if (p_item->GetReach() != -1 && p_item->GetReach() != 1)
			line_count++;
		if (p_item->GetMagazineSize() != -1)
			line_count++;
		if (p_item->GetPocketNumber() != -1)
			line_count++;
		if (p_item->IsPileItem() == true || p_item->IsChargeItem())
			line_count++;
		if(g_pTimeItemManager->IsExist( p_item->GetID() ) )
		{
			if( gbl_sell_running || gbl_silvering_running )
				line_count++;
		} else
		{
			if ((gbl_sell_running || gbl_buy_running || gbl_repair_running || gbl_silvering_running))
				line_count++;
		}
		if (p_item->GetItemClass() == MItem::ITEM_CLASS_VAMPIRE_PORTAL_ITEM && p_item->IsIdentified())
		{
			if(((MVampirePortalItem *)p_item)->IsMarked())
				line_count+=2;
			else
				line_count++;
		}
		
		if(p_item->GetItemClass() != MItem::ITEM_CLASS_COUPLE_RING && p_item->GetItemClass() != MItem::ITEM_CLASS_VAMPIRE_COUPLE_RING &&
			!p_item->IsQuestItem())
		{
			if(p_item->IsIdentified())
			{
				if(p_item->IsSlayerItem())
				{
					if (p_item->GetRequireSTR() > 0)
					{
						line_count++;
						wsprintf(sz_buf,"%s%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(),
							(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ALL_STAT_SUM].GetString(),
							(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE_STAT].GetString());
						rect.w = max(rect.w, g_GetStringWidth(sz_buf,gpC_base->m_item_desc_pi.hfont));
					}
					if (p_item->GetRequireDEX() > 0)
					{
						line_count++;
						wsprintf(sz_buf,"%s%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(),
							(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ALL_STAT_SUM].GetString(),
							(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE_STAT].GetString());
						rect.w = max(rect.w, g_GetStringWidth(sz_buf,gpC_base->m_item_desc_pi.hfont));
					}
					if (p_item->GetRequireINT() > 0)
					{
						line_count++;
						wsprintf(sz_buf,"%s%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(),
							(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ALL_STAT_SUM].GetString(),
							(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE_STAT].GetString());
						rect.w = max(rect.w, g_GetStringWidth(sz_buf,gpC_base->m_item_desc_pi.hfont));
					}
					if (p_item->GetRequireSUM() > 0 &&
						p_item->GetRequireSUM() > p_item->GetRequireSTR() &&
						p_item->GetRequireSUM() > p_item->GetRequireDEX() &&
						p_item->GetRequireSUM() > p_item->GetRequireINT() )
					{
						line_count++;
						wsprintf(sz_buf,"%s%s%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(),
							(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ALL_STAT_SUM].GetString(),
							(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE_STAT].GetString());
						
						rect.w = max(rect.w, g_GetStringWidth(sz_buf,gpC_base->m_item_desc_pi.hfont));
					}
				}
				else if(p_item->IsVampireItem())
				{
					if (p_item->GetRequireLevel() > 0)
					{
						wsprintf(sz_buf,"%s%s%s",
							(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), 
							(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_LEVEL].GetString(),
							"level more than 100");
						rect.w = max(rect.w, g_GetStringWidth(sz_buf,gpC_base->m_item_desc_pi.hfont));
						line_count++;
					}	
				}
				
				if((!p_item->IsVampireItem() && gbl_vampire_interface) ||
					(!p_item->IsSlayerItem() && !gbl_vampire_interface))
					line_count++;
				
				if (p_item->IsGenderForMale() || p_item->IsGenderForFemale())
					line_count++;
			}
			else
				line_count++;
		}

		if((*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].Description.GetLength() != 0)
			line_count++;

		if(gpC_mouse_pointer->GetPickUpItem() != NULL && gpC_mouse_pointer->GetPickUpItem()->GetItemClass() == MItem::ITEM_CLASS_EVENT_STAR && p_item->IsGearItem())
		{
			line_count++;
		}
	}

	char sz_name[NAME_STRING_LEN];
	char sz_ename[NAME_STRING_LEN];

	sz_name[0] = '\0'; // for strcat()
	sz_ename[0] = '\0'; // for strcat()
#ifndef __ENGLISH_VERSION__
	strcat(sz_ename, "(");
#endif

	if (g_pTimeItemManager->IsExist( p_item->GetID() ) )
	{
		strcat( sz_name, (*g_pGameStringTable)[UI_STRING_MESSAGE_QUEST_HAN].GetString() );
		strcat(sz_name, " "); // add 'space'
		strcat( sz_ename,(*g_pGameStringTable)[UI_STRING_MESSAGE_QUEST_ENG].GetString() );
		strcat(sz_ename, " "); // add 'space'
	}
	
	if (!p_item->IsEmptyItemOptionList())
	{
		for(int i=0;i<min(2,p_item->GetItemOptionListCount());i++)
		{
			if(p_item->GetItemOptionName(i)!=NULL)
			{
				strcat(sz_name, p_item->GetItemOptionName(i));
				strcat(sz_name, " "); // add 'space'
			}
			if(p_item->GetItemOptionEName(i)!=NULL)
			{
				strcat(sz_ename, p_item->GetItemOptionEName(i));
				strcat(sz_ename, " "); // add 'space'
			}
		}
	}

	strcat(sz_name, p_item->GetName());
	strcat(sz_ename, p_item->GetEName());
#ifndef __ENGLISH_VERSION__
	strcat(sz_ename, ")");
#endif

	
#ifdef __ENGLISH_VERSION__
	rect.w = max(rect.w,g_GetStringWidth(sz_ename, gpC_base->m_item_name_pi.hfont));
#else
	rect.w = max(g_GetStringWidth(sz_name, gpC_base->m_item_name_pi.hfont), g_GetStringWidth(sz_ename, gpC_base->m_item_desc_pi.hfont));
#endif
	rect.h = NORMAL_FONT_Y_GAP+(line_count-1)*SMALL_FONT_Y_GAP+5;

	if((*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].Description.GetLength() != 0)
		rect.w = max(rect.w, g_GetStringWidth((*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].Description.GetString(), gpC_base->m_item_desc_pi.hfont));

	// ¼ÕÈñ½Â ¹Ùº¸ ¿ìÇìÇì
	if(g_pTimeItemManager->IsExist( p_item->GetID() ) )
	{
		char	temp[256] = {0,};	
		bool	bContinue = false;
		TYPE_OBJECTID objectID = p_item->GetID();		
		
		int tempInt;
		
		if( g_pTimeItemManager->GetDay( objectID ) == 0 && g_pTimeItemManager->GetHour (objectID ) ==0 &&
			g_pTimeItemManager->GetMinute( objectID ) == 0 )
		{
			if( g_pTimeItemManager->GetSecond( objectID ) == 0 )
			{
				strcpy(temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_EXPIRED_ITEM].GetString() );
			} else
			{
				strcpy(temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_LEFT_TIME].GetString() );
				strcat(temp, " ");
				char tempSecond[10];
				wsprintf(tempSecond, (*g_pGameStringTable)[UI_STRING_MESSAGE_SECOND].GetString() , g_pTimeItemManager->GetSecond( objectID ) );
				strcat(temp, tempSecond );
			}
		} else
		{
			strcpy(temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_LEFT_TIME].GetString() );

			if((tempInt = g_pTimeItemManager->GetDay( objectID ))>0  || bContinue )
			{
				char tempDay[30] = {0,};
				strcat(temp, " ");
				wsprintf(tempDay, (*g_pGameStringTable)[UI_STRING_MESSAGE_DAY].GetString() , tempInt );
				strcat(temp, tempDay );
				bContinue = true;
			}
			if((tempInt = g_pTimeItemManager->GetHour( objectID ))>0 || bContinue)
			{
				char tempDay[30] = {0,};
				strcat(temp, " ");
				wsprintf(tempDay, (*g_pGameStringTable)[UI_STRING_MESSAGE_HOUR].GetString() , tempInt );
				strcat ( temp, tempDay );
				bContinue = true;
			}
			if((tempInt = g_pTimeItemManager->GetMinute( objectID ))>0  || bContinue)
			{
				char tempDay[30] = {0,};
				strcat(temp, " ");
				wsprintf(tempDay, (*g_pGameStringTable)[UI_STRING_MESSAGE_MINUTE].GetString() , tempInt );
				strcat ( temp, tempDay );
				bContinue = true;
			}			
		}
		rect.w = max (rect.w, g_GetStringWidth( temp, gpC_base->m_item_desc_pi.hfont ) );
	}

	rect.w += SIDE_GAP;
	rect.h += SIDE_GAP;

	// revision?
	if (rect.w < DEFAULT_BOX_WIDTH)
		rect.w = DEFAULT_BOX_WIDTH;

	// get (x, y)
	rect.x = x;
	rect.y = y-(rect.h+1);

	if (rect.x < 0)
		rect.x = 0;
	if (rect.x+rect.w > RESOLUTION_X)
		rect.x = RESOLUTION_X-rect.w;
	if (rect.y < 0)
		rect.y = y+(p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y);
	if (rect.y+rect.h > RESOLUTION_Y)
		rect.y = RESOLUTION_Y-rect.h;

	//////////////////////////////// end calculation

	g_descriptor_manager.RectCalculationFinished(fp_show, rect, void_ptr, left, right);
}

//-----------------------------------------------------------------------------
// _Skill_Description_Calculator
//
// left = ACTIONINFO
// right = icon height
//-----------------------------------------------------------------------------
void _Skill_Description_Calculator(void (*fp_show)(Rect, void *, long, long), int x, int y, void * void_ptr, long left, long right)
{
	Rect rect;

	//////////////////////////////// start calculation
	int line_count = 2;
	if (gbl_vampire_interface || (*g_pSkillInfoTable)[left].IsPassive() || left == SKILL_LOVE_CHAIN)
	{
		line_count--;
	}

	if(left >= SKILL_HOLYLAND_BLOOD_BIBLE_ARMEGA && left <= SKILL_HOLYLAND_BLOOD_BIBLE_CHASPA)
		rect.w = g_GetStringWidth((*g_pGameStringTable)[STRING_MESSAGE_BLOOD_BIBLE_BONUS_ARMEGA+left-SKILL_HOLYLAND_BLOOD_BIBLE_ARMEGA].GetString(), gpC_base->m_item_desc_pi.hfont);
	else
	if(left == SKILL_LOVE_CHAIN)
		rect.w = g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_LOVE_CHAIN].GetString(), gpC_base->m_item_desc_pi.hfont);
	else
		rect.w = 0;


	rect.w = max(rect.w, g_GetStringWidth((*g_pSkillInfoTable)[left].GetName(), gpC_base->m_item_name_pi.hfont));
	rect.h = NORMAL_FONT_Y_GAP+(line_count)*SMALL_FONT_Y_GAP;

	rect.w += SIDE_GAP;
	rect.h += SIDE_GAP;

	// revision?
	if (rect.w < DEFAULT_BOX_WIDTH)
		rect.w = DEFAULT_BOX_WIDTH;

	// get (x, y)
	rect.x = x;
	rect.y = y-rect.h;

	if (rect.x < 0)
		rect.x = 0;
	if (rect.x+rect.w > RESOLUTION_X)
		rect.x = RESOLUTION_X-rect.w;
	if (rect.y < 0)
		rect.y = y+right;
	//////////////////////////////// end calculation

	g_descriptor_manager.RectCalculationFinished(fp_show, rect, void_ptr, left, right);
}

//-----------------------------------------------------------------------------
// _Info_Description_Calculator
//
// void_ptr = string
//-----------------------------------------------------------------------------
void _Info_Description_Calculator(void (*fp_show)(Rect, void *, long, long), int x, int y, void * void_ptr, long left, long right)
{
	Rect rect;

	//////////////////////////////// start calculation
	int line_count = 0;

	rect.w = g_GetStringWidth((const char *)void_ptr, gpC_base->m_item_name_pi.hfont);
	rect.h = NORMAL_FONT_Y_GAP+(line_count)*SMALL_FONT_Y_GAP;

	rect.w += SIDE_GAP;
	rect.h += SIDE_GAP;

	// revision?
//	if (rect.w < DEFAULT_BOX_WIDTH)
//		rect.w = DEFAULT_BOX_WIDTH;

	// get (x, y)
	rect.x = x;
	rect.y = y-rect.h;

	if (rect.x < 0)
		rect.x = 0;
	if (rect.x+rect.w > RESOLUTION_X)
		rect.x = RESOLUTION_X-rect.w;
	if (rect.y < 0)
		rect.y = y+right;
	//////////////////////////////// end calculation

	g_descriptor_manager.RectCalculationFinished(fp_show, rect, void_ptr, left, right);
}

//-----------------------------------------------------------------------------
// _Strings_Description_Calculator
//
// void_ptr = string
//-----------------------------------------------------------------------------
void _Strings_Description_Calculator(void (*fp_show)(Rect, void *, long, long), int x, int y, void * void_ptr, long left, long right)
{
	Rect rect;

	//////////////////////////////// start calculation
	int line_count = left-1;

	LPSTR* ptr = (LPSTR *)void_ptr;

	rect.w = 0;
	for(int i = 0; i < left; i++)
	{
		rect.w = max(g_GetStringWidth( (const char *)ptr[i], gpC_base->m_item_name_pi.hfont), rect.w);
	}

	rect.h = NORMAL_FONT_Y_GAP+(line_count)*NORMAL_FONT_Y_GAP;

	rect.w += SIDE_GAP;
	rect.h += SIDE_GAP;

	// revision?
//	if (rect.w < DEFAULT_BOX_WIDTH)
//		rect.w = DEFAULT_BOX_WIDTH;

	// get (x, y)
	rect.x = x;
	rect.y = y-rect.h;

	if (rect.x < 0)
		rect.x = 0;
	if (rect.x+rect.w > RESOLUTION_X)
		rect.x = RESOLUTION_X-rect.w;
	if (rect.y < 0)
		rect.y = y+right;
	//////////////////////////////// end calculation

	g_descriptor_manager.RectCalculationFinished(fp_show, rect, void_ptr, left, right);
}
//-----------------------------------------------------------------------------
// _SkillTree_Description_Calculator
//
// left = ACTIONINFO
// right = icon height
// void_ptr = SKILLDOMAIN
//-----------------------------------------------------------------------------
void _SkillTree_Description_Calculator(void (*fp_show)(Rect, void *, long, long), int x, int y, void * void_ptr, long left, long right)
{
	Rect rect;

	if(left < 0)return;
	//////////////////////////////// start calculation
	int line_count = 3;
	MSkillDomain::SKILLSTATUS status = (*g_pSkillManager)[*(SKILLDOMAIN *)void_ptr].GetSkillStatus((ACTIONINFO)left);
//	assert(status != MSkillDomain::SKILLSTATUS_NULL);
//	if (status == MSkillDomain::SKILLSTATUS_NEXT ||
//		 status == MSkillDomain::SKILLSTATUS_OTHER)
//		line_count++;

	if ((gbl_vampire_interface || (*g_pSkillInfoTable)[left].IsPassive()) && status == MSkillDomain::SKILLSTATUS_LEARNED)
	{
		line_count--;
	}
	else if(status == MSkillDomain::SKILLSTATUS_LEARNED)
	{
		line_count++;
		int domain = (*g_pSkillInfoTable)[(ACTIONINFO)left].DomainType;
		if(domain >= 0)
		{
			int domain_grade = max((*g_pSkillManager)[domain].GetDomainLevel()/25,
				(*g_pSkillInfoTable)[(ACTIONINFO)left].GetLearnLevel()/25);
//			int learn_level_grade = (*g_pSkillInfoTable)[(ACTIONINFO)left].GetLearnLevel()/25;
			//int skill_grade = ;//(*g_pSkillInfoTable)[(ACTIONINFO)left].GetSkillStep() - SKILL_STEP_APPRENTICE;
			
			//if(domain_grade - skill_grade >= 0
			//	&& domain_grade - skill_grade <= 2 
			//	&& domain_grade != SKILL_STEP_GRAND_MASTER - SKILL_STEP_APPRENTICE)
			if(domain_grade>= 0 && domain_grade<=2)
				line_count++;
		}
	}

	rect.w = g_GetStringWidth((*g_pSkillInfoTable)[left].GetName(), gpC_base->m_item_name_pi.hfont);

	if(left >= SKILL_HOLYLAND_BLOOD_BIBLE_ARMEGA && left <= SKILL_HOLYLAND_BLOOD_BIBLE_CHASPA)
	{
		rect.w = max(rect.w, g_GetStringWidth((*g_pGameStringTable)[STRING_MESSAGE_BLOOD_BIBLE_BONUS_ARMEGA+left-SKILL_HOLYLAND_BLOOD_BIBLE_ARMEGA].GetString(), gpC_base->m_item_desc_pi.hfont));
	}

	rect.h = NORMAL_FONT_Y_GAP+(line_count)*SMALL_FONT_Y_GAP;

	rect.w += SIDE_GAP;
	rect.h += SIDE_GAP;

	// revision?
	if (rect.w < DEFAULT_BOX_WIDTH)
		rect.w = DEFAULT_BOX_WIDTH;

	// get (x, y)
	rect.x = x;
	rect.y = y-rect.h;

	if (rect.x < 0)
		rect.x = 0;
	if (rect.x+rect.w > RESOLUTION_X)
		rect.x = RESOLUTION_X-rect.w;
	if (rect.y < 0)
		rect.y = y+right;
	//////////////////////////////// end calculation

	g_descriptor_manager.RectCalculationFinished(fp_show, rect, void_ptr, left, right);
}

//-----------------------------------------------------------------------------
// _Help_Description_Calculator
//
// void_ptr = S_DEFAULT_HELP_STRING *
// left = item count
//-----------------------------------------------------------------------------
void _Help_Description_Calculator(void (*fp_show)(Rect, void *, long, long), int x, int y, void * void_ptr, long left, long right)
{
	Rect rect;

	//////////////////////////////// start calculation
	int line_count = 0;
	int sub_w = 0, main_w2 = 0;
	if (((S_DEFAULT_HELP_STRING *)void_ptr)->sz_main_str2 != NULL)
	{
		line_count += 1;
		main_w2 = g_GetStringWidth(((S_DEFAULT_HELP_STRING *)void_ptr)->sz_main_str2, gpC_base->m_item_desc_pi.hfont);
	}
	if (((S_DEFAULT_HELP_STRING *)void_ptr)->sz_sub_str != NULL)
	{
		line_count += 1;
		sub_w = g_GetStringWidth(((S_DEFAULT_HELP_STRING *)void_ptr)->sz_sub_str, gpC_base->m_item_desc_pi.hfont);
	}
	rect.w = g_GetStringWidth(((S_DEFAULT_HELP_STRING *)void_ptr)->sz_main_str, gpC_base->m_item_name_pi.hfont);
	rect.h = NORMAL_FONT_Y_GAP+line_count*SMALL_FONT_Y_GAP;

	if (rect.w < main_w2)
		rect.w = main_w2;
	if (rect.w < sub_w)
		rect.w = sub_w;

	rect.w += SIDE_GAP;
	rect.h += SIDE_GAP;

	// get (x, y)
	rect.x = x;
	rect.y = y-rect.h;

	if (rect.x < 0)
		rect.x = 0;
	if (rect.x+rect.w > RESOLUTION_X)
		rect.x = RESOLUTION_X-rect.w;
	if (rect.y < 0)
		rect.y = 0;
//		rect.y = y+right;

	//////////////////////////////// end calculation

	g_descriptor_manager.RectCalculationFinished(fp_show, rect, void_ptr, left, right);
}

//-----------------------------------------------------------------------------
// _Help_Description_Calculator
//
// void_ptr = S_DEFAULT_HELP_STRING *
// left = item count
//-----------------------------------------------------------------------------
void _BloodBible_Description_Calculator(void (*fp_show)(Rect, void *, long, long), int x, int y, void * void_ptr, long left, long right)
{
	Rect rect;

	S_BLOOD_BIBLE_DESCRIPTION *desc = (S_BLOOD_BIBLE_DESCRIPTION*) void_ptr;

	//////////////////////////////// start calculation
	int line_count = 0;
	int width = 0;//, height = 0;


	g_FL2_GetDC();
	if(desc->sz_name != NULL)
	{
		line_count ++;
//		height += g_GetStringHeight(desc->sz_name,gpC_base->m_item_name_pi.hfont);
		width = max(width, g_GetStringWidth(desc->sz_name,gpC_base->m_item_name_pi.hfont) + g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_NAME].GetString(),gpC_base->m_item_name_pi.hfont));
	}
	
	if(desc->sz_place != NULL)
	{
		line_count++;
//		height += g_GetStringHeight(desc->sz_place,gpC_base->m_item_desc_pi.hfont);
		width = max(width, g_GetStringWidth(desc->sz_place,gpC_base->m_item_desc_pi.hfont) + g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_DESC_POSITION].GetString(), gpC_base->m_item_desc_pi.hfont));
	}
	
	if(desc->sz_shrine != NULL)
	{		
		line_count++;
//		height += g_GetStringHeight(desc->sz_shrine,gpC_base->m_item_desc_pi.hfont);
		width = max(width, g_GetStringWidth(desc->sz_shrine,gpC_base->m_item_desc_pi.hfont) + g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_DESC_POSITION].GetString(), gpC_base->m_item_desc_pi.hfont));
	}

	if(desc->sz_player != NULL)
	{
		line_count++;
//		height += g_GetStringHeight(desc->sz_player,gpC_base->m_item_desc_pi.hfont);
		width = max(width, g_GetStringWidth(desc->sz_player,gpC_base->m_item_desc_pi.hfont) + g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_DESC_PLAYER].GetString(), gpC_base->m_item_desc_pi.hfont));
	}
	
	if(desc->sz_option != NULL)
	{
		line_count++;
//		height += g_GetStringHeight(desc->sz_option,gpC_base->m_item_desc_pi.hfont);
		width = max(width, g_GetStringWidth(desc->sz_option,gpC_base->m_item_desc_pi.hfont) + g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_OPTION].GetString(), gpC_base->m_item_desc_pi.hfont));
	}

	if(desc->sz_status != NULL)
	{
		line_count++;
//		height += g_GetStringHeight(desc->sz_status, gpC_base->m_item_desc_pi.hfont);
		width = max(width, g_GetStringWidth(desc->sz_status, gpC_base->m_item_desc_pi.hfont) +g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_BLOOD_BIBLE_DESC_STATUS].GetString(), gpC_base->m_item_desc_pi.hfont));
	}
	g_FL2_ReleaseDC();
	
	rect.w = width + SIDE_GAP;
	rect.h = NORMAL_FONT_Y_GAP + SMALL_FONT_Y_GAP *( line_count -1)+ SIDE_GAP;
	

	// get (x, y)
	rect.x = x;
	rect.y = y - rect.h;

	if (rect.x < 0)
		rect.x = 0;
	if (rect.x+rect.w > RESOLUTION_X)
		rect.x = RESOLUTION_X-rect.w;
	if (rect.y < 0)
		rect.y = 0;
//		rect.y = y+right;

	//////////////////////////////// end calculation

	g_descriptor_manager.RectCalculationFinished(fp_show, rect, void_ptr, left, right);
}


//-----------------------------------------------------------------------------
// g_InstallDescriptor
//
// 
//-----------------------------------------------------------------------------
void	g_InstallDescriptor()
{
	g_descriptor_manager.AddDescribedUnit(DID_ITEM, _Item_Description_Calculator, _Item_Description_Show);
	g_descriptor_manager.AddDescribedUnit(DID_SKILL, _Skill_Description_Calculator, _Skill_Description_Show);
	g_descriptor_manager.AddDescribedUnit(DID_HELP, _Help_Description_Calculator, _Help_Description_Show, false);
	g_descriptor_manager.AddDescribedUnit(DID_SKILLTREE, _SkillTree_Description_Calculator, _SkillTree_Description_Show);
	g_descriptor_manager.AddDescribedUnit(DID_INFO, _Info_Description_Calculator, _Info_Description_Show);
	g_descriptor_manager.AddDescribedUnit(DID_STRINGS, _Strings_Description_Calculator, _Strings_Description_Show);
	g_descriptor_manager.AddDescribedUnit(DID_BLOOD_BIBLE_STATUS, _BloodBible_Description_Calculator, _BloodBible_Description_Show);
}

