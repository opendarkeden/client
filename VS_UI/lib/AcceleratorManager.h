//-----------------------------------------------------------------------------
// AcceleratorManager.h
//-----------------------------------------------------------------------------

#ifndef __ACCELERATORMANAGER_H__
#define __ACCELERATORMANAGER_H__

#include <Windows.h>
#include "MFunctionManager.h"

class AcceleratorManager : public MFunctionManager {
	public :
		AcceleratorManager();
		~AcceleratorManager();

	public :
		static void		Function_ACCEL_F1(void* pVoid);
		static void		Function_ACCEL_F2(void* pVoid);
		static void		Function_ACCEL_F3(void* pVoid);
		static void		Function_ACCEL_F4(void* pVoid);
		static void		Function_ACCEL_F5(void* pVoid);
		static void		Function_ACCEL_F6(void* pVoid);
		static void		Function_ACCEL_F7(void* pVoid);
		static void		Function_ACCEL_F8(void* pVoid);
		static void		Function_ACCEL_F9(void* pVoid);
		static void		Function_ACCEL_F10(void* pVoid);
		static void		Function_ACCEL_F11(void* pVoid);
		static void		Function_ACCEL_F12(void* pVoid);
		static void		Function_ACCEL_ESC(void* pVoid);
		static void		Function_ACCEL_SKILL(void* pVoid);
		static void		Function_ACCEL_INVENTORY(void* pVoid);
		static void		Function_ACCEL_GEAR(void* pVoid);
		static void		Function_ACCEL_CHARINFO(void* pVoid);
		static void		Function_ACCEL_SKILLINFO(void* pVoid);
		static void		Function_ACCEL_MINIMAP(void* pVoid);
		static void		Function_ACCEL_PARTY(void* pVoid);
		static void		Function_ACCEL_MARK(void* pVoid);
		static void		Function_ACCEL_HELP(void* pVoid);
		static void		Function_ACCEL_QUICKITEM_SLOT(void* pVoid);
		static void		Function_ACCEL_EXTEND_CHAT(void* pVoid);
		static void		Function_ACCEL_CHAT(void* pVoid);
		static void		Function_ACCEL_GUILD_CHAT(void* pVoid);
		static void		Function_ACCEL_ZONE_CHAT(void* pVoid);
		static void		Function_ACCEL_WHISPER(void* pVoid);
		static void		Function_ACCEL_GRADE1INFO(void* pVoid);
		static void		Function_ACCEL_PARTY_CHAT(void* pVoid);
		static void		Function_ACCEL_QUEST(void* pVoid);
		static void		Function_ACCEL_MAILBOX(void* pVoid);
		static void		Function_ACCEL_PETINFO(void* pVoid);
		static void		Function_ACCEL_SUMMON_PET(void* pVoid);
		static void		Function_ACCEL_UNION_CHAT(void* pVoid);
};

extern AcceleratorManager*		g_pAcceleratorManager;

#endif

