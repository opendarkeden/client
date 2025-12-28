//-----------------------------------------------------------------------------
// CDirectSoundStream.cpp
//-----------------------------------------------------------------------------

#include "CDirectSoundStream.h"
#include "CDirectSound.h"

//-----------------------------------------------------------------------------
//
// constructor / destructor
//
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

	m_MaxVolume = DSBVOLUME_MAX;
}

CDirectSoundStream::~CDirectSoundStream()
{
	Release();
}

//-----------------------------------------------------------------------------
//
// member functions
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Release
//-----------------------------------------------------------------------------
void
CDirectSoundStream::Release()
{
	Stop();

	if (m_pDSBuffer != NULL)
	{
		m_pDSBuffer->Release();
		m_pDSBuffer = NULL;
	}

	if (m_pDSNotify != NULL)
	{
		m_pDSNotify->Release();
		m_pDSNotify = NULL;
	}

	m_bLoad = FALSE;
	m_bPlay = FALSE;

	ZeroMemory( &m_aPosNotify, sizeof(DSBPOSITIONNOTIFY) * 
							   (NUM_PLAY_NOTIFICATIONS + 1) );
	m_dwBufferSize      = 0;
	m_dwNotifySize      = 0;
	m_dwNextWriteOffset = 0;
	m_dwProgress        = 0;
	m_dwLastPos         = 0;
	m_bFoundEnd         = FALSE;

	
	CloseHandle( m_hNotificationEvents[0] );
    CloseHandle( m_hNotificationEvents[1] );
}


