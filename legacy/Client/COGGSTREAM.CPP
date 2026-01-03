/*

	stream.c

	스트림 관리 (sound class의 friend class)



				Fuzzface in 2003.

*/

#include "dxlib/CDirectSound.h"
#include "coggstream.h"


CDirectSoundBuffer::CDirectSoundBuffer(HWND hwnd, unsigned int stereo, unsigned int freq, unsigned int bit)
{
	initialize (hwnd, stereo, freq, bit);
}
CDirectSoundBuffer::~CDirectSoundBuffer()
{
	destroy();
}

HRESULT CDirectSoundBuffer::initialize(HWND hwnd, unsigned int stereo, unsigned int freq, unsigned int bit)
{

	WAVEFORMATEX	format;
	DSBUFFERDESC	buffer_info;

	dsp = NULL;
	buffer = NULL;

	stream_stop = FALSE;

	ZeroMemory (list, sizeof(IDirectSoundBuffer *) * SOUND_MAX * SOUND_COPY_MAX);

	CoInitialize(NULL);//, COINIT_MULTITHREADED);

//	if (FAILED(DirectSoundCreate (NULL, &dsp, NULL))) return SOUND_ERR_CANNOT_CREATE_DS;

	dsp = g_DXSound.GetDS();
//	if (FAILED(dsp->SetCooperativeLevel(hwnd, DSSCL_PRIORITY))) return SOUND_ERR_CANNOT_SET_LEVEL;

	ZeroMemory (&format, sizeof(format));

	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nChannels = (unsigned short)stereo;
	format.nSamplesPerSec = freq;
	format.wBitsPerSample = (unsigned short)bit;
	format.nBlockAlign = (unsigned short)((format.wBitsPerSample / 8) * format.nChannels);
	format.nAvgBytesPerSec = (unsigned short)format.nSamplesPerSec * format.nBlockAlign;

	ZeroMemory (&buffer_info, sizeof(DSBUFFERDESC));

	buffer_info.dwSize = sizeof(DSBUFFERDESC);
	buffer_info.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_PRIMARYBUFFER;
	buffer_info.dwBufferBytes = 0;
	buffer_info.lpwfxFormat = NULL;

	if (FAILED(dsp->CreateSoundBuffer(&buffer_info, &buffer, NULL))) {buffer = NULL; return SOUND_ERR_CANNOT_CREATE_BUFFER; }
	
	return SOUND_ERR_OK;

}

HRESULT CDirectSoundBuffer::destroy(void)
{
	
	int loop = 0;

	if (!dsp) return SOUND_ERR_CANNOT_FIND_DS;

	for (loop = 0; loop < SOUND_MAX; ++loop)
	{

		close (loop);

	}

	stream_stop = TRUE;

	deleteBuffer (buffer);

//	dsp->Release();

//	dsp = NULL;

	CoUninitialize();

	return SOUND_ERR_OK;

}

HRESULT CDirectSoundBuffer::createBuffer (IDirectSoundBuffer **buf, DSBUFFERDESC *desc)
{

	IDirectSoundBuffer *temp_buf;

	if (!dsp) return SOUND_ERR_CANNOT_FIND_DS;

	if (FAILED(dsp->CreateSoundBuffer(desc, &temp_buf, NULL))) return SOUND_ERR_CANNOT_CREATE_BUFFER;
	
	if (FAILED(temp_buf->QueryInterface(IID_IDirectSoundBuffer, (void**)buf)))
	{	deleteBuffer (*buf); return SOUND_ERR_CANNOT_GET_INTERFACE;}
	//	위 함수를 쓰기 위해서는 dxguid.lib를 링크하고 WIN32_DCOM을 선언한다.

	deleteBuffer (temp_buf);

	return SOUND_ERR_OK;

}

HRESULT CDirectSoundBuffer::duplicateBuffer(IDirectSoundBuffer *source, IDirectSoundBuffer **target)
{
	
	if (!dsp) return SOUND_ERR_CANNOT_FIND_DS;

	if (!source) return SOUND_ERR_NULL_BUFFER;

	if (FAILED(dsp->DuplicateSoundBuffer(source,
		target)))
			return SOUND_ERR_CANNOT_DUPLICATE;

	return SOUND_ERR_OK;

}

