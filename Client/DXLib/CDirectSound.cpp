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
CSDLAudio g_SDLAudio;

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------

CSDLAudio::CSDLAudio()
{
	m_pDS = NULL;
	m_bInit = false;
	m_bMute = false;
	m_MaxVolume = 0;
}

CSDLAudio::~CSDLAudio()
{
	// Stub - SDL mixer is managed elsewhere
}

//-----------------------------------------------------------------------------
// Initialization
//-----------------------------------------------------------------------------

bool CSDLAudio::Init(HWND hwnd)
{
	// Not implemented - use Mix_OpenAudio() from SDL_mixer instead
	(void)hwnd;
	return false;
}

void CSDLAudio::Release()
{
	// Not implemented - use Mix_CloseAudio() from SDL_mixer instead
}

bool CSDLAudio::IsInit() const
{
	return m_bInit;
}

//-----------------------------------------------------------------------------
// Mute Control
//-----------------------------------------------------------------------------

bool CSDLAudio::IsMute() const
{
	return m_bMute;
}

void CSDLAudio::SetMute()
{
	m_bMute = true;
}

void CSDLAudio::UnSetMute()
{
	m_bMute = false;
}

//-----------------------------------------------------------------------------
// Volume Control
//-----------------------------------------------------------------------------

LONG CSDLAudio::GetVolumeLimit() const
{
	return m_MaxVolume;
}

void CSDLAudio::SetVolumeLimit(LONG volume)
{
	m_MaxVolume = volume;
}

bool CSDLAudio::SetMaxVolume(LPDIRECTSOUNDBUFFER buffer)
{
	// Not implemented - use Mix_Volume() from SDL_mixer instead
	(void)buffer;
	return false;
}

bool CSDLAudio::AddVolume(LPDIRECTSOUNDBUFFER buffer, int step)
{
	// Not implemented - use Mix_Volume() from SDL_mixer instead
	(void)buffer;
	(void)step;
	return false;
}

bool CSDLAudio::SubVolume(LPDIRECTSOUNDBUFFER buffer, int step)
{
	// Not implemented - use Mix_Volume() from SDL_mixer instead
	(void)buffer;
	(void)step;
	return false;
}

bool CSDLAudio::SubVolumeFromMax(LPDIRECTSOUNDBUFFER buffer, int step)
{
	// Not implemented - use Mix_Volume() from SDL_mixer instead
	(void)buffer;
	(void)step;
	return false;
}

//-----------------------------------------------------------------------------
// Frequency Control
//-----------------------------------------------------------------------------

bool CSDLAudio::AddFrequency(LPDIRECTSOUNDBUFFER buffer, int step)
{
	// Not implemented - SDL_mixer doesn't support real-time frequency changes
	(void)buffer;
	(void)step;
	return false;
}

bool CSDLAudio::SubFrequency(LPDIRECTSOUNDBUFFER buffer, int step)
{
	// Not implemented - SDL_mixer doesn't support real-time frequency changes
	(void)buffer;
	(void)step;
	return false;
}

//-----------------------------------------------------------------------------
// Pan Control
//-----------------------------------------------------------------------------

bool CSDLAudio::RightPan(LPDIRECTSOUNDBUFFER buffer, int step)
{
	// Not implemented - SDL_mixer doesn't support real-time panning
	(void)buffer;
	(void)step;
	return false;
}

bool CSDLAudio::LeftPan(LPDIRECTSOUNDBUFFER buffer, int step)
{
	// Not implemented - SDL_mixer doesn't support real-time panning
	(void)buffer;
	(void)step;
	return false;
}

bool CSDLAudio::CenterToRightPan(LPDIRECTSOUNDBUFFER buffer, int step)
{
	// Not implemented - SDL_mixer doesn't support real-time panning
	(void)buffer;
	(void)step;
	return false;
}

bool CSDLAudio::CenterToLeftPan(LPDIRECTSOUNDBUFFER buffer, int step)
{
	// Not implemented - SDL_mixer doesn't support real-time panning
	(void)buffer;
	(void)step;
	return false;
}

bool CSDLAudio::CenterPan(LPDIRECTSOUNDBUFFER buffer)
{
	// Not implemented - SDL_mixer doesn't support real-time panning
	(void)buffer;
	return false;
}

bool CSDLAudio::ChangePan(LPDIRECTSOUNDBUFFER buffer, int pan)
{
	// Not implemented - SDL_mixer doesn't support real-time panning
	(void)buffer;
	(void)pan;
	return false;
}

//-----------------------------------------------------------------------------
// Sound Buffer Operations
//-----------------------------------------------------------------------------

LPDIRECTSOUNDBUFFER CSDLAudio::LoadWav(LPSTR filename)
{
	// Not implemented - use Mix_LoadWAV() from SDL_mixer instead
	(void)filename;
	return NULL;
}

LPDIRECTSOUNDBUFFER CSDLAudio::CreateBuffer(LPVOID sdat, DWORD size, DWORD caps, LPWAVEFORMATEX wfx)
{
	// Not implemented - use SDL_mixer sound functions instead
	(void)sdat;
	(void)size;
	(void)caps;
	(void)wfx;
	return NULL;
}

void CSDLAudio::Release(LPDIRECTSOUNDBUFFER buffer)
{
	// Not implemented - use Mix_FreeChunk() from SDL_mixer instead
	(void)buffer;
}

LPDIRECTSOUNDBUFFER CSDLAudio::DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER buffer, bool bAutoRelease)
{
	// Not implemented - SDL_mixer doesn't need buffer duplication
	(void)buffer;
	(void)bAutoRelease;
	return NULL;
}

void CSDLAudio::ReleaseDuplicateBuffer()
{
	// Not implemented - not needed with SDL_mixer
}

void CSDLAudio::ReleaseTerminatedDuplicateBuffer()
{
	// Not implemented - not needed with SDL_mixer
}

//-----------------------------------------------------------------------------
// Playback Control
//-----------------------------------------------------------------------------

bool CSDLAudio::IsPlay(LPDIRECTSOUNDBUFFER buffer) const
{
	// Not implemented - use Mix_Playing() from SDL_mixer instead
	(void)buffer;
	return false;
}

bool CSDLAudio::NewPlay(LPDIRECTSOUNDBUFFER buffer, bool loop)
{
	// Not implemented - use Mix_PlayChannel() from SDL_mixer instead
	(void)buffer;
	(void)loop;
	return false;
}

bool CSDLAudio::Play(LPDIRECTSOUNDBUFFER buffer, bool loop, bool duplicate)
{
	// Not implemented - use Mix_PlayChannel() from SDL_mixer instead
	(void)buffer;
	(void)loop;
	(void)duplicate;
	return false;
}

bool CSDLAudio::Stop(LPDIRECTSOUNDBUFFER buffer)
{
	// Not implemented - use Mix_HaltChannel() from SDL_mixer instead
	(void)buffer;
	return false;
}

//-----------------------------------------------------------------------------
// DirectSound Access
//-----------------------------------------------------------------------------

LPDIRECTSOUND CSDLAudio::GetDS() const
{
	return m_pDS;
}

//-----------------------------------------------------------------------------
// Error Handling
//-----------------------------------------------------------------------------

bool CSDLAudio::DirectSoundFailed(const char* str)
{
	// Not implemented - log error to console instead
	(void)str;
	return false;
}
