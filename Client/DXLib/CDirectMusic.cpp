//----------------------------------------------------------------------
// CSDLMusic.cpp
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
CSDLMusic g_SDLMusic;

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------

CSDLMusic::CSDLMusic()
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

CSDLMusic::~CSDLMusic()
{
	// Stub - SDL mixer is managed elsewhere
}

//-----------------------------------------------------------------------------
// Initialization
//-----------------------------------------------------------------------------

bool CSDLMusic::Init(HWND hWnd, DIRECTMUSIC_TYPE type)
{
	// Not implemented - use Mix_OpenAudio() from SDL_mixer instead
	(void)hWnd;
	(void)type;
	return false;
}

void CSDLMusic::Release()
{
	// Not implemented - use Mix_CloseAudio() from SDL_mixer instead
}

bool CSDLMusic::CreatePerformance()
{
	// Not implemented - SDL mixer doesn't have separate performance object
	return false;
}

bool CSDLMusic::CreatePort(DIRECTMUSIC_TYPE type)
{
	// Not implemented - SDL mixer uses system audio output
	(void)type;
	return false;
}

bool CSDLMusic::CreateLoader()
{
	// Not implemented - use Mix_LoadMUS() from SDL_mixer instead
	return false;
}

//-----------------------------------------------------------------------------
// Playback Control
//-----------------------------------------------------------------------------

bool CSDLMusic::Play(const char* filename, WORD repeat)
{
	// Not implemented - use Mix_PlayMusic() from SDL_mixer instead
	(void)filename;
	(void)repeat;
	return false;
}

void CSDLMusic::Pause()
{
	// Not implemented - use Mix_PauseMusic() from SDL_mixer instead
}

void CSDLMusic::Resume()
{
	// Not implemented - use Mix_ResumeMusic() from SDL_mixer instead
}

void CSDLMusic::Stop()
{
	// Not implemented - use Mix_HaltMusic() from SDL_mixer instead
}

//-----------------------------------------------------------------------------
// Tempo Control
//-----------------------------------------------------------------------------

void CSDLMusic::SetCurrentTempo(int tempo)
{
	// Not implemented - SDL_mixer doesn't support tempo changes
	(void)tempo;
}
