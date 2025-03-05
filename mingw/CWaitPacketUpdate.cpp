//-----------------------------------------------------------------------------
// CWaitPacketUpdate.cpp
//-----------------------------------------------------------------------------
#include "Client_PCH.h"
#pragma warning(disable:4786)
#include "CWaitPacketUpdate.h"
#include "Client.h"


// Global
CWaitPacketUpdate*		g_pCWaitPacketUpdate = NULL;

//-----------------------------------------------------------------------------
// Init
//-----------------------------------------------------------------------------
void
CWaitPacketUpdate::Init()
{
	// mouse event 처리
	g_pDXInput->SetMouseEventReceiver( NULL );

	// keyboard event 처리
	g_pDXInput->SetKeyboardEventReceiver( NULL );
}


//-----------------------------------------------------------------------------
// Update WaitPosition
//-----------------------------------------------------------------------------
// 게임 시작 전에.. Player의 좌표를 받기 위해서 기다린다.
//-----------------------------------------------------------------------------
void 
CWaitPacketUpdate::Update()
{
	//DEBUG_ADD("[WaitPacketUpdate] Begin");

	//------------------------------------------	
	// 한계 시간을 넘어갔으면..
	//------------------------------------------
	if ((DWORD)g_CurrentTime > m_DelayLimit)
	{
		DEBUG_ADD_FORMAT("[WaitPacket] 시간 초과 : Current=%d, Limit=%d", g_CurrentTime, m_DelayLimit);			

		SetMode( MODE_MAINMENU );
		UpdateDisconnected();

	}

	//------------------------------------------
	// Sound Stream
	//------------------------------------------
//	if (g_pDXSoundStream!=NULL)
//	{
//		//DEBUG_ADD("[WaitPacketUpdate] SoundStream");
//
//		g_pDXSoundStream->Update();
//	}

	//------------------------------------------
	// Socket부분 처리
	//------------------------------------------	
	if (!UpdateSocketInput())
	{
		//DEBUG_ADD("[WaitPacketUpdate] SocketInput");

		return;
	}

	//------------------------------------------	
	// Mode가 바뀐 경우
	//------------------------------------------	
	if (g_ModeNext!=MODE_NULL)
	{
		DEBUG_ADD_FORMAT("[WaitPacketUpdate] g_ModeNext is Not MODE_NULL(%d)", (int)g_ModeNext);

		SetMode( g_ModeNext );
		g_ModeNext = MODE_NULL;

		return;
	}

	//DEBUG_ADD("[WaitPacketUpdate] SocketOutput");
	
	UpdateSocketOutput();

	//DEBUG_ADD("[WaitPacketUpdate] End");
}

//-----------------------------------------------------------------------------
// Set Delay
//-----------------------------------------------------------------------------
// 현재 시간(g_CurrentTime)부터 delay만큼 더 Update를 하다가
// Server부터 메세지가 오지 않으면 .... 프로그램 종료~
//-----------------------------------------------------------------------------
void		
CWaitPacketUpdate::SetDelay(DWORD delay)
{
	g_CurrentTime = timeGetTime();
			
	m_DelayLimit = g_CurrentTime + delay;

	DEBUG_ADD_FORMAT("[WaitPacket] Current=%d, Limit=%d", g_CurrentTime, m_DelayLimit);
}