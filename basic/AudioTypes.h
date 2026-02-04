/*-----------------------------------------------------------------------------

	AudioTypes.h

	Platform-independent audio type definitions and constants.
	Replaces DirectX DSBVOLUME_*, DSBCAPS_* constants.

	2025.02.04 - Created to eliminate DirectX audio dependencies

-----------------------------------------------------------------------------*/

#ifndef __AUDIOTYPES_H__
#define __AUDIOTYPES_H__

/* Include Platform.h for basic type definitions */
#include "Platform.h"

/*-----------------------------------------------------------------------------
	Volume Constants

	DirectX used logarithmic volume scale from -10000 to 0.
	SDL_mixer uses linear scale from 0 to MIX_MAX_VOLUME (128).

	For compatibility, we maintain the DirectX constants but map them
	to SDL volume values in the implementation.
-----------------------------------------------------------------------------*/

/* Volume range (DirectX-compatible values) */
#ifndef DSBVOLUME_MIN
#define DSBVOLUME_MIN            -10000    /* Silence */
#endif

#ifndef DSBVOLUME_MAX
#define DSBVOLUME_MAX            0         /* Full volume */
#endif

/* New platform-independent names */
#define AUDIO_VOLUME_MIN          DSBVOLUME_MIN
#define AUDIO_VOLUME_MAX          DSBVOLUME_MAX

/*-----------------------------------------------------------------------------
	Buffer Capabilities

	These flags control sound buffer properties.
	In SDL, these are handled differently but we maintain the
	interface for compatibility with existing code.
-----------------------------------------------------------------------------*/

#ifndef DSBCAPS_CTRLFREQUENCY
#define DSBCAPS_CTRLFREQUENCY     0x00000001  /* Frequency control */
#endif

#ifndef DSBCAPS_CTRLPAN
#define DSBCAPS_CTRLPAN           0x00000002  /* Pan control */
#endif

#ifndef DSBCAPS_CTRLVOLUME
#define DSBCAPS_CTRLVOLUME        0x00000004  /* Volume control */
#endif

#ifndef DSBCAPS_GLOBALFOCUS
#define DSBCAPS_GLOBALFOCUS       0x00000080  /* Global focus */
#endif

/* New platform-independent names */
#define AUDIO_BUFFER_CTRLFREQUENCY    DSBCAPS_CTRLFREQUENCY
#define AUDIO_BUFFER_CTRLPAN          DSBCAPS_CTRLPAN
#define AUDIO_BUFFER_CTRLVOLUME       DSBCAPS_CTRLVOLUME
#define AUDIO_BUFFER_GLOBALFOCUS      DSBCAPS_GLOBALFOCUS

/*-----------------------------------------------------------------------------
	Multimedia Structures

	Platform-independent definitions for multimedia structures.
	Replaces DirectX multimedia types.
-----------------------------------------------------------------------------*/

/* Forward declarations */
typedef DWORD FOURCC;
typedef void* HMMIO;

/* Multimedia RIFF chunk information */
typedef struct _MMCKINFO {
    FOURCC  ckid;
    FOURCC  fccType;
    DWORD   dwDataOffset;
    DWORD   dwSize;
} MMCKINFO;

/* DirectSound buffer position notify structure */
typedef struct _DSBPOSITIONNOTIFY {
    DWORD   dwFlags;
    DWORD   dwOffset;
    DWORD   dwCallback;
} DSBPOSITIONNOTIFY;

/* Multimedia constants */
#ifndef MMIO_READ
#define MMIO_READ      0
#endif

#ifndef MMIO_ALLOCBUF
#define MMIO_ALLOCBUF  0x10000
#endif

#ifndef FOURCC
#define FOURCC(a,b,c,d) (((DWORD)(a)<<0)|((DWORD)(b)<<8)|((DWORD)(c)<<16)|((DWORD)(d)<<24))
#endif

#endif // __AUDIOTYPES_H__
