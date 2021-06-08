// VS_UI_ui_result_receiver.cpp

#include "client_PCH.h"
#include <assert.h>
#include "VS_UI_ui_result_receiver.h"

/*-----------------------------------------------------------------------------
- C_VS_UI_UI_RESULT_RECEIVER
-
-----------------------------------------------------------------------------*/
C_VS_UI_UI_RESULT_RECEIVER::C_VS_UI_UI_RESULT_RECEIVER()
{
	m_fp_result_receiver = NULL;
}

/*-----------------------------------------------------------------------------
- ~C_VS_UI_UI_RESULT_RECEIVER
-
-----------------------------------------------------------------------------*/
C_VS_UI_UI_RESULT_RECEIVER::~C_VS_UI_UI_RESULT_RECEIVER()
{

}

/*-----------------------------------------------------------------------------
- SetResultReceiver
-
-----------------------------------------------------------------------------*/
void C_VS_UI_UI_RESULT_RECEIVER::SetResultReceiver(void (*fp)(DWORD, int, int, void *))
{
	assert(fp);
	
	m_fp_result_receiver = fp;
}

/*-----------------------------------------------------------------------------
- SendMessage
- Message queue에 message를 넣는다.
-----------------------------------------------------------------------------*/
void C_VS_UI_UI_RESULT_RECEIVER::_SendMessage(DWORD message, int left, int right, 
															 void *void_ptr)
{
	assert(message != INVALID_MESSAGE);

	MESSAGE * msg = new MESSAGE;

	msg->message = message;
	msg->left = left;
	msg->right = right;
	msg->void_ptr = void_ptr;

	m_message_queue.Add(msg);
}

/*-----------------------------------------------------------------------------
- DispatchMessage
- 가장 빨리 저장된 message를 한 개 보낸다. 그리고 그것을 queue에서 삭제한다.
-----------------------------------------------------------------------------*/
void C_VS_UI_UI_RESULT_RECEIVER::_DispatchMessage()
{
	if (m_fp_result_receiver != NULL)
	{
		if (m_message_queue.Size() > 0)
		{
			MESSAGE * data;
			if (m_message_queue.Data(0, data))
			{
				m_fp_result_receiver(data->message, data->left, data->right, data->void_ptr);
				delete data;
				m_message_queue.Delete(data);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// C_MESSAGE_QUEUE
//
// 
//-----------------------------------------------------------------------------
C_MESSAGE_QUEUE::C_MESSAGE_QUEUE()
{

}

//-----------------------------------------------------------------------------
// ~C_MESSAGE_QUEUE
//
// 
//-----------------------------------------------------------------------------
C_MESSAGE_QUEUE::~C_MESSAGE_QUEUE()
{
	MESSAGE * data;
	for (int i=0; i < Size(); i++)
		if (Data(i, data))
			delete data;
}