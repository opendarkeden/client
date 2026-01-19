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

//-----------------------------------------------------------------------------
void Scrollbar::IncreaseAmountToScroll()
{
	if (m_dw_amount_to_scroll < m_dw_amount_for_scroll)
		m_dw_amount_to_scroll++;
}

//-----------------------------------------------------------------------------
// DecreaseAmountToScroll
//

//-----------------------------------------------------------------------------
void Scrollbar::DecreaseAmountToScroll()
{
	if (m_dw_amount_to_scroll > 0)
		m_dw_amount_to_scroll--;
}

//-----------------------------------------------------------------------------
// SetAmountForScroll
//

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

//-----------------------------------------------------------------------------
void Scrollbar::SetAmountToScrollByPercent(double percent)
{
	assert(percent <= PERCENTAGE);

	double val = Proportion(percent, PERCENTAGE, m_dw_amount_for_scroll);


	val += 0.5;
	m_dw_amount_to_scroll = (DWORD)floor(val);
}

//-----------------------------------------------------------------------------
// NoScroll
//

//-----------------------------------------------------------------------------
bool Scrollbar::NoScroll()
{
	return (GetPercentToScroll() == 0.);
}

//-----------------------------------------------------------------------------
// GetPercentToScroll
//

//

//-----------------------------------------------------------------------------
int Scrollbar::GetPercentToScroll() const
{
	assert(m_dw_amount_to_scroll <= m_dw_amount_for_scroll);

	double percent = Percentage(m_dw_amount_to_scroll, m_dw_amount_for_scroll);


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
