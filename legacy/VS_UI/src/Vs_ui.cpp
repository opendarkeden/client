// VS_UI.cpp

#include "client_PCH.h"
#include "VS_UI.h"
#include "VS_UI_Mouse_pointer.h"
#include "VS_UI_Message.h"
#include "VS_UI_Description.h"
#include "VS_UI_Filepath.h"
#include "KeyAccelerator.h"
#include "UserOption.h"
#include "AcceleratorManager.h"
#include "MParty.h"
#include "CImm.h"
#include "MZoneTable.h"
#include "debuginfo.h"
#include "MEventManager.h"
#include "MGameStringTable.H"
#include "SkinManager.h"
#include "assert.h"
#include "MHelpDef.h"
CImm *gpC_Imm = NULL;

//----------------------------------------------------------------------------
// Local defines
//----------------------------------------------------------------------------
#define INTERFACE_BLINK_MILLISEC		150
#define PREV_KEYSTATE					0x40000000


//----------------------------------------------------------------------------
// Function prototype
//----------------------------------------------------------------------------
void _AcquireMouseFocusHandler();
void _UnacquireMouseFocusHandler();
void _AcquireDisappearHandler();
void _Timer_InterfaceBlink();

#ifndef _LIB
void ShowItem_MouseControl(UINT message, int _x, int _y);
void ShowItem_KeyboardControl(UINT message, UINT key, long extra);
void ShowItem();

int g_ui_item_max = 0;
// for sound..
void InitSound();
void UnInitSound();
#endif

extern HWND	g_hWnd;

//----------------------------------------------------------------------------
// Globals data
//----------------------------------------------------------------------------
C_VS_UI	gC_vs_ui;

bool gbl_global_empty_move = false;

//
// g_interface_blink_tid
//
// Title/Game에서 범용적으로 사용하는 blink timer.
//
timer_id_t	g_interface_blink_tid = INVALID_TID;
bool gbl_ask_go_biling_page_running;


UINT			g_blink_value = 0;

#ifndef _LIB
bool	gbl_game_mode;
#endif

//----------------------------------------------------------------------------
// Operations
//----------------------------------------------------------------------------
void _Timer_InterfaceBlink()
{
	if (g_blink_value == INTERFACE_BLINK_VALUE_MAX-1)
		g_blink_value = 0;

	g_blink_value++;
}

void _AcquireMouseFocusHandler()
{
	UNSET_BACKGROUND_MOUSE_FOCUS
}

void _UnacquireMouseFocusHandler()
{
	SET_BACKGROUND_MOUSE_FOCUS;
	g_descriptor_manager.Unset();
}

void _AcquireDisappearHandler()
{
	SET_BACKGROUND_MOUSE_FOCUS;
}

void	ExecF_GoBilingPage(C_VS_UI_DIALOG *p_this_dialog, id_t id)
{
	switch(id)
	{
	case DIALOG_EXECID_OK :
		gpC_base->SendMessage(UI_GO_BILING_PAGE, TRUE);
		gbl_ask_go_biling_page_running= false;
		break;
	case DIALOG_EXECID_EXIT :
	case DIALOG_EXECID_CANCEL :
		gpC_base->SendMessage(UI_GO_BILING_PAGE, FALSE);
		gbl_ask_go_biling_page_running= false;
		break;
	}
}


/////////////////////////////////////////////////////////////////////////////////
//
// DI를 사용하지 않을 경우, ctrl+space bar에 대한 message가 발생하지 않기 때문에,
// (왜 그런지 아직 모름) 외부에서 DI의 message를 받는다.
//
/////////////////////////////////////////////////////////////////////////////////

void C_VS_UI::HotKey_Mark()
{
	if (m_pC_game)
		m_pC_game->HotKey_Mark();
}

//void C_VS_UI::HotKey_WindowToggle()
//{
//	if (m_pC_game)
//		m_pC_game->HotKey_WindowToggle();
//}
#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
	void C_VS_UI::HotKey_Inventory(bool IsCheckSubInventory)	
	{
		if (m_pC_game)
		{
			m_pC_game->HotKey_Inventory(IsCheckSubInventory);
		}
	}
#else
	void C_VS_UI::HotKey_Inventory()	
	{
		if (m_pC_game)
		{
			m_pC_game->HotKey_Inventory();
		}
	}
#endif

void C_VS_UI::HotKey_Gear()		
{
	if (m_pC_game)
		m_pC_game->HotKey_Gear();
}

void C_VS_UI::HotKey_QuickItemSlot()
{
	if (m_pC_game)
		m_pC_game->HotKey_QuickItemSlot();
}

//add by viva
void C_VS_UI::HotKey_Friend()
{
	if (m_pC_game)
		m_pC_game->HotKey_Party();
}

void C_VS_UI::HotKey_Party()
{
	if (m_pC_game)
		m_pC_game->HotKey_Party();
}

void C_VS_UI::HotKey_CharInfo()
{
	if (m_pC_game)
		m_pC_game->HotKey_CharInfo();
}

void C_VS_UI::HotKey_Grade1Info()
{
	if (m_pC_game)
		m_pC_game->HotKey_Grade1Info();
}

void C_VS_UI::HotKey_Grade2Info()
{
	if (m_pC_game)
		m_pC_game->HotKey_Grade2Info();
}

void C_VS_UI::HotKey_Help()
{
	if(m_pC_game)
		m_pC_game->HotKey_Help();
}

void C_VS_UI::HotKey_SkillInfo()
{
	if (m_pC_game)
		m_pC_game->HotKey_SkillInfo();
}

void C_VS_UI::HotKey_Minimap()
{
	if (m_pC_game)
		m_pC_game->HotKey_Minimap();
}

void C_VS_UI::HotKey_Skill()
{
	if (m_pC_game)
		m_pC_game->HotKey_Skill();
}

void C_VS_UI::HotKey_Chat()			// ctrl + c
{
	if (m_pC_game)
		m_pC_game->HotKey_Chat();
}
void C_VS_UI::HotKey_ZoneChat()		// ctrl + z
{
	if (m_pC_game)
		m_pC_game->HotKey_ZoneChat();
}

void C_VS_UI::HotKey_GuildChat()		// ctrl + g
{
	if (m_pC_game)
		m_pC_game->HotKey_GuildChat();
}

void C_VS_UI::HotKey_UnionChat()		// ctrl + d
{
	if (m_pC_game)
		m_pC_game->HotKey_UnionChat();
}

void C_VS_UI::HotKey_Whisper()		// ctrl + w
{
	if (m_pC_game)
		m_pC_game->HotKey_Whisper();
}

void C_VS_UI::HotKey_ExtendChat()		// ctrl + e
{
	if (m_pC_game)
		m_pC_game->HotKey_ExtendChat();
}

void C_VS_UI::HotKey_ESC()			
{
	if (m_pC_game)
		m_pC_game->HotKey_ESC();
}

void C_VS_UI::HotKey_F1()				
{
	if (m_pC_game)
		m_pC_game->HotKey_F1();
}

void C_VS_UI::HotKey_F2()				
{
	if (m_pC_game)
		m_pC_game->HotKey_F2();
}

void C_VS_UI::HotKey_F3()				
{
	if (m_pC_game)
		m_pC_game->HotKey_F3();
}

void C_VS_UI::HotKey_F4()				
{
	if (m_pC_game)
		m_pC_game->HotKey_F4();
}

void C_VS_UI::HotKey_F5()				
{
	if (m_pC_game)
		m_pC_game->HotKey_F5();
}

void C_VS_UI::HotKey_F6()				
{
	if (m_pC_game)
		m_pC_game->HotKey_F6();
}

void C_VS_UI::HotKey_F7()				
{
	if (m_pC_game)
		m_pC_game->HotKey_F7();
}

void C_VS_UI::HotKey_F8()				
{
	if (m_pC_game)
		m_pC_game->HotKey_F8();
}

void C_VS_UI::HotKey_F9()				
{
	if (m_pC_game)
		m_pC_game->HotKey_F9();
}

void C_VS_UI::HotKey_F10()			
{
	if (m_pC_game)
		m_pC_game->HotKey_F10();
}

void C_VS_UI::HotKey_F11()			
{
	if (m_pC_game)
		m_pC_game->HotKey_F11();
}

void C_VS_UI::HotKey_F12()
{
	if (m_pC_game)
		m_pC_game->HotKey_F12();
}

void C_VS_UI::HotKey_PartyChat()
{
	if (m_pC_game)
		m_pC_game->HotKey_PartyChat();
}

void C_VS_UI::HotKey_Quest()
{
	if (m_pC_game)
		m_pC_game->HotKey_Quest();
}

void C_VS_UI::HotKey_MailBox()
{
	if (m_pC_game)
		m_pC_game->HotKey_MailBox();
}

void C_VS_UI::HotKey_PetInfo()
{
	if (m_pC_game)
		m_pC_game->HotKey_PetInfo();
}

void C_VS_UI::HotKey_SummonPet()
{
	if (m_pC_game)
		m_pC_game->HotKey_SummonPet();
}

//-----------------------------------------------------------------------------
// PickUpItem
//
// Client -> UI
// Item을 집었다.
//-----------------------------------------------------------------------------
void C_VS_UI::PickUpItem(MItem * p_item, int item_x, int item_y)
{
	gpC_mouse_pointer->PickUpItem(p_item, item_x, item_y);
}

//-----------------------------------------------------------------------------
// DropItem
//
// Item을 성공적으로 떨어뜨렸다.
//-----------------------------------------------------------------------------
void C_VS_UI::DropItem()
{
	gpC_mouse_pointer->DropItem();
}

//-----------------------------------------------------------------------------
// Lock/Unlock Item
//
// Lock되면 Item 집기/놓기/바꾸기가 불가능해진다. Unlock은 그것을 해제한다.
//-----------------------------------------------------------------------------
void C_VS_UI::LockItem()
{
	if (m_pC_game)
		m_pC_game->LockItem();
}

void C_VS_UI::UnlockItem()
{
	if (m_pC_game)
		m_pC_game->UnlockItem();
}

//-----------------------------------------------------------------------------
// Lock/Unlock item trade
//
// Item sell/buy interface는 실행되고있어도 lock되면 실제적으로 '행위'가 불가능
// 해진다.
//-----------------------------------------------------------------------------
void	C_VS_UI::LockItemTrade()
{
	if (m_pC_game)
		m_pC_game->LockItemTrade();
}

void	C_VS_UI::UnlockItemTrade()
{
	if (m_pC_game)
		m_pC_game->UnlockItemTrade();
}

//-----------------------------------------------------------------------------
// Lock/Unlock gear item
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::LockGear()
{
	if (m_pC_game)
		m_pC_game->LockGear();
}

void C_VS_UI::UnlockGear()
{
	if (m_pC_game)
		m_pC_game->UnlockGear();
}

/*-----------------------------------------------------------------------------
- AddChatToHistory
- Chat history에 str을 추가한다.
-----------------------------------------------------------------------------*/
void C_VS_UI::AddChatToHistory(char * str, char * sz_id, CHAT_LINE_CONDITION condition, DWORD color)
{
	if (m_pC_game)
		m_pC_game->AddChatToHistory(str, sz_id, condition, color);
}


/*-----------------------------------------------------------------------------
- ChangeToSlayerInterface
- Slayer interface로 바꾼다.

  이전 Interface는 free한다.
-----------------------------------------------------------------------------*/
void C_VS_UI::ChangeToSlayerInterface()
{
	if (m_pC_game)
	{
		m_pC_game->ChangeToSlayerInterface();
		CloseOption();
		DeleteNew(m_pC_option);
	}
}

/*-----------------------------------------------------------------------------
- ChangeToVampireInterface
- Vampire interface로 바꾼다.
-----------------------------------------------------------------------------*/
void C_VS_UI::ChangeToVampireInterface()
{
	if (m_pC_game)
	{
		m_pC_game->ChangeToVampireInterface();	
		CloseOption();
		DeleteNew(m_pC_option);
	}
}

//-----------------------------------------------------------------------------
// DeleteCharacter
//
// slot에 있는 character를 삭제한다.
//-----------------------------------------------------------------------------
void C_VS_UI::DeleteCharacter(int slot)
{
	if (m_pC_title)
		m_pC_title->DeleteCharacter(slot);
}

//-----------------------------------------------------------------------------
// NewCharacterCreateOk
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::NewCharacterCreateOk()
{
	if (m_pC_title)
		m_pC_title->NewCharacterCreateOk();
}

//-----------------------------------------------------------------------------
// StartCharacterManager
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::StartCharacterManager(bool back)
{
	if (m_pC_title)
		m_pC_title->StartCharacterManager(back);
}

//-----------------------------------------------------------------------------
// StartServerSelect
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::StartServerSelect(bool bGroup)
{
	if (m_pC_title)
		m_pC_title->StartServerSelect(bGroup);
}

//-----------------------------------------------------------------------------
// GetZoneID
//
//-----------------------------------------------------------------------------
int C_VS_UI::GetZoneID()
{
	if (m_pC_game)
		return m_pC_game->GetZoneID();
	return 0;
}


//-----------------------------------------------------------------------------
// SetZoneName
//
// ZoneName을 설정한다.
//-----------------------------------------------------------------------------
void C_VS_UI::SetZone(int zone_id)
{
	if (m_pC_game)
		m_pC_game->SetZone(zone_id);
}

void C_VS_UI::SetBlock(int x, int y)
{
	if (m_pC_game)
		m_pC_game->SetBlock(x,y);
}

//-----------------------------------------------------------------------------
// SetTime
//
// Time을 설정한다.
//-----------------------------------------------------------------------------
void C_VS_UI::SetTime(const char * str)
{
	if (m_pC_game)
		m_pC_game->SetTime(str);
}

//-----------------------------------------------------------------------------
// SetDate
//
// Date을 설정한다.
//-----------------------------------------------------------------------------
void C_VS_UI::SetDate(const char * str)
{
	if (m_pC_game)
		m_pC_game->SetDate(str);
}

//-----------------------------------------------------------------------------
// SetXY
//
// 맵좌표를 설정한다.
//-----------------------------------------------------------------------------
void C_VS_UI::SetXY(int x, int y)
{
	if (m_pC_game)
		m_pC_game->SetXY(x, y);
}

//-----------------------------------------------------------------------------
// SetSize
//
// map size를 설정한다.
//-----------------------------------------------------------------------------
void C_VS_UI::SetSize(SIZE size)
{
	if (m_pC_game)
		m_pC_game->SetSize(size);
}

//-----------------------------------------------------------------------------
// SetSafetyZone
//
// 미니맵 안전지대를 설정한다.
//-----------------------------------------------------------------------------
void C_VS_UI::SetSafetyZone(RECT rect, bool my_zone)
{
	if (m_pC_game)
		m_pC_game->SetSafetyZone(rect, my_zone);
}


//-----------------------------------------------------------------------------
// SetNPC
//
// 미니맵 NPC를 설정한다.
//-----------------------------------------------------------------------------
//struct MINIMAP_NPC
//{
//	int x;
//	int y;
//	int id;
//};
void C_VS_UI::SetNPC(MINIMAP_NPC npc)
{
	if (m_pC_game)
		m_pC_game->SetNPC(npc);
}

void C_VS_UI::ClearNPC()
{
	if(m_pC_game)
		m_pC_game->ClearNPC();
}

//-----------------------------------------------------------------------------
// SetNPC
//
// 미니맵 NPC를 설정한다.
//-----------------------------------------------------------------------------
void C_VS_UI::SetNPC(int x, int y, int id, const char* name)
{
	MINIMAP_NPC npc;// = {x, y, id};
	npc.x=x;
	npc.y=y;
	npc.id=id;
	npc.name = name;
	
	if (m_pC_game)
		m_pC_game->SetNPC(npc);
}

//-----------------------------------------------------------------------------
// SetPortal
//
// 미니맵 포탈을 설정한다.
//-----------------------------------------------------------------------------
void C_VS_UI::SetPortal(RECT rect, int id)
{
	if (m_pC_game)
		m_pC_game->SetPortal(rect, id);
}


//-----------------------------------------------------------------------------
// SetPortal
//
// 미니맵 포탈을 설정한다.
//-----------------------------------------------------------------------------
//void C_VS_UI::SetPortal(int x, int y)
//{
//	if (m_pC_game)
//		m_pC_game->SetPortal(x, y);
//}

//-----------------------------------------------------------------------------
// ClearAllCharacter
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::ClearAllCharacter()
{
	if (m_pC_title)
		m_pC_title->ClearAllCharacter();
}