HRESULT CDirectSoundBuffer::deleteBuffer(IDirectSoundBuffer *buf)
{

//	if (!dsp) return SOUND_ERR_CANNOT_FIND_DS;

	if (buf)
	{

		buf->Release();
		buf = NULL;

	} else return SOUND_ERR_CANNOT_DELETE_BUFFER;

	return SOUND_ERR_OK;

}

HRESULT	CDirectSoundBuffer::getFourCC(FILE *fp, unsigned char *fourcc)
{

	if (!fp) return SOUND_ERR_NO_FILE;

	ZeroMemory (fourcc, 4);

	if (fread (fourcc, 4, 1, fp) == EOF)
		return SOUND_ERR_CANNOT_FIND_FOURCC;

	return SOUND_ERR_OK;

}

HRESULT	CDirectSoundBuffer::findFourCC(FILE *fp, wave_fourcc *chunk, const unsigned char *fourcc)
{

	if (!fp) return SOUND_ERR_NO_FILE;
	
	ZeroMemory (chunk, sizeof (wave_fourcc));

	do
	{

		fseek (fp, chunk->size, SEEK_CUR);
		if (fread (chunk, sizeof(wave_fourcc), 1, fp) == 0)
			return SOUND_ERR_CANNOT_FIND_FOURCC;

	} while (memcmp (chunk->fourcc, fourcc, 4) != 0);

	return SOUND_ERR_OK;

}

HRESULT CDirectSoundBuffer::openFromFile (FILE *fp, sound_handle *handle)
{

	wave_fourcc			fourcc;
	unsigned int		fmt_size = 0;

	if (!fp) return SOUND_ERR_NO_FILE;

	ZeroMemory (handle, sizeof(sound_handle));

	// if it's wave file
	if (getFourCC (fp, fourcc.fourcc) != SOUND_ERR_OK)
		return SOUND_ERR_INVAILD_FILE_TYPE;

	if (memcmp (fourcc.fourcc, "RIFF", 4) == 0)
	{

		handle->type = SOUND_FILE_TYPE_WAVE;
		handle->pt = fp;

		fseek (handle->pt, 4, SEEK_CUR);	// skip "WAVE" chunk
		fseek (handle->pt, 4, SEEK_CUR);	// skip "WAVE" chunk

		if (findFourCC (handle->pt, &fourcc, (const unsigned char *)"fmt ") != SOUND_ERR_OK)
			return SOUND_ERR_INVAILD_FILE_TYPE;

		fmt_size = fourcc.size;

		fread (&handle->fmt.wFormatTag, 2, 1, handle->pt);

		switch (handle->fmt.wFormatTag)
		{

			case  SOUND_WAVE_FMT_PCM:
			{

				wave_pcm_header		header;

				fseek (handle->pt, -2, SEEK_CUR);	// rewind for read header
				fread (&header, sizeof (wave_pcm_header), 1, handle->pt);

				handle->fmt.nChannels = header.Channels;
				handle->fmt.nSamplesPerSec = header.SampleRate;
				handle->fmt.wBitsPerSample = header.BitsPerSample;
				handle->fmt.nBlockAlign = header.BlockAlign;
				handle->fmt.nAvgBytesPerSec = handle->fmt.nSamplesPerSec * handle->fmt.nBlockAlign;
				break;

			}
			case  SOUND_WAVE_FMT_MS_ADPCM:
			{

				break;
			}
			case  SOUND_WAVE_FMT_IMA_ADPCM:
			{

				wave_ima_adpcm_header	header;
				fseek (handle->pt, -2, SEEK_CUR);	// rewind for read header
				fread (&header, sizeof (wave_ima_adpcm_header), 1, handle->pt);
				handle->fmt.nChannels = header.Channels;
				handle->fmt.nSamplesPerSec = header.SampleRate;
				handle->fmt.wBitsPerSample = header.BitsPerSample;
				handle->fmt.nBlockAlign = header.BlockAlign;//fmt->wBitsPerSample / 8 * fmt->nChannels;
				handle->fmt.nAvgBytesPerSec = handle->fmt.nSamplesPerSec * handle->fmt.nBlockAlign;
				break;

			}

			default:
				return SOUND_ERR_INVAILD_FILE_TYPE;
				break;

		}

		if (findFourCC (handle->pt, &fourcc, (const unsigned char *)"data") != SOUND_ERR_OK)
			return SOUND_ERR_INVAILD_FILE_TYPE;

		handle->size = fourcc.size;

		handle->desc.dwSize = sizeof(DSBUFFERDESC);
		handle->desc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY;
		handle->desc.dwBufferBytes = handle->size;
		handle->desc.lpwfxFormat = &handle->fmt;
		handle->start_position = ftell (handle->pt);
		return SOUND_ERR_OK;
	}
	else if (memcmp (fourcc.fourcc, "OggS", 4) == 0)
	{

		handle->type = SOUND_FILE_TYPE_OGG;
		handle->pt = fp;
		fseek (handle->pt, -4, SEEK_CUR);	//rewind for read ogg
		//ogg open

		if (ov_open (handle->pt, &handle->vf, NULL, 0) < 0)
			return SOUND_ERR_CANNOT_OPEN_OGG;

		if ((handle->vi = ov_info (&handle->vf, -1)) == NULL)
			return SOUND_ERR_CANNOT_READ_OGG_INFO;

		handle->fmt.wFormatTag = SOUND_WAVE_FMT_PCM;
		handle->fmt.nChannels = (unsigned short)handle->vi->channels;
		handle->fmt.nSamplesPerSec = handle->vi->rate;
		handle->fmt.wBitsPerSample = 16;
		handle->fmt.nBlockAlign = (unsigned short)(handle->fmt.wBitsPerSample / 8 * handle->fmt.nChannels);
		handle->fmt.nAvgBytesPerSec = handle->fmt.nSamplesPerSec * handle->fmt.nBlockAlign;

		handle->desc.dwSize = sizeof(DSBUFFERDESC);
		handle->desc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY;
		handle->desc.dwBufferBytes = (unsigned long)(ov_pcm_total(&handle->vf,-1) * handle->fmt.nBlockAlign);
		handle->size = handle->desc.dwBufferBytes;
		handle->desc.lpwfxFormat = &handle->fmt;
		handle->start_position = 0;

		return SOUND_ERR_OK;

	}

	return SOUND_ERR_OK;

}

