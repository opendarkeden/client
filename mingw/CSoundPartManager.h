//-----------------------------------------------------------------------------
// CSoundPartManager.h
//-----------------------------------------------------------------------------

#ifndef	__CSOUNDPARTMANAGER_H__
#define __CSOUNDPARTMANAGER_H__


#include "CPartManager.h"
#include <DSound.h>

class CSoundPartManager : public CPartManager<WORD, BYTE, LPDIRECTSOUNDBUFFER> {
	public :
		CSoundPartManager()		{}
		~CSoundPartManager()	{ Release(); }

		void			Release();

		void			Stop();

	protected :

};

#endif