//-----------------------------------------------------------------------------
// 
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI::MouseControl(UINT message, int x, int y)
{
#ifdef _LIB
	if(g_pEventManager->GetEventByFlag(EVENTFLAG_DENY_INPUT_MOUSE))
		return false;
#endif
	
	gpC_mouse_pointer->Set(message, x, y);

	if (message == M_MOVING)
	{
		g_descriptor_manager.Unset();
	}


	#ifdef _LIB
		if(m_pC_game != NULL && m_pC_game->IsRunningWebBrowser())
		{
			if(m_pC_game->IsInRectPointWebBrowser(x, y))
			return false;
		}
	#endif



	bool ret = gpC_window_manager->MouseControl(message, x, y);
	if(m_pC_game != NULL)m_pC_game->MouseControlExtra(message, x, y);

#ifndef _LIB
	if (gbl_game_mode && !ret)
		ShowItem_MouseControl(message, x, y);
#endif

	//
	// pick up된 Item이 있으면, Item을 바닥에 떨어뜨린다. - Game 중에서..
	//
	if (ret == false && (message == M_LEFTBUTTON_DOWN || message == M_LB_DOUBLECLICK))
	{
		if (gpC_mouse_pointer->IsPickUpItem())
		{
			gpC_base->SendMessage(UI_ITEM_DROP_TO_CLIENT, 0, 0, gpC_mouse_pointer->GetPickUpItem());
		
#ifdef _LIB
			//
			// Client에서 Item을 떨어뜨리면서 이동하는 것을 막기 위해 UI 입력으로 return 한다.
			//
			return true; // Item을 가지고 있을 때는 떨어뜨리는 것 외의 Client 입력은 없다.
#endif
		}
	}

	if (!ret && (message == M_WHEEL_UP || message == M_WHEEL_DOWN))
	{
		ChangeSkillWheel(message == M_WHEEL_UP);
	}

	if(!ret && ( message == M_CENTERBUTTON_DOWN) )
	{
		if(m_pC_game != NULL)
			m_pC_game->HotKey_Skill();
	}

	return ret;
}

void C_VS_UI::DIKeyboardControl(CDirectInput::E_KEYBOARD_EVENT event, DWORD scan_code)
{
#ifdef OUTPUT_DEBUG
//	DEBUG_ADD("[C_VS_UI] DIKeyboardControl");
#endif
#ifdef _LIB
	if(g_pEventManager != NULL && g_pEventManager->GetEventByFlag(EVENTFLAG_DENY_INPUT_KEYBOARD))
		return;
#endif

	if( g_pKeyAccelerator == NULL || g_pDXInput == NULL || g_pUserOption == NULL )
		return;
	
	if(scan_code == DIK_ESCAPE)
	{
		if(IsRunningOption())
		{
			CloseOption();

			switch(g_eRaceInterface)
			{
			case RACE_SLAYER:
				PlaySound(SOUND_SLAYER_BUTTON);
				break;

			case RACE_VAMPIRE:
				PlaySound(SOUND_VAMPIRE_BUTTON);			
				break;

			case RACE_OUSTERS:
				PlaySound(SOUND_OUSTERS_INTERFACE);
				break;
			}

#ifdef OUTPUT_DEBUG
//			DEBUG_ADD("[C_VS_UI] DIKeyboardControl ESCAPE");
#endif
			return;
		}

		if(IsRunningGameMenu())
		{
			CloseGameMenu();
			switch(g_eRaceInterface)
			{
			case RACE_SLAYER:
				PlaySound(SOUND_SLAYER_BUTTON);
				break;

			case RACE_VAMPIRE:
				PlaySound(SOUND_VAMPIRE_BUTTON);
				break;

			case RACE_OUSTERS:
				PlaySound(SOUND_OUSTERS_INTERFACE);
				break;
			}

#ifdef OUTPUT_DEBUG
//			DEBUG_ADD("[C_VS_UI] DIKeyboardControl Close Game Menu");
#endif
			return;
		}
	}

	if(gpC_window_manager != NULL && gpC_window_manager->GetTopmostWindow() != NULL && gpC_window_manager->GetTopmostWindow()->GetAttributes()->keyboard_control == true)
	{
#ifdef OUTPUT_DEBUG
//		DEBUG_ADD("[C_VS_UI] DIKeyboardControl FALSE");
#endif
		return;
	}

	if(scan_code == DIK_LCONTROL || scan_code == DIK_RCONTROL ||
		scan_code == DIK_LALT || scan_code == DIK_RALT ||
		scan_code == DIK_LSHIFT || scan_code == DIK_RSHIFT)
	{
#ifdef OUTPUT_DEBUG
//		DEBUG_ADD("[C_VS_UI]oardControl Control");
#endif
		return;
	}

	WORD pressed_key = scan_code;

	if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
	{
		pressed_key = ACCEL_ADD_CONTROL( pressed_key );

		// Control+ESC는 단축키로 사용할 수 없다
		if(scan_code == SCANCODE_ESC)
		{
#ifdef OUTPUT_DEBUG
//			DEBUG_ADD("[C_VS_UI] DIKeyboardControl Control+ESC");
#endif
			return;
		}
	}

	if (g_pDXInput->KeyDown(DIK_LALT) || g_pDXInput->KeyDown(DIK_RALT))
	{
//		pressed_key = ACCEL_ADD_ALT( pressed_key );
		
		// Alt+Tab은 단축키로 사용할 수 없다
		if(scan_code == SCANCODE_TAB)
		{
#ifdef OUTPUT_DEBUG
//			DEBUG_ADD("[C_VS_UI] DIKeyboardControl ALT+TAB");
#endif
			return;
		}
	}

	// function key는 단축키로 쓸 수 없다
	if(m_bl_accel_mode && (scan_code >= DIK_F1 && scan_code <= DIK_F10 || scan_code >= DIK_F11 && scan_code <= DIK_F12))
	{
#ifdef OUTPUT_DEBUG
//		DEBUG_ADD("[C_VS_UI] DIKeyboardControl Function Key");
#endif
		return;
	}

	if(m_bl_accel_mode &&
		(scan_code == SCANCODE_ESC)
		)
	{
		m_bl_accel_mode = false;
		m_bl_hotkey = true;
	}

	if (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL))
		pressed_key = ACCEL_ADD_CONTROL( pressed_key );

	if (g_pDXInput->KeyDown(DIK_LALT) || g_pDXInput->KeyDown(DIK_RALT))
		pressed_key = ACCEL_ADD_ALT( pressed_key );

	if (g_pDXInput->KeyDown(DIK_LSHIFT) || g_pDXInput->KeyDown(DIK_RSHIFT))
		pressed_key = ACCEL_ADD_SHIFT( pressed_key );	

	if(!IsAccelMode() && 
		(
			!g_pUserOption->UseEnterChat && !ACCEL_HAS_CONTROL(pressed_key) && !ACCEL_HAS_ALT(pressed_key)
			|| g_pUserOption->UseEnterChat && gC_vs_ui.IsInputMode() &&  !ACCEL_HAS_CONTROL(pressed_key) && !ACCEL_HAS_ALT(pressed_key)
		))
	{
		BYTE key = ACCEL_GET_KEY(pressed_key);
		if(key >= SCANCODE_1 && key <= SCANCODE_0 || 
			key >= SCANCODE_Q && key <= SCANCODE_RBRACKET ||
			key >= SCANCODE_A && key <= SCANCODE_GRAVE ||
			key >= SCANCODE_BACKSLASH && key <= SCANCODE_SLASH ||
			key == SCANCODE_SPACE)
		{
#ifdef OUTPUT_DEBUG
//			DEBUG_ADD("[C_VS_UI] DIKeyboardControl AccelMode");
#endif
			return;
		}
	}


	if(m_bl_accel_mode)
	{
		// 이미 등록된 핫키는 사용 못함
		// GetAcceleratorSimilar를 쓰면 안된다
		// Tab, Control+Tab같은게 지정 아안되기 때문이다
		BYTE accel = g_pKeyAccelerator->GetAccelerator( pressed_key );
		
		if(accel != ACCEL_NULL)
		{
#ifdef OUTPUT_DEBUG
//			DEBUG_ADD("[C_VS_UI] DIKeyboardControl Accel NULL");
#endif
			return;
		}

		g_pKeyAccelerator->SetAcceleratorKey( m_iAccel, pressed_key );

		m_bl_accel_mode = false;
		m_bl_hotkey = true;
	}
	else
	{
		BYTE accel = g_pKeyAccelerator->GetAcceleratorSimilar( pressed_key );

		// accel이 있을때!
		// 채팅모드가 아닐때는 채팅 단축키 안먹게! 
		if(accel !=0 && 
			(
				(!g_pUserOption->UseEnterChat || g_pUserOption->UseEnterChat && gC_vs_ui.IsInputMode()) || // 채팅 모드일때
				(accel != ACCEL_UNION_CHAT && accel != ACCEL_CHAT && accel != ACCEL_GUILD_CHAT && accel != ACCEL_ZONE_CHAT && accel != ACCEL_WHISPER && accel != ACCEL_PARTY_CHAT ) // 채팅 단축키가 아니고,
			)
		)
		{
			g_pAcceleratorManager->Execute( accel );
			m_bl_hotkey = true;
		}
		else
			m_bl_hotkey = false;
	}
#ifdef OUTPUT_DEBUG
//	DEBUG_ADD("[C_VS_UI] DIKeyboardControl OK");
#endif
	
}

//-----------------------------------------------------------------------------
// KeyboardControl
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::KeyboardControl(UINT message, UINT key, long extra)
{
#ifdef _LIB
	if(g_pEventManager != NULL && g_pEventManager->GetEventByFlag(EVENTFLAG_DENY_INPUT_KEYBOARD))
		return;
#endif

	if( gpC_window_manager == NULL )
		return;
	
#ifdef OUTPUT_DEBUG
//	DEBUG_ADD("[C_VS_UI] KeyboardControl");
#endif
//	const int CTRL_GARBAGE = 229;
//	if(key == CTRL_GARBAGE) return ;

#ifndef _LIB
	if (gbl_game_mode)
		ShowItem_KeyboardControl(message, key, extra);
#endif

//	if(m_bl_hotkey)return;

//	if (m_pC_title)
//	{
		gpC_window_manager->KeyboardControl(message, key, extra);
		m_bl_hotkey = false;
//		return;
//	}

//	const int CTRL_GARBAGE = 229;

//	unsigned char scan_code = SCAN_CODE(extra);

//	if(key != CTRL_GARBAGE && key != VK_CONTROL)
//	{
//		bool bPressedCtrl	= (GetKeyState(VK_CONTROL) < 0);
//		bool bPressedAlt	= (GetKeyState(VK_MENU) < 0);
//		bool bPressedShift	= (GetKeyState(VK_SHIFT) < 0);
//		
//		WORD pressed_key = scan_code;
//		
//		if (bPressedCtrl) 	pressed_key = ACCEL_ADD_CONTROL( pressed_key );
//		if (bPressedAlt) 	pressed_key = ACCEL_ADD_ALT( pressed_key );
//		if (bPressedShift) 	pressed_key = ACCEL_ADD_SHIFT( pressed_key );	
//		
//		BYTE accel = g_pKeyAccelerator->GetAcceleratorSimilar( pressed_key );
//		
//		if (message == WM_KEYDOWN && !AlreadyPress(message, extra))
//		{
//			if(g_pAcceleratorManager->Execute( accel ) == false)
//			{
//				gpC_window_manager->KeyboardControl(message, key, extra);
//			}
//		}
//		else
//		{
//			gpC_window_manager->KeyboardControl(message, key, extra);
//		}
//	}
/*

	//
	//---------
	// !문제점
	//---------
	// ctrl+space를 누르면 VK_SPACE가 오지 않는다. - 알수 없음
	// 하지만 scan code는 어느때나 유효하다. 그래서 scan code로 VK_SPACE대신
	// 사용한다. 그런데 이상하게 유독 ctrl+space 메시지만 두 번 온다.
	// 다른 것 전부를 테스트해보지는 않았지만 지금까지는 그렇다.
	//
	// 두 번 중 다른 것은 인자중 long key 값이 처음에는 25 다음에는 229라는
	// 것이다. 이 중 하나만 막는 방법이 OS상의 호환성문제를 발생시키지는 않을지
	// 알 수 없다. 테스트한 바로는 Windows ME에서는 Windows 2000에서처럼 문제없었다.
	// 이것으로보아 Windows98에서도 문제없을 것이다.
	//
	//--------------
	// hot-key 정책
	//--------------
	//
	// 하나의 key가 동시에 다른 두 가지 동작을 하는 것을 막기 위해서, hot-key로 처리되면
	// window로 가지 못하게 한다.
	//

	//
	// CTRL_GARBAGE
	//
	// scan_code를 이용하는 경우, ctrl과 함께 누르면 때로는 이 값이 먼저 들어오고 그 다음
	// 바른값이 들어온다. 이것은 한글을 입력하고 있는 도중(ImeRunning() == true)에 발견되었다.
	// 이 값을 반드시 무시해야 한다.
	//
	const int CTRL_GARBAGE = 229;
	if(key != CTRL_GARBAGE)
	{
		int a = 0;
	}

	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
			case VK_F1:
				if (!AlreadyPress(message, extra))
					HotKey_F1();
				return;

			case VK_F2:
				if (!AlreadyPress(message, extra))
					HotKey_F2();
				return;

			case VK_F3:
				if (!AlreadyPress(message, extra))
					HotKey_F3();
				return;

			case VK_F4:
				if (!AlreadyPress(message, extra))
					HotKey_F4();
				return;

			case VK_F5:
				if (!AlreadyPress(message, extra))
					HotKey_F5();
				return;

			case VK_F6:
				if (!AlreadyPress(message, extra))
					HotKey_F6();
				return;

			case VK_F7:
				if (!AlreadyPress(message, extra))
					HotKey_F7();
				return;

			case VK_F8:
				if (!AlreadyPress(message, extra))
					HotKey_F8();
				return;

			case VK_F9:
				if (!AlreadyPress(message, extra))
					HotKey_F9();
				return;

			case VK_F10:
				//HotKey_F10();
				return;

			case VK_F11:
				if (!AlreadyPress(message, extra))
					HotKey_F11();
				return;

			case VK_F12:
				if (!AlreadyPress(message, extra))
					HotKey_F12();
				return;

			case VK_ESCAPE:
				if (!AlreadyPress(message, extra))
					HotKey_ESC();
				return;

			case VK_TAB:
				if (g_GetCtrlPushState() == true)
				{
					if (!AlreadyPress(message, extra))
						HotKey_Gear();
				}
				else
				{
					if (!AlreadyPress(message, extra))
						HotKey_Inventory();
				}
				return;
		}

		unsigned char scan_code = SCAN_CODE(extra);
		if (key != CTRL_GARBAGE)
		{
			int a = 0;
		}

		switch (scan_code)
		{
		case SCANCODE_X:	//ctrl+x
			if (key != CTRL_GARBAGE)
				if (g_GetCtrlPushState() == true)
					if (!AlreadyPress(message, extra))
						HotKey_Mark();
					break;

		case SCANCODE_M:
			if (key != CTRL_GARBAGE)
				if (g_GetCtrlPushState() == true)
					if (!AlreadyPress(message, extra))
						HotKey_Minimap();
					break; // break!
					
		case SCANCODE_I:
			if (key != CTRL_GARBAGE)
				if (g_GetCtrlPushState() == true)
					if (!AlreadyPress(message, extra))
						HotKey_CharInfo();
					break; // break!
					
		case SCANCODE_S:
			if (key != CTRL_GARBAGE)
				if (g_GetCtrlPushState() == true)
					if (!AlreadyPress(message, extra))
						HotKey_Skill();
					break; // break!
					
		case SCANCODE_TILDE:
			if (key != CTRL_GARBAGE)
				if (g_GetCtrlPushState() == true)
					if (!AlreadyPress(message, extra))
						HotKey_QuickItemSlot();
					break; // break!
					
		case SCANCODE_E:
			if (key != CTRL_GARBAGE)
				if (g_GetCtrlPushState() == true)
					if (!AlreadyPress(message, extra))
						HotKey_ExtendChat();
					break;
					
		case SCANCODE_C:
			if (key != CTRL_GARBAGE)
				if (g_GetCtrlPushState() == true)
					if (!AlreadyPress(message, extra))
						HotKey_Chat();
					break;
					
		case SCANCODE_Z:
			if (key != CTRL_GARBAGE)
				if (g_GetCtrlPushState() == true)
					if (!AlreadyPress(message, extra))
						HotKey_ZoneChat();
					break;
					
		case SCANCODE_G:
			if (key != CTRL_GARBAGE)
				if (g_GetCtrlPushState() == true)
					if (!AlreadyPress(message, extra))
						HotKey_GuildChat();
					break;

		case SCANCODE_P:
			if (key != CTRL_GARBAGE)
				if (g_GetCtrlPushState() == true)
					if (!AlreadyPress(message, extra))
						HotKey_Party();
					break;
			
		case SCANCODE_H:
			if (key != CTRL_GARBAGE)
				if (g_GetCtrlPushState() == true)
					if (!AlreadyPress(message, extra))
						HotKey_Help();
					break;
			
		case SCANCODE_K:
			if (key != CTRL_GARBAGE)
				if (g_GetCtrlPushState() == true)
					if (!AlreadyPress(message, extra))
						HotKey_SkillInfo();
					break;
			
		case SCANCODE_W:
			if (key != CTRL_GARBAGE)
				if (g_GetCtrlPushState() == true)
					if (!AlreadyPress(message, extra))
						HotKey_Whisper();
					break;

			//
			// 2000.12.22.
			// ctrl(좌) + space가 WinME/Win98에서는 눌림상태로 들어온다. Win2000에서는
			// 정상적으로 들어온다. 음... 이 문제는 현재 해결할 수 없으므로 Dinput으로
			// 처리한다.				
			//---------------------------------------------------------------------------
			//			case SCANCODE_SPACE:
			//				if (key != CTRL_GARBAGE)
			//				{
			//					if (g_GetCtrlPushState() == true)
			//					{
			//						if (!AlreadyPress(message, extra))
			//							HotKey_WindowToggle();
			//						return;
			//					}
			//				}
			//				break;
			//
			//case SCANCODE_TAB:
			//	if (g_GetCtrlPushState() == true)
			//	{
			//		if (!AlreadyPress(message, extra))
			//			HotKey_Gear();
			//	}
			//	else
			//	{
			//		if (!AlreadyPress(message, extra))
			//			HotKey_Inventory();
			//	}
			//	return;
				
			//case SCANCODE_ESC:
			//	if (!AlreadyPress(message, extra))
			//		HotKey_ESC();
			//	return;
		} // 'switch (scan_code)'
	}

	//
	// ignore hot-key & all ctrl push
	//
	// WM_KEYDOWN/WM_KEYUP
	//
	if (g_GetCtrlPushState() == false)
	{
		//switch (key)
		//{
			// VK_F1 ~ test하면 몇몇 키기 안눌린다! 황당...
			
//			case VK_F1:
//				break;
//			case VK_F2:
//				break;
//			case VK_F3:
//				break;
//			case VK_F4:
//				break;
//			case VK_F5:
//				break;
//			case VK_F6:
//				break;
//			case VK_F7:
//				break;
//			case VK_F8:
//				break;
				
		//	case VK_ESCAPE:
		//		break;
		//	case VK_TAB:
		//		break;

		//	default:
				gpC_window_manager->KeyboardControl(message, key, extra);
		//}
	}
*/
#ifdef OUTPUT_DEBUG
//	DEBUG_ADD("[C_VS_UI] KeyboardControl OK");
#endif
}

