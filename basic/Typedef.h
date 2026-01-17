/*-----------------------------------------------------------------------------

	Typedef.h

	General type definition.
	Now includes platform abstraction layer for cross-platform support.

	Original: 19971226. KJTINC
	Updated: 2025.01.14 - Added Platform.h include

-----------------------------------------------------------------------------*/

#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

/* Include platform abstraction first */
#include "Platform.h"

/* Platform.h already defines these, but we keep them for compatibility */
/*
#define NULL						      0
#define NOT_SELECTED						-1
typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned int	UINT;
typedef unsigned long   DWORD;
typedef DWORD				id_t;
typedef WORD char_t;
*/

#endif
