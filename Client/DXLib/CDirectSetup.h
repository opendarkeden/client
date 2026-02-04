//-----------------------------------------------------------------------------
// CDirectSetup.h
//-----------------------------------------------------------------------------

#ifndef	__CDIRECTSETUP_H__
#define	__CDIRECTSETUP_H__

/* Platform-independent includes (SDL2 backend on all platforms) */
#include "../../basic/Platform.h"

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------

#define DSETUP_REINSTALL	0x00000080

#define SHOW_ALL		1
#define SHOW_UPGRADES	2
#define SHOW_PROBLEMS	3
#define SHOW_NONE		4

//-----------------------------------------------------------------------------
// SDL2 Version (replaces DirectX Version)
//-----------------------------------------------------------------------------

// Using SDL2 on all platforms - no DirectX version needed
#define	SDL2_MAJOR_VERSION	2
#define	SDL2_MINOR_VERSION	0
#define	SDL2_PATCHLEVEL		0


class CDirectSetup {
	public :
		CDirectSetup();
		~CDirectSetup();

		// Get SDL2 version (replaces DirectX version detection)
		static void			GetVersion( DWORD* pdwMajor, DWORD* pdwMinor, DWORD* pdwPatch );

	protected :
};


#endif