//----------------------------------------------------------------------
// 화일 로드(*.wav)
//----------------------------------------------------------------------
void
CDirectSoundStream::Load(LPSTR filename)
{
	if (g_DXSound.IsInit())
	{
		Release();

		//if (m_bInit)
		{
			
			// open a wav file
			//HMMIO m_hmmioIn;
			m_hmmioIn = mmioOpen(filename, 0, MMIO_READ|MMIO_ALLOCBUF);
			if(m_hmmioIn == NULL)
			{
				//DirectSoundFailed("Direct Sound mmioOpen Error!");		
				return;
			}

			m_hNotificationEvents[0] = CreateEvent( NULL, FALSE, FALSE, NULL );
			m_hNotificationEvents[1] = CreateEvent( NULL, FALSE, FALSE, NULL );
			
			
			// find wave data
			//MMCKINFO m_ckInRiff;
			memset(&m_ckInRiff, 0, sizeof(MMCKINFO));
			m_ckInRiff.fccType = mmioFOURCC('W','A','V','E');
			mmioDescend(m_hmmioIn, &m_ckInRiff, 0, MMIO_FINDRIFF);

			// find fmt data
			//MMCKINFO m_ckIn;
			memset(&m_ckIn, 0, sizeof(MMCKINFO));
			m_ckIn.fccType = mmioFOURCC('f','m','t',' ');
			mmioDescend(m_hmmioIn, &m_ckIn, &m_ckInRiff,0);

			// read the format
			//WAVEFORMATEX m_wavefmt;
			mmioRead(m_hmmioIn, (char*)&m_wavefmt, sizeof(m_wavefmt));
			if(m_wavefmt.wFormatTag != WAVE_FORMAT_PCM)
			{
				//DirectSoundFailed("Direct Sound mmioRead Error!");		
				return;
			}

			// find the wave data chunk
			mmioAscend(m_hmmioIn, &m_ckIn, 0);
			m_ckIn.ckid = mmioFOURCC('d','a','t','a');
			mmioDescend(m_hmmioIn, &m_ckIn, &m_ckInRiff, MMIO_FINDCHUNK);

			// This samples works by dividing a 3 second streaming buffer into 
			// NUM_PLAY_NOTIFICATIONS (or 16) pieces.  it creates a notification for each
			// piece and when a notification arrives then it fills the circular streaming 
			// buffer with new wav data over the sound data which was just played

			// The size of wave data is in pWaveFileSound->m_ckIn
			DWORD nBlockAlign = (DWORD)m_wavefmt.nBlockAlign;
			INT nSamplesPerSec = m_wavefmt.nSamplesPerSec;

			// The m_dwNotifySize should be an integer multiple of nBlockAlign
			m_dwNotifySize = nSamplesPerSec * 3 * nBlockAlign / NUM_PLAY_NOTIFICATIONS;
			m_dwNotifySize -= m_dwNotifySize % nBlockAlign;   

			// The buffersize should approximately 3 seconds of wav data
			m_dwBufferSize  = m_dwNotifySize * NUM_PLAY_NOTIFICATIONS;
			m_bFoundEnd     = FALSE;
			m_dwProgress    = 0;
			m_dwLastPos     = 0;

			// Set up the direct sound buffer, and only request the flags needed
			// since each requires some overhead and limits if the buffer can 
			// be hardware accelerated
			DSBUFFERDESC dsbd;
			ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
			dsbd.dwSize        = sizeof(DSBUFFERDESC);
			dsbd.dwFlags       = DSBCAPS_CTRLPOSITIONNOTIFY 
								// 우헤헤 이거 안 넣은걸 이제 발견했다. 2001.9.21
								| DSBCAPS_CTRLVOLUME		
								| DSBCAPS_GETCURRENTPOSITION2;
			dsbd.dwBufferBytes = m_dwBufferSize;
			dsbd.lpwfxFormat   = &m_wavefmt;

			// Create a DirectSound buffer
			if( FAILED( g_DXSound.m_pDS->CreateSoundBuffer( &dsbd, &m_pDSBuffer, NULL ) ) )
				return;

			// Create a notification event, for when the sound stops playing
			if( FAILED( m_pDSBuffer->QueryInterface( IID_IDirectSoundNotify, 
														  (VOID**)&m_pDSNotify ) ) )
				return;

			for( int i = 0; i < NUM_PLAY_NOTIFICATIONS; i++ )
			{
				m_aPosNotify[i].dwOffset = (m_dwNotifySize * i) + m_dwNotifySize - 1;
				m_aPosNotify[i].hEventNotify = m_hNotificationEvents[0];             
			}
    
			m_aPosNotify[i].dwOffset     = DSBPN_OFFSETSTOP;
			m_aPosNotify[i].hEventNotify = m_hNotificationEvents[1];

			// Tell DirectSound when to notify us. the notification will come in the from 
			// of signaled events that are handled in WinMain()
			if( FAILED( m_pDSNotify->SetNotificationPositions( NUM_PLAY_NOTIFICATIONS + 1, 
																	m_aPosNotify ) ) )
				return;

			// write wave data to directsound buffer you just created
			/*
			void *write1 = 0; 
			void *write2 = 0;
			unsigned long length1;
			unsigned long length2;

			m_pDSBuffer->Lock(0, m_ckIn.cksize, &write1, &length1, &write2, &length2, 0);
			if(write1 > 0)
			{
				mmioRead(m_hmmioIn, (char*)write1, length1);
			}
			if(write2 > 0)
			{
				mmioRead(m_hmmioIn, (char*)write2, length2);
			}
			m_pDSBuffer->Unlock(write1, length1, write2, length2);

			//close the m_hmmioIn, don't need it anymore, it's in the directsound buffer now
			mmioClose(m_hmmioIn, 0);
			*/
			//return buffer;
		}

		m_bLoad = TRUE;

		//return;
	}
}



//-----------------------------------------------------------------------------
// Name: Play()
// Desc: Play the DirectSound buffer
//-----------------------------------------------------------------------------
void
CDirectSoundStream::Play( BOOL bLooped )
{
	if (g_DXSound.IsInit())
	{
		if (!m_bLoad)
		{
			return;
		}

		VOID*   pbBuffer = NULL;

		if( NULL == m_pDSBuffer )
			return;

		// Restore the buffers if they are lost
		if( FAILED(RestoreBuffers( bLooped ) ) )
			return;

		// Fill the entire buffer with wave data
		if( FAILED(FillBuffer( bLooped ) ) )
			return;

		// Always play with the LOOPING flag since the streaming buffer
		// wraps around before the entire WAV is played
		if( FAILED(m_pDSBuffer->Play( 0, 0, DSBPLAY_LOOPING ) ) )
			return;

		m_bLoop = bLooped;

		m_bPlay = TRUE;
	
		// volume 조절
		if (m_pDSBuffer!=NULL)
		{
			m_pDSBuffer->SetVolume( m_MaxVolume );	
		}

	}
}




