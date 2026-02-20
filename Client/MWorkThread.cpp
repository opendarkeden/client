//----------------------------------------------------------------------
// MWorkThread.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MWorkThread.h"

#ifdef PLATFORM_WINDOWS
// Windows implementation (original code)

//----------------------------------------------------------------------
// Platform-specific wrappers for Windows API compatibility
static inline BOOL SetEvent(HANDLE event) {
    return platform_event_signal((platform_event_t)event) == 0 ? TRUE : FALSE;
}

static inline BOOL ResetEvent(HANDLE event) {
    return platform_event_reset((platform_event_t)event) == 0 ? TRUE : FALSE;
}

#include <process.h>

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
	m_pCurrentWork = NULL;
}

MWorkThread::~MWorkThread()
{
	Release();
}

//----------------------------------------------------------------------
// Init
//----------------------------------------------------------------------
void MWorkThread::Init(LPTHREAD_START_ROUTINE FileThreadProc, int priority)
{
	//--------------------------------------------------------------------
	// Event 생성
	//--------------------------------------------------------------------
	m_hHasWorkEvent = CreateEvent( NULL, FALSE, FALSE, NULL);
	m_hEndWorkEvent = CreateEvent( NULL, FALSE, FALSE, NULL);
	m_hStopWorkEvent = CreateEvent( NULL, FALSE, FALSE, NULL);

	//--------------------------------------------------------------------
	// WorkThread 생성
	//--------------------------------------------------------------------
	m_hWorkThread = (HANDLE)platform_thread_create(
		FileThreadProc, 
		this,
		priority
	);

	if(m_hWorkThread==0)
	{
		Release();
		return;
	}
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void MWorkThread::Release()
{
	if(m_hWorkThread)
	{
		// 작업을 중지시킨다.
		SetEvent( m_hStopWorkEvent );

		// Thread가 종료되기를 기다린다.
		platform_thread_wait( m_hWorkThread );
		platform_thread_close( m_hWorkThread );
		m_hWorkThread = 0;
	}

	if(m_hHasWorkEvent)
	{
		platform_event_close( m_hHasWorkEvent );
		m_hHasWorkEvent = 0;
	}

	if(m_hEndWorkEvent)
	{
		platform_event_close( m_hEndWorkEvent );
		m_hEndWorkEvent = 0;
	}

	if(m_hStopWorkEvent)
	{
		platform_event_close( m_hStopWorkEvent );
		m_hStopWorkEvent = 0;
	}

	//--------------------------------------------------------------------
	// List
	//--------------------------------------------------------------------
	ReleaseWork();
}

//----------------------------------------------------------------------
// ReleaseWork
//----------------------------------------------------------------------
void MWorkThread::ReleaseWork()
{
	//--------------------------------------------------------------------
	// WorkNode삭제
	//--------------------------------------------------------------------
	MWorkNode* pNode;
	while((pNode = GetFirstWorkNode())!=NULL)
	{
		Remove( pNode );
	}

	m_pCurrentWork = NULL;
}

//----------------------------------------------------------------------
// Execute
//----------------------------------------------------------------------
void MWorkThread::Execute()
{
	while(1)
	{
		//--------------------------------------------------------------------
		// Work를 기다린다.
		//--------------------------------------------------------------------
		WaitForSingleObject(m_hHasWorkEvent, PLATFORM_INFINITE);

		//--------------------------------------------------------------------
		// StopWork이 요구되었는가?
		//--------------------------------------------------------------------
		if(WaitForSingleObject(m_hStopWorkEvent, 0)==WAIT_OBJECT_0)
		{
			break;
		}

		//--------------------------------------------------------------------
		// 처리할 Work가 있다.
		//--------------------------------------------------------------------
		if(m_pCurrentWork)
		{
			//--------------------------------------------------------------------
			// 현재 작업 처리
			//--------------------------------------------------------------------
			m_pCurrentWork->Execute();

			//--------------------------------------------------------------------
			// 현재 작업은 끝났음을 알린다.
			//--------------------------------------------------------------------
			SetEvent( m_hEndWorkEvent );
		}
	}
}

//----------------------------------------------------------------------
// Remove
//----------------------------------------------------------------------
void MWorkThread::Remove(int type)
{
	//--------------------------------------------------------------------
	// 지정된 타입과 같은 Work를 찾아서 삭제한다.
	//--------------------------------------------------------------------
	MWorkNode* pNode;
	MWorkNode* pPrevNode = NULL;

	for(pNode=m_pFirstWorkNode; pNode; pNode=pNode->m_pNext)
	{
		if(pNode->GetType()==type)
		{
			//--------------------------------------------------------------------
			// 이것이 현재 처리중인 Work라면?
			//--------------------------------------------------------------------
			if(pNode==m_pCurrentWork)
			{
				// 작업 종료를 기다린다.
				WaitForSingleObject( m_hEndWorkEvent, PLATFORM_INFINITE );
			}

			//--------------------------------------------------------------------
			// 삭제
			//--------------------------------------------------------------------
			Remove( pNode, pPrevNode );
			pPrevNode = NULL; // 포인터가 변경되었으므로
		}
		else
		{
			pPrevNode = pNode;
		}
	}
}

//----------------------------------------------------------------------
// AddFirst
//----------------------------------------------------------------------
void MWorkThread::AddFirst(MWorkNode* pNode)
{
	//--------------------------------------------------------------------
	// 작업이 없다면?
	//--------------------------------------------------------------------
	if(m_pFirstWorkNode==NULL)
	{
		m_pFirstWorkNode = pNode;
		m_pLastWorkNode = pNode;
	}
	else
	{
		pNode->m_pNext = m_pFirstWorkNode;
		m_pFirstWorkNode->m_pPrev = pNode;
		m_pFirstWorkNode = pNode;
	}

	pNode->m_pWorkThread = this;
}

//----------------------------------------------------------------------
// AddLast
//----------------------------------------------------------------------
void MWorkThread::AddLast(MWorkNode* pNode)
{
	//--------------------------------------------------------------------
	// 작업이 없다면?
	//--------------------------------------------------------------------
	if(m_pFirstWorkNode==NULL)
	{
		m_pFirstWorkNode = pNode;
		m_pLastWorkNode = pNode;
	}
	else
	{
		pNode->m_pPrev = m_pLastWorkNode;
		m_pLastWorkNode->m_pNext = pNode;
		m_pLastWorkNode = pNode;
	}

	pNode->m_pWorkThread = this;
}

//----------------------------------------------------------------------
// Remove
//----------------------------------------------------------------------
void MWorkThread::Remove(MWorkNode* pNode, MWorkNode* pPrevNode)
{
	//--------------------------------------------------------------------
	// 이전 Node
	//--------------------------------------------------------------------
	if(pPrevNode)
	{
		pPrevNode->m_pNext = pNode->m_pNext;
	}
	else
	{
		m_pFirstWorkNode = pNode->m_pNext;
	}

	//--------------------------------------------------------------------
	// 다음 Node
	//--------------------------------------------------------------------
	if(pNode->m_pNext)
	{
		pNode->m_pNext->m_pPrev = pPrevNode;
	}
	else
	{
		m_pLastWorkNode = pPrevNode;
	}

	//--------------------------------------------------------------------
	// 삭제
	//--------------------------------------------------------------------
	delete pNode;
}

//----------------------------------------------------------------------
// GetFirstWorkNode
//----------------------------------------------------------------------
MWorkNode* MWorkThread::GetFirstWorkNode()
{
	//--------------------------------------------------------------------
	// 첫번째 Work를 얻는다.
	//--------------------------------------------------------------------
	MWorkNode* pNode = m_pFirstWorkNode;
	if(pNode==NULL)
	{
		return NULL;
	}

	//--------------------------------------------------------------------
	// List에서 제거
	//--------------------------------------------------------------------
	m_pFirstWorkNode = pNode->m_pNext;
	if(m_pFirstWorkNode)
	{
		m_pFirstWorkNode->m_pPrev = NULL;
	}
	else
	{
		m_pLastWorkNode = NULL;
	}

	//--------------------------------------------------------------------
	// 현재 작업으로 설정
	//--------------------------------------------------------------------
	m_pCurrentWork = pNode;

	//--------------------------------------------------------------------
	// 이전/다음을 없앤다.
	//--------------------------------------------------------------------
	pNode->m_pPrev = NULL;
	pNode->m_pNext = NULL;

	return pNode;
}

//----------------------------------------------------------------------
// GetFirstWorkNode (with type)
//----------------------------------------------------------------------
MWorkNode* MWorkThread::GetFirstWorkNode(int type)
{
	//--------------------------------------------------------------------
	// 지정된 타입의 첫번째 Work를 얻는다.
	//--------------------------------------------------------------------
	MWorkNode* pNode = m_pFirstWorkNode;
	MWorkNode* pPrevNode = NULL;

	while(pNode)
	{
		if(pNode->GetType()==type)
		{
			//--------------------------------------------------------------------
			// 이전 Node
			//--------------------------------------------------------------------
			if(pPrevNode)
			{
				pPrevNode->m_pNext = pNode->m_pNext;
			}
			else
			{
				m_pFirstWorkNode = pNode->m_pNext;
			}

			//--------------------------------------------------------------------
			// 다음 Node
			//--------------------------------------------------------------------
			if(pNode->m_pNext)
			{
				pNode->m_pNext->m_pPrev = pPrevNode;
			}
			else
			{
				m_pLastWorkNode = pPrevNode;
			}

			//--------------------------------------------------------------------
			// 현재 작업으로 설정
			//--------------------------------------------------------------------
			m_pCurrentWork = pNode;

			//--------------------------------------------------------------------
			// 이전/다음을 없앤다.
			//--------------------------------------------------------------------
			pNode->m_pPrev = NULL;
			pNode->m_pNext = NULL;

			return pNode;
		}

		pPrevNode = pNode;
		pNode = pNode->m_pNext;
	}

	return NULL;
}

//----------------------------------------------------------------------
// ExecuteWorkNode
//----------------------------------------------------------------------
void MWorkThread::ExecuteWorkNode()
{
	//--------------------------------------------------------------------
	// 작업이 있는가?
	//--------------------------------------------------------------------
	m_pCurrentWork = GetFirstWorkNode();
	if(m_pCurrentWork==NULL)
	{
		return;
	}

	//--------------------------------------------------------------------
	// 일이 있다고 thread에 알린다.
	//--------------------------------------------------------------------
	SetEvent( m_hHasWorkEvent );
}

#else
// Non-Windows platforms (macOS/Linux) - Stub implementations

#include "MWorkThread.h"

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
	m_pCurrentWork = NULL;
}

