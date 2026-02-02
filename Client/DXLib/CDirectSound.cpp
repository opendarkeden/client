//----------------------------------------------------------------------
// CDirectSound.cpp
//
// SDL2 Implementation (Cross-platform)
// Windows DirectSound implementation removed - using SDL2_mixer on all platforms
// NOTE: This class is deprecated. Use SDL_mixer functions directly instead.
//----------------------------------------------------------------------

#include "CDirectSound.h"

//-----------------------------------------------------------------------------
// Global instance
//-----------------------------------------------------------------------------
CDirectSound g_DXSound;

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------

CDirectSound::CDirectSound()
{
	m_pDS = NULL;
	m_bInit = false;
	m_bMute = false;
	m_MaxVolume = 0;
}

CDirectSound::~CDirectSound()
{
	// Stub - SDL mixer is managed elsewhere
}

//-----------------------------------------------------------------------------
// Initialization
//-----------------------------------------------------------------------------

bool CDirectSound::Init(HWND hwnd)
{
	// Not implemented - use Mix_OpenAudio() from SDL_mixer instead
	(void)hwnd;
	return false;
}

void CDirectSound::Release()
{
	// Not implemented - use Mix_CloseAudio() from SDL_mixer instead
}

bool CDirectSound::IsInit() const
{
	return m_bInit;
}

//-----------------------------------------------------------------------------
// Mute Control
//-----------------------------------------------------------------------------

bool CDirectSound::IsMute() const
{
	return m_bMute;
}

void CDirectSound::SetMute()
{
	m_bMute = true;
}

void CDirectSound::UnSetMute()
{
	m_bMute = false;
}

//-----------------------------------------------------------------------------
// Volume Control
//-----------------------------------------------------------------------------

LONG CDirectSound::GetVolumeLimit() const
{
	return m_MaxVolume;
}

void CDirectSound::SetVolumeLimit(LONG volume)
{
	m_MaxVolume = volume;
}

bool CDirectSound::SetMaxVolume(LPDIRECTSOUNDBUFFER buffer)
{
	// Not implemented - use Mix_Volume() from SDL_mixer instead
	(void)buffer;
	return false;
}

bool CDirectSound::AddVolume(LPDIRECTSOUNDBUFFER buffer, int step)
{
	// Not implemented - use Mix_Volume() from SDL_mixer instead
	(void)buffer;
	(void)step;
	return false;
}

bool CDirectSound::SubVolume(LPDIRECTSOUNDBUFFER buffer, int step)
{
	// Not implemented - use Mix_Volume() from SDL_mixer instead
	(void)buffer;
	(void)step;
	return false;
}

bool CDirectSound::SubVolumeFromMax(LPDIRECTSOUNDBUFFER buffer, int step)
{
	// Not implemented - use Mix_Volume() from SDL_mixer instead
	(void)buffer;
	(void)step;
	return false;
}

//-----------------------------------------------------------------------------
// Frequency Control
//-----------------------------------------------------------------------------

bool CDirectSound::AddFrequency(LPDIRECTSOUNDBUFFER buffer, int step)
{
	// Not implemented - SDL_mixer doesn't support real-time frequency changes
	(void)buffer;
	(void)step;
	return false;
}

bool CDirectSound::SubFrequency(LPDIRECTSOUNDBUFFER buffer, int step)
{
	// Not implemented - SDL_mixer doesn't support real-time frequency changes
	(void)buffer;
	(void)step;
	return false;
}

//-----------------------------------------------------------------------------
// Pan Control
//-----------------------------------------------------------------------------

bool CDirectSound::RightPan(LPDIRECTSOUNDBUFFER buffer, int step)
{
	// Not implemented - SDL_mixer doesn't support real-time panning
	(void)buffer;
	(void)step;
	return false;
}

bool CDirectSound::LeftPan(LPDIRECTSOUNDBUFFER buffer, int step)
{
	// Not implemented - SDL_mixer doesn't support real-time panning
	(void)buffer;
	(void)step;
	return false;
}

