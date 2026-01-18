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


//



//


//----------------------------------------------------------------------------
class Scrollbar
{
private:

protected:
	DWORD				m_dw_amount_for_scroll;
	DWORD				m_dw_amount_to_scroll;

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
