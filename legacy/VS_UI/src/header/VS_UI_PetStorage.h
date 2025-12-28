/*-----------------------------------------------------------------------------

	VS_UI_Storage.h

	Storage interface.

	2000.11.9. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_PETSTORAGE_H__
#define __VS_UI_PETSTORAGE_H__

#include "Unicorn.h"
#include "VS_UI_Widget.h"

#include "MGameDef.h"
#include "MStorage.h"
#include "MShopShelf.h"

#include "VS_UI_Base.h"
#include "VS_UI_Util.h"
#include "VS_UI_description.h"
#include "VS_UI_ExtraDialog.h"
#include "VS_UI_Title.h" // for S_SLOT
#include "SkillDef.h"
#include "MItem.h"




//-----------------------------------------------------------------------------
// C_VS_UI_PETSTORAGE
//
// 보관함. 보관한 1, 2, 3으로 tab이 있다. Shop 인터페이스와 같은 형태이다.
//-----------------------------------------------------------------------------
class C_VS_UI_PETSTORAGE: public Window, public Exec, public ButtonVisual
{
public:
	enum IMAGE_SPK_INDEX
	{
		MAIN_WINDOW,
	};

	enum{
		PET_STORAGE_CLOSE_ID,
		PET_STORAGE_HELP_ID
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

	ButtonGroup *				m_pC_button_group;

	int							m_shelf_start_x_offset;
	int							m_shelf_start_y_offset[SLOT_Y_COUNT];
	int							m_Click_Slot;

	int	GetSlotX(int slot) const;
	int	GetSlotY(int slot) const;
	int	GetDialogShowX() const;
	int	GetDialogShowY() const;
	MItem * GetSlotItem(int slot) const;

public:
	C_VS_UI_PETSTORAGE();
	~C_VS_UI_PETSTORAGE();

	void	SetPetStorage(MStorage * pStorage);

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

	int		GetEmptySlot();
	int		GetFocusSlot();
	void	Start();
	void	Finish();

	void	Show();
};

class C_VS_UI_PERSNALSHOP: public Window, public Exec, public ButtonVisual
{
public:
	enum IMAGE_SPK_INDEX
	{
		MAIN_WINDOW,
	};


private:
	C_SPRITE_PACK				m_image_spk;
	C_SPRITE_PACK				m_image_btn;

	enum INFO_SPK
	{
			PERSNALSHOP_MARK,
			INPUT_MESSAGE,			// 수락 버튼
			INPUT_MESSAGE_HILIGHT,	// 수락 버튼 하이라이트
			INPUT_MESSAGE_PUSH,		// 수락 버튼 푸쉬
			SHOP_OPEN_MESSAGE,			// 수락 버튼
			SHOP_OPEN_MESSAGE_HILIGHT,	// 수락 버튼 하이라이트
			SHOP_OPEN_MESSAGE_PUSH,		// 수락 버튼 푸쉬
			SHOP_CLOSE_MESSAGE,			// 수락 버튼
			SHOP_CLOSE_MESSAGE_HILIGHT,	// 수락 버튼 하이라이트
			SHOP_CLOSE_MESSAGE_PUSH,		// 수락 버튼 푸쉬
	};

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
	enum EXEC_ID
	{
		OK_ID,
		CANCEL_ID,		
		WRITE_MESSAGE,
	};

	int							m_shelf_start_x_offset;
	int							m_shelf_start_y_offset[SLOT_Y_COUNT];
	int							m_Click_Slot;

	int	GetSlotX(int slot) const;
	int	GetSlotY(int slot) const;
	int	GetDialogShowX() const;
	int	GetDialogShowY() const;
	MItem * GetSlotItem(int slot) const;

public:
	C_VS_UI_PERSNALSHOP();
	~C_VS_UI_PERSNALSHOP();

	void	SetPersnalShop(MStorage * pStorage);
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
	void	StartBuyConfirmDialog(int _x, int _y, int num);
	int		GetEmptySlot();
	int		GetFocusSlot();
	void	Start();
	void	Finish();

	void	Show();
	C_VS_UI_DIALOG	*			m_pC_dialog_remove_confirm;

};


#endif