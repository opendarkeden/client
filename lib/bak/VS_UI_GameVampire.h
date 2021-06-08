/*-----------------------------------------------------------------------------

	VS_UI_GameVampire.h

	Vampire interface.

	2000.8.11. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_GAMEVAMPIRE_H__
#define __VS_UI_GAMEVAMPIRE_H__

#include "VS_UI_GameCommon.h"
#include "VS_UI_skill_tree.h"

#include "MVampireGear.h"

//-----------------------------------------------------------------------------
// class C_VS_UI_VAMPIRE_GEAR
//
// 
//-----------------------------------------------------------------------------

class C_VS_UI_VAMPIRE_GEAR : public C_VS_UI_GEAR
{
private:
	enum SLOT_NAME
	{
		SN_NECKLACE,
		SN_COAT,
		SN_BRACELET1,
		SN_BRACELET2,
		SN_RING1,
		SN_RING2,
		SN_RING3,
		SN_RING4,
		SN_EARRING1,
		SN_EARRING2,
		SN_LEFTHAND,
		SN_RIGHTHAND,
		SN_AMULET1,
		SN_AMULET2,
		SN_AMULET3,
		SN_AMULET4,

		SLOT_SIZE,
	};

	enum GEAR_SLOT_SPK_INDEX
	{
		EARRING,
		NECKLACE,
		COAT,
		BRACELET,
		RING,
		WEAPON,
		AMULET,

		//SHOES,
	};

	static int					m_slot_image[SLOT_SIZE];
	Rect							m_slot_rect[SLOT_SIZE];

public:
	C_VS_UI_VAMPIRE_GEAR();
	~C_VS_UI_VAMPIRE_GEAR();
};
//-----------------------------------------------------------------------------
// C_VS_UI_VAMPIRE
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_VAMPIRE : public C_VS_UI_TRIBE
{
public:
private:
	C_VS_UI_VAMPIRE_GEAR	*	m_pC_gear;

public:
	C_VS_UI_VAMPIRE();
	~C_VS_UI_VAMPIRE();

	void	RunQuickItemSlot() { return; }
	void	CloseQuickItemSlot()  { return; };
	bool	IsRunningQuickItemSlot()  { return false; };

	C_VS_UI_GEAR * GetGearWindow() const;
	void	CloseGear();
	void	OpenGear(bool bl_set_load = true);

	void	DoCommonActionBeforeEventOccured();
	void	DoCommonActionAfterEventOccured();

	void	Start();
	void	Show();
	void	Process();
//	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	void	HotKey_F1();	//��û ����ϱ�
	void	HotKey_F5();	
	void	HotKey_F6();
	void	HotKey_F7();
	void	HotKey_F8();
};

#endif