//-----------------------------------------------------------------------------
// StartTitle
//
//	초기화면은 게임중에는 없어도 되는 것이므로 메모리 할당은 여기서 한다.
// 초기화면 상황이 종료되면 메모리를 해제하여야 한다.
//
// 이미 Title이 시작중에 있어도 "Server가 죽을 경우" 다시 Login 부분으로 
// 와야하기 때문에 두 번 이상 StartTitle() 할 수 있다.
//-----------------------------------------------------------------------------
void C_VS_UI::StartTitle()
{
	gC_ci->Init();
	if (gpC_base->EventOccured() == true)
		gpC_base->FinishEvent();

	EndGame();

	if(m_pC_title != NULL)
	{
		DeleteNew(m_pC_title);
	}

	m_pC_title = new C_VS_UI_TITLE;				

	m_pC_title->Start();
}

//-----------------------------------------------------------------------------
// EndTitle
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::EndTitle()
{
	if(m_pC_title != NULL)
		DeleteNew(m_pC_title);
}

//-----------------------------------------------------------------------------
// StartGame
//
// 두 번 이상 실행할 수 없다.
//-----------------------------------------------------------------------------
void C_VS_UI::StartGame()
{
	EndTitle();

	if (m_pC_game != NULL)
	{
//		m_pC_game->SetSaveSet(true);
		DeleteNew(m_pC_game);
		m_pC_game = NULL;
//		_Error(FAILED_JOB);
	}

	m_pC_game = new C_VS_UI_GAME;

//	if(!bl_load_set)
	gpC_vs_ui_window_manager->SetDefault();

	m_pC_game->Start();

#ifndef _LIB
	gbl_game_mode = true;
#endif
}

//-----------------------------------------------------------------------------
// EndGame
//
// StartGame()에서 할당한 memory를 해제하여야 한다.
//-----------------------------------------------------------------------------
void C_VS_UI::EndGame()
{
	if(m_pC_game != NULL)
		DeleteNew(m_pC_game);

	DropItem();

#ifndef _LIB
	gbl_game_mode = false;
#endif
}

//-----------------------------------------------------------------------------
// Process
//
// UI 처리부분.
//-----------------------------------------------------------------------------
void C_VS_UI::Process()
{
	gC_timer2.Execute();
	gpC_base->DispatchMessage();

	gpC_process_runner->Process();

	gpC_window_manager->Process();

	// UI only process
	if (gbl_global_empty_move)
	{
		MouseControl(M_MOVING, gpC_mouse_pointer->GetX(), gpC_mouse_pointer->GetY());
		gbl_global_empty_move = false;
	}
}


void C_VS_UI::UnselectSkill()
{
	if(m_pC_game)
		m_pC_game->UnselectSkill();
}

void C_VS_UI::ChangeSkillWheel(bool direct)
{
	if(m_pC_game)
		m_pC_game->ChangeSkillWheel(direct);
}

void C_VS_UI::SelectSkill(int id)
{
	if(m_pC_game)
		m_pC_game->SelectSkill(id);
}

//-----------------------------------------------------------------------------
// ShowItem
//
// Item pickup/drop test funtions.
// 
// -> ONLY TEST <-
//-----------------------------------------------------------------------------
#ifndef _LIB
#include "VS_UI_Mouse_pointer.h"
#include "VS_UI_Item.h"
#include "Mitem.h"
//extern MItemManager	g_item_manager;
extern MItemManager		g_item_manager;
extern int				g_item_list_size;
const int				g_item_show_x = 20;
const int				g_item_show_y = 120;
const int				g_item_show_w = 8;
const int				g_item_show_h = 4;
const int				g_item_gap	  = 90;
int						g_item_page   = 0; // 모든 아이템을 출력하기 위해서.
int						g_item_select = NOT_SELECTED;

extern bool				gbl_show_item;

void ShowItem()
{
	///// -- Test
	// Show all Item.

	if(gbl_show_item == false)return;

	int i_index;
	for (int j = 0; j < g_item_show_h; j++)
		for (int i = 0; i < g_item_show_w; i++)
		{
			i_index = g_item_page*(g_item_show_w*g_item_show_h) + j*g_item_show_w+i;
			if (i_index >= g_item_list_size)
				break;
			
			MItem* pItem = g_item_manager.GetItemToModify(i_index);

			if (pItem!=NULL)
			{
				SPRITE_ID id = pItem->GetInventoryFrameID(); // frame id = data id

				if (gpC_mouse_pointer->GetPickUpItem()) // Item을 집었다면.
				{
					if (gpC_mouse_pointer->GetPickUpItem() == pItem)
						continue; // 출력하지 않는다.
				}			

				int item_x = g_item_show_x+g_item_gap*i;
				int item_y = g_item_show_y+g_item_gap*j;
				//if (g_item_select == i_index)
				//{
					
				//}
				//else
				if(pItem->IsQuestItem())
					CIndexSprite::SetUsingColorSet(pItem->GetQuestItemColorset(), 0 );
				else
				if(pItem->IsUniqueItem())
					CIndexSprite::SetUsingColorSet(pItem->GetUniqueItemColorset(), 0);
				else
					CIndexSprite::SetUsingColorSet(pItem->GetItemOptionColorSet(), 0);
					gpC_item->Blt(item_x, item_y, id);
			}
		}
	//*/
}

void ShowItem_MouseControl(UINT message, int _x, int _y)
{
	if(gbl_show_item == false)return;

	int i_index;
	g_item_select = NOT_SELECTED;

//	gpC_mouse_pointer->Set((gbl_vampire_interface?MOUSE_CURSOR_VAMPIRE_NORMAL:MOUSE_CURSOR_SLAYER_NORMAL));

	for (int j = 0; j < g_item_show_h; j++)
		for (int i = 0; i < g_item_show_w; i++)
		{
			i_index = g_item_page*(g_item_show_w*g_item_show_h) + j*g_item_show_w+i;
			if (i_index >= g_item_list_size)
				break;

			MItem* pItem = g_item_manager.GetItemToModify(i_index);

			if (pItem!=NULL)
			{
				SPRITE_ID id = pItem->GetInventoryFrameID(); // frame id = data id

				int item_x = g_item_show_x+g_item_gap*i;
				int item_y = g_item_show_y+g_item_gap*j;

				//CSprite * p_sprite = m_pC_item->GetSprite(id);
				//if (p_sprite != NULL)
				{
					//if (p_sprite->IsColorPixel(_x-item_x, _y-item_y))
					if (_x >= item_x && _x < item_x+g_item_gap &&
						 _y >= item_y && _y < item_y+g_item_gap)
					{
						g_item_select = i_index;
						
						MItem* pSelectItem = g_item_manager.GetItemToModify( g_item_select );

						if (pSelectItem != NULL)
						{
							g_descriptor_manager.Set(DID_ITEM, gpC_mouse_pointer->GetPointerX(), gpC_mouse_pointer->GetPointerY(), (void *)pSelectItem);
						}
					}
				}
			}
		}

	if (message == M_LEFTBUTTON_DOWN || message == M_LB_DOUBLECLICK) // selection.
	{
		if (g_item_select != NOT_SELECTED && gpC_mouse_pointer->GetPickUpItem() == NULL)
		{
			MItem* pSelectItem = g_item_manager.GetItem(g_item_select);

			if (pSelectItem!=NULL)
			{
				MItem * p_item = MItem::NewItem(pSelectItem->GetItemClass());
				p_item->SetItemType( pSelectItem->GetItemType() );	
				p_item->SetItemOptionList( pSelectItem->GetItemOptionList() );
				p_item->SetID( g_ui_item_max++ );

				gpC_mouse_pointer->PickUpItem( p_item );
			}			
		}
		else if (gpC_mouse_pointer->GetPickUpItem())
		{
			MItem* pItem = gpC_mouse_pointer->GetPickUpItem();

			if (pItem!=NULL)
			{
				g_item_manager.AddItem( pItem );
			}

			gpC_mouse_pointer->DropItem();
		}
	}

	if(message == M_WHEEL_UP)
		ShowItem_KeyboardControl(WM_KEYDOWN, VK_UP, 0);

	if(message == M_WHEEL_DOWN)
		ShowItem_KeyboardControl(WM_KEYDOWN, VK_DOWN, 0);


	if (message == M_RIGHTBUTTON_DOWN) // selection.
	{
		if (g_item_select != NOT_SELECTED && gpC_mouse_pointer->GetPickUpItem() == NULL)
		{
			MItem* pSelectItem = g_item_manager.GetItemToModify( g_item_select );
			if(pSelectItem != NULL)
			{
				gC_vs_ui.RunDescDialog(DID_ITEM, (void *)pSelectItem);
			}
		}
	}

}

void ShowItem_KeyboardControl(UINT message, UINT key, long extra)
{
	if (message == WM_KEYDOWN)
	{
		switch (key)
		{
			case VK_UP:
				if (g_item_page > 0)
					g_item_page--;
				break;

			case VK_DOWN:
				if (g_item_page < g_item_list_size/(g_item_show_h*g_item_show_w))
					g_item_page++;
				break;
		}
	}
}

#endif

//-----------------------------------------------------------------------------
// Show
//
// UI 출력부분.
//-----------------------------------------------------------------------------
void C_VS_UI::Show()
{
#ifdef _LIB
	if(g_pEventManager->GetEventByFlag(EVENTFLAG_NOT_DRAW_UI) != NULL)
	{
		return;
	}
#endif
	
#ifdef OUTPUT_DEBUG
//	DEBUG_ADD("[C_VS_UI] Show");
#endif
#ifndef _LIB
	if (gbl_game_mode)
		ShowItem();
#endif

	if(IsRunningProgress())			//프로그레스 들갈때 다운되는게 있어서 윈도우 쪽이 꼬이나 해서 방식 바꿈 // 게다가 속도도 UP!
		m_pC_progress->Show();
	else
		gpC_window_manager->Show();


#ifndef _LIB
extern bool gbl_info_show;
if(gbl_info_show)
{

	// message
	char msg_buf[512];
	sprintf(msg_buf, "msg_queue = %d", gpC_base->GetMessageSize());
	g_Print(0, TEXT_LINE(8), msg_buf);

	char win_buf[100];
	sprintf(win_buf, "m_focus_window = 0x%X", gpC_window_manager->GetMouseFocusedWindow());
	g_Print(0, TEXT_LINE(9), win_buf);

	sprintf(win_buf, "event flag = %d", gpC_base->EventOccured());
	g_Print(0, TEXT_LINE(10), win_buf);

	sprintf(win_buf, "item id = %d", g_item_select);
	g_Print(0, TEXT_LINE(11), win_buf);

//	sprintf(win_buf, "elevator = %d", gC_vs_ui.IsRunningElevator());
//	g_Print(0, TEXT_LINE(12), win_buf);

	if(gpC_window_manager->GetMouseFocusedWindow())
	{
		extern int g_mouse_x, g_mouse_y;
		sprintf(win_buf, "point = (%d, %d)", g_mouse_x - gpC_window_manager->GetMouseFocusedWindow()->x, g_mouse_y - gpC_window_manager->GetMouseFocusedWindow()->y);
		g_Print(0, TEXT_LINE(11), win_buf);
	}

extern EventButton *g_EventButton;
	C_VS_UI_EVENT_BUTTON *button = (C_VS_UI_EVENT_BUTTON *)g_EventButton;

	if(button)
		sprintf(msg_buf, "EventButton(%d:%d) = %d, %d", button->GetID(), button->m_image_index, button->x, button->y);
	else
		sprintf(msg_buf, "EventButton Nothing");
	g_Print(5, TEXT_LINE(24), msg_buf);

}	

//	sprintf(win_buf, "event flag = %d", gpC_base->EventOccured());
//	g_Print(0, TEXT_LINE(10), win_buf);
#endif

	//파티원 표시 화살표
#define TILE_X 48
#define TILE_Y 24
#define WIDTH 1023
#define HEIGHT 665
#define CENTER_X 399
#define CENTER_Y 249
	if(!IsRunningProgress() && g_pParty != NULL && g_pParty->GetSize() > 0)
	{
		static C_SPRITE_PACK cursor(SPK_PARTY_CURSOR);
		static DWORD dw_prev_tickcount = GetTickCount();
		static int frame = 0;
		static const POINT point_table[8] = { {-cursor.GetWidth(0)/2, 0}, 
												{-cursor.GetWidth(10), -cursor.GetHeight(10)/2},
												{-cursor.GetWidth(20), -cursor.GetHeight(20)},
												{0, -cursor.GetHeight(30)/2}, 
												{0, 0},
												{-cursor.GetWidth(50), 0},
												{-cursor.GetWidth(60), -cursor.GetHeight(60) },
												{0, -cursor.GetHeight(70) },
											};

		if(dw_prev_tickcount+150 <= GetTickCount())
		{
			dw_prev_tickcount = GetTickCount();
			frame = (frame +1)%10;
		}

		for(int i = 0; i < g_pParty->GetSize(); i++)
		{
			PARTY_INFO *info = g_pParty->GetMemberInfo(i);
			if(!info->bInSight && info->zoneID == GetZoneID())
			{
				int __x = (info->zoneX - GetX())*TILE_X;
				int __y = (info->zoneY - GetY())*TILE_Y;
				int _x, _y;
				int direct = 0;

				if(__y == 0)
				{
					_x = min(WIDTH, max(0, __x+CENTER_X));
					_y = CENTER_Y;
					if(_x > CENTER_X)
						direct = 10;
					else
						direct = 30;
				}
				else if(__x == 0)
				{
					_y = min(HEIGHT, max(0, __y+CENTER_Y));
					_x = CENTER_X;
					if(_y > CENTER_Y)
						direct = 20;
					else
						direct = 0;
				}
				else if(__x != 0 && __y != 0)
				{

					double rate = (double)__y/(double)__x;
					if(rate*(-CENTER_X) + CENTER_Y > 0 && rate*(-CENTER_X) + CENTER_Y < HEIGHT)
					{
						if(__x > 0)
						{
							_y = rate*(CENTER_X) + CENTER_Y;
							direct = 10;
						}
						else
						{
							_y = rate*(-CENTER_X) + CENTER_Y;
							direct = 30;
						}
						_x = min(WIDTH, max(0, __x+CENTER_X));
					}
					else
					{
						if(__y > 0)
						{
							_x = (CENTER_Y)/rate+CENTER_X;
							direct = 20;
						}
						else
						{
							_x = (-CENTER_Y)/rate+CENTER_X;
							direct = 0;
						}
						_y = min(HEIGHT, max(0, __y+CENTER_Y));
					}
				}
				_x += point_table[direct/10].x;
				_y += point_table[direct/10].y;
				cursor.Blt(_x, _y, direct+frame);
//				gpC_global_resource->m_pC_assemble_box_button_spk->Blt(_x, _y, C_GLOBAL_RESOURCE::AB_SLAYER_SCROLL_TAG);
				if(gpC_mouse_pointer->GetX() > _x && gpC_mouse_pointer->GetX() < _x+20 && gpC_mouse_pointer->GetY() > _y && gpC_mouse_pointer->GetY() < _y +20)
				{
					static S_DEFAULT_HELP_STRING party_string;
					party_string.sz_main_str = info->Name;
					char xy[20];
					char zonename[20];
					wsprintf(xy, (*g_pGameStringTable)[UI_STRING_MESSAGE_ZONEINFO_XY].GetString(), info->zoneX, info->zoneY);
					party_string.sz_sub_str = xy;
					wsprintf(zonename, "%s", g_pZoneTable->Get(info->zoneID)->Name.GetString());
					party_string.sz_main_str2 = zonename;
					g_descriptor_manager.Set(DID_HELP, _x, _y, (void *)&party_string,0,0);
				}
			}
		}
	}

	if(!IsRunningProgress())
		g_descriptor_manager.Show();

#ifndef _LIB
	if (gpC_press_button)
	{
		Rect rect(gpC_press_button->x, gpC_press_button->y-20, 80, 20);

		if (gpC_base->m_p_DDSurface_back->Lock())
		{
			S_SURFACEINFO	surfaceinfo;
			SetSurfaceInfo(&surfaceinfo, gpC_base->m_p_DDSurface_back->GetDDSD());

			FillRect(&surfaceinfo, &rect, YELLOW);

			gpC_base->m_p_DDSurface_back->Unlock();
		}	

		char buf[100];
		sprintf(buf, "%dx%d", gpC_press_button->x, gpC_press_button->y);
		g_Print(rect.x+7, rect.y+2, buf);
	}
#endif
#ifdef OUTPUT_DEBUG
//	DEBUG_ADD("[C_VS_UI] Show OK");
#endif
}

