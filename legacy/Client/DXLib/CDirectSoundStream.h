//----------------------------------------------------------------------
// CDirectSoundStream.h
//----------------------------------------------------------------------
// 우헤헤..
// DirectX 샘플을 긁어서 급조된 필살 허접 class.. - -;
//----------------------------------------------------------------------

#ifndef __CDIRECTSOUNDSTREAM_H__
#define __CDIRECTSOUNDSTREAM_H__


#define NUM_PLAY_NOTIFICATIONS  16

#include <Windows.h>
#include <DSound.h>

class CDirectSoundStream {
	public :
		CDirectSoundStream();
		~CDirectSoundStream();

		
		void					Release();

		BOOL					IsLoad() const			{ return m_bLoad; }
		BOOL					IsPlay() const			{ return m_bPlay; }

		void					Load(LPSTR filename);

		void					Play(BOOL bLooped);
		void					Stop();

		void					SetVolumeLimit(LONG volume);
		LONG					GetVolumeLimit() const	{ return m_MaxVolume; }

		// main loop에서 돌려줘야 한다.
		void					Update();

		// get
		LPDIRECTSOUNDBUFFER		GetBuffer() const	{ return m_pDSBuffer; }

	protected :
		BOOL					UpdateProgress();
		BOOL					HandleNotification( BOOL bLooped );		
		BOOL					FillBuffer( BOOL bLooped );
		BOOL					ReadStream( BOOL bLooped, VOID* pbBuffer, DWORD dwBufferLength );
		BOOL					RestoreBuffers( BOOL bLooped );

		BOOL					Reset();
		HRESULT					WaveReadFile( HMMIO hmmioIn, UINT cbRead, BYTE* pbDest, MMCKINFO* pckIn, UINT* cbActualRead );

	protected :
		BOOL					m_bLoad;
		BOOL					m_bPlay;
		BOOL					m_bLoop;
		
		LPDIRECTSOUNDBUFFER		m_pDSBuffer;
		LPDIRECTSOUNDNOTIFY		m_pDSNotify;
		
		DSBPOSITIONNOTIFY		m_aPosNotify[ NUM_PLAY_NOTIFICATIONS + 1 ];  

		HANDLE					m_hNotificationEvents[2];

		// 음헤헤...
		DWORD					m_dwBufferSize;
		DWORD					m_dwNotifySize;
		DWORD					m_dwNextWriteOffset;
		DWORD					m_dwProgress;
		DWORD					m_dwLastPos;
		BOOL					m_bFoundEnd;

		// 가장 최근에 load한 Wav에 대한 정보
		WAVEFORMATEX			m_wavefmt;        // Pointer to WAVEFORMATEX structure
		HMMIO					m_hmmioIn;     // MM I/O handle for the WAVE
		MMCKINFO				m_ckIn;        // Multimedia RIFF chunk
		MMCKINFO				m_ckInRiff;    // Use in opening a WAVE file

		LONG					m_MaxVolume;		// 현재의 최대 소리 크기
};

#endif

