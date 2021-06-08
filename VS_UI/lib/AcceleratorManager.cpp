//-----------------------------------------------------------------------------
// AcceleratorManager.cpp
//-----------------------------------------------------------------------------

#include "client_PCH.h"
#include "AcceleratorManager.h"
#include "AcceleratorDef.h"
#include "VS_UI.h"

//-----------------------------------------------------------------------------
// Global
//-----------------------------------------------------------------------------
AcceleratorManager*		g_pAcceleratorManager = NULL;

//-----------------------------------------------------------------------------
//
// constructor
//
//-----------------------------------------------------------------------------
AcceleratorManager::AcceleratorManager()
{
	MFunctionManager::Init( MAX_ACCELERATOR );

	MFunctionManager::SetFunction(ACCEL_F1, Function_ACCEL_F1);
	MFunctionManager::SetFunction(ACCEL_F2, Function_ACCEL_F2);
	MFunctionManager::SetFunction(ACCEL_F3, Function_ACCEL_F3);
	MFunctionManager::SetFunction(ACCEL_F4, Function_ACCEL_F4);
	MFunctionManager::SetFunction(ACCEL_F5, Function_ACCEL_F5);
	MFunctionManager::SetFunction(ACCEL_F6, Function_ACCEL_F6);
	MFunctionManager::SetFunction(ACCEL_F7, Function_ACCEL_F7);
	MFunctionManager::SetFunction(ACCEL_F8, Function_ACCEL_F8);
	MFunctionManager::SetFunction(ACCEL_F9, Function_ACCEL_F9);
	MFunctionManager::SetFunction(ACCEL_F10, Function_ACCEL_F10);
	MFunctionManager::SetFunction(ACCEL_F11, Function_ACCEL_F11);
	MFunctionManager::SetFunction(ACCEL_F12, Function_ACCEL_F12);
	MFunctionManager::SetFunction(ACCEL_ESC, Function_ACCEL_ESC);
	MFunctionManager::SetFunction(ACCEL_SKILL, Function_ACCEL_SKILL);
	MFunctionManager::SetFunction(ACCEL_INVENTORY, Function_ACCEL_INVENTORY);
	MFunctionManager::SetFunction(ACCEL_GEAR, Function_ACCEL_GEAR);
	MFunctionManager::SetFunction(ACCEL_CHARINFO, Function_ACCEL_CHARINFO);
	MFunctionManager::SetFunction(ACCEL_SKILLINFO, Function_ACCEL_SKILLINFO);
	MFunctionManager::SetFunction(ACCEL_MINIMAP, Function_ACCEL_MINIMAP);
	MFunctionManager::SetFunction(ACCEL_PARTY, Function_ACCEL_PARTY);
	MFunctionManager::SetFunction(ACCEL_MARK, Function_ACCEL_MARK);
	MFunctionManager::SetFunction(ACCEL_HELP, Function_ACCEL_HELP);
	MFunctionManager::SetFunction(ACCEL_QUICKITEM_SLOT, Function_ACCEL_QUICKITEM_SLOT);
	MFunctionManager::SetFunction(ACCEL_EXTEND_CHAT, Function_ACCEL_EXTEND_CHAT);
	MFunctionManager::SetFunction(ACCEL_CHAT, Function_ACCEL_CHAT);
	MFunctionManager::SetFunction(ACCEL_GUILD_CHAT, Function_ACCEL_GUILD_CHAT);
	MFunctionManager::SetFunction(ACCEL_ZONE_CHAT, Function_ACCEL_ZONE_CHAT);
	MFunctionManager::SetFunction(ACCEL_WHISPER, Function_ACCEL_WHISPER);
	MFunctionManager::SetFunction(ACCEL_GRADE1INFO, Function_ACCEL_GRADE1INFO);
	MFunctionManager::SetFunction(ACCEL_PARTY_CHAT, Function_ACCEL_PARTY_CHAT);
	MFunctionManager::SetFunction(ACCEL_QUEST, Function_ACCEL_QUEST);
	MFunctionManager::SetFunction(ACCEL_MAILBOX, Function_ACCEL_MAILBOX);
	// 2004, 5, 17 sobeit add start
	MFunctionManager::SetFunction(ACCEL_PETINFO, Function_ACCEL_PETINFO);
	MFunctionManager::SetFunction(ACCEL_SUMMON_PET, Function_ACCEL_SUMMON_PET);
	// 2004, 5, 17 sobeit add end

	MFunctionManager::SetFunction(ACCEL_UNION_CHAT, Function_ACCEL_UNION_CHAT);
}

AcceleratorManager::~AcceleratorManager()
{
}

//-----------------------------------------------------------------------------
// ACCEL_F1
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_F1(void* pVoid)
{
	gC_vs_ui.HotKey_F1();
}

//-----------------------------------------------------------------------------
// ACCEL_F2
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_F2(void* pVoid)
{
	gC_vs_ui.HotKey_F2();
}

//-----------------------------------------------------------------------------
// ACCEL_F3
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_F3(void* pVoid)
{
	gC_vs_ui.HotKey_F3();
}

//-----------------------------------------------------------------------------
// ACCEL_F4
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_F4(void* pVoid)
{
	gC_vs_ui.HotKey_F4();
}

//-----------------------------------------------------------------------------
// ACCEL_F5
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_F5(void* pVoid)
{
	gC_vs_ui.HotKey_F5();
}

//-----------------------------------------------------------------------------
// ACCEL_F6
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_F6(void* pVoid)
{
	gC_vs_ui.HotKey_F6();
}

//-----------------------------------------------------------------------------
// ACCEL_F7
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_F7(void* pVoid)
{
	gC_vs_ui.HotKey_F7();
}

