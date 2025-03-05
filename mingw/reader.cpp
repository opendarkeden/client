#include "DirectXlib_PCH.h"
#include "reader.h"
#include "common.h"
#include <wtypes.h>
#include "mp3.h"
#include <sys/types.h>
#include <sys/stat.h>


static FILE*	MP3File = NULL;
static int		FileSize = 0 ;
static UINT		nSpare = 0 ;
static BYTE		prvVal = 0 ;
static char		MP3FileName[128] ;

int	Open(LPSTR pathname)
{
	if ( MP3File != NULL ) fclose (MP3File) ;
	MP3File = fopen(pathname, "rb") ;
	if ( MP3File == NULL ) return -1 ;

	strcpy (MP3FileName, pathname) ;
	struct _stat buf;
	_stat( MP3FileName, &buf );
	FileSize = buf.st_size ;

	nSpare = 0 ;
	prvVal = 0 ;
	return 0 ;
}

int GetCurOffset()
{
	return ftell(MP3File) ;
}

int Close()
{
	if ( MP3File != NULL ) fclose (MP3File) ;
	MP3File = NULL ;
	return 0 ;
}

int GetFileSize()
{
	return FileSize ;
}

//***************************************************************************
// Func :	ReadHeaderPart
//---------------------------------------------------------------------------
// Desc :	MP3 파일의 헤더부를 읽어들인다.
// Params :
// Return :
//===========================================================================
int ReadHeader(UINT *headerStr) 
{
	BOOL	result, sync ;

	// 헤더부분을 찾을 때까지 검색한다.
	do 
	{
		if ( fread( headerStr, sizeof(UINT), 1, MP3File) != 1)
			return -1 ;
		sync = ((*headerStr & 0xf8ff) == 0xf8ff) ;

		if ( !sync)
			result = fseek(MP3File, -3, SEEK_CUR) ;
	} while (!sync && result) ;

	if (!result)
		return -1 ;

	BYTE *pByte = (BYTE*) headerStr ;
	*headerStr = BYTES2UINT( pByte[0], pByte[1], pByte[2], pByte[3] ) ;
	return 0 ;
}

// BYTE 단위로 읽어들인다.
int ReadBytes(BYTE* buf, UINT bytes)
{
	if ( fread ( buf, sizeof(BYTE), bytes, MP3File) != bytes )
		return -1 ;
	return 0 ;
}

// WORD 단위로 읽어들인다.
int ReadWords(WORD* buf, UINT words)
{
	if ( fread ( buf, sizeof(WORD), words, MP3File) != words )
		return -1 ;
	return 0 ;
}

// DWORD 단위로 읽어들인다.
int ReadDwords(DWORD* buf, UINT dwords)
{
	if ( fread ( buf, sizeof(DWORD), dwords, MP3File) != dwords )
		return -1 ;
	return 0 ;
}


UINT ReadBitsFromByte (BYTE byte, UINT &spare, UINT bits)
{
	UINT retVal = 0 ;
	UINT rBits = bits ;
	UINT sBit  = 0x80 >> (8 - spare ) ;

	if ( bits == 0 ) return 0 ;
	
	for ( ; rBits-- ; sBit >>= 1 )
	{
		if ( sBit & byte ) retVal |= 1 ;
		retVal <<= 1 ;
	}
	retVal >>= 1 ;
	spare -= bits ;
	return retVal ;
}

//***************************************************************************
// Func :	 ReadBits 
//---------------------------------------------------------------------------
// Desc :	 bit 단위로 읽어들여 UINT형으로 리턴시킨다.
// Params :  읽어들일 비트 수 
// Return :  결과 값 
//===========================================================================
UINT ReadBits(UINT bits)
{
	UINT retVal = 0 ;

	if ( bits > 32 ) return -1 ;	// 32 bit를 초과하는 것은 허용하지 않는다.

	if ( nSpare >= bits )
	{
		UINT rBits = bits ;
		UINT sBit  = 0x80 >> (8 - nSpare ) ;

		for ( ; rBits-- ; sBit >>= 1 )
		{
			if ( sBit & prvVal ) retVal |= 1 ;
			retVal <<= 1 ;
		}
		retVal >>= 1 ;
		nSpare -= bits ;
	}
	else
	{
		UINT rest = bits - nSpare ;
		UINT rBytes = (rest + 7) / 8  ;	// 더 읽어 들여야 하는 바이트 수를 계산한다.
		
		BYTE byte[4] ;
		ReadBytes( byte, rBytes ) ;
		
		retVal = ReadBitsFromByte (prvVal, nSpare, nSpare) ;
		
		for ( UINT i = 0 ; i < rest / 8 ; i++ )
		{
			retVal <<= 8 ;
			retVal |= byte[i] ;
		}

		if ( rBytes != rest/8 )
		{
			nSpare = 8 ;
			prvVal = byte[rBytes-1] ;

			UINT rBits = rest - (rest/8)*8 ;
			retVal <<= rBits ;
			retVal |= ReadBitsFromByte(prvVal, nSpare, rBits) ;
		}
	}
	return retVal ;
}


