//----------------------------------------------------------------------
// CDirectSound.cpp
//----------------------------------------------------------------------
#include "DirectXlib_PCH.h"
#include "CDirectSound.h"
//#include "DebugInfo.h"
#include "Profiler.h"

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
CDirectSound		g_DXSound;

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
CDirectSound::CDirectSound()
{
	m_pDS = NULL;
	m_bInit = false;
	m_bMute = false;

	m_MaxVolume = DSBVOLUME_MAX;
}

CDirectSound::~CDirectSound()
{
	Release();
}


//----------------------------------------------------------------------
//
//  member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Init(hWnd)
//----------------------------------------------------------------------
bool
CDirectSound::Init(HWND hwnd)
{
	// ÀÌ¹Ì ÃÊ±âÈ­µÇ¾îÀÖÀ» °æ¿ì...
	if (m_bInit)
		return false;

	if( FAILED(CoInitialize(NULL)) )
        return false;

	// Direct Sound Create
	if( FAILED(DirectSoundCreate(NULL, &m_pDS, NULL)) )
	{
		Release();
		DirectSoundFailed("Direct Sound Create Error!");
		return false;
	}

	// Interface Cooperativelevel
	if( FAILED(m_pDS->SetCooperativeLevel(hwnd, DSSCL_NORMAL)) )
	{
		Release();
		DirectSoundFailed("Direct Sound CooperativeLevel Error!");
		return false;
	}
	
	// Speaker Setting
	if( FAILED(m_pDS->SetSpeakerConfig(DSSPEAKER_COMBINED(DSSPEAKER_STEREO, DSSPEAKER_GEOMETRY_WIDE))) )
	{
		Release();
		DirectSoundFailed("Direct Sound Speaker Setting Error!");
		return false;
	}


	m_bInit = true;
	return true;
}


//----------------------------------------------------------------------
// Release()
//----------------------------------------------------------------------
void 
CDirectSound::Release()
{
	if(m_bInit && m_pDS)
	{		
		///*
		// 2001.8.20 ÁÖ¼®Ã³¸®
		LPDIRECTSOUNDBUFFER_LIST::iterator iBuffer = m_listDuplicatedBuffer.begin();

		// °¢°¢ÀÇ Duplicated Buffer¸¦ Áö¿î´Ù.
		while (iBuffer != m_listDuplicatedBuffer.end())
		{
			(*iBuffer)->Release();

			// ´ÙÀ½ °Í
			iBuffer++;			
		}
		m_listDuplicatedBuffer.clear();
		//*/

		// DirectSound¸¦ release
		m_pDS->Release(); 
		m_pDS	= NULL;

		// Release COM
		CoUninitialize();
	}
}

//----------------------------------------------------------------------
// Release Duplicate Buffer
//----------------------------------------------------------------------
void
CDirectSound::ReleaseDuplicateBuffer()
{
	LPDIRECTSOUNDBUFFER_LIST::iterator iBuffer = m_listDuplicatedBuffer.begin();

	while (iBuffer != m_listDuplicatedBuffer.end())
	{
		LPDIRECTSOUNDBUFFER	pBuffer = *iBuffer;

		if (pBuffer != NULL)
		{
			Stop( pBuffer );
			Release( pBuffer );
		}
		
		// ´ÙÀ½ °Í
		iBuffer++;		
	}

	m_listDuplicatedBuffer.clear();
}

