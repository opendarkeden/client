/*-------------------------------------------------------------------------------------

  OGG FILE Streaming						by sonee

  Date : 2003/06/30


  도움 : fuzzface

-------------------------------------------------------------------------------------*/
#ifndef __OGG_STREAMING__
#define __OGG_STREAMING__

#include <dsound.h>

#include "directxlib/codec.h"
#include "directxlib/vorbisfile.h"

#define STREAM_MAX					5

#define STREAM_PERMIT_NEXT_OFF		0
#define STREAM_PERMIT_NEXT_ON		1
#define STREAM_PERMIT_NEXT_NOW		2

#define STREAM_STATE_ERR			0
#define STREAM_STATE_NONE			1
#define STREAM_STATE_INITIALIZE		2
#define STREAM_STATE_LOAD			3
#define STREAM_STATE_READY			4
#define STREAM_STATE_PLAY			5
#define STREAM_STATE_PAUSE			6
#define STREAM_STATE_STOP			7
#define STREAM_STATE_CLOSE			8
#define STREAM_STATE_WAIT_FOR_END	9

#define STREAM_ERR_OK						0
#define STREAM_ERR_CANNOT_CREATE			-1
#define STREAM_ERR_CANNOT_PLAY				-2
#define STREAM_ERR_CANNOT_PAUSE				-3
#define STREAM_ERR_CANNOT_RESUME			-4
#define STREAM_ERR_CANNOT_STOP				-5
#define STREAM_ERR_CANNOT_DELETE			-6
#define STREAM_ERR_NOT_READY				-7
#define STREAM_ERR_UNAVAILABLE_SOUND_OBJECT	-8
#define STREAM_ERR_UNAVAILABLE_HWND			-9
#define STREAM_ERR_UNAVAILABLE_POSITION_SET	-10
#define	STREAM_ERR_UNAVAILABLE_TIMER_INDEX	-11

#define SOUND_ERR_OK					0
#define SOUND_ERR_CANNOT_CREATE_DS		-1
#define SOUND_ERR_CANNOT_FIND_DS		-2
#define SOUND_ERR_CANNOT_SET_LEVEL		-3
#define SOUND_ERR_NO_FILE				-4
#define SOUND_ERR_OUT_OF_LIMIT			-5
#define SOUND_ERR_OUT_OF_INDEX			-6
#define SOUND_ERR_OUT_OF_COPY_INDEX		-7		// no use
#define SOUND_ERR_INDEX_COLLISION		-8
#define SOUND_ERR_NOT_READABLE			-9
#define SOUND_ERR_CANNOT_CREATE_BUFFER	-10
#define SOUND_ERR_CANNOT_DELETE_BUFFER	-11
#define SOUND_ERR_BUFFER_LOST			-12
#define SOUND_ERR_CANNOT_UNLOCK			-13
#define SOUND_ERR_CANNOT_DUPLICATE		-14
#define SOUND_ERR_CANNOT_SET_POSITION	-15
#define SOUND_ERR_CANNOT_PLAY			-16
#define SOUND_ERR_CANNOT_STOP			-17
#define SOUND_ERR_CANNOT_SET_PAN		-18
#define SOUND_ERR_CANNOT_SET_VOLUME		-19
#define SOUND_ERR_CANNOT_GET_INTERFACE	-20
#define SOUND_ERR_INVAILD_PAN			-21
#define SOUND_ERR_INVAILD_VOLUME		-22
#define SOUND_ERR_CANNOT_CREATE_STREAM	-23
#define SOUND_ERR_CANNOT_OPEN_STREAM	-24
#define SOUND_ERR_CANNOT_FIND_FOURCC	-25
#define SOUND_ERR_INVAILD_FILE_TYPE		-26
#define SOUND_ERR_CANNOT_OPEN_OGG		-27
#define SOUND_ERR_CANNOT_READ_OGG_INFO	-28
#define SOUND_ERR_CANNOT_CREATE_THREAD	-29
#define SOUND_ERR_CANNOT_CREATE_NOTIFY	-30
#define SOUND_ERR_CANNOT_MOVE			-31
#define SOUND_ERR_CANNOT_SET_FORMAT		-32
#define SOUND_ERR_NULL_BUFFER			-33

#define SOUND_RESTORE_MAX		3
#define SOUND_STREAM_POINT_MAX	4

#define SOUND_MAX				100
#define SOUND_COPY_MAX			10

#define SOUND_8BIT				8
#define SOUND_16BIT				16

#define SOUND_11K				11025
#define SOUND_22K				22050
#define SOUND_44K				44100

