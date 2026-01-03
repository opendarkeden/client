//--------------------------------------------------------------------------------
// RequestClientPlayerManager.cpp
//--------------------------------------------------------------------------------

//#include "RequestServerPlayerManager.h"
#include "Client_PCH.h"
#include "RequestClientPlayerManager.h"
#include "WhisperManager.h"
#include "ClientConfig.h"
#include "MPlayer.h"
#include "UserInformation.h"
#include "MGameStringTable.h"
#include "RequestUserManager.h"
#include "ProfileManager.h"
#include "DebugInfo.h"
#include "ServerInfo.h"
#include "ClientDef.h"

#include "Rpackets\CRConnect.h"
#include "Rpackets\CRWhisper.h"
#include "Rpackets\CRRequest.h"

#if defined(_DEBUG) && defined(OUTPUT_DEBUG)
	extern CMessageArray*		g_pGameMessage;
#endif


//--------------------------------------------------------------------------------
// Global
//--------------------------------------------------------------------------------
RequestClientPlayerManager*	g_pRequestClientPlayerManager = NULL;

LONG					RequestConnectionThreadProc(LPVOID lpParameter);

//--------------------------------------------------------------------------------
//
// constructor / destructor
//
//--------------------------------------------------------------------------------
RequestClientPlayerManager::RequestClientPlayerManager()
{
	InitializeCriticalSection(&m_Lock);
}

RequestClientPlayerManager::~RequestClientPlayerManager()
{
	Release();

	DeleteCriticalSection(&m_Lock);
}

//--------------------------------------------------------------------------------
// Release
//--------------------------------------------------------------------------------
void
RequestClientPlayerManager::Release()
{
	Lock();

	//------------------------------------------------------------------------
	// thread
	//------------------------------------------------------------------------
	HANDLE_LIST::iterator iHandle = m_listConnectionThread.begin();

	while (iHandle != m_listConnectionThread.end())
	{
		HANDLE handle = *iHandle;

		TerminateThread( handle, 0 );
		CloseHandle( handle );

		iHandle ++;
	}

	m_listConnectionThread.clear();

	//------------------------------------------------------------------------
	// m_mapRequestClientPlayer
	//------------------------------------------------------------------------
	REQUESTCLIENTPLAYER_MAP::iterator iPlayer = m_mapRequestClientPlayer.begin();
		
	while (iPlayer != m_mapRequestClientPlayer.end())
	{
		RequestClientPlayer* pPlayer = iPlayer->second;

		try {
			delete pPlayer;
		} catch (Throwable&) {
		}
		
		iPlayer++;
	}

	m_mapRequestClientPlayer.clear();

	//------------------------------------------------------------------------
	// m_mapConnectionInfo
	//------------------------------------------------------------------------
	CONNECTION_INFO_MAP::iterator iInfo = m_mapConnectionInfo.begin();
		
	while (iInfo != m_mapConnectionInfo.end())
	{	
		CONNECTION_INFO* pInfo = iInfo->second;

		delete pInfo;

		iInfo++;
	}

	m_mapConnectionInfo.clear();
		

	Unlock();
}

//----------------------------------------------------------------------
// Remove Terminated Thread
//----------------------------------------------------------------------
void		
RequestClientPlayerManager::RemoveTerminatedThread()
{
	HANDLE_LIST::iterator iHandle = m_listConnectionThread.begin();

	DWORD exitCode;
		
	while (iHandle != m_listConnectionThread.end())
	{
		HANDLE handle = *iHandle;

		if (GetExitCodeThread(handle, &exitCode))
		{
			if (exitCode!=STILL_ACTIVE)
			{
				CloseHandle( handle );

				HANDLE_LIST::iterator iTemp = iHandle ++;

				m_listConnectionThread.erase( iTemp );

				continue;
			}
		}

		iHandle ++;
	}
}

