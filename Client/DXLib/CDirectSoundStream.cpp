//-----------------------------------------------------------------------------
// CDirectSoundStream.cpp
//
// SDL2 Implementation (Cross-platform)
// Windows DirectSoundStream implementation removed - using SDL2_mixer on all platforms
// NOTE: This class is deprecated. Use Mix_PlayChannel() from SDL_mixer for streaming audio.
//-----------------------------------------------------------------------------

#include "CDirectSoundStream.h"

// Define E_FAIL for compatibility (Windows HRESULT value)
#ifndef E_FAIL
#define E_FAIL 0x80004005L
#endif

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------

CDirectSoundStream::CDirectSoundStream()
{
	m_pDSBuffer = NULL;
	m_pDSNotify = NULL;

	m_bLoad = FALSE;
	m_bPlay = FALSE;
	m_bLoop = FALSE;

	m_hNotificationEvents[0] = NULL;
	m_hNotificationEvents[1] = NULL;

	m_MaxVolume = 0;
}

CDirectSoundStream::~CDirectSoundStream()
{
	// Stub - SDL mixer is managed elsewhere
}

//-----------------------------------------------------------------------------
// Stream Operations
//-----------------------------------------------------------------------------

void CDirectSoundStream::Release()
{
	// Not implemented - use Mix_FreeChunk() from SDL_mixer instead
}

void CDirectSoundStream::Load(LPSTR filename)
{
	// Not implemented - use Mix_LoadWAV() from SDL_mixer instead
	(void)filename;
}

void CDirectSoundStream::Play(BOOL bLooped)
{
	// Not implemented - use Mix_PlayChannel() from SDL_mixer instead
	(void)bLooped;
}

void CDirectSoundStream::Stop()
{
	// Not implemented - use Mix_HaltChannel() from SDL_mixer instead
}

BOOL CDirectSoundStream::FillBuffer(BOOL bLooped)
{
	// Not implemented - SDL_mixer handles buffering automatically
	(void)bLooped;
	return FALSE;
}

BOOL CDirectSoundStream::ReadStream(BOOL bLooped, VOID* pbBuffer, DWORD dwBufferLength)
{
	// Not implemented - SDL_mixer handles streaming automatically
	(void)bLooped;
	(void)pbBuffer;
	(void)dwBufferLength;
	return FALSE;
}

BOOL CDirectSoundStream::Reset()
{
	// Not implemented - SDL_mixer handles position automatically
	return FALSE;
}

HRESULT CDirectSoundStream::WaveReadFile(HMMIO hmmioIn, UINT cbRead, BYTE* pbDest,
	MMCKINFO* pckIn, UINT* cbActualRead)
{
	// Not implemented - Windows multimedia API only
	(void)hmmioIn;
	(void)cbRead;
	(void)pbDest;
	(void)pckIn;
	(void)cbActualRead;
	return E_FAIL;
}

//-----------------------------------------------------------------------------
// Notification and Updates
//-----------------------------------------------------------------------------

BOOL CDirectSoundStream::HandleNotification(BOOL bLooped)
{
	// Not implemented - SDL_mixer doesn't use notification events
	(void)bLooped;
	return FALSE;
}

BOOL CDirectSoundStream::UpdateProgress()
{
	// Not implemented - SDL_mixer handles position tracking
	return FALSE;
}

BOOL CDirectSoundStream::RestoreBuffers(BOOL bLooped)
{
	// Not implemented - SDL_mixer handles buffer restoration
	(void)bLooped;
	return FALSE;
}

void CDirectSoundStream::Update()
{
	// Not implemented - SDL_mixer handles updates automatically
}

//-----------------------------------------------------------------------------
// Volume Control
//-----------------------------------------------------------------------------

void CDirectSoundStream::SetVolumeLimit(LONG volume)
{
	// Not implemented - use Mix_Volume() from SDL_mixer instead
	m_MaxVolume = volume;
}