HRESULT	CDirectSoundBuffer::readFromFile (IDirectSoundBuffer *buf, sound_handle *handle, unsigned int pos, unsigned int size, DWORD flag, DWORD *read)
{

	char			*pt1 = NULL, *pt2 = NULL;
	DWORD			size_byte = 0, pos_byte = 0, size1 = 0, size2 = 0;
	DWORD			count = 0;

	if (!dsp) return SOUND_ERR_CANNOT_FIND_DS;

	if (!buf) return SOUND_ERR_NULL_BUFFER;

	size_byte = size * handle->fmt.nBlockAlign;
	pos_byte = pos * handle->fmt.nBlockAlign;

	if (buf->Lock(pos_byte,size_byte,(void**)&pt1, (DWORD*)&size1, (void**)&pt2, (DWORD*)&size2, flag) == DSERR_BUFFERLOST)
	{

		buf->Restore();
		if (buf->Lock(pos_byte,size_byte,(void**)&pt1, (DWORD*)&size1, (void**)&pt2, (DWORD*)&size2, flag) == DSERR_BUFFERLOST)
			return SOUND_ERR_BUFFER_LOST;

	}
	else
	{

		switch (handle->type)
		{

			case SOUND_FILE_TYPE_WAVE:
			{

				DWORD temp = 0;

				temp = fread (pt1, 1, size1, handle->pt);
				count += temp;
				temp = fread (pt2, 1, size2, handle->pt);
				count += temp;

				count /= handle->fmt.nBlockAlign;

			}
			break;

			case SOUND_FILE_TYPE_OGG:
			{

				DWORD loop = 0, limit = SOUND_OGG_BUFFER_MAX, sz = size1;
				long loop_temp = 0;
				char *ptx = pt1;
				char temp_v[SOUND_OGG_BUFFER_MAX];

				while (ptx)
				{

					loop = 0;

					while (loop < sz)
					{

						if (sz - loop < limit) limit = sz - loop;
						else limit = SOUND_OGG_BUFFER_MAX;
						loop_temp = ov_read (&handle->vf, temp_v, limit, SOUND_ENDIAN, handle->fmt.wBitsPerSample / 8, SOUND_SIGN_TYPE, &handle->ogg_current_section);

						if (loop_temp > 0)
						{
								
							CopyMemory (ptx+loop, temp_v, loop_temp);
							loop += loop_temp;

						}
						else if (loop_temp == 0)
							break;

					}

					count += loop;

					if (ptx == pt1) 
					{
						ptx = pt2;
						sz = size2;
					}
					else break;

				}

				count /= handle->fmt.nBlockAlign;

				break;

			}

		}

	}

	if (read != NULL) *read = count;
	if (FAILED(buf->Unlock((void*)pt1, size1, (void*)pt2, size2)))
	{
	 return SOUND_ERR_CANNOT_UNLOCK;}

	return SOUND_ERR_OK;

}

