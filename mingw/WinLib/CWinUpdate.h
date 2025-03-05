//-----------------------------------------------------------------------------
// CWinUpdate.h
//-----------------------------------------------------------------------------
// 시간을 비교하면서 일정시간마다 한번씩 Update가 되도록 한다.
//-----------------------------------------------------------------------------

#ifndef	__CWINUPDATE_H__
#define	__CWINUPDATE_H__

#include <Windows.h>
#include <MMSystem.h>

class CWinUpdate {
	public :
		CWinUpdate();
		virtual ~CWinUpdate();

		//-------------------------------------------------------
		// Init
		//-------------------------------------------------------
		virtual void	Init() {}

		//-------------------------------------------------------
		// Update
		//-------------------------------------------------------
		virtual void	Update();

		//-------------------------------------------------------
		// Current Time
		//-------------------------------------------------------
		static void		SetCurrentTime()	{ m_CurrentTime	= timeGetTime(); }
		static DWORD	GetCurrentTime()	{ return m_CurrentTime; }

		//-------------------------------------------------------
		// Update Delay
		//-------------------------------------------------------
		void			SetDelay(DWORD delay)	{ m_UpdateDelay = delay; }
		DWORD			GetDelay() const		{ return m_UpdateDelay; }

		//-------------------------------------------------------
		// Last Time
		//-------------------------------------------------------
		DWORD			GetLastTime()		{ return m_LastTime; }
		

	private :
		static DWORD	m_CurrentTime;		// 현재 time

	protected :
		DWORD			m_LastTime;			// 가장 최근에 Update()한 시간
		DWORD			m_UpdateDelay;		// update delay시간
};

#endif		