//-----------------------------------------------------------------------------
// DrawMousePointer
//
// Client가 Mouse pointer를 다시 그리고 싶을 때 사용하는 Method. UI Loop와
// 불리시켰다(Client의 요구로).
//-----------------------------------------------------------------------------
void C_VS_UI::DrawMousePointer()
{
#ifdef _LIB
	if(g_pEventManager->GetEventByFlag(EVENTFLAG_NOT_DRAW_MOUSE_POINTER))
		return;
#endif
	
	gpC_mouse_pointer->Show();
}

// 2004, 10, 28, sobeit add start
void C_VS_UI::DrawTargetArrow(int TargetX, int TargetY)
{
	gpC_mouse_pointer->DrawTargetArrow(TargetX, TargetY);
}
//-----------------------------------------------------------------------------
// GetCurrentMousePointerInfo
//
// Client에서 Mouse pointer buffer를 생성하기 위한 정보를 얻는 Method.
//-----------------------------------------------------------------------------
void C_VS_UI::GetCurrentMousePointerInfo(MOUSEPOINTER_INFO &info)
{
	info.x		= gpC_mouse_pointer->GetPointerX();
	info.y		= gpC_mouse_pointer->GetPointerY();
	info.width	= gpC_mouse_pointer->GetPointerW();
	info.height = gpC_mouse_pointer->GetPointerH();
}

//-----------------------------------------------------------------------------
// C_VS_UI
//
// 
//-----------------------------------------------------------------------------
C_VS_UI::C_VS_UI()
{
	m_pC_game = NULL;
	m_pC_title = NULL;
	m_pC_progress = NULL;
	m_pC_option = NULL;
	
	m_bl_accel_mode = false;
	m_iAccel = 0;
	m_bl_hotkey = false;
	gbl_ask_go_biling_page_running = false;
	m_otherojectid = 0;
	m_mode = 0;
}

//-----------------------------------------------------------------------------
// ~C_VS_UI
//
//
//-----------------------------------------------------------------------------
C_VS_UI::~C_VS_UI()
{	
	Release();
}

//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningStorage
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningStorage() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningStorage();

	return false;
}
//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningStorage
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningPetStorage() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningPetStorage();

	return false;
}

//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningRepair
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningRepair() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningRepair();

	return false;
}

//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningSilvering
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningSilvering() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningSilvering();

	return false;
}

//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningShop
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningShop() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningShop();

	return false;
}

//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningSell
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningSell() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningSell();

	return false;
}


//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningExchange
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningExchange() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningExchange();

	return false;
}

//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningBookcase by larosel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningBookcase() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningBookcase();

	return false;
}
/*
//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningBriefing by larosel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningBriefing() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningBriefing();

	return false;
}
*/
//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningComputer by larosel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningComputer() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningComputer();

	return false;
}
/*
//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningTutorialExit by larosel
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningTutorialExit() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningTutorialExit();

	return false;
}

//-----------------------------------------------------------------------------
// RunTutorialExitAsk
//
// 그거들구 튜토리얼 나갈래?하고 묻는다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunTutorialExitAsk(const int select, const char* pName)
{
	if (m_pC_game)
		m_pC_game->RunTutorialExitAsk(select, pName);
}

//-----------------------------------------------------------------------------
// CloseTutorialExitAsk
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseTutorialExitAsk()
{
	if (m_pC_game)
		m_pC_game->CloseTutorialExitAsk();
}
*/
//-----------------------------------------------------------------------------
// C_VS_UI::RestoreWhenActivateGame
//
//
//-----------------------------------------------------------------------------
void	C_VS_UI::RestoreWhenActivateGame()
{
	if (m_pC_game)
		m_pC_game->RestoreWhenActivateGame();
}

//-----------------------------------------------------------------------------
// C_VS_UI::BackupPrevId
//
// login할 때 이전에 입력한 id를 자동으로 미리 입력시키기 위해서 file에 backup한다.
//-----------------------------------------------------------------------------
void	C_VS_UI::BackupPrevId(const char * sz_id)
{
	if( strlen( sz_id ) >= 11 )
		return;

	strcpy(g_pUserOption->BackupID, sz_id);
//	if (sz_id)
//	{
//		ofstream file(FILE_BACKUP_ID, ios::binary);
//		if (file)
//		{
//			file.write(sz_id, strlen(sz_id));
//		}
//
//		file.close();
//	}
}

//-----------------------------------------------------------------------------
// C_VS_UI::AlreadyPress
//
// 이전에 push된 상태면 true를 반환한다.
//-----------------------------------------------------------------------------
bool	C_VS_UI::AlreadyPress(UINT message, long extra) const
{
	if (message == WM_KEYDOWN)
		if ((extra&PREV_KEYSTATE) != 0)
			return true;

	return false;
}

//-----------------------------------------------------------------------------
// C_VS_UI::Release
//
// global제거를 확실한 시점에서 하기 위해서 추가..  by sigi
//-----------------------------------------------------------------------------
void
C_VS_UI::Release()
{
#ifndef _LIB
//	DeleteNew(m_p_edit_dialog);
#endif

	DeleteNew(gpC_Imm);
	gpC_Imm = NULL;
	DeleteNew(m_pC_title);
	DeleteNew(m_pC_game);
	DeleteNew(m_pC_progress);
	DeleteNew(m_pC_option);
	m_pC_progress = NULL;

	g_FreeMessage(); // Window Manager 이전에 해준다.

	DeleteNew(gpC_vs_ui_window_manager);

	DeleteNew(gpC_window_manager);
	DeleteNew(gpC_process_runner);

	DeleteNew(gpC_item);
	DeleteNew(gpC_global_resource);
	DeleteNew(gpC_mouse_pointer);

	DeleteNew(gpC_base);
	DeleteNew(g_pSkinManager);

#ifndef _LIB
	//UnInitSound();

	
#endif
}

//-----------------------------------------------------------------------------
// C_VS_UI::GetHotkey
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI::SetHotkey(C_VS_UI_SKILL::HOTKEY hotkey, ACTIONINFO id)
{
	if (m_pC_game)
		m_pC_game->SetHotkey(hotkey, id);
}

//-----------------------------------------------------------------------------
// C_VS_UI::GetHotkey
//
// 
//-----------------------------------------------------------------------------
int	C_VS_UI::GetHotkey(C_VS_UI_SKILL::HOTKEY hotkey, C_VS_UI_SKILL::HOTKEY_GRADE grade) const
{
	if (m_pC_game)
		return m_pC_game->GetHotkey(hotkey, grade);

	return NOT_SELECTED;
}

//-----------------------------------------------------------------------------
// C_VS_UI::FinishLevelUp
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI::FinishLevelUp()
{
	if (m_pC_game)
		m_pC_game->FinishLevelUp();
}

//-----------------------------------------------------------------------------
// C_VS_UI::LevelUp
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI::LevelUp()
{
	if (m_pC_game)
		m_pC_game->LevelUp();
}

//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningLevelUp
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningLevelUp()
{
	if (m_pC_game)
		return m_pC_game->IsRunningLevelUp();
	return false;
}


//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningCharInfo
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI::IsRunningCharInfo()
{
	if (m_pC_game)
		return m_pC_game->IsRunningCharInfo();

	return false;
}

bool	C_VS_UI::IsRunningGrade2Info()
{
	if (m_pC_game)
		return m_pC_game->IsRunningGrade2Info();

	return false;
}

bool	C_VS_UI::IsRunningGrade3Info()
{
	if (m_pC_game)
		return m_pC_game->IsRunningGrade3Info();

	return false;
}


bool	C_VS_UI::IsRunningGrade1Info()
{
	if (m_pC_game)
		return m_pC_game->IsRunningGrade1Info();

	return false;
}


//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningCharInfo
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI::IsRunningSkillInfo()
{
	if (m_pC_game)
		return m_pC_game->IsRunningSkillInfo();

	return false;
}

//-----------------------------------------------------------------------------
// C_VS_UI::RunCharacterInfo
//
// 
//-----------------------------------------------------------------------------
//void	C_VS_UI::RunInfo(bool skill_mode)
//{
//	if (m_pC_game)
//		m_pC_game->RunInfo(skill_mode);
//}

void	C_VS_UI::RunInfo(C_VS_UI_INFO::INFO_MODE Mode)
{
	if (m_pC_game)
		m_pC_game->RunInfo(Mode);
}

//-----------------------------------------------------------------------------
// C_VS_UI::RunOtherCharacterInfo
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI::RunOtherInfo()
{
	if (m_pC_game)
		m_pC_game->RunOtherInfo();
}


//-----------------------------------------------------------------------------
// C_VS_UI::AcquireChatting
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI::AcquireChatting()
{
	if (m_pC_game)
		m_pC_game->AcquireChatting();
}

//-----------------------------------------------------------------------------
// C_VS_UI::IsEmptyChatting
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsEmptyChatting()
{
	if (m_pC_game)
		return m_pC_game->IsEmptyChatting();
	return true;
}

/*
//-----------------------------------------------------------------------------
// C_VS_UI::DisconnectPCS
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI::DisconnectPCS(int slot)
{
	if (m_pC_game)
		m_pC_game->DisconnectPCS(slot);
}

//-----------------------------------------------------------------------------
// C_VS_UI::ConnectionFailed
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::ConnectionFailed()
{
	if (m_pC_game)
		m_pC_game->ConnectionFailed();
}
*/
//-----------------------------------------------------------------------------
// C_VS_UI::CloseUserRegistrationWindow
//
// 
//-----------------------------------------------------------------------------
//void C_VS_UI::CloseUserRegistrationWindow()
//{
//	if (m_pC_title)
//		m_pC_title->CloseUserRegistrationWindow();
//}
/*
//-----------------------------------------------------------------------------
// C_VS_UI::GrantPCSWaitRoomToDuplex
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::GrantPCSWaitRoomToDuplex(int slot)
{
	if (m_pC_game)
		m_pC_game->GrantPCSWaitRoomToDuplex(slot);
}

//-----------------------------------------------------------------------------
// C_VS_UI::GetSendPossibleSlot
//
// 
//-----------------------------------------------------------------------------
int C_VS_UI::GetSendPossibleSlot() const
{
	if (m_pC_game)
		return m_pC_game->GetSendPossibleSlot();

	return NOT_SELECTED;
}
*/
//-----------------------------------------------------------------------------
// C_VS_UI::RunNewUserRegistration
//
// 
//-----------------------------------------------------------------------------
//void C_VS_UI::RunNewUserRegistration()
//{
//	if (m_pC_title)
//		m_pC_title->RunNewUserRegistration();
//}

//-----------------------------------------------------------------------------
// C_VS_UI::CloseInfo
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseInfo()
{
	if (m_pC_game)
		m_pC_game->CloseInfo();
}


//-----------------------------------------------------------------------------
// C_VS_UI::CloseGameMenu
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseGameMenu()
{
	if (m_pC_game)
		m_pC_game->CloseGameMenu();
}

//-----------------------------------------------------------------------------
// C_VS_UI::GetInventoryPosition
//
// Client에서 Inventory의 위치를 얻음.
//-----------------------------------------------------------------------------
Point C_VS_UI::GetInventoryPosition() const
{
	if (m_pC_game)
		return m_pC_game->GetInventoryPosition();

	Point point(0, 0);
	return point;
}

//-----------------------------------------------------------------------------
// C_VS_UI::GetInventoryGridPosition
//
// Client에서 Inventory의 (x, y) grid의 위치를 얻음.
//-----------------------------------------------------------------------------
Point	C_VS_UI::GetInventoryGridPosition(int grid_x, int grid_y) const
{
	if (m_pC_game)
		return m_pC_game->GetInventoryGridPosition(grid_x, grid_y);

	Point point(0, 0);
	return point;
}

//-----------------------------------------------------------------------------
// C_VS_UI::ReplaceItemInGear
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI::ReplaceItemInGear(MItem* pItem, int slot, MItem*& pOldItem)
{
	if (m_pC_game)
		return m_pC_game->ReplaceItemInGear(pItem, slot, pOldItem);

	return false;
}

//-----------------------------------------------------------------------------
// C_VS_UI::RemoveItemInGear
//
// 
//-----------------------------------------------------------------------------
MItem * C_VS_UI::RemoveItemInGear(int slot)
{
	if (m_pC_game)
		return m_pC_game->RemoveItemInGear(slot);

	return NULL;
}

//-----------------------------------------------------------------------------
// C_VS_UI::CanReplaceItemInGear
//
// slayer/vampire gear창에서 item이 replace될 수 있는가?
//-----------------------------------------------------------------------------
bool C_VS_UI::CanReplaceItemInGear(MItem* pItem, int slot, MItem*& pOldItem)
{
	if (m_pC_game)
		return m_pC_game->CanReplaceItemInGear(pItem, slot, pOldItem);

	return false;
}

//-----------------------------------------------------------------------------
// C_VS_UI::GetGearItem
//
// slayer인지 vampire인지 모를 때 사용한다.
//-----------------------------------------------------------------------------
const MItem * C_VS_UI::GetGearItem(int slot) const
{
	if (m_pC_game)
		return m_pC_game->GetGearItem(slot);

	return NULL;
}
const MItem * C_VS_UI::GetGearItem_PickUp(int &slot) const
{
	if (m_pC_game)
		return m_pC_game->GetGearItem_PickUp(slot);

	return NULL;
}
const MItem * C_VS_UI::GetGearCoreZapItem(int slot) const // 특정 슬롯에 붙어 있는 코어잽을 리턴, slot : 코어잽 밑의 슬롯
{
	if (m_pC_game)
		return m_pC_game->GetGearCoreZapItem(slot);

	return NULL;
}
const MItem * C_VS_UI::GetGearCoreZapedItem(int slot) const // 코어잽 밑에 깔린 아이템을 리턴, slot : 코어잽 슬롯
{
	if (m_pC_game)
		return m_pC_game->GetGearCoreZapedItem(slot);

	return NULL;
}
const bool C_VS_UI::IsHasAllCoreZap(int CoreZapType) const
{
	if (m_pC_game)
		return m_pC_game->IsHasAllCoreZap(CoreZapType);

	return false;
}

const bool C_VS_UI::IsCloseBloodBibleSlot(int slot) const
{
	if (m_pC_game)
		return m_pC_game->IsCloseBloodBibleSlot(slot);

	return false;
}

//-----------------------------------------------------------------------------
// C_VS_UI::GetGearSize
//
// slayer인지 vampire인지 모를 때 사용한다.
//-----------------------------------------------------------------------------
const int C_VS_UI::GetGearSize() const
{
	if (m_pC_game)
		return m_pC_game->GetGearSize();

	return NULL;
}

//-----------------------------------------------------------------------------
// C_VS_UI::CloseAllDialog
//
// 어디에도 종속되지않는 dialog는 server disconnect message가 떳을 경우와 같은 때에
// 전부 닫아줘야 한다.
//-----------------------------------------------------------------------------
void	C_VS_UI::CloseAllDialog()
{
	if (m_pC_game)
	{
		while(m_pC_game->ClosePopupWindow());
		
		if(m_pC_game->IsRunningBloodBibleStatus())		//-_-;;
			m_pC_game->CloseBloodBibleStatus();
		
		if(m_pC_game->IsRunningCTFStatusWindow() )
			m_pC_game->CloseCTFStatusWindow();
	}

	if(gbl_ask_go_biling_page_running)
	{
		gpC_base->SendMessage(UI_GO_BILING_PAGE, FALSE);
		gbl_ask_go_biling_page_running = false;		
	}


	g_FreeMessage();
	g_InitMessage();
}
/*
//-----------------------------------------------------------------------------
// AcquireQuitPCSOnlineModeMessage
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::AcquireQuitPCSOnlineModeMessage()
{
	if (m_pC_game)
		m_pC_game->AcquireQuitPCSOnlineModeMessage();
}

//-----------------------------------------------------------------------------
// C_VS_UI::PleaseConnectPCS
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::PleaseConnectPCS(char * sz_name, long pcs_number, int slot)
{
	if (m_pC_game)
		m_pC_game->PleaseConnectPCS(sz_name, pcs_number, slot);
}

//-----------------------------------------------------------------------------
// C_VS_UI::OnLinePCS
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI::OnLinePCS(char * sz_name, long pcs_number)
{
	if (m_pC_game)
		m_pC_game->OnLinePCS(sz_name, pcs_number);
}

//-----------------------------------------------------------------------------
// C_VS_UI::AcquirePDSClosedMessage
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI::AcquirePDSClosedMessage()
{
	if (m_pC_game)
		m_pC_game->AcquirePDSClosedMessage();
}
*/
/*
//-----------------------------------------------------------------------------
// C_VS_UI::RunEnchantSkillTree
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI::RunSwordSkillTree()
{
	if (m_pC_game)
		m_pC_game->RunSwordSkillTree();
}

//-----------------------------------------------------------------------------
// C_VS_UI::RunEnchantSkillTree
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI::RunEnchantSkillTree()
{
	if (m_pC_game)
		m_pC_game->RunEnchantSkillTree();
}

//-----------------------------------------------------------------------------
// C_VS_UI::RunGunSkillTree
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI::RunGunSkillTree()
{
	if (m_pC_game)
		m_pC_game->RunGunSkillTree();
}

//-----------------------------------------------------------------------------
// C_VS_UI::RunHealSkillTree
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI::RunHealSkillTree()
{
	if (m_pC_game)
		m_pC_game->RunHealSkillTree();
}

//-----------------------------------------------------------------------------
// C_VS_UI::RunBladeSkillTree
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI::RunBladeSkillTree()
{
	if (m_pC_game)
		m_pC_game->RunBladeSkillTree();
}

//-----------------------------------------------------------------------------
// C_VS_UI::RunVampireSkillTree
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI::RunVampireSkillTree()
{
	if (m_pC_game)
		m_pC_game->RunVampireSkillTree();
}
*/
//-----------------------------------------------------------------------------
// RunDescDialog
//
// 설명창을 띄운다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunDescDialog(int type, void *ptr, void* ptr2)
{
	if (m_pC_game)
		m_pC_game->RunDescDialog(type, ptr, ptr2);
}