HRESULT	CDirectSoundBuffer::moveFromFile (sound_handle *handle, long position)
{

	if (!dsp) return SOUND_ERR_CANNOT_FIND_DS;

	switch (handle->type)
	{

		case SOUND_FILE_TYPE_WAVE:
			if (fseek (handle->pt, handle->start_position + (position * handle->fmt.nBlockAlign), SEEK_SET) != 0)
				 return SOUND_ERR_CANNOT_MOVE;
		break;

		case SOUND_FILE_TYPE_OGG:
			if (ov_seekable (&handle->vf) != 0)
			{
				ov_pcm_seek (&handle->vf, position);

			} else return SOUND_ERR_CANNOT_MOVE;
		break;

	}

	return SOUND_ERR_OK;

}

HRESULT	CDirectSoundBuffer::closeFromFile (sound_handle *handle)
{

	if (!dsp) return SOUND_ERR_CANNOT_FIND_DS;

	switch (handle->type)
	{

		case SOUND_FILE_TYPE_WAVE:
		break;

		case SOUND_FILE_TYPE_OGG:
			ov_clear (&handle->vf);
		break;

	}

	ZeroMemory (handle, sizeof(sound_handle));

	return SOUND_ERR_OK;

}

int CDirectSoundBuffer::load (FILE *fp, int index)
{

	sound_handle	handle;
	HRESULT			err = SOUND_ERR_OK;

	int loop = 0, loop2 = 1;

	if (!dsp) return SOUND_ERR_CANNOT_FIND_DS;
	if (index < 0 || index >= SOUND_MAX) index = 0;

	if ((err = openFromFile (fp, &handle)) != SOUND_ERR_OK) return err;

	for (loop = index; loop < SOUND_MAX; ++loop)
	{

		if (!list[loop][0])
		{

			if ((err = createBuffer ((IDirectSoundBuffer **)&list[loop][0], &handle.desc)) != SOUND_ERR_OK) return err;

			if ((err = readFromFile (list[loop][0], &handle, 0, handle.desc.dwBufferBytes / handle.fmt.nBlockAlign, SOUND_READ_ENTIRE, NULL)) != SOUND_ERR_OK) return err;

			for (loop2 = 1; loop2 < SOUND_COPY_MAX; ++loop2)
			{

				if (!list[loop][loop2])
					if ((err = duplicateBuffer(list[loop][0], (IDirectSoundBuffer **)&list[loop][loop2])) != SOUND_ERR_OK) return err;

			}

			closeFromFile (&handle);
			return loop;

		}

		if (loop == index) {closeFromFile (&handle);return SOUND_ERR_INDEX_COLLISION;}

	}

	closeFromFile (&handle);
	return SOUND_ERR_OUT_OF_LIMIT;

}

HRESULT CDirectSoundBuffer::close(int sound_index)
{

	int loop = 0;

	if (!dsp) return SOUND_ERR_CANNOT_FIND_DS;

	if (sound_index < 0 || sound_index >= SOUND_MAX) return SOUND_ERR_OUT_OF_INDEX;

	for (loop = 0; loop < SOUND_COPY_MAX; ++loop)
	{
		
		deleteBuffer (list[sound_index][loop]);
		list[sound_index][loop] = NULL;

	}

	return SOUND_ERR_OK;

}

HRESULT CDirectSoundBuffer::play(int sound_index, int sound_sub_index, unsigned char option, long pan_v, long vol_v)
{

	if (!dsp) return SOUND_ERR_CANNOT_FIND_DS;
	
	if (sound_index < 0 || 
		sound_index >= SOUND_MAX ||
		sound_sub_index < 0 || 
		sound_sub_index >= SOUND_COPY_MAX)
			return SOUND_ERR_OUT_OF_INDEX;

	if (!list[sound_index][sound_sub_index])
		return SOUND_ERR_NULL_BUFFER;

	if (SUCCEEDED(list[sound_index][sound_sub_index]->SetCurrentPosition(0)))
	{

		pan (sound_index, sound_sub_index, pan_v);
		volume (sound_index, sound_sub_index, vol_v);

		if (SUCCEEDED(list[sound_index][sound_sub_index]->Play(0,0,option)))
			return SOUND_ERR_OK;
		else return SOUND_ERR_CANNOT_PLAY;

	}

	return SOUND_ERR_CANNOT_SET_POSITION;

}

