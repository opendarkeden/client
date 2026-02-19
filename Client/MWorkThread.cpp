//----------------------------------------------------------------------
// MWorkThread.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MWorkThread.h"

// Platform-specific wrappers for Windows API compatibility
static inline BOOL SetEvent(HANDLE event) {
    return platform_event_set((platform_event_t)event) == 0 ? TRUE : FALSE;
}

static inline BOOL ResetEvent(HANDLE event) {
    return platform_event_reset((platform_event_t)event) == 0 ? TRUE : FALSE;
}

#ifdef PLATFORM_WINDOWS
#include <process.h>
#endif

// Cross-platform implementation

//----------------------------------------------------------------------
//
// constructor / destructor
//
//----------------------------------------------------------------------
MWorkThread::MWorkThread()
{
	m_hWorkThread = 0;

	m_hHasWorkEvent = 0;
	m_hEndWorkEvent = 0;
	m_hStopWorkEvent = 0;
	//m_hDequeLock = 0;
	//m_hCurrentLock = 0;

	m_pCurrentWork = NULL;
}

MWorkThread::~MWorkThread()
{
	Release();
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Init
//----------------------------------------------------------------------
void
MWorkThread::Init(LPTHREAD_START_ROUTINE FileThreadProc, int priority)
{
	if (m_hWorkThread!=0)
	{
		Release();
	}

	//-----------------------------------------------------------
	// Create Thread
	//-----------------------------------------------------------
	#ifdef PLATFORM_WINDOWS
	DWORD	dwChildThreadID;

	m_hWorkThread = CreateThread(NULL, 
								0,	// default stack size
								(LPTHREAD_START_ROUTINE)FileThreadProc,
								NULL,
								NULL,
								&dwChildThreadID);

	SetThreadPriority(m_hWorkThread, 
					priority
					);
#else
	// Non-Windows: Use platform_thread_create (pthread)
	// Note: priority not implemented in platform_thread_create
	m_hWorkThread = (HANDLE)platform_thread_create(
		(platform_thread_func_t)FileThreadProc,
		NULL
	);
	(void)priority; // priority parameter not used on non-Windows
#endif
	//-----------------------------------------------------------
	// critical section object
	//-----------------------------------------------------------
	InitializeCriticalSection(&m_csDeque);
	InitializeCriticalSection(&m_csCurrent);	

 
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void
MWorkThread::Release()
{
	ReleaseWork();

	//-------------------------------------------------------------
	// 작업 중지 event가 끝날때까지 기다림
	//-------------------------------------------------------------
	while (IsStopWork());

//--------------------------------------------
	// event cleanup
	//--------------------------------------------
	platform_event_close((platform_event_t)m_hHasWorkEvent);
	platform_event_close((platform_event_t)m_hEndWorkEvent);
	platform_event_close((platform_event_t)m_hStopWorkEvent);
	//CloseHandle(m_hDequeLock);
	//CloseHandle(m_hCurrentLock);
	DeleteCriticalSection(&m_csDeque);
	DeleteCriticalSection(&m_csCurrent);

	//--------------------------------------------
	// thread 중지
	//--------------------------------------------
#ifdef PLATFORM_WINDOWS
	TerminateThread(m_hWorkThread, 0);
	CloseHandle( m_hWorkThread );
#else
	// Non-Windows: Wait for thread to finish (pthread)
	platform_thread_wait((platform_thread_t)m_hWorkThread);
	platform_thread_close((platform_thread_t)m_hWorkThread);
#endif

	m_hHasWorkEvent = 0;	
	m_hEndWorkEvent = 0;	
	m_hStopWorkEvent = 0;	
	//m_hDequeLock = 0;
	//m_hCurrentLock = 0;
}

//----------------------------------------------------------------------
// ReleaseWork
//----------------------------------------------------------------------
void
MWorkThread::ReleaseWork()
{
	//-------------------------------------------------------------
	// 작업 중지 event
	//-------------------------------------------------------------
	SetEvent( m_hStopWorkEvent );

	//------------------------------------------------------------
	// 현재 작업중인 일이 있을때..
	//------------------------------------------------------------
	if (!IsFinishCurrentWork())
	{
		//WaitUnlockCurrent();
		LockCurrent();

		if (m_pCurrentWork!=NULL)		// 현재 일이 설정된 경우
		{
			// 중단			
			m_pCurrentWork->Stop();
		}

		UnlockCurrent();
	}

	//------------------------------------------------------------
	// 일이 끝날때까지 기다린다.
	//------------------------------------------------------------
	while (IsWorking());

	//-------------------------------------------------------------
	// deque의 WorkNode들을 모두 지운다.
	//-------------------------------------------------------------
	//WaitUnlockDeque();
	LockDeque();

	WORKNODE_DEQUE::iterator iNode = m_dequeWorkNode.begin();

	while (iNode != m_dequeWorkNode.end())
	{
		MWorkNode* pNode = *iNode;

		delete pNode;

		iNode ++;
	}

	m_dequeWorkNode.clear();

	UnlockDeque();

	//-------------------------------------------------------------
	// 작업 중지 event제거
	//-------------------------------------------------------------
	ResetEvent( m_hStopWorkEvent );
}


//----------------------------------------------------------------------
// Execute
//----------------------------------------------------------------------
void				
MWorkThread::Execute()
{
	MWorkNode* pToRemove = NULL;

	while (1)
	{
		//------------------------------------------------------
		// 할 일이 있을 때까지 기다린다.
		//------------------------------------------------------
		WaitForSingleObject(m_hHasWorkEvent, INFINITE);

		//------------------------------------------------------
		// deque의 일들을 모두 처리한다.
		//------------------------------------------------------
		//WaitUnlockDeque();
		LockDeque();

		int queueSize = m_dequeWorkNode.size();

		UnlockDeque();

		while (queueSize!=0)
		{
			//------------------------------------------------------
			// 일 시작한다고 표시
			//------------------------------------------------------
			ResetEvent( m_hEndWorkEvent );

			//------------------------------------------------------
			// 현재 작업하는 WorkNode로 설정
			//------------------------------------------------------
			//WaitUnlockDeque();
			LockDeque();

			//WaitUnlockCurrent();
			LockCurrent();

			m_pCurrentWork = m_dequeWorkNode.front();

			UnlockCurrent();

			// 현재 WorkNode를 지운다.
			m_dequeWorkNode.pop_front();

			UnlockDeque();

			//------------------------------------------------------
			// 실행
			//------------------------------------------------------
			// execute 도중에 중단될 수도 있다. (Stop()에 의해서)
			MWorkNode* pRemainNode = NULL;
			
			if (m_pCurrentWork->Execute(pRemainNode))
			{
				//------------------------------------------------
				// 정상적으로 모든 작업이 끝난 경우
				//------------------------------------------------				
			}
			else
			{
				//------------------------------------------------
				// 비정상적으로 작업이 중단된 경우
				//------------------------------------------------
				// 에러가 생겼거나


				//------------------------------------------------
				// Stop된 경우일 수 있다. - 남은 일이 있는 경우
				//------------------------------------------------
				if (pRemainNode!=NULL)
				{
					//------------------------------------------------
					// 중단된 경우				
					// 남은 일을 다시 node에 추가한다.
					//------------------------------------------------
					//WaitUnlockDeque();
					LockDeque();

					m_dequeWorkNode.push_front( pRemainNode );			

					UnlockDeque();
				}
			}

			//------------------------------------------------------
			// 작업 중인 일이 끝났다고 표시
			//------------------------------------------------------
			SetEvent( m_hEndWorkEvent );

			//------------------------------------------------------
			// 실행이 끝났으므로 제거한다.
			//------------------------------------------------------
			pToRemove = m_pCurrentWork;
		
			//WaitUnlockCurrent();
			LockCurrent();

			m_pCurrentWork = NULL;			

			UnlockCurrent();
			
			//-------------------------------------------------------------
			// 작업 중지 event 체크
			//-------------------------------------------------------------
			if (IsStopWork())
			{
				break;
			}

			delete pToRemove;
			pToRemove = NULL;

			//-------------------------------------------------------------
			// 남아있는거 체크
			//-------------------------------------------------------------
			//WaitUnlockDeque();
			LockDeque();

			queueSize = m_dequeWorkNode.size();

			UnlockDeque();
		}
		
		//------------------------------------------------------
		// 일을 끝냈다고 표시..
		//------------------------------------------------------
		ResetEvent( m_hHasWorkEvent );

		if (pToRemove!=NULL)
		{
			delete pToRemove;
			pToRemove = NULL;
		}
	}
}

//----------------------------------------------------------------------
// Remove
//----------------------------------------------------------------------
// type과 관련이 있는 작업들을 중단한다.
//----------------------------------------------------------------------
void				
MWorkThread::Remove(int type)
{
	//-------------------------------------------------------------
	// 작업 중지 event
	//-------------------------------------------------------------
	SetEvent( m_hStopWorkEvent );

	//------------------------------------------------------------
	// 현재 작업중인 일 체크
	//------------------------------------------------------------
	if (!IsFinishCurrentWork())
	{
		//WaitUnlockCurrent();
		LockCurrent();

		if (m_pCurrentWork!=NULL)
		{
			if (m_pCurrentWork->IsTypeOf(type))
			{
				// 중지
				m_pCurrentWork->Stop();
			}
		}

		UnlockCurrent();
	}

	//------------------------------------------------------------
	// 일이 끝날때까지 기다린다.
	//------------------------------------------------------------
	while (IsWorking());

	//-------------------------------------------------------------
	// type과 관련이 있는 deque의 WorkNode들을 모두 지운다.
	//-------------------------------------------------------------
	//WaitUnlockDeque();
	LockDeque();

	WORKNODE_DEQUE::iterator iNode = m_dequeWorkNode.begin();
	WORKNODE_DEQUE::iterator iNodeTemp;

	while (iNode != m_dequeWorkNode.end())
	{
		MWorkNode* pNode = *iNode;

		//------------------------------------------
		// 같은 type이면 지운다.
		//------------------------------------------
		if (pNode->IsTypeOf(type))
		{
			delete pNode;		

			iNodeTemp = iNode;

			iNode ++;

			m_dequeWorkNode.erase( iNodeTemp );
		}
		else
		{
			iNode ++;
		}
	}

	UnlockDeque();

	//-------------------------------------------------------------
	// 다시 일 시작..
	//-------------------------------------------------------------
	SetEvent( m_hHasWorkEvent );
}

//----------------------------------------------------------------------
// Add First
//----------------------------------------------------------------------
// 가장 우선 순위가 높은 work로서 추가시킨다.
// 외부에서 new해야 한다.
//
// 현재 진행 중인 작업을 중단시키고 그 때 남은 일을 다시 list에 추가하고
// pNode를 우선적으로 실행한다.
//----------------------------------------------------------------------
void				
MWorkThread::AddFirst(MWorkNode* pNode)
{
	if (pNode==NULL)
	{
		return;
	}

	//-------------------------------------------------------------
	// 작업 중지 event
	//-------------------------------------------------------------
	SetEvent( m_hStopWorkEvent );

	//------------------------------------------------------------
	// 현재 작업중인 일이 있을때..
	//------------------------------------------------------------
	if (!IsFinishCurrentWork())
	{
		//WaitUnlockCurrent();
		LockCurrent();

		if (m_pCurrentWork!=NULL)		// 현재 일이 설정된 경우
		{
			// 중단			
			m_pCurrentWork->Stop();
		}

		UnlockCurrent();
	}

	//------------------------------------------------------------
	// 일이 끝날때까지 기다린다.
	//------------------------------------------------------------
	while (IsWorking());

	//------------------------------------------------------------
	// deque의 처음에 추가한다.
	//------------------------------------------------------------
	//WaitUnlockDeque();
	LockDeque();

	m_dequeWorkNode.push_front( pNode );

	UnlockDeque();

	//------------------------------------------------------------
	// 일이 있다고 thread에 알린다.
	//------------------------------------------------------------
	SetEvent( m_hHasWorkEvent );
}

//----------------------------------------------------------------------
// Add Last
//----------------------------------------------------------------------
// 끝에 추가..
// 외부에서 new해야 한다.
//----------------------------------------------------------------------
void				
MWorkThread::AddLast(MWorkNode* pNode)
{
	if (pNode==NULL)
	{
		return;
	}

	//------------------------------------------------------------
	// 일이 끝나고.. size도 0인 경우는 기다린다.
	//------------------------------------------------------------
	//WaitUnlockDeque();
	LockDeque();

	if (IsFinishCurrentWork() && m_dequeWorkNode.size()==0)
	{
		while (IsWorking());
	}

	//------------------------------------------------------------
	// deque의 끝에 추가한다.
	//------------------------------------------------------------
	m_dequeWorkNode.push_back( pNode );

	UnlockDeque();

	
	//------------------------------------------------------------
	// 일이 있다고 thread에 알린다.
	//------------------------------------------------------------
	SetEvent( m_hHasWorkEvent );
}

#else
// Non-Windows platforms (macOS/Linux) - Stub implementations

#include "MWorkThread.h"

MWorkThread::MWorkThread()
{
	m_hWorkThread = 0;
	m_hHasWorkEvent = 0;
	m_hEndWorkEvent = 0;
	m_hStopWorkEvent = 0;
	m_pCurrentWork = NULL;
}

MWorkThread::~MWorkThread()
{
	Release();
}

void MWorkThread::Init(LPTHREAD_START_ROUTINE FileThreadProc, int priority)
{
	(void)FileThreadProc;
	(void)priority;
	// Not implemented on non-Windows
}

void MWorkThread::Release()
{
	ReleaseWork();
}

void MWorkThread::ReleaseWork()
{
	// Stub implementation
}

void MWorkThread::Execute()
{
	// Stub implementation - not functional on non-Windows
}

void MWorkThread::Remove(int type)
{
	(void)type;
}

void MWorkThread::AddFirst(MWorkNode* pNode)
{
	(void)pNode;
}

void MWorkThread::AddLast(MWorkNode* pNode)
{
	(void)pNode;
}

/* End of MWorkThread.cpp */
