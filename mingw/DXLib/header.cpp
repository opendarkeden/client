#include "header.h"
#include "reader.h"

//***************************************************************************
// Func :	MP3_ReadHeader			
//---------------------------------------------------------------------------
// Desc :	MP3의 헤더 부분을 읽어 온다.	
// Params : 리턴 버퍼
// Return : ErrCode ( 0 : OK )
//===========================================================================
int MP3_ReadHeader(MP3Header* header) 
{
	UINT headerStr ;

	ReadHeader(&headerStr) ;	//헤더 부분을 읽어 온다.(4byte)
	
	UINT Layer = 4 - (headerStr >> 17 ) & 3 ;	// MPEG Layer Version을 얻는다.

	if ( Layer != 3 )	// Layer Version 3이 아니면 에러 MP3만 지원한다.
		return -1 ;

	header->ProtectionBit = (headerStr >> 16) & 1 ;
	header->BitrateIndex = (headerStr >> 12) & 0xf ;
	header->Sample_Frequency = (e_sample_frequency) ((headerStr>>10) & 3) ;
	header->padding_bit = (headerStr >> 9 ) & 1 ;
	header->Mode = (e_mode)((headerStr >> 6) & 3) ;
	header->ModeExt = (headerStr >> 4) & 3 ;

	if ( header->Mode == joint_stereo)
		header->IntensityStereoBound = (header->ModeExt << 2) + 4 ;
	else
		header->IntensityStereoBound = 0 ;

	header->CopyRight = (BOOL)((headerStr>>3) & 1) ;
	header->Original = (BOOL)((headerStr>>2) & 1) ;


	header->Channel_Bitrate = header->BitrateIndex ;

	if ( header->Mode != single_channel )
	{
		if ( header->Channel_Bitrate == 4 )
			header->Channel_Bitrate = 1 ;
		else
			header->Channel_Bitrate -= 4 ;
	}

	if ( header->Channel_Bitrate == 1 || header->Channel_Bitrate == 2 )
	{
		if ( header->Sample_Frequency == thirtytwo )
			header->NumOfSubbands = 12 ;
		else
			header->NumOfSubbands = 8 ;
	}
	else
	{
		if ( header->Sample_Frequency == fourtyeight || (header->Channel_Bitrate >= 3 && header->Channel_Bitrate <= 5))
			header->NumOfSubbands = 27 ;
		else
			header->NumOfSubbands = 30 ;
	}

	if ( header->IntensityStereoBound > header->NumOfSubbands )
	{
		header->IntensityStereoBound = header->NumOfSubbands ;
	}
	return 0 ;
}