//-----------------------------------------------------------------------------
// Name: FillBuffer()
// Desc: Fills the DirectSound buffer with wave data
//-----------------------------------------------------------------------------
BOOL
CDirectSoundStream::FillBuffer( BOOL bLooped )
{
	if (g_DXSound.IsInit())
	{
		if (!m_bLoad)
		{
			return FALSE;
		}

		VOID*   pbBuffer = NULL;
		DWORD   dwBufferLength;

		if( NULL == m_pDSBuffer )
			return FALSE;

		m_bFoundEnd = FALSE;
		m_dwNextWriteOffset = 0; 
		m_dwProgress = 0;
		m_dwLastPos  = 0;

		// Reset the wav file to the beginning
		Reset();
		
		m_pDSBuffer->SetCurrentPosition( 0 );

		// Lock the buffer down, 
		if( FAILED( m_pDSBuffer->Lock( 0, m_dwBufferSize, 
											&pbBuffer, &dwBufferLength, 
											NULL, NULL, 0L ) ) )
			return FALSE;

		// Fill the buffer with wav data 
		if( FAILED(ReadStream( bLooped, pbBuffer, dwBufferLength ) ) )
			return FALSE;

		// Now unlock the buffer
		m_pDSBuffer->Unlock( pbBuffer, dwBufferLength, NULL, 0 );

		m_dwNextWriteOffset = dwBufferLength; 
		m_dwNextWriteOffset %= m_dwBufferSize; // Circular buffer

		return TRUE;
	}

	return FALSE;
}