bool CDirectSound::CenterToRightPan(LPDIRECTSOUNDBUFFER buffer, int step)
{
	// Not implemented - SDL_mixer doesn't support real-time panning
	(void)buffer;
	(void)step;
	return false;
}

bool CDirectSound::CenterToLeftPan(LPDIRECTSOUNDBUFFER buffer, int step)
{
	// Not implemented - SDL_mixer doesn't support real-time panning
	(void)buffer;
	(void)step;
	return false;
}

bool CDirectSound::CenterPan(LPDIRECTSOUNDBUFFER buffer)
{
	// Not implemented - SDL_mixer doesn't support real-time panning
	(void)buffer;
	return false;
}

bool CDirectSound::ChangePan(LPDIRECTSOUNDBUFFER buffer, int pan)
{
	// Not implemented - SDL_mixer doesn't support real-time panning
	(void)buffer;
	(void)pan;
	return false;
}

//-----------------------------------------------------------------------------
// Sound Buffer Operations
//-----------------------------------------------------------------------------

LPDIRECTSOUNDBUFFER CDirectSound::LoadWav(LPSTR filename)
{
	// Not implemented - use Mix_LoadWAV() from SDL_mixer instead
	(void)filename;
	return NULL;
}

LPDIRECTSOUNDBUFFER CDirectSound::CreateBuffer(LPVOID sdat, DWORD size, DWORD caps, LPWAVEFORMATEX wfx)
{
	// Not implemented - use SDL_mixer sound functions instead
	(void)sdat;
	(void)size;
	(void)caps;
	(void)wfx;
	return NULL;
}

void CDirectSound::Release(LPDIRECTSOUNDBUFFER buffer)
{
	// Not implemented - use Mix_FreeChunk() from SDL_mixer instead
	(void)buffer;
}

LPDIRECTSOUNDBUFFER CDirectSound::DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER buffer, bool bAutoRelease)
{
	// Not implemented - SDL_mixer doesn't need buffer duplication
	(void)buffer;
	(void)bAutoRelease;
	return NULL;
}

void CDirectSound::ReleaseDuplicateBuffer()
{
	// Not implemented - not needed with SDL_mixer
}

void CDirectSound::ReleaseTerminatedDuplicateBuffer()
{
	// Not implemented - not needed with SDL_mixer
}

//-----------------------------------------------------------------------------
// Playback Control
//-----------------------------------------------------------------------------

bool CDirectSound::IsPlay(LPDIRECTSOUNDBUFFER buffer) const
{
	// Not implemented - use Mix_Playing() from SDL_mixer instead
	(void)buffer;
	return false;
}

bool CDirectSound::NewPlay(LPDIRECTSOUNDBUFFER buffer, bool loop)
{
	// Not implemented - use Mix_PlayChannel() from SDL_mixer instead
	(void)buffer;
	(void)loop;
	return false;
}

bool CDirectSound::Play(LPDIRECTSOUNDBUFFER buffer, bool loop, bool duplicate)
{
	// Not implemented - use Mix_PlayChannel() from SDL_mixer instead
	(void)buffer;
	(void)loop;
	(void)duplicate;
	return false;
}

bool CDirectSound::Stop(LPDIRECTSOUNDBUFFER buffer)
{
	// Not implemented - use Mix_HaltChannel() from SDL_mixer instead
	(void)buffer;
	return false;
}

//-----------------------------------------------------------------------------
// DirectSound Access
//-----------------------------------------------------------------------------

LPDIRECTSOUND CDirectSound::GetDS() const
{
	return m_pDS;
}

//-----------------------------------------------------------------------------
// Error Handling
//-----------------------------------------------------------------------------

bool CDirectSound::DirectSoundFailed(const char* str)
{
	// Not implemented - log error to console instead
	(void)str;
	return false;
}