//----------------------------------------------------------------------
// Connect
//----------------------------------------------------------------------
// IP의 컴퓨터에 Name이란 캐릭터에게 접속한다.
//
// 게임 돌아가는데 지장을 주지 않기 위해서
// thread를 하나 생성해서 접속을 시도한다.
// 
// 접속이 실패하면? 어떻하면 될까.. - -;;
//----------------------------------------------------------------------
void		
RequestClientPlayerManager::Connect(const char* pIP, const char* pRequestName, REQUEST_CLIENT_MODE requestMode)
{
	// 넘 많을 경우는 더 이상 접속을 안하도록 해야한다.
	if (m_mapRequestClientPlayer.size() >= g_pClientConfig->MAX_REQUEST_SERVICE)
	{
		return;
	}

	if (HasConnection(pRequestName)
		|| HasTryingConnection(pRequestName))
	{
		return;
	}

	int port = 9650;
	
	//RequestUserInfo* pUserInfo = g_pRequestUserManager->GetUserInfo(pRequestName);

	//if (pUserInfo!=NULL && pUserInfo->TCPPort!=0)
	{
	//	port = pUserInfo->TCPPort;
	}

	// 일단 하나 생성해서 thread로 넘겨준다.
	CONNECTION_INFO* pInfo = new CONNECTION_INFO;
	pInfo->name			= pRequestName;
	pInfo->ip			= pIP;
	pInfo->port			= port;
	pInfo->requestMode	= requestMode;

	DWORD dwChildThreadID;	// 의미 없당 -- ;


	//------------------------------------------------------------
	// 접속 시도
	//------------------------------------------------------------
	Lock();		// 같이 lock쓴다. - -;

	m_mapConnectionInfo[pInfo->name] = pInfo;
	
	HANDLE hConnectionThread = CreateThread(NULL, 
									0,	// default stack size
									(LPTHREAD_START_ROUTINE)RequestConnectionThreadProc,
									pInfo,
									NULL,
									&dwChildThreadID);

	// priority는 낮게
	SetThreadPriority(hConnectionThread, THREAD_PRIORITY_LOWEST);	

	// 나중에 지울 수 있게 추가해둔다.
	m_listConnectionThread.push_back( hConnectionThread );

	Unlock();
}

//----------------------------------------------------------------------
// Send Packet
//----------------------------------------------------------------------
bool		
RequestClientPlayerManager::SendPacket(const char* pName, Packet* pPacket)
{
	Lock();

	REQUESTCLIENTPLAYER_MAP::const_iterator iPlayer = m_mapRequestClientPlayer.find( std::string(pName) );
		
	if (iPlayer != m_mapRequestClientPlayer.end())
	{
		RequestClientPlayer* pPlayer = iPlayer->second;

		pPlayer->sendPacket( pPacket );

		Unlock();
		return true;
	}

	Unlock();
	return false;
	
}

//----------------------------------------------------------------------
// Has Connection
//----------------------------------------------------------------------
bool	
RequestClientPlayerManager::HasConnection(const char* pRequestName)
{
	Lock();

	std::string name = pRequestName;

	// 연결된 경우
	REQUESTCLIENTPLAYER_MAP::const_iterator iPlayer = m_mapRequestClientPlayer.find( name );
		
	if (iPlayer != m_mapRequestClientPlayer.end())
	{
		Unlock();		
		return true;		
	}
	
	Unlock();
	return false;	
}

//----------------------------------------------------------------------
// Has TryingConnection
//----------------------------------------------------------------------
bool	
RequestClientPlayerManager::HasTryingConnection(const char* pRequestName)
{
	Lock();

	std::string name = pRequestName;

	// 접속 시도 중인 경우
	CONNECTION_INFO_MAP::const_iterator iInfo = m_mapConnectionInfo.find( name );
		
	if (iInfo != m_mapConnectionInfo.end())
	{	
		Unlock();
		return true;
	}

	Unlock();
	return false;	
}

//----------------------------------------------------------------------
// Disconnect
//----------------------------------------------------------------------
void		
RequestClientPlayerManager::Disconnect(const char* pRequestName)
{
	Lock();

	REQUESTCLIENTPLAYER_MAP::iterator iPlayer = m_mapRequestClientPlayer.find( std::string(pRequestName) );
		
	if (iPlayer != m_mapRequestClientPlayer.end())
	{
		RequestClientPlayer* pPlayer = iPlayer->second;

		try {

			m_mapRequestClientPlayer.erase( iPlayer );
			delete pPlayer;				

		} catch (Throwable& t)	{
			DEBUG_ADD_ERR( t.toString().c_str() );
		}

		Unlock();
		return;		
	}

	Unlock();
}

