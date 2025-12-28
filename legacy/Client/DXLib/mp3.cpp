#include <memory.h>
#include "mp3.h"
#include "reader.h"
#include "huffman.h"
#include "soundbuf.h"
#include "DXLib\CDirectSound.h"
#include "debuginfo.h"
#include <math.h>

#define	MP3BUFSIZE		1024

static BYTE	MP3_buffer[MP3BUFSIZE] ;

// 프레임 단위로 이동할 수 있도록 Offset을 만들어 둔다.
static int*	Offset = NULL ;

UINT MP3_Calc_FrameSize (MP3* mp3)
/* calculates framesize in bytes excluding header size */
{
	static const int bitrates[15] = 
	{
		0 , 32000, 40000, 48000, 56000, 64000, 80000,
		96000, 112000, 128000, 160000, 192000, 224000, 256000, 320000
	} ;
	
	static const int samplefrequencies[3] = { 44100, 48000, 32000 };
	
	UINT framesize;

	framesize = (144 * bitrates[mp3->Header.BitrateIndex]) / samplefrequencies[mp3->Header.Sample_Frequency];
		
	if ( mp3->Header.padding_bit ) framesize++;
			 
	mp3->Header.nSlots = framesize - ((mp3->Header.Mode == single_channel) ? 17 : 32) // side info size
									  -  (mp3->Header.ProtectionBit ? 0 : 2) 		  // CRC size
									  - 4; 								              // header size
	
	return(framesize - 4);		// subtract header size
}

UINT MaxFrames (MP3* mp3)
{
	return GetFileSize() / (MP3_Calc_FrameSize(mp3) + 4 - mp3->Header.padding_bit);
}

UINT MinFrames (MP3* mp3)
{
	return GetFileSize() / (MP3_Calc_FrameSize(mp3) +5 - mp3->Header.padding_bit);
}


int MP3_ReadFrame(MP3* mp3)
{
	DEBUG_ADD("[MP3_ReadFrame]");
	UINT size ;
	SetOffset(0) ;
	int maxframes = MaxFrames(mp3) ;
	mp3->maxframes = maxframes ;
	mp3->offset = new int[maxframes] ;

	DEBUG_ADD_FORMAT("[MP3_ReadFrame] maxframes %d", maxframes);
	for ( int i = 0 ; i < maxframes ; i++ )
	{
	DEBUG_ADD("[MP3_ReadFrame] 1");
		mp3->offset[i] = GetCurOffset() ;
	DEBUG_ADD("[MP3_ReadFrame] 2");
		if(MP3_ReadHeader(&mp3->Header) == -1)
		{
			DEBUG_ADD("[MP3_ReadFrame] MP3_ReadHeader(&mp3->Header) == -1");
			mp3->maxframes = i;
			return 0;
		}
	DEBUG_ADD("[MP3_ReadFrame] 3");
		size = MP3_Calc_FrameSize(mp3) ;
	DEBUG_ADD_FORMAT("[MP3_ReadFrame] 4 Calc_FrameSize %d", size);
		// 프레임 데이타 부분을 읽어버린다.
		if ( ReadBytes( MP3_buffer, size) < 0 )
		{
			mp3->maxframes = i;
			DEBUG_ADD("[MP3_ReadFrame] ReadBytes( MP3_buffer, size) < 0 return");
			return 0;
		}
	DEBUG_ADD("[MP3_ReadFrame] 5");

		// CRC 체크를 한다.
		if (!mp3->Header.ProtectionBit)
		{
			// CRC는 단순히 읽어들이기만 하자
			WORD CheckSum ;
	DEBUG_ADD("[MP3_ReadFrame] 6");
			ReadWords(&CheckSum, 1) ;
		}
	DEBUG_ADD("[MP3_ReadFrame] 7");
	}

	
	DEBUG_ADD("[MP3_ReadFrame] OK");
	return 0  ;
}

extern int Reset(LPSOUNDBUF lpsb) ;

