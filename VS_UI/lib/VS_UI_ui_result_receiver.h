#ifndef __VS_UI_UI_RESULT_RECEIVER_H__
#define __VS_UI_UI_RESULT_RECEIVER_H__

#include "typedef.h"
#include "SimpleDataList.h"
#include "VS_UI_UIMessage.h"


struct MESSAGE
{
	DWORD				message;
	int				left;
	int				right;
	void *			void_ptr;
};

//-----------------------------------------------------------------------------
// C_MESSAGE_QUEUE 
//
// 
//-----------------------------------------------------------------------------
class C_MESSAGE_QUEUE : public SimpleDataList<MESSAGE *>
{
public:
	C_MESSAGE_QUEUE();
	~C_MESSAGE_QUEUE();
};

/*-----------------------------------------------------------------------------
  Class VS UI - UI Result Receiver

  `어떤 상태에서 message를 곧바로 보내지 않는다. 왜냐하면 그 process가 종료되기
   전에 message를 받아 처리하면 때로는 그 process를 kill할 수 있기 때문이다.
	따라서 message는 message queue에 저장한다.
-----------------------------------------------------------------------------*/
class C_VS_UI_UI_RESULT_RECEIVER
{
private:
	C_MESSAGE_QUEUE			m_message_queue;

	void (*m_fp_result_receiver)(DWORD, int, int, void *);

public:
	C_VS_UI_UI_RESULT_RECEIVER();
	~C_VS_UI_UI_RESULT_RECEIVER();

	void _SendMessage(DWORD message, int left = 0, int right = 0, void *void_ptr = NULL);
	void	_DispatchMessage();

/*-----------------------------------------------------------------------------
  Set.
-----------------------------------------------------------------------------*/
	void SetResultReceiver(void (*fp)(DWORD, int, int, void *));

#ifndef _LIB
	int	GetMessageSize() const { return m_message_queue.Size(); }
#endif
};

#endif