HRESULT	CDirectSoundBuffer::stop(int sound_index, int sound_sub_index)
{

	if (!dsp) return SOUND_ERR_CANNOT_FIND_DS;

	if (sound_index < 0 || 
		sound_index >= SOUND_MAX ||
		sound_sub_index < 0 || 
		sound_sub_index >= SOUND_COPY_MAX)
			return SOUND_ERR_OUT_OF_INDEX;

	if (!list[sound_index][sound_sub_index]) return SOUND_ERR_NULL_BUFFER;

	if (FAILED(list[sound_index][sound_sub_index]->Stop()))
		return SOUND_ERR_CANNOT_STOP;

	return SOUND_ERR_OK;

}

HRESULT	CDirectSoundBuffer::allStop(void)
{

	int	loop = 0, loop2 = 0;

	if (!dsp) return SOUND_ERR_CANNOT_FIND_DS;

	for (loop = 0; loop < SOUND_MAX; ++loop)
	{

		for (loop2 = 0; loop2 < SOUND_COPY_MAX; ++loop2)
		{
		
			if (isPlaying(loop, loop2))
				list[loop][loop2]->Stop();

		}

	}

	if (stream_stop == FALSE) stream_stop = TRUE;

	return SOUND_ERR_OK;

}

HRESULT	CDirectSoundBuffer::isPlaying(int sound_index, int sound_sub_index)
{

	DWORD status = 0;

	if (!dsp) return SOUND_ERR_CANNOT_FIND_DS;

	if (sound_index < 0 || 
		sound_index >= SOUND_MAX ||
		sound_sub_index < 0 || 
		sound_sub_index >= SOUND_COPY_MAX)
			return SOUND_ERR_OUT_OF_INDEX;

	if (list[sound_index][sound_sub_index])
		list[sound_index][sound_sub_index]->GetStatus(&status);
	else return SOUND_ERR_NULL_BUFFER;

	if (status == DSBSTATUS_PLAYING) return TRUE;
	return FALSE;

}

HRESULT CDirectSoundBuffer::pan (int sound_index, int sound_sub_index, long pan_v)
{

	if (!dsp) return SOUND_ERR_CANNOT_FIND_DS;

	if (sound_index < 0 || 
		sound_index >= SOUND_MAX ||
		sound_sub_index < 0 || 
		sound_sub_index >= SOUND_COPY_MAX)
			return SOUND_ERR_OUT_OF_INDEX;

	if (pan_v < SOUND_PAN_LEFT || pan_v >= SOUND_PAN_RIGHT) return SOUND_ERR_INVAILD_PAN;

	if (!list[sound_index][sound_sub_index]) return SOUND_ERR_NULL_BUFFER;

	if (FAILED(list[sound_index][sound_sub_index]->SetPan (pan_v)))
		return SOUND_ERR_CANNOT_SET_PAN;

	return SOUND_ERR_OK;

}

HRESULT CDirectSoundBuffer::volume (int sound_index, int sound_sub_index, long vol_v)
{

	if (!dsp) return SOUND_ERR_CANNOT_FIND_DS;

	if (sound_index < 0 || 
		sound_index >= SOUND_MAX ||
		sound_sub_index < 0 || 
		sound_sub_index >= SOUND_COPY_MAX)
			return SOUND_ERR_OUT_OF_INDEX;

	if (vol_v < SOUND_VOLUME_MIN || vol_v >= SOUND_VOLUME_MAX) return SOUND_ERR_INVAILD_VOLUME;

	if (!list[sound_index][sound_sub_index]) return SOUND_ERR_NULL_BUFFER;

	if (FAILED(list[sound_index][sound_sub_index]->SetVolume (vol_v)))
		return SOUND_ERR_CANNOT_SET_VOLUME;

	return SOUND_ERR_OK;

}