LPDIRECTSOUNDBUFFER	MP3_CreateDSBuffer(MP3* mp3)
{
	int nFrames = MinFrames(mp3) ;
	int size = nFrames * 0x1200 ;

	DEBUG_ADD("MP3_CreateDSBuffer가 시작합니다.") ;

	BYTE *dataBuff = new BYTE[size] ;

	DEBUG_ADD("MP3_CreateDSBuffer MP3_ReadFrame") ;
	if( MP3_ReadFrame(mp3) == -1)
		return NULL;
	DEBUG_ADD("MP3_CreateDSBuffer MP3_ReadFrame OK") ;

	mp3->soundbuf = CreateSoundBuf(mp3, nFrames) ;
	DEBUG_ADD("사운드 버퍼를 생성했습니다.") ;

	LPSOUNDBUF lpsb = mp3->soundbuf ;
	DEBUG_ADD_FORMAT("nFrames : %d frame");

	for ( int i = 0 ; i < nFrames ; i++ )
	{
		DEBUG_ADD_FORMAT("%d번째 프레임 작업 시작", i ) ;
		if( SeekFrame(mp3, i) == -1)
		{
			DEBUG_ADD("SeekFrame return -1");
			return NULL;
		}
		DEBUG_ADD_FORMAT("%d번째 프레임 헤더 읽기", i ) ;
		if( MP3_ReadHeader(&mp3->Header) == -1)
		{
			DEBUG_ADD("MP3_ReadHeader return -1");
			return NULL;
		}
		DEBUG_ADD_FORMAT("%d번째 프레임 사이즈 정보 얻기", i ) ;
		MP3_Calc_FrameSize(mp3) ;	// 슬롯 수를 얻어오기 위해 
		DEBUG_ADD_FORMAT("%d번째 프레임 디코딩", i ) ;
		MP3_Decode(mp3) ;
		DEBUG_ADD_FORMAT("%d번째 프레임의 디코딩 정보를 dataBuff에 복사", i ) ;
		memcpy ( &dataBuff[i*0x1200], mp3->soundbuf->databuf, 0x1200 ) ;
		lpsb->nPushedFrames++ ;
		lpsb->offWrite += lpsb->bufSize ;
		lpsb->offWrite %= lpsb->bufSize*lpsb->nPushedFrames ;
		DEBUG_ADD_FORMAT("%d번째 프레임 Reset", i ) ;
		Reset(lpsb) ;
		DEBUG_ADD_FORMAT("%d번째 프레임 작업 종료", i ) ;
		mp3->curFrame++ ;
	}

	static const int samplefrequencies[3] = { 44100, 48000, 32000 };

	//@@@@@@@@@@@@@@@
	// 버퍼 생성 루틴
	
	WAVEFORMATEX wfx ;
	wfx.wFormatTag		= WAVE_FORMAT_PCM;
	wfx.wBitsPerSample	= 16 ;
	wfx.nChannels		= mp3->channels;
	wfx.nSamplesPerSec	= samplefrequencies[mp3->Header.Sample_Frequency];
	wfx.nBlockAlign		= (WORD) (mp3->channels<< 1);;
	wfx.nAvgBytesPerSec	= wfx.nSamplesPerSec * wfx.nBlockAlign;
	wfx.cbSize			= 0;

	LPDIRECTSOUNDBUFFER buf = g_DXSound.CreateBuffer ( dataBuff, size, DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME, &wfx) ;

	// DS_CreateBuffer() ;
	//@@@@@@@@@@@@@@@

	DEBUG_ADD("[MP3_CreateDSBuffer] MP3_Close");
	MP3_Close(mp3) ;
	DEBUG_ADD("[MP3_CreateDSBuffer] MP3_Close OK");
	if (Offset) { delete [] Offset ; Offset = NULL ; }

	if ( dataBuff ) { delete [] dataBuff; dataBuff = NULL  ;}

	return buf ;
}

