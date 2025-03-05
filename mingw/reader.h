#ifndef _MP3_READER_H_
#define _MP3_READER_H_

#include <wtypes.h>
#include "mp3.h"
#include <stdio.h>

extern int Open(LPSTR PathName) ;						// MP3 파일을 연다.
extern int Close() ;									// MP3 파일을 닫는다.
extern int ReadHeader(UINT *headerStr) ;				// 헤더부를 읽어들인다.	
extern int ReadBytes(BYTE* buf, UINT bytes) ;			// 1 바이트 단위로 읽어들인다.
extern int ReadWords(WORD* buf, UINT words) ;			// 2 바이트 단위로 읽어들인다.
extern int ReadDwords(DWORD* buf, UINT dwords) ;		// 4 바이트 단위로 읽어들인다.
extern UINT ReadBits(UINT bits) ;
extern int ReadSideInfo(UINT channel, side_info_t *sideInfo) ;	// 
extern int GetCurOffset() ;				// 현재의 offset을 얻어 온다.
extern int GetFileSize() ;				// 파일 사이즈를 얻어온다.
extern int SetOffset(int offset) ;
extern int GetMaxFrames(MP3 *mp3) ;
extern int GetMinFrames(MP3 *mp3) ;

#endif //_MP3_READER_H_