MWorkThread::~MWorkThread()
{
	// Stub implementation
}

//----------------------------------------------------------------------
// Init
//----------------------------------------------------------------------
void MWorkThread::Init(LPTHREAD_START_ROUTINE FileThreadProc, int priority)
{
	// Stub implementation
	(void)FileThreadProc;
	(void)priority;
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void MWorkThread::Release()
{
	// Stub implementation
}

//----------------------------------------------------------------------
// ReleaseWork
//----------------------------------------------------------------------
void MWorkThread::ReleaseWork()
{
	// Stub implementation
}

//----------------------------------------------------------------------
// Execute
//----------------------------------------------------------------------
void MWorkThread::Execute()
{
	// Stub implementation - not functional on non-Windows
}

//----------------------------------------------------------------------
// Remove
//----------------------------------------------------------------------
void MWorkThread::Remove(int type)
{
	(void)type;
}

//----------------------------------------------------------------------
// AddFirst
//----------------------------------------------------------------------
void MWorkThread::AddFirst(MWorkNode* pNode)
{
	(void)pNode;
}

//----------------------------------------------------------------------
// AddLast
//----------------------------------------------------------------------
void MWorkThread::AddLast(MWorkNode* pNode)
{
	(void)pNode;
}

#endif /* PLATFORM_WINDOWS */