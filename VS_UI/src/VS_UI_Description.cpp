// VS_UI_Description.cpp

#include "client_PCH.h"

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
#include "SystemAvailabilities.h"
#include "VS_UI.h"
#include "VS_UI_GameCommon2.h"

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

extern RECT g_GameRect;

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
//	//test
//	int _grade = 4;

	COLORREF ITEM_NAME_RARE_OPTION_SHADE = RGB(0,0,31<<3);
	MItem * p_item = (MItem *)void_ptr;
	MItem* pMouseItem = gpC_mouse_pointer->GetPickUpItem();
	if(p_item == NULL)
		return;

	MItem* p_AddItem = (MItem*)right;
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

	strcat(sz_ename, "(");
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
		else if(p_item->GetItemClass() == ITEM_CLASS_PET_ITEM)
		{
			if(p_item->GetSilver() > 0)
			{
				name_color = ITEM_NAME_OPTION_COLOR;
				shadow_color = ITEM_NAME_OPTION_SHADE;
				ename_color = ITEM_ENAME_OPTION_COLOR;

				MPetItem *pPetItem = (MPetItem *)p_item;
				
				strcat(sz_name, pPetItem->GetPetOptionName().c_str());
//				strcat(sz_name, " "); // add 'space'

				strcat(sz_ename, pPetItem->GetPetOptionEName().c_str());
//				strcat(sz_ename, " "); // add 'space'
			}
		}
		else if (!p_item->IsEmptyItemOptionList() && p_item->GetItemClass() != ITEM_CLASS_COUPLE_RING &&
				p_item->GetItemClass() != ITEM_CLASS_VAMPIRE_COUPLE_RING
				&& p_item->GetItemClass() != ITEM_CLASS_CODE_SHEET )
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

	char sz_buf[50];

	strcat(sz_name, p_item->GetName());
	strcat(sz_ename, p_item->GetEName());

	if( p_item->GetGrade() > 0 &&  p_item->GetGrade() <= 10 && p_item->IsGearItem() && !p_item->IsUniqueItem() && p_item->GetItemClass() != ITEM_CLASS_CORE_ZAP)
	{
		static char *szGrade = (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_GRADE].GetString();
		strncat( sz_name, szGrade+p_item->GetGrade()*2, 2 );
		strncat( sz_ename, szGrade+p_item->GetGrade()*2, 2 );
	}
	
	strcat(sz_ename, ")");

	if(!p_item->IsIdentified())
	{
		for(int i = 0; i < strlen(sz_name); i++)
			sz_name[i] = '?';
		
		for(int i = 0; i < strlen(sz_ename); i++)
			sz_ename[i] = '?';
	}

	

	g_FL2_GetDC();
	// name
	// optionÀÌ ºÙ¾îÀÖÀ¸¸é ´Ù¸¥ »öÀ¸·Î Ãâ·ÂÇÑ´Ù.
	g_PrintColorStr(px+1, py+1, sz_name, gpC_base->m_item_name_pi, shadow_color);
	g_PrintColorStr(px, py, sz_name, gpC_base->m_item_name_pi, name_color);
	py += NORMAL_FONT_Y_GAP;
	g_PrintColorStr(px+1, py+1, sz_ename, gpC_base->m_item_desc_pi, shadow_color);
	g_PrintColorStr(px, py, sz_ename, gpC_base->m_item_desc_pi, ename_color);
	py += SMALL_FONT_Y_GAP+5;

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
			case ITEM_CLASS_SWORD:
				wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_SWORD].GetString());
				break;
				
			case ITEM_CLASS_BLADE:
				wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_BLADE].GetString());
				break;
				
			case ITEM_CLASS_CROSS:
				wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_CROSS].GetString());
				break;
				
			case ITEM_CLASS_MACE:
				wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_MACE].GetString());
				break;
				
			case ITEM_CLASS_MINE:
				wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_MINE].GetString());
				break;
				
			case ITEM_CLASS_BOMB:
				wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_BOMB].GetString());
				break;
				
			case ITEM_CLASS_BOMB_MATERIAL:
				if(p_item->GetItemType() < 5)
					wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_BOMB_MATERIAL].GetString());
				else
					wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_MINE_MATERIAL].GetString());
				break;
				
			case ITEM_CLASS_SG:
				wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_SG].GetString());
				break;
				
			case ITEM_CLASS_SMG:
				wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_SMG].GetString());
				break;
				
			case ITEM_CLASS_AR:
				wsprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_CLASS_AR].GetString());
				break;
				
			case ITEM_CLASS_SR:
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
			const int itemClass = p_item->GetItemClass();
			const int itemType = p_item->GetItemType();

			// Æê ¾ÆÀÌÅÛ Á¤º¸ Ãâ·Â ¿ö³« Æ¯ÀÌÇØ¼­ µû·Î Ã³¸®
			if(itemClass == ITEM_CLASS_PET_ITEM)
			{
				// Æê ·¹º§Àº Num¿¡ ÀÖ´Ù
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_LEVEL].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "%d", p_item->GetNumber());				
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
				py += SMALL_FONT_Y_GAP;

				// ÆêÀº enchant levelÀ» Æê¼Ó¼ºÀ¸·Î Ãâ·ÂÇÑ´Ù
				// enchant level Àº ÀÎÃ¾Æ®¿Í´Â ¾Æ¹« °ü·Ã ¾øÀ¸¸ç »ç½ÇÀº ChargeItemÀÇ Charge¼ö¸¦ ¹ÞÀ» ¶§ ¾´´Ù
				// »ç½Ç ¹ÞÀ»¶§´Â Enchant Level·Î ¹ÞÁö¸¸ ÀúÀåÀº Number¿¡ ÀúÀåÇÑ´Ù. ±×·¡¼­ Â÷Áö ¾ÆÀÌÅÛ¿¡¼­
				// GetEnchantLevelÀ» ÇØµµ ¾Æ¹«°Íµµ ¾È³ª¿Â´Ù.
				// ±×°Ç Â÷Áö ¾ÆÀÌÅÛ¸¸ÀÌ°í ÆêÀº ±×³É EnchantLevel ±×´ë·Î ¾´´Ù
				// Æê ¼Ó¼º ¼öÄ¡´Â Silver°ªÀ» ¾´´Ù.

				// 2Â÷ ´É·ÂÀÌ ÀÖÀ» ¶§
				if(((MPetItem*)p_item)->IsCanCutHead())
				{
					vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_PET_CAN_CUT_HEAD].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
					py += SMALL_FONT_Y_GAP;
				}

				if(p_item->GetSilver() > 0/* && p_item->GetEnchantLevel()!=0xFFFF add by viva */)
				{
					vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_PET_ATTR].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
					sprintf(sz_buf, "%s +%d", g_pItemOptionTable->ITEMOPTION_PARTNAME[p_item->GetEnchantLevel()].GetString(), p_item->GetSilver());				
					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
					py += SMALL_FONT_Y_GAP;
				}

				// Á×¾ú´Ù
				if( p_item->GetGrade() != -1 )
				{
					sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_PET_DEAD_DAY].GetString(), p_item->GetGrade());				
					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
					py += SMALL_FONT_Y_GAP;
				}
				else	// ¾ÈÁ×¾ú´Ù
				{
					// ÆêÀº durÀ» ³²Àº ¸ÔÀÌ¾çÀ¸·Î Ãâ·ÂÇÑ´Ù
					vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_PET_DESC_DURABILITY].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
					TYPE_ITEM_DURATION leftTime = p_item->GetCurrentDurability();
					int timeGap = ((timeGetTime() - ((MPetItem *)p_item)->GetUpdateTime())/1000/60);
					if(timeGap > leftTime)
					{
						leftTime = 0;
					}
					else
					{
						leftTime = leftTime - timeGap;
					}
					int day,hour,minute;
					
					day = leftTime / 60 / 24;
					hour = (leftTime / 60)%24;
 					minute = leftTime % 60;

					std::string time;

					if(leftTime == 0)
					{
						time = (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_NOT_EXIST].GetString();
					}
					else
					{
						if( day > 0 )
						{
							sprintf( sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_DAY].GetString(), day );
							time = sz_buf;
							time += " ";
						}
						if( hour > 0 )
						{
							sprintf( sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_HOUR].GetString(), hour );
							time += sz_buf;
							time += " ";
						}
						if( minute > 0 )
						{
							sprintf( sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_MINUTE].GetString(), minute );
							time += sz_buf;					
						}
					}
					g_PrintColorStr(vx, py, time.c_str() , gpC_base->m_item_desc_pi, RGB_WHITE);				
					py += SMALL_FONT_Y_GAP;
				}