HRESULT	CDirectSoundBuffer::freq(int sound_index, int sound_sub_index, DWORD freq_v)
{

	if (!dsp) return SOUND_ERR_CANNOT_FIND_DS;

	if (sound_index < 0 || 
		sound_index >= SOUND_MAX ||
		sound_sub_index < 0 || 
		sound_sub_index >= SOUND_COPY_MAX)
			return SOUND_ERR_OUT_OF_INDEX;

	if (freq_v < SOUND_FREQ_MIN || freq_v >= SOUND_FREQ_MAX) return SOUND_ERR_INVAILD_VOLUME;

	if (!list[sound_index][sound_sub_index]) return SOUND_ERR_NULL_BUFFER;

	if (FAILED(list[sound_index][sound_sub_index]->SetFrequency (freq_v)))
		return SOUND_ERR_CANNOT_SET_VOLUME;

	return SOUND_ERR_OK;

}

#ifdef _MT
COGGSTREAM::COGGSTREAM(HWND h, CDirectSoundBuffer *s, DWORD s_buffer, DWORD g_buffer, DWORD o_buffer)
#else
COGGSTREAM::COGGSTREAM(HWND h, CDirectSoundBuffer *s, DWORD s_buffer, DWORD g_buffer, DWORD o_buffer, DWORD index)
#endif
{

#ifdef _MT
	streamInitialize (h, s, s_buffer, g_buffer, o_buffer);
#else
	streamInitialize (h, s, s_buffer, g_buffer, o_buffer, index);
#endif

}

COGGSTREAM::~COGGSTREAM(void)
{

	streamClose();

#ifdef _MT
	DeleteCriticalSection (&cs);
#else
	KillTimer (hWnd, stream_timer_handle);
#endif

	snd = NULL;
	buffer_size = 0;
	gap = 0;
	read_once = 0;

	state = STREAM_STATE_NONE;

}
#ifdef _MT
int	COGGSTREAM::streamInitialize(HWND h, CDirectSoundBuffer *s, DWORD s_buffer, DWORD g_buffer, DWORD o_buffer)
#else
int	COGGSTREAM::streamInitialize(HWND h, CDirectSoundBuffer *s, DWORD s_buffer, DWORD g_buffer, DWORD o_buffer, DWORD index)
#endif
{

	if (!h) return STREAM_ERR_UNAVAILABLE_HWND;
	if (!s) return STREAM_ERR_UNAVAILABLE_SOUND_OBJECT;
#ifndef _MT
	if (!index) return STREAM_ERR_UNAVAILABLE_TIMER_INDEX;
	stream_timer_handle = index;
#endif

	hWnd = h;
	snd = s;

#ifdef _MT
	timer = 0;
	InitializeCriticalSection (&cs);
#else
	SetTimer(hWnd, stream_timer_handle, 42, NULL);		// 1000 / 24 = 42
#endif

	state = STREAM_STATE_NONE;

	if (!snd->dsp) return SOUND_ERR_CANNOT_FIND_DS;

	buffer_size = s_buffer;
	gap = g_buffer;
	read_once = o_buffer;

	state = STREAM_STATE_INITIALIZE;

	return STREAM_ERR_OK;

}

HRESULT	COGGSTREAM::streamLoad(FILE *fp, position_set *p)
{

	int err = 0;

	if (!snd->dsp) return SOUND_ERR_CANNOT_FIND_DS;

	//	set stream
	if (state == STREAM_STATE_INITIALIZE)
	{

		state = STREAM_STATE_LOAD;

		if ((err = snd->openFromFile (fp, &handle)) != SOUND_ERR_OK)
			return err;

		// create buffer
		size = handle.desc.dwBufferBytes / handle.fmt.nBlockAlign;
		read_size = 0;
		play_size = 0;
		old_play = 0;
		option = 0;
		temp_play_size = 0;
		if (p)
		{
			streamSetCurrentPosition (p);
		}
		else 
		{
			
			pos.index = 0;
			pos.start_pos = 0;
			pos.loop_start_pos = 0;
			pos.option = STREAM_PERMIT_NEXT_OFF;
			pos.size = handle.desc.dwBufferBytes / handle.fmt.nBlockAlign;
			next_pos = NULL;

		}
		cur_play = 0;
		cur_write = 0;

		// insert buffer size
		handle.desc.dwBufferBytes = buffer_size * handle.fmt.nBlockAlign;

		if ((err = snd->createBuffer ((IDirectSoundBuffer **)&buffer, &handle.desc)) != SOUND_ERR_OK)
			return err;

		if ((err = snd->readFromFile (buffer, &handle, cur_write, min(buffer_size, size), 0, &read_size)) != SOUND_ERR_OK) 
			return err;
		cur_write = (cur_write + read_size) % buffer_size;

		state = STREAM_STATE_READY;

		return SOUND_ERR_OK;

	}

	return SOUND_ERR_CANNOT_CREATE_STREAM;

}

