//--------------------------------------------------------------------------------
// RequestFunction.cpp
//--------------------------------------------------------------------------------
#include "Client_PCH.h"
#include "RequestServerPlayerManager.h"
#include "RequestClientPlayerManager.h"
#include "DebugInfo.h"


//--------------------------------------------------------------------------------
// Request Connect
//--------------------------------------------------------------------------------
// IP의 컴퓨터에 Name이란 캐릭터에게 접속한다.
//--------------------------------------------------------------------------------
void	
RequestConnect(const char* pIP, const char* pName)
{
	DEBUG_ADD_FORMAT("[RequestConnect] ip=%s, name=%s", pIP, pName);

	if (g_pRequestClientPlayerManager!=NULL)
	{
		g_pRequestClientPlayerManager->Connect( pIP, pName );
	}
}

//--------------------------------------------------------------------------------
// Request Disconnect
//--------------------------------------------------------------------------------
// Name이란 캐릭터로부터의 접속을 해제한다.
//--------------------------------------------------------------------------------
void	
RequestDisconnect(const char* pName)
{
	DEBUG_ADD_FORMAT("[RequestDisconnect] name=%d", pName);

	//-------------------------------------------------------------------
	// 내가 Name한테 접속해 있는 걸 끊는다.
	//-------------------------------------------------------------------
	if (g_pRequestClientPlayerManager!=NULL)
	{
		g_pRequestClientPlayerManager->Disconnect( pName );
	}

	//-------------------------------------------------------------------
	// Name이 나한테 접속해 있는걸 끊는다.
	//-------------------------------------------------------------------
	if (g_pRequestServerPlayerManager!=NULL)
	{
		g_pRequestServerPlayerManager->Disconnect( pName );
	}
}