//-----------------------------------------------------------------------------
// ACCEL_F8
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_F8(void* pVoid)
{
	gC_vs_ui.HotKey_F8();
}

//-----------------------------------------------------------------------------
// ACCEL_F9
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_F9(void* pVoid)
{
	gC_vs_ui.HotKey_F9();
}

//-----------------------------------------------------------------------------
// ACCEL_F10
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_F10(void* pVoid)
{
	gC_vs_ui.HotKey_F10();
}

//-----------------------------------------------------------------------------
// ACCEL_F11
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_F11(void* pVoid)
{
	gC_vs_ui.HotKey_F11();
}

//-----------------------------------------------------------------------------
// ACCEL_F12
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_F12(void* pVoid)
{
	gC_vs_ui.HotKey_F12();
}

//-----------------------------------------------------------------------------
// ACCEL_ESC
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_ESC(void* pVoid)
{
	gC_vs_ui.HotKey_ESC();
}

//-----------------------------------------------------------------------------
// ACCEL_SKILL
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_SKILL(void* pVoid)
{
	gC_vs_ui.HotKey_Skill();
}

//-----------------------------------------------------------------------------
// ACCEL_INVENTORY
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_INVENTORY(void* pVoid)
{
	gC_vs_ui.HotKey_Inventory();
}

//-----------------------------------------------------------------------------
// ACCEL_GEAR
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_GEAR(void* pVoid)
{
	gC_vs_ui.HotKey_Gear();
}

//-----------------------------------------------------------------------------
// ACCEL_CHARINFO
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_CHARINFO(void* pVoid)
{
	gC_vs_ui.HotKey_CharInfo();
}

//-----------------------------------------------------------------------------
// ACCEL_SKILLINFO
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_SKILLINFO(void* pVoid)
{
	gC_vs_ui.HotKey_SkillInfo();
}

//-----------------------------------------------------------------------------
// ACCEL_MINIMAP
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_MINIMAP(void* pVoid)
{
	gC_vs_ui.HotKey_Minimap();
}

//-----------------------------------------------------------------------------
// ACCEL_PARTY
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_PARTY(void* pVoid)
{
	gC_vs_ui.HotKey_Party();
}

//-----------------------------------------------------------------------------
// ACCEL_MARK
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_MARK(void* pVoid)
{
	gC_vs_ui.HotKey_Mark();
}

//-----------------------------------------------------------------------------
// ACCEL_HELP
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_HELP(void* pVoid)
{
	gC_vs_ui.HotKey_Help();
}

//-----------------------------------------------------------------------------
// ACCEL_QUICKITEM_SLOT
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_QUICKITEM_SLOT(void* pVoid)
{
	gC_vs_ui.HotKey_QuickItemSlot();
}

//-----------------------------------------------------------------------------
// ACCEL_EXTEND_CHAT
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_EXTEND_CHAT(void* pVoid)
{
	gC_vs_ui.HotKey_ExtendChat();
}

//-----------------------------------------------------------------------------
// ACCEL_CHAT
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_CHAT(void* pVoid)
{
	gC_vs_ui.HotKey_Chat();
}

//-----------------------------------------------------------------------------
// ACCEL_GUILD_CHAT
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_GUILD_CHAT(void* pVoid)
{
	gC_vs_ui.HotKey_GuildChat();
}
//-----------------------------------------------------------------------------
// ACCEL_UNION_CHAT
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_UNION_CHAT(void* pVoid)
{
	gC_vs_ui.HotKey_UnionChat();
}
//-----------------------------------------------------------------------------
// ACCEL_ZONE_CHAT
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_ZONE_CHAT(void* pVoid)
{
	gC_vs_ui.HotKey_ZoneChat();
}

//-----------------------------------------------------------------------------
// ACCEL_WHISPER
//-----------------------------------------------------------------------------
void		
AcceleratorManager::Function_ACCEL_WHISPER(void* pVoid)
{
	gC_vs_ui.HotKey_Whisper();
}

//-----------------------------------------------------------------------------
// ACCEL_GRADE
//-----------------------------------------------------------------------------
void
AcceleratorManager::Function_ACCEL_GRADE1INFO(void* pVoid)
{
	gC_vs_ui.HotKey_Grade1Info();
}

//-----------------------------------------------------------------------------
// ACCEL_PARTY_CHAT
//-----------------------------------------------------------------------------
void
AcceleratorManager::Function_ACCEL_PARTY_CHAT(void* pVoid)
{
	gC_vs_ui.HotKey_PartyChat();
}

//-----------------------------------------------------------------------------
// ACCEL_QUEST
//-----------------------------------------------------------------------------
void
AcceleratorManager::Function_ACCEL_QUEST(void* pVoid)
{
	gC_vs_ui.HotKey_Quest();
}

//-----------------------------------------------------------------------------
// ACCEL_MAILBOX
//-----------------------------------------------------------------------------
void
AcceleratorManager::Function_ACCEL_MAILBOX(void* pVoid)
{
	gC_vs_ui.HotKey_MailBox();
}

//-----------------------------------------------------------------------------
// ACCEL_MAILBOX
//-----------------------------------------------------------------------------
void
AcceleratorManager::Function_ACCEL_PETINFO(void* pVoid)
{
	gC_vs_ui.HotKey_PetInfo();
}
//////
//// 2004, 5, 17 sobeit add start
////-----------------------------------------------------------------------------
//// ACCEL_MAILBOX
////-----------------------------------------------------------------------------
void
AcceleratorManager::Function_ACCEL_SUMMON_PET(void* pVoid)
{
	gC_vs_ui.HotKey_SummonPet();
}
 //2004, 5, 17 sobeit add start