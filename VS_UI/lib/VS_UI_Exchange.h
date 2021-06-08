/*-----------------------------------------------------------------------------

	VS_UI_Exchange.h

	아이템 교환 인터페이스.

	2000.11.9. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_EXCHANGE_H__
#define __VS_UI_EXCHANGE_H__

#include "Unicorn.h"
#include "VS_UI_Widget.h"

#include "MTradeManager.h"

//-----------------------------------------------------------------------------
// C_VS_UI_EXCHANGE
//
// 아이템을 교환하는 인터페이스이다. 자신의 Grid와 상대방의 Grid가 보이며,
// 서로 confirm을 해야 교환이 이루어진다.
//-----------------------------------------------------------------------------
class C_VS_UI_EXCHANGE : public Window, public Exec, public ButtonVisual
{
private:
	enum IMAGE_SPK_INDEX
	{
		EXCHANGE_WINDOW,
		EXCHANGE_ITEMBACK,
	};

	enum INVENTORY_SPK_INDEX
	{
		INVENTORY_WINDOW,
		INVENTORY_WINDOW_ALPHA,
		INVENTORY_ITEMBACK,
		INVENTORY_ITEMBACK_ALPHA,
		INVENTORY_WINDOW_BOTTOM,
	};

	enum EXEC_ID
	{
		EXCHANGE_OK_ID,			// trade할거다라고 OK누르는거.
		EXCHANGE_OK_CANCEL_ID,	// OK하고 나서 OK를 취소한다.
		EXCHANGE_CANCEL_ID,		// 창닫기
		HELP_ID,
		MONEY_ID,
		INVENTORY_MONEY_ID,
		YOUR_MONEY_ID,
	};

	C_SPRITE_PACK				m_image_spk;

	ButtonGroup *				m_pC_button_group;

	C_VS_UI_EVENT_BUTTON*		m_pC_button_cancel;	// 그저 참조를 위한 pointer이다.

	int							m_focus_grid_x, m_focus_grid_y;
	Rect							m_my_grid_rect, m_your_grid_rect;
	
	POINT							m_money_button_point;			// 교환창에 넣을 돈
	POINT							m_inventory_money_button_point;	// inventory에 갖고 있는 돈
	POINT							m_your_money_button_point;		//남돈-_-

	int							m_other_ok_button_offset_x, m_other_ok_button_offset_y;
	int							m_your_name_x, m_your_name_y, m_my_name_x, m_my_name_y;

	// 2004, 5, 20 sobeit add start
	bool						m_CheckMoneyMove;
	// 2004, 5, 20 sobeit add end
	bool	Click(int grid_start_x, int grid_start_y);

	bool	Check(int grid_start_x, int grid_start_y);

public:
	C_VS_UI_EXCHANGE();
	~C_VS_UI_EXCHANGE();

	void	CancelPushState();
	void	Process();
	void	UnacquireMouseFocus();
	bool	IsPixel(int _x, int _y);
	void	Run(id_t id);
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button);
	bool	MouseControl(UINT message, int _x, int _y);

	void	Start();
	void	Finish();

	void	Show();

	int	GetFocusedItemGridX(const MItem * p_item);
	int	GetFocusedItemGridY(const MItem * p_item);

	int	GetFocusedOtherItemGridX(const MItem * p_item);
	int	GetFocusedOtherItemGridY(const MItem * p_item);
};

#endif