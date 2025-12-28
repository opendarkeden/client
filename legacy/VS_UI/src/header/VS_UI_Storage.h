/*-----------------------------------------------------------------------------

	VS_UI_Storage.h

	Storage interface.

	2000.11.9. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_STORAGE_H__
#define __VS_UI_STORAGE_H__

#include "Unicorn.h"
#include "VS_UI_Widget.h"

#include "MGameDef.h"
#include "MStorage.h"
#include "MShopShelf.h"

//-----------------------------------------------------------------------------
// C_VS_UI_STORAGE
//
// 보관함. 보관한 1, 2, 3으로 tab이 있다. Shop 인터페이스와 같은 형태이다.
//-----------------------------------------------------------------------------
#define STORAGE_TAB1_ID					0
#define STORAGE_TAB2_ID					1
#define STORAGE_TAB3_ID					2
#define STORAGE_CLOSE_ID				3
#define STORAGE_HELP_ID					4
#define	STORAGE_MONEY_ID				5

class C_VS_UI_STORAGE: public Window, public Exec, public ButtonVisual
{
public:
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

	int							m_money_button_offset_x, m_money_button_offset_y;

	ButtonGroup *				m_pC_button_group;

	int							m_tab_x_offset, m_tab_y_offset;
	int							m_shelf_start_x_offset;
	int							m_shelf_start_y_offset[SLOT_Y_COUNT];

	int	GetSlotX(int slot) const;
	int	GetSlotY(int slot) const;
	int	GetDialogShowX() const;
	int	GetDialogShowY() const;
	MItem * GetSlotItem(int slot) const;

public:
	C_VS_UI_STORAGE();
	~C_VS_UI_STORAGE();

	void	SetStorage(MStorage * pStorage);

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