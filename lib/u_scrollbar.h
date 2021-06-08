/*-----------------------------------------------------------------------------

	u_scrollbar.h

	Scrollbar Widget.

	2000.6.7. KJTINC
	2000.7.29.

-----------------------------------------------------------------------------*/

#ifndef __U_SCROLLBAR_H__
#define __U_SCROLLBAR_H__

#include "Basics.h"
#include "Mother.h"

//----------------------------------------------------------------------------
// Widget Scrollbar
//
// Design:
//				scroll될 '대상'과 scroll시키는 '장치'가 존재한다. '대상'은 보이지
//				않는 부분 즉, 미래에 scroll되어 보일 부분이다.
//
//				scroll이 얼만큼되었는지 알 수 있어야 한다.
//				'장치'는 대상에 대해 정확히 알아야 한다. '대상'은 양(amount)으로써
//				'현재상태'를 나타내며 percent로 나타낼 수 있다.
//
//				이 Widget은 '대상'에 대해 알고 있으며, '대상'을 변화시킬 수 있는
//				변화량을 산출한다.
//----------------------------------------------------------------------------
class Scrollbar
{
private:

protected:
	DWORD				m_dw_amount_for_scroll; // '대상'
	DWORD				m_dw_amount_to_scroll;	// scroll한 amount (x <= m_dw_amount_for_scroll)

public:
	Scrollbar();
	virtual ~Scrollbar();

	void		Init();

	//---------------------------------------
	// Set
	//---------------------------------------
	void		SetAmountForScroll(DWORD amount_for_scroll);
	void		SetAmountToScroll(DWORD amount_to_scroll);
	void		SetAmountToScrollByPercent(double percent);
	void		IncreaseAmountToScroll();
	void		DecreaseAmountToScroll();

	//---------------------------------------
	// Get
	//---------------------------------------
	bool		NoScroll();
	int		GetPercentToScroll() const;
	DWORD		GetAmountToScroll() const;
	DWORD		GetAmountForScroll() const;
};

#endif
