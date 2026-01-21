//--------------------------------------------------------------------------------
// RequestServerPlayerManager.cpp
//--------------------------------------------------------------------------------
#include "Client_PCH.h"
#include "RequestServerPlayerManager.h"
//#include "RequestClientPlayerManager.h"
#include "ClientConfig.h"
#include "ClientDef.h"
#include "DebugInfo.h"
#include "ServerInfo.h"

// Platform-specific threading includes
#ifdef PLATFORM_WINDOWS
	#include <windows.h>
	#include <process.h>
#elif defined(__APPLE__) || defined(__linux__)
	#include <pthread.h>
	#include <unistd.h>

	// Additional Windows type definitions
	typedef DWORD* LPDWORD;
	typedef void* (*LPTHREAD_START_ROUTINE)(void*);
	typedef void* LPVOID;

	// Undefine macros that conflict with our stub functions
	#undef CloseHandle

	// Stub constants
	#define STILL_ACTIVE ((DWORD)-1)
	#define THREAD_PRIORITY_NORMAL 0
	#define THREAD_PRIORITY_LOWEST -2

	// Stub functions
	static inline BOOL TerminateThread(HANDLE thread, DWORD exitCode) {
		return (pthread_cancel((pthread_t)(size_t)thread) == 0);
	}

	static inline BOOL CloseHandle(HANDLE handle) {
		return TRUE;
	}

	static inline HANDLE GetCurrentThread() {
		return (HANDLE)pthread_self();
	}

	static inline BOOL SetThreadPriority(HANDLE thread, int priority) {
		return TRUE;
	}

	// Stub thread creation
	static inline HANDLE _beginthreadex(void* security, unsigned stack_size,
		LPTHREAD_START_ROUTINE start_proc, LPVOID arg,
		unsigned flags, DWORD* thread_id) {
		pthread_t thread;
		if (pthread_create(&thread, NULL, (void*(*)(void*))start_proc, arg) == 0) {
			if (thread_id) *thread_id = (unsigned long)thread;
			return (HANDLE)(size_t)thread;
		}
		return (HANDLE)0;
	}

	// Stub CreateThread (Windows API)
	static inline HANDLE CreateThread(void* security, unsigned stack_size,
		LPTHREAD_START_ROUTINE start_proc, LPVOID arg,
		unsigned flags, DWORD* thread_id) {
		pthread_t thread;
		if (pthread_create(&thread, NULL, (void*(*)(void*))start_proc, arg) == 0) {
			if (thread_id) *thread_id = (unsigned long)thread;
			return (HANDLE)(size_t)thread;
		}
		return (HANDLE)0;
	}
#endif

//#include "Rpackets/RCPositionInfo.h"

#if defined(_DEBUG) && defined(OUTPUT_DEBUG)
	extern CMessageArray*		g_pGameMessage;
#endif

#ifdef OUTPUT_DEBUG
	extern CRITICAL_SECTION			g_Lock;
#endif


//--------------------------------------------------------------------------------
// Global
//--------------------------------------------------------------------------------
RequestServerPlayerManager*	g_pRequestServerPlayerManager = NULL;

LONG					WaitRequestThreadProc(LPVOID lpParameter);

//--------------------------------------------------------------------------------
//
// constructor / destructor
//
//--------------------------------------------------------------------------------
RequestServerPlayerManager::RequestServerPlayerManager()
{
	m_pServerSocket		= NULL;
	m_hRequestThread	= NULL;
	m_bThreadRunning	= false;

	InitializeCriticalSection(&m_Lock);
}

RequestServerPlayerManager::~RequestServerPlayerManager()
{
	Release();

	DeleteCriticalSection(&m_Lock);
}

//--------------------------------------------------------------------------------
// Release
//--------------------------------------------------------------------------------
void
RequestServerPlayerManager::Release()
{
	// Signal thread to stop gracefully
	m_bThreadRunning = false;

	// Wait for thread to exit (with timeout)
	if (m_hRequestThread != NULL)
	{
#ifdef PLATFORM_WINDOWS
		// Windows: Wait up to 2 seconds for thread to exit
		DWORD waitResult = WaitForSingleObject(m_hRequestThread, 2000);
		if (waitResult == WAIT_TIMEOUT)
		{
			// Thread didn't exit gracefully, force terminate
			TerminateThread(m_hRequestThread, 0);
		}
		CloseHandle(m_hRequestThread);
#else
		// macOS/Linux: pthread_join with timeout simulation
		// Note: We don't force terminate on POSIX as it can corrupt heap
		// Just close the handle and let the thread exit on its own
		CloseHandle(m_hRequestThread);
#endif
		m_hRequestThread = NULL;
	}

	try {
		if (m_pServerSocket!=NULL)
		{
			delete m_pServerSocket;
			m_pServerSocket = NULL;
		}
	} catch (Throwable&) {
	}

	Lock();

	RequestServerPlayer_LIST::iterator iPlayer = m_listRequestServerPlayer.begin();

	while (iPlayer != m_listRequestServerPlayer.end())
	{
		RequestServerPlayer* pPlayer = *iPlayer;

		try {
			delete pPlayer;
		} catch (Throwable&) {
		}

		iPlayer++;
	}

	m_listRequestServerPlayer.clear();

	Unlock();
}

