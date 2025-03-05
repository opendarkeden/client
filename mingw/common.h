#ifndef _MP3_COMMON_H_
#define _MP3_COMMON_H_

//#include "l3types.h"

enum e_mode { stereo, joint_stereo, dual_channel, single_channel  };
enum e_sample_frequency { fourtyfour_point_one, fourtyeight, thirtytwo };
enum e_channels { both, left, right };

typedef unsigned int        UINT;
typedef unsigned char BYTE;

inline  UINT BYTES2UINT( BYTE a, BYTE b, BYTE c, BYTE d )
{
	return ((UINT)a << 24)&0xff000000 | ((UINT)b << 16)&0x00ff0000 | ((UINT)c << 8) &0x0000ff00 | ((UINT)d & 0x000000ff) ;
}


#define MAX(a,b)	( (a) > (b) ? (a) : (b))
#define MIN(a,b)	( (a) < (b) ? (a) : (b))
#define ABS(x)		( ((x) > 0) ? (x) : (-(x)) )


#endif //_MP3_COMMON_H_