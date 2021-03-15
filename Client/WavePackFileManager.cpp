//--------------------------------------------------------------------------
// WavePackFileManager.cpp
//--------------------------------------------------------------------------
#include "Client_PCH.h"
#include "WavePackFileManager.h"
#include <MMSystem.h>
#include "CDirectSound.h"
#include "Profiler.h"

//--------------------------------------------------------------------------
// Global
//--------------------------------------------------------------------------
WavePackFileManager*	g_pWavePackFileManager = NULL;

//--------------------------------------------------------------------------
//
//					WavePackFileInfo
//
//--------------------------------------------------------------------------
bool		
WavePackFileInfo::SaveToFileData(ofstream& file)
{
	if (m_Filename.c_str()==NULL)
	{
		return false;
	}

	const int wavefmtSize = sizeof(WAVEFORMATEX);

	// open a wav file
	HMMIO wavefile;
	char filename[256];
	strcpy(filename, m_Filename.c_str());
	wavefile = mmioOpen(filename, 0, MMIO_READ|MMIO_ALLOCBUF);
	if(wavefile == NULL)
	{
		//DirectSoundFailed("Direct Sound mmioOpen Error!");		
		return false;
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
	mmioRead(wavefile, (char*)&wavefmt, wavefmtSize);
	if(wavefmt.wFormatTag != WAVE_FORMAT_PCM)
	{
		return false;
	}

	// find the wave data chunk
	mmioAscend(wavefile, &child, 0);
	child.ckid = mmioFOURCC('d','a','t','a');
	mmioDescend(wavefile, &child, &parent, MMIO_FINDCHUNK);

	DWORD cksize = child.cksize;

	char* pBuffer = new char [cksize];
	mmioRead(wavefile, (char*)pBuffer, cksize);
	mmioClose(wavefile, 0);

	//--------------------------------------------------------------
	// file에 저장한다.
	//--------------------------------------------------------------
	file.write((const char*)&cksize, 4);
	file.write((const char*)&wavefmt, wavefmtSize);
	file.write((const char*)pBuffer, cksize);

	delete [] pBuffer;
	
	return true;
}

//--------------------------------------------------------------------------
// Load From File Data
//--------------------------------------------------------------------------
LPDIRECTSOUNDBUFFER		
WavePackFileInfo::LoadFromFileData(ifstream& file)
{
	DWORD cksize;
	WAVEFORMATEX wavefmt;
	const int wavefmtSize = sizeof(WAVEFORMATEX);

	file.read((char*)&cksize, 4);
	file.read((char*)&wavefmt, wavefmtSize);
	
	// create a directsound buffer to hold wave data
	LPDIRECTSOUNDBUFFER buffer;
	DSBUFFERDESC bufdesc;
	memset(&bufdesc, 0, sizeof(DSBUFFERDESC));
	bufdesc.dwSize			= sizeof(DSBUFFERDESC);
	bufdesc.dwFlags			= DSBCAPS_CTRLPAN 
							| DSBCAPS_CTRLVOLUME ;
							//| DSBCAPS_LOCDEFER;	// 이건 왜 넣어둔거였을까. - -;
	bufdesc.dwBufferBytes	= cksize;
	bufdesc.lpwfxFormat		= &wavefmt;

	if( FAILED(g_DXSound.GetDS()->CreateSoundBuffer(&bufdesc, &buffer, NULL)) )
	{
		return NULL;
	}
	
	// write wave data to directsound buffer you just created
	void *write1 = 0; 
	void *write2 = 0;
	unsigned long length1;
	unsigned long length2;

	buffer->Lock(0, cksize, &write1, &length1, &write2, &length2, 0);
	if(write1 > 0)
	{
		file.read((char*)write1, length1);
	}
	if(write2 > 0)
	{
		file.read((char*)write2, length2);
	}
	buffer->Unlock(write1, length1, write2, length2);
	
	return buffer;
}

//--------------------------------------------------------------------------
//
//					WavePackFileManager
//
//--------------------------------------------------------------------------
WavePackFileManager::WavePackFileManager()
{
}

WavePackFileManager::~WavePackFileManager()
{
}

//--------------------------------------------------------------------------
// Load From File Data
//--------------------------------------------------------------------------
LPDIRECTSOUNDBUFFER		
WavePackFileManager::LoadFromFileData(TYPE_SOUNDID id)
{
	__BEGIN_PROFILE("WavePackFileManager-Load")

	if (m_DataFilename.c_str()==NULL)
	{
		__END_PROFILE("WavePackFileManager-Load")
		return NULL;
	}

	WavePackFileInfo* pInfo = GetInfo(id);

	if (pInfo!=NULL)
	{
		ifstream file(m_DataFilename.c_str(), std::ios::binary);
		file.seekg( pInfo->GetFilePosition() );
		
		LPDIRECTSOUNDBUFFER pBuffer = pInfo->LoadFromFileData(file);

		file.close();

		__END_PROFILE("WavePackFileManager-Load")

		return pBuffer;
	}

	__END_PROFILE("WavePackFileManager-Load")
	return NULL;
}
