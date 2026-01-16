/*-----------------------------------------------------------------------------

	Timer2.cpp

	Timer version 2 implementation.

	2000.6.15. KJTINC

-----------------------------------------------------------------------------*/

#include "Timer2.h"
#include <cstring>
#include <cstdlib>

//----------------------------------------------------------------------------
// Global instance
//----------------------------------------------------------------------------
C_TIMER2		gC_timer2;

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------
C_TIMER2::C_TIMER2()
{
	m_timer_queue.size = 0;
	m_timer_queue.pS_timerunit = NULL;
	m_id_generator = 0;
}

//----------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------
C_TIMER2::~C_TIMER2()
{
	if (m_timer_queue.pS_timerunit != NULL)
	{
		free(m_timer_queue.pS_timerunit);
		m_timer_queue.pS_timerunit = NULL;
	}
	m_timer_queue.size = 0;
}

//----------------------------------------------------------------------------
// Add - Add a new timer
//----------------------------------------------------------------------------
timer_id_t
C_TIMER2::Add(DWORD dw_millisec, void (*fp_proc)(void))
{
	// Check if we need to expand the queue
	if (m_id_generator >= m_timer_queue.size)
	{
		// Expand by 8 timers at a time
		int new_size = m_timer_queue.size + 8;
		S_TIMERUNIT* new_queue = (S_TIMERUNIT*)realloc(m_timer_queue.pS_timerunit,
														new_size * sizeof(S_TIMERUNIT));
		if (new_queue == NULL)
		{
			return INVALID_TID;
		}

		// Initialize new entries
		for (int i = m_timer_queue.size; i < new_size; i++)
		{
			new_queue[i].tid = INVALID_TID;
			new_queue[i].fp_proc = NULL;
		}

		m_timer_queue.pS_timerunit = new_queue;
		m_timer_queue.size = new_size;
	}

	// Find a slot using the id generator
	timer_id_t tid = m_id_generator++;
	S_TIMERUNIT* pUnit = &m_timer_queue.pS_timerunit[tid];

	// Initialize the timer unit
	pUnit->tid = tid;
	pUnit->fp_proc = fp_proc;
	pUnit->dw_millisec = dw_millisec;
	pUnit->dw_prev_tickcount = GetTickCount();
	pUnit->bl_pause = 1; // Start paused

	return tid;
}

//----------------------------------------------------------------------------
// Delete - Delete a timer
//----------------------------------------------------------------------------
bool
C_TIMER2::Delete(timer_id_t &tid)
{
	if (tid < 0 || tid >= m_id_generator)
	{
		return false;
	}

	S_TIMERUNIT* pUnit = &m_timer_queue.pS_timerunit[tid];
	if (pUnit->tid == INVALID_TID)
	{
		return false;
	}

	// Mark as deleted
	pUnit->tid = INVALID_TID;
	pUnit->fp_proc = NULL;
	tid = INVALID_TID;

	return true;
}

//----------------------------------------------------------------------------
// Execute - Execute all active timers
//----------------------------------------------------------------------------
void
C_TIMER2::Execute()
{
	DWORD dw_current_tickcount = GetTickCount();

	for (int i = 0; i < m_id_generator; i++)
	{
		S_TIMERUNIT* pUnit = &m_timer_queue.pS_timerunit[i];

		// Skip invalid or paused timers
		if (pUnit->tid == INVALID_TID || pUnit->bl_pause)
		{
			continue;
		}

		// Check if timer has elapsed
		if (dw_current_tickcount - pUnit->dw_prev_tickcount >= pUnit->dw_millisec)
		{
			// Execute the timer callback
			if (pUnit->fp_proc != NULL)
			{
				Execute(pUnit);
			}

			// Update the previous tick count
			pUnit->dw_prev_tickcount = dw_current_tickcount;
		}
	}
}

//----------------------------------------------------------------------------
// Execute - Execute a single timer unit (private)
//----------------------------------------------------------------------------
void
C_TIMER2::Execute(S_TIMERUNIT *pS_timerunit)
{
	if (pS_timerunit != NULL && pS_timerunit->fp_proc != NULL)
	{
		pS_timerunit->fp_proc();
	}
}

//----------------------------------------------------------------------------
// Refresh - Reset a timer's tick count
//----------------------------------------------------------------------------
void
C_TIMER2::Refresh(timer_id_t tid)
{
	if (tid < 0 || tid >= m_id_generator)
	{
		return;
	}

	S_TIMERUNIT* pUnit = &m_timer_queue.pS_timerunit[tid];
	if (pUnit->tid != INVALID_TID)
	{
		pUnit->dw_prev_tickcount = GetTickCount();
	}
}

//----------------------------------------------------------------------------
// Pause - Pause a timer
//----------------------------------------------------------------------------
void
C_TIMER2::Pause(timer_id_t tid)
{
	if (tid < 0 || tid >= m_id_generator)
	{
		return;
	}

	S_TIMERUNIT* pUnit = &m_timer_queue.pS_timerunit[tid];
	if (pUnit->tid != INVALID_TID)
	{
		pUnit->bl_pause = 1;
	}
}

//----------------------------------------------------------------------------
// Continue - Resume a paused timer
//----------------------------------------------------------------------------
void
C_TIMER2::Continue(timer_id_t tid)
{
	if (tid < 0 || tid >= m_id_generator)
	{
		return;
	}

	S_TIMERUNIT* pUnit = &m_timer_queue.pS_timerunit[tid];
	if (pUnit->tid != INVALID_TID)
	{
		pUnit->bl_pause = 0;
		pUnit->dw_prev_tickcount = GetTickCount();
	}
}

//----------------------------------------------------------------------------
// ResetSpeed - Change a timer's interval
//----------------------------------------------------------------------------
void
C_TIMER2::ResetSpeed(timer_id_t tid, DWORD millisec)
{
	if (tid < 0 || tid >= m_id_generator)
	{
		return;
	}

	S_TIMERUNIT* pUnit = &m_timer_queue.pS_timerunit[tid];
	if (pUnit->tid != INVALID_TID)
	{
		pUnit->dw_millisec = millisec;
		pUnit->dw_prev_tickcount = GetTickCount();
	}
}