//----------------------------------------------------------------------
// Release Terminated DuplicateBuffer
//----------------------------------------------------------------------
void
CDirectSound::ReleaseTerminatedDuplicateBuffer()
{
	///*
	// 2001.8.20 ÁÖ¼®Ã³¸®
	LPDIRECTSOUNDBUFFER_LIST::iterator iBuffer = m_listDuplicatedBuffer.begin();

	DWORD	status;

	while (iBuffer != m_listDuplicatedBuffer.end())
	{
		LPDIRECTSOUNDBUFFER	pBuffer = *iBuffer;

		// bufferÀÇ »óÅÂ¸¦ ÀÐ¾î¿Â´Ù.
		pBuffer->GetStatus(&status);

		// Á¾·á µÇ¾úÀ¸¸é..
		// Buffer¸¦ ReleaseÇÑ´Ù.
		if (status == DSBSTATUS_TERMINATED
			|| status != DSBSTATUS_PLAYING
				&& status != DSBSTATUS_LOOPING)
		//if (status != DSBSTATUS_TERMINATED)
		{			
			pBuffer->Release();

			// node»èÁ¦
			LPDIRECTSOUNDBUFFER_LIST::iterator iBufferTemp = iBuffer;

			iBuffer++;

			m_listDuplicatedBuffer.erase( iBufferTemp );
		}
		else
		{
			// ´ÙÀ½ °Í
			iBuffer++;
		}
	}
	//*/
}

//----------------------------------------------------------------------
// DirectSound Failed MessageBox
//----------------------------------------------------------------------
bool
CDirectSound::DirectSoundFailed(const char *str)
{
	//::MessageBox( NULL, str, "DirectSoundFaild!", MB_ICONINFORMATION|MB_OK );

	#ifdef OUTPUT_DEBUG
		DEBUG_ADD_FORMAT("DirectSoundFailed! : %s", str);
	#endif

	return false;
}


//----------------------------------------------------------------------
// È­ÀÏ ·Îµå(*.wav)
//----------------------------------------------------------------------
LPDIRECTSOUNDBUFFER 
CDirectSound::LoadWav(LPSTR filename)
{
	__BEGIN_PROFILE("LoadWav")

	if (m_bInit)
	{
		LPDIRECTSOUNDBUFFER buffer;

		// open a wav file
		HMMIO wavefile;
		wavefile = mmioOpen(filename, 0, MMIO_READ|MMIO_ALLOCBUF);
		if(wavefile == NULL)
		{
			//DirectSoundFailed("Direct Sound mmioOpen Error!");		
			__END_PROFILE("LoadWav")
			return NULL;
		}
		
		// find wave data
		MMCKINFO parent;
		memset(&parent, 0, sizeof(MMCKINFO));
		parent.fccType = mmioFOURCC('W','A','V','E');
		mmioDescend(wavefile, &parent, 0, MMIO_FINDRIFF);

		// find fmt data
		MMCKINFO child;
		memset(&child, 0, sizeof(MMCKINFO));
		child.fccType = mmioFOURCC('f','m','t',' ');
		mmioDescend(wavefile, &child, &parent,0);

		// read the format
		WAVEFORMATEX wavefmt;
		mmioRead(wavefile, (char*)&wavefmt, sizeof(wavefmt));
		if(wavefmt.wFormatTag != WAVE_FORMAT_PCM)
		{
			DirectSoundFailed("Direct Sound mmioRead Error!");		
			__END_PROFILE("LoadWav")
			return NULL;
		}

		// find the wave data chunk
		mmioAscend(wavefile, &child, 0);
		child.ckid = mmioFOURCC('d','a','t','a');
		mmioDescend(wavefile, &child, &parent, MMIO_FINDCHUNK);

		// create a directsound buffer to hold wave data
		DSBUFFERDESC bufdesc;
		memset(&bufdesc, 0, sizeof(DSBUFFERDESC));
		bufdesc.dwSize			= sizeof(DSBUFFERDESC);
		bufdesc.dwFlags			= DSBCAPS_CTRLPAN 
								| DSBCAPS_CTRLVOLUME ;
								//| DSBCAPS_LOCDEFER;	// ÀÌ°Ç ¿Ö ³Ö¾îµÐ°Å¿´À»±î. - -;
		bufdesc.dwBufferBytes	= child.cksize;
		bufdesc.lpwfxFormat		= &wavefmt;
		if( FAILED(m_pDS->CreateSoundBuffer(&bufdesc, &buffer, NULL)) )
		{
			DirectSoundFailed("Direct Sound CreateBuffer Error!");		
			__END_PROFILE("LoadWav")
			return NULL;
		}
		
		// write wave data to directsound buffer you just created
		void *write1 = 0; 
		void *write2 = 0;
		unsigned long length1;
		unsigned long length2;

		buffer->Lock(0, child.cksize, &write1, &length1, &write2, &length2, 0);

		__BEGIN_PROFILE("LoadWavRead")
		if(write1 > 0)
		{
			mmioRead(wavefile, (char*)write1, length1);
		}
		if(write2 > 0)
		{
			mmioRead(wavefile, (char*)write2, length2);
		}
		__END_PROFILE("LoadWavRead")

		buffer->Unlock(write1, length1, write2, length2);

		//close the wavefile, don't need it anymore, it's in the directsound buffer now
		mmioClose(wavefile, 0);

		__END_PROFILE("LoadWav")
		return buffer;
	}

	__END_PROFILE("LoadWav")
	return NULL;
}