HRESULT	COGGSTREAM::streamPlay(DWORD o)
{

	if (!snd->dsp) return SOUND_ERR_CANNOT_FIND_DS;

	if (state != STREAM_STATE_READY) return STREAM_ERR_NOT_READY;

	option = o;

#ifdef _MT
		// create thread
		if ((thread_handle = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)streamUpdate, (void *)this, CREATE_SUSPENDED, &thread_id)) == NULL)
			return SOUND_ERR_CANNOT_CREATE_THREAD;
#endif

	buffer->SetCurrentPosition(0);
	buffer->Play(0,0,SOUND_PLAY_REPEAT);
#ifdef _MT
	ResumeThread (thread_handle);
#endif
	state = STREAM_STATE_PLAY;

	return SOUND_ERR_OK;

}

HRESULT	COGGSTREAM::streamStop(void)
{

	if (!snd->dsp) return SOUND_ERR_CANNOT_FIND_DS;

	if (!streamIsPlaying()) return STREAM_ERR_CANNOT_STOP;
	buffer->Stop();
//#ifdef _MT
//	SuspendThread (thread_handle);
//#endif
	state = STREAM_STATE_STOP;

	return SOUND_ERR_OK;

}

HRESULT	COGGSTREAM::streamPause(void)
{

	if (!snd->dsp) return SOUND_ERR_CANNOT_FIND_DS;

	if (!streamIsPlaying()) return STREAM_ERR_CANNOT_PAUSE;
	buffer->Stop();
//#ifdef _MT
//	SuspendThread (thread_handle);
//#endif
	state = STREAM_STATE_PAUSE;

	return SOUND_ERR_OK;

}

HRESULT	COGGSTREAM::streamResume(void)
{

	if (!snd->dsp) return SOUND_ERR_CANNOT_FIND_DS;

	if (state != STREAM_STATE_PAUSE) return STREAM_ERR_CANNOT_RESUME;

	buffer->Play(0,0,SOUND_PLAY_REPEAT);
//#ifdef _MT
//	ResumeThread (thread_handle);
//#endif

	state = STREAM_STATE_PLAY;

	return SOUND_ERR_OK;

}

HRESULT	COGGSTREAM::streamClose(void)
{

	if (!snd->dsp) return SOUND_ERR_CANNOT_FIND_DS;

	if (streamIsPlaying())
		streamStop();

#ifdef _MT
	CloseHandle (thread_handle);
#endif

	snd->closeFromFile (&handle);

	state = STREAM_STATE_INITIALIZE;

	return SOUND_ERR_OK;

}

