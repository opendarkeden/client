/*-----------------------------------------------------------------------------

	Timer2.h

	Timer version 2.

	2000.6.15. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __TIMER2__
#define __TIMER2__

#include "BasicS.h"

typedef long timer_id_t;

#define INVALID_TID						-1
#define INVALID_INDEX					-1

//----------------------------------------------------------------------------
// Class Timer - Timer Manager.
//----------------------------------------------------------------------------
class C_TIMER2
{
private:
	//
	// 타이머 개체 정보 구조체
	//
	struct S_TIMERUNIT
	{
		DWORD				bl_pause; // being paused ?
		DWORD				dw_prev_tickcount;
		DWORD				dw_millisec;

		//
		// tid
		//
		// unique한 id로써 하나의 timer와 대응한다.
		//
		timer_id_t		tid;
		void				(*fp_proc)(void);	// method to execute
	};

	//
	// Timer queue
	//
	struct S_TIMERQUEUE
	{
		int				size;
		S_TIMERUNIT	*	pS_timerunit;
	};

	S_TIMERQUEUE		m_timer_queue;

	//
	// m_id_generator
	//
	// unique한 id를 생성한다.
	// 이 것은 계속 증가는 것으로 tid가 delete되어도 변하지 않는다.
	//
	timer_id_t			m_id_generator;

	void	Execute(S_TIMERUNIT *pS_timerunit);

public:
	C_TIMER2();
	~C_TIMER2();

	void	Execute();
	void	Refresh(timer_id_t tid);
	void	Pause(timer_id_t tid);
	void	Continue(timer_id_t tid);
	bool	Delete(timer_id_t &tid);
	void	ResetSpeed(timer_id_t tid, DWORD millisec);

	timer_id_t Add(DWORD dw_millisec, void (*fp_proc)(void));
};

extern C_TIMER2		gC_timer2;

#endif