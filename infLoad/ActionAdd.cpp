// ActionAdd.cpp: implementation of the CActionAdd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "infLoad.h"
#include "ActionAdd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CActionAdd::CActionAdd()
{

}

CActionAdd::~CActionAdd()
{

}

void CActionAdd::LoadFile(const char *FileName)
{
	CString m_pach = ".\\data\\Info\\Action.inf";

	if (g_pActionInfoTable != NULL)
	{
		delete g_pActionInfoTable;
		g_pActionInfoTable = NULL;
	}

	g_pActionInfoTable = new MActionInfoTable;

	class ifstream actionInfoFile2;
	if (!FileOpenBinary(m_pach, actionInfoFile2))
		return;
	g_pActionInfoTable->LoadFromFile(actionInfoFile2);
}
