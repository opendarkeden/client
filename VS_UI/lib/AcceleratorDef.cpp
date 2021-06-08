
//----------------------------------------------------------------------
// AcceleratorDef.cpp
//----------------------------------------------------------------------

#include "client_PCH.h"
#include "KeyAccelerator.h"
#include "CDirectInput.h"
#include "mgamestringtable.h"

//#include "VS_UI_Widget.h"

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
const char* ACCELERATOR_NAME[MAX_ACCELERATOR] =
{
	"AcceleratorNULL",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"ESC",
	"SKILL",
	"INVENTORY",
	"GEAR",
	"CHARINFO",
	"SKILLINFO",
	"MINIMAP",
	"PARTY",
	"MARK",
	"HELP",
	"QUICKITEM_SLOT",
	"EXTEND_CHAT",
	"CHAT",	
	"GUILD_CHAT",
	"ZONE_CHAT",
	"WHISPER",
	"GRADE1",
	"PARTY_CHAT",
	"QUEST",
	"MAILBOX",
	"PETINFO",
	"SUMMONPET",
	"UNION_CHAT",
};

//----------------------------------------------------------------------
// Set DefaultAccelerator
//----------------------------------------------------------------------
// default file을 생성해두고 load하는 것도 괜찮을거 같다.
//----------------------------------------------------------------------
void	
SetDefaultAccelerator()
{
	const WORD keys[MAX_ACCELERATOR] =  
	{
		0,														// ACCEL_NULL,
		DIK_F1,													// ACCEL_F1
		DIK_F2,													// ACCEL_F2
		DIK_F3,													// ACCEL_F3
		DIK_F4,													// ACCEL_F4
		DIK_F5,													// ACCEL_F5
		DIK_F6,													// ACCEL_F6
		DIK_F7,													// ACCEL_F7
		DIK_F8,													// ACCEL_F8
		DIK_F9,													// ACCEL_F9
		DIK_F10,												// ACCEL_F10
		DIK_F11,												// ACCEL_F11
		DIK_F12,												// ACCEL_F12
		DIK_ESCAPE,												// ACCEL_ESC
		DIK_S,													// ACCEL_SKILL
		DIK_TAB,												// ACCEL_INVENTORY
		ACCEL_MAKE_KEY( DIK_TAB, FLAG_ACCELERATOR_CONTROL ),	// ACCEL_GEAR
		DIK_I,													// ACCEL_CHARINFO
		DIK_K,													// ACCEL_SKILLINFO
		DIK_M,													// ACCEL_MINIMAP
		DIK_P,													// ACCEL_PARTY
		DIK_X,													// ACCEL_MARK
		DIK_H,													// ACCEL_HELP
		DIK_GRAVE,												// ACCEL_QUICKITEM_SLOT
		DIK_E,													// ACCEL_EXTEND_CHAT
		DIK_C,													// ACCEL_CHAT
		DIK_G,													// ACCEL_GUILD_CHAT
		DIK_Z,													// ACCEL_ZONE_CHAT
		DIK_W,													// ACCEL_WHISPER
		DIK_R,													// ACCEL_GRADE1INFO
		DIK_A,													// ACCEL_PARTY_CHAT
		DIK_Q,													// ACCEL_QUEST
		DIK_B,													// ACCEL_MAILBOX
		ACCEL_MAKE_KEY( DIK_SPACE, FLAG_ACCELERATOR_ALT ),	// ACCEL_PETINFO
		ACCEL_MAKE_KEY( DIK_SPACE, FLAG_ACCELERATOR_CONTROL ),	// ACCEL_SUMMON_PET
		DIK_D,
	};

	for (int accel=1; accel<MAX_ACCELERATOR; accel++)
	{
		g_pKeyAccelerator->SetAcceleratorKey( accel, keys[accel] );
	}
}

