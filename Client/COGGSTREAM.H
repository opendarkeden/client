#ifndef __COGGSTREAM_H__
#define __COGGSTREAM_H__

#include <stdio.h>

class CDirectSoundBuffer;

struct tag_position_set {
	int position;
	// Add other fields as needed
};

class COGGSTREAM
{
public:
	COGGSTREAM(void* pDirectSound, CDirectSoundBuffer* pBuffer,
	           unsigned long ulMin, unsigned long ulMax,
	           unsigned long ulFlags, unsigned long ulBufferSize);
	~COGGSTREAM();

	bool streamLoad(FILE* file, tag_position_set* pos);
	void streamClose();
	bool streamPlay(unsigned long ulFlags);
	bool streamUpdate(void* pBuffer);
	long streamVolume(long lVolume);

private:
	CDirectSoundBuffer* m_pBuffer;
	bool m_bPlaying;
};

#endif /* __COGGSTREAM_H__ */