//----------------------------------------------------------------------
// »ç¿îµå ¹öÆÛ Á¦°Å
//----------------------------------------------------------------------
void 
CDirectSound::Release(LPDIRECTSOUNDBUFFER buffer)
{
	if( m_bInit && buffer )
	{
		buffer->Release();
		buffer	= NULL;
	}
}

//----------------------------------------------------------------------
// »ç¿îµå ¹öÆÛ Create
//----------------------------------------------------------------------
LPDIRECTSOUNDBUFFER
CDirectSound::CreateBuffer(LPVOID sdat, DWORD size, DWORD caps, LPWAVEFORMATEX wfx)
{
	LPDIRECTSOUNDBUFFER dsb;
	DSBUFFERDESC dsbd;
	HRESULT rsl;
	LPVOID ptr1, ptr2;
	DWORD size1, size2;
	if (m_pDS == NULL)
		return NULL;

	// Sound Bufferì¬
	ZeroMemory(&dsbd, sizeof(dsbd));
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwFlags = caps;
	dsbd.dwBufferBytes = size;
	dsbd.lpwfxFormat = wfx;
	if ((rsl = m_pDS->CreateSoundBuffer(&dsbd, &dsb, NULL)) != DS_OK)					goto Exit;
	if (sdat == NULL) return dsb;

	// Soundƒf[ƒ^“]‘—
	if ((rsl = dsb->Lock(0, size, &ptr1, &size1, &ptr2, &size2, 0)) != DS_OK)			goto Exit;
	CopyMemory(ptr1, sdat, size1);
	if (size2 > 0)
		CopyMemory(ptr2, (LPBYTE)sdat + size1, size2);
	if ((rsl = dsb->Unlock(ptr1, size1, ptr2, size2)) != DS_OK)							goto Exit;
	return dsb;

Exit:
//	if (FAILED(rsl)) DispDebugMsg("dslib: %s", GetDXErrorMsg(rsl));
	return NULL;
}

//----------------------------------------------------------------------
// »ç¿îµå ¹öÆÛ º¹»ç
//----------------------------------------------------------------------
LPDIRECTSOUNDBUFFER	
CDirectSound::DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER srcbuffer, bool bAutoRelease)
{
	__BEGIN_PROFILE("DuplicateSoundBuffer")

	if (m_bInit)
	{
		LPDIRECTSOUNDBUFFER buffer;

		if( srcbuffer )
		{
			if( FAILED(m_pDS->DuplicateSoundBuffer(srcbuffer, &buffer)) )
			{
				//DirectSoundFailed("Direct Sound DuplicateBuffer Error!");			
				__END_PROFILE("DuplicateSoundBuffer")
				return NULL;
			}
		}
		else
		{
			DirectSoundFailed("Direct Sound DuplicateBuffer Error!!");		
			__END_PROFILE("DuplicateSoundBuffer")
			return NULL;
		}

		// Á¤»óÀûÀ¸·Î DuplicateµÈ °æ¿ì..
		// DuplicateµÈ buffer¸¦ ±â¾ïÇØµÐ´Ù. --> ³ªÁß¿¡ releaseÇÏ±â À§ÇØ¼­...
		
		// 2001.8.20 ÁÖ¼®Ã³¸®
		if (bAutoRelease)
		{
			m_listDuplicatedBuffer.push_back( buffer );
		}

		__END_PROFILE("DuplicateSoundBuffer")
		return buffer;
	}

	__END_PROFILE("DuplicateSoundBuffer")
	return NULL;
}