//--------------------------------------------------------------------------------
// Add RequestClientPlayer
//--------------------------------------------------------------------------------
bool
RequestClientPlayerManager::AddRequestClientPlayer(RequestClientPlayer* pRequestClientPlayer)
{
	Lock();	// 구차나서 같은 락을.. - -;

	bool bAdd = false;

	std::string serverName = pRequestClientPlayer->getRequestServerName();

	if (g_Mode==MODE_GAME)
	{		
		// 넘 많을 경우는 더 이상 접속을 안하도록 해야한다.
		//if (m_mapRequestClientPlayer.size() < g_pClientConfig->MAX_REQUEST_SERVICE)
		{
			//------------------------------------------------------------
			// 일단 list에 넣어둔다.
			//------------------------------------------------------------
			m_mapRequestClientPlayer[pRequestClientPlayer->getRequestServerName()] = pRequestClientPlayer;

			pRequestClientPlayer->setPlayerStatus( CPS_REQUEST_CLIENT_BEGIN_SESSION );

			#if defined(_DEBUG) && defined(OUTPUT_DEBUG)
				if (g_pGameMessage!=NULL)
					g_pGameMessage->AddFormat("Connected To %s", pRequestClientPlayer->getRequestServerName().c_str());
			#endif

			bAdd = true;
		}
	}
	else
	{
		pRequestClientPlayer->disconnect( UNDISCONNECTED );
		delete pRequestClientPlayer;
	}
	
	
	CONNECTION_INFO_MAP::iterator iInfo = m_mapConnectionInfo.find( serverName );
		
	if (iInfo != m_mapConnectionInfo.end())
	{	
		CONNECTION_INFO* pInfo = iInfo->second;

		delete pInfo;
		m_mapConnectionInfo.erase( iInfo );
	}

	Unlock();

	return bAdd;	
}

//--------------------------------------------------------------------------------
// Add RequestClientPlayer
//--------------------------------------------------------------------------------
bool
RequestClientPlayerManager::RemoveRequestClientPlayer(const char* pRequestName)
{
	Lock();

	REQUESTCLIENTPLAYER_MAP::iterator iPlayer = m_mapRequestClientPlayer.find( std::string(pRequestName) );
		
	if (iPlayer != m_mapRequestClientPlayer.end())
	{
		RequestClientPlayer* pPlayer = iPlayer->second;

		try {

			pPlayer->disconnect(UNDISCONNECTED);
			delete pPlayer;

		} catch (Throwable& t) {
			DEBUG_ADD_ERR( t.toString().c_str() );
		}

		Unlock();
		return true;
	}	

	Unlock();

	return false;
}

//--------------------------------------------------------------------------------
// Remove ConnectionInfo
//--------------------------------------------------------------------------------
void
RequestClientPlayerManager::RemoveConnectionInfo(const char* pName)
{
	Lock();

	CONNECTION_INFO_MAP::iterator iInfo = m_mapConnectionInfo.find( pName );

	if (iInfo != m_mapConnectionInfo.end())
	{	
		CONNECTION_INFO* pInfo = iInfo->second;

		delete pInfo;

		m_mapConnectionInfo.erase( iInfo );
	}

	Unlock();
}

