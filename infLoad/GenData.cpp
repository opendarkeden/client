// GenData.cpp: implementation of the CGenData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "infLoad.h"
#include "GenData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGenData::CGenData(CListCtrl &List)
{
	CLList = &List;
}

CGenData::~CGenData()
{

}

//////////////////////////////////////////////////////////////////////////
// 定义
//////////////////////////////////////////////////////////////////////////
int CGenData::GetListInt(int nItem)
{
	CString m_cszTemp;
	int m_itemp;

	m_cszTemp = CLList->GetItemText(nItem,1);
	
	m_itemp = atoi(m_cszTemp.GetBuffer(m_cszTemp.GetLength()));

	if(m_itemp == 0 && m_cszTemp != "0")
	{
		AfxMessageBox("请输入数字!");
		return -2;
	}

	return m_itemp;
}

CString CGenData::GetListCString(int nItem)
{
	return CLList->GetItemText(nItem,1);
}

BYTE CGenData::GetListBYTE(int nItem)
{
	CString m_cszTemp;
	BYTE m_Btemp;

	m_cszTemp = CLList->GetItemText(nItem,1);
	
	m_Btemp = atoi(m_cszTemp.GetBuffer(m_cszTemp.GetLength()));

	if (m_Btemp > 255)
	{
		AfxMessageBox("该数据非BYTE类型 请更正后再试!");
		return -3;
	}

	if(m_Btemp == 0 && m_cszTemp != "0")
	{
		AfxMessageBox("请输入数字!");
		return -2;
	}

	return m_Btemp;
}

BOOL CGenData::GetListBool(int nItem)
{
	CString m_cszTemp;
	long m_Btemp;

	m_cszTemp = CLList->GetItemText(nItem,1);
	
	m_Btemp = atoi(m_cszTemp.GetBuffer(m_cszTemp.GetLength()));

	if (m_Btemp)
		return TRUE;
	else
		return FALSE;
}