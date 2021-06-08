//----------------------------------------------------------------------
// AcceleratorDef.h
//----------------------------------------------------------------------

#ifndef __ACCELERATOR_H__
#define __ACCELERATOR_H__

enum ACCELERATOR 
{
	ACCEL_NULL,
	ACCEL_F1,
	ACCEL_F2,
	ACCEL_F3,
	ACCEL_F4,
	ACCEL_F5,
	ACCEL_F6,
	ACCEL_F7,
	ACCEL_F8,
	ACCEL_F9,
	ACCEL_F10,
	ACCEL_F11,
	ACCEL_F12,
	ACCEL_ESC,
	ACCEL_SKILL,
	ACCEL_INVENTORY,
	ACCEL_GEAR,
	ACCEL_CHARINFO,
	ACCEL_SKILLINFO,
	ACCEL_MINIMAP,
	ACCEL_PARTY,
	ACCEL_MARK,
	ACCEL_HELP,
	ACCEL_QUICKITEM_SLOT,
	ACCEL_EXTEND_CHAT,
	ACCEL_CHAT,
	ACCEL_GUILD_CHAT,
	ACCEL_ZONE_CHAT,
	ACCEL_WHISPER,
	ACCEL_GRADE1INFO,
	ACCEL_PARTY_CHAT,
	ACCEL_QUEST,	
	ACCEL_MAILBOX,
	ACCEL_PETINFO,

//	// 2004, 5, 17 sobeit add start
	ACCEL_SUMMON_PET,
//	// 2004, 5, 17 sobeit add end
	ACCEL_UNION_CHAT,
	MAX_ACCELERATOR
};

extern const char* ACCELERATOR_NAME[MAX_ACCELERATOR];		

extern void	SetDefaultAccelerator();

#endif