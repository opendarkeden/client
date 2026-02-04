/*-----------------------------------------------------------------------------

	CDirectSound_Adapter.cpp

	DirectSound adapter using DXLibBackend.
	This file provides SDL2 backend support for CDirectSound class.

	2025.01.14

-----------------------------------------------------------------------------*/

#include "CDirectSound.h"
#include "DXLibBackend.h"

/* Global instance */
CDirectSound	g_SDLAudio;

/*=============================================================================
 * SDL Backend Implementation
 *=============================================================================*/

#ifdef DXLIB_BACKEND_SDL

/* Sound buffer wrapper structure */
struct SoundBufferWrapper {
	dxlib_sound_t sound;		// Backend sound handle
	bool			is_playing;	// Playing state
	int				volume;		// Current volume (0-100)
	int				frequency;	// Frequency offset (not supported in SDL)
	int				pan;		// Pan value (-100 to 100, not supported in SDL)
	bool			auto_release; // Auto-release on stop
};

/* Constructor */
CSDLAudio::CDirectSound()
{
	m_pDS = NULL;
	m_bInit = false;
	m_bMute = false;
	m_MaxVolume = 100;
	m_listDuplicatedBuffer.clear();
}

/* Destructor */
CSDLAudio::~CDirectSound()
{
	Release();
}

/* Initialize SDL backend */
bool CSDLAudio::Init(HWND hWnd)
{
	if (dxlib_sound_init(hWnd) != 0) {
		return false;
	}

	m_pDS = (LPDIRECTSOUND)0x01;	// Non-null indicator
	m_bInit = true;
	return true;
}

/* Release SDL backend */
void CSDLAudio::Release()
{
	// Release all duplicated buffers
	ReleaseDuplicateBuffer();

	// Release backend
	dxlib_sound_release();

	m_pDS = NULL;
	m_bInit = false;
}

/* Release all duplicated buffers */
void CSDLAudio::ReleaseDuplicateBuffer()
{
	for (LPDIRECTSOUNDBUFFER_LIST::iterator it = m_listDuplicatedBuffer.begin();
		it != m_listDuplicatedBuffer.end(); ++it)
	{
		SoundBufferWrapper* wrapper = (SoundBufferWrapper*)(*it);
		if (wrapper) {
			dxlib_sound_free(wrapper->sound);
			delete wrapper;
		}
	}
	m_listDuplicatedBuffer.clear();
}

/* Load WAV file */
LPDIRECTSOUNDBUFFER CSDLAudio::LoadWav(LPSTR filename)
{
	if (!m_bInit) return NULL;

	dxlib_sound_t sound = dxlib_sound_load_wav(filename);
	if (!sound) {
		return NULL;
	}

	// Create wrapper
	SoundBufferWrapper* wrapper = new SoundBufferWrapper();
	wrapper->sound = sound;
	wrapper->is_playing = false;
	wrapper->volume = 100;	// Max volume
	wrapper->frequency = 0;
	wrapper->pan = 0;
	wrapper->auto_release = false;

	return (LPDIRECTSOUNDBUFFER)wrapper;
}

/* Create sound buffer from raw data */
LPDIRECTSOUNDBUFFER CSDLAudio::CreateBuffer(LPVOID sdat, DWORD size, DWORD caps, LPWAVEFORMATEX wfx)
{
	if (!m_bInit) return NULL;

	// Extract format info
	int channels = wfx->nChannels;
	int sample_rate = wfx->nSamplesPerSec;
	int bits_per_sample = wfx->wBitsPerSample;

	dxlib_sound_t sound = dxlib_sound_create_buffer(sdat, size,
		channels, sample_rate, bits_per_sample);

	if (!sound) {
		return NULL;
	}

	// Create wrapper
	SoundBufferWrapper* wrapper = new SoundBufferWrapper();
	wrapper->sound = sound;
	wrapper->is_playing = false;
	wrapper->volume = 100;
	wrapper->frequency = 0;
	wrapper->pan = 0;
	wrapper->auto_release = false;

	return (LPDIRECTSOUNDBUFFER)wrapper;
}

