// ToActionInfo.h: interface for the ToActionInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOACTIONINFO_H__27E4BF13_3384_400E_905B_1A533329D1A0__INCLUDED_)
#define AFX_TOACTIONINFO_H__27E4BF13_3384_400E_905B_1A533329D1A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\CLIENT\MActionInfoTable.h"

class ToActionInfo : public MActionInfo  
{
public:
	ToActionInfo();
	virtual ~ToActionInfo();
public:
	void NoUseRepeatFrame() {m_bUseRepeatFrame = false;}
	void UnSetCurrentWeapon() {m_fCurrentWeapon = 0;}
	BYTE GetfCurrentWeapon() {return m_fCurrentWeapon;}
};

#endif // !defined(AFX_TOACTIONINFO_H__27E4BF13_3384_400E_905B_1A533329D1A0__INCLUDED_)
