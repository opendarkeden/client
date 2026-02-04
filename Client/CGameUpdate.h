//-----------------------------------------------------------------------------
// CGameUpdate.h
//-----------------------------------------------------------------------------
// Game Loop
//-----------------------------------------------------------------------------

#ifndef	__CGAMEUPDATE_H__
#define	__CGAMEUPDATE_H__

#include "WinLib/CWinUpdate.h"
#include "InputService.h"

class CGameUpdate : public CWinUpdate {
	public :
		CGameUpdate() {}
		~CGameUpdate() {}

		void		Init();

		void		Update();	

		void		UpdateDraw();

	protected :
		void		ProcessInput();
		void		UpdateDrawHelp();

		static void	DXMouseEvent(CSDLInput::E_MOUSE_EVENT event, int x, int y, int z);
		static void	DXKeyboardEvent(CSDLInput::E_KEYBOARD_EVENT, DWORD);
		static void	SDLTextInputEvent(const char* text, int* window_coords);
};

extern CGameUpdate*		g_pCGameUpdate;

#endif
