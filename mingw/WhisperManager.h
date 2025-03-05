//----------------------------------------------------------------------
// WhisperManager.h
//----------------------------------------------------------------------
// 다른 사용자와의 귓속말을 잘~ 전달하기 위한...
// 
// 서버에서 IP를 얻어 P2P 연결.
/*

	//---------------------------------------------------------
	// [0] 귓속말은 Keyboard입력에 의해서 시작된다.
	//---------------------------------------------------------
	UI_INPUT --(whisper)--> [1] WhisperManager::Send(name, message)

	//---------------------------------------------------------
	// [1] 귓속말 보낼 때
	//---------------------------------------------------------
	WhisperManager::SendMessage(toUser, message)
	{
		if (has toUser's IP in RUM)		// RUM = RequestUserManager
		{			
			RequestClientManager --(CRWhisper)--> RequestServer(toUser) [2]			
		}
		else
		{
			CGWhisper --> GameServer [3]
		}
	}

	//---------------------------------------------------------
	// [2] RequestClientManager --> RequestServer
	//---------------------------------------------------------
	RequestClientManager::ConnectForOnePacket(toUser, IP, packet)
	{
		if (connect(toUser, IP))
		{
			sendPacket(packet) --> toUser [5]			
		}
		else
		{
			// 접속 실패하면 -_-;
			RequestUserManager::RemoveRequestUser( toUser )
		}
	}

	//---------------------------------------------------------
	// [3] GameServer received CGWhisper
	//---------------------------------------------------------
	CGWhisperHandler(fromUser, toUser)
	{
		if (exist toUser in current GameServer 
			|| exist toUser in CurrentConnectedUser)
		{
			GCRequestedIP(toUser's ip) --> client(fromUser) [4]
		}
		else
		{
			GCWhisperFailed --> client(fromUser)
		}
	}

	//---------------------------------------------------------
	// [4] GCRequestedIP
	//---------------------------------------------------------
	GCRequestedIPHandler(toUser, IP)
	{
		RequestUserManager::addRequestUser( toUser, IP )

		if (WhisperManager::HasMessage(toUser))
		{
			RequestClientManager --(CRWhisper)--> RequestServer(toUser) [2]			
		}
	}
	
	//---------------------------------------------------------
	// [5] RequestServer(toUser)
	//---------------------------------------------------------
	RequestServer::Receive(fromUser, toUser, packet)
	{
		if (toUser != me)
		{
			send RCWhisperFailed --> fromUser [6]
		}

		// 한번 packet을 받으면 끊어버린다.
		disconnect()
	}

	//---------------------------------------------------------
	// [6] RequestClient(fromUser)
	//---------------------------------------------------------
	RequestClient::RCWhisperFailed(toUser)
	{
		// toUser는 다른 사람이다.
		RequestUserManager::RemoveRequestUser( toUser )
	}


//--------------------------------------------------------------------*/

#ifndef __WHISPER_MANAGER_H__
#define __WHISPER_MANAGER_H__

#pragma warning(disable:4786)

#include <windows.h>
#include <map>
#include <string>
#include <list>

struct WHISPER_MESSAGE
{
	std::string msg;
	DWORD color;
};

//----------------------------------------------------------------------
// WhisperInfo
//----------------------------------------------------------------------
// 귓속말에 대한 정보(받는사람, 보낼내용)
//----------------------------------------------------------------------
class WhisperInfo {
	public :
		std::string					Name;
		std::list<WHISPER_MESSAGE>	Messages;

		int							TryingCount;	// 접속 시도 회수

	public :
		WhisperInfo()
		{
			TryingCount = 0;
		}

		void		SendToGameServer() const;
};

//----------------------------------------------------------------------
// WhisperManager
//----------------------------------------------------------------------
// 귓속말 관리...
//----------------------------------------------------------------------
class WhisperManager {
	public :
		typedef std::map<std::string, WhisperInfo*>		WHISPER_INFO_MAP;

	public :
		WhisperManager();
		~WhisperManager();

		//--------------------------------------------------------------
		// Release
		//--------------------------------------------------------------
		void		Release();

		//--------------------------------------------------------------
		// Message
		//--------------------------------------------------------------
		bool		HasWhisperMessage(const char* pName) const;
		const std::list<WHISPER_MESSAGE>*	GetWhisperMessages(const char* pName) const;

		void		SendWhisperMessage(const char* pName, const char* pMessage, DWORD color);

		void		AddWhisperMessage(const char* pName, const char* pMessage, DWORD color);		
		bool		RemoveWhisperMessage(const char* pName);
		void		TryToSendWhisperMessage(const char* pName);

		//--------------------------------------------------------------
		// Update
		//--------------------------------------------------------------
		void		Update();

	protected :
		//----------------------------------------------------------------------
		// Lock / Unlock
		//----------------------------------------------------------------------
		void		Lock()					{ EnterCriticalSection(&m_Lock); }
		void		Unlock()				{ LeaveCriticalSection(&m_Lock); }

	private :
		CRITICAL_SECTION		m_Lock;

		WHISPER_INFO_MAP		m_WhisperInfos;
};


extern WhisperManager*		g_pWhisperManager;


#endif

