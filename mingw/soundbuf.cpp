#include "DirectXlib_PCH.h"
#include <wtypes.h>
#include <memory.h>
#include "soundbuf.h"
#include "mp3.h"
//#include "dslib.h"
//#include "debug.h"
#include "DXLib\CDirectSound.h"

const UINT frequencies[3] = { 44100, 48000, 32000 };

int Reset(LPSOUNDBUF lpsb)
{
	lpsb->lp = 0 ;
	lpsb->rp = 2 ;
	lpsb->nWritten = 0 ;
	return 0 ;
}

LPDIRECTSOUNDNOTIFY		DSNotify = NULL;
LPSOUNDBUF CreateSoundBuf ( MP3 *mp3, int nFrames)
{
	static const int samplefrequencies[3] = { 44100, 48000, 32000 };
	if ( nFrames <= 0)  return NULL ;

	LPSOUNDBUF	lpsb = new SOUNDBUF ;
	memset(lpsb, 0, sizeof(SOUNDBUF)) ;
	lpsb->channels = mp3->channels ;
	lpsb->curBuffer= 0 ;
	lpsb->nFrames = nFrames ;
	lpsb->nWritten	= 0 ;
	lpsb->bufSize = 0x1200 ;

	return lpsb ;
}


int AppendData (int  ch, LPSOUNDBUF lpsb, int value )
{
	/*
	static FILE *file1 = NULL ;
	static FILE *file2 = NULL ;

	if ( file1 == NULL)
		file1 = fopen("src1.wv", "wb") ;
	if ( file2 == NULL)
		file2 = fopen("src2.wv", "wb") ;
	*/

	if ( value != 0 )
	{
		value = value ;
	}
	if  ( ch == 0 )
	{
		lpsb->databuf[lpsb->lp]	= (char) (value & 0xff); lpsb->nWritten++ ;
		lpsb->databuf[lpsb->lp+1]	= (char) (value >> 8); lpsb->nWritten++ ;
		// fwrite(&lpsb->databuf[lpsb->lp], 1, 2, file1) ;
		lpsb->lp += lpsb->channels*2 ;
	}
	else if ( ch == 1 ) 
	{
		lpsb->databuf[lpsb->rp]	= (char) (value & 0xff); lpsb->nWritten++ ;
		lpsb->databuf[lpsb->rp+1]	= (char) (value >> 8); lpsb->nWritten++ ;
		// fwrite(&lpsb->databuf[lpsb->rp], 1, 2, file2) ;
		lpsb->rp += lpsb->channels*2 ;
	}
	
	return 0 ;
}

int AppendData ( int ch, LPSOUNDBUF lpsb, short value )
{
	lpsb->databuf[lpsb->nWritten++]	= (char) (value & 0xff);
	lpsb->databuf[lpsb->nWritten++]	= (char) (value >> 8);

	return 0 ;
}

int OutputData (LPSOUNDBUF lpsb)
{
	LPVOID	audioPtr1, audioPtr2 ;
	DWORD	audioBytes1, audioBytes2 ;

//	DispDebugMsg("디코딩 정보를 기록하려 합니다.") ;

Fill :
	while ( lpsb->bPlaying && lpsb->readyBufs == 1 ) Sleep(5) ;
	if ( lpsb->nPushedFrames < lpsb->nFrames )	// 버퍼가 여유가 있을 경우 채운다.
	{
		lpsb->dsWriteBuf->Lock( lpsb->offWrite, lpsb->bufSize, 
			(void**)&audioPtr1, &audioBytes1, (void**)&audioPtr2, &audioBytes2, 0 ) ;

		if ( audioBytes1 >= (DWORD)lpsb->bufSize )
		{
			memcpy ( audioPtr1, lpsb->databuf, lpsb->bufSize ) ;
		}
		else
		{
			memcpy ( audioPtr1, lpsb->databuf, audioBytes1 ) ;
			memcpy ( audioPtr2, lpsb->databuf + audioBytes1, lpsb->bufSize - audioBytes1 ) ;
		}

		lpsb->dsWriteBuf->Unlock( audioPtr1, audioBytes1, audioPtr2, lpsb->bufSize - audioBytes1) ;

		lpsb->nPushedFrames++ ;
		lpsb->offWrite += lpsb->bufSize ;
		lpsb->offWrite %= lpsb->bufSize*lpsb->nPushedFrames ;
	}
	else	// 버퍼가 차면 연주를 시작한다.
	{
		lpsb->readyBufs++ ;

		if ( lpsb->bPlaying == FALSE )
		{
			g_DXSound.Play(lpsb->dsPlayBuf, TRUE) ;
			lpsb->bPlaying = TRUE ;			
		}

		lpsb->offWrite = 0 ;
		lpsb->nPushedFrames = 0 ;

		goto Fill ; 
	}
//	DispDebugMsg("디코딩 정보를 기록했습니다.") ;

	Reset(lpsb) ;
	return 0 ;
}

int WriteTo ( LPSOUNDBUF lpsb , BYTE* arr)
{
	memcpy ( lpsb->databuf, arr, lpsb->nWritten ) ;
	return 0 ;
}

