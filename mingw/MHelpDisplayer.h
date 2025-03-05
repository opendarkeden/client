//---------------------------------------------------------------------------
// MHelpDisplayer.h
//---------------------------------------------------------------------------
// 도움말 출력 담당..
//---------------------------------------------------------------------------

#ifndef __MHELPDISPLAYER_H__
#define __MHELPDISPLAYER_H__

#include "MHelpDef.h"

class MHelpDisplayer {
	public :
		MHelpDisplayer();
		~MHelpDisplayer();

		//-----------------------------------------------------
		// Output Help
		//-----------------------------------------------------
		void	OutputHelp(HELP_OUTPUT ho);
		
	protected :
		DWORD	m_DelayTime;		// 최근에 출력한 시간
};

extern MHelpDisplayer*		g_pHelpDisplayer;

#endif

