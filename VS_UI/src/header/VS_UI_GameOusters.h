/*-----------------------------------------------------------------------------

	VS_UI_GameOusters.h

	Ousters interface.

	2000.8.11. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_GAMEOUSTERS_H__
#define __VS_UI_GAMEOUSTERS_H__

#include "VS_UI_GameCommon.h"
#include "VS_UI_skill_tree.h"

#include "MOustersGear.h"

//-----------------------------------------------------------------------------
// class C_VS_UI_OUSTERS_GEAR
//
// 
//-----------------------------------------------------------------------------

class C_VS_UI_OUSTERS_GEAR : public C_VS_UI_GEAR
{
public:
	enum SLOT_NAME
	{
		SN_CIRCLET,
		SN_COAT,
		SN_LEFTHAND,
		SN_RIGHTHAND,
		SN_BOOTS,
		SN_ARMSBAND1,
		SN_ARMSBAND2,
		SN_RING1,
		SN_RING2,
		SN_PENDENT1,
		SN_PENDENT2,
		SN_PENDENT3,
		SN_STONE1,
		SN_STONE2,
		SN_STONE3,
		SN_STONE4,

		SN_COREZAP1,
		SN_COREZAP2,
		SN_COREZAP3,
		SN_COREZAP4,
		
		SN_FASCIA,
        SN_MITTEN,

		SN_BLOODBIBLE1,
		SN_BLOODBIBLE2,
		SN_BLOODBIBLE3,
		SN_BLOODBIBLE4,
		SN_BLOODBIBLE5,
		SN_BLOODBIBLE6,

		SLOT_SIZE,	
	};

	C_VS_UI_OUSTERS_GEAR();
	~C_VS_UI_OUSTERS_GEAR();
private:
	
	enum GEAR_SLOT_SPK_INDEX
	{
		CIRCLET,
		COAT,
		CHAKRAM,
		BOOTS,
		ARMSBAND,
		RING,
		PENDENT,
		WRISTLET,
		STONE_1,
		STONE_2,
		STONE_3,
		STONE_4,

		BLOODBIBLE,
		FASCIA,
		MITTEN

	};

	static int					m_slot_image[SLOT_SIZE];
	Rect						m_slot_rect[SLOT_SIZE];


//	void Show();
};
//-----------------------------------------------------------------------------
// C_VS_UI_OUSTERS
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_OUSTERS : public C_VS_UI_TRIBE
{
public:
private:
	C_VS_UI_OUSTERS_GEAR	*	m_pC_gear;

public:
	C_VS_UI_OUSTERS();
	~C_VS_UI_OUSTERS();

	void	RunQuickItemSlot();
	void	CloseQuickItemSlot();
	bool	IsRunningQuickItemSlot();

	C_VS_UI_GEAR * GetGearWindow() const;
	void	CloseGear();
	void	OpenGear(bool bl_set_load = true);

	void	DoCommonActionBeforeEventOccured();
	void	DoCommonActionAfterEventOccured();

	void	ResetOustersQuickItemSize();

	void	Start();
	void	Show();
	void	Process();
//	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	void	HotKey_F1();
	void	HotKey_F2();
	void	HotKey_F3();
	void	HotKey_F4();
	void	HotKey_F5();
	void	HotKey_F6();
	void	HotKey_F7();
	void	HotKey_F8();
	void	HotKey_QuickItemSlot();
};

//-----------------------------------------------------------------------------
// C_VS_UI_OUSTERS_QUICKITEM
//
// OUSTERS only Quick item interface object.
//-----------------------------------------------------------------------------
class C_VS_UI_OUSTERS_QUICKITEM : public Window, public ButtonVisual, public Exec
{
private:
	enum 
	{ 
		QUICKSLOT_TOTAL = 8,
		QUICKSLOT_W = 30,
		QUICKSLOT_H = 30,
	};

	enum EXEC_ID
	{
		ALPHA_ID,
		PUSHPIN_ID,
		ROTATE_ID,
	};

public:
	enum
	{
		SLOT_1 = 0,
		SLOT_2,
		SLOT_3,
		SLOT_4,
		SLOT_5,
		SLOT_6,
		
		SLOT_MAX
	};

private:
	enum SPK_INDEX
	{
		SLOT,
		SLOT_ALPHA,
		STATCH_EDGE,
		START_EDGE,
		START_EDGE_EXTEND,
		END_EDGE,
		END_EDGE_EXTEND,
		BUTTON_ROTATE_HILIGHTED,
		BUTTON_ROTATE_HILIGHTED_PUSHED,

		ROTATED_OFFSET,
	};

	ButtonGroup *				m_pC_button_group;

	C_SPRITE_PACK 				m_image_spk;

	int							m_p_slot_x[QUICKSLOT_TOTAL];

	int							m_focus_slot;

	bool						m_bl_width, m_bl_resize;

	bool	Click();

	int	GetPocketCount() const;

public:
	C_VS_UI_OUSTERS_QUICKITEM();
	~C_VS_UI_OUSTERS_QUICKITEM();

	void	ToggleBelt();
	void	Open();
	void	Close();
	void	AcquireDisappear() {}
	void	Use(int slot, bool bFunctionKey=true);
	void	UnacquireMouseFocus();
	void	ResetSize();
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);
	void	CancelPushState();
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	void	Run(id_t id);


	void	KeyboardControl(UINT message, UINT key, long extra);
	bool	MouseControl(UINT message, int _x, int _y);
	void	Show();
	void	Start();
	void	Process();

	static MItem*	GetItem(int slot);
};


#endif