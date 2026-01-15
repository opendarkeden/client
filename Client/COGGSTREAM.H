/*-------------------------------------------------------------------------------------

  OGG FILE Streaming						by sonee

  Date : 2003/06/30


  ���� : fuzzface

-------------------------------------------------------------------------------------*/
#ifndef __OGG_STREAMING__
#define __OGG_STREAMING__

#ifdef PLATFORM_WINDOWS
#include <dsound.h>
#else
#include "../../basic/Platform.h"
#include <string.h>

/* Forward declarations */
struct IDirectSound;
struct IDirectSoundBuffer;
struct IDirectSoundNotify;
typedef struct IDirectSound* LPDIRECTSOUND;

/* GUID type */
typedef struct _GUID {
    DWORD Data1;
    WORD  Data2;
    WORD  Data3;
    BYTE  Data4[8];
} GUID;

/* IID type (same as GUID) */
typedef GUID IID;

/* SIZE_T type */
#ifndef SIZE_T
#define SIZE_T size_t
#endif

/* WAVE format structure */
typedef struct _WAVEFORMATEX {
    WORD    wFormatTag;
    WORD    nChannels;
    DWORD   nSamplesPerSec;
    DWORD   nAvgBytesPerSec;
    WORD    nBlockAlign;
    WORD    wBitsPerSample;
    WORD    cbSize;
} WAVEFORMATEX, *LPWAVEFORMATEX;

/* DSBUFFERDESC structure */
typedef struct _DSBUFFERDESC {
    DWORD           dwSize;
    DWORD           dwFlags;
    DWORD           dwBufferBytes;
    DWORD           dwReserved;
    LPWAVEFORMATEX  lpwfxFormat;
    GUID            guid3DAlgorithm;
} DSBUFFERDESC, *LPDSBUFFERDESC;

/* DirectSound buffer flags */
#define DSBCAPS_CTRLVOLUME              0x00000010
#define DSBCAPS_PRIMARYBUFFER           0x00000001
#define DSBCAPS_LOCSOFTWARE             0x00080000
#define DSBCAPS_GETCURRENTPOSITION2     0x00010000
#define DSBCAPS_CTRLPAN                 0x00000020

/* WAVE format constants */
#define WAVE_FORMAT_PCM        1

/* COM interface IDs */
extern const IID IID_IDirectSoundBuffer;
extern const IID IID_IDirectSound;

/* COM initialization functions (stubs for non-Windows) */
static inline HRESULT CoInitialize(void* pvReserved) { (void)pvReserved; return 0; }
static inline void CoUninitialize() {}
static inline void ZeroMemory(void* ptr, SIZE_T size) { memset(ptr, 0, size); }

#endif

#include "DXLib/codec.h"
#include "DXLib/vorbisfile.h"

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

	DWORD						index;				//	������ ������ �ε��� �ѹ�
	DWORD						option;				//	�ɼ�
	DWORD						start_pos;			//	���� �κ��� ���� �ѹ�
	DWORD						loop_start_pos;		//	������ �κ��� ���� �ѹ�
	DWORD						size;				//	��ü ���� ��
	struct tag_position_set		*next;				//	���� ������ ������ �ּ�

} position_set;

#ifndef _MT
extern DWORD	stream_timer_handle;
#endif

class COGGSTREAM
{

private:

	IDirectSoundBuffer	*buffer;					//	���� ��Ʈ���� ����
	HWND				hWnd;						//	���� �ڵ�
	CDirectSoundBuffer				*snd;						//	���� ��ü�� ������
	DWORD				buffer_size;				//	������ ��ü ũ�� (���� ����)
	DWORD				gap;						//	��Ʈ�� �б� Ŀ���� �÷��� Ŀ������ �ּ� �Ÿ�
	DWORD				read_once;					//	�ѹ��� �д� �ִ� ���� ��
	DWORD				option;						//	�ɼ�
	DWORD				state;						//	���� ����
	DWORD				size;						//	������ ��ü ���� ��
	DWORD				play_size;					//	������� �÷����� ���� ��
	DWORD				read_size;					//	������� �о���� ���� ��
	DWORD				temp_play_size;				//	��Ʈ���� �ѹ��� ���� ���� ������ �÷��� ���� ���� ���� ��
	DWORD				cur_play;					//	���� �÷��� Ŀ��
	DWORD				old_play;					//	���� �÷��� Ŀ��
	DWORD				cur_write;					//	���� �б�	Ŀ��
	position_set		pos;
	position_set		*next_pos;
	sound_handle		handle;						//	���� �ڵ�
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