#include "COGGSTREAM.h"
#include "DXLib/CDirectSound.h"
#include "basic/Platform.h"

COGGSTREAM::COGGSTREAM(void* pDirectSound, CDirectSoundBuffer* pBuffer,
                       unsigned long ulMin, unsigned long ulMax,
                       unsigned long ulFlags, unsigned long ulBufferSize)
{
	m_pBuffer = pBuffer;
	m_bPlaying = false;
	(void)pDirectSound; (void)ulMin; (void)ulMax; (void)ulFlags; (void)ulBufferSize;
}

COGGSTREAM::~COGGSTREAM()
{
	streamClose();
}

bool COGGSTREAM::streamLoad(FILE* file, tag_position_set* pos)
{
	// OGG stream loading - not implemented for SDL backend
	(void)file; (void)pos;
	return false;
}

void COGGSTREAM::streamClose()
{
	m_bPlaying = false;
}

bool COGGSTREAM::streamPlay(unsigned long ulFlags)
{
	m_bPlaying = true;
	(void)ulFlags;
	return true;
}

bool COGGSTREAM::streamUpdate(void* pBuffer)
{
	(void)pBuffer;
	return m_bPlaying;
}

long COGGSTREAM::streamVolume(long lVolume)
{
	(void)lVolume;
	return 0;
}
