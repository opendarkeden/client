//----------------------------------------------------------------------
// SoundNode.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "SoundNode.h"

extern DWORD	g_CurrentTime;

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Set
//----------------------------------------------------------------------
void			
SOUND_NODE::Set(TYPE_SOUNDID sid, DWORD delay, int x, int y)
{
	m_PlayTime	= g_CurrentTime + delay;
	m_SoundID	= sid;
	m_X			= x;
	m_Y			= y;
}
