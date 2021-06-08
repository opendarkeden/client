// u_scrollbar.cpp
#include "client_PCH.h"
#include "u_scrollbar.h"
#include <assert.h>


Scrollbar::Scrollbar()
{
	Init();
}

Scrollbar::~Scrollbar()
{

}

//-----------------------------------------------------------------------------
// Init
//
// 
//-----------------------------------------------------------------------------
void Scrollbar::Init()
{
	m_dw_amount_for_scroll = 0;
	m_dw_amount_to_scroll = 0;
}

//-----------------------------------------------------------------------------
// IncreaseAmountToScroll
//
// m_dw_amount_to_scroll을 1 증가시킨다.
//-----------------------------------------------------------------------------
void Scrollbar::IncreaseAmountToScroll()
{
	if (m_dw_amount_to_scroll < m_dw_amount_for_scroll)
		m_dw_amount_to_scroll++;
}

//-----------------------------------------------------------------------------
// DecreaseAmountToScroll
//
// m_dw_amount_to_scroll을 1 감소시킨다.
//-----------------------------------------------------------------------------
void Scrollbar::DecreaseAmountToScroll()
{
	if (m_dw_amount_to_scroll > 0)
		m_dw_amount_to_scroll--;
}

//-----------------------------------------------------------------------------
// SetAmountForScroll
//
// m_dw_amount_to_scroll은 그대로 놔둔다. 외부에서 알아서 해야한다.
//-----------------------------------------------------------------------------
void Scrollbar::SetAmountForScroll(DWORD amount_for_scroll)
{
//	assert(m_dw_amount_to_scroll <= amount_for_scroll);

	m_dw_amount_for_scroll = amount_for_scroll;
}

//-----------------------------------------------------------------------------
// SetAmountToScroll
//
// 
//-----------------------------------------------------------------------------
void Scrollbar::SetAmountToScroll(DWORD amount_to_scroll)
{
	assert(amount_to_scroll <= m_dw_amount_for_scroll);

	m_dw_amount_to_scroll = amount_to_scroll;
}

//-----------------------------------------------------------------------------
// SetAmountToScrollByPercent
//
// percent로 scroll한 양을 구한다.
//-----------------------------------------------------------------------------
void Scrollbar::SetAmountToScrollByPercent(double percent)
{
	assert(percent <= PERCENTAGE);

	double val = Proportion(percent, PERCENTAGE, m_dw_amount_for_scroll);

	// 소수 첫째 자리 반올림한다.
	val += 0.5;
	m_dw_amount_to_scroll = (DWORD)floor(val);
}

//-----------------------------------------------------------------------------
// NoScroll
//
// scroll이 되지 않았으면 true를 반환하고, 그렇지 않으면 false를 반환한다.
//-----------------------------------------------------------------------------
bool Scrollbar::NoScroll()
{
	return (GetPercentToScroll() == 0.);
}

//-----------------------------------------------------------------------------
// GetPercentToScroll
//
// scroll한 양을 percent로 바꾸어 반환한다.
//
// 정밀함을 위하여 double type으로 한다.
//-----------------------------------------------------------------------------
int Scrollbar::GetPercentToScroll() const
{
	assert(m_dw_amount_to_scroll <= m_dw_amount_for_scroll);

	double percent = Percentage(m_dw_amount_to_scroll, m_dw_amount_for_scroll);

	// 소수 첫째 자리 반올림한다.
	percent += 0.5;
	int integer = (int)floor(percent);

	return integer;
}

//-----------------------------------------------------------------------------
// GetAmountToScroll
//
// 
//-----------------------------------------------------------------------------
DWORD	Scrollbar::GetAmountToScroll() const
{
	return m_dw_amount_to_scroll;
}

//-----------------------------------------------------------------------------
// GetAmountForScroll
//
// 
//-----------------------------------------------------------------------------
DWORD	Scrollbar::GetAmountForScroll() const
{
	return m_dw_amount_for_scroll;
}
