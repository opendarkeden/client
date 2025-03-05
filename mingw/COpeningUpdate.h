//-----------------------------------------------------------------------------
// COpeningUpdate.h
//-----------------------------------------------------------------------------
// Opening에서 메시지가 오기를 기다리는 Loop
//-----------------------------------------------------------------------------

#ifndef	__COPENINGUPDATE_H__
#define	__COPENINGUPDATE_H__

#include "CWinUpdate.h"

class COpeningUpdate : public CWinUpdate {
	public :
		COpeningUpdate() {}
		~COpeningUpdate() {}

		void		Init();

		void		PlayMPG(char* filename);				
		void		Update();		

	protected :
		void		ProcessInput();		
};

extern COpeningUpdate*	g_pCOpeningUpdate;

#endif