#define SOUND_MONO				1
#define SOUND_STEREO			2

#define SOUND_PLAY_ONCE			0
#define SOUND_PLAY_REPEAT		1

#define SOUND_PAN_LEFT			-10000
#define SOUND_PAN_CENTER		0
#define SOUND_PAN_RIGHT			10000

#define SOUND_VOLUME_MAX		0
#define SOUND_VOLUME_MIN		-10000

#define SOUND_FREQ_MAX			100000
#define SOUND_FREQ_MIN			0

#define SOUND_FILE_TYPE_WAVE		0
#define SOUND_FILE_TYPE_OGG			1

#define SOUND_WAVE_FMT_PCM			0x01
#define SOUND_WAVE_FMT_MS_ADPCM		0x02
#define SOUND_WAVE_FMT_IMA_ADPCM	0x11

#define SOUND_OGG_BUFFER_MAX		512

#define SOUND_READ_FROM_CURRENT		DSBLOCK_FROMWRITECURSOR
#define SOUND_READ_ENTIRE			DSBLOCK_ENTIREBUFFER

#define	SOUND_LITTLE_ENDIAN			0
#define	SOUND_BIG_ENDIAN			1
#define	SOUND_ENDIAN				SOUND_LITTLE_ENDIAN

#define	SOUND_SIGN_TYPE_SIGNED		1
#define	SOUND_SIGN_TYPE_UNSIGNED	0
#define	SOUND_SIGN_TYPE				SOUND_SIGN_TYPE_SIGNED

#undef _MT

#pragma pack(1)

typedef struct tag_sound_handle
{

	int				type;
	unsigned long	size;
	FILE			*pt;
	unsigned long	start_position;
	OggVorbis_File	vf;
	vorbis_info		*vi;
	int				ogg_current_section;
	WAVEFORMATEX	fmt;
	DSBUFFERDESC	desc;

} sound_handle;

typedef struct tag_wav_fourcc
{

	unsigned char	fourcc[4];
	unsigned int	size;

} wave_fourcc;

typedef struct tag_wave_pcm_header {
  short FormatTag;          // WAVE_FORMAT_PCM
  short Channels;           // # of channels				2
  long  SampleRate;         // sampling rate				4
  long  BytesPerSec;        // bytes per second				4
  short BlockAlign;         // sample block alignment		2
  short BitsPerSample;      // bits per second				2
} wave_pcm_header;

typedef struct tag_wave_ima_adpcm_header {
  short FormatTag;          // WAVE_FORMAT_PCM
  short Channels;           // # of channels				2
  long  SampleRate;         // sampling rate				4
  long  BytesPerSec;        // bytes per second				4
  short BlockAlign;         // sample block alignment		2
  short BitsPerSample;      // bits per second				2
  short	cbSize;
  short	wSamplesPerBlock;
} wave_ima_adpcm_header;

#pragma pack()

class CDirectSoundBuffer
{

	protected:

		LPDIRECTSOUND		dsp;

		IDirectSoundBuffer	*buffer;
		IDirectSoundBuffer	*list[SOUND_MAX][SOUND_COPY_MAX];

		friend class COGGSTREAM;
		friend class music;

		BOOL			stream_stop;

		HRESULT			initialize(HWND hwnd, unsigned int stereo, unsigned int freq, unsigned int bit);
		HRESULT			destroy(void);

		HRESULT			getFourCC(FILE *fp, unsigned char *fourcc);
		HRESULT			findFourCC(FILE *fp, wave_fourcc *chunk, const unsigned char *fourcc);
		HRESULT			createBuffer (IDirectSoundBuffer **buf, DSBUFFERDESC *desc);
		HRESULT			duplicateBuffer(IDirectSoundBuffer *source, IDirectSoundBuffer **target);
		HRESULT			deleteBuffer(IDirectSoundBuffer *buf);
		HRESULT			openFromFile (FILE *fp, sound_handle *handle);
		HRESULT			openFromMem (unsigned char *source, WAVEFORMATEX *fmt, DSBUFFERDESC *desc);
		HRESULT			readFromFile (IDirectSoundBuffer *buf, sound_handle *handle, unsigned int pos, unsigned int size, DWORD flag, DWORD *read);
		HRESULT			readFromMem (IDirectSoundBuffer *buf, unsigned char *source, unsigned int pos, unsigned int size, DWORD flag, DWORD *read);
		HRESULT			moveFromFile (sound_handle *handle, long position);
		HRESULT			moveFromMem (sound_handle *handle, long position);
		HRESULT			closeFromFile (sound_handle *handle);
		HRESULT			closeFromMem (sound_handle *handle);