//-----------------------------------------------------------------------------
// CloseDescDialog
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseDescDialog()
{
	if (m_pC_game)
		m_pC_game->CloseDescDialog();
}

//-----------------------------------------------------------------------------
// RunFileDialog
//
// 설명창을 띄운다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunFileDialog(C_VS_UI_FILE_DIALOG::MODE Mode, char *type)
{
	if (m_pC_game)
		m_pC_game->RunFileDialog(Mode, type);
}

//-----------------------------------------------------------------------------
// CloseFileDialog
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseFileDialog()
{
	if (m_pC_game)
		m_pC_game->CloseFileDialog();
}

//-----------------------------------------------------------------------------
// RunElevator
//
// 엘리베이터 인터페이스를 띄운다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunElevator()
{
	if (m_pC_game)
		m_pC_game->RunElevator();
}

//-----------------------------------------------------------------------------
// CloseElevator
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseElevator()
{
	if (m_pC_game)
		m_pC_game->CloseElevator();
}

//-----------------------------------------------------------------------------
// RunBookcase
//
// 책장을 띄운다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunBookcase()
{
	if (m_pC_game)
		m_pC_game->RunBookcase();
}

//-----------------------------------------------------------------------------
// CloseBookcase
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseBookcase()
{
	if (m_pC_game)
		m_pC_game->CloseBookcase();
}

/*
//-----------------------------------------------------------------------------
// RunBriefing
//
// 브리핑을 띄운다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunBriefing()
{
	if (m_pC_game)
		m_pC_game->RunBriefing();
}

//-----------------------------------------------------------------------------
// CloseBriefing
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseBriefing()
{
	if (m_pC_game)
		m_pC_game->CloseBriefing();
}
*/
//-----------------------------------------------------------------------------
// RunComputer
//
// 컴퓨터를 띄운다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunComputer()
{
	if (m_pC_game)
		m_pC_game->RunComputer();
}

//-----------------------------------------------------------------------------
// CloseComputer
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseComputer()
{
	if (m_pC_game)
		m_pC_game->CloseComputer();
}
/*
//-----------------------------------------------------------------------------
// RunTutorialExit
//
// 튜토리얼 나가기창을 띄운다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunTutorialExit()
{
	if (m_pC_game)
		m_pC_game->RunTutorialExit();
}

//-----------------------------------------------------------------------------
// CloseTutorialExit
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseTutorialExit()
{
	if (m_pC_game)
		m_pC_game->CloseTutorialExit();
}

//-----------------------------------------------------------------------------
// RunTutorialExitAccept
//
//-----------------------------------------------------------------------------
void C_VS_UI::RunTutorialExitAccept()
{
	if(m_pC_game)
		m_pC_game->RunTutorialExitAccept();
}
*/
//-----------------------------------------------------------------------------
// RunSlayerPortal
//
// 슬레이어포탈 인터페이스를 띄운다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunSlayerPortal()
{
	if (m_pC_game)
		m_pC_game->RunSlayerPortal();
}

//-----------------------------------------------------------------------------
// CloseSlayerPortal
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseSlayerPortal()
{
	if (m_pC_game)
		m_pC_game->CloseSlayerPortal();
}

//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningSlayerPortal
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningSlayerPortal() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningSlayerPortal();

	return false;
}


//-----------------------------------------------------------------------------
// RunTeamList
//
//-----------------------------------------------------------------------------
void C_VS_UI::RunTeamList(bool ready, bool IsUnion)
{
	if (m_pC_game)
		m_pC_game->RunTeamList(ready, IsUnion);
}

//-----------------------------------------------------------------------------
// CloseTeamList
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseTeamList()
{
	if (m_pC_game)
		m_pC_game->CloseTeamList();
}

//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningTeamList
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningTeamList(bool IsUnion) const
{
	if (m_pC_game)
		return m_pC_game->IsRunningTeamList(IsUnion);

	return false;
}

//-----------------------------------------------------------------------------
// RunTeamList
//
//-----------------------------------------------------------------------------
void C_VS_UI::RunTeamMemberList()
{
	if (m_pC_game)
		m_pC_game->RunTeamMemberList();
}

//-----------------------------------------------------------------------------
// CloseTeamList
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseTeamMemberList()
{
	if (m_pC_game)
		m_pC_game->CloseTeamMemberList();
}

//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningTeamList
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningTeamMemberList() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningTeamMemberList();

	return false;
}

//-----------------------------------------------------------------------------
// RunTeamInfo
//
//-----------------------------------------------------------------------------
void C_VS_UI::RunTeamInfo(bool ready, void *info, bool IsUnion)
{
	if (m_pC_game)
		m_pC_game->RunTeamInfo(ready, info, IsUnion);
}

//-----------------------------------------------------------------------------
// CloseTeamInfo
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseTeamInfo()
{
	if (m_pC_game)
		m_pC_game->CloseTeamInfo();
}

void C_VS_UI::CloseTeamInfoDialogBox()
{
	if (m_pC_game)
		m_pC_game->CloseTeamInfoDialogBox();
}

//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningTeamInfo
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningTeamInfo() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningTeamInfo();

	return false;
}


//-----------------------------------------------------------------------------
// RunTeamMemberInfo
//
//-----------------------------------------------------------------------------
void C_VS_UI::RunTeamMemberInfo(C_VS_UI_TEAM_MEMBER_INFO::MEMBER_INFO *info)
{
	if (m_pC_game)
		m_pC_game->RunTeamMemberInfo(info);
}

//-----------------------------------------------------------------------------
// CloseTeamMemberInfo
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseTeamMemberInfo()
{
	if (m_pC_game)
		m_pC_game->CloseTeamMemberInfo();
}

//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningTeamMemberInfo
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningTeamMemberInfo() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningSlayerPortal();

	return false;
}


//-----------------------------------------------------------------------------
// RunTeamRegist
//
//-----------------------------------------------------------------------------
void C_VS_UI::RunTeamRegist(bool member, int reg_fee, int rank, char *date, char *team_name, int guild_id)
{
	if (m_pC_game)
		m_pC_game->RunTeamRegist(member, reg_fee, rank, date, team_name, guild_id);
}

//-----------------------------------------------------------------------------
// CloseTeamRegist
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseTeamRegist()
{
	if (m_pC_game)
		m_pC_game->CloseTeamRegist();
}
//-----------------------------------------------------------------------------
//add by viva
//CloseFriendChattinginfo
//-----------------------------------------------------------------------------
void C_VS_UI::CloseFriendChattingInfo(C_VS_UI_FRIEND_CHATTING_INFO* pInfo)
{
	if(m_pC_game)
		m_pC_game->CloseFriendChattingInfo(pInfo);
}
//-----------------------------------------------------------------------------
//add by viva
//OpenFriendChattingInfo
//-----------------------------------------------------------------------------
void C_VS_UI::OpenFriendChattingInfo(C_VS_UI_FRIEND_INFO::FRIEND_LIST* pList)
{
	if(m_pC_game)
		m_pC_game->OpenFriendChattingInfo(pList);
}
//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningTeamRegist
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningTeamRegist() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningTeamRegist();

	return false;
}






void C_VS_UI::RunQuickItemSlot()
{
	if(m_pC_game)
		m_pC_game->RunQuickItemSlot();
}

void C_VS_UI::CloseQuickItemSlot()
{
	if(m_pC_game)
		m_pC_game->CloseQuickItemSlot();
}

bool C_VS_UI::IsRunningQuickItemSlot()
{
	if(m_pC_game)
		return m_pC_game->IsRunningQuickItemSlot();
	return false;
}

void C_VS_UI::RunMinimap()
{
	if(m_pC_game)
		m_pC_game->RunMinimap();
}

void C_VS_UI::CloseMinimap()
{
	if(m_pC_game)
		m_pC_game->CloseMinimap();
}

bool C_VS_UI::IsRunningMinimap()
{
	if(m_pC_game)
		return m_pC_game->IsRunningMinimap();
	return false;
}


//-----------------------------------------------------------------------------
// C_VS_UI::CloseSkillTree
//
// 
//-----------------------------------------------------------------------------
//void	C_VS_UI::CloseSkillTree()
//{
//	if (m_pC_game)
//		m_pC_game->CloseSkillTree();
//}

//-----------------------------------------------------------------------------
// FinishItemSelling
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::FinishItemSelling()
{
	if (m_pC_game)
		m_pC_game->FinishItemSelling();
}

//-----------------------------------------------------------------------------
// FinishItemRepairing
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::FinishItemRepairing()
{
	if (m_pC_game)
		m_pC_game->FinishItemRepairing();
}

//-----------------------------------------------------------------------------
// FinishItemSilvering
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::FinishItemSilvering()
{
	if (m_pC_game)
		m_pC_game->FinishItemSilvering();
}

//-----------------------------------------------------------------------------
// OpenInventoryToSell
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::OpenInventoryToSell()
{
	if (m_pC_game)
		m_pC_game->OpenInventoryToSell();
}

//-----------------------------------------------------------------------------
// OpenInventoryToRepair
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::OpenInventoryToRepair()
{
	if (m_pC_game)
		m_pC_game->OpenInventoryToRepair();
}

//-----------------------------------------------------------------------------
// OpenInventoryToSilvering
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::OpenInventoryToSilvering()
{
	if (m_pC_game)
		m_pC_game->OpenInventoryToSilvering();
}

//-----------------------------------------------------------------------------
// SetShop
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::SetShop(MShop* pShop)
{
	if (m_pC_game)
		m_pC_game->SetShop(pShop);
}

//-----------------------------------------------------------------------------
// SetStorage
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::SetStorage(MStorage* pStorage)
{
	if (m_pC_game)
		m_pC_game->SetStorage(pStorage);
}
//-----------------------------------------------------------------------------
// SetStorage
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::SetPetStorage(MStorage* pStorage)
{
	if (m_pC_game)
		m_pC_game->SetPetStorage(pStorage);
}
//-----------------------------------------------------------------------------
// CloseShop
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseShop()
{
	if (m_pC_game)
		m_pC_game->CloseShop();
}

//-----------------------------------------------------------------------------
// CloseStorage
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseStorage()
{
	if (m_pC_game)
		m_pC_game->CloseStorage();
}
//-----------------------------------------------------------------------------
// CloseStorage
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::ClosePetStorage()
{
	if (m_pC_game)
		m_pC_game->ClosePetStorage();
}
//-----------------------------------------------------------------------------
// CloseExchange
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseExchange()
{
	if (m_pC_game)
		m_pC_game->CloseExchange();
}

//-----------------------------------------------------------------------------
// CloseExchangeAsk
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseExchangeAsk()
{
	if (m_pC_game)
		m_pC_game->CloseExchangeAsk();
}

//-----------------------------------------------------------------------------
// CloseExchangeCancel
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseExchangeCancel()
{
	if (m_pC_game)
		m_pC_game->CloseExchangeCancel();
}

//-----------------------------------------------------------------------------
// CloseSkillView
//
// 
//-----------------------------------------------------------------------------
//void C_VS_UI::CloseSkillView()
//{
//	if (m_pC_game)
//		m_pC_game->CloseSkillView();
//}

//-----------------------------------------------------------------------------
// RunSkillView
//
// SkillView을 띄운다.
//-----------------------------------------------------------------------------
//void C_VS_UI::RunSkillView()
//{
//	if (m_pC_game)
//		m_pC_game->RunSkillView();
//}

//-----------------------------------------------------------------------------
// GetSkillViewWindow
//
// SkillView창의 포인터를 반환한다
//-----------------------------------------------------------------------------
//C_VS_UI_SKILL_VIEW * C_VS_UI::GetSkillViewWindow()
//{
//	if (m_pC_game)
//		return m_pC_game->GetSkillViewWindow();
//	return NULL;
//}


//-----------------------------------------------------------------------------
// RunShop
//
// Shop을 띄운다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunShop()
{
	if (m_pC_game)
		m_pC_game->RunShop();
}

//-----------------------------------------------------------------------------
// RunStorage
//
// Storage을 띄운다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunStorage()
{
	if (m_pC_game)
		m_pC_game->RunStorage();
}
//-----------------------------------------------------------------------------
// RunPetStorage
//
// Storage을 띄운다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunPetStorage()
{
	if (m_pC_game)
		m_pC_game->RunPetStorage();
}

//-----------------------------------------------------------------------------
// RunStorageBuy
//
// Storage을 띄운다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunStorageBuy(int price)
{
	if (m_pC_game)
		m_pC_game->RunStorageBuy(price);
}

//-----------------------------------------------------------------------------
// RunExchangeCancel
//
// 교환취소할래?하고 묻는다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunExchangeCancel(const char* pName)
{
	if (m_pC_game)
		m_pC_game->RunExchangeCancel(pName);
}

//-----------------------------------------------------------------------------
// RunExchangeAsk
//
// 교환할래?하고 묻는다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunExchangeAsk(const char* pName)
{
	if (m_pC_game)
		m_pC_game->RunExchangeAsk(pName);
}
//------------------------------------------------------------------------------
//	RunFriendRequestAsk
//
//	add by viva
//------------------------------------------------------------------------------
void C_VS_UI::RunFriendRequestAsk(const char* pName)
{
	if(m_pC_game)
		m_pC_game->RunFriendRequestAsk(pName);
}
//-----------------------------------------------------------------------------
//	RunFriendRefuseAsk
//
//	add by viva
//-----------------------------------------------------------------------------
void C_VS_UI::RunFriendRefuseAsk(const char* pName)
{
	if(m_pC_game)
		m_pC_game->RunFriendRefuseAsk(pName);
}
//------------------------------------------------------------------------------
//	RunFriendWaitAsk
//
//	add by viva
//------------------------------------------------------------------------------
void C_VS_UI::RunFriendWaitAsk(const char* pName)
{
	if(m_pC_game)
		m_pC_game->RunFriendWaitAsk(pName);
}
//-----------------------------------------------------------------------------
//	RunFriendOK
//
//	add by viva
//----------------------------------------------------------------------------
void C_VS_UI::RunFriendOK()
{
	if(m_pC_game)
		m_pC_game->RunFriendOK();
}
//-----------------------------------------------------------------------------
//	SendAddFriend
//
//	add by viva
//-----------------------------------------------------------------------------
//void C_VS_UI::SendAddFriend()
//{
//	if(m_pC_game)
//		m_pC_game->SendAddFriend();
//}
//-----------------------------------------------------------------------------
//	RunFriendExistAsk
//
//	add by viva
//----------------------------------------------------------------------------
void C_VS_UI::RunFriendExistAsk(const char* pName)
{
	if(m_pC_game)
		m_pC_game->RunFriendExistAsk(pName);
}
//-----------------------------------------------------------------------------
//	RunFriendBlackAsk
//
//	add by viva
//-----------------------------------------------------------------------------
void C_VS_UI::RunFriendBlackAsk(const char* pName)
{
	if(m_pC_game)
		m_pC_game->RunFriendBlackAsk(pName);
}
//------------------------------------------------------------------------------
//	RunFriendDeleteAsk
//
//	add by viva
//------------------------------------------------------------------------------
void C_VS_UI::RunFriendDeleteAsk(const char* pName)
{
	if(m_pC_game)
		m_pC_game->RunFriendDeleteAsk(pName);
}




