//----------------------------------------------------------------------
// CDirectSound.h
//----------------------------------------------------------------------

#ifndef __CDirectSound_H__
#define __CDirectSound_H__

#pragma warning(disable:4786)

#include <Windows.h>
#include <MMSystem.h>
#include <DSound.h>
#include <list>

typedef	std::list<LPDIRECTSOUNDBUFFER>	LPDIRECTSOUNDBUFFER_LIST;

class CDirectSound
{

	public:		// 함수
		CDirectSound();
		~CDirectSound();

		//---------------------------------------------------------
		// Init / Release
		//---------------------------------------------------------
		bool					Init(HWND);									// 초기화 
		void					Release();									// 제거
		void					ReleaseDuplicateBuffer();

		bool					IsInit() const		{ return m_bInit; }

		//---------------------------------------------------------
		// Load / Release / Duplicate
		//---------------------------------------------------------
		LPDIRECTSOUNDBUFFER		LoadWav(LPSTR filename);					// 화일 로드(*,wav)
		LPDIRECTSOUNDBUFFER		CreateBuffer(LPVOID sdat, DWORD size, DWORD caps, LPWAVEFORMATEX wfx);
		void					Release(LPDIRECTSOUNDBUFFER);				// 사운드 버퍼 제거
		LPDIRECTSOUNDBUFFER		DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER, bool bAutoRelease=true);	// 사운드 버퍼 복사	

		//---------------------------------------------------------
		// Play / Stop
		//---------------------------------------------------------
		bool					IsPlay(LPDIRECTSOUNDBUFFER) const;								// 연주중인가?
		bool					NewPlay(LPDIRECTSOUNDBUFFER, bool bLoop=false);	// 사운드 플레이(처음부터 다시 시작)
		bool					Play(LPDIRECTSOUNDBUFFER, bool bLoop=false, bool bDuplicate=true);		// 사운드 플레이(끝날때까지 기다림)
		bool					Stop(LPDIRECTSOUNDBUFFER);					// 사운드 멈춤
		void					ReleaseTerminatedDuplicateBuffer();

		//---------------------------------------------------------
		// Mute
		//---------------------------------------------------------
		bool					IsMute() const	{ return m_bMute; }
		void					SetMute()		{ m_bMute = true; }
		void					UnSetMute()		{ m_bMute = false; }

		//---------------------------------------------------------
		// Frequency
		//---------------------------------------------------------
		bool					AddFrequency(LPDIRECTSOUNDBUFFER, int);		// 주파수 올리기				
		bool					SubFrequency(LPDIRECTSOUNDBUFFER, int);		// 주파수 내리기				
	
		//---------------------------------------------------------
		// Volume
		//---------------------------------------------------------
		bool					SetMaxVolume(LPDIRECTSOUNDBUFFER buffer);
		bool					AddVolume(LPDIRECTSOUNDBUFFER, int);		// 볼륨 높임
		bool					SubVolume(LPDIRECTSOUNDBUFFER, int);		// 볼륨 내림				
		bool					SubVolumeFromMax(LPDIRECTSOUNDBUFFER, int);	// Max부터 볼륨 내림				
		void					SetVolumeLimit(LONG volume);
		LONG					GetVolumeLimit() const	{ return m_MaxVolume; }

		//---------------------------------------------------------
		// Pan
		//---------------------------------------------------------
		bool					RightPan(LPDIRECTSOUNDBUFFER, int);			// 오른쪽 팬
		bool					LeftPan(LPDIRECTSOUNDBUFFER, int);			// 왼쪽 팬
		bool					CenterToRightPan(LPDIRECTSOUNDBUFFER, int);			// center부터 오른쪽 팬
		bool					CenterToLeftPan(LPDIRECTSOUNDBUFFER, int);			// center부터 왼쪽 팬
		bool					CenterPan(LPDIRECTSOUNDBUFFER);				// 가운데 팬				
		bool					ChangePan(LPDIRECTSOUNDBUFFER buffer, int pan);	// -10000 ~ 10000

		LPDIRECTSOUND			GetDS() const		{ return m_pDS;	}

		

	protected:
		bool					DirectSoundFailed(const char *str);		


	protected :
		LPDIRECTSOUND			m_pDS;										// 다이렉트 사운드 오브젝트
		bool					m_bInit;

		bool					m_bMute;			// 소리를 내지 않는다.

		LONG					m_MaxVolume;		// 현재의 최대 소리 크기

		LPDIRECTSOUNDBUFFER_LIST	m_listDuplicatedBuffer;


	friend class CDirectMusic;
	friend class CDirectSoundStream;
};

extern	CDirectSound		g_DXSound;

#endif