MP3* MP3_Load (LPSTR pathname)
{
	if ( Open(pathname) < 0 ) return NULL ;
	MP3*	mp3 = new MP3 ;
	memset( mp3, 0, sizeof(MP3) ) ;

//	DispDebugMsg("MP3 파일 %s의 헤더 정보를 읽으려 합니다.", pathname ) ;
	if(	MP3_ReadHeader(&mp3->Header) == -1)
		return NULL;

	if( MP3_ReadFrame(mp3) == -1)
		return NULL;

	mp3->br = new Bit_Reserve() ;

	if (mp3->Header.Mode == single_channel)
	{ mp3->which_ch = left;}
	
	mp3->filter1 = new SynthesisFilter ;
	InitSynthesisFilter(mp3->filter1, 0, 32768.0) ;

	if (mp3->Header.Mode != single_channel && mp3->which_ch == both)
	{
		mp3->filter2 = new SynthesisFilter ;
		InitSynthesisFilter(mp3->filter2, 1, 32768.0) ;
	}

	for ( int i = 0 ; i < 2 ; i++) 
		for ( int j = 0 ; j < SBLIMIT ; j++ ) 
			for ( int k = 0 ; k < SSLIMIT ; k++ )
				mp3->prevblck[i][j][k] = 0.f ;

	e_mode mode = mp3->Header.Mode ;
	// channel수를 구한다.
	if (mode == single_channel )
	{ mp3->channels = 1 ; }
	else
	{ mp3->channels = 2 ; }

	return mp3 ;
}

extern void dequntize_sample(MP3 *mp3, float xr[SBLIMIT][SSLIMIT] , int ch, int gr) ;
extern void stereodecode(MP3 *mp3, int gr) ;
extern void reorder (MP3 *mp3, float xr[SBLIMIT][SSLIMIT], int ch, int gr) ;
extern void antialias(MP3* mp3, int ch, int gr) ;
void hybrid(MP3 *mp3, int ch, int gr, int sb) ;


int MP3_Decode(MP3* mp3)
{
//	if ( mp3 == NULL ) DispDebugMsg("MP3_Decode의 인자로 NULL값이 들어왔습니다.") ;
	MP3Header *header = &mp3->Header ;
	int		nSlots = header->nSlots ;
	int channel ;

	e_mode mode = mp3->Header.Mode ;
	// channel수를 구한다.
	if (mode == single_channel )
	{channel = 1 ; mp3->channels = 1 ;}
	else
	{channel = 2 ; mp3->channels = 2 ;}


	int offset = GetCurOffset() ;

	ReadSideInfo(channel, &mp3->si ) ;	// side info 를 읽어온다.

	Bit_Reserve*	br = mp3->br ;
	for ( ; nSlots > 0 ; nSlots-- )
	{	br->hputbuf(ReadBits(8)) ; }

	int main_data_end = br->hsstell() >> 3 ;		// of previous frame
	int flush_main ;

	if ( flush_main  = (br->hsstell() & 0x7))
	{
		br->hgetbits(8 - flush_main) ;
		main_data_end++ ;
	}

	int bytes_to_discard ;
	bytes_to_discard = mp3->frameStart - main_data_end - mp3->si.main_data_begin ;


	if ( main_data_end > 4096)
	{
		mp3->frameStart -= 4096 ;
		br->rewindNbytes(4096) ;
	}


	mp3->frameStart += mp3->Header.nSlots ;	


	if ( bytes_to_discard < 0 )
		return -1;

	//초과된 데이타를 방출한다.
	for ( ; bytes_to_discard > 0 ; bytes_to_discard-- ) br->hgetbits(8) ;


//	DispDebugMsg("MP3 파일 디코딩 중") ;
	for ( int gr = 0 ; gr < 2 ; gr++ )
	{
//		DispDebugMsg("%d번 그룹을 디코딩합니다.", gr) ;
		for ( int ch = 0 ; ch < channel ; ch++ )
		{
			mp3->part2Start = br->hsstell() ;

			// 실제 샘플의 압축을 푸는 부분 ///////////////////////////////////
			get_scale_factors(mp3, ch, gr) ;
			hufman_decode(mp3, ch, gr) ;
			dequntize_sample(mp3, mp3->ro[ch], ch, gr) ;
			
			///////////////////////////////////////////////////////////////////
		}
		stereodecode(mp3, gr);
		
		for (ch=0; (unsigned int)ch<mp3->channels; ch++)
		{
			reorder (mp3, mp3->lr[ch], ch, gr);
			antialias(mp3, ch, gr);
			for (int sb=0;sb<SBLIMIT;sb++) { // Hybrid synthesis.
				hybrid(mp3, ch, gr, sb);
			}
			
			for (int ss=1;ss<SSLIMIT;ss+=2) // Frequency inversion for polyphase.
				for (sb=1;sb<SBLIMIT;sb+=2)
					mp3->re_hybridOut[sb][ss] = -mp3->re_hybridOut[sb][ss];
			
			if (ch == 0)
			{
				
				for (ss=0;ss<SSLIMIT;ss++) 
				{ // Polyphase synthesis
					for (sb=0;sb<SBLIMIT;sb++)
					{
						input_sample(mp3->filter1, mp3->re_hybridOut[sb][ss], sb);
					}
					calculate_pcm_samples(ch, mp3->filter1, mp3->soundbuf );
				}
			}
			else
				for (ss=0;ss<SSLIMIT;ss++) 
				{ // Polyphase synthesis
					for (sb=0;sb<SBLIMIT;sb++)
						input_sample(mp3->filter2, mp3->re_hybridOut[sb][ss], sb);
					calculate_pcm_samples(ch, mp3->filter2, mp3->soundbuf);
				}
		}	// channels


	}	// granule
//	DispDebugMsg("MP3 파일 디코딩 완료") ;
	return 0 ;
}


