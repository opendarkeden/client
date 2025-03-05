// heckSystem.cpp: implementation of the CheckSystem class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "checkSystemVer.h"
#include "heckSystem.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CheckSystem::CheckSystem()
{

}

CheckSystem::~CheckSystem()
{

}

BOOL CheckSystem::GetSystem()
{
	OSVERSIONINFO OsInfo;
	OsInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (GetVersionEx(&OsInfo))
	{
		// platform
		switch (OsInfo.dwPlatformId)
		{
		case VER_PLATFORM_WIN32_WINDOWS:
			if(OsInfo.dwMajorVersion == 3)
				//MessageBox(0,"操作系统:WIN95","信息",MB_OK);
				//strcpy(sys.chSystem,"操作系统:WIN95");
				//return FALSE;
			else if(OsInfo.dwMajorVersion == 4)
				//MessageBox(0,"操作系统:WIN98","信息",MB_OK);
				//return FALSE;
				//strcpy(sys.chSystem,"操作系统:WIN98");
			break;
		case VER_PLATFORM_WIN32_NT:
			if(OsInfo.dwMajorVersion == 5)
				//MessageBox(0,"操作系统:WIN2000","信息",MB_OK);
				//return TRUE;
				//strcpy(sys.chSystem,"操作系统:WIN2000");
			else
				//return TRUE;
				//MessageBox(0,"操作系统:WINNT","信息",MB_OK);
				//strcpy(sys.chSystem,"操作系统:WINNT");
			break;
		default:
			//return FALSE;
			//strcpy(sys.chSystem,"未知系统!!");
			break;
		}
		// version and language
	}
	return FALSE;
}