//-----------------------------------------------------------------------------
// RunExchange
//
// 교환창을 띄운다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunExchange()
{
	if (m_pC_game)
		m_pC_game->RunExchange();
}
/*
//-----------------------------------------------------------------------------
// RunOption
//
// 옵션창을 띄운다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunOption()
{
	if (m_pC_game)
		m_pC_game->RunOption();
	else if(m_pC_title)
		m_pC_title->RunOption();
}

//-----------------------------------------------------------------------------
// CloseOption
//
// 교환창을 띄운다.
//-----------------------------------------------------------------------------
void	C_VS_UI::CloseOption()
{
	if (m_pC_game)
		m_pC_game->CloseOption();
	else if(m_pC_title)
		m_pC_title->CloseOption();
}
*/
//-----------------------------------------------------------------------------
// GetOpenState
//
// 
//-----------------------------------------------------------------------------
bool C_VS_UI::GetInventoryOpenState() const
{
	if (m_pC_game)
		return m_pC_game->GetInventoryOpenState();

	return false;
}

bool C_VS_UI::GetGearOpenState() const
{
	if (m_pC_game)
		return m_pC_game->GetGearOpenState();

	return false;
}

//-----------------------------------------------------------------------------
// NewCharacterCreateFailed
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::NewCharacterCreateFailed(int error)
{
	if (m_pC_title)
		m_pC_title->NewCharacterCreateFailed(error);
}

//-----------------------------------------------------------------------------
// SetCharacter
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::SetCharacter(int slot, S_SLOT &S_slot)
{
	if (m_pC_title)
		m_pC_title->SetCharacter(slot, S_slot);

}

//-----------------------------------------------------------------------------
// ServerDisconnectMessage
//
//
//-----------------------------------------------------------------------------
void C_VS_UI::ServerDisconnectMessage()
{
	gC_ci->Init();
	CloseAllDialog();
	if (gpC_base->EventOccured() == true)
		gpC_base->FinishEvent();


	// 확실히 모든 Window를 닫기위해서..
//	if (m_pC_title)
//	{
//		DeleteNew(m_pC_title);
//	}
//	m_pC_title = new C_VS_UI_TITLE;
//	m_pC_title->Start();

	StartTitle();

	//g_msg_server_disconnect->Start();
}

//-----------------------------------------------------------------------------
// InvalidIdPasswordMessage
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::InvalidIdPasswordMessage()
{
	g_msg_invalid_id_password->Start();
}

//-----------------------------------------------------------------------------
// AleadyExistIdMessage
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::AleadyExistIdMessage()
{
	g_msg_id_aleady_exist->Start();
}

//-----------------------------------------------------------------------------
// NoAleadyExistIdMessage
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::NoAleadyExistIdMessage()
{
	g_msg_id_no_aleady_exist->Start();
}

//-----------------------------------------------------------------------------
// Invalid_SSN_Message
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::Invalid_SSN_Message()
{
	g_msg_invalid_ssn->Start();
}

//-----------------------------------------------------------------------------
// StartProgress
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::StartProgress()
{
	DeleteNew(m_pC_progress);

	m_pC_progress = new C_VS_UI_PROGRESS;

	m_pC_progress->Start();
}

//-----------------------------------------------------------------------------
// EndProgress
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::EndProgress()
{
	DeleteNew(m_pC_progress);
	m_pC_progress = NULL;
}

//-----------------------------------------------------------------------------
// SetProgress
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::SetProgress(WORD cur_val, WORD max_val)
{
	if (m_pC_progress)
		m_pC_progress->SetProgress(cur_val, max_val);
}

//-----------------------------------------------------------------------------
// ToggleESC4UI
//
// Game 중 GameMenu를 부르기 위해...
//-----------------------------------------------------------------------------
void C_VS_UI::ToggleESC4UI()
{
	if (m_pC_game)
		m_pC_game->ToggleESC4UI();
}

//-----------------------------------------------------------------------------
// ResetSlayerQuickItemSize
//
// Gear에서 belt를 교체할 경우에 실행된다.
//-----------------------------------------------------------------------------
void C_VS_UI::ResetSlayerQuickItemSize()
{
	if (m_pC_game)
		m_pC_game->ResetSlayerQuickItemSize();
}

#ifndef _LIB
void VS_UI_DialogRun(C_VS_UI_DIALOG * p_this_dialog, id_t id)
{

}
#endif
class C_VS_UI_FRIEND_INFO;
//-----------------------------------------------------------------------------
// Init
//
// 당연히 가장 먼저 실행하여야 한다.
//-----------------------------------------------------------------------------
void C_VS_UI::Init(CSpriteSurface *surface, void (*fp)(DWORD, int, int, void *))
{
	g_SetNewHandler();

	if( g_pSkinManager == NULL )
	{
		g_pSkinManager = new SkinManager;
		std::string filename;
		
		if( gC_ci->IsChinese() )
			filename = CHINESE_INTERFACE_INFO;
		else if (gC_ci->IsJapanese() )
			filename = JAPANESE_INTERFACE_INFO;
		else filename = KOREAN_INTERFACE_INFO;
		
		//assert( g_pSkinManager->LoadInformation( filename.c_str() ) );
		if ( g_pSkinManager->LoadInformation( filename.c_str() ) == false )
			assert( FALSE );			
	}

#ifndef _LIB
	g_pMoneyManager->SetMoney(2000000000);
#endif	

	gpC_base = new Base;
	gpC_base->Init(surface, fp);

	//-----------------------------------------------
	// global object allocation
	//-----------------------------------------------

	//
	// Process runner
	//
	gpC_process_runner = new PI_ProcessRunner;

	//
	// Window manager
	//
	gpC_window_manager = new WindowManager;
	gpC_window_manager->SetAcquireMouseFocusHandler(_AcquireMouseFocusHandler);
	gpC_window_manager->SetAcquireDisappearHandler(_AcquireDisappearHandler);
	gpC_window_manager->SetUnacquireMouseFocusHandler(_UnacquireMouseFocusHandler);

	gpC_vs_ui_window_manager = new C_VS_UI_WINDOW_MANAGER;

	gpC_global_resource = new C_GLOBAL_RESOURCE;

	gpC_mouse_pointer = new C_VS_UI_MOUSE_POINTER;


	gpC_item = new C_VS_UI_ITEM;
	
	g_interface_blink_tid = gC_timer2.Add(INTERFACE_BLINK_MILLISEC, _Timer_InterfaceBlink);
	gC_timer2.Continue(g_interface_blink_tid);
	g_InitMessage();
	g_InstallDescriptor();

#ifndef _LIB
	g_pUserOption->UseForceFeel = true;
#endif

	if(gpC_Imm != NULL)
	{
		delete gpC_Imm;
		gpC_Imm = NULL;
	}
	gpC_Imm = new CImm;
	if(gpC_Imm && g_pUserOption->UseForceFeel)gpC_Imm->Enable();
	else if(gpC_Imm)gpC_Imm->Disable();

#ifndef _LIB
//	InitSound();

	//---------------------------------------------------------------------
	//
	// Skill Tree 전체를 초기화 하는 방법
	//
	//---------------------------------------------------------------------
//	g_pSkillManager->Init();

	//---------------------------------------------------------------------
	// 기본 기술로부터 skill tree를 초기화한다.
	//---------------------------------------------------------------------
//	(*g_pSkillManager)[SKILLDOMAIN_BLADE].SetRootSkill( SKILL_SINGLE_BLOW );
//	(*g_pSkillManager)[SKILLDOMAIN_SWORD].SetRootSkill( SKILL_DOUBLE_IMPACT );
//	(*g_pSkillManager)[SKILLDOMAIN_GUN].SetRootSkill( SKILL_SNIPPING );
//	(*g_pSkillManager)[SKILLDOMAIN_GUN].SetRootSkill( SKILL_FAST_RELOAD );
//	(*g_pSkillManager)[SKILLDOMAIN_ENCHANT].SetRootSkill( MAGIC_CREATE_HOLY_WATER );
//	(*g_pSkillManager)[SKILLDOMAIN_HEAL].SetRootSkill( MAGIC_CURE_LIGHT_WOUNDS );
//	(*g_pSkillManager)[SKILLDOMAIN_VAMPIRE].SetRootSkill( MAGIC_HIDE );	
#endif

#ifndef _LIB
//	m_p_edit_dialog = NULL;//new C_VS_UI_MONEY_DIALOG(100, 100, 1, 0, VS_UI_DialogRun, DIALOG_OK|DIALOG_CANCEL, 10);

	
	//m_p_edit_dialog->Start();
#endif
}

//-----------------------------------------------------------------------------
// C_VS_UI::RequestDie
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI::RequestDie(DWORD timer)
{
	if (m_pC_game)
		m_pC_game->RequestDie(timer);
}

//-----------------------------------------------------------------------------
// C_VS_UI::FinishRequestDie
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI::FinishRequestDie()
{
	if (m_pC_game)
		m_pC_game->FinishRequestDie();
}

//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningRequestDie
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningRequestDie() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningRequestDie();
	return false;
}


// party
//-----------------------------------------------------------------------------
// C_VS_UI::RequestParty
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI::RequestParty(const char *name, DWORD timer)
{
	if (m_pC_game)
		m_pC_game->RequestParty(name, timer);
}

//-----------------------------------------------------------------------------
// C_VS_UI::FinishRequestParty
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI::FinishRequestParty()
{
	if (m_pC_game)
		m_pC_game->FinishRequestParty();
}

//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningRequestParty
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningRequestParty() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningRequestParty();
	return false;
}

bool	C_VS_UI::IsRunningAskParty()	const
{
	if(m_pC_game)
		return m_pC_game->IsRunningPartyAsk();

	return false;
}

//-----------------------------------------------------------------------------
// RunPartyAsk
//
// 파티에참가할래?하고 묻는다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunPartyAsk(const char* pName, C_VS_UI_REQUEST_PARTY::REQUEST_PARTY type)
{
	if (m_pC_game)
		m_pC_game->RunPartyAsk(pName, type);
}

//-----------------------------------------------------------------------------
// ClosePartyAsk
//
//-----------------------------------------------------------------------------
void C_VS_UI::ClosePartyAsk()
{
	if (m_pC_game)
		m_pC_game->ClosePartyAsk();
}

//-----------------------------------------------------------------------------
// RunUsePetFood
//
// 파티에참가할래?하고 묻는다.
//-----------------------------------------------------------------------------
#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관

	void C_VS_UI::RunUsePetFood(DWORD UsingObjectID, MItem* SubInventory)
	{
		if (m_pC_game)
			m_pC_game->RunUsePetFood(UsingObjectID, SubInventory);
	}

#else

	void C_VS_UI::RunUsePetFood()
	{
		if (m_pC_game)
			m_pC_game->RunUsePetFood();
	}

#endif


//-----------------------------------------------------------------------------
// CloseUsePetFood
//
//-----------------------------------------------------------------------------
void C_VS_UI::CloseUsePetFood()
{
	if (m_pC_game)
		m_pC_game->CloseUsePetFood();
}
//-----------------------------------------------------------------------------
// RunKeepPetItemDialog
//
// 펫 맞길래??하고 묻는다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunKeepPetItemDialog()
{
	if (m_pC_game)
		m_pC_game->RunKeepPetItemDialog();
}

//-----------------------------------------------------------------------------
// CloseKeepPetItemDialog
//
//-----------------------------------------------------------------------------
void C_VS_UI::CloseKeepPetItemDialog()
{
	if (m_pC_game)
		m_pC_game->CloseKeepPetItemDialog();
}
//-----------------------------------------------------------------------------
// RunGetKeepPetItemDialog
//
// 펫 찾을래??하고 묻는다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunGetKeepPetItemDialog()
{
	if (m_pC_game)
		m_pC_game->RunGetKeepPetItemDialog();
}

//-----------------------------------------------------------------------------
// CloseGetKeepPetItemDialog
//
//-----------------------------------------------------------------------------
void C_VS_UI::CloseGetKeepPetItemDialog()
{
	if (m_pC_game)
		m_pC_game->CloseGetKeepPetItemDialog();
}
//-----------------------------------------------------------------------------
// RunEnchant
//
// Enchant할래?하고 묻는다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunEnchant(int value)
{
	if (m_pC_game)
		m_pC_game->RunEnchant(value);
}

//-----------------------------------------------------------------------------
// CloseEnchant
//
//-----------------------------------------------------------------------------
void C_VS_UI::CloseEnchant()
{
	if (m_pC_game)
		m_pC_game->CloseEnchant();
}

//-----------------------------------------------------------------------------
// RunNoSearchResult
//
// 검색결과 없음 확인창
//-----------------------------------------------------------------------------
void C_VS_UI::RunNoSearchResult()
{
	if (m_pC_game)
		m_pC_game->RunNoSearchResult();
}

//-----------------------------------------------------------------------------
// CloseNoSearchResult
//
//-----------------------------------------------------------------------------
void C_VS_UI::CloseNoSearchResult()
{
	if (m_pC_game)
		m_pC_game->CloseNoSearchResult();
}

//-----------------------------------------------------------------------------
// RunDepositLimit
//
// 검색결과 없음 확인창
//-----------------------------------------------------------------------------
void C_VS_UI::RunDepositLimit()
{
	if (m_pC_game)
		m_pC_game->RunDepositLimit();
}

//-----------------------------------------------------------------------------
// CloseDepositLimit
//
//-----------------------------------------------------------------------------
void C_VS_UI::CloseDepositLimit()
{
	if (m_pC_game)
		m_pC_game->CloseDepositLimit();
}

//-----------------------------------------------------------------------------
// RunWithdrawLimit
//
// 검색결과 없음 확인창
//-----------------------------------------------------------------------------
void C_VS_UI::RunWithdrawLimit()
{
	if (m_pC_game)
		m_pC_game->RunWithdrawLimit();
}

//-----------------------------------------------------------------------------
// CloseWithdrawLimit
//
//-----------------------------------------------------------------------------
void C_VS_UI::CloseWithdrawLimit()
{
	if (m_pC_game)
		m_pC_game->CloseWithdrawLimit();
}

void	C_VS_UI::RunBringFeeLimit()
{
	if(m_pC_game)
		m_pC_game->RunBringFeeLimit();
}

//-----------------------------------------------------------------------------
// RunPartyCancel
//
// 파티에참가신청중... 하고 나온다
//-----------------------------------------------------------------------------
void C_VS_UI::RunPartyCancel(const char* pName)
{
	if (m_pC_game)
		m_pC_game->RunPartyCancel(pName);
}

//-----------------------------------------------------------------------------
// ClosePartyCancel
//
//-----------------------------------------------------------------------------
void C_VS_UI::ClosePartyCancel()
{
	if (m_pC_game)
		m_pC_game->ClosePartyCancel();
}

//-----------------------------------------------------------------------------
// ClosePartyManager
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::ClosePartyManager()
{
	if (m_pC_game)
		m_pC_game->ClosePartyManager();
}

//-----------------------------------------------------------------------------
// RunPartyManager
//
// PartyManager을 띄운다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunPartyManager()
{
	if (m_pC_game)
		m_pC_game->RunPartyManager();
}

//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningPartyManager
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningPartyManager() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningPartyManager();

	return false;
}

void C_VS_UI::CloseOption()
{
	if (!m_pC_option)
		return;

	DeleteNew(m_pC_option);
//	m_pC_option = NULL;

//	m_pC_option->Finish();

	//assert(m_pC_tribe_interface != NULL);
	//m_pC_tribe_interface->DoCommonActionAfterEventOccured();	

//	gbl_option_running = false;
	gC_vs_ui.AcquireChatting();
}

//-----------------------------------------------------------------------------
// RunOption
//
// 옵션창 띄우기
//-----------------------------------------------------------------------------
void C_VS_UI::RunOption(bool IsTitle)
{
	//gC_vs_ui.CloseGameMenu();

	if (m_pC_option == NULL)
	{
		m_pC_option = new C_VS_UI_OPTION(IsTitle);
	}

	// center

	assert(m_pC_option != NULL);

//	gbl_option_running = true;

	m_pC_option->Start();	
}

void	C_VS_UI::CloseOtherInfo()
{
	if (m_pC_game)
		m_pC_game->CloseOtherInfo();	
}

void	C_VS_UI::RunTraceWindow()
{
	if (m_pC_game)
		m_pC_game->RunTraceWindow();
}

void	C_VS_UI::CloseTraceWindow()
{
	if (m_pC_game)
		m_pC_game->CloseTraceWindow();
}

bool	C_VS_UI::IsRunningTraceWindow() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningTraceWindow();

	return false;
}

bool	C_VS_UI::IsStartTrace()
{
	if (m_pC_game)
		return m_pC_game->IsStartTrace();

	return false;
}
void	C_VS_UI::SetCannotTrace()
{
	if (m_pC_game)
		m_pC_game->SetCannotTrace();
}

void	C_VS_UI::RunXmasCardWindow(const MItem *pItem)
{
	if (m_pC_game)
		m_pC_game->RunXmasCardWindow(pItem);
}

void	C_VS_UI::CloseXmasCardWindow()
{
	if (m_pC_game)
		m_pC_game->CloseXmasCardWindow();
}

bool	C_VS_UI::IsRunningXmasCardWindow() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningXmasCardWindow();

	return false;
}


void	C_VS_UI::RunBringFeeWindow(UINT totalfee, UINT bringfee) 
{ 
	if(m_pC_game) 
		m_pC_game->RunBringFeeWindow(totalfee,bringfee);
}
void	C_VS_UI::CloseBringFeeWindow() 
{ 
	if(m_pC_game) 
		m_pC_game->CloseBringFeeWindow(); 
}

