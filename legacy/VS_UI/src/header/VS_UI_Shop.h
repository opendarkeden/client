/*-----------------------------------------------------------------------------

	VS_UI_Shop.h

	Shop interface.

	2000.11.9. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_SHOP_H__
#define __VS_UI_SHOP_H__

#include "Unicorn.h"
#include "VS_UI_Widget.h"
#include "VS_UI_ExtraDialog.h"

#include "MGameDef.h"
#include "MShop.h"
#include "MShopShelf.h"

//-----------------------------------------------------------------------------
// C_VS_UI_SHOP
//
// 세 종류의 Item, Normal/Special/Mysterious을 구입할 수 있는 인터페이스이다.
// 구입시 개수를 결정할 수 있다.
//-----------------------------------------------------------------------------
#define NORMAL_TAB_ID					0
#define SPECIAL_TAB_ID					1
#define MYSTERIOUS_TAB_ID				2
#define SHOP_CLOSE_ID					3
#define SHOP_HELP_ID					4

class C_VS_UI_SHOP : public Window, public Exec, public ButtonVisual
{
public:
	static bool					m_bl_mysterious_tab; // item description price 얻을 때 check한다.
	static MShop::SHOP_TYPE		m_shop_type; // item description price 얻을 때 check한다.

	enum IMAGE_SPK_INDEX
	{
		MAIN_WINDOW,
		NORMAL_TAB,
		SPECIAL_TAB,
		MYSTERIOUS_TAB,
		NORMAL_TAB_HILIGHTED,
		SPECIAL_TAB_HILIGHTED,
		MYSTERIOUS_TAB_HILIGHTED,
		NORMAL_TAB_HILIGHTED_PUSHED,
		SPECIAL_TAB_HILIGHTED_PUSHED,
		MYSTERIOUS_TAB_HILIGHTED_PUSHED,
		STORAGE_TAB1,
		STORAGE_TAB2,
		STORAGE_TAB3,
		STORAGE_TAB1_HILIGHTED,
		STORAGE_TAB2_HILIGHTED,
		STORAGE_TAB3_HILIGHTED,
		STORAGE_TAB1_HILIGHTED_PUSHED,
		STORAGE_TAB2_HILIGHTED_PUSHED,
		STORAGE_TAB3_HILIGHTED_PUSHED,
	};

private:
	C_SPRITE_PACK				m_image_spk;
	C_SPRITE_PACK				m_question_spk;

	enum
	{
		TAB_WIDTH = 95,
		TAB_HEIGHT = 27,

		SLOT_X_COUNT = 5,
		SLOT_Y_COUNT = 4,

		// 2x3 grid size
		SLOT_WIDTH = 60,
		SLOT_HEIGHT = 90,
	};

	ButtonGroup *				m_pC_button_group;

	int							m_shelf_start_x_offset;
	int							m_shelf_start_y_offset[SLOT_Y_COUNT];

	// dialog
	C_VS_UI_DIALOG	*			m_pC_dialog_buy_confirm;

	int	GetSlotX(int slot) const;
	int	GetSlotY(int slot) const;
	int	GetDialogShowX() const;
	int	GetDialogShowY() const;
	void	StartBuyConfirmDialog(int _x, int _y, int num);
	MItem * GetSlotItem(int slot) const;

//timer
	DWORD						m_dw_prev_tickcount;
	DWORD						m_dw_millisec;

	bool	Timer();

public:
	C_VS_UI_SHOP();
	~C_VS_UI_SHOP();

	void	SetShop(MShop* pShop);

	void	CancelPushState();
	void	Process();
	void	AcquireDisappear() {}
	void	UnacquireMouseFocus();
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);
	void	Run(id_t id);
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	bool	MouseControl(UINT message, int _x, int _y);

	void	Start();
	void	Finish();

	void	Show();
};

#endif