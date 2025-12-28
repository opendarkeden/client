// APICheck.h: interface for the APICheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APICHECK_H__9A3E0710_3AC7_4BC7_955A_323AAC3F0D3A__INCLUDED_)
#define AFX_APICHECK_H__9A3E0710_3AC7_4BC7_955A_323AAC3F0D3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class APICheck  
{
public:
	BOOL CheckApi();
	void init();
	APICheck();
	virtual ~APICheck();
	HMODULE m_hws32;
	int (__stdcall *m_hsend)(HWND ,LPSTR,LPSTR,int);
	int (__stdcall *m_hrecv)(HWND ,LPSTR,LPSTR,int);
	BYTE m_bSaveSend[5];//保存发送封包地址
	BYTE m_bSaveRecv[5];//保存发送封包地址
private:
	BOOL GetWsAddr();
private:
	DWORD g_ppProceAddress[3];
};

#endif // !defined(AFX_APICHECK_H__9A3E0710_3AC7_4BC7_955A_323AAC3F0D3A__INCLUDED_)