bool	C_VS_UI::IsRunningBringFeeWindow() 
{ 
	if(m_pC_game) 
		return m_pC_game->IsRunningBringFeeWindow(); 
	
	return false; 
}

void	C_VS_UI::SetTotalFee(UINT fee) 
{ 
	if(m_pC_game) 
		m_pC_game->SetTotalFee(fee); 
}

UINT	C_VS_UI::GetBringFee()
{ 
	if(m_pC_game) 
		return m_pC_game->GetWillBringFee();

	return 0;
}

void	C_VS_UI::SetBringFee(UINT fee)
{
	if(m_pC_game)
		m_pC_game->SetBringFee(fee);
}

UINT	C_VS_UI::GetTotalFee()
{
	if(m_pC_game == NULL)
		return 0;

	return m_pC_game->GetTotalFee();
}

void	C_VS_UI::RunWarListWindow()
{
	if(m_pC_game)
		m_pC_game->RunWarListWindow();
}

void	C_VS_UI::CloseWarListWindow()
{
	if(m_pC_game)
		m_pC_game->CloseWarListWindow();
}

bool	C_VS_UI::IsRunningWarListWindow() const
{
	if(m_pC_game)
		return m_pC_game->IsRunningWarListWindow();

	return false;
}

void	C_VS_UI::AddWarList(C_VS_UI_WAR_LIST::WarInfo &war_info)
{
	if(m_pC_game)
		m_pC_game->AddWarList(war_info);
}

void	C_VS_UI::ClearWarList()
{
	if(m_pC_game)
		m_pC_game->ClearWarList();
}

void	C_VS_UI::RunBloodBibleStatus()
{
	if(m_pC_game)
		m_pC_game->RunBloodBibleStatusWindow();
}

void	C_VS_UI::CloseBloodBibleStatus()
{
	if(m_pC_game)
		m_pC_game->CloseBloodBibleStatus();
}

bool	C_VS_UI::IsRunningBloodBibleStatus()
{
	if(m_pC_game)
		return m_pC_game->IsRunningBloodBibleStatus();

	return false;
}

void	C_VS_UI::SetBloodBibleStatus(int& blood_bible_id, int& zone_id, int& x,int& y, C_VS_UI_BLOOD_BIBLE_STATUS::BLOOD_BIBLE_STATUS_INFO status, const char *name)
{
	if(m_pC_game)
	{
		C_VS_UI_BLOOD_BIBLE_STATUS::BLOOD_BIBLE_STATUS bbs;

		bbs.zone_id = zone_id;
		bbs.x = x;
		bbs.y = y;
		bbs.name = name;
		bbs.status = status;
		
		m_pC_game->SetBloodBibleStatus(blood_bible_id, bbs);
	}
}

void	C_VS_UI::SetBloodBibleStatus(int& blood_bible_id, C_VS_UI_BLOOD_BIBLE_STATUS::BLOOD_BIBLE_STATUS &blood_bible_status)
{
	if(m_pC_game)
		m_pC_game->SetBloodBibleStatus(blood_bible_id, blood_bible_status);
}

void	C_VS_UI::UnSetBloodBibleStatus(int blood_bible_id)
{
	if(m_pC_game)
		m_pC_game->UnSetBloodBibleStatus(blood_bible_id);
}


void	C_VS_UI::RunInputNameWindow(C_VS_UI_INPUT_NAME::INPUT_NAME_MODE_LIST mode )
{
	if(m_pC_game)
		m_pC_game->RunInputNameWindow( mode );
}

void	C_VS_UI::CloseInputNameWindow()
{
	if(m_pC_game)
		m_pC_game->CloseInputNameWindow();
}

bool	C_VS_UI::IsRunningInputNameWindow()
{
	if(m_pC_game == NULL)
		return false;

	return m_pC_game->IsRunningInputNameWindow();
}

const char*	C_VS_UI::GetCurrentInputName()
{
	if(m_pC_game == NULL)
		return NULL;

	return m_pC_game->GetCurrentInputName();
}

void	C_VS_UI::SetInputNameWindow(const char* name)
{
	if(m_pC_game == NULL)
		return;
	
	m_pC_game->SetInputNameWindow(name);
}

void	C_VS_UI::SetEnableInputName()
{
	if(m_pC_game != NULL)
		m_pC_game->SetEnableInputName();
}

void	C_VS_UI::SetDisableInputName()
{
	if(m_pC_game != NULL)
		m_pC_game->SetDisableInputName();
}

void	C_VS_UI::RunCannotPlayMessage()
{
	if(m_pC_title == NULL)
		return;
	
	m_pC_title->RunCannotPlayMessage();
}

void	C_VS_UI::RunCannotPlayByAttrMessage()
{
	if(m_pC_title == NULL)
		return;
	
	m_pC_title->RunCannotPlayByAttrMessage();
}

void	C_VS_UI::RunPopupMessage(const char *str, C_VS_UI_POPUP_MESSAGE::POPUP_TYPE type)
{
	if(m_pC_game != NULL)
		m_pC_game->RunPopupMessage( str, type );
}

void	C_VS_UI::RunPopupMessage( C_VS_UI_POPUP_MESSAGE::PopupWindowStyle& style )
{
	if(m_pC_game != NULL)
		m_pC_game->RunPopupMessage( style );
}

void	C_VS_UI::ClosePopupMessage()
{
	if(m_pC_game != NULL)
		m_pC_game->ClosePopupMessage();
}

bool	C_VS_UI::IsRunningPopupMessage()
{
	if(m_pC_game != NULL)
		return m_pC_game->IsRunningPopupMessage();

	return false;
}

// 퀘스트 정보창
void	C_VS_UI::RunQuestStatusWindow()
{
	if(m_pC_game != NULL)
		m_pC_game->RunQuestStatusWindow();
}

void	C_VS_UI::CloseQuestStatusWindow()
{
	if(m_pC_game != NULL)
		m_pC_game->CloseQuestStatusWindow();
}

bool	C_VS_UI::IsRunningQuestStatusWindow()
{
	if(m_pC_game != NULL)
		return m_pC_game->IsRunningQuestStatusWindow();

	return false;
}

void	C_VS_UI::SetQuestStatus(C_VS_UI_QUEST_STATUS::QUEST_STATUS &status)
{
	if(m_pC_game)
		m_pC_game->SetQuestStatus(status);
}

void	C_VS_UI::SetQuestStatusInit()
{
	if(m_pC_game)
		m_pC_game->SetQuestStatusInit();
}

DWORD	C_VS_UI::GetQuestStatusID()
{
	if(m_pC_game)
		return m_pC_game->GetQuestStatusID();
	return 0xffffffff;
}

// 복권창
void	C_VS_UI::RunLotteryCard( int& step )
{
	if(m_pC_game != NULL)
		m_pC_game->RunLotteryCard( step );
}

void	C_VS_UI::CloseLotteryCard()
{
	if(m_pC_game != NULL)
		m_pC_game->CloseLotteryCard();
}

bool	C_VS_UI::IsRunningLotteryCard()
{
	if(m_pC_game != NULL)
		return m_pC_game->IsRunningLotteryCard();

	return false;
}

void	C_VS_UI::RunItemListWindow()
{
	if(m_pC_game)
		m_pC_game->RunItemListWindow();
}

bool	C_VS_UI::IsRunningItemListWindow()
{
	if(m_pC_game)
		return m_pC_game->IsRunningItemListWindow();

	return false;
}

void	C_VS_UI::CloseItemListWindow()
{
	if(m_pC_game)
		m_pC_game->CloseItemListWindow();
}

bool	C_VS_UI::AddItemToItemList(C_VS_UI_ITEM_LIST::ItemList pItem)
{
	if(m_pC_game)
		return m_pC_game->AddItemToItemList( pItem );

	return false;
}

void	C_VS_UI::RunImageNotice()
{
	if(m_pC_game)
		m_pC_game->RunImageNotice( );
}

void	C_VS_UI::CloseImageNotice()
{
	if(m_pC_game)
		m_pC_game->CloseImageNotice();
}

bool	C_VS_UI::IsRunningImageNotice()
{
	if(m_pC_game)
		return m_pC_game->IsRunningImageNotice();
	return false;
}

void	C_VS_UI::AddNotice(const char *name, DWORD& id)
{
	if(m_pC_game)
		m_pC_game->AddNotice( name, id );
}

bool	C_VS_UI::RemoveItemInItemList( DWORD ID )
{
	if(m_pC_game)
		return m_pC_game->RemoveItemInItemList( ID );

	return false;
}

void	C_VS_UI::RunBulletinBoardWindow(const MItem *pItem)
{
	if (m_pC_game)
		m_pC_game->RunBulletinBoardWindow(pItem);
}

void	C_VS_UI::CloseBulletinBoardWindow()
{
	if (m_pC_game)
		m_pC_game->CloseBulletinBoardWindow();
}

bool	C_VS_UI::IsRunningBulletinBoardWindow() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningBulletinBoardWindow();

	return false;
}

void	C_VS_UI::RunTransItem()
{
	if (m_pC_game)
		m_pC_game->RunTransItem();
}

void	C_VS_UI::CloseTransItem()
{
	if ( m_pC_game) 
		m_pC_game->CloseTransItem();
}

//-----------------------------------------------------------------------------
// C_VS_UI::RequestDie
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI::RequestResurrect(bool f1, bool f2, bool eternity, bool IsSiegeAttacker)
{
	if (m_pC_game)
		m_pC_game->RequestResurrect(f1,f2,eternity,IsSiegeAttacker);
}

void	C_VS_UI::SetDelayRequestResurrect( C_VS_UI_REQUEST_RESURRECT::RESURRECT_MODE mode, int delay)
{
	if( m_pC_game )
		m_pC_game->SetDelayRequestResurrect( mode, delay );
}

//-----------------------------------------------------------------------------
// C_VS_UI::FinishRequestDie
//
// 
//-----------------------------------------------------------------------------
void	C_VS_UI::FinishRequestResurrect()
{
	if (m_pC_game)
		m_pC_game->FinishRequestResurrect();
}

//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningRequestDie
//
// 
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningRequestResurrect() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningRequestResurrect();
	return false;
}

/*-----------------------------------------------------------------------------
- ChangeToOustersInterface
- Ousters interface로 바꾼다.
-----------------------------------------------------------------------------*/
void C_VS_UI::ChangeToOustersInterface()
{
	if (m_pC_game)
	{
		m_pC_game->ChangeToOustersInterface();	
		CloseOption();
		DeleteNew(m_pC_option);
	}
}

//-----------------------------------------------------------------------------
// RunMixingForge
//
// MixingForge을 띄운다.
//-----------------------------------------------------------------------------
void C_VS_UI::RunMixingForge(C_VS_UI_MIXING_FORGE::FORGE_CLASS forge_class, C_VS_UI_MIXING_FORGE::FORGE_TYPE forge_type)
{
	if (m_pC_game)
		m_pC_game->RunMixingForge(forge_class, forge_type);
}

//-----------------------------------------------------------------------------
// C_VS_UI::IsRunningMixingForge
//
//
//-----------------------------------------------------------------------------
bool	C_VS_UI::IsRunningMixingForge() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningMixingForge();

	return false;
}

//-----------------------------------------------------------------------------
// CloseMixingForge
//
// 
//-----------------------------------------------------------------------------
void C_VS_UI::CloseMixingForge()
{
	if (m_pC_game)
		m_pC_game->CloseMixingForge();
}

void	C_VS_UI::RunRemoveOptionFromRareItem(const MItem* pItem, const MItem* pItem2)
{
	if( m_pC_game )
		m_pC_game->RunRemoveOptionFromRareItem( pItem, pItem2);
}

bool	C_VS_UI::IsRunningRemoveOptionFromRareItem() const
{
	if( m_pC_game )
		return m_pC_game->IsRunningRemoveOptionFromRareItem();

	return false;
}

void	C_VS_UI::CloseRemoveOptionFromRareItem()
{
	if( m_pC_game)
		m_pC_game->CloseRemoveOptionFromRareItem();
}

///////////////////////////////////////////////////////
//
// Ousters Skill Info
//
///////////////////////////////////////////////////////
void	C_VS_UI::RunOustersSkillInfo(int skillID, int window_x, int window_y, bool flag)
{
	if( m_pC_game )
		m_pC_game->RunOustersSkillInfo( skillID, window_x, window_y, flag );
}

bool	C_VS_UI::IsRunningOustersSkillInfo() const
{
	if( m_pC_game )
		return m_pC_game->IsRunningOustersSkillInfo();
	
	return false;
}

void	C_VS_UI::CloseOustersSkillInfo()
{
	if( m_pC_game)
		m_pC_game->CloseOustersSkillInfo();
}

void C_VS_UI::ResetOustersQuickItemSize()
{
	if (m_pC_game)
		m_pC_game->ResetOustersQuickItemSize();
}

void	C_VS_UI::RunHorn(int currentZoneID)
{
	if( m_pC_game )
		m_pC_game->RunHorn( currentZoneID );
}

bool	C_VS_UI::IsRunningHorn() const
{
	if( m_pC_game )
		return m_pC_game->IsRunningHorn();
	
	return false;
}

void	C_VS_UI::CloseHorn()
{
	if( m_pC_game)
		m_pC_game->CloseHorn();
}

void	C_VS_UI::RunMailBox()
{
	if( m_pC_game )
		m_pC_game->RunMailBox();
}

bool	C_VS_UI::IsRunningMailBox() const
{
	if( m_pC_game )
		return m_pC_game->IsRunningMailBox();
	
	return false;
}

void	C_VS_UI::CloseMailBox()
{
	if( m_pC_game)
		m_pC_game->CloseMailBox();
}

void	C_VS_UI::CloseMailBoxDialogBox()
{
	if( m_pC_game)
		m_pC_game->CloseMailBoxDialogBox();
}

void	C_VS_UI::AddMail(int tab_id, DWORD id, SIZE windowSize, const char * sender, const char * title, DWORD date, bool open, int guildid, int guildtype)
{
	if(m_pC_game)
		m_pC_game->AddMail(tab_id, id, windowSize, sender, title, date, open,guildid, guildtype);
}

void	C_VS_UI::AddMailContents(int tab_id, DWORD id, const char * contents)
{
	if(m_pC_game)
		m_pC_game->AddMailContents(tab_id, id, contents);
}

void	C_VS_UI::AddHelpMail(DWORD id, bool open)
{
	if(m_pC_game)
		m_pC_game->AddHelpMail(id, open);
}

void	C_VS_UI::DeleteMail(int tab_id, DWORD id)
{
	if(m_pC_game)
		m_pC_game->DeleteMail(tab_id, id);
}

void	C_VS_UI::RunFindingMine()
{
	if( m_pC_game )
		m_pC_game->RunFindingMine();
}

bool	C_VS_UI::IsRunningFindingMine() const
{
	if( m_pC_game )
		return m_pC_game->IsRunningFindingMine();
	
	return false;
}

void	C_VS_UI::CloseFindingMine()
{
	if( m_pC_game)
		m_pC_game->CloseFindingMine();
}

void	C_VS_UI::SetOustersSkillDown()
{
	if( m_pC_game )
		m_pC_game->SetOustersDownSkill();
}


void	C_VS_UI::RunArrowTile()
{
	if( m_pC_game )
		m_pC_game->RunArrowTile();
}

bool	C_VS_UI::IsRunningArrowTile() const
{
	if( m_pC_game )
		return m_pC_game->IsRunningArrowTile();
	
	return false;
}

void	C_VS_UI::CloseArrowTile()
{
	if( m_pC_game )
		m_pC_game->CloseArrowTile();
}

void	C_VS_UI::RunCrazyMine()
{
	if( m_pC_game )
		m_pC_game->RunCrazyMine();
}

bool	C_VS_UI::IsRunningCrazyMine() const
{
	if( m_pC_game )
		return m_pC_game->IsRunningCrazyMine();
	return false;
}

void	C_VS_UI::CloseCrazyMine()
{
	if( m_pC_game )
		m_pC_game->CloseCrazyMine();
}

void	C_VS_UI::SetMiniGameScore(BYTE& Type,std::string& topname, WORD& topscore, WORD& myscore)
{
	if( m_pC_game )
		m_pC_game->SetMiniGameScore(Type, topname, topscore, myscore);
}

void	C_VS_UI::RunQuestInventory(const MItem* pItem)
{
	if( pItem == NULL )
		return;
	
	if( m_pC_game )
		m_pC_game->RunQuestInventory( pItem );
}

bool	C_VS_UI::IsRunningQuestInventory() const
{
	if( m_pC_game )
		return m_pC_game->IsRunningQuestInventory();

	return false;
}

void	C_VS_UI::CloseQuestInventory()
{
	if( m_pC_game )
		m_pC_game->CloseQuestInventory();
}

void	C_VS_UI::RunCTFStatusWindow()
{
	if( m_pC_game )
		m_pC_game->RunCTFStatusWindow();
}

void	C_VS_UI::CloseCTFStatusWindow()
{
	if( m_pC_game )
		m_pC_game->CloseCTFStatusWindow();
}

bool	C_VS_UI::IsRunningCTFStatusWindow()
{
	if( m_pC_game )
		return m_pC_game->IsRunningCTFStatusWindow();

	return false;
}

void	C_VS_UI::SetCTFStatus(DWORD &endtime, int &flag_s, int &flag_v, int &flag_o)
{
	if( m_pC_game )
		m_pC_game->SetCTFStatus( endtime, flag_s, flag_v, flag_o );
}

