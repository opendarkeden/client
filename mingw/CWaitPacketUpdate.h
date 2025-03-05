//-----------------------------------------------------------------------------
// CWaitPacketUpdate.h
//-----------------------------------------------------------------------------
// Server에서 메시지가 오기를 기다리는 Loop
//-----------------------------------------------------------------------------

#ifndef	__CWAITPACKETUPDATE_H__
#define	__CWAITPACKETUPDATE_H__

#include "CWinUpdate.h"

class CWaitPacketUpdate : public CWinUpdate {
	public :
		CWaitPacketUpdate()	{ m_DelayLimit = 0xFFFFFFFF; }
		~CWaitPacketUpdate() {}

		// 초기화
		void		Init();

		// Update하는 한계 시간을 설정한다.
		void		SetDelay(DWORD delay);		

		// update
		void		Update();		

	protected :				
		DWORD		m_DelayLimit;	// 기다리는 한계 시간
};


extern CWaitPacketUpdate*	g_pCWaitPacketUpdate;

#endif