	public:
		CDirectSoundBuffer(HWND hwnd, unsigned int stereo, unsigned int freq, unsigned int bit);
		~CDirectSoundBuffer(void);

		int				load(FILE *fp, int index);
		HRESULT			play(int sound_index, int sound_sub_index, unsigned char option, long pan_v, long vol_v);
		HRESULT			stop(int sound_index, int sound_sub_index);
		HRESULT			allStop(void);
		HRESULT			close(int sound_index);

		HRESULT			isPlaying(int sound_index, int sound_sub_index);
		HRESULT			pan(int sound_index, int sound_sub_index, long pan_v);
		HRESULT			volume(int sound_index, int sound_sub_index, long vol_v);
		HRESULT			freq(int sound_index, int sound_sub_index, DWORD freq_v);

} ;


typedef struct tag_position_set
{

	DWORD						index;				//	포지션 정보의 인덱스 넘버
	DWORD						option;				//	옵션
	DWORD						start_pos;			//	시작 부분의 샘플 넘버
	DWORD						loop_start_pos;		//	루핑할 부분의 샘플 넘버
	DWORD						size;				//	전체 샘플 수
	struct tag_position_set		*next;				//	다음 포지션 정보의 주소

} position_set;

#ifndef _MT
extern DWORD	stream_timer_handle;
#endif

class COGGSTREAM
{

private:

	IDirectSoundBuffer	*buffer;					//	실제 스트림의 버퍼
	HWND				hWnd;						//	윈도 핸들
	CDirectSoundBuffer				*snd;						//	사운드 객체의 포인터
	DWORD				buffer_size;				//	버퍼의 전체 크지 (샘플 단위)
	DWORD				gap;						//	스트림 읽기 커서와 플레이 커서간의 최소 거리
	DWORD				read_once;					//	한번에 읽는 최대 샘플 수
	DWORD				option;						//	옵션
	DWORD				state;						//	현재 상태
	DWORD				size;						//	버퍼의 전체 샘플 수
	DWORD				play_size;					//	현재까지 플레이한 샘플 수
	DWORD				read_size;					//	현재까지 읽어들인 샘플 수
	DWORD				temp_play_size;				//	스트림이 한바퀴 돌때 이전 루프에 플레이 되지 않은 샘플 수
	DWORD				cur_play;					//	현재 플레이 커서
	DWORD				old_play;					//	이전 플레이 커서
	DWORD				cur_write;					//	현재 읽기	커서
	position_set		pos;
	position_set		*next_pos;
	sound_handle		handle;						//	사운드 핸들
#ifdef _MT
	HANDLE				thread_handle;
	DWORD				thread_id;
	DWORD				timer;
	CRITICAL_SECTION	cs;
#endif

#ifdef _MT
	int					streamInitialize(HWND h, CDirectSoundBuffer *s, DWORD s_buffer, DWORD g_buffer, DWORD o_buffer);
#else
	int					streamInitialize(HWND h, CDirectSoundBuffer *s, DWORD s_buffer, DWORD g_buffer, DWORD o_buffer, DWORD index);
#endif

public:

#ifndef _MT
	DWORD				stream_timer_handle;
#endif

#ifdef _MT
	COGGSTREAM(HWND h, CDirectSoundBuffer *s, DWORD s_buffer, DWORD g_buffer, DWORD o_buffer);
#else
	COGGSTREAM(HWND h, CDirectSoundBuffer *s, DWORD s_buffer, DWORD g_buffer, DWORD o_buffer, DWORD index);
#endif
	~COGGSTREAM(void);

	HRESULT				streamLoad(FILE *fp, position_set *p);
	HRESULT				streamPlay(DWORD o);
	HRESULT				streamIsPlaying(void) 
	{
		
		return (
					(this->state == STREAM_STATE_PLAY || 
					this->state == STREAM_STATE_WAIT_FOR_END) ? TRUE : FALSE 
					
				); 
	
	}
	HRESULT				streamStop(void);
	HRESULT				streamPause(void);
	HRESULT				streamResume(void);
	HRESULT				streamPan(long pan_v);
	HRESULT				streamVolume(long vol_v);
	HRESULT				streamClose(void);
	HRESULT				streamSetNextPosition(position_set *p);
	HRESULT				streamSetCurrentPosition(position_set *p);
	HRESULT				streamPermitNextPosition(DWORD flag);
	static void			streamUpdate(void *s);

};


#endif