//-----------------------------------------------------------------------------
// Name: ReadStream()
// Desc: Writes wave data to the streaming DirectSound buffer 
//-----------------------------------------------------------------------------
BOOL
CDirectSoundStream::ReadStream( BOOL bLooped, VOID* pbBuffer, DWORD dwBufferLength )
{
	if (g_DXSound.IsInit())
	{
		if (!m_bLoad)
		{
			return FALSE;
		}

		UINT nActualBytesWritten;

		if( !m_bFoundEnd )
		{
			// Fill the DirectSound buffer with WAV data
			//if( FAILED( Read( dwBufferLength, 
			  //               (BYTE*) pbBuffer, 
				//             &nActualBytesWritten ) ) )           
			if (FAILED( WaveReadFile( m_hmmioIn, 
									dwBufferLength, (BYTE*) pbBuffer, &m_ckIn, 
									&nActualBytesWritten )))
				return FALSE;
		}
		else
		{
			// Fill the DirectSound buffer with silence
			FillMemory( pbBuffer, dwBufferLength, 
						(BYTE)( m_wavefmt.wBitsPerSample == 8 ? 128 : 0 ) );
			nActualBytesWritten = dwBufferLength;
		}

		// If the number of bytes written is less than the 
		// amount we requested, we have a short file.
		if( nActualBytesWritten < dwBufferLength )
		{
			if( !bLooped ) 
			{
				m_bFoundEnd = TRUE;

				// Fill in silence for the rest of the buffer.
				FillMemory( (BYTE*) pbBuffer + nActualBytesWritten, 
							dwBufferLength - nActualBytesWritten, 
							(BYTE)(m_wavefmt.wBitsPerSample == 8 ? 128 : 0 ) );
			}
			else
			{
				// We are looping.
				UINT nWritten = nActualBytesWritten;    // From previous call above.
				while( nWritten < dwBufferLength )
				{  
					// This will keep reading in until the buffer is full. For very short files.
					if( FAILED( Reset() ) )
						return FALSE;

					//if( FAILED( Read( (UINT)dwBufferLength - nWritten,
					  //                                        (BYTE*)pbBuffer + nWritten,
						//                                      &nActualBytesWritten ) ) )
					if (FAILED( WaveReadFile( m_hmmioIn, 
									(UINT)dwBufferLength - nWritten, (BYTE*)pbBuffer+nWritten, &m_ckIn, 
									&nActualBytesWritten )))
						return FALSE;

					nWritten += nActualBytesWritten;
				} 
			} 
		}

		return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// Reset
//-----------------------------------------------------------------------------
BOOL
CDirectSoundStream::Reset()
{
	if( -1 == mmioSeek( m_hmmioIn, m_ckInRiff.dwDataOffset + sizeof(FOURCC),
                        SEEK_SET ) )
        return FALSE;

    // Search the input file for for the 'data' chunk.
    m_ckIn.ckid = mmioFOURCC('d', 'a', 't', 'a');
    if( 0 != mmioDescend( m_hmmioIn, &m_ckIn, &m_ckInRiff, MMIO_FINDCHUNK ) )
        return FALSE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Wave Read File
//-----------------------------------------------------------------------------
HRESULT 
CDirectSoundStream::WaveReadFile( HMMIO hmmioIn, UINT cbRead, BYTE* pbDest,
                      MMCKINFO* pckIn, UINT* cbActualRead )
{
    MMIOINFO mmioinfoIn;         // current status of <hmmioIn>

    *cbActualRead = 0;

    if( 0 != mmioGetInfo( hmmioIn, &mmioinfoIn, 0 ) )
        return E_FAIL;
                
    UINT cbDataIn = cbRead;
    if( cbDataIn > pckIn->cksize ) 
        cbDataIn = pckIn->cksize;       

    pckIn->cksize -= cbDataIn;
    
    for( DWORD cT = 0; cT < cbDataIn; cT++ )
    {
        // Copy the bytes from the io to the buffer.
        if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
        {
            if( 0 != mmioAdvance( hmmioIn, &mmioinfoIn, MMIO_READ ) )
                return E_FAIL;

            if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
                return E_FAIL;
        }

        // Actual copy.
        *((BYTE*)pbDest+cT) = *((BYTE*)mmioinfoIn.pchNext);
        mmioinfoIn.pchNext++;
    }

    if( 0 != mmioSetInfo( hmmioIn, &mmioinfoIn, 0 ) )
        return E_FAIL;

    *cbActualRead = cbDataIn;
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Stop()
// Desc: Stop the DirectSound buffer
//-----------------------------------------------------------------------------
void
CDirectSoundStream::Stop() 
{
	if (g_DXSound.IsInit())
	{
		if (!m_bLoad || !m_bPlay)
		{
			return;
		}


		if( NULL != m_pDSBuffer )
		{
			m_pDSBuffer->Stop();   
			m_pDSBuffer->Release();
			m_pDSBuffer = NULL;			
		}
	}

	m_bLoad = FALSE;
	m_bPlay = FALSE;
}




//-----------------------------------------------------------------------------
// Name: HandleNotification()
// Desc: Handle the notification that tell us to put more wav data in the 
//       circular buffer
//-----------------------------------------------------------------------------
BOOL 
CDirectSoundStream::HandleNotification( BOOL bLooped ) 
{
	if (!m_bLoad)
	{
		return FALSE;
	}

    VOID* pbBuffer  = NULL;
    DWORD dwBufferLength;

    UpdateProgress();

    // Lock the buffer down
    if( FAILED( m_pDSBuffer->Lock( m_dwNextWriteOffset, m_dwNotifySize, 
                                        &pbBuffer, &dwBufferLength, NULL, NULL, 0L ) ) )
        return FALSE;

    // Fill the buffer with wav data 
    if( FAILED( ReadStream( bLooped, pbBuffer, dwBufferLength ) ) )
	{
		//m_pDSBuffer->Unlock( pbBuffer, dwBufferLength, NULL, 0 );
        return FALSE;
	}

    // Now unlock the buffer
    m_pDSBuffer->Unlock( pbBuffer, dwBufferLength, NULL, 0 );
    pbBuffer = NULL;
	
	// volume 조절
	static LONG previousVolume = m_MaxVolume;

	if (previousVolume!=m_MaxVolume && m_pDSBuffer!=NULL)
	{
		m_pDSBuffer->SetVolume( m_MaxVolume );	

		previousVolume = m_MaxVolume;
	}

    // If the end was found, detrimine if there's more data to play 
    // and if not, stop playing
    if( m_bFoundEnd )
    {
        // We don't want to cut off the sound before it's done playing.
        // if doneplaying is set, the next notification event will post a stop message.
        if( m_ckInRiff.cksize > m_dwNotifySize )
        {
            if( m_dwProgress >= m_ckInRiff.cksize - m_dwNotifySize )
            {
                m_pDSBuffer->Stop();
            }
        }
        else // For short files.
        {
            if( m_dwProgress >= m_ckInRiff.cksize )
            {
                m_pDSBuffer->Stop();
            }
        }
    }

    m_dwNextWriteOffset += dwBufferLength; 
    m_dwNextWriteOffset %= m_dwBufferSize; // Circular buffer

    return TRUE;
}




//-----------------------------------------------------------------------------
// Name: UpdateProgress()
// Desc: Update the progress variable to know when the entire buffer was played
//-----------------------------------------------------------------------------
BOOL
CDirectSoundStream::UpdateProgress()
{
	if (g_DXSound.IsInit())
	{
		if (!m_bLoad)
		{
			return FALSE;
		}

		DWORD   dwPlayPos;
		DWORD   dwWritePos;
		DWORD   dwPlayed;

		if( FAILED( m_pDSBuffer->GetCurrentPosition( &dwPlayPos, &dwWritePos ) ) )
			return FALSE;

		if( dwPlayPos < m_dwLastPos )
			dwPlayed = m_dwBufferSize - m_dwLastPos + dwPlayPos;
		else
			dwPlayed = dwPlayPos - m_dwLastPos;

		m_dwProgress += dwPlayed;
		m_dwLastPos = dwPlayPos;

		return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// Name: RestoreBuffers()
// Desc: Restore lost buffers and fill them up with sound if possible
//-----------------------------------------------------------------------------
BOOL
CDirectSoundStream::RestoreBuffers( BOOL bLooped )
{
	HRESULT hr;

   if( NULL == m_pDSBuffer )
        return FALSE;

    DWORD dwStatus;
    if( FAILED( m_pDSBuffer->GetStatus( &dwStatus ) ) )
        return FALSE;

    if( dwStatus & DSBSTATUS_BUFFERLOST )
    {
        // Since the app could have just been activated, then
        // DirectSound may not be giving us control yet, so 
        // the restoring the buffer may fail.  
        // If it does, sleep until DirectSound gives us control.
        do 
        {
            hr = m_pDSBuffer->Restore();
            if( hr == DSERR_BUFFERLOST )
                Sleep( 10 );
        }
        while( hr = m_pDSBuffer->Restore() );

        if( FAILED( FillBuffer( bLooped ) ) )
            return FALSE;
    }

    return TRUE;
}



//-----------------------------------------------------------------------------
// Update
//-----------------------------------------------------------------------------
// event처리
//-----------------------------------------------------------------------------
void					
CDirectSoundStream::Update()
{
	if (g_DXSound.IsInit())
	{
		if (m_bLoad)
		{
			//---------------------------------------------------------
			// This means that DirectSound just finished playing 
			// a piece of the buffer, so we need to fill the circular 
			// buffer with new sound from the wav file 
			//---------------------------------------------------------
			if (WaitForSingleObject(m_hNotificationEvents[0], 0) == WAIT_OBJECT_0)
			{
				HandleNotification( m_bLoop );
			}

			//---------------------------------------------------------
			// 중지 됐을 때,
			//---------------------------------------------------------
			if (WaitForSingleObject(m_hNotificationEvents[1], 0) == WAIT_OBJECT_0)
			{
			}
		}
	}
}

//----------------------------------------------------------------------
// Set Volume Limit
//----------------------------------------------------------------------
void					
CDirectSoundStream::SetVolumeLimit(LONG volume)
{
	if (volume > DSBVOLUME_MAX)
	{
		m_MaxVolume = DSBVOLUME_MAX;
	}
	else if (volume < DSBVOLUME_MIN)
	{
		m_MaxVolume = DSBVOLUME_MIN;
	}
	else 
	{
		m_MaxVolume = volume;
	}
	
}