//----------------------------------------------------------------------
// »ç¿îµå ÇÃ·¹ÀÌ(Ã³À½ºÎÅÍ Ç×»ó ´Ù½Ã ÇÃ·¹ÀÌ)
//----------------------------------------------------------------------
bool 
CDirectSound::NewPlay(LPDIRECTSOUNDBUFFER buffer, bool loop)
{
	if (m_bInit && !m_bMute)
	{
		DWORD	dwFlag;

		if( buffer )
		{
			switch(loop)
			{
			case true:
				dwFlag = DSBPLAY_LOOPING;
				break;
			case false:
				dwFlag = 0;
				break;
			}
		
			buffer->SetCurrentPosition(0);
			buffer->Play(0, 0, dwFlag);
		}
		else
		{
			DirectSoundFailed("Direct Sound NewPlay Error!");
			return false;
		}

		return true;
	}

	return false;
}


//----------------------------------------------------------------------
// Is Play
//----------------------------------------------------------------------
// ¿¬ÁÖ ÁßÀÎ°¡?
//----------------------------------------------------------------------
bool 
CDirectSound::IsPlay(LPDIRECTSOUNDBUFFER buffer) const
{
	if (m_bInit && !m_bMute)
	{
		DWORD	status;
	
		if ( buffer )
		{		
			buffer->GetStatus(&status);

			if (status & DSBSTATUS_BUFFERLOST)
			{
				buffer->Restore();
			}

			if (status & DSBSTATUS_PLAYING)
			{
				return true; 
			}			
		}		
	}

	return false;
}

//----------------------------------------------------------------------
// »ç¿îµå ÇÃ·¹ÀÌ(³¡±îÁö ¸ðµÎ ÇÃ·¹ÀÌµÉ¶§±îÁö ±â´Ù¸²)
//----------------------------------------------------------------------
bool 
CDirectSound::Play(LPDIRECTSOUNDBUFFER buffer, bool loop, bool bDuplicate)
{
	if (m_bInit && !m_bMute)
	{

		DWORD	status;
		DWORD	dwFlag;

		if( buffer )
		{
			//buffer->Restore();

			switch(loop)
			{
			case true:
				dwFlag = DSBPLAY_LOOPING;
				break;
			case false:
				dwFlag = 0;
				break;
			}

			buffer->GetStatus(&status);

			if (status & DSBSTATUS_BUFFERLOST)
			{
				buffer->Restore();
			}

			// ÀÌ¹Ì ¿¬ÁÖÁßÀÌ¸é.. »õ·Î¿î Buffer¸¦ ¸¸µé¾î¼­ play
			// 2001.8.20 - ±×³É ´Ù½Ã play
			// ´Ù½ÃÇÑ¹ø µµÀü.. - -; 2001.8.28
			///*
			if( bDuplicate && 
				(status & DSBSTATUS_PLAYING))
			{				
				//ReleaseTerminatedDuplicateBuffer();

				LPDIRECTSOUNDBUFFER newBuffer = DuplicateSoundBuffer(buffer);

				if (newBuffer)
				{
					newBuffer->SetCurrentPosition(0);
					newBuffer->Play(0, 0, dwFlag);
					//Play(newBuffer, loop);				
				}
				else
				{					
					buffer->SetCurrentPosition(0);
					buffer->Play(0, 0, dwFlag);
				}
			}
			else
			//*/
			{				
				buffer->SetCurrentPosition(0);
				buffer->Play(0, 0, dwFlag);
			}
		}
		else
		{
			DirectSoundFailed("Direct Sound Play Error!");
			return false;
		}

		return true;
	}

	return false;
}


