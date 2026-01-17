//-----------------------------------------------------------------------------
// CSoundPartManager.h
//-----------------------------------------------------------------------------

#ifndef	__CSOUNDPARTMANAGER_H__
#define __CSOUNDPARTMANAGER_H__

#include "CPartManager.h"

#ifdef PLATFORM_WINDOWS
#include <DSound.h>
#else
#include "../../basic/Platform.h"

// Forward declaration for DirectSound buffer
typedef struct IDirectSoundBuffer* LPDIRECTSOUNDBUFFER;

#endif

class CSoundPartManager : public CPartManager<WORD, BYTE, LPDIRECTSOUNDBUFFER> {
	public :
		CSoundPartManager()		{}
		~CSoundPartManager()	{ Release(); }

		void			Release();

		void			Stop();

	protected :

};

#endif
