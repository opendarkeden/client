#ifndef _MP3_H_
#define _MP3_H_
//#include "l3types.h"
#include "header.h"
#include "common.h"
//#include "dslib.h"
#include "bit_res.h"
#include <dsound.h>

typedef int	OFFSET ;

#define SSLIMIT 18
#define SBLIMIT 32

typedef	struct 
{
	UINT part2_3_length;
	UINT big_values;
	UINT global_gain;
	UINT scalefac_compress;
	UINT window_switching_flag;
	UINT block_type;
	UINT mixed_block_flag;
	UINT table_select[3];
	UINT subblock_gain[3];
	UINT region0_count;
	UINT region1_count;
	UINT preflag;
	UINT scalefac_scale;
	UINT count1table_select;
} gr_info_s;


typedef struct
{
	UINT main_data_begin;
	UINT private_bits;

	struct
	{
		UINT scfsi[4];
		gr_info_s gr[2];
	} ch[2];

} side_info_t;


typedef struct
{
	int l[23];            /* [cb] */
	int s[3][13];         /* [window][cb] */
} scalefac_t[2];

struct SynthesisFilter
{
  static const float d[512];
  float v1[512], v2[512];
  float *actual_v;			// v1 or v2
  UINT actual_write_pos;		// 0-15

  float	 samples[32];			// 32 new subband samples

  UINT channel;
  float	 scalefactor;
  UINT range_violations;
  float	 max_violation;
  UINT written_samples;
} ;

struct SOUNDBUF ;

struct MP3
{
	MP3Header		Header ;
	OFFSET			frameStart ;		// 프레임 데이타가 시작하는 Offset (SEEK_SET)
	OFFSET			part2Start ;		// Sample Data가 시작되는 Offset
	OFFSET			*offset ;
	int				maxframes ;
	int				curFrame ;			// 현재 연주하고 있는 프레임 Number
	e_channels		which_ch ;
	char			*Huffdec_path;
	SOUNDBUF		*soundbuf ;

	int is[SBLIMIT][SSLIMIT];
	float ro[2][SBLIMIT][SSLIMIT];
	float lr[2][SBLIMIT][SSLIMIT];
	float hybridIn[SBLIMIT][SSLIMIT];
	float re_hybridOut[SBLIMIT][SSLIMIT];
	float prevblck[2][SBLIMIT][SSLIMIT];

	Bit_Reserve		*br ;
	SynthesisFilter *filter1, *filter2;
	side_info_t si ;
	scalefac_t scalefac;

	UINT	channels;
} ;

enum PLAY_STATE { PS_NONE, PS_PLAYING, PS_END } ;

extern MP3* MP3_Load(LPSTR pathname) ;
extern void MP3_Close (MP3* mp3) ;
extern int	MP3_WriteToBuffer(MP3* mp3, int FrameNum, HANDLE event) ;
extern UINT MP3_Calc_FrameSize (MP3* mp3) ;
extern void get_scale_factors(MP3 *mp3, int ch, int gr) ;
extern int SeekFrame (MP3 *mp3, int FrameNumber) ;
extern void hufman_decode(MP3 *mp3, int ch, int gr) ;

extern void InitSynthesisFilter (SynthesisFilter *filter, UINT channelnumber, float factor) ;
extern void input_sample (SynthesisFilter *filter,float sample, UINT subbandnumber) ;
extern float seconds_played (SynthesisFilter *filter,UINT frequency) ;
extern void compute_new_v (SynthesisFilter* filter) ;
struct SOUNDBUF ;
extern void compute_pcm_samples (int ch, SynthesisFilter* filter, SOUNDBUF*) ;
extern void calculate_pcm_samples (int ch, SynthesisFilter *filter, SOUNDBUF*) ;
extern int MP3_Decode(MP3* mp3) ;




// MP3X cozy function
extern int MP3X_LoadMP3 (LPSTR pathname) ;
extern int MP3X_Streaming() ;
extern int MP3X_StartPlay () ;
extern void MP3X_Clear() ;
extern int MP3X_Close () ;

extern LPDIRECTSOUNDBUFFER	MP3_CreateDSBuffer(MP3* mp3) ;


#endif // _MP3_H_