int	MP3_WriteToBuffer(MP3* mp3, int num, HANDLE event)
{
//	DispDebugMsg("MP3_WriteToBuffer") ;
	SeekFrame(mp3, num) ;

//	DispDebugMsg("%d번 프레임 부분을 찾았습니다.", num) ;
	MP3_ReadHeader(&mp3->Header) ;
//	DispDebugMsg("헤더 정보를 읽어왔습니다.") ;
	MP3_Calc_FrameSize(mp3) ;	// 슬롯 수를 얻어오기 위해 
//	DispDebugMsg("프레임 사이즈를 계산했습니다..") ;
	MP3_Decode(mp3) ;
//	DispDebugMsg("MP3파일 디코딩합니다.") ;
	OutputData(mp3->soundbuf) ;
//	DispDebugMsg("디코딩 된 내용을 버퍼에 기록했습니다.") ;
	return 0 ;
}

void MP3_Close (MP3* mp3)
{
	Close() ;
	if ( mp3->offset ) delete mp3->offset ;
	if ( mp3->br ) delete mp3->br ;
	if ( mp3->soundbuf ) delete mp3->soundbuf ;
	if ( mp3->filter1 ) delete mp3->filter1 ;
	if ( mp3->filter2 ) delete mp3->filter2 ;
	if ( mp3 != NULL ) delete mp3 ;
}

int MP3_ReadSideInfo()
{
	return 0 ;
}
void inv_mdct(MP3 *mp3, float *in, float *out, int block_type) ;

