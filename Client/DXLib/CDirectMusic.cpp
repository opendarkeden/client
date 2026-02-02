//----------------------------------------------------------------------
// CDirectMusic.cpp
//
// SDL2 Implementation (Cross-platform)
// Windows DirectMusic implementation removed - using SDL2_mixer on all platforms
// NOTE: This class is deprecated. Use Mix_PlayMusic() from SDL_mixer instead.
//----------------------------------------------------------------------

#include "CDirectSound.h"
#include "CDirectMusic.h"

//-----------------------------------------------------------------------------
// Global instance
//-----------------------------------------------------------------------------
CDirectMusic g_DXMusic;

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------

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

	m_bInit = false;
	m_bLoad = false;
	m_bPlay = false;

	m_bSoftwareSynth = true;
}

CDirectMusic::~CDirectMusic()
{
	// Stub - SDL mixer is managed elsewhere
}

//-----------------------------------------------------------------------------
// Initialization
//-----------------------------------------------------------------------------

bool CDirectMusic::Init(HWND hWnd, DIRECTMUSIC_TYPE type)
{
	// Not implemented - use Mix_OpenAudio() from SDL_mixer instead
	(void)hWnd;
	(void)type;
	return false;
}

void CDirectMusic::Release()
{
	// Not implemented - use Mix_CloseAudio() from SDL_mixer instead
}

bool CDirectMusic::CreatePerformance()
{
	// Not implemented - SDL mixer doesn't have separate performance object
	return false;
}

bool CDirectMusic::CreatePort(DIRECTMUSIC_TYPE type)
{
	// Not implemented - SDL mixer uses system audio output
	(void)type;
	return false;
}

bool CDirectMusic::CreateLoader()
{
	// Not implemented - use Mix_LoadMUS() from SDL_mixer instead
	return false;
}

//-----------------------------------------------------------------------------
// Playback Control
//-----------------------------------------------------------------------------

bool CDirectMusic::Play(const char* filename, WORD repeat)
{
	// Not implemented - use Mix_PlayMusic() from SDL_mixer instead
	(void)filename;
	(void)repeat;
	return false;
}

void CDirectMusic::Pause()
{
	// Not implemented - use Mix_PauseMusic() from SDL_mixer instead
}

void CDirectMusic::Resume()
{
	// Not implemented - use Mix_ResumeMusic() from SDL_mixer instead
}

void CDirectMusic::Stop()
{
	// Not implemented - use Mix_HaltMusic() from SDL_mixer instead
}

//-----------------------------------------------------------------------------
// Tempo Control
//-----------------------------------------------------------------------------

void CDirectMusic::SetCurrentTempo(int tempo)
{
	// Not implemented - SDL_mixer doesn't support tempo changes
	(void)tempo;
}