//--------------------------------------------------------------------------------
// Process Mode
//--------------------------------------------------------------------------------
// RequestMode에 따른 특별한 처리
//--------------------------------------------------------------------------------
void
RequestClientPlayerManager::ProcessMode(RequestClientPlayer* pRequestClientPlayer)
{
	//------------------------------------------------------------
	// 최초의 접속요구 패킷
	//------------------------------------------------------------
	switch (pRequestClientPlayer->getRequestMode())
	{
		//------------------------------------------------------------
		// 계속 접속유지 시켜서 뭔가 할떄..
		//------------------------------------------------------------
		case REQUEST_CLIENT_MODE_NULL :
		{
			if (pRequestClientPlayer->getPlayerStatus()==CPS_REQUEST_CLIENT_BEGIN_SESSION)
			{
				CRConnect _CRConnect;
				_CRConnect.setRequestServerName( pRequestClientPlayer->getRequestServerName().c_str() );
				_CRConnect.setRequestClientName( g_pUserInformation->CharacterID.GetString() );

				// 접속 체크 설정
				pRequestClientPlayer->setPlayerStatus( CPS_REQUEST_CLIENT_AFTER_SENDING_CONNECT );			

				pRequestClientPlayer->sendPacket( &_CRConnect );
			}
		}
		break;

		//------------------------------------------------------------
		// 귓속말 보낼 때..
		//------------------------------------------------------------
		case REQUEST_CLIENT_MODE_WHISPER :
		{
			if (pRequestClientPlayer->getPlayerStatus()==CPS_REQUEST_CLIENT_BEGIN_SESSION)
			{
				const char* pRequestServerName = pRequestClientPlayer->getRequestServerName().c_str();

				// 귓속말 체크.. 
				if (g_pWhisperManager->HasWhisperMessage( pRequestServerName ))
				{
					const std::list<WHISPER_MESSAGE>* pMessageList = g_pWhisperManager->GetWhisperMessages( pRequestServerName );

					if (pMessageList)
					{
						// CRWhisper만들어서 packet보내기
						CRWhisper _CRWhisper;

						_CRWhisper.setName( g_pUserInformation->CharacterID.GetString() );
						_CRWhisper.setTargetName( pRequestServerName );
						
//						if (g_pPlayer->IsSlayer())
//						{
//							_CRWhisper.setSlayer();
//						}
//						else
//						{
//							_CRWhisper.setVampire();
//						}
						_CRWhisper.setRace(g_pPlayer->GetRace());

						_CRWhisper.setWorldID(g_pUserInformation->WorldID);

						std::list<WHISPER_MESSAGE>::const_iterator iMessage = pMessageList->begin();

						// 모든 message 추가
						while (iMessage != pMessageList->end())
						{
							_CRWhisper.addMessage ( *iMessage );

							iMessage ++;
						}

						pRequestClientPlayer->sendPacket( &_CRWhisper );

						pRequestClientPlayer->setPlayerStatus( CPS_REQUEST_CLIENT_NORMAL );
					}

					g_pWhisperManager->RemoveWhisperMessage( pRequestServerName );
				}					
			}
		}
		break;

		//------------------------------------------------------------
		// Profile을 요청할 때..
		//------------------------------------------------------------
		case REQUEST_CLIENT_MODE_PROFILE :
		{
			if (pRequestClientPlayer->getPlayerStatus()==CPS_REQUEST_CLIENT_BEGIN_SESSION)
			{
				const char* pRequestServerName = pRequestClientPlayer->getRequestServerName().c_str();

				CRRequest crRequest;
				crRequest.setCode( CR_REQUEST_FILE_PROFILE );
				crRequest.setRequestName( pRequestServerName );

				pRequestClientPlayer->sendPacket( &crRequest );

				pRequestClientPlayer->setPlayerStatus( CPS_REQUEST_CLIENT_NORMAL );
			}
		}
		break;
	}
}

//--------------------------------------------------------------------------------
// Update
//--------------------------------------------------------------------------------
void		
RequestClientPlayerManager::Update()
{
	if (m_mapRequestClientPlayer.empty())
	{
		return;
	}

	Lock();

	RemoveTerminatedThread();

	try {

		REQUESTCLIENTPLAYER_MAP::iterator iPlayer = m_mapRequestClientPlayer.begin();
			
		while (iPlayer != m_mapRequestClientPlayer.end())
		{
			RequestClientPlayer* pPlayer = iPlayer->second;

			if(pPlayer != NULL)
			{
				try {

					if (!pPlayer->getSocket()->isValid())
					{
						throw SocketException("sock error");
					}

					ProcessMode(pPlayer);
				
					pPlayer->processInput();
					pPlayer->processCommand();
					pPlayer->processOutput();

				} catch (Throwable &t) 	{
					
					DEBUG_ADD_ERR( t.toString().c_str() );

					// 나한테서도 짜른다.
					//if (g_pRequestServerPlayerManager!=NULL)
					{
					//	g_pRequestServerPlayerManager->Disconnect( pPlayer->getRequestServerName().c_str() );
					}

					// exception이 나면 무조건 잘라버린다. --;
					pPlayer->disconnect(UNDISCONNECTED);
					delete pPlayer;

					REQUESTCLIENTPLAYER_MAP::iterator iTemp = iPlayer;
					iPlayer++;

					m_mapRequestClientPlayer.erase( iTemp );

					continue;
				}
			}
			
			iPlayer++;
		}

	} catch (Throwable&t) {
		DEBUG_ADD_ERR( t.toString().c_str() );
	}

	Unlock();
}

