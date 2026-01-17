//-----------------------------------------------------------------------------
// MZoneSoundManager.h
//-----------------------------------------------------------------------------
// Zone에서 소리가 나고 있는 도중에..
// Player가 움직여버리면.. 소리들의 방향과 크기가 달라진다.
// 순간적으로 나는 소리들은 무시하더라도...
// 반복되는 소리같은 것들이나.. 좀 긴것들은.. 바꿔줘야한다.. 냠.
//
// ZONESOUND_NODE는 현재 play되고 있는 소리 하나에 대한 정보이고
// MZoneSoundManager는 그 소리들을 관리한다..
//-----------------------------------------------------------------------------

#ifndef	__MZONESOUNDMANAGER_H__
#define __MZONESOUNDMANAGER_H__

#include "MZoneSound.h"
#include "CTypeMap.h"
#include "CDirectSound.h"
#ifdef PLATFORM_WINDOWS
#include <DSound.h>
#endif
#include <map>

#include <fstream>
using namespace std;

//-----------------------------------------------------------------------------
// ZONESOUND_NODE
//-----------------------------------------------------------------------------
// 현재 Play되고 있는 소리에 관한 정보
// m_pBuffer는 g_pSoundTable의 Buffer를 Duplicate해서 갖고 있는다.
//-----------------------------------------------------------------------------
class ZONESOUND_NODE
{
	public :
		ZONESOUND_NODE(TYPE_SOUNDID id=SOUNDID_NULL);
		~ZONESOUND_NODE();

		//---------------------------------------------------------------
		// Get
		//---------------------------------------------------------------
		TYPE_SOUNDID			GetSoundID() const		{ return m_SoundID; }
		LPDIRECTSOUNDBUFFER		GetBuffer() const		{ return m_pBuffer; }
		int						GetX() const			{ return m_X; }
		int						GetY() const			{ return m_Y; }
		bool					IsLoop() const			{ return m_bLoop; }

		//---------------------------------------------------------------
		// Play
		//---------------------------------------------------------------
		void					Play(int x=0, int y=0, bool bLoop=false);
		void					Stop();
		
		//---------------------------------------------------------------
		// Loop
		//---------------------------------------------------------------
		void					StopLoop();			
		bool					IsContinueLoop() const	{ return m_bContinueLoop; }
		void					SetContinueLoop()		{ m_bContinueLoop = true; }
		void					UnSetContinueLoop()		{ m_bContinueLoop = false; }
		
		//---------------------------------------------------------------
		// File I/O
		//---------------------------------------------------------------
		void					SaveToFile(std::ofstream& file);
		void					LoadFromFile(std::ifstream& file);

	protected :
		TYPE_SOUNDID				m_SoundID;				// Sound ID
		LPDIRECTSOUNDBUFFER			m_pBuffer;				// buffer
		int							m_X, m_Y;				// 소리가 출력되는 좌표
		bool						m_bLoop;				// 반복 여부
		bool						m_bContinueLoop;		// 계속 반복해서 소리낼까?
		int							m_playerX, m_playerY;	// 이 소리가 날때의 player의 좌표
};


//-----------------------------------------------------------------------------
// MZoneSoundTable
//-----------------------------------------------------------------------------
// Zone에 존재하는 소리들에 대한 정보
//-----------------------------------------------------------------------------
class MZoneSoundTable : public CTypeMap<ZONESOUND_INFO>
{
	public :
		MZoneSoundTable();
		~MZoneSoundTable();
};

//-----------------------------------------------------------------------------
// MZoneSoundManager
//-----------------------------------------------------------------------------
// id==ZoneSoundID. 현재 Play되고 있는 소리들 모음
//-----------------------------------------------------------------------------
class MZoneSoundManager : public CTypeMap<ZONESOUND_NODE>
{
	public :
		MZoneSoundManager();
		~MZoneSoundManager();

		//---------------------------------------------------------------------
		// Update Sound
		//---------------------------------------------------------------------		
		void			UpdateSound();

	protected :
		TYPE_SECTORPOSITION		m_LastX, m_LastY;
		DWORD					m_LastUpdateTime;
};

//-----------------------------------------------------------------------------
// global
//-----------------------------------------------------------------------------
extern MZoneSoundTable*		g_pZoneSoundTable;

extern MZoneSoundManager*	g_pZoneSoundManager;

#endif