void	C_VS_UI::RunPetInfo(PETINFO *pPetInfo)
{
	if( m_pC_game )
		m_pC_game->RunPetInfo(pPetInfo);
}

void	C_VS_UI::ClosePetInfo()
{
	if( m_pC_game )
		m_pC_game->ClosePetInfo();
}

bool	C_VS_UI::IsRunningPetInfo()
{
	if( m_pC_game )
		return m_pC_game->IsRunningPetInfo();

	return false;
}

void C_VS_UI::RunHelpDesc(const char *detail,const char *title,DWORD id)
{
	if(m_pC_game)
		m_pC_game->RunHelpDesc(detail,title, id);
}

bool C_VS_UI::IsRunningHelpDesc()
{
	if(m_pC_game)
	{
		return m_pC_game->IsRunningHelpDesc();
	}
	return false;
}

void C_VS_UI::CloseHelpDesc()
{
	if(m_pC_game)
	   m_pC_game->CloseHelpDesc();
}
void	C_VS_UI::RequestShrineMinimap(DWORD timer)
{
	if( m_pC_game )
		m_pC_game->RequestShrineMinimap(timer);
}

void	C_VS_UI::CloseRequestShrineMinimap()
{
	if( m_pC_game )
		m_pC_game->FinishRequestShrineMinimap();
}

bool	C_VS_UI::IsRunningRequestShrineMinimap()
{
	if( m_pC_game )
		return m_pC_game->IsRunningRequestShrineMinimap();

	return false;
}
int 	C_VS_UI::GetPetStorageEmptySlot()
{
	if( m_pC_game )
		return m_pC_game->GetPetStorageEmptySlot();

	return -1;
}
int 	C_VS_UI::GetPetStorageFocusSlot()
{
	if( m_pC_game )
		return m_pC_game->GetPetStorageFocusSlot();

	return -1;
}

// 2004, 5, 27 sobeit add start - sms message
void C_VS_UI::RunSMSMessage()
{
	if (m_pC_game)
		m_pC_game->RunSMSMessage();
}

bool	C_VS_UI::IsRunningSMSMessage() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningSMSMessage();

	return false;
}

void	C_VS_UI::CloseSMSMessage() 
{
	if (m_pC_game)
		m_pC_game->CloseSMSMessage();
}

void C_VS_UI::RunSMSList()
{
	if (m_pC_game)
		m_pC_game->RunSMSList();
}

bool	C_VS_UI::IsRunningSMSList() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningSMSList();

	return false;
}

void	C_VS_UI::CloseSMSList() 
{
	if (m_pC_game)
		m_pC_game->CloseSMSList();
}

void C_VS_UI::RunSMSRecord()
{
	if (m_pC_game)
		m_pC_game->RunSMSRecord();
}

bool	C_VS_UI::IsRunningSMSRecord() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningSMSRecord();

	return false;
}

void	C_VS_UI::CloseSMSRecord() 
{
	if (m_pC_game)
		m_pC_game->CloseSMSRecord();
}

void	C_VS_UI::SetSMSAddressList(void* pVoid) 
{
	if (m_pC_game)
		m_pC_game->SetSMSAddressList(pVoid);
}
void	C_VS_UI::AddSMSAddressList(DWORD element, char* name, char* id, char* num) 
{
	if (m_pC_game)
		m_pC_game->AddSMSAddressList(element, name, id, num);
}
void	C_VS_UI::DeleteSMSAddressList(int id) 
{
	if (m_pC_game)
		m_pC_game->DeleteSMSAddressList(id);
}
void	C_VS_UI::AddSMSSendList(char* str) 
{
	if (m_pC_game)
		m_pC_game->AddSMSSendList(str);
}
void	C_VS_UI::AcquireSMS()
{
	if (m_pC_game)
		m_pC_game->AcquireSMS();
}

void	C_VS_UI::SetSMSCharage(int i)
{
	if (m_pC_game)
		m_pC_game->SetSMSCharage(i);
}
// 2004, 5, 27 sobeit add end


// 2004, 6, 14 sobeit add start- naming
void C_VS_UI::RunNaming()
{
	if (m_pC_game)
		m_pC_game->RunNaming();
}

bool	C_VS_UI::IsRunningNaming() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningNaming();

	return false;
}

void	C_VS_UI::CloseNaming() 
{
	if (m_pC_game)
		m_pC_game->CloseNaming();
}

void C_VS_UI::RunAskUseItemDialog(int AskType)
{
	if (m_pC_game)
		m_pC_game->RunAskUseItemDialog(AskType);
}
void C_VS_UI::CloseAskUseItemDialog()
{
	if (m_pC_game)
		m_pC_game->CloseAskUseItemDialog();
}
// 2004, 6, 14 sobeit end

// 2004, 6, 15 sobeit add start
bool	C_VS_UI::IsRunningGearWindow() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningGearWindow();
	return false;
}
void C_VS_UI::Change_Custom_Naming(int nID, char* szName)
{
	if (m_pC_game)
		m_pC_game->Change_Custom_Naming(nID, szName);
}
// 2004, 6, 15 sobeit end

// 2004, 6, 16 sobeit add start
void C_VS_UI::SetNickNameList(void* pData)
{
	if(m_pC_game)
		m_pC_game->SetNickNameList(pData);
}
void C_VS_UI::AddNickNameList(void* pData)
{
	if(m_pC_game)
		m_pC_game->AddNickNameList(pData);
}
// 2004, 6, 16 sobeit add end

// 2004, 6, 18 sobeit add start
//bool	C_VS_UI::GetChangeableNickName() 
//{
//	if(m_pC_game)
//		return m_pC_game->GetChangeableNickName();
//	return false;
//}
//bool	C_VS_UI::GetChangeablePetNickName() 
//{ 
//	if(m_pC_game)
//		return m_pC_game->GetChangeablePetNickName();
//	return false;
//}
//void	C_VS_UI::SetChangeableNickName(bool Is) 
//{
//	if(m_pC_game)
//		m_pC_game->SetChangeableNickName(Is);
//}
//void	C_VS_UI::SetChangeablePetNickName(bool Is)
//{
//	if(m_pC_game)
//		m_pC_game->SetChangeablePetNickName(Is);
//}
// 2004, 6, 18 sobeit add end

// 2004, 6, 21 sobiet add start
void	C_VS_UI::RunNamingChange(MItem* pItem, char* szCustom)
{
	if(m_pC_game)
		m_pC_game->RunNamingChange(pItem, szCustom);
}

bool	C_VS_UI::IsRunningNamingChange() const
{
	if(m_pC_game)
		return m_pC_game->IsRunningNamingChange();
	return false;
}

void	C_VS_UI::CloseNamingChange() 
{
	if(m_pC_game)
		m_pC_game->CloseNamingChange();
}
// 2004, 6, 21 sobeit add end

// 2004, 6, 30 sobiet add start
void	C_VS_UI::RunQuestManager()
{
	if(m_pC_game)
		m_pC_game->RunQuestManager();
}

bool	C_VS_UI::IsRunningQuestManager() const
{
	if(m_pC_game)
		return m_pC_game->IsRunningQuestManager();
	return false;
}

void	C_VS_UI::CloseQuestManager() 
{
	if(m_pC_game)
		m_pC_game->CloseQuestManager();
}

void	C_VS_UI::ReleaseQuestXML()
{
	if(m_pC_game)
		m_pC_game->ReleaseQuestXML();
}

bool	C_VS_UI::SetQuestManagerInfo(void* pVoid)
{
	if(m_pC_game)
		return m_pC_game->SetQuestManagerInfo(pVoid);
	return false;
}
bool	C_VS_UI::ModifyQuestManagerInfo(void* pVoid, int nType)
{
	if(m_pC_game)
		return m_pC_game->ModifyQuestManagerInfo(pVoid, nType);
	return false;
}
void	C_VS_UI::SetQuestManagerSubWindow(DWORD dwID)
{
	if(m_pC_game)
		m_pC_game->SetQuestManagerSubWindow(dwID);
}
void	C_VS_UI::SendQuestIntention(bool bIntention)
{
	if(m_pC_game)
		m_pC_game->SendQuestIntention(bIntention);
}
// 2004, 6, 30 sobeit add end

// 2004, 7, 5, sobeit add start

void	C_VS_UI::RunQuestList()
{
	if(m_pC_game)
		m_pC_game->RunQuestList();
}

bool	C_VS_UI::IsRunningQuestList() const
{
	if(m_pC_game)
		return m_pC_game->IsRunningQuestList();
	return false;
}

void	C_VS_UI::CloseQuestList() 
{
	if(m_pC_game)
		m_pC_game->CloseQuestList();
}

void	C_VS_UI::RunQuestDetail()
{
	if(m_pC_game)
		m_pC_game->RunQuestDetail();
}

bool	C_VS_UI::IsRunningQuestDetail() const
{
	if(m_pC_game)
		return m_pC_game->IsRunningQuestDetail();
	return false;
}

void	C_VS_UI::CloseQuestDetail() 
{
	if(m_pC_game)
		m_pC_game->CloseQuestDetail();
}

void	C_VS_UI::RunQuestMission()
{
	if(m_pC_game)
		m_pC_game->RunQuestMission();
}

bool	C_VS_UI::IsRunningQuestMission() const
{
	if(m_pC_game)
		return m_pC_game->IsRunningQuestMission();
	return false;
}

void	C_VS_UI::CloseQuestMission() 
{
	if(m_pC_game)
		m_pC_game->CloseQuestMission();
}

void	C_VS_UI::RunQuestItem()
{
	if(m_pC_game)
		m_pC_game->RunQuestItem();
}

bool	C_VS_UI::IsRunningQuestItem() const
{
	if(m_pC_game)
		return m_pC_game->IsRunningQuestItem();
	return false;
}

void	C_VS_UI::CloseQuestItem() 
{
	if(m_pC_game)
		m_pC_game->CloseQuestItem();
}

void	C_VS_UI::CloseQuestIcon(int JustClose) 
{
	if(m_pC_game)
		m_pC_game->CloseQuestIcon(JustClose);
}
void	C_VS_UI::UpdateQuestItemInfo(std::vector<MItem*>& vItem)
{
	if (m_pC_game)
		m_pC_game->UpdateQuestItemInfo(vItem);

}

void	C_VS_UI::PushGQuestExcuteElement(DWORD qID, BYTE bCondition, WORD wIndex)
{
	if (m_pC_game)
		m_pC_game->PushGQuestExcuteElement(qID,bCondition,wIndex);

}
void	C_VS_UI::RunNextGQuestExcuteElement()
{
	if (m_pC_game)
		m_pC_game->RunNextGQuestExcuteElement();

}
// 2004, 7, 5, sobeit add end
void	C_VS_UI::DeleteQuestItem(int nSlot)
{
	if (m_pC_game)
		m_pC_game->DeleteQuestItem(nSlot);

}

// 2004, 10, 25, sobeit add start - 세율 변경
void	C_VS_UI::RunModifyTax()
{
	if (m_pC_game)
		m_pC_game->RunModifyTax();

}


void	C_VS_UI::RunRangerChat()
{
	if (m_pC_game)
		m_pC_game->RunRangerChat();
}
bool	C_VS_UI::IsRunngingRangerChat()
{
	if (m_pC_game)
		return m_pC_game->IsRunngingRangerChat();
	return false;
}
void	C_VS_UI::CloseRangerChat()
{
	if (m_pC_game)
		m_pC_game->CloseRangerChat();
}
void	C_VS_UI::SetRangerChatString(char* str)
{
	if (m_pC_game)
		m_pC_game->SetRangerChatString(str);
}
// 2004, 10, 25, sobeit add end


bool	C_VS_UI::IsRunningPersnalShopMessage() const
{
	if(m_pC_game)
		return m_pC_game->IsRunningPersnalShopMessage();
	return false;

}
void    C_VS_UI::RunPersnalShopMessage()
{
	if(m_pC_game)
		m_pC_game->RunPersnalShopMessage();

}
void	C_VS_UI::ClosePersnalShopMessage()
{
	if(m_pC_game)
		m_pC_game->ClosePersnalShopMessage();
	
}


bool	C_VS_UI::IsRunningPersnalShop() const
{
	if (m_pC_game)
		return m_pC_game->IsRunningPersnalShop();

	return false;
}

void C_VS_UI::RunPersnalShop()
{
	if(m_pC_game)
	{
		m_pC_game->RunPersnalShop();
	}
}

void C_VS_UI::ClosePersnalShop()
{
	if(m_pC_game)
		m_pC_game->ClosePersnalShop();
}


void C_VS_UI::SetPersnalShop(MStorage *pStorage)
{
	if(m_pC_game)
		m_pC_game->SetPersnalShop(pStorage);
}


// 2004, 11, 30, sobeit add start
void	C_VS_UI::RunPowerjjang()
{
	if (m_pC_game)
		m_pC_game->RunPowerjjang();
}
bool	C_VS_UI::IsRunningPowerjjang()
{
	if (m_pC_game)
		return m_pC_game->IsRunningPowerjjang();
	return false;
}
void	C_VS_UI::ClosePowerjjang()
{
	if (m_pC_game)
		m_pC_game->ClosePowerjjang();
}

void	C_VS_UI::PowerjjangGambleResult(BYTE bItemCode)
{
	if (m_pC_game)
		m_pC_game->PowerjjangGambleResult(bItemCode);
}
// 2004, 11, 30, sobeit add end

// 2005, 1, 3, sobeit add start
void C_VS_UI::OpenInventoryToSwapAdvanceItem()
{
	if (m_pC_game)
		m_pC_game->OpenInventoryToSwapAdvanceItem();
}
void C_VS_UI::FinishSwapAdvancementItem()
{
	if (m_pC_game)
		m_pC_game->FinishSwapAdvancementItem();
}
bool C_VS_UI::IsRunningSwapAdvancementItem()
{
	if (m_pC_game)
		return m_pC_game->IsRunningSwapAdvancementItem();

	return false;
}
// 2005, 1, 3, sobeit add end

// 2005, 1, 11, sobeit add start - 불우이웃돕기 창
void	C_VS_UI::Run_Campaign_Help_Unfortunate_Neighbors(int value)
{
	if (m_pC_game)
		m_pC_game->Run_Campaign_Help_Unfortunate_Neighbors(value);

}
// 2005, 1, 11, sobeit add end

// 2005, 1, 17, sobeit add start - 퀘스트 관련
void	C_VS_UI::SetQuestNpcDialog(void* pVoid)
{
	if(m_pC_game)
		m_pC_game->SetQuestNpcDialog(pVoid);
}
// 2005, 1, 17, sobeit add end - 퀘스트 관련
	
// 2005, 1, 24, sobeit add start - 아이템 받기 이벤트
void	C_VS_UI::Run_Confirm_GetItemEvent(int value)
{
	if (m_pC_game)
		m_pC_game->Run_Confirm_GetItemEvent(value);

}
// 2005, 1, 24, sobeit add end

	
	// 2005, 2, 1, sobeit add start

void	C_VS_UI::RunWebBrowser(HWND hWnd, char* szURL, void* pWebOjbect)
{
	if (m_pC_game)
		m_pC_game->RunWebBrowser(hWnd, szURL, pWebOjbect);
}
bool	C_VS_UI::IsRunningWebBrowser()
{
	if (m_pC_game)
		return m_pC_game->IsRunningWebBrowser();
	return false;
}
void	C_VS_UI::CloseWebBrowser()
{
	if (m_pC_game)
		m_pC_game->CloseWebBrowser();
}
int	C_VS_UI::GetMouseCursorInfo()
{
	if (m_pC_game)
		return m_pC_game->GetMouseCursorInfo();
	return 0;
}

bool C_VS_UI::IsInRectPointWebBrowser(int X, int Y)
{
	if (m_pC_game)
		return m_pC_game->IsInRectPointWebBrowser(X, Y);
	return false;
}
// 2005, 2, 1, sobeit add end
#ifdef __TEST_SUB_INVENTORY__   // add by Coffee 2007-8-9 藤속관櫓관
// 2005, 2, 24, sobeit add start
	void	C_VS_UI::RunSubInventory(MItem* pItem)
	{
		if (m_pC_game)
			m_pC_game->RunSubInventory(pItem);
	}
	bool	C_VS_UI::IsRunningSubInventory()
	{
		if (m_pC_game)
			return m_pC_game->IsRunningSubInventory();
		return false;
	}
	MItem*	C_VS_UI::GetSubInventoryItem()
	{
		if (m_pC_game)
			return m_pC_game->GetSubInventoryItem();
		return NULL;
	}
	bool	C_VS_UI::ReplaceSubInventoryItem(MItem* pItem, BYTE x, BYTE y, MItem*& pOldItem)
	{
		if (m_pC_game)
			return m_pC_game->ReplaceSubInventoryItem(pItem, x, y, pOldItem);
		return false;
	}
	bool	C_VS_UI::AddItemToSubInventory(MItem* pItem, int X, int Y)
	{
		if (m_pC_game)
			return m_pC_game->AddItemToSubInventory(pItem, X, Y);
		return false;
	}
	void	C_VS_UI::CloseSubInventory()
	{
		if (m_pC_game)
			m_pC_game->CloseSubInventory();
	}
	// 2005, 2, 25, sobeit add end
#endif