//--------------------------------------------------------------------------------
// RequestConnectionThreadProc
//--------------------------------------------------------------------------------
LONG					
RequestConnectionThreadProc(LPVOID lpParameter)
{
	CONNECTION_INFO* pInfo = (CONNECTION_INFO*)lpParameter;
	
	HANDLE hConnectionThread = GetCurrentThread();

	Socket * pSocket = NULL;

	//for (int i=0; i<5; i++)
	{
		try {
			pSocket = new Socket( pInfo->ip, pInfo->port );

			// try to connect to server
			pSocket->connect();

			// make nonblocking socket
			pSocket->setNonBlocking();

			// make no-linger socket
			pSocket->setLinger(0);

			// create player
			RequestClientPlayer* pPlayer = new RequestClientPlayer(pSocket);
			pSocket = NULL;

			pPlayer->setRequestServerName( pInfo->name.c_str() );
			pPlayer->setRequestServerIP( pInfo->ip.c_str() );
			pPlayer->setRequestMode( pInfo->requestMode );

			if (g_pRequestClientPlayerManager!=NULL)
			{
				SetThreadPriority(hConnectionThread, THREAD_PRIORITY_NORMAL);

				g_pRequestClientPlayerManager->AddRequestClientPlayer( pPlayer );

				SetThreadPriority(hConnectionThread, THREAD_PRIORITY_LOWEST);
			}
			else
			{
			}

		} catch (Throwable& t) {

			DEBUG_ADD_FORMAT_ERR("[RequestClientPlayerManager] Can't Connect %s:%s(%d)", pInfo->name.c_str(), pInfo->ip.c_str(), pInfo->port);
			DEBUG_ADD(t.toString().c_str());

			if (pSocket!=NULL)
			{
				delete pSocket;
			}

			//------------------------------------------------------
			// request mode에 따라서 접속이 안된 경우에 처리..
			//------------------------------------------------------
			switch (pInfo->requestMode)
			{
				//------------------------------------------------------
				// 귓속말 보낼려고 했는데 접속이 안 된 경우
				//------------------------------------------------------
				case REQUEST_CLIENT_MODE_WHISPER :

					SetThreadPriority(hConnectionThread, THREAD_PRIORITY_NORMAL);

					// 이렇게 하면..
					// 서버에 다시 한번 IP를 요청해서 귓속말을 보내든지 말든지.. 그케 된다.
//					UI_AddChatToHistory( (*g_pGameStringTable)[STRING_MESSAGE_WHISPER_FAILED].GetString(), NULL, 5 );
					g_pRequestUserManager->RemoveRequestUserLater( pInfo->name.c_str() );
//					g_pWhisperManager->RemoveWhisperMessage( pInfo->name.c_str() );
					g_pWhisperManager->TryToSendWhisperMessage( pInfo->name.c_str() );					

					SetThreadPriority(hConnectionThread, THREAD_PRIORITY_LOWEST);
				break;

				//------------------------------------------------------
				// profile을 요청할려고 했는데 접속이 안 된 경우
				//------------------------------------------------------
				case REQUEST_CLIENT_MODE_PROFILE :
					g_pProfileManager->RemoveRequire(pInfo->name.c_str());
				break;
			}

			// CONNECTION_INFO 제거
			g_pRequestClientPlayerManager->RemoveConnectionInfo(pInfo->name.c_str());
		}
	}

	//delete pInfo;

	return 0L;
}
