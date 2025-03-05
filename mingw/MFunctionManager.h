//-----------------------------------------------------------------------------
// MFunctionManager.h
//-----------------------------------------------------------------------------

#ifndef __MFUNCTIONMANAGER_H__
#define __MFUNCTIONMANAGER_H__

class MFunctionManager {
	public :
		typedef	void (*FUNCTION_TYPE)(void*);

	public :
		MFunctionManager();
		virtual ~MFunctionManager();

		//-----------------------------------------------------------
		// 초기화 - 필수~~
		//-----------------------------------------------------------
		virtual void	Init(int max);
		virtual void	Release();

		//-----------------------------------------------------------
		// Function 실행
		//-----------------------------------------------------------
		virtual bool	Execute(int id, void* pVoid=NULL) const;

	protected :
		virtual void	SetFunction(int id, FUNCTION_TYPE function);

	private :
		int					m_nFunction;
		FUNCTION_TYPE*		m_pFunction;
};

#endif

