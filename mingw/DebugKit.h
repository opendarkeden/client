// DebugKit.h: interface for the CDebugKit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEBUGKIT_H__6554585F_E870_43CA_A7F9_D8233A8EFB90__INCLUDED_)
#define AFX_DEBUGKIT_H__6554585F_E870_43CA_A7F9_D8233A8EFB90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Properties.h"
////
//yckou added 2005.09.11
//#define WINDOWS_SCREEN_DISPLAY
//yckou added 2005.10.04
#ifdef WINDOWS_SCREEN_DISPLAY
#define DEBUG_INFO
#endif

class CDebugKit  
{
public:
	CDebugKit();
	virtual ~CDebugKit();

public:
	BOOL load();
	std::string GetMsgFileName();
	BOOL GetMsgOutPutFlag();
	BOOL GetMsgDetailFlag();
	BOOL GetMsgContentFlag();
	BYTE GetAuthKeyMap();
	int GetGameVersion();
private:
	Properties *m_DebugInfo;
	
};

class CMessageStringTable
{
public:
	CMessageStringTable();
	~CMessageStringTable();

	void	SaveToFile(std::string strFileName);
	void	LoadFromFile(std::string strFileName);

	BOOL IsExistMessageName( WORD MessageID ) { return m_MessageName.find( MessageID ) != m_MessageName.end(); }
	const char *GetMessageName(WORD MessageID);
	void SetMessageName(WORD MessageID, std::string& name) { m_MessageName[MessageID] = name; }
	void ClearMessageName() { m_MessageName.clear(); }
private:
	std::map<WORD, std::string> m_MessageName;
	
};
#endif // !defined(AFX_DEBUGKIT_H__6554585F_E870_43CA_A7F9_D8233A8EFB90__INCLUDED_)