void hybrid(MP3 *mp3, int ch, int gr, int sb)
{
	float *fsIn  = mp3->hybridIn[sb];
	float *tsOut = mp3->re_hybridOut[sb];
	side_info_t *si = &mp3->si ;
	gr_info_s *gr_info = &(si->ch[ch].gr[gr]);
	
	float rawout[36];
	int bt;
	
	bt = (gr_info->window_switching_flag && gr_info->mixed_block_flag &&
		(sb < 2)) ? 0 : gr_info->block_type;
	
	inv_mdct(mp3, fsIn, rawout, bt);
	
	/* overlap addition */
	tsOut[0]             = rawout[0]  + mp3->prevblck[ch][sb][0];
	mp3->prevblck[ch][sb][0]  = rawout[18];
	tsOut[1]             = rawout[1]  + mp3->prevblck[ch][sb][1];
	mp3->prevblck[ch][sb][1]  = rawout[19];
	tsOut[2]             = rawout[2]  + mp3->prevblck[ch][sb][2];
	mp3->prevblck[ch][sb][2]  = rawout[20];
	tsOut[3]             = rawout[3]  + mp3->prevblck[ch][sb][3];
	mp3->prevblck[ch][sb][3]  = rawout[21];
	tsOut[4]             = rawout[4]  + mp3->prevblck[ch][sb][4];
	mp3->prevblck[ch][sb][4]  = rawout[22];
	tsOut[5]             = rawout[5]  + mp3->prevblck[ch][sb][5];
	mp3->prevblck[ch][sb][5]  = rawout[23];
	tsOut[6]             = rawout[6]  + mp3->prevblck[ch][sb][6];
	mp3->prevblck[ch][sb][6]  = rawout[24];
	tsOut[7]             = rawout[7]  + mp3->prevblck[ch][sb][7];
	mp3->prevblck[ch][sb][7]  = rawout[25];
	tsOut[8]             = rawout[8]  + mp3->prevblck[ch][sb][8];
	mp3->prevblck[ch][sb][8]  = rawout[26];
	tsOut[9]             = rawout[9]  + mp3->prevblck[ch][sb][9];
	mp3->prevblck[ch][sb][9]  = rawout[27];
	tsOut[10]            = rawout[10] + mp3->prevblck[ch][sb][10];
	mp3->prevblck[ch][sb][10] = rawout[28];
	tsOut[11]            = rawout[11] + mp3->prevblck[ch][sb][11];
	mp3->prevblck[ch][sb][11] = rawout[29];
	tsOut[12]            = rawout[12] + mp3->prevblck[ch][sb][12];
	mp3->prevblck[ch][sb][12] = rawout[30];
	tsOut[13]            = rawout[13] + mp3->prevblck[ch][sb][13];
	mp3->prevblck[ch][sb][13] = rawout[31];
	tsOut[14]            = rawout[14] + mp3->prevblck[ch][sb][14];
	mp3->prevblck[ch][sb][14] = rawout[32];
	tsOut[15]            = rawout[15] + mp3->prevblck[ch][sb][15];
	mp3->prevblck[ch][sb][15] = rawout[33];
	tsOut[16]            = rawout[16] + mp3->prevblck[ch][sb][16];
	mp3->prevblck[ch][sb][16] = rawout[34];
	tsOut[17]            = rawout[17] + mp3->prevblck[ch][sb][17];
	mp3->prevblck[ch][sb][17] = rawout[35];
	
}

#define PI   3.141593
#define PI12 0.2617994
#define PI18 0.17453293
#define PI24 0.1308997
#define PI36 0.08726646
#define PI72 0.04363323