/* Release sound buffer */
void CSDLAudio::Release(LPDIRECTSOUNDBUFFER buffer)
{
	if (!buffer) return;

	SoundBufferWrapper* wrapper = (SoundBufferWrapper*)buffer;
	dxlib_sound_free(wrapper->sound);
	delete wrapper;
}

/* Duplicate sound buffer */
LPDIRECTSOUNDBUFFER CSDLAudio::DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER buffer, bool bAutoRelease)
{
	if (!buffer) return NULL;

	SoundBufferWrapper* src_wrapper = (SoundBufferWrapper*)buffer;

	// Use backend duplicate function
	dxlib_sound_t duplicated = dxlib_sound_duplicate(src_wrapper->sound);
	if (!duplicated) {
		return NULL;
	}

	// Create wrapper for duplicate
	SoundBufferWrapper* wrapper = new SoundBufferWrapper();
	wrapper->sound = duplicated;
	wrapper->is_playing = false;
	wrapper->volume = src_wrapper->volume;
	wrapper->frequency = src_wrapper->frequency;
	wrapper->pan = src_wrapper->pan;
	wrapper->auto_release = bAutoRelease;

	// Add to list if auto-release is enabled
	if (bAutoRelease) {
		m_listDuplicatedBuffer.push_back((LPDIRECTSOUNDBUFFER)wrapper);
	}

	return (LPDIRECTSOUNDBUFFER)wrapper;
}

/* Check if sound is playing */
bool CSDLAudio::IsPlay(LPDIRECTSOUNDBUFFER buffer) const
{
	if (!buffer) return false;

	SoundBufferWrapper* wrapper = (SoundBufferWrapper*)buffer;
	return dxlib_sound_is_playing(wrapper->sound) != 0;
}

/* Play sound (restart if already playing) */
bool CSDLAudio::NewPlay(LPDIRECTSOUNDBUFFER buffer, bool bLoop)
{
	if (!buffer) return false;
	if (m_bMute) return false;

	SoundBufferWrapper* wrapper = (SoundBufferWrapper*)buffer;

	// Stop if already playing
	if (dxlib_sound_is_playing(wrapper->sound)) {
		dxlib_sound_stop(wrapper->sound);
	}

	// Apply volume
	dxlib_sound_set_volume(wrapper->sound, wrapper->volume);

	// Play
	int loop_flag = bLoop ? 1 : 0;
	return dxlib_sound_play(wrapper->sound, loop_flag) == 0;
}

/* Play sound (allow simultaneous playback) */
bool CSDLAudio::Play(LPDIRECTSOUNDBUFFER buffer, bool bLoop, bool bDuplicate)
{
	if (!buffer) return false;
	if (m_bMute) return false;

	SoundBufferWrapper* wrapper = (SoundBufferWrapper*)buffer;

	// If duplicate is requested and sound is playing, create duplicate
	if (bDuplicate && dxlib_sound_is_playing(wrapper->sound)) {
		LPDIRECTSOUNDBUFFER dup = DuplicateSoundBuffer(buffer, true);
		if (dup) {
			wrapper = (SoundBufferWrapper*)dup;
		}
	}

	// Apply volume
	dxlib_sound_set_volume(wrapper->sound, wrapper->volume);

	// Play
	int loop_flag = bLoop ? 1 : 0;
	return dxlib_sound_play(wrapper->sound, loop_flag) == 0;
}

/* Stop sound */
bool CSDLAudio::Stop(LPDIRECTSOUNDBUFFER buffer)
{
	if (!buffer) return false;

	SoundBufferWrapper* wrapper = (SoundBufferWrapper*)buffer;
	return dxlib_sound_stop(wrapper->sound) == 0;
}

/* Release terminated duplicate buffers */
void CSDLAudio::ReleaseTerminatedDuplicateBuffer()
{
	LPDIRECTSOUNDBUFFER_LIST::iterator it = m_listDuplicatedBuffer.begin();
	while (it != m_listDuplicatedBuffer.end()) {
		SoundBufferWrapper* wrapper = (SoundBufferWrapper*)(*it);
		if (wrapper && !dxlib_sound_is_playing(wrapper->sound)) {
			dxlib_sound_free(wrapper->sound);
			delete wrapper;
			it = m_listDuplicatedBuffer.erase(it);
		} else {
			++it;
		}
	}
}

