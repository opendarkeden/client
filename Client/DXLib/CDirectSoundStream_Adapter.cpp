/*-----------------------------------------------------------------------------

	CDirectSoundStream_Adapter.cpp

	DirectSoundStream adapter using DXLibBackend.
	This file provides SDL2 backend support for CDirectSoundStream class.

	2025.01.14

-----------------------------------------------------------------------------*/

#include "CDirectSoundStream.h"
#include "DXLibBackend.h"

/*=============================================================================
 * SDL Backend Implementation
 *=============================================================================*/

#ifdef DXLIB_BACKEND_SDL

/* Constructor */
CDirectSoundStream::CDirectSoundStream()
{
	m_bLoad = false;
	m_bPlay = false;
	m_bLoop = false;

	m_pDSBuffer = NULL;
	m_pDSNotify = NULL;

	m_dwBufferSize = 0;
	m_dwNotifySize = 0;
	m_dwNextWriteOffset = 0;
	m_dwProgress = 0;
	m_dwLastPos = 0;
	m_bFoundEnd = false;

	m_MaxVolume = 100;
}

/* Destructor */
CDirectSoundStream::~CDirectSoundStream()
{
	Release();
}

/* Release stream */
void CDirectSoundStream::Release()
{
	// Stop if playing
	if (m_bPlay) {
		Stop();
	}

	// Free stream
	if (m_bLoad) {
		dxlib_stream_t stream = (dxlib_stream_t)m_pDSBuffer;
		dxlib_stream_free(stream);
	}

	m_pDSBuffer = NULL;
	m_bLoad = false;
	m_bPlay = false;
}

/* Load audio file for streaming */
void CDirectSoundStream::Load(LPSTR filename)
{
	// Initialize stream backend if needed
	if (!dxlib_get_backend_name()) {
		dxlib_stream_init(NULL);
	}

	// Load stream
	dxlib_stream_t stream = dxlib_stream_load(filename);
	if (stream) {
		m_pDSBuffer = (LPDIRECTSOUNDBUFFER)stream;
		m_bLoad = true;
		m_bPlay = false;
	}
}

/* Play stream */
void CDirectSoundStream::Play(BOOL bLooped)
{
	if (!m_bLoad) return;

	dxlib_stream_t stream = (dxlib_stream_t)m_pDSBuffer;
	if (dxlib_stream_play(stream, bLooped ? 1 : 0) == 0) {
		m_bPlay = true;
		m_bLoop = bLooped;
	}
}

/* Stop stream */
void CDirectSoundStream::Stop()
{
	if (!m_bLoad) return;

	dxlib_stream_t stream = (dxlib_stream_t)m_pDSBuffer;
	dxlib_stream_stop(stream);
	m_bPlay = false;
}

/* Set volume limit */
void CDirectSoundStream::SetVolumeLimit(LONG volume)
{
	m_MaxVolume = volume;
	if (m_MaxVolume > 100) m_MaxVolume = 100;
	if (m_MaxVolume < 0) m_MaxVolume = 0;

	if (m_bLoad) {
		dxlib_stream_t stream = (dxlib_stream_t)m_pDSBuffer;
		dxlib_stream_set_volume(stream, m_MaxVolume);
	}
}

/* Update stream (call regularly to refill buffers) */
void CDirectSoundStream::Update()
{
	if (!m_bLoad || !m_bPlay) return;

	dxlib_stream_t stream = (dxlib_stream_t)m_pDSBuffer;
	dxlib_stream_update(stream);

	// Check if stream has finished
	if (!dxlib_stream_is_playing(stream)) {
		m_bPlay = false;
	}
}

/* Update progress (stub for SDL backend) */
BOOL CDirectSoundStream::UpdateProgress()
{
	// Not applicable for SDL backend
	return TRUE;
}

/* Handle notification (stub for SDL backend) */
BOOL CDirectSoundStream::HandleNotification(BOOL bLooped)
{
	// Not applicable for SDL backend
	return TRUE;
}

/* Fill buffer (stub for SDL backend) */
BOOL CDirectSoundStream::FillBuffer(BOOL bLooped)
{
	// Not applicable for SDL backend
	return TRUE;
}

/* Read stream (stub for SDL backend) */
BOOL CDirectSoundStream::ReadStream(BOOL bLooped, VOID* pbBuffer, DWORD dwBufferLength)
{
	// Not applicable for SDL backend
	return TRUE;
}

/* Restore buffers (stub for SDL backend) */
BOOL CDirectSoundStream::RestoreBuffers(BOOL bLooped)
{
	// Not applicable for SDL backend
	return TRUE;
}

/* Reset (stub for SDL backend) */
BOOL CDirectSoundStream::Reset()
{
	// Not applicable for SDL backend
	return TRUE;
}

/* Wave read file (stub for SDL backend) */
HRESULT CDirectSoundStream::WaveReadFile(HMMIO hmmioIn, UINT cbRead, BYTE* pbDest,
	MMCKINFO* pckIn, UINT* cbActualRead)
{
	// Not applicable for SDL backend
	return S_OK;
}

#endif /* DXLIB_BACKEND_SDL */