//------------------------------------------------
// Mono		: 126 bits ( = 17 bytes )
// Stereo	: 256 bits ( = 32 bytes ) 
//--------------------------------------------------
int ReadSideInfo(UINT channels, side_info_t *side_info)
{
	int ch, gr;
	
	side_info->main_data_begin = ReadBits(9);
	
	if (channels == 1)
		side_info->private_bits = ReadBits(5);
	else 
		side_info->private_bits = ReadBits(3);
	
	for (ch=0; (unsigned int)ch<channels; ch++)
	{
		side_info->ch[ch].scfsi[0] = ReadBits(1);
		side_info->ch[ch].scfsi[1] = ReadBits(1);
		side_info->ch[ch].scfsi[2] = ReadBits(1);
		side_info->ch[ch].scfsi[3] = ReadBits(1);
	}
	
	for (gr=0; gr<2; gr++)
	{
		for (ch=0; (unsigned int)ch<channels; ch++) 
		{
			side_info->ch[ch].gr[gr].part2_3_length = ReadBits(12);
			side_info->ch[ch].gr[gr].big_values = ReadBits(9);
			side_info->ch[ch].gr[gr].global_gain = ReadBits(8);
			side_info->ch[ch].gr[gr].scalefac_compress = ReadBits(4);
			side_info->ch[ch].gr[gr].window_switching_flag = ReadBits(1);
			
			if (side_info->ch[ch].gr[gr].window_switching_flag)
			{
				side_info->ch[ch].gr[gr].block_type = ReadBits(2);
				side_info->ch[ch].gr[gr].mixed_block_flag = ReadBits(1);
				
				side_info->ch[ch].gr[gr].table_select[0] = ReadBits(5);
				side_info->ch[ch].gr[gr].table_select[1] = ReadBits(5);
				
				side_info->ch[ch].gr[gr].subblock_gain[0] = ReadBits(3);
				side_info->ch[ch].gr[gr].subblock_gain[1] = ReadBits(3);
				side_info->ch[ch].gr[gr].subblock_gain[2] = ReadBits(3);
				
				/* Set region_count parameters since they are implicit in this case. */
				
				if (side_info->ch[ch].gr[gr].block_type == 0)
				{
				/*					printf("Side info bad: block_type == 0 in split block.\n");
					exit(0); */
					return -1;
				} else if (side_info->ch[ch].gr[gr].block_type == 2
					&& side_info->ch[ch].gr[gr].mixed_block_flag == 0)
					side_info->ch[ch].gr[gr].region0_count = 8; /* MI 9; */
				else side_info->ch[ch].gr[gr].region0_count = 7; /* MI 8; */
				side_info->ch[ch].gr[gr].region1_count = 20 -
					side_info->ch[ch].gr[gr].region0_count;
			}
			else
			{
				
				side_info->ch[ch].gr[gr].table_select[0] = ReadBits(5);
				side_info->ch[ch].gr[gr].table_select[1] = ReadBits(5);
				side_info->ch[ch].gr[gr].table_select[2] = ReadBits(5);
				side_info->ch[ch].gr[gr].region0_count = ReadBits(4);
				side_info->ch[ch].gr[gr].region1_count = ReadBits(3);
				side_info->ch[ch].gr[gr].block_type = 0;
			}
			side_info->ch[ch].gr[gr].preflag = ReadBits(1);
			side_info->ch[ch].gr[gr].scalefac_scale = ReadBits(1);
			side_info->ch[ch].gr[gr].count1table_select = ReadBits(1);
		}
	}
	return 0;
}

int SeekFrame (MP3 *mp3, int FrameNumber)
{
	if(FrameNumber >= mp3->maxframes)
		return -1;

	SetOffset(mp3->offset[FrameNumber]) ;
	return 0 ;
}

int GetMaxFrames(MP3 *mp3) 
{
	return ( FileSize / MP3_Calc_FrameSize(mp3) + 4 - mp3->Header.ProtectionBit ) ;
}

int GetMinFrames(MP3 *mp3) 
{
	return ( FileSize / MP3_Calc_FrameSize(mp3) + 5 - mp3->Header.ProtectionBit )  ;
}

int SetOffset(int offset)
{
	fseek ( MP3File, offset, SEEK_SET ) ;
	return 0 ;
}