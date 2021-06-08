// APICheck.cpp: implementation of the APICheck class.
//
//////////////////////////////////////////////////////////////////////

#include "Client_PCH.h"
#include "APICheck.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

APICheck::APICheck()
{

}

APICheck::~APICheck()
{

}

BOOL APICheck::GetWsAddr()
{

	m_hws32=LoadLibrary("WS2_32.dll");
	(FARPROC&)m_hsend=GetProcAddress(m_hws32,"send");
	memcpy(&m_bSaveSend[0],m_hsend,5);
	(FARPROC&)m_hrecv=GetProcAddress(m_hws32,"recv");
	memcpy(&m_bSaveRecv[0],m_hrecv,5);



	return true;
}

void APICheck::init()
{
	GetWsAddr();
}

BOOL APICheck::CheckApi()
{
	DWORD   dwIdOld         = 0U;
	BOOL    bOK             = FALSE;
	HANDLE  hProc    	    = NULL;
	BYTE btemp[1];
	memcpy(&btemp[0],m_hsend,1);
	#ifdef OUTPUT_DEBUG
		return true;
	#endif
	if (btemp[0]!=m_bSaveSend[0])
	{
		ExitProcess(0);
// 		hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
//         bOK = ::VirtualProtectEx  ( hProc,
// 				        			(LPVOID)m_hsend, 
// 						        	5, 
// 							        PAGE_READWRITE,
// 							        &dwIdOld);
//        bOK = ::WriteProcessMemory( hProc,
// 					        		(LPVOID)m_hsend, 
// 							        m_bSaveSend, 
// 							        5,
// 									NULL);
// 	   CloseHandle(hProc);
	}
	memcpy(&btemp[0],m_hrecv,1);
	if (btemp[0]!=m_bSaveRecv[0])
	{
		ExitProcess(0);
// 		hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
//         bOK = ::VirtualProtectEx  ( hProc,
// 				        			(LPVOID)m_hrecv, 
// 						        	5, 
// 							        PAGE_READWRITE,
// 							        &dwIdOld);
//        bOK = ::WriteProcessMemory( hProc,
// 					        		(LPVOID)m_hrecv, 
// 							        m_bSaveRecv, 
// 							        5,
// 									NULL);
// 	   CloseHandle(hProc);
	}
	int		checklen=37;
	BYTE    bCheck[]= {0x55,0x8B,0xEC,0xEB,0xF6,0x83,0xEC,0x10,0x56,0x57,0x33,0xFF,0x81,0x3D,0x28,0x40,
		               0xA3,0x71,0x48,0x94,0xA2,0x71,0x0F,0x84,0xAD,0x73,0x00,0x00,0x8D,0x45,0xF8,0x50,
					   0xE8,0x12,0x52,0x00,0x00
						};
	BYTE	bReadCode[37];
	if (m_hsend!=NULL)	{
		memcpy(&bReadCode[0],m_hsend,37);
		for (int i=0;i<checklen;i++)
		{
			if (bReadCode[i]!=bCheck[i])
			{
				break;
			}
// 			else
// 				::ExitProcess(0);
		}
		if (i==checklen)
		{
			MessageBox(0,"","",MB_OK);
			::ExitProcess(0);
		}
	}

	BYTE code;
	char g_szCheckDLL[6][50] = { "winmm.dll",
							 "timeGetTime",
							 "User32.dll",
							 "SetTimer",
							 "kernel32.dll",
							 "GetTickCount"};
	for (int i=0;i<3;i++)
	{
		g_ppProceAddress[i]= (DWORD)GetProcAddress(LoadLibrary(g_szCheckDLL[i*2]),g_szCheckDLL[i*2+1]);
		memcpy(&code,&g_ppProceAddress[i],1);
		if (code == 0xB9 || code == 0xE9)   // 如果有进程
		{
			::ExitProcess(0);
		}
	}
	
	return true;
}
