  // VS_UI_GameCommon.cpp

#include "client_PCH.h"
#include "VS_UI_GameCommon.h"
#include "VS_UI_GameCommon2.h"
#include "VS_UI_GlobalResource.h"
#include "VS_UI_filepath.h"
#include "VS_UI_Mouse_pointer.h"
//#include "VS_UI_Item.h"

#include "VS_UI.h"
#include "MPriceManager.h"
//#include "MCreatureTable.h"
#include "CDirectInput.h"
#include "MParty.h"
#include "VS_UI_Title.h"
#include "ExperienceTable.h"
#include "UserOption.h"
#include "UserInformation.h"
#include "MItemOptionTable.h"
#include "MGameStringTable.H"
#include "KeyAccelerator.h"
#include "ClientConfig.H"
#include "MTimeItemManager.h"
#include "SystemAvailabilities.h"
#include "ShrineInfoManager.h"
#ifdef _LIB
#include "MZone.h"
#endif

#include "MZoneTable.h"

#include <algorithm>
#include <time.h>
#include <stdio.h>
#ifdef PLATFORM_WINDOWS
#include "TCHAR.H"
#endif
#include "CJpeg.h"
#include "MFakeCreature.h"
#include "MPlayer.h"

#include "MHelpDef.h"
#include "MGameTime.h"

#include "SXml.h"

extern DWORD g_CurrentFrame;
extern BOOL g_MyFull;
extern RECT g_GameRect;
#define WHISPER_MAX 10	// by larosel

#define MAX_SLAYER_ATTR_OLD				200
#define MAX_SLAYER_ATTR_SUM_OLD			300
#define MAX_SLAYER_DOMAIN_SUM_OLD		100
#define MAX_VAMPIRE_LEVEL_OLD			100

#define	MAX_SLAYER_ATTR					290
#define	MAX_SLAYER_ATTR_SUM				435
#define MAX_VAMPIRE_LEVEL				150


#define ITEM_NAME_NORMAL_COLOR			RGB_WHITE
#define ITEM_NAME_OPTION_COLOR			RGB_YELLOW
#define ITEM_ENAME_NORMAL_COLOR			RGB(200,200,200)
#define ITEM_ENAME_OPTION_COLOR			RGB(200,200,100)
#define ITEM_NAME_NORMAL_SHADE			RGB(100,100,100)
#define ITEM_NAME_OPTION_SHADE			RGB(100,100,0)
#define ITEM_ENAME_RARE_COLOR			RGB(15<<3,15<<3,31<<3)
#define ITEM_DESC_RGB					RGB(192, 192, 255)

#define MAXBUFFER						20480
#define dSTRING_LEN						2048
#define LIMITLINE						60
#define LINEWIDTH						20


extern CSDLInput*			g_pSDLInput;
extern BOOL g_bActiveGame;
int			g_CurrentSkillID = 0xFFFFFFFF;

#define FONT_GAP							17



class EventGiftInfo
{
public :
	EventGiftInfo() { m_bActive = 1; m_Step = 1; m_Name = "";}
	~EventGiftInfo(){}
	void LoadFromFile(ifstream &file)	{		file.read((char*)&m_ID, sizeof(DWORD) );file.read((char*)&m_bActive, sizeof(char) );		file.read((char*)&m_Step, sizeof( DWORD) );		m_Name.LoadFromFile( file );	}
	void SaveToFile(ofstream &file)	{		file.write((const char*)&m_ID, sizeof(DWORD) );file.write((const char*)&m_bActive, sizeof(char) );		file.write((const char*)&m_Step, sizeof( int ) );		m_Name.SaveToFile( file );	}
	char	m_bActive;	DWORD		m_Step;	MString	m_Name; DWORD m_ID;
};

typedef CTypeTable<EventGiftInfo> CEventGiftInfo;

MItem*		C_VS_UI_ITEM_LIST::m_pItem = NULL;

static C_VS_UI_DIALOG *	gpC_dialog_select_item_from_shop = NULL;
static C_VS_UI_DIALOG * gpC_dialog_ousters_down_skill = NULL;
static C_VS_UI_DIALOG * gpC_dialog_ousters_level_up_to_last_skill_level = NULL;
static C_VS_UI_DIALOG * gpC_dialog_game_over = NULL;
static C_VS_UI_DIALOG * gpC_dialog_all_stage_clear = NULL;

void ExecF_GameOverMessage(C_VS_UI_DIALOG * p_this_dialog, id_t id )
{
	switch( id )
	{
	case DIALOG_EXECID_OK :
		gpC_base->SendMessage(UI_REQUEST_FORCE_DIE, 10 );
		break;
	}
}

void g_StartGameOverMessage( int _x, int _y )
{
	DeleteNew( gpC_dialog_game_over );
	gpC_dialog_game_over = new C_VS_UI_DIALOG(_x,_y, 2, 0, ExecF_GameOverMessage, DIALOG_OK);
//	char overmessage[200] = "Game Over";
	char * pp_dmsg[1] = {
		(*g_pGameStringTable)[STRING_MESSAGE_MINIGAME_GAME_OVER].GetString(),
	};
	gpC_dialog_game_over->SetMessage( pp_dmsg, 1, SMO_NOFIT );
	gpC_dialog_game_over->Start();
}

void ExecF_AllStageClearMessage( C_VS_UI_DIALOG * p_this_dialog, id_t id )
{
	switch( id )
	{
	case DIALOG_EXECID_OK :
		gpC_base->SendMessage(UI_CLEAR_ALL_STAGE);
		break;
	}
}

void g_StartAllStageClearMessage(int _x, int _y )
{
	DeleteNew( gpC_dialog_all_stage_clear );
	gpC_dialog_all_stage_clear = new C_VS_UI_DIALOG( _x, _y, 2, 0, ExecF_AllStageClearMessage, DIALOG_OK );

//	char clearmessage[200] = "All Stage Clear!!!";
	char * pp_dmsg[1] = {
		(*g_pGameStringTable)[STRING_MESSAGE_MINIGAME_ALL_STAGE_CLEAR].GetString(),
	};

	PlaySound( SOUND_EVENT_FANFARE );

	gpC_dialog_all_stage_clear->SetMessage( pp_dmsg, 1, SMO_NOFIT );
	gpC_dialog_all_stage_clear->Start();
}

void ExecF_SelectItemFromShop(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch (id)
	{
	case DIALOG_EXECID_OK:
		//gpC_base->SendMessage(UI_ITEM_REPAIR_FINISHED);
		if(C_VS_UI_ITEM_LIST::m_pItem != NULL)
			gpC_base->SendMessage(UI_SELECT_ITEM_FROM_SHOP, C_VS_UI_ITEM_LIST::m_pItem->GetID());
		break;
	}
}

void g_StartTakeOutConfirmDialog(int _x, int _y)
{
	DeleteNew(gpC_dialog_select_item_from_shop);
	
	gpC_dialog_select_item_from_shop = new C_VS_UI_DIALOG(_x, _y, 2, 0, ExecF_SelectItemFromShop, DIALOG_OK|DIALOG_CANCEL);
	char * pp_dmsg_sell_confirm[1] = {(*g_pGameStringTable)[UI_STRING_MESSAGE_CONFIRM_SELECT_ITEM_FROM_SHOP].GetString()};
	gpC_dialog_select_item_from_shop->SetMessage(pp_dmsg_sell_confirm, 1, SMO_NOFIT);	
	gpC_dialog_select_item_from_shop->Start();
}

void ExecF_ConfirmUpToLastSkillLevel(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch ( id )
	{
	case DIALOG_EXECID_OK :
		if( g_CurrentSkillID != 0xFFFFFFFF )
			gpC_base->SendMessage( UI_OUSTERS_LEARN_SKILL, g_CurrentSkillID );
		break;
	}
}

void g_StartComfirmUpToLastSkillLevel( int _x, int _y )
{
	DeleteNew( gpC_dialog_ousters_level_up_to_last_skill_level );

	gpC_dialog_ousters_level_up_to_last_skill_level = new C_VS_UI_DIALOG(_x, _y, 3, 1, ExecF_ConfirmUpToLastSkillLevel, DIALOG_OK|DIALOG_CANCEL );
	char * pp_dmsg[1] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CONFIRM_UP_TO_LAST_SKILL_LEVEL].GetString()
	};
	gpC_dialog_ousters_level_up_to_last_skill_level->SetMessage( pp_dmsg, 1, SMO_NOFIT );
	gpC_dialog_ousters_level_up_to_last_skill_level->Start();
}

void ExecF_DownSkillLevel(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch( id )
	{
	case DIALOG_EXECID_OK :
		if( g_CurrentSkillID != 0xFFFFFFFF )
		{
			gpC_base->SendMessage( UI_OUSTERS_DOWN_SKILL, g_CurrentSkillID );
		}
		break;
	}
}

void ExecF_GQuestNpcScript(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	switch( id )
	{
	case DIALOG_EXECID_OK :
		gpC_base->SendMessage(UI_RUN_NEXT_GQUEST_EXCUTE_ELEMENT); 
		break;
	}
	return;
}

void g_StartOustersDownSkill( int _x, int _y , int PriceRaito)
{
	DeleteNew(gpC_dialog_ousters_down_skill);

	gpC_dialog_ousters_down_skill = new C_VS_UI_DIALOG( _x, _y, 3, 1, ExecF_DownSkillLevel, DIALOG_OK|DIALOG_CANCEL );
	char szBuffer[256];
	char szCost[100];

	// 1�������Ҵ�ȸ������Ʈ * ���ҷ��� * (ĳ���ͷ���)^1.3 * 200
	int DownPrice = int ( float((*g_pSkillInfoTable)[g_CurrentSkillID].LevelUpPoint) * pow(g_char_slot_ingame.level, 1.3 ) * 200.0f   ) * PriceRaito;
	if(0 == DownPrice)
		DownPrice = 1000000; // �鸸��
	wsprintf(szCost,"%d", DownPrice);
	
	std::string sstr = szCost;
	for(int i = 3; i <= 13; i += 4)
		if(sstr.size() > i)sstr.insert(sstr.size()-i, ",");

	if(PriceRaito != 1)
	{
		wsprintf(szBuffer, (*g_pGameStringTable)[UI_STRING_MESSAGE_CONFIRM_DOWN_SKILL2].GetString(),
			(*g_pSkillInfoTable)[g_CurrentSkillID].GetHName(),
			sstr.c_str() );		
	}
	else
	{
		wsprintf(szBuffer, (*g_pGameStringTable)[UI_STRING_MESSAGE_CONFIRM_DOWN_SKILL].GetString(),
			(*g_pSkillInfoTable)[g_CurrentSkillID].GetHName(),
			(*g_pSkillInfoTable)[g_CurrentSkillID].GetExpLevel(),
			(*g_pSkillInfoTable)[g_CurrentSkillID].GetExpLevel()-1,
			sstr.c_str() );		
	}		
	char * pp_dmsg[1] = 
	{
		szBuffer,
	};
	gpC_dialog_ousters_down_skill->SetMessage( pp_dmsg, 1, SMO_NOFIT );
	gpC_dialog_ousters_down_skill->Start();
}

//------------------------------------------------------------------------------
// C_VS_UI_ITEM_LIST
//
// �˾�â�� ����.
//------------------------------------------------------------------------------
C_VS_UI_ITEM_LIST::C_VS_UI_ITEM_LIST()
{	
	int window_x,window_y,window_w,window_h;


	AttrTopmost( true );
	window_w = 450;
	window_h = 400;

	window_x = g_GameRect.right / 2 - window_w/2;
	window_y = g_GameRect.bottom / 2 - window_h/2;

	m_ItemList.clear();

	m_pItem = NULL;
	
	Set(window_x, window_y, window_w, window_h);

	int ok_x = w-150,ok_y = h-44;
	int cancel_x=w-70,cancel_y=h-44;
	
	int button_y1 = 190;
	int prev_button_x = 30;
	int next_button_y = 100;
	
	m_pC_button_group = new ButtonGroup(this);
	
 
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(cancel_x,cancel_y,
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE),
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE),
		OK_ID,this,C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE));


	m_pC_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(w-90,h-165,-1,130));
	m_pC_scroll_bar->SetScrollPos(0);
	m_pC_scroll_bar->SetPosMax(0);

	m_CurrentItem = 0;
	g_RegisterWindow(this);
}

C_VS_UI_ITEM_LIST::~C_VS_UI_ITEM_LIST()
{	
	ClearItem();
	g_UnregisterWindow(this);

	DeleteNew(gpC_dialog_select_item_from_shop);
	DeleteNew(m_pC_button_group);
	DeleteNew(m_pC_scroll_bar);
}

void	C_VS_UI_ITEM_LIST::Start()
{
	PI_Processor::Start();
	gpC_window_manager->AppearWindow(this);
	AttrPin(false);
	AttrTopmost( true );
	m_pC_button_group->Init();	
}

void	C_VS_UI_ITEM_LIST::Finish()
{
	ClearItem();
	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);
}

void	C_VS_UI_ITEM_LIST::Show()
{
	MItem *pCurrentFocusItem = NULL;

	if( !( m_CurrentItem < 0 || m_CurrentItem >= m_ItemList.size() ) )
		pCurrentFocusItem = m_ItemList[m_CurrentItem].pItem;

	Rect rect(x+20,y+20,110,139);
	Rect ItemListRect(rect.x, rect.y + rect.h + 20 + 40, 340,160);

	RECT BoxRect = {rect.x, rect.y , rect.x+rect.w, rect.y+rect.h};
	RECT ItemListRECT = {ItemListRect.x,ItemListRect.y,ItemListRect.x+ItemListRect.w,ItemListRect.y+ItemListRect.h};
		
	gpC_global_resource->DrawDialog(x,y,w,h,g_pUserOption->DefaultAlpha == TRUE);
		
	if( g_pUserOption->DefaultAlpha == FALSE )
	{
		gpC_base->m_p_DDSurface_back->FillRect( &BoxRect,0 );
		gpC_base->m_p_DDSurface_back->FillRect( &ItemListRECT,0 );
	}
	
	if(gpC_base->m_p_DDSurface_back->Lock())
	{		
		
		if( m_CurrentItem >= m_pC_scroll_bar->GetScrollPos() && m_pC_scroll_bar->GetScrollPos() + 8 > m_CurrentItem )
		{
			RECT SelectRECT = { ItemListRect.x, ItemListRect.y + (m_CurrentItem - m_pC_scroll_bar->GetScrollPos()) * 20 , ItemListRect.x+ ItemListRect.w, ItemListRect.y + 20 + (m_CurrentItem - m_pC_scroll_bar->GetScrollPos() ) * 20 };
			DrawAlphaBox(&SelectRECT, 12,12,12 ,g_pUserOption->ALPHA_DEPTH);
			Rect SelectBox( SelectRECT.left+2, SelectRECT.top, SelectRECT.right - SelectRECT.left-4, SelectRECT.bottom - SelectRECT.top + 2 );
			gpC_global_resource->DrawOutBoxLocked( SelectBox );
		}		
		
		if( !m_ItemList.empty() )
		{
			if(pCurrentFocusItem != NULL )
			{
				TYPE_FRAMEID FrameID = pCurrentFocusItem->GetInventoryFrameID();
				int item_x = (BoxRect.left +(BoxRect.right - BoxRect.left)/2) - gpC_item->GetWidth( FrameID) /2;
				int item_y = (rect.y + rect.h/2) - gpC_item->GetHeight( FrameID ) /2 ;
				
				if(pCurrentFocusItem->IsSpecialColorItem() || pCurrentFocusItem->IsQuestItem())
					CIndexSprite::SetUsingColorSet(pCurrentFocusItem->GetSpecialColorItemColorset(), 0);
				else			
					CIndexSprite::SetUsingColorSet(pCurrentFocusItem->GetItemOptionColorSet(), 0);
				
				if (pCurrentFocusItem->IsAffectStatus() || pCurrentFocusItem->IsQuestItem())
				{
					// frame id -> sprite id
					gpC_item->BltLocked(item_x, item_y, FrameID);
				}
				else
				{
					gpC_item->BltLockedColorSet(item_x, item_y, FrameID, ITEM_DISABLE_COLOR_SET);
				}
			}
		}
		gpC_global_resource->DrawOutBoxLocked(rect);
		gpC_global_resource->DrawOutBoxLocked(ItemListRect);

		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	
	m_pC_scroll_bar->Show(x,y);


	const int line_num = 8;
	int line_gap = 20;
	int i; // Declare i outside for loop scope

	for(i=0;i<line_num -1 ;i++)
	{
		if( m_CurrentItem >= m_pC_scroll_bar->GetScrollPos() && m_CurrentItem < m_pC_scroll_bar->GetScrollPos() + 8 )
		{
			if( m_CurrentItem - m_pC_scroll_bar->GetScrollPos() != i &&
				max(0,m_CurrentItem - m_pC_scroll_bar->GetScrollPos() -1 ) != i)
				gpC_base->m_p_DDSurface_back->HLine(ItemListRect.x+3,ItemListRect.y + line_gap *(i+1) ,ItemListRect.w-5,DARKGRAY);
		} else
		{
			gpC_base->m_p_DDSurface_back->HLine(ItemListRect.x+3,ItemListRect.y + line_gap *(i+1) ,ItemListRect.w-5,DARKGRAY);
		}
	}

	g_FL2_GetDC();	
	g_PrintColorStr(x+35, y+200, (*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_SHOP].GetString(), gpC_base->m_chatting_pi, RGB_WHITE);
	for(i=0;i<line_num;i++)
	{
		COLORREF TitleColor,ShadowColor;
		const COLORREF required_rgb = RGB(255, 128, 64);
		
		if( !(i+m_pC_scroll_bar->GetScrollPos() >= 0 && m_ItemList.size() > i+m_pC_scroll_bar->GetScrollPos() ))
			continue;
		
		MItem *pItem = m_ItemList[i + m_pC_scroll_bar->GetScrollPos() ].pItem;
		
		if ( pItem->IsSpecialColorItem()|| pItem->IsQuestItem() )
		{
			WORD temp_color;
			if( pItem->IsQuestItem())
				temp_color = CIndexSprite::ColorSet[MItem::GetQuestItemColorset()][15];
			else
				temp_color = CIndexSprite::ColorSet[MItem::GetUniqueItemColorset()][15];				
			TitleColor = RGB(CSDLGraphics::Red(temp_color)<<3, CSDLGraphics::Green(temp_color)<<3, CSDLGraphics::Blue(temp_color)<<3);
			
			if( pItem->IsQuestItem())
				temp_color = CIndexSprite::ColorSet[MItem::GetQuestItemColorset()][29];			
			else
				temp_color = CIndexSprite::ColorSet[MItem::GetUniqueItemColorset()][29];				
			ShadowColor = RGB(CSDLGraphics::Red(temp_color)<<3, CSDLGraphics::Green(temp_color)<<3, CSDLGraphics::Blue(temp_color)<<3);
		}
		else
		if ( pItem->GetItemOptionListCount() ==2)
		{
			TitleColor = g_pClientConfig->COLOR_NAME_ITEM_RARE_OPTION;
			ShadowColor = 0;//RGB(0,0,31<<3);
		}
		// add by Sonic 2006.10.28 ������ʾ������װ��Ϊ��ɫ
		else if(pItem->GetItemOptionListCount() > 2)
		{
			TitleColor = g_pClientConfig->COLOR_NAME_VAMPIRE; //Red
			ShadowColor = g_pClientConfig->COLOR_NAME_VAMPIRE; //Red
		}
		// end by Sonic 2006.10.28 ������ʾ������װ��Ϊ��ɫ
		else
		if ( pItem->IsEmptyItemOptionList() )
		{
			TitleColor = ITEM_NAME_NORMAL_COLOR;
			ShadowColor = ITEM_NAME_NORMAL_SHADE;
		}
		else
		{
			TitleColor = ITEM_NAME_OPTION_COLOR;		
			ShadowColor = ITEM_NAME_OPTION_SHADE;
		}

		std::string item_name;
		if(pItem->IsQuestItem())
		{
			item_name += (*g_pGameStringTable)[UI_STRING_MESSAGE_QUEST_HAN].GetString();

			item_name += " ";
		}
		item_name += GetItemName( i+ m_pC_scroll_bar->GetScrollPos() ).c_str();
		
		g_PrintColorStr(ItemListRect.x+5+1, ItemListRect.y +4 +1+ line_gap*i, 
			item_name.c_str()
			, gpC_base->m_chatting_pi, ShadowColor);		

		g_PrintColorStr(ItemListRect.x+5, ItemListRect.y +4 + line_gap*i, 
			item_name.c_str()
			, gpC_base->m_chatting_pi, TitleColor);		
	}

	char tempstr[256];
	wsprintf(tempstr,"<%d/%d>",m_ItemList.empty()?0:m_CurrentItem+1, m_ItemList.size());
	int len = g_GetStringWidth(tempstr, gpC_base->m_chatting_pi.hfont);	
	g_PrintColorStr(rect.x+rect.w/2 - len/2,y+162, tempstr, gpC_base->m_chatting_pi, RGB_WHITE);
	
	if(pCurrentFocusItem != NULL)
	{
		int strX, strY;
		strX = rect.x+rect.w + 20;
		strY = rect.y;
		COLORREF TitleColor,ShadowColor;
		const COLORREF required_rgb = RGB(255, 128, 64);

		if ( pCurrentFocusItem->IsSpecialColorItem()|| pCurrentFocusItem->IsQuestItem())
		{
			WORD temp_color;
			if( pCurrentFocusItem->IsQuestItem())
				temp_color = CIndexSprite::ColorSet[MItem::GetQuestItemColorset()][15];
			else
				temp_color = CIndexSprite::ColorSet[MItem::GetUniqueItemColorset()][15];				
			TitleColor = RGB(CSDLGraphics::Red(temp_color)<<3, CSDLGraphics::Green(temp_color)<<3, CSDLGraphics::Blue(temp_color)<<3);
			
			if( pCurrentFocusItem->IsQuestItem())
				temp_color = CIndexSprite::ColorSet[MItem::GetQuestItemColorset()][29];			
			else
				temp_color = CIndexSprite::ColorSet[MItem::GetUniqueItemColorset()][29];				
			ShadowColor = RGB(CSDLGraphics::Red(temp_color)<<3, CSDLGraphics::Green(temp_color)<<3, CSDLGraphics::Blue(temp_color)<<3);
		}
		else
		if ( pCurrentFocusItem->GetItemOptionListCount() == 2)
		{
			TitleColor = g_pClientConfig->COLOR_NAME_ITEM_RARE_OPTION;
			ShadowColor = 0;//RGB(0,0,31<<3);
		}
		// add by Sonic 2006.10.28 ������ʾ������װ��Ϊ��ɫ
		else if(pCurrentFocusItem->GetItemOptionListCount() > 2)
		{
			TitleColor = g_pClientConfig->COLOR_NAME_VAMPIRE; //Red
			ShadowColor = g_pClientConfig->COLOR_NAME_VAMPIRE; //Red
		}
		// end by Sonic 2006.10.28 ������ʾ������װ��Ϊ��ɫ
		else
		if ( pCurrentFocusItem->IsEmptyItemOptionList() )
		{
			TitleColor = ITEM_NAME_NORMAL_COLOR;
			ShadowColor = ITEM_NAME_NORMAL_SHADE;
		}
		else
		{
			TitleColor = ITEM_NAME_OPTION_COLOR;		
			ShadowColor = ITEM_NAME_OPTION_SHADE;
		}

		g_PrintColorStr(strX+1, strY+1, GetItemName(m_CurrentItem).c_str(), gpC_base->m_char_chat_pi, ShadowColor);
		g_PrintColorStr(strX,strY, GetItemName(m_CurrentItem).c_str(), gpC_base->m_char_chat_pi, TitleColor );

		strY+=line_gap;
		line_gap = 14;

		char sz_buf[256]={0,};
		switch(pCurrentFocusItem->GetItemClass())
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
			if(pCurrentFocusItem->GetItemType() < 5)
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
			int vx = g_PrintColorStr(strX, strY, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_CLASS].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			g_PrintColorStr(vx, strY, sz_buf, gpC_base->m_item_desc_pi, required_rgb);			
			strY+=line_gap;
		}
		int vx;
		// Damage
		if (pCurrentFocusItem->GetMaxDamage() != -1) // damage�� �ִ°�?
		{
			vx = g_PrintColorStr(strX, strY, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DAMAGE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			sprintf(sz_buf, "%d~%d", pCurrentFocusItem->GetMinDamage(), pCurrentFocusItem->GetMaxDamage());				
			g_PrintColorStr(vx, strY, sz_buf, gpC_base->m_item_desc_pi, RGB_RED);
			strY += line_gap;
		}			
		// critical hit
		if (pCurrentFocusItem->GetCriticalHit() != -1)//ũ��Ʈ�� ��Ʈ�� �ִ°�?
		{
			vx = g_PrintColorStr(strX, strY, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_CRITICALHIT].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			sprintf(sz_buf, "%d", pCurrentFocusItem->GetCriticalHit());
			g_PrintColorStr(vx, strY, sz_buf, gpC_base->m_item_desc_pi, RGB_RED);
			strY += line_gap;
		}		
		// Defense
		if (pCurrentFocusItem->GetDefenseValue() != -1)
		{
			vx = g_PrintColorStr(strX, strY,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DEFENSE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			sprintf(sz_buf, "%d", pCurrentFocusItem->GetDefenseValue());
			g_PrintColorStr(vx, strY, sz_buf, gpC_base->m_item_desc_pi, RGB_GREEN);				
			strY += line_gap;
		}			
		// Protection
		if (pCurrentFocusItem->GetProtectionValue() != -1)
		{
			vx = g_PrintColorStr(strX, strY, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_PROTECTION].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			sprintf(sz_buf, "%d", pCurrentFocusItem->GetProtectionValue());
			g_PrintColorStr(vx, strY, sz_buf, gpC_base->m_item_desc_pi, RGB_GREEN);				
			strY += line_gap;
		}			
		// TOHIT
		if (pCurrentFocusItem->GetToHit() != -1)
		{
			vx = g_PrintColorStr(strX, strY, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ACCURACY].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			sprintf(sz_buf, "%d", pCurrentFocusItem->GetToHit());
			g_PrintColorStr(vx, strY, sz_buf, gpC_base->m_item_desc_pi, RGB_GREEN);				
			strY += line_gap;
		}			
		// Heal point
		if (pCurrentFocusItem->GetHealPoint() != -1)
		{
			vx = g_PrintColorStr(strX, strY, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_HP].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			sprintf(sz_buf, "+%d", pCurrentFocusItem->GetHealPoint());
			g_PrintColorStr(vx, strY, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
			strY += line_gap;
		}			
		// Mana point
		if (pCurrentFocusItem->GetManaPoint() != -1)
		{
			vx = g_PrintColorStr(strX, strY, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_MP].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			sprintf(sz_buf, "+%d", pCurrentFocusItem->GetManaPoint());
			g_PrintColorStr(vx, strY, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
			strY += line_gap;
		}
		
		// Attack range
		if (pCurrentFocusItem->GetReach() != -1 && pCurrentFocusItem->GetReach() != 1)
		{
			vx = g_PrintColorStr(strX, strY, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_RANGE].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			sprintf(sz_buf, "%d %s", pCurrentFocusItem->GetReach(),(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_TILE_PIECE].GetString());
			g_PrintColorStr(vx, strY, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
			strY += line_gap;
		}			
		// Bullet
		if (pCurrentFocusItem->GetMagazineSize() != -1)
		{
			vx = g_PrintColorStr(strX, strY, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_MAGAZINE_NUM].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			sprintf(sz_buf, "%d %s", pCurrentFocusItem->GetMagazineSize(),(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_MAGAZINE_COUNT].GetString());
			g_PrintColorStr(vx, strY, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
			strY += line_gap;
		}
		// Pocket size
		if (pCurrentFocusItem->GetPocketNumber() != -1)
		{
			vx = g_PrintColorStr(strX, strY, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_POCKET_NUM].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			sprintf(sz_buf, "%d %s", pCurrentFocusItem->GetPocketNumber(),(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_NUMBER].GetString());
			g_PrintColorStr(vx, strY, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
			strY += line_gap;
		}			
		// Pile size
		if (pCurrentFocusItem->IsPileItem() == true)
		{
			vx = g_PrintColorStr(strX, strY, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ITEM_NUM].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			sprintf(sz_buf, "%d %s", pCurrentFocusItem->GetNumber(),(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_NUMBER].GetString());
			g_PrintColorStr(vx, strY, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);			
			strY += line_gap;
		}
		
		// Charge size
		if (pCurrentFocusItem->IsChargeItem() == true)
		{
			vx = g_PrintColorStr(strX, strY,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_LEFT_NUM].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			sprintf(sz_buf, "%d/%d", pCurrentFocusItem->GetNumber(), pCurrentFocusItem->GetMaxNumber());
			g_PrintColorStr(vx, strY, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);				
			strY += line_gap;
		}
		if (pCurrentFocusItem->IsIdentified() && pCurrentFocusItem->GetItemClass() == ITEM_CLASS_VAMPIRE_PORTAL_ITEM)
		{
			vx = g_PrintColorStr(strX, strY, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_ARRIVAL_LOCATION].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			MVampirePortalItem *tempItem = (MVampirePortalItem *)pCurrentFocusItem;

			if(tempItem->IsMarked())
			{
				sprintf(sz_buf, "%s", g_pZoneTable->Get(tempItem->GetZoneID())->Name.GetString());
				g_PrintColorStr(vx, strY, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
				strY += line_gap;
				
				sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_ZONEINFO_XY].GetString(), tempItem->GetZoneX(), tempItem->GetZoneY());
				g_PrintColorStr(vx, strY, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
				strY += line_gap;
			}
			else
			{
				sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_NOT_EXIST].GetString());
				g_PrintColorStr(vx, strY, sz_buf, gpC_base->m_item_desc_pi, RGB_WHITE);
				strY += line_gap;
			}
		}

		// 2004, 5, 18 sobeit add start - Item Grade 
		if (pCurrentFocusItem->GetGrade()>0)
		{
			vx = g_PrintColorStr(strX, strY,(*g_pGameStringTable)[UI_STRING_MESSAGE_ITEM_GRADE_DESC].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			sprintf(sz_buf, " %d", pCurrentFocusItem->GetGrade());
			g_PrintColorStr(vx, strY, sz_buf, gpC_base->m_item_desc_pi, RGB_GREEN);				
			strY += line_gap;
		}
		// 2004, 5, 18 sobeit add end - Item Grade 

		// Add option
		if(pCurrentFocusItem->IsIdentified())
		{			
			BOOL fOptionCheck=FALSE;
			if(!pCurrentFocusItem->IsEmptyItemOptionList())
			{			
				const std::list<TYPE_ITEM_OPTION> &optionList=pCurrentFocusItem->GetItemOptionList();
				std::list<TYPE_ITEM_OPTION>::const_iterator itr=optionList.begin();				
				
				while(itr != optionList.end() && *itr < g_pItemOptionTable->GetSize())
				{
					ITEMOPTION_INFO& optionInfo=(*g_pItemOptionTable)[*itr];
					char pPartName[20];
					strcpy(pPartName,g_pItemOptionTable->ITEMOPTION_PARTNAME[optionInfo.Part].GetString());
					if(pCurrentFocusItem->IsVampireItem() && strstr(pPartName,"MP") != NULL)
						*strstr(pPartName,"MP") = 'H';
					
					BYTE PlusPoint	= optionInfo.PlusPoint; // ����Ǵ� ��ġ
					
					if (pPartName)
					{
						if(!fOptionCheck)
						{
							vx = g_PrintColorStr(strX, strY,(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_OPTION].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
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
							g_PrintColorStr(vx, strY, sz_buf, gpC_base->m_item_desc_pi, RGB_GOLD);//RGB_YELLOW);
					}
					strY += line_gap;
					itr++;
				}
			}
			if(!pCurrentFocusItem->IsEmptyItemDefaultOption())
			{
				const std::list<TYPE_ITEM_OPTION> &DefaultOptionList = pCurrentFocusItem->GetItemDefaultOptionList();
				std::list<TYPE_ITEM_OPTION>::const_iterator itr=DefaultOptionList.begin();
				
				while(itr != DefaultOptionList.end())
				{
					ITEMOPTION_INFO& optionInfo=(*g_pItemOptionTable)[*itr];
					char pPartName[20];
					strcpy(pPartName,g_pItemOptionTable->ITEMOPTION_PARTNAME[optionInfo.Part].GetString());
					if(pCurrentFocusItem->IsVampireItem() && strstr(pPartName,"MP") != NULL)
						*strstr(pPartName,"MP") = 'H';
					
					BYTE PlusPoint	= optionInfo.PlusPoint; // ����Ǵ� ��ġ
					
					if (pPartName)
					{
						if(!fOptionCheck)
						{
							vx = g_PrintColorStr(strX, strY, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_OPTION].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
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
							g_PrintColorStr(vx, strY, sz_buf, gpC_base->m_item_desc_pi, RGB_GOLD);//RGB_YELLOW);
					}
					strY += line_gap;
					itr++;
				}				
			}
		}
		else
		{
			sprintf(sz_buf,"%s???????",(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_OPTION].GetString());
			vx = g_PrintColorStr(strX, strY, sz_buf, gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
			
			strY += line_gap;
		}		
		bool bl_required = false;
		if(pCurrentFocusItem->IsQuestItem() > 0 )
		{
			bool bContinue = false;		
			char temp[256];
			vx = g_PrintColorStr(strX, strY, (*g_pGameStringTable)[UI_STRING_MESSAGE_LEFT_TIME].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB );
			memset( temp, 0, sizeof( temp ) );

			DWORD TimeLimit = m_ItemList[m_CurrentItem].TimeLimit;

			int days = TimeLimit / 60 / 60 / 24;
			int hours = (TimeLimit / 60 / 60) % 24;
			int minutes = (TimeLimit / 60 ) % 60;
			int seconds = TimeLimit % 60;

			
			if(days > 0|| bContinue )
			{
				char tempDay[30] = {0,};
				strcat(temp, " ");
				wsprintf(tempDay, (*g_pGameStringTable)[UI_STRING_MESSAGE_DAY].GetString() , days );
				strcat ( temp, tempDay );				
			}
			if(hours > 0|| bContinue)
			{
				char tempDay[30] = {0,};
				strcat(temp, " ");
				wsprintf(tempDay, (*g_pGameStringTable)[UI_STRING_MESSAGE_HOUR].GetString() , hours );
				strcat ( temp, tempDay );				
			}
			if(minutes || bContinue)
			{
				char tempDay[30] = {0,};
				strcat(temp, " ");
				wsprintf(tempDay, (*g_pGameStringTable)[UI_STRING_MESSAGE_MINUTE].GetString() , minutes );
				strcat ( temp, tempDay );				
			}
			if(seconds)
			{
				char tempDay[30] = {0,};
				strcat(temp, " ");
				wsprintf(tempDay, (*g_pGameStringTable)[UI_STRING_MESSAGE_SECOND].GetString() , seconds );
				strcat ( temp, tempDay );				
			}
			g_PrintColorStr(vx,strY, temp,gpC_base->m_item_desc_pi,RGB(255,255,255));
		}				
	}
	g_FL2_ReleaseDC();
	SHOW_WINDOW_ATTR;
}

bool	C_VS_UI_ITEM_LIST::MouseControl(UINT message, int _x, int _y)
{
	Rect rect(x+20,y+20,110,139);
	Rect ItemListRect(rect.x, rect.y + rect.h + 20 + 40, 340,160);

	const int line_num = 8;
	const int line_gap = 20;

	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);
	m_pC_scroll_bar->MouseControl(message, _x, _y);

	switch(message)
	{
	case M_WHEEL_UP :
		m_pC_scroll_bar->ScrollUp();
		EMPTY_MOVE;
		break;

	case M_WHEEL_DOWN :
		m_pC_scroll_bar->ScrollDown();
		EMPTY_MOVE;
		break;
		
	case M_MOVING :
		if( _x+x > ItemListRect.x && _x+x < ItemListRect.x+ItemListRect.w &&
			_y+y > ItemListRect.y && _y+y < ItemListRect.y+ItemListRect.h)
		{
			m_pItem = NULL;
			int select =  ( (_y+y) - ItemListRect.y ) / line_gap;
			if( select >= line_num )
				break;

			m_MouseFocus = select + m_pC_scroll_bar->GetScrollPos();			
			
			if(m_MouseFocus >= 0 && m_MouseFocus < m_ItemList.size() && m_ItemList[m_MouseFocus].pItem != NULL )
			{
				m_CurrentItem = m_MouseFocus;
			}
		} else
		{
			m_MouseFocus = -1;
		}
		break;
	case M_LEFTBUTTON_DOWN :
		if( _x+x > ItemListRect.x && _x+x < ItemListRect.x+ItemListRect.w &&
			_y+y > ItemListRect.y && _y+y < ItemListRect.y+ItemListRect.h)
		{
			int select =  ( (_y+y) - ItemListRect.y ) / line_gap;
			if( select >= line_num )
				break;

		//	m_MouseFocus = select;
			int CurrentItem = select + m_pC_scroll_bar->GetScrollPos();
			if(CurrentItem  < 0 || m_ItemList.size() <= CurrentItem  )
				return false;

			MItem *pItem = m_ItemList[CurrentItem ].pItem;
			if(pItem != NULL)
			{
				m_pItem = pItem;
				g_StartTakeOutConfirmDialog( _x+x, _y+y );
			//	gpC_base->SendMessage(UI_SELECT_ITEM_FROM_SHOP, pItem->GetID());
			}

		} else
		{
			m_MouseFocus = -1;
		}		
		break;	
	}

	
	return true;
}

void	C_VS_UI_ITEM_LIST::KeyboardControl(UINT message, UINT key, long extra)	
{	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_LEFT :
			if( m_CurrentItem > 0 )
				--m_CurrentItem;
			break;
		case VK_RIGHT :
			if( m_CurrentItem + 1 < m_ItemList.size() )
				++m_CurrentItem;
			break;
		case VK_ESCAPE :
			gpC_base->SendMessage(UI_CLOSE_ITEM_LIST);
			break;		
		}
	}	
}

bool	C_VS_UI_ITEM_LIST::IsPixel(int _x,int _y)
{
	return IsInRect(_x, _y);
}

void	C_VS_UI_ITEM_LIST::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetFocusState())
	{
		if(p_button->GetPressState())
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
	} else
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index);
}

void	C_VS_UI_ITEM_LIST::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
	char* m_bring_fee_msg[2] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
	};
	
	switch(p_button->GetID())
	{
	case OK_ID :
		g_descriptor_manager.Set(DID_INFO,p_button->x+x,p_button->y+y,
			(void *)m_bring_fee_msg[0],0,0);		
		break;
	case CANCEL_ID :
		g_descriptor_manager.Set(DID_INFO,p_button->x+x,p_button->y+y,
			(void *)m_bring_fee_msg[1],0,0);		
		break;
	}
}

void	C_VS_UI_ITEM_LIST::WindowEventReceiver(id_t event)
{
}

void	C_VS_UI_ITEM_LIST::Run(id_t id)
{
	switch(id)
	{
	case OK_ID :
		gpC_base->SendMessage(UI_CLOSE_ITEM_LIST);
		break;
	case CANCEL_ID :
		break;		
	}
}

void	C_VS_UI_ITEM_LIST::Process()
{
	m_pC_button_group->Process();	
}


void	C_VS_UI_ITEM_LIST::ClearItem()
{
	while(!m_ItemList.empty())
	{
		ItemList Item = m_ItemList.back();
		if(Item.pItem != NULL)
			delete Item.pItem;
		Item.pItem = NULL;
		m_ItemList.pop_back();
	}

	m_ItemList.clear();
	m_pC_scroll_bar->SetPosMax(0);
}

bool	C_VS_UI_ITEM_LIST::AddItem( ItemList pItem )
{
	if(pItem.pItem == NULL)
		return false;

	m_ItemList.push_back( pItem );

	m_pC_scroll_bar->SetPosMax( m_ItemList.size() - 8 + 1);

	return true;
}


std::string		C_VS_UI_ITEM_LIST::GetItemName( int Focus )
{
	if(Focus < 0 || Focus >= m_ItemList.size() )
		return "";

	MItem* pItem = m_ItemList[ Focus ].pItem;
	if(pItem == NULL)
		return "";

	char sz_name[200]={0,};

	for(int i=0;i<min(2,pItem->GetItemOptionListCount());i++)
	{
		if(pItem->GetItemOptionName(i)!=NULL)
		{
			strcat(sz_name, pItem->GetItemOptionName(i));
			strcat(sz_name, " "); // add 'space'
		}
	}
	strcat(sz_name, pItem->GetName() );

	return sz_name;
}

bool	C_VS_UI_ITEM_LIST::DeleteItem( DWORD ID )
{
	if( m_ItemList.empty() )
		return false;

	MItem *pDeleteItem = NULL;
	std::vector<ItemList>::iterator itr = m_ItemList.begin();
	std::vector<ItemList>::iterator endItr = m_ItemList.end();

	while( itr != endItr )
	{
		MItem* pItem = (*itr).pItem;

		if(pItem != NULL)
		{
			if ( pItem->GetID() == ID )
			{
				pDeleteItem = pItem;
				m_ItemList.erase( itr );
				break;
			}
		}
		itr++;
	}

	m_pC_scroll_bar->SetPosMax( m_ItemList.size() - 8 + 1);
	m_pC_scroll_bar->SetScrollPos( 0 );

	if(pDeleteItem != NULL)
	{
		delete pDeleteItem;
		pDeleteItem = NULL;

		EMPTY_MOVE;
		return true;
		
	}		

//	if(m_CurrentItem >= m_ItemList.size()) m_CurrentItem = m_ItemList.size()-2;
	
	return false;
}

//------------------------------------------------------------------------------
// C_VS_UI_IMAGE_NOTICE
//
// �˾�â�� ����.
//------------------------------------------------------------------------------

C_VS_UI_IMAGE_NOTICE::C_VS_UI_IMAGE_NOTICE()
{	
	m_pC_image_spk = NULL;
	m_pC_image_spk = new C_SPRITE_PACK("data\\ui\\spk\\lotterynotice.spk");
	
	Set(0, 200, m_pC_image_spk->GetWidth(0), m_pC_image_spk->GetHeight(0));
	
	g_RegisterWindow(this);
	AttrTopmost(false);
	m_CloseTime = 10000;

	m_NoticeList.clear();
}

void	C_VS_UI_IMAGE_NOTICE::AddNotice(const char* name, DWORD id)
{
	SNotice *notice = new SNotice;

	notice->m_Name = name;
	notice->m_Time = timeGetTime();
	notice->m_GiftName = LoadInfo( id );
	notice->m_Alpha	=	32;

	m_NoticeList.push_back( notice );
}

std::string C_VS_UI_IMAGE_NOTICE::LoadInfo(DWORD ID)
{
	CEventGiftInfo *Event = new CEventGiftInfo;
	std::string convertedPath = ConvertGamePath("data\\info\\eventquest.inf");
	ifstream info(convertedPath.c_str(), ios::binary );
	Event->LoadFromFile ( info );
	info.close();
	
	for(int i=0;i<Event->GetSize();i++)
	{
		if ( (*Event)[i].m_ID == ID )
		{
			static std::string name;
			name = ((*Event)[i].m_Name.GetString());
			delete Event;
			return name;			
		}
	}
	delete Event;
	return "";
}


C_VS_UI_IMAGE_NOTICE::~C_VS_UI_IMAGE_NOTICE()
{
	ClearNotice();
	DeleteNew( m_pC_image_spk );
}

void	C_VS_UI_IMAGE_NOTICE::Start()
{
	PI_Processor::Start();
	gpC_window_manager->AppearWindow(this);
	AttrTopmost(false);

//	m_Time = timeGetTime();
}

void	C_VS_UI_IMAGE_NOTICE::Finish()
{
	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);

}

void	C_VS_UI_IMAGE_NOTICE::Show()
{
	int strX = x;
	int strY = y;	

	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		for(int i=0;i<m_NoticeList.size();i++)
		{
			SNotice *pNotice = m_NoticeList[i];

			if( pNotice == NULL )
				continue;

			if(pNotice->m_Alpha >= 32)
			{
				m_pC_image_spk->BltLocked(strX,strY,0);
				strY+=m_pC_image_spk->GetHeight(0);
			}
			else
			{
				m_pC_image_spk->BltLockedAlpha(strX,strY,0, min(31,max(0,pNotice->m_Alpha) ) );
				strY+= (m_pC_image_spk->GetHeight(0) * pNotice->m_Alpha) / 32;
			}			
			
			if(strY >= 600 )
				break;
		}
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	strX = x;
	strY = y;

	if(g_FL2_GetDC())
	{
		for(int i=0;i<m_NoticeList.size();i++)
		{
			SNotice *pNotice = m_NoticeList[i];

			if( pNotice == NULL )
				continue;

			if(pNotice->m_Alpha >= 32)
			{
				//wsprintf(temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_USER_1].GetString(),pNotice->m_Name.c_str() );
				int vx = g_PrintColorStr(strX+66, strY+10, pNotice->m_Name.c_str(), gpC_base->m_chatting_pi,RGB_YELLOW);
				g_PrintColorStr(vx, strY+10,	(*g_pGameStringTable)[UI_STRING_MESSAGE_USER_1].GetString(), gpC_base->m_chatting_pi, RGB_WHITE);
				g_PrintColorStr(strX+66, strY+30,	pNotice->m_GiftName.c_str(), gpC_base->m_chatting_pi, RGB(180,195,255));
				g_PrintColorStr(strX+66, strY+50,	(*g_pGameStringTable)[UI_STRING_MESSAGE_WIN_A_PRIZE2].GetString(), gpC_base->m_chatting_pi, RGB_WHITE);
				strY+=m_pC_image_spk->GetHeight(0);
			}
			else
			{
				strY+= (m_pC_image_spk->GetHeight(0) * pNotice->m_Alpha) / 32;
			}			
			
			if(strY >= 600 )
				break;
		}
		g_FL2_ReleaseDC();
	}
	SHOW_WINDOW_ATTR;
}

bool	C_VS_UI_IMAGE_NOTICE::MouseControl(UINT message, int _x, int _y)
{
	return true;
}

void	C_VS_UI_IMAGE_NOTICE::KeyboardControl(UINT message, UINT key, long extra)	
{	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_ESCAPE :
			//gpC_base->SendMessage(UI_CLOSE_POPUP_MESSAGE);
			DeleteNotice(0);
			break;		
		}
	}
}

bool	C_VS_UI_IMAGE_NOTICE::IsPixel(int _x,int _y)
{
//	return IsInRect(_x, _y);
	return false;
}

void	C_VS_UI_IMAGE_NOTICE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
}

void	C_VS_UI_IMAGE_NOTICE::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
}

void	C_VS_UI_IMAGE_NOTICE::WindowEventReceiver(id_t event)
{
}

void	C_VS_UI_IMAGE_NOTICE::Run(id_t id)
{
}

void	C_VS_UI_IMAGE_NOTICE::Process()
{
	if(!m_NoticeList.empty())
	{
		SNotice *pNotice = m_NoticeList[0];
		if ( timeGetTime() - pNotice->m_Time > m_CloseTime )
		{
			pNotice->m_Alpha--;			
		}
	}

	std::vector<SNotice*>::iterator itr = m_NoticeList.begin();
	std::vector<SNotice*>::iterator endItr = m_NoticeList.end();
	while( itr != endItr )
	{
		SNotice *pNotice = *itr;
		if(pNotice->m_Alpha <= 0)
		{
			m_NoticeList.erase( itr );
			if( pNotice != NULL )
			{
				delete pNotice;
				pNotice = NULL;
			}
			break;
		}		
		itr++;
	}
	
	if(m_NoticeList.empty() )
		gpC_base->SendMessage(UI_CLOSE_IMAGE_NOTICE);
}

void	C_VS_UI_IMAGE_NOTICE::ClearNotice()
{
	std::vector<SNotice*>::iterator itr = m_NoticeList.begin();
	std::vector<SNotice*>::iterator endItr = m_NoticeList.end();
	
	while( itr != endItr )
	{
		SNotice *pNotice = *itr;
		itr++;
		
		delete pNotice;
		pNotice = NULL;
	}

	m_NoticeList.clear();
}

void	C_VS_UI_IMAGE_NOTICE::DeleteNotice(int n)
{
	if(n < 0 || n>= m_NoticeList.size() )
		return;

	std::vector<SNotice*>::iterator itr = m_NoticeList.begin();
	std::vector<SNotice*>::iterator	endItr = m_NoticeList.end();

	itr += n;
	SNotice *pNotice = *itr;	

	m_NoticeList.erase( itr );
	delete pNotice;
	pNotice = NULL;
}


//-----------------------------------------------------------------------------
// C_VS_UI_BULLETIN_BOARD::C_VS_UI_BULLETIN_BOARD
//
//
//-----------------------------------------------------------------------------
C_VS_UI_BULLETIN_BOARD::C_VS_UI_BULLETIN_BOARD(const MItem *pItem)
{
	if(pItem == NULL)
		return;

	m_pItem = (MItem *)pItem;
	
	status = false;
	m_pC_back_spk = NULL;
	m_pC_back_spk = new C_SPRITE_PACK(SPK_BULLETIN_BOARD);
	
	Set(g_GameRect.right/2 - m_pC_back_spk->GetWidth(MAIN_WINDOW)/2, g_GameRect.bottom/2 - m_pC_back_spk->GetHeight(MAIN_WINDOW)/2, m_pC_back_spk->GetWidth(MAIN_WINDOW), m_pC_back_spk->GetHeight(MAIN_WINDOW));
	
	g_RegisterWindow(this);

	AttrTopmost(true);
	AttrKeyboardControl(true);
	
	//�����ư
	m_pC_button_group = new ButtonGroup(this);
	
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(169, 116, m_pC_back_spk->GetWidth(BUTTON_OK), 
		m_pC_back_spk->GetHeight(BUTTON_OK), OK_ID, this, BUTTON_OK));
	
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(181, 116, m_pC_back_spk->GetWidth(BUTTON_CANCEL),
		m_pC_back_spk->GetHeight(BUTTON_CANCEL), CANCEL_ID, this, BUTTON_CANCEL));
	
	m_pos_from_x = 111;
	m_pos_from_y = 88;
	m_pos_msg_x = 47;
	m_pos_msg_y = 28;

	m_lev_message.SetEditorMode(20, 3);
	m_lev_message.SetPosition(x+m_pos_msg_x,y+m_pos_msg_y);
	m_lev_message.SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_message.SetByteLimit(60);
	m_lev_message.SetAbsWidth(115);
	m_lev_message.SetInputStringColor(RGB(0, 0, 150));
	m_lev_message.SetCursorColor(RGB_BLACK);
	Attach(&m_lev_message);	

	m_lev_from.SetByteLimit(14);
	m_lev_from.SetPosition(x+m_pos_from_x,y+m_pos_from_y);
	m_lev_from.SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_from.SetCursorColor(RGB_BLACK);
	m_lev_from.SetInputStringColor(RGB(0, 150, 0));
	m_lev_from.AddString( g_char_slot_ingame.sz_name.c_str() );
	Attach(&m_lev_from);	
	
}

//-----------------------------------------------------------------------------
// C_VS_UI_BULLETIN_BOARD::~C_VS_UI_BULLETIN_BOARD
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_BULLETIN_BOARD::~C_VS_UI_BULLETIN_BOARD()
{
	g_UnregisterWindow(this);
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::TRACE_WINDOW, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::TRACE_WINDOW, Rect(x, y, w, h));	
	
	m_lev_message.Unacquire();
	m_lev_from.Unacquire();
	gC_vs_ui.AcquireChatting();

	DeleteNew(m_pC_back_spk);
	DeleteNew(m_pC_button_group);
}

//-----------------------------------------------------------------------------
// C_VS_UI_BULLETIN_BOARD::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_BULLETIN_BOARD::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char* m_trace_window_button_string[2] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_BULLETIN_BOARD_OK].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_BULLETIN_BOARD_CANCEL].GetString(),
	};

	switch(p_button->GetID())
	{
	case CANCEL_ID :
		g_descriptor_manager.Set(DID_INFO,p_button->x+x,p_button->y+y,(void *)m_trace_window_button_string[1],0,0);
		break;
	case OK_ID :
		g_descriptor_manager.Set(DID_INFO,p_button->x+x,p_button->y+y,(void *)m_trace_window_button_string[0],0,0);
		break;		
	}
	
}


//-----------------------------------------------------------------------------
// C_VS_UI_BULLETIN_BOARD_WINDOW::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_BULLETIN_BOARD::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetFocusState())
	{
		if(p_button->GetPressState())
			m_pC_back_spk->Blt(x+p_button->x,y+p_button->y,p_button->m_image_index+2);
		else
			m_pC_back_spk->Blt(x+p_button->x,y+p_button->y,p_button->m_image_index+1);
	} else
		m_pC_back_spk->Blt(x+p_button->x,y+p_button->y,p_button->m_image_index);
	
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_BULLETIN_BOARD::WindowEventReceiver(id_t event)
{
}


//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_BULLETIN_BOARD::Run(id_t id)
{
	// ��ư�� ��Ȱ�������̸� ����!
	if(status == true) return;

	switch (id)
	{
	case OK_ID:		
		{
			char *psz_msg, *psz_from = NULL;

			g_Convert_DBCS_Ascii2SingleByte(m_lev_message.GetStringWide(), m_lev_message.Size(), psz_msg);
			//g_Convert_DBCS_Ascii2SingleByte(m_lev_from.GetStringWide(), m_lev_from.Size(), psz_from);
			
			psz_from = (char*)g_char_slot_ingame.sz_name.c_str();

			if(psz_msg != NULL && psz_from != NULL)
			{				
				m_szTreeMessage = "";
				
				char *find;
				do{
					find = strrchr(psz_msg, '\n');
					if(find != NULL && *(find+1) == '\0')
						*find = '\0';
				}while(find != NULL && *find == '\0');
				
				if ( m_pItem->GetItemClass() == ITEM_CLASS_EFFECT_ITEM )
				{
					m_szTreeMessage += g_char_slot_ingame.sz_name.c_str();
					m_szTreeMessage += ">";
	//				m_szTreeMessage += psz_from;
					m_szTreeMessage += psz_msg;
				}
				else
				{
					m_szTreeMessage += psz_msg;

					m_szTreeMessage += "%";
	//				m_szTreeMessage += psz_from;
					m_szTreeMessage += g_char_slot_ingame.sz_name.c_str();
				}


				gpC_base->SendMessage(UI_USE_XMAS_TREE, (int)(intptr_t)m_pItem, 1, (void*)m_szTreeMessage.c_str());

				DeleteNew(psz_msg);
//				DeleteNew(psz_from);
			}
			else
			{
				// ���׸��� �ֽ��ϴ�.
				gpC_base->SendMessage(UI_USE_XMAS_TREE, (int)(intptr_t)m_pItem, 1, NULL);
			}
		}
		break;
		
	case CANCEL_ID:
		gpC_base->SendMessage(UI_CLOSE_BULLETIN_BOARD_WINDOW);
		EMPTY_MOVE;
		break;

	}
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_BULLETIN_BOARD::MouseControl(UINT message, int _x, int _y)
{
//	if(!m_lev_message.IsAcquire())
//		m_lev_message.Acquire();

	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;	
	bool re = m_pC_button_group->MouseControl(message, _x, _y);
	
	switch(message)
	{
	case M_MOVING:
		break;
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if(_x >= 45 && _x <= 160 && _y >= 26 && _y <= 81)
		{
			m_lev_from.Unacquire();
			m_lev_message.Acquire();
		}
		else if(_x >= 108 && _x <= 160 && _y >= 85 && _y <= 101)
		{
//			m_lev_message.Unacquire();
//			m_lev_from.Acquire();
		}
		else if (gpC_mouse_pointer->GetPickUpItem()==FALSE && re)
		{
			MoveReady();
			SetOrigin(_x, _y);
		} 
		break;
	}
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_BULLETIN_BOARD
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_BULLETIN_BOARD::KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_CHAR)
	{		
		char ignore_char[] = {'%', '\\', '\"', '\''};
	
		for (int i=0; i<sizeof(ignore_char); i++)
			if ((char)key == ignore_char[i])
				return;
	}

	if(!(message == WM_KEYDOWN && key == VK_RETURN ))
		Window::KeyboardControl(message, key, extra);

	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_ESCAPE:
			Run(CANCEL_ID);
			break;

		case VK_RETURN :
			if(m_lev_message.IsAcquire() && m_lev_message.GetLineCount() > 2)
			{
//				m_lev_message.Unacquire();
//				m_lev_from.Acquire();
			}
			break;

		case VK_BACK:
			if(m_lev_from.IsAcquire() && m_lev_from.Size() == 0)
			{
				m_lev_from.Unacquire();
				m_lev_message.Acquire();
			}
			break;

		case VK_UP:
			if(m_lev_from.IsAcquire())
			{
				m_lev_from.Unacquire();
				m_lev_message.Acquire();
				m_lev_message.EndCursor();
			}
			break;

		case VK_DOWN:
		case VK_TAB:
			if(m_lev_message.IsAcquire())
			{
//				m_lev_message.Unacquire();
//				m_lev_from.Acquire();
			}
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// Show
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_BULLETIN_BOARD::Show()
{
	m_pC_back_spk->Blt(x,y,MAIN_WINDOW);


	m_lev_message.SetPosition(x+m_pos_msg_x,y+m_pos_msg_y);
	m_lev_message.Show();
	m_lev_from.SetPosition(x+m_pos_from_x,y+m_pos_from_y);
	m_lev_from.Show();

	m_pC_button_group->Show();

	m_pC_button_group->ShowDescription();
	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_BULLETIN_BOARD::Start()
{
	PI_Processor::Start();

	m_pC_button_group->Init();
	AttrTopmost(true);
	gpC_window_manager->AppearWindow(this);	
	
	m_lev_message.EraseAll();
	m_lev_from.EraseAll();

	m_lev_from.AddString( g_char_slot_ingame.sz_name.c_str() );

	m_lev_message.Acquire();
}

void	C_VS_UI_BULLETIN_BOARD::Finish()
{
	PI_Processor::Finish();	
	gpC_window_manager->DisappearWindow(this);
 	m_lev_message.Unacquire();
 	m_lev_from.Unacquire();
	gC_vs_ui.AcquireChatting();
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_BULLETIN_BOARD::Process()
{
	m_pC_button_group->Process();	
	if(m_lev_from.IsAcquire() )
		m_lev_from.Unacquire();
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_BULLETIN_BOARD::IsPixel(int _x, int _y)
{
	//m_pC_monitor_spk->IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y), MONITOR_WINDOW);
	return m_pC_back_spk->IsPixel(SCR2WIN_X(_x),SCR2WIN_Y(_y),MAIN_WINDOW)
		|| m_pC_back_spk->IsPixel(SCR2WIN_X(_x)-130, SCR2WIN_Y(_y)-146, BUTTON_OK)
		|| m_pC_back_spk->IsPixel(SCR2WIN_X(_x)-165, SCR2WIN_Y(_y)-146, BUTTON_CANCEL);	
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_RESURRECT::C_VS_UI_REQUEST_RESURRECT
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_REQUEST_RESURRECT::C_VS_UI_REQUEST_RESURRECT(bool resurrect, bool elixir, bool eternity, bool IsSiegeAttacker)
{
	m_ResurrectButton[RESURRECT_MODE_RESURRECT_SCROLL].m_Enable = resurrect;
	m_ResurrectButton[RESURRECT_MODE_ELIXIR].m_Enable = elixir;
	m_ResurrectButton[RESURRECT_MODE_ETERNITY].m_Enable = eternity;
	m_ResurrectButton[RESURRECT_MODE_SIEGE_ATTACKER].m_Enable = IsSiegeAttacker;
	
	g_RegisterWindow(this);
	
	AttrPin(true);
	
	int _y;
	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		m_image_spk.Open(SPK_LEVELUP);
		_y = 382;
		break;

	case RACE_VAMPIRE:
		m_image_spk.Open(SPK_LEVELUP);
		_y = 425;
		break;

	case RACE_OUSTERS:
		m_image_spk.Open(SPK_LEVELUP);
		_y = 382;
		break;
	}

	Set(2, _y, m_image_spk.GetWidth(RESURRECT_GUARD_SLAYER), m_image_spk.GetHeight(RESURRECT_GUARD_SLAYER));

	
	int posx=2 + 15 + m_image_spk.GetWidth( RESURRECT_GUARD_SLAYER );
	for( int i=0;i<RESURRECT_MODE_MAX;i++ )
	{
		m_ResurrectButton[i].SetRect (
			posx,
			_y,
			m_image_spk.GetWidth( RESURRECT_GUARD_SLAYER ),
			m_image_spk.GetHeight( RESURRECT_GUARD_SLAYER ) );
		
		posx += m_image_spk.GetWidth(RESURRECT_GUARD_SLAYER) + 15;

		if( m_ResurrectButton[i].m_Enable )
		{
			if(i == RESURRECT_MODE_SIEGE_ATTACKER)
			{
				switch(g_eRaceInterface)
				{
				case RACE_SLAYER:
					m_ResurrectButton[i].m_Image = RESURRECT_SLAYER;
					break;

				case RACE_VAMPIRE:
					m_ResurrectButton[i].m_Image = RESURRECT_VAMPIRE;
					break;

				case RACE_OUSTERS:
					m_ResurrectButton[i].m_Image = RESURRECT_OUSTERS;
					break;
				}
			}
			else
				m_ResurrectButton[i].m_Image = RESURRECT_SCROLL+i;
		}
		else
			m_ResurrectButton[i].m_Image = -1;
	}
	
	m_focus = -1;
	m_pushed = -1;
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_RESURRECT::~C_VS_UI_REQUEST_RESURRECT
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_REQUEST_RESURRECT::~C_VS_UI_REQUEST_RESURRECT()
{
	g_UnregisterWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_RESURRECT::Show
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_REQUEST_RESURRECT::Show()
{
	int i; // Declare i at function scope

	for(i = 0; i<RESURRECT_MODE_MAX ; i++ )
	{
		if( m_ResurrectButton[i].m_Enable == true && m_ResurrectButton[i].m_Image != -1 )
		{
			RECT rt = { m_ResurrectButton[i].m_ButtonRect.left + 9, m_ResurrectButton[i].m_ButtonRect.top + 9,
				m_ResurrectButton[i].m_ButtonRect.left + 9 + 36, m_ResurrectButton[i].m_ButtonRect.top + 9 + 36 };

			gpC_base->m_p_DDSurface_back->FillRect( &rt, 0 );
		}
	}

	if(gpC_base->m_p_DDSurface_back->Lock() )
	{
		for(i=0;i<RESURRECT_MODE_MAX; i++ )
		{
			if( m_ResurrectButton[i].m_Enable == true && m_ResurrectButton[i].m_Image != -1 )
			{
				if(m_focus == i && m_pushed == i )
					m_image_spk.BltLocked( m_ResurrectButton[i].m_ButtonRect.left + 10, m_ResurrectButton[i].m_ButtonRect.top + 10, m_ResurrectButton[i].m_Image );
				else
					m_image_spk.BltLocked( m_ResurrectButton[i].m_ButtonRect.left + 9, m_ResurrectButton[i].m_ButtonRect.top + 9, m_ResurrectButton[i].m_Image );
				
				if( m_ResurrectButton[i].m_Delay > 0 )
				{
					DWORD endTime = m_ResurrectButton[i].m_Delay + m_ResurrectButton[i].m_Time;
					
					if( endTime > timeGetTime() )
					{
						int percent = min(100, 
							(endTime - timeGetTime())*100/m_ResurrectButton[i].m_Delay );

						if(percent)
						{
							Rect rect;
							RECT rt;
							POINT point = { m_ResurrectButton[i].m_ButtonRect.left + 9, m_ResurrectButton[i].m_ButtonRect.top + 9 };

							S_SURFACEINFO surface_info;
							CSprite* pSprite = &m_image_spk.GetSprite(m_ResurrectButton[i].m_Image);
							SetSurfaceInfo(&surface_info, gpC_base->m_p_DDSurface_back->GetDDSD());
					
							SetRect((RECT *)&rect, 0, 0, pSprite->GetWidth(), pSprite->GetHeight()*(percent)/100);
							
							rt.left = 0;
							rt.top = 0;
							rt.right = rect.x+rect.w;
							rt.bottom = rect.y+rect.h;
							
							if(rt.left < rt.right && rt.top < rt.bottom)
							{
								if( point.y < 0 )
								{
									rt.top = -point.y;
									point.y=0;
								}								
								WORD * p_dest = (WORD *)surface_info.p_surface+point.x+rt.left;
								p_dest = (WORD *)((BYTE *)p_dest+(point.y+rt.top)*surface_info.pitch);
								
								if( rt.top < rt.bottom )
									pSprite->BltColorClipHeight(p_dest, surface_info.pitch, &rt, rgb_RED);
							}
						}
					}
				}

				switch(g_eRaceInterface)
				{
				case RACE_SLAYER:
					m_image_spk.BltLocked( m_ResurrectButton[i].m_ButtonRect.left, m_ResurrectButton[i].m_ButtonRect.top, RESURRECT_GUARD_SLAYER);
					break;

				case RACE_VAMPIRE:
					m_image_spk.BltLocked( m_ResurrectButton[i].m_ButtonRect.left, m_ResurrectButton[i].m_ButtonRect.top, RESURRECT_GUARD_VAMPIRE);
					break;

				case RACE_OUSTERS:
					m_image_spk.BltLocked( m_ResurrectButton[i].m_ButtonRect.left, m_ResurrectButton[i].m_ButtonRect.top, RESURRECT_GUARD_OUSTERS);
					break;
				}
			}
		}
		
		gpC_base->m_p_DDSurface_back->Unlock();
	}
}


void	C_VS_UI_REQUEST_RESURRECT::SetDelay( RESURRECT_MODE mode, int delay )
{
	m_ResurrectButton[ mode ].m_Delay = delay;
	m_ResurrectButton[ mode ].m_Time = timeGetTime();
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_RESURRECT::IsPixel
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_REQUEST_RESURRECT::IsPixel(int _x, int _y)
{
	bool flag = false;

	for(int i = 0 ; i < RESURRECT_MODE_MAX; i++ )
	{
		if(m_ResurrectButton[i].m_Enable == true )
		{
			if( m_ResurrectButton[i].m_ButtonRect.left < _x && m_ResurrectButton[i].m_ButtonRect.right > _x &&
				m_ResurrectButton[i].m_ButtonRect.top < _y && m_ResurrectButton[i].m_ButtonRect.bottom > _y )
				flag = true;			
		}

		if( flag )
			break;
	}
	return flag;
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_RESURRECT::Start
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_REQUEST_RESURRECT::Start()
{
	PI_Processor::Start();
	
	gpC_window_manager->AppearWindow(this);
	
	m_focus = -1;
	m_pushed = -1;

	EMPTY_MOVE;
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_RESURRECT::Finish
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_REQUEST_RESURRECT::Finish()
{
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
}

void C_VS_UI_REQUEST_RESURRECT::Process()
{
	for(int i = 0; i < RESURRECT_MODE_MAX; i++ )
	{
		if( m_ResurrectButton[i].m_Enable )
		{
			DWORD endTime = m_ResurrectButton[i].m_Delay + m_ResurrectButton[i].m_Time;
			
			if( endTime <= timeGetTime() )
				m_ResurrectButton[i].m_Delay = 0;		
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_REQUEST_RESURRECT::MouseControl
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_REQUEST_RESURRECT::MouseControl(UINT message, int _x, int _y)
{
	const char * szMsgList[RESURRECT_MODE_MAX] =  {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_RESURRECT_BY_SCROLL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_RESURRECT_BY_ELIXIR].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_USE_ETERNITY_FOR_RESURRECT].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_USE_SIEGE_FOR_RESURRECT].GetString()
	};
	
	Window::MouseControl(message, _x, _y);
	int i;
	
	switch (message)
	{
	case M_MOVING:
		m_focus = -1;
		for(i = 0 ;i < RESURRECT_MODE_MAX; i ++ )
		{
			if( m_ResurrectButton[i].m_Enable == true )
			{
				if( _x > m_ResurrectButton[i].m_ButtonRect.left && _x < m_ResurrectButton[i].m_ButtonRect.right &&
					_y > m_ResurrectButton[i].m_ButtonRect.top && _y < m_ResurrectButton[i].m_ButtonRect.bottom )
				{
					m_focus = i;
					g_descriptor_manager.Set(DID_INFO, m_ResurrectButton[i].m_ButtonRect.left, m_ResurrectButton[i].m_ButtonRect.top, (void*)szMsgList[i],0,0);
					break;
				}
			}
		}				
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if (m_focus != -1 && m_ResurrectButton[ m_focus ].m_Delay <= 0)
			m_pushed = m_focus;
		break;
		
	case M_LEFTBUTTON_UP:
		if (m_pushed != -1 && m_focus != -1)
		{
			if(m_pushed == m_focus &&
				m_ResurrectButton[ m_focus ].m_Delay <= 0)
				gpC_base->SendMessage(UI_FINISH_REQUEST_RESURRECT_BUTTON, m_pushed);
		}		
		m_pushed = -1;

		break;
	}
	
	return true;
}


//-----------------------------------------------------------------------------
// C_VS_UI_MIXING_FORGE::C_VS_UI_MIXING_FORGE
//
//-----------------------------------------------------------------------------
C_VS_UI_MIXING_FORGE::C_VS_UI_MIXING_FORGE( FORGE_CLASS forge_class, FORGE_TYPE forge_type )
{	
	m_forge_class = forge_class;
	m_forge_type  = forge_type;

	m_pC_back_spk = NULL;
	m_pC_back_spk = new C_SPRITE_PACK(SPK_MIXING_FORGE);	

	Set( 100, 150, m_pC_back_spk->GetWidth(MAIN_WINDOW), m_pC_back_spk->GetHeight(MAIN_WINDOW));

	g_RegisterWindow(this);

	int w_x, w_y;	


	switch( g_eRaceInterface )
	{
	case RACE_VAMPIRE :
		m_pC_inventory_spk = new C_SPRITE_PACK(SPK_VAMPIRE_INVENTORY);
		w_x = 350;
		w_y = 150;		
		m_my_grid_rect.Set(17+w_x, 150+19, C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*C_VS_UI_INVENTORY::GRID_X, C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*C_VS_UI_INVENTORY::GRID_Y);
		break;
	case RACE_SLAYER :
		m_pC_inventory_spk = new C_SPRITE_PACK(SPK_SLAYER_INVENTORY);
		w_x = 350;
		w_y = 150;		
		m_my_grid_rect.Set(13+w_x, 150+26, C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*C_VS_UI_INVENTORY::GRID_X, C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*C_VS_UI_INVENTORY::GRID_Y);		
		break;
	case RACE_OUSTERS :
		m_pC_inventory_spk = new C_SPRITE_PACK(SPK_OUSTERS_INVENTORY);
		w_x = 350;
		w_y = 150;		
		m_my_grid_rect.Set(25+w_x, 150+35, C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*C_VS_UI_INVENTORY::GRID_X, C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*C_VS_UI_INVENTORY::GRID_Y);
		break;
	}

	AttrTopmost(true);
	//AttrKeyboardControl(true);
	
	//�����ư
	m_pC_button_group = new ButtonGroup(this);
	
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(26, 123, m_pC_back_spk->GetWidth(BUTTON_OK), 
		m_pC_back_spk->GetHeight(BUTTON_OK), OK_ID, this, BUTTON_OK));
	
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(113, 123, m_pC_back_spk->GetWidth(BUTTON_CANCEL),
		m_pC_back_spk->GetHeight(BUTTON_CANCEL), CANCEL_ID, this, BUTTON_CANCEL));

	m_p_select_item[0] = NULL;
	m_p_select_item[1] = NULL;
	
}

//-----------------------------------------------------------------------------
// C_VS_UI_MIXING_FORGE::~C_VS_UI_MIXING_FORGE
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_MIXING_FORGE::~C_VS_UI_MIXING_FORGE()
{
	g_UnregisterWindow( this );

	DeleteNew(m_pC_back_spk);
	DeleteNew(m_pC_button_group);
	DeleteNew(m_pC_inventory_spk);		
}

//-----------------------------------------------------------------------------
// C_VS_UI_MIXING_FORGE::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_MIXING_FORGE::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	static char* m_trace_window_button_string[2] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_MIXING_FORGE_OK].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_MIXING_FORGE_CANCEL].GetString(),
	};
	
	switch(p_button->GetID())
	{
	case CANCEL_ID :
		g_descriptor_manager.Set(DID_INFO,p_button->x+x,p_button->y+y,(void *)m_trace_window_button_string[1],0,0);
		break;
	case OK_ID :
		if( m_p_select_item[0] != NULL && m_p_select_item[1] != NULL )
		{
			g_descriptor_manager.Set(DID_INFO,p_button->x+x,p_button->y+y,(void *)m_trace_window_button_string[0],0,0);
		}
		break;		
	}		
}


//-----------------------------------------------------------------------------
// C_VS_UI_MIXING_FORGE_WINDOW::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_MIXING_FORGE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->m_image_index == BUTTON_OK )
	{
		if( m_p_select_item[0] != NULL && m_p_select_item[1] != NULL )
		{
			if(p_button->GetFocusState())
			{
				if(p_button->GetPressState())
					m_pC_back_spk->Blt(x+p_button->x,y+p_button->y,p_button->m_image_index+2);
				else
					m_pC_back_spk->Blt(x+p_button->x,y+p_button->y,p_button->m_image_index+1);
			} else
				m_pC_back_spk->Blt(x+p_button->x,y+p_button->y,p_button->m_image_index);
		}		
	}
	else
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				m_pC_back_spk->Blt(x+p_button->x,y+p_button->y,p_button->m_image_index+2);
			else
				m_pC_back_spk->Blt(x+p_button->x,y+p_button->y,p_button->m_image_index+1);
		} else
			m_pC_back_spk->Blt(x+p_button->x,y+p_button->y,p_button->m_image_index);
	}
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_MIXING_FORGE::WindowEventReceiver(id_t event)
{
}


//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_MIXING_FORGE::Run(id_t id)
{
	switch (id)
	{
	case OK_ID:
		{
			if( m_p_select_item[0] != NULL && m_p_select_item[1] != NULL )
				gpC_base->SendMessage(UI_USE_MIXING_FORGE, (int)m_p_select_item[0]->GetID(), (int)m_p_select_item[1]->GetID(), NULL);
		}		
		break;
		
	case CANCEL_ID:
		gpC_base->SendMessage(UI_CLOSE_MIXING_FORGE);
		//EMPTY_MOVE;
		break;
	}
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_MIXING_FORGE::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;	
	bool re = m_pC_button_group->MouseControl(message, _x, _y);
	
	const MItem * p_selected_item;

	switch(message)
	{
	case M_MOVING:
		if(m_p_select_item[0] != NULL && _x > 30 && _x < m_p_select_item[0]->GetGridWidth()*30+30 && _y > 27 && _y < m_p_select_item[0]->GetGridHeight()*30+27 )
		{
			if(m_p_select_item[0] != NULL)
				g_descriptor_manager.Set(DID_ITEM, x+_x, y+_y, (void *)m_p_select_item[0]);
				
		} else
		if( m_p_select_item[1] != NULL && _x > 107 && _x < m_p_select_item[1]->GetGridWidth() * 30 + 107&& _y > 27 && _y < m_p_select_item[1]->GetGridHeight()*30+27)
		{
			if(m_p_select_item[1] != NULL)
				g_descriptor_manager.Set(DID_ITEM, x+_x, y+_y, (void *)m_p_select_item[1]);
		}

		int i;
		int loop;
		int px, py;		

		if (gpC_mouse_pointer->GetPickUpItem())
			loop = ITEM_REF_POINT_COUNT;
		else
		{
			loop = 1;
		}

		for (i=0; i < loop; i++)
		{
			if (loop == ITEM_REF_POINT_COUNT)
			{
				px = g_item_ref_point[i].x+gpC_mouse_pointer->GetPointerX();
				py = g_item_ref_point[i].y+gpC_mouse_pointer->GetPointerY();
			}
			else
			{
				px = _x+x;
				py = _y+y;
			}

			// search grid
			int distance_x = px - m_my_grid_rect.x;
			int distance_y = py - m_my_grid_rect.y;

			if (distance_x >= 0 && distance_x < m_my_grid_rect.w && 
				 distance_y >= 0 && distance_y < m_my_grid_rect.h)
			{
				if(gpC_Imm && m_focus_grid_x != distance_x/C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X || m_focus_grid_y != distance_y/C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)
					gpC_Imm->ForceUI(CImm::FORCE_UI_GRID);
				m_focus_grid_x = distance_x/C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X;
				m_focus_grid_y = distance_y/C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y;

				if (loop == ITEM_REF_POINT_COUNT)
				{
					// item�� grid ������ ������� ������ ������ ��ġ��Ų��.
					const MItem * p_pickup_item = gpC_mouse_pointer->GetPickUpItem();
					int a, b;
					switch (i)
					{
						case 0: // left up - first!
							a = m_focus_grid_x+p_pickup_item->GetGridWidth()-C_VS_UI_INVENTORY::GRID_X;
							b = m_focus_grid_y+p_pickup_item->GetGridHeight()-C_VS_UI_INVENTORY::GRID_Y;
							if (a > 0)
								m_focus_grid_x -= a;
							if (b > 0)
								m_focus_grid_y -= b;
							break;

						case 1: // right up
							m_focus_grid_x = 0;
							b = m_focus_grid_y+p_pickup_item->GetGridHeight()-C_VS_UI_INVENTORY::GRID_Y;
							if (b > 0)
								m_focus_grid_y -= b;
							break;

						case 2: // left down
							m_focus_grid_y = 0;
							a = m_focus_grid_x+p_pickup_item->GetGridWidth()-C_VS_UI_INVENTORY::GRID_X;
							if (a > 0)
								m_focus_grid_x -= a;
							break;

						case 3: // right down
							m_focus_grid_y = 0;
							if (m_focus_grid_x+1 <= p_pickup_item->GetGridHeight())
								m_focus_grid_x = 0;
					}
				}

				p_selected_item = g_pInventory->GetItem(m_focus_grid_x, m_focus_grid_y);				
				if (p_selected_item != NULL)
						g_descriptor_manager.Set(DID_ITEM, GetFocusedItemGridX(p_selected_item), GetFocusedItemGridY(p_selected_item), (void *)p_selected_item);

				return true;
			}			
		}
		if(gpC_Imm && m_focus_grid_x != NOT_SELECTED || m_focus_grid_y != NOT_SELECTED)
			gpC_Imm->ForceUI(CImm::FORCE_UI_GRID);
		m_focus_grid_x = NOT_SELECTED;
		m_focus_grid_y = NOT_SELECTED;
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:		
		break;

	case M_RIGHTBUTTON_DOWN:
		{
			bool ret = Check(m_my_grid_rect.x, m_my_grid_rect.y);

			if (ret)
				EMPTY_MOVE;
		}
		break;
	}
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_MIXING_FORGE
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_MIXING_FORGE::KeyboardControl(UINT message, UINT key, long extra)
{
	if(!(message == WM_KEYDOWN && key == VK_RETURN ))
		Window::KeyboardControl(message, key, extra);

	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_ESCAPE :
			gpC_base->SendMessage(UI_CLOSE_MIXING_FORGE);
			break;		
		}
	}
}

//-----------------------------------------------------------------------------
// Show
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_MIXING_FORGE::Show()
{
	m_pC_back_spk->Blt(x,y,MAIN_WINDOW);
	m_pC_button_group->Show();

	m_pC_button_group->ShowDescription();
	SHOW_WINDOW_ATTR;

	//���� �����͵��� ����ص״ٰ� �Ѳ����� ��´�.
	std::vector<RECT>	vNumRect;
	std::vector<int>	vNum;
	
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		switch (g_eRaceInterface)
		{
		case RACE_OUSTERS :
			m_pC_inventory_spk->BltLocked(350, 150, INVENTORY_WINDOW);
			break;
		case RACE_VAMPIRE :
			m_pC_inventory_spk->BltLocked(350, 150, INVENTORY_WINDOW);			
			m_pC_inventory_spk->BltLocked(355, 150+m_pC_inventory_spk->GetHeight(INVENTORY_WINDOW), INVENTORY_WINDOW_BOTTOM);
			break;
		case RACE_SLAYER :
			m_pC_inventory_spk->BltLocked(350, 150, INVENTORY_WINDOW);
			break;
		}
		
		//----------------------------------------------------------------		
		// show My trade Item		
		//----------------------------------------------------------------
		g_pInventory->SetBegin();

		const MItem * p_selected_item = g_pInventory->GetItem(m_focus_grid_x, m_focus_grid_y);
		int   check_index = 0;

		while (g_pInventory->IsNotEnd())
		{
			const MItem * p_item = g_pInventory->Get();

			// p_item�� NULL�� �ݵ�� �ƴϴ�. �ֳ��ϸ� �����ϴ� �͸� Get()�ϱ� �����̴�.
			assert(p_item);

			// frame id -> sprite id
			TYPE_FRAMEID frame_id = p_item->GetInventoryFrameID();

			int item_x = GetFocusedItemGridX(p_item);
			int item_y = GetFocusedItemGridY(p_item);

			int print_y = item_y + p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y - 12;
			int print_x = item_x + p_item->GetGridWidth()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X-1;

			// Item�� �����ִ� ���� ǥ��
			for (int j = 0; j < p_item->GetGridHeight(); j++)
				for (int i = 0; i < p_item->GetGridWidth(); i++)
				{
					//------------------------------------------------------------
					// ��ȯ�ҷ��� �������� ���
					//------------------------------------------------------------
					if( m_v_check[check_index] )//if (p_item->IsTrade())
					{
//						if(gbl_vampire_interface)
//							m_pC_inventory_spk->BltLocked(item_x+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*i, item_y+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*j, INVENTORY_ITEMBACK);
//						else
						m_pC_inventory_spk->BltLocked(item_x+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*i, item_y+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*j, INVENTORY_ITEMBACK);
					}					
				}

			item_x += (p_item->GetGridWidth()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X)/2-gpC_item->GetWidth(frame_id)/2;
			item_y += (p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)/2-gpC_item->GetHeight(frame_id)/2;
			
			if (p_selected_item && (p_selected_item->GetID() == p_item->GetID()))
			{
				if(p_item->IsQuestItem())
					CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetQuestItemColorset(), 0);
				else
				if(p_item->IsUniqueItem())
					CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetUniqueItemColorset(), 0);
				else				
					CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetItemOptionColorSet(), 0);
				gpC_item->BltLockedOutline(item_x, item_y, WHITE, frame_id);
			}
			else
			{
				if(p_item->IsQuestItem() )
					CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetQuestItemColorset(), 0);
				else					
				if(p_item->IsUniqueItem())
					CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetUniqueItemColorset(), 0);
				else				
					CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetItemOptionColorSet(), 0);

				//------------------------------------------------------------
				// ��ȯ�ҷ��� �������� ���
				//------------------------------------------------------------
				if( m_v_check[check_index] )
				{
					if (p_item->IsAffectStatus())
					{
						// frame id -> sprite id
						gpC_item->BltLocked(item_x, item_y, frame_id);
					}
					else
					{
						gpC_item->BltLockedColorSet(item_x, item_y, frame_id, ITEM_DISABLE_COLOR_SET);
					}
				}
				//------------------------------------------------------------
				// ��ȯ�ҷ��� �������� �ƴ� ���
				//------------------------------------------------------------
				else
				{
					gpC_item->BltLockedDarkness(item_x, item_y, frame_id, 1 );
				}
			}

			//������ ���� ���� alphabox ���ڴ� ���� �ʴ´� ������ �״ٰ� ���߿� ���Ƽ� �Ѳ����� ��´�
			if(p_item->IsPileItem() || p_item->IsChargeItem())
			{
				RECT rt;
				rt.right = print_x;
				int depth, number;
				for(depth = 0, number = p_item->GetNumber(); number > 0; number/=10, depth++);
				if(depth == 0) depth = 1;
				rt.left = print_x - 7*depth;
				rt.top = print_y;
				rt.bottom = rt.top+12;
				
				rt.left = max(0, rt.left);
				rt.top = max(0, rt.top);
				
				if(rt.left < rt.right && rt.top < rt.bottom)DrawAlphaBox(&rt, 0, 0, 0, g_pUserOption->ALPHA_DEPTH);

				vNumRect.push_back(rt);
				vNum.push_back(p_item->GetNumber());
				
			}
			g_pInventory->Next();
			check_index++;
		}		

		for(int i=0; i<2; i++)
		{
			if( m_p_select_item[i] != NULL )
			{
				if(m_p_select_item[i]->IsQuestItem() )
					CIndexSprite::SetUsingColorSet(m_p_select_item[i]->GetQuestItemColorset(), 0);
				else
				if(m_p_select_item[i]->IsUniqueItem())
					CIndexSprite::SetUsingColorSet(m_p_select_item[i]->GetUniqueItemColorset(), 0);
				else				
					CIndexSprite::SetUsingColorSet(m_p_select_item[i]->GetItemOptionColorSet(), 0);

				TYPE_FRAMEID frame_id = m_p_select_item[i]->GetInventoryFrameID();
				gpC_item->BltLocked( 100+29+(i*77) + (m_p_select_item[i]->GetGridWidth()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X)/2-gpC_item->GetWidth(frame_id)/2, 
									 150+25 + (m_p_select_item[i]->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)/2-gpC_item->GetHeight(frame_id)/2, 
									 frame_id);
			}			
		}
		
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	g_FL2_GetDC();
	m_pC_button_group->ShowDescription();

	char sz_num[10];
	COLORREF markColor = RGB(220, 220, 220);//RGB(140, 140, 255);
	for(int i = 0; i < vNum.size(); i++)
	{
		wsprintf(sz_num, "%d", vNum[i]);
		g_PrintColorStr(vNumRect[i].left, vNumRect[i].top, sz_num, gpC_base->m_item_desc_pi, markColor);
	}
	g_FL2_ReleaseDC();

	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_MIXING_FORGE::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();

	AttrPin(true);
	gpC_window_manager->AppearWindow(this);

	// check setting
//	g_pInventory->SetBegin();
//	while (g_pInventory->IsNotEnd())
//	{			
//		m_v_check.push_back(0);		
//		g_pInventory->Next();
//	}	
//
	assert( g_pInventory != NULL );
	g_pInventory->SetBegin();
	while( g_pInventory->IsNotEnd() )
	{
		m_v_check.push_back(0);
		g_pInventory->Next();
	}	
	
	m_focus_grid_x = NOT_SELECTED;
	m_focus_grid_y = NOT_SELECTED;
}

void	C_VS_UI_MIXING_FORGE::Finish()
{
	PI_Processor::Finish();	
	gpC_window_manager->DisappearWindow(this); 		
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_MIXING_FORGE::Process()
{

}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_MIXING_FORGE::IsPixel(int _x, int _y)
{
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_MIXING_FORGE::Check
//
// ��ȯ�ҷ��� item�� üũ�Ѵ�. 
// �̹� üũ�Ǿ��ִٸ�.. ����Ѵ�.
//
// grid_start_x, grid_start_y�� igrid �������̴�. �̰��� �����Ͽ� item
// (x, y)�� ���Ѵ�.
//-----------------------------------------------------------------------------
bool C_VS_UI_MIXING_FORGE::Check(int grid_start_x, int grid_start_y)
{
	MItem * p_item = g_pInventory->GetItem(m_focus_grid_x, m_focus_grid_y);
	MItem * p_item2;
	bool  b_check = false;
	
#ifdef _LIB
	if( p_item != NULL)
	{
		int msg = -1;
		if(p_item->IsUniqueItem() || p_item->IsQuestItem() )
			msg = UI_STRING_MESSAGE_CANNOT_MIXING_SPECIAL_ITEM;
		else
		if(p_item->GetItemOptionListCount() != 1)
			msg = UI_STRING_MESSAGE_CANNOT_MIXING_OPTION_COUNT;
		else
		if( !IsCorrectClass( p_item->GetItemClass() ) )
			msg = UI_STRING_MESSAGE_CANNOT_MIXING_ITEM_CLASS;
		else
		if( m_p_select_item[0] != NULL && m_p_select_item[1] != NULL && 
			!IsCorrectOption(m_p_select_item[0], m_p_select_item[1]))
			msg = UI_STRING_MESSAGE_CANNOT_MIXING_ITEM_OPTION;

		else
		if( m_p_select_item[0] == NULL && m_p_select_item[1] != NULL && 
			!IsCorrectOption(m_p_select_item[1],p_item))
			msg = UI_STRING_MESSAGE_CANNOT_MIXING_ITEM_OPTION;
		else
		if( m_p_select_item[0] != NULL && m_p_select_item[1] == NULL && 
			!IsCorrectOption(m_p_select_item[0], p_item))
			msg = UI_STRING_MESSAGE_CANNOT_MIXING_ITEM_OPTION;

		else		
		if(m_p_select_item[0] == NULL && m_p_select_item[1] != NULL &&
			m_p_select_item[1]->GetItemClass() != p_item->GetItemClass() )
			msg = UI_STRING_MESSAGE_CANNOT_MIXING_ITEM_CLASS;
		else
		if(m_p_select_item[0] != NULL && m_p_select_item[1] == NULL &&
			m_p_select_item[0]->GetItemClass() != p_item->GetItemClass() )
			msg = UI_STRING_MESSAGE_CANNOT_MIXING_ITEM_CLASS;
		//2004,04,27 sobeit check grade add start
		else
		if(m_p_select_item[0] == NULL && m_p_select_item[1] != NULL &&
			abs(m_p_select_item[1]->GetGrade() - p_item->GetGrade())>2)
			msg = UI_STRING_MESSAGE_CANNOT_MIXING_GRADE_VALUE;
		else
		if(m_p_select_item[0] != NULL && m_p_select_item[1] == NULL &&
			abs(m_p_select_item[0]->GetGrade() - p_item->GetGrade())>2)
			msg = UI_STRING_MESSAGE_CANNOT_MIXING_GRADE_VALUE;
		/*********************************************************

			TO DO : ���� �ɼ� üũ�� �� �ʿ� �� �� ���⿡ �߰�
			
		**********************************************************/
//		else if(p_item->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET)
//		{
//			ITEMTABLE_INFO::ELEMENTAL_TYPE eType = (*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].ElementalType;
//			if(eType != ITEMTABLE_INFO::ELEMENTAL_TYPE_ANY)
//			{
//				WORD eNum = (*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].Elemental;
//				if(!eNum)
//					msg = UI_STRING_MESSAGE_CANNOT_MIXING_ITEM_TYPE;
//				else
//				{
//					switch(m_forge_type)
//					{
//					case TYPE_A:
//						if(eNum>3)
//							msg = UI_STRING_MESSAGE_CANNOT_MIXING_ITEM_TYPE;
//						break;
//					case TYPE_B:
//						if(eNum>4)
//							msg = UI_STRING_MESSAGE_CANNOT_MIXING_ITEM_TYPE;
//						break;
//					case TYPE_C:
//						if(eNum>5)
//							msg = UI_STRING_MESSAGE_CANNOT_MIXING_ITEM_TYPE;
//						break;
//					}
//				}
//			}
//			else
//			{
//				msg = UI_STRING_MESSAGE_CANNOT_MIXING_ITEM_TYPE;
//			}
//		}
//		//2004,04,27 sobeit check grade add end
//		else if(p_item->GetItemClass() != ITEM_CLASS_OUSTERS_STONE)
//		{// �ƿ콺���� ���ɼ��� �ƴ� �� �ɼ� üũ 
//			if( !IsCorrectType( p_item->GetItemType() , p_item->GetItemClass()))
//				msg = UI_STRING_MESSAGE_CANNOT_MIXING_ITEM_TYPE;
//			else		
//			if(m_p_select_item[0] == NULL && m_p_select_item[1] != NULL &&
//				m_p_select_item[1]->GetItemType() != p_item->GetItemType() )
//				msg = UI_STRING_MESSAGE_CANNOT_MIXING_ITEM_TYPE;
//			else
//			if(m_p_select_item[0] != NULL && m_p_select_item[1] == NULL &&
//				m_p_select_item[0]->GetItemType() != p_item->GetItemType() )
//				msg = UI_STRING_MESSAGE_CANNOT_MIXING_ITEM_TYPE;
//
//		}
//		else if(p_item->GetItemClass() == ITEM_CLASS_OUSTERS_STONE)
//		{	// �ƿ콺���� ���ɼ� �� ��
//			// 0~4 �� ���ɼ�
//			// 5~9 �� ���ɼ�
//			// 10~14 ���� ���ɼ�
//			int nType1 = -1; int nType2 = -1;
//			if(m_p_select_item[0] == NULL && m_p_select_item[1] != NULL)
//			{
//				nType1 = m_p_select_item[1]->GetItemType();
//				nType2 = p_item->GetItemType();
//			}
//			else if(m_p_select_item[0] != NULL && m_p_select_item[1] == NULL)
//			{
//				nType1 = m_p_select_item[0]->GetItemType();
//				nType2 = p_item->GetItemType();
//			}
//
//			if( nType1 != -1  &&  nType2 != -1 )
//			{
//				if( nType1 >= 0 && nType1 <= 4 )
//				{
//					if( nType2 < 0 || nType2 > 4 )
//						msg = UI_STRING_MESSAGE_CANNOT_MIXING_ITEM_TYPE;
//				}
//				else if( nType1 >= 5 && nType1 <= 9 )
//				{
//					if( nType2 < 5 || nType2 > 10 )
//						msg = UI_STRING_MESSAGE_CANNOT_MIXING_ITEM_TYPE;
//				}
//				else if( nType1 >= 10 && nType1 <= 14 )
//				{
//					if( nType2 < 10 || nType2 > 14 )
//						msg = UI_STRING_MESSAGE_CANNOT_MIXING_ITEM_TYPE;
//				}
//			}
//		}

		if(msg != -1)
		{
			gpC_base->SendMessage(UI_CLOSE_MIXING_FORGE, msg);
			return false;
		}
	}
#endif
	if( m_p_select_item[0] == NULL && m_p_select_item[1] == NULL )
	{
		// �ͽ̰����� ����, ����,�Ƹ�,�Ǽ��� �´��� üũ
		if (p_item != NULL &&
			p_item->IsUniqueItem() == false &&
			p_item->GetItemOptionListCount() == 1 &&
			IsCorrectClass(p_item->GetItemClass()) &&
			(p_item->GetItemClass() == ITEM_CLASS_OUSTERS_STONE ||
			p_item->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET ||
			 IsCorrectType(p_item->GetItemType(), p_item->GetItemClass())
			 )
			)
		{			
			m_p_select_item[0] = p_item;
			b_check = true;
		}
	}
	else
	if( m_p_select_item[0] == NULL && m_p_select_item[1] != NULL )
	{
		// ���� �������̸� Check Off
		if( p_item == m_p_select_item[1] )
		{
			m_p_select_item[1] = NULL;
			b_check = true;
		}
		else
		// ù��° �����۰� �����迭����, �ͽ̰����� �������� �˻�
		// ���� �迭, �ٸ� �ɼ�, ����������� �ʵ�
		if( p_item != NULL &&			
			p_item->GetItemClass() == m_p_select_item[1]->GetItemClass() &&
			p_item->GetItemType()  == m_p_select_item[1]->GetItemType() &&
			p_item->IsUniqueItem() == false &&
			p_item->GetItemOptionListCount() == 1 &&			
			IsCorrectClass(p_item->GetItemClass()) &&
			(p_item->GetItemClass() == ITEM_CLASS_OUSTERS_STONE ||
			p_item->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET ||
			IsCorrectType(p_item->GetItemType(), p_item->GetItemClass())) &&
			IsCorrectOption(p_item, m_p_select_item[1]))
		{			
			m_p_select_item[0] = p_item;
			b_check = true;
		}
	}
	else
	if( m_p_select_item[0] != NULL && m_p_select_item[1] == NULL )
	{
		// ���� �������̸� Check Off
		if( p_item == m_p_select_item[0] )
		{
			m_p_select_item[0] = NULL;
			b_check = true;
		}
		else
		// ù��° �����۰� �����迭����, �ͽ̰����� �������� �˻�
		// ���� �迭, �ٸ� �ɼ�, ����������� �ʵ�
		if( p_item != NULL &&			
			p_item->GetItemClass() == m_p_select_item[0]->GetItemClass() &&
			p_item->GetItemType() == m_p_select_item[0]->GetItemType() &&
			p_item->IsUniqueItem() == false &&
			p_item->GetItemOptionListCount() == 1 &&
			IsCorrectClass(p_item->GetItemClass()) &&
			(p_item->GetItemClass() == ITEM_CLASS_OUSTERS_STONE ||
			p_item->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET||
			IsCorrectType(p_item->GetItemType(), p_item->GetItemClass())) &&
			IsCorrectOption(p_item, m_p_select_item[0]))
		{			
			m_p_select_item[1] = p_item;
			b_check = true;
		}
	}
	else
	if( m_p_select_item[0] != NULL && m_p_select_item[1] != NULL )
	{
		if( p_item == m_p_select_item[0] )
		{
			m_p_select_item[0] = NULL;
			b_check = true;
			
		}		
		else 
		if( p_item == m_p_select_item[1] )
		{
			m_p_select_item[1] = NULL;
			b_check = true;			
		}
	}


	if( b_check )
	{
		int check_index=0;
		g_pInventory->SetBegin();
		while (g_pInventory->IsNotEnd())
		{
			p_item2 = g_pInventory->Get();
			if( p_item == p_item2 )
			{
				m_v_check[check_index] = !m_v_check[check_index];
			}		
			g_pInventory->Next();
			check_index++;
		}
	}	

	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_MIXING_FORGE::GetFocusedItemGridX
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_MIXING_FORGE::GetFocusedItemGridX(const MItem * p_item)
{
	assert(p_item);

	if (p_item == NULL)
		return 0;

	return m_my_grid_rect.x+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_X*p_item->GetGridX();
}

//-----------------------------------------------------------------------------
// C_VS_UI_MIXING_FORGE::GetFocusedItemGridY
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_MIXING_FORGE::GetFocusedItemGridY(const MItem * p_item)
{
	assert(p_item);

	if (p_item == NULL)
		return 0;

	return m_my_grid_rect.y+C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y*p_item->GetGridY();
}

bool C_VS_UI_MIXING_FORGE::IsCorrectClass(ITEM_CLASS item_class)
{
	switch(m_forge_class)
	{
	case CLASS_WEAPON:
		switch( g_eRaceInterface )
		{
		case RACE_VAMPIRE :
			if( item_class == ITEM_CLASS_VAMPIRE_WEAPON )
				return true;
			break;
		case RACE_SLAYER :
			if( item_class == ITEM_CLASS_SWORD ||
				item_class == ITEM_CLASS_BLADE ||
				item_class == ITEM_CLASS_CROSS ||
				item_class == ITEM_CLASS_MACE  ||
				item_class == ITEM_CLASS_SG  ||
				item_class == ITEM_CLASS_SMG ||
				item_class == ITEM_CLASS_AR  ||
				item_class == ITEM_CLASS_SR )
				return true;
			break;
		case RACE_OUSTERS :
			if ( item_class == ITEM_CLASS_OUSTERS_CHAKRAM ||
				 item_class == ITEM_CLASS_OUSTERS_WRISTLET )
				return true;
			break;
		}
		break;
	case CLASS_ARMOR:
		switch( g_eRaceInterface )
		{
		case RACE_VAMPIRE :
			if( item_class == ITEM_CLASS_VAMPIRE_COAT )
				return true;
			break;
		case RACE_OUSTERS :
			if( item_class == ITEM_CLASS_OUSTERS_COAT ||
				item_class == ITEM_CLASS_OUSTERS_ARMSBAND ||
				item_class == ITEM_CLASS_OUSTERS_CIRCLET ||
				item_class == ITEM_CLASS_OUSTERS_BOOTS )
				return true;
			break;
		case RACE_SLAYER :
			if( item_class == ITEM_CLASS_COAT ||
				item_class == ITEM_CLASS_TROUSER ||
				item_class == ITEM_CLASS_SHOES ||
				item_class == ITEM_CLASS_GLOVE ||
				item_class == ITEM_CLASS_HELM ||
				item_class == ITEM_CLASS_BELT ||
				item_class == ITEM_CLASS_SHIELD )
				return true;
			break;
		}
		break;		
	case CLASS_ACCESSORY:
		switch( g_eRaceInterface )
		{
		case RACE_VAMPIRE :
			if( item_class == ITEM_CLASS_VAMPIRE_RING ||
				item_class == ITEM_CLASS_VAMPIRE_BRACELET ||
				item_class == ITEM_CLASS_VAMPIRE_NECKLACE ||
				item_class == ITEM_CLASS_VAMPIRE_AMULET ||
				item_class == ITEM_CLASS_VAMPIRE_EARRING )
				return true;
			break;
		case RACE_SLAYER :
			if( item_class == ITEM_CLASS_RING ||
				item_class == ITEM_CLASS_BRACELET ||
				item_class == ITEM_CLASS_NECKLACE )
				return true;
			break;
		case RACE_OUSTERS :
			if( item_class == ITEM_CLASS_OUSTERS_PENDENT ||
				item_class == ITEM_CLASS_OUSTERS_RING ||
				item_class == ITEM_CLASS_OUSTERS_STONE)
				return true;
			break;
		}
		break;
	}

	return false;
}

bool C_VS_UI_MIXING_FORGE::IsCorrectType(TYPE_ITEMTYPE item_type, ITEM_CLASS item_class)
{
	return true;
	switch( m_forge_type )
	{
	case TYPE_A:
		switch( g_eRaceInterface )
		{
		case RACE_VAMPIRE :
			switch( m_forge_class )
			{
			case CLASS_WEAPON:    if( item_type <= 8 ) return true; break;
			case CLASS_ARMOR:     if( item_type <= 5 ) return true; break;
			case CLASS_ACCESSORY: if( item_type <= 4 ) return true; break;
			}
			break;
		case RACE_SLAYER :
			switch( m_forge_class )
			{
			case CLASS_WEAPON:    if( item_type <= 5 ) return true; break;
			case CLASS_ARMOR:     if( item_type <= 11 ) return true; break;
			case CLASS_ACCESSORY: if( item_type <= 4 ) return true; break;
			}
			break;
		case RACE_OUSTERS :
			switch( m_forge_class )
			{
			case CLASS_WEAPON:		if( item_type <=6 ) return true; break;
			case CLASS_ARMOR:		if( item_type <=6 ) return true; break;
			case CLASS_ACCESSORY:	if( item_type <=6 ) return true; break;
			}
			break;
		}
		break;

	case TYPE_B:
		switch( g_eRaceInterface )
		{
		case RACE_VAMPIRE :
			switch( m_forge_class )
			{
			case CLASS_WEAPON:    if( item_type <= 13 ) return true; break;
			case CLASS_ARMOR:     if( item_type <= 9 ) return true; break;
			case CLASS_ACCESSORY: if( item_type <= 7 ) return true; break;
			}
			break;
		case RACE_SLAYER :
			switch( m_forge_class )
			{
			case CLASS_WEAPON:    if( item_type <= 8 ) return true; break;
			case CLASS_ARMOR:     if( item_type <= 15 ) return true; break;
			case CLASS_ACCESSORY: if( item_type <= 7 ) return true; break;
			}
		
			break;
		case RACE_OUSTERS :		
			switch( m_forge_class )
			{
			case CLASS_WEAPON :		if( item_type <= 8 ) return true; break;
			case CLASS_ARMOR :		if( item_type <= 8 ) return true; break;
			case CLASS_ACCESSORY :	if( item_type <= 8 ) return true; break;
			}
			break;
		}
		break;				

	case TYPE_C:
		switch( g_eRaceInterface )
		{
		case RACE_VAMPIRE :
			switch( m_forge_class )
			{
			case CLASS_WEAPON:    if( item_type <= 16 ) return true; break;
			case CLASS_ARMOR:     if( item_type <= 15 ) return true; break;
			case CLASS_ACCESSORY: if( item_type <= 9 ) return true; break;
			}
			break;
		case RACE_SLAYER :
			switch( m_forge_class )
			{
			case CLASS_WEAPON:    if( item_type <= 11 ) return true; break;
			case CLASS_ARMOR:     if( item_type <= 19 ) return true; break;
			case CLASS_ACCESSORY: if( item_type <= 9 ) return true; break;
			}
			break;
		case RACE_OUSTERS :
			switch( m_forge_class )
			{
			case CLASS_WEAPON :		if( item_type <= 10 ) return true; break;
			case CLASS_ARMOR:		if( item_type <= 10 ) return true; break;
			case CLASS_ACCESSORY :	if( item_type <= 10 ) return true; break;
			}
			break;
		}
		break;		
	case TYPE_D:
		switch( g_eRaceInterface )
		{
		case RACE_VAMPIRE :
			switch( m_forge_class )
			{
			case CLASS_WEAPON:    if( item_type <= 18 ) return true; break;
			case CLASS_ARMOR:     if( item_type <= 19 ) return true; break;
			case CLASS_ACCESSORY: if( item_type <= 13 ) return true; break;
			}
			break;
		case RACE_SLAYER :
			switch( m_forge_class )
			{
			case CLASS_WEAPON:    
				{
					if(item_class == ITEM_CLASS_CROSS || item_class == ITEM_CLASS_MACE)
					{
						if( item_type <= 11 ) 
							return true;
					}
					else
						if( item_type <= 13 ) 
							return true;
				}
				break;
			case CLASS_ARMOR:     if( item_type <= 23 ) return true; break;
			case CLASS_ACCESSORY: if( item_type <= 13 ) return true; break;
			}
			break;
		case RACE_OUSTERS :
			switch( m_forge_class )
			{
			case CLASS_WEAPON :		
				{
					if(item_class == ITEM_CLASS_OUSTERS_CHAKRAM)
					{
						if( item_type <= 13 ) 
							return true; 
					}
					else if(item_class == ITEM_CLASS_OUSTERS_WRISTLET)
					{
						if( item_type <= 42 ) 
							return true; 
					}
				}
				break;
			case CLASS_ARMOR :		if( item_type <= 11 ) return true; break;
			case CLASS_ACCESSORY :	if( item_type <= 13 ) return true; break;
			}
			break;
		}
		break;
	}

	return false;
}

bool C_VS_UI_MIXING_FORGE::IsCorrectOption(MItem *p_item1, MItem *p_item2)
{
	const std::list<TYPE_ITEM_OPTION> &optionList1 = p_item1->GetItemOptionList();
	const std::list<TYPE_ITEM_OPTION> &optionList2 = p_item2->GetItemOptionList();

	std::list<TYPE_ITEM_OPTION>::const_iterator itr1 = optionList1.begin();
	std::list<TYPE_ITEM_OPTION>::const_iterator itr2 = optionList2.begin();

	ITEMOPTION_INFO& optionInfo1 = (*g_pItemOptionTable)[*itr1];
	ITEMOPTION_INFO& optionInfo2 = (*g_pItemOptionTable)[*itr2];

	if( optionInfo1.Part != optionInfo2.Part )
		return true;

	return false;
}


//--------------------------------------------------------------------------------------
// C_VS_UI_REMOVE_OPTION
// 
// ��������� �ɼ��� ����� ������.
//--------------------------------------------------------------------------------------
C_VS_UI_REMOVE_OPTION::C_VS_UI_REMOVE_OPTION(const MItem * pItem, const MItem *pCurItem)
{		
	if(pItem == NULL || pCurItem == NULL)
		return;

	AttrTopmost( true );
	g_RegisterWindow(this);		
	
	int window_w = 340;
	int window_h = 180;
	
	Set(g_GameRect.right/2-window_w/2, g_GameRect.bottom/2 - window_h/2, window_w, window_h);
	
	int ok_x = w-200,  ok_y = h - 50;
	int cancle_x = w-90, cancle_y = h-50;
	
	m_pCurItem = pCurItem;
	m_pMouseItem = pItem;
		
	m_IsBokjory = false;

	if(m_pMouseItem->GetItemClass() == ITEM_CLASS_MIXING_ITEM && m_pMouseItem->GetItemType() == 18)
		m_IsBokjory = true;
	//�����ư
	m_pC_button_group = new ButtonGroup(this);
	
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(ok_x, ok_y, 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_OK), 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_OK), 
		OK_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_OK));
	
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(cancle_x, cancle_y, 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL), 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL),
		CANCEL_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL));

	m_status = STATUS_SELECT_OPTION;
	m_select_option = -1;
	m_b_press = false;
	
	const std::list<TYPE_ITEM_OPTION> &optionList=m_pCurItem->GetItemOptionList();
	std::list<TYPE_ITEM_OPTION>::const_iterator itr=optionList.begin();				

	for(int i=0;itr!=optionList.end();itr++,i++)
	{		
		if(*itr > g_pItemOptionTable->GetSize() || i >= 2)
			break;

		ITEMOPTION_INFO& optionInfo=(*g_pItemOptionTable)[*itr];
		m_str_option[i] = g_pItemOptionTable->ITEMOPTION_PARTNAME[optionInfo.Part].GetString();

		char temp[256] ={0,};
		
		int Point = optionInfo.PlusPoint - 100 ;

		if( optionInfo.Part == ITEMOPTION_TABLE::PART_DURABILITY )
		{
			if( Point > 0 )
				wsprintf(temp," +%d", Point);
			else
				wsprintf(temp," %d", Point);
		} else
		if( optionInfo.Part >= ITEMOPTION_TABLE::PART_STR_TO_DEX &&
			optionInfo.Part <= ITEMOPTION_TABLE::PART_INT_TO_DEX)
		{
			if( Point > 0)
				wsprintf(temp," +%d%", Point);
			else
				wsprintf(temp," %d%", Point);
		} else
		{
			if( Point+100 > 0)
				wsprintf(temp," +%d", Point + 100 );
			else
				wsprintf(temp," %d", Point + 100 );
		}
		m_str_option[i] += temp;
	}
}

C_VS_UI_REMOVE_OPTION::~C_VS_UI_REMOVE_OPTION()
{
	g_UnregisterWindow(this);
	
	DeleteNew(m_pC_button_group);	
}

void	C_VS_UI_REMOVE_OPTION::Start()
{
	PI_Processor::Start();
	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();
	AttrTopmost(true);
	CheckOption();
}

void	C_VS_UI_REMOVE_OPTION::Finish()
{
	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);	
}

void	C_VS_UI_REMOVE_OPTION::Show()
{
	int		field_y = 70, gap_y = 30;
	gpC_global_resource->DrawDialog(x, y, w, h, GetAttributes()->alpha);

	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_pC_button_group->Show();	
		
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	
	if(m_status == STATUS_SELECT_OPTION )
	{
		RECT rect = {x+30,y+field_y,x+w-30,y+field_y+23};	
		WORD	selectColor = CSDLGraphics::Color( 3,3,3);
		WORD	normalColor = 0;
		if(m_select_option == 0)
			gpC_base->m_p_DDSurface_back->FillRect(&rect, selectColor);
		else
			gpC_base->m_p_DDSurface_back->FillRect(&rect, normalColor);

		Rect rt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top);
		gpC_global_resource->DrawOutBox(rt);
		if(!m_IsBokjory)
		{
			rect.top += gap_y;
			rect.bottom += gap_y;	
			
			if(m_select_option == 1)
				gpC_base->m_p_DDSurface_back->FillRect(&rect, selectColor);
			else
				gpC_base->m_p_DDSurface_back->FillRect(&rect, normalColor);
			
			rt.y+=gap_y;
			gpC_global_resource->DrawOutBox(rt);
		}
	}
	if(g_FL2_GetDC())
	{
		switch( m_status )
		{
		case STATUS_SELECT_OPTION:
			{
				g_PrintColorStrOut( x+w/2-g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_WHAT_OPTION_REMOVE].GetString(), gpC_base->m_dialog_msg_pi.hfont)/2, 
								y+30, (*g_pGameStringTable)[UI_STRING_MESSAGE_WHAT_OPTION_REMOVE].GetString(), gpC_base->m_dialog_msg_pi, RGB_WHITE, RGB_BLACK);
				
				if( m_select_option == 0 )
					g_PrintColorStr( x+w/2-g_GetStringWidth(m_str_option[0].c_str(), gpC_base->m_dialog_msg_pi.hfont)/2,
		    							y+field_y+m_b_press+5, m_str_option[0].c_str(), gpC_base->m_dialog_msg_pi, RGB_WHITE);
				else
					g_PrintColorStr( x+w/2-g_GetStringWidth(m_str_option[0].c_str(), gpC_base->m_dialog_msg_pi.hfont)/2,
										y+field_y+5, m_str_option[0].c_str(), gpC_base->m_dialog_msg_pi, RGB_GRAY);
						
				if( m_select_option == 1 )
					g_PrintColorStr( x+w/2-g_GetStringWidth(m_str_option[1].c_str(), gpC_base->m_dialog_msg_pi.hfont)/2,		
			    						y+field_y+gap_y+m_b_press+5, m_str_option[1].c_str(), gpC_base->m_dialog_msg_pi, RGB_WHITE);
				else
					g_PrintColorStr( x+w/2-g_GetStringWidth(m_str_option[1].c_str(), gpC_base->m_dialog_msg_pi.hfont)/2,		
										y+field_y+gap_y+5, m_str_option[1].c_str(), gpC_base->m_dialog_msg_pi, RGB_GRAY);
			}
			break;

		case STATUS_QUESTION:
			{
				char sz_temp[100];
				wsprintf( sz_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_SELECT_OPTION].GetString(), m_str_option[m_select_option].c_str() ); 
				g_PrintColorStrOut( x+w/2-g_GetStringWidth(sz_temp, gpC_base->m_dialog_msg_pi.hfont)/2, 
									y+45, sz_temp, gpC_base->m_dialog_msg_pi, RGB_WHITE, RGB_BLACK);
				g_PrintColorStrOut( x+w/2-g_GetStringWidth((*g_pGameStringTable)[UI_STRING_MESSAGE_QUESTION_REMOVE_OPTION].GetString(), gpC_base->m_dialog_msg_pi.hfont)/2, 
									y+70, (*g_pGameStringTable)[UI_STRING_MESSAGE_QUESTION_REMOVE_OPTION].GetString(), gpC_base->m_dialog_msg_pi, RGB_WHITE, RGB_BLACK);
			}
			break;
		}	
			
		g_FL2_ReleaseDC();
	}
}

bool	C_VS_UI_REMOVE_OPTION::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);

	static char sz_temp[2][100];

	switch(message)
	{
	case M_MOVING:
		{
			if( m_status == STATUS_SELECT_OPTION )
			{				
				Rect rect;
				Point point(_x,_y);
//				int str_width;
				int MaxOption = m_IsBokjory?1:2;
				for(int i=0; i<MaxOption; i++)
				{
					//str_width = g_GetStringWidth(m_str_option[0].c_str(), gpC_base->m_chatting_pi.hfont);					
					rect.Set( w/2-(w-60)/2, 70+(i*30), w-60, 20);

					if(rect.IsInRect(point) && m_status == STATUS_SELECT_OPTION ) 
					{
						m_select_option = i;
						break;
					}
					else m_select_option = -1;
				}					
			}			
		}
		break;

	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		{
			m_b_press = true;						
		}
		break;

	case M_LEFTBUTTON_UP:
		{
			m_b_press = false;
			if( m_status == STATUS_SELECT_OPTION && (m_select_option == 0 || m_select_option == 1) )
			{
				m_status = STATUS_QUESTION;
			}
		}
		break;
	}	
	return true;
}

void	C_VS_UI_REMOVE_OPTION::KeyboardControl(UINT message, UINT key, long extra)	
{	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{		
		case VK_ESCAPE :
			gpC_base->SendMessage(UI_CLOSE_REMOVE_OPTION);
			break;		
		}
	}
}

bool	C_VS_UI_REMOVE_OPTION::IsPixel(int _x,int _y)
{
	return IsInRect(_x, _y);
}

void	C_VS_UI_REMOVE_OPTION::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	switch( p_button->GetID() )
	{
	case OK_ID:
		{
			if( m_status == STATUS_QUESTION )
			{
				if(p_button->GetFocusState())
				{
					if(p_button->GetPressState())
						gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
					else
						gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
				} else
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index);
			}
		}
		break;

	case CANCEL_ID:
		{
			if(p_button->GetFocusState())
			{
				if(p_button->GetPressState())
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
				else
					gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
			} else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index);
		}
		break;
	}	
}

void	C_VS_UI_REMOVE_OPTION::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
	char* m_other_info_button_string[2]=
	{	
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
	};

	switch(p_button->GetID())
	{
	case OK_ID:
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[0],0,0);
		break;

	case CANCEL_ID :
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[1],0,0);
		break;	
	}	
}

void	C_VS_UI_REMOVE_OPTION::WindowEventReceiver(id_t event)
{
}

void	C_VS_UI_REMOVE_OPTION::Run(id_t id)
{
	switch (id)
	{
	case OK_ID:
		if(m_select_option <0 || m_select_option > 1)
			return;
		gpC_base->SendMessage(UI_SEND_REMOVE_OPTION, m_select_option, m_pCurItem->GetID(), (void*)m_pMouseItem);
		break;

	case CANCEL_ID:
		{
			gpC_base->SendMessage(UI_CLOSE_REMOVE_OPTION);			
		}		
		break;		
	}
}

void	C_VS_UI_REMOVE_OPTION::Process()
{
	m_pC_button_group->Process();
	ProcessHide();
}

void	C_VS_UI_REMOVE_OPTION::CheckOption()
{	
	m_status = STATUS_SELECT_OPTION;
}


// ���� ��� �ִ� ���������� ������ �������� �ɼ��� ������ �� �ִ��� �Ǻ��Ѵ�.
bool
C_VS_UI_REMOVE_OPTION::IsCanRemoveOption_Puritas(const MItem* pItem, const MItem* pTItem)
{
	if( pItem == NULL || pTItem == NULL ||
		pTItem->IsQuestItem() || pTItem->IsUniqueItem() || 
		
		pItem->GetItemClass() == ITEM_CLASS_MIXING_ITEM && pItem->GetItemType() != 18 &&pTItem->GetItemClass() != ITEM_CLASS_PET_ITEM && pTItem->GetItemOptionListCount() != 2 || // �Ϲ������� 2�ɼ��� �ϳ��� ���� �Ѵ�.
		pItem->GetItemClass() == ITEM_CLASS_MIXING_ITEM && pItem->GetItemType() == 18 &&pTItem->GetItemClass() != ITEM_CLASS_PET_ITEM && pTItem->GetItemOptionListCount() != 1||
		!(
		pItem->GetItemClass() == ITEM_CLASS_MIXING_ITEM && 
		(pItem->GetItemType() >= 9 && pItem->GetItemType() <= 18 || pItem->GetItemType() >= 22 && pItem->GetItemType() <= 24)
		)
		)
		return false;
	
	if(pTItem->GetItemClass() == ITEM_CLASS_PET_ITEM)
	{
		if(pTItem->GetNumber() == 49 && pTItem->GetItemOptionListCount() > 0)
			return true;
		else
			return false;
	}
	enum { CLASS_WEAPON, CLASS_ARMOR, CLASS_ACCESSORY , CLASS_ALL};	

	int		RemoveItemClass = -1;
	if(pItem->GetItemType() >= 9 && pItem->GetItemType() <=11 
		|| pItem->GetItemType() == 22)
		RemoveItemClass = CLASS_WEAPON;
	else if(pItem->GetItemType() >= 12 && pItem->GetItemType() <= 14
		|| pItem->GetItemType() == 23)
		RemoveItemClass = CLASS_ARMOR;
	else if(pItem->GetItemType() >= 15 && pItem->GetItemType() <= 17
		|| pItem->GetItemType() == 24)
		RemoveItemClass = CLASS_ACCESSORY;
	else if(pItem->GetItemType() == 18) // ������
		RemoveItemClass = CLASS_ALL;
	else return false;

	switch( g_eRaceInterface )
	{
	case RACE_VAMPIRE :
		if( !pTItem->IsVampireItem() )
			return false;
		break;
	case RACE_OUSTERS :
		if( !pTItem->IsOustersItem() )
			return false;
		break;
	case RACE_SLAYER :
		if( !pTItem->IsSlayerItem() )
			return false;
		break;
	}
	
	switch( RemoveItemClass )
	{
	case CLASS_WEAPON :
		if ( !(pTItem->GetItemClass() == ITEM_CLASS_VAMPIRE_WEAPON ||pTItem->GetItemClass() == ITEM_CLASS_SWORD ||
			pTItem->GetItemClass() == ITEM_CLASS_BLADE ||pTItem->GetItemClass() == ITEM_CLASS_CROSS ||
			pTItem->GetItemClass() == ITEM_CLASS_MACE  ||pTItem->GetItemClass() == ITEM_CLASS_SG  ||
			pTItem->GetItemClass() == ITEM_CLASS_SMG ||pTItem->GetItemClass() == ITEM_CLASS_AR  ||
			pTItem->GetItemClass() == ITEM_CLASS_SR || pTItem->GetItemClass() == ITEM_CLASS_OUSTERS_CHAKRAM ||
			pTItem->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET) )
			return false;
		break;
	case CLASS_ARMOR :
		if( !(pTItem->GetItemClass() == ITEM_CLASS_VAMPIRE_COAT ||pTItem->GetItemClass() == ITEM_CLASS_COAT ||
			pTItem->GetItemClass() == ITEM_CLASS_TROUSER ||pTItem->GetItemClass() == ITEM_CLASS_SHOES ||
			pTItem->GetItemClass() == ITEM_CLASS_GLOVE ||pTItem->GetItemClass() == ITEM_CLASS_HELM ||
			pTItem->GetItemClass() == ITEM_CLASS_BELT ||pTItem->GetItemClass() == ITEM_CLASS_SHIELD ||
			pTItem->GetItemClass() == ITEM_CLASS_OUSTERS_COAT || pTItem->GetItemClass() == ITEM_CLASS_OUSTERS_CIRCLET ||
			pTItem->GetItemClass() == ITEM_CLASS_OUSTERS_ARMSBAND || pTItem->GetItemClass() == ITEM_CLASS_OUSTERS_BOOTS))
			return false;
		break;
	case CLASS_ACCESSORY :
		if( !(pTItem->GetItemClass() == ITEM_CLASS_VAMPIRE_RING ||pTItem->GetItemClass() == ITEM_CLASS_VAMPIRE_BRACELET ||
			pTItem->GetItemClass() == ITEM_CLASS_VAMPIRE_NECKLACE ||pTItem->GetItemClass() == ITEM_CLASS_VAMPIRE_AMULET ||
			pTItem->GetItemClass() == ITEM_CLASS_VAMPIRE_EARRING || pTItem->GetItemClass() == ITEM_CLASS_RING ||
			pTItem->GetItemClass() == ITEM_CLASS_BRACELET || pTItem->GetItemClass() == ITEM_CLASS_NECKLACE ||
			pTItem->GetItemClass() == ITEM_CLASS_OUSTERS_PENDENT || pTItem->GetItemClass() == ITEM_CLASS_OUSTERS_RING ||
			pTItem->GetItemClass() == ITEM_CLASS_OUSTERS_STONE
			))
			return false;
		break;
	case CLASS_ALL:
		if( !pTItem->IsGearItem())
			return false;
		break;
	default :
		return false;
	}	
	// ������� ������ Ŭ������ �����Ǿ���. ������ Ÿ����-_- �����ϸ� �ȴ�.
	
	enum {TYPE_A,TYPE_B,TYPE_C, TYPE_D};
	int RemoveItemType = TYPE_A + (pItem->GetItemType() - 9) %3;
	if(pItem->GetItemType() >= 22 && pItem->GetItemType() <= 24)
		RemoveItemType = TYPE_D;

	switch( RemoveItemType )
	{
	case TYPE_A:
		switch( g_eRaceInterface )
		{
		case RACE_VAMPIRE :
			switch( RemoveItemClass )
			{
			case CLASS_WEAPON:    if( pTItem->GetItemType() > 8 ) return false; break;
			case CLASS_ARMOR:     if( pTItem->GetItemType() > 5 ) return false; break;
			case CLASS_ACCESSORY: if( pTItem->GetItemType() > 4 ) return false; break;	
			case CLASS_ALL: break;
			}
			break;
		case RACE_SLAYER :
			switch( RemoveItemClass )
			{
			case CLASS_WEAPON:    if( pTItem->GetItemType() > 5 ) return false; break;
			case CLASS_ARMOR:     if( pTItem->GetItemType() > 11 ) return false; break;
			case CLASS_ACCESSORY: if( pTItem->GetItemType() > 4 ) return false; break;
			case CLASS_ALL: break;
			}
			break;
		case RACE_OUSTERS :
			switch( RemoveItemClass )
			{
			case CLASS_WEAPON :		if( pTItem->GetItemType() > 6 ) return false; break;
			case CLASS_ARMOR :		if( pTItem->GetItemType() > 6 ) return false; break;
			case CLASS_ACCESSORY :	if( pTItem->GetItemType() > 6 && pTItem->GetItemClass() != ITEM_CLASS_OUSTERS_STONE) return false; break;
			case CLASS_ALL: break;
			}
			break;
		}
		break;

	case TYPE_B:
		switch( g_eRaceInterface )
		{
		case RACE_VAMPIRE :
			switch( RemoveItemClass )
			{
			case CLASS_WEAPON:    if( pTItem->GetItemType() > 13 ) return false; break;
			case CLASS_ARMOR:     if( pTItem->GetItemType() > 9 ) return false; break;
			case CLASS_ACCESSORY: if( pTItem->GetItemType() > 7 ) return false; break;
			case CLASS_ALL: break;
			}
			break;
		case RACE_SLAYER :
			switch( RemoveItemClass )
			{
			case CLASS_WEAPON:    if( pTItem->GetItemType() > 8 ) return false; break;
			case CLASS_ARMOR:     if( pTItem->GetItemType() > 15 ) return false; break;
			case CLASS_ACCESSORY: if( pTItem->GetItemType() > 7 ) return false; break;
			case CLASS_ALL: break;
			}
			break;
		case RACE_OUSTERS :
			switch( RemoveItemClass )
			{
			case CLASS_WEAPON :		if( pTItem->GetItemType() > 8 ) return false; break;
			case CLASS_ARMOR :		if( pTItem->GetItemType() > 8 ) return false; break;
			case CLASS_ACCESSORY :	if( pTItem->GetItemType() > 8 && pTItem->GetItemClass() != ITEM_CLASS_OUSTERS_STONE) return false; break;
			case CLASS_ALL: break;
			}
			break;
		}
		break;				

	case TYPE_C:
		switch( g_eRaceInterface )
		{
		case RACE_VAMPIRE :
			switch( RemoveItemClass )
			{
			case CLASS_WEAPON:    if( pTItem->GetItemType() > 16 ) return false; break;
			case CLASS_ARMOR:     if( pTItem->GetItemType() > 15 ) return false; break;
			case CLASS_ACCESSORY: if( pTItem->GetItemType() > 9 ) return false; break;
			case CLASS_ALL: break;
			}
			break;
		case RACE_SLAYER :
			switch( RemoveItemClass )
			{
			case CLASS_WEAPON:    if( pTItem->GetItemType() > 11 ) return false; break;
			case CLASS_ARMOR:     if( pTItem->GetItemType() > 19 ) return false; break;
			case CLASS_ACCESSORY: if( pTItem->GetItemType() > 9 ) return false; break;
			case CLASS_ALL: break;
			}
			break;
		case RACE_OUSTERS :
			switch( RemoveItemClass )
			{
			case CLASS_WEAPON :		if( pTItem->GetItemType() > 10 ) return false; break;
			case CLASS_ARMOR :		if( pTItem->GetItemType() > 10 ) return false; break;
			case CLASS_ACCESSORY :	if( pTItem->GetItemType() > 10 && pTItem->GetItemClass() != ITEM_CLASS_OUSTERS_STONE) return false; break;
			case CLASS_ALL: break;
			}
			break;
		}
		break;		
	case TYPE_D:
		switch( g_eRaceInterface )
		{
		case RACE_VAMPIRE :
			switch( RemoveItemClass )
			{
			case CLASS_WEAPON:    if( pTItem->GetItemType() > 18 ) return false; break;
			case CLASS_ARMOR:     if( pTItem->GetItemType() > 19 ) return false; break;
			case CLASS_ACCESSORY: if( pTItem->GetItemType() > 13 ) return false; break;
			case CLASS_ALL: break;
			}
			break;
		case RACE_SLAYER :
			switch( RemoveItemClass )
			{
			case CLASS_WEAPON:    
				{
					if(pTItem->GetItemClass() == ITEM_CLASS_CROSS || pTItem->GetItemClass() == ITEM_CLASS_MACE)
					{
						if( pTItem->GetItemType() > 11 ) 
							return false;
					}
					else
						if( pTItem->GetItemType() > 13 ) 
							return false;
				}
				break;
			case CLASS_ARMOR:     if( pTItem->GetItemType() > 23 ) return false; break;
			case CLASS_ACCESSORY: if( pTItem->GetItemType() > 13 ) return false; break;
			case CLASS_ALL: break;
			}
			break;
		case RACE_OUSTERS :
			switch( RemoveItemClass )
			{
			case CLASS_WEAPON :		
				{
					if(pTItem->GetItemClass() == ITEM_CLASS_OUSTERS_CHAKRAM)
					{
						if( pTItem->GetItemType() > 13 ) 
							return false; 
					}
					else if(pTItem->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET)
					{
						if( pTItem->GetItemType() > 42 ) 
							return false; 
					}
					else
						return false;
				}
				break;
			case CLASS_ARMOR :		if( pTItem->GetItemType() > 11 ) return false; break;
			case CLASS_ACCESSORY :	if( pTItem->GetItemType() > 13 && pTItem->GetItemClass() != ITEM_CLASS_OUSTERS_STONE) return false; break;
			case CLASS_ALL: break;
			}
			break;
		}
		break;

	}

	// -_-v
	return true;
}

///////////////////////////////////////////////////////////////////
//
// Ousters SkillInfo
//
///////////////////////////////////////////////////////////////////
C_VS_UI_OUSTERS_SKILL_INFO::C_VS_UI_OUSTERS_SKILL_INFO(int skillID, int window_x, int window_y, bool DownSkill)
{		
//	AttrTopmost( true );
	g_RegisterWindow(this);		
	
	int window_w = 300;
	int window_h = 350;

	m_bDownSkill = DownSkill;
	
	Set(window_x, window_y, window_w, window_h);

	m_pC_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(240, 200, -1, 70));

	SetSkillID(skillID);
	
	//�����ư
	m_pC_button_group = new ButtonGroup(this);

	int close_x = w-45, close_y = h-40;
	int help_x = w-70, help_y = h-40;
	int alpha_x = 30, alpha_y = h-40;
	int learn_x = w-140, learn_y = h-43;

	//�����ư
	m_pC_button_group = new ButtonGroup(this);
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_x, close_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_X), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_X), CLOSE_ID, this,C_GLOBAL_RESOURCE::AB_BUTTON_X));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(help_x, help_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION), HELP_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION));
// 	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(alpha_x, alpha_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), ALPHA_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA));

	if( m_bDownSkill )
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(learn_x, learn_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::OUSTERS_DOWN_SKILL_BUTTON), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::OUSTERS_DOWN_SKILL_BUTTON), LEARN_ID, this,C_GLOBAL_RESOURCE::OUSTERS_DOWN_SKILL_BUTTON));
	else
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(learn_x, learn_y, gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::OUSTERS_LEARN_BUTTON), gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::OUSTERS_LEARN_BUTTON), LEARN_ID, this,C_GLOBAL_RESOURCE::OUSTERS_LEARN_BUTTON));

}

C_VS_UI_OUSTERS_SKILL_INFO::~C_VS_UI_OUSTERS_SKILL_INFO()
{
	g_UnregisterWindow(this);
	
	DeleteNew(m_pC_button_group);
	DeleteNew(m_pC_scroll_bar);
	DeleteNew(gpC_dialog_ousters_down_skill);
	DeleteNew(gpC_dialog_ousters_level_up_to_last_skill_level);
}

void	C_VS_UI_OUSTERS_SKILL_INFO::Start()
{
	PI_Processor::Start();
	AttrPin(true);
	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();	

	g_CurrentSkillID = 0xFFFFFFFF;
}

void	C_VS_UI_OUSTERS_SKILL_INFO::Finish()
{
	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);	
}

void	C_VS_UI_OUSTERS_SKILL_INFO::Show()
{
	bool bCanLearn = false;

	MSkillDomain::SKILLSTATUS status = (*g_pSkillManager)[SKILLDOMAIN_OUSTERS].GetSkillStatus((ACTIONINFO)m_skillID);
	SKILLINFO_NODE sInfo = (*g_pSkillInfoTable)[m_skillID];

	CSpriteSurface::SetEffect(CSpriteSurface::EFFECT_GRAY_SCALE);

	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_global_resource->DrawDialogLocked(x, y, w, h, gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::INFO));

		Rect rect(x+30, y+50, w-60, h-100);
		gpC_global_resource->DrawOutBoxLocked(rect);
		rect.Set(x+40, y+180, w-80, 110);
		gpC_global_resource->DrawOutBoxLocked(rect);

		POINT p = {x+50, y+70};

		int sprID = sInfo.GetSpriteID();
		
		switch(status)
		{
		case MSkillDomain::SKILLSTATUS_LEARNED:
			{
				if(sInfo.IsEnable() == false)
				{
					if(sprID < C_VS_UI_SKILL::m_C_spk.GetSize() && C_VS_UI_SKILL::m_C_spk[sprID].IsInit())
						gpC_base->m_p_DDSurface_back->BltSpriteEffect(&p, &C_VS_UI_SKILL::m_C_spk[sprID]);
				}
				if(sInfo.IsPassive())
				{
					if(sprID < C_VS_UI_SKILL::m_C_spk.GetSize() && C_VS_UI_SKILL::m_C_spk[sprID].IsInit())
						gpC_base->m_p_DDSurface_back->BltSpriteColorSet(&p, &C_VS_UI_SKILL::m_C_spk[sprID], 315);
				}
				else
				{
					if(sprID < C_VS_UI_SKILL::m_C_spk.GetSize() && C_VS_UI_SKILL::m_C_spk[sprID].IsInit())
						gpC_base->m_p_DDSurface_back->BltSprite(&p, &C_VS_UI_SKILL::m_C_spk[sprID]);
				}
			}
			break;
			
		default:
			{
				if(sInfo.SkillPoint <= g_char_slot_ingame.skill_point && sInfo.GetLearnLevel() <= g_char_slot_ingame.level)
				{
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
										&& sInfo.GetSkillStep() == (*g_pSkillInfoTable)[*itr2].GetSkillStep())
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
					{
						if(sprID < C_VS_UI_SKILL::m_C_spk.GetSize() && C_VS_UI_SKILL::m_C_spk[sprID].IsInit())
							gpC_base->m_p_DDSurface_back->BltSpriteColor(&p, &C_VS_UI_SKILL::m_C_spk[sprID], rgb_GREEN);
					}
				}
				
				if(bCanLearn == false)
				{
					if(sprID < C_VS_UI_SKILL::m_C_spk.GetSize() && C_VS_UI_SKILL::m_C_spk[sprID].IsInit())
						gpC_base->m_p_DDSurface_back->BltSpriteEffect(&p, &C_VS_UI_SKILL::m_C_spk[sprID]);
				}
			}
			break;
		}
		//Skill icon back
		p.x-=7;p.y-=7;
		//gpC_base->m_p_DDSurface_back->BltSprite(&p, &gpC_global_resource->m_pC_info_spk->GetSprite(C_GLOBAL_RESOURCE::OUSTERS_SKILL_BACK));

		if(sInfo.GetLearnLevel()>1)
			gpC_base->m_p_DDSurface_back->BltSprite(&p, &gpC_global_resource->m_pC_info_spk->GetSprite(C_GLOBAL_RESOURCE::OUSTERS_SKILL_BACK_2));
		else
			gpC_base->m_p_DDSurface_back->BltSprite(&p, &gpC_global_resource->m_pC_info_spk->GetSprite(C_GLOBAL_RESOURCE::OUSTERS_SKILL_BACK));

		
		
		m_pC_button_group->Show();	
		
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	SetDescScrollPos(m_pC_scroll_bar->GetScrollPos());
	ShowDesc(x, y+20);
	
	m_pC_scroll_bar->Show(x, y);

	if(g_FL2_GetDC())
	{
		int TempX = g_PrintColorStr(x+30, y+35, sInfo.GetHName(), gpC_base->m_chatting_pi, RGB_WHITE);
		
		char szTemp[512];

		const int exp_level = sInfo.GetExpLevel();

		COLORREF color = RGB_WHITE;
		DWORD shadow_color = RGB_BLACK;

//		SKILLINFO_NODE sInfo = (*g_pSkillInfoTable)[m_skillID];
		if(sInfo.IsPassive())
		{	
			color = RGB(150, 200, 255);
			// 2004, 10, 20, sobeit modify start - 
			//strcpy(szTemp, (*g_pGameStringTable)[UI_STRING_MESSAGE_HAN_PASSIVE].GetString());
			//g_PrintColorStrShadow(x+100, y+86, szTemp, gpC_base->m_chatting_pi, color, shadow_color);
			sprintf(szTemp, "(%s)", (*g_pGameStringTable)[UI_STRING_MESSAGE_HAN_PASSIVE].GetString());
			g_PrintColorStrShadow(TempX +10, y+35, szTemp, gpC_base->m_chatting_pi, color, shadow_color);
			// 2004, 10, 20, sobeit modify end
			
//			if(g_char_slot_ingame.skill_point < sInfo.LevelUpPoint)
//				color = RGB_GRAY;
//			else
//				color = RGB_WHITE;
//			
//			sprintf(szTemp,(*g_pGameStringTable)[UI_STRING_MESSAGE_REQUIRE_SKILL_POINT].GetString(), sInfo.LevelUpPoint );
//			g_PrintColorStr(x+40, y+138, szTemp, gpC_base->m_chatting_pi, RGB_WHITE);
//			
//			sprintf(szTemp, (*g_pGameStringTable)[UI_STRING_MESSAGE_SKILL_LEVEL].GetString(), exp_level);					
		}

		switch(status)
		{
			case MSkillDomain::SKILLSTATUS_LEARNED:
				{
					if(g_char_slot_ingame.skill_point < sInfo.LevelUpPoint)
						color = RGB_GRAY;
					else
						color = RGB_WHITE;
					
					if( m_bDownSkill )
					{
						g_PrintColorStr(x+40, y+142, (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_DOWN_SKILL].GetString(), gpC_base->m_chatting_pi, color);
												
						sprintf(szTemp,(*g_pGameStringTable)[UI_STRING_MESSAGE_WITHDRAW_POINT].GetString(), sInfo.LevelUpPoint );
						g_PrintColorStr(x+40, y+158, szTemp, gpC_base->m_chatting_pi, RGB_WHITE);
					} else
					{
						g_PrintColorStr(x+40, y+142, (*g_pGameStringTable)[UI_STRING_MESSAGE_SKILL_LEVEL_UP].GetString(), gpC_base->m_chatting_pi, color);
						
						sprintf(szTemp,(*g_pGameStringTable)[UI_STRING_MESSAGE_REQUIRE_SKILL_POINT].GetString(), sInfo.LevelUpPoint );
						g_PrintColorStr(x+40, y+158, szTemp, gpC_base->m_chatting_pi, RGB_WHITE);
					}
					sprintf(szTemp, (*g_pGameStringTable)[UI_STRING_MESSAGE_SKILL_LEVEL].GetString(), exp_level);
				}
			break;

			default:
				if(sInfo.SkillPoint <= g_char_slot_ingame.skill_point && bCanLearn)
					color = RGB_GREEN;
				else
					color = RGB_GRAY;

				sprintf(szTemp, (*g_pGameStringTable)[UI_STRING_MESSAGE_SKILL_LEVEL].GetString(), exp_level);
				g_PrintColorStr(x+40, y+140, (*g_pGameStringTable)[UI_STRING_MESSAGE_LEARN_SKILL].GetString(), gpC_base->m_chatting_pi, color);
				sprintf(szTemp,(*g_pGameStringTable)[UI_STRING_MESSAGE_REQUIRE_SKILL_POINT].GetString(), sInfo.SkillPoint );
				g_PrintColorStr(x+40, y+158, szTemp, gpC_base->m_chatting_pi, color);

				sprintf(szTemp, (*g_pGameStringTable)[UI_STRING_MESSAGE_SKILL_LEVEL].GetString(), exp_level);
				break;
		}

		g_PrintColorStrShadow(x+100, y+66, szTemp, gpC_base->m_chatting_pi, color, shadow_color);

		// 200, 10, 20, sobeit add start
		int line_gap = 0;
		if(sInfo.GetLearnLevel()>1)
		{
			sprintf(szTemp, (*g_pGameStringTable)[UI_STRING_LEARN_SKILL_LEVEL].GetString(), sInfo.GetLearnLevel());
			g_PrintColorStr(x+100, y+82, szTemp, gpC_base->m_chatting_pi, color);
			line_gap += 16;
		}
			
		
		// 200, 10, 20, sobeit add end		
		if(sInfo.ElementalDomain == ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE ||
			sInfo.ElementalDomain == ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER ||
			sInfo.ElementalDomain == ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH ||
			sInfo.ElementalDomain == ITEMTABLE_INFO::ELEMENTAL_TYPE_WIND
			)
		{
			int elemental_point[5] = { sInfo.Fire, sInfo.Water, sInfo.Earth, sInfo.Wind, sInfo.Sum };			
			sprintf(szTemp, (*g_pGameStringTable)[UI_STRING_MESSAGE_REQUIRE_ELEMENTAL_LEVEL].GetString(), (*g_pGameStringTable)[g_ELEMENTAL_STRING_ID[sInfo.ElementalDomain]].GetString(), elemental_point[sInfo.ElementalDomain]);
			g_PrintColorStr(x+100, y+82+line_gap, szTemp, gpC_base->m_chatting_pi, g_ELEMENTAL_COLOR[sInfo.ElementalDomain]);
			line_gap += 16;
		}

		if(sInfo.CanDelete)
			g_PrintColorStr(x+100, y+82+line_gap, (*g_pGameStringTable)[UI_STRING_MESSAGE_CAN_SKILL_DELETE].GetString(), gpC_base->m_chatting_pi, RGB_GREEN);
		else
			g_PrintColorStr(x+100, y+82+line_gap, (*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_SKILL_DELETE].GetString(), gpC_base->m_chatting_pi, RGB_RED);


		g_FL2_ReleaseDC();
	}

}

bool	C_VS_UI_OUSTERS_SKILL_INFO::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	_x-=x; _y-=y;
	bool re = m_pC_scroll_bar->MouseControl(message, _x, _y);
	re &= m_pC_button_group->MouseControl(message, _x, _y);

	switch(message)
	{
	case M_MOVING:
		break;

	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if (!gpC_mouse_pointer->GetPickUpItem() && re)
		{
			MoveReady();
			SetOrigin(_x, _y);
			break;
		}
		break;

	case M_LEFTBUTTON_UP:
		break;
		
	case M_WHEEL_UP:
		m_pC_scroll_bar->ScrollUp();
		break;
		
	case M_WHEEL_DOWN:
		m_pC_scroll_bar->ScrollDown();
		break;
	}	
	return true;
}

void	C_VS_UI_OUSTERS_SKILL_INFO::KeyboardControl(UINT message, UINT key, long extra)	
{	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{		
		case VK_ESCAPE :
			gpC_base->SendMessage(UI_CLOSE_OUSTERS_SKILL_INFO);
			break;		
		}
	}
}

bool	C_VS_UI_OUSTERS_SKILL_INFO::IsPixel(int _x,int _y)
{
	return IsInRect(_x, _y);
}

void	C_VS_UI_OUSTERS_SKILL_INFO::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetID() == ALPHA_ID)
	{
		if(GetAttributes()->alpha)
		{
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
		}
		else
		{
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
		}
	}
	else if(p_button->GetID() == LEARN_ID)
	{
		MSkillDomain::SKILLSTATUS status = (*g_pSkillManager)[SKILLDOMAIN_OUSTERS].GetSkillStatus((ACTIONINFO)m_skillID);
		
		int sprID;
		if(p_button->GetFocusState())
		{		
			if(p_button->GetPressState())
				sprID = p_button->m_image_index+2;
			else
				sprID = p_button->m_image_index+1;
		}
		else
			sprID = p_button->m_image_index;
		
		POINT p = { x+p_button->x, y+p_button->y };

		switch(status)
		{
		case MSkillDomain::SKILLSTATUS_LEARNED:
			if( m_bDownSkill )
			{
				bool bCanDown = false;

				SKILLINFO_NODE sInfo = (*g_pSkillInfoTable)[m_skillID];
				if( (sInfo.GetExpLevel() > 1  || 
					(sInfo.CanDelete && sInfo.GetExpLevel() == 1) )

				//	&&(*g_pSkillManager)[SKILLDOMAIN_OUSTERS].IsAvailableDeleteSkill((ACTIONINFO)m_skillID)
					/*&& sInfo.GetExpLevel() < 30*/ )
				{
					bCanDown = true;
				}

				if( bCanDown )
					gpC_base->m_p_DDSurface_back->BltSprite( &p, &gpC_global_resource->m_pC_info_spk->GetSprite( sprID ) );
				else
					gpC_base->m_p_DDSurface_back->BltSpriteEffect( &p, &gpC_global_resource->m_pC_info_spk->GetSprite( C_GLOBAL_RESOURCE::OUSTERS_DOWN_SKILL_BUTTON ) );
			} else
			{
				bool bCanLearn = false;
				SKILLINFO_NODE sInfo = (*g_pSkillInfoTable)[m_skillID];
				if(sInfo.LevelUpPoint <= g_char_slot_ingame.skill_point && 
					sInfo.GetExpLevel() < 30 
					)
				{
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
					{
						if( m_skillID == SKILL_FIRE_OF_SOUL_STONE || m_skillID == SKILL_ICE_OF_SOUL_STONE || 
							m_skillID == SKILL_SAND_OF_SOUL_STONE || m_skillID == SKILL_BLOCK_HEAD ||
							m_skillID == SKILL_ABSORB_SOUL || m_skillID == SKILL_SUMMON_SYLPH )
							gpC_base->m_p_DDSurface_back->BltSpriteEffect(&p, &gpC_global_resource->m_pC_info_spk->GetSprite(C_GLOBAL_RESOURCE::OUSTERS_PLUS_BUTTON));
						else
							gpC_base->m_p_DDSurface_back->BltSprite(&p, &gpC_global_resource->m_pC_info_spk->GetSprite(sprID+3));
					}
				}
				if(!bCanLearn)
					gpC_base->m_p_DDSurface_back->BltSpriteEffect(&p, &gpC_global_resource->m_pC_info_spk->GetSprite(C_GLOBAL_RESOURCE::OUSTERS_PLUS_BUTTON));
			}
			break;
			
		default:
			if( m_bDownSkill )
			{
				gpC_base->m_p_DDSurface_back->BltSpriteEffect(&p, &gpC_global_resource->m_pC_info_spk->GetSprite(C_GLOBAL_RESOURCE::OUSTERS_DOWN_SKILL_BUTTON));				
			} else
			{
				bool bCanLearn = false;

				SKILLINFO_NODE sInfo = (*g_pSkillInfoTable)[m_skillID];
				if(sInfo.SkillPoint <= g_char_slot_ingame.skill_point && sInfo.GetLearnLevel() <= g_char_slot_ingame.level)
				{
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

					if(bCanLearn )
						gpC_base->m_p_DDSurface_back->BltSprite(&p, &gpC_global_resource->m_pC_info_spk->GetSprite(sprID));
				}
				if(!bCanLearn)
					gpC_base->m_p_DDSurface_back->BltSpriteEffect(&p, &gpC_global_resource->m_pC_info_spk->GetSprite(C_GLOBAL_RESOURCE::OUSTERS_LEARN_BUTTON));
			}
			break;
		}

	}
	else if(p_button->GetFocusState())
	{		
		if(p_button->GetPressState())
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
	}
	else
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index);
	
}

void	C_VS_UI_OUSTERS_SKILL_INFO::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
//	const static char *m_inventory_button_string[6] = 
//	{
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP_GEAR_WINDOW].GetString(),
//			(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_GEAR_WINDOW].GetString(),
//			(*g_pGameStringTable)[UI_STRING_MESSAGE_FIRST_GEAR_SET].GetString(),
//			(*g_pGameStringTable)[UI_STRING_MESSAGE_SECOND_GEAR_SET].GetString(),
//			(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW].GetString(),
//			(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW].GetString(),
//	};	
//	
//	if(p_button->GetID() == ALPHA_ID)
//	{
//		if(GetAttributes()->alpha)
//		{
//			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_inventory_button_string[5],0,0);
//		}
//		else
//		{
//			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_inventory_button_string[p_button->GetID()],0,0);
//		}
//	} else if(p_button->GetID() == FIRST_ID || p_button->GetID() == SECOND_ID)
//	{
//		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_inventory_button_string[p_button->GetID()],0,0);	
//	} else
//	{
//		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_inventory_button_string[p_button->GetID()],0,0);
//	}
}

void	C_VS_UI_OUSTERS_SKILL_INFO::WindowEventReceiver(id_t event)
{
}

void	C_VS_UI_OUSTERS_SKILL_INFO::Run(id_t id)
{
	switch (id)
	{
	case HELP_ID:
		gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::OUSTERS_SKILL_INFO);
		break;
		
	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_OUSTERS_SKILL_INFO);
		break;
		
	case ALPHA_ID:
		AttrAlpha(!GetAttributes()->alpha);
		EMPTY_MOVE;
		break;

	case LEARN_ID:		
		{
			MSkillDomain::SKILLSTATUS status = (*g_pSkillManager)[SKILLDOMAIN_OUSTERS].GetSkillStatus((ACTIONINFO)m_skillID);
			
			SKILLINFO_NODE sInfo = (*g_pSkillInfoTable)[m_skillID];

			if( ! m_bDownSkill )
			{
				if((status == MSkillDomain::SKILLSTATUS_LEARNED && 
					(*g_pSkillInfoTable)[m_skillID].LevelUpPoint <= g_char_slot_ingame.skill_point 
					&& !( m_skillID == SKILL_FIRE_OF_SOUL_STONE || m_skillID == SKILL_ICE_OF_SOUL_STONE || m_skillID == SKILL_SAND_OF_SOUL_STONE || m_skillID == SKILL_BLOCK_HEAD ||
					m_skillID == SKILL_ABSORB_SOUL || m_skillID == SKILL_SUMMON_SYLPH)
					&& sInfo.GetExpLevel() < 30) ||
					status != MSkillDomain::SKILLSTATUS_LEARNED && (*g_pSkillInfoTable)[m_skillID].SkillPoint <= g_char_slot_ingame.skill_point)
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
					
//					if(sInfo.SkillTypeList.empty())
//						bCanLearn = true;
//					else
//					{
//						SKILLINFO_NODE::SKILLTYPE_LIST::iterator itr = sInfo.SkillTypeList.begin();
//						SKILLINFO_NODE::SKILLTYPE_LIST::iterator endItr = sInfo.SkillTypeList.end();
//						
//						while(itr != endItr)
//						{
//							if((*g_pSkillManager)[SKILLDOMAIN_OUSTERS].GetSkillStatus((ACTIONINFO)*itr) == MSkillDomain::SKILLSTATUS_LEARNED)
//							{
//								bCanLearn = true;
//								break;
//							}
//							itr++;
//						}
//					}
					
					if(bCanLearn)
					{
//						if( sInfo.GetExpLevel() == 29 )
//						{
//							g_CurrentSkillID = m_skillID;
//							g_StartComfirmUpToLastSkillLevel( -1, -1 );
//						} 
//						else
						{
							gpC_base->SendMessage(UI_OUSTERS_LEARN_SKILL, m_skillID);
						}
					}
				}
			} else
			{
				if(((status == MSkillDomain::SKILLSTATUS_LEARNED && (*g_pSkillInfoTable)[m_skillID].GetExpLevel() > 1 ) 
					||((*g_pSkillInfoTable)[m_skillID].CanDelete && (*g_pSkillInfoTable)[m_skillID].GetExpLevel() == 1 ))
				//	&&(*g_pSkillManager)[SKILLDOMAIN_OUSTERS].IsAvailableDeleteSkill((ACTIONINFO)m_skillID)
					/*&&(*g_pSkillInfoTable)[m_skillID].GetExpLevel() < 30*/ )
				{
					g_CurrentSkillID = m_skillID;
					int PriceRatio = 1;
					if((*g_pSkillInfoTable)[m_skillID].CanDelete && (*g_pSkillInfoTable)[m_skillID].GetExpLevel() == 1 )
						PriceRatio = 5;
					g_StartOustersDownSkill( -1, -1 , PriceRatio);
					//gpC_base->SendMessage( UI_OUSTERS_DOWN_SKILL, m_skillID );
				}
			}
		}
		break;
	}
}

void	C_VS_UI_OUSTERS_SKILL_INFO::Process()
{
	m_pC_button_group->Process();
	ProcessHide();
}

void	C_VS_UI_OUSTERS_SKILL_INFO::SetSkillID(int skillID)
{
	m_skillID = skillID; 

	m_pack_file.SetRAR(RPK_SKILL, RPK_PASSWORD);
	std::string filename = (*g_pSkillInfoTable)[m_skillID].GetName();
	filename += ".txt";

	if(LoadDesc(filename.c_str(), 30, 5, false)==true)
	{
		SetDesc(50, 170, RGB_GRAY, gpC_base->m_chatting_pi);
		m_pC_scroll_bar->SetPosMax(GetDescSize() - GetDescCol() +1);
	}
	
}


///////////////////////////////////////////////////////////////////
//
// Horn
//
///////////////////////////////////////////////////////////////////
C_VS_UI_HORN::C_VS_UI_HORN(int currentZoneID)
{		
	AttrTopmost( true );
	g_RegisterWindow(this);		

	m_SPK.Open(SPK_HORN);
	m_MapSPK.Open(SPK_HORN_MAP);
// add by Sonic 2006.9.26
	if(g_MyFull)
	{
		Set(1024/2-m_SPK.GetWidth(MAIN_WINDOW)/2, 768/2-m_SPK.GetHeight(MAIN_WINDOW)/2, m_SPK.GetWidth(MAIN_WINDOW), m_SPK.GetHeight(MAIN_WINDOW));
	}
	else
	{
		Set(800/2-m_SPK.GetWidth(MAIN_WINDOW)/2, 600/2-m_SPK.GetHeight(MAIN_WINDOW)/2, m_SPK.GetWidth(MAIN_WINDOW), m_SPK.GetHeight(MAIN_WINDOW));
	}

	//�����ư
//	m_pC_button_group = new ButtonGroup(this);

	int close_x = 217, close_y = 242;
	int up_x = 114, up_y = 51;
	int down_x = 114, down_y = 198;

	//�����ư
	m_pC_button_group = new ButtonGroup(this);
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(up_x, up_y, m_SPK.GetWidth(UD_BUTTON), m_SPK.GetHeight(UD_BUTTON), UP_ID, this,UD_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(down_x, down_y, m_SPK.GetWidth(UD_BUTTON), m_SPK.GetHeight(UD_BUTTON), DOWN_ID, this,UD_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_x, close_y, m_SPK.GetWidth(CLOSE_BUTTON), m_SPK.GetHeight(CLOSE_BUTTON), CLOSE_ID, this,CLOSE_BUTTON));

	m_MapX = 44;
	m_MapY = 89;
	m_Cost = 0;
	m_focus = false;

	UI_PORTAL_LIST portalList;
	
	m_CurrentZone = 0;
#ifdef _LIB
	for(int i = 0; i < g_pZone->GetHorn().size(); i++)
	{
		portalList = g_pZone->GetHorn()[i];
		
		UI_PORTAL_LIST::iterator itr = portalList.begin();
		UI_PORTAL_LIST::iterator endItr = portalList.end();
		
		while(itr != endItr)
		{
			if(itr->zone_id == currentZoneID)
			{
				m_CurrentZone = i;
				break;
			}
			itr++;
		}
	}
#endif

//	int zoneNum, portalNum;
//	UI_PORTAL_FLAG portal;
//	UI_PORTAL_LIST portalList;
//
//	ifstream hornInfoFile(FILE_INFO_HORN, ios::binary);
//	hornInfoFile.read((char *)&zoneNum, sizeof(int));
//	for(int zone = 0; zone < zoneNum; zone++)
//	{
//		hornInfoFile.read((char *)&portalNum, sizeof(int));
//		for(int p = 0; p < portalNum; p++)
//		{
//			hornInfoFile.read((char *)&portal.zone_id, sizeof(int));
//			hornInfoFile.read((char *)&portal.x, sizeof(int));
//			hornInfoFile.read((char *)&portal.y, sizeof(int));
//			hornInfoFile.read((char *)&portal.portal_x, sizeof(int));
//			hornInfoFile.read((char *)&portal.portal_y, sizeof(int));
//			portalList.push_back(portal);
//		}
//		m_portal.push_back(portalList);
//		portalList.clear();
//	}
//	hornInfoFile.close();
}

C_VS_UI_HORN::~C_VS_UI_HORN()
{
	g_UnregisterWindow(this);
	
	DeleteNew(m_pC_button_group);
}

void	C_VS_UI_HORN::Start()
{
	PI_Processor::Start();
	AttrPin(true);
	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();

	m_Cost = int(pow(g_char_slot_ingame.level, 1.3) * 100 );
}

void	C_VS_UI_HORN::Finish()
{
	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);	
}

void	C_VS_UI_HORN::Show()
{
	// by Csm  isthai
	RECT rect[4] = 
	{
		{ x+43, y+90, x+143, y+140 },
		{ x+143, y+90,x+243, y+140 },
		{ x+43, y+140, x+143, y+190 },
		{ x+143, y+140, x+243, y+190 }
		
	};
#ifdef _LIB
	UI_PORTAL_LIST portalList = g_pZone->GetHorn()[m_CurrentZone];
#else
	UI_PORTAL_LIST portalList;
#endif
	int num = 0;
	UI_PORTAL_LIST::iterator itr = portalList.begin();
	UI_PORTAL_LIST::iterator endItr = portalList.end();

	if(gpC_base->m_p_DDSurface_back->Lock())
	{  
		m_SPK.BltLocked(x, y, MAIN_WINDOW);
		m_MapSPK.BltLocked(x+m_MapX, y+m_MapY, m_CurrentZone);
		m_pC_button_group->Show();
		
		while(itr != endItr)
		{
			if(itr->zone_id == 9999)	
			{
				itr++;
				continue;
			}

			if( ! g_pSystemAvailableManager->ZoneFiltering( itr->zone_id ))
			{
				
				itr++;
				continue;
			}
				
			if(m_focus == num)
				gpC_global_resource->m_pC_info_spk->BltLocked(x+m_MapX+itr->x-gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::OUSTERS_HORN)/2, y+m_MapY+itr->y-gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::OUSTERS_HORN)+5, C_GLOBAL_RESOURCE::OUSTERS_HORN_HILIGHTED);
			else
				gpC_global_resource->m_pC_info_spk->BltLocked(x+m_MapX+itr->x-gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::OUSTERS_HORN)/2, y+m_MapY+itr->y-gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::OUSTERS_HORN)+5, C_GLOBAL_RESOURCE::OUSTERS_HORN);
			
			num++;	itr++;
		} 
		
		
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	
	// ���� �� ������ 
	num = 0;	itr = portalList.begin();
	while(itr != endItr)
	{
		if(itr->zone_id == 9999)	
		{
			gpC_base->m_p_DDSurface_back->FillRect(&rect[num], 0);
		}
		itr++; num++;
	}
	m_pC_button_group->ShowDescription();
}

bool	C_VS_UI_HORN::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	
	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);
	
	switch(message)
	{
	case M_MOVING:
		if(_x >= m_MapX && _x <= m_MapX+200 && _y >= m_MapY && _y <= m_MapY+100)
		{
#ifdef _LIB
			UI_PORTAL_LIST portalList = g_pZone->GetHorn()[m_CurrentZone];
#else
			UI_PORTAL_LIST portalList ;
#endif
			
			UI_PORTAL_LIST::iterator itr = portalList.begin();
			UI_PORTAL_LIST::iterator endItr = portalList.end();
			int num = 0;
			
			while(itr != endItr)
			{
				if(itr->zone_id == 9999)	
				{
					itr++;
					continue;
				}
				if( !g_pSystemAvailableManager->ZoneFiltering( itr->zone_id ) )
				{
					itr++;
					continue;
				}
				if(_x >= m_MapX+itr->x-5 && _x <= m_MapX+itr->x+5 && _y >= m_MapY+itr->y-5 && _y <= m_MapY+itr->y+5)
				{
					static S_DEFAULT_HELP_STRING flag_string;
					static char flag_temp[50];
					
					flag_string.sz_main_str = g_pZoneTable->Get(itr->zone_id)->Name.GetString();
					flag_string.sz_sub_str = flag_temp;
					wsprintf(flag_temp, (*g_pGameStringTable)[UI_STRING_MESSAGE_ZONEINFO_XY].GetString(), itr->portal_x, itr->portal_y);
					
					g_descriptor_manager.Set(DID_HELP, itr->x+x+m_MapX, itr->y+y-gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::OUSTERS_HORN)+5+m_MapY, (void *)&flag_string);
					
					m_focus = num;
					
					break;
				}
				
				num++;
				itr++;
			}
			
			if(itr == endItr)
				m_focus = -1;
		}
		else
		{
			m_focus = -1;
		}
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if(_x >= m_MapX && _x <= m_MapX+200 && _y >= m_MapY && _y <= m_MapY+100)
		{
#ifdef _LIB
			UI_PORTAL_LIST portalList = g_pZone->GetHorn()[m_CurrentZone];
#else
			UI_PORTAL_LIST portalList ;
#endif
			
			UI_PORTAL_LIST::iterator itr = portalList.begin();
			UI_PORTAL_LIST::iterator endItr = portalList.end();
			
			while(itr != endItr)
			{

				if(_x >= m_MapX+itr->x-5 && _x <= m_MapX+itr->x+5 && _y >= m_MapY+itr->y-5 && _y <= m_MapY+itr->y+5)
				{
					int left = itr->zone_id;
					int right = ((itr->portal_x)<<16) + itr->portal_y;
					gpC_base->SendMessage(UI_SLAYER_PORTAL, left, right);
					break;
				}
				itr++;
			}
		}
		break;
		
	case M_LEFTBUTTON_UP:
		break;
		
	case M_WHEEL_UP:
		Run(UP_ID);
		break;
		
	case M_WHEEL_DOWN:
		Run(DOWN_ID);
		break;
		
	}	
	return true;
}

void	C_VS_UI_HORN::KeyboardControl(UINT message, UINT key, long extra)	
{	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{		
		case VK_ESCAPE :
			gpC_base->SendMessage(UI_CLOSE_HORN);
			break;		
		}
	}
}

bool	C_VS_UI_HORN::IsPixel(int _x,int _y)
{
	return m_SPK.IsPixel(_x-x, _y-y);
}

void	C_VS_UI_HORN::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetFocusState())
	{		
		if(p_button->GetPressState())
			m_SPK.BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index+2);
		else
			m_SPK.BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index+1);
	}
	else
		m_SPK.BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index);
	
}

void	C_VS_UI_HORN::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
	const static char *m_button_string[3] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_HORN].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_PREV_MAP].GetString(),
			(*g_pGameStringTable)[UI_STRING_MESSAGE_NEXT_MAP].GetString(),
	};	
	
	g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_button_string[p_button->GetID()],0,0);
}

void	C_VS_UI_HORN::WindowEventReceiver(id_t event)
{
}

int		C_VS_UI_HORN::GetNext(int map, bool bUp)
{
	for(int i = 1; i < m_MapSPK.GetSize(); i++ )
	{
		int nmap;
		if( bUp )
			nmap = (m_CurrentZone+m_MapSPK.GetSize()-i)%m_MapSPK.GetSize();
		else
			nmap = (m_CurrentZone+i)%m_MapSPK.GetSize();
		
		bool bCheck = true;
#ifdef _LIB
		UI_PORTAL_LIST &portalList = g_pZone->GetHorn()[nmap];
#else
		UI_PORTAL_LIST portalList;
#endif
		
		UI_PORTAL_LIST::iterator itr = portalList.begin();
		UI_PORTAL_LIST::iterator enditr = portalList.end();
		
		while( itr != enditr )
		{
			if( !g_pSystemAvailableManager->ZoneFiltering( itr->zone_id ) )
			{
				bCheck = false;
				break;
			}
			itr++;
		}
		
		if( bCheck ) 
			return nmap;
	}
	
	if( bUp )
		return (map+1)%m_MapSPK.GetSize();
	
	return (m_CurrentZone+m_MapSPK.GetSize()-1)%m_MapSPK.GetSize();
}

void	C_VS_UI_HORN::Run(id_t id)
{
//	g_pSystemAvailableManager->SetOpenDegree(3);

	switch (id)
	{
	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_HORN);
		break;
		
	case UP_ID:
		m_CurrentZone = (m_CurrentZone+m_MapSPK.GetSize()-1)%m_MapSPK.GetSize();
		{
#ifdef _LIB
			UI_PORTAL_LIST portalList = g_pZone->GetHorn()[m_CurrentZone];
#else
			UI_PORTAL_LIST portalList;
#endif
			
			UI_PORTAL_LIST::iterator itr = portalList.begin();
			UI_PORTAL_LIST::iterator endItr = portalList.end();
			bool bCheck = false;
			
			while( itr != endItr )
			{
				if( !g_pSystemAvailableManager->ZoneFiltering( itr->zone_id ) )
				{
					bCheck = true;
					break;					
				}
				itr++;
			}
			if( bCheck )
				m_CurrentZone = GetNext( m_CurrentZone, true );
		}
		break;
		
	case DOWN_ID:
		m_CurrentZone = (m_CurrentZone+1)%m_MapSPK.GetSize();
		{
#ifdef _LIB
			UI_PORTAL_LIST portalList = g_pZone->GetHorn()[m_CurrentZone];
#else
			UI_PORTAL_LIST portalList;
#endif
			
			UI_PORTAL_LIST::iterator itr = portalList.begin();
			UI_PORTAL_LIST::iterator endItr = portalList.end();
			
			bool bCheck = false;
			
			while( itr != endItr )
			{
				if( !g_pSystemAvailableManager->ZoneFiltering( itr->zone_id ) )
				{
					bCheck = true;
					break;
				}
				itr++;
			}
			if( bCheck )
				m_CurrentZone = GetNext( m_CurrentZone, false );
		}
		break;
	}
}

void	C_VS_UI_HORN::Process()
{
	m_pC_button_group->Process();
	ProcessHide();
}

///////////////////////////////////////////////////////////////////
//
// MAILBOX
//
///////////////////////////////////////////////////////////////////
C_VS_UI_MAILBOX::C_VS_UI_MAILBOX()
{		
//	AttrTopmost( true );
	g_RegisterWindow(this);		

	m_SPK.Open(SPK_MAILBOX);
	
	int window_w = 580, window_h = 370;

	Set(g_GameRect.right/2-window_w/2, g_GameRect.bottom/2-window_h/2, window_w, window_h);

	m_listCount = 10;
	m_overcnt = NULL;
	//�����ư
//	m_pC_button_group = new ButtonGroup(this);
	m_pC_button_group = NULL;
	m_pC_scroll_bar = NULL;

	m_ptNewMail.x = 10;
	m_ptNewMail.y = 50;
	m_pC_dialog =NULL;

}

C_VS_UI_MAILBOX::~C_VS_UI_MAILBOX()
{
	DeleteNew(m_pC_button_group);
	DeleteNew(m_pC_scroll_bar);
	if(m_pC_dialog !=NULL)
		DeleteNew(m_pC_dialog);
	g_UnregisterWindow(this);
}

void	C_VS_UI_MAILBOX::CloseMailBoxDialogBox()
{
	if(m_pC_dialog !=NULL)
		DeleteNew(m_pC_dialog);
}

void	C_VS_UI_MAILBOX::Start(TAB_ID tab_id)
{
	int close_x = w-95, close_y = h-50;
	
	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		m_ptTab.x = 75;
		m_ptTab.y = -6;
		break;
		
	case RACE_VAMPIRE:
		m_ptTab.x = 75;
		m_ptTab.y = -9;
		break;
		
	case RACE_OUSTERS:
		m_ptTab.x = 75;
		m_ptTab.y = -3;
		break;
	}

	//�����ư
	m_pC_button_group = new ButtonGroup(this);
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ptTab.x, m_ptTab.y, 62, 18, MAILTAB_ID, this, 0));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ptTab.x+62, m_ptTab.y, 57, 18, HELPTAB_ID, this, 0));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ptTab.x+62+57, m_ptTab.y, 57, 18, MEMOTAB_ID, this, 0));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_x, close_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), CLOSE_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE));
	
	m_currentTab = tab_id;
	m_pC_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(w-30,90,-1,200));
	//m_pC_scroll_bar->SetPosMax(m_mail[tab_id].size()-9);


	m_focusContents = -1;

	PI_Processor::Start();
	AttrPin(false);

	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();

}

void	C_VS_UI_MAILBOX::Finish()
{
	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);	

	DeleteNew(m_pC_button_group);
	DeleteNew(m_pC_scroll_bar);
}

void	C_VS_UI_MAILBOX::Show()
{
	if(IsNewMail())
	{
		if( m_ptNewMail.x >= 10)
			m_ptNewMail.x  = 10;
		else
			m_ptNewMail.x += 10;
		int TempCount = (g_CurrentFrame%8)/4;
		m_SPK.Blt(m_ptNewMail.x/*-TempCount*/, m_ptNewMail.y/*-TempCount*/, HAS_MAIL+TempCount+(m_currentTab-NEW_MAIL_ID)*2);
		if( m_mail[m_currentTab-NEW_MAIL_ID].empty() == false )
		{
			C_VS_UI_MAIL *pLastMail = &m_mail[m_currentTab-NEW_MAIL_ID].back();
			if( pLastMail != NULL )
			{
				COLORREF color = 0;
				switch( m_currentTab )
				{
				case NEW_MAIL_ID:
					color = RGB(100, 255, 100);
					break;
					
				case NEW_HELP_ID:
					color = RGB(255, 255, 0);
					break;
				}
				
				g_PrintColorStr( m_ptNewMail.x +30, m_ptNewMail.y+4, pLastMail->title.GetString(), gpC_base->m_chatting_pi, color );
			}
		}
	}
	else
	{
		// ����Ʈ ��¿�
		if(gpC_base->m_p_DDSurface_back->Lock())
		{
			gpC_global_resource->DrawDialogLocked(x+10, y, w, h, GetAttributes()->alpha);

			int titleSpriteID;
			int tabSpriteID = m_currentTab;
			int contentsSpriteID;

			switch(g_eRaceInterface)
			{
			case RACE_SLAYER:
				tabSpriteID += TAB_MAIL_SLAYER;
				titleSpriteID = TITLE_SLAYER;
				contentsSpriteID = CONTENTS_SLAYER;
				break;

			case RACE_VAMPIRE:
				tabSpriteID += TAB_MAIL_VAMPIRE;
				titleSpriteID = TITLE_VAMPIRE;
				contentsSpriteID = CONTENTS_VAMPIRE;
				break;
				
			case RACE_OUSTERS:
				tabSpriteID += TAB_MAIL_OUSTERS;
				titleSpriteID = TITLE_OUSTERS;
				contentsSpriteID = CONTENTS_OUSTERS;
				break;
			}
			
			if(m_pC_scroll_bar->GetScrollPos() >= 0 )
			{
				m_overcnt = m_pC_scroll_bar->GetScrollPos();
				if(m_overcnt > m_mail[m_currentTab].size()-m_listCount)
					m_overcnt = m_mail[m_currentTab].size()-m_listCount;
			}
			

			const int title_x = w/2-m_SPK.GetWidth(titleSpriteID)/2, title_y = 20;

			m_SPK.BltLocked(x+title_x, y+title_y, titleSpriteID);
			m_SPK.BltLocked(x+m_ptTab.x, y+m_ptTab.y, tabSpriteID);

			// �ȿ� ���� ����Ʈ ���
			m_rtContents.x = w/2-m_SPK.GetWidth(contentsSpriteID)/2;
			m_rtContents.y = 80;
			m_rtContents.w = m_SPK.GetWidth(contentsSpriteID);
			m_rtContents.h = m_SPK.GetHeight(contentsSpriteID);

			gpC_global_resource->DrawInBoxLocked(x+m_rtContents.x-2, y+m_rtContents.y-2, m_rtContents.w+4, m_rtContents.h*(m_listCount+1)+4);
			m_SPK.BltLocked(x+m_rtContents.x, y+m_rtContents.y, contentsSpriteID);

			for(int i = 0; i < m_listCount; i++)
			{
				m_SPK.BltLocked(x+m_rtContents.x, y+m_rtContents.y+m_rtContents.h*(i+1), contentsSpriteID+1);
				
			}

			int line = 0;

			for(int mailIndex = 0; line < m_listCount && mailIndex < m_mail[m_currentTab].size(); mailIndex++)
			{
				if(m_mail[m_currentTab][mailIndex+m_overcnt].show)
				{
					if(m_mail[m_currentTab][mailIndex+m_overcnt].mark == true)
						m_SPK.BltLocked(x+m_rtContents.x+3, y+m_rtContents.y+2+m_rtContents.h*(line+1), MAIL_OPENED);
					else
						m_SPK.BltLocked(x+m_rtContents.x+3, y+m_rtContents.y+4+m_rtContents.h*(line+1), MAIL_CLOSED);
					line++;
				}
			}

			m_pC_button_group->Show();

			gpC_base->m_p_DDSurface_back->Unlock();
		}
		
		// ���� ����Ʈ ���� ���
		g_FL2_GetDC();

		const int sender_x = 80, title_x = 145, line_plus = 4, date_x = 450;
		
		int line = 0;


		for(int mailIndex = 0; line < m_listCount && mailIndex < m_mail[m_currentTab].size(); mailIndex++)
		{
			if(m_mail[m_currentTab][mailIndex+m_overcnt].show)
			{
				COLORREF color;

				if(line == m_focusContents)
					color = RGB(200, 200, 255);
				else if(m_mail[m_currentTab][mailIndex+m_overcnt].mark == true)
					color = RGB_GRAY;
				else
					color = RGB_WHITE;

				g_PrintColorStr(x+m_rtContents.x+sender_x-g_GetStringWidth(m_mail[m_currentTab][mailIndex+m_overcnt].sender, gpC_base->m_chatting_pi.hfont)/2, y+m_rtContents.y+line_plus+m_rtContents.h*(line+1), m_mail[m_currentTab][mailIndex+m_overcnt].sender, gpC_base->m_chatting_pi, color);
				g_PrintColorStr(x+m_rtContents.x+title_x, y+m_rtContents.y+line_plus+m_rtContents.h*(line+1), m_mail[m_currentTab][mailIndex+m_overcnt].title, gpC_base->m_chatting_pi, color);
				g_PrintColorStr(x+m_rtContents.x+date_x, y+m_rtContents.y+line_plus+m_rtContents.h*(line+1), m_mail[m_currentTab][mailIndex+m_overcnt].date, gpC_base->m_chatting_pi, color);
				line++;
			}
		}
		
		g_FL2_ReleaseDC();
		if(m_mail[m_currentTab].size()>m_listCount)
		{
			m_pC_scroll_bar->Show(x,y);
		}
		m_pC_button_group->ShowDescription();
	}
}

bool	C_VS_UI_MAILBOX::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	_x-=x; _y-=y;

	if(IsNewMail())
	{
		switch(message)
		{
		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
			m_currentTab = (TAB_ID)(m_currentTab - NEW_MAIL_ID);
			m_pC_scroll_bar->SetPosMax(m_mail[m_currentTab].size()-(m_listCount-1));
			break;
		}
	}
	else
	{
		bool re = m_pC_button_group->MouseControl(message, _x, _y);
	//	re &= m_pC_scroll_bar->MouseControl(message, _x, _y);

		if(_x > m_rtContents.x && _x < m_rtContents.x+m_rtContents.w &&
			_y > m_rtContents.y+m_rtContents.h && _y < m_rtContents.y+m_rtContents.h*(m_listCount+1))
		{
			m_focusContents = (_y-(m_rtContents.y+m_rtContents.h))/m_rtContents.h;

			// ������ �Ѿ�� -1
			int indexSize = 0;
			for(int mailIndex = 0; mailIndex < m_mail[m_currentTab].size(); mailIndex++)
			{
				if(m_mail[m_currentTab][mailIndex].show == true)
					indexSize++;
			}

			if(m_focusContents >= indexSize)
				m_focusContents = -1;
		}
		else
			m_focusContents = -1;

		switch(message)
		{
		case M_MOVING:
			break;

		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
//			m_pC_scroll_bar->SetPosMax(m_mail[m_currentTab].size()-(m_listCount-1));
			if(m_focusContents != -1)
			{
				int index = 0;
				for(int mailIndex = 0; mailIndex < m_mail[m_currentTab].size(); mailIndex++)
				{
					if(m_mail[m_currentTab][mailIndex].show == true)
					{
						if(index == m_focusContents)
						{
							int ID = m_mail[m_currentTab][mailIndex+m_overcnt].id;
							ReadMail(m_currentTab, ID);
							break;
						}
						index++;
					}
				}

			}
			else
			{
				if (re)
				{
					MoveReady();
					SetOrigin(_x, _y);
					break;
				}
			}
			break;

		case M_LEFTBUTTON_UP:
			break;

		case M_WHEEL_UP:
			if( m_pC_scroll_bar != NULL )
				m_pC_scroll_bar->ScrollUp();
			break;
			
		case M_WHEEL_DOWN:
			if( m_pC_scroll_bar != NULL )
				m_pC_scroll_bar->ScrollDown();
			break;
			
		}
	}
	return true;
}

void	C_VS_UI_MAILBOX::KeyboardControl(UINT message, UINT key, long extra)	
{	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{		
		case VK_ESCAPE :
			gpC_base->SendMessage(UI_CLOSE_MAILBOX);
			break;		
		}
	}
}

bool	C_VS_UI_MAILBOX::IsPixel(int _x,int _y)
{
	bool re = false;
	if(IsNewMail())
	{
		if(_x > m_ptNewMail.x && _x < m_ptNewMail.x+m_SPK.GetWidth(HAS_MAIL) &&
			_y > m_ptNewMail.y && _y < m_ptNewMail.y+m_SPK.GetHeight(HAS_MAIL))
			re = true;
		else
			re = false;
	}
	else
	{
		re = IsInRect(_x, _y);
		if(re == false)
			re = m_SPK.IsPixel(_x-m_ptTab.x-x, _y-m_ptTab.y-y, TAB_MAIL_OUSTERS);
	}

	return re;
}

void	C_VS_UI_MAILBOX::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetID() != MAILTAB_ID && p_button->GetID() != HELPTAB_ID && p_button->GetID() != MEMOTAB_ID)
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
			else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
		}
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
	}
//	if(p_button->GetFocusState())
//	{		
//		if(p_button->GetPressState())
//			m_SPK.BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index+2);
//		else
//			m_SPK.BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index+1);
//	}
//	else
//		m_SPK.BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index);
	
}

void	C_VS_UI_MAILBOX::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
	const static char *m_button_string[4] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_MAILBOX].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_MAILBOX_TAB_MAIL].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_MAILBOX_TAB_HELP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_MAILBOX_TAB_MEMO].GetString(),
	};	
	
	g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_button_string[p_button->GetID()],0,0);
}

void	C_VS_UI_MAILBOX::WindowEventReceiver(id_t event)
{
}


void	C_VS_UI_MAILBOX::Run(id_t id)
{
	switch (id)
	{
	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_MAILBOX);
		break;

	case UP_ID:
		break;

	case DOWN_ID:
		break;

	case MAILTAB_ID:
		m_currentTab = TAB_MAIL_ID;
		m_pC_scroll_bar->SetPosMax(m_mail[m_currentTab].size()-(m_listCount-1));
		break;

	case HELPTAB_ID:
		m_currentTab = TAB_HELP_ID;
		m_pC_scroll_bar->SetPosMax(m_mail[m_currentTab].size()-(m_listCount-1));
		break;

	case MEMOTAB_ID:
//		m_currentTab = TAB_MEMO_ID;
		break;

	}
}

void	C_VS_UI_MAILBOX::Process()
{
	m_pC_button_group->Process();
}

void	C_VS_UI_MAILBOX::AddHelpMail(DWORD id, bool open)
{
	if(MHelpMessageManager::Instance().getMessageSize() <= id)
		return;
	MAILVECTOR_TYPE::iterator itr = m_mail[TAB_HELP_ID].begin();
	MAILVECTOR_TYPE::iterator endItr = m_mail[TAB_HELP_ID].end();

	// ���� ������ ���� �׳� ����..
	while(itr != endItr)
	{
		if(itr->id == id)
		{
			return;
		}
		itr++;
	}
	
//	MHelpMessageManager::Instance().LoadHelpMessageRpk("helpmessage.txt");
	const MHelpMessage& message = MHelpMessageManager::Instance().getMessage(id);

	
	// ���� üũ
	switch(g_eRaceInterface)
	{
		case RACE_SLAYER: // �ɷ�ġ ������ üũ
			if(message.m_iAttrLow[RACE_SLAYER] != -1)
			{
				int Attr_sum = g_char_slot_ingame.STR_CUR + g_char_slot_ingame.DEX_CUR + g_char_slot_ingame.INT_CUR;
				if(Attr_sum < message.m_iAttrLow[RACE_SLAYER] || Attr_sum > message.m_iAttrLow[RACE_SLAYER])
					return;
			}
//			if(message.m_iLevelLow[RACE_SLAYER] != -1) // ������ üũ
//			{
//				int Lead_Lev = max(g_char_slot_ingame.DOMAIN_SWORD, max(g_char_slot_ingame.DOMAIN_BLADE, max(g_char_slot_ingame.DOMAIN_GUN, max(g_char_slot_ingame.DOMAIN_HEAL, g_char_slot_ingame.DOMAIN_ENCHANT))));
//				if(Lead_Lev < message.m_iAttrLow[RACE_SLAYER] || Lead_Lev > message.m_iLevelMax[RACE_SLAYER])
//					return;
//			}
			break;
		case RACE_VAMPIRE: // ������ üũ
			if(message.m_iLevelLow[RACE_VAMPIRE] != -1)
			{
				if(g_char_slot_ingame.level < message.m_iLevelLow[RACE_VAMPIRE] || g_char_slot_ingame.level > message.m_iLevelMax[RACE_VAMPIRE])
					return;
			}
			break;
		case RACE_OUSTERS: // ������ üũ
			if(message.m_iLevelLow[RACE_OUSTERS] != -1)
			{
				if(g_char_slot_ingame.level < message.m_iLevelLow[RACE_OUSTERS] || g_char_slot_ingame.level > message.m_iLevelMax[RACE_OUSTERS])
					return;
			}
			break;
	}

	int nSender = message.m_iSender[g_eRaceInterface];
	int nMaxSenderSize = MHelpMessageManager::Instance().getSenderSize();
	MString  strSender;
	
	if(nSender<0 || nSender >= nMaxSenderSize)
	{
		// ���� ��� �Է� ���߰ų�..���� �̻� �� �� 
		strSender = "";  
	}
	else
	{
		strSender = MHelpMessageManager::Instance().getSender(nSender).GetString();
	}

	C_VS_UI_MAIL mail;
	mail.id = id;
	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		mail.sender = strSender;
		mail.title = message.m_strTitle[g_eRaceInterface];
		mail.contents = message.m_strDetail[g_eRaceInterface];
		break;
		
	case RACE_VAMPIRE:
		mail.sender = strSender;
		mail.title = message.m_strTitle[g_eRaceInterface];
		mail.contents = message.m_strDetail[g_eRaceInterface];
		break;
		
	case RACE_OUSTERS:
		mail.sender = strSender;
		mail.title = message.m_strTitle[g_eRaceInterface];
		mail.contents = message.m_strDetail[g_eRaceInterface];
		break;
	}
	if(!mail.title.GetLength()) //������ ������...����..
		return;
	mail.show = true;
	mail.mark = false;
	mail.open = open;

#ifdef _LIB
	char szData[32];
	sprintf(szData, "%d/%d/%d",	g_pGameTime->GetYear(),
									g_pGameTime->GetMonth(),
									g_pGameTime->GetDay());
	mail.date = szData;
#endif

	m_mail[TAB_HELP_ID].push_back(mail);

	
	Start((TAB_ID)(NEW_MAIL_ID+TAB_HELP_ID));

	m_ptNewMail.x = -400;
	if( open == true )
	{
		Finish();
		ReadMail(TAB_HELP_ID, id);
	}
}


void	C_VS_UI_MAILBOX::AddMail(TAB_ID tab_id, DWORD id, SIZE windowSize, const char * sender, const char * title, DWORD date, bool open, int guildid ,int guildtype)
{
	char szTemp[512];
	std::string dateString;
	sprintf(szTemp, "%d", date/100);
	
	// �տ� �⵵�� ���� 03 �̷������� ���� �տ� 0�� ���µ� ����Ҷ� 03�� �̷��� ������ �ϹǷ� �⵵�� ���ڸ����� �տ� 0�� �ٿ��ش�.
//	if(date < 10000000)
//		// �⵵�� ���ڸ��϶�
//	{
//		dateString = "0";
//	}
	if(strlen(szTemp) < 6)
		dateString = "0";

	

	dateString += szTemp;
	dateString.insert(2, "/");
	dateString.insert(5, "/");
	
	MAILVECTOR_TYPE::iterator itr = m_mail[tab_id].begin();
	MAILVECTOR_TYPE::iterator endItr = m_mail[tab_id].end();
	
	while(itr != endItr)
	{
		if(itr->id == id)
		{
			// ��¥�� �޶�
			if(strcmp(itr->date.GetString(), dateString.c_str()) != 0)
			{
				itr->date = dateString.c_str();
				itr->mark = false;
				itr->open = open;

				Start((TAB_ID)(NEW_MAIL_ID+tab_id));
			}
			itr->show = true;
			itr->open = false;
			return;
		}
		itr++;
	}

	C_VS_UI_MAIL mail;
	
	mail.id = id;
	mail.windowSize = windowSize;
	mail.sender = sender;
	mail.title = title;
	mail.date = dateString.c_str();
	mail.show = true;
	mail.mark = false;
	mail.open = open;
	mail.guild_id = guildid;
	mail.guild_type = guildtype;
	m_mail[tab_id].push_back(mail);

	m_ptNewMail.x = -400;
	
	Start((TAB_ID)(NEW_MAIL_ID+tab_id));

}

//void	C_VS_UI_MAILBOX::AddMail(TAB_ID tab_id, DWORD id, SIZE windowSize, const char * sender, const char * title, DWORD date, bool open)
//{
//	char szTemp[512];
//	std::string dateString;
//	sprintf(szTemp, "%d", date/100);
//	
//	// �տ� �⵵�� ���� 03 �̷������� ���� �տ� 0�� ���µ� ����Ҷ� 03�� �̷��� ������ �ϹǷ� �⵵�� ���ڸ����� �տ� 0�� �ٿ��ش�.
////	if(date < 10000000)
////		// �⵵�� ���ڸ��϶�
////	{
////		dateString = "0";
////	}
//	if(strlen(szTemp) < 6)
//		dateString = "0";
//
//	
//
//	dateString += szTemp;
//	dateString.insert(2, "/");
//	dateString.insert(5, "/");
//	
//	MAILVECTOR_TYPE::iterator itr = m_mail[tab_id].begin();
//	MAILVECTOR_TYPE::iterator endItr = m_mail[tab_id].end();
//	
//	while(itr != endItr)
//	{
//		if(itr->id == id)
//		{
//			// ��¥�� �޶�
//			if(strcmp(itr->date.GetString(), dateString.c_str()) != 0)
//			{
//				itr->date = dateString.c_str();
//				itr->mark = false;
//				itr->open = open;
//
//				Start((TAB_ID)(NEW_MAIL_ID+tab_id));
//			}
//			itr->show = true;
//			itr->open = false;
//			return;
//		}
//		itr++;
//	}
//
//	C_VS_UI_MAIL mail;
//	
//	mail.id = id;
//	mail.windowSize = windowSize;
//	mail.sender = sender;
//	mail.title = title;
//	mail.date = dateString.c_str();
//	mail.show = true;
//	mail.mark = false;
//	mail.open = open;
//
//	m_mail[tab_id].push_back(mail);
//
//	m_ptNewMail.x = -400;
//	
//	Start((TAB_ID)(NEW_MAIL_ID+tab_id));
//
//}

void	C_VS_UI_MAILBOX::AddMailContents(TAB_ID tab_id, DWORD id, const char * contents)
{
	MAILVECTOR_TYPE::iterator itr = m_mail[tab_id].begin();
	MAILVECTOR_TYPE::iterator endItr = m_mail[tab_id].end();

	while(itr != endItr)
	{
		if(itr->id == id)
		{
			itr->contents = contents;
			if(itr->open == true)
			{
				Finish();
				ReadMail(tab_id, id);
			}
			break;
		}
		itr++;
	}

}

void	C_VS_UI_MAILBOX::DeleteMail(TAB_ID tab_id, DWORD id)
{
	MAILVECTOR_TYPE::iterator itr = m_mail[tab_id].begin();
	MAILVECTOR_TYPE::iterator endItr = m_mail[tab_id].end();
	
	while(itr != endItr)
	{
		if(itr->id == id)
		{
			itr->show = false;
			break;
		}
		itr++;
	}
}

int	C_VS_UI_MAILBOX::m_cur_id;
int	C_VS_UI_MAILBOX::m_guild_id;

void ExecF_Union_Joinask(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	if(id == 0)
	{
			gpC_base->SendMessage(UI_REQUEST_UNION_ACCEPT, C_VS_UI_MAILBOX::GetGuildId(), 0);
			gC_vs_ui.DeleteMail(0, C_VS_UI_MAILBOX::GetCurId());
			gC_vs_ui.CloseAllDialog();
	}
	if(id == 1)
	{
			gpC_base->SendMessage(UI_REQUEST_UNION_DENY, C_VS_UI_MAILBOX::GetGuildId(), 0);
			gC_vs_ui.DeleteMail(0, C_VS_UI_MAILBOX::GetCurId());
			gC_vs_ui.CloseAllDialog();
	}
}	

void ExecF_Union_Quitask(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{
	if(id == 0)
	{
			gpC_base->SendMessage(UI_REQUEST_UNION_QUIT_ACCEPT, C_VS_UI_MAILBOX::GetGuildId(), 0);
			gC_vs_ui.DeleteMail(0, C_VS_UI_MAILBOX::GetCurId());
			gC_vs_ui.CloseAllDialog();
	}
	
	if(id == 1)
	{
			gpC_base->SendMessage(UI_REQUEST_UNION_QUIT_DENY, C_VS_UI_MAILBOX::GetGuildId(), 0);
			gC_vs_ui.DeleteMail(0, C_VS_UI_MAILBOX::GetCurId());
			gC_vs_ui.CloseAllDialog();
	}
}	


void	C_VS_UI_MAILBOX::ReadMail(TAB_ID tabID, DWORD id)
{
	MAILVECTOR_TYPE::iterator itr = m_mail[tabID].begin();
	MAILVECTOR_TYPE::iterator endItr = m_mail[tabID].end();

	//by csm ���� �о�帰 id �� �ܺο��� �ʿ��ؼ� 
	SetCurId(id);
	DIALOG_MENU d_menu[] = {	{(*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_JOIN_ACCEPT].GetString(), 0},
								{(*g_pGameStringTable)[UI_STRING_MESSAGE_TEAM_INFO_JOIN_DENY].GetString(), 1},
								{(*g_pGameStringTable)[UI_STRING_MESSAGE_CANCEL].GetString(), DIALOG_EXECID_EXIT},
	};

	DIALOG_MENU d_menu2[] = {	{(*g_pGameStringTable)[UI_STRING_MESSAGE_TOTAL_GUILD_LEAVE_ACCEPT].GetString(), 0},
								{(*g_pGameStringTable)[UI_STRING_MESSAGE_TOTAL_GUILD_LEAVE_DENY].GetString(), 1},
								{(*g_pGameStringTable)[UI_STRING_MESSAGE_CANCEL].GetString(), DIALOG_EXECID_EXIT},
	};


	static char * pp_dmsg_union[] = { (*g_pGameStringTable)[UI_STRING_MESSAGE_TOTAL_GUILD_JOIN_ASK].GetString() };
	static char * pp_dmsg_union2[] = { (*g_pGameStringTable)[UI_STRING_MESSAGE_TOTAL_GUILD_LEAVE_ASK].GetString() };

	
	while(itr != endItr)
	{
		if(itr->id == id)
		{
			itr->mark = true;

			if(itr->contents.GetLength() == 0)
			{
				// �޼��� ������
			}
			else
			{
				if(tabID == TAB_HELP_ID) //HelPMessage ���
				{
					//gC_vs_ui.RunHelpDesc(m_mail[tabID][id].contents ,m_mail[tabID][id].title , id);
					gC_vs_ui.RunHelpDesc((itr)->contents,(itr)->title, id);
				}
				else
				{
					
					C_VS_UI_POPUP_MESSAGE::PopupWindowStyle style;
					if(id == 1 || id == 4 || id == 6)
					{
						style.Style = C_VS_UI_POPUP_MESSAGE::STYLE_JOIN_BUTTON | C_VS_UI_POPUP_MESSAGE::STYLE_ABSENCE_BUTTON;
					}
					else
					{
						style.Style = C_VS_UI_POPUP_MESSAGE::STYLE_CANCEL_BUTTON;
					}
					
					style.WindowX = itr->windowSize.cx;
					style.WindowY = itr->windowSize.cy;
					
					
					if(id != 9)
					{
						style.Type = C_VS_UI_POPUP_MESSAGE::POPUP_NORMAL;
						style.str = itr->contents;
						style.SendID = itr->id;
			
						m_focusContents = -1;
						gC_vs_ui.RunPopupMessage( style );
					} 
					else
					{
						
						SetGuildId(itr->guild_id);

						if(itr->guild_type == 0)
						{
							m_pC_dialog = new C_VS_UI_NPC_DIALOG(ExecF_Union_Joinask, NULL, NULL, 500, 250);
							m_pC_dialog->SetMenu(d_menu, 3, false);
							m_pC_dialog->SetMessage(pp_dmsg_union,sizeof(pp_dmsg_union)/sizeof(char *));//, SMO_NOFIT);
							m_pC_dialog->Start();
						}
						else if(itr->guild_type  == 1)
						{
							m_pC_dialog = new C_VS_UI_NPC_DIALOG(ExecF_Union_Quitask, NULL, NULL, 500, 250);
							m_pC_dialog->SetMenu(d_menu2, 3, false);
							m_pC_dialog->SetMessage(pp_dmsg_union2,sizeof(pp_dmsg_union)/sizeof(char *));//, SMO_NOFIT);
							m_pC_dialog->Start();
						}
					}

				}
			}

			break;
		}
		itr++;
	}
}

C_VS_UI_MAILBOX::C_VS_UI_MAIL::C_VS_UI_MAIL()
{
	id = NULL;
	mark = false;
	show = false;
	open = false;
	windowSize.cx = -1;
	windowSize.cy = -1;
}

C_VS_UI_MAILBOX::C_VS_UI_MAIL::~C_VS_UI_MAIL()
{
}


void C_VS_UI_MAILBOX::C_VS_UI_MAIL::SaveToFile(std::ofstream &file)
{
	file.write((const char *)&id, sizeof(DWORD));
	file.write((const char *)&mark, sizeof(bool));
	sender.SaveToFile(file);
	title.SaveToFile(file);
	contents.SaveToFile(file);
	date.SaveToFile(file);
	file.write((const char *)&windowSize, sizeof(SIZE));
}

bool C_VS_UI_MAILBOX::C_VS_UI_MAIL::LoadFromFile(std::ifstream &file)
{
	file.read((char *)&id, sizeof(DWORD));
	file.read((char *)&mark, sizeof(bool));
	sender.LoadFromFile(file);
	title.LoadFromFile(file);
	contents.LoadFromFile(file);
	date.LoadFromFile(file);
	file.read((char *)&windowSize, sizeof(SIZE));
	if(id == 9) // ���Ͽ� ��忡 ���ѰŸ� �ε��ʴ´�.
		return false;
	return true;
}

void C_VS_UI_MAILBOX::C_VS_UI_MAIL::SaveToFile(std::string filename)
{
	ofstream file(filename.c_str(), ios::binary);
	
	if(file)
	{
		SaveToFile(file);
		file.close();
	}

}

void C_VS_UI_MAILBOX::C_VS_UI_MAIL::LoadFromFile(std::string filename)
{
	ifstream file(filename.c_str(), ios::binary);
	
	if(file)
	{
		LoadFromFile(file);
		file.close();
	}
}

void C_VS_UI_MAILBOX::SaveToFile(std::ofstream &file)
{
	DWORD flag = 0;
	file.write((const char *)&flag, sizeof(DWORD));

	BYTE tabCount = TAB_MEMO_ID;
	file.write((const char *)&tabCount, sizeof(BYTE));

	for(int tab = 0; tab < tabCount; tab++)
	{
		DWORD mailCount = m_mail[tab].size();
		file.write((const char *)&mailCount, sizeof(DWORD));

		MAILVECTOR_TYPE::iterator itr = m_mail[tab].begin();
		MAILVECTOR_TYPE::iterator endItr = m_mail[tab].end();

		while(itr != endItr)
		{
			itr->SaveToFile(file);
			itr++;
		}
	}
}

void C_VS_UI_MAILBOX::LoadFromFile(std::ifstream &file)
{
	DWORD flag = 0;
	file.read((char *)&flag, sizeof(DWORD));
	
	BYTE tabCount;
	file.read((char *)&tabCount, sizeof(BYTE));
	
	for(int tab = 0; tab < tabCount; tab++)
	{
		DWORD mailCount;
		file.read((char *)&mailCount, sizeof(DWORD));
		
		C_VS_UI_MAIL tempMail;
		for(int mail = 0; mail < mailCount; mail++)
		{
			if(tempMail.LoadFromFile(file))
			{
				if( tab == TAB_HELP_ID )
					tempMail.show = true;
				m_mail[tab].push_back(tempMail);
			}
		}
	}
}

void C_VS_UI_MAILBOX::SaveToFile(std::string filename)
{
	ofstream file(filename.c_str(), ios::binary);
	
	if(file)
	{
		SaveToFile(file);
		file.close();
	}
	
}

void C_VS_UI_MAILBOX::LoadFromFile(std::string filename)
{
	ifstream file(filename.c_str(), ios::binary);
	
	if(file)
	{
		LoadFromFile(file);
		file.close();
	}
}

///////////////////////////////////////////////////////////////////
//
// FINDING MINE
//
///////////////////////////////////////////////////////////////////
C_VS_UI_FINDING_MINE::C_VS_UI_FINDING_MINE()
{		
	g_RegisterWindow(this);		

// 	m_SPK.Open(SPK_HORN);
	
	//�����ư
	m_pC_button_group = new ButtonGroup(this);

	int close_x = 217, close_y = 242;
	int start_x = 30, start_y = BLOCK_START_Y-41;
;

	//�����ư
	m_pC_button_group = new ButtonGroup(this);
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(start_x, start_y, 26, 26, START_ID, this, 0));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(BLOCK_START_X, BLOCK_START_X, 42, 26, LEVEL0_ID, this, 0));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(BLOCK_START_X+52, BLOCK_START_X, 42, 26, LEVEL1_ID, this, 0));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(BLOCK_START_X+52*2, BLOCK_START_X, 42, 26, LEVEL2_ID, this, 0));
//	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_x, close_y, m_SPK.GetWidth(CLOSE_BUTTON), m_SPK.GetHeight(CLOSE_BUTTON), CLOSE_ID, this,CLOSE_BUTTON));
	m_startTime = 0;
	m_Blocks = NULL;
	bLButton = false;
	bRButton = false;

	NewGame(GAMELEVEL_0);
}

void	C_VS_UI_FINDING_MINE::NewGame(GAMELEVEL level)
{
	m_level = level;

	int levelMineCount[GAMELEVEL_MAX] = 
	{
		LEFTMINE_LEVEL0,
		LEFTMINE_LEVEL1,
		LEFTMINE_LEVEL2,
	};

	int levelBoardWidth[GAMELEVEL_MAX] =
	{
		BOARDSIZE_LEVEL0_WIDTH,
		BOARDSIZE_LEVEL1_WIDTH,
		BOARDSIZE_LEVEL2_WIDTH,
	};

	int levelBoardHeight[GAMELEVEL_MAX] =
	{
		BOARDSIZE_LEVEL0_HEIGHT,
		BOARDSIZE_LEVEL1_HEIGHT,
		BOARDSIZE_LEVEL2_HEIGHT,
	};
	
	m_leftMineCount = levelMineCount[level];
	m_boardSize.cx = levelBoardWidth[level];
	m_boardSize.cy = levelBoardHeight[level];

	m_status = GAMESTATUS_READY;

	int window_w = m_boardSize.cx*BLOCK_WIDTH+BLOCK_START_X*2, window_h = m_boardSize.cy*BLOCK_HEIGHT+BLOCK_START_X+BLOCK_START_Y;
	// add by Sonic 2006.9.26
	if(g_MyFull)
	{
		Set(1024/2-window_w/2, 768/2-window_h/2, window_w, window_h);
	}
	else
	{
		Set(800/2-window_w/2, 600/2-window_h/2, window_w, window_h);
	}
	// end

	DeleteNewArray(m_Blocks);
	m_Blocks = new BLOCK_STRUCT[m_boardSize.cx*m_boardSize.cy];

	m_BlockRect.Set(BLOCK_START_X, BLOCK_START_Y, m_boardSize.cx*BLOCK_WIDTH, m_boardSize.cy*BLOCK_HEIGHT);

	Button *pButton = m_pC_button_group->GetButton(START_ID);
	pButton->x = w/2-pButton->w/2;

	srand(timeGetTime());
	
	int i, j, k;

	for(i = 0; i < m_leftMineCount; i++)
	{
		int mineX = rand()%m_boardSize.cx;
		int mineY = rand()%m_boardSize.cy;
		if(m_Blocks[mineY*m_boardSize.cx+mineX].num == -1)
		{
			i--;
			continue;
		}

		m_Blocks[mineY*m_boardSize.cx+mineX].num = -1;
		for(j = max(0, mineY-1); j <= min(m_boardSize.cy-1, mineY+1); j++)
		{
			for(k = max(0, mineX-1); k <= min(m_boardSize.cx-1, mineX+1); k++)
			{
				if(m_Blocks[j*m_boardSize.cx+k].num != -1)
					m_Blocks[j*m_boardSize.cx+k].num++;
			}
		}

	}

}

C_VS_UI_FINDING_MINE::~C_VS_UI_FINDING_MINE()
{
	g_UnregisterWindow(this);
	
	DeleteNew(m_pC_button_group);
	DeleteNewArray(m_Blocks);
}

void	C_VS_UI_FINDING_MINE::Start()
{
	PI_Processor::Start();
	AttrPin(true);
	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();

}

void	C_VS_UI_FINDING_MINE::Finish()
{
	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);	
}

void	C_VS_UI_FINDING_MINE::Show()
{
	// �⺻ ��� �����
	RECT rect = {x, y, x+w, y+h};
	gpC_base->m_p_DDSurface_back->FillRect(&rect, GRAY);

	DrawRect(rect, 3, WHITE, DARKGRAY, GRAY);
	
	rect.left = x+BLOCK_START_X-3;
	rect.top = y+BLOCK_START_Y-3;
	rect.right = x+w-BLOCK_START_X+3;
	rect.bottom = y+h-BLOCK_START_X+3;
	DrawRect(rect, 3, DARKGRAY, WHITE, GRAY);

	rect.top = y+BLOCK_START_Y-47;
	rect.bottom = rect.top + 37;
	DrawRect(rect, 2, DARKGRAY, WHITE, GRAY);

	// number
	rect.left = x+16;
	rect.top = y+BLOCK_START_Y-41;
	rect.right = rect.left+41;
	rect.bottom = rect.top+25;
	DrawRect(rect, 1, DARKGRAY, WHITE, GRAY);
	DrawNumbers(rect.left+1, rect.top+1, m_leftMineCount);

	rect.right = x+w-16;
	rect.top = y+BLOCK_START_Y-41;
	rect.left = rect.right-41;
	rect.bottom = rect.top+25;
	DrawRect(rect, 1, DARKGRAY, WHITE, GRAY);

	if(m_status == GAMESTATUS_GAME)
	{
		DrawNumbers(rect.left+1, rect.top+1, (timeGetTime()-m_startTime)/1000);
	}
	else if(m_status == GAMESTATUS_DIE || m_status == GAMESTATUS_CLEAR)
	{
		DrawNumbers(rect.left+1, rect.top+1, m_startTime/1000);
	}
	else
	{
		DrawNumbers(rect.left+1, rect.top+1, 0);
	}
	
	// draw blocks
	for(int i = 0; i < m_boardSize.cy; i++)
	{
		for(int j = 0; j < m_boardSize.cx; j++)
		{
			DrawBlock(x+BLOCK_START_X+j*BLOCK_WIDTH, y+BLOCK_START_Y+i*BLOCK_HEIGHT, m_Blocks[i*m_boardSize.cx+j]);
		}
	}

	m_pC_button_group->Show();
//	m_pC_button_group->ShowDescription();
}

bool	C_VS_UI_FINDING_MINE::MouseControl(UINT message, int _x, int _y)
{
	static POINT lastPoint = {-1, -1};

	Window::MouseControl(message, _x, _y);

	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);

	switch(message)
	{
	case M_MOVING:
		if(bLButton == true && bRButton == true)
		{
			ClearBlockFocus(lastPoint.x-1, lastPoint.y-1, 3, 3);
			if(m_BlockRect.IsInRect(_x, _y))
			{
				lastPoint.x = (_x-m_BlockRect.x)/BLOCK_WIDTH;
				lastPoint.y = (_y-m_BlockRect.y)/BLOCK_HEIGHT;

				for(int i = max(0, lastPoint.y-1); i <= min(m_boardSize.cy-1, lastPoint.y+1); i++)
				{
					for(int j = max(0, lastPoint.x-1); j <= min(m_boardSize.cx-1, lastPoint.x+1); j++)
					{
						m_Blocks[i*m_boardSize.cx+j].bl_focused = true;
					}
				}
			}
		}
		else
		if(bLButton == true)
		{
			ClearBlockFocus(lastPoint.x, lastPoint.y, 1, 1);
			if(m_BlockRect.IsInRect(_x, _y))
			{
				lastPoint.x = (_x-m_BlockRect.x)/BLOCK_WIDTH;
				lastPoint.y = (_y-m_BlockRect.y)/BLOCK_HEIGHT;
				m_Blocks[lastPoint.y*m_boardSize.cx+lastPoint.x].bl_focused = true;
			}
		}
		break;

	case M_LEFTBUTTON_DOWN:
		if(m_BlockRect.IsInRect(_x, _y))
		{
			bLButton = true;
			EMPTY_MOVE;
		}
	case M_LB_DOUBLECLICK:
		break;

	case M_LEFTBUTTON_UP:
		if(m_BlockRect.IsInRect(_x, _y))
		{
			if(bRButton == true)
			{
				lastPoint.x = (_x-m_BlockRect.x)/BLOCK_WIDTH;
				lastPoint.y = (_y-m_BlockRect.y)/BLOCK_HEIGHT;
				OpenBlock(lastPoint.x, lastPoint.y, true);
			}
			else
			{
				if(m_Blocks[lastPoint.y*m_boardSize.cx+lastPoint.x].bl_focused == true)
					OpenBlock(lastPoint.x, lastPoint.y);
			}
			
			if(bRButton == true)
			{
				ClearBlockFocus(lastPoint.x-1, lastPoint.y-1, 3, 3);
			}
			else
			{
				ClearBlockFocus(lastPoint.x, lastPoint.y, 1, 1);
			}
			
			if(CheckClear() == true)
			{
				m_status = GAMESTATUS_CLEAR;
				m_startTime = timeGetTime()-m_startTime;
				for(int i = 0; i < m_boardSize.cy; i++)
				{
					for(int j = 0; j < m_boardSize.cx; j++)
					{
						if(m_Blocks[i*m_boardSize.cx+j].num == -1)
							m_Blocks[i*m_boardSize.cx+j].status = BLOCKSTATUS_FLAG;
					}
				}
			}

		}
		bLButton = false;
		break;

	case M_RIGHTBUTTON_DOWN:
		if(m_BlockRect.IsInRect(_x, _y))
		{
			bRButton = true;

			if(bLButton == true)
			{
				EMPTY_MOVE;
			}
			else
			{
				lastPoint.x = (_x-m_BlockRect.x)/BLOCK_WIDTH;
				lastPoint.y = (_y-m_BlockRect.y)/BLOCK_HEIGHT;
				
				BLOCK_STATUS status;
				
				switch(m_Blocks[lastPoint.y*m_boardSize.cx+lastPoint.x].status)
				{
				case BLOCKSTATUS_CLOSE:
					m_leftMineCount--;
					status = BLOCKSTATUS_FLAG;
					break;
					
				case BLOCKSTATUS_FLAG:
					m_leftMineCount++;
					status = BLOCKSTATUS_QUESTION;
					break;
					
				case BLOCKSTATUS_QUESTION:
					status = BLOCKSTATUS_CLOSE;
					break;
					
				default:
					status = BLOCKSTATUS_OPEN;
					break;
				}
				m_Blocks[lastPoint.y*m_boardSize.cx+lastPoint.x].status = status;
			}
		}
		break;

	case M_RIGHTBUTTON_UP:
		if(m_BlockRect.IsInRect(_x, _y))
		{
			bRButton = false;

			if(bLButton == true)
			{
				ClearBlockFocus(lastPoint.x-1, lastPoint.y-1, 3, 3);

				lastPoint.x = (_x-m_BlockRect.x)/BLOCK_WIDTH;
				lastPoint.y = (_y-m_BlockRect.y)/BLOCK_HEIGHT;
				OpenBlock(lastPoint.x, lastPoint.y, true);
			}
		}
		break;

	case M_WHEEL_UP:
		break;
		
	case M_WHEEL_DOWN:
		break;
		
	}	
	return true;
}

void	C_VS_UI_FINDING_MINE::KeyboardControl(UINT message, UINT key, long extra)	
{	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{		
		case VK_ESCAPE :
			gpC_base->SendMessage(UI_CLOSE_FINDING_MINE);
			break;		
		}
	}
}

bool	C_VS_UI_FINDING_MINE::IsPixel(int _x,int _y)
{
	return IsInRect(_x, _y);
}

void	C_VS_UI_FINDING_MINE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	RECT rect = {x+p_button->x, y+p_button->y, x+p_button->x+p_button->w, y+p_button->y+p_button->h};

	DrawRect(rect, 1, DARKGRAY, DARKGRAY, GRAY);

	int smile_x = x+p_button->x, smile_y = y+p_button->y;

	if(p_button->GetFocusState() && p_button->GetPressState())
	{
		gpC_base->m_p_DDSurface_back->HLine(rect.left+1, rect.top+1, p_button->w-2, DARKGRAY);
//		gpC_base->m_p_DDSurface_back->HLine(rect.left+1, rect.top+2, p_button->w-2, DARKGRAY);
		gpC_base->m_p_DDSurface_back->VLine(rect.left+1, rect.top+1, p_button->h-2, DARKGRAY);
//		gpC_base->m_p_DDSurface_back->VLine(rect.left+2, rect.top+1, p_button->h-2, DARKGRAY);
		smile_x++;
		smile_y++;
	}
	else
	{
		rect.left++;
		rect.top++;
		rect.right--;
		rect.bottom--;
		DrawRect(rect, 2, WHITE, DARKGRAY, GRAY);
	}

	if(p_button->GetID() == START_ID)
	{
		// draw smile
		gpC_base->m_p_DDSurface_back->HLine(smile_x+11, smile_y+5, 5, BLACK);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+9, smile_y+6, 2, BLACK);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+16, smile_y+6, 2, BLACK);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+11, smile_y+6, 5, YELLOW);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+8, smile_y+7, 1, BLACK);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+18, smile_y+7, 1, BLACK);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+9, smile_y+7, 9, YELLOW);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+7, smile_y+8, 1, BLACK);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+19, smile_y+8, 1, BLACK);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+8, smile_y+8, 11, YELLOW);
		gpC_base->m_p_DDSurface_back->VLine(smile_x+6, smile_y+9, 2, BLACK);
		gpC_base->m_p_DDSurface_back->VLine(smile_x+20, smile_y+9, 2, BLACK);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+7, smile_y+9, 13, YELLOW);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+7, smile_y+10, 13, YELLOW);
		
		gpC_base->m_p_DDSurface_back->VLine(smile_x+5, smile_y+11, 5, BLACK);
		gpC_base->m_p_DDSurface_back->VLine(smile_x+21, smile_y+11, 5, BLACK);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+6, smile_y+11, 15, YELLOW);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+6, smile_y+12, 15, YELLOW);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+6, smile_y+13, 15, YELLOW);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+6, smile_y+14, 15, YELLOW);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+6, smile_y+15, 15, YELLOW);

		gpC_base->m_p_DDSurface_back->VLine(smile_x+6, smile_y+16, 2, BLACK);
		gpC_base->m_p_DDSurface_back->VLine(smile_x+20, smile_y+16, 2, BLACK);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+7, smile_y+16, 13, YELLOW);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+7, smile_y+17, 13, YELLOW);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+7, smile_y+18, 1, BLACK);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+19, smile_y+18, 1, BLACK);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+8, smile_y+18, 11, YELLOW);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+8, smile_y+19, 1, BLACK);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+18, smile_y+19, 1, BLACK);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+9, smile_y+19, 9, YELLOW);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+9, smile_y+20, 2, BLACK);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+16, smile_y+20, 2, BLACK);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+11, smile_y+20, 5, YELLOW);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+11, smile_y+21, 5, BLACK);

		// face
		gpC_base->m_p_DDSurface_back->HLine(smile_x+8, smile_y+11, 5, BLACK);
		gpC_base->m_p_DDSurface_back->HLine(smile_x+14, smile_y+11, 5, BLACK);

		switch(m_status)
		{
		case GAMESTATUS_READY:
		case GAMESTATUS_GAME:
			if(bLButton == true)
			{
				gpC_base->m_p_DDSurface_back->VLine(smile_x+12, smile_y+15, 3, BLACK);
				gpC_base->m_p_DDSurface_back->VLine(smile_x+14, smile_y+15, 3, BLACK);
				gpC_base->m_p_DDSurface_back->VLine(smile_x+13, smile_y+14, 1, BLACK);
				gpC_base->m_p_DDSurface_back->VLine(smile_x+13, smile_y+18, 1, BLACK);
			}
			else
			{
				gpC_base->m_p_DDSurface_back->HLine(smile_x+10, smile_y+16, 7, BLACK);
			}
			break;

		case GAMESTATUS_CLEAR:
			gpC_base->m_p_DDSurface_back->HLine(smile_x+13, smile_y+11, 3, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(smile_x+8, smile_y+10, 5, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(smile_x+14, smile_y+10, 5, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(smile_x+8, smile_y+12, 5, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(smile_x+14, smile_y+12, 5, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(smile_x+8, smile_y+13, 5, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(smile_x+14, smile_y+13, 5, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(smile_x+9, smile_y+11, 2, WHITE);
			gpC_base->m_p_DDSurface_back->HLine(smile_x+15, smile_y+11, 2, WHITE);
			break;

		case GAMESTATUS_DIE:
			gpC_base->m_p_DDSurface_back->VLine(smile_x+9, smile_y+11, 6, BLACK);
			gpC_base->m_p_DDSurface_back->VLine(smile_x+11, smile_y+11, 6, BLACK);
			gpC_base->m_p_DDSurface_back->VLine(smile_x+15, smile_y+11, 6, BLACK);
			gpC_base->m_p_DDSurface_back->VLine(smile_x+17, smile_y+11, 6, BLACK);
			break;
		}
	}
	else
	{
		char szGameLevel[GAMELEVEL_MAX][16] = 
		{
			"�ʱ�",
			"�߱�",
			"����",
		};

		g_PrintColorStr(smile_x+5, smile_y+6, szGameLevel[p_button->GetID()-LEVEL0_ID], gpC_base->m_item_name_pi, BLACK);
	}
}

void	C_VS_UI_FINDING_MINE::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
// 	const static char *m_button_string[3] = 
//	{
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_HORN].GetString(),
//			(*g_pGameStringTable)[UI_STRING_MESSAGE_PREV_MAP].GetString(),
//			(*g_pGameStringTable)[UI_STRING_MESSAGE_NEXT_MAP].GetString(),
//	};	
//	
//	g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_button_string[p_button->GetID()],0,0);
}

void	C_VS_UI_FINDING_MINE::WindowEventReceiver(id_t event)
{
}

void	C_VS_UI_FINDING_MINE::Run(id_t id)
{
	switch (id)
	{
	case START_ID:
		NewGame(m_level);
		break;

	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_FINDING_MINE);
		break;

	case LEVEL0_ID:
		NewGame(GAMELEVEL_0);
		break;

	case LEVEL1_ID:
		NewGame(GAMELEVEL_1);
		break;
		
	case LEVEL2_ID:
		NewGame(GAMELEVEL_2);
		break;
	}
}

void	C_VS_UI_FINDING_MINE::Process()
{
	m_pC_button_group->Process();
}

void C_VS_UI_FINDING_MINE::DrawRect(RECT &rect, int depth, WORD color, WORD color2, WORD color3)
{
	RECT drawRect = {rect.left, rect.top, rect.right, rect.bottom};
	
	// �ٱ� �׵θ�
	drawRect.bottom = drawRect.top+depth;
	gpC_base->m_p_DDSurface_back->FillRect(&drawRect, color);
	
	drawRect.right = rect.left+depth;
	drawRect.bottom = rect.bottom;
	gpC_base->m_p_DDSurface_back->FillRect(&drawRect, color);
	
	drawRect.left = rect.right-depth;
	drawRect.right = rect.right;
	drawRect.bottom = rect.bottom;
	gpC_base->m_p_DDSurface_back->FillRect(&drawRect, color2);
	
	drawRect.left = rect.left;
	drawRect.top = rect.bottom-depth;
	drawRect.right = rect.right;
	drawRect.bottom = rect.bottom;
	gpC_base->m_p_DDSurface_back->FillRect(&drawRect, color2);

	for(int i = 0; i < depth; i++)
	{
		// ���
		drawRect.left = rect.right-i-1;
		drawRect.top = rect.top+i;
		drawRect.right = drawRect.left+1;
		drawRect.bottom = drawRect.top+1;
		gpC_base->m_p_DDSurface_back->FillRect(&drawRect, color3);

		drawRect.left = rect.right-depth;
		drawRect.top = rect.top+i;
		drawRect.right = drawRect.left+depth-i-1;
		drawRect.bottom = drawRect.top+1;
		gpC_base->m_p_DDSurface_back->FillRect(&drawRect, color);

		// ����
		drawRect.left = rect.left+i;
		drawRect.top = rect.bottom-i-1;
		drawRect.right = drawRect.left+1;
		drawRect.bottom = drawRect.top+1;
		gpC_base->m_p_DDSurface_back->FillRect(&drawRect, color3);
		
		drawRect.left = rect.left;
		drawRect.top = rect.bottom-i-1;
		drawRect.right = drawRect.left+i;
		drawRect.bottom = drawRect.top+1;
		gpC_base->m_p_DDSurface_back->FillRect(&drawRect, color);
	}
}

void	C_VS_UI_FINDING_MINE::ClearBlockFocus(int x, int y, int w, int h)
{
	if(x == -1 && y == -1)
	{
		x = 0; y = 0;
		w = m_boardSize.cy; h = m_boardSize.cx;
	}

	int i, j;
	
	for(i = y; i < y+w; i++)
	{
		for(j = x; j < x+h; j++)
		{
			m_Blocks[i*m_boardSize.cx+j].bl_focused = false;
		}
	}
}

void	C_VS_UI_FINDING_MINE::OpenBlock(int x, int y, bool bCenter)
{

	// ������ ������� return
	if(x < 0 || y < 0 || x >= m_boardSize.cx || y >= m_boardSize.cy)
		return;

	// �����غ����̳� �������ΰ�찡 �ƴϸ� return
	if(m_status == GAMESTATUS_DIE || m_status == GAMESTATUS_CLEAR)
	{
		return;
	}
	
	if(bCenter)	// ��� ��ư ��������
	{
		// ���� ���°� �ƴϸ� return
		if(m_Blocks[y*m_boardSize.cx+x].status != BLOCKSTATUS_OPEN)
			return;

		int flagSum = 0;
		for(int i = max(0, y-1); i <= min(m_boardSize.cy-1, y+1); i++)
		{
			for(int j = max(0, x-1); j <= min(m_boardSize.cx-1, x+1); j++)
			{
				if(m_Blocks[i*m_boardSize.cx+j].status == BLOCKSTATUS_FLAG)
					flagSum++;
			}
		}

		// ������ ��� ������ num�� Ʋ���� return
		if(flagSum != m_Blocks[y*m_boardSize.cx+x].num)
			return;

		// ���� 8���� ����
		OpenBlock(x-1, y-1);
		OpenBlock(x, y-1);
		OpenBlock(x+1, y-1);
		OpenBlock(x-1, y);
		OpenBlock(x+1, y);
		OpenBlock(x-1, y+1);
		OpenBlock(x, y+1);
		OpenBlock(x+1, y+1);

	}
	else	// ���ʹ�ư ��������
	{
		// �̹� �����ְų� ��� ���¸� return
		if(m_Blocks[y*m_boardSize.cx+x].status == BLOCKSTATUS_OPEN || m_Blocks[y*m_boardSize.cx+x].status == BLOCKSTATUS_FLAG)
			return;

		if(m_status == GAMESTATUS_READY)
		{
			m_status = GAMESTATUS_GAME;
			m_startTime = timeGetTime();
		}

		// ����
		m_Blocks[y*m_boardSize.cx+x].status = BLOCKSTATUS_OPEN;
		
		// ���ڰ� ������ ���̴���
		if(m_Blocks[y*m_boardSize.cx+x].num == -1)
		{
			m_status = GAMESTATUS_DIE;
			m_startTime = timeGetTime()-m_startTime;
		}
		else if(m_Blocks[y*m_boardSize.cx+x].num == 0)	// ���ڰ� ������ ���� ���ϵ��� ����
		{
			OpenBlock(x-1, y-1);
			OpenBlock(x, y-1);
			OpenBlock(x+1, y-1);
			OpenBlock(x-1, y);
			OpenBlock(x+1, y);
			OpenBlock(x-1, y+1);
			OpenBlock(x, y+1);
			OpenBlock(x+1, y+1);
		}
	}

}

void	C_VS_UI_FINDING_MINE::DrawBlock(int x, int y, BLOCK_STRUCT &block)
{
	COLORREF numberColor[8] = 
	{
		RGB(0, 0, 255),
			RGB(0, 130, 0),
			RGB(255, 0, 0),
			RGB(0, 0, 132),
			RGB(132, 0, 0),
			RGB(0, 0, 0),
			RGB(0, 0, 0),
			RGB(0, 0, 0),
	};
	
	RECT rect;
	
	rect.left = x;
	rect.top = y;
	rect.right = x+BLOCK_WIDTH;
	rect.bottom = y+BLOCK_HEIGHT;
	
	switch(block.status)
	{
	case BLOCKSTATUS_CLOSE:
		if(m_status == GAMESTATUS_DIE && block.num == -1)	// ���ڱ׸���
		{
			gpC_base->m_p_DDSurface_back->HLine(rect.left, rect.top, rect.right-rect.left, DARKGRAY);
			gpC_base->m_p_DDSurface_back->VLine(rect.left, rect.top, rect.bottom-rect.top, DARKGRAY);
			
			gpC_base->m_p_DDSurface_back->HLine(rect.left+2, rect.top+BLOCK_HEIGHT/2, BLOCK_WIDTH-3, BLACK);
			gpC_base->m_p_DDSurface_back->VLine(rect.left+BLOCK_WIDTH/2, rect.top+2, BLOCK_HEIGHT-3, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+4, 1, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+12, rect.top+4, 1, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+6, rect.top+4, 5, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+5, rect.top+5, 7, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+6, 9, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+7, 9, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+9, 9, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+10, 9, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+5, rect.top+11, 7, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+6, rect.top+12, 5, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+12, 1, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+12, rect.top+12, 1, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+6, rect.top+6, 2, WHITE);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+6, rect.top+7, 2, WHITE);
		}
		else if(block.bl_focused && m_status != GAMESTATUS_DIE && m_status != GAMESTATUS_CLEAR)
		{
			gpC_base->m_p_DDSurface_back->HLine(rect.left, rect.top, rect.right-rect.left, DARKGRAY);
			gpC_base->m_p_DDSurface_back->VLine(rect.left, rect.top, rect.bottom-rect.top, DARKGRAY);
		}
		else
		{
			DrawRect(rect, 2, WHITE, DARKGRAY, GRAY);
		}
		break;
		
	case BLOCKSTATUS_QUESTION:
		if(m_status == GAMESTATUS_DIE && block.num == -1)	// ���ڱ׸���
		{
			gpC_base->m_p_DDSurface_back->HLine(rect.left, rect.top, rect.right-rect.left, DARKGRAY);
			gpC_base->m_p_DDSurface_back->VLine(rect.left, rect.top, rect.bottom-rect.top, DARKGRAY);

			gpC_base->m_p_DDSurface_back->HLine(rect.left+2, rect.top+BLOCK_HEIGHT/2, BLOCK_WIDTH-3, BLACK);
			gpC_base->m_p_DDSurface_back->VLine(rect.left+BLOCK_WIDTH/2, rect.top+2, BLOCK_HEIGHT-3, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+4, 1, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+12, rect.top+4, 1, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+6, rect.top+4, 5, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+5, rect.top+5, 7, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+6, 9, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+7, 9, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+9, 9, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+10, 9, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+5, rect.top+11, 7, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+6, rect.top+12, 5, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+12, 1, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+12, rect.top+12, 1, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+6, rect.top+6, 2, WHITE);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+6, rect.top+7, 2, WHITE);
		}
		else if(block.bl_focused && m_status != GAMESTATUS_DIE && m_status != GAMESTATUS_CLEAR)
		{
			gpC_base->m_p_DDSurface_back->HLine(rect.left, rect.top, rect.right-rect.left, DARKGRAY);
			gpC_base->m_p_DDSurface_back->VLine(rect.left, rect.top, rect.bottom-rect.top, DARKGRAY);

			g_PrintColorStr(rect.left+4, rect.top+3, "?", gpC_base->m_user_id_pi, RGB(0, 0, 132));
		}
		else
		{
			DrawRect(rect, 2, WHITE, DARKGRAY, GRAY);
			g_PrintColorStr(rect.left+4, rect.top+3, "?", gpC_base->m_user_id_pi, RGB(0, 0, 132));
		}
		break;
		
	case BLOCKSTATUS_OPEN:
		gpC_base->m_p_DDSurface_back->HLine(rect.left, rect.top, rect.right-rect.left, DARKGRAY);
		gpC_base->m_p_DDSurface_back->VLine(rect.left, rect.top, rect.bottom-rect.top, DARKGRAY);
		
		if(block.num == -1)	// ���ڱ׸���
		{
			RECT fillRect = {rect.left+1, rect.top+1, rect.right, rect.bottom };
			gpC_base->m_p_DDSurface_back->FillRect(&fillRect, RED);
			
			gpC_base->m_p_DDSurface_back->HLine(rect.left+2, rect.top+BLOCK_HEIGHT/2, BLOCK_WIDTH-3, BLACK);
			gpC_base->m_p_DDSurface_back->VLine(rect.left+BLOCK_WIDTH/2, rect.top+2, BLOCK_HEIGHT-3, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+4, 1, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+12, rect.top+4, 1, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+6, rect.top+4, 5, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+5, rect.top+5, 7, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+6, 9, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+7, 9, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+9, 9, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+10, 9, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+5, rect.top+11, 7, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+6, rect.top+12, 5, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+12, 1, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+12, rect.top+12, 1, BLACK);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+6, rect.top+6, 2, WHITE);
			gpC_base->m_p_DDSurface_back->HLine(rect.left+6, rect.top+7, 2, WHITE);
		}
		else if(block.num != 0)
		{
			char szTemp[2];
			sprintf(szTemp, "%d", block.num);
			g_PrintColorStr(rect.left+5, rect.top+3, szTemp, gpC_base->m_user_id_pi, numberColor[block.num-1]);
		}
		break;
			
	case BLOCKSTATUS_FLAG:
		DrawRect(rect, 2, WHITE, DARKGRAY, GRAY);
		gpC_base->m_p_DDSurface_back->HLine(rect.left+7, rect.top+3, 2, LIGHTRED);
		gpC_base->m_p_DDSurface_back->HLine(rect.left+5, rect.top+4, 4, LIGHTRED);
		gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+5, 5, LIGHTRED);
		gpC_base->m_p_DDSurface_back->HLine(rect.left+5, rect.top+6, 4, LIGHTRED);
		gpC_base->m_p_DDSurface_back->HLine(rect.left+7, rect.top+7, 2, LIGHTRED);
		gpC_base->m_p_DDSurface_back->VLine(rect.left+8, rect.top+8, 2, BLACK);
		gpC_base->m_p_DDSurface_back->HLine(rect.left+6, rect.top+10, 4, BLACK);
		gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+11, 8, BLACK);
		gpC_base->m_p_DDSurface_back->HLine(rect.left+4, rect.top+12, 8, BLACK);

		break;
		
	}
}

bool C_VS_UI_FINDING_MINE::CheckClear()
{
	for(int i = 0; i < m_boardSize.cy; i++)
	{
		for(int j = 0; j < m_boardSize.cx; j++)
		{
			if(m_Blocks[i*m_boardSize.cx+j].status != BLOCKSTATUS_OPEN && m_Blocks[i*m_boardSize.cx+j].num != -1)
				return false;
		}
	}
	return true;
}

void C_VS_UI_FINDING_MINE::DrawNumbers(int x, int y, int number)
{
	BYTE numberFlag[11] = 
	{
		0x77,	// 1110111b,	'0'
		0x12,	// 0010010b,	'1'
		0x5D,	// 1011101b,	'2'
		0x5B,	// 1011011b,	'3'
		0x3A,	// 0111010b,	'4'
		0x6B,	// 1101011b,	'5'
		0x6F,	// 1101111b,	'6'
		0x72,	// 1110010b,	'7'
		0x7F,	// 1111111b,	'8'
		0x7B,	// 1111011b,	'9'
		0x08,	// 0001000b,	'-'
	};

	RECT rect = {x, y, x+13*3, y+23};

	gpC_base->m_p_DDSurface_back->FillRect(&rect, BLACK);

	bool bMinus = false;

	if(number < 0)
	{
		bMinus = true;
		number *= -1;
		number %= 100;
	}
	else
	{
		number %= 1000;
	}


	if(bMinus)
	{
		DrawNumber(x, y, numberFlag[10]);
		DrawNumber(x+13, y, numberFlag[number/10]);
		DrawNumber(x+26, y, numberFlag[number%10]);
	}
	else
	{
		DrawNumber(x, y, numberFlag[number/100]);
		DrawNumber(x+13, y, numberFlag[(number%100)/10]);
		DrawNumber(x+26, y, numberFlag[number%10]);
	}
}

void C_VS_UI_FINDING_MINE::DrawNumber(int x, int y, BYTE flag)
{
	// �� ���� 
	if(flag & 0x40)
	{
		gpC_base->m_p_DDSurface_back->HLine(x+2, y+1, 9, LIGHTRED);
		gpC_base->m_p_DDSurface_back->HLine(x+3, y+2, 7, LIGHTRED);
		gpC_base->m_p_DDSurface_back->HLine(x+4, y+3, 5, LIGHTRED);
	}
	else
	{
		gpC_base->m_p_DDSurface_back->HLine(x+3, y+1, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+5, y+1, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+7, y+1, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+9, y+1, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+4, y+2, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+6, y+2, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+8, y+2, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+5, y+3, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+7, y+3, 1, DARKRED);
	}

	// ���� ����
	if(flag & 0x20)
	{
		gpC_base->m_p_DDSurface_back->VLine(x+1, y+2, 9, LIGHTRED);
		gpC_base->m_p_DDSurface_back->VLine(x+2, y+3, 7, LIGHTRED);
		gpC_base->m_p_DDSurface_back->VLine(x+3, y+4, 5, LIGHTRED);
	}
	else
	{
		gpC_base->m_p_DDSurface_back->VLine(x+1, y+2, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+1, y+4, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+1, y+6, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+1, y+8, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+1, y+10, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+2, y+3, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+2, y+5, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+2, y+7, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+2, y+9, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+3, y+4, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+3, y+6, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+3, y+8, 1, DARKRED);
	}

	// ���� ������
	if(flag & 0x10)
	{
		gpC_base->m_p_DDSurface_back->VLine(x+11, y+2, 9, LIGHTRED);
		gpC_base->m_p_DDSurface_back->VLine(x+10, y+3, 7, LIGHTRED);
		gpC_base->m_p_DDSurface_back->VLine(x+9, y+4, 5, LIGHTRED);
	}
	else
	{
		gpC_base->m_p_DDSurface_back->VLine(x+11, y+2, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+11, y+4, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+11, y+6, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+11, y+8, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+11, y+10, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+10, y+3, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+10, y+5, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+10, y+7, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+10, y+9, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+9, y+4, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+9, y+6, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+9, y+8, 1, DARKRED);
	}

	// ���
	if(flag & 0x08)
	{
		gpC_base->m_p_DDSurface_back->HLine(x+3, y+10, 7, LIGHTRED);
		gpC_base->m_p_DDSurface_back->HLine(x+2, y+11, 9, LIGHTRED);
		gpC_base->m_p_DDSurface_back->HLine(x+3, y+12, 7, LIGHTRED);
	}
	else
	{
		gpC_base->m_p_DDSurface_back->HLine(x+4, y+10, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+6, y+10, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+8, y+10, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+3, y+11, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+5, y+11, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+7, y+11, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+9, y+11, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+4, y+12, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+6, y+12, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+8, y+12, 1, DARKRED);
	}

	// �Ʒ� ����
	if(flag & 0x04)
	{
		gpC_base->m_p_DDSurface_back->VLine(x+1, y+12, 9, LIGHTRED);
		gpC_base->m_p_DDSurface_back->VLine(x+2, y+13, 7, LIGHTRED);
		gpC_base->m_p_DDSurface_back->VLine(x+3, y+14, 5, LIGHTRED);
	}
	else
	{
		gpC_base->m_p_DDSurface_back->VLine(x+1, y+12, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+1, y+14, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+1, y+16, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+1, y+18, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+1, y+20, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+2, y+13, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+2, y+15, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+2, y+17, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+2, y+19, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+3, y+14, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+3, y+16, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+3, y+18, 1, DARKRED);
	}
	
	// �Ʒ� ������
	if(flag & 0x02)
	{
		gpC_base->m_p_DDSurface_back->VLine(x+11, y+12, 9, LIGHTRED);
		gpC_base->m_p_DDSurface_back->VLine(x+10, y+13, 7, LIGHTRED);
		gpC_base->m_p_DDSurface_back->VLine(x+9, y+14, 5, LIGHTRED);
	}
	else
	{
		gpC_base->m_p_DDSurface_back->VLine(x+11, y+12, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+11, y+14, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+11, y+16, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+11, y+18, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+11, y+20, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+10, y+13, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+10, y+15, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+10, y+17, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+10, y+19, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+9, y+14, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+9, y+16, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->VLine(x+9, y+18, 1, DARKRED);
	}

	// �� �̷�
	if(flag & 0x01)
	{
		gpC_base->m_p_DDSurface_back->HLine(x+2, y+21, 9, LIGHTRED);
		gpC_base->m_p_DDSurface_back->HLine(x+3, y+20, 7, LIGHTRED);
		gpC_base->m_p_DDSurface_back->HLine(x+4, y+19, 5, LIGHTRED);
	}
	else
	{
		gpC_base->m_p_DDSurface_back->HLine(x+3, y+21, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+5, y+21, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+7, y+21, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+9, y+21, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+4, y+20, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+6, y+20, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+8, y+20, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+5, y+19, 1, DARKRED);
		gpC_base->m_p_DDSurface_back->HLine(x+7, y+19, 1, DARKRED);
	}
}


///////////////////////////////////////////////////////////////////
//
// ARROW_TILE
//
///////////////////////////////////////////////////////////////////
C_VS_UI_ARROW_TILE::C_VS_UI_ARROW_TILE()
{		
	AttrTopmost( true );
	g_RegisterWindow(this);	
	AttrKeyboardControl(true);
	m_pC_button_group = NULL;

	m_SPK.Open("DATA\\UI\\SPK\\ArrowTile.spk");
	
	int window_w,window_h;
	window_w = 500;
	window_h = 500;
	Set(g_GameRect.right/2-window_w/2, g_GameRect.bottom/2 - window_h/2, window_w, window_h);
	m_pC_button_group = new ButtonGroup(this);	
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(window_w-60,90,m_SPK.GetWidth(SPK_ROTATION_RIGHT), m_SPK.GetHeight(SPK_ROTATION_RIGHT), ROTATION_RIGHT_ID, this, SPK_ROTATION_RIGHT ) );
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(window_w-60,150,m_SPK.GetWidth(SPK_ROTATION_LEFT), m_SPK.GetHeight(SPK_ROTATION_LEFT), ROTATION_LEFT_ID, this, SPK_ROTATION_LEFT ) );
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(window_w-60,210,m_SPK.GetWidth(SPK_MOVE), m_SPK.GetHeight(SPK_MOVE), MOVE_ID, this, SPK_MOVE ) );
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(window_w-50,20,m_SPK.GetWidth(SPK_HELP), m_SPK.GetHeight(SPK_HELP), HELP_ID, this, SPK_HELP ) );
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(window_w-25,20,m_SPK.GetWidth(SPK_CLOSE), m_SPK.GetHeight(SPK_CLOSE), CLOSE_ID, this, SPK_CLOSE ) );

	m_Stage = STAGE_NONE;
	m_bCanStart = false;
}

C_VS_UI_ARROW_TILE::~C_VS_UI_ARROW_TILE()
{
	InitMonster();
	g_UnregisterWindow(this);	
	DeleteNew(m_pC_button_group);	
}

void	C_VS_UI_ARROW_TILE::Start()
{
	PI_Processor::Start();
	AttrPin(true);
	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();
	InitStage();
	InitGame();

	Run(HELP_ID);
}

void	C_VS_UI_ARROW_TILE::Finish()
{
	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);	
	InitMonster();
}

void	C_VS_UI_ARROW_TILE::Show()
{
	int BoardSizeX,BoardSizeY,startX,startY;

	startX = 20+x;
	startY = 90+y;

	if( m_Stage == STAGE_5X5 )
		startX += 40;
	else if (m_Stage == STAGE_7X7 )
		startX += 30;

	BoardSizeX = m_SPK.GetWidth(SPK_GOAL) + m_SPK.GetWidth(SPK_GOAL) +
		m_SPK.GetWidth(SPK_ARROW_BACK_1) * GetMapSize();
	BoardSizeY = m_SPK.GetHeight(SPK_ARROW_BACK_1) * GetMapSize();

	RECT rect = { startX, startY, startX + BoardSizeX, startY + BoardSizeY };

	if(gpC_base->m_p_DDSurface_back->Lock())
	{		
		Rect rtOut(0,0,w,h);
		m_SPK.BltLockedClip( x,y,rtOut,SPK_BACK );				
		
		m_SPK.BltLockedClip( x,y,rtOut,SPK_OUT_BOX_LEFT_UP );		
		rtOut.Set(0,0, m_SPK.GetWidth(SPK_OUT_BOX_RIGHT), h );
		m_SPK.BltLockedClip( x+w,y, rtOut, SPK_OUT_BOX_RIGHT );

		
		rtOut.Set(0,0,w+1,m_SPK.GetHeight( SPK_OUT_BOX_DOWN ));		
		m_SPK.BltLockedClip( x,y+h, rtOut, SPK_OUT_BOX_DOWN );
		m_SPK.BltLocked(x+w+1,y+h,SPK_OUT_BOX_BOTTOM);
		
		int ArrowSizeX = m_SPK.GetWidth(SPK_ARROW_BACK_1);
		int ArrowSizeY = m_SPK.GetHeight(SPK_ARROW_BACK_1);		
		Rect rt(startX-1, startY-1, rect.right - rect.left +2, rect.bottom - rect.top +2);
		
		m_SPK.BltLocked( startX-10, startY-5, SPK_GOAL );	
		m_SPK.BltLocked( startX-10+6, startY+2, SPK_START );
		m_SPK.BltLocked( startX + ArrowSizeX*GetMapSize()+m_SPK.GetWidth(SPK_GOAL)+10, startY + ArrowSizeY*(GetMapSize()-1)-6, SPK_GOAL);

		if(GetMapSize() > 0 )
		{
			// ���带 �׸��� 		
			int index=0;
			for(int tx=0;tx<GetMapSize();tx++)
			{
				for(int ty=0;ty<GetMapSize();ty++)
				{
					int SpriteID = -1;
					TILE_ATTRIBUTE Attr = m_Tile[ty * GetMapSize() + tx].Attr;
					short	Direction = (m_Tile[ty * GetMapSize() + tx].Direction)%4;
					switch( Attr )
					{
					case ATTR_NORMAL :
						SpriteID = SPK_ARROW_UP + Direction*4;
						break;
					case ATTR_SLIDING :
						SpriteID = SPK_ARROW_UP + Direction*4;
						break;
					case ATTR_2_MOVE :
						break;
					case ATTR_TRAP :
						SpriteID = SPK_ARROW_UP + Direction*4;
						break;
					}
					if( m_Tile[ty*GetMapSize()+tx].RemainRotationAngle > 0 )
					{
						m_Tile[ty*GetMapSize() + tx].RemainRotationAngle --;
						SpriteID = m_Tile[ty*GetMapSize() + tx].AniFrame[m_Tile[ty*GetMapSize() + tx].RemainRotationAngle];
					}
					if(SpriteID != -1)
					{
						int BoxX = startX+ tx*ArrowSizeX + m_SPK.GetWidth(SPK_GOAL);
						int BoxY = startY+ ArrowSizeY * ty;
						m_SPK.BltLocked(BoxX , BoxY, SPK_ARROW_BACK_1 + (index&1) );				
						m_SPK.BltLocked( (BoxX + (ArrowSizeX/2))-m_SPK.GetWidth(SpriteID)/2, 
							(BoxY+(ArrowSizeY/2))-m_SPK.GetHeight(SpriteID)/2, SpriteID);					
					}
					index++;
				}
			}

			rt.Set(0,0,ArrowSizeX*GetMapSize()+m_SPK.GetWidth(SPK_IN_BOX_RIGHT),ArrowSizeY*GetMapSize() +m_SPK.GetHeight(SPK_IN_BOX_DOWN));
			m_SPK.BltLockedClip( startX+m_SPK.GetWidth(SPK_GOAL)-m_SPK.GetWidth(SPK_IN_BOX_RIGHT), startY-m_SPK.GetHeight(SPK_IN_BOX_DOWN), rt, SPK_IN_BOX_LEFT_UP );
			rt.Set(0,0,ArrowSizeX*GetMapSize()+m_SPK.GetWidth(SPK_IN_BOX_RIGHT),m_SPK.GetHeight(SPK_IN_BOX_DOWN));
			m_SPK.BltLockedClip( startX+m_SPK.GetWidth(SPK_GOAL)-m_SPK.GetWidth(SPK_IN_BOX_RIGHT),
				startY+ArrowSizeY*GetMapSize(), rt, SPK_IN_BOX_DOWN);

			rt.Set(0,0,m_SPK.GetWidth(SPK_IN_BOX_RIGHT),ArrowSizeY*GetMapSize()+m_SPK.GetHeight(SPK_IN_BOX_DOWN) );
			m_SPK.BltLockedClip( startX+ArrowSizeX*GetMapSize()+m_SPK.GetWidth(SPK_GOAL), startY-m_SPK.GetHeight(SPK_IN_BOX_DOWN),rt, SPK_IN_BOX_RIGHT );		


			// Player
			int myspr = -1;

			switch( g_eRaceInterface )
			{
			case RACE_SLAYER :
				if(g_char_slot_ingame.bl_female)
					myspr = SPK_SLAYER_FEMALE_UP;
				else
					myspr = SPK_SLAYER_MALE_UP;
				break;
			case RACE_VAMPIRE :
				if(g_char_slot_ingame.bl_female)
					myspr = SPK_VAMPIRE_FEMALE_UP;
				else
					myspr = SPK_VAMPIRE_MALE_UP;
				break;
			case RACE_OUSTERS :
				myspr = SPK_OUSTERS_UP;
				break;
			}
			m_SPK.BltLocked(startX+ArrowSizeX*GetMapSize()+m_SPK.GetWidth(SPK_GOAL)-2, startY+ArrowSizeY*GetMapSize(),SPK_IN_BOX_BOTTOM);
			if( m_Player.X == -1 && m_Player.Y == 0 )		// �÷��̾ ���� �غ����̸�
			{
				m_SPK.BltLocked( startX + ( m_SPK.GetWidth(SPK_START)/2 - m_SPK.GetWidth(myspr+2)/2) ,
					startY + (m_SPK.GetHeight(SPK_START)/2 - m_SPK.GetHeight(myspr+2)/2), myspr+2 );
			} else
			if( m_Player.X == GetMapSize() && m_Player.Y == GetMapSize() -1 )		// �÷��̾ ��ǥ ������ �������� ���
			{
				m_SPK.BltLocked( startX + m_SPK.GetWidth(SPK_GOAL)+GetMapSize() * ArrowSizeX + m_SPK.GetWidth(SPK_START) + (m_SPK.GetWidth(SPK_GOAL)/2 - m_SPK.GetWidth(myspr+2)/2) ,
					startY + (GetMapSize() -1) * ArrowSizeY + (m_SPK.GetHeight(SPK_GOAL)/2 - m_SPK.GetHeight(myspr+2)/2), myspr+2 );
				m_Player.Status = PLAYER_STATUS_FINISH;
			} else
			{												// �Ϲ� �������̸�
				if( m_Player.Status != PLAYER_STATUS_FINISH )
				{
					int sprid = myspr + GetDirection(&m_Player);
					m_SPK.BltLocked( (startX + m_SPK.GetWidth(SPK_GOAL) + ArrowSizeX * m_Player.X)+m_SPK.GetWidth(SPK_ARROW_BACK_1)/2-m_SPK.GetWidth(sprid)/2,
						startY + ArrowSizeY * m_Player.Y + ArrowSizeY/2 - m_SPK.GetHeight(sprid)/2, sprid);
				}
			}

			std::list<S_CHARACTER*>::const_iterator itr = m_MonsterList.begin();
			std::list<S_CHARACTER*>::const_iterator enditr = m_MonsterList.end();
			
			while( itr != enditr )
			{
				S_CHARACTER* Monster = *itr;
				if(Monster == NULL )
				{
					itr++;
					continue;
				}
				
				int sprid = SPK_DEAD_BODY_UP;
				
				if( GetDirection(Monster) < 0xFE )
				{
					m_SPK.BltLocked( startX + m_SPK.GetWidth(SPK_GOAL) + ArrowSizeX * Monster->X + m_SPK.GetWidth(SPK_ARROW_BACK_1)/2-m_SPK.GetWidth(sprid)/2,
						startY + ArrowSizeY * Monster->Y+ArrowSizeY/2-m_SPK.GetHeight(sprid)/2, sprid+GetDirection(Monster));
				}
				
				itr++;
			}
		}
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	rect.left = x+70;
	rect.top = y+50;
	rect.right = rect.left+100;
	rect.bottom = rect.top + 20;
	gpC_base->m_p_DDSurface_back->FillRect(&rect, CSDLGraphics::Color(3,3,3));
	gpC_global_resource->DrawOutBox(rect.left-2, rect.top-2, rect.right-rect.left+4, rect.bottom-rect.top+4);

	rect.left = x+400;
	rect.top = y+50;
	rect.right = rect.left + 85;
	rect.bottom = rect.top + 20;

//	gpC_base->m_p_DDSurface_back->FillRect(&rect, CSDLGraphics::Color(3,3,3));	
//	gpC_global_resource->DrawOutBox(Rect(rect.left-2, rect.top-2, rect.right-rect.left+4, rect.bottom-rect.top+4) );
	

	if(g_FL2_GetDC()) 
	{
		char szTemp[256];
		m_pC_button_group->ShowDescription();
		g_PrintColorStrOut(x+20, y+53,"TIME ", gpC_base->m_item_name_pi,RGB_WHITE,RGB_BLACK);	
//		g_PrintColorStrOut(x+280, y+53,"My Best Score ",gpC_base->m_item_name_pi,RGB_WHITE,RGB_BLACK);

//		wsprintf(szTemp,"%d.%d",m_MyBestScore/100, (m_MyBestScore%100) );
//		g_PrintColorStrOut(x+440,y+53,szTemp, gpC_base->m_item_name_pi, RGB_WHITE, RGB_BLACK);

		// �ð� ǥ��		
		if( m_Player.Status != PLAYER_STATUS_READY )
		{
			if( m_Player.Status == PLAYER_STATUS_DIE || m_Player.Status == PLAYER_STATUS_FINISH )
			{
				wsprintf(szTemp,"%5d.%2d",(m_Player.EndTime - m_Player.StartTime)/1000,
					((m_Player.EndTime - m_Player.StartTime)%1000 )/10);
				g_PrintColorStrOut(x+80,y+53,szTemp, gpC_base->m_item_name_pi, RGB_WHITE, RGB_BLACK);
			} else
			{
				wsprintf(szTemp,"%5d.%2d",(timeGetTime() - m_Player.StartTime)/1000,
					((timeGetTime() - m_Player.StartTime)%1000)/10);
				g_PrintColorStrOut(x+80,y+53,szTemp, gpC_base->m_item_name_pi, RGB_WHITE, RGB_BLACK);
			}
		}

		// �ְ� ���ǥ��		
		wsprintf(szTemp, "%s Best Score : %s %d.%d",g_pUserInformation->WorldName.GetString(),
			m_topName.c_str(), m_topScore/100, (m_topScore%100));
		g_PrintColorStrOut(x+120, y+20, szTemp,gpC_base->m_item_name_pi, RGB_WHITE,RGB_BLACK);

		wsprintf(szTemp,"#%d",m_GameCode);
		g_PrintColorStr(x+10,y+160,szTemp,gpC_base->m_item_desc_pi,RGB_WHITE);
		g_FL2_ReleaseDC();
	}
}

bool	C_VS_UI_ARROW_TILE::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);

	switch(message)
	{
	case M_MOVING:
		break;

	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		break;

	case M_LEFTBUTTON_UP:
		break;
	}	
	return true;
}

void	C_VS_UI_ARROW_TILE::KeyboardControl(UINT message, UINT key, long extra)	
{		
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{		
		case VK_ESCAPE :
			Run(CLOSE_ID);
			break;		
		case VK_RETURN :
//			ActionMove();
			break;
		case VK_LEFT :
//			ActionRotation(0);
			break;
		case VK_RIGHT :
//			ActionRotation(1);
			break;
		}
	}
}

bool	C_VS_UI_ARROW_TILE::IsPixel(int _x,int _y)
{
	return true;
}

void	C_VS_UI_ARROW_TILE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if( p_button->GetID() == ROTATION_LEFT_ID || p_button->GetID() == ROTATION_RIGHT_ID )
	{
		if( m_Player.bCanRotation == true )
		{
			if(p_button->GetFocusState())
			{
				if(p_button->GetPressState())
					m_SPK.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
				else
					m_SPK.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
			}
			else
				m_SPK.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);	
		} else
		{
			m_SPK.BltLockedDarkness(x+p_button->x, y+p_button->y, p_button->m_image_index ,1);			
		}
	} 
	else if( p_button->GetID() ==MOVE_ID )
	{
		if( m_Player.Status == PLAYER_STATUS_FINISH || m_Player.Status == PLAYER_STATUS_DIE )
		{			
			m_SPK.BltLockedDarkness(x+p_button->x, y+p_button->y, p_button->m_image_index ,1);			
		} else
		{
			if(p_button->GetFocusState())
			{
				if(p_button->GetPressState())
					m_SPK.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
				else
					m_SPK.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
			}
			else
				m_SPK.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);	
		} 		
	} else
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				m_SPK.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
			else
				m_SPK.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
		}
		else
			m_SPK.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);	
	}
}

void	C_VS_UI_ARROW_TILE::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
}

void	C_VS_UI_ARROW_TILE::WindowEventReceiver(id_t event)
{
}

void	C_VS_UI_ARROW_TILE::Run(id_t id)
{
	switch (id)
	{
	case ROTATION_LEFT_ID :
		ActionRotation(0);
		break;
	case ROTATION_RIGHT_ID :
		ActionRotation(1);
		break;
	case MOVE_ID :
		ActionMove();
		break;
	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_ARROW_TILE);
		break;
	case HELP_ID :
		gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::MINI_GAME_ARROW_TILE);
		break;
	}
}

void	C_VS_UI_ARROW_TILE::Process()
{
	m_pC_button_group->Process();
	ProcessGameMain();
}

void	C_VS_UI_ARROW_TILE::ProcessGameMain()
{
	switch( m_Stage )
	{
	case STAGE_5X5 :
	case STAGE_7X7 :
	case STAGE_9X9 :
		ProcessCharacter();
		break;
	case STAGE_NONE :
		break;
	case STAGE_END :
		break;
	}

	if(m_Player.Status == PLAYER_STATUS_FINISH )
	{
		gpC_base->SendMessage(UI_CLEAR_STAGE,MAKELONG(WORD(m_Stage-STAGE_5X5),0), m_Player.EndTime-m_Player.StartTime);
		m_Stage = GAME_STAGE(m_Stage+1);
		if(m_Stage == STAGE_END )
		{
			// ����
//			gpC_base->SendMessage(UI_CLEAR_STAGE,MAKELONG(WORD(m_Stage-STAGE_5X5),0), m_Player.EndTime-m_Player.StartTime);
			m_Stage = GAME_STAGE(m_Stage);

			g_StartAllStageClearMessage(-1,-1);
//			gpC_base->SendMessage(UI_CLEAR_ALL_STAGE,0);
		}
		else
		{			
			InitStage();
			InitGame();
		}
	}
}

void	C_VS_UI_ARROW_TILE::ProcessTile()
{
	if( m_Player.X < 0 || m_Player.Y >= GetMapSize() || m_Player.X >= GetMapSize() || m_Player.Y < 0 )
		return;
	
	S_TILE &Tile = m_Tile[m_Player.Y * GetMapSize() + m_Player.X];
	
	switch( Tile.Attr )
	{
	case ATTR_SLIDING :
		break;
	case ATTR_TRAP :
		break;
	}
}

void	C_VS_UI_ARROW_TILE::ProcessTimeExpired()
{
	// ���� �ð������� ����.
}

void	C_VS_UI_ARROW_TILE::ProcessRotation()
{
	// ȸ���κ� ó��.
}

void	C_VS_UI_ARROW_TILE::ProcessCharacter()
{
	std::list<S_CHARACTER*>::const_iterator itr = m_MonsterList.begin();
	std::list<S_CHARACTER*>::const_iterator enditr = m_MonsterList.end();
	
	while( itr != enditr )
	{
		S_CHARACTER* Monster = *itr;
		itr++;

		if( Monster == NULL )
			continue;
		
		if( m_Player.X == Monster->X && m_Player.Y == Monster->Y )
		{
			// ���� ��ġ�� ������ ����
			
			SetDie();
		}
	}
	switch( m_Player.Status )
	{
	case PLAYER_STATUS_TRAP :
		if( m_Player.DelayTime <= timeGetTime() )
		{
			m_Player.DelayTime = 0;
			m_Player.Status = PLAYER_STATUS_NORMAL;
		}
		break;
	case PLAYER_STATUS_DIE :
		break;
	}
	
	if( m_RemainCountMoveMonster > 0)
	{
		if(TimerMonsterMove(600) )
		{
			MoveMonster();
			m_RemainCountMoveMonster--;
		}
	}

	if( m_Player.X == 0 && m_Player.bCanRotation == false && GetDirection(&m_Player) == DIRECTION_LEFT 
		&&	m_Player.Status != PLAYER_STATUS_READY)
	{
		// ĳ���� ����
		SetDie();
	}
	else if ( m_Player.Y == 0 && m_Player.bCanRotation == false && GetDirection(&m_Player) == DIRECTION_UP 
		&&	m_Player.Status != PLAYER_STATUS_READY)
	{
		SetDie();
	}
	else if (m_Player.X == GetMapSize()-1 && m_Player.bCanRotation == false && GetDirection(&m_Player) == DIRECTION_RIGHT
		&& m_Player.Y != GetMapSize() -1 && m_Player.Status != PLAYER_STATUS_READY)
	{
		SetDie();
	}
	else if (m_Player.Y == GetMapSize()-1&& m_Player.bCanRotation == false && GetDirection(&m_Player) == DIRECTION_DOWN &&	m_Player.Status != PLAYER_STATUS_READY &&
		m_Player.X != GetMapSize()-1)
	{
		SetDie();
	}
}

void	C_VS_UI_ARROW_TILE::ActionMove()					// Move ��ư Ŭ����
{
	if( m_Player.Status == PLAYER_STATUS_DIE || !m_bCanStart)
		return;
	
	// ������̸�
	if( m_Player.X == -1 && m_Player.Y == 0 || m_Player.Status == PLAYER_STATUS_READY)
	{
		// ����
		m_Player.X = 0;
		m_Player.Y = 0;
		m_Player.bCanRotation = true;
		m_Player.StartTime = timeGetTime();
		m_Player.Status = PLAYER_STATUS_NORMAL;
	} 
	else if( m_Player.X == GetMapSize() -1 && m_Player.Y == GetMapSize() -1 && GetDirection(&m_Player) == DIRECTION_RIGHT ) 
	{
		// �������̸�
		m_Player.EndTime = timeGetTime();
		m_Player.X ++;
	} 
	else if( m_Player.X >= 0 && m_Player.Y < GetMapSize() && m_Player.X < GetMapSize() && m_Player.Y >= 0 )
	{
		BYTE Direction = GetDirection(&m_Player);
		bool	bMoveOK = false;
		switch(Direction)
		{
		case DIRECTION_UP :
			if( MoveUP(&m_Player) )
			{
				bMoveOK = true;
			}
			break;
		case DIRECTION_RIGHT :
			if( MoveRIGHT(&m_Player) )
			{
				bMoveOK = true;
			}
			break;
		case DIRECTION_DOWN :
			if ( MoveDOWN(&m_Player) )
			{
				bMoveOK = true;
			}
			break;
		case DIRECTION_LEFT :
			if ( MoveLEFT(&m_Player) )
			{
				bMoveOK = true;
			}
			break;
		}
		if( bMoveOK )
		{
			std::list<S_CHARACTER*>::const_iterator itr = m_MonsterList.begin();
			std::list<S_CHARACTER*>::const_iterator enditr = m_MonsterList.end();

			bool	bCheck = false;
			
			while( itr != enditr )
			{
				const S_CHARACTER *Character = *itr;
				if(Character == NULL )
				{
					++itr;
					continue;		
				}
				if( Character->X == m_Player.X && Character->Y == m_Player.Y)
					bCheck = true;					
				
				++itr;
			}

			if( bCheck )
			{
				switch( GetDirection(&m_Player) )
				{
				case DIRECTION_RIGHT :
					if( Direction == DIRECTION_LEFT )
						SetDie();
					break;
				case DIRECTION_LEFT :
					if( Direction == DIRECTION_RIGHT )
						SetDie();
					break;
				case DIRECTION_UP :
					if( Direction == DIRECTION_DOWN )
						SetDie();
					break;
				case DIRECTION_DOWN :
					if( Direction == DIRECTION_UP )
						SetDie();
					break;
				}
			}

			m_Player.bCanRotation = true;		
			m_Player.MoveCount++;				
			m_Player.MoveTime = timeGetTime();
			MoveMonster();
		}
	}
}

void	C_VS_UI_ARROW_TILE::ActionRotation(BYTE Direction)					// 0�� ���� 1�� ������
{
	if( m_Stage == STAGE_NONE || m_Stage == STAGE_END || !m_Player.bCanRotation || m_Player.Status == PLAYER_STATUS_DIE)
		return;
	
	char Value = 0;
	switch( Direction )
	{
	case 0 :		
		Value = -1;
		break;
	case 1 :
		Value = 1;
		break;
	}

	// ���� �밡�� ȸ��
	if( Value != 0 )
	{
		for(int i=0;i<GetMapSize()*GetMapSize();i++)
		{
			int temp = m_Tile[i].Direction;
			m_Tile[i].Direction += Value;
			if(m_Tile[i].Direction >= MAX_DIRECTION)
				m_Tile[i].Direction -= MAX_DIRECTION;
			if(m_Tile[i].Direction < 0 )
				m_Tile[i].Direction += MAX_DIRECTION;
			if( temp < m_Tile[i].Direction )	// ���������� ���� ���
			{
				m_Tile[i].RemainRotationAngle = 3;
				m_Tile[i].AniFrame[2] = SPK_ARROW_UP+(temp*4) +1;
				m_Tile[i].AniFrame[1] = SPK_ARROW_UP+(temp*4) +2;
				m_Tile[i].AniFrame[0] = SPK_ARROW_UP+(temp*4) +3;
			} else
			{
				// �������� ���� ���
				if( temp == DIRECTION_RIGHT )
				{
					m_Tile[i].RemainRotationAngle = 3;
					m_Tile[i].AniFrame[2] = SPK_ARROW_LEFT_UP_1;
					m_Tile[i].AniFrame[1] = SPK_ARROW_LEFT_UP_2;
					m_Tile[i].AniFrame[0] = SPK_ARROW_LEFT_UP_3;
				} else
				{
					m_Tile[i].RemainRotationAngle = 3;
					m_Tile[i].AniFrame[2] = SPK_ARROW_UP+(m_Tile[i].Direction*4)+3;
					m_Tile[i].AniFrame[1] = SPK_ARROW_UP+(m_Tile[i].Direction*4)+2;
					m_Tile[i].AniFrame[0] = SPK_ARROW_UP+(m_Tile[i].Direction*4)+1;
				}
			}
		}
	}
	m_Player.bCanRotation = false;
}

void	C_VS_UI_ARROW_TILE::MoveMonster()
{
	// ���Ͱ� ������ �ð� -_- ��� �ʿ� ���µ� ��
	m_TimeMoveMonster = timeGetTime();
	
	std::list<S_CHARACTER*>::const_iterator itr = m_MonsterList.begin();
	std::list<S_CHARACTER*>::const_iterator enditr = m_MonsterList.end();
	
	while( itr != enditr )
	{
		S_CHARACTER *Monster = *itr;
		itr++;

		if(Monster == NULL)
			continue;
		
		switch( GetDirection( Monster ) )
		{
		case DIRECTION_UP :
			MoveUP( Monster );
			break;
		case DIRECTION_LEFT :
			MoveLEFT( Monster );
			break;
		case DIRECTION_DOWN :
			MoveDOWN( Monster );
			break;
		case DIRECTION_RIGHT :
			MoveRIGHT( Monster );
			break;
		}
	}
}

bool	C_VS_UI_ARROW_TILE::MoveRIGHT(S_CHARACTER *Character)
{
	TILE_ATTRIBUTE Attr = m_Tile[Character->Y*GetMapSize()+Character->X].Attr;
	
	if( Attr == ATTR_NORMAL || Attr == ATTR_SLIDING || (Character->PlayerType == PLAYER_MONSTER && Attr == ATTR_TRAP) )
	{
		Character->X ++;

		// �ⱸ���� �Ǻ��Ѵ�.
		if(Character->Y == GetMapSize() - 1 && Character->X == GetMapSize() && Character->PlayerType == PLAYER_USER )
		{
			m_Player.Status = PLAYER_STATUS_FINISH;
			m_Player.EndTime = timeGetTime();
			return true;
		} else
		if(Character->X >= GetMapSize() || IsExistMonster( Character->ID, Character->X, Character->Y ) )
		{
			Character->X --;
			return false;
		}
		return true;
	} 
	else if ( Attr == ATTR_2_MOVE )
	{
	}
	return false;
}

bool	C_VS_UI_ARROW_TILE::MoveLEFT(S_CHARACTER *Character)
{
	TILE_ATTRIBUTE Attr = m_Tile[Character->Y*GetMapSize()+Character->X].Attr;
	
	if( Attr == ATTR_NORMAL || Attr == ATTR_SLIDING || (Character->PlayerType == PLAYER_MONSTER && Attr == ATTR_TRAP) )
	{
		Character->X --;
		
		if(Character->X < 0 || IsExistMonster( Character->ID, Character->X, Character->Y ) )
		{
			Character->X ++;
			return false;
		}
		return true;
	} 
	else if ( Attr == ATTR_2_MOVE )
	{
	}
	return false;
}

bool	C_VS_UI_ARROW_TILE::MoveUP(S_CHARACTER *Character)
{
	TILE_ATTRIBUTE Attr = m_Tile[Character->Y*GetMapSize()+Character->X].Attr;
	
	if( Attr == ATTR_NORMAL || Attr == ATTR_SLIDING || (Character->PlayerType == PLAYER_MONSTER && Attr == ATTR_TRAP) )
	{
		Character->Y --;
		
		if(Character->Y < 0 || IsExistMonster( Character->ID, Character->X, Character->Y ) )
		{
			Character->Y ++;
			return false;
		}
		return true;
	} 
	else if ( Attr == ATTR_2_MOVE )
	{
	}
	return false;
}

bool	C_VS_UI_ARROW_TILE::MoveDOWN(S_CHARACTER *Character)
{
	TILE_ATTRIBUTE Attr = m_Tile[Character->Y*GetMapSize()+Character->X].Attr;
	
	if( Attr == ATTR_NORMAL || Attr == ATTR_SLIDING || (Character->PlayerType == PLAYER_MONSTER && Attr == ATTR_TRAP) )
	{
		Character->Y ++;
		
		if(Character->Y >= GetMapSize() || IsExistMonster( Character->ID, Character->X, Character->Y ) )
		{
			Character->Y --;
			return false;
		}
		return true;
	} 
	else if ( Attr == ATTR_2_MOVE )
	{
	}
	return false;
}

BYTE	C_VS_UI_ARROW_TILE::GetDirection(S_CHARACTER *Character)
{
	if( Character == NULL )
		return 0xFF;
	
	int offset = Character->Y * GetMapSize() + Character->X;
	
	if( offset >= GetMapSize() * GetMapSize() )
		return 0xFE;

	return m_Tile[offset].Direction;
}

void	C_VS_UI_ARROW_TILE::AddMonster()
{
	InitMonster();

	POINT MonsterPos_5X5[2] = { {3,3}, {4,4} };
	POINT MonsterPos_7X7[3] = { {6,3}, {3,6}, {6,6} };
	POINT MonsterPos_9X9[5] = { {4,4},{8,4},{4,8},{6,6},{8,8} };
	
	POINT *Pos = NULL;
	int nMonster=0;

	switch(m_Stage)
	{		
	case STAGE_5X5 :
		nMonster = sizeof(MonsterPos_5X5)/sizeof(POINT);
		Pos = MonsterPos_5X5;
		break;
	case STAGE_7X7 :
		nMonster = sizeof(MonsterPos_7X7)/sizeof(POINT);
		Pos = MonsterPos_7X7;
		break;
	case STAGE_9X9 :
		nMonster = sizeof(MonsterPos_9X9)/sizeof(POINT);
		Pos = MonsterPos_9X9;
		break;
	}
	
	if( nMonster == 0 || Pos == NULL )
		return;

	short ID = 1;
	for(int i=0;i<nMonster;i++)
	{
		S_CHARACTER *Character = new S_CHARACTER;
		Character->bActive = true;
		Character->bCanRotation = false;
		Character->DelayTime = 0;
		Character->ID = ID++;
		Character->MoveCount = 0;
		Character->MoveTime = 0;
		Character->PlayerType = PLAYER_MONSTER;
		Character->RotationCount = 0;
		Character->StartTime = timeGetTime();
		Character->Status =	PLAYER_STATUS_NORMAL;
		Character->X = Pos[i].x;
		Character->Y = Pos[i].y;
		m_MonsterList.push_back( Character );
	}
}

void	C_VS_UI_ARROW_TILE::InitGame()
{
	BYTE MapSize = GetMapSize();	
	
	if(MapSize != 0)
		m_Tile.reserve( MapSize * MapSize );
	else
		m_Tile.clear();

	srand(timeGetTime() );
	m_GameCode = rand() % 50000;
	
	srand(m_GameCode);
	for(int i=0;i<MapSize*MapSize;i++)
	{
		m_Tile[i].Attr = ATTR_NORMAL;
		m_Tile[i].Direction = rand()%DIRECTION_MAX;
		m_Tile[i].AniFrame[0] = 0;
		m_Tile[i].AniFrame[1] = 0;
		m_Tile[i].AniFrame[2] = 0;
		m_Tile[i].RemainRotationAngle =0;
	}

	InitMonster();	
	m_Player.ID = 0;
	m_Player.X = -1;
	m_Player.Y = 0;
	m_Player.Status = PLAYER_STATUS_READY;
	m_Player.bCanRotation = false;
	m_Player.MoveCount = 0;
	m_Player.RotationCount = 0;
	m_Player.StartTime = 0;
	m_Player.MoveTime = 0;
	m_Player.DelayTime = 0;
	m_Player.bActive = true;
	m_Player.PlayerType = PLAYER_USER;
	m_topScore = 0;
	m_MyBestScore = 0;
	m_topName ="";

	m_RemainCountMoveMonster = 0;
	m_bCanStart = false;
	AddMonster();
}

void	C_VS_UI_ARROW_TILE::InitMonster()
{
	if( m_MonsterList.empty() )
		return;

	std::list<S_CHARACTER*>::iterator itr = m_MonsterList.begin();
	std::list<S_CHARACTER*>::iterator enditr = m_MonsterList.end();

	while( itr != enditr )
	{
		S_CHARACTER* Character = *itr;
		if( Character == NULL )
		{
			++itr;
			continue;
		}

		(*itr) = NULL;
		++itr;
		
		delete Character;
	}
	m_MonsterList.clear();
}

void	C_VS_UI_ARROW_TILE::ResetGame()
{
}

void	C_VS_UI_ARROW_TILE::InitStage()
{
	if( m_Stage == STAGE_NONE )
		m_Stage = STAGE_5X5;

	if( m_Stage == STAGE_END )
		m_Stage = STAGE_NONE;


	switch( m_Stage )
	{
	case STAGE_5X5 :
		h = 300;
		break;
	case STAGE_7X7 :
		h = 380;
		break;
	case STAGE_END :
	case STAGE_9X9 :
		h = 460;
		break;		
	}

	Set(g_GameRect.right/2-w/2, g_GameRect.bottom/2 - h/2, w, h);
}

BYTE	C_VS_UI_ARROW_TILE::GetMapSize()
{
	switch( m_Stage )
	{
	case STAGE_NONE :
		return 0;
		break;
	case STAGE_5X5 :
		return 5;
		break;
	case STAGE_7X7 :
		return 7;
		break;
	case STAGE_END :	
	case STAGE_9X9 :
		return 9;
		break;
	}
	return 9;
}

bool	C_VS_UI_ARROW_TILE::TimerMonsterMove(DWORD len)
{
	if(timeGetTime() - m_TimeMoveMonster > len )
		return true;
	return false;
}

bool	C_VS_UI_ARROW_TILE::IsExistMonster(short id, int x, int y )
{
	// ���ͳ����� �浹�ϴ°� �������-_- ��
	if( id == 0 )
		return false;
	
	std::list<S_CHARACTER*>::const_iterator itr = m_MonsterList.begin();
	std::list<S_CHARACTER*>::const_iterator enditr = m_MonsterList.end();
	
	while( itr != enditr )
	{
		const S_CHARACTER *Character = *itr;
		if(Character == NULL )
		{
			++itr;
			continue;		
		}
		if( Character->X == x && Character->Y == y && Character->ID != id )
			return true;

		++itr;
	}
	return false;
}


void	C_VS_UI_ARROW_TILE::SetDie()
{
	if( m_Player.Status == PLAYER_STATUS_DIE )
		return;
	
	m_Player.Status = PLAYER_STATUS_DIE;
	m_Player.EndTime = timeGetTime();

	g_StartGameOverMessage(-1,-1);

//	gpC_base->SendMessage(UI_REQUEST_FORCE_DIE, 10 );
}

///////////////////////////////////////////////////////////////////
//
// CRAZY_MINE
//
///////////////////////////////////////////////////////////////////
C_VS_UI_CRAZY_MINE::C_VS_UI_CRAZY_MINE()
{		
	AttrTopmost( true );
	g_RegisterWindow(this);	
	AttrKeyboardControl(true);
	m_pC_button_group = NULL;
	
	int window_w,window_h;
	window_w = 300;
	window_h = 225;
	Set(g_GameRect.right/2-window_w/2, g_GameRect.bottom/2 - window_h/2, window_w, window_h);
	m_SPK.Open("Data\\UI\\SPK\\ArrowTile.spk");

	m_pC_button_group = new ButtonGroup(this);
	m_bStart = false;
	m_bCanStart = false;
	m_OIdMouseX = -1;
	m_OldMouseY = -1;
	m_LatestClickTime = 0;
}

C_VS_UI_CRAZY_MINE::~C_VS_UI_CRAZY_MINE()
{
	g_UnregisterWindow(this);	
	DeleteNew(m_pC_button_group);	
}

void	C_VS_UI_CRAZY_MINE::Start()
{
	PI_Processor::Start();
	AttrPin(true);
	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();
	InitMineBoard( 2 );
	
	Run(HELP_ID);
}

void	C_VS_UI_CRAZY_MINE::Finish()
{
	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);	
}

void	C_VS_UI_CRAZY_MINE::Show()
{
	int startX = x+40;
	int startY = y+40;
	int BoardSizeX = m_SPK.GetWidth(SPK_GOAL) + m_SPK.GetWidth(SPK_GOAL) + m_SPK.GetWidth(SPK_ARROW_BACK_1) * m_MineBoardSize;
	int BoardSizeY = m_SPK.GetHeight(SPK_ARROW_BACK_1) * m_MineBoardSize;

	RECT rect = { startX, startY, startX + BoardSizeX, startY + BoardSizeY };

	if(gpC_base->m_p_DDSurface_back->Lock())
	{		
		Rect rtOut(0,0,w,h);
		m_SPK.BltLockedClip( x,y,rtOut,SPK_BACK );				
		
		m_SPK.BltLockedClip( x,y,rtOut,SPK_OUT_BOX_LEFT_UP );		
		rtOut.Set(0,0, m_SPK.GetWidth(SPK_OUT_BOX_RIGHT), h );
		m_SPK.BltLockedClip( x+w,y, rtOut, SPK_OUT_BOX_RIGHT );

		
		rtOut.Set(0,0,w+1,m_SPK.GetHeight( SPK_OUT_BOX_DOWN ));		
		m_SPK.BltLockedClip( x,y+h, rtOut, SPK_OUT_BOX_DOWN );
		m_SPK.BltLocked(x+w+1,y+h,SPK_OUT_BOX_BOTTOM);

		DrawBoard();		

		gpC_base->m_p_DDSurface_back->Unlock();
	}

	rect.left = x+175;
	rect.top = y+60;
	rect.right = rect.left+100;
	rect.bottom = rect.top + 130;
	gpC_base->m_p_DDSurface_back->FillRect(&rect, CSDLGraphics::Color(3,3,3));
	gpC_global_resource->DrawOutBox(rect.left-2, rect.top-2, rect.right-rect.left+4, rect.bottom-rect.top+4);

	rect.left = x+40;
	rect.top = y+30;
	rect.right = rect.left + 70;
	rect.bottom = rect.top + 20;

	gpC_base->m_p_DDSurface_back->FillRect(&rect, CSDLGraphics::Color(3,3,3));
	gpC_global_resource->DrawOutBox(rect.left-2, rect.top-2, rect.right-rect.left+4, rect.bottom-rect.top+4);

	rect.left = x+200;
	rect.top = y+30;
	rect.right = rect.left + 90;
	rect.bottom = rect.top + 20;
	
//	gpC_base->m_p_DDSurface_back->FillRect(&rect, CSDLGraphics::Color(3,3,3));	
//	gpC_global_resource->DrawOutBox(Rect(rect.left-2, rect.top-2, rect.right-rect.left+4, rect.bottom-rect.top+4) );


	if( g_FL2_GetDC() )
	{
		char szTemp[256];
		
		wsprintf(szTemp, "%s Best Score : %s %d.%d",g_pUserInformation->WorldName.GetString(),
			m_topName.c_str(), m_topScore/100, m_topScore%100 );
		g_PrintColorStrOut(x+20, y+10, szTemp,gpC_base->m_item_desc_pi, RGB_WHITE,RGB_BLACK);				
		m_pC_button_group->ShowDescription();

		g_PrintColorStr(x+8,y+34,"TIME ", gpC_base->m_item_desc_pi, RGB_WHITE);
//		g_PrintColorStr(x+115,y+34,"My Best Score ", gpC_base->m_item_desc_pi, RGB_WHITE);
		
		if( m_bStart )
		{
			wsprintf(szTemp,"%5d.%2d",(timeGetTime()-m_StartTime)/1000,((timeGetTime()-m_StartTime)%1000)/10);
			g_PrintColorStr(x+49,y+34,szTemp, gpC_base->m_item_desc_pi, RGB_WHITE);
		}
		
//		wsprintf(szTemp,"%2d.%2d",m_MyBestScore/100, m_MyBestScore%100);
//		g_PrintColorStr( x+ 230, y+34, szTemp,gpC_base->m_item_desc_pi, RGB_WHITE);

		std::list<SelectHistory>::iterator itr = m_History.begin();
		std::list<SelectHistory>::iterator endItr = m_History.end();
		int cnt = 0;
		int line=0;

		bool bSel = false;
		if ( m_SelectedHistory >= 0 && m_SelectedHistory < m_History.size() )
		{
			bSel = true;
		}
		
		for ( ; itr != endItr ; ++itr )
		{
//			fNextLine += 40.0f;
			if ( itr->m_X == 3 || itr->m_O == 3 )
			{
				sprintf( szTemp, TEXT("3X or 3O") );
			}
			else
			{
				sprintf( szTemp, TEXT("%dX, %dO"), itr->m_X, itr->m_O );
			}
			if ( bSel && cnt++ == m_SelectedHistory )
				g_PrintColorStrOut(x+180, y+65+line, szTemp,gpC_base->m_item_name_pi, RGB_WHITE,RGB_RED);				
			else
				g_PrintColorStrOut(x+180, y+65+line, szTemp,gpC_base->m_item_name_pi, RGB_WHITE,RGB_BLACK);				
			line += 30;			
				
		}

		if ( m_bSuccess ) 
		{
			wsprintf(szTemp,"Success!");
			g_PrintColorStrOut(x+180, y+170, szTemp,gpC_base->m_item_name_pi, RGB_WHITE,RGB_BLACK);				
		} 
		else if ( m_bDead )
		{
			wsprintf(szTemp,"Dead!");
			g_PrintColorStrOut(x+180, y+170, szTemp,gpC_base->m_item_name_pi, RGB_WHITE,RGB_RED);				
		}

		wsprintf(szTemp,(*g_pGameStringTable)[STRING_MESSAGE_SEARCHING_MINE].GetString(),m_MineCount,m_FlagCount);
		g_PrintColorStrOut(x+20, y+205, szTemp, gpC_base->m_item_desc_pi, RGB_WHITE, RGB_BLACK);
		g_FL2_ReleaseDC();
	}
}

bool	C_VS_UI_CRAZY_MINE::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);

	switch(message)
	{
	case M_MOVING:
		break;

	case M_LEFTBUTTON_DOWN:
		if( m_OIdMouseX == _x && m_OldMouseY == _y && (GetTickCount() - m_LatestClickTime) < GetDoubleClickTime() )
		{
			if( _x > 30 && _y > 60 )
			{
				ActionDoubleClick(_x-30,_y-60);			
			}
		} else
		{	
			if ( _y > 60 && _x>30 && _x < 274 && _y < 189)
			{
				ActionClick(_x,_y);
			}
		}
		m_OIdMouseX = _x;
		m_OldMouseY = _y;
		m_LatestClickTime = GetTickCount();
		break;
//	case M_LB_DOUBLECLICK:
//		break;

	case M_RIGHTBUTTON_DOWN :
		if( _x > 30 && _y > 60 )
		{
			ActionRClick(_x-30,_y-60);
		}
		break;
	case M_LEFTBUTTON_UP:
		break;
	}	
	return true;
}

void	C_VS_UI_CRAZY_MINE::KeyboardControl(UINT message, UINT key, long extra)	
{	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{		
		case VK_ESCAPE :
			Run(CLOSE_ID);
			break;		
		case VK_RETURN :
			break;
		case VK_LEFT :
			break;
		case VK_RIGHT :
			break;
		}
	}
}

bool	C_VS_UI_CRAZY_MINE::IsPixel(int _x,int _y)
{
	return true;
}

void	C_VS_UI_CRAZY_MINE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	
}

void	C_VS_UI_CRAZY_MINE::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
}

void	C_VS_UI_CRAZY_MINE::WindowEventReceiver(id_t event)
{
}

void	C_VS_UI_CRAZY_MINE::Run(id_t id)
{
	switch( id )
	{
	case CLOSE_ID :
		gpC_base->SendMessage(UI_CLOSE_CRAZY_MINE);
		break;
	case HELP_ID :
		gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::MINI_GAME_CRAZY_MINE);
		break;
	}
}

void	C_VS_UI_CRAZY_MINE::Process()
{
	m_pC_button_group->Process();
}

void C_VS_UI_CRAZY_MINE::DrawBoard()
{
	int startX= 30+x;
	int startY =60+y;
	bool bHistory = false;
	SelectHistory sh;

	if ( m_SelectedHistory >= 0 && m_SelectedHistory < m_History.size() )
	{
		bHistory = true;
		std::list<SelectHistory>::iterator itr = m_History.begin();
		std::list<SelectHistory>::iterator endItr = m_History.end();

		for ( int i=0; i<m_SelectedHistory; ++i )
		{
			++itr;
			if ( endItr == itr )
			{
				bHistory = false;
				break;
			}
		}

		if ( endItr != itr ) sh = *itr;
	}

	for ( int i=0; i < m_MineBoardSize; ++i )
	{
		for ( int j=0; j < m_MineBoardSize; ++j )
		{
			int sprid = -1;
			int flagid = -1;
			
			if ( m_MineBoard[i][j].isFlag )
			{
				flagid = SPK_FLAG;
			}
			else if ( m_MineBoard[i][j].isQuestion )
			{
				flagid = SPK_QUESTION_MARK;
			}
			
			sprid = SPK_MINEGAME_NORMAL;
			
			
			
			if ( bHistory &&
				std::find(sh.m_Selected.begin(),sh.m_Selected.end(),std::pair<int,int>(i,j)) != sh.m_Selected.end() )
			{
				sprid = SPK_MINEGAME_YELLOW;
			}
			
			if ( m_MineBoard[i][j].isSelected ) 
				sprid = SPK_MINEGAME_GREEN;
			if ( m_MineBoard[i][j].isOpen ) 
			{
				if( sprid == SPK_MINEGAME_GREEN )
					sprid = SPK_MINEGAME_GREEN_PUSHED;
				else if( sprid == SPK_MINEGAME_YELLOW )
					sprid = SPK_MINEGAME_YELLOW_PUSHED;
				else
					sprid = SPK_MINEGAME_NORMAL_PUSHED;
			}

			m_SPK.BltLocked(i*m_SPK.GetWidth(SPK_MINEGAME_NORMAL)+startX,startY+j*m_SPK.GetHeight(SPK_MINEGAME_NORMAL), sprid);
			if(flagid != -1)
				m_SPK.BltLocked(i*m_SPK.GetWidth(SPK_MINEGAME_NORMAL)+startX+m_SPK.GetWidth(SPK_MINEGAME_NORMAL)/2 - m_SPK.GetWidth(flagid)/2,startY+j*m_SPK.GetHeight(SPK_MINEGAME_NORMAL) + m_SPK.GetHeight(SPK_MINEGAME_NORMAL)/2 - m_SPK.GetHeight(flagid)/2, flagid);
		}
	}

	Rect rt;
	int ArrowSizeX = m_SPK.GetWidth(SPK_MINEGAME_NORMAL);
	int ArrowSizeY = m_SPK.GetHeight(SPK_MINEGAME_NORMAL);

	rt.Set(0,0,ArrowSizeX*m_MineBoardSize+m_SPK.GetWidth(SPK_IN_BOX_RIGHT),ArrowSizeY*m_MineBoardSize +m_SPK.GetHeight(SPK_IN_BOX_DOWN));
	m_SPK.BltLockedClip( startX-m_SPK.GetWidth(SPK_IN_BOX_RIGHT), startY-m_SPK.GetHeight(SPK_IN_BOX_DOWN), rt, SPK_IN_BOX_LEFT_UP );
	rt.Set(0,0,ArrowSizeX*m_MineBoardSize+m_SPK.GetWidth(SPK_IN_BOX_RIGHT),m_SPK.GetHeight(SPK_IN_BOX_DOWN));
	m_SPK.BltLockedClip( startX+-m_SPK.GetWidth(SPK_IN_BOX_RIGHT),startY+ArrowSizeY*m_MineBoardSize, rt, SPK_IN_BOX_DOWN);
	
	rt.Set(0,0,m_SPK.GetWidth(SPK_IN_BOX_RIGHT),ArrowSizeY*m_MineBoardSize+m_SPK.GetHeight(SPK_IN_BOX_DOWN) );
	m_SPK.BltLockedClip( startX+ArrowSizeX*m_MineBoardSize, startY-m_SPK.GetHeight(SPK_IN_BOX_DOWN),rt, SPK_IN_BOX_RIGHT );		
	m_SPK.BltLocked(startX+ArrowSizeX*m_MineBoardSize-2, startY+ArrowSizeY*m_MineBoardSize,SPK_IN_BOX_BOTTOM);
}	

void C_VS_UI_CRAZY_MINE::ActionClick(int x, int y)
{
	if( !m_bCanStart )
		return;

	if ( y > 62 && x>176 && x < 274 && y < 189)
	{
		y-=62;
		y /= 30;

//		if ( y > 5 )
//		{
//			y -= 5;
//			switch(y)
//			{
//			case 1:
//				if ( m_MineCount < m_MineBoardSize*m_MineBoardSize ) m_MineCount++;
//				break;
//			case 2:
//				if ( m_MineCount > 1 ) m_MineCount--;
//				break;
//			case 3:
//				if ( m_MineBoardSize < 20 ) ++m_MineBoardSize;
//				break;
//			case 4:
//				if ( m_MineBoardSize > 2 ) m_MineBoardSize--;
//				break;
//			default:
//				return;
//			}	
//
//			InitMineBoard(m_MineBoardSize, m_MineCount);
//		}

		if ( y < m_History.size() )
		{
			m_SelectedHistory = y;
		}

		return;
	}

	if ( m_bDead ) return;

	x-=30;
	y-=60;

	x /=32;
	y /=32;

	if ( x >= 0 && x < m_MineBoardSize && y >= 0 && y < m_MineBoardSize )
	{
		if( m_bStart == false )
		{
			m_bStart = true;
			m_StartTime = timeGetTime();
		}
		m_MineBoard[x][y].isSelected = !m_MineBoard[x][y].isSelected;
		updateMembers();

		if ( m_SelectedCount == 3 )
		{
			m_History.push_back( MakeHistory() );
			if ( m_History.size() > 4 ) 
				m_History.pop_front();
			ClearSelected();
		}
	}
}

void C_VS_UI_CRAZY_MINE::ActionNewGame(int n)
{
	InitMineBoard(n);
}


void C_VS_UI_CRAZY_MINE::InitMineBoard(int size, int mine)
{
	int i; // Declare i at function scope

	m_topScore = 0;
	m_MyBestScore = 0;
	m_topName ="";
	srand(timeGetTime());

	m_MineBoardSize = size;
	if ( mine == -1 )
	{
		m_MineCount = pow( 2, m_MineBoardSize-1 );
		if ( m_MineCount > m_MineBoardSize * m_MineBoardSize ) m_MineCount = m_MineBoardSize * (m_MineBoardSize-2);
	}
	else
	{
		m_MineCount = mine;
	}

	for ( i=0; i<size; ++i )
	{
		for ( int j=0; j<size; ++j )
		{
			m_MineBoard[i][j].isFlag = false;
			m_MineBoard[i][j].isMine = false;
			m_MineBoard[i][j].isSelected = false;
			m_MineBoard[i][j].isOpen = false;
		}
	}

	for ( i=0; i<m_MineCount; ++i )
	{
		int x,y;

		do
		{
			x = rand()%m_MineBoardSize;
			y = rand()%m_MineBoardSize;
		} while( m_MineBoard[x][y].isMine );

		m_MineBoard[x][y].isMine=true;
	}

	m_SelectedCount = 0;
	m_FlagCount = 0;
	m_X = m_O = 0;
	m_bSuccess = false;
	m_bDead = false;
	m_History.clear();
	m_SelectedHistory = -1;
}

void C_VS_UI_CRAZY_MINE::ActionRClick(int x, int y)
{	
	if( !m_bCanStart )
		return;

	x /=32;
	y /=32;

	if ( x >= 0 && x < m_MineBoardSize && y >= 0 && y < m_MineBoardSize )
	{
		bool temp = m_MineBoard[x][y].isQuestion;
		m_MineBoard[x][y].isQuestion = m_MineBoard[x][y].isFlag&&!m_MineBoard[x][y].isQuestion;
		m_MineBoard[x][y].isFlag = !m_MineBoard[x][y].isFlag&&!temp;
		updateMembers();
//		CheckSuccess();
	}
}

void C_VS_UI_CRAZY_MINE::updateMembers()
{
	m_FlagCount = 0;
	m_SelectedCount = 0;
	m_O = m_X = 0;

	for ( int i=0; i<m_MineBoardSize; ++i )
		for ( int j=0; j<m_MineBoardSize; ++j )
		{
			if ( m_MineBoard[i][j].isFlag ) ++m_FlagCount;
			if ( m_MineBoard[i][j].isSelected )
			{
				++m_SelectedCount;
				if ( m_MineBoard[i][j].isMine ) ++m_O;
					else ++m_X;
			}
		}
}

void C_VS_UI_CRAZY_MINE::ClearSelected()
{
	for ( int i=0; i<m_MineBoardSize; ++i )
		for ( int j=0; j<m_MineBoardSize; ++j )
		{
			m_MineBoard[i][j].isSelected = false;
		}

	m_SelectedCount = 0;
	m_SelectedHistory = -1;
}

C_VS_UI_CRAZY_MINE::SelectHistory C_VS_UI_CRAZY_MINE::MakeHistory()
{
	SelectHistory ret;

	for ( int i=0; i<m_MineBoardSize; ++i )
		for ( int j=0; j<m_MineBoardSize; ++j )
		{
			if (m_MineBoard[i][j].isSelected)
			{
				ret.m_Selected.push_back( std::pair<int,int>(i,j) );
			}
		}

	ret.m_O = m_O;
	ret.m_X = m_X;

	return ret;
}

void C_VS_UI_CRAZY_MINE::CheckSuccess()
{
	m_bSuccess=true;

	for ( int i=0; i<m_MineBoardSize; ++i )
		for ( int j=0; j<m_MineBoardSize; ++j )
		{
			if ( !m_MineBoard[i][j].isOpen && !m_MineBoard[i][j].isMine )
			{
				m_bSuccess=false;
				break;
			}
		}

	if ( m_bSuccess ) { 
		if( m_MineBoardSize >= 4 )
		{
			m_bStart = false;
			m_bCanStart = false;
			gpC_base->SendMessage(UI_CLEAR_STAGE,MAKELONG(WORD(m_MineBoardSize-2),1), timeGetTime() - m_StartTime);
			g_StartAllStageClearMessage(-1, -1);
//			gpC_base->SendMessage(UI_CLEAR_ALL_STAGE,1);
		} else
		{
			m_bStart = false;
			m_bCanStart = false;
			gpC_base->SendMessage(UI_CLEAR_STAGE,MAKELONG(WORD(m_MineBoardSize-2),1), timeGetTime() - m_StartTime);
			InitMineBoard( m_MineBoardSize + 1 ); 
		}
	}
}

void C_VS_UI_CRAZY_MINE::ActionDoubleClick(int x, int y)
{
	if( !m_bCanStart )
		return;

	if ( m_bDead ) return;
	x /=32;
	y /=32;

	if ( x >= 0 && x < m_MineBoardSize && y >= 0 && y < m_MineBoardSize )
	{
		m_MineBoard[x][y].isSelected = false;
		if ( !m_MineBoard[x][y].isOpen && !m_MineBoard[x][y].isFlag )
		{
			m_MineBoard[x][y].isOpen = true;

			if ( m_MineBoard[x][y].isMine )
			{
				/// �ٿ���
				m_bDead = true;
				g_StartGameOverMessage(-1,-1);
//				gpC_base->SendMessage(UI_REQUEST_FORCE_DIE, 11);
			}
			else
			{
			}
		}
	}

	m_LatestClickTime = GetTickCount() - 5000;				// -_- 5�������� �س��´�~

	CheckSuccess();
}



//-----------------------------------------------------------------------------
// C_VS_UI_QUEST_INVENTORY::C_VS_UI_QUEST_INVENTORY
//
//
//-----------------------------------------------------------------------------
C_VS_UI_QUEST_INVENTORY::C_VS_UI_QUEST_INVENTORY(const MItem* pItem)
{
	g_RegisterWindow(this);

	int	close_button_offset_x, close_button_offset_y;
	int	help_button_offset_x, help_button_offset_y;

	m_pCodeSheet = const_cast<MItem*>(pItem);
	
	switch(g_eRaceInterface)
	{
	case RACE_SLAYER :
		m_FixPos.x = 13;
		m_FixPos.y = 25;
		close_button_offset_x = 294;
		close_button_offset_y = 204+10;
		help_button_offset_x = 40;
		help_button_offset_y = 204+10;

		m_SPK.Open(SPK_SLAYER_INVENTORY);
		break;
	case RACE_OUSTERS :
		
		m_FixPos.x = 25;
		m_FixPos.y = 35;
		close_button_offset_x = 307;
		close_button_offset_y = 224;
		help_button_offset_x = 53;
		help_button_offset_y = 224;

		m_SPK.Open(SPK_OUSTERS_INVENTORY);
		break;
	case RACE_VAMPIRE :
		
		m_FixPos.x = 17;
		m_FixPos.y = 19;
		close_button_offset_x = 294;
		close_button_offset_y = 204;
		help_button_offset_x = 40;
		help_button_offset_y = 204;

		m_SPK.Open(SPK_VAMPIRE_INVENTORY);
		break;
	}
	
	Set(80, g_GameRect.bottom/2 - m_SPK.GetHeight(INVENTORY_WINDOW)/2, m_SPK.GetWidth(INVENTORY_WINDOW), m_SPK.GetHeight(INVENTORY_WINDOW));
	
	
	//�����ư
	m_pC_button_group = new ButtonGroup(this);
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(help_button_offset_x, help_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION), HELP_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_button_offset_x, close_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_X), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_X), CLOSE_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_X));	
	
	m_Inventory.reserve( 6*10 );
	
	const std::list<TYPE_ITEM_OPTION> &Option = m_pCodeSheet->GetItemOptionList();
	
	std::list<TYPE_ITEM_OPTION>::const_iterator itr = Option.begin();
	std::list<TYPE_ITEM_OPTION>::const_iterator enditr = Option.end();

	int off=0;
	while( itr != enditr && Option.size() == 30)
	{
		TYPE_ITEM_OPTION io = *itr;
		
		m_Inventory[off] = char( (io & 0xf0) >> 4 );
		if( m_Inventory[off] == 0xf )
			m_Inventory[off] = -1;
		else
			m_Inventory[off]--;

		m_Inventory[off+1] = char(io & 0x0f);
		if( m_Inventory[off+1] == 0xf )
			m_Inventory[off+1] = -1;
		else
			m_Inventory[off+1]--;

		off+=2;

		++itr;
	}

	m_ItemFrameMap[0]=737;
	m_ItemFrameMap[1]=733;
	m_ItemFrameMap[2]=738;
	m_ItemFrameMap[3]=735;
	m_ItemFrameMap[4]=734;
}

//-----------------------------------------------------------------------------
// C_VS_UI_QUEST_INVENTORY::~C_VS_UI_QUEST_INVENTORY
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_QUEST_INVENTORY::~C_VS_UI_QUEST_INVENTORY()
{
	g_UnregisterWindow(this);

	DeleteNew(m_pC_button_group);
}

//-----------------------------------------------------------------------------
// C_VS_UI_QUEST_INVENTORY::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_QUEST_INVENTORY::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	
}


//-----------------------------------------------------------------------------
// C_VS_UI_QUEST_INVENTORY_WINDOW::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_QUEST_INVENTORY::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetFocusState())
	{
		if(p_button->GetPressState())
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
	}
	else
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index);

}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_QUEST_INVENTORY::WindowEventReceiver(id_t event)
{
}


//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_QUEST_INVENTORY::Run(id_t id)
{
	// ��ư�� ��Ȱ�������̸� ����!
	switch (id)
	{
	case OK_ID:		
		break;		
	case CLOSE_ID:
		EMPTY_MOVE;
		gC_vs_ui.CloseQuestInventory();
		break;
	case HELP_ID :
		break;	
	}
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_QUEST_INVENTORY::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;
	m_CurMousePos = -1;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);
	
	switch(message)
	{
	case M_MOVING:
		if( _x >= m_FixPos.x && _x < m_FixPos.x + 30*10 && _y >= m_FixPos.y && _y < m_FixPos.y + 30*6 )
			m_CurMousePos = ((_y-m_FixPos.y)/30)*10+((_x-m_FixPos.x)/30);
		break;
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:		
		if( _x >= m_FixPos.x && _x < m_FixPos.x + 30*10 && _y >= m_FixPos.y && _y < m_FixPos.y + 30*6 )
			m_CurMousePos = ((_y-m_FixPos.y)/30)*10+((_x-m_FixPos.x)/30);

		if (re)
		{
			if( gpC_mouse_pointer->GetPickUpItem()==NULL )
			{
				MoveReady();
				SetOrigin(_x, _y);
			} else
			{
				const MItem* pItem = gpC_mouse_pointer->GetPickUpItem();
				if( pItem == NULL || m_CurMousePos == -1 )
					return false;
				
				DropToQuestInventory( pItem );
			}
		}
		break;
	}
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_QUEST_INVENTORY
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_QUEST_INVENTORY::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_ESCAPE:
			Run(CLOSE_ID);
			break;

		case VK_RETURN :
//			Run(OK_ID);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// Show
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_QUEST_INVENTORY::Show()
{
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_SPK.BltLocked(x,y,INVENTORY_WINDOW);
		
		if( m_CurMousePos >= 0 && m_CurMousePos < 60)
		{
			if( gpC_mouse_pointer->GetPickUpItem() == NULL || gpC_mouse_pointer->GetPickUpItem()->GetItemClass() != ITEM_CLASS_EVENT_ITEM )
				m_SPK.BltLocked(x+m_FixPos.x+30*(m_CurMousePos%10), y+m_FixPos.y+30*(m_CurMousePos/10), INVENTORY_ITEMBACK);
			else
			{
				const MItem *pItem = gpC_mouse_pointer->GetPickUpItem();
					
				if( CanDropToInventory(pItem) )
				{
					m_SPK.BltLocked(x+m_FixPos.x+30*(m_CurMousePos%10), y+m_FixPos.y+30*(m_CurMousePos/10), INVENTORY_ITEMBACK);
					gpC_item->BltLockedOutlineOnly( x+m_FixPos.x+30*(m_CurMousePos%10) + 15 - gpC_item->GetWidth(pItem->GetInventoryFrameID())/2,
						y+m_FixPos.y+30*(m_CurMousePos/10)+15-gpC_item->GetHeight(pItem->GetInventoryFrameID())/2,WHITE, pItem->GetInventoryFrameID() );
				}
				else
				{
					m_SPK.BltLocked(x+m_FixPos.x+30*(m_CurMousePos%10), y+m_FixPos.y+30*(m_CurMousePos/10), INVENTORY_ITEMBACK);
					gpC_item->BltLockedOutlineOnly( x+m_FixPos.x+30*(m_CurMousePos%10) + 15 - gpC_item->GetWidth(pItem->GetInventoryFrameID())/2,
						y+m_FixPos.y+30*(m_CurMousePos/10)+15-gpC_item->GetHeight(pItem->GetInventoryFrameID())/2,RED, pItem->GetInventoryFrameID() );						
				}
			}
			
		} else
			m_CurMousePos = -1;

		const int minType = 22;
		for(int iy=0;iy<6;iy++)
		{
			for(int ix=0;ix<10;ix++)
			{
				if(m_Inventory[iy*10+ix] == -1 )
					continue;
				m_SPK.BltLocked(x+m_FixPos.x+30*(ix), y+m_FixPos.y+30*(iy), INVENTORY_ITEMBACK);
				gpC_item->BltLocked( x+m_FixPos.x+30*(ix) + 15 - gpC_item->GetWidth(m_ItemFrameMap[0])/2,
					y+m_FixPos.y+30*(iy)+15-gpC_item->GetHeight(m_ItemFrameMap[0])/2, m_ItemFrameMap[m_Inventory[iy*10+ix]] );
				
			}
		}
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	m_pC_button_group->ShowDescription();
	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_QUEST_INVENTORY::Start()
{
	PI_Processor::Start();

	m_pC_button_group->Init();
	gpC_window_manager->AppearWindow(this);	
}

void	C_VS_UI_QUEST_INVENTORY::Finish()
{
	PI_Processor::Finish();	
	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_QUEST_INVENTORY::Process()
{
	m_pC_button_group->Process();	
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_QUEST_INVENTORY::IsPixel(int _x, int _y)
{
	return m_SPK.IsPixel(SCR2WIN_X(_x),SCR2WIN_Y(_y),INVENTORY_WINDOW);		
}

void	C_VS_UI_QUEST_INVENTORY::SetInventory(int i, BYTE Option)
{
	int offset = i*2;

	m_Inventory[i] = Option >> 4;
	m_Inventory[i+1] = (Option&0xF);
}

void	C_VS_UI_QUEST_INVENTORY::DropToQuestInventory( const MItem* pItem)
{
	const int minType = 22;
	if(! CanDropToInventory( pItem ) || m_CurMousePos < 0 || m_CurMousePos >= 60)
		return;

	// �������� ���� �� �ִ�.

	gpC_base->SendMessage(UI_ADD_ITEM_TO_CODE_SHEET, MAKELONG(m_CurMousePos/10,m_CurMousePos%10), pItem->GetItemType() - minType ,(void*) m_pCodeSheet );
}

bool	C_VS_UI_QUEST_INVENTORY::CanDropToInventory(const MItem* pItem)
{
	// �� üũ�غ���.
	const int minType = 22;
	
	int InvenX = m_CurMousePos%10;
	int InvenY = m_CurMousePos/10;
	char ItemType = pItem->GetItemType() - minType;

	if(! ( pItem->GetItemType() >= 22 && pItem->GetItemType() <= 26 ) )
		return false;
	
	if( InvenX < 0 || InvenX >= 10 || InvenY < 0 || InvenY >= 6 )
		return false;

	// 4���� ������ üũ�ؾ� �Ѵ�. ���� ������ Ÿ���� ������ ��ȿ!
	bool	bCanDrop = true;

	if( m_Inventory[InvenY*10+InvenX] != -1 )
		return false;

	// ���� -_-
	if( InvenX > 0 )
	{
		 if( m_Inventory[InvenY*10+InvenX-1] == ItemType )
			 bCanDrop = false;
	}
	// ������ -_-
	if( InvenX < 9 )
	{
		if( m_Inventory[InvenY*10+InvenX+1] == ItemType )
			 bCanDrop = false;		
	}
	// ���� -_-
	if( InvenY > 0 )
	{
		if( m_Inventory[(InvenY-1)*10+InvenX] == ItemType )
			 bCanDrop = false;		
	}
	// �Ʒ��� -_-
	if( InvenY < 5 )
	{
		if( m_Inventory[(InvenY+1)*10+InvenX] == ItemType )
			 bCanDrop = false;		
	}

	return bCanDrop;
}

//-----------------------------------------------------------------------------
// C_VS_UI_STATUS_CTF::C_VS_UI_STATUS_CTF
//
//
//-----------------------------------------------------------------------------
C_VS_UI_STATUS_CTF::C_VS_UI_STATUS_CTF()
{		
	m_pC_spk.Open(SPK_CTF_STATUS);

	int window_w = m_pC_spk.GetWidth(MAIN_WINDOW);
	int window_h = m_pC_spk.GetHeight(MAIN_WINDOW);
	
	Set(g_GameRect.right/2 - window_w/2, 60, m_pC_spk.GetWidth(MAIN_WINDOW), m_pC_spk.GetHeight(MAIN_WINDOW));
	
	g_RegisterWindow(this);
	
	//�����ư
	m_pC_button_group = new ButtonGroup(this);
	
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(248, 7, m_pC_spk.GetWidth(HELP_BUTTON), 
		m_pC_spk.GetHeight(HELP_BUTTON), HELP_ID, this, HELP_BUTTON));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(-2, 9, 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), 
		PUSHPIN_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN));

	m_finish_time = 0;
	for(int i =0;i<3;i++)
		m_num_flag[i] = 0;	
#ifndef _LIB
	m_finish_time = timeGetTime() + 2*20*60*1000;

	m_num_flag[0] = rand()%100;
	m_num_flag[1] = rand()%100;
	m_num_flag[2] = rand()%100;
	
#endif

	gpC_vs_ui_window_manager->SetAutoHide( C_VS_UI_WINDOW_MANAGER::CTF_STATUS, ATTRIBUTES_HIDE_NOT );
}

//-----------------------------------------------------------------------------
// C_VS_UI_STATUS_CTF::~C_VS_UI_STATUS_CTF
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_STATUS_CTF::~C_VS_UI_STATUS_CTF()
{
	g_UnregisterWindow(this);
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::CTF_STATUS, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::CTF_STATUS, Rect(x, y, w, h));
	
	DeleteNew(m_pC_button_group);
}

//-----------------------------------------------------------------------------
// C_VS_UI_STATUS_CTF::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_STATUS_CTF::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{	
	const char* description_msg[3] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_ON].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_OFF].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP].GetString()
	};
	
	switch( p_button->GetID() )
	{
	case PUSHPIN_ID :
		if(GetAttributes()->autohide)
			g_descriptor_manager.Set(DID_INFO, x+p_button->x, y+h-p_button->y-p_button->h, (void *)description_msg[1],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, x+p_button->x, y+h-p_button->y-p_button->h, (void *)description_msg[0],0,0);
		break;
	case HELP_ID :
		g_descriptor_manager.Set(DID_INFO, x+p_button->x, y+h-p_button->y-p_button->h, (void *)description_msg[2],0,0);
		break;
	}
}


//-----------------------------------------------------------------------------
// C_VS_UI_STATUS_CTF_WINDOW::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_STATUS_CTF::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	switch(p_button->GetID())
	{
	case OK_ID :
		break;
	case CLOSE_ID :
		break;
	case HELP_ID :
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
			{
				m_pC_spk.Blt(p_button->x+x+1, p_button->y+y, p_button->m_image_index-2);
			}
			else
				m_pC_spk.Blt(p_button->x+x, p_button->y+y, p_button->m_image_index-1);
		} else
			m_pC_spk.Blt(p_button->x+x, p_button->y+y, p_button->m_image_index);
		break;
	case PUSHPIN_ID :		
		if(GetAttributes()->autohide)
			gpC_global_resource->m_pC_assemble_box_button_spk->Blt(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->Blt(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN_PUSHED);
		break;
	}
	
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_STATUS_CTF::WindowEventReceiver(id_t event)
{
}


//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_STATUS_CTF::Run(id_t id)
{
	switch (id)
	{
	case OK_ID:		
		break;		
	case CLOSE_ID:
		break;
	case HELP_ID :
		gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::CTF_WINDOW);
		break;	
	case PUSHPIN_ID :
		if(GetAttributes()->autohide)
			AttrAutoHide(ATTRIBUTES_HIDE_NOT);
		else
		{
			AttrAutoHide(ATTRIBUTES_HIDE_WIDTH);
		}		
		break;
	}
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_STATUS_CTF::MouseControl(UINT message, int _x, int _y)
{
	static char szBuffer[256];
	RECT rectRemainTime = {18,9, 74,23};
	RECT rectFlag[3] = {
		{101,9, 129,23},						// SLAYER
		{151,9, 179,23},						// VAMPIRE
		{200,9, 228,23}							// OUSTERS
	};	

	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);
	
	switch(message)
	{
	case M_MOVING:
		{
			if( _x > rectRemainTime.left && _x < rectRemainTime.right && _y > rectRemainTime.top && _y < rectRemainTime.bottom )
			{
				DWORD RemainTime = m_finish_time - timeGetTime();
				wsprintf( szBuffer,"%s : %d:%d:%d",(*g_pGameStringTable)[UI_STRING_MESSAGE_STATUS_LEFT_TIME].GetString() ,
					((RemainTime/1000)/60)/60, ((RemainTime/1000)/60)%60,(RemainTime/1000)%60);

				g_descriptor_manager.Set(DID_INFO, x+_x, y+_y, (void *)szBuffer,0,0);
			} else
			{
				int RaceString[3] = {STRING_MESSAGE_SLAYER, STRING_MESSAGE_VAMPIRE, STRING_MESSAGE_OUSTERS};
				for(int i = 0 ; i<3 ; i ++ )
				{
					if( _x > rectFlag[i].left && _x < rectFlag[i].right &&
						_y > rectFlag[i].top && _y < rectFlag[i].bottom )
					{
						char szTemp[256];

						memset( szBuffer, 0, 256 );
						strcat( szBuffer, (*g_pGameStringTable)[RaceString[i]].GetString() );
						strcat( szBuffer, " ");
						wsprintf( szTemp, (*g_pGameStringTable)[UI_STRING_MESSAGE_GET_EVENT_FLAG_STATUS].GetString(), m_num_flag[i] );
						strcat( szBuffer, szTemp );
						g_descriptor_manager.Set(DID_INFO, x+_x, y+_y, (void *)szBuffer,0,0);
						break;
					}
				}		
			}
		}
		break;
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if (gpC_mouse_pointer->GetPickUpItem()==FALSE && re)
		{
			MoveReady();
			SetOrigin(_x, _y);
		}
		break;
	}
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_STATUS_CTF
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_STATUS_CTF::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);

	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_ESCAPE:
			Run(CLOSE_ID);
			break;

		case VK_RETURN :
			Run(OK_ID);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// Show
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_STATUS_CTF::Show()
{
	int i; // Declare i at function scope

	RECT rectRemainTime = {18,9, 74,23};
	RECT rectFlag[3] = {
		{101,9, 129,23},						// SLAYER
		{151,9, 179,23},						// VAMPIRE
		{200,9, 228,23}							// OUSTERS
	};

	m_pC_spk.Blt(x,y,MAIN_WINDOW);
	m_pC_button_group->Show();

	if( g_FL2_GetDC() )
	{
		int topscore = 1;
		for(i=0;i<3;i++)
		{
			if( topscore < m_num_flag[i] )
				topscore = m_num_flag[i];			
		}
		
		if( m_num_flag[0] == m_num_flag[1] && m_num_flag[1] == m_num_flag[2] )
			topscore = -1;
		
		// �����ð� ���
		char szBuffer[64],min[5],sec[5];
		DWORD RemainTime = m_finish_time - timeGetTime();
		if( (RemainTime/1000)/60/60 > 3 )
			RemainTime = 0;
		
		int time = ((RemainTime/1000)/60)%60;
		if( time < 10 )	wsprintf(min,"0%d",time);
		else wsprintf(min,"%d",time);
		
		time = (RemainTime/1000)%60;
		if( time < 10 )	wsprintf(sec,"0%d",time);
		else wsprintf(sec,"%d",time);		

		wsprintf( szBuffer, "%d:%s:%s",(RemainTime/1000)/60/60,min,sec);
		g_PrintColorStr(
			x+rectRemainTime.left+(rectRemainTime.right - rectRemainTime.left)/2 - g_GetStringWidth( szBuffer, gpC_base->m_chatting_pi.hfont )/2,
			y+rectRemainTime.top+(rectRemainTime.bottom - rectRemainTime.top)/2 - g_GetStringHeight( szBuffer, gpC_base->m_chatting_pi.hfont)/2, 
			szBuffer, gpC_base->m_chatting_pi, RGB_WHITE);

		for( i =0 ; i< 3; i++ )
		{
			wsprintf( szBuffer, "%d",m_num_flag[i]);
			if( m_num_flag[i] == topscore )
			{
				g_PrintColorStr(
					x+rectFlag[i].left+(rectFlag[i].right - rectFlag[i].left)/2 - g_GetStringWidth( szBuffer, gpC_base->m_desc_menu_pi.hfont )/2,
					y+rectFlag[i].top+(rectFlag[i].bottom - rectFlag[i].top)/2 - g_GetStringHeight( szBuffer, gpC_base->m_desc_menu_pi.hfont)/2, 
					szBuffer, gpC_base->m_desc_menu_pi, RGB_YELLOW);
			} else
			{
				g_PrintColorStr(
					x+rectFlag[i].left+(rectFlag[i].right - rectFlag[i].left)/2 - g_GetStringWidth( szBuffer, gpC_base->m_chatting_pi.hfont )/2,
					y+rectFlag[i].top+(rectFlag[i].bottom - rectFlag[i].top)/2 - g_GetStringHeight( szBuffer, gpC_base->m_chatting_pi.hfont)/2, 
					szBuffer, gpC_base->m_chatting_pi, RGB_WHITE);
			}
		}	
		m_pC_button_group->ShowDescription();
		g_FL2_ReleaseDC();
	}
	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_STATUS_CTF::Start()
{
	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::CTF_STATUS));	
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::CTF_STATUS);

	if(rect.w != -1)
	{
		x = rect.x;
		y = rect.y;
	}	
	PI_Processor::Start();

	m_pC_button_group->Init();
	gpC_window_manager->AppearWindow(this);	
}

void	C_VS_UI_STATUS_CTF::Finish()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::CTF_STATUS, GetAttributes()->alpha);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::CTF_STATUS, Rect(x, y, w, h));
	
	PI_Processor::Finish();	
	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_STATUS_CTF::Process()
{
	m_pC_button_group->Process();	
	ProcessHide();
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_STATUS_CTF::IsPixel(int _x, int _y)
{
	//m_pC_monitor_spk->IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y), MONITOR_WINDOW);
	return m_pC_spk.IsPixel(SCR2WIN_X(_x),SCR2WIN_Y(_y),MAIN_WINDOW) | (m_pC_button_group->IsInRect(_x - x, _y -y)!=NULL);
}

void	C_VS_UI_STATUS_CTF::SetStatus(DWORD &endtime, int &flag_s, int &flag_v, int &flag_o)
{
	m_num_flag[0] = flag_s;
	m_num_flag[1] = flag_v;
	m_num_flag[2] = flag_o;

	m_finish_time = endtime;
}

//-----------------------------------------------------------------------------
// C_VS_UI_REGEN_TOWER_MINIMAP::C_VS_UI_REGEN_TOWER_MINIMAP
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_REGEN_TOWER_MINIMAP::C_VS_UI_REGEN_TOWER_MINIMAP(DWORD timer)
{
	g_RegisterWindow(this);	
	AttrPin(true);
	
	m_image_spk.Open(RPK_HOLYLAND_MINIMAP);

	Set( (g_GameRect.right-m_image_spk.GetWidth(MINIMAP))/2, (g_GameRect.bottom - m_image_spk.GetHeight() ) /2, m_image_spk.GetWidth(MINIMAP), m_image_spk.GetHeight(MINIMAP));
	
	//timer
	m_dw_timer_tickcount = timer;
	m_selected = -1;
	Timer(true);	
}

//-----------------------------------------------------------------------------
// C_VS_UI_REGEN_TOWER_MINIMAP::~C_VS_UI_REGEN_TOWER_MINIMAP
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_REGEN_TOWER_MINIMAP::~C_VS_UI_REGEN_TOWER_MINIMAP()
{
	g_UnregisterWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_REGEN_TOWER_MINIMAP::Show
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_REGEN_TOWER_MINIMAP::Show()
{
	if (gpC_base->m_p_DDSurface_back->Lock())
	{
		m_image_spk.BltLocked( x, y, MINIMAP );
		RECT rt[3] = 
		{
			{ 4, 4, 104, 104 },
			{ 104, 4, 204, 104 },
			{ 204, 4, 304, 104 }
		};

		const int map_width = 128, map_height=256;
		
		for(int i = 0; i < g_pRegenTowerInfoManager->GetSize(); i++ )
		{
			int index,cx,cy,image;
			const RegenTowerInfo* pInfo = &g_pRegenTowerInfoManager->Get(i);
			
			index = 2 - (pInfo->zoneID - 71);
			
			cx = pInfo->x * (rt[index].right-rt[index].left) / map_width;
			cy = pInfo->y * (rt[index].bottom-rt[index].top) / map_height;

			switch( pInfo->owner )
			{
			case RACE_SLAYER :
				image = SLAYER_EYE;
				break;
			case RACE_VAMPIRE :
				image = VAMPIRE_EYE;
				break;
			case RACE_OUSTERS :
				image = OUSTERS_EYE;
				break;
			default :
				image = GRAY_EYE;
				break;
			}
			
			m_image_spk.BltLocked( x+cx+rt[index].left, y+cy+rt[index].top, image );
		}
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	if( !Timer() )
		gpC_base->SendMessage(UI_CLOSE_SHRINE_MINIMAP,1 );
}

//-----------------------------------------------------------------------------
// C_VS_UI_REGEN_TOWER_MINIMAP::IsPixel
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_REGEN_TOWER_MINIMAP::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// C_VS_UI_REGEN_TOWER_MINIMAP::Start
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_REGEN_TOWER_MINIMAP::Start()
{
	PI_Processor::Start();
	
	gpC_window_manager->AppearWindow(this);
	
	m_bl_pushed = false;
	m_bl_focused = false;

	EMPTY_MOVE;
}

//-----------------------------------------------------------------------------
// C_VS_UI_REGEN_TOWER_MINIMAP::Finish
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_REGEN_TOWER_MINIMAP::Finish()
{
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_REGEN_TOWER_MINIMAP::MouseControl
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_REGEN_TOWER_MINIMAP::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	RECT rt[3] = 
	{
		{ 4, 4, 104, 104 },
		{ 104, 4, 204, 104 },
		{ 204, 4, 304, 104 }
	};

	const int map_width = 128, map_height=256;

	_x-=x; _y-=y;
	switch(message)
	{
	case M_MOVING:
		{
			m_selected = -1;
			for(int i = 0; i < g_pRegenTowerInfoManager->GetSize(); i++ )
			{
				int index,cx,cy;
				const RegenTowerInfo* pInfo = &g_pRegenTowerInfoManager->Get(i);
				
				index = 2 - (pInfo->zoneID - 71);
				
				cx = pInfo->x * (rt[index].right-rt[index].left) / map_width + rt[index].left;
				cy = pInfo->y * (rt[index].bottom-rt[index].top) / map_height + rt[index].top;
				
				Rect rt( cx,cy, m_image_spk.GetWidth(SLAYER_EYE), m_image_spk.GetHeight( SLAYER_EYE ) );

				if( g_eRaceInterface == pInfo->owner && rt.IsInRect( _x, _y ) )
				{
					m_selected = i;
					g_descriptor_manager.Set(DID_INFO, _x + x, _y + y, (void *)(*g_pGameStringTable)[UI_STRING_MESSAGE_CLICK_TO_WARP_REGEN_TOWER].GetString() ,0,0);
				}
			}
		}
		break;
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if( m_selected != -1 )
		{
			gpC_base->SendMessage(UI_WARP_REGEN_TOWER, m_selected);
		}else
		if (!gpC_mouse_pointer->GetPickUpItem())
		{
			MoveReady();
			SetOrigin(_x, _y);
			break;
		}
		break;
	}
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_REGEN_TOWER_MINIMAP::Timer
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_REGEN_TOWER_MINIMAP::Timer(bool reset)
{
	if(reset)
	{
		m_dw_prev_tickcount = GetTickCount();
	}
	else if(m_dw_prev_tickcount+m_dw_timer_tickcount >= GetTickCount())
	{
		return true;
	}
	
	return false;
}

void	C_VS_UI_REGEN_TOWER_MINIMAP::KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_KEYDOWN && key == VK_ESCAPE)
	{
		gpC_base->SendMessage(UI_CLOSE_SHRINE_MINIMAP,1 );
	}
}

////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// C_VS_UI_PET_INFO::C_VS_UI_PET_INFO
//
//
//-----------------------------------------------------------------------------
C_VS_UI_PET_INFO::C_VS_UI_PET_INFO(PETINFO *petInfo)
{	
	if(petInfo == NULL)
		return;

	m_PetInfo = *petInfo;
	if(m_PetInfo.ITEM_TYPE == 2 && m_PetInfo.ChangeAbleNickName == true)
		m_IsWolverine = true;
	else
		m_IsWolverine = false;
	// add by coffee 2006-12-21  ����ʥ����������
	//if (m_PetInfo.ITEM_TYPE==6)
	//{
	//	m_PetInfo.NAME="¹";
	//}
	switch(m_PetInfo.ITEM_TYPE)
	{
	// WOLF DOG
	case 1:
	case 2:
		m_pC_spk.Open(SPK_PET_INFO_WOLFDOG);
		m_MainWindowPosition.x = 0;
		m_MainWindowPosition.y = 0;
		break;

	case 3:
		m_pC_spk.Open(SPK_PET_INFO_CENTAURO);
		m_MainWindowPosition.x = 2;
		m_MainWindowPosition.y = -38;
		break;

	case 4:
		m_pC_spk.Open(SPK_PET_INFO_STIRGE);
		m_MainWindowPosition.x = -46;
		m_MainWindowPosition.y = -40;
		break;

	case 5:
		m_pC_spk.Open(SPK_PET_INFO_PIXIE);
		m_MainWindowPosition.x = 2;
		m_MainWindowPosition.y = -24;
		break;

		// lupeng	add by viva
	case 6:
		m_pC_spk.Open(SPK_PET_INFO_LUPENG);
		m_MainWindowPosition.x = 0;
		m_MainWindowPosition.y = 0;
		break;
	case 7:	//MilitaryBear
		m_pC_spk.Open(SPK_PET_INFO_MILITARYBEAR);
		m_MainWindowPosition.x = 0;
		m_MainWindowPosition.y = 0;
		break;

	case 8:	//Gargoyle
		m_pC_spk.Open(SPK_PET_INFO_GARGOYLE);
		m_MainWindowPosition.x = 0;
		m_MainWindowPosition.y = 0;
		break;

	default:
		return;
	}

	m_pC_pet_face.Open(SPK_PET_FACE);
	int window_w = 261;//m_pC_spk.GetWidth(MAIN_WINDOW);
	int window_h = 246;//m_pC_spk.GetHeight(MAIN_WINDOW);
	
	Set(g_GameRect.right/2 - window_w/2, 60, window_w, window_h);
	
	g_RegisterWindow(this);

	AttrPin(true);
	
	//�����ư
	m_pC_button_group = new ButtonGroup(this);
	
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(223, h-19, m_pC_spk.GetWidth(CLOSE_BUTTON), m_pC_spk.GetHeight(CLOSE_BUTTON), CLOSE_ID, this, CLOSE_BUTTON));
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(198, h-22, m_pC_spk.GetWidth(GAMBLE_BUTTON), m_pC_spk.GetHeight(GAMBLE_BUTTON), GAMBLE_ID, this, GAMBLE_BUTTON));
	
	if(m_IsWolverine)
	{
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(25, h-22, m_pC_spk.GetWidth(NICKNAME_BUTTON), m_pC_spk.GetHeight(NICKNAME_BUTTON), NICKNAME_ID, this, NICKNAME_BUTTON));

		m_AddName.SetPosition(x+50, y+227);
		m_AddName.SetPrintInfo(gpC_base->m_chatting_pi);
		m_AddName.SetByteLimit(22);
		m_AddName.SetInputStringColor(RGB_WHITE);
		m_AddName.SetCursorColor(RGB_WHITE);
		m_AddName.SetDigitOnlyMode(false);
		m_AddName.SetAbsWidth(134);
		if(m_PetInfo.NICK_NAME.size())
			m_AddName.AddString(m_PetInfo.NICK_NAME.c_str());
		Attach(&m_AddName);
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_PET_INFO::~C_VS_UI_PET_INFO
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_PET_INFO::~C_VS_UI_PET_INFO()
{
	g_UnregisterWindow(this);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::PET_INFO, Rect(x, y, w, h));

	if(m_IsWolverine)
	{
		m_AddName.Unacquire();
		gC_vs_ui.AcquireChatting();
	}
	DeleteNew(m_pC_button_group);
}

//-----------------------------------------------------------------------------
// C_VS_UI_PET_INFO::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_PET_INFO::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{	
	const char* description_msg[6] = {
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_PET_INFO].GetString(), //"�ֿϵ��� ����â�� �ݽ��ϴ�.";
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CAN_ENCHANT_PET].GetString(), //"2�� �ɷ��� �ο��� �� �ֽ��ϴ�.";
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_ENCHANT_PET].GetString(), //"������ 2�� �ɷ��� �ο��� �� �����ϴ�."
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CAN_3RD_ENCHANT_PET].GetString(),	//"3�� �ɷ��� �ο��� �� �ֽ��ϴ�.";	
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CANNOT_3RD_ENCHANT_PET].GetString(),	//"������ 3�� �ɷ��� �ο��� �� �����ϴ�."
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CHANGE_PET_NICKNAME].GetString()	//] = "���� �г����� �����մϴ�.";
	};
	
	switch( p_button->GetID() )
	{
	case NICKNAME_ID:
		g_descriptor_manager.Set(DID_INFO, x+p_button->x, y+p_button->y, (void *)description_msg[5],0,0);
		break;
	case CLOSE_ID :
		g_descriptor_manager.Set(DID_INFO, x+p_button->x, y+p_button->y, (void *)description_msg[0],0,0);
		break;
		
	case GAMBLE_ID :
		{
			if( m_PetInfo.GAMBLE )
			{
				if(m_PetInfo.CUT_HEAD)
					g_descriptor_manager.Set(DID_INFO, x+p_button->x, y+p_button->y, (void *)description_msg[3],0,0);
				else
					g_descriptor_manager.Set(DID_INFO, x+p_button->x, y+p_button->y, (void *)description_msg[1],0,0);
			}
			else
			{
				if(!m_PetInfo.ATTACK)
				{ 
					if(m_PetInfo.CUT_HEAD && m_PetInfo.ITEM_TYPE >2) // 2�� �길 3���ɷ�(���� or ����)�� �ִ�
						g_descriptor_manager.Set(DID_INFO, x+p_button->x, y+p_button->y, (void *)description_msg[4],0,0);
					else
						g_descriptor_manager.Set(DID_INFO, x+p_button->x, y+p_button->y, (void *)description_msg[2],0,0);
				}
			}
		}
		break;
	}
}


//-----------------------------------------------------------------------------
// C_VS_UI_PET_INFO_WINDOW::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_PET_INFO::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	int focusedPlus = 0, pushedPlus = 0;

//	if(p_button->GetID() == NICKNAME_ID && (!gC_vs_ui.GetChangeablePetNickName()||!m_PetInfo.ChangeAbleNickName))
//	{
//		m_pC_spk.BltLockedDarkness(p_button->x+x, p_button->y+y, p_button->m_image_index, 1);
//		return;
//	}
	if(p_button->GetID() == GAMBLE_ID)
	{
		if(!m_PetInfo.GAMBLE)
		{
			m_pC_spk.BltLockedDarkness(p_button->x+x, p_button->y+y, p_button->m_image_index, 1);
			return;
		}
	}
	if(p_button->GetFocusState())
		focusedPlus = 1;
	if(p_button->GetPressState())
		pushedPlus = 1;

	m_pC_spk.BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index+focusedPlus+pushedPlus);
}

//-----------------------------------------------------------------------------
// WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_PET_INFO::WindowEventReceiver(id_t event)
{
}


//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_PET_INFO::Run(id_t id)
{
	switch (id)
	{
	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_PET_INFO);
		break;

	case GAMBLE_ID: 
		if(m_PetInfo.GAMBLE)
			gpC_base->SendMessage(UI_PET_GAMBLE);
		break;
	case NICKNAME_ID:
		if(m_IsWolverine)
		{
			if(m_AddName.Size())
			{
				char* szName;
				g_Convert_DBCS_Ascii2SingleByte(m_AddName.GetStringWide(), m_AddName.Size(), szName);
				if(szName != NULL)
				{
					m_PetInfo.NICK_NAME = szName;
					gpC_base->SendMessage(UI_CHANGE_CUSTOM_NAMING, (int)(intptr_t)m_PetInfo.NICK_NAME.c_str(),0);
				}
				DeleteNew(szName);
			}
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// MouseControl
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_PET_INFO::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);

	Rect Exp(116,98+13, 75,23), Level(219,24,25,14);

	char sztemp[100];
	static char str[3][100];
	
	switch(message)
	{
	case M_MOVING:
		{
			static LPSTR pstr[3] = { str[0], str[1], str[2] };
			
			std::string tempStr;

			// ���� ����ġ
			if( Exp.IsInRect( _x, _y ) && m_PetInfo.LEVEL < 50 )
			{
				sprintf( sztemp, "%d", m_PetInfo.LEVEL );
				
				strcpy( str[0], (*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_LEVEL].GetString() );
				strcat( str[0], sztemp );

				int goal_exp = g_pExperienceTable->GetPetExp(m_PetInfo.LEVEL).GoalExp;
				wsprintf(str[1], (*g_pGameStringTable)[UI_STRING_MESSAGE_HPBAR_EXP_DESCRIPTION_NEW].GetString(), g_GetNumberString(m_PetInfo.EXP_REMAIN).c_str(), g_GetNumberString((goal_exp - m_PetInfo.EXP_REMAIN)*100/max(1, (goal_exp))).c_str());
				
				g_descriptor_manager.Set(DID_STRINGS,x+_x,y+_y,(void*)pstr,2);
			}
			else if ( Level.IsInRect( _x, _y ) )
			{
				sprintf(sztemp, "%d", m_PetInfo.LEVEL );
				strcpy( str[0], (*g_pGameStringTable)[UI_STRING_MESSAGE_CHAR_MANAGER_LEVEL].GetString() );
				strcat( str[0], sztemp );

				g_descriptor_manager.Set(DID_INFO, x+_x, y+_y, (void *)str[0],0,0);
			}
		}
		break;
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if(m_IsWolverine && _x>50 && _x<180 && _y>227&& _y <247)
			m_AddName.Acquire();
		else if (gpC_mouse_pointer->GetPickUpItem()==FALSE && re)
		{
			MoveReady();
			SetOrigin(_x, _y);
		}
		break;
	}
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_PET_INFO
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_PET_INFO::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);

	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_ESCAPE:
			Run(CLOSE_ID);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// Show
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_PET_INFO::Show()
{
	bool bLock = false;

	m_AddName.SetPosition(x+56, y+227);

	if(g_pUserOption->Use3DHAL && gpC_base->m_p_DDSurface_back->IsLock())
	{
		gpC_base->m_p_DDSurface_back->Unlock();
		bLock = true;
	}
	else if(!g_pUserOption->Use3DHAL && !gpC_base->m_p_DDSurface_back->IsLock())
	{
		gpC_base->m_p_DDSurface_back->Lock();
		bLock = true;
	}

	RECT windowRECT = { x+27, y+47, x+w-27, y+w-47 };
	
	DrawAlphaBox(&windowRECT, 0, 0, 0 , 19);

	for(int i = 0; i < 2; i++)
	{
		windowRECT.top = windowRECT.bottom;
		windowRECT.bottom = windowRECT.top+3;
		windowRECT.left += 3;
		windowRECT.right -= 3;
		DrawAlphaBox(&windowRECT, 0, 0, 0 , 19);
	}
	windowRECT.top = windowRECT.bottom;
	windowRECT.bottom = windowRECT.top+1;
	windowRECT.left += 3;
	windowRECT.right -= 3;
	DrawAlphaBox(&windowRECT, 0, 0, 0 , 19);

	if(g_pUserOption->Use3DHAL && bLock)
	{
		gpC_base->m_p_DDSurface_back->Lock();
		bLock = false;
	}
	else if(!g_pUserOption->Use3DHAL && bLock)
	{
//		gpC_base->m_p_DDSurface_back->Unlock();
		bLock = true;
	}

	int hpday_y = y+166, hptime_y = y+180, hp_x = x+117;
	// hptime �� �д����� ������ �ִ´�.
	int hptime = m_PetInfo.HP%(60*24), hpday = m_PetInfo.HP/(60*24);



	if(bLock || gpC_base->m_p_DDSurface_back->Lock())
	{
		m_pC_spk.BltLocked(x+m_MainWindowPosition.x, y+m_MainWindowPosition.y, MAIN_WINDOW);

		// pet ��
		int PetFaceLevel = max(0,m_PetInfo.LEVEL/10 -1);
		int PetType  = max( 0, (m_PetInfo.ITEM_TYPE-2));
		int faceSpriteID = max(0, PetFaceLevel+(PetType*5));
		m_pC_pet_face.BltLocked(x+48, y+68, faceSpriteID);

		// pet ����
		TYPE_FRAMEID frame_id = (*g_pItemTable)[ITEM_CLASS_PET_FOOD][m_PetInfo.FOODTYPE].InventoryFrameID;
		int item_x = x+73-gpC_item->GetWidth(frame_id)/2, item_y = y+173-gpC_item->GetHeight(frame_id)/2;
		gpC_item->BltLocked(item_x, item_y, frame_id);
		
		// hp bar
		m_pC_spk.BltLocked(hp_x, hpday_y, HP_DAY_BACK);
		m_pC_spk.BltLocked(hp_x, hptime_y, HP_TIME_BACK);

		if(hpday > 0 && hpday <= 30)
		{
			Rect rect(0, 0, m_pC_spk.GetWidth(HP_DAY_BAR)*hpday/30, m_pC_spk.GetHeight(HP_DAY_BAR));	// 30�� �ƽ�
			m_pC_spk.BltLockedClip(hp_x+1, hpday_y+1, rect, HP_DAY_BAR);
		}

		if(hptime > 0 && hptime < (60*24) )
		{
			Rect rect(0, 0, m_pC_spk.GetWidth(HP_TIME_BAR)*hptime/(60*24), m_pC_spk.GetHeight(HP_TIME_BAR));
			m_pC_spk.BltLockedClip(hp_x+1, hptime_y+1, rect, HP_TIME_BAR);
		}

		m_pC_button_group->Show();

		gpC_base->m_p_DDSurface_back->Unlock();
	}
	
	int str_y[5] = {y+66, y+79, y+92, y+110, y+123};
	if( g_FL2_GetDC() )
	{
		COLORREF fontColor = RGB(255, 255, 120);
		
		// �� �̸�
		const int name_x = x+130, name_y = y+25;
		std::string petName;
//		if(m_PetInfo.NICK_NAME.size())
//			petName = m_PetInfo.NICK_NAME;
//		else
			petName = m_PetInfo.NAME;
		
		g_PrintColorStr(name_x-g_GetStringWidth(petName.c_str(), gpC_base->m_item_name_pi.hfont)/2, name_y, petName.c_str(), gpC_base->m_item_name_pi, RGB_YELLOW);
		if(!m_IsWolverine && m_PetInfo.NICK_NAME.size())
			g_PrintColorStr(x +50, name_y+ 202, m_PetInfo.NICK_NAME.c_str(), gpC_base->m_char_chat_pi, RGB_YELLOW);

		// �Ӽ�
		// ���� enchant level�� ��Ӽ����� ����Ѵ�
		// enchant level �� ��þƮ�ʹ� �ƹ� ���� ������ ����� ChargeItem�� Charge���� ���� �� ����
		// ��� �������� Enchant Level�� ������ ������ Number�� �����Ѵ�. �׷��� ���� �����ۿ���
		// GetEnchantLevel�� �ص� �ƹ��͵� �ȳ��´�.
		// �װ� ���� �����۸��̰� ���� �׳� EnchantLevel �״�� ����
		// �� �Ӽ� ��ġ�� Silver���� ����. 

		int px;

		char sz_buf[512];

		// ����
		sprintf(sz_buf, "%d", m_PetInfo.LEVEL);
		int level_x = x+230-g_GetStringWidth(sz_buf, gpC_base->m_char_chat_large_pi.hfont)/2, level_y = y+23;
		g_PrintColorStr(level_x, level_y, sz_buf, gpC_base->m_char_chat_large_pi, RGB_YELLOW);

		// �� �Ӽ�
		px = g_PrintColorStr(hp_x, str_y[0], (*g_pGameStringTable)[UI_STRING_MESSAGE_PET_ATTR].GetString(), gpC_base->m_item_desc_pi, ITEM_DESC_RGB);
		if(m_PetInfo.ATTR_VALUE > 0)
		{
			sprintf(sz_buf, "%s +%d", g_pItemOptionTable->ITEMOPTION_PARTNAME[m_PetInfo.ATTR].GetString(), m_PetInfo.ATTR_VALUE);				
			g_PrintColorStr(px, str_y[0], sz_buf, gpC_base->m_item_desc_pi, RGB_GOLD);
		}
		else
		{
			g_PrintColorStr(px, str_y[0], (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_NOT_EXIST].GetString(), gpC_base->m_chatting_pi, RGB_WHITE);
		}

		// �ɼ�
		px = g_PrintColorStr(hp_x, str_y[1], (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_OPTION].GetString(), gpC_base->m_chatting_pi, ITEM_DESC_RGB);
		if(m_PetInfo.OPTION >= 0)
		{
			WORD itemOption = m_PetInfo.OPTION;
			ITEMOPTION_INFO& optionInfo=(*g_pItemOptionTable)[itemOption];
			char pPartName[20];
			strcpy(pPartName,g_pItemOptionTable->ITEMOPTION_PARTNAME[optionInfo.Part].GetString());
			if(strstr(pPartName,"MP") != NULL)
			{
				if(g_eRaceInterface == RACE_VAMPIRE)
					*strstr(pPartName,"MP") = 'H';
				else if(g_eRaceInterface == RACE_OUSTERS)
					*strstr(pPartName,"MP") = 'E';
			}
			
			BYTE PlusPoint	= optionInfo.PlusPoint; // ����Ǵ� ��ġ
			
			if (pPartName)
			{
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

				g_PrintColorStr(px, str_y[1], sz_buf, gpC_base->m_chatting_pi, RGB_GOLD);//RGB_YELLOW);
			}
		}
		else
		{
			g_PrintColorStr(px, str_y[1], (*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_NOT_EXIST].GetString(), gpC_base->m_chatting_pi, RGB_WHITE);
		}
		// 2���ɷ�
		if(m_PetInfo.CUT_HEAD)
			g_PrintColorStr(hp_x, str_y[2], (*g_pGameStringTable)[UI_STRING_MESSAGE_PET_CAN_CUT_HEAD].GetString(), gpC_base->m_chatting_pi, RGB_YELLOW);
		else
			g_PrintColorStr(hp_x, str_y[2], (*g_pGameStringTable)[UI_STRING_MESSAGE_PET_CANNOT_CUT_HEAD].GetString(), gpC_base->m_chatting_pi, RGB_RED);

		// ����ġ
		g_PrintColorStr(hp_x, str_y[3], (*g_pGameStringTable)[UI_STRING_MESSAGE_LEFT_EXP].GetString(), gpC_base->m_chatting_pi, ITEM_DESC_RGB);

		// ����ġ ��ġ
		g_PrintColorStr(hp_x, str_y[4], g_GetNumberString(m_PetInfo.EXP_REMAIN).c_str(), gpC_base->m_chatting_pi, RGB_WHITE);

		// ���� ���̾�
		g_PrintColorStr(hp_x, hpday_y-15, (*g_pGameStringTable)[UI_STRING_MESSAGE_PET_DESC_DURABILITY_2].GetString(), gpC_base->m_chatting_pi, ITEM_DESC_RGB);
		
		// hp
		sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_DAY].GetString(), hpday);
		g_PrintColorStrOut(hp_x+m_pC_spk.GetWidth(HP_DAY_BACK)/2-g_GetStringWidth(sz_buf, gpC_base->m_chatting_pi.hfont)/2, hpday_y+2, sz_buf, gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);

		std::string timeString;
		sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_HOUR].GetString(), hptime/60);
		timeString += sz_buf;
		timeString += " ";
		sprintf(sz_buf, (*g_pGameStringTable)[UI_STRING_MESSAGE_MINUTE].GetString(), hptime%60);
		timeString += sz_buf;
		g_PrintColorStrOut(hp_x+m_pC_spk.GetWidth(HP_TIME_BACK)/2-g_GetStringWidth(timeString.c_str(), gpC_base->m_chatting_pi.hfont)/2, hptime_y+2, timeString.c_str(), gpC_base->m_chatting_pi, RGB_WHITE, RGB_BLACK);

		

		m_pC_button_group->ShowDescription();
		g_FL2_ReleaseDC();
	}
	if(m_IsWolverine)
		m_AddName.Show();

#ifdef _LIB
	MFakeCreature *pPet = (MFakeCreature *)g_pZone->GetFakeCreature(g_pPlayer->GetPetID());
	if(pPet != NULL)
	{
		MPetItem *pPetItem = pPet->GetPetItem();
		if(pPetItem != NULL)
		{
			if(m_PetInfo.ITEM_ID == pPetItem->GetID())
			{	// exp, level - update
				m_PetInfo.EXP_REMAIN = pPetItem->GetPetExpRemain();
				m_PetInfo.LEVEL	= pPetItem->GetNumber();
			}
		}
	}
#endif

	SHOW_WINDOW_ATTR;
}

//-----------------------------------------------------------------------------
// Start
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_PET_INFO::Start()
{
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::PET_INFO);

	if(rect.w != -1)
	{
		x = rect.x;
		y = rect.y;
	}	
	PI_Processor::Start();
	if(m_IsWolverine)
		m_AddName.Acquire();
	m_pC_button_group->Init();
	gpC_window_manager->AppearWindow(this);	
}

void	C_VS_UI_PET_INFO::Finish()
{
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::CTF_STATUS, Rect(x, y, w, h));
	
	PI_Processor::Finish();	
	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// Process
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_PET_INFO::Process()
{
	m_pC_button_group->Process();	
}

//-----------------------------------------------------------------------------
// IsPixel
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI_PET_INFO::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);//m_pC_spk.IsPixel(SCR2WIN_X(_x),SCR2WIN_Y(_y),MAIN_WINDOW);
}
//-----------------------------------------------------------------------------
// MHelpMessageManager
// by seok min
// 
//-----------------------------------------------------------------------------


C_VS_UI_HELPDESC::C_VS_UI_HELPDESC(const char *detail,const char *title,DWORD id)
{	
	m_helpindex = id;
	m_title = title;
	m_detail = detail;
	int window_w = 500, window_h = 450;
	Set(g_GameRect.right/2-window_w/2, g_GameRect.bottom/2-window_h/2, window_w, window_h);
	int close_x = w-95, close_y = h-50;
//	m_SPK.Open(HELP_MESSAGE_DESC); // ���� ���� �޼��� �̹����� �����Ѵ�.
//	m_SPK.Open(SPK_BULLETIN_BOARD);// �ӽÿ� �̹��� 
	m_pC_button_group = new ButtonGroup(this);
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_x, close_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), CLOSE_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE));
	m_pC_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(w-25,50,-1,window_h-140));
	m_pC_scroll_bar->SetScrollPos(0);
//	AttrPin(true);

//	m_pSurface = new CSpriteSurface;
//	m_pSurface->InitOffsurface(800, 600, DDSCAPS_SYSTEMMEMORY);
	
	AttrTopmost(true);
	AttrPin(true);


#ifndef _LIB

#endif	
	HelpDescPasing();  
	g_RegisterWindow(this);
	
}

C_VS_UI_HELPDESC::~C_VS_UI_HELPDESC()
{
	DeleteNew(m_pC_scroll_bar);
	DeleteNew(m_pC_button_group);
	g_UnregisterWindow(this);
	ClearHelpJpg();
}

void
C_VS_UI_HELPDESC::ClearHelpJpg()
{
	JPGIMG_MAP::iterator itr = m_jpgData.begin();
	JPGIMG_MAP::iterator endItr = m_jpgData.end();

	for( ; itr != endItr; itr++ )
	{
		delete itr->second;
	}

	m_jpgData.clear();
}

void C_VS_UI_HELPDESC::Start()
{
	PI_Processor::Start();
	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();
}


void C_VS_UI_HELPDESC::Run(id_t id)
{
	switch (id)
	{
	case CLOSE_ID:
		Finish();
		// 2004, 5, 6 - sobeit add start - �������̽� ���� ���� �� �ʺ��� ����� ���� �߰� ^^
		if( HELP_EVENT_INTERFACE == m_helpindex )
			gC_vs_ui.AddHelpMail(HELP_EVENT_BEGINNER_ZONE);
		// 2004, 5, 6 - sobeit add end
  
		break;
	}
}
/*
void C_VS_UI_HELPDESC::Show()
{
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_global_resource->DrawDialogLocked2(x,y,w,h,GetAttributes()->alpha);
		m_pC_button_group->Show();	
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	
//	if(MHelpMessageManager::Instance().getMessageSize() == NULL) return;
//	const MHelpMessage& message = MHelpMessageManager::Instance().getMessage(m_helpindex);
	std::string content  = m_title;
	
	char buf[1024];
	sprintf(buf, "���� : %s", content.c_str());
	g_PrintColorStr(x+30, y+10, buf , gpC_base->m_dialog_msg_pi, RGB_GOLD);
	
	int NullSizex,NullSizey;
	std::string tempappend;
	std::string tempspk;
	std::string tempPos;
	std::string tempindex; 
	std::string tempcolor;
	std::vector<std::string>::iterator itr = parsing_data.begin();
	std::vector<std::string>::iterator EndItr = parsing_data.end();
	const int char_width = g_GetStringWidth("a", gpC_base->m_dialog_msg_pi.hfont);
	const int char_height = g_GetStringHeight("a", gpC_base->m_dialog_msg_pi.hfont);
	
	int line = 0;
	int imgcnt = 0;
	int linelimit = 15;
	int  m_width,m_height;
			
	DWORD color;
	
	const char *isfont = NULL;
	const char *istag  = NULL;
	
	while(itr!=EndItr)
	{		

		std::string str = *itr;
		std::string tagstr;
		istag = strstr(str.c_str(), "<");
		isfont =  strstr(str.c_str(),"#");

		if(istag)  // ù��° ���ڿ��� '<'�ΰ��
		{
			tagstr = str ;
			if(isfont)// ��ũ�� "#"�� �������
			{
				tempappend = findkey(tagstr.c_str(),"a");
				tempcolor = findkey(tagstr.c_str(),"color");
			}
			else // �±׿� "<" �� ���ԵǾ��������
			{
				
				tempspk = findkey( tagstr.c_str(), "file" );     					 // spk ���� ���´�.
				tempPos = findkey( tagstr.c_str(), "pos" );							 // pos ���� ���´�
				tempspk += ".jpg";
				
				CDirectDrawSurface *pSurface = GetJpgFileLoading(tempspk);
				m_width = pSurface->GetWidth();										// ������ spk ��m_midth �� ���´�
				m_height = pSurface->GetHeight();		
				
				NullSizex = (m_width/(char_width));									// �̹����� �ʿ�� ���� ���
				NullSizey = (m_height/(char_height+5))+1;	
				// �̹����� �ʿ��� ������ ���
				DrawImg(m_width, m_height, tempspk.c_str(), tempPos.c_str() , max(imgcnt,line));
				imgcnt+=NullSizey;		
				// Memdc ��..pasing �� �����͸� �����ͼ� ��ο� �Ѵ�.
			}
		}
		else 
		{
			// ���� ���õ� ������ ������?
			color = RGB_WHITE;
			if(!tempcolor.empty())
			{
				_stscanf(tempcolor.c_str(),_T("%x"),&color);
			}
			
			if(strcmp(tempappend.c_str(),"y") == 0) // ��Ʈ ��ũ �������� �̾���Ѵ�
			{
				line --;
				if(line < 0) line = 0;
				tempappend = "";
			}
			
			m_pC_scroll_bar->Show(x,y);
			int scrollpos = m_pC_scroll_bar->GetScrollPos();
			int top = line - scrollpos;
			if(top >= 0)
			{
				g_PrintColorStr(x+30, y+40+(top*LINEWIDTH), itr->c_str() , gpC_base->m_dialog_msg_pi, color);
			}
			line++;
		}
		itr++;
		
		if((line) - m_pC_scroll_bar->GetScrollPos()  > (linelimit))
		{
			break;
		}
	}
	m_pC_button_group->ShowDescription();

	line = 0;
	imgcnt = 0;
	tempPos = "";
	SHOW_WINDOW_ATTR;
	
}
*/


void C_VS_UI_HELPDESC::Show()
{
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_global_resource->DrawDialogLocked2(x,y,w,h,GetAttributes()->alpha);
		m_pC_button_group->Show();	
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	
	std::string content  = m_title;
	
	int NullSizex,NullSizey;
	std::string tempappend;
	std::string tempspk;
	std::string tempPos;
	std::string tempindex; 
	std::string tempcolor;
	
	int line = 0;
	int imgcnt = 0;
	int linelimit = 15;
	int  m_width,m_height;
			
	DWORD color;
	
	const char *isfont = NULL;
	const char *istag  = NULL;

	g_FL2_GetDC();

	char buf[1024];
	sprintf(buf, "%s", content.c_str());
	g_PrintColorStr(x+30, y+10, buf , gpC_base->m_dialog_msg_pi, RGB_GOLD);
	
	const int char_width = g_GetStringWidth("a", gpC_base->m_dialog_msg_pi.hfont);
	const int char_height = g_GetStringHeight("a", gpC_base->m_dialog_msg_pi.hfont);
	std::vector<std::string>::iterator itr = parsing_data.begin();
	std::vector<std::string>::iterator EndItr = parsing_data.end();
	while(itr!=EndItr)
	{		

		std::string str = *itr;
		std::string tagstr;
		istag = strstr(str.c_str(), "<");
		isfont =  strstr(str.c_str(),"#");

		if(istag)  // ù��° ���ڿ��� '<'�ΰ��
		{
			tagstr = str ;
			if(isfont)// ��ũ�� "#"�� �������
			{
				tempappend = findkey(tagstr.c_str(),"a");
				tempcolor = findkey(tagstr.c_str(),"color");
			}
			else // �±׿� "<" �� ���ԵǾ��������
			{
//				tempspk = findkey( tagstr.c_str(), "file" );     					 // spk ���� ���´�.
//				tempPos = findkey( tagstr.c_str(), "pos" );							 // pos ���� ���´�
//				tempspk += ".jpg";
//				
//				CDirectDrawSurface *pSurface = GetJpgFileLoading(tempspk);
//				m_width = pSurface->GetWidth();										// ������ spk ��m_midth �� ���´�
//				m_height = pSurface->GetHeight();		
//				
//				NullSizex = (m_width/(char_width-1));									// �̹����� �ʿ�� ���� ���
//				NullSizey = (m_height/(char_height+5))+1;	
				// �̹����� �ʿ��� ������ ���
//				DrawImg(m_width, m_height, tempspk.c_str(), tempPos.c_str() , max(imgcnt,line));
//				imgcnt+=NullSizey;		
				// Memdc ��..pasing �� �����͸� �����ͼ� ��ο� �Ѵ�.	
			}
		}
		else 
		{
			// ���� ���õ� ������ ������?
			color = RGB_WHITE;
			if(!tempcolor.empty())
			{
				_stscanf(tempcolor.c_str(),_T("%x"),&color);
			}
			
			if(strcmp(tempappend.c_str(),"y") == 0) // ��Ʈ ��ũ �������� �̾���Ѵ�
			{
				line --;
				if(line < 0) line = 0;
				tempappend = "";
			}
			m_pC_scroll_bar->Show(x,y);

			int scrollpos = m_pC_scroll_bar->GetScrollPos();
			int top = line - scrollpos;
			if(top >= 0)
			{
				g_PrintColorStr(x+30, y+40+(top*LINEWIDTH), itr->c_str() , gpC_base->m_dialog_msg_pi, color);
			}
			line++;
		}
		itr++;
		
		if((line) - m_pC_scroll_bar->GetScrollPos()  > (linelimit))
		{
			break;
		}
		
	}
	g_FL2_ReleaseDC();

	itr = parsing_data.begin();
	EndItr = parsing_data.end();
	line = 0;
	imgcnt  = 0;

	while(itr!=EndItr)
	{		
		
		std::string str = *itr;
		std::string tagstr;
		istag = strstr(str.c_str(), "<");
		isfont =  strstr(str.c_str(),"#");
		
		if(istag)  // ù��° ���ڿ��� '<'�ΰ��
		{
			tagstr = str ;
			if(isfont)// ��ũ�� "#"�� �������
			{
				tempappend = findkey(tagstr.c_str(),"a");
				tempcolor = findkey(tagstr.c_str(),"color");
			}
			else // �±׿� "<" �� ���ԵǾ��������
			{
				
				tempspk = findkey( tagstr.c_str(), "file" );     					 // spk ���� ���´�.
				tempPos = findkey( tagstr.c_str(), "pos" );							 // pos ���� ���´�
				tempspk += ".jpg";
				
				CDirectDrawSurface *pSurface = GetJpgFileLoading(tempspk);
				m_width = pSurface->GetWidth();										// ������ spk ��m_midth �� ���´�
				m_height = pSurface->GetHeight();		
				
				NullSizex = (m_width/(char_width-1));									// �̹����� �ʿ�� ���� ���
				NullSizey = (m_height/(char_height+5))+1;	
				// �̹����� �ʿ��� ������ ���
				DrawImg(m_width, m_height, tempspk.c_str(), tempPos.c_str() , max(imgcnt,line));
				imgcnt+=NullSizey;		
				// Memdc ��..pasing �� �����͸� �����ͼ� ��ο� �Ѵ�.
			}
		}
		else 
		{
			color = RGB_WHITE;
			if(!tempcolor.empty())
			{
				_stscanf(tempcolor.c_str(),_T("%x"),&color);
			}
			
			if(strcmp(tempappend.c_str(),"y") == 0) // ��Ʈ ��ũ �������� �̾���Ѵ�
			{
				line --;
				if(line < 0) line = 0;
				tempappend = "";
			}
			m_pC_scroll_bar->Show(x,y);
			
			int scrollpos = m_pC_scroll_bar->GetScrollPos();
			int top = line - scrollpos;
			if(top >= 0)
			{
				//g_PrintColorStr(x+30, y+40+(top*LINEWIDTH), itr->c_str() , gpC_base->m_dialog_msg_pi, color);
			}
			line++;
		}
		itr++;
		
		if((line) - m_pC_scroll_bar->GetScrollPos()  > (linelimit))
		{
			break;
		}
		
	}

	m_pC_button_group->ShowDescription();
	line = 0;
	imgcnt = 0;
	tempPos = "";
	SHOW_WINDOW_ATTR;
	
}


void  C_VS_UI_HELPDESC::DrawImg(int m_width, int m_height ,const char * filename,const char * pos, int linecnt)
{

	int Pagewidth = 500;										// ���� �����信 ��µ� x ������
	int Pageheight = 500;										// ���� �����信 ��µ� y ������
	int blankwidth = 35;										// left ���� ������
	int blankheight = 7;										// top ���� ������
	int imagelinesize = 20;
	
	Rect rect;
	if(strcmp("L",pos) == 0)			// ���������ϰ��
	{
		rect.x = blankheight+20;        
		rect.y = (linecnt*imagelinesize)+blankwidth-15;
	}
	
	if(strcmp("R",pos) == 0)			// �����������ϰ��,
	{
		rect.x = Pagewidth-m_width-30 ; 
		rect.y =(linecnt*imagelinesize) +blankwidth-15;
	}

	if(strcmp("LT",pos) == 0)			// ,���� ��ü 
	{
		rect.x = blankheight+20;        
		rect.y = (linecnt*imagelinesize)+blankwidth-15;
	}

	if( strcmp("RT",pos)  == 0)		 //  ��������ü 
	{
		rect.x = Pagewidth-m_width+blankheight -40; 
		rect.y  =(linecnt*imagelinesize) +blankwidth-15;
	}
	
	if(strcmp("C",pos) == 0)		// �����ϰ�� 
	{
		rect.x = (Pagewidth-m_width)/2;  
		rect.y = (linecnt*imagelinesize)+blankwidth+15;
	}
	Rect rectimg(0,0, m_width, m_height);
	// Ŭ���� ó�� 	
	int top = rect.y - (m_pC_scroll_bar->GetScrollPos()*imagelinesize);

	POINT p = { x + rect.x,  20 + y +top };
	RECT r = {0, 0 , m_width, m_height};


		CDirectDrawSurface *pSurface = GetJpgFileLoading(filename);
		RECT re = {x ,  y+30 , x+ 500, y +400};
		gpC_base->m_p_DDSurface_back->SetClip(&re);

#ifdef PLATFORM_WINDOWS
		gpC_base->m_p_DDSurface_back->BltNoColorkey( &p, pSurface, &r );
#else
		// SDL backend: cast CDirectDrawSurface* to CSpriteSurface*
		gpC_base->m_p_DDSurface_back->BltNoColorkey( &p, reinterpret_cast<CSpriteSurface*>(pSurface), &r );
#endif
		// add by Sonic 2006.9.26
		RECT clientrect = {0,0,800,600};
		if(g_MyFull)
		{
			clientrect.right=1024;
			clientrect.bottom=768;
		}
		// end
		gpC_base->m_p_DDSurface_back->SetClip(&clientrect);

}

void C_VS_UI_HELPDESC::LoadHelpJpg(std::string filename)
{

	CJpeg jpg;
	
	std::string file_path;
	file_path = _ROOT;
	file_path += "\\spk\\";
	file_path += filename;
	
	
	CDirectDrawSurface *pHelpmsgSurface = new CDirectDrawSurface;
	
	bool bOpen = jpg.Open(file_path.c_str());
	if(bOpen == true && jpg.GetWidth() > 0 && jpg.GetHeight() > 0 && jpg.GetHeight() > 0)
	{		
		CDirectDrawSurface *surface = pHelpmsgSurface;
		const int bpp = jpg.GetBpp(), width = jpg.GetWidth(), height = jpg.GetHeight(), pitch = width*bpp;
		
		if (surface->InitOffsurface(width, height, DDSCAPS_SYSTEMMEMORY))
		{
			if (surface->Lock())
			{
				WORD *pSurface = (WORD *)surface->GetSurfacePointer();
				unsigned char *pData = jpg.GetImage(), *pDataTemp;
				WORD *pSurfaceTemp;
				
				int surfacePitch = surface->GetSurfacePitch();
				
				if (pSurface)
				{
					if (bpp == 1)
					{
						for (register int y = 0; y < height; y++)
						{
							pDataTemp = pData;
							pSurfaceTemp = pSurface;								
							
							for (register int x = 0; x < width; x++)
							{
								BYTE temp_data = *pDataTemp++;	//p_data[y*(pitch)+x];
								BYTE r = temp_data>>3;
								BYTE g = r;
								BYTE b = r;
								
								*pSurfaceTemp++ = CSDLGraphics::Color(r, g, b);
							}
							
							pData = pData + pitch;
							pSurface = (WORD*)((BYTE*)pSurface + surfacePitch);
						}
					}
					else if (bpp == 3)
					{
						for (register int y = 0; y < height; y++)
						{
							pDataTemp = pData;
							pSurfaceTemp = pSurface;	
							
							for (register int x = 0; x < width; x++)
							{
								//char *temp_data = &p_data[y*pitch+x*bpp];
								BYTE r = *(pDataTemp+2) >> 3;		//temp_data[2]>>3;
								BYTE g = *(pDataTemp+1) >> 3;	//temp_data[1]>>3;
								BYTE b = *pDataTemp >> 3;	//temp_data[0]>>3;
								
								pDataTemp += bpp;
								
								*pSurfaceTemp++ = CSDLGraphics::Color(r, g, b);
							}
							
							pData = pData + pitch;
							pSurface = (WORD*)((BYTE*)pSurface + surfacePitch);
						}
					}
					
				}
				surface->Unlock();
			}
		}
	}
	m_jpgData[filename] = pHelpmsgSurface;
	jpg.Release();
}

/*
bool C_VS_UI_HELPDESC::jpgdraw(const  char * filename)
{
	CJpeg jpg;
	
	std::string file_path;
	file_path = _ROOT;
	file_path += "\\HelpImg\\";
	file_path += filename;

	helpmsgSurface = new CDirectDrawSurface;
	if(helpmsgSurface->GetSurface() != NULL)
	{
		return true;
	}
		
	bool bOpen = jpg.Open(file_path.c_str());
	if(bOpen == true && jpg.GetWidth() > 0 && jpg.GetHeight() > 0 && jpg.GetHeight() > 0)
	{		
		CDirectDrawSurface *surface = helpmsgSurface;
		const int bpp = jpg.GetBpp(), width = jpg.GetWidth(), height = jpg.GetHeight(), pitch = width*bpp;
		
		if (surface->InitOffsurface(width, height, DDSCAPS_SYSTEMMEMORY))
		{
			if (surface->Lock())
			{
				WORD *pSurface = (WORD *)surface->GetSurfacePointer();
				unsigned char *pData = jpg.GetImage(), *pDataTemp;
				WORD *pSurfaceTemp;
				
				int surfacePitch = surface->GetSurfacePitch();
				
				if (pSurface)
				{
					if (bpp == 1)
					{
						for (register int y = 0; y < height; y++)
						{
							pDataTemp = pData;
							pSurfaceTemp = pSurface;								
							
							for (register int x = 0; x < width; x++)
							{
								BYTE temp_data = *pDataTemp++;	//p_data[y*(pitch)+x];
								BYTE r = temp_data>>3;
								BYTE g = r;
								BYTE b = r;
								
								*pSurfaceTemp++ = CSDLGraphics::Color(r, g, b);
							}
							
							pData = pData + pitch;
							pSurface = (WORD*)((BYTE*)pSurface + surfacePitch);
						}
					}
					else if (bpp == 3)
					{
						for (register int y = 0; y < height; y++)
						{
							pDataTemp = pData;
							pSurfaceTemp = pSurface;	
							
							for (register int x = 0; x < width; x++)
							{
								//char *temp_data = &p_data[y*pitch+x*bpp];
								BYTE r = *(pDataTemp+2) >> 3;		//temp_data[2]>>3;
								BYTE g = *(pDataTemp+1) >> 3;	//temp_data[1]>>3;
								BYTE b = *pDataTemp >> 3;	//temp_data[0]>>3;
								
								pDataTemp += bpp;
								
								*pSurfaceTemp++ = CSDLGraphics::Color(r, g, b);
							}
							
							pData = pData + pitch;
							pSurface = (WORD*)((BYTE*)pSurface + surfacePitch);
						}
					}
					
				}
				surface->Unlock();
			}
		}
	}
	jpg.Release();
	return true;
}
*/
void C_VS_UI_HELPDESC::Process()
{
	m_pC_button_group->Process();
}

void C_VS_UI_HELPDESC::Finish()
{
	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);	
}

bool C_VS_UI_HELPDESC::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);
}


void C_VS_UI_HELPDESC::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetID() == CLOSE_ID)
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
			else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
		} else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index);
	}
}

void C_VS_UI_HELPDESC::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
	switch(p_button->GetID())
	{
	case CLOSE_ID :
		g_descriptor_manager.Set(DID_INFO,p_button->x+x,p_button->y+y,
			(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),0,0);		
		break;
	}
}

void C_VS_UI_HELPDESC::WindowEventReceiver(id_t event)
{
}

void C_VS_UI_HELPDESC::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);
	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_ESCAPE:
			Run(CLOSE_ID);
			break;
		}
	}
}


bool C_VS_UI_HELPDESC::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;	
	
	bool re = m_pC_button_group->MouseControl(message, _x, _y);
	re &= m_pC_scroll_bar->MouseControl(message, _x, _y);

	switch(message)
	{
	case M_MOVING:
		break;
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		if( re )
		{
			MoveReady();
			SetOrigin(_x, _y);
		}
		break;
	case M_WHEEL_UP:

		m_pC_scroll_bar->ScrollUp();
		break;
		
	case M_WHEEL_DOWN:
		m_pC_scroll_bar->ScrollDown();
		break;
	}
	return true;
}

void C_VS_UI_HELPDESC::LoadCustomstr(char * customstrfilename)
{
	ifstream file(customstrfilename, ios::binary);
	if(!file) return;
	char sztemp[1024];
	
	while(file.getline(sztemp,1024))
	{
		int len = strlen(sztemp);				    	// �Ѷ����� ���̸� ���Ѵ�.
		for(int i = len ; i >= 0; i--)					// ������������ �������� /r,/n �� ������ �����ֱ� ���ؼ�
		{
			if(sztemp[i] =='\r' || sztemp[i] =='\n')
			{
				sztemp[i] = '\0';
				break;
			}
		}
		custom_strting.push_back(sztemp);
	}
}

void C_VS_UI_HELPDESC::HelpDescPasing()
{
	ClearHelpJpg();

	const int char_width = g_GetStringWidth("a", gpC_base->m_dialog_msg_pi.hfont);
	const int char_height = g_GetStringHeight("a", gpC_base->m_dialog_msg_pi.hfont);

	char sztemp[MAXBUFFER];
	int  m_width,m_height;
	std::string szParsingData;
	std::string tempstr;												// ��Ʈ���� �����ϴ� �Լ�
	std::string tempspk;												// spk ����
	std::string tempPos;												// �̹��� ��ġ 
	std::string tempindex;												// spk ���ؽ�
	std::string tempappend;												// �̾��
	char *pCur = NULL;													// �Ѷ����� �о����� �����ϴ� ����
	const char *istag = NULL;													//��ũ�� �ִ��� �˻��ϴ� ����
	const char *isfont = NULL;												//��ũ�� �ִ��� �˻��ϴ� ����
	
	int NullSizex = 0,NullSizey = 0;
	
	size_t count = LIMITLINE;											
	const size_t limitlinecnt = LIMITLINE;
	size_t  appendtemp =0; 
	int  appendtot = 0;
	int  linecnt =-1;
	int cur = 0;

//	if(MHelpMessageManager::Instance().getMessageSize() == NULL) return;
//	const MHelpMessage& message = MHelpMessageManager::Instance().getMessage(m_helpindex);
//	std::string  content = 	message.m_strDetail[g_eRaceInterface];
	std::string content  = m_detail;
	
	content +="\r\n";		// �������� ���α��� �˻縦 �ϱ����ؼ� 
	while(pCur != NULL || ((cur= content.find("\r\n")) != -1))
	{
		if(pCur == NULL)
		{
//			if(!content.empty())
//			{
			memset( sztemp, 0, MAXBUFFER );
			memcpy( sztemp, content.c_str(), cur );
			content.erase(0,cur+2);
//			}
		}
		tempstr = sztemp;
		istag = strstr(tempstr.c_str(),"<");  //�̹��� �±��ϰ��
		isfont = strstr(tempstr.c_str(),"#"); // �ؽ�Ʈ �±�

		int  num =0;
		int start = 0;
		int index = 0;
		char* pch = NULL;
		std::string digit,custom;										// Ŀ���� ��Ʈ�� ���� ����
		pch = strchr(sztemp, '%');										// '%' �� ã�Ƽ� �װ��� ��ġ�� �����Ѵ�.
		szParsingData = sztemp;											// �Ѷ����� �о�ͼ� szpasingdata �� �����Ѵ�.
		
		while (pch!=NULL)												// '%' �� ������ ���� while ����
		{
			num = pch-(sztemp+start);									// % �������������� ���̸� ���Ѵ�/
			custom = "%";												// customstr "%" ���� �ʱ�ȭ���ش�
			int cnt = 0;
			start += (num+1);											///%���������������� ���̸� ��Ӵ����ش�.
			while(sztemp[start+cnt] >= '0' && sztemp[start+cnt] <= '9') // ������ ���ڰ� �ƴҶ� ���� �����ش�/
			{
				digit += sztemp[start+cnt];		
				custom += sztemp[start+cnt];
				cnt++;
			}
			
			if(custom_strting.size() > 0)
			{
				if(num >= 0)									// �̻������� empty �� �̻��Ѱ��� ���ͼ� ��¿�� ���� size �� ó���ߵ� �Ѥ�
				{
					if(custom_strting[num].size() < 2048)
					{						
						int pos = szParsingData.find( "%" );
						if( pos != -1 )
						{
							szParsingData.replace( szParsingData.begin() + pos,
								szParsingData.begin() + pos + cnt,
								custom_strting[ num ] );
						}						
					}
				}
			}
			
			pch=strchr(sztemp+start,'%');                 
			
//			custom.erase();
//			digit.erase();
			custom.empty();
			digit.empty();
		}
		
/*		int  num =0;
		int start = 0;
		int index = 0;
		szParsingData = sztemp;											// �Ѷ����� �о�ͼ� szpasingdata �� �����Ѵ�.
		int pos;

		while( ( pos = szParsingData.find( "%" ) ) != -1 )				// '%' �� ���������� ���ڿ��� ã�Ƴ���
		{
			int cnt = 1;
			strcpy(sztemp ,szParsingData.c_str());
			while(sztemp[pos+cnt] >= '0' && sztemp[pos+cnt] <= '9') // ������ ���ڰ� �ƴҶ� ���� �����ش�/
			cnt++;
			char digit[100];
			memset( digit, 0, 100 );
			memcpy( digit, szParsingData.c_str() + pos + 1, cnt );
			int num = atoi(digit);			
			if(custom_strting.size() > 0)
			{
				if(num >= 0)									// �̻������� empty �� �̻��Ѱ��� ���ͼ� ��¿�� ���� size �� ó���ߵ� �Ѥ�
				{
					if(custom_strting[num].size() < 2048)
					{						
						int pos = szParsingData.find( "%" );
						if( pos != -1 )
						{
							szParsingData.replace( szParsingData.begin() + pos,
								szParsingData.begin() + pos + cnt,
								custom_strting[ num ] );
						}						
					}
				}
			}
		}
*/
		strcpy(sztemp,szParsingData.c_str());       // Ŀ���� ��Ʈ���� ����ȹ��ڿ�//
		int len = strlen(sztemp);					// �Ѷ����� ���̸� ���Ѵ�.
		for(int  i = len ; i >= 0; i--)					// ������������ �������� /r,/n �� ������ �����ֱ� ���ؼ�
		{
			if(sztemp[i] =='\r' || sztemp[i] =='\n')
			{
				sztemp[i] = '\0';
				break;
			}
		}

		if(istag)// �±��ϰ��
		{
			if(isfont) // ��Ʈ(����) �±� �ϰ��
			{
				parsing_data.push_back(tempstr.c_str());
				tempappend = findkey(tempstr.c_str(),"a"); // ����� ������ �̾���Ѵ�
				linecnt = 1;
			}
			else // �̹��� ����±��ϰ��
			{
				while(NullSizey > 0)						// �̰��� �̹����� ysize ���� �ؽ�Ʈ�� ������
				{											// ���� ysize ��ŭ ��������� �־��ش�
					NullSizey--;
					parsing_data.push_back(" ");
					if(NullSizey == 0) break;
				}

				parsing_data.push_back(tempstr.c_str());        // �̰��� �±� ��ü ������ �����Ѵ�
				tempspk = findkey( tempstr.c_str(), "file" );    // �±װ��� ��������Ʈ ���� ã��
				tempPos = findkey( tempstr.c_str(), "pos" );	// �±��� �̹��� ��ġ�� ����
				//tempindex = findkey( tempstr.c_str(), "index" ); // spk �� �ε����� ã�´�
				tempstr = "";
			
//				CJpeg jpg;
//				std::string file_path;
//				file_path = _ROOT;
//				file_path += "\\spk\\";
//				file_path += tempspk; file_path+=".jpg";
				tempspk +=".jpg";
				LoadHelpJpg(tempspk);
//				jpg.Open(file_path.c_str());
				
				CDirectDrawSurface *pJpgSurface = GetJpgFileLoading( tempspk );
				m_width = pJpgSurface->GetWidth();										// ������ spk ��m_midth �� ���´�
				m_height = pJpgSurface->GetHeight();	
			//	m_width = m_SPK.GetWidth(atoi(tempindex.c_str()));										// ������ spk ��m_midth �� ���´�
			//	m_height = m_SPK.GetHeight(atoi(tempindex.c_str()));									// ������ spk ��m_heigh �� ���´�
				
				NullSizex = (m_width/(char_width-1));									// �̹����� �ʿ�� ���� ���
				NullSizey = (m_height/(char_height+5))+1;								// �̹����� �ʿ��� ������ ���
//				jpg.Release();
				
			}
		}
		else 
		{
			tempstr = "";
			if(strcmp(tempPos.c_str(),"L") == 0) // ���ʿ� �̹����� �ִ°��:�켱�տ� sizex �� ������ ������ ���ٿ� ������ �ִ� ���̸� �����Ѵ�
			{
				if(strcmp(tempappend.c_str(),"y") == 0 )
				{
					NullSizex = 0;
				}
				count = limitlinecnt - NullSizex;
				while(NullSizex > 0)
				{
					NullSizex--;
					if(gC_ci->IsChinese() == true)
						tempstr += "  ";
					else
						tempstr += " ";
				}
			}

			if(strcmp(tempPos.c_str(),"R") == 0) //�����ʿ� �̹����� �ִ°��:���ٿ� �������ִ� ���̸� ����
			{
				count = limitlinecnt -NullSizex;
			}

			if(strcmp(tempPos.c_str(),"C") == 0 || strcmp(tempPos.c_str(),"RT") == 0 || strcmp(tempPos.c_str(),"LT") == 0)
			{ // �߰� , ���� ��ü����, ������ ��ü ���� : size y ��ŭ�� ����������־��ش�
				while(NullSizey > 0)
				{
					NullSizey--;
					parsing_data.push_back(" ");
				}
			}

			if(strcmp(tempappend.c_str(),"y") == 0 ) // ����� ����?�ƴϳ�?
			{
				count = count - appendtemp;
				while(appendtemp > 0)
				{
					appendtemp--;
					if(gC_ci->IsChinese() == true)
						tempstr += "  ";
					else
						tempstr += " ";
				}
				tempappend = "";
				NullSizey++;
			}

			if(NullSizey > 0)						// �̹��� �� �ִٸ� yszie �� �����������
			{
				NullSizex = (m_width/(char_width-1)); // ������ �־ �پ��  Nullsizex ũ�⸦ �������ش�
			}
			else
			{
				tempPos = "";					 // �̹����� ysize �� ��������� ������ pos �±׸� Ŭ���� ���ش�
			}

			if(pCur == NULL) // pCur �� ���������
			{
				pCur = sztemp;  // pCur ��.. �Ѷ����о�帰 ���ڿ��� �����Ѵ�.
			}
			
			size_t linecount = strlen(pCur); // linecount �� �Ѷ����� �ѹ��ڿ� ���̸� �����Ѵ�
			size_t cutCount = count;         // �ѱ� 2byte �߰��� ������ ������ �������ؼ� ����ϴ� ����
			if(linecount < count)			//  ���ڿ��� count ���� �������
			{
				tempstr += pCur;			// �ش繮�ڿ��� �����Ѵ�
				count -= linecount;			// count �� �������ڿ������� ���� �����;� ������ ���������� ���ڿ����̸� ���ش�
				pCur = NULL;		
				count = limitlinecnt;
			}
			else
			{
				char szbuf[MAXBUFFER];    
				if(g_PossibleStringCut(pCur, static_cast<int>(cutCount)) == false) // ���������ڿ��� �����ؼ� �ѱ��ϰ�� 
				{
					cutCount--;  // ���ڿ��� ���ڸ� �ϳ� �ٿ��ش�
				}
				memcpy(szbuf, pCur, cutCount);
				szbuf[cutCount] = '\0';
				tempstr += szbuf;
				pCur += cutCount; 
				count = limitlinecnt;
			}
			
			appendtemp = tempstr.length(); // appen ����϶� �����ٿ� ���ʿ� ������ ��Ű���ؼ�
			parsing_data.push_back(tempstr.c_str());
			NullSizey--; // �̹����� ������� �������� push_back �� ������ ���� ���پ� ���ش�.
			tempstr = "";
		}
	}
	while(NullSizey > 0)						// �̰��� �̹����� ysize ���� �ؽ�Ʈ�� ������
	{											// ���� ysize ��ŭ ��������� �־��ش�
		NullSizey--;
		parsing_data.push_back(" ");
		if(NullSizey == 0) break;
	}

	if(parsing_data.size() > 15)
	{
			m_pC_scroll_bar->SetPosMax(parsing_data.size()-15);	
	}
	else
	{
			m_pC_scroll_bar->SetPosMax(1);
	}
}

char* C_VS_UI_HELPDESC::findkey(const char* tagstr ,char* keyword)
{
	if(tagstr == NULL) return NULL;
	
	const char *s = strstr(tagstr, keyword );
	if(s == NULL )
		return NULL;
	
	static char Buffer[512];  // ������ static  ���� �ؾ��Ѵ� �޸� ħ���� �������ؼ� (�߿��ϴ�)
	memset( Buffer, 0, 512 );
	
	const char *start = strstr( s, "'" );
	start++;
	const char *end = strstr( start,"'" );
	
	memcpy( Buffer, start, end-start);
	return Buffer;
}



//----------------------------------------------------------------------------
// C_VS_UI_SMS_MESSAGE::C_VS_UI_SMS_MESSAGE
//
//
//----------------------------------------------------------------------------
C_VS_UI_SMS_MESSAGE::C_VS_UI_SMS_MESSAGE()
{
	g_RegisterWindow(this);
	int i;
//	AttrTopmost(true);
	AttrKeyboardControl(true);
	m_bComboList = -1;
	
	for(i = 0; i<6; i++)
		m_bComboSelect[i] = 0;
	wsprintf(m_szPhoneNum[0], "010");
	wsprintf(m_szPhoneNum[1], "011");
	wsprintf(m_szPhoneNum[2], "016");
	wsprintf(m_szPhoneNum[3], "017");
	wsprintf(m_szPhoneNum[4], "018");
	wsprintf(m_szPhoneNum[5], "019");

	m_pC_button_group = new ButtonGroup(this);
	m_SubWindow = false;
	m_sub_selected.x = -1;

	m_pC_sub_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(135, 35, -1, 30));
	m_pC_sub_scroll_bar->SetPosMax(MARK_MAX - 5+1);
	m_pC_sub_scroll_bar->SetScrollPos(0);
	
	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		m_SMS_spk.Open(SPK_SLAYER_SMS_MESSAGE);
		m_Main.cx = 100;
		m_Main.cy = 60;
		m_ComboPos[5].cx = m_Main.cx+46;
		m_ComboPos[5].cy = m_Main.cy+155;
		for(i = 0; i< 5; i++)
		{
			m_ComboPos[i].cx = m_Main.cx+31;
			m_ComboPos[i].cy = m_Main.cy+207 + (i*21);
		}
		szSMSMessagePos.cx = m_Main.cx+38;
		szSMSMessagePos.cy = m_Main.cy+36;
		m_IconPos.cx = 39;
		m_IconPos.cy = 349;
		m_ChargePos.cx = m_IconPos.cx+25 ;
		m_ChargePos.cy = m_IconPos.cy+38 ;
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+101, m_Main.cy+358, m_SMS_spk.GetWidth(SEND_BUTTON_HILIGHT), m_SMS_spk.GetHeight(SEND_BUTTON_HILIGHT), SMS_SEND_ID, this, SEND_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+61, m_Main.cy+317, m_SMS_spk.GetWidth(PHONE_LIST_BUTTON_HILIGHT), m_SMS_spk.GetHeight(PHONE_LIST_BUTTON_HILIGHT), SMS_PHONE_LIST_ID, this, PHONE_LIST_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+128, m_Main.cy+10, m_SMS_spk.GetWidth(SMILE_BUTTON_HILIGHT), m_SMS_spk.GetHeight(SMILE_BUTTON_HILIGHT), SMS_SMILE_ID, this, SMILE_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+145, m_Main.cy+10, m_SMS_spk.GetWidth(EXIT_BUTTON_HILIGHT), m_SMS_spk.GetHeight(EXIT_BUTTON_HILIGHT), SMS_CLOSE_ID, this, EXIT_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos[0].cx, m_ComboPos[0].cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_OTHER_1, this, COMBO_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos[1].cx, m_ComboPos[1].cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_OTHER_2, this, COMBO_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos[2].cx, m_ComboPos[2].cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_OTHER_3, this, COMBO_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos[3].cx, m_ComboPos[3].cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_OTHER_4, this, COMBO_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos[4].cx, m_ComboPos[4].cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_OTHER_5, this, COMBO_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos[5].cx, m_ComboPos[5].cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_MINE, this, COMBO_BUTTON_HILIGHT));

		m_lev_MyNum.SetPosition(m_Main.cx+100, m_Main.cy+156);
		m_lev_OtherNum[0].SetPosition(m_Main.cx+84, m_Main.cy+208);
		m_lev_OtherNum[1].SetPosition(m_Main.cx+84, m_Main.cy+229);
		m_lev_OtherNum[2].SetPosition(m_Main.cx+84, m_Main.cy+250);
		m_lev_OtherNum[3].SetPosition(m_Main.cx+84, m_Main.cy+271);
		m_lev_OtherNum[4].SetPosition(m_Main.cx+84, m_Main.cy+292);
		m_lev_SMSMessage.SetPosition(szSMSMessagePos.cx, szSMSMessagePos.cy);
		m_lev_SMSMessage.SetEditorMode(15, 6);
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(szSMSMessagePos.cx, szSMSMessagePos.cy, 140, 110, SMS_CHAT_MESSAGE_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+100, m_Main.cy+156, 60, 15, SMS_CHAT_MINE_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+84, m_Main.cy+208, 60, 15, SMS_CHAT_OTHER1_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+84, m_Main.cy+229, 60, 15, SMS_CHAT_OTHER2_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+84, m_Main.cy+250, 60, 15, SMS_CHAT_OTHER3_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+84, m_Main.cy+271, 60, 15, SMS_CHAT_OTHER4_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+84, m_Main.cy+292, 60, 15, SMS_CHAT_OTHER5_ID, this, 0));

		m_Color = RGB_BLACK;

		m_lev_SMSMessage.SetPrintInfo(gpC_base->m_chatting_pi);
		m_lev_SMSMessage.SetInputStringColor(m_Color);
		m_lev_SMSMessage.SetCursorColor(m_Color);	
		
		m_sub_rect.Set(0, 0, 160, 100);
		m_pC_sub_scroll_bar->x = 135;
		m_pC_sub_scroll_bar->y = 35;
		break;
		
	case RACE_VAMPIRE:
		m_SMS_spk.Open(SPK_VAMPIRE_SMS_MESSAGE);
		m_Main.cx = 100;
		m_Main.cy = 40;
		m_ComboPos[5].cx = m_Main.cx+66;
		m_ComboPos[5].cy = m_Main.cy+253;
		for(i = 0; i< 5; i++)
		{
			m_ComboPos[i].cx = m_Main.cx+66;
			m_ComboPos[i].cy = m_Main.cy+292 + (i*21);
		}
		szSMSMessagePos.cx = m_Main.cx+75;
		szSMSMessagePos.cy = m_Main.cy+100;
		m_IconPos.cx = 53;
		m_IconPos.cy = 418;
		m_ChargePos.cx = m_IconPos.cx+32 ;
		m_ChargePos.cy = m_IconPos.cy+8 ;
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+111, m_Main.cy+402, m_SMS_spk.GetWidth(PHONE_LIST_BUTTON_HILIGHT), m_SMS_spk.GetHeight(PHONE_LIST_BUTTON_HILIGHT), SMS_PHONE_LIST_ID, this, PHONE_LIST_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+110, m_Main.cy+434, m_SMS_spk.GetWidth(SEND_BUTTON_HILIGHT), m_SMS_spk.GetHeight(SEND_BUTTON_HILIGHT), SMS_SEND_ID, this, SEND_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+37, m_Main.cy+102, m_SMS_spk.GetWidth(SMILE_BUTTON_HILIGHT), m_SMS_spk.GetHeight(SMILE_BUTTON_HILIGHT), SMS_SMILE_ID, this, SMILE_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+37, m_Main.cy+84, m_SMS_spk.GetWidth(EXIT_BUTTON_HILIGHT), m_SMS_spk.GetHeight(EXIT_BUTTON_HILIGHT), SMS_CLOSE_ID, this, EXIT_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos[0].cx, m_ComboPos[0].cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_OTHER_1, this, COMBO_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos[1].cx, m_ComboPos[1].cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_OTHER_2, this, COMBO_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos[2].cx, m_ComboPos[2].cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_OTHER_3, this, COMBO_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos[3].cx, m_ComboPos[3].cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_OTHER_4, this, COMBO_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos[4].cx, m_ComboPos[4].cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_OTHER_5, this, COMBO_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos[5].cx, m_ComboPos[5].cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_MINE, this, COMBO_BUTTON_HILIGHT));

		m_lev_MyNum.SetPosition(m_Main.cx+120, m_Main.cy+254);
		m_lev_OtherNum[0].SetPosition(m_Main.cx+120, m_Main.cy+292);
		m_lev_OtherNum[1].SetPosition(m_Main.cx+120, m_Main.cy+313);
		m_lev_OtherNum[2].SetPosition(m_Main.cx+120, m_Main.cy+334);
		m_lev_OtherNum[3].SetPosition(m_Main.cx+120, m_Main.cy+355);
		m_lev_OtherNum[4].SetPosition(m_Main.cx+120, m_Main.cy+376);
		m_lev_SMSMessage.SetPosition(szSMSMessagePos.cx, szSMSMessagePos.cy);
		m_lev_SMSMessage.SetEditorMode(15, 6);

		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(szSMSMessagePos.cx, szSMSMessagePos.cy, 140, 110, SMS_CHAT_MESSAGE_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+120, m_Main.cy+254, 60, 15, SMS_CHAT_MINE_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+120, m_Main.cy+292, 60, 15, SMS_CHAT_OTHER1_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+120, m_Main.cy+313, 60, 15, SMS_CHAT_OTHER2_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+120, m_Main.cy+334, 60, 15, SMS_CHAT_OTHER3_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+120, m_Main.cy+355, 60, 15, SMS_CHAT_OTHER4_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+120, m_Main.cy+376, 60, 15, SMS_CHAT_OTHER5_ID, this, 0));

		m_Color = RGB_WHITE;
			
		m_lev_SMSMessage.SetPrintInfo(gpC_base->m_chatting_pi);
		m_lev_SMSMessage.SetInputStringColor(m_Color);
		m_lev_SMSMessage.SetCursorColor(m_Color);
		
		m_sub_rect.Set(0, 0, 160, 100);
		m_pC_sub_scroll_bar->x = 135;
		m_pC_sub_scroll_bar->y = 35;
		break;
		
	case RACE_OUSTERS:
		m_SMS_spk.Open(SPK_OUSTERS_SMS_MESSAGE);
		m_Main.cx = 40;
		m_Main.cy = 80;
		m_ComboPos[5].cx = m_Main.cx+170;
		m_ComboPos[5].cy = m_Main.cy+120;
		for(i = 0; i< 5; i++)
		{
			m_ComboPos[i].cx = m_Main.cx+170;
			m_ComboPos[i].cy = m_Main.cy+159 + (i*21);
		}
		szSMSMessagePos.cx = m_Main.cx+32;
		szSMSMessagePos.cy = m_Main.cy+41;
		m_IconPos.cx = m_Main.cx+132;
		m_IconPos.cy = m_Main.cy+198;
		m_ChargePos.cx = m_IconPos.cx+28 ;
		m_ChargePos.cy = m_IconPos.cy+22 ;
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+228, m_Main.cy+269, m_SMS_spk.GetWidth(PHONE_LIST_BUTTON_HILIGHT), m_SMS_spk.GetHeight(PHONE_LIST_BUTTON_HILIGHT), SMS_PHONE_LIST_ID, this, PHONE_LIST_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+228, m_Main.cy+302, m_SMS_spk.GetWidth(SEND_BUTTON_HILIGHT), m_SMS_spk.GetHeight(SEND_BUTTON_HILIGHT), SMS_SEND_ID, this, SEND_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+87, m_Main.cy+22, m_SMS_spk.GetWidth(SMILE_BUTTON_HILIGHT), m_SMS_spk.GetHeight(SMILE_BUTTON_HILIGHT), SMS_SMILE_ID, this, SMILE_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+105, m_Main.cy+22, m_SMS_spk.GetWidth(EXIT_BUTTON_HILIGHT), m_SMS_spk.GetHeight(EXIT_BUTTON_HILIGHT), SMS_CLOSE_ID, this, EXIT_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos[0].cx, m_ComboPos[0].cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_OTHER_1, this, COMBO_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos[1].cx, m_ComboPos[1].cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_OTHER_2, this, COMBO_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos[2].cx, m_ComboPos[2].cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_OTHER_3, this, COMBO_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos[3].cx, m_ComboPos[3].cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_OTHER_4, this, COMBO_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos[4].cx, m_ComboPos[4].cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_OTHER_5, this, COMBO_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos[5].cx, m_ComboPos[5].cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_MINE, this, COMBO_BUTTON_HILIGHT));

		m_lev_MyNum.SetPosition(m_Main.cx+223, m_Main.cy+120);
		m_lev_OtherNum[0].SetPosition(m_Main.cx+223, m_Main.cy+160);
		m_lev_OtherNum[1].SetPosition(m_Main.cx+223, m_Main.cy+181);
		m_lev_OtherNum[2].SetPosition(m_Main.cx+223, m_Main.cy+202);
		m_lev_OtherNum[3].SetPosition(m_Main.cx+223, m_Main.cy+223);
		m_lev_OtherNum[4].SetPosition(m_Main.cx+223, m_Main.cy+244);
		m_lev_SMSMessage.SetPosition(szSMSMessagePos.cx, szSMSMessagePos.cy);
		m_lev_SMSMessage.SetEditorMode(18, 6);

		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(szSMSMessagePos.cx, szSMSMessagePos.cy, 140, 110, SMS_CHAT_MESSAGE_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+223, m_Main.cy+120, 60, 15, SMS_CHAT_MINE_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+223, m_Main.cy+160, 60, 15, SMS_CHAT_OTHER1_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+223, m_Main.cy+181, 60, 15, SMS_CHAT_OTHER2_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+223, m_Main.cy+202, 60, 15, SMS_CHAT_OTHER3_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+223, m_Main.cy+223, 60, 15, SMS_CHAT_OTHER4_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+223, m_Main.cy+244, 60, 15, SMS_CHAT_OTHER5_ID, this, 0));

		m_Color = RGB_WHITE;

		m_lev_SMSMessage.SetPrintInfo(gpC_base->m_chatting_pi);
		m_lev_SMSMessage.SetInputStringColor(m_Color);
		m_lev_SMSMessage.SetCursorColor(m_Color);	
		
		m_sub_rect.Set(0, 0, 200, 140);
		m_pC_sub_scroll_bar->x = 155;
		m_pC_sub_scroll_bar->y = 55;
		break;
	}

	Set(m_Main.cx, m_Main.cy, m_SMS_spk.GetWidth(), m_SMS_spk.GetHeight());
	
	m_lev_MyNum.SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_MyNum.SetByteLimit(8);
	m_lev_MyNum.SetInputStringColor(RGB_BLACK);
	m_lev_MyNum.SetCursorColor(RGB_BLACK);
	m_lev_MyNum.SetDigitOnlyMode(true);

	
	m_lev_OtherNum[0].SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_OtherNum[0].SetByteLimit(8);
	m_lev_OtherNum[0].SetInputStringColor(RGB_BLACK);
	m_lev_OtherNum[0].SetCursorColor(RGB_BLACK);
	m_lev_OtherNum[0].SetDigitOnlyMode(true);

	
	m_lev_OtherNum[1].SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_OtherNum[1].SetByteLimit(8);
	m_lev_OtherNum[1].SetInputStringColor(RGB_BLACK);
	m_lev_OtherNum[1].SetCursorColor(RGB_BLACK);
	m_lev_OtherNum[1].SetDigitOnlyMode(true);

	m_lev_OtherNum[2].SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_OtherNum[2].SetByteLimit(8);
	m_lev_OtherNum[2].SetInputStringColor(RGB_BLACK);
	m_lev_OtherNum[2].SetCursorColor(RGB_BLACK);
	m_lev_OtherNum[2].SetDigitOnlyMode(true);

	m_lev_OtherNum[3].SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_OtherNum[3].SetByteLimit(8);
	m_lev_OtherNum[3].SetInputStringColor(RGB_BLACK);
	m_lev_OtherNum[3].SetCursorColor(RGB_BLACK);
	m_lev_OtherNum[3].SetDigitOnlyMode(true);

	m_lev_OtherNum[4].SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_OtherNum[4].SetByteLimit(8);
	m_lev_OtherNum[4].SetInputStringColor(RGB_BLACK);
	m_lev_OtherNum[4].SetCursorColor(RGB_BLACK);
	m_lev_OtherNum[4].SetDigitOnlyMode(true);
	
	
//	m_lev_SMSMessage.SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_SMSMessage.SetByteLimit(20*4);
	m_lev_SMSMessage.SetAbsWidth(90);
	
	Attach(&m_lev_SMSMessage);

	AttrTopmost(true);
	AttrPin(true);

	char* TempNum = gpC_vs_ui_window_manager->GetSMSMyNum();
	if(strlen(TempNum)>0)
	{
		char Num[16];
		wsprintf(Num, "%s", TempNum+3);
		m_lev_MyNum.AddString(Num);
		for(int j = 0; j< 6; j++)
		{
			if(m_szPhoneNum[j][2] == TempNum[2])
			{
				m_bComboSelect[5] = j;
				break;
			}
		}
	}
}

C_VS_UI_SMS_MESSAGE::~C_VS_UI_SMS_MESSAGE()
{
	if(m_lev_MyNum.Size()>0)
	{
		char *szMyNum;
		g_Convert_DBCS_Ascii2SingleByte(m_lev_MyNum.GetStringWide(), m_lev_MyNum.Size(), szMyNum);
		m_szMyNum = m_szPhoneNum[m_bComboSelect[5]];
		m_szMyNum += szMyNum;
		gpC_vs_ui_window_manager->SetSMSMynum((char*)m_szMyNum.c_str());
	}


	g_UnregisterWindow(this);
	
	m_lev_SMSMessage.Unacquire();
	gC_vs_ui.AcquireChatting();

	DeleteNew( m_pC_button_group );
	DeleteNew(m_pC_sub_scroll_bar);
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_MESSAGE::Process
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_MESSAGE::Process()
{
	m_pC_button_group->Process();
}

void	C_VS_UI_SMS_MESSAGE::AddSendList(char* str)
{
	for(int i = 0; i< 5; i++)
	{
		if(0 == m_lev_OtherNum[i].Size())
		{
			char Num[16];
			wsprintf(Num, "%s", str+3);
			m_lev_OtherNum[i].AddString(Num);
			for(int j = 0; j< 6; j++)
			{
				if(m_szPhoneNum[j][2] == str[2])
					m_bComboSelect[i] = j;
			}
			return;
		}
	}

}
//-----------------------------------------------------------------------------
// C_VS_UI_SMS_MESSAGE::UnacquireMouseFocus
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_MESSAGE::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_MESSAGE::Start
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_MESSAGE::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();
	
	m_lev_SMSMessage.Acquire();
	
	AttrTopmost(true);
	gpC_window_manager->AppearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_MESSAGE::Finish
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_MESSAGE::Finish()
{
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_MESSAGE::Show
//
//
//-----------------------------------------------------------------------------
void C_VS_UI_SMS_MESSAGE::Show()
{	
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_SMS_spk.BltLocked(x, y);
		if(g_char_slot_ingame.m_SMS_Charge)
			m_SMS_spk.BltLocked(x+m_IconPos.cx, y+m_IconPos.cy, SMS_ITEM_ICON);
		
		m_pC_button_group->Show();
		
		if(m_SubWindow != false)
		{
			int ousters_plus = 18;
			if(g_eRaceInterface != RACE_OUSTERS)
				ousters_plus = 0;

			m_sub_rect.x = x+w +10;
			m_sub_rect.y = y+10;

			gpC_global_resource->DrawDialogLocked(m_sub_rect, GetAttributes()->alpha);
			if(m_sub_selected.x != -1)
			{
				RECT rect = {m_sub_rect.x+MARK_X+ousters_plus+m_sub_selected.x*14-1, m_sub_rect.y+MARK_Y+ousters_plus-2+(m_sub_selected.y-m_pC_sub_scroll_bar->GetScrollPos())*14, 0, 0};
				rect.right = rect.left +15;
				rect.bottom = rect.top +16;
				
				if(m_sub_selected.y >= m_pC_sub_scroll_bar->GetScrollPos() && m_sub_selected.y < m_pC_sub_scroll_bar->GetScrollPos()+5)
					DrawAlphaBox(&rect, 0, 0, 255, g_pUserOption->ALPHA_DEPTH);
			}
			//				
			gpC_base->m_p_DDSurface_back->Unlock();
			
			
			g_FL2_GetDC();
			for(int i = 0; i < 5; i++)
			{
				for(int j = 0; j < 9; j++)
				{
					g_PrintColorStr(m_sub_rect.x + j*14 +MARK_X+ousters_plus, m_sub_rect.y + i*14 +MARK_Y+ousters_plus, g_mark[i+m_pC_sub_scroll_bar->GetScrollPos()][j], gpC_base->m_chatting_pi, RGB_WHITE);
				}
			}
			g_FL2_ReleaseDC();
			m_pC_sub_scroll_bar->Show(m_sub_rect.x, m_sub_rect.y);
			
		}
		else
			gpC_base->m_p_DDSurface_back->Unlock();
		
	}

	if(g_FL2_GetDC())
	{
		int i;

		for(i =0; i< 6; i++)
		{
			g_PrintColorStr(m_ComboPos[i].cx-24+30, m_ComboPos[i].cy+1, m_szPhoneNum[m_bComboSelect[i]], gpC_base->m_chatting_pi, RGB_BLACK);
		}
		char szString[32];
		wsprintf(szString, "%d/80 byte" , g_GetByteLenth(m_lev_SMSMessage.GetStringWide(),m_lev_SMSMessage.Size()));
		g_PrintColorStr(szSMSMessagePos.cx + 30 , szSMSMessagePos.cy + 92, szString, gpC_base->m_chatting_pi, m_Color);
//		if(g_char_slot_ingame.m_SMS_Charge)
		{
			wsprintf(szString,"%d",/*(*g_pGameStringTable)[UI_STRING_MESSAGE_DESC_LEFT_NUM].GetString(),*/g_char_slot_ingame.m_SMS_Charge);
			g_PrintColorStr(x+m_ChargePos.cx, y+m_ChargePos.cy, szString, gpC_base->m_item_desc_pi, RGB_YELLOW);
		}
		g_FL2_ReleaseDC();
	}
	if(-1 != m_bComboList)
	{
		if(gpC_base->m_p_DDSurface_back->Lock())
		{
			m_SMS_spk.BltLocked(x+m_ComboListPos.cx+30, y+m_ComboListPos.cy,COMBO_LIST);
			m_SMS_spk.BltLocked(x+m_ComboListPos.cx+30, y+m_ComboListPos.cy+(m_bComboSelect[m_bComboList]*12),COMBO_SELECTER);
			gpC_base->m_p_DDSurface_back->Unlock();
		}
		if(g_FL2_GetDC())
		{
			for(int i = 0; i< 6; i++)
				g_PrintColorStr(x+m_ComboListPos.cx+2+30, y+m_ComboListPos.cy+(i*12), m_szPhoneNum[i], gpC_base->m_chatting_pi, RGB_BLACK);
			g_FL2_ReleaseDC();
		}
	}

	m_lev_MyNum.Show();
	for(int i = 0; i<5; i++)
		m_lev_OtherNum[i].Show();
	m_lev_SMSMessage.Show();
	
	m_pC_button_group->ShowDescription();
	
	SHOW_WINDOW_ATTR;
}

void	C_VS_UI_SMS_MESSAGE::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	const static char*	m_help_string[4] = {
		(*g_pGameStringTable)[UI_STRING_HELP_SMS_SEND].GetString(),
		(*g_pGameStringTable)[UI_STRING_HELP_SMS_VIEW_LIST].GetString(),
		(*g_pGameStringTable)[UI_STRING_HELP_SMS_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
	};
	
	if (p_button->GetID() < 4)
		g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()],0,0);	
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_MESSAGE::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_MESSAGE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	assert(p_button);
//	
//	
	int TempX = 0;
	if(p_button->GetFocusState())
	{
		if(p_button->GetID()>SMS_COMBO_MINE)
			return;
		if(p_button->GetID()>SMS_CLOSE_ID)
			TempX = 30;
		if(p_button->GetPressState())
			m_SMS_spk.BltLocked(p_button->x + TempX, p_button->y, p_button->m_image_index+1);
		else
			m_SMS_spk.BltLocked(p_button->x + TempX, p_button->y, p_button->m_image_index);
	}

}

//-----------------------------------------------------------------------------
//
// keyboard
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_MESSAGE::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);
//	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_TAB:
			if(m_lev_MyNum.IsAcquire())
				m_lev_OtherNum[0].Acquire();
			else if(m_lev_OtherNum[0].IsAcquire())
				m_lev_OtherNum[1].Acquire();
			else if(m_lev_OtherNum[1].IsAcquire())
				m_lev_OtherNum[2].Acquire();
			else if(m_lev_OtherNum[2].IsAcquire())
				m_lev_OtherNum[3].Acquire();
			else if(m_lev_OtherNum[3].IsAcquire())
				m_lev_OtherNum[4].Acquire();
			else if(m_lev_OtherNum[4].IsAcquire())
				m_lev_SMSMessage.Acquire();
			else if(m_lev_SMSMessage.IsAcquire())
				m_lev_MyNum.Acquire();

			break;
			
		case VK_RETURN:
			if(m_lev_MyNum.IsAcquire())
				m_lev_OtherNum[0].Acquire();
			else if(m_lev_OtherNum[0].IsAcquire())
				m_lev_OtherNum[1].Acquire();
			else if(m_lev_OtherNum[1].IsAcquire())
				m_lev_OtherNum[2].Acquire();
			else if(m_lev_OtherNum[2].IsAcquire())
				m_lev_OtherNum[3].Acquire();
			else if(m_lev_OtherNum[3].IsAcquire())
				m_lev_OtherNum[4].Acquire();
			else if(m_lev_OtherNum[4].IsAcquire())
				m_lev_SMSMessage.Acquire();
//			else if(m_lev_SMSMessage.IsAcquire())
//				m_lev_MyNum.Acquire();
			break;
			
		case VK_ESCAPE: 
			gpC_base->SendMessage(UI_CLOSE_SMS_MESSAGE);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_MESSAGE::MouseControl
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_SMS_MESSAGE::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	bool re = true;
//	_x-=x; _y-=y;	
//	bool re = m_pC_button_group->MouseControl(message, _x, _y);
//
//	//	re &= m_pC_scroll_bar->MouseControl(message, _x, _y);
	if(m_SubWindow == true && m_sub_rect.IsInRect(_x, _y))
	{
		gpC_mouse_pointer->SetCursorDefault();
		// sub window �ȿ� ������ sub scroll
		re &= m_pC_sub_scroll_bar->MouseControl(message, _x-m_sub_rect.x, _y-m_sub_rect.y);
		switch(message)
		{
		case M_LEFTBUTTON_DOWN:
		case M_LB_DOUBLECLICK:
			{
				m_sub_selected.x = (_x - m_sub_rect.x-MARK_X)/14;
				m_sub_selected.y = (_y - m_sub_rect.y-MARK_Y)/14+m_pC_sub_scroll_bar->GetScrollPos();
				// 2004, 5, 10 sobeit add start - Ŭ�� ��ġ ����
				if(RACE_OUSTERS == g_eRaceInterface)
				{
					m_sub_selected.x -= 1;
					m_sub_selected.y -= 1;
				}
				// 2004, 5, 10 sobeit add end - Ŭ�� ��ġ ����
				if(m_sub_selected.x < 0 || m_sub_selected.x > 8 || m_sub_selected.y < 0 || m_sub_selected.y >= MARK_MAX)
					m_sub_selected.x = -1;
				if(m_sub_selected.x != -1 )
				{
					char_t ch;
					memcpy(&ch, g_mark[m_sub_selected.y][m_sub_selected.x], 2);
					m_lev_SMSMessage.InsertMark(ch);
				}
		
			}
			break;
			
		case M_WHEEL_UP:
			m_pC_sub_scroll_bar->ScrollUp();
			break;
			
		case M_WHEEL_DOWN:
			m_pC_sub_scroll_bar->ScrollDown();
			break;
		}
		return true;
	}
	
	switch(message)
	{		
	case M_MOVING:
		if(-1 != m_bComboList)
		{
			int TempY = _y - m_ComboListPos.cy - m_Main.cy;
			if(TempY<0) TempY = 0;
			if(TempY>71) TempY = 71;
			m_bComboSelect[m_bComboList] = TempY/12;
		}
		break;
	case M_LEFTBUTTON_DOWN:
		if(-1 == m_bComboList)
		{
			C_VS_UI_EVENT_BUTTON* TempButton = m_pC_button_group->IsInRect(_x, _y);
			if(NULL != TempButton)
			{
				int ButtonID = TempButton->GetID();
				if(ButtonID>SMS_CLOSE_ID && ButtonID<SMS_CHAT_MESSAGE_ID)
				{
					m_bComboList = ButtonID - 4;
					m_ComboListPos.cx = TempButton->x - 128;
					if(g_eRaceInterface == RACE_SLAYER)
						m_ComboListPos.cy = TempButton->y - 47;
					else if(g_eRaceInterface == RACE_VAMPIRE)
						m_ComboListPos.cy = TempButton->y - 26;
					else if(g_eRaceInterface == RACE_OUSTERS)
					{
						m_ComboListPos.cx = TempButton->x -68;
						m_ComboListPos.cy = TempButton->y -65;
					}
				}
			}
		}

		break;
	case M_LEFTBUTTON_UP:
		m_bComboList = -1;
		break;
	case M_WHEEL_DOWN:
		if(-1 == m_bComboList)
		{
			C_VS_UI_EVENT_BUTTON* TempButton = m_pC_button_group->IsInRect(_x, _y);
			if(NULL != TempButton)
			{			
				int ButtonID = TempButton->GetID();
				if(ButtonID>SMS_CLOSE_ID && ButtonID<SMS_CHAT_MESSAGE_ID)
				{
					if(m_bComboSelect[ButtonID-SMS_CLOSE_ID -1]<5)
						m_bComboSelect[ButtonID-SMS_CLOSE_ID -1 ] ++;
				}
			}
		}
		break;
			
	case M_WHEEL_UP:
		if(-1 == m_bComboList)
		{
			C_VS_UI_EVENT_BUTTON* TempButton = m_pC_button_group->IsInRect(_x, _y);
			if(NULL != TempButton)
			{			
				int ButtonID = TempButton->GetID();
				if(ButtonID>SMS_CLOSE_ID && ButtonID<SMS_CHAT_MESSAGE_ID)
				{
					if(m_bComboSelect[ButtonID-SMS_CLOSE_ID -1]>0)
						m_bComboSelect[ButtonID-SMS_CLOSE_ID -1 ] --;
				}
			}
		}
		break;
	}
	
	m_pC_button_group->MouseControl(message, _x, _y);
	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_MESSAGE::IsPixel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_SMS_MESSAGE::IsPixel(int _x, int _y)
{
	bool re = false;

	re = IsInRect(_x, _y);
	if(re == false && m_SubWindow == true)
		re = m_sub_rect.IsInRect(_x, _y);

	return re;
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_MESSAGE::Run
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_MESSAGE::Run(id_t id)
{
	char *szMyNum,*szOtherNum[5],*szSMSMessage;
	int i;
	switch (id)
		{
		case SMS_CHAT_MESSAGE_ID:
			if(false == m_lev_SMSMessage.IsAcquire())
				m_lev_SMSMessage.Acquire();
			break;
		case SMS_CHAT_MINE_ID:
			if(false == m_lev_MyNum.IsAcquire())
				m_lev_MyNum.Acquire();
			break;
		case SMS_CHAT_OTHER1_ID:
			if(false == m_lev_OtherNum[0].IsAcquire())
				m_lev_OtherNum[0].Acquire();
			break;
		case SMS_CHAT_OTHER2_ID:
			if(false == m_lev_OtherNum[1].IsAcquire())
				m_lev_OtherNum[1].Acquire();
			break;
		case SMS_CHAT_OTHER3_ID:
			if(false == m_lev_OtherNum[2].IsAcquire())
				m_lev_OtherNum[2].Acquire();
			break;
		case SMS_CHAT_OTHER4_ID:
			if(false == m_lev_OtherNum[3].IsAcquire())
				m_lev_OtherNum[3].Acquire();
			break;
		case SMS_CHAT_OTHER5_ID:
			if(false == m_lev_OtherNum[4].IsAcquire())
				m_lev_OtherNum[4].Acquire();
			break;
		case SMS_SEND_ID:
			szMyNum = NULL;
			for(i = 0; i< 5; i++)
			{
				szOtherNum[i] = NULL;
				g_Convert_DBCS_Ascii2SingleByte(m_lev_OtherNum[i].GetStringWide(), m_lev_OtherNum[i].Size(), szOtherNum[i]);
			}
			szSMSMessage = NULL;
			g_Convert_DBCS_Ascii2SingleByte(m_lev_MyNum.GetStringWide(), m_lev_MyNum.Size(), szMyNum);
			g_Convert_DBCS_Ascii2SingleByte(m_lev_SMSMessage.GetStringWide(), m_lev_SMSMessage.Size(), szSMSMessage);

			if(szMyNum != NULL && szSMSMessage != NULL && ( szOtherNum[0] != NULL ||
															szOtherNum[1] != NULL ||
															szOtherNum[2] != NULL ||
															szOtherNum[3] != NULL ||
															szOtherNum[4] != NULL ))
			{
				
				m_szMyNum = m_szPhoneNum[m_bComboSelect[5]];
				m_szMyNum += szMyNum;
				m_szSMSMessage = szSMSMessage;

				m_szOtherNum.clear();
				std::string temp;

				for(int i = 0; i< 5; i++)
				{
					if(szOtherNum[i] != NULL && strlen(szOtherNum[i])>6)
					{
						temp = m_szPhoneNum[m_bComboSelect[i]];
						temp += szOtherNum[i];
						m_szOtherNum.push_back(temp);
					}
				}
				if(m_szOtherNum.size()<=g_char_slot_ingame.m_SMS_Charge && g_char_slot_ingame.m_SMS_Charge)
					gpC_base->SendMessage(UI_SEND_SMS_MESSAGE, (int)(intptr_t)m_szMyNum.c_str(), (int)(intptr_t)m_szSMSMessage.c_str(), &m_szOtherNum);
				else
					gpC_base->SendMessage(UI_MESSAGE_BOX, UI_STRING_MESSAGE_SMS_FAIL_NOT_ENOUGH_CHARGE, 0, 	NULL);
			}
				
			DeleteNew(szMyNum);
			DeleteNew(szSMSMessage);
			for(i=0; i<5; i++)
				DeleteNew(szOtherNum[i]);
			break;
		case SMS_PHONE_LIST_ID:
			if(gC_vs_ui.IsRunningSMSList())
			{
				gC_vs_ui.CloseSMSList();
				if(gC_vs_ui.IsRunningSMSRecord())
					gC_vs_ui.CloseSMSRecord();
			}
			else
				gpC_base->SendMessage(UI_SMS_OPEN_LIST);
			//	gC_vs_ui.RunSMSList();
			
			break;
		case SMS_SMILE_ID:
			m_SubWindow =! m_SubWindow;
			break;
		case SMS_CLOSE_ID:
			gpC_base->SendMessage(UI_CLOSE_SMS_MESSAGE);
			break;
	}
}


//----------------------------------------------------------------------------
// C_VS_UI_SMS_LIST::C_VS_UI_SMS_LIST
//
//
//----------------------------------------------------------------------------
C_VS_UI_SMS_LIST::C_VS_UI_SMS_LIST()
{
	g_RegisterWindow(this);
	
//	AttrTopmost(true);
	AttrKeyboardControl(true);
	m_SelectPos = -1;
	
	m_pC_button_group = new ButtonGroup(this);
	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		m_SMS_spk.Open(SPK_SLAYER_SMS_LIST);
		m_Main.cx = 300;
		m_Main.cy = 250;
			m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+10, m_Main.cy+114, m_SMS_spk.GetWidth(ADD_SEND_LIST_BUTTON_HILIGHT), m_SMS_spk.GetHeight(ADD_SEND_LIST_BUTTON_HILIGHT), SMS_ADD_SEND_ID, this, ADD_SEND_LIST_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+118, m_Main.cy+114, m_SMS_spk.GetWidth(DELETE_PHONE_LIST_BUTTON_HILIGHT), m_SMS_spk.GetHeight(DELETE_PHONE_LIST_BUTTON_HILIGHT), SMS_DELETE_LIST_ID, this, DELETE_PHONE_LIST_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+225, m_Main.cy+114, m_SMS_spk.GetWidth(NEW_PHONE_LIST_BUTTON_HILIGHT), m_SMS_spk.GetHeight(NEW_PHONE_LIST_BUTTON_HILIGHT), SMS_NEW_PHONE_ID, this, NEW_PHONE_LIST_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+293, m_Main.cy+7, m_SMS_spk.GetWidth(EXIT_BUTTON_HILIGHT), m_SMS_spk.GetHeight(EXIT_BUTTON_HILIGHT), SMS_CLOSE_PHONE_LIST_ID, this, EXIT_BUTTON_HILIGHT));

		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+29, m_Main.cy+11, m_SMS_spk.GetWidth(BUTTON_NAME_PUSH), m_SMS_spk.GetHeight(BUTTON_NAME_PUSH), SMS_SORT_NAME_ID, this, BUTTON_NAME_PUSH));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+92, m_Main.cy+12, m_SMS_spk.GetWidth(BUTTON_ID_PUSH), m_SMS_spk.GetHeight(BUTTON_ID_PUSH), SMS_SORT_CHAR_ID, this, BUTTON_ID_PUSH));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+206, m_Main.cy+12, m_SMS_spk.GetWidth(BUTTON_NUM_PUSH), m_SMS_spk.GetHeight(BUTTON_NUM_PUSH), SMS_SORT_NUMBER_ID, this, BUTTON_NUM_PUSH));
	
		
		m_pC_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(m_Main.cx+294,  m_Main.cy+ 34, 30, 66), false, SPK_SLAYER_SMS_SCROLL);
		
		break;
		
	case RACE_VAMPIRE:
		m_SMS_spk.Open(SPK_VAMPIRE_SMS_LIST);
		m_Main.cx = 340;
		m_Main.cy = 300;
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+8, m_Main.cy+112, m_SMS_spk.GetWidth(ADD_SEND_LIST_BUTTON_HILIGHT), m_SMS_spk.GetHeight(ADD_SEND_LIST_BUTTON_HILIGHT), SMS_ADD_SEND_ID, this, ADD_SEND_LIST_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+116, m_Main.cy+112, m_SMS_spk.GetWidth(DELETE_PHONE_LIST_BUTTON_HILIGHT), m_SMS_spk.GetHeight(DELETE_PHONE_LIST_BUTTON_HILIGHT), SMS_DELETE_LIST_ID, this, DELETE_PHONE_LIST_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+223, m_Main.cy+112, m_SMS_spk.GetWidth(NEW_PHONE_LIST_BUTTON_HILIGHT), m_SMS_spk.GetHeight(NEW_PHONE_LIST_BUTTON_HILIGHT), SMS_NEW_PHONE_ID, this, NEW_PHONE_LIST_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+291, m_Main.cy+5, m_SMS_spk.GetWidth(EXIT_BUTTON_HILIGHT), m_SMS_spk.GetHeight(EXIT_BUTTON_HILIGHT), SMS_CLOSE_PHONE_LIST_ID, this, EXIT_BUTTON_HILIGHT));
		
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+28, m_Main.cy+10, m_SMS_spk.GetWidth(BUTTON_NAME_PUSH), m_SMS_spk.GetHeight(BUTTON_NAME_PUSH), SMS_SORT_NAME_ID, this, BUTTON_NAME_PUSH));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+90, m_Main.cy+10, m_SMS_spk.GetWidth(BUTTON_ID_PUSH), m_SMS_spk.GetHeight(BUTTON_ID_PUSH), SMS_SORT_CHAR_ID, this, BUTTON_ID_PUSH));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+204, m_Main.cy+10, m_SMS_spk.GetWidth(BUTTON_NUM_PUSH), m_SMS_spk.GetHeight(BUTTON_NUM_PUSH), SMS_SORT_NUMBER_ID, this, BUTTON_NUM_PUSH));
	
		m_pC_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(m_Main.cx+292,  m_Main.cy+ 32, 30, 66), false, SPK_SLAYER_SMS_SCROLL);
		
		break;
		
	case RACE_OUSTERS:
		m_SMS_spk.Open(SPK_OUSTERS_SMS_LIST);
		m_Main.cx = 380;
		m_Main.cy = 300;
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+8, m_Main.cy+112, m_SMS_spk.GetWidth(ADD_SEND_LIST_BUTTON_HILIGHT), m_SMS_spk.GetHeight(ADD_SEND_LIST_BUTTON_HILIGHT), SMS_ADD_SEND_ID, this, ADD_SEND_LIST_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+116, m_Main.cy+112, m_SMS_spk.GetWidth(DELETE_PHONE_LIST_BUTTON_HILIGHT), m_SMS_spk.GetHeight(DELETE_PHONE_LIST_BUTTON_HILIGHT), SMS_DELETE_LIST_ID, this, DELETE_PHONE_LIST_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+223, m_Main.cy+112, m_SMS_spk.GetWidth(NEW_PHONE_LIST_BUTTON_HILIGHT), m_SMS_spk.GetHeight(NEW_PHONE_LIST_BUTTON_HILIGHT), SMS_NEW_PHONE_ID, this, NEW_PHONE_LIST_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+291, m_Main.cy+5, m_SMS_spk.GetWidth(EXIT_BUTTON_HILIGHT), m_SMS_spk.GetHeight(EXIT_BUTTON_HILIGHT), SMS_CLOSE_PHONE_LIST_ID, this, EXIT_BUTTON_HILIGHT));
		
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+28, m_Main.cy+10, m_SMS_spk.GetWidth(BUTTON_NAME_PUSH), m_SMS_spk.GetHeight(BUTTON_NAME_PUSH), SMS_SORT_NAME_ID, this, BUTTON_NAME_PUSH));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+90, m_Main.cy+10, m_SMS_spk.GetWidth(BUTTON_ID_PUSH), m_SMS_spk.GetHeight(BUTTON_ID_PUSH), SMS_SORT_CHAR_ID, this, BUTTON_ID_PUSH));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+204, m_Main.cy+10, m_SMS_spk.GetWidth(BUTTON_NUM_PUSH), m_SMS_spk.GetHeight(BUTTON_NUM_PUSH), SMS_SORT_NUMBER_ID, this, BUTTON_NUM_PUSH));
	
		m_pC_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(m_Main.cx+292,  m_Main.cy+ 32, 30, 66), false, SPK_SLAYER_SMS_SCROLL);
		
		break;
	}
		
	Set(m_Main.cx, m_Main.cy, m_SMS_spk.GetWidth(), m_SMS_spk.GetHeight());

	m_pC_scroll_bar->SetPosMax(0);

	AttrTopmost(true);
	AttrPin(true);
}

C_VS_UI_SMS_LIST::~C_VS_UI_SMS_LIST()
{
	g_UnregisterWindow(this);

	DeleteNew(m_pC_scroll_bar);
	DeleteNew( m_pC_button_group );
//	gC_vs_ui.AcquireSMS();
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_LIST::Process
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_LIST::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_LIST::UnacquireMouseFocus
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_LIST::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_LIST::Start
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_LIST::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();
	
//	m_lev_SMSMessage.Acquire();
	
	AttrTopmost(true);
	gpC_window_manager->AppearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_LIST::Finish
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_LIST::Finish()
{
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
}
void	C_VS_UI_SMS_LIST::SetSMSList(void *pVoid)
{
	std::vector<AddressUnit*>*	NewList = (std::vector<AddressUnit*>*)pVoid;
	std::vector<AddressUnit*>::iterator itr = NewList->begin();
	m_Addresses.clear();
	while(itr != NewList->end())
	{
		m_Addresses.push_back((*itr));
		itr++;
	}

	m_pC_scroll_bar->SetPosMax(m_Addresses.size()-4);
}

void	C_VS_UI_SMS_LIST::AddList(DWORD element, char* name, char* id, char* num) 
{
	AddressUnit* _AddressUnit= new AddressUnit;
	_AddressUnit->ElementID = element;
	_AddressUnit->CharacterName = name;
	_AddressUnit->CustomName = id;
	_AddressUnit->Number = num;

	m_Addresses.push_back(_AddressUnit);

	m_pC_scroll_bar->SetPosMax(m_Addresses.size());
}
void	C_VS_UI_SMS_LIST::DeleteList(int id)
{
	AddressUnit* _AddressUnit = NULL;
	std::vector<AddressUnit*>::iterator itr = m_Addresses.begin();
	while(itr != m_Addresses.end())
	{
		_AddressUnit = (*itr);
		if(_AddressUnit->ElementID == id)
		{
			m_Addresses.erase(itr);
			break;
		}
		itr++;
	}
	DeleteNew(_AddressUnit);
	m_pC_scroll_bar->SetPosMax(m_Addresses.size());
}
	
//-----------------------------------------------------------------------------
// C_VS_UI_SMS_LIST::Show
//
//
//-----------------------------------------------------------------------------
void C_VS_UI_SMS_LIST::Show()
{
	
	int ScrPos = m_pC_scroll_bar->GetScrollPos();
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_SMS_spk.BltLocked(x, y);
		if(-1 != m_SelectPos && m_SelectPos>=ScrPos && m_SelectPos<= ScrPos+4)
		{
			int Temp = 0;
			if(g_eRaceInterface != RACE_SLAYER)
				Temp = -2;
			int TempPos = (m_SelectPos-ScrPos)%5;
			m_SMS_spk.BltLocked(x+11+Temp, y+25+(TempPos*17)+Temp, SELECT_PHONE_LIST_1);
			m_SMS_spk.BltLocked(x+29+Temp, y+25+(TempPos*17)+Temp, SELECT_PHONE_LIST_2);
			m_SMS_spk.BltLocked(x+91+Temp, y+25+(TempPos*17)+Temp, SELECT_PHONE_LIST_3);
			m_SMS_spk.BltLocked(x+205+Temp, y+25+(TempPos*17)+Temp, SELECT_PHONE_LIST_4);
		}
		m_pC_button_group->Show();
	
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	m_pC_scroll_bar->Show2(0,0);


	if(g_FL2_GetDC())
	{
		int i;
		char szString[32];
		for(i =ScrPos; i< m_Addresses.size(); i++)
		{
			if(i>ScrPos+4)
				break;
			AddressUnit* TempInfo = (AddressUnit*)m_Addresses[i];
			if(NULL != TempInfo)
			{
//				DWORD TempColor = m_Color;
//				if(i == m_SelectPos)
//					TempColor = RGB_BLACK;

				wsprintf(szString, "%d" , i+1);
				g_PrintColorStr(m_Main.cx+12, m_Main.cy+27 + (i-ScrPos)*17, szString, gpC_base->m_chatting_pi, RGB_BLACK);
				wsprintf(szString, "%s" , TempInfo->CharacterName.c_str());
				g_PrintColorStr(m_Main.cx+30, m_Main.cy+27 + (i-ScrPos)*17, szString, gpC_base->m_chatting_pi, RGB_BLACK);
				wsprintf(szString, "%s" , TempInfo->CustomName.c_str());
				g_PrintColorStr(m_Main.cx+92, m_Main.cy+27 + (i-ScrPos)*17, szString, gpC_base->m_chatting_pi, RGB_BLACK);
				wsprintf(szString, "%s" , TempInfo->Number.c_str());
				g_PrintColorStr(m_Main.cx+206, m_Main.cy+27 + (i-ScrPos)*17, szString, gpC_base->m_chatting_pi, RGB_BLACK);
			}
		}
		g_FL2_ReleaseDC();
	}

	m_pC_button_group->ShowDescription();
	SHOW_WINDOW_ATTR;
}

void	C_VS_UI_SMS_LIST::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	const static char*	m_help_string[4] = {
		(*g_pGameStringTable)[UI_STRING_HELP_SMS_ADDSEND].GetString(),
		(*g_pGameStringTable)[UI_STRING_HELP_SMS_DELETE].GetString(),
		(*g_pGameStringTable)[UI_STRING_HELP_SMS_NEW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
	};
	
	if (p_button->GetID() < 4)
		g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()],0,0);	
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_LIST::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_LIST::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	assert(p_button);
	
	if(p_button->GetFocusState())
	{
		if(p_button->GetID()>SMS_CLOSE_PHONE_LIST_ID)
		{
			if(p_button->GetPressState())
				m_SMS_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index);
		}
		else
		{
			if(p_button->GetPressState())
				m_SMS_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index+1);
			else
				m_SMS_spk.BltLocked(p_button->x, p_button->y, p_button->m_image_index);
		}
	}
}

//-----------------------------------------------------------------------------
//
// keyboard
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_LIST::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);
//	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_ESCAPE: 
			if(gC_vs_ui.IsRunningSMSRecord())
				gpC_base->SendMessage(UI_CLOSE_SMS_RECORD);
			gpC_base->SendMessage(UI_CLOSE_SMS_LIST);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_LIST::MouseControl
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_SMS_LIST::MouseControl(UINT message, int _x, int _y)
{
//	int scroll_tag_x = x+365, scroll_tag_y = y+108, scroll_tag_height = 198;
//	
	Window::MouseControl(message, _x, _y);

	bool re = false;
	if(NULL != m_pC_button_group)
		re = m_pC_button_group->MouseControl(message, _x, _y);
	if(NULL != m_pC_scroll_bar)
		re &= m_pC_scroll_bar->MouseControl(message, _x, _y);
//	
	switch(message)
	{
	case M_WHEEL_UP:
		m_pC_scroll_bar->ScrollUp();
		break;
		
	case M_WHEEL_DOWN:
		m_pC_scroll_bar->ScrollDown();
		break;
	case M_LEFTBUTTON_UP:
		if(_x>m_Main.cx + 11 && _x<m_Main.cx + 290 && _y>m_Main.cy + 25 && _y <m_Main.cy + 108)
		{
			m_SelectPos = (_y-25-m_Main.cy )/17;
			m_SelectPos += m_pC_scroll_bar->GetScrollPos();
		}
		break;
	}
//	
	
//	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_LIST::IsPixel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_SMS_LIST::IsPixel(int _x, int _y)
{
	bool re = false;
//	re = m_SMS_spk.IsPixel(SCR2WIN_X(_x), SCR2WIN_Y(_y));
//	if(re == false)
//	{
//		if(m_SMS_spk.IsPixel(_x - (x+w-14), _y - (y+h-49), CLOSE_BUTTON) ||
//			m_SMS_spk.IsPixel(_x - (x+w-14), _y - (y+h-49-30), JOIN_BUTTON))
//			re = true;
//	}
	return IsInRect(_x, _y);
	return re;
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_LIST::Run
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_LIST::Run(id_t id)
{
//	char *szMyNum,*szOtherNum[5],*szSMSMessage;
	switch (id)
		{
		case SMS_ADD_SEND_ID:
			if(-1 != m_SelectPos)
			{
				if(m_SelectPos< m_Addresses.size())
				{
					AddressUnit* _AddressUnit = m_Addresses[m_SelectPos];
					gpC_base->SendMessage(UI_SMS_ADD_SEND_LIST, (int)(intptr_t)_AddressUnit->Number.c_str(), 0, NULL);
				}
			}
			break;
		case SMS_DELETE_LIST_ID:
			if(-1 != m_SelectPos)
			{
				if(m_SelectPos< m_Addresses.size())
				{
					AddressUnit* _AddressUnit = m_Addresses[m_SelectPos];
					gpC_base->SendMessage(UI_SMS_DELETE, _AddressUnit->ElementID,0, NULL);
				}
			}
			
			break;
		case SMS_NEW_PHONE_ID:
			if(gC_vs_ui.IsRunningSMSRecord())
				gpC_base->SendMessage(UI_CLOSE_SMS_RECORD);
			else
				gC_vs_ui.RunSMSRecord();
			break;
		case SMS_CLOSE_PHONE_LIST_ID:
			if(gC_vs_ui.IsRunningSMSRecord())
				gpC_base->SendMessage(UI_CLOSE_SMS_RECORD);
			gpC_base->SendMessage(UI_CLOSE_SMS_LIST);
			break;
		case SMS_SORT_NAME_ID:
			Address_Sort(SORT_BY_NAME);
			break;
		case SMS_SORT_CHAR_ID:
			Address_Sort(SORT_BY_ID);
			break;
		case SMS_SORT_NUMBER_ID:
			Address_Sort(SORT_BY_NUM);
			break;


	}
} 


class CAddress_By_Name
{
public:
	bool operator()(const C_VS_UI_SMS_LIST::AddressUnit* p, const C_VS_UI_SMS_LIST::AddressUnit* q) const
	{
		return strcmp(q->CustomName.c_str(), p->CustomName.c_str())>0;
	}
};
class CAddress_By_Id
{
public:
	bool operator()(const C_VS_UI_SMS_LIST::AddressUnit* p, const C_VS_UI_SMS_LIST::AddressUnit* q) const
	{
		return strcmp(q->CharacterName.c_str(), p->CharacterName.c_str())>0;
	}
};

class CAddress_By_Num
{
public:
	bool operator()(const C_VS_UI_SMS_LIST::AddressUnit* p, const C_VS_UI_SMS_LIST::AddressUnit* q) const
	{
		return strcmp(q->Number.c_str(), p->Number.c_str())>0;
	}
};

void C_VS_UI_SMS_LIST::Address_Sort(int nSort)
{
	int nSize = m_Addresses.size();
	if(nSize < 2) return;
	if(nSort == SORT_BY_NAME)
	{
		std::sort(m_Addresses.begin(), m_Addresses.end(), CAddress_By_Name());
	}
	else if( nSort == SORT_BY_ID)
	{
		std::sort(m_Addresses.begin(), m_Addresses.end(), CAddress_By_Id());
	}
	else if( nSort == SORT_BY_NUM)
	{
		std::sort(m_Addresses.begin(), m_Addresses.end(), CAddress_By_Num());
	}
	m_pC_scroll_bar->SetScrollPos(0);
	m_SelectPos = 0;

}
 
//----------------------------------------------------------------------------
// C_VS_UI_SMS_RECORD::C_VS_UI_SMS_RECORD
//
//
//----------------------------------------------------------------------------
C_VS_UI_SMS_RECORD::C_VS_UI_SMS_RECORD()
{
	g_RegisterWindow(this);
	
//	AttrTopmost(true);
	AttrKeyboardControl(true);
	wsprintf(m_szPhoneNum[0], "010");
	wsprintf(m_szPhoneNum[1], "011");
	wsprintf(m_szPhoneNum[2], "016");
	wsprintf(m_szPhoneNum[3], "017");
	wsprintf(m_szPhoneNum[4], "018");
	wsprintf(m_szPhoneNum[5], "019");
	m_bComboList = -1;
	m_bComboSelect = 0;
	m_pC_button_group = new ButtonGroup(this);
	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		m_SMS_spk.Open(SPK_SLAYER_SMS_RECORD);
		m_Main.cx = 300;
		m_Main.cy = 120;
		m_ComboPos.cx = m_Main.cx+62;
		m_ComboPos.cy = m_Main.cy+50;
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+63, m_Main.cy+69, m_SMS_spk.GetWidth(RECORD_PHONE_LIST_BUTTON_HILIGHT), m_SMS_spk.GetHeight(RECORD_PHONE_LIST_BUTTON_HILIGHT), SMS_RECORD_PHONE_ID, this, RECORD_PHONE_LIST_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+165, m_Main.cy+76, m_SMS_spk.GetWidth(EXIT_BUTTON_HILIGHT), m_SMS_spk.GetHeight(EXIT_BUTTON_HILIGHT), SMS_EXIT_ID, this, EXIT_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos.cx, m_ComboPos.cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_NEW_5, this, COMBO_BUTTON_HILIGHT));

		m_lev_AddName.SetPosition(m_Main.cx+66, m_Main.cy+12);
		m_lev_AddID.SetPosition(m_Main.cx+66, m_Main.cy+31);
		m_lev_AddNum.SetPosition(m_Main.cx+112, m_Main.cy+50);

		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+66, m_Main.cy+12, 115, 15, SMS_CHAT_NAME_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+66, m_Main.cy+31, 115, 15, SMS_CHAT_ID_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+112, m_Main.cy+50, 65, 15, SMS_CHAT_NUM_ID, this, 0));
		break;
		
	case RACE_VAMPIRE:
		m_SMS_spk.Open(SPK_VAMPIRE_SMS_RECORD);
		m_Main.cx = 340;
		m_Main.cy = 200;
		m_ComboPos.cx = m_Main.cx+58;
		m_ComboPos.cy = m_Main.cy+48;
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+61, m_Main.cy+67, m_SMS_spk.GetWidth(RECORD_PHONE_LIST_BUTTON_HILIGHT), m_SMS_spk.GetHeight(RECORD_PHONE_LIST_BUTTON_HILIGHT), SMS_RECORD_PHONE_ID, this, RECORD_PHONE_LIST_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+163, m_Main.cy+74, m_SMS_spk.GetWidth(EXIT_BUTTON_HILIGHT), m_SMS_spk.GetHeight(EXIT_BUTTON_HILIGHT), SMS_EXIT_ID, this, EXIT_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos.cx, m_ComboPos.cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_NEW_5, this, COMBO_BUTTON_HILIGHT));

		m_lev_AddName.SetPosition(m_Main.cx+64, m_Main.cy+10);
		m_lev_AddID.SetPosition(m_Main.cx+64, m_Main.cy+29);
		m_lev_AddNum.SetPosition(m_Main.cx+110, m_Main.cy+48);
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+64, m_Main.cy+10, 115, 15, SMS_CHAT_NAME_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+64, m_Main.cy+29, 115, 15, SMS_CHAT_ID_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+110, m_Main.cy+48, 65, 15, SMS_CHAT_NUM_ID, this, 0));
		
		break;
		
	case RACE_OUSTERS:
		m_SMS_spk.Open(SPK_OUSTERS_SMS_RECORD);
		m_Main.cx = 380;
		m_Main.cy = 200;
		m_ComboPos.cx = m_Main.cx+60;
		m_ComboPos.cy = m_Main.cy+48;
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+61, m_Main.cy+67, m_SMS_spk.GetWidth(RECORD_PHONE_LIST_BUTTON_HILIGHT), m_SMS_spk.GetHeight(RECORD_PHONE_LIST_BUTTON_HILIGHT), SMS_RECORD_PHONE_ID, this, RECORD_PHONE_LIST_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+163, m_Main.cy+74, m_SMS_spk.GetWidth(EXIT_BUTTON_HILIGHT), m_SMS_spk.GetHeight(EXIT_BUTTON_HILIGHT), SMS_EXIT_ID, this, EXIT_BUTTON_HILIGHT));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_ComboPos.cx, m_ComboPos.cy, m_SMS_spk.GetWidth(COMBO_BUTTON_HILIGHT)+30, m_SMS_spk.GetHeight(COMBO_BUTTON_HILIGHT), SMS_COMBO_NEW_5, this, COMBO_BUTTON_HILIGHT));

		m_lev_AddName.SetPosition(m_Main.cx+64, m_Main.cy+10);
		m_lev_AddID.SetPosition(m_Main.cx+64, m_Main.cy+29);
		m_lev_AddNum.SetPosition(m_Main.cx+110, m_Main.cy+48);
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+64, m_Main.cy+10, 115, 15, SMS_CHAT_NAME_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+64, m_Main.cy+29, 115, 15, SMS_CHAT_ID_ID, this, 0));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(m_Main.cx+110, m_Main.cy+48, 65, 15, SMS_CHAT_NUM_ID, this, 0));

		break;
	}
	
	Set(m_Main.cx, m_Main.cy, m_SMS_spk.GetWidth(), m_SMS_spk.GetHeight()+30);

	

	m_lev_AddName.SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_AddName.SetByteLimit(8);
	m_lev_AddName.SetInputStringColor(RGB_BLACK);
	m_lev_AddName.SetCursorColor(RGB_BLACK);
	Attach(&m_lev_AddName);

	m_lev_AddID.SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_AddID.SetByteLimit(16);
	m_lev_AddID.SetInputStringColor(RGB_BLACK);
	m_lev_AddID.SetCursorColor(RGB_BLACK);
	
	m_lev_AddNum.SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_AddNum.SetByteLimit(8);
	m_lev_AddNum.SetInputStringColor(RGB_BLACK);
	m_lev_AddNum.SetCursorColor(RGB_BLACK);
	m_lev_AddNum.SetDigitOnlyMode(true);

	AttrTopmost(true);
	AttrPin(true);
}

C_VS_UI_SMS_RECORD::~C_VS_UI_SMS_RECORD()
{
	g_UnregisterWindow(this);
	
	m_lev_AddName.Unacquire();
	gC_vs_ui.AcquireSMS();
	DeleteNew( m_pC_button_group );
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_RECORD::Process
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_RECORD::Process()
{
	m_pC_button_group->Process();
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_RECORD::UnacquireMouseFocus
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_RECORD::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_RECORD::Start
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_RECORD::Start()
{
	PI_Processor::Start();
	m_pC_button_group->Init();
	
	m_lev_AddName.Acquire();
	
	AttrTopmost(true);
	gpC_window_manager->AppearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_RECORD::Finish
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_RECORD::Finish()
{
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_RECORD::Show
//
//
//-----------------------------------------------------------------------------
void C_VS_UI_SMS_RECORD::Show()
{
//	const int scroll_tag_x = x+365+m_vampire_point.x, 
//		scroll_tag_y = y+108+m_vampire_point.y, 
//		scroll_tag_height = 198;
//	const int line_gap = 12;
//	
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		m_SMS_spk.BltLocked(x, y);
		m_pC_button_group->Show();
		
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	if(g_FL2_GetDC())
	{
		g_PrintColorStr(m_ComboPos.cx-24+30, m_ComboPos.cy+1, m_szPhoneNum[m_bComboSelect], gpC_base->m_chatting_pi, RGB_BLACK);
		g_FL2_ReleaseDC();
	}
	if(-1 != m_bComboList)
	{
		if(gpC_base->m_p_DDSurface_back->Lock())
		{
			m_SMS_spk.BltLocked(x+m_ComboListPos.cx+30, y+m_ComboListPos.cy,COMBO_LIST);
			m_SMS_spk.BltLocked(x+m_ComboListPos.cx+30, y+m_ComboListPos.cy+(m_bComboSelect*12),COMBO_SELECTER);
		}
		gpC_base->m_p_DDSurface_back->Unlock();
		if(g_FL2_GetDC())
		{
			for(int i = 0; i< 6; i++)
				g_PrintColorStr(x+m_ComboListPos.cx+2+30, y+m_ComboListPos.cy+(i*12), m_szPhoneNum[i], gpC_base->m_chatting_pi, RGB_BLACK);
			g_FL2_ReleaseDC();
		}
	}
	m_lev_AddNum.Show();
	m_lev_AddName.Show();
	m_lev_AddID.Show();

	m_pC_button_group->ShowDescription();
	SHOW_WINDOW_ATTR;
}

void	C_VS_UI_SMS_RECORD::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	const static char*	m_help_string[2] = {
		(*g_pGameStringTable)[UI_STRING_HELP_SMS_NEW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
	};
	
	if (p_button->GetID() < 2)
		g_descriptor_manager.Set(DID_INFO, p_button->x, p_button->y, (void *)m_help_string[p_button->GetID()],0,0);	
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_RECORD::ShowButtonWidget
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_RECORD::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	assert(p_button);
//	
//	
	int TempX = 0;
	if(p_button->GetFocusState())
	{
		if(p_button->GetID() > SMS_COMBO_NEW_5)
			return;
		if(p_button->GetID()  == SMS_COMBO_NEW_5)
			TempX = 30;
		if(p_button->GetPressState())
			m_SMS_spk.BltLocked(p_button->x+TempX, p_button->y, p_button->m_image_index+1);
		else
			m_SMS_spk.BltLocked(p_button->x+TempX, p_button->y, p_button->m_image_index);
	}

}

//-----------------------------------------------------------------------------
//
// keyboard
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_RECORD::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);
//	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_TAB:
			if(m_lev_AddName.IsAcquire())
				m_lev_AddID.Acquire();
			else if(m_lev_AddID.IsAcquire())
				m_lev_AddNum.Acquire();
			else if(m_lev_AddNum.IsAcquire())
				m_lev_AddName.Acquire();
			break;
			
		case VK_RETURN:
			if(m_lev_AddName.IsAcquire())
				m_lev_AddID.Acquire();
			else if(m_lev_AddID.IsAcquire())
				m_lev_AddNum.Acquire();
			else if(m_lev_AddNum.IsAcquire())
				m_lev_AddName.Acquire();
			break;
			
		case VK_ESCAPE: 
			gpC_base->SendMessage(UI_CLOSE_SMS_RECORD);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_RECORD::MouseControl
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_SMS_RECORD::MouseControl(UINT message, int _x, int _y)
{
//	int scroll_tag_x = x+365, scroll_tag_y = y+108, scroll_tag_height = 198;
//	
	Window::MouseControl(message, _x, _y);
//	
	switch(message)
	{
	case M_MOVING:
		if(-1 != m_bComboList)
		{
			int TempY = _y - m_ComboListPos.cy - m_Main.cy;
			if(TempY<0) TempY = 0;
			if(TempY>71) TempY = 71;
			m_bComboSelect = TempY/12;
		}
		break;
	case M_LEFTBUTTON_DOWN:
		if(-1 == m_bComboList)
		{
			C_VS_UI_EVENT_BUTTON* TempButton = m_pC_button_group->IsInRect(_x, _y);
			if(NULL != TempButton)
			{
				int ButtonID = TempButton->GetID();
				if(ButtonID == SMS_COMBO_NEW_5)
				{
					m_bComboList = 0;
					if(g_eRaceInterface == RACE_SLAYER)
					{
						m_ComboListPos.cx = TempButton->x - 330;
						m_ComboListPos.cy = TempButton->y - 106;
					}
					else if(g_eRaceInterface == RACE_VAMPIRE)
					{
						m_ComboListPos.cx = TempButton->x - 368;
						m_ComboListPos.cy = TempButton->y - 188;
					}
					else if(g_eRaceInterface == RACE_OUSTERS)
					{
						m_ComboListPos.cx = TempButton->x -410;
						m_ComboListPos.cy = TempButton->y -188;
					}
					//m_bComboSelect[m_bComboList] = 0;
				}
			}
		}
//		if( re )
//		{
//			MoveReady();
//			SetOrigin(_x, _y);
//		}

		break;
	case M_LEFTBUTTON_UP:
		m_bComboList = -1;
		break;
	case M_WHEEL_DOWN:
		if(-1 == m_bComboList)
		{
			C_VS_UI_EVENT_BUTTON* TempButton = m_pC_button_group->IsInRect(_x, _y);
			if(NULL != TempButton)
			{			
				if(SMS_COMBO_NEW_5 == TempButton->GetID())
				{
					if(m_bComboSelect<5)
						m_bComboSelect ++;
				}
			}
		}
		break;
			
	case M_WHEEL_UP:
		if(-1 == m_bComboList)
		{
			C_VS_UI_EVENT_BUTTON* TempButton = m_pC_button_group->IsInRect(_x, _y);
			if(NULL != TempButton)
			{			
				if(SMS_COMBO_NEW_5 == TempButton->GetID())
				{
					if(m_bComboSelect>0)
						m_bComboSelect --;
				}
			}
		}
	}
//	
	m_pC_button_group->MouseControl(message, _x, _y);
//	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_RECORD::IsPixel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI_SMS_RECORD::IsPixel(int _x, int _y)
{
	return IsInRect(_x, _y);
}

//-----------------------------------------------------------------------------
// C_VS_UI_SMS_RECORD::Run
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI_SMS_RECORD::Run(id_t id)
{
	char *szNum,*szID,*szName;
	switch (id)
		{
		case SMS_CHAT_NAME_ID:
			if(false == m_lev_AddName.IsAcquire())
				m_lev_AddName.Acquire();
			break;
		case SMS_CHAT_ID_ID:
			if(false == m_lev_AddID.IsAcquire())
				m_lev_AddID.Acquire();
			break;
		case SMS_CHAT_NUM_ID:
			if(false == m_lev_AddNum.IsAcquire())
				m_lev_AddNum.Acquire();
			break;
		case SMS_RECORD_PHONE_ID:
			szNum = NULL;
			szID = NULL;
			szName = NULL;
			g_Convert_DBCS_Ascii2SingleByte(m_lev_AddName.GetStringWide(), m_lev_AddName.Size(), szName);
			g_Convert_DBCS_Ascii2SingleByte(m_lev_AddID.GetStringWide(), m_lev_AddID.Size(), szID);
			g_Convert_DBCS_Ascii2SingleByte(m_lev_AddNum.GetStringWide(), m_lev_AddNum.Size(), szNum);

			if(szID!= NULL && szName != NULL && szNum != NULL )
			{
				m_szNum = m_szPhoneNum[m_bComboSelect];
				m_szNum += szNum;
				m_szID = szID;
				m_szName = szName;

			gpC_base->SendMessage(UI_SMS_RECORD, (int)(intptr_t)m_szName.c_str(), (int)(intptr_t)m_szID.c_str(), (void*)m_szNum.c_str());
			}
				
			DeleteNew(szNum);
			DeleteNew(szID);
			DeleteNew(szName);

			break;
		case SMS_EXIT_ID:
			gpC_base->SendMessage(UI_CLOSE_SMS_RECORD);
			break;
		case SMS_COMBO_NEW_5:
			m_lev_AddNum.Acquire();
			break;
	}
}



//------------------------------------------------------------------------------
// C_VS_UI_NAMING
//
// ���̹� ����Ʈ
//------------------------------------------------------------------------------
C_VS_UI_NAMING::C_VS_UI_NAMING(std::vector<C_VS_UI_NicknameInfo*> &_NamingList)
{	
	
	g_RegisterWindow(this);	
//	AttrKeyboardControl(true);

//	AttrTopmost(true);
	AttrPin(true);	
	
	int window_w = 220;
	int window_h = 280;

	m_SelectPos = -1;
//	m_CustomIndex = -1;
	
	Set(g_GameRect.right/2-window_w/2, g_GameRect.bottom/2 - window_h/2, window_w, window_h);
//add by viva
//	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW));
//	AttrAutoHide(gpC_vs_ui_window_manager->GetAutoHide(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW));
//	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW);
//	if(rect.w != -1)
//	{ 
//		x = rect.x;
//		y = rect.y;
//	}
	
	m_pC_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(w-35,85,-1,window_h-150));
	m_pC_scroll_bar->SetPosMax(0);

	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		m_pC_spk.Open(SPK_SLAYER_NAMING);
		break;

	case RACE_VAMPIRE:
		m_pC_spk.Open(SPK_VAMPIRE_NAMING);
		break;

	case RACE_OUSTERS:
		m_pC_spk.Open(SPK_OUSTERS_NAMING);
		break;		
	}
	
//	int addok_x = w-40, addok_y = h-238;
	int close_x = w-24, close_y = h-19;		
	int select_x = w-140, select_y = h-45;		
	int pin_x = w-24-20, pin_y = h-19;
	int alpha_x = 6, alpha_y = h-21;

	//�����ư
	m_pC_button_group = new ButtonGroup(this);
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_x, close_y, 
		gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::BUTTON_CLOSE), 
		gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::BUTTON_CLOSE), 
		CLOSE_ID, this, C_GLOBAL_RESOURCE::BUTTON_CLOSE));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(select_x, select_y, 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_SELECT), 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_SELECT),
		SELECT_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_SELECT));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pin_x, pin_y, 
		gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::BUTTON_HELP_BACK), 
		gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::BUTTON_HELP_BACK), 
		HELP_ID, this, C_GLOBAL_RESOURCE::BUTTON_HELP_BACK));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(alpha_x, alpha_y,
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA),
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA),
		ALPHA_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA));	

	SetNameList(_NamingList);
	
}

C_VS_UI_NAMING::~C_VS_UI_NAMING()
{
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW, Rect(x, y, w, h));	
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW, GetAttributes()->alpha);

	g_UnregisterWindow(this);
	
//	m_AddName.Unacquire();
//	gC_vs_ui.AcquireChatting();
	DeleteNew(m_pC_scroll_bar);
	DeleteNew(m_pC_button_group);	
}

void	C_VS_UI_NAMING::Start()
{//add by viva
//	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW));
//	AttrAutoHide(gpC_vs_ui_window_manager->GetAutoHide(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW));
//	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW);
//	if(rect.w != -1)
//	{ 
//		x = rect.x;
//		y = rect.y;
//	}
	PI_Processor::Start();
//	AttrTopmost(true);	
	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();
//	m_AddName.Acquire();
	

	
	
}

void	C_VS_UI_NAMING::Finish()
{
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW, Rect(x, y, w, h));	
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW, GetAttributes()->alpha);

	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);	
}


void	C_VS_UI_NAMING::Show()
{

	static DWORD SECOND_PET_QUEST = 0xffff;
	const int tab_x = 0;
	const int tab_y = 60;
	int ScrPos = m_pC_scroll_bar->GetScrollPos();

	gpC_global_resource->DrawDialog(x, y, w, h, GetAttributes()->alpha);

	RECT rect2 = { x+20, y+70, x+185, y+230 };
	Rect rt2(rect2.left, rect2.top, rect2.right - rect2.left, rect2.bottom - rect2.top );

	gpC_base->m_p_DDSurface_back->FillRect(&rect2, 0);
	gpC_global_resource->DrawOutBox(rt2);

	if(-1 != m_SelectPos && m_SelectPos>=ScrPos && m_SelectPos<= ScrPos+8)
	{
		WORD TempColor = CSDLGraphics::Color(192,192,192);
		int TempPos = (m_SelectPos-ScrPos)%9;
		RECT rect3 = { x+23, y+75+(TempPos*17),x+180,y+72+(TempPos*17) +19} ;
		C_VS_UI_NicknameInfo* TempInfo = (C_VS_UI_NicknameInfo*)m_NameList[m_SelectPos];
		if(NULL != TempInfo)
		{
			switch(TempInfo->getNicknameType())
			{
			case C_VS_UI_NicknameInfo::NICK_BUILT_IN:
				TempColor = CSDLGraphics::Color(255,0,0);
				break;
			case C_VS_UI_NicknameInfo::NICK_QUEST:
				TempColor = CSDLGraphics::Color(150,150,150);
				break;
			
			}
		}
	//	if(TempColor)
			gpC_base->m_p_DDSurface_back->FillRect(&rect3, TempColor);
	}
	
	if ( gpC_base->m_p_DDSurface_back->Lock() )
	{
		m_pC_spk.BltLocked( rect2.left+44, rect2.top-34, NICKNAME_LIST_STR);
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(rect2.left, rect2.top-47, C_GLOBAL_RESOURCE::AB_TITLE_DECORATION_LEFT);
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(rect2.right-28, rect2.top-47, C_GLOBAL_RESOURCE::AB_TITLE_DECORATION_RIGHT);

		m_pC_button_group->Show();
		
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	m_pC_scroll_bar->Show(x,y);

	if(g_FL2_GetDC())
	{
		int i;
		char szString[64];
		for(i =ScrPos; i< m_NameList.size(); i++)
		{
			if(i>ScrPos+8)
				break;
			C_VS_UI_NicknameInfo* TempInfo = (C_VS_UI_NicknameInfo*)m_NameList[i];
			if(NULL != TempInfo)
			{
				DWORD CustomColor;
				switch(TempInfo->getNicknameType())
				{
				case C_VS_UI_NicknameInfo::NICK_BUILT_IN:
					CustomColor = RGB_YELLOW;
					break;
				case C_VS_UI_NicknameInfo::NICK_QUEST:
					CustomColor = RGB_BLUE;
					break;
				case C_VS_UI_NicknameInfo::NICK_FORCED:
				case C_VS_UI_NicknameInfo::NICK_CUSTOM_FORCED:
					CustomColor = RGB_RED;
					break;
				case C_VS_UI_NicknameInfo::NICK_CUSTOM:
					CustomColor = 	RGB(50,170,230);
					break;
				default:
					CustomColor = RGB_YELLOW;
					break;
					
				}
				wsprintf(szString, "%s" , (TempInfo->getNickname()).c_str());
				if(i == m_SelectPos)
					g_PrintColorStr(x+24, y+78 + (i-ScrPos)*17, szString, gpC_base->m_user_id_pi, CustomColor);
				else
					g_PrintColorStr(x+24, y+78 + (i-ScrPos)*17, szString, gpC_base->m_chatting_pi, CustomColor);
			}
		}
		m_pC_button_group->ShowDescription();
		g_FL2_ReleaseDC();
	}	

	SHOW_WINDOW_ATTR;
}

void	C_VS_UI_NAMING::ShowDesc(int strX,int strY,const char *str)
{
	return;

}


bool	C_VS_UI_NAMING::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);

	if(NULL != m_pC_scroll_bar)
	re &= m_pC_scroll_bar->MouseControl(message, _x, _y);
//	

	switch(message)
	{
	case M_MOVING:
		{

		}
		break;
	case M_WHEEL_UP:
		m_pC_scroll_bar->ScrollUp();
		break;
		
	case M_WHEEL_DOWN:
		m_pC_scroll_bar->ScrollDown();
		break;

	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		{
			if(_x>23 && _x<160 && _y>74&& _y <224)
			{
				m_SelectPos = (_y-74 )/17;
				m_SelectPos += m_pC_scroll_bar->GetScrollPos();
				if(m_SelectPos>=m_NameList.size())
					m_SelectPos = m_NameList.size()-1;
			}
			else if(re && NULL == m_pC_button_group->IsInRect(_x, _y))
			{
				MoveReady();
				SetOrigin(_x, _y);
			}
			break;
		}
		break;
	}	
	g_descriptor_manager.Unset();
	return true;
}

void	C_VS_UI_NAMING::KeyboardControl(UINT message, UINT key, long extra)	
{	
//	Window::KeyboardControl(message, key, extra);
//	if (message == WM_KEYDOWN)
//	{
//		switch (key)
//		{		
//		case VK_ESCAPE :
//			gpC_base->SendMessage(UI_CLOSE_NAMING);
//			break;		
//		}
//	}
}

bool	C_VS_UI_NAMING::IsPixel(int _x,int _y)
{
	return IsInRect(_x, _y);
}

void	C_VS_UI_NAMING::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetID() == ALPHA_ID)
	{
		if(GetAttributes()->alpha)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
	} else

	if(p_button->GetID() == HELP_ID)
	{
		gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x-5, y+p_button->y-5, C_GLOBAL_RESOURCE::BUTTON_HELP_BACK);
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
			else
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+3);
		}		
	} else
	if(p_button->GetID() == CLOSE_ID)
	{
		gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x-5, y+p_button->y-5, C_GLOBAL_RESOURCE::BUTTON_CLOSE_BACK);
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+3);
			else
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
		}
		else
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
			else
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
		}

	}
	else
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y,  p_button->m_image_index+2);
			else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y,  p_button->m_image_index+1);
		}
		else
		{
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
		}
	}
}

void	C_VS_UI_NAMING::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
	const static char* m_other_info_button_string[8]=
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_OFF].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_ON].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CHANGE_PLAYER_NICKNAME].GetString(),// = "Ŀ���� �г����� �����մϴ�.";
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SELECT_PLAYER_NICKNAME].GetString(),// = "���õ� �г������� �����մϴ�.";
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP].GetString(),// = "������ ���ϴ�..";
	};
	switch(p_button->GetID())
	{
	case CLOSE_ID :
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[0],0,0);
		break;
	case SELECT_ID:
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[6],0,0);
		break;
	case HELP_ID:
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[7],0,0);
		break;
	case ALPHA_ID:
		if(GetAttributes()->alpha)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[2],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[1],0,0);
		break;
	}	
	
}


void	C_VS_UI_NAMING::Run(id_t id)
{
	switch (id)
	{
	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_NAMING);
		break;
	case SELECT_ID:
		if(m_SelectPos > -1 && m_SelectPos < m_NameList.size())
		{
			C_VS_UI_NicknameInfo* TempInfo = (C_VS_UI_NicknameInfo*)m_NameList[m_SelectPos];
			if(TempInfo != NULL)
				gpC_base->SendMessage(UI_SELECT_NAMING, (int)TempInfo->getNicknameID());
		}
		break;
	case ALPHA_ID:
		AttrAlpha(!GetAttributes()->alpha);
	
		break;
		
	case HELP_ID:
		gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::NICK_NAME_WINDOW);
		m_pC_button_group->UnacquireMouseFocus();
		break;
		
	}
}

void	C_VS_UI_NAMING::Process()
{
	m_pC_button_group->Process();

}

void	C_VS_UI_NAMING::ToggleWindow()
{
//	if(GetAttributes()->autohide) Run(PUSHPIN_ID);	
}
void	C_VS_UI_NAMING::SetNameList(std::vector<C_VS_UI_NicknameInfo*> &_NamingList)
{
	m_NameList = _NamingList;
//	if(m_NameList.size())
//	{
//		std::vector<C_VS_UI_NicknameInfo*>::iterator itr = m_NameList.begin();
//		while(itr != m_NameList.end())
//		{
//			C_VS_UI_NicknameInfo* TempInfo = (*itr);
//			if(TempInfo->getNicknameType() == C_VS_UI_NicknameInfo::NICK_CUSTOM)
//			{
//				m_CustomIndex = TempInfo->getNicknameID();
//				m_szCustomName = TempInfo->getNickname();
//				m_AddName.AddString(m_szCustomName.c_str());
//				break;
//			}
//			itr++;
//		}
//	}
	m_pC_scroll_bar->SetPosMax(m_NameList.size());
}

void	C_VS_UI_NAMING::AddNameList(C_VS_UI_NicknameInfo* Info) 
{
	m_NameList.push_back(Info);
	m_pC_scroll_bar->SetPosMax(m_NameList.size());
}
void	C_VS_UI_NAMING::DeleteNameList(int id)
{
	C_VS_UI_NicknameInfo* _st_NAMES = NULL;
	std::vector<C_VS_UI_NicknameInfo*>::iterator itr = m_NameList.begin();
	while(itr != m_NameList.end())
	{
		_st_NAMES = (*itr);
		if(_st_NAMES->getNicknameID() == id)
		{
			m_NameList.erase(itr);
			break;
		}
		itr++;
	}
	DeleteNew(_st_NAMES);
	m_pC_scroll_bar->SetPosMax(m_NameList.size());
}
void	C_VS_UI_NAMING::Change_Custom_Naming(int nID, char* szName)
{
	if(m_NameList.size())
	{
		std::vector<C_VS_UI_NicknameInfo*>::iterator itr = m_NameList.begin();
		while(itr != m_NameList.end())
		{
			C_VS_UI_NicknameInfo * TempInfo = (*itr);
			if(TempInfo->getNicknameID() == nID)
			{
				TempInfo->setNickname(szName);
				break;
			}
			itr ++;
		}
	}
}


//------------------------------------------------------------------------------
// C_VS_UI_NAMING_CHANGE
//
// ���̹� ����Ʈ ü����
//------------------------------------------------------------------------------
C_VS_UI_NAMING_CHANGE::C_VS_UI_NAMING_CHANGE(MItem* pItem, char* szCustom)
{	

	g_RegisterWindow(this);	
	AttrKeyboardControl(true);
	
	int window_w = 186;
	int window_h = 160;
	
	m_PenItem = pItem;
	if(NULL != szCustom)
		m_szCustomString = szCustom;
	
	Set(g_GameRect.right/2-window_w/2, g_GameRect.bottom/2 - window_h/2, window_w, window_h);

	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW);
	if(rect.w != -1)
	{ 
		x = rect.x;
		y = rect.y;
	}
	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		m_pC_spk.Open(SPK_SLAYER_NAMING);
		break;

	case RACE_VAMPIRE:
		m_pC_spk.Open(SPK_VAMPIRE_NAMING);
		break;

	case RACE_OUSTERS:
		m_pC_spk.Open(SPK_OUSTERS_NAMING);
		break;		
	}
	
	m_ChangeNick.SetPosition(x+24, y+86);
	m_ChangeNick.SetPrintInfo(gpC_base->m_chatting_pi);
	m_ChangeNick.SetByteLimit(22);
	m_ChangeNick.SetInputStringColor(RGB_WHITE);
	m_ChangeNick.SetCursorColor(RGB_WHITE);
	m_ChangeNick.SetDigitOnlyMode(false);
	m_ChangeNick.SetAbsWidth(140);
	Attach(&m_ChangeNick);

	int close_x = w-24, close_y = h-22;		
	int select_x = w-120, select_y = h-45;		

	//�����ư
	m_pC_button_group = new ButtonGroup(this);
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_x, close_y, 
		gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::BUTTON_CLOSE), 
		gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::BUTTON_CLOSE), 
		CLOSE_ID, this, C_GLOBAL_RESOURCE::BUTTON_CLOSE));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(select_x, select_y, 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CHANGE), 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CHANGE),
		CHANGE_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_CHANGE));

	AttrTopmost(true);
	AttrPin(true);	
	
}

C_VS_UI_NAMING_CHANGE::~C_VS_UI_NAMING_CHANGE()
{
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW, Rect(x, y, w, h));	

	g_UnregisterWindow(this);
	
	m_ChangeNick.Unacquire();
	gC_vs_ui.AcquireChatting();

	DeleteNew(m_pC_button_group);	
//	DeleteNew(m_pC_scroll_bar);
}

void	C_VS_UI_NAMING_CHANGE::Start()
{
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW);
	if(rect.w != -1)
	{ 
		x = rect.x;
		y = rect.y;
	}
	PI_Processor::Start();
	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();
	m_ChangeNick.Acquire();
	AttrTopmost(true);	

}

void	C_VS_UI_NAMING_CHANGE::Finish()
{
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW, Rect(x, y, w, h));	

	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);	
}


void	C_VS_UI_NAMING_CHANGE::Show()
{
	const int tab_x = 0;
	const int tab_y = 60;
	m_ChangeNick.SetPosition(x+24, y+95);

	gpC_global_resource->DrawDialog(x, y, w, h, GetAttributes()->alpha);

	RECT rect2 = { x+20, y+90, x+162, y+112 };
	Rect rt2(rect2.left, rect2.top, rect2.right - rect2.left, rect2.bottom - rect2.top );

	gpC_base->m_p_DDSurface_back->FillRect(&rect2, 0);
	gpC_global_resource->DrawOutBox(rt2);


	if ( gpC_base->m_p_DDSurface_back->Lock() )
	{
		m_pC_button_group->Show();
		
		if(m_PenItem != NULL)
		{
			if(m_PenItem->GetItemType() == 22 || m_PenItem->GetItemType() == 24 || m_PenItem->GetItemType() == 25)
				m_pC_spk.BltLocked( rect2.left+50, rect2.top-54, NICKNAME_STR);
			else if(m_PenItem->GetItemType() == 23)
				m_pC_spk.BltLocked( rect2.left+50, rect2.top-54, PETNAME_STR);
		}
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(rect2.left, rect2.top-67, C_GLOBAL_RESOURCE::AB_TITLE_DECORATION_LEFT);
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(rect2.right-38, rect2.top-67, C_GLOBAL_RESOURCE::AB_TITLE_DECORATION_RIGHT);
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	m_ChangeNick.Show();

	if(g_FL2_GetDC())
	{
		g_PrintColorStr(x+24, y+70 , m_szCustomString.c_str(), gpC_base->m_user_id_pi, RGB_GRAY);
		m_pC_button_group->ShowDescription();
		g_FL2_ReleaseDC();
	}	


//_bl_focus = false;
}


bool	C_VS_UI_NAMING_CHANGE::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);

	switch(message)
	{
	case M_MOVING:
		{
		}
		break;

	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		{
			if(_x>20 && _x<162 && _y>60&& _y <82)
			{
				m_ChangeNick.Acquire();
			}
			else if(NULL == m_pC_button_group->IsInRect(_x, _y))
			{
				MoveReady();
				SetOrigin(_x, _y);
			}
			break;
		}
		break;
	}	
	return true;
}

void	C_VS_UI_NAMING_CHANGE::KeyboardControl(UINT message, UINT key, long extra)	
{	
	Window::KeyboardControl(message, key, extra);
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{		
		case VK_ESCAPE :
			gpC_base->SendMessage(UI_CLOSE_NAMING_CHANGE);
			break;		
		}
	}
}

bool	C_VS_UI_NAMING_CHANGE::IsPixel(int _x,int _y)
{
	return IsInRect(_x, _y);
}

void	C_VS_UI_NAMING_CHANGE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetID() == CLOSE_ID)
	{
		gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x-5, y+p_button->y-5, C_GLOBAL_RESOURCE::BUTTON_CLOSE_BACK);
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+3);
			else
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
		}
		else
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
			else
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
		}

	}
	else
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y,  p_button->m_image_index+2);
			else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y,  p_button->m_image_index+1);
		}
		else
		{
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
		}
	}
}

void	C_VS_UI_NAMING_CHANGE::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
	const static char* help_info_button_string[4]=
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CHANGE_PLAYER_NICKNAME].GetString(),// = "���� ���� �г����� �����մϴ�.";.";
		(*g_pGameStringTable)[UI_STRING_MESSAGE_ADD_PLAYER_NICKNAME].GetString(),// = "�г����� �߰� �մϴ�".";
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CHANGE_PET_NICKNAME].GetString(),// = "���� �г����� �����մϴ�.".";
		

	};
	switch(p_button->GetID())
	{
	case CLOSE_ID :
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)help_info_button_string[0],0,0);
		break;
	case CHANGE_ID:
		if(m_PenItem)
		{
			if(m_PenItem->GetItemType() == 22 || m_PenItem->GetItemType() == 25) // �������̹� ���� ��
				g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)help_info_button_string[1],0,0);
			else if(m_PenItem->GetItemType() == 23)// �� ���̹� ���� ��
				g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)help_info_button_string[3],0,0);
			else if(m_PenItem->GetItemType() == 24)
				g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)help_info_button_string[2],0,0);
		}
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)help_info_button_string[2],0,0);
		break;
	}	
}


void	C_VS_UI_NAMING_CHANGE::Run(id_t id)
{
	switch (id)
	{
	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_NAMING_CHANGE);
		break;
	case CHANGE_ID:
		if(m_ChangeNick.Size()&& m_PenItem != NULL)
		{
			char* szName;
			g_Convert_DBCS_Ascii2SingleByte(m_ChangeNick.GetStringWide(), m_ChangeNick.Size(), szName);
			if(szName != NULL)
			{
				m_szEditName = szName;
				gpC_base->SendMessage(UI_CHANGE_CUSTOM_NAMING, (int)(intptr_t)m_szEditName.c_str(), (int)m_PenItem->GetID(), (void*)m_PenItem);
			//	gpC_base->SendMessage(UI_CLOSE_NAMING_CHANGE);
			}
			DeleteNew(szName);
		}
		break;
	}
}

void	C_VS_UI_NAMING_CHANGE::Process()
{
	m_pC_button_group->Process();
	ProcessHide();
}


C_VS_UI_QUEST_NPC_DIALOG::C_VS_UI_QUEST_NPC_DIALOG()
{
	m_pC_dialog = NULL;
}

C_VS_UI_QUEST_NPC_DIALOG::~C_VS_UI_QUEST_NPC_DIALOG()
{
	DeleteNew(m_pC_dialog);
}

void
C_VS_UI_QUEST_NPC_DIALOG::SetQuestNpcDialog(C_VS_UI_NPC_DIALOG* TempDialog)
{
	if(NULL == TempDialog)
		return;
	DeleteNew(m_pC_dialog);

	m_pC_dialog = TempDialog;
	m_pC_dialog->Start();
	return;
}

//-----------------------------------------------------------------------------
// C_VS_UI_QUEST_MANAGER::C_VS_UI_QUEST_MANAGER
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_QUEST_MANAGER::C_VS_UI_QUEST_MANAGER()
{	
	m_Ui_Quest_List		= NULL;
	m_Ui_Quest_Detail	= NULL;
	m_Ui_Quest_Mission	= NULL;
	m_Ui_Quest_Item		= NULL;
	m_Ui_Quest_Icon		= NULL;

	m_pImage_Spk_MailBox = new C_SPRITE_PACK; 
	m_pImage_Spk_MailBox->Open(SPK_QUEST_MODIFY);
	
	m_pImage_Spk		= new C_SPRITE_PACK;
	m_SelectedQuestID	= 0xffffffff;
	
	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		m_pImage_Spk->Open(SPK_SLAYER_QUESTMAMAGER);
		break;

	case RACE_VAMPIRE:
		m_pImage_Spk->Open(SPK_VAMPIRE_QUESTMAMAGER);
		break;

	case RACE_OUSTERS:
		m_pImage_Spk->Open(SPK_OUSTERS_QUESTMAMAGER);
		break;		
	}
	

//	GetMissionTitle(1, 1,3);
//	SetQuestManagerInfo("1", false);
//	RunAllWinow();
	if(LoadQuestXML() == false) 
		return;
#ifndef _LIB
	
	

//	struct _GMissionInfo{
//		std::string		szMissionTitle;
//		BYTE			bStatus;
//		BYTE			bCondition;	// ��� ���ǿ� �ִ°� 0 : Happen, 1 : Complete, 2 : Fail, 3 : Reward
//		WORD			bIndex;		// �ش� ������ ���° element�ΰ�
//	};

		
	//RunAllWinow();
	_GQuestInfo *Temp1 = new _GQuestInfo;
	Temp1->bStatus = C_VS_UI_QUEST_LIST::CAN_ACCEPT;
	Temp1->dwQuestID = 1001;
	_GMissionInfo *Temp1_sub1 = new _GMissionInfo;
	Temp1_sub1->bStatus = C_VS_UI_QUEST_MISSION::CURRENT;
	Temp1_sub1->bCondition = 2;
	Temp1_sub1->bIndex = 1;
	Temp1_sub1->m_NumArg = 2;
	Temp1_sub1->dwTimeLimit = 0;
	Temp1->vMissionList.push_back(Temp1_sub1);
	SetQuestManagerInfo((void*)Temp1);

	_GQuestInfo *Temp2 = new _GQuestInfo;
	Temp2->bStatus = C_VS_UI_QUEST_LIST::CAN_ACCEPT;
	Temp2->dwQuestID = 102;
	_GMissionInfo *Temp2_sub1 = new _GMissionInfo;
	Temp2_sub1->bStatus = C_VS_UI_QUEST_MISSION::CURRENT;
	Temp2_sub1->bCondition = 2;
	Temp2_sub1->bIndex = 1;
	Temp2->vMissionList.push_back(Temp2_sub1);
	SetQuestManagerInfo((void*)Temp2);

	_GQuestInfo *Temp3 = new _GQuestInfo;
	Temp3->bStatus = C_VS_UI_QUEST_LIST::DOING;
	Temp3->dwQuestID = 102;
	_GMissionInfo *Temp3_sub1 = new _GMissionInfo;
	Temp3_sub1->bStatus = C_VS_UI_QUEST_MISSION::CURRENT;
	Temp3_sub1->bCondition = 1;
	Temp3_sub1->bIndex = 1;
	Temp3->vMissionList.push_back(Temp3_sub1);
	_GMissionInfo *Temp3_sub2 = new _GMissionInfo;
	Temp3_sub2->bStatus = C_VS_UI_QUEST_MISSION::CURRENT;
	Temp3_sub2->bCondition = 1;
	Temp3_sub2->bIndex = 1;
	Temp3_sub2->m_NumArg = 2;
	Temp3->vMissionList.push_back(Temp3_sub2);
	ModifyQuestManagerInfo((void*)Temp3);

	_GQuestInfo *Temp4 = new _GQuestInfo;
	Temp4->bStatus = C_VS_UI_QUEST_LIST::FAIL;
	Temp4->dwQuestID = 103;
	_GMissionInfo *Temp4_sub1 = new _GMissionInfo;
	Temp4_sub1->bStatus = C_VS_UI_QUEST_MISSION::CURRENT;
	Temp4_sub1->bCondition = 2;
	Temp4_sub1->bIndex = 1;
	Temp4->vMissionList.push_back(Temp4_sub1);
	SetQuestManagerInfo((void*)Temp4);

	_GQuestInfo *Temp5 = new _GQuestInfo;
	Temp5->bStatus = C_VS_UI_QUEST_LIST::CAN_REPLAY;
	Temp5->dwQuestID = 104;
	_GMissionInfo *Temp5_sub1 = new _GMissionInfo;
	Temp5_sub1->bStatus = C_VS_UI_QUEST_MISSION::CURRENT;
	Temp5_sub1->bCondition = 2;
	Temp5_sub1->bIndex = 1;
	Temp5->vMissionList.push_back(Temp5_sub1);
	SetQuestManagerInfo((void*)Temp5);


	MItem*		pItem;

	std::vector<MItem*> TempItemList;
	for(int TempCount = 0; TempCount< 13 ; TempCount++)
	{
		//----------------------------------------------------
		// Item ���� --> �߰�
		//----------------------------------------------------
		pItem = MItem::NewItem( ITEM_CLASS_GQUEST_ITEM );
	//	pItem->SetID( 0 );
		pItem->SetItemType( TempCount );
	//	pItem->SetItemOptionList( 0 );
		pItem->SetCurrentDurability( 1 );	
		TempItemList.push_back(pItem);
	}

	ReleaseQuestXML();
	UpdateQuestItemInfo(TempItemList);
	PushGQuestExcuteElement(1010,1,2);
	PushGQuestExcuteElement(1010,1,3);

	RunQuestIcon();

#endif 
}

C_VS_UI_QUEST_MANAGER::~C_VS_UI_QUEST_MANAGER()
{
	CloseAllWinow();
	CloseQuestIcon();
	ClearQuestInfo();
	ClearQuestItem();
	ClearExcuteElement();
	ReleaseQuestXML();
	DeleteNew(m_pImage_Spk);
	DeleteNew(m_pImage_Spk_MailBox);
	
}

// �ʿ��Ҷ����� �ε� �ؼ� ����...�ѹ� �ε��س��� ����..������..
// �ϴ� ÷�� �ε��س��� ��~�� ����� �ڵ�^^;
// ���� �غ��� �׶� �׶� �ε��ϴ°� ������ �ؼ� ����..����
// ó�� ����Ʈ ���� �� �⺻ tree�� ��� ������ �ְ� �߰��߰� modify�Ҷ� �׶� �׶� �о� �´�.
bool	C_VS_UI_QUEST_MANAGER::LoadQuestXML()
{
	m_Quest_XML_file.SetRAR(RPK_TUTORIAL_ETC, RPK_PASSWORD);
	
	m_Quest_XML_file.Open(QUEST_XML_FILE);

	XMLParser	parser;

	// quest xml�� vector�� ����� �ؼ���..-_-;
	parser.parse( (char *)m_Quest_XML_file.GetFilePointer(), &m_Quest_XML_Tree, true);

	m_Quest_XML_file.Release();
	

	m_Quest_XML_file.Open(QUEST_EVENT_XML_FILE);

	parser.parse( (char *)m_Quest_XML_file.GetFilePointer(), &m_Quest_XML_Tree, true);
	
	m_Quest_XML_file.Release();

	return TRUE;
}
void	C_VS_UI_QUEST_MANAGER::ReleaseQuestXML()
{
	m_Quest_XML_Tree.Release();
}
// ����Ʈ ����/���� ��Ŷ ����
void	C_VS_UI_QUEST_MANAGER::SendQuestIntention(bool bIntention)
{
	if(bIntention)
	{
		gpC_base->SendMessage(UI_GQUEST_ACCEPT, m_SelectedQuestID);
	}
	else
	{
		gpC_base->SendMessage(UI_GQUEST_GIVEUP, m_SelectedQuestID);
	}
}
// detail, mission ������ �����Ѵ�.
void	C_VS_UI_QUEST_MANAGER::SetSubWindowInfo(DWORD qID, bool bOpenSubWindows)
{
	if(bOpenSubWindows)
	{
		RunQuestDetail();
		RunQuestMission();
		if(m_QuestItemInfo.size()>0)
			RunQuestItem();
	}
	//if(m_SelectedQuestID != qID)
	{
		m_SelectedQuestID = qID;
		std::vector<_GQuestInfo*>::iterator itr = m_QuestListInfo.begin();
		while(itr != m_QuestListInfo.end())
		{
			_GQuestInfo* TempInfo = (*itr);
			if(TempInfo->dwQuestID == m_SelectedQuestID)
			{
				if(IsRunningQuestDetail())
				{
					m_Ui_Quest_Detail->SetQuestDetailInfo((char*)TempInfo->szQuestTitle.c_str(), (char*)TempInfo->szQuestDescription.c_str(), TempInfo->bStatus);
				}
				if(IsRunningQuestMission())
				{
					m_Ui_Quest_Mission->SetQuestMissionInfo(TempInfo->vMissionList);
				}
				return ;
			}
			itr ++;
		}
	}
}
// Item ������ �����Ѵ�.
void	C_VS_UI_QUEST_MANAGER::UpdateQuestItemInfo(std::vector<MItem*>& vItem)
{
	ClearQuestItem();
	m_QuestItemInfo = vItem;
	if(IsRunningQuestItem())
	{
		m_Ui_Quest_Item->SetQuestItemInfo(m_QuestItemInfo);
	}
}

void	C_VS_UI_QUEST_MANAGER::DeleteQuestItem(int nSlot)
{
	std::vector<MItem*>::iterator itr = m_QuestItemInfo.begin();
	for(int i = 0; i< m_QuestItemInfo.size() ; i++)
	{
		if(i == nSlot)
		{
			MItem * TempInfo = (*itr);
			itr = m_QuestItemInfo.erase(itr);
			DeleteNew(TempInfo);
			if(IsRunningQuestItem())
			{
				m_Ui_Quest_Item->SetQuestItemInfo(m_QuestItemInfo);
			}
			return;
		}
		itr ++;
	}
	
}
// ����Ʈ ����Ʈ�� ������ �߰�(ó�� �ε��� ����´�)
bool	C_VS_UI_QUEST_MANAGER::SetQuestManagerInfo(void* pVoid)
{
	_GQuestInfo * TempInfo = (_GQuestInfo*)pVoid;
	if(NULL != TempInfo)
	{
		// string update �� �߰� - ����Ʈ ����Ʈ�� id�� ã�´�..����÷���.-_-;;;
		const XMLTree *pElement = m_Quest_XML_Tree.GetChildByAttr( TempInfo->dwQuestID , "id");
		if(NULL != pElement)
		{
			if( TempInfo->bStatus == C_VS_UI_QUEST_LIST::CAN_ACCEPT ||
				TempInfo->bStatus == C_VS_UI_QUEST_LIST::CAN_REPLAY ) // ���� ���� �̸�
			{
				RunQuestIcon();

				// ���డ���̰� �ڵ� ���� ���� �϶�
				const XMLAttribute* _TempAttr = pElement->GetAttribute("autostart");
				if(_TempAttr != NULL)
				{
					if(_TempAttr->ToInt()>0) // �ڵ� �����̸�
					{
						m_SelectedQuestID = TempInfo->dwQuestID;
						gC_vs_ui.SendQuestIntention(true); // �����ϰڴ� ��Ŷ�� ������.
					}
				}
			}
			TempInfo->szQuestTitle = GetQuestTitle(pElement);
			TempInfo->szQuestDescription = GetQuestDescription(pElement);
			std::vector<_GMissionInfo*>::iterator itr = TempInfo->vMissionList.begin();
			while(itr != TempInfo->vMissionList.end())
			{
				_GMissionInfo* TempMission = (*itr);

				const XMLTree * TempMissionElement = GetMissionElement(pElement,TempMission->bCondition);
				if( NULL != TempMissionElement)
				{
					// �̼� ����Ʈ�� "index"�� ã�´�..����÷���.-_-;;;
					const XMLTree *pChildElement2 = TempMissionElement->GetChildByAttr(TempMission->bIndex, "index");
					if(NULL != pChildElement2)
					{
						TempMission->szMissionTitle =  (char*)pChildElement2->GetText().c_str();
						if(0 == stricmp(pChildElement2->GetName().c_str(),"Time")) // �ð� ������ �ִ� �̼��̸�
						{
							TempMission->dwTimeLimit = timeGetTime();
						}

					}
				}
				//TempMission->szMissionTitle = GetMissionTitle(pElement, TempMission->bIndex, TempMission->bCondition);
				
				// modify start- condition�� fail(2)�϶� �����̸� ���з�, ���и� �������� �����޶�� �ؼ� ����..��..
				if(TempMission->bCondition == 2)
				{
					if(TempMission->bStatus == 2)
						TempMission->bStatus = 3;
					else if(TempMission->bStatus == 3)
						TempMission->bStatus = 2;
				}
				// modify end
				itr++;
			}
			m_QuestListInfo.push_back(TempInfo);

			if(IsRunningQuestList())
			{
				m_Ui_Quest_List->SetQuestListInfo((void*)&m_QuestListInfo);
			}
		}
		return true;
	}
	return false;
}
// ������ �ִ� ����Ʈ�� ����, ������ �߰�(���� �߰��� ����´�)
bool	C_VS_UI_QUEST_MANAGER::ModifyQuestManagerInfo(void* pVoid, int nType)
{
	if(LoadQuestXML() == false)
		return false;
	RunQuestIcon();
	_GQuestInfo * TempInfo = (_GQuestInfo*)pVoid;
	if(NULL != TempInfo)
	{
		if( true == UpdateQuestInfo(TempInfo, nType))
		{
			if(IsRunningQuestList())
			{
				m_Ui_Quest_List->SetQuestListInfo((void*)&m_QuestListInfo);
			}

		
		}
		else
		{
			if( false == SetQuestManagerInfo(pVoid))
			{
				ReleaseQuestXML();
				return false;
			}
		}

		SetSubWindowInfo(TempInfo->dwQuestID , false); // ���� �����츦 ���� ������ ���� â�� ���� �ʴ´�.
		ReleaseQuestXML();
		return true;
	}
	return false;
}
// ������ qid�� ������ ���� �����͸� QInfo�� �ٲ۴�.
bool	C_VS_UI_QUEST_MANAGER::UpdateQuestInfo(_GQuestInfo *QInfo, int nType)
{
	if(NULL != QInfo)
	{
		std::vector<_GQuestInfo*>::iterator itr = m_QuestListInfo.begin();
		while(itr != m_QuestListInfo.end())
		{
			_GQuestInfo* TempInfo = (*itr);
			if(TempInfo->dwQuestID == QInfo->dwQuestID)
			{
				// ����Ʈ ����Ʈ�� id�� ã�´�..����÷���.-_-;;;
				const XMLTree *pElement = m_Quest_XML_Tree.GetChildByAttr( TempInfo->dwQuestID , "id");
				if(pElement != NULL)
				{
					if( TempInfo->bStatus == C_VS_UI_QUEST_LIST::CAN_ACCEPT ||
						TempInfo->bStatus == C_VS_UI_QUEST_LIST::CAN_REPLAY ) // ���� ���� �̸�
					{
						// ���డ���̰� �ڵ� ���� ���� �϶�
						const XMLAttribute* _TempAttr = pElement->GetAttribute("autostart");
						if(_TempAttr != NULL)
						{
							if(_TempAttr->ToInt()>0) // �ڵ� �����̸�
							{
								m_SelectedQuestID = TempInfo->dwQuestID;
								gC_vs_ui.SendQuestIntention(true); // �����ϰڴ� ��Ŷ�� ������.
							}
						}
					}
					QInfo->szQuestTitle = TempInfo->szQuestTitle;/*GetQuestTitle(pElement);*/
					QInfo->szQuestDescription = TempInfo->szQuestDescription;/*GetQuestDescription(pElement);*/
					std::vector<_GMissionInfo*>::iterator itr2 = QInfo->vMissionList.begin();
					while(itr2 != QInfo->vMissionList.end())
					{
						_GMissionInfo* TempMission = (*itr2);

						const XMLTree * TempMissionElement = GetMissionElement(pElement,TempMission->bCondition);
						if( NULL != TempMissionElement)
						{
							// �̼� ����Ʈ�� "index"�� ã�´�..����÷���.-_-;;;
							const XMLTree *pChildElement2 = TempMissionElement->GetChildByAttr(TempMission->bIndex, "index");
							if(NULL != pChildElement2)
							{
								TempMission->szMissionTitle =  (char*)pChildElement2->GetText().c_str();
								if(0 == stricmp(pChildElement2->GetName().c_str(),"Time")) // �ð� ������ �ִ� �̼��̸�
								{
									TempMission->dwTimeLimit = timeGetTime();
								}

							}
						}
						//TempMission->szMissionTitle = GetMissionTitle(pElement, TempMission->bIndex, TempMission->bCondition);

						// modify start- condition�� fail(2)�϶� �����̸� ���з�, ���и� �������� �����޶�� �ؼ� ����..��..
						if(TempMission->bCondition == 2)
						{
							if(TempMission->bStatus == 2)
								TempMission->bStatus = 3;
							else if(TempMission->bStatus == 3)
								TempMission->bStatus = 2;
						}
						// modify end
						itr2++;
					}
						
					(*itr) = QInfo;

					if(nType == 0||nType == 1) // 0: ���� or 1:���� �϶� - ������ �߰�..-_-;
					{
						RunQuestFail_or_Sucess(pElement, nType);
					}
					DeleteNew(TempInfo);
					return true;
				}
			}
			itr ++;
		}
	}
	return false;
}
// xml���� ����Ʈ�� title�� �����´�
char*	C_VS_UI_QUEST_MANAGER::GetQuestTitle(const XMLTree *pElement)
{
	char* TempTitle = "";
//	const XMLTree *pElement = m_Quest_XML_Tree.GetChild( qID );
	if(NULL != pElement)
	{
		const XMLTree *pChildElement = pElement->GetChild( "Title" );
		if(NULL != pChildElement)
			return (char*)pChildElement->GetText().c_str();
	}
	return TempTitle;
}
// xml���� ����Ʈ�� description�� �����´�
char*	C_VS_UI_QUEST_MANAGER::GetQuestDescription(const XMLTree *pElement)
{
	char* TempTitle = "";
//	const XMLTree *pElement = m_Quest_XML_Tree.GetChild( qID );
	if(NULL != pElement)
	{
		const XMLTree *pChildElement = pElement->GetChild( "Script" );
		if(NULL != pChildElement)
			return (char*)pChildElement->GetText().c_str();
	}
	return TempTitle;
}
// xml���� ����Ʈ ���� �̼� Ÿ��Ʋ�� �����´�.
char*	C_VS_UI_QUEST_MANAGER::GetMissionTitle(const XMLTree *pElement, WORD Index, BYTE Condition)
{
	char* TempTitle = "";
//	const XMLTree *pElement = m_Quest_XML_Tree.GetChild( qID );
	if(NULL != pElement)
	{
		const static char * szConditonType[4] = {	"Happen",
													"Complete",
													"Fail",
													"Reward",
												};
						
		const XMLTree *pChildElement = pElement->GetChild( szConditonType[ min(3, Condition)] );
		if(NULL != pChildElement)
		{
			// �̼� ����Ʈ�� "index"�� ã�´�..����÷���.-_-;;;
			const XMLTree *pChildElement2 = pChildElement->GetChildByAttr(Index, "index");
			if(NULL != pChildElement2)
				return (char*)pChildElement2->GetText().c_str();
		}
	}
	return TempTitle;
}
// xml���� ����Ʈ ���� �̼� Ÿ��Ʋ�� �����´�.
const XMLTree *	C_VS_UI_QUEST_MANAGER::GetMissionElement(const XMLTree *pElement, BYTE Condition)
{
	if(NULL != pElement)
	{
		const static char * szConditonType[4] = {	"Happen",
													"Complete",
													"Fail",
													"Reward",
												};
						
		const XMLTree *pChildElement = pElement->GetChild( szConditonType[ min(3, Condition)] );
		if(NULL != pChildElement)
		{
			return pChildElement;
		}
	}
	return NULL;
}
// ����Ʈ ����Ʈ�� ����
void	C_VS_UI_QUEST_MANAGER::ClearQuestInfo()
{
	std::vector<_GQuestInfo*>::iterator itr = m_QuestListInfo.begin();
	while(itr != m_QuestListInfo.end())
	{
		_GQuestInfo * TempInfo = (*itr);
		itr = m_QuestListInfo.erase(itr);
		DeleteNew(TempInfo);
	}
	m_QuestListInfo.clear();
}
// ����Ʈ �������� ����
void	C_VS_UI_QUEST_MANAGER::ClearQuestItem()
{
	std::vector<MItem*>::iterator itr = m_QuestItemInfo.begin();
	while(itr != m_QuestItemInfo.end())
	{
		MItem * TempInfo = (*itr);
		itr = m_QuestItemInfo.erase(itr);
		DeleteNew(TempInfo);
	}
	m_QuestItemInfo.clear();
}

void	C_VS_UI_QUEST_MANAGER::ClearExcuteElement()
{
	std::vector<_GQuestExcuteElement*>::iterator itr = m_QuestExcutElement.begin();
	while(itr != m_QuestExcutElement.end())
	{
		_GQuestExcuteElement * TempInfo = (*itr);
		itr = m_QuestExcutElement.erase(itr);
		DeleteNew(TempInfo);
	}
	m_QuestExcutElement.clear();
}
// sub �����찡 ���ִ��� �˻�
bool	C_VS_UI_QUEST_MANAGER::IsAnyWindowOpened() const
{
	if( IsRunningQuestList()
	 || IsRunningQuestDetail()
	 || IsRunningQuestMission()
	 || IsRunningQuestItem())
		return true;

	return false;
}
// ���� ���� ��ü�� �ݾҴ� ������ �Ѵ�.
void	C_VS_UI_QUEST_MANAGER::ToggleWindow() 
{
	if(IsAnyWindowOpened()) // �� ����Ʈ ������ �������� �ݴ´�
		CloseAllWinow();
	else // ������ �ϳ��� ������ �� �����ش� ����
	{
		//RunAllWinow();
		RunQuestList();
		if(m_QuestItemInfo.size()>0)
			RunQuestItem();
		if(IsRunningQuestList())
		{
			m_Ui_Quest_List->SetQuestListInfo((void*)&m_QuestListInfo);
		}
		if(IsRunningQuestItem())
		{
			m_Ui_Quest_Item->SetQuestItemInfo(m_QuestItemInfo);
		}
	}
}

void	C_VS_UI_QUEST_MANAGER::RunAllWinow()
{
	RunQuestList();
	RunQuestDetail();
	RunQuestMission();
	RunQuestItem();
	if(IsRunningQuestList())
	{
		m_Ui_Quest_List->SetQuestListInfo((void*)&m_QuestListInfo);
	}
	if(IsRunningQuestItem())
	{
		m_Ui_Quest_Item->SetQuestItemInfo(m_QuestItemInfo);
	}
}
void	C_VS_UI_QUEST_MANAGER::CloseAllWinow()
{
	CloseQuestList();
	CloseQuestDetail();
	CloseQuestMission();
	CloseQuestItem();
	
}
void	C_VS_UI_QUEST_MANAGER::RunQuestList()
{
	if (m_Ui_Quest_List == NULL)
	{
		m_Ui_Quest_List = new C_VS_UI_QUEST_LIST(m_pImage_Spk);

		assert(m_Ui_Quest_List!= NULL);

		m_Ui_Quest_List->Start();
	}
}
bool	C_VS_UI_QUEST_MANAGER::IsRunningQuestList() const
{
	return (NULL != m_Ui_Quest_List);
}
void	C_VS_UI_QUEST_MANAGER::CloseQuestList()
{
	if (NULL ==m_Ui_Quest_List)
		return;
	
	DeleteNew(m_Ui_Quest_List);
//	if(IsAllWindowClosed())
//		gpC_base->SendMessage(UI_CLOSE_QUEST_MANAGER);
}

void	C_VS_UI_QUEST_MANAGER::RunQuestDetail()
{
	if (m_Ui_Quest_Detail == NULL)
	{
		m_Ui_Quest_Detail = new C_VS_UI_QUEST_DETAIL(m_pImage_Spk);

		assert(m_Ui_Quest_Detail!= NULL);

		m_Ui_Quest_Detail->Start();
	}
}
bool	C_VS_UI_QUEST_MANAGER::IsRunningQuestDetail() const
{
	return (NULL != m_Ui_Quest_Detail);
}
void	C_VS_UI_QUEST_MANAGER::CloseQuestDetail()
{
	if (NULL ==m_Ui_Quest_Detail)
		return;
	
	DeleteNew(m_Ui_Quest_Detail);

//	if(IsAllWindowClosed())
//		gpC_base->SendMessage(UI_CLOSE_QUEST_MANAGER);
}


void	C_VS_UI_QUEST_MANAGER::RunQuestMission()
{
	if (m_Ui_Quest_Mission == NULL)
	{
		m_Ui_Quest_Mission = new C_VS_UI_QUEST_MISSION(m_pImage_Spk);

		assert(m_Ui_Quest_Mission!= NULL);

		m_Ui_Quest_Mission->Start();
	}
}
bool	C_VS_UI_QUEST_MANAGER::IsRunningQuestMission() const
{
	return (NULL != m_Ui_Quest_Mission);
}
void	C_VS_UI_QUEST_MANAGER::CloseQuestMission()
{
	if (NULL ==m_Ui_Quest_Mission)
		return;
	
	DeleteNew(m_Ui_Quest_Mission);
//	if(IsAllWindowClosed())
//		gpC_base->SendMessage(UI_CLOSE_QUEST_MANAGER);
}

void	C_VS_UI_QUEST_MANAGER::RunQuestItem()
{
	if (m_Ui_Quest_Item == NULL)
	{
		m_Ui_Quest_Item = new C_VS_UI_QUEST_ITEM(m_pImage_Spk);

		assert(m_Ui_Quest_Item!= NULL);

		m_Ui_Quest_Item->Start(m_QuestItemInfo);
	}
}
bool	C_VS_UI_QUEST_MANAGER::IsRunningQuestItem() const
{
	return (NULL != m_Ui_Quest_Item);
}
void	C_VS_UI_QUEST_MANAGER::CloseQuestItem()
{
	if (NULL ==m_Ui_Quest_Item)
		return;
	
	DeleteNew(m_Ui_Quest_Item);
	
//	if(IsAllWindowClosed())
//		gpC_base->SendMessage(UI_CLOSE_QUEST_MANAGER);
}

void	C_VS_UI_QUEST_MANAGER::RunQuestIcon()
{
	if (m_Ui_Quest_Icon == NULL)
	{
		m_Ui_Quest_Icon = new C_VS_UI_QUEST_ICON(m_pImage_Spk_MailBox);

		assert(m_Ui_Quest_Icon!= NULL);

		m_Ui_Quest_Icon->Start();
	}
}
bool	C_VS_UI_QUEST_MANAGER::IsRunningQuestIcon() const
{
	return (NULL != m_Ui_Quest_Icon);
}
void	C_VS_UI_QUEST_MANAGER::CloseQuestIcon(int JustClose)
{
	if (NULL ==m_Ui_Quest_Icon)
		return;
	
	DeleteNew(m_Ui_Quest_Icon);
	
	if(JustClose>0) // �ٸ� â�� �����ش�
	{
		RunQuestList();
		if(IsRunningQuestList())
		{
			m_Ui_Quest_List->SetQuestListInfo((void*)&m_QuestListInfo);
		}
//		if(m_QuestItemInfo.size()>0)
//		{
//			RunQuestItem();
//			if(IsRunningQuestItem())
//			{
//				m_Ui_Quest_Item->SetQuestItemInfo(m_QuestItemInfo);
//			}
//		}
	}
	

//	if(IsAllWindowClosed())
//		gpC_base->SendMessage(UI_CLOSE_QUEST_MANAGER);
}
// ����,������ npc dialog�� ����. nType == 0 : ����, nType == 1 : ����
void	C_VS_UI_QUEST_MANAGER::RunQuestFail_or_Sucess(const XMLTree *pElement, int nType)
{
	if(pElement != NULL)
	{
		// ������ string ���� ã�´�..-_-
		const static char * szFail_or_Sucess[2] = {
													"CompleteMessage",
													"FailMessage",
													};

		const XMLTree *pElement2 = pElement->GetChild( szFail_or_Sucess[min(1, nType)]);
		if(pElement2 != NULL)
		{
			const char* Sender = "";
			int NpcID = 0;
			const XMLAttribute* _TempAttr = pElement2->GetAttribute("sender");
			if(NULL != _TempAttr)
				Sender = _TempAttr->ToString();
			else
			{
				_TempAttr = pElement2->GetAttribute("name");
				if(NULL != _TempAttr)
					Sender = _TempAttr->ToString();

			}

			bool IsShowItemDescription = false;

			_TempAttr = pElement2->GetAttribute("target");
			if(NULL != _TempAttr)
				NpcID = _TempAttr->ToInt();
			else
			{
				_TempAttr = pElement2->GetAttribute("targetitem");
				if(NULL != _TempAttr)
				{
					int TempItemType = _TempAttr->ToInt();
					int TempDescriptionID = (*g_pItemTable)[ITEM_CLASS_GQUEST_ITEM][TempItemType].DescriptionFrameID;
					if(0 != TempDescriptionID)
					{
						IsShowItemDescription = true;
						NpcID = TempDescriptionID;
						Sender = (*g_pItemTable)[ITEM_CLASS_GQUEST_ITEM][TempItemType].HName.GetString();
					}
				}
			}

			C_VS_UI_NPC_DIALOG* TempDialog = new C_VS_UI_NPC_DIALOG(ExecF_GQuestNpcScript, NpcID, Sender, 500, 250 ,DIALOG_OK, IsShowItemDescription);
			char * pp_dmsg[] = {(char*)(pElement2->GetText().c_str()),};
			TempDialog->SetMessage(pp_dmsg, sizeof(pp_dmsg)/sizeof(char *));//, SMO_NOFIT);
			gC_vs_ui.SetQuestNpcDialog((void*)TempDialog);
		}
		
		if(nType == 0) // ������ ��� ���� �̺�Ʈ�� ������� �Ѵٸ�
		{
			const XMLAttribute* _TempAttr = pElement->GetAttribute("ending");
			if(NULL != _TempAttr)
			{
				int EndingEventID = _TempAttr->ToInt();
				gpC_base->SendMessage(UI_GQUEST_ENDING_EVENT, EndingEventID);
			}
		}
	}
}

void	C_VS_UI_QUEST_MANAGER::PushGQuestExcuteElement(DWORD qID, BYTE bCondition, WORD wIndex)
{
	if(0 == m_QuestExcutElement.size()) // ���ΰ� ������ �ٷ� ����
	{
		RunGQuestExcuteElement(qID, bCondition, wIndex);
	}
	_GQuestExcuteElement* TempGQuestExcuteElement = new _GQuestExcuteElement;
	TempGQuestExcuteElement->qID = qID;
	TempGQuestExcuteElement->bCondition = bCondition;
	TempGQuestExcuteElement->wIndex = wIndex;
	m_QuestExcutElement.push_back(TempGQuestExcuteElement);

}
void	C_VS_UI_QUEST_MANAGER::RunNextGQuestExcuteElement()
{
	if(m_QuestExcutElement.size()) // �ϴ� ������ üũ
	{
		std::vector<_GQuestExcuteElement*>::iterator itr = m_QuestExcutElement.begin();
		C_VS_UI_QUEST_MANAGER::_GQuestExcuteElement * TempRun = NULL;
		if(itr != m_QuestExcutElement.end())
		{
			TempRun = (*itr);
			m_QuestExcutElement.erase(itr);

			RunGQuestExcuteElementAction(TempRun->qID, TempRun->bCondition, TempRun->wIndex);

			DeleteNew(TempRun);
		}
		if(m_QuestExcutElement.size())
		{
			itr = m_QuestExcutElement.begin();
			if(itr != m_QuestExcutElement.end())
			{
				TempRun = (*itr);
				if(NULL != TempRun)
					RunGQuestExcuteElement(TempRun->qID, TempRun->bCondition, TempRun->wIndex);
			}
		}
	}
}
// npc dialog Ȯ�� Ŭ�� �Ŀ� Ŭ���̾�Ʈ���� ���� ó���� �ൿ�� �ִ��� üũ...�ϵ��ڵ���.
void	C_VS_UI_QUEST_MANAGER::RunGQuestExcuteElementAction(DWORD qID, BYTE bCondition, WORD wIndex)
{
	if(LoadQuestXML() == false)
		return ;
	
	// ����Ʈ ����Ʈ�� id�� ã�´�..����÷���.-_-;;;
	const XMLTree *pElement = m_Quest_XML_Tree.GetChildByAttr( qID , "id");
	if(pElement != NULL)
	{
		// ������ string ���� ã�´�..-_-
		const static char * szConditonType[4] = {	"Happen",
													"Complete",
													"Fail",
													"Reward",
												};

		const XMLTree *pElement2 = pElement->GetChild( szConditonType[min(5, bCondition)]);
		if(pElement2 != NULL)
		{
	
			const XMLTree *pElement3 = pElement2->GetChildByAttr(  wIndex , "index");
			if(NULL != pElement3)
			{
				const XMLAttribute* _TempAttr = pElement3->GetAttribute("action");
				if(NULL != _TempAttr)
				{
					const char* TempAction = _TempAttr->ToString();
					if(0 == strcmp(TempAction, "dead")) // g_pPlayer �״� ����
					{
						int TargetZoneID = 0;
						const XMLAttribute* _TempAttr2 = pElement3->GetAttribute("warp");
						if(NULL != _TempAttr2)
						{
							TargetZoneID = _TempAttr2->ToInt();
						}
						// 1 : �״� �������� ����..-_-;
						gpC_base->SendMessage(UI_GQUEST_SET_ACTION, 1, TargetZoneID ); 
					}
				}
			}
		}
	}
	ReleaseQuestXML();
}
// GCExcuteElement ���� ���� �� npc dialog�� ����.
void	C_VS_UI_QUEST_MANAGER::RunGQuestExcuteElement(DWORD qID, BYTE bCondition, WORD wIndex)
{
	if(LoadQuestXML() == false)
		return ;
	
	// ����Ʈ ����Ʈ�� id�� ã�´�..����÷���.-_-;;;
	const XMLTree *pElement = m_Quest_XML_Tree.GetChildByAttr( qID , "id");
	if(pElement != NULL)
	{
		// ������ string ���� ã�´�..-_-
		const static char * szConditonType[4] = {	"Happen",
													"Complete",
													"Fail",
													"Reward",
												};

		const XMLTree *pElement2 = pElement->GetChild( szConditonType[min(5, bCondition)]);
		if(pElement2 != NULL)
		{
	
			const XMLTree *pElement3 = pElement2->GetChildByAttr(  wIndex , "index");
			if(NULL != pElement3)
			{
				const char* Sender = "";
				int NpcID = 0;
				const XMLAttribute* _TempAttr = pElement3->GetAttribute("sender");
				if(NULL != _TempAttr)
					Sender = _TempAttr->ToString();
				else
				{
					_TempAttr = pElement3->GetAttribute("name");
					if(NULL != _TempAttr)
						Sender = _TempAttr->ToString();

				}
				_TempAttr = pElement3->GetAttribute("target");

				bool IsShowItemDescription = false;
				if(NULL != _TempAttr)
					NpcID = _TempAttr->ToInt();
				else
				{
					_TempAttr = pElement3->GetAttribute("targetitem");
					if(NULL != _TempAttr)
					{
						int TempItemType = _TempAttr->ToInt();
						int TempDescriptionID = (*g_pItemTable)[ITEM_CLASS_GQUEST_ITEM][TempItemType].DescriptionFrameID;
						if(0 != TempDescriptionID)
						{
							IsShowItemDescription = true;
							NpcID = TempDescriptionID;
							Sender = (*g_pItemTable)[ITEM_CLASS_GQUEST_ITEM][TempItemType].HName.GetString();
						}
					}
				}

				C_VS_UI_NPC_DIALOG* TempDialog = new C_VS_UI_NPC_DIALOG(ExecF_GQuestNpcScript, NpcID, Sender, 500, 250 ,DIALOG_OK, IsShowItemDescription);
				char * pp_dmsg[] = {(char*)(pElement3->GetText().c_str()),};
				TempDialog->SetMessage(pp_dmsg, sizeof(pp_dmsg)/sizeof(char *));//, SMO_NOFIT);
				gC_vs_ui.SetQuestNpcDialog((void*)TempDialog);

			}
		}
	}
	ReleaseQuestXML();
}
//-----------------------------------------------------------------------------
// C_VS_UI_QUEST_LIST::C_VS_UI_QUEST_LIST
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_QUEST_LIST::C_VS_UI_QUEST_LIST(C_SPRITE_PACK* spr)
{	
	AttrTopmost(false);
	g_RegisterWindow(this);	
	AttrPin(true);	
	
	int window_w = 290;
	int window_h = 160;

	m_bl_focus = false;
	
	Set(g_GameRect.right/2-window_w/2, g_GameRect.bottom/2 - window_h/2, window_w, window_h);

	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_LIST));
	AttrAutoHide(gpC_vs_ui_window_manager->GetAutoHide(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_LIST));
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_LIST);
	if(rect.w != -1)
	{ 
		x = rect.x;
		y = rect.y;
	}
	if(g_eRaceInterface == RACE_OUSTERS)
		m_OustersOffset = 2;
	else
		m_OustersOffset = 0;

	m_pImage_Spk = spr;

	m_SelectPos = NOT_SELECTED;
	m_SelectContents = NOT_SELECTED;
	
	m_pC_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(w-18,75,-1,window_h-108));
	m_pC_scroll_bar->SetPosMax(0);

	m_TabID = 0;

	int pin_x = 24, pin_y = h-154 +m_OustersOffset;
	int close_x = w-20, close_y = h-152 +m_OustersOffset;		
	int alpha_x = 6, alpha_y = h-154 +m_OustersOffset;
		
	//�����ư
	m_pC_button_group = new ButtonGroup(this);
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pin_x, pin_y, 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), 
		PUSHPIN_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_x, close_y, 
		gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::BUTTON_CLOSE), 
		gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::BUTTON_CLOSE), 
		CLOSE_ID, this, C_GLOBAL_RESOURCE::BUTTON_CLOSE));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(alpha_x, alpha_y,
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA),
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA),
		ALPHA_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA));	

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(5, 30, 60, 20, TAB1_ID, this, NULL));	
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(65, 30, 60, 20,	TAB2_ID, this, NULL));	
#ifndef _LIB
//
//	C_VS_UI_QUEST_MANAGER::_GQuestInfo* Test1 = new C_VS_UI_QUEST_MANAGER::_GQuestInfo;
//	Test1->bStatus = CAN_ACCEPT;
//	Test1->szQuestTitle = "���������������������������������Ͽ�����";
//	Test1->dwQuestID = 1;
//	m_QuestListInfo[0].push_back(Test1);
//	C_VS_UI_QUEST_MANAGER::_GQuestInfo* Test2 = new C_VS_UI_QUEST_MANAGER::_GQuestInfo;
//	Test2->bStatus = DOING;
//	Test2->szQuestTitle = "���������������������������������̿�����";
//	Test2->dwQuestID = 2;
//	m_QuestListInfo[1].push_back(Test2);
//	m_pC_scroll_bar->SetPosMax(1);
#endif

}

C_VS_UI_QUEST_LIST::~C_VS_UI_QUEST_LIST()
{
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_LIST, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_LIST, Rect(x, y, w, h));	
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_LIST, GetAttributes()->alpha);

	g_UnregisterWindow(this);

	m_QuestListInfo[0].clear();	
	m_QuestListInfo[1].clear();

	DeleteNew(m_pC_button_group);	
	DeleteNew(m_pC_scroll_bar);
}

void	C_VS_UI_QUEST_LIST::Start()
{
	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_LIST));
	AttrAutoHide(gpC_vs_ui_window_manager->GetAutoHide(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_LIST));
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_LIST);
	if(rect.w != -1)
	{ 
		x = rect.x;
		y = rect.y;
	}
	else // default setting
	{
		x = 0;
		y = 37;
	}
	PI_Processor::Start();
	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();
	AttrTopmost(false);	
}

void	C_VS_UI_QUEST_LIST::Finish()
{
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_LIST, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_LIST, Rect(x, y, w, h));	
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_LIST, GetAttributes()->alpha);

	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);	
}

void	C_VS_UI_QUEST_LIST::Show()
{
	if(gpC_base->m_p_DDSurface_back->Lock())
	{	
		gpC_global_resource->DrawDialogLocked4(x, y, w, h, GetAttributes()->alpha);
		m_pImage_Spk->BltLocked(x+115, y+7+m_OustersOffset, C_VS_UI_QUEST_MANAGER::QUEST_LIST_TITLE);
		
		if(m_TabID == 0)
			m_pImage_Spk->BltLocked(x+4, y+30, C_VS_UI_QUEST_MANAGER::TAB_BUTTON_PROCESS);
		else
			m_pImage_Spk->BltLocked(x+4, y+30, C_VS_UI_QUEST_MANAGER::TAB_BUTTON_COMPLETE);
			
	//	m_pImage_Spk->BltLocked(x+2, y+2, QUEST_TITLE_BG);

		m_pImage_Spk->BltLocked(x+5, y+46, C_VS_UI_QUEST_MANAGER::QUEST_LIST_SUBJECT);
		m_pImage_Spk->BltLocked(x+5, y+68, C_VS_UI_QUEST_MANAGER::QUEST_TABLE_BG);
		m_pImage_Spk->BltLocked(x+5, y+85, C_VS_UI_QUEST_MANAGER::QUEST_TABLE_BG);
		m_pImage_Spk->BltLocked(x+5, y+102, C_VS_UI_QUEST_MANAGER::QUEST_TABLE_BG);
		m_pImage_Spk->BltLocked(x+5, y+119, C_VS_UI_QUEST_MANAGER::QUEST_TABLE_BG);
		
		

		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}
		
	m_pC_scroll_bar->Show(x,y);
	
	int ScrPos = m_pC_scroll_bar->GetScrollPos();
	if(g_FL2_GetDC())
	{
		char szString[256] = "\0";

		const static char* szQuestStatus[7]=
		{
			(*g_pGameStringTable)[UI_STRING_GQUEST_CAN_NOT].GetString(),// = "����Ұ�";
			(*g_pGameStringTable)[UI_STRING_GQUEST_CAN_ACCEPT].GetString(),// = "���డ��";
			(*g_pGameStringTable)[UI_STRING_GQUEST_DOING].GetString(),// = "������";
			(*g_pGameStringTable)[UI_STRING_GQUEST_SUCCESS].GetString(),// = "����";
			(*g_pGameStringTable)[UI_STRING_GQUEST_COMPLETE].GetString(),// = "�Ϸ�";
			(*g_pGameStringTable)[UI_STRING_GQUEST_FAIL].GetString(),// = "����";
			(*g_pGameStringTable)[UI_STRING_GQUEST_CAN_REPLAY].GetString(),// = "����డ";
		};
		for(int i =ScrPos; i< m_QuestListInfo[m_TabID].size(); i++)
		{
			if(i>ScrPos+4)
				break;
			C_VS_UI_QUEST_MANAGER::_GQuestInfo* TempInfo = (C_VS_UI_QUEST_MANAGER::_GQuestInfo*) m_QuestListInfo[m_TabID][i];
			if(TempInfo != NULL)
			{
				sprintf(szString,"%s", TempInfo->szQuestTitle.c_str());
				if(strlen(szString)>34)
					ReduceString2(szString,32);
				DWORD NormalColor = RGB_WHITE;
				if(i== m_SelectContents)
					NormalColor = RGB(200, 200, 255);
				g_PrintColorStr(x+7, y+72 + (i-ScrPos)*17, szString, gpC_base->m_chatting_pi, NormalColor);
				sprintf(szString,"%s", szQuestStatus[TempInfo->bStatus]);
				g_PrintColorStr(x+215, y+72 + (i-ScrPos)*17, szString, gpC_base->m_chatting_pi, RGB_YELLOW);
			}
		}
		m_pC_button_group->ShowDescription();
		g_FL2_ReleaseDC();
	}	
	m_bl_focus = false;
}

bool	C_VS_UI_QUEST_LIST::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	
	_x-=x; _y-=y;

	bool re = m_pC_button_group->MouseControl(message, _x, _y);

	if(NULL != m_pC_scroll_bar)
	re &= m_pC_scroll_bar->MouseControl(message, _x, _y);

	m_bl_focus = false;
	Rect rt(x, y, x+290, y+160);
	switch(message)
	{
	case M_MOVING:
		{
			Point pt(x+_x,y+_y);
			if( rt.IsInRect(pt) )
			{
				m_bl_focus = true;	
			}
			if(_x>5 && _x<270 && _y>68&& _y <136)
			{
				m_SelectContents = (_y-68 )/17;
				m_SelectContents += m_pC_scroll_bar->GetScrollPos();
			}
			else
				m_SelectContents = NOT_SELECTED;
//			if( !m_bl_active ) break;
//			
//			Point pt(x+_x,y+_y);
//			if( rt.IsInRect(pt) )
			{
//				m_bl_focus = true;				
//				g_descriptor_manager.Set(DID_INFO, _x+x,_y+y, (void *)(*g_pGameStringTable)[UI_STRING_MESSAGE_CLICK_TO_SHOW_DETIAL].GetString(),0,0);
			}
//			ShowQuestDescription(_x,_y);
		}
		break;
	case M_WHEEL_UP:
		m_pC_scroll_bar->ScrollUp();
		break;
		
	case M_WHEEL_DOWN:
		m_pC_scroll_bar->ScrollDown();
		break;

	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		{
			
//			Point pt(x+_x,y+_y);
//			if( rt.IsInRect(pt) )
//				m_bl_focus = true;
//
//			if( m_bl_focus )
//			{
//				Run(DETAIL_ID);
//			}
			if(_x>5 && _x<270 && _y>68&& _y <136)
			{
				m_SelectPos = (_y-68 )/17;
				m_SelectPos += m_pC_scroll_bar->GetScrollPos();
				if(m_SelectPos<m_QuestListInfo[m_TabID].size())
				{
					C_VS_UI_QUEST_MANAGER::_GQuestInfo* TempInfo = (C_VS_UI_QUEST_MANAGER::_GQuestInfo*) m_QuestListInfo[m_TabID][m_SelectPos];
					if(NULL != TempInfo)
						gC_vs_ui.SetQuestManagerSubWindow(TempInfo->dwQuestID);
				}
				else
					m_SelectPos = m_QuestListInfo[m_TabID].size()-1;
			}
			else if(re&&NULL == m_pC_button_group->IsInRect(_x, _y))
			{
				MoveReady();
				SetOrigin(_x, _y);
			}
//			if(IsRunningQuestDetail())
//			{
//				m_Ui_Quest_Detail->MoveReady();
//				m_Ui_Quest_Detail->SetOrigin(_x, _y);
//			}
			break;
		}
		break;
	}	
	return true;
}

void	C_VS_UI_QUEST_LIST::KeyboardControl(UINT message, UINT key, long extra)	
{	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{		
		case VK_ESCAPE :
			gpC_base->SendMessage(UI_CLOSE_QUEST_LIST);
			break;		
		}
	}
}

bool	C_VS_UI_QUEST_LIST::IsPixel(int _x,int _y)
{
	return IsInRect(_x, _y);
}

void	C_VS_UI_QUEST_LIST::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetID() == TAB1_ID || p_button->GetID() == TAB2_ID)
		return;
	if(p_button->GetID() == ALPHA_ID)
	{
		if(GetAttributes()->alpha)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
	} else
	if(p_button->GetID() == PUSHPIN_ID)
	{
		if(GetAttributes()->autohide)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN_PUSHED);
		
	} else
	if(p_button->GetID() == CLOSE_ID)
	{
		gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x-5, y+p_button->y-5, C_GLOBAL_RESOURCE::BUTTON_CLOSE_BACK);
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+3);
			else
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
		}
		else
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
			else
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
		}

	}else
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION_PUSHED);
			else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION_HILIGHTED);
		}
		else
		{
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
		}
	}
}

void	C_VS_UI_QUEST_LIST::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
	if(false == m_bl_focus)
		return;
	const static char* m_gQuest_List_Desc[8] =
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_OFF].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_ON].GetString(),
		(*g_pGameStringTable)[UI_STRING_GQUEST_TAB_PROCESS].GetString(),
		(*g_pGameStringTable)[UI_STRING_GQUEST_TAB_COMPLETE].GetString(),
	};
	switch(p_button->GetID())
	{
	case PUSHPIN_ID :
		if(GetAttributes()->autohide)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_gQuest_List_Desc[4],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_gQuest_List_Desc[5],0,0);
		break;
	case CLOSE_ID :
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_gQuest_List_Desc[2],0,0);
		break;
	case ALPHA_ID :
		if(GetAttributes()->alpha)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_gQuest_List_Desc[3],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_gQuest_List_Desc[0],0,0);
		break;
	case TAB1_ID:
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_gQuest_List_Desc[6],0,0);
		break;
	case TAB2_ID:
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_gQuest_List_Desc[7],0,0);
		break;
	}	
}

void	C_VS_UI_QUEST_LIST::Run(id_t id)
{
	switch (id)
	{
	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_QUEST_LIST);
		break;
		
	case ALPHA_ID:
		AttrAlpha(!GetAttributes()->alpha);
		EMPTY_MOVE;
		break;
		
	case PUSHPIN_ID :
		if(GetAttributes()->autohide)
			AttrAutoHide(ATTRIBUTES_HIDE_NOT);
		else
		{
			AttrAutoHide(ATTRIBUTES_HIDE_WIDTH);
		}		
		break;
	case TAB1_ID:
		m_TabID = 0;
		m_pC_scroll_bar->SetPosMax(m_QuestListInfo[0].size());
		m_pC_scroll_bar->SetScrollPos(0);
		break;
	case TAB2_ID:
		m_TabID = 1;
		m_pC_scroll_bar->SetPosMax(m_QuestListInfo[1].size());
		m_pC_scroll_bar->SetScrollPos(0);
		break;

	}
}

void	C_VS_UI_QUEST_LIST::Process()
{
	m_pC_button_group->Process();
	ProcessHide();
}
// ����Ʈ ����Ʈ�� �޾� �������� �Ͱ� �Ϸ�Ȱ͵� ���� ����Ʈ�� �����.
void	C_VS_UI_QUEST_LIST::SetQuestListInfo(void* pVoid)
{
	if(m_QuestListInfo[0].size())
	{
		std::vector<C_VS_UI_QUEST_MANAGER::_GQuestInfo*>::iterator itr = m_QuestListInfo[0].begin();
		while(itr != m_QuestListInfo[0].end())
		{
			C_VS_UI_QUEST_MANAGER::_GQuestInfo * TempInfo2 = (*itr);
			itr = m_QuestListInfo[0].erase(itr);
		//	DeleteNew(TempInfo2);
		}
		m_QuestListInfo[0].clear();
	}
	if(m_QuestListInfo[1].size())
	{
		std::vector<C_VS_UI_QUEST_MANAGER::_GQuestInfo*>::iterator itr = m_QuestListInfo[1].begin();
		while(itr != m_QuestListInfo[1].end())
		{
			C_VS_UI_QUEST_MANAGER::_GQuestInfo * TempInfo2 = (*itr);
			itr = m_QuestListInfo[1].erase(itr);
		//	DeleteNew(TempInfo2);
		}
		m_QuestListInfo[1].clear();
	}
	// setting
	std::vector<C_VS_UI_QUEST_MANAGER::_GQuestInfo*>* pTempList = (std::vector<C_VS_UI_QUEST_MANAGER::_GQuestInfo*>*)pVoid;
	std::vector<C_VS_UI_QUEST_MANAGER::_GQuestInfo*>::iterator itr = pTempList->begin();
	while(itr != pTempList->end())
	{
		C_VS_UI_QUEST_MANAGER::_GQuestInfo * TempInfo = (*itr);
		if(TempInfo->bStatus == CAN_ACCEPT
	     ||TempInfo->bStatus == DOING
		 ||TempInfo->bStatus == SUCCESS ) // ������ ����Ʈ�� �߰�
		 m_QuestListInfo[0].push_back(TempInfo);
		else if(  TempInfo->bStatus == COMPLETE
				||TempInfo->bStatus == FAIL
				||TempInfo->bStatus == CAN_REPLAY ) // �Ϸ� ����Ʈ�� �߰�
		m_QuestListInfo[1].push_back(TempInfo);

		itr++;
	}

	if(m_TabID == 0) // ���� ��
		m_pC_scroll_bar->SetPosMax(m_QuestListInfo[0].size());
	else if(m_TabID == 1) // �Ϸ� ��
		m_pC_scroll_bar->SetPosMax(m_QuestListInfo[1].size());
}
//-----------------------------------------------------------------------------
// C_VS_UI_QUEST_DETAIL::C_VS_UI_QUEST_DETAIL
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_QUEST_DETAIL::C_VS_UI_QUEST_DETAIL(C_SPRITE_PACK* spr)
{	
	AttrTopmost(false);
	g_RegisterWindow(this);	
	AttrPin(true);	
	
	int window_w = 290;
	int window_h = 160;
	
	Set(g_GameRect.right/2-window_w/2, g_GameRect.bottom/2 - window_h/2, window_w, window_h);

	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_DETAIL));
	AttrAutoHide(gpC_vs_ui_window_manager->GetAutoHide(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_DETAIL));
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_DETAIL);
	if(rect.w != -1)
	{ 
		x = rect.x;
		y = rect.y;
	}
	if(g_eRaceInterface == RACE_OUSTERS)
		m_OustersOffset = 2;
	else
		m_OustersOffset = 0;

	m_pImage_Spk = spr;
	m_IsAbleRunQuest = 0;
	m_bl_focus = false;

	m_pC_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(w-18,60,-1,window_h-105));
	m_pC_scroll_bar->SetPosMax(0);

	int pin_x = 24, pin_y = h-154+m_OustersOffset;
	int close_x = w-20, close_y = h-152+m_OustersOffset;		
	int alpha_x = 6, alpha_y = h-154+m_OustersOffset;
	int accept_x = w-80, accept_y = h-36+m_OustersOffset;
		
	//�����ư
	m_pC_button_group = new ButtonGroup(this);
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pin_x, pin_y, 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), 
		PUSHPIN_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_x, close_y, 
		gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::BUTTON_CLOSE), 
		gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::BUTTON_CLOSE), 
		CLOSE_ID, this, C_GLOBAL_RESOURCE::BUTTON_CLOSE));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(alpha_x, alpha_y,
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA),
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA),
		ALPHA_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA));	
	
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(accept_x, accept_y,
		m_pImage_Spk->GetWidth(C_VS_UI_QUEST_MANAGER::BUTTON_ACCEPT),
		m_pImage_Spk->GetHeight(C_VS_UI_QUEST_MANAGER::BUTTON_ACCEPT),
		ACCEPT_ID, this, C_VS_UI_QUEST_MANAGER::BUTTON_ACCEPT));
#ifndef _LIB
//	char* TempStr = "���̻�����ĥ�ȱ������̻�����ĥ�ȱ���";
//	if(LoadDescFromString(TempStr, 44, 4, false)==true)
//	{
//		SetDesc(8, 50+m_OustersOffset, RGB_GRAY, gpC_base->m_chatting_pi);
//		m_pC_scroll_bar->SetPosMax(GetDescSize() - GetDescCol() +1);
//		m_szTitle = TempStr;
////		SetDescTitle(TempStr);
////		SetDescTitle(10, 31+m_OustersOffset, RGB_YELLOW, gpC_base->m_user_id_pi);
//	}
#endif	
}

C_VS_UI_QUEST_DETAIL::~C_VS_UI_QUEST_DETAIL()
{
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_DETAIL, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_DETAIL, Rect(x, y, w, h));	
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_DETAIL, GetAttributes()->alpha);

	g_UnregisterWindow(this);
	
	DeleteNew(m_pC_button_group);	
	DeleteNew(m_pC_scroll_bar);
}

void	C_VS_UI_QUEST_DETAIL::Start()
{
	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_DETAIL));
	AttrAutoHide(gpC_vs_ui_window_manager->GetAutoHide(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_DETAIL));
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_DETAIL);
	if(rect.w != -1)
	{ 
		x = rect.x;
		y = rect.y;
	}
	else // default setting
	{
		x = 0;
		y = 197;
	}
	PI_Processor::Start();
	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();
	AttrTopmost(false);	
}

void	C_VS_UI_QUEST_DETAIL::Finish()
{
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_DETAIL, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_DETAIL, Rect(x, y, w, h));	
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_DETAIL, GetAttributes()->alpha);

	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);	
}

void	C_VS_UI_QUEST_DETAIL::Show()
{
	int ScrPos = m_pC_scroll_bar->GetScrollPos();
	if(gpC_base->m_p_DDSurface_back->Lock())
	{	
		gpC_global_resource->DrawDialogLocked4(x, y, w, h, GetAttributes()->alpha);
		m_pImage_Spk->BltLocked(x+100, y+7+m_OustersOffset, C_VS_UI_QUEST_MANAGER::QUEST_DETAIL_TITLE);

		m_pImage_Spk->BltLocked(x+5, y+26+m_OustersOffset, C_VS_UI_QUEST_MANAGER::QUEST_TITLE_BG);
		m_pImage_Spk->BltLocked(x+5, y+46+m_OustersOffset, C_VS_UI_QUEST_MANAGER::QUEST_DETAIL_BG);

		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}
		
	m_pC_scroll_bar->Show(x,y);
	
	SetDescScrollPos(m_pC_scroll_bar->GetScrollPos());
	ShowDesc(x,y);
	if(g_FL2_GetDC())
	{
		char szString[64];
		sprintf(szString,"%s", m_szTitle.c_str());
		if(strlen(szString)>40)
			ReduceString2(szString,38);
		g_PrintColorStr(x+9, y+31+m_OustersOffset, szString, gpC_base->m_user_id_pi, RGB_YELLOW);
		m_pC_button_group->ShowDescription();
		g_FL2_ReleaseDC();
	}	
	m_bl_focus = false;
}

bool	C_VS_UI_QUEST_DETAIL::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);

	if(NULL != m_pC_scroll_bar)
	re &= m_pC_scroll_bar->MouseControl(message, _x, _y);

	m_bl_focus = false;
	Rect rt(x, y, x+290, y+160);
	switch(message)
	{
	case M_MOVING:
		{
			Point pt(x+_x,y+_y);
			if( rt.IsInRect(pt) )
			{
				m_bl_focus = true;	
			}
		}
		break;
	case M_WHEEL_UP:
		m_pC_scroll_bar->ScrollUp();
		break;
		
	case M_WHEEL_DOWN:
		m_pC_scroll_bar->ScrollDown();
		break;

	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		{
			
//			Point pt(x+_x,y+_y);
//			if( rt.IsInRect(pt) )
//				m_bl_focus = true;
//
//			if( m_bl_focus )
//			{
//				Run(DETAIL_ID);
//			}
			if(re && NULL == m_pC_button_group->IsInRect(_x, _y))
			{
				MoveReady();
				SetOrigin(_x, _y);
			}
			break;
		}
		break;
	}	
	return true;
}

void	C_VS_UI_QUEST_DETAIL::KeyboardControl(UINT message, UINT key, long extra)	
{	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{		
		case VK_ESCAPE :
			gpC_base->SendMessage(UI_CLOSE_QUEST_DETAIL);
			break;		
		}
	}
}

bool	C_VS_UI_QUEST_DETAIL::IsPixel(int _x,int _y)
{
	return IsInRect(_x, _y);
}

void	C_VS_UI_QUEST_DETAIL::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
//	if(p_button->GetID() == TAB1_ID || p_button->GetID() == TAB2_ID)
//		return;
	if(p_button->GetID() == ALPHA_ID)
	{
		if(GetAttributes()->alpha)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
	} else
	if(p_button->GetID() == PUSHPIN_ID)
	{
		if(GetAttributes()->autohide)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN_PUSHED);
		
	} else
	if(p_button->GetID() == CLOSE_ID)
	{
		gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x-5, y+p_button->y-5, C_GLOBAL_RESOURCE::BUTTON_CLOSE_BACK);
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+3);
			else
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
		}
		else
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
			else
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
		}

	}else
	if(p_button->GetID() == ACCEPT_ID)
	{
		if(m_IsAbleRunQuest == 1)//����
		{
			if(p_button->GetFocusState())
			{
				if(p_button->GetPressState())
					m_pImage_Spk->BltLocked(x+p_button->x, y+p_button->y,  C_VS_UI_QUEST_MANAGER::BUTTON_ACCEPT+2);
				else
					m_pImage_Spk->BltLocked(x+p_button->x, y+p_button->y,  C_VS_UI_QUEST_MANAGER::BUTTON_ACCEPT+1);
			}
			else
			{
				m_pImage_Spk->BltLocked(x+p_button->x, y+p_button->y, C_VS_UI_QUEST_MANAGER::BUTTON_ACCEPT);
			}
		}
		else if(m_IsAbleRunQuest == 2) // ����
		{
			if(p_button->GetFocusState())
			{
				if(p_button->GetPressState())
					m_pImage_Spk->BltLocked(x+p_button->x, y+p_button->y,  C_VS_UI_QUEST_MANAGER::BUTTON_GIVEUP+2);
				else
					m_pImage_Spk->BltLocked(x+p_button->x, y+p_button->y,  C_VS_UI_QUEST_MANAGER::BUTTON_GIVEUP+1);
			}
			else
			{
				m_pImage_Spk->BltLocked(x+p_button->x, y+p_button->y, C_VS_UI_QUEST_MANAGER::BUTTON_GIVEUP);
			}

		}
		else
			m_pImage_Spk->BltLockedDarkness(x+p_button->x, y+p_button->y, C_VS_UI_QUEST_MANAGER::BUTTON_ACCEPT,1);
	}
	else
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				m_pImage_Spk->BltLocked(x+p_button->x, y+p_button->y,  p_button->m_image_index+2);
			else
				m_pImage_Spk->BltLocked(x+p_button->x, y+p_button->y,  p_button->m_image_index+1);
		}
		else
		{
			m_pImage_Spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
		}
	}
}

void	C_VS_UI_QUEST_DETAIL::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
	if(false == m_bl_focus)
		return;
	const static char* m_qQuest_Detail_Desc[8]=
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_OFF].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_ON].GetString(),
		(*g_pGameStringTable)[UI_STRING_GQUEST_BUTTON_ACCEPT].GetString(),
		(*g_pGameStringTable)[UI_STRING_GQUEST_BUTTON_GIVEUP].GetString(),
	};
	switch(p_button->GetID())
	{
	case PUSHPIN_ID :
		if(GetAttributes()->autohide)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_qQuest_Detail_Desc[4],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_qQuest_Detail_Desc[5],0,0);
		break;
	case CLOSE_ID :
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_qQuest_Detail_Desc[2],0,0);
		break;
	case ALPHA_ID :
		if(GetAttributes()->alpha)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_qQuest_Detail_Desc[3],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_qQuest_Detail_Desc[0],0,0);
		break;
	case ACCEPT_ID:
		if(m_IsAbleRunQuest == 1)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_qQuest_Detail_Desc[6],0,0);
		else if(m_IsAbleRunQuest == 2)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_qQuest_Detail_Desc[7],0,0);
		break;
	}	
}

void	C_VS_UI_QUEST_DETAIL::Run(id_t id)
{
	switch (id)
	{
	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_QUEST_DETAIL);
		break;
		
	case ALPHA_ID:
		AttrAlpha(!GetAttributes()->alpha);
		EMPTY_MOVE;
		break;
		
	case PUSHPIN_ID :
		if(GetAttributes()->autohide)
			AttrAutoHide(ATTRIBUTES_HIDE_NOT);
		else
		{
			AttrAutoHide(ATTRIBUTES_HIDE_WIDTH);
		}		
		break;
	case ACCEPT_ID:
		if(m_IsAbleRunQuest == 1)
		{
			gC_vs_ui.SendQuestIntention(true);
		}
		else if(m_IsAbleRunQuest == 2)
		{
			gC_vs_ui.SendQuestIntention(false);
		}
//	case TAB1_ID:
//		m_TabID = 0;
//		break;
//	case TAB2_ID:
//		m_TabID = 1;
//		break;

	}
}

void	C_VS_UI_QUEST_DETAIL::Process()
{
	m_pC_button_group->Process();
	ProcessHide();
}
// ����Ʈ�� Ÿ��Ʋ�� ��ũ��Ʈ, �׸��� ���డ�� ���¸� ���� �Ѵ�.
void	C_VS_UI_QUEST_DETAIL::SetQuestDetailInfo(char* szTitle, char* szDescription, int Status)
{
	if(szDescription != NULL && strlen(szDescription)>0)
	{
		if(LoadDescFromString(szDescription, 44, 4, false)==true)
		{
			SetDesc(7, 50+m_OustersOffset, RGB_WHITE, gpC_base->m_chatting_pi);
			m_pC_scroll_bar->SetPosMax(GetDescSize() - GetDescCol() +1);
//			if(szTitle != NULL)
//			{
//				SetDescTitle(szTitle);
//				SetDescTitle(10, 31+m_OustersOffset, RGB_YELLOW, gpC_base->m_user_id_pi);
//			}
		}
	}
	if(szTitle != NULL&& strlen(szTitle)>0)
		m_szTitle = szTitle;
	// status�� C_VS_UI_QUEST_LIST�� QUEST_LIST_STATUS �̴�.
	if( Status == 1 || Status == 6)
		m_IsAbleRunQuest = 1; // accept
	else if( Status == 2 || Status == 3)
		m_IsAbleRunQuest = 2; // giveup
	else
		m_IsAbleRunQuest = 0; // disable
}
//-----------------------------------------------------------------------------
// C_VS_UI_QUEST_MISSION::C_VS_UI_QUEST_MISSION
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_QUEST_MISSION::C_VS_UI_QUEST_MISSION(C_SPRITE_PACK* spr)
{	
	AttrTopmost(false);
	g_RegisterWindow(this);	
	AttrPin(true);	
	
	int window_w = 290;
	int window_h = 160;
	
	m_bl_focus = false;
	Set(g_GameRect.right/2-window_w/2, g_GameRect.bottom/2 - window_h/2, window_w, window_h);

	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_MISSION));
	AttrAutoHide(gpC_vs_ui_window_manager->GetAutoHide(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_MISSION));
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_MISSION);
	if(rect.w != -1)
	{ 
		x = rect.x;
		y = rect.y;
	}
	if(g_eRaceInterface == RACE_OUSTERS)
		m_OustersOffset = 2;
	else
		m_OustersOffset = 0;

	m_pImage_Spk = spr;
	m_SelectPos = NOT_SELECTED;

	m_pC_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(w-18,45,-1,window_h-68));
	m_pC_scroll_bar->SetPosMax(0);

	int pin_x = 24, pin_y = h-154 +m_OustersOffset;
	int close_x = w-20, close_y = h-152+m_OustersOffset;		
	int alpha_x = 6, alpha_y = h-154 +m_OustersOffset;
		
	//�����ư
	m_pC_button_group = new ButtonGroup(this);
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pin_x, pin_y, 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), 
		PUSHPIN_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_x, close_y, 
		gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::BUTTON_CLOSE), 
		gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::BUTTON_CLOSE), 
		CLOSE_ID, this, C_GLOBAL_RESOURCE::BUTTON_CLOSE));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(alpha_x, alpha_y,
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA),
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA),
		ALPHA_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA));	

#ifndef _LIB
	C_VS_UI_QUEST_MANAGER::_GMissionInfo *TempInfo = new C_VS_UI_QUEST_MANAGER::_GMissionInfo ;
	TempInfo->szMissionTitle = "���̻�����ĥ��%s:%d";
	TempInfo->bStatus = CURRENT;
	TempInfo->dwTimeLimit = timeGetTime() + (60000);
	m_QuestMissionInfo.push_back(TempInfo);
	C_VS_UI_QUEST_MANAGER::_GMissionInfo *TempInfo2 = new C_VS_UI_QUEST_MANAGER::_GMissionInfo ;
	TempInfo2->szMissionTitle = "�ٺ��� ���϶�";
	TempInfo2->bStatus = SUCCESS;
	m_QuestMissionInfo.push_back(TempInfo2);

	m_pC_scroll_bar->SetPosMax(2);
#endif 
}

C_VS_UI_QUEST_MISSION::~C_VS_UI_QUEST_MISSION()
{
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_MISSION, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_MISSION, Rect(x, y, w, h));	
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_MISSION, GetAttributes()->alpha);

	g_UnregisterWindow(this);
	
	DeleteNew(m_pC_button_group);	
	DeleteNew(m_pC_scroll_bar);
}

void	C_VS_UI_QUEST_MISSION::Start()
{
	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_MISSION));
	AttrAutoHide(gpC_vs_ui_window_manager->GetAutoHide(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_MISSION));
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_MISSION);
	if(rect.w != -1)
	{ 
		x = rect.x;
		y = rect.y;
	}
	else // default setting
	{
		x = 0;
		y = 357;
	}
	PI_Processor::Start();
	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();
	AttrTopmost(false);	
}

void	C_VS_UI_QUEST_MISSION::Finish()
{
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_MISSION, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_MISSION, Rect(x, y, w, h));	
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_MISSION, GetAttributes()->alpha);

	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);	
}

void	C_VS_UI_QUEST_MISSION::Show()
{
	static char szMissionPopupString[512];
	if(gpC_base->m_p_DDSurface_back->Lock())
	{	
		gpC_global_resource->DrawDialogLocked4(x, y, w, h, GetAttributes()->alpha);
		m_pImage_Spk->BltLocked(x+110, y+7+m_OustersOffset, C_VS_UI_QUEST_MANAGER::QUEST_MISSION_TITLE);

		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}
//	RECT rect = { x+10, y+30, x+270, y+150};
//	Rect rt( rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top );
//
//	if(!GetAttributes()->alpha)
//	{
//		gpC_base->m_p_DDSurface_back->FillRect(&rect, 0);
//	}
//
//	gpC_global_resource->DrawOutBox(rt);
		
	m_pC_scroll_bar->Show(x,y);
	int ScrPos = m_pC_scroll_bar->GetScrollPos();
	
	if(g_FL2_GetDC())
	{
		const static char* szQuestStatus[4]=
		{
			(*g_pGameStringTable)[UI_STRING_GQUEST_CAN_NOT].GetString(),// = "����Ұ�";
			(*g_pGameStringTable)[UI_STRING_GQUEST_DOING].GetString(),// = "������";
			(*g_pGameStringTable)[UI_STRING_GQUEST_COMPLETE].GetString(),// = "�Ϸ�";
			(*g_pGameStringTable)[UI_STRING_GQUEST_FAIL].GetString(),// = "����";
		};
		char szString[512];
		char szString2[512];
		for(int i =ScrPos; i< m_QuestMissionInfo.size(); i++)
		{
			if(i>ScrPos+5)
				break;
			C_VS_UI_QUEST_MANAGER::_GMissionInfo* TempInfo = (C_VS_UI_QUEST_MANAGER::_GMissionInfo*)m_QuestMissionInfo[i];
			if(TempInfo != NULL)
			{
				sprintf(szString,(*g_pGameStringTable)[UI_STRING_GQUEST_MISSION].GetString(), i+1, TempInfo->szMissionTitle.c_str());
				DWORD TempValue = TempInfo->m_NumArg;
				if(TempInfo->dwTimeLimit)
				{
					DWORD CurrentTime = timeGetTime() ;
					if(TempInfo->dwTimeLimit < CurrentTime)
						TempValue = TempInfo->m_NumArg - ((CurrentTime - TempInfo->dwTimeLimit) / 60000);
					else
					{
						TempValue = 0;
					}

					if(TempInfo->bStatus == SUCCESS || TempInfo->bStatus == FAIL) // ���� Ȥ�� ���� �϶�
					{
						TempInfo->dwTimeLimit = 0;
						TempInfo->m_NumArg = 0;
					}
				}
				sprintf(szString2, szString, TempInfo->m_StrArg.c_str(), TempValue);
				if(strlen(szString2)>36)
				{
					if(i == m_SelectPos)
					{
						strcpy(szMissionPopupString, szString2);
						if(m_bl_focus)
						{
							g_descriptor_manager.Set(DID_MULTILINE_INFO, x+4,y+38+(m_SelectPos-ScrPos)*17, (void *)szMissionPopupString,0,36);
						}
					}
					ReduceString2(szString2,36);
				}
				g_PrintColorStr(x+7, y+42 + (i-ScrPos)*17, szString2, gpC_base->m_chatting_pi, RGB_WHITE);
				if(TempInfo->bStatus != 1 && TempInfo->bStatus<3) // ������..�̰� �������� ����� �ؼ�.����
				{
					sprintf(szString,"%s", szQuestStatus[TempInfo->bStatus]);
					g_PrintColorStr(x+235, y+42 + (i-ScrPos)*17, szString, gpC_base->m_chatting_pi, RGB_YELLOW);
				}
			}
		}
		m_pC_button_group->ShowDescription();
		g_FL2_ReleaseDC();
	}	
	m_bl_focus = false;
}

bool	C_VS_UI_QUEST_MISSION::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);

	if(NULL != m_pC_scroll_bar)
	re &= m_pC_scroll_bar->MouseControl(message, _x, _y);

	m_bl_focus = false;

	Rect rt(x, y, x+290, y+160);
	switch(message)
	{
	case M_MOVING:
		{
			Point pt(x+_x,y+_y);
			if( rt.IsInRect(pt) )
			{
				m_bl_focus = true;	
			}
			if(_x>7 && _x<260 && _y>40&& _y <142)
			{
				int ScrPos = m_pC_scroll_bar->GetScrollPos();
				int TempPos = (_y-40)/17 + ScrPos;
				if(TempPos < m_QuestMissionInfo.size())
				{
					m_SelectPos = TempPos;
					break;
//					C_VS_UI_QUEST_MANAGER::_GMissionInfo* TempInfo = (C_VS_UI_QUEST_MANAGER::_GMissionInfo*)m_QuestMissionInfo[TempPos];
//					if(NULL != TempInfo)
//					{
//						if(strlen(TempInfo->szMissionTitle.c_str()) > 30)
//							g_descriptor_manager.Set(DID_INFO, x+4,y+38+(TempPos-ScrPos)*17, (void *)TempInfo->szMissionTitle.c_str(),0,0);
//					}
				}
					
			}
			m_SelectPos = NOT_SELECTED;
//			if( !m_bl_active ) break;
//			
//			Point pt(x+_x,y+_y);
//			if( rt.IsInRect(pt) && (m_quest_status.QuestID == 57 || m_quest_status.QuestID == 72 || m_quest_status.QuestID == 87))
//			{
//				m_bl_focus = true;				
//				g_descriptor_manager.Set(DID_INFO, _x+x,_y+y, (void *)(*g_pGameStringTable)[UI_STRING_MESSAGE_CLICK_TO_SHOW_DETIAL].GetString(),0,0);
//			}
//			ShowQuestDescription(_x,_y);
		}
		break;
	case M_WHEEL_UP:
		m_pC_scroll_bar->ScrollUp();
		break;
		
	case M_WHEEL_DOWN:
		m_pC_scroll_bar->ScrollDown();
		break;

	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		{
			
//			Point pt(x+_x,y+_y);
//			if( rt.IsInRect(pt) )
//				m_bl_focus = true;
//
//			if( m_bl_focus )
//			{
//				Run(DETAIL_ID);
//			}
			if(re && NULL == m_pC_button_group->IsInRect(_x, _y))
			{
				MoveReady();
				SetOrigin(_x, _y);
			}
			break;
		}
		break;
	}	
	return true;
}

void	C_VS_UI_QUEST_MISSION::KeyboardControl(UINT message, UINT key, long extra)	
{	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{		
		case VK_ESCAPE :
			gpC_base->SendMessage(UI_CLOSE_QUEST_MISSION);
			break;		
		}
	}
}

bool	C_VS_UI_QUEST_MISSION::IsPixel(int _x,int _y)
{
	return IsInRect(_x, _y);
}

void	C_VS_UI_QUEST_MISSION::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetID() == TAB1_ID || p_button->GetID() == TAB2_ID)
		return;
	if(p_button->GetID() == ALPHA_ID)
	{
		if(GetAttributes()->alpha)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
	} else
	if(p_button->GetID() == PUSHPIN_ID)
	{
		if(GetAttributes()->autohide)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN_PUSHED);
		
	} else
	if(p_button->GetID() == CLOSE_ID)
	{
		gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x-5, y+p_button->y-5, C_GLOBAL_RESOURCE::BUTTON_CLOSE_BACK);
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+3);
			else
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
		}
		else
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
			else
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
		}

	}else
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION_PUSHED);
			else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION_HILIGHTED);
		}
		else
		{
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
		}
	}
}

void	C_VS_UI_QUEST_MISSION::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
	if(m_bl_focus == false)
		return ;
	const static char* m_gQuest_Mission_desc[6]=
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_OFF].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_ON].GetString(),
	};
	switch(p_button->GetID())
	{
	case PUSHPIN_ID :
		if(GetAttributes()->autohide)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_gQuest_Mission_desc[4],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_gQuest_Mission_desc[5],0,0);
		break;
	case CLOSE_ID :
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_gQuest_Mission_desc[2],0,0);
		break;
	case ALPHA_ID :
		if(GetAttributes()->alpha)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_gQuest_Mission_desc[3],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_gQuest_Mission_desc[0],0,0);
		break;
	}	
}

void	C_VS_UI_QUEST_MISSION::Run(id_t id)
{
	switch (id)
	{
	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_QUEST_MISSION);
		break;
		
	case ALPHA_ID:
		AttrAlpha(!GetAttributes()->alpha);
		EMPTY_MOVE;
		break;
		
	case PUSHPIN_ID :
		if(GetAttributes()->autohide)
			AttrAutoHide(ATTRIBUTES_HIDE_NOT);
		else
		{
			AttrAutoHide(ATTRIBUTES_HIDE_WIDTH);
		}		
		break;

	}
}

void	C_VS_UI_QUEST_MISSION::Process()
{
	m_pC_button_group->Process();
	ProcessHide();
}
// ����Ʈ �̼� ������ �����Ѵ�.
void	C_VS_UI_QUEST_MISSION::SetQuestMissionInfo(std::vector<C_VS_UI_QUEST_MANAGER::_GMissionInfo*>& Info)
{
	m_QuestMissionInfo = Info;
//	if(m_QuestMissionInfo.size())
//	{
//		std::vector<C_VS_UI_QUEST_MANAGER::_GMissionInfo*>::iterator itr = m_QuestMissionInfo.begin();
//		while(itr != m_QuestMissionInfo.end())
//		{
//			C_VS_UI_QUEST_MANAGER::_GMissionInfo * TempInfo2 = (*itr);
//			m_QuestMissionInfo.erase(itr);
//			DeleteNew(TempInfo2);
//		}
//		m_QuestMissionInfo.clear();
//	}
//	std::vector<C_VS_UI_QUEST_MANAGER::_GMissionInfo*>* TempInfo = (std::vector<C_VS_UI_QUEST_MANAGER::_GMissionInfo*>*)pVoid;
//	std::vector<C_VS_UI_QUEST_MANAGER::_GMissionInfo*>::iterator itr = TempInfo->begin();
//	while(itr != TempInfo->end())
//	{
//		C_VS_UI_QUEST_MANAGER::_GMissionInfo * TempInfo = (*itr);
//		if(TempInfo->bStatus != HIDE) // �������� ����
//			m_QuestMissionInfo.push_back(TempInfo);
//		
//		itr++;
//	}
	m_pC_scroll_bar->SetPosMax(m_QuestMissionInfo.size());
}

//-----------------------------------------------------------------------------
// C_VS_UI_QUEST_ITEM::C_VS_UI_QUEST_ITEM
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_QUEST_ITEM::C_VS_UI_QUEST_ITEM(C_SPRITE_PACK* spr)
{	
	AttrTopmost(false);
	g_RegisterWindow(this);	
	AttrPin(true);	
	
	int window_w = 315;//290;
	int window_h = 260;//160;
	
	m_bl_focus = false;
	Set(g_GameRect.right/2-window_w/2, g_GameRect.bottom/2 - window_h/2, window_w, window_h);
//
//	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_ITEM));
	AttrAutoHide(gpC_vs_ui_window_manager->GetAutoHide(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_ITEM));
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_ITEM);
	if(rect.w != -1)
	{ 
		x = rect.x;
		y = rect.y;
	}
	if(g_eRaceInterface == RACE_OUSTERS)
		m_OustersOffset = 2;
	else
		m_OustersOffset = 0;

	m_pImage_Spk = spr;
	m_SelectPos = NOT_SELECTED;
	m_pC_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(20,230,276,-1), false, NULL, 6, 6, 0,false);
	m_pC_scroll_bar->SetPosMax(0);

	int pin_x = 6, pin_y = h-254+m_OustersOffset;
	int close_x = w-20, close_y = h-252+m_OustersOffset;		
	int alpha_x = 24, alpha_y = h-254+m_OustersOffset;
		
	//�����ư
	m_pC_button_group = new ButtonGroup(this);
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pin_x, pin_y, 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), 
		PUSHPIN_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_x, close_y, 
		gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::BUTTON_CLOSE), 
		gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::BUTTON_CLOSE), 
		CLOSE_ID, this, C_GLOBAL_RESOURCE::BUTTON_CLOSE));

//	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(alpha_x, alpha_y,
//		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA),
//		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA),
//		ALPHA_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA));	


#ifndef _LIB
	// -- Test only
//	MItem*		pItem;
//
//	//----------------------------------------------------
//	// Item ���� --> �߰�
//	//----------------------------------------------------
//	pItem = MItem::NewItem( ITEM_CLASS_POTION );
//	pItem->SetID( 0 );
//	pItem->SetItemType( 0 );
////	pItem->SetItemOptionList( 0 );
//	pItem->SetNumber(10);
//	pItem->SetCurrentDurability( 1 );	
//	m_QuestItemInfo.push_back(pItem);
//
//	//----------------------------------------------------
//	// Item ���� --> �߰�
//	//----------------------------------------------------
//	pItem = MItem::NewItem( ITEM_CLASS_RING );
//	pItem->SetID( 1 );
//	pItem->SetItemType( 1 );
////	pItem->SetItemOptionList( 0 );
//	pItem->SetCurrentDurability( 1 );	
//	m_QuestItemInfo.push_back(pItem);
//	
//	//----------------------------------------------------
//	// Item ���� --> �߰�
//	//----------------------------------------------------
//	pItem = MItem::NewItem( ITEM_CLASS_RING );
//	pItem->SetID( 2 );
//	pItem->SetItemType( 2 );
////	pItem->SetItemOptionList( 0 );
//	pItem->SetCurrentDurability( 1 );	
//	m_QuestItemInfo.push_back(pItem);
//
//	//----------------------------------------------------
//	// Item ���� --> �߰�
//	//----------------------------------------------------
//	pItem = MItem::NewItem( ITEM_CLASS_RING );
//	pItem->SetID( 3 );
//	pItem->SetItemType( 3 );
////	pItem->SetItemOptionList( 0 );
//	pItem->SetCurrentDurability( 1 );	
//	m_QuestItemInfo.push_back(pItem);
//
//	//----------------------------------------------------
//	// Item ���� --> �߰�
//	//----------------------------------------------------
//	pItem = MItem::NewItem( ITEM_CLASS_RING );
//	pItem->SetID( 4 );
//	pItem->SetItemType( 4 );
////	pItem->SetItemOptionList( 0 );
//	pItem->SetCurrentDurability( 1 );	
//	m_QuestItemInfo.push_back(pItem);
//	
//
//	m_pC_scroll_bar->SetPosMax(m_QuestItemInfo.size()/2+1);
#endif  
}

C_VS_UI_QUEST_ITEM::~C_VS_UI_QUEST_ITEM()
{
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_ITEM, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_ITEM, Rect(x, y, w, h));	
//	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_ITEM, GetAttributes()->alpha);

	g_UnregisterWindow(this);
	
	DeleteNew(m_pC_button_group);	
	DeleteNew(m_pC_scroll_bar);
}

void	C_VS_UI_QUEST_ITEM::Start(std::vector<MItem*>& Info)
{
//	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_ITEM));
	AttrAutoHide(gpC_vs_ui_window_manager->GetAutoHide(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_ITEM));
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_ITEM);
	if(rect.w != -1)
	{ 
		x = rect.x;
		y = rect.y;
	}
	else // default setting
	{
		x = 290;
		y = 221;
	}
	PI_Processor::Start();
	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();
	AttrTopmost(false);	

	m_QuestItemInfo = Info;
	m_pC_scroll_bar->SetPosMax(m_QuestItemInfo.size()/2+1);
}

void	C_VS_UI_QUEST_ITEM::Finish()
{
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_ITEM, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_ITEM, Rect(x, y, w, h));	
//	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::QUEST_MANAGER_ITEM, GetAttributes()->alpha);

	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);	
}

void	C_VS_UI_QUEST_ITEM::Show()
{
//	RECT rect[10];
//	int num[10];
//	int len = 0;
	if(gpC_base->m_p_DDSurface_back->Lock())
	{	
		gpC_global_resource->DrawDialogLocked4(x, y, w, h, GetAttributes()->alpha);
		m_pImage_Spk->BltLocked(x+115, y+7+m_OustersOffset, C_VS_UI_QUEST_MANAGER::QUEST_ITEM_TITLE);
		if(GetAttributes()->alpha)
		{
//			m_pImage_Spk->BltLockedAlpha(x+5, y+35+m_OustersOffset, INVENTORY_SLOT,16);
//			m_pImage_Spk->BltLockedAlpha(x+5, y+135+m_OustersOffset, INVENTORY_SLOT,32);
		}
		else
		{
			m_pImage_Spk->BltLocked(x+5, y+35+m_OustersOffset, C_VS_UI_QUEST_MANAGER::INVENTORY_SLOT);
			m_pImage_Spk->BltLocked(x+5, y+135+m_OustersOffset, C_VS_UI_QUEST_MANAGER::INVENTORY_SLOT);
		}
	//	m_pImage_Spk->BltLocked(x+5, y+210+m_OustersOffset, INVENTORY_SLOT);

		int ScrPos = m_pC_scroll_bar->GetScrollPos();
		for(int i =ScrPos*2; i< m_QuestItemInfo.size(); i++)
		{
			if(i>(ScrPos*2)+9)
				break;
			const MItem* p_item = m_QuestItemInfo[i];
			
			if (p_item != NULL)
			{
				// frame id -> sprite id
				TYPE_FRAMEID frame_id = p_item->GetInventoryFrameID();
				
				int item_x = x+5 + ((i-ScrPos*2)/2)*SLOT_WIDTH;
				
				// �ٴۿ� ���̰� �Ѵ�.
				int item_y = y+ 124 + ((i&0x01)*SLOT_HEIGHT) -gpC_item->GetHeight(frame_id)-4 + m_OustersOffset;
				
				// item�� center�� �����.
				item_x += SLOT_WIDTH/2-gpC_item->GetWidth(frame_id)/2;
				
				// identify�� �������� ���.. by sigi
				if (p_item->IsIdentified())
				{
					if(p_item->IsQuestItem())
						CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetQuestItemColorset(), 0);
					else
					if(p_item->IsUniqueItem())
						CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetUniqueItemColorset(), 0);
					else
						CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetItemOptionColorSet(), 0);
				}
				else
				{
					// identify �� �� ���� ������ ���
					CIndexSprite::SetUsingColorSet(378, 378);
				}
								
				if (m_SelectPos == i)
				{
					gpC_item->BltLockedOutline(item_x, item_y, WHITE, frame_id);
					if(p_item->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET && g_eRaceInterface == RACE_OUSTERS)
					{
						ITEMTABLE_INFO::ELEMENTAL_TYPE eType = (*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].ElementalType;
						if(eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE ||
							eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER ||
							eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH
							)
						{
							gpC_global_resource->m_pC_info_spk->BltLockedOutline(item_x, item_y+gpC_item->GetHeight(frame_id)/2-(p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)/2, RGB_WHITE, C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType);
						}
					}
				}
				else
				{
					if (p_item->IsAffectStatus())
					{
						// frame id -> sprite id
						gpC_item->BltLocked(item_x, item_y, frame_id);
						if(p_item->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET && g_eRaceInterface == RACE_OUSTERS)
						{
							ITEMTABLE_INFO::ELEMENTAL_TYPE eType = (*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].ElementalType;
							if(eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE ||
								eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER ||
								eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH
								)
							{
								gpC_global_resource->m_pC_info_spk->BltLocked(item_x, item_y+gpC_item->GetHeight(frame_id)/2-(p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)/2, C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType);
							}
						}
					}
					else
					{
						gpC_item->BltLockedColorSet(item_x, item_y, frame_id, ITEM_DISABLE_COLOR_SET);
						if(p_item->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET && g_eRaceInterface == RACE_OUSTERS)
						{
							ITEMTABLE_INFO::ELEMENTAL_TYPE eType = (*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].ElementalType;
							if(eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE ||
								eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER ||
								eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH
								)
							{
								gpC_global_resource->m_pC_info_spk->BltLockedColor(item_x, item_y+gpC_item->GetHeight(frame_id)/2-(p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y)/2, C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType, 0);
							}
						}
					}
				}

//				// ������ ����ǥ��
//				if(p_item->IsPileItem() || p_item->IsChargeItem())
//				{
//					RECT rt;
//					rt.right = item_x+SLOT_WIDTH;
//					for(int depth = 0, number = p_item->GetNumber(); number > 0; number/=10, depth++);
//					if(depth == 0) depth = 1;
//					rt.left = rt.right - 7*depth;
//					rt.bottom = item_y;
//					rt.top = rt.bottom - 12;
//					
//					rect[len] = rt;
//					rt.left = max(0, rt.left);
//					rt.top = max(0, rt.top);
//					
//					if(rt.left < rt.right && rt.top < rt.bottom)DrawAlphaBox(&rt, 0, 0, 0, g_pUserOption->ALPHA_DEPTH);
//					num[len] = p_item->GetNumber();
//					len++;
//				}

			}
		}
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}
		
	m_pC_scroll_bar->Show(x,y);
	
	if(g_FL2_GetDC())
	{
	
		m_pC_button_group->ShowDescription();
		g_FL2_ReleaseDC();
	}	
	m_bl_focus = false;
}

bool	C_VS_UI_QUEST_ITEM::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);

	if(NULL != m_pC_scroll_bar)
	re &= m_pC_scroll_bar->MouseControl(message, _x, _y);

	m_bl_focus = false;

	Rect rt(x, y, x+315, y+260);
	switch(message)
	{
	case M_MOVING:
		{
			Point pt(x+_x,y+_y);
			if( rt.IsInRect(pt) )
			{
				m_bl_focus = true;	
			}
			if(_x>5 && _x<305 && _y>36&& _y <226 && m_bl_focus)
			{

				int TempPosY = 0;
				if(_y>130)
					TempPosY = 1;
				int TempPosX = (_x-5)/60;

				int ScrPos = m_pC_scroll_bar->GetScrollPos();

				int TempPos = (TempPosX*2 + TempPosY)+ScrPos*2;
				
				if(TempPos < m_QuestItemInfo.size())
				{
					MItem * p_item = m_QuestItemInfo[TempPos];
					if(NULL != p_item)
					{
						m_SelectPos = TempPos;
						g_descriptor_manager.Set(DID_ITEM, x+TempPosX*SLOT_WIDTH, y+124+(TempPosY*SLOT_HEIGHT)-(p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y), (void *)p_item);
						
					}
				}
				else
					m_SelectPos = NOT_SELECTED;
//				if(gpC_Imm && m_focused_slot == NOT_SELECTED)
//						gpC_Imm->ForceUI(CImm::FORCE_UI_BUTTON);
			}
			else
				m_SelectPos = NOT_SELECTED;
//			for(int i =ScrPos*2; i< m_QuestItemInfo.size(); i++)
//			{
//				if (_x >= GetSlotX(i) && _x < GetSlotX(i)+SLOT_WIDTH &&
//					 _y >= GetSlotY(i) && _y < GetSlotY(i)+SLOT_HEIGHT)
//				{
//					if(gpC_Imm && m_focused_slot == NOT_SELECTED)
//						gpC_Imm->ForceUI(CImm::FORCE_UI_BUTTON);
//
//					m_focused_slot = i;
//
//					MItem * p_item = GetSlotItem(m_focused_slot);
//					if (p_item != NULL)
//					{
//						g_descriptor_manager.Set(DID_ITEM, x+GetSlotX(m_focused_slot), y+GetSlotY(m_focused_slot)+SLOT_HEIGHT-p_item->GetGridHeight()*C_VS_UI_INVENTORY::GRID_UNIT_PIXEL_Y, (void *)p_item);
//					}
//					return true;
////					break;
//				}
//			}
//			if( !m_bl_active ) break;
//			
//			Point pt(x+_x,y+_y);
//			if( rt.IsInRect(pt) && (m_quest_status.QuestID == 57 || m_quest_status.QuestID == 72 || m_quest_status.QuestID == 87))
//			{
//				m_bl_focus = true;				
//				g_descriptor_manager.Set(DID_INFO, _x+x,_y+y, (void *)(*g_pGameStringTable)[UI_STRING_MESSAGE_CLICK_TO_SHOW_DETIAL].GetString(),0,0);
//			}
//			ShowQuestDescription(_x,_y);
		}
		break;
	case M_WHEEL_UP:
		m_pC_scroll_bar->ScrollUp();
		break;
		
	case M_WHEEL_DOWN:
		m_pC_scroll_bar->ScrollDown();
		break;

	case M_RIGHTBUTTON_DOWN:
		{
			Point pt(x+_x,y+_y);
			if( rt.IsInRect(pt) )
			{
				m_bl_focus = true;	
			}
			if(_x>5 && _x<305 && _y>36&& _y <226 && m_bl_focus)
			{

				int TempPosY = 0;
				if(_y>130)
					TempPosY = 1;
				int TempPosX = (_x-5)/60;

				int ScrPos = m_pC_scroll_bar->GetScrollPos();

				int TempPos = (TempPosX*2 + TempPosY)+ScrPos*2;
				
				if(TempPos < m_QuestItemInfo.size())
				{
					MItem * p_item = m_QuestItemInfo[TempPos];
					
					if(NULL != p_item)
					{
						if(p_item->GetItemType() >= 13 && p_item->GetItemType() <= 20)
						{
							p_item->SetItemSlot(TempPos);
							gpC_base->SendMessage(UI_ITEM_USE_GQUEST_ITEM, TempPos, 0, (MItem *)p_item);
						}
					}
				}
				else
					m_SelectPos = NOT_SELECTED;
	//				if(gpC_Imm && m_focused_slot == NOT_SELECTED)
	//						gpC_Imm->ForceUI(CImm::FORCE_UI_BUTTON);
			}
			else
				m_SelectPos = NOT_SELECTED;
		}
		break;
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		{
			
//			Point pt(x+_x,y+_y);
//			if( rt.IsInRect(pt) )
//				m_bl_focus = true;
//
//			if( m_bl_focus )
//			{
//				Run(DETAIL_ID);
//			}
			if(gpC_mouse_pointer->IsCursorDescription())
			{
				Point pt(x+_x,y+_y);
				if( rt.IsInRect(pt) )
				{
					m_bl_focus = true;	
				}
				if(_x>5 && _x<305 && _y>36&& _y <226 && m_bl_focus)
				{

					int TempPosY = 0;
					if(_y>130)
						TempPosY = 1;
					int TempPosX = (_x-5)/60;

					int ScrPos = m_pC_scroll_bar->GetScrollPos();

					int TempPos = (TempPosX*2 + TempPosY)+ScrPos*2;
					
					if(TempPos < m_QuestItemInfo.size())
					{
						MItem * p_item = m_QuestItemInfo[TempPos];
						
						if(NULL != p_item)
						{
							gC_vs_ui.RunDescDialog(DID_ITEM, (void *)p_item);
						}
					}
					else
						m_SelectPos = NOT_SELECTED;
				}
				else
					m_SelectPos = NOT_SELECTED;

			}
			else if(re)
			{
				MoveReady();
				SetOrigin(_x, _y);
			}
			break;
		}
		break;
	}	
	return true;
}

void	C_VS_UI_QUEST_ITEM::KeyboardControl(UINT message, UINT key, long extra)	
{	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{		
		case VK_ESCAPE :
			gpC_base->SendMessage(UI_CLOSE_QUEST_ITEM);
			break;		
		}
	}
}

bool	C_VS_UI_QUEST_ITEM::IsPixel(int _x,int _y)
{
	return IsInRect(_x, _y);
}

void	C_VS_UI_QUEST_ITEM::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
//	if(p_button->GetID() == TAB1_ID || p_button->GetID() == TAB2_ID)
//		return;
	if(p_button->GetID() == ALPHA_ID)
	{
		if(GetAttributes()->alpha)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
	} else
	if(p_button->GetID() == PUSHPIN_ID)
	{
		if(GetAttributes()->autohide)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN_PUSHED);
		
	} else
	if(p_button->GetID() == CLOSE_ID)
	{
		gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x-5, y+p_button->y-5, C_GLOBAL_RESOURCE::BUTTON_CLOSE_BACK);
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+3);
			else
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
		}
		else
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
			else
				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
		}

	}
//	else
//	{
//		if(p_button->GetFocusState())
//		{
//			if(p_button->GetPressState())
//				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION_PUSHED);
//			else
//				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, C_GLOBAL_RESOURCE::AB_BUTTON_QUESTION_HILIGHTED);
//		}
//		else
//		{
//			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
//		}
//	}
}

void	C_VS_UI_QUEST_ITEM::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
	if( false == m_bl_focus )
		return;
	const static char* m_qQuest_Item_Desc[5]=
	{
//		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_HELP].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_OFF].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_ON].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW].GetString(),
	};
	switch(p_button->GetID())
	{
	case PUSHPIN_ID :
		if(GetAttributes()->autohide)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_qQuest_Item_Desc[1],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_qQuest_Item_Desc[2],0,0);
		break;
	case CLOSE_ID :
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_qQuest_Item_Desc[0],0,0);
		break;
	case ALPHA_ID :
		if(GetAttributes()->alpha)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_qQuest_Item_Desc[4],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_qQuest_Item_Desc[3],0,0);
		break;
	}	
}

void	C_VS_UI_QUEST_ITEM::Run(id_t id)
{
	switch (id)
	{
	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_QUEST_ITEM);
//		break;
//		
	case ALPHA_ID:
		AttrAlpha(!GetAttributes()->alpha);
		EMPTY_MOVE;
		break;
		
	case PUSHPIN_ID :
		if(GetAttributes()->autohide)
			AttrAutoHide(ATTRIBUTES_HIDE_NOT);
		else
		{
			AttrAutoHide(ATTRIBUTES_HIDE_WIDTH);
		}		
		break;

	}
}

void	C_VS_UI_QUEST_ITEM::Process()
{
	m_pC_button_group->Process();
	ProcessHide();
}

void	C_VS_UI_QUEST_ITEM::SetQuestItemInfo(std::vector<MItem*>& Info)
{
	m_QuestItemInfo = Info;
	m_pC_scroll_bar->SetPosMax(m_QuestItemInfo.size()/2+1);
}

//-----------------------------------------------------------------------------
// C_VS_UI_QUEST_ICON::C_VS_UI_QUEST_ICON
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_QUEST_ICON::C_VS_UI_QUEST_ICON(C_SPRITE_PACK* spr)
{	
	AttrTopmost(false);
	g_RegisterWindow(this);	
	AttrPin(true);	
	
	m_pImage_Spk = spr;

	int window_w = 24;
	int window_h = 17;
	
	Set(10, 80, window_w, window_h);
	m_StartPosX = -400;

	m_dwDelay = g_CurrentFrame;
}

C_VS_UI_QUEST_ICON::~C_VS_UI_QUEST_ICON()
{
	g_UnregisterWindow(this);
}

void	C_VS_UI_QUEST_ICON::Start()
{
	PI_Processor::Start();
	gpC_window_manager->AppearWindow(this);
	m_dwDelay = g_CurrentFrame;
	AttrTopmost(false);	
}

void	C_VS_UI_QUEST_ICON::Finish()
{
	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);	
}

void	C_VS_UI_QUEST_ICON::Show()
{
	if(gpC_base->m_p_DDSurface_back->Lock())
	{	
		if( g_CurrentFrame > m_dwDelay + 160)
		{
			if( m_StartPosX > - 400)
				m_StartPosX  -= 10;
			else
				gpC_base->SendMessage(UI_CLOSE_QUEST_ICON, 0);	
		}
		else
		{
			if( m_StartPosX >= 10)
				m_StartPosX  = 10;
			else
				m_StartPosX += 10;
		}
		
		int Temp = (g_CurrentFrame%8)/4;
		m_pImage_Spk->BltLocked(m_StartPosX/*-Temp*/, 80/*-Temp*/, Temp);
		
		gpC_base->m_p_DDSurface_back->Unlock();
		g_PrintColorStr( m_StartPosX + 30, 84, (*g_pGameStringTable)[UI_STRING_GQUEST_UPDATE].GetString(), gpC_base->m_chatting_pi, RGB_WHITE );
	}
	
}

bool	C_VS_UI_QUEST_ICON::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	switch(message)
	{
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		{
			gpC_base->SendMessage(UI_CLOSE_QUEST_ICON, 1);	
		}
		break;
	}	
	return true;
}

bool	C_VS_UI_QUEST_ICON::IsPixel(int _x,int _y)
{
	return IsInRect(_x, _y);
}



//-----------------------------------------------------------------------------
// C_VS_UI_RANGER_CHAT::C_VS_UI_RANGER_CHAT
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_RANGER_CHAT::C_VS_UI_RANGER_CHAT()
{	
	AttrTopmost(false);
	g_RegisterWindow(this);	
	AttrPin(true);	
	
	int window_w = 620;
	int window_h = 30;
	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		window_h = 28;
		m_Offset = 6;
		break;

	case RACE_VAMPIRE:
		window_h = 28;
		m_Offset = 6;
		break;

	case RACE_OUSTERS:
		m_Offset = 8;
		break;
	}

	
	m_bl_focus = false;
	Set(g_GameRect.right/2-window_w/2, g_GameRect.bottom/2 - window_h/2, window_w, window_h);

	int pin_x = 580, pin_y = h-22;
	int alpha_x = 600, alpha_y = h-22;
		
	//�����ư
	m_pC_button_group = new ButtonGroup(this);
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pin_x, pin_y, 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN), 
		PUSHPIN_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(alpha_x, alpha_y,
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA),
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA),
		ALPHA_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA));	

	
#ifndef _LIB
	m_Str = "a�����ٶ󸶹� �������īŸ���ϰ����ٶ󸶹ٻ������īŸ���ϰ����ٶ󸶹ٻ������īŸ���ϰ����ٶ󸶹ٻ��";
#endif
}

C_VS_UI_RANGER_CHAT::~C_VS_UI_RANGER_CHAT()
{
	g_UnregisterWindow(this);
	
	DeleteNew(m_pC_button_group);	
}

void	C_VS_UI_RANGER_CHAT::Start()
{
	x = 28;
	y = 450;
	PI_Processor::Start();
	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();
	AttrTopmost(false);	

}

void	C_VS_UI_RANGER_CHAT::Finish()
{
	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);	
}

void	C_VS_UI_RANGER_CHAT::Show()
{

	if(gpC_base->m_p_DDSurface_back->Lock())
	{	
		gpC_global_resource->DrawTempChatBoxLocked(x, y, w, h,GetAttributes()->alpha);
		//gpC_global_resource->DrawOutBoxLocked(x, y, w, h);
		
		m_pC_button_group->Show();
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	if(g_FL2_GetDC())
	{
		int TempX = g_PrintColorStr(x+4, y+m_Offset , (*g_pGameStringTable)[UI_STRING_MESSAGE_RANGER_SAY2].GetString(), gpC_base->m_user_id_pi, RGB_GRAY);
		
		char szString[128];
		strcpy(szString, m_Str.c_str());
		ReduceString3(szString,80);
		g_PrintColorStr(TempX + 4, y+m_Offset , szString, gpC_base->m_user_id_pi, RGB_GREEN);
		m_pC_button_group->ShowDescription();
		g_FL2_ReleaseDC();
	}	
	m_bl_focus = false;
}

bool	C_VS_UI_RANGER_CHAT::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);

	m_bl_focus = false;

	Rect rt(x, y, x+620, y+30);
	switch(message)
	{
	case M_MOVING:
		{
			Point pt(x+_x,y+_y);
			if( rt.IsInRect(pt) )
			{
				m_bl_focus = true;	
			}
		}
		break;

	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		{
			if(re && NULL == m_pC_button_group->IsInRect(_x, _y))
			{
				MoveReady();
				SetOrigin(_x, _y);
			}
			break;
		}
		break;
	}	
	return true;
}

void	C_VS_UI_RANGER_CHAT::KeyboardControl(UINT message, UINT key, long extra)	
{	

}

bool	C_VS_UI_RANGER_CHAT::IsPixel(int _x,int _y)
{
	return IsInRect(_x, _y);
}

void	C_VS_UI_RANGER_CHAT::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetID() == ALPHA_ID)
	{
		if(GetAttributes()->alpha)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
	} else
	if(p_button->GetID() == PUSHPIN_ID)
	{
		if(GetAttributes()->autohide)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_PUSHPIN_PUSHED);
		
	} 
}

void	C_VS_UI_RANGER_CHAT::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
	if(m_bl_focus == false)
		return ;
	const static char* m_gQuest_Mission_desc[4]=
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_OFF].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_AUTO_HIDE_ON].GetString(),
	};
	switch(p_button->GetID())
	{
	case PUSHPIN_ID :
		if(GetAttributes()->autohide)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_gQuest_Mission_desc[2],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_gQuest_Mission_desc[3],0,0);
		break;
	case ALPHA_ID :
		if(GetAttributes()->alpha)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_gQuest_Mission_desc[1],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_gQuest_Mission_desc[0],0,0);
		break;
	}	
}

void	C_VS_UI_RANGER_CHAT::Run(id_t id)
{
	switch (id)
	{
	case ALPHA_ID:
		AttrAlpha(!GetAttributes()->alpha);
		EMPTY_MOVE;
		break;
		
	case PUSHPIN_ID :
		if(GetAttributes()->autohide)
			AttrAutoHide(ATTRIBUTES_HIDE_NOT);
		else
		{
			AttrAutoHide(ATTRIBUTES_HIDE_WIDTH);
		}		
		break;

	}
}

void	C_VS_UI_RANGER_CHAT::Process()
{
	m_pC_button_group->Process();
	ProcessHide();
}


//------------------------------------------------------------------------------
// C_VS_UI_PERSNALSHOP_MESSAGE
//
// ���λ����� ���� �޼��� 
//------------------------------------------------------------------------------
C_VS_UI_PERSNALSHOP_MESSAGE::C_VS_UI_PERSNALSHOP_MESSAGE()
{	
	
	Set(g_GameRect.right/2 - 340/2, g_GameRect.bottom/2 - 267/2, 340-80, 267-50);

	int ok_x = 160,ok_y = h-44;
	int cancel_x=236,cancel_y=h-44;


	m_pC_button_group = new ButtonGroup(this);
	

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(ok_x-70,ok_y,
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_OK),
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_OK),
		OK_ID,this,C_GLOBAL_RESOURCE::AB_BUTTON_OK));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(cancel_x-70,cancel_y,
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL),
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL),
		CANCEL_ID,this,C_GLOBAL_RESOURCE::AB_BUTTON_CANCEL));

	g_RegisterWindow(this);
	AttrTopmost(true);
	AttrKeyboardControl(true);
	


	m_lev_content.SetPosition(x,y);
	m_lev_content.SetPrintInfo(gpC_base->m_chatting_pi);
	m_lev_content.SetByteLimit(80);
	m_lev_content.SetEditorMode(20, 4);
	m_lev_content.SetAbsWidth(125);
	
	m_lev_content.SetInputStringColor(RGB_WHITE);
	m_lev_content.SetCursorColor(RGB_WHITE);
	Attach(&m_lev_content);	

//	m_lev_content = "";
}

C_VS_UI_PERSNALSHOP_MESSAGE::~C_VS_UI_PERSNALSHOP_MESSAGE()
{
	g_UnregisterWindow(this);
	DeleteNew(m_pC_button_group);
//	gC_vs_ui.SetPersnalShoptoUI(2);
}

void	C_VS_UI_PERSNALSHOP_MESSAGE::Start()
{
	PI_Processor::Start();
	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();
	AttrTopmost(true);
	m_lev_content.Acquire();
	m_lev_content.EraseAll();
}

void	C_VS_UI_PERSNALSHOP_MESSAGE::Finish()
{
	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);

	if(m_lev_content.IsAcquire())			
		m_lev_content.Unacquire();
	gC_vs_ui.AcquireChatting();
}

void	C_VS_UI_PERSNALSHOP_MESSAGE::Show()
{
	int box_x = 160+x, box_y = 189+y;
	int name_x =190+x, name_y = 193+y;

	RECT rect = { x+50, y+90, x+ 220, y+200 };

	
	if(gpC_base->m_p_DDSurface_back->Lock())
	{
		gpC_global_resource->DrawDialogLocked(x,y,w,h,g_pUserOption->DefaultAlpha == TRUE);
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+30, y+70,C_GLOBAL_RESOURCE::AB_MONEY_BAR);		
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+30, y+90,C_GLOBAL_RESOURCE::AB_MONEY_BAR);		
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+30, y+110,C_GLOBAL_RESOURCE::AB_MONEY_BAR);		
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+30, y+130,C_GLOBAL_RESOURCE::AB_MONEY_BAR);		
		
		m_pC_button_group->Show();	
		
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	
	if(g_FL2_GetDC())
	{		
		g_PrintColorStr(x+30, y+30, (*g_pGameStringTable)[UI_STRING_MESSAGE_PERSNALSHOP_MESSAGE].GetString(), gpC_base->m_chatting_pi, RGB_WHITE);
		
		std::string str;


		int next=0;
		char sz_string[512];

		int print_x=30+x,vx;
		int py = 40+y;
		const int print_gap = 20;
		const int char_width = g_GetStringWidth("a", gpC_base->m_chatting_pi.hfont);

		vx = print_x;

		while(str.size() > next)
		{
			strcpy(sz_string, str.c_str()+next);
			
			char *sz_string2 = sz_string;
			
			while(*sz_string2 == ' ')		// ���� ��������
			{
				sz_string2++;
				next++;
			}
			
			int cut_pos = (x+w-30 -vx)/char_width;
			
			if(!g_PossibleStringCut(sz_string2, cut_pos))
				cut_pos--;
			sz_string2[cut_pos] = NULL;
			
			char *return_char = NULL;
			if((return_char = strchr(sz_string2, '\n')) != NULL)	// return ó��
			{
				cut_pos = return_char - sz_string2+1;
				sz_string2[cut_pos-1] = NULL;
			}
						
			g_PrintColorStr(vx, py, sz_string2, gpC_base->m_chatting_pi, RGB_WHITE);
			next += cut_pos;
			vx = print_x;
			py += print_gap;
		}
		g_FL2_ReleaseDC();
	}

	m_lev_content.SetPosition(x+33,y+72);
	m_lev_content.Show();


	m_pC_button_group->ShowDescription();

	SHOW_WINDOW_ATTR;
}

bool	C_VS_UI_PERSNALSHOP_MESSAGE::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;
	m_pC_button_group->MouseControl(message, _x, _y);

		if(!m_lev_content.IsAcquire())
			m_lev_content.Acquire();

		return true;
}

void	C_VS_UI_PERSNALSHOP_MESSAGE::KeyboardControl(UINT message, UINT key, long extra)	
{
	char ignore_char[] = {'\"', '\'', '/',' '};
	
	for (int i=0; i<sizeof(ignore_char); i++)
		if ((char)key == ignore_char[i] || (char)key == 13)
 			return;

	Window::KeyboardControl(message, key, extra);
	
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
		case VK_RETURN :
			{			
				char *str=NULL;
				g_Convert_DBCS_Ascii2SingleByte(m_lev_content.GetStringWide(),m_lev_content.Size(),str);
//				if(str != NULL && m_Status == INPUT_STATUS_NORMAL)
//				{
//					m_name = str;
//					gpC_base->SendMessage(UI_SEND_NAME_FOR_COUPLE,(int) m_Mode,0,(void*)m_name.c_str());
//				}
//				DeleteNew(str);
			}
			break;
		case VK_ESCAPE :
			//gpC_base->SendMessage(UI_CLOSE_INPUT_NAME);
			break;		
		}
	}
}

bool	C_VS_UI_PERSNALSHOP_MESSAGE::IsPixel(int _x,int _y)
{
	return IsInRect(_x, _y);
}

void	C_VS_UI_PERSNALSHOP_MESSAGE::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetID() == OK_ID || p_button->GetID() == CANCEL_ID)
	{
//		if(m_Status == INPUT_STATUS_NORMAL)
//		{
			if(p_button->GetFocusState())
			{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
			else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
			} else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index);
//		} 
//		else
//		{
//			gpC_global_resource->m_pC_assemble_box_button_spk->BltLockedDarkness(x+p_button->x,y+p_button->y,p_button->m_image_index,1);
//		}
	} else
	if(p_button->GetFocusState())
	{
		if(p_button->GetPressState())
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
	} else
		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x,y+p_button->y,p_button->m_image_index);
}

void	C_VS_UI_PERSNALSHOP_MESSAGE::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
	char* m_bring_fee_msg[2] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_COMPLETE].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CANCEL].GetString(),
	};
	
	switch(p_button->GetID())
	{
	case OK_ID :
		g_descriptor_manager.Set(DID_INFO,p_button->x+x,p_button->y+y,
			(void *)m_bring_fee_msg[0],0,0);		
		break;
	case CANCEL_ID :
		g_descriptor_manager.Set(DID_INFO,p_button->x+x,p_button->y+y,
			(void *)m_bring_fee_msg[1],0,0);		
		break;
	}
}

void	C_VS_UI_PERSNALSHOP_MESSAGE::WindowEventReceiver(id_t event)
{
}

void	C_VS_UI_PERSNALSHOP_MESSAGE::Run(id_t id)
{
	switch(id)
	{
	case OK_ID :
		{			
			char *str=NULL;
			g_Convert_DBCS_Ascii2SingleByte(m_lev_content.GetStringWide(),m_lev_content.Size(),str);
			
			if(str != NULL)
			{
				m_content = str;
				gpC_base->SendMessage(UI_STORE_SIGN,0,0,(void*)(m_content.c_str()));
			}
			DeleteNew(str);
			
		}
		break;
	case CANCEL_ID :
		gC_vs_ui.ClosePersnalShopMessage();
		break;		
	}
	gC_vs_ui.AcquireChatting();
}

void	C_VS_UI_PERSNALSHOP_MESSAGE::Process()
{
	m_pC_button_group->Process();
	
//	if(m_Status != INPUT_STATUS_NORMAL )
//		m_lev_content.Unacquire();
//	else
//	{
		if(!m_lev_content.IsAcquire())
			m_lev_content.Acquire();
//	}

}

//const char* C_VS_UI_PERSNALSHOP_MESSAGE::GetCurrentContent()
//{
//	char *str=NULL;
//	g_Convert_DBCS_Ascii2SingleByte(m_lev_content.GetStringWide(),m_lev_content.Size(),str);
//	if(str != NULL)
//		m_lev_content = str;
//	else
//		return NULL;
//	 
//	DeleteNew(str);
//
//	return m_content.c_str();
//}



//------------------------------------------------------------------------------
// C_VS_UI_POWER_JJANG
//
// �Ŀ�¯ -_-
//------------------------------------------------------------------------------
C_VS_UI_POWER_JJANG::C_VS_UI_POWER_JJANG()
{	
	
	g_RegisterWindow(this);	
	AttrPin(true);	
	
	int window_w = 250;
	int window_h = 355;

	m_SelectPos = -1;
	m_GambleMode = 0;
	
	Set(g_GameRect.right/2-window_w/2, g_GameRect.bottom/2 - window_h/2, window_w, window_h);

	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW));
	AttrAutoHide(gpC_vs_ui_window_manager->GetAutoHide(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW));
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW);
	if(rect.w != -1)
	{ 
		x = rect.x;
		y = rect.y;
	}

	m_pC_item_spk.Open(SPK_ITEM_POWERJJANG);
	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		m_pC_spk.Open(SPK_SLAYER_POWERJJANG);
		break;

	case RACE_VAMPIRE:
		m_pC_spk.Open(SPK_VAMPIRE_POWERJJANG);
		break;

	case RACE_OUSTERS:
		m_pC_spk.Open(SPK_OUSTERS_POWERJJANG);
		break;		
	}

	m_pC_scroll_bar = new C_VS_UI_SCROLL_BAR(0, Rect(w-35,215,-1,window_h-280));
	m_pC_scroll_bar->SetPosMax(0);

	int close_x = w-110, close_y = h-45;		
	int select_x = w-190, select_y = h-45;		
	int pin_x = w-24-20, pin_y = h-19;
	int alpha_x = 6, alpha_y = h-21;
	int phone_x = w - 85, phone_y = h - 267;
	//�����ư
	m_pC_button_group = new ButtonGroup(this);
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_x, close_y, 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE), 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE),
		CLOSE_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE));
	
	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(select_x, select_y, 
		m_pC_spk.GetWidth(JJANG_BUTTON_EXCHANGE), 
		m_pC_spk.GetHeight(JJANG_BUTTON_EXCHANGE),
		EXCHANGE_ID, this, JJANG_BUTTON_EXCHANGE));

//	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(pin_x, pin_y, 
//		gpC_global_resource->m_pC_info_spk->GetWidth(C_GLOBAL_RESOURCE::BUTTON_HELP_BACK), 
//		gpC_global_resource->m_pC_info_spk->GetHeight(C_GLOBAL_RESOURCE::BUTTON_HELP_BACK), 
//		HELP_ID, this, C_GLOBAL_RESOURCE::BUTTON_HELP_BACK));

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(alpha_x, alpha_y,
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA),
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA),
		ALPHA_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA));	

	m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(phone_x, phone_y, 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_OK), 
		gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_OK), 
		PHONENUM_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_OK));
	
}

C_VS_UI_POWER_JJANG::~C_VS_UI_POWER_JJANG()
{
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW, Rect(x, y, w, h));	
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW, GetAttributes()->alpha);

	g_UnregisterWindow(this);
	
//	m_AddName.Unacquire();
//	gC_vs_ui.AcquireChatting();
	DeleteNew(m_pC_scroll_bar);
	DeleteNew(m_pC_button_group);	
	ReleaseItemList();

	gC_vs_ui.AcquireChatting();
}

void	C_VS_UI_POWER_JJANG::Start()
{
	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW));
	AttrAutoHide(gpC_vs_ui_window_manager->GetAutoHide(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW));
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW);
	if(rect.w != -1)
	{ 
		x = rect.x;
		y = rect.y;
	}
	PI_Processor::Start();
//	AttrTopmost(true);	
	gpC_window_manager->AppearWindow(this);
	m_pC_button_group->Init();
//	m_AddName.Acquire();

	m_EditPhoneNumber.SetPosition(x+80, y+99);
	m_EditPhoneNumber.SetPrintInfo(gpC_base->m_chatting_pi);
	m_EditPhoneNumber.SetByteLimit(11);
	m_EditPhoneNumber.SetInputStringColor(RGB_WHITE);
	m_EditPhoneNumber.SetCursorColor(RGB_WHITE);
	m_EditPhoneNumber.SetDigitOnlyMode(true);
	Attach(&m_EditPhoneNumber);
	m_EditPhoneNumber.Acquire();

	SetItemList();
	
	g_pUserOption->UseEnterChat = true;
}

void	C_VS_UI_POWER_JJANG::Finish()
{
	gpC_vs_ui_window_manager->SetAutoHide(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW, GetAttributes()->autohide);
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW, Rect(x, y, w, h));	
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::NAMING_WINDOW, GetAttributes()->alpha);

	PI_Processor::Finish();
	gpC_window_manager->DisappearWindow(this);	
}


void	C_VS_UI_POWER_JJANG::Show()
{
	static DWORD SECOND_PET_QUEST = 0xffff;
	const int tab_x = 0;
	const int tab_y = 60;
	int ScrPos = m_pC_scroll_bar->GetScrollPos();


	gpC_global_resource->DrawDialog(x, y, w, h, GetAttributes()->alpha);
	
	RECT rect2 = { x+20, y+190, x+215, y+310 }; // ������ ĭ
	Rect rt2(rect2.left, rect2.top, rect2.right - rect2.left, rect2.bottom - rect2.top );
	//gpC_base->m_p_DDSurface_back->FillRect(&rect2, 0);
	gpC_global_resource->DrawOutBox(rt2);
	
	SetRect(&rect2, x+70, y+90, x+158, y+118 ); // �ڵ��� ĭ
	rt2.Set(rect2.left, rect2.top, rect2.right - rect2.left, rect2.bottom - rect2.top );
//	RECT rect2 = { x+70, y+90, x+158, y+118 }; // �ڵ��� ĭ
//	Rect rt2(rect2.left, rect2.top, rect2.right - rect2.left, rect2.bottom - rect2.top );
	gpC_base->m_p_DDSurface_back->FillRect(&rect2, 0);
	gpC_global_resource->DrawOutBox(rt2);

	SetRect(&rect2, x+20, y+65, x+230, y+125 ); //���� �ܰ�
	rt2.Set(rect2.left, rect2.top, rect2.right - rect2.left, rect2.bottom - rect2.top );
	gpC_global_resource->DrawOutBox(rt2);

	SetRect(&rect2, x+168, y+128, x+222, y+152 ); // ���� ����Ʈ ĭ
	rt2.Set(rect2.left, rect2.top, rect2.right - rect2.left, rect2.bottom - rect2.top );
	gpC_base->m_p_DDSurface_back->FillRect(&rect2, 0);
	gpC_global_resource->DrawOutBox(rt2);

	SetRect(&rect2, x+20, y+125, x+230, y+155 ); // ���� ����Ʈ �ܰ�
	rt2.Set(rect2.left, rect2.top, rect2.right - rect2.left, rect2.bottom - rect2.top );
	gpC_global_resource->DrawOutBox(rt2);

	SetRect(&rect2, x+168, y+158, x+222, y+182 ); // �׺� ����  ����Ʈ ĭ
	rt2.Set(rect2.left, rect2.top, rect2.right - rect2.left, rect2.bottom - rect2.top );
	gpC_base->m_p_DDSurface_back->FillRect(&rect2, 0);
	gpC_global_resource->DrawOutBox(rt2);

	SetRect(&rect2, x+20, y+155, x+230, y+185 ); // �׺� ���� ����Ʈ �ܰ�
	rt2.Set(rect2.left, rect2.top, rect2.right - rect2.left, rect2.bottom - rect2.top );
	gpC_global_resource->DrawOutBox(rt2);
	
	if ( gpC_base->m_p_DDSurface_back->Lock() )
	{
		m_pC_spk.BltLocked(x+ 38, y+ 20, JJANG_MAIN);
		for(int i =ScrPos; i< m_Powerjjang_ItemList.size(); i++)
		{
			if(i>ScrPos+2)
				break;
			if(NULL != m_Powerjjang_ItemList[i])
			{
				if(!m_GambleMode)
					m_pC_item_spk.BltLocked(x+30, y+198 + (i-ScrPos)*35, m_Powerjjang_ItemList[i]->bItemImageNum);
				else
				{
					int TempImage = (i+m_GambleMode)%m_Powerjjang_ItemList.size();
					m_pC_item_spk.BltLocked(x+30, y+198 + (i-ScrPos)*35, m_Powerjjang_ItemList[TempImage]->bItemImageNum/*rand()%m_pC_item_spk.GetSize()*/);
				}
			}
		}

		
		m_pC_button_group->Show();
		
		gpC_base->m_p_DDSurface_back->Unlock();
	}

	if(m_GambleMode)
	{
		DWORD TempTime = timeGetTime();
		if(m_dwCurrentTime + m_dwSpeed < TempTime)
		{
			m_GambleMode --;
			m_dwCurrentTime = TempTime;
			m_dwSpeed += 2;
		}
		if(m_dwOutCurrentTime + 200 < TempTime)
		{
			m_dwOutCurrentTime = TempTime;
			if(0 == m_OutLinePositon) m_OutLinePositon = 0xff;
			m_OutLinePositon--;
		}
		BYTE TempPos = m_OutLinePositon%3;

		SetRect(&rect2, x+29, y+197 + (TempPos)*35, x+205, y+230 + (TempPos)*35 ); // �׺� ���� ����Ʈ �ܰ�
		gpC_base->m_p_DDSurface_back->DrawRect(&rect2, YELLOW);
	}
	else if(m_SelectPos>=ScrPos && m_SelectPos <= ScrPos+2)
	{
		int TempPos = m_SelectPos - ScrPos;/*min(2, max(0, (m_SelectPos - ScrPos)));*/
		SetRect(&rect2, x+29, y+197 + (TempPos)*35, x+205, y+230 + (TempPos)*35 ); // �׺� ���� ����Ʈ �ܰ�
		gpC_base->m_p_DDSurface_back->DrawRect(&rect2, YELLOW);
	}


	m_pC_scroll_bar->Show(x,y);

	if(g_FL2_GetDC())
	{
		char szString[64];
		
		g_PrintColorStr(x+50, y+72 , (*g_pGameStringTable)[UI_STRING_POWER_JJANG_GET_POINT].GetString(), gpC_base->m_user_id_pi, RGB_WHITE);
		g_PrintColorStr(x+30, y+135 , (*g_pGameStringTable)[UI_STRING_POWER_JJANG_POINT].GetString(), gpC_base->m_user_id_pi, RGB_WHITE);
		g_PrintColorStr(x+30, y+165 , (*g_pGameStringTable)[UI_STRING_POWER_JJANG_AVAILABLE].GetString(), gpC_base->m_user_id_pi, RGB_WHITE);
		g_PrintColorStr(x+26, y+92 , (*g_pGameStringTable)[UI_STRING_POWER_JJANG_NUMBER_1].GetString(), gpC_base->m_chatting_pi, RGB_GRAY);
		g_PrintColorStr(x+30, y+107 , (*g_pGameStringTable)[UI_STRING_POWER_JJANG_NUMBER_2].GetString(), gpC_base->m_chatting_pi, RGB_GRAY);
	
		wsprintf(szString, "%d", g_char_slot_ingame.m_Powerjjang_Point);
		g_PrintColorStr(x+175, y+132 , szString, gpC_base->m_money_pi, RGB_WHITE);

		wsprintf(szString, "%d", m_AvailablePoint);
		g_PrintColorStr(x+175, y+162 , szString, gpC_base->m_money_pi, RGB_WHITE);

		m_pC_button_group->ShowDescription();
		g_FL2_ReleaseDC();
	}	

	m_EditPhoneNumber.SetPosition(x+80, y+99);
	m_EditPhoneNumber.Show();
	
	SHOW_WINDOW_ATTR;
}


bool	C_VS_UI_POWER_JJANG::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);

	_x-=x; _y-=y;
	bool re = m_pC_button_group->MouseControl(message, _x, _y);

	if(NULL != m_pC_scroll_bar && !m_GambleMode)
	re &= m_pC_scroll_bar->MouseControl(message, _x, _y);
//	

	switch(message)
	{
	case M_MOVING:
		{
			if(_x>20 && _x<215 && _y>195&& _y <300 && !m_GambleMode)
			{
				int SelectPos = (_y-195 )/35;
				SelectPos += m_pC_scroll_bar->GetScrollPos();
				if(SelectPos>=m_Powerjjang_ItemList.size())
				{
					SelectPos = /*m_Powerjjang_ItemList.size()*/-1;
				}
				else
				{
					if(NULL != m_Powerjjang_ItemList[SelectPos])
					{
						MItem* TempItem = (MItem*)(m_Powerjjang_ItemList[SelectPos]->pItem);
						if(NULL != TempItem)
							g_descriptor_manager.Set(DID_ITEM, x+23, y+198 +((SelectPos-m_pC_scroll_bar->GetScrollPos())*35), (void *)TempItem);

					}
				}
			}
		}
		break;
	case M_WHEEL_UP:
		if(!m_GambleMode)
			m_pC_scroll_bar->ScrollUp();
		break;
		
	case M_WHEEL_DOWN:
		if(!m_GambleMode)
			m_pC_scroll_bar->ScrollDown();
		break;

	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:
		{
			if(_x>20 && _x<215 && _y>190&& _y <310)
			{
//				m_SelectPos = (_y-74 )/17;
//				m_SelectPos += m_pC_scroll_bar->GetScrollPos();
//				if(m_SelectPos>=m_NameList.size())
//					m_SelectPos = m_NameList.size()-1;
			}
			else if(re && NULL == m_pC_button_group->IsInRect(_x, _y))
			{
				MoveReady();
				SetOrigin(_x, _y);
			}
//			if(_x>20 && _x<160 && _y>40&& _y <65)
//			{
//				m_AddName.Acquire();
//			}
			break;
		}
		break;
	}	
	//g_descriptor_manager.Unset();
	return true;
}

void	C_VS_UI_POWER_JJANG::KeyboardControl(UINT message, UINT key, long extra)	
{	
//	Window::KeyboardControl(message, key, extra);
//	if (message == WM_KEYDOWN)
//	{
//		switch (key)
//		{		
//		case VK_ESCAPE :
//			gpC_base->SendMessage(UI_CLOSE_NAMING);
//			break;		
//		}
//	}
}

bool	C_VS_UI_POWER_JJANG::IsPixel(int _x,int _y)
{
	return IsInRect(_x, _y);
}

void	C_VS_UI_POWER_JJANG::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{
	if(p_button->GetID() == ALPHA_ID)
	{
		if(GetAttributes()->alpha)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
	} else
//	if(p_button->GetID() == HELP_ID)
//	{
//		gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x-5, y+p_button->y-5, C_GLOBAL_RESOURCE::BUTTON_HELP_BACK);
//		if(p_button->GetFocusState())
//		{
//			if(p_button->GetPressState())
//				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
//			else
//				gpC_global_resource->m_pC_info_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+3);
//		}
//	} 
//	else
//	if(p_button->GetID() == CLOSE_ID)
//	{
//
//			gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE),
//
//		gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x-5, y+p_button->y-5, C_GLOBAL_RESOURCE::AB_BUTTON_CLOSE);
//		if(p_button->GetFocusState())
//		{
//			if(p_button->GetPressState())
//				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+3);
//			else
//				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+2);
//		}
//		else
//		{
//			if(p_button->GetPressState())
//				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index+1);
//			else
//				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
//		}
//
//	}
//	else 
	if(p_button->GetID() == PHONENUM_ID || p_button->GetID() == CLOSE_ID)
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y,  p_button->m_image_index+2);
			else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y,  p_button->m_image_index+1);
		}
		else
		{
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
		}
	}
	else
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				m_pC_spk.BltLocked(x+p_button->x, y+p_button->y,  p_button->m_image_index+2);
			else
				m_pC_spk.BltLocked(x+p_button->x, y+p_button->y,  p_button->m_image_index+1);
		}
		else
		{
			m_pC_spk.BltLocked(x+p_button->x, y+p_button->y, p_button->m_image_index);
		}
	}
}

void	C_VS_UI_POWER_JJANG::ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button)
{
	const static char* m_other_info_button_string[5]=
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_POWER_JJANG_GET_POINT_HELP].GetString(),//] = "�Ŀ�¯ ����Ʈ�� �����ɴϴ�.";
		(*g_pGameStringTable)[UI_STRING_POWER_JJANG_EXCHANGE_HELP].GetString(),//] = "�Ŀ�¯ ����Ʈ�� �������� ��ȯ�մϴ�.";
	};
	switch(p_button->GetID())
	{
	case CLOSE_ID :
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[0],0,0);
		break;
	case EXCHANGE_ID:
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[4],0,0);
		break;
	case PHONENUM_ID:
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[3],0,0);
		break;
	case ALPHA_ID:
		if(GetAttributes()->alpha)
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[2],0,0);
		else
			g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_other_info_button_string[1],0,0);
		break;
	}	
	
}


void	C_VS_UI_POWER_JJANG::Run(id_t id)
{
	switch (id)
	{
	case CLOSE_ID:
		gpC_base->SendMessage(UI_CLOSE_POWER_JJANG);
		break;
	case EXCHANGE_ID:
		if(g_char_slot_ingame.m_Powerjjang_Point>=m_AvailablePoint)
		{
			gpC_base->SendMessage(UI_POWER_JJANG_GAMBLE);

			m_GambleMode = 40;
			m_dwSpeed = 20;
			m_dwOutCurrentTime = m_dwCurrentTime = timeGetTime();
			m_OutLinePositon = 0xff;
		}
		else
			gpC_base->SendMessage(UI_MESSAGE_BOX, UI_STRING_POWER_JJANG_ERROR_NO_POINT, 0, 	NULL);
		break;
	case PHONENUM_ID:
		{
			if(!m_GambleMode)
			{
				char* TempBuffer = NULL;
				g_Convert_DBCS_Ascii2SingleByte(m_EditPhoneNumber.GetStringWide(), m_EditPhoneNumber.Size(), TempBuffer);
				if(NULL != TempBuffer)
				{
					m_szMyPhoneNumber = TempBuffer;
					gpC_base->SendMessage(UI_REQUEST_POWER_JJANG_POINT, 0, 0, (void*)m_szMyPhoneNumber.c_str());
				}
			}
		}
		break;
	case ALPHA_ID:
		AttrAlpha(!GetAttributes()->alpha);
	
		break;
	case HELP_ID:
		gC_vs_ui.RunDescDialog(DID_HELP, (void *)C_VS_UI_DESC_DIALOG::NICK_NAME_WINDOW);
		m_pC_button_group->UnacquireMouseFocus();
		break;
		
	}
}

void	C_VS_UI_POWER_JJANG::Process()
{
	m_pC_button_group->Process();

}
void	C_VS_UI_POWER_JJANG::ReleaseItemList()
{
	if(m_Powerjjang_ItemList.size())
	{
		std::vector<POWER_JJANG_ITEM*>::iterator itr = m_Powerjjang_ItemList.begin();
		while(itr != m_Powerjjang_ItemList.end())
		{
			POWER_JJANG_ITEM* TempItem = (*itr);
			DeleteNew(TempItem->pItem);
			DeleteNew(TempItem);
			itr++;
		}
	}
	m_Powerjjang_ItemList.clear();
}
//
void	C_VS_UI_POWER_JJANG::SetItemList()
{
	ReleaseItemList();

	// ����..Ŭ���� ����� ����Ÿ..
	BYTE MaxItem = 0;
	std::string convertedPath = ConvertGamePath("data\\info\\PowerjjangItem.inf");
	ifstream file(convertedPath.c_str(), ios::binary);
	file.read((char*)&MaxItem, 1);
	file.read((char*)&m_AvailablePoint, 2);
	for(int i = 0; i < MaxItem; i++)
	{
		POWER_JJANG_ITEM *TempItem = new POWER_JJANG_ITEM;
		file.read((char*)TempItem, 3);
		MItem * p_item = MItem::NewItem((enum ITEM_CLASS) TempItem->bItemClass);
		p_item->SetItemType( TempItem->bItemType );
		p_item->SetCurrentDurability(p_item->GetMaxDurability());
		TempItem->pItem = p_item;
		m_Powerjjang_ItemList.push_back(TempItem);
	}
	file.close();
	m_pC_scroll_bar->SetPosMax(max(0, m_Powerjjang_ItemList.size()-2));
}

void	C_VS_UI_POWER_JJANG::PowerjjangGambleResult(BYTE bItemCode)
{
//	m_GambleMode = 0;
	m_SelectPos = bItemCode;

	m_pC_scroll_bar->SetPosMax(max(0, m_Powerjjang_ItemList.size()-2));

	int ScrPos = m_pC_scroll_bar->GetScrollPos();
	if(m_SelectPos != ScrPos)
	{
		for(int i = 0; i<m_Powerjjang_ItemList.size()-1; i++)
		{
			m_pC_scroll_bar->ScrollDown();
			ScrPos = m_pC_scroll_bar->GetScrollPos();
			if(ScrPos == m_SelectPos)
			{
				m_pC_scroll_bar->ScrollUp();
				return;
			}
		}
	}
}

#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 ���Ӱ��а�
//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY_SUB::C_VS_UI_INVENTORY_SUB
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_INVENTORY_SUB::C_VS_UI_INVENTORY_SUB(MItem* pItem)
{
	assert(pItem);

	g_RegisterWindow(this);
	
	m_SubInventoryType = 0;
	
	m_MainWindowSpk = 0;
	m_pC_inventory_spk = NULL;

	int	close_button_offset_x, close_button_offset_y;
	int alpha_button_offset_x, alpha_button_offset_y;
	
	m_pC_button_group = new ButtonGroup(this);
	
	m_MultiPackItem = (MSubInventory*)pItem;

	if(m_MultiPackItem->GetItemType() == 0)
	{
		n_nType			= TYPE_2X4;
	}
	else if (m_MultiPackItem->GetItemType() == 1)
	{
		n_nType			= TYPE_4X6;
	}
	
	switch(n_nType)
	{
	case TYPE_2X4:
		{
			m_MainWindowSpk = INVENTORY_WINDOW_2X4;
		
			m_GridX = 2;
			m_GridY = 4;
	
			m_bl_Alpha = new BYTE* [m_GridX];

			for (int i=0; i<m_GridX; i++)
			{
				m_bl_Alpha[i] = new BYTE [m_GridY];
			}
		}
		break;
	case TYPE_4X6:
		{
			m_MainWindowSpk = INVENTORY_WINDOW_4X6;
		
			m_GridX = 4;
			m_GridY = 6;
	
			m_bl_Alpha = new BYTE* [m_GridX];

			for (int i=0; i<m_GridX; i++)
			{
				m_bl_Alpha[i] = new BYTE [m_GridY];
			}
		}
		break ; 
	default:
		{
			m_MainWindowSpk = INVENTORY_WINDOW_2X4;

			m_GridX = 2;
			m_GridY = 4;

			m_bl_Alpha = new BYTE* [m_GridX];
			
			for (int i=0; i<m_GridX; i++)
			{
				m_bl_Alpha[i] = new BYTE [m_GridY];
			}
		}
		break;
	}
	
	switch(g_eRaceInterface)
	{
	case RACE_SLAYER:
		m_pC_inventory_spk = new C_SPRITE_PACK(SPK_SLAYER_INVENTORY_SUB);

		m_grid_start_offset_x = 13;
		m_grid_start_offset_y = 25;
		close_button_offset_x = 55;
		close_button_offset_y = 155;
		alpha_button_offset_x = 10;
		alpha_button_offset_y = 170;
		
		Set(10, 128, m_pC_inventory_spk->GetWidth(m_MainWindowSpk), m_pC_inventory_spk->GetHeight(m_MainWindowSpk));
		break;

	case RACE_VAMPIRE:
		m_pC_inventory_spk = new C_SPRITE_PACK(SPK_VAMPIRE_INVENTORY_SUB);
		
		m_grid_start_offset_x = 14;
		m_grid_start_offset_y = 16;
		close_button_offset_x = 53;
		close_button_offset_y = 150;
		alpha_button_offset_x = 10;
		alpha_button_offset_y = 170;
		
		if(n_nType	== TYPE_4X6)
		{
			m_grid_start_offset_x += 3;
			m_grid_start_offset_y += 3;
		}

		Set(10, 128, m_pC_inventory_spk->GetWidth(m_MainWindowSpk), m_pC_inventory_spk->GetHeight(m_MainWindowSpk));
		break;

	case RACE_OUSTERS:
		m_pC_inventory_spk = new C_SPRITE_PACK(SPK_OUSTERS_INVENTORY_SUB);
		
		m_grid_start_offset_x = 25;
		m_grid_start_offset_y = 35;
		close_button_offset_x = 63;
		close_button_offset_y = 162;
		alpha_button_offset_x = 25;
		alpha_button_offset_y = 170;
		
		Set(10, 128, m_pC_inventory_spk->GetWidth(m_MainWindowSpk), m_pC_inventory_spk->GetHeight(m_MainWindowSpk));
		break;		
	}

	if(n_nType  ==  TYPE_2X4)
	{
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_button_offset_x, close_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_X), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_X), CLOSE_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_X));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(alpha_button_offset_x, alpha_button_offset_y, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), ALPHA_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA));	
	}
	else
	{
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(close_button_offset_x + 60 , close_button_offset_y + 60, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_X), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_X), CLOSE_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_X));
		m_pC_button_group->Add(new C_VS_UI_EVENT_BUTTON(alpha_button_offset_x , alpha_button_offset_y + 60, gpC_global_resource->m_pC_assemble_box_button_spk->GetWidth(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), gpC_global_resource->m_pC_assemble_box_button_spk->GetHeight(C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA), ALPHA_ID, this, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA));	
	}	
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY_SUB::~C_VS_UI_INVENTORY_SUB
//
// 
//-----------------------------------------------------------------------------
C_VS_UI_INVENTORY_SUB::~C_VS_UI_INVENTORY_SUB()
{
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::INVENTORY_SUB, Rect(x, y, w, h));
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::INVENTORY_SUB, GetAttributes()->alpha);
	
	g_UnregisterWindow(this);
	
	if(m_pC_button_group != NULL)
		DeleteNew(m_pC_button_group);
	if(m_pC_inventory_spk != NULL)
		DeleteNew(m_pC_inventory_spk);

	if( m_bl_Alpha != NULL)
	{
		for(int i=0; i< m_GridX; i++)
			DeleteNewArray(m_bl_Alpha[i]);
		DeleteNewArray(m_bl_Alpha);
	}

}


//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY_SUB::WindowEventReceiver
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INVENTORY_SUB::WindowEventReceiver(id_t event)
{
	switch (event)
	{
	case EVENT_WINDOW_MOVE:
		ResetRect();
		break;
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY_SUB::Start
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INVENTORY_SUB::Start()
{
	AttrAlpha(gpC_vs_ui_window_manager->IsAlpha(C_VS_UI_WINDOW_MANAGER::INVENTORY_SUB));
	Rect &rect = gpC_vs_ui_window_manager->GetRect(C_VS_UI_WINDOW_MANAGER::INVENTORY_SUB);
	
	if(rect.w != -1)
	{
		x = rect.x;
		y = rect.y;
	}
	
	PI_Processor::Start();
	
	AttrPin(true);
	gpC_window_manager->AppearWindow(this);
	
	// init
	ResetRect();
	m_focus_grid_x = NOT_SELECTED;
	m_focus_grid_y = NOT_SELECTED;

	
}


//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY_SUB::Finish
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INVENTORY_SUB::Finish()
{
	gpC_vs_ui_window_manager->SetAlpha(C_VS_UI_WINDOW_MANAGER::INVENTORY_SUB, GetAttributes()->alpha);
	
	gpC_vs_ui_window_manager->SetRect(C_VS_UI_WINDOW_MANAGER::INVENTORY_SUB, Rect(x, y, w, h));
	
	PI_Processor::Finish();
	
	gpC_window_manager->DisappearWindow(this);
	if(gpC_mouse_pointer->IsCursorDescription())gpC_mouse_pointer->CursorDescriptionToggle();
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY_SUB::IsPixel
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_INVENTORY_SUB::IsPixel(int _x, int _y)
{
	if(Moving()) return true;
	
	return IsInRect(_x, _y);
	//return AnyMatchWindowPixel(_x, _y);
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY_SUB::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_INVENTORY_SUB::ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button)
{
	const static char* m_inventory_button_string[3] = 
	{
		(*g_pGameStringTable)[UI_STRING_MESSAGE_CLOSE_INVENTORY_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_ALPHA_WINDOW].GetString(),
		(*g_pGameStringTable)[UI_STRING_MESSAGE_SHOW_NO_ALPHA_WINDOW].GetString(),		
	};

	if(GetAttributes()->alpha && p_button->GetID() == ALPHA_ID)
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_inventory_button_string[2],0,0);
	else
	{
		g_descriptor_manager.Set(DID_INFO, p_button->x+x, p_button->y+y, (void *)m_inventory_button_string[p_button->GetID()],0,0);
	}
}


//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY_SUB::ShowButtonWidget
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_INVENTORY_SUB::ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button)
{		
	if(p_button->GetID() == ALPHA_ID)
	{
		if(GetAttributes()->alpha)
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA);
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, C_GLOBAL_RESOURCE::AB_BUTTON_ALPHA_PUSHED);
	}
	else
	{
		if(p_button->GetFocusState())
		{
			if(p_button->GetPressState())
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_PUSHED_OFFSET);
			else
				gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index+C_GLOBAL_RESOURCE::AB_BUTTON_HILIGHTED_OFFSET);
		}
		else
			gpC_global_resource->m_pC_assemble_box_button_spk->BltLocked(p_button->x+x, p_button->y+y, p_button->m_image_index);
	}	
}

//-----------------------------------------------------------------------------
// Run
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INVENTORY_SUB::Run(id_t id)
{
	switch (id)
	{
	case CLOSE_ID:
	
		gpC_base->SendMessage(UI_CLOSE_INVENTORY_SUB);
	
		break;
		
	case ALPHA_ID:
		AttrAlpha(!GetAttributes()->alpha);
		EMPTY_MOVE;
		break;
	}
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY_SUB::AnyMatchWindowPixel
//
// Window image�� pixel�� (x, y) �� ���̶� ��ġ�ϸ� true�� �ƴϸ� false�� ��ȯ�Ѵ�.
//
// Item�� ��� ���� ������ 'item ��������Ʈ'�� ���� �����Ѵ�.
//-----------------------------------------------------------------------------
bool C_VS_UI_INVENTORY_SUB::AnyMatchWindowPixel(int _x, int _y) const
{
//	if (gpC_mouse_pointer->GetPickUpItem())
//	{
//		for (int i=0; i < ITEM_REF_POINT_COUNT; i++)
//		{
//			int px = g_item_ref_point[i].x+gpC_mouse_pointer->GetPointerX();
//			int py = g_item_ref_point[i].y+gpC_mouse_pointer->GetPointerY();
//			if (m_pC_inventory_spk->IsPixel(px-x, py-y))
//				return true;
//		}
//		
//		return false;
//	}
//	else
//	{
//		return m_pC_inventory_spk->IsPixel(_x-x, _y-y);
//	}
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY_SUB::AcquireMouseFocus
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI_INVENTORY_SUB::AcquireMouseFocus()
{
	gpC_base->SendMessage(UI_REMOVE_BACKGROUND_MOUSE_FOCUS);
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY_SUB::AcquireDisappear
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INVENTORY_SUB::AcquireDisappear()
{
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY_SUB::UnacquireMouseFocus
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INVENTORY_SUB::UnacquireMouseFocus()
{
	m_pC_button_group->UnacquireMouseFocus();
	m_focus_grid_x = NOT_SELECTED;
	m_focus_grid_y = NOT_SELECTED;
	
	//	m_bl_money_button_focused = false;
	//	m_bl_desc_button_focused = false;
	//	m_bl_bike_button_focused = false;
	//	m_bl_close_button_focused = false;
	//	m_bl_help_button_focused = false;
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY_SUB::GetFocusedItemGridX
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_INVENTORY_SUB::GetFocusedItemGridX(const MItem * p_item)
{
	assert(p_item);
	
	if (p_item == NULL)
		return 0;
	
	return m_grid_rect.x+GRID_UNIT_PIXEL_X*p_item->GetGridX();
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY_SUB::GetFocusedItemGridY
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_INVENTORY_SUB::GetFocusedItemGridY(const MItem * p_item)
{
	assert(p_item);
	
	if (p_item == NULL)
		return 0;
	
	return m_grid_rect.y+GRID_UNIT_PIXEL_Y*p_item->GetGridY();
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY_SUB::GetFocusedItemGridH
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI_INVENTORY_SUB::GetFocusedItemGridH(const MItem * p_item)
{
	assert(p_item);
	
	if (p_item == NULL)
		return 0;
	
	return GRID_UNIT_PIXEL_Y*p_item->GetGridHeight();
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY_SUB::Show
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI_INVENTORY_SUB::Show()
{	
	assert(m_MultiPackItem);


	if (gpC_base->m_p_DDSurface_back->Lock())
	{				
		Rect rect;
				
		if(GetAttributes()->alpha)
		{
			RECT alpha_rect;
			switch(g_eRaceInterface)
			{
			case RACE_SLAYER:
				{
					if(n_nType == TYPE_2X4)
					{
						SetRect(&alpha_rect, x+2, y+7, x+82, y+185);
					}
					DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
				}
				break;
			case RACE_VAMPIRE:
				{
					if(n_nType == TYPE_2X4)
					{
						SetRect(&alpha_rect, x+11, y+15, x+75, y+138);
					}
					DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
				}
				break;
			case RACE_OUSTERS:
				{
					if(n_nType == TYPE_2X4)
					{
						SetRect(&alpha_rect, x+16, y+14, x+95, y+200 );
					}
					DrawAlphaBox(&alpha_rect, 0, 2, 2, g_pUserOption->ALPHA_DEPTH);
				}
				break;
			}
			m_pC_inventory_spk->BltLocked(x, y, m_MainWindowSpk+1);
		}
		else
		{
			switch(g_eRaceInterface)
			{
			case RACE_SLAYER:
				m_pC_inventory_spk->BltLocked(x, y, m_MainWindowSpk);
				
				break;

			case RACE_VAMPIRE:
				m_pC_inventory_spk->BltLocked(x, y,  m_MainWindowSpk);
				break;

			case RACE_OUSTERS:
				m_pC_inventory_spk->BltLocked(x, y, m_MainWindowSpk);
				break;					
			}				
		}

		
		const MItem * p_selected_item = m_MultiPackItem->GetItem(m_focus_grid_x, m_focus_grid_y);
		
		m_MultiPackItem->SetBegin();
		for(int i = 0; i< m_GridX; i++)
			memset(m_bl_Alpha[i], 0, m_GridY*sizeof(BYTE));

		while (m_MultiPackItem->IsNotEnd())
		{
			
			MItem * p_item = m_MultiPackItem->Get();
			
			
			// p_item�� NULL�� �ݵ�� �ƴϴ�. �ֳ��ϸ� �����ϴ� �͸� Get()�ϱ� �����̴�.
			assert(p_item);

			// frame id -> sprite id
			if(p_item)
			{
				TYPE_FRAMEID frame_id = p_item->GetInventoryFrameID();
				
				
				int item_x = x+GetFocusedItemGridX(p_item);
				
				int item_y = y+GetFocusedItemGridY(p_item);
				
				
				int alpha_r, alpha_g, alpha_b;
				switch(g_eRaceInterface)
				{
				case RACE_SLAYER:
					alpha_r = 5;
					alpha_g = 7;
					alpha_b = 7;
					break;

				case RACE_VAMPIRE:
					alpha_r = 7;
					alpha_g = 5;
					alpha_b = 5;
					break;

				case RACE_OUSTERS:
					alpha_r = 5;
					alpha_g = 7;
					alpha_b = 5;
					break;
				}				
				
				// Item�� �����ִ� ���� ǥ��
				for (int j = 0; j < p_item->GetGridHeight(); j++)
				{
					for (int i = 0; i < p_item->GetGridWidth(); i++)
					{
						
						if(GetAttributes()->alpha)
						{
							
							RECT alpha_rect;
							alpha_rect.left = item_x+GRID_UNIT_PIXEL_X*i+1;
							alpha_rect.top = item_y+GRID_UNIT_PIXEL_Y*j+1;
							alpha_rect.right = alpha_rect.left+28;
							alpha_rect.bottom = alpha_rect.top+28;
							//							gpC_base->m_p_DDSurface_back->Unlock();
							DrawAlphaBox(&alpha_rect, alpha_r, alpha_g, alpha_b, g_pUserOption->ALPHA_DEPTH);
							
							//							gpC_base->m_p_DDSurface_back->Lock();
							
							m_bl_Alpha[p_item->GetGridX()+i][p_item->GetGridY()+j] = 1;
							
						}
						else
						{
							
							m_pC_inventory_spk->BltLocked(item_x+GRID_UNIT_PIXEL_X*i, 
								item_y+GRID_UNIT_PIXEL_Y*j,
								INVENTORY_ITEMBACK);
							
						}
					}
				}
					
				
				item_x += (p_item->GetGridWidth()*GRID_UNIT_PIXEL_X)/2-gpC_item->GetWidth(frame_id)/2;
				item_y += (p_item->GetGridHeight()*GRID_UNIT_PIXEL_Y)/2-gpC_item->GetHeight(frame_id)/2;
				
				
				if (p_selected_item && (p_selected_item->GetID() == p_item->GetID()))
				{
					if(p_item->IsSpecialColorItem() )
						CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetSpecialColorItemColorset(), 0);
					else			
						CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetItemOptionColorSet(), 0);
					gpC_item->BltLockedOutline(item_x, item_y, WHITE, frame_id);
					if(p_item->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET && g_eRaceInterface == RACE_OUSTERS)
					{
						ITEMTABLE_INFO::ELEMENTAL_TYPE eType = (*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].ElementalType;
						if(eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE ||
							eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER ||
							eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH
							)
						{
							gpC_global_resource->m_pC_info_spk->BltLockedOutline(x+GetFocusedItemGridX(p_item), y+GetFocusedItemGridY(p_item), RGB_WHITE, C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType);
							
						}
					}
					
				}
				else
				{
					
					if(p_item->IsSpecialColorItem() )
						CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetSpecialColorItemColorset(), 0);
					else			
						CIndexSprite::SetUsingColorSet(const_cast<MItem *>(p_item)->GetItemOptionColorSet(), 0);
					
					
					if (p_item->IsAffectStatus() || p_item->IsQuestItem() )
					{
						// frame id -> sprite id
						gpC_item->BltLocked(item_x, item_y, frame_id);
						if(p_item->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET && g_eRaceInterface == RACE_OUSTERS)
						{
							ITEMTABLE_INFO::ELEMENTAL_TYPE eType = (*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].ElementalType;
							if(eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE ||
								eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER ||
								eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH
								)
							{
								gpC_global_resource->m_pC_info_spk->BltLocked(x+GetFocusedItemGridX(p_item), y+GetFocusedItemGridY(p_item), C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType);
								
							}
						}
					}
					else
					{
						gpC_item->BltLockedColorSet(item_x, item_y, frame_id, ITEM_DISABLE_COLOR_SET);
						if(p_item->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET && g_eRaceInterface == RACE_OUSTERS)
						{
							ITEMTABLE_INFO::ELEMENTAL_TYPE eType = (*g_pItemTable)[p_item->GetItemClass()][p_item->GetItemType()].ElementalType;
							if(eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_FIRE ||
								eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_WATER ||
								eType == ITEMTABLE_INFO::ELEMENTAL_TYPE_EARTH
								)
							{
								gpC_global_resource->m_pC_info_spk->BltLockedColor(x+GetFocusedItemGridX(p_item), y+GetFocusedItemGridY(p_item), C_GLOBAL_RESOURCE::OUSTERS_ELEMENTAL_MARK_FIRE+eType, 0);
								
							}
						}
					}

				}
			}
			m_MultiPackItem->Next();
		}
		
		if(GetAttributes()->alpha)
		{
			
			int alpha_r, alpha_g, alpha_b;
			switch(g_eRaceInterface)
			{
			case RACE_SLAYER:
				alpha_r = 0;
				alpha_g = 2;
				alpha_b = 2;
				break;

			case RACE_VAMPIRE:
				alpha_r = 2;
				alpha_g = 0;
				alpha_b = 0;
				break;

			case RACE_OUSTERS:
				alpha_r = 0;
				alpha_g = 2;
				alpha_b = 0;
				break;				
			}
			
			
			for(int alpha_x = 0; alpha_x < m_GridX; alpha_x++)
			{
				for(int alpha_y = 0; alpha_y < m_GridY; alpha_y++)
				{
					if(m_bl_Alpha[alpha_x][alpha_y] == 0)
					{
						RECT alpha_rect;
						alpha_rect.left = x+m_grid_rect.x+alpha_x*GRID_UNIT_PIXEL_X+1;
						alpha_rect.top = y+m_grid_rect.y+alpha_y*GRID_UNIT_PIXEL_Y+1;
						alpha_rect.right = alpha_rect.left+28;
						alpha_rect.bottom = alpha_rect.top+28;
						
						DrawAlphaBox(&alpha_rect, alpha_r, alpha_g, alpha_b, g_pUserOption->ALPHA_DEPTH);
					}
				}
			}
			
		}
		//
		// Item�� ��� ������ grid ��ġ�� �̸� �� �� �ֵ��� �Ѵ�.
		//
		if (gpC_mouse_pointer->GetPickUpItem() && 
			m_focus_grid_x != NOT_SELECTED && 
			m_focus_grid_y != NOT_SELECTED)
		{
			
			MItem * p_item = gpC_mouse_pointer->GetPickUpItem();
			
			TYPE_FRAMEID frame_id = p_item->GetInventoryFrameID();
			int item_x = x+m_grid_rect.x+GRID_UNIT_PIXEL_X*m_focus_grid_x+(p_item->GetGridWidth()*GRID_UNIT_PIXEL_X)/2-gpC_item->GetWidth(frame_id)/2;
			int item_y = y+m_grid_rect.y+GRID_UNIT_PIXEL_Y*m_focus_grid_y+(p_item->GetGridHeight()*GRID_UNIT_PIXEL_Y)/2-gpC_item->GetHeight(frame_id)/2;
			
			MItem * p_old_item;
			
			if (m_MultiPackItem->CanReplaceItem(p_item, m_focus_grid_x, m_focus_grid_y, p_old_item))
			{
				if (p_old_item)
					gpC_item->BltLockedOutlineOnly(item_x, item_y, ga_item_blink_color_table[g_blink_value], frame_id);
				else
					
					gpC_item->BltLockedOutlineOnly(item_x, item_y, POSSIBLE_COLOR, frame_id);				
			}
			else
			{
				gpC_item->BltLockedOutlineOnly(item_x, item_y, IMPOSSIBLE_COLOR, frame_id);
				
			}
		}

		
		m_pC_button_group->Show();
		
		gpC_base->m_p_DDSurface_back->Unlock();
	}
	//
	// ������ ���� ǥ��
	//
	int len = 0;
	RECT rect[60];
	int num[60];
	
	m_MultiPackItem->SetBegin();
	
	while (m_MultiPackItem->IsNotEnd())
	{
		
		const MItem * p_item = m_MultiPackItem->Get();
		
		// p_item�� NULL�� �ݵ�� �ƴϴ�. �ֳ��ϸ� �����ϴ� �͸� Get()�ϱ� �����̴�.
		
				// ������ ����ǥ��

		if( p_item == NULL )
		{			
			break;
//			sprintf(g_szBugReportBuffer,"Inventory::Show itemnum ItemNULL");
//			gpC_base->SendMessage( UI_SEND_BUG_REPORT, __LINE__,0,g_szBugReportBuffer);
		}

		assert(p_item);
		
		
		if(p_item->IsPileItem() || p_item->IsChargeItem())
		{
			RECT rt;
			
			rt.right = x+GetFocusedItemGridX(p_item) + p_item->GetGridWidth()*GRID_UNIT_PIXEL_X-1;
			
			for(int depth = 0, number = p_item->GetNumber(); number > 0; number/=10, depth++);
			
			if(depth == 0) depth = 1;
			rt.left = rt.right - 7*depth;
			
			rt.bottom = y+GetFocusedItemGridY(p_item) + p_item->GetGridHeight()*GRID_UNIT_PIXEL_Y;
			
			rt.top = rt.bottom - 12;
			
			rect[len] = rt;
			rt.left = max(0, rt.left);
			rt.top = max(0, rt.top);
			
			if(rt.left < rt.right && rt.top < rt.bottom)DrawAlphaBox(&rt, 0, 0, 0, g_pUserOption->ALPHA_DEPTH);
			num[len] = p_item->GetNumber();
			
			len++;
		}


		m_MultiPackItem->Next();
	}
	
	char sz_temp[512];
	g_FL2_GetDC();	
	COLORREF markColor = RGB(220, 220, 220);//RGB(140, 140, 255);
	
	for(int i = 0; i < len; i++)
	{
		
		wsprintf(sz_temp, "%d", num[i]);
		g_PrintColorStr(rect[i].left, rect[i].top, sz_temp, gpC_base->m_item_desc_pi, markColor);	
		
	}

	m_pC_button_group->ShowDescription();
	g_FL2_ReleaseDC();
		
	SHOW_WINDOW_ATTR;
		
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY_SUB::MouseControl
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI_INVENTORY_SUB::MouseControl(UINT message, int _x, int _y)
{
	Window::MouseControl(message, _x, _y);
	_x-=x; _y-=y;
	
	bool re = m_pC_button_group->MouseControl(message, _x, _y);
	
	const MItem * p_selected_item;
	
	switch (message)
	{
	case M_MOVING:
		
		//
		int i;
		int loop;
		int px, py;
		
		if (gpC_mouse_pointer->GetPickUpItem())
			loop = ITEM_REF_POINT_COUNT;
		else
		{
			loop = 1;
		}
		
		for (i=0; i < loop; i++)
		{
			if (loop == ITEM_REF_POINT_COUNT)
			{
				px = g_item_ref_point[i].x+gpC_mouse_pointer->GetPointerX()-x;
				py = g_item_ref_point[i].y+gpC_mouse_pointer->GetPointerY()-y;
			}
			else
			{
				px = _x;
				py = _y;
			}
			
			// search grid
			int distance_x = px - m_grid_rect.x;
			int distance_y = py - m_grid_rect.y;
			
			if (distance_x >= 0 && distance_x < m_grid_rect.w && 
				distance_y >= 0 && distance_y < m_grid_rect.h)
			{
				if(gpC_Imm != NULL && (m_focus_grid_x != distance_x/GRID_UNIT_PIXEL_X || m_focus_grid_y != distance_y/GRID_UNIT_PIXEL_Y))
					gpC_Imm->ForceUI(CImm::FORCE_UI_GRID);
				m_focus_grid_x = distance_x/GRID_UNIT_PIXEL_X;
				m_focus_grid_y = distance_y/GRID_UNIT_PIXEL_Y;
				
				if (loop == ITEM_REF_POINT_COUNT)
				{
					// item�� grid ������ ������� ������ ������ ��ġ��Ų��.
					const MItem * p_pickup_item = gpC_mouse_pointer->GetPickUpItem();
					int a, b;
					switch (i)
					{
					case 0: // left up - first!
						a = m_focus_grid_x+p_pickup_item->GetGridWidth()-m_GridX;
						b = m_focus_grid_y+p_pickup_item->GetGridHeight()-m_GridY;
						if (a > 0)
							m_focus_grid_x -= a;
						if (b > 0)
							m_focus_grid_y -= b;
						break;
						
					case 1: // right up
						m_focus_grid_x = 0;
						b = m_focus_grid_y+p_pickup_item->GetGridHeight()-m_GridY;
						if (b > 0)
							m_focus_grid_y -= b;
						break;
						
					case 2: // left down
						m_focus_grid_y = 0;
						a = m_focus_grid_x+p_pickup_item->GetGridWidth()-m_GridX;
						if (a > 0)
							m_focus_grid_x -= a;
						break;
						
					case 3: // right down
						m_focus_grid_y = 0;
						if (m_focus_grid_x+1 <= p_pickup_item->GetGridHeight())
							m_focus_grid_x = 0;
					}
				}
				 
				p_selected_item = m_MultiPackItem->GetItem(m_focus_grid_x, m_focus_grid_y);
				if (p_selected_item != NULL)
				{
					MItem *pMouseItem = gpC_mouse_pointer->GetPickUpItem();
					g_descriptor_manager.Set(DID_ITEM, x+GetFocusedItemGridX(p_selected_item), y+GetFocusedItemGridY(p_selected_item), (void *)p_selected_item);
				}
				
				return true;
				//					break; // escape 'for'
			}
		}
		if(m_focus_grid_x != NOT_SELECTED || m_focus_grid_y != NOT_SELECTED)
			gpC_Imm->ForceUI(CImm::FORCE_UI_GRID);
		m_focus_grid_x = NOT_SELECTED;
		m_focus_grid_y = NOT_SELECTED;
		break;
		
	case M_LEFTBUTTON_DOWN:
	case M_LB_DOUBLECLICK:

			if(gC_vs_ui.inventory_mode != 1 && !gC_vs_ui.IsRunningExchange())// ���λ��� ���¸�� �̰ų� ��ȯâ�� �� ���� ��
			{
				if (gpC_mouse_pointer->GetPickUpItem() == NULL && re && m_MultiPackItem->GetItem(m_focus_grid_x, m_focus_grid_y) == NULL)
					//TestGridRect(_x, _y) == false && re)
				{
					MoveReady();
					SetOrigin(_x, _y);
					break;
				}
				
				//
				// Item�� ������ ���´�.
				//
				{
					bool ret = Click(m_grid_rect.x, m_grid_rect.y);
					
					if (ret)
						EMPTY_MOVE;
				}
			}
			break;
		
	case M_LEFTBUTTON_UP:
		CancelPushState();
		break;
		
	case M_RIGHTBUTTON_DOWN:
		// ���� ���� ���, �ŷ� �� �ƴҶ�, ���콺�� �������� ������
		if(gC_vs_ui.inventory_mode != 1 && !gC_vs_ui.IsRunningExchange() && NULL == gpC_mouse_pointer->GetPickUpItem()) 
		{
			const MItem* p_cur_item = m_MultiPackItem->GetItem(m_focus_grid_x, m_focus_grid_y);
			
			if(NULL == p_cur_item)
				break;
			if(p_cur_item->GetItemClass() == ITEM_CLASS_PET_FOOD)
			{
				PETINFO *pPetInfo = gC_vs_ui.GetMyPetInfo();
				if( pPetInfo != NULL && pPetInfo->EXIST )
				{
					TYPE_ITEMTYPE itemType = pPetInfo->ITEM_TYPE;
					TYPE_ITEMTYPE foodType = p_cur_item->GetItemType();

					if( ( foodType >=0 && foodType <= 5 && (itemType == 1 || itemType == 2) ) ||
						(foodType >= 6 && foodType <= 9 && itemType == 3) ||
						(foodType >= 10 && foodType <= 13 && itemType == 4)||
						(foodType >= 14 && foodType <= 17 && itemType == 5))
					{
						gC_vs_ui.RunUsePetFood(p_cur_item->GetID(), m_MultiPackItem);
					}
					return true;
				}
			}
			else 
			if(p_cur_item->GetItemClass() == ITEM_CLASS_PET_ITEM && p_cur_item->GetItemType() >2) // 2�� �� ��ȯ������ �̸�
			{
				switch(g_eRaceInterface)
				{
					case RACE_SLAYER:
						if((*g_pSkillManager)[SKILLDOMAIN_BLADE].GetDomainLevel() < 40 &&
						(*g_pSkillManager)[SKILLDOMAIN_SWORD].GetDomainLevel() < 40 &&
						(*g_pSkillManager)[SKILLDOMAIN_GUN].GetDomainLevel() < 40 &&
						(*g_pSkillManager)[SKILLDOMAIN_HEAL].GetDomainLevel() < 40 &&
						(*g_pSkillManager)[SKILLDOMAIN_ENCHANT].GetDomainLevel() < 40)
//						if(g_char_slot_ingame.DOMAIN_BLADE		< 40 &&
//							g_char_slot_ingame.DOMAIN_ENCHANT	< 40 &&
//							g_char_slot_ingame.DOMAIN_GUN		< 40 &&
//							g_char_slot_ingame.DOMAIN_HEAL		< 40 &&
//							g_char_slot_ingame.DOMAIN_SWORD) // �� 40 ���ϴ� �� ����.
							{
								gpC_base->SendMessage(UI_MESSAGE_BOX, UI_STRING_MESSAGE_CANNOT_SUMMON_2ND_PET, 0, 	NULL);
								return true;
							}
						break;

					case RACE_VAMPIRE:
					case RACE_OUSTERS:
						if(g_char_slot_ingame.level < 40)
						{
							gpC_base->SendMessage(UI_MESSAGE_BOX, UI_STRING_MESSAGE_CANNOT_SUMMON_2ND_PET, 0, 	NULL);
							return true;
						}
						break;		
				}
			}
									
			gpC_base->SendMessage(UI_ITEM_USE_SUBINVENTORY, p_cur_item->GetID(), m_MultiPackItem->GetID(), (MItem *)p_cur_item);
			
		}

		break;
		
	case M_RIGHTBUTTON_UP:

		break;
	}
	
	return true;
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY_SUB::ResetRect
//
// ��ü Grid rect�� �����Ѵ�. �̰��� Inventory�� �̵��� ���� ������ ����� �Ѵ�.
//-----------------------------------------------------------------------------
void C_VS_UI_INVENTORY_SUB::ResetRect()
{
	m_grid_rect.Set(m_grid_start_offset_x, 
								m_grid_start_offset_y, 
								GRID_UNIT_PIXEL_X*m_GridX, GRID_UNIT_PIXEL_Y*m_GridY);
	
}

//-----------------------------------------------------------------------------
// C_VS_UI_INVENTORY_SUB::Click
//
// ���� Item�� ��� ������ ������ ��ü�ϰ�, ��� ���� ������ Inventory�� �ִ�
// ���� ���´�.
//
// ���� �ߴٸ� true��, �׷��������� false�� ��ȯ�Ѵ�.
//
// grid_start_x, grid_start_y�� inventory grid �������̴�. �̰��� �����Ͽ� item
// (x, y)�� ���Ѵ�.
//-----------------------------------------------------------------------------
bool C_VS_UI_INVENTORY_SUB::Click(int grid_start_x, int grid_start_y)
{
	if (m_focus_grid_x == NOT_SELECTED || 
		m_focus_grid_y == NOT_SELECTED ||
		gbl_item_lock == true)
	{
		return false;
	}
		
	//	int item_x, item_y;
	
	const MItem* pMouseItem = gpC_mouse_pointer->GetPickUpItem();
	
	 
	if (pMouseItem) // ��� �ִ°�?
	{
//		if(pMouseItem->GetItemClass() == ITEM_CLASS_PET_ITEM ) // ��� �㿡 ����]
//			return false;
		// �߰��� �� ���� ���
		// ������ �������� ��Ƽ�ѿ� �߰��� �� ����. by bezz
		ITEM_CLASS ItemClass = pMouseItem->GetItemClass();
		TYPE_ITEMTYPE ItemType = pMouseItem->GetItemType();
		if ( ( ItemClass == ITEM_CLASS_RELIC )								// ����
			|| ( ItemClass == ITEM_CLASS_BLOOD_BIBLE )						// ���� ����
			|| ( ItemClass == ITEM_CLASS_CASTLE_SYMBOL )					// �� ��¡
			|| ( ItemClass == ITEM_CLASS_WAR_ITEM )							// ���� ������/ �巡�� ����
			|| ( ItemClass == ITEM_CLASS_EVENT_ITEM && ItemType == 27 )		// ���

			// sjheon 2004.04.28 add
			|| ( ItemClass == ITEM_CLASS_EVENT_ETC && ItemType == 18 )		// �йи� ����
			|| ( ItemClass == ITEM_CLASS_EVENT_ITEM && (ItemType >= 32 && ItemType <= 36) )		// ǳ�� �Ӹ��� 
			// sjheon 2004.04.28 add
							
			|| ( ItemClass == ITEM_CLASS_SWEEPER ) )						// ������
		{
			return false;
		}
		// by bezz

		const MItem* p_cur_item = m_MultiPackItem->GetItem(m_focus_grid_x, m_focus_grid_y);

		MItem* p_old_item  = NULL;
			
		if (m_MultiPackItem->CanReplaceItem(gpC_mouse_pointer->GetPickUpItem(),		// �߰��� item
			m_focus_grid_x, m_focus_grid_y,	// �߰��� ��ġ 
			p_old_item))								// �����ִ� item
		{
			
			gpC_base->SendMessage(UI_ITEM_DROP_TO_INVENTORY_SUB, 
				m_focus_grid_x, m_focus_grid_y,
				(void *)pMouseItem);
			
		}
		else
		{
			// failed.
			return false;
		}
	}
	else
	{
		// ���´�.
		MItem * p_item = m_MultiPackItem->GetItem(m_focus_grid_x, m_focus_grid_y);
		
		if (p_item != NULL) // Item�� �ִ�.
		{
			int number = p_item->GetNumber();
			
			if(gpC_mouse_pointer->IsCursorDescription())
				gC_vs_ui.RunDescDialog(DID_ITEM, (void *)p_item);
			else
			{
				m_MultiPackItem->RemoveItem(m_focus_grid_x, m_focus_grid_y);
				
#ifdef _LIB
				gpC_base->SendMessage(UI_ITEM_PICKUP_FROM_INVENTORY_SUB,
					m_focus_grid_x, m_focus_grid_y,
					(MItem *)p_item);
#else
				int item_x = x+grid_start_x+p_item->GetGridX()*GRID_UNIT_PIXEL_X+(p_item->GetGridWidth()*GRID_UNIT_PIXEL_X)/2-gpC_item->GetWidth(p_item->GetInventoryFrameID())/2;
				int item_y = y+grid_start_y+p_item->GetGridY()*GRID_UNIT_PIXEL_Y+(p_item->GetGridHeight()*GRID_UNIT_PIXEL_Y)/2-gpC_item->GetHeight(p_item->GetInventoryFrameID())/2;
				
				
				gpC_base->SendMessage(UI_ITEM_PICKUP_FROM_INVENTORY_SUB,
					MAKEDWORD(m_focus_grid_x, m_focus_grid_y),
					MAKEDWORD(item_x, item_y),
					(MItem *)p_item);
#endif

				if(g_pSDLInput->KeyDown(DIK_LSHIFT))
				{
					POINT point;
					if(p_item != NULL && g_pInventory->GetFitPosition((MItem *)p_item, point))
					{
						const MItem* p_cur_item = g_pInventory->GetItem(point.x, point.y);
						
						// �ѿ� źâ�� ����� �Ͱ� ���� ���� insert item�̴�.
						// ��ġ�� ������ ��ġ�Ұ�쿡�� �߰��Ѵ�.
						if (p_item->IsInsertToItem( p_cur_item ) && p_cur_item->GetGridX() == point.x && p_cur_item->GetGridY() == point.y)
						{
							// ������ �ִ� item�� �߰��� �� �ִ� ���
							
							// ��� Item�� ��� �ִ�(�߰��� Item)�� Client���� �˾ƾ� �Ѵ�.
							// ��� �ִ� Item�� Client���� access�� �� �����Ƿ� ��� Item�� ������.
							gpC_base->SendMessage(UI_ITEM_INSERT_FROM_INVENTORY,
								point.x, point.y,
								(void *)p_cur_item); // ��� Item
						}
						else
						{	
							// �߰��� �� ���� ���
							MItem* p_old_item  = NULL;
							
							if (g_pInventory->CanReplaceItem((MItem *)p_item,		// �߰��� item
								point.x, point.y,	// �߰��� ��ġ 
								p_old_item))								// �����ִ� item
							{
								
								gpC_base->SendMessage(UI_ITEM_DROP_TO_INVENTORY, 
									point.x, point.y,
									(void *)p_item);
								
							}
							else
							{
								// failed.
								return false;
							}
						}						
					}
				}
			}
		}
	}

	return true;
}

void C_VS_UI_INVENTORY_SUB::KeyboardControl(UINT message, UINT key, long extra)
{
	Window::KeyboardControl(message, key, extra);
	

}

bool	C_VS_UI_INVENTORY_SUB::ReplaceSubInventoryItem(MItem* pItem, BYTE x, BYTE y, MItem*& pOldItem)
{
	if(NULL == m_MultiPackItem)
		return false;
	return m_MultiPackItem->ReplaceItem(pItem, x, y, pOldItem);
}
bool	C_VS_UI_INVENTORY_SUB::AddItemToSubInventory(MItem* pItem, int X, int Y)
{
	if(NULL == m_MultiPackItem)
		return false;
	if(m_MultiPackItem->AddItem(pItem, X, Y))
	{
		m_MultiPackItem->CheckAffectStatusAll();
		return true;
	}
	return false;
}

#endif