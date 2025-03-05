#ifndef _SOUNDBUF_H_
#define _SOUNDBUF_H_

#include <wtypes.h>
#include "mp3.h"
//#include "dslib.h"
#include <dsound.h>


struct MP3 ;

struct SOUNDBUF
{
	char		databuf[0x1200] ;		// 사운드 서피스에 집어넣을 데이타의 버퍼 
	int			nWritten ;				// 쓴 바이트 수 
	int			lp ;					// 왼쪽 채널 인덱스 (모노일 경우 전 데이타 인덱스)
	int			rp ;					// 오른쪽 채널 인덱스 
	int			channels ;				// 채널 수 ( 1 or 2 ) 
	int			nFrames ;				// 버퍼링 수
	int			nPushedFrames ;			// 현재 버퍼링된 수 
	int			bufSize ;				// 버퍼의 사이즈 
	int			curBuffer ;				// 현재 버퍼의 ID 
	LPDIRECTSOUNDBUFFER		dsPlayBuf ;				// 연주용 버퍼 
	LPDIRECTSOUNDBUFFER		dsWriteBuf ;			// 쓰기용 버퍼 
	int			readyBufs ;				// 데이타가 담긴 버퍼의 수 
	int			offWrite ;				// 사운드 버퍼의 write 오프셋
	BOOL		bPlaying ;				// 현재 연주중??
} ;

typedef SOUNDBUF	*LPSOUNDBUF ;


LPSOUNDBUF CreateSoundBuf ( MP3 *mp3, int nFrames, DSBPOSITIONNOTIFY[], int n  ) ;
LPSOUNDBUF CreateSoundBuf ( MP3 *mp3, int nFrames) ;
int AppendData ( int ch, LPSOUNDBUF lpsb, int data ) ;
int Destroy ( LPSOUNDBUF lpsb ) ;
int WriteTo ( LPSOUNDBUF lpsb , BYTE* arr) ;
extern int OutputData (LPSOUNDBUF lpsb ) ;
#endif // _SOUNDBUF_H_