/* Set max volume */
bool CSDLAudio::SetMaxVolume(LPDIRECTSOUNDBUFFER buffer)
{
	if (!buffer) return false;

	SoundBufferWrapper* wrapper = (SoundBufferWrapper*)buffer;
	wrapper->volume = m_MaxVolume;
	return dxlib_sound_set_volume(wrapper->sound, m_MaxVolume) == 0;
}

/* Add volume */
bool CSDLAudio::AddVolume(LPDIRECTSOUNDBUFFER buffer, int amount)
{
	if (!buffer) return false;

	SoundBufferWrapper* wrapper = (SoundBufferWrapper*)buffer;
	wrapper->volume += amount;

	// Clamp volume
	if (wrapper->volume > 100) wrapper->volume = 100;
	if (wrapper->volume < 0) wrapper->volume = 0;

	return dxlib_sound_set_volume(wrapper->sound, wrapper->volume) == 0;
}

/* Subtract volume */
bool CSDLAudio::SubVolume(LPDIRECTSOUNDBUFFER buffer, int amount)
{
	if (!buffer) return false;

	SoundBufferWrapper* wrapper = (SoundBufferWrapper*)buffer;
	wrapper->volume -= amount;

	// Clamp volume
	if (wrapper->volume < 0) wrapper->volume = 0;

	return dxlib_sound_set_volume(wrapper->sound, wrapper->volume) == 0;
}

/* Subtract volume from max */
bool CSDLAudio::SubVolumeFromMax(LPDIRECTSOUNDBUFFER buffer, int amount)
{
	if (!buffer) return false;

	SoundBufferWrapper* wrapper = (SoundBufferWrapper*)buffer;
	wrapper->volume = m_MaxVolume - amount;

	// Clamp volume
	if (wrapper->volume < 0) wrapper->volume = 0;
	if (wrapper->volume > 100) wrapper->volume = 100;

	return dxlib_sound_set_volume(wrapper->sound, wrapper->volume) == 0;
}

/* Set volume limit */
void CSDLAudio::SetVolumeLimit(LONG volume)
{
	m_MaxVolume = volume;
	if (m_MaxVolume > 100) m_MaxVolume = 100;
	if (m_MaxVolume < 0) m_MaxVolume = 0;
}

/* Add frequency (not supported in SDL backend) */
bool CSDLAudio::AddFrequency(LPDIRECTSOUNDBUFFER buffer, int amount)
{
	// SDL_mixer does not support frequency adjustment
	return false;
}

/* Subtract frequency (not supported in SDL backend) */
bool CSDLAudio::SubFrequency(LPDIRECTSOUNDBUFFER buffer, int amount)
{
	// SDL_mixer does not support frequency adjustment
	return false;
}

/* Pan right (not supported in SDL backend) */
bool CSDLAudio::RightPan(LPDIRECTSOUNDBUFFER buffer, int amount)
{
	// SDL_mixer does not support pan control
	return false;
}

/* Pan left (not supported in SDL backend) */
bool CSDLAudio::LeftPan(LPDIRECTSOUNDBUFFER buffer, int amount)
{
	// SDL_mixer does not support pan control
	return false;
}

/* Pan from center to right (not supported in SDL backend) */
bool CSDLAudio::CenterToRightPan(LPDIRECTSOUNDBUFFER buffer, int amount)
{
	// SDL_mixer does not support pan control
	return false;
}

/* Pan from center to left (not supported in SDL backend) */
bool CSDLAudio::CenterToLeftPan(LPDIRECTSOUNDBUFFER buffer, int amount)
{
	// SDL_mixer does not support pan control
	return false;
}

/* Center pan (not supported in SDL backend) */
bool CSDLAudio::CenterPan(LPDIRECTSOUNDBUFFER buffer)
{
	// SDL_mixer does not support pan control
	return false;
}

/* Change pan (not supported in SDL backend) */
bool CSDLAudio::ChangePan(LPDIRECTSOUNDBUFFER buffer, int pan)
{
	// SDL_mixer does not support pan control
	// Pan range: -10000 to 10000 (DirectX) vs -100 to 100 (our backend)
	return false;
}

#endif /* DXLIB_BACKEND_SDL */