//--------------------------------------------------------------------------------
// Add RequestServerPlayer
//--------------------------------------------------------------------------------
bool
RequestServerPlayerManager::AddRequestServerPlayer(RequestServerPlayer* pRequestServerPlayer)
{
	if (1)//g_Mode==MODE_GAME)
	{
		Lock();

		// 넘 많을 경우는 더 이상 요청을 안 받도록 해야한다.
		if (m_listRequestServerPlayer.size() < g_pClientConfig->MAX_REQUEST_SERVICE)
		{
			// 일단 list에 넣어둔다.
			m_listRequestServerPlayer.push_back( pRequestServerPlayer );

			#if defined(_DEBUG) && defined(OUTPUT_DEBUG)
			
				EnterCriticalSection(&g_Lock);
	
				if (g_pGameMessage!=NULL)
				{
					Socket* pSocket = pRequestServerPlayer->getSocket();

					if (pSocket!=NULL)
					{
						const char* addrIP = pSocket->getHost().c_str();
						g_pGameMessage->AddFormat("New Connection From %s", addrIP);
					}
					else
					{
						g_pGameMessage->AddFormat("New Connection From where?");
					}
				}

				LeaveCriticalSection(&g_Lock);
			#endif

			Unlock();
			return true;
		}
		else
		{
			pRequestServerPlayer->disconnect( UNDISCONNECTED );
			delete pRequestServerPlayer;
		}
		
		Unlock();		
	}
	/*
	else
	{
		pRequestServerPlayer->disconnect( UNDISCONNECTED );
		delete pRequestServerPlayer;
	}
	*/

	return false;	
}

//--------------------------------------------------------------------------------
// Disconnect
//--------------------------------------------------------------------------------
void		
RequestServerPlayerManager::Disconnect(const char* pName)
{
	Lock();


	RequestServerPlayer_LIST::iterator iPlayer = m_listRequestServerPlayer.begin();
		
	while (iPlayer != m_listRequestServerPlayer.end())
	{
		RequestServerPlayer* pPlayer = *iPlayer;

		// 같은 이름의 player의 접속을 해제시킨다.
		if (pPlayer->getName()==pName)
		{
			pPlayer->disconnect(UNDISCONNECTED);
			delete pPlayer;

			m_listRequestServerPlayer.erase( iPlayer );

			Unlock();
			return;
		}
		
		iPlayer++;
	}


	Unlock();
}

//--------------------------------------------------------------------------------
// Broadcast
//--------------------------------------------------------------------------------
void		
RequestServerPlayerManager::Broadcast(Packet* pPacket)
{
	Lock();

	// 나에게 접속한 모든 player들에게 packet을 전송한다.
	RequestServerPlayer_LIST::iterator iPlayer = m_listRequestServerPlayer.begin();
		
	while (iPlayer != m_listRequestServerPlayer.end())
	{
		RequestServerPlayer* pPlayer = *iPlayer;

		pPlayer->sendPacket( pPacket );
		
		iPlayer++;
	}

	Unlock();
}


//--------------------------------------------------------------------------------
// Update
//--------------------------------------------------------------------------------
void		
RequestServerPlayerManager::ProcessMode(RequestServerPlayer* pPlayer)
{
	/*
	switch (pPlayer->getRequestMode())
	{		
		//------------------------------------------------------
		// 지속적으로 좌표를 보내는 경우
		//------------------------------------------------------		
		case REQUEST_CLIENT_MODE_POSITION_REPEATLY :
		{
			static DWORD nextTime = g_CurrentTime;
			static DWORD sendTime = g_CurrentTime;

			static int oldX = -1;
			static int oldY = -1;
			static int oldZoneID = -1;

			if (g_CurrentTime >= nextTime)
			{
				int x		= g_pPlayer->GetX();
				int y		= g_pPlayer->GetY();
				int zoneID	= (g_bZonePlayerInLarge?g_nZoneLarge : g_nZoneSmall);

				// 좌표가 달라졌으면 보낸다.
				if (oldX!=x || oldY!=y || oldZoneID!=zoneID
					|| g_CurrentTime >= sendTime)
				{
					RCPositionInfo _RCPositionInfo;

					_RCPositionInfo.setZoneX( x );
					_RCPositionInfo.setZoneY( y );
					_RCPositionInfo.setZoneID( zoneID );

					pPlayer->sendPacket( &_RCPositionInfo );

					// 보낸 정보 기억
					oldX = x;
					oldY = y;
					oldZoneID = zoneID;

					sendTime = g_CurrentTime + 30000;	// 30*1000;  // 30초
				}

				// 1초에 한번 갱신
				nextTime = g_CurrentTime + 1000;				
			}
		}
		break;
	}
	*/
}