//				MPetItem *pPetItem = (MPetItem *)p_item;
//				if(pPetItem->GetPetKeepedDay() > 0)
//				{
//					vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_PET_ATTR].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
//					sprintf(sz_buf, "%s +%d", g_pItemOptionTable->ITEMOPTION_PARTNAME[p_item->GetEnchantLevel()].GetString(), p_item->GetSilver());				
//					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
//					py += SMALL_FONT_Y_GAP;
//				}

			} 
		
			else
			{
				// Durability
				if (
					p_item->GetMaxDurability() != -1 && !p_item->IsUniqueItem() && !p_item->IsQuestItem() &&
					itemClass!=ITEM_CLASS_VAMPIRE_AMULET &&
					itemClass!=ITEM_CLASS_COUPLE_RING &&
					itemClass!=ITEM_CLASS_VAMPIRE_COUPLE_RING &&
					itemClass!=ITEM_CLASS_CORE_ZAP&&
					itemClass!=ITEM_CLASS_BLOOD_BIBLE_SIGN
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
					int stringID = UI_STRING_MESSAGE_DESC_DAMAGE;
					if(p_item->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET)
						stringID = UI_STRING_MESSAGE_DESC_MAGIC_DAMAGE;

					vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[stringID].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
					sprintf(sz_buf, "%d~%d", p_item->GetMinDamage(), p_item->GetMaxDamage());				
					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_RED);
					py += SMALL_FONT_Y_GAP;
				}			

				if (p_item->GetOriginalSpeed() > 0)
				{
					vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ATTACK_SPEED].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
					sprintf(sz_buf, "%d", p_item->GetOriginalSpeed());
					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB(200, 200, 255));
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
					if( g_eRaceInterface == RACE_OUSTERS )
						vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_EP].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
					else
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

				// lucky value
				if (p_item->GetLucky() != - 9999 && itemClass != ITEM_CLASS_COUPLE_RING && itemClass != ITEM_CLASS_VAMPIRE_COUPLE_RING &&  itemClass != ITEM_CLASS_CORE_ZAP)
				{
					vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_LUCKY].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
					sprintf(sz_buf, "%d", p_item->GetLucky());
					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_RED);
					py += SMALL_FONT_Y_GAP;
				}
				
				// ¾Æ¿ì½ºÅÍÁî Á¤·É¼Ó¼º
				if(itemClass == ITEM_CLASS_OUSTERS_WRISTLET || itemClass == ITEM_CLASS_OUSTERS_STONE)
				{
					static COLORREF stoneRGB[5] = { RGB(255, 100, 100), RGB(100, 100, 255), RGB(255, 180, 100), RGB(100, 100, 255), ITEM_DESC_RGB };
					ITEMTABLE_INFO::ELEMENTAL_TYPE eType = (*g_pItemTable)[itemClass][itemType].ElementalType;
					WORD eNum = (*g_pItemTable)[itemClass][itemType].Elemental;

					if(eType != ITEMTABLE_INFO::ELEMENTAL_TYPE_ANY)
					{
						sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_OUSTERS_STONE].GetString(), (*g_pGameStringTable)[g_ELEMENTAL_STRING_ID[eType]].GetString());
						vx = g_PrintColorStr(px, py, sz_buf, gpC_base->m_item_desc_pi, stoneRGB[eType]);
						sprintf(sz_buf, "%d", eNum);
						g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
						py += SMALL_FONT_Y_GAP;
					}
				}
				if(itemClass == ITEM_CLASS_CORE_ZAP && itemType >= 0 && itemType <= 3)
				{
					vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_OPTION].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
					// ÄÚ¾îÀì ¿É¼Ç
					sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_CORE_ZAP_BLACK+p_item->GetItemType()].GetString(), p_item->GetGrade());
					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_PEARL);				
					py += SMALL_FONT_Y_GAP;

					sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_CORE_ZAP_REWARD_ALL_STAT].GetString(), 3);
					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_DARKGRAY);				
					py += SMALL_FONT_Y_GAP;
					sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_CORE_ZAP_REWARD_ALL_REG].GetString(), 9);
					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_DARKGRAY);				
					py += SMALL_FONT_Y_GAP;
					
				}
			}
		} else
		{
			// Durability
			if (p_item->GetMaxDurability() != -1&&!p_item->IsUniqueItem()&&p_item->GetItemClass()!=ITEM_CLASS_VAMPIRE_AMULET && !p_item->IsQuestItem())
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
			
			// Damage
			if (p_item->GetOriginalSpeed() > 0) // damage°¡ ÀÖ´Â°¡?
			{
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ATTACK_SPEED].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "???");				
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB(200, 200, 255));
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
			// lucky value
			if (p_item->GetLucky() != - 9999)
			{
				vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_LUCKY].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				sprintf(sz_buf, "???");
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_RED);
				py += SMALL_FONT_Y_GAP;
			}
		}

		/*// Durability
		if (p_item->GetMaxDurability() != -1&&!p_item->IsUniqueItem()&&p_item->GetItemClass()!=ITEM_CLASS_VAMPIRE_AMULET)
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
		if (p_item->IsIdentified() && p_item->GetItemClass() == ITEM_CLASS_VAMPIRE_PORTAL_ITEM)
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
			}
			else
			{
				sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_NOT_EXIST].GetString());
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
			}
			py += SMALL_FONT_Y_GAP;
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
			if(!p_item->IsEmptyItemOptionList()&& p_item->GetItemClass() != ITEM_CLASS_CODE_SHEET )
			{			
				const std::list<TYPE_ITEM_OPTION> &optionList=p_item->GetItemOptionList();
				std::list<TYPE_ITEM_OPTION>::const_iterator itr=optionList.begin();				
				
				while(itr != optionList.end() && *itr < g_pItemOptionTable->GetSize())
				{
					ITEMOPTION_INFO& optionInfo=(*g_pItemOptionTable)[*itr];
					char pPartName[20];
					strcpy(pPartName,g_pItemOptionTable->ITEMOPTION_PARTNAME[optionInfo.Part].GetString());
					if(strstr(pPartName,"MP") != NULL)
					{
						if(g_eRaceInterface == RACE_VAMPIRE)
							*strstr(pPartName,"MP") = 'H';
						else if(g_eRaceInterface == RACE_OUSTERS)
							*strstr(pPartName,"MP") = 'E';
					}
					
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
					strcpy(pPartName,g_pItemOptionTable->ITEMOPTION_PARTNAME[optionInfo.Part].GetString());
					
					if(strstr(pPartName,"MP") != NULL)
					{
						if(p_item->IsVampireItem())
							*strstr(pPartName,"MP") = 'H';
						else if(p_item->IsOustersItem())
							*strstr(pPartName,"MP") = 'E';
					}
					
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
			
			if(p_AddItem!=NULL)
			{
				if(p_AddItem->GetItemClass() == ITEM_CLASS_CORE_ZAP && p_AddItem->GetItemType()>=0 && p_AddItem->GetItemType()<=3)
				{
						const std::list<TYPE_ITEM_OPTION> &optionList=p_item->GetItemOptionList();
					const std::list<TYPE_ITEM_OPTION> &DefaultOptionList = p_item->GetItemDefaultOptionList();
					if(optionList.size() ==0 && DefaultOptionList.size() == 0)
						vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_OPTION].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
					
					sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_CORE_ZAP_BLACK+p_AddItem->GetItemType()].GetString(), p_AddItem->GetGrade());
					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_PEARL);				
					py += SMALL_FONT_Y_GAP;
					
					DWORD TempColor = RGB_DARKGRAY;
					if(gC_vs_ui.IsHasAllCoreZap())
						TempColor = RGB_GREEN;
					sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_CORE_ZAP_REWARD_ALL_STAT].GetString(), 3);
					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, TempColor);				
					py += SMALL_FONT_Y_GAP;
					sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_CORE_ZAP_REWARD_ALL_REG].GetString(), 9);
					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, TempColor);				
					py += SMALL_FONT_Y_GAP;
					
				}
			}
		}
		else
		{
			sprintf(sz_buf,"%s???????",(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_OPTION].GetString());
			vx = g_PrintColorStr(px, py, sz_buf, gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			
			py += SMALL_FONT_Y_GAP;
		}

//		if(-1 != p_item->GetGrade() &&
//			(p_item->GetItemClass() == ITEM_CLASS_RING ||
//			p_item->GetItemClass() == ITEM_CLASS_BRACELET ||
//			p_item->GetItemClass() == ITEM_CLASS_NECKLACE ||
//			)
//			}
//		{
//			sprintf(sz_buf, "%s +%d", g_pItemOptionTable->ITEMOPTION_PARTNAME[21].GetString(), p_item->GetGrade()-4);
//			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_GOLD);//RGB_YELLOW);
//			py += SMALL_FONT_Y_GAP;
//		}
			
		bool bl_required = false;

		if(p_item->GetItemClass() != ITEM_CLASS_COUPLE_RING && p_item->GetItemClass() != ITEM_CLASS_VAMPIRE_COUPLE_RING &&
			!p_item->IsQuestItem())
		{
			
			if(p_item->IsIdentified())
			{
				if(p_item->IsGearItem())
				{
					// Required
					if(p_item->IsSlayerItem() || p_item->IsOustersItem())
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
					if(p_item->IsVampireItem() || p_item->IsOustersItem())
					{
						if (p_item->GetRequireLevel() > 0)
						{
							if(!bl_required)
								vx = g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
							else
								vx = px + g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi.hfont);
							bl_required = true;
							
							vx = g_PrintColorStr(vx, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_LEVEL].GetString(), gpC_base->m_item_desc_pi, required_rgb);
							sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE_STAT].GetString(), p_item->GetRequireLevel());
							vx = g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_GREEN);
							
							py += SMALL_FONT_Y_GAP;
						}
					}
					// 2005, 1, 18, sobeit add start - ÇÊ¿ä ½ÂÁ÷ ·¹º§
					int RequireAdvancementLevel = (*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].GetRequireAdvancementLevel();
					if( RequireAdvancementLevel > 0 )
					{
						if(!bl_required)
							vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
						else
							vx = px + g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi.hfont);
						bl_required = true;
						
						int LevelGrade = (RequireAdvancementLevel-1)/10;
						int LevelUnits = (RequireAdvancementLevel-1)%10;
						// edit by Coffee  2007-5-20 ÐÞÕý×°±¸ÏÔÊ¾¶þ×ªµÈ¼¶²»ÕýÎÊÌâ
						//sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_REQUIRE_ADVANCEMENT_LEVEL_0+LevelGrade].GetString(), LevelUnits+1);
						sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_REQUIRE_ADVANCEMENT_LEVEL_0+LevelGrade].GetString(), RequireAdvancementLevel);
						// edit end 
						if(g_char_slot_ingame.m_AdvancementLevel >= RequireAdvancementLevel)
							vx = g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_GREEN);
						else
							vx = g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_RED);
						
						py += SMALL_FONT_Y_GAP;

					}
					// 2005, 1, 18, sobeit add end - ÇÊ¿ä ½ÂÁ÷ ·¹º§
				}
				
				bool bSlayer = !p_item->IsSlayerItem() && g_eRaceInterface == RACE_SLAYER;
				bool bVampire = !p_item->IsVampireItem() && g_eRaceInterface == RACE_VAMPIRE;
				bool bOusters = !p_item->IsOustersItem() && g_eRaceInterface == RACE_OUSTERS;
//					if(!p_item->IsSlayerItem() && g_eRaceInterface == RACE_SLAYER ||
//						!p_item->IsVampireItem() && g_eRaceInterface == RACE_VAMPIRE ||
//						!p_item->IsOustersItem() && g_eRaceInterface == RACE_OUSTERS
//						)
				if( bSlayer||bVampire||bOusters)
				{
					if(!bl_required)
						vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
					else
						vx = px + g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REQUIRE].GetString(), gpC_base->m_item_desc_pi.hfont);
					bl_required = true;
					
					vx = g_PrintColorStr(vx, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_OTHER_TRIBE].GetString(), gpC_base->m_item_desc_pi, required_rgb);
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

//	if(p_item->GetItemClass() != ITEM_CLASS_EVENT_GIFT_BOX)
	{
		
		// Price
		// by csm

		if(gC_vs_ui.IsRunningPersnalShop() && p_item->GetPersnalPrice() !=-1 ) 
		{
			
			std::string sstr;
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_PRICE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			
			if(p_item->IsPileItem() && p_item->GetNumber() >= 1)
			{
				//int price = max( 0, g_pPriceManager->GetItemPrice(p_item, MPriceManager::PC_TO_NPC) );
				int TempPrice = p_item->GetPersnalPrice(); 
				
				if( g_pTimeItemManager->IsExist( p_item->GetID() ) )
					TempPrice = 50;

				if(gC_ci->IsKorean() && g_pUserOption->ShowGameMoneyWithHANGUL)
				{
					sstr = "$";
					sstr += g_GetStringByMoney(TempPrice);
					vx = g_PrintColorStr(vx, py, sstr.c_str(), gpC_base->m_item_desc_pi, RGB_WHITE);
					sstr = TempPrice/p_item->GetNumber();
					sprintf(sz_buf, "(%sx%d)", sstr.c_str(), max(0, p_item->GetNumber()));
					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, required_rgb);
				}
				else
				{
					sprintf(sz_buf, "%d", TempPrice);
					std::string sstr = sz_buf;
					for(int i = 3; i <= 13; i += 4)
						if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
					sprintf(sz_buf, "$%s", sstr.c_str());
					vx = g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
					sprintf(sz_buf, "(%dx%d)", TempPrice/p_item->GetNumber(),max(0, p_item->GetNumber()));
					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, required_rgb);
				}
			}
			else 
			{
				int TempPrice = g_pTimeItemManager->IsExist(p_item->GetID()) ? 50 : max( 0, p_item->GetPersnalPrice() );

				if(gC_ci->IsKorean() && g_pUserOption->ShowGameMoneyWithHANGUL)
				{
					sstr = "$";
					sstr += g_GetStringByMoney(TempPrice);
					g_PrintColorStr(vx, py, sstr.c_str(), gpC_base->m_item_desc_pi, RGB_WHITE);
				}
				else
				{
					sprintf(sz_buf, "%d", TempPrice);

					sstr = sz_buf;
					for(int i = 3; i <= 13; i += 4)
						if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
					sprintf(sz_buf, "$%s", sstr.c_str());
					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
				}
			}
			
			py += SMALL_FONT_Y_GAP;
		}


		if (gbl_sell_running == true)
		{
			assert(gbl_buy_running == false);
			
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_PRICE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			
			if(p_item->IsPileItem() && p_item->GetNumber() >= 1)
			{
				int TempPrice = max( 0, g_pPriceManager->GetItemPrice(p_item, MPriceManager::PC_TO_NPC) );
				
				if( g_pTimeItemManager->IsExist( p_item->GetID() ) )
					TempPrice = 50;

				std::string sstr;
				if(gC_ci->IsKorean() && g_pUserOption->ShowGameMoneyWithHANGUL)
				{
					sstr = "$";
					sstr += g_GetStringByMoney(TempPrice*max(0,p_item->GetNumber()));
					vx = g_PrintColorStr(vx, py, sstr.c_str(), gpC_base->m_item_desc_pi, RGB_WHITE);
					sstr = g_GetStringByMoney(TempPrice);
					sprintf(sz_buf, "(%sx%d)", sstr.c_str(), max(0, p_item->GetNumber()));
					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, required_rgb);
				}
				else
				{
					sprintf(sz_buf, "%d", TempPrice*max(0,p_item->GetNumber()));
					sstr = sz_buf;
					for(int i = 3; i <= 13; i += 4)
						if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
					sprintf(sz_buf, "$%s", sstr.c_str());
					vx = g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
					
					sprintf(sz_buf, "(%dx%d)", TempPrice,max(0, p_item->GetNumber()));
					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, required_rgb);
				}
			}
			else 
			{
				std::string sstr;
				int TempPrice = g_pTimeItemManager->IsExist(p_item->GetID()) ? 50 : max( 0, g_pPriceManager->GetItemPrice(p_item, MPriceManager::PC_TO_NPC)) ;
				if(gC_ci->IsKorean() && g_pUserOption->ShowGameMoneyWithHANGUL)
				{
					sstr = "$";
					sstr += g_GetStringByMoney(TempPrice);
					g_PrintColorStr(vx, py, sstr.c_str(), gpC_base->m_item_desc_pi, RGB_WHITE);
				}
				else
				{
					sprintf(sz_buf, "%d", TempPrice);
					sstr = sz_buf;
					for(int i = 3; i <= 13; i += 4)
						if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
					sprintf(sz_buf, "$%s", sstr.c_str());
					g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
				}
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
					wsprintf(sz_buf, "%s %d%s", (*g_pItemTable)[ITEM_CLASS_EVENT_STAR][price.type].HName.GetString(), max( 0, price.number) , (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_NUMBER].GetString());
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
					int TempPrice = max( 0, g_pPriceManager->GetItemPrice(p_item, MPriceManager::NPC_TO_PC) );
					std::string sstr;
					if(gC_ci->IsKorean() && g_pUserOption->ShowGameMoneyWithHANGUL)
					{
						sstr = "$";
						sstr += g_GetStringByMoney(TempPrice*p_item->GetNumber());
						vx = g_PrintColorStr(vx, py, sstr.c_str(), gpC_base->m_item_desc_pi, RGB_WHITE);
						sstr = g_GetStringByMoney(TempPrice);
						sprintf(sz_buf, "(%sx%d)", sstr.c_str(), max(p_item->GetNumber(),0) );
						g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, required_rgb);
					}
					else
					{
						sprintf(sz_buf, "%d", TempPrice*p_item->GetNumber());
						sstr = sz_buf;
						for(int i = 3; i <= 13; i += 4)
							if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
						sprintf(sz_buf, "$%s", sstr.c_str());
						vx = g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
						sprintf(sz_buf, "(%dx%d)", TempPrice, max(p_item->GetNumber(),0) );
						g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, required_rgb);
					}
				}
				else 
				{
					std::string sstr;
					int TempPrice = max( 0, g_pPriceManager->GetItemPrice(p_item, MPriceManager::NPC_TO_PC) );
					if(gC_ci->IsKorean() && g_pUserOption->ShowGameMoneyWithHANGUL)
					{
						sstr = "$";
						sstr += g_GetStringByMoney(TempPrice);
						g_PrintColorStr(vx, py, sstr.c_str(), gpC_base->m_item_desc_pi, RGB_WHITE);
					}
					else
					{
						sprintf(sz_buf, "%d", TempPrice);
						sstr = sz_buf;
						for(int i = 3; i <= 13; i += 4)
							if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
						sprintf(sz_buf, "$%s", sstr.c_str());
						g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
					}
				}
			}
			
			py += SMALL_FONT_Y_GAP;
		}
		
		if (gbl_repair_running == true && !g_pTimeItemManager->IsExist( p_item->GetID() ) && p_item->GetItemClass() != ITEM_CLASS_VAMPIRE_AMULET&& p_item->GetItemClass() != ITEM_CLASS_CORE_ZAP)
		{
			assert(gbl_buy_running == false);
			assert(gbl_sell_running == false);			
			if(p_item->IsChargeItem())
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_CHARGE_PRICE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			else
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_REPAIR_PRICE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);

			std::string sstr;
			int TempPrice = max( 0, g_pPriceManager->GetItemPrice(p_item, MPriceManager::REPAIR) );
			if(gC_ci->IsKorean() && g_pUserOption->ShowGameMoneyWithHANGUL)
			{
				sstr = "$";
				sstr += g_GetStringByMoney(TempPrice);
				g_PrintColorStr(vx, py, sstr.c_str(), gpC_base->m_item_desc_pi, RGB_WHITE);
			}
			else
			{
				sprintf(sz_buf, "%d", TempPrice);
				std::string sstr = sz_buf;
				for(int i = 3; i <= 13; i += 4)
					if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
				sprintf(sz_buf, "$%s", sstr.c_str());
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
			}			
			py += SMALL_FONT_Y_GAP;
		}
		
		if (gbl_silvering_running == true)
		{
			assert(gbl_buy_running == false);
			assert(gbl_sell_running == false);
			assert(gbl_repair_running == false);			
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_SILVERING_PRICE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);

			std::string sstr;
			int TempPrice = g_pPriceManager->GetItemPrice(p_item, MPriceManager::SILVERING);
			if(gC_ci->IsKorean() && g_pUserOption->ShowGameMoneyWithHANGUL)
			{
				sstr = "$";
				sstr += g_GetStringByMoney(TempPrice);
				g_PrintColorStr(vx, py, sstr.c_str(), gpC_base->m_item_desc_pi, RGB_WHITE);
			}	
			else
			{
				sprintf(sz_buf, "%d", max( 0, TempPrice));
				std::string sstr = sz_buf;
				for(int i = 3; i <= 13; i += 4)
					if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");
				sprintf(sz_buf, "$%s", sstr.c_str());
				g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
			}			
			py += SMALL_FONT_Y_GAP;
		}
		
	}

//	MString temp_string = (*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].Description;
	if((*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].Description.GetLength() != 0)
	{
		if(p_item->GetItemClass() == ITEM_CLASS_BLOOD_BIBLE_SIGN)
			g_PrintColorStr(px, py, (*g_pGameStringTable)[STRING_MESSAGE_BLOOD_BIBLE_BONUS_ARMEGA+p_item->GetItemType()].GetString(), gpC_base->m_item_desc_pi, required_rgb);
		else
			g_PrintColorStr(px, py, (*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].Description.GetString(), gpC_base->m_item_desc_pi, required_rgb);
		py += SMALL_FONT_Y_GAP;
	}
	
	if(pMouseItem != NULL && pMouseItem->GetItemClass() == ITEM_CLASS_EVENT_STAR && p_item->IsGearItem())
	{
		const std::list<TYPE_ITEM_OPTION> &optionList = p_item->GetItemOptionList();
		std::list<TYPE_ITEM_OPTION>::const_iterator itr = optionList.begin();	
		
		bool bCanUpgrade = false;
	
		if(p_item->IsUniqueItem() || p_item->IsQuestItem())
		{
			bCanUpgrade = false;
		} else		
		if(pMouseItem->GetItemType() >= 8 && 
			pMouseItem->GetItemType() <=14  ||
			pMouseItem->GetItemType() >= 17 && 
			pMouseItem->GetItemType() <= 21)
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
		if(pMouseItem->GetItemType() == 12)					// ºí·ç ¹öµå´Â ¿É¼Ç µÎ°³¸¸
		{
			if(p_item->GetItemOptionListCount() != 2)
				bCanUpgrade = false;
			else
			{
				bCanUpgrade = true;
				itr = optionList.begin();
				while(itr != optionList.end() )
				{
					if((*g_pItemOptionTable)[*itr].UpgradeOptionType == 0)
					{
						bCanUpgrade = false;
						break;
					}
					itr++;
				}
				
			}
		} else
		if(p_item->GetItemOptionListCount()>1)
			bCanUpgrade = false;

// add by Coffee 2006.11.4  ÐÞÕý×ãÇòÎªÀ¶±¦Ê¯£¬²¢ÏÔÊ¾ÄÜ³äÈýÊôÐÔ×°±¸
		if(pMouseItem->GetItemType() == 6)					// ºí·ç ¹öµå´Â ¿É¼Ç µÎ°³¸¸
		{
			if(p_item->GetItemOptionListCount() != 3)
				bCanUpgrade = false;
			else
			{
				bCanUpgrade = true;
				itr = optionList.begin();
				int allUpgrade=0;
				while(itr != optionList.end() )
				{
					if((*g_pItemOptionTable)[*itr].UpgradeOptionType == 0)
					{
						//bCanUpgrade = false;
						//break;
						allUpgrade++;
					}
					itr++;
				}
				if (allUpgrade>=3)
					bCanUpgrade = false;
				
			}
		} else
		if(p_item->GetItemOptionListCount()>2)
			bCanUpgrade = false;

// end by Coffee À¶±¦Ê¯Ð§¹û
		if(pMouseItem->GetItemType() == 16 )			// Æ®·£½º ¾ÆÀÌÅÛ Å°Æ®
		{
			if(!p_item->IsGenderForAll())
				bCanUpgrade = true;
			else
				bCanUpgrade = false;
		} else
		// add by svi 2009-07-01 ÏÔÊ¾»ÆÉ«Ë®µÎ¿ÉÒÔÉý¼¶1,2ÊôÐÔÎïÆ·
		if(pMouseItem->GetItemType() == 22 )			// »ÆÉ«Ë®µÎ
		{
			if(p_item->GetItemOptionListCount() == 1 || p_item->GetItemOptionListCount() == 2)
				bCanUpgrade = true;
			else
				bCanUpgrade = false;
		} else

		if( pMouseItem->GetItemType() == 23) // Ô¶¹ÅË®µÎ
		{
			// modified by svi 2009-06-25 :  "> 0" -> ">= 0"  
			if(p_item->GetGrade() >= 0 &&  p_item->GetGrade() <= 10)
				bCanUpgrade = true;
		} else
		// end

		// add by svi 2009-07-15 ÊôÐÔÏû³ý±¦Ê¯
		if(pMouseItem->GetItemType() == 24 )			// ÔÂ¹âÊ¯
		{
			if(p_item->GetItemOptionListCount() >= 1 && p_item->GetItemOptionListCount() <= 3)
				bCanUpgrade = true;
			else
				bCanUpgrade = false;
		}else
		if(pMouseItem->GetItemType() == 25 )			// ×ÏÑ»Ê¯
		{
			if(p_item->GetItemOptionListCount() == 2 || p_item->GetItemOptionListCount() == 3)
				bCanUpgrade = true;
			else
				bCanUpgrade = false;
		}else
		if(pMouseItem->GetItemType() == 26 )			// ÁâÐ¿Ê¯
		{
			if(p_item->GetItemOptionListCount() == 3)
				bCanUpgrade = true;
			else
				bCanUpgrade = false;
		}
		// end

		if(bCanUpgrade)
		{
			if(pMouseItem->GetNumber() != 1)
				g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_ONLY_PICK_UP_ITEM_ONE].GetString(), gpC_base->m_item_desc_pi, RGB(150, 150, 255));
			else
			{
				if(pMouseItem->GetItemType() == 16 )
					g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CAN_TRANS].GetString(), gpC_base->m_item_desc_pi, RGB(150, 150, 255));				
				else
				{
					g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CAN_ENCHANT].GetString(), gpC_base->m_item_desc_pi, RGB(150, 150, 255));
				}
			}
		}
		else
		{
			if( pMouseItem->GetItemType() == 16)
				g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_CANNOT_USE].GetString(),gpC_base->m_item_desc_pi, RGB(255, 150, 150));
			else
				g_PrintColorStr(px, py, (*g_pGameStringTable)[STRING_MESSAGE_ITEM_TO_ITEM_IMPOSIBLE].GetString(), gpC_base->m_item_desc_pi, RGB(255, 150, 150));
		}

		py += SMALL_FONT_Y_GAP;
	}

	if( pMouseItem != NULL && pMouseItem->GetItemClass() == ITEM_CLASS_PET_ENCHANT_ITEM &&
		p_item->GetItemClass() == ITEM_CLASS_PET_ITEM )
	{		
		MPetItem* pPetItem = (MPetItem*)p_item;
		int bCanUpgrade = 0;
		
		if((pMouseItem->GetItemType() < 13 || pMouseItem->GetItemType() >= 16 && pMouseItem->GetItemType() <= 19) &&	// 12±îÁö´Â ·¹µå µå·Ó
			pPetItem->GetSilver() == 0 &&		// silver´Â AttrLevelÀÌ 0ÀÎ °æ¿ì´Â ¼Ó¼ºÀÌ ¾ø´Ù°í º¸°í ÀÎÃ¾Æ® °¡´É
			pPetItem->GetNumber() >= 10)		// Æê ·¹º§ 10ÀÌ»óÀÎ°æ¿ì
			bCanUpgrade = 1;
		
		if(pMouseItem->GetItemType() == 13 || pMouseItem->GetItemType() == 14)
		{
			if(pPetItem->GetCurrentDurability() == 0)		// ÇöÀç HP°¡ 0ÀÏ¶§
				bCanUpgrade = 2;
			else
				bCanUpgrade = 3;
		}
		
		if(pMouseItem->GetItemType() == 15 && // 15´Â ·¹µå ¹öµå
			pPetItem->GetNumber() == 49 && pPetItem->GetItemOptionListCount() == 0)
			bCanUpgrade = 1;

//		if( !pPetItem->IsCanGamble() )
//			bCanUpgrade = 0;
		
		switch(bCanUpgrade)
		{
		case 1:
			g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CAN_ENCHANT].GetString(), gpC_base->m_item_desc_pi, RGB(150, 150, 255));			
			py += SMALL_FONT_Y_GAP;
			break;

		case 2:
			g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CAN_PET_REVIVAL].GetString(), gpC_base->m_item_desc_pi, RGB(150, 150, 255));			
			py += SMALL_FONT_Y_GAP;
			break;

		case 3:
			g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_PET_REVIVAL].GetString(), gpC_base->m_item_desc_pi, RGB(150, 150, 255));			
			py += SMALL_FONT_Y_GAP;
			break;

		default:
			g_PrintColorStr(px, py, (*g_pGameStringTable)[STRING_MESSAGE_ITEM_TO_ITEM_IMPOSIBLE].GetString(), gpC_base->m_item_desc_pi, RGB(255, 150, 150));
			py += SMALL_FONT_Y_GAP;
			break;
		}
		
	}

	if(pMouseItem != NULL && pMouseItem->GetItemClass() == ITEM_CLASS_MIXING_ITEM 
		&&pMouseItem->GetItemType() >= 9 && pMouseItem->GetItemType() <= 17 &&
		p_item != NULL && !p_item->IsEmptyItemOptionList() && !p_item->IsQuestItem() && !p_item->IsUniqueItem())
	{
		if(pMouseItem->GetNumber() != 1)
			g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_ONLY_PICK_UP_ITEM_ONE].GetString(), gpC_base->m_item_desc_pi, RGB(150, 150, 255));
		else
		{
//			if((*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].Description.GetLength() != 0)
//				py += SMALL_FONT_Y_GAP;
			if( C_VS_UI_REMOVE_OPTION::IsCanRemoveOption_Puritas( pMouseItem, p_item ) )
			{
				g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CAN_REMOVE_OPTION].GetString(), gpC_base->m_item_desc_pi, RGB(150, 150, 255));			
			} else
				g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSGAE_CANNOT_REMOVE_OPTION].GetString(), gpC_base->m_item_desc_pi, RGB(255, 150, 150));
		}
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

	if(p_item->GetItemClass() == ITEM_CLASS_PET_FOOD)
	{
		PETINFO *pPetInfo = gC_vs_ui.GetMyPetInfo();
		if(pPetInfo != NULL && pPetInfo->EXIST)
		{
			g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CAN_USE_PET_FOOD].GetString(),gpC_base->m_item_desc_pi,RGB(255,255,255));
		}
		else
		{
			g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DETACH_PET_FOOD].GetString(),gpC_base->m_item_desc_pi,RGB(255,255,255));
		}
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
	const char *szSkillName;
	char szTemp2[10] = "???";
	bool bEnable = true;

	if( g_pSystemAvailableManager->GetLimitLearnSkillLevel() >= (*g_pSkillInfoTable)[left].GetLearnLevel() )
		szSkillName = (*g_pSkillInfoTable)[left].GetHName();
	else
	{
		szSkillName = szTemp2;
		bEnable = false;
	}
	
	g_PrintColorStr(px+1, py+1, szSkillName, gpC_base->m_item_name_pi, RGB(100,100,0));
	g_PrintColorStr(px, py, szSkillName, gpC_base->m_item_name_pi, RGB_WHITE);

	py += NORMAL_FONT_Y_GAP;
	std::string SkillEName;
	SkillEName = "(";
	if( bEnable )
		SkillEName += (*g_pSkillInfoTable)[left].GetName();
	else
		SkillEName += szTemp2;
	SkillEName += ")";

	g_PrintColorStr(px, py, SkillEName.c_str(), gpC_base->m_item_desc_pi, RGB_GRAY);
	py += NORMAL_FONT_Y_GAP;

	if( !bEnable )
	{
		g_FL2_ReleaseDC();
		return;
	}
	
	int vx;

	if(left == SKILL_LOVE_CHAIN)
	{
		vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_LOVE_CHAIN].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
		
		g_FL2_ReleaseDC();
		return;
	}

	if (g_eRaceInterface != RACE_VAMPIRE && (!(*g_pSkillInfoTable)[left].IsPassive() || g_eRaceInterface == RACE_OUSTERS))
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
			switch(g_eRaceInterface)
			{
			case RACE_SLAYER:
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CONSUME_MP].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				break;

			case RACE_VAMPIRE:
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CONSUME_HP].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				break;

			case RACE_OUSTERS:
				vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CONSUME_EP].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
				break;
			}

		if( left == SKILL_WILL_OF_LIFE )
			sprintf(sz_buf, "%d",5 + (g_char_slot_ingame.level / 7));
		else
			sprintf(sz_buf, "%d", (*g_pSkillInfoTable)[left].GetMP());
		g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
		py += SMALL_FONT_Y_GAP;
	}
	else
	{
		if(left >= SKILL_HOLYLAND_BLOOD_BIBLE_ARMEGA && left <= SKILL_HOLYLAND_BLOOD_BIBLE_CHASPA)
			g_PrintColorStr(px, py, (*g_pGameStringTable)[STRING_MESSAGE_BLOOD_BIBLE_BONUS_ARMEGA+(left-SKILL_HOLYLAND_BLOOD_BIBLE_ARMEGA)].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
		else
		if(left >= SKILL_SWEEPER_BONUS_1 && left <= SKILL_SWEEPER_BONUS_12 )
			g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_SWEEPER_BONUS_1+(left-SKILL_SWEEPER_BONUS_1)].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
		else
			g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_HAN_PASSIVE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
		py += SMALL_FONT_Y_GAP;
	}

	if(g_eRaceInterface == RACE_OUSTERS)
	{
		SKILLINFO_NODE sInfo = (*g_pSkillInfoTable)[left];
		if(sInfo.ElementalDomain == ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE ||
			sInfo.ElementalDomain == ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER ||
			sInfo.ElementalDomain == ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH ||
			sInfo.ElementalDomain == ITEMTABLE_INFO::ELEMENTAL_TYPE_WIND
			)
		{
			int elemental_point[5] = { sInfo.Fire, sInfo.Water, sInfo.Earth, sInfo.Wind, sInfo.Sum };
			
			sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_REQUIRE_ELEMENTAL_LEVEL].GetString(), (*g_pGameStringTable)[g_ELEMENTAL_STRING_ID[sInfo.ElementalDomain]].GetString(), elemental_point[sInfo.ElementalDomain]);
			g_PrintColorStr(px, py, sz_buf, gpC_base->m_chatting_pi, g_ELEMENTAL_COLOR[sInfo.ElementalDomain]);
			py += SMALL_FONT_Y_GAP;
		}
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
		else if (strcmp( desc->sz_shrine, (*g_pGameStringTable)[STRING_MESSAGE_VAMPIRE].GetString()) == 0 )
			tempColor = RGB_RED;
		else tempColor = RGB_GREEN;

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
	bool bEnable = true;
	const char *szSkillName;
	char szTemp2[10] = "???";

	if( g_pSystemAvailableManager->GetLimitLearnSkillLevel() < (*g_pSkillInfoTable)[left].GetLearnLevel() )
	{
		bEnable = false;
		szSkillName = szTemp2;
	} else
	{
		szSkillName = (*g_pSkillInfoTable)[left].GetHName();
	}

	g_FL2_GetDC();
	// name
	// optionÀÌ ºÙ¾îÀÖÀ¸¸é ´Ù¸¥ »öÀ¸·Î Ãâ·ÂÇÑ´Ù.
	g_PrintColorStr(px+1, py+1, szSkillName, gpC_base->m_item_name_pi, RGB(100,100,0) );
	g_PrintColorStr(px, py, szSkillName, gpC_base->m_item_name_pi, RGB_WHITE );

	py += NORMAL_FONT_Y_GAP;
	std::string SkillEName;
	SkillEName = "(";
	if( bEnable )
		SkillEName += (*g_pSkillInfoTable)[left].GetName();
	else
		SkillEName += szTemp2;
	SkillEName += ")";
	g_PrintColorStr(px, py, SkillEName.c_str(), gpC_base->m_item_desc_pi, RGB_GRAY);
	py += NORMAL_FONT_Y_GAP;
	int vx;

	if( !bEnable ) 
	{
		g_FL2_ReleaseDC();
		return;
	}

	MSkillDomain::SKILLSTATUS status = (*g_pSkillManager)[*(SKILLDOMAIN *)void_ptr].GetSkillStatus((ACTIONINFO)left);

	// level
	if(g_eRaceInterface != RACE_OUSTERS)
	{
		vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_REQUIRE_LEVEL].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
		if((*g_pSkillInfoTable)[left].GetLearnLevel() >= 0)
		{
			sprintf(sz_buf, "%d", (*g_pSkillInfoTable)[left].GetLearnLevel());
			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
		}
		py += SMALL_FONT_Y_GAP;
	}
	
	if(status==MSkillDomain::SKILLSTATUS_LEARNED || g_eRaceInterface == RACE_OUSTERS)
	{
		if (g_eRaceInterface != RACE_VAMPIRE && !(*g_pSkillInfoTable)[left].IsPassive() || g_eRaceInterface == RACE_OUSTERS)
		{
			sprintf(sz_buf,"%s: ",(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_LEVEL].GetString());
			vx = g_PrintColorStr(px, py, sz_buf, gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			sprintf(sz_buf, "%d", (*g_pSkillInfoTable)[left].GetExpLevel());
			g_PrintColorStr(vx, py, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
			py += SMALL_FONT_Y_GAP;
		}
	}
	if(g_eRaceInterface == RACE_OUSTERS)
	{
		if(status==MSkillDomain::SKILLSTATUS_LEARNED)
			sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_REQUIRE_SKILL_POINT].GetString(), (*g_pSkillInfoTable)[left].LevelUpPoint );
		else
			sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_REQUIRE_SKILL_POINT].GetString(),  (*g_pSkillInfoTable)[left].SkillPoint);
		g_PrintColorStr(px, py, sz_buf, gpC_base->m_chatting_pi, ITEM_DESC_RGB);
		py += SMALL_FONT_Y_GAP;

		// 2004, 10, 16, sobeit add start
		if((*g_pSkillInfoTable)[left].GetLearnLevel()> 1)
		{
			sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_LEARN_SKILL_LEVEL].GetString(), (*g_pSkillInfoTable)[left].GetLearnLevel());
			g_PrintColorStr(px, py, sz_buf, gpC_base->m_chatting_pi, ITEM_DESC_RGB);
			py += SMALL_FONT_Y_GAP;
		}
		// 2004, 10, 16, sobeit add end
	}

	// MP
	if(!(*g_pSkillInfoTable)[left].IsPassive())
	{
		switch(g_eRaceInterface)
		{
		case RACE_SLAYER:
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CONSUME_MP].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			break;
			
		case RACE_VAMPIRE:
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CONSUME_HP].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			break;
			
		case RACE_OUSTERS:
			vx = g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CONSUME_EP].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			break;
		}
		
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
		else if (left >= SKILL_SWEEPER_BONUS_1 && left <= SKILL_SWEEPER_BONUS_12 )
		{
			g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_SWEEPER_BONUS_1+left-SKILL_SWEEPER_BONUS_1].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
		}
		else
			g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_HAN_PASSIVE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
		
	}
	py += SMALL_FONT_Y_GAP;
	
	if(g_eRaceInterface == RACE_OUSTERS)
	{
		SKILLINFO_NODE sInfo = (*g_pSkillInfoTable)[left];
		if(sInfo.ElementalDomain == ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE ||
			sInfo.ElementalDomain == ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER ||
			sInfo.ElementalDomain == ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH ||
			sInfo.ElementalDomain == ITEMTABLE_INFO::ELEMENTAL_TYPE_WIND
			)
		{
			int elemental_point[5] = { sInfo.Fire, sInfo.Water, sInfo.Earth, sInfo.Wind, sInfo.Sum };
			
			sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_REQUIRE_ELEMENTAL_LEVEL].GetString(), (*g_pGameStringTable)[g_ELEMENTAL_STRING_ID[sInfo.ElementalDomain]].GetString(), elemental_point[sInfo.ElementalDomain]);
			g_PrintColorStr(px, py, sz_buf, gpC_base->m_chatting_pi, g_ELEMENTAL_COLOR[sInfo.ElementalDomain]);
			py += SMALL_FONT_Y_GAP;
		}
	}
	

//	assert(status != MSkillDomain::SKILLSTATUS_NULL);

	// »õ·Î¿î skillÀ» ¹è¿ï ¼ö ÀÖ´Â »óÅÂÀÎÁö Ã¼Å©ÇØ¾ßÇÔ... by sigi
	if(status==MSkillDomain::SKILLSTATUS_LEARNED)
	{
		if(g_eRaceInterface == RACE_SLAYER)
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
		switch(g_eRaceInterface)
		{
		case RACE_SLAYER:
			if((*g_pSkillInfoTable)[left].DomainType >= 0)
				domain_level = (*g_pSkillManager)[(*g_pSkillInfoTable)[left].DomainType].GetDomainLevel();
			else domain_level = 0;
			break;
			
		case RACE_VAMPIRE:
			domain_level = (*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].GetDomainLevel();
			break;
			
		case RACE_OUSTERS:
			domain_level = (*g_pSkillManager)[SKILLDOMAIN_OUSTERS].GetDomainLevel();
			break;
		}

		if(g_eRaceInterface == RACE_OUSTERS)
		{
			SKILLINFO_NODE sInfo = (*g_pSkillInfoTable)[left];
			if(sInfo.SkillPoint <= g_char_slot_ingame.skill_point&& sInfo.GetLearnLevel() <= g_char_slot_ingame.level )
			{
				bool bCanLearn = false;
				
				if(sInfo.SkillTypeList.empty())
					bCanLearn = true;
				else
				{
					SKILLINFO_NODE::SKILLTYPE_LIST::iterator itr = sInfo.SkillTypeList.begin();
					SKILLINFO_NODE::SKILLTYPE_LIST::iterator endItr = sInfo.SkillTypeList.end();
					
					while(itr != endItr)
					{
						if((*g_pSkillInfoTable)[*itr].GetSkillStep() == SKILL_STEP_OUSTERS_ETC && (*g_pSkillManager)[SKILLDOMAIN_OUSTERS].GetSkillStatus((ACTIONINFO)*itr) == MSkillDomain::SKILLSTATUS_LEARNED)
						{
							SKILLINFO_NODE::SKILLTYPE_LIST::iterator itr2 = (*g_pSkillInfoTable)[*itr].SkillTypeList.begin();
							SKILLINFO_NODE::SKILLTYPE_LIST::iterator endItr2 = (*g_pSkillInfoTable)[*itr].SkillTypeList.end();
							while(itr2 != endItr2)
							{
								if((*g_pSkillManager)[SKILLDOMAIN_OUSTERS].GetSkillStatus((ACTIONINFO)*itr2) == MSkillDomain::SKILLSTATUS_LEARNED
									&& (sInfo.GetSkillStep() == (*g_pSkillInfoTable)[*itr2].GetSkillStep() || (*g_pSkillInfoTable)[*itr2].GetSkillStep() == SKILL_STEP_OUSTERS_ETC))
								{
									bCanLearn = true;
									break;
								}
								itr2++;
							}
							if(bCanLearn)
								break;
						}
						else
						{
							if((*g_pSkillManager)[SKILLDOMAIN_OUSTERS].GetSkillStatus((ACTIONINFO)*itr) == MSkillDomain::SKILLSTATUS_LEARNED)
							{
								bCanLearn = true;
								break;
							}
						}
						itr++;
					}
				}
				
				if(bCanLearn)
					g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_CAN_LEARN_SKILL].GetString(), gpC_base->m_item_desc_pi, RGB_GREEN);
				else
					g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_CANNOT_LEARN_SKILL].GetString(), gpC_base->m_item_desc_pi, RGB_RED);
			} else
				g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_CANNOT_LEARN_SKILL].GetString(), gpC_base->m_item_desc_pi, RGB_RED);
		}
		else
		{
			if((*g_pSkillInfoTable)[left].GetLearnLevel() <= domain_level && (*g_pSkillInfoTable)[left].GetLearnLevel() >= 0)
				g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_CAN_LEARN_SKILL].GetString(), gpC_base->m_item_desc_pi, RGB_GREEN);
			else
				g_PrintColorStr(px, py,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_CANNOT_LEARN_SKILL].GetString(), gpC_base->m_item_desc_pi, RGB_RED);
		}
		py += SMALL_FONT_Y_GAP;
	}
//	else if (status==MSkillDomain::SKILLSTATUS_OTHER		// by sigi
//			|| status==MSkillDomain::SKILLSTATUS_NEXT)
//				g_PrintColorStr(px, py, "\"¾ÆÁ÷ ¹è¿ï ¼ö ¾ø½À´Ï´Ù.\"", gpC_base->m_item_desc_pi, RGB_RED);				
	

	if(g_eRaceInterface == RACE_OUSTERS && left <365)
	{
		if((*g_pSkillInfoTable)[left].CanDelete)
			g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CAN_SKILL_DELETE].GetString(), gpC_base->m_chatting_pi, RGB_GREEN);
		else
			g_PrintColorStr(px, py, (*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_SKILL_DELETE].GetString(), gpC_base->m_chatting_pi, RGB_RED);
		py += SMALL_FONT_Y_GAP;
	}
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
	Rect rect(0,0,0,0);
	char sz_name[NAME_STRING_LEN];
	char sz_ename[NAME_STRING_LEN];
	
	//////////////////////////////// start calculation
	MItem * p_item = (MItem *)void_ptr;

	MItem * pAddItem = (MItem*)right;


	
	MItem* pMouseItem = gpC_mouse_pointer->GetPickUpItem();
	if (p_item == NULL)
		return;

	int line_count = 2;//2; // default: name, weight -> ºüÁü
	
	if( g_pTimeItemManager->IsExist( p_item->GetID() ) )
		line_count++;

	// 2004, 7, 1, sobeit add start - corezap °ü·Ã
	if(pAddItem != NULL)
	{
		if(pAddItem->GetItemClass() == ITEM_CLASS_CORE_ZAP)
		{
			if(pAddItem->GetItemType() >= 0 && pAddItem->GetItemType() <=3) // Áúµå·¹ ÄÚ¾îÀì
				line_count+= 3; // ¾ÆÀÌÅÛ ÀÚÃ¼ ¿É¼Ç 1°³, 4°³ ¸ð¾ÒÀ»¶§ ¿É¼Ç 2°³
		}
	}
	// 2004, 7, 1, sobeit add end - corezap °ü·Ã

	// identifyµÈ ¾ÆÀÌÅÛÀÎ °æ¿ì.. by sigi
//	if (p_item->IsIdentified())
	{
		const int itemClass = p_item->GetItemClass();
		const int itemType = p_item->GetItemType();

		if ( p_item->IsIdentified())
		{
			if (!p_item->IsEmptyItemOptionList()&& p_item->GetItemClass() != ITEM_CLASS_CODE_SHEET )
				line_count+=p_item->GetItemOptionListCount();
		}
		else
		{
			line_count++;
		}
		
		line_count+=(*g_pItemTable)[itemClass][itemType].DefaultOptionList.size();


		// ÆêÀº ¿ö³« Æ¯ÀÌÇØ¼­ µû·ÎÃ³¸®
		if(itemClass == ITEM_CLASS_PET_ITEM)
		{
			// ÆêÀº durÀ» ³²Àº ¸ÔÀÌ¾çÀ¸·Î Ãâ·ÂÇÑ´Ù
			line_count++;

			// Æê ·¹º§Àº Num¿¡ ÀÖ´Ù
			line_count++;

			// 2Â÷ ´É·ÂÀÌ ÀÖÀ» ¶§
			if(((MPetItem*)p_item)->IsCanCutHead())
				line_count++;

			// ÆêÀº enchant levelÀ» Æê¼Ó¼ºÀ¸·Î Ãâ·ÂÇÑ´Ù
			// enchant level Àº ÀÎÃ¾Æ®¿Í´Â ¾Æ¹« °ü·Ã ¾øÀ¸¸ç »ç½ÇÀº ChargeItemÀÇ Charge¼ö¸¦ ¹ÞÀ» ¶§ ¾´´Ù
			// »ç½Ç ¹ÞÀ»¶§´Â Enchant Level·Î ¹ÞÁö¸¸ ÀúÀåÀº Number¿¡ ÀúÀåÇÑ´Ù. ±×·¡¼­ Â÷Áö ¾ÆÀÌÅÛ¿¡¼­
			// GetEnchantLevelÀ» ÇØµµ ¾Æ¹«°Íµµ ¾È³ª¿Â´Ù.
			// ±×°Ç Â÷Áö ¾ÆÀÌÅÛ¸¸ÀÌ°í ÆêÀº ±×³É EnchantLevel ±×´ë·Î ¾´´Ù
			// Æê ¼Ó¼º ¼öÄ¡´Â Silver°ªÀ» ¾´´Ù.

			if(g_pTimeItemManager->IsExist( p_item->GetID() ) )
			{
				if( gbl_sell_running || gbl_silvering_running )
					line_count++;
			} else
			{
				if ((gbl_sell_running || gbl_buy_running || gbl_repair_running || gbl_silvering_running))
					line_count++;
			}

			if( p_item->GetGrade() != -1 )
			{
				TYPE_ITEM_DURATION leftTime = p_item->GetCurrentDurability();
				int day,hour,minute;
				
				day = leftTime / 60 / 24;
				hour = (leftTime / 60)%24;
				minute = leftTime % 60;
				
				std::string time;

				time = (*g_pGameStringTable)[UI_STRING_MESSAGE_PET_DESC_DURABILITY].GetString();
				time += " ";
				
				if( day > 0 )
				{
					sprintf( sz_name, (*g_pGameStringTable)[UI_STRING_MESSAGE_DAY].GetString(), day );
					time += sz_name;
					time += " ";
				}
				if( hour > 0 )
				{
					sprintf( sz_name, (*g_pGameStringTable)[UI_STRING_MESSAGE_HOUR].GetString(), hour );
					time += sz_name;
					time += " ";
				}
				if( minute > 0 )
				{
					sprintf( sz_name, (*g_pGameStringTable)[UI_STRING_MESSAGE_MINUTE].GetString(), minute );
					time += sz_name;					
				}

				rect.w = max(rect.w, g_GetStringWidth(time.c_str(), gpC_base->m_item_desc_pi.hfont));
			}

			if(p_item->GetSilver() > 0)
				line_count++;

			const MItem* pMouseItem = gpC_mouse_pointer->GetPickUpItem();

			if( pMouseItem != NULL && pMouseItem->GetItemClass() == ITEM_CLASS_PET_ENCHANT_ITEM )
			{
				line_count++;			
			}
		}
		else if(itemClass == ITEM_CLASS_CORE_ZAP)
		{
			line_count += 3;
			if( gbl_sell_running || gbl_buy_running )
				line_count++;
		}
		else
		{
			if(itemClass == ITEM_CLASS_SWORD || 
				itemClass == ITEM_CLASS_BLADE || 
				itemClass == ITEM_CLASS_CROSS || 
				itemClass == ITEM_CLASS_MACE || 
				itemClass == ITEM_CLASS_MINE || 
				itemClass == ITEM_CLASS_BOMB || 
				itemClass == ITEM_CLASS_BOMB_MATERIAL || 
				p_item->IsGunItem())
				line_count++;
			if (p_item->GetMaxDamage() != -1)
				line_count++;
			if (p_item->GetOriginalSpeed() > 0)
				line_count++;
			if (p_item->GetCriticalHit() != -1)
				line_count++;
			if (p_item->GetMaxDurability() != -1 &&!p_item->IsUniqueItem()&& itemClass != ITEM_CLASS_VAMPIRE_AMULET &&
				itemClass != ITEM_CLASS_COUPLE_RING && itemClass != ITEM_CLASS_VAMPIRE_COUPLE_RING &&
				!p_item->IsQuestItem()&&itemClass != ITEM_CLASS_BLOOD_BIBLE_SIGN)
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
			if (p_item->GetPersnalPrice() != -1)
				line_count++;
			// 2004, 04, 3 sobeit add start
			if(p_item->GetLucky() != -9999 && itemClass != ITEM_CLASS_COUPLE_RING && itemClass != ITEM_CLASS_VAMPIRE_COUPLE_RING)
				line_count++;
			// 2004, 04, 3 sobeit add end
			if(g_pTimeItemManager->IsExist( p_item->GetID() ) )
			{
				if( gbl_sell_running || gbl_silvering_running )
					line_count++;
			} else
			{
				if ((gbl_sell_running || gbl_buy_running || (gbl_repair_running&&(itemClass!=ITEM_CLASS_VAMPIRE_AMULET)) || gbl_silvering_running))
					line_count++;
			}
			if (itemClass == ITEM_CLASS_VAMPIRE_PORTAL_ITEM && p_item->IsIdentified())
			{
				if(((MVampirePortalItem *)p_item)->IsMarked())
					line_count+=2;
				else
					line_count++;
			}
			
			if(itemClass != ITEM_CLASS_COUPLE_RING && itemClass != ITEM_CLASS_VAMPIRE_COUPLE_RING &&
				!p_item->IsQuestItem())
			{
				if(p_item->IsIdentified())
				{
					if(p_item->IsGearItem())
					{
						if(p_item->IsSlayerItem() || p_item->IsOustersItem())
						{
							if (p_item->GetRequireSTR() > 0)
								line_count++;
							if (p_item->GetRequireDEX() > 0)
								line_count++;
							if (p_item->GetRequireINT() > 0)
								line_count++;
							if (p_item->GetRequireSUM() > 0 &&
								p_item->GetRequireSUM() > p_item->GetRequireSTR() &&
								p_item->GetRequireSUM() > p_item->GetRequireDEX() &&
								p_item->GetRequireSUM() > p_item->GetRequireINT() )
								line_count++;
						}
						if(p_item->IsVampireItem() || p_item->IsOustersItem())
						{
							if (p_item->GetRequireLevel() > 0)
								line_count++;
						}
						// 2005, 1, 18, sobeit add start - ÇÊ¿ä ½ÂÁ÷ ·¹º§
						if((*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].GetRequireAdvancementLevel()>0)
							line_count++;
						// 2005, 1, 18, sobeit add end - ÇÊ¿ä ½ÂÁ÷ ·¹º§
					}
					
					if(!p_item->IsSlayerItem() && g_eRaceInterface == RACE_SLAYER ||
						!p_item->IsVampireItem() && g_eRaceInterface == RACE_VAMPIRE ||
						!p_item->IsOustersItem() && g_eRaceInterface == RACE_OUSTERS)
						line_count++;
					
					if (p_item->IsGenderForMale() || p_item->IsGenderForFemale())
						line_count++;
				}
				else
					line_count++;
			}
		}

		if((*g_pItemTable)[itemClass][itemType].Description.GetLength() != 0)
			line_count++;

		if(pMouseItem != NULL && pMouseItem->GetItemClass() == ITEM_CLASS_EVENT_STAR && p_item->IsGearItem())
		{
			line_count++;
		}
		if(pMouseItem != NULL && pMouseItem->GetItemClass() == ITEM_CLASS_MIXING_ITEM &&
			pMouseItem->GetItemType()>= 9 &&pMouseItem->GetItemType()<= 17 && p_item != NULL &&
			!p_item->IsEmptyItemOptionList() && !p_item->IsQuestItem() && !p_item->IsUniqueItem())
		{
			line_count++;
			if( C_VS_UI_REMOVE_OPTION::IsCanRemoveOption_Puritas( pMouseItem, p_item) )
			{				
				rect.w = max(rect.w, g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_CAN_REMOVE_OPTION].GetString(), gpC_base->m_item_desc_pi.hfont));
			} else
				rect.w = max(rect.w, g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSGAE_CANNOT_REMOVE_OPTION].GetString(), gpC_base->m_item_desc_pi.hfont));
		}
		if((itemClass == ITEM_CLASS_OUSTERS_WRISTLET || itemClass == ITEM_CLASS_OUSTERS_STONE)
			&& (*g_pItemTable)[itemClass][itemType].ElementalType != ITEMTABLE_INFO::ELEMENTAL_TYPE_ANY)
			line_count++;
		
	}
	

	if(p_item->GetItemClass() == ITEM_CLASS_PET_FOOD)
	{
		line_count++;
		PETINFO *pPetInfo = gC_vs_ui.GetMyPetInfo();
		if(pPetInfo != NULL && pPetInfo->EXIST)
		{
			rect.w = max(rect.w,g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_CAN_USE_PET_FOOD].GetString(), gpC_base->m_item_desc_pi.hfont));
		}
		else
		{
			rect.w = max(rect.w,g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_DETACH_PET_FOOD].GetString(), gpC_base->m_item_desc_pi.hfont));
		}
	}
	
	sz_name[0] = '\0'; // for strcat()
	sz_ename[0] = '\0'; // for strcat()
	strcat(sz_ename, "(");

	if (g_pTimeItemManager->IsExist( p_item->GetID() ) )
	{
		strcat( sz_name, (*g_pGameStringTable)[UI_STRING_MESSAGE_QUEST_HAN].GetString() );
		strcat(sz_name, " "); // add 'space'
		strcat( sz_ename,(*g_pGameStringTable)[UI_STRING_MESSAGE_QUEST_ENG].GetString() );
		strcat(sz_ename, " "); // add 'space'
	}
	
	if(p_item->GetItemClass() == ITEM_CLASS_PET_ITEM)
	{
		if(p_item->GetSilver() > 0)
		{
			MPetItem *pPetItem = (MPetItem *)p_item;
			
			strcat(sz_name, pPetItem->GetPetOptionName().c_str());
//				strcat(sz_name, " "); // add 'space'

			strcat(sz_ename, pPetItem->GetPetOptionEName().c_str());
//				strcat(sz_ename, " "); // add 'space'
		}
	}
	else if (!p_item->IsEmptyItemOptionList() && p_item->GetItemClass() != ITEM_CLASS_CODE_SHEET )
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

//	strcat(sz_name, p_item->GetName());
//	strcat(sz_ename, p_item->GetEName());
//	strcat(sz_ename, ")");
//
//	rect.w = max(g_GetStringWidth(sz_name, gpC_base->m_item_name_pi.hfont), g_GetStringWidth(sz_ename, gpC_base->m_item_desc_pi.hfont));
//	rect.h = NORMAL_FONT_Y_GAP+(line_count-1)*SMALL_FONT_Y_GAP+5;
//
//	if((*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].Description.GetLength() != 0)
//		rect.w = max(rect.w, g_GetStringWidth((*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].Description.GetString(), gpC_base->m_item_desc_pi.hfont));

	
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

	if(gpC_mouse_pointer != NULL && pMouseItem != NULL &&
		pMouseItem->GetItemClass() == ITEM_CLASS_EVENT_STAR )
	{	
		rect.w = max (rect.w, g_GetStringWidth( (*g_pGameStringTable)[UI_STRING_MESSAGE_CAN_TRANS].GetString(), 
			gpC_base->m_item_desc_pi.hfont) );
	}

	strcat(sz_name, p_item->GetName());
	strcat(sz_ename, p_item->GetEName());
	strcat(sz_ename, ")");

	rect.w = max(rect.w,max(g_GetStringWidth(sz_name, gpC_base->m_item_name_pi.hfont), g_GetStringWidth(sz_ename, gpC_base->m_item_desc_pi.hfont)));
	rect.h = NORMAL_FONT_Y_GAP+(line_count-1)*SMALL_FONT_Y_GAP+5;

	if((*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].Description.GetLength() != 0)
	{
		int MaxWidth = 0;
		if(p_item->GetItemClass() == ITEM_CLASS_BLOOD_BIBLE_SIGN)
			MaxWidth = g_GetStringWidth((*g_pGameStringTable)[STRING_MESSAGE_BLOOD_BIBLE_BONUS_ARMEGA+p_item->GetItemType()].GetString(), gpC_base->m_item_desc_pi.hfont);
		else
			MaxWidth = g_GetStringWidth((*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].Description.GetString(), gpC_base->m_item_desc_pi.hfont);
		rect.w = max(rect.w, MaxWidth);
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
	if (rect.x+rect.w > g_GameRect.right)
		rect.x = g_GameRect.right-rect.w;
	if (rect.y < 0)
		rect.y = y+(p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y);
	if (rect.y+rect.h > g_GameRect.bottom)
		rect.y = g_GameRect.bottom-rect.h;

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

	int line_count = 3;

	//////////////////////////////// start calculation
	if( (*g_pSkillInfoTable)[left].GetLearnLevel() > g_pSystemAvailableManager->GetLimitLearnSkillLevel() )
	{
		line_count = 2;
		rect.w = DEFAULT_BOX_WIDTH;
		rect.h = NORMAL_FONT_Y_GAP+(line_count)*SMALL_FONT_Y_GAP;
		rect.x = x;
		rect.y = y-rect.h;
		
		g_descriptor_manager.RectCalculationFinished(fp_show, rect, void_ptr, left, right);
		return;
	}	
	if (g_eRaceInterface == RACE_VAMPIRE || (*g_pSkillInfoTable)[left].IsPassive() && g_eRaceInterface != RACE_OUSTERS || left == SKILL_LOVE_CHAIN)
	{
		line_count--;
	}

	if(g_eRaceInterface == RACE_OUSTERS)
	{
		SKILLINFO_NODE sInfo = (*g_pSkillInfoTable)[left];
		if(sInfo.ElementalDomain == ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE ||
			sInfo.ElementalDomain == ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER ||
			sInfo.ElementalDomain == ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH ||
			sInfo.ElementalDomain == ITEMTABLE_INFO::ELEMENTAL_TYPE_WIND
			)
		{
			line_count++;
		}
	}

	if(left >= SKILL_HOLYLAND_BLOOD_BIBLE_ARMEGA && left <= SKILL_HOLYLAND_BLOOD_BIBLE_CHASPA)
		rect.w = g_GetStringWidth((*g_pGameStringTable)[STRING_MESSAGE_BLOOD_BIBLE_BONUS_ARMEGA+left-SKILL_HOLYLAND_BLOOD_BIBLE_ARMEGA].GetString(), gpC_base->m_item_desc_pi.hfont);
	else if(left >= SKILL_SWEEPER_BONUS_1 && left <= SKILL_SWEEPER_BONUS_12 )
		rect.w = g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_SWEEPER_BONUS_1+left-SKILL_SWEEPER_BONUS_1].GetString(), gpC_base->m_item_desc_pi.hfont);
	else if(left == SKILL_LOVE_CHAIN)
		rect.w = g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_LOVE_CHAIN].GetString(), gpC_base->m_item_desc_pi.hfont);
	else
		rect.w = 0;


	rect.w = max(rect.w, g_GetStringWidth((*g_pSkillInfoTable)[left].GetHName(), gpC_base->m_item_name_pi.hfont));
	rect.w = max(rect.w, g_GetStringWidth((*g_pSkillInfoTable)[left].GetName(), gpC_base->m_item_desc_pi.hfont));
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
	if (rect.x+rect.w > g_GameRect.right)
		rect.x = g_GameRect.right-rect.w;
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
	if (rect.x+rect.w > g_GameRect.right)
		rect.x = g_GameRect.right-rect.w;
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
	if (rect.x+rect.w > g_GameRect.right)
		rect.x = g_GameRect.right-rect.w;
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
	int line_count = 4;

	if(left < 0)return;
	//////////////////////////////// start calculation
	if( (*g_pSkillInfoTable)[left].GetLearnLevel() > g_pSystemAvailableManager->GetLimitLearnSkillLevel() )
	{
		line_count = 2;
		rect.w = DEFAULT_BOX_WIDTH;
		rect.h = NORMAL_FONT_Y_GAP+(line_count)*SMALL_FONT_Y_GAP;
		rect.x = x;
		rect.y = y-rect.h;		
		g_descriptor_manager.RectCalculationFinished(fp_show, rect, void_ptr, left, right);
		return;
	}
	
	MSkillDomain::SKILLSTATUS status = (*g_pSkillManager)[*(SKILLDOMAIN *)void_ptr].GetSkillStatus((ACTIONINFO)left);
//	assert(status != MSkillDomain::SKILLSTATUS_NULL);
//	if (status == MSkillDomain::SKILLSTATUS_NEXT ||
//		 status == MSkillDomain::SKILLSTATUS_OTHER)
//		line_count++;

	if ((g_eRaceInterface != RACE_SLAYER || (*g_pSkillInfoTable)[left].IsPassive()) && status == MSkillDomain::SKILLSTATUS_LEARNED)
	{
		line_count--;
	}
	else if(status == MSkillDomain::SKILLSTATUS_LEARNED && g_eRaceInterface == RACE_SLAYER)
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
	if(g_eRaceInterface == RACE_OUSTERS)
	{
		SKILLINFO_NODE sInfo = (*g_pSkillInfoTable)[left];
		if(sInfo.ElementalDomain == ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE ||
			sInfo.ElementalDomain == ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER ||
			sInfo.ElementalDomain == ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH ||
			sInfo.ElementalDomain == ITEMTABLE_INFO::ELEMENTAL_TYPE_WIND
			)
		{
			line_count++;
		}
		line_count++;
		line_count++;
		
		// 2004, 10, 16, sobiet add start
		if(sInfo.GetLearnLevel()!= 1 && left <365 )
			line_count++;
		// 2004, 10, 16, sobiet add end
	}

	rect.w = g_GetStringWidth((*g_pSkillInfoTable)[left].GetHName(), gpC_base->m_item_name_pi.hfont);	
	rect.w = max( rect.w,g_GetStringWidth((*g_pSkillInfoTable)[left].GetName(), gpC_base->m_item_desc_pi.hfont));

	if(left >= SKILL_HOLYLAND_BLOOD_BIBLE_ARMEGA && left <= SKILL_HOLYLAND_BLOOD_BIBLE_CHASPA)
	{
		rect.w = max(rect.w, g_GetStringWidth((*g_pGameStringTable)[STRING_MESSAGE_BLOOD_BIBLE_BONUS_ARMEGA+left-SKILL_HOLYLAND_BLOOD_BIBLE_ARMEGA].GetString(), gpC_base->m_item_desc_pi.hfont));
	}
	else if (left >= SKILL_SWEEPER_BONUS_1 && left <= SKILL_SWEEPER_BONUS_12 )
	{
		rect.w = max(rect.w, g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_SWEEPER_BONUS_1+left-SKILL_SWEEPER_BONUS_1].GetString(), gpC_base->m_item_desc_pi.hfont));
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
	if (rect.x+rect.w > g_GameRect.right)
		rect.x = g_GameRect.right-rect.w;
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
	if (rect.x+rect.w > g_GameRect.right)
		rect.x = g_GameRect.right-rect.w;
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
	if (rect.x+rect.w > g_GameRect.right)
		rect.x = g_GameRect.right-rect.w;
	if (rect.y < 0)
		rect.y = 0;
//		rect.y = y+right;

	//////////////////////////////// end calculation

	g_descriptor_manager.RectCalculationFinished(fp_show, rect, void_ptr, left, right);
}


//-----------------------------------------------------------------------------
// _Multiline_Description_Calculator
//
// void_ptr = S_DEFAULT_HELP_STRING *
// left = item count
//-----------------------------------------------------------------------------
void _Multiline_Info_Calculator(void (*fp_show)(Rect, void *, long, long), int x, int y, void * void_ptr, long left, long right)
{
	Rect rect;

	//////////////////////////////// start calculation
	
	int line_count = 0;

	if(right != 0) // ÇÑ¶óÀÎ¿¡ Ç¥½ÃÇÒ ½ºÆ®¸µ
	{
		if(NULL != void_ptr)
		{	
			line_count = (strlen((char*)void_ptr)/right) + 1;

			char szTempBuf[128] = {0,};
			memcpy(szTempBuf, void_ptr, right);
			rect.w = g_GetStringWidth((const char *)szTempBuf, gpC_base->m_item_name_pi.hfont);
		}
		else
			rect.w = g_GetStringWidth((const char *)void_ptr, gpC_base->m_item_name_pi.hfont);
	}
	else
		rect.w = g_GetStringWidth((const char *)void_ptr, gpC_base->m_item_name_pi.hfont);

	rect.h = (line_count)*NORMAL_FONT_Y_GAP;

	rect.w += SIDE_GAP;
	rect.h += SIDE_GAP;

	// get (x, y)
	rect.x = x;
	rect.y = y-rect.h;

	if (rect.x < 0)
		rect.x = 0;
	if (rect.x+rect.w > g_GameRect.right)
		rect.x = g_GameRect.right-rect.w;
	if (rect.y < 0)
		rect.y = y+right;
	//////////////////////////////// end calculation

	g_descriptor_manager.RectCalculationFinished(fp_show, rect, void_ptr, left, right);
}

//-----------------------------------------------------------------------------
// _Info_Description_Show
//
// 
//-----------------------------------------------------------------------------
void	_Multiline_Info_Show(Rect rect, void * void_ptr, long left, long right)
{
//#ifdef _LIB // Client¿¡¼­´Â 3D¿ëÀ¸·Î »ç¿ëÇÒ ¼ö ÀÖ´Ù. -.-
	if(NULL == void_ptr)
		return;
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

	int CurrentPos = right;

	int LineCount = (strlen((char*)void_ptr)/right) + 1;

	char sz_temp[4048];
	char *cur = (char*)void_ptr;
	char char_temp;
	
	strcpy(sz_temp, cur);
	int TempCount = 0;
	while(TempCount<LineCount)
	{
		int check;
		if(g_PossibleStringCut(cur, CurrentPos))
			check = 0; 
		else 
			check = 1;
		
		char_temp = cur[CurrentPos - check];
		cur[CurrentPos - check] = '\0';
		
		g_PrintColorStr(px, py, cur, gpC_base->m_item_name_pi, left);
		py += NORMAL_FONT_Y_GAP;
		
		if(strlen(cur) < CurrentPos-check) 
			break;
		cur += CurrentPos-check;
		*cur = char_temp;
		if(*cur == ' ')cur++;
		TempCount ++;
	}
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
	g_descriptor_manager.AddDescribedUnit(DID_MULTILINE_INFO, _Multiline_Info_Calculator, _Multiline_Info_Show);
}