void COGGSTREAM::streamUpdate(void *s)
{

	DWORD read = 0;
	COGGSTREAM *str = (COGGSTREAM *)s;

#ifdef _MT
	while (str->state != STREAM_STATE_CLOSE || str->state != STREAM_STATE_LOAD)
#else
	if (str->state != STREAM_STATE_CLOSE || str->state != STREAM_STATE_LOAD)
#endif
	{
#ifdef _MT
		while (GetTickCount() - str->timer >= 42 && str->streamIsPlaying() )
#else
		if (str->streamIsPlaying() )
#endif
		{

#ifdef _MT
			str->timer = GetTickCount();
#endif
			if (str->snd->stream_stop == TRUE)
			{
				
				str->streamStop();
				str->snd->stream_stop = FALSE;

			}
			//check event
			str->old_play = str->cur_play;
// ENTER C/S
#ifdef _MT
EnterCriticalSection (&str->cs);
#endif
			str->buffer->GetCurrentPosition(&str->cur_play, NULL);
			str->cur_play /= str->handle.fmt.nBlockAlign;
// LEAVE C/S
#ifdef _MT
LeaveCriticalSection (&str->cs);
#endif
			if (str->old_play > str->cur_play)
				str->play_size += str->buffer_size - str->old_play + str->cur_play;
			else 
				str->play_size += str->cur_play - str->old_play;


			if (str->state == STREAM_STATE_WAIT_FOR_END && str->play_size >= str->pos.size)
			{

// ENTER C/S
#ifdef _MT
EnterCriticalSection (&str->cs);
#endif
			str->streamStop();
// LEAVE C/S
#ifdef _MT
LeaveCriticalSection (&str->cs);
#endif

#ifdef _MT
			break;}
#else
			}
#endif

			else if (
				
					abs((long)((str->play_size + str->temp_play_size) - str->read_size)) < str->gap
					&& str->state != STREAM_STATE_WAIT_FOR_END)
			{

				//update buffer

// ENTER C/S
#ifdef _MT
EnterCriticalSection (&str->cs);
#endif
				str->snd->readFromFile (str->buffer, &str->handle, str->cur_write, min(str->read_once, str->pos.size - str->read_size), 0, &read);
// LEAVE C/S
#ifdef _MT
LeaveCriticalSection (&str->cs);
#endif
				
				str->read_size += read;
				str->cur_write = (str->cur_write + read) % str->buffer_size;

			}

			if ((str->pos.option == STREAM_PERMIT_NEXT_NOW || str->read_size >= str->pos.size) && str->state != STREAM_STATE_WAIT_FOR_END ) //break;
			{

				switch (str->option)
				{

					case SOUND_PLAY_ONCE:
						str->state = STREAM_STATE_WAIT_FOR_END;
						break;

					case SOUND_PLAY_REPEAT:
						// rewind
// ENTER C/S
#ifdef _MT
EnterCriticalSection (&str->cs);
#endif
						str->temp_play_size = (long)((long)(str->play_size+str->temp_play_size) - str->read_size);
						str->read_size = 0;
						str->play_size = 0;

						if (str->next_pos != NULL && (str->pos.option == STREAM_PERMIT_NEXT_ON || str->pos.option == STREAM_PERMIT_NEXT_NOW))
							// 새 포지션 정보를 갱신한다.
						{
							
							str->streamSetCurrentPosition (str->next_pos);

						} 
						else 
							// 현재 포지션 정보를 수정해서 loop
						{
							
							str->pos.start_pos = str->pos.loop_start_pos;
							str->pos.size -= (str->pos.loop_start_pos - str->pos.start_pos);

						}

						str->snd->moveFromFile (&str->handle, str->pos.start_pos);//-(long)(str->read_size), SOUND_MOVE_FROM_CURRENT);

// LEAVE C/S
#ifdef _MT
LeaveCriticalSection (&str->cs);
#endif
						break;

					default:
						break;

				}

			}
	
//

		}

	}
	
}

HRESULT COGGSTREAM::streamPan (long pan_v)
{

	if (!snd->dsp) return SOUND_ERR_CANNOT_FIND_DS;

	if (pan_v < SOUND_PAN_LEFT || pan_v >= SOUND_PAN_RIGHT) return SOUND_ERR_INVAILD_PAN;

	if (!streamIsPlaying() || FAILED(buffer->SetPan (pan_v)))
		return SOUND_ERR_CANNOT_SET_PAN;

	return SOUND_ERR_OK;

}

HRESULT COGGSTREAM::streamVolume (long vol_v)
{

	if (!snd->dsp) return SOUND_ERR_CANNOT_FIND_DS;

	if (vol_v < SOUND_VOLUME_MIN || vol_v >= SOUND_VOLUME_MAX) return SOUND_ERR_INVAILD_VOLUME;

	if (!streamIsPlaying() || buffer == NULL || FAILED(buffer->SetVolume (vol_v)))
		return SOUND_ERR_CANNOT_SET_VOLUME;

	return SOUND_ERR_OK;

}

HRESULT COGGSTREAM::streamSetNextPosition(position_set *p)
{

	if (!snd->dsp) return SOUND_ERR_CANNOT_FIND_DS;

	if (p)
		next_pos = p;
	else STREAM_ERR_UNAVAILABLE_POSITION_SET;

	return STREAM_ERR_OK;

}

HRESULT COGGSTREAM::streamSetCurrentPosition(position_set *p)
{

	if (!snd->dsp) return SOUND_ERR_CANNOT_FIND_DS;

	if (p)
	{
		CopyMemory (&pos, p, sizeof(position_set));
		next_pos = pos.next;
	}
	else STREAM_ERR_UNAVAILABLE_POSITION_SET;

	return STREAM_ERR_OK;

}

HRESULT	COGGSTREAM::streamPermitNextPosition(DWORD flag)
{

	if (!snd->dsp) return SOUND_ERR_CANNOT_FIND_DS;

	pos.option = flag;

	return STREAM_ERR_OK;

}