//--------------------------------------------------------------------------------
// Update
//--------------------------------------------------------------------------------
void
RequestServerPlayerManager::Update()
{
	Lock();

	if (m_listRequestServerPlayer.empty())
	{
		Unlock();
		return;
	}

	try {

		RequestServerPlayer_LIST::iterator iPlayer = m_listRequestServerPlayer.begin();

		while (iPlayer != m_listRequestServerPlayer.end())
		{
			RequestServerPlayer* pPlayer = *iPlayer;

			try {

				if (!pPlayer->getSocket()->isValid())
				{
					throw SocketException("sock error");
				}

				ProcessMode( pPlayer );

				pPlayer->processInput();
				pPlayer->processCommand();
				pPlayer->processOutput();

			} catch (NonBlockingIOException& t) {

				// 무시..
				DEBUG_ADD_ERR( t.toString().c_str() );

			} catch (Throwable &t) 	{

				DEBUG_ADD_ERR( t.toString().c_str() );

				// 내가 요청하고 있는것도 짜른다.
				//if (g_pRequestClientPlayerManager!=NULL)
				{
				//	g_pRequestClientPlayerManager->Disconnect( pPlayer->getName().c_str() );
				}

				// exception이 나면 무조건 잘라버린다. --;
				pPlayer->disconnect(UNDISCONNECTED);
				delete pPlayer;

				RequestServerPlayer_LIST::iterator iTemp = iPlayer;
				iPlayer++;

				m_listRequestServerPlayer.erase( iTemp );

				continue;
			}

			iPlayer++;
		}

	} catch (Throwable&t) {
		DEBUG_ADD_ERR( t.toString().c_str() );
	}

	Unlock();
}

//--------------------------------------------------------------------------------
// Init
//--------------------------------------------------------------------------------
void
RequestServerPlayerManager::Init(int port)
{
	if (m_pServerSocket!=NULL)
	{
		delete m_pServerSocket;
	}

	m_pServerSocket = new ServerSocket( port );

	// Set running flag before creating thread
	m_bThreadRunning = true;

	DWORD dwChildThreadID;	// 의미 없당 -- ;

	m_hRequestThread = CreateThread(NULL,
								0,	// default stack size
								(LPTHREAD_START_ROUTINE)WaitRequestThreadProc,
								this,
								NULL,
								&dwChildThreadID);

	// priority는 낮게
	SetThreadPriority(m_hRequestThread, THREAD_PRIORITY_LOWEST);
}

//--------------------------------------------------------------------------------
// Wait Request
//--------------------------------------------------------------------------------
void
RequestServerPlayerManager::WaitRequest()
{
	// Check if socket is still valid (may be NULL during shutdown)
	if (m_pServerSocket == NULL || !m_bThreadRunning)
	{
		return;
	}

	Socket* pSocket = m_pServerSocket->accept();

	// accept() returns NULL if no connection or socket closed
	if (pSocket == NULL)
	{
		return;
	}

	// request에 등록
	RequestServerPlayer* pRequestServerPlayer = new RequestServerPlayer( pSocket );

	pRequestServerPlayer->setPlayerStatus( CPS_REQUEST_SERVER_BEGIN_SESSION );

	pSocket->setNonBlocking();

	SetThreadPriority(m_hRequestThread, THREAD_PRIORITY_NORMAL);

	if (AddRequestServerPlayer( pRequestServerPlayer ))
	{
		// g_pDebugMessage에 lock걸어야 한다. - -;
		//DEBUG_ADD_FORMAT("[Request] New Connection from %s:%d", pSocket->getHost().c_str(), pSocket->getPort());
	}

	SetThreadPriority(m_hRequestThread, THREAD_PRIORITY_LOWEST);
}

//--------------------------------------------------------------------------------
// WaitRequest
//--------------------------------------------------------------------------------
LONG
WaitRequestThreadProc(LPVOID lpParameter)
{
	RequestServerPlayerManager* pRequestServerPlayerManager = (RequestServerPlayerManager*)lpParameter;

	while (pRequestServerPlayerManager != NULL && pRequestServerPlayerManager->IsThreadRunning())
	{
		pRequestServerPlayerManager->WaitRequest();
	}

	return 0L;
}
