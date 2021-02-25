#ifndef __MCRASHREPORT_H__
#define __MCRASHREPORT_H__

#include "CTypeTable.h"
#include "MString.h"

class MCrashReport
{
private:
	MString	m_ExecutableTime;
	WORD	m_Version;
	MString	m_Address;
	MString	m_OS;
	MString	m_CallStack;
	MString	m_Message;

public:
	void	SetExcutableTime(const char *pszExecutableTime)		{ m_ExecutableTime = pszExecutableTime; }
	void	SetVersion(const WORD nVersion)						{ m_Version = nVersion; }
	void	SetAddress(const char *pszAddress)					{ m_Address = pszAddress; }
	void	SetOS(const char *pszOS)							{ m_OS = pszOS; }
	void	SetCallStack(const char *pszCallStack)				{ m_CallStack = pszCallStack; }
	void	SetMessage(const char *pszMessage)					{ m_Message = pszMessage; }

	const char *	GetExecutableTime() const					{ return m_ExecutableTime.GetString(); }
	const WORD		GetVersion() const							{ return m_Version; }
	const char *	GetAddress() const							{ return m_Address.GetString(); }
	const char *	GetOS() const								{ return m_OS.GetString(); }
	const char *	GetCallStack() const						{ return m_CallStack.GetString(); }
	const char *	GetMessage() const							{ return m_Message.GetString(); }

	void	LoadFromFile(class ifstream &file);
	void	SaveToFile(class ofstream &file);
	
};

typedef CTypeTable<MCrashReport> MCrashReportManager;

#endif