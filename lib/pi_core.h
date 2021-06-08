/*-----------------------------------------------------------------------------

	pi_core.h

	Program Interface core.

	2000.9.18. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __PI_CORE_H__
#define __PI_CORE_H__

#include "SimpleDataList.h"

//-----------------------------------------------------------------------------
// PI_Processor
//
// Object start/finish/process.
//
// <개요>
//
// 이것은 보이지 않게 실행되는 object이다. Start는 object의 시작을 알린다. 곧, 
// 이때부터 Process가 실행된다. 또한 object의 초기화를 수행하는 역할도 한다.
// Finish는 Process를 정지한다.
//-----------------------------------------------------------------------------
class PI_Processor
{
private:
	bool							m_bl_running;

public:
	//
	// constructor/destructor는 object의 탄생/죽음시 실행된다면, 
	// 이것들은 그것을 실제로 시작하고 끝내는 것이다. 이것이 object의 시작과 끝의
	// 유일한 interface이다.
	//
	// Finish()는 실제로 Process를 그만두게 할 뿐이다. override하여 사용할 때에는
	// 반드시 실행시켜줘야 한다.
	//
	// !Process를 시작해서 정지시키는 기능으로 Start(), Finish()는 매우 중요하다.
	//
	virtual void	Start() { m_bl_running = true; }
	virtual void	Finish() { m_bl_running = false; }

	//
	// BaseInterface를 상속받은 object는 instance가 생성될 때 그 pointer를 등록함으로써
	// background에서 묵시적으로 Process()가 자동으로 실행된다. Start()가 실행되고부터
	// 실행되며, Finish()가 실행되면 실행되지 않는다.
	//
	virtual void	Process() {}; // no pure

	bool	Running() const { return m_bl_running; }

	PI_Processor();
	~PI_Processor();
};

//-----------------------------------------------------------------------------
// PI_ProcessRunner
//
//
//-----------------------------------------------------------------------------
class PI_ProcessRunner : public SimpleDataList<PI_Processor *>
{
public:
	void	Process();
};

//-----------------------------------------------------------------------------
// PI_Visual
//
//
//-----------------------------------------------------------------------------
class PI_Visual : public PI_Processor
{
public:
	virtual void	Show() = 0;
};

// [note] PI_Control은 정의하지 않는다. 왜냐하면 이것은 상황에 따라 달라질 수 있는 요소가
//			 많기 때문이다.

extern PI_ProcessRunner *	gpC_process_runner;

#endif