//----------------------------------------------------------------------
// »ç¿îµå ¸ØÃã
//----------------------------------------------------------------------
bool 
CDirectSound::Stop(LPDIRECTSOUNDBUFFER buffer)
{
	if (m_bInit)
	{
		if( buffer )
		{
			buffer->Stop();
		}
		else
		{
			DirectSoundFailed("Direct Sound Stop Error!");
			return false;
		}

		return true;
	}

	return false;
}


//----------------------------------------------------------------------
// ÁÖÆÄ¼ö ¿Ã¸®±â
//----------------------------------------------------------------------
bool 
CDirectSound::AddFrequency(LPDIRECTSOUNDBUFFER buffer, int step)
{
	if (m_bInit)
	{
		DWORD	freq;

		if( buffer )
		{
			buffer->GetFrequency(&freq);
			freq = freq + step;	
			if( freq > DSBFREQUENCY_MAX ) freq = DSBFREQUENCY_MAX;
			buffer->SetFrequency(freq);
		}
		else
		{
			DirectSoundFailed("Direct Sound AddFrequency Error!");
			return false;
		}

		return true;
	}
	return false;
}


//----------------------------------------------------------------------
// ÁÖÆÄ¼ö ³»¸®±â
//----------------------------------------------------------------------
bool 
CDirectSound::SubFrequency(LPDIRECTSOUNDBUFFER buffer, int step)
{
	if (m_bInit)
	{
		DWORD	freq;

		if( buffer )
		{
			buffer->GetFrequency(&freq);
			freq = freq - step;
			if( freq < DSBFREQUENCY_MIN ) freq = DSBFREQUENCY_MAX;
			buffer->SetFrequency(freq);
		}
		else
		{
			DirectSoundFailed("Direct Sound SubFrequency Error!");
			return false;
		}

		return true;

	}

	return false;
}

//----------------------------------------------------------------------
// Set Max Volume
//----------------------------------------------------------------------
bool 
CDirectSound::SetMaxVolume(LPDIRECTSOUNDBUFFER buffer)
{
	if (m_bInit)
	{
		if( buffer )
		{
			buffer->SetVolume(m_MaxVolume);	
		}
		else
		{
			DirectSoundFailed("Direct Sound SetVolume Error!");
			return false;
		}

		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// º¼·ý ³ôÀÓ
//----------------------------------------------------------------------
bool 
CDirectSound::AddVolume(LPDIRECTSOUNDBUFFER buffer, int step)
{
	if (m_bInit)
	{
		LONG	volume;

		if( buffer )
		{
			buffer->GetVolume(&volume);
			volume = volume + step;
			if( volume > m_MaxVolume ) volume = m_MaxVolume;
			buffer->SetVolume(volume);	
		}
		else
		{
			DirectSoundFailed("Direct Sound AddVolume Error!");
			return false;
		}

		return true;
	}

	return false;
}


//----------------------------------------------------------------------
// º¼·ý ³»¸²				
//----------------------------------------------------------------------
bool 
CDirectSound::SubVolume(LPDIRECTSOUNDBUFFER buffer, int step)
{
	if (m_bInit)
	{
		LONG	volume;

		if( buffer )
		{
			//ESULT hr = 
				buffer->GetVolume(&volume);

				/*
			if (hr==DS_OK)
			{
				int a =0 ;
			}
			else if (hr==DSERR_CONTROLUNAVAIL)
			{
				int a = 0;
			}
			else if (hr==DSERR_INVALIDPARAM)
			{
				int a = 0;
			}
			else if (hr==DSERR_PRIOLEVELNEEDED)
			{
				int a = 0;
			}
			*/

			volume = volume - step;
			if( volume < DSBVOLUME_MIN ) volume = DSBVOLUME_MIN;
			buffer->SetVolume(volume);	
		}
		else
		{
			DirectSoundFailed("Direct Sound SubVolume Error!");
			return false;
		}

		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// º¼·ý ³»¸²				
//----------------------------------------------------------------------
bool 
CDirectSound::SubVolumeFromMax(LPDIRECTSOUNDBUFFER buffer, int step)
{
	if (m_bInit)
	{
		if( buffer )
		{
			LONG volume = m_MaxVolume - step;
			if( volume < DSBVOLUME_MIN ) volume = DSBVOLUME_MIN;
			buffer->SetVolume(volume);	
		}
		else
		{
			DirectSoundFailed("Direct Sound SubVolume Error!");
			return false;
		}

		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// Set Volume Limit
//----------------------------------------------------------------------
void					
CDirectSound::SetVolumeLimit(LONG volume)
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

//----------------------------------------------------------------------
// ¿À¸¥ÂÊ ÆÒ
//----------------------------------------------------------------------
bool 
CDirectSound::RightPan(LPDIRECTSOUNDBUFFER buffer, int step)
{
	if (m_bInit)
	{
		LONG	pan;

		if( buffer )
		{
			buffer->GetPan(&pan);
			pan = pan + step;
			if( pan > DSBPAN_RIGHT ) pan = DSBPAN_RIGHT;
			buffer->SetPan(pan);	
		}
		else
		{
			DirectSoundFailed("Direct Sound RightPan error!");
			return false;
		}

		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// ¿ÞÂÊ ÆÒ
//----------------------------------------------------------------------
bool 
CDirectSound::LeftPan(LPDIRECTSOUNDBUFFER buffer, int step)
{
	if (m_bInit)
	{
		LONG	pan;

		if( buffer )
		{
			buffer->GetPan(&pan);
			pan = pan - step;
			if( pan < DSBPAN_LEFT ) pan = DSBPAN_LEFT;
			buffer->SetPan(pan);	
		}
		else
		{
			DirectSoundFailed("Direct Sound LeftPan error!");
			return false;
		}

		return true;
	}	
	
	return false;
}

//----------------------------------------------------------------------
// °¡¿îµ¥ ÆÒ
//----------------------------------------------------------------------
bool 
CDirectSound::CenterPan(LPDIRECTSOUNDBUFFER buffer)
{
	if (m_bInit)
	{
		if( buffer )
		{
			buffer->SetPan(DSBPAN_CENTER);	
		}
		else
		{
			DirectSoundFailed("Direct Sound CenterPan error!");
			return false;
		}

		return true;
	}

	return false;
}		

//----------------------------------------------------------------------
// ¿À¸¥ÂÊ ÆÒ
//----------------------------------------------------------------------
bool 
CDirectSound::CenterToRightPan(LPDIRECTSOUNDBUFFER buffer, int step)
{
	if (m_bInit)
	{
		LONG	pan;

		if( buffer )
		{
			buffer->SetPan(DSBPAN_CENTER);
			pan = step;
			if( pan > DSBPAN_RIGHT ) pan = DSBPAN_RIGHT;
			buffer->SetPan(pan);	
		}
		else
		{
			DirectSoundFailed("Direct Sound RightPan error!");
			return false;
		}

		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// ¿ÞÂÊ ÆÒ
//----------------------------------------------------------------------
bool 
CDirectSound::CenterToLeftPan(LPDIRECTSOUNDBUFFER buffer, int step)
{
	if (m_bInit)
	{
		LONG	pan;

		if( buffer )
		{
			buffer->SetPan(DSBPAN_CENTER);
			pan = - step;
			if( pan < DSBPAN_LEFT ) pan = DSBPAN_LEFT;
			buffer->SetPan(pan);	
		}
		else
		{
			DirectSoundFailed("Direct Sound LeftPan error!");
			return false;
		}

		return true;
	}	
	
	return false;
}	

//----------------------------------------------------------------------
// ¿À¸¥ÂÊ ÆÒ
//----------------------------------------------------------------------
bool 
CDirectSound::ChangePan(LPDIRECTSOUNDBUFFER buffer, int pan)
{
	if (m_bInit)
	{
		if( buffer )
		{		
			if( pan < DSBPAN_LEFT ) pan = DSBPAN_LEFT;
			else if( pan > DSBPAN_RIGHT ) pan = DSBPAN_RIGHT;
			buffer->SetPan(pan);	
		}
		else
		{
			DirectSoundFailed("Direct Sound ChangePan error!");
			return false;
		}

		return true;
	}

	return false;
}
