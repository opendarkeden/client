#ifndef _MP3_HEADER_H_
#define _MP3_HEADER_H_

#include "common.h"
#include <wtypes.h>

struct MP3Header
{
	e_mode	Mode;										// 연주 방식 
	UINT	ModeExt ;									// 확장 연주 방식 
	UINT	ProtectionBit ;								
	UINT	BitrateIndex ;
	UINT	padding_bit ;
	BOOL	CopyRight ;
	BOOL	Original ;
	UINT	Channel_Bitrate ;
	UINT	NumOfSubbands ;
	UINT	nSlots ;
	UINT	IntensityStereoBound ;
	e_sample_frequency		Sample_Frequency ;
} ;


int MP3_ReadHeader(MP3Header* header) ;

#endif _MP3_HEADER_H_