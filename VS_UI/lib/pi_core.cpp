// pi_core.cpp

#include "client_PCH.h"

#include "pi_core.h"
#include <assert.h>

PI_ProcessRunner *	gpC_process_runner;

//-----------------------------------------------------------------------------
// PI_ProcessRunner::Process
//
// 
//-----------------------------------------------------------------------------
void PI_ProcessRunner::Process()
{
	PI_Processor * data;
	for (int i=0; i < Size(); i++)
		if (Data(i, data))
			if (data->Running())
				data->Process();
}

//-----------------------------------------------------------------------------
// PI_Processor
//
// 
//-----------------------------------------------------------------------------
PI_Processor::PI_Processor()
{ 
	m_bl_running = false;

	assert(gpC_process_runner != NULL);

	gpC_process_runner->Add(this);
}

//-----------------------------------------------------------------------------
// ~PI_Processor
//
// 
//-----------------------------------------------------------------------------
PI_Processor::~PI_Processor()
{
	assert(gpC_process_runner != NULL);

	gpC_process_runner->Delete(this);
}