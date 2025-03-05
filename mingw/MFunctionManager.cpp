//-----------------------------------------------------------------------------
// MFunctionManager.cpp
//-----------------------------------------------------------------------------
#include "Client_PCH.h"
#include "MFunctionManager.h"

//-----------------------------------------------------------------------------
//
// constructor / destructor
//
//-----------------------------------------------------------------------------
MFunctionManager::MFunctionManager()
{
	m_nFunction = NULL;
	m_pFunction = NULL;
}

MFunctionManager::~MFunctionManager()
{
	Release();
}

//-----------------------------------------------------------------------------
//
// member functions
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Init
//-----------------------------------------------------------------------------
void
MFunctionManager::Init(int max)
{
	Release();

	if (max > 0)
	{
		m_nFunction = max;
		m_pFunction = new FUNCTION_TYPE [ max ];

		for (int i=0; i<max; i++)
		{
			m_pFunction[i] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// Release
//-----------------------------------------------------------------------------
void
MFunctionManager::Release()
{
	if (m_pFunction!=NULL)
	{
		delete [] m_pFunction;
		m_pFunction = NULL;
	}

	m_nFunction = 0;
}


//-----------------------------------------------------------------------------
// Execute
//-----------------------------------------------------------------------------
// message와 연결되어 있는 적절한 처리 함수를 호출한다.
//-----------------------------------------------------------------------------
bool			
MFunctionManager::Execute(int id, void* pVoid) const
{
	if (id >=0 && id < m_nFunction)
	{
		if (m_pFunction[id]==NULL)
		{
			//DEBUG_ADD_FORMAT("[Error] NO UI_MessageFunction. msg=%d", message);
			return false;
		}
		else
		{
			(*m_pFunction[id])(pVoid);
			return true;

			//DEBUG_ADD("[UI] Execute OK");
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
// Set Function
//-----------------------------------------------------------------------------
void	
MFunctionManager::SetFunction(int id, FUNCTION_TYPE function)
{
	if (id >=0 && id < m_nFunction)
	{
		m_pFunction[id] = function;
	}
}