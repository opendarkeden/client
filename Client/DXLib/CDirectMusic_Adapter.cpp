/*-----------------------------------------------------------------------------

	CDirectMusic_Adapter.cpp

	DirectMusic adapter using DXLibBackend.
	This file provides SDL2 backend support for CDirectMusic class.

	2025.01.14

-----------------------------------------------------------------------------*/

#include "CDirectMusic.h"
#include "DXLibBackend.h"

/* Global instance */
CDirectMusic	g_DXMusic;

/*=============================================================================
 * SDL Backend Implementation
 *=============================================================================*/

#ifdef DXLIB_BACKEND_SDL

/* Constructor */
CDirectMusic::CDirectMusic()
{
	m_pDM = NULL;
	m_pDMPerformance = NULL;
	m_pDMPort = NULL;
	m_pDMLoader = NULL;
	m_pDMSegment = NULL;
	m_pDMSegmentState = NULL;

	m_mtStart = 0;
	m_mtOffset = 0;
	m_rtStart = 0;
	m_rtOffset = 0;

	m_bSoftwareSynth = true;

	m_bInit = false;
	m_bLoad = false;
	m_bPlay = false;

	m_OriginalTempo = 100;	// Default tempo (100%)
	m_CurrentTempo = 100;
}

/* Destructor */
CDirectMusic::~CDirectMusic()
{
	Release();
}

/* Initialize SDL backend */
bool CDirectMusic::Init(HWND hWnd, DIRECTMUSIC_TYPE type)
{
	if (dxlib_music_init(hWnd) != 0) {
		return false;
	}

	// Set software synth flag based on type
	m_bSoftwareSynth = (type == DIRECTMUSIC_TYPE_SW);

	// Mark as initialized
	m_pDM = (IDirectMusic*)0x01;
	m_pDMPerformance = (IDirectMusicPerformance*)0x01;
	m_pDMPort = (IDirectMusicPort*)0x01;
	m_pDMLoader = (IDirectMusicLoader*)0x01;

	m_bInit = true;
	m_bLoad = false;
	m_bPlay = false;

	return true;
}

/* Release SDL backend */
void CDirectMusic::Release()
{
	// Stop if playing
	if (m_bPlay) {
		Stop();
	}

	// Free loaded music
	if (m_bLoad) {
		dxlib_music_free();
	}

	// Release backend
	dxlib_music_release();

	m_pDM = NULL;
	m_pDMPerformance = NULL;
	m_pDMPort = NULL;
	m_pDMLoader = NULL;
	m_pDMSegment = NULL;
	m_pDMSegmentState = NULL;

	m_bInit = false;
	m_bLoad = false;
	m_bPlay = false;
}

/* Play music file */
bool CDirectMusic::Play(const char* filename, WORD repeat)
{
	if (!m_bInit) return false;

	// Load music file
	if (dxlib_music_load(filename) != 0) {
		return false;
	}

	m_bLoad = true;

	// Set repeat flag (0xFFFF means infinite loop)
	int loop = (repeat == 0xFFFF) ? 1 : 0;

	// Play music
	if (dxlib_music_play(loop) != 0) {
		return false;
	}

	m_bPlay = true;

	// Set current tempo
	dxlib_music_set_tempo(m_CurrentTempo / 100.0f);

	return true;
}

/* Stop music */
void CDirectMusic::Stop()
{
	if (!m_bInit) return;

	dxlib_music_stop();

	// Free music
	dxlib_music_free();

	m_bLoad = false;
	m_bPlay = false;
}

/* Pause music */
void CDirectMusic::Pause()
{
	if (!m_bInit || !m_bPlay) return;

	dxlib_music_pause();
	m_bPlay = false;
}

/* Resume music */
void CDirectMusic::Resume()
{
	if (!m_bInit || m_bPlay) return;

	dxlib_music_resume();
	m_bPlay = true;
}

/* Set current tempo */
void CDirectMusic::SetCurrentTempo(int t)
{
	if (!m_bInit) return;

	m_CurrentTempo = t;

	// Clamp tempo to reasonable range
	if (m_CurrentTempo < 50) m_CurrentTempo = 50;	// Half speed
	if (m_CurrentTempo > 200) m_CurrentTempo = 200;	// Double speed

	// Apply tempo
	// Note: SDL_mixer does not support tempo change, this is a no-op
	dxlib_music_set_tempo(m_CurrentTempo / 100.0f);
}

/* Create performance (stub for SDL backend) */
bool CDirectMusic::CreatePerformance()
{
	// Not applicable for SDL backend
	return true;
}

/* Create port (stub for SDL backend) */
bool CDirectMusic::CreatePort(DIRECTMUSIC_TYPE type)
{
	// Not applicable for SDL backend
	return true;
}

/* Create loader (stub for SDL backend) */
bool CDirectMusic::CreateLoader()
{
	// Not applicable for SDL backend
	return true;
}

#endif /* DXLIB_BACKEND_SDL */
