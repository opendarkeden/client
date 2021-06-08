// VS_UI_ELEVATOR.h: interface for the C_VS_UI_ELEVATOR class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _VS_UI_ELEVATOR_H_
#define _VS_UI_ELEVATOR_H_

#include "Unicorn.h"
#include "VS_UI_Widget.h"

class C_VS_UI_ELEVATOR : public Window, public Exec, public ButtonVisual
{
private:
	enum ELEVATOR_SPK_INDEX
	{
		ELEVATOR,
		OK_BUTTON,
		CLOSE_BUTTON,
		LEVEL_3,
		LEVEL_2,
		LEVEL_1,
		LEVEL_B1,
	};

	ButtonGroup *				m_pC_button_group;
	C_SPRITE_PACK				m_image_spk;
	int							m_iLevel;

public:
	C_VS_UI_ELEVATOR();
	~C_VS_UI_ELEVATOR();

	void	Process();
	void	UnacquireMouseFocus();
	bool	IsPixel(int _x, int _y);
	void	Run(id_t id);
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON *p_button);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);

	void	Start();
	void	Finish();

	void	Show();

};

#endif