void inv_mdct(MP3 *mp3, float *in, float *out, int block_type)
{
	int   i, six_i, p;
	int   odd_i, two_odd_i, four_odd_i, eight_odd_i;
	float    tmp[18], save, sum;
	float 	  pp1, pp2;
	static  BOOL MDCT_init = FALSE;
	static  float  win[4][36];
	static  float COS18[138];
	
	if(!MDCT_init){
		
		/* type 0 */
		for(i=0;i<36;i++)
			win[0][i] = (float) sin( PI36 *(i+0.5) );
		
		/* type 1*/
		for(i=0;i<18;i++)
			win[1][i] = (float) sin( PI36 *(i+0.5) );
		for(i=18;i<24;i++)
			win[1][i] = 1.0f;
		for(i=24;i<30;i++)
			win[1][i] = (float) sin( PI12 *(i+0.5-18) );
		for(i=30;i<36;i++)
			win[1][i] = 0.0f;
		
		/* type 2 (not needed anymore) */
		/*		for(i=0;i<12;i++)
		win[2][i] = (float) sin( PI12*(i+0.5) ) ;
		for(i=12;i<36;i++)
		win[2][i] = 0.0f; */
		
		/* type 3*/
		for(i=0;i<6;i++)
			win[3][i] = 0.0f;
		for(i=6;i<12;i++)
			win[3][i] = (float) sin(PI12 * (i+ 0.5 - 6.0));
		for(i=12;i<18;i++)
			win[3][i] =1.0f;
		for(i=18;i<36;i++)
			win[3][i] = (float) sin(PI36 * (i + 0.5));
		
		for(i=0;i<138;i++)
			COS18[i] = (float) cos(PI18 * i);
		
		MDCT_init = TRUE;
	}
	
	if(block_type == 2){
		
		for(p=0;p<36;p+=9) {
			out[p]   = out[p+1] = out[p+2] = out[p+3] =
				out[p+4] = out[p+5] = out[p+6] = out[p+7] =
				out[p+8] = 0.0f;
		}
		
		six_i = 0;
		for(i=0;i<3;i++)
		{
			
			// 12 point IMDCT
			
			// Begin 12 point IDCT
			
			// Input aliasing for 12 pt IDCT
			in[15+i] += in[12+i]; in[12+i] += in[9+i]; in[9+i]  +=  in[6+i];
			in[6+i]  += in[3+i];  in[3+i]  += in[0+i];
			
			// Input aliasing on odd indices (for 6 point IDCT)
			in[15+i] += in[9+i];  in[9+i]  += in[3+i];
			
			// 3 point IDCT on even indices
			
			pp2 = in[12+i] * 0.500000000f;
			pp1 = in[ 6+i] * 0.866025403f;
			sum = in[0+i] + pp2;
			tmp[1] = in[0+i] - in[12+i];
			tmp[0] = sum + pp1;
			tmp[2] = sum - pp1;
			
			// End 3 point IDCT on even indices
			
			// 3 point IDCT on odd indices (for 6 point IDCT)
			
			pp2 = in[15+i] * 0.500000000f;
			pp1 = in[9+i]  * 0.866025403f;
			sum = in[3+i] + pp2;
			tmp[4] = in[3+i] - in[15+i];
			tmp[5] = sum + pp1;
			tmp[3] = sum - pp1;
			
			// End 3 point IDCT on odd indices
			
			// Twiddle factors on odd indices (for 6 point IDCT)
			
			tmp[3] *= 1.931851653f;
			tmp[4] *= 0.707106781f;
			tmp[5] *= 0.517638090f;
			
			// Output butterflies on 2 3 point IDCT's (for 6 point IDCT)
			
			save = tmp[0];
			tmp[0] += tmp[5];
			tmp[5] = save - tmp[5];
			save = tmp[1];
			tmp[1] += tmp[4];
			tmp[4] = save - tmp[4];
			save = tmp[2];
			tmp[2] += tmp[3];
			tmp[3] = save - tmp[3];
			
			// End 6 point IDCT
			
			// Twiddle factors on indices (for 12 point IDCT)
			
			tmp[0]  *=  0.504314480f;
			tmp[1]  *=  0.541196100f;
			tmp[2]  *=  0.630236207f;
			tmp[3]  *=  0.821339815f;
			tmp[4]  *=  1.306562965f;
			tmp[5]  *=  3.830648788f;
			
			// End 12 point IDCT
			
			// Shift to 12 point modified IDCT, multiply by window type 2
			tmp[8]  = -tmp[0] * 0.793353340f;
			tmp[9]  = -tmp[0] * 0.608761429f;
			tmp[7]  = -tmp[1] * 0.923879532f;
			tmp[10] = -tmp[1] * 0.382683432f;
			tmp[6]  = -tmp[2] * 0.991444861f;
			tmp[11] = -tmp[2] * 0.130526192f;
			
			tmp[0]  =  tmp[3];
			tmp[1]  =  tmp[4] * 0.382683432f;
			tmp[2]  =  tmp[5] * 0.608761429f;
			
			tmp[3]  = -tmp[5] * 0.793353340f;
			tmp[4]  = -tmp[4] * 0.923879532f;
			tmp[5]  = -tmp[0] * 0.991444861f;
			
			tmp[0] *= 0.130526192f;
			
			out[six_i + 6]  += tmp[0];
			out[six_i + 7]  += tmp[1];
			out[six_i + 8]  += tmp[2];
			out[six_i + 9]  += tmp[3];
			out[six_i + 10] += tmp[4];
			out[six_i + 11] += tmp[5];
			out[six_i + 12] += tmp[6];
			out[six_i + 13] += tmp[7];
			out[six_i + 14] += tmp[8];
			out[six_i + 15] += tmp[9];
			out[six_i + 16] += tmp[10];
			out[six_i + 17] += tmp[11];
			
			six_i += 6;
		}
		
	 } else {
		 
		 // 36 point IDCT
		 
		 // input aliasing for 36 point IDCT
		 in[17]+=in[16]; in[16]+=in[15]; in[15]+=in[14]; in[14]+=in[13];
		 in[13]+=in[12]; in[12]+=in[11]; in[11]+=in[10]; in[10]+=in[9];
		 in[9] +=in[8];  in[8] +=in[7];  in[7] +=in[6];  in[6] +=in[5];
		 in[5] +=in[4];  in[4] +=in[3];  in[3] +=in[2];  in[2] +=in[1];
		 in[1] +=in[0];
		 
		 // 18 point IDCT for odd indices
		 
		 // input aliasing for 18 point IDCT
		 in[17]+=in[15]; in[15]+=in[13]; in[13]+=in[11]; in[11]+=in[9];
		 in[9] +=in[7];  in[7] +=in[5];  in[5] +=in[3];  in[3] +=in[1];
		 
		 // 9 point IDCT on even indices
		 /*   for(i=0; i<9; i++) {
		 sum = 0.0;
		 
		   for(j=0;j<18;j+=2)
		   sum += in[j] * cos(PI36 * (2*i + 1) * j);
		   
			 tmp[i] = sum;
	 } */
		 
		 for(i=0; i<9; i++) {
			 
			 odd_i = (i << 1) + 1;
			 
			 sum = in[0];
			 
			 two_odd_i = odd_i << 1;
			 four_odd_i = odd_i << 2;
			 
			 sum += in[2]  * COS18[odd_i];
			 sum += in[4]  * COS18[two_odd_i];
			 
			 eight_odd_i = two_odd_i << 2;
			 
			 sum += in[6]  * COS18[four_odd_i - odd_i];
			 sum += in[8]  * COS18[four_odd_i];
			 sum += in[10] * COS18[four_odd_i + odd_i];
			 sum += in[12] * COS18[four_odd_i + two_odd_i];
			 sum += in[14] * COS18[eight_odd_i - odd_i];
			 sum += in[16] * COS18[eight_odd_i];
			 
			 tmp[i] = sum;
		 }
		 
		 // End 9 point IDCT on even indices
		 
		 // 9 point IDCT on odd indices
		 /*   for(i=0; i<9; i++) {
		 sum = 0.0;
		 
		   for(j=0;j<18;j+=2)
		   sum += in[j+1] * cos(PI36 * (2*i + 1) * j);
		   
			 tmp[17-i] = sum;
	 } */
		 
		 for(i=0; i<9; i++) {
			 
			 odd_i = (i << 1) + 1;
			 
			 sum = in[1];
			 
			 two_odd_i = odd_i << 1;
			 four_odd_i = odd_i << 2;
			 
			 sum += in[3]  * COS18[odd_i];
			 sum += in[5]  * COS18[two_odd_i];
			 
			 eight_odd_i = two_odd_i << 2;
			 
			 sum += in[7]  * COS18[four_odd_i - odd_i];
			 sum += in[9]  * COS18[four_odd_i];
			 sum += in[11] * COS18[four_odd_i + odd_i];
			 sum += in[13] * COS18[four_odd_i + two_odd_i];
			 sum += in[15] * COS18[eight_odd_i - odd_i];
			 sum += in[17] * COS18[eight_odd_i];
			 
			 tmp[17-i] = sum;
		 }
		 
		 // End 9 point IDCT on odd indices
		 
		 // Twiddle factors on odd indices
		 
		 tmp[9]  *= 5.736856623f;
		 tmp[10] *= 1.931851653f;
		 tmp[11] *= 1.183100792f;
		 tmp[12] *= 0.871723397f;
		 tmp[13] *= 0.707106781f;
		 tmp[14] *= 0.610387294f;
		 tmp[15] *= 0.551688959f;
		 tmp[16] *= 0.517638090f;
		 tmp[17] *= 0.501909918f;
		 
		 // Butterflies on 9 point IDCT's
		 for (i=0;i<9;i++) {
			 save = tmp[i];
			 tmp[i] += tmp[17-i];
			 tmp[17-i] = save - tmp[17-i];
		 }
		 // end 18 point IDCT
		 
		 // twiddle factors for 36 point IDCT
		 
		 tmp[0] *=  -0.500476342f;
		 tmp[1] *=  -0.504314480f;
		 tmp[2] *=  -0.512139757f;
		 tmp[3] *=  -0.524264562f;
		 tmp[4] *=  -0.541196100f;
		 tmp[5] *=  -0.563690973f;
		 tmp[6] *=  -0.592844523f;
		 tmp[7] *=  -0.630236207f;
		 tmp[8] *=  -0.678170852f;
		 tmp[9] *=  -0.740093616f;
		 tmp[10]*=  -0.821339815f;
		 tmp[11]*=  -0.930579498f;
		 tmp[12]*=  -1.082840285f;
		 tmp[13]*=  -1.306562965f;
		 tmp[14]*=  -1.662754762f;
		 tmp[15]*=  -2.310113158f;
		 tmp[16]*=  -3.830648788f;
		 tmp[17]*= -11.46279281f;
		 
		 // end 36 point IDCT
		 
		 // shift to modified IDCT
		 
		 out[0] =-tmp[9]  * win[block_type][0];
		 out[1] =-tmp[10] * win[block_type][1];
		 out[2] =-tmp[11] * win[block_type][2];
		 out[3] =-tmp[12] * win[block_type][3];
		 out[4] =-tmp[13] * win[block_type][4];
		 out[5] =-tmp[14] * win[block_type][5];
		 out[6] =-tmp[15] * win[block_type][6];
		 out[7] =-tmp[16] * win[block_type][7];
		 out[8] =-tmp[17] * win[block_type][8];
		 
		 out[9] = tmp[17] * win[block_type][9];
		 out[10]= tmp[16] * win[block_type][10];
		 out[11]= tmp[15] * win[block_type][11];
		 out[12]= tmp[14] * win[block_type][12];
		 out[13]= tmp[13] * win[block_type][13];
		 out[14]= tmp[12] * win[block_type][14];
		 out[15]= tmp[11] * win[block_type][15];
		 out[16]= tmp[10] * win[block_type][16];
		 out[17]= tmp[9]  * win[block_type][17];
		 out[18]= tmp[8]  * win[block_type][18];
		 out[19]= tmp[7]  * win[block_type][19];
		 out[20]= tmp[6]  * win[block_type][20];
		 out[21]= tmp[5]  * win[block_type][21];
		 out[22]= tmp[4]  * win[block_type][22];
		 out[23]= tmp[3]  * win[block_type][23];
		 out[24]= tmp[2]  * win[block_type][24];
		 out[25]= tmp[1]  * win[block_type][25];
		 out[26]= tmp[0]  * win[block_type][26];
		 
		 out[27]= tmp[0]  * win[block_type][27];
		 out[28]= tmp[1]  * win[block_type][28];
		 out[29]= tmp[2]  * win[block_type][29];
		 out[30]= tmp[3]  * win[block_type][30];
		 out[31]= tmp[4]  * win[block_type][31];
		 out[32]= tmp[5]  * win[block_type][32];
		 out[33]= tmp[6]  * win[block_type][33];
		 out[34]= tmp[7]  * win[block_type][34];
		 out[35]= tmp[8]  * win[block_type][35];
		 
   }
}

