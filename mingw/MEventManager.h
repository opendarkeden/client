//----------------------------------------------------------------------
// MEventManager.h
//----------------------------------------------------------------------

#ifndef __MEVENT_MANAGER_H__
#define __MEVENT_MANAGER_H__

#pragma warning(disable:4786)

#include "ctypetable.h"
#include "CDirectDrawSurface.h"

#define	EVENTFLAG_SHOW_STRING				0x00000001		// 화면에 string를 표시한다
#define EVENTFLAG_SHOW_DELAY				0x00000002		// Delay타임을 화면에 표시한다
#define	EVENTFLAG_SHOW_DELAY_STRING			0x00000003		// Delay타임을 넣는 string를 표시한다
#define	EVENTFLAG_SHAKE_SCREEN				0x00000004		// event동안 화면을 흔든다. parameter3을 사용한다
#define	EVENTFLAG_FADE_SCREEN				0x00000008		// event동안 화면색을 바꾼다. parameter2를 사용한다
#define	EVENTFLAG_NOT_DRAW_BACKGROUND		0x00000010		// event동안 배경을 찍지 않는다
#define	EVENTFLAG_EVENT_BACKGROUND			0x00000020		// event동안 이벤트용 뒷배경을 출력한다
#define	EVENTFLAG_ONLY_EVENT_BACKGROUND		0x00000030		// event동안 배경을 찍지 않고 뒷배경을 출력한다
#define EVENTFLAG_QUEST_INFO				0x00000040		// 심플 퀘스트 관련
#define EVENTFLAG_NOT_DRAW_UI				0x00000080		// 인터페이스 안찍음
#define EVENTFLAG_NOT_DRAW_CREATURE			0x00000100		// 크리쳐 안찍음
#define EVENTFLAG_NOT_DRAW_INFORMATION		0x00000200		// 화면 정보 안찍음
#define EVENTFLAG_NOT_DRAW_CREATURE_SHADOW	0x00000400		// 크리쳐 그림자 안찍음
#define EVENTFLAG_NOT_DRAW_ITEM				0x00000800		// 아이템 안찍음
#define EVENTFLAG_NOT_DRAW_EFFECT			0x00001000		// 이펙트 안찍음
#define EVENTFLAG_NOT_DRAW_MOUSE_POINTER	0x00002000		// 마우스 포인터 안찍음
#define EVENTFLAG_DENY_INPUT_MOUSE			0x00004000		// 마우스 입력 안받음 위험!
#define EVENTFLAG_DENY_INPUT_KEYBOARD		0x00008000		// 키보드 입력 안받음 위험!
#define EVENTFLAG_DENY_INPUT				0x0000c000		// 입력 안받음 위험!
#define EVENTFLAG_NOT_FADE_SCREEN			0x00010000		// DrawFade안함
#define EVENTFLAG_NOT_PLAY_SOUND			0x00020000		// SoundPlay안함
#define EVENTFLAG_CLOUD_BACKGROUND			0x00040000		// 타일 뒤쪽에 구름 이미지를 뿌려준다..

enum EVENT_ID
{
	EVENTID_NULL,
	EVENTID_HALLUCINATION,			// Hallu가 걸렸다
	EVENTID_KICK_OUT_FROM_ZONE,		// 존에서 추방하는 시간(초)
	EVENTID_CONTINUAL_GROUND_ATTACK,	// 존에서 지속적인 공격이 일어나는 경우(초)
	EVENTID_COMBAT_MASTER,				// 마스터와 싸운다
	EVENTID_METEOR,						// 메테오 시작할때 색, 배경 바꾸기
	EVENTID_METEOR_SHAKE,				// 메테오 땅에 떨어질때 흔들기
	EVENTID_PREMIUM_HALF,				// 프리미엄 사용자는 포션/혈청 반값
	EVENTID_TAX_CHANGE,					// 상점 세금 바뀜
	EVENTID_LOGOUT,						// ?초 후에 로그아웃 합니다.
	EVENTID_LOVECHAIN,					// ?초 후에 이동 합니다.
	EVENTID_FORCE_LOGOUT_BY_PREMIUM,	// 유료 사용자가 아니라서 몇초후에 로그아웃됨 
	EVENTID_MONSTER_KILL_QUEST,			// 몬스터 죽이기 심플 퀘스트 관련
	EVENTID_OUSTERS_FIN,
	EVENTID_QUEST_FIN,					// 퀘스트 끝났을때
	EVENTID_POUR_ITEM,					// %d 초 후에 깃발지역에서 아이템이 터집니다.
	EVENTID_RESURRECT,					// %d 초 후에 부활합니다.
	EVENTID_GDR_PRESENT,				// 질드레 연출 10초간 흔들림->10초간 어두워짐
	EVENTID_BG_CLOUD,					// 백그라운드에 구름..-_-타일 뒷쪽에..
	EVENTID_WAR_EFFECT,
	//add by zdj
	EVENTID_ADVANCEMENT_QUEST_ENDING,
	EVENTID_MAX,
};

enum EVENT_TYPE
{
	EVENTTYPE_NULL,		// 
	EVENTTYPE_ZONE,		// Zone에 종속 되며 존이 바뀌면 사라진다.
	EVENTTYPE_EFFECT,	// Effect에 종속 되며 Effect가 사라지면 사라진다. parameter1을 사용한다

	EVENTTYPE_MAX
};	

enum EVENTBACKGROUND_ID
{
	EVENTBACKGROUNDID_COSMOS,
	EVENTBACKGROUNDID_OUSTERS_SLAYER,
	EVENTBACKGROUNDID_OUSTERS_VAMPIRE,
	EVENTBACKGROUNDID_QUEST_2,
	EVENTBACKGROUNDID_CLOUD,
	
	EVENTBACKGROUNDID_MAX,
};

class MEvent
{
	public:
		MEvent();
		~MEvent();

	public:
		EVENT_ID				eventID;
		EVENT_TYPE				eventType;
		DWORD					eventStartTickCount;
		int						eventDelay;			// 단위 : 1/1000초, event의 지속시간이며 0이 되면 이벤트가 종료된다. -1 : delay없음
		int						showTime;
		int						totalTime;
		DWORD					eventFlag;			// event의 Flag
		DWORD					parameter1;			// EVENT_TYPE에 종속(effect id)
		DWORD					parameter2;			// FADE_SCREEN에 종속
		DWORD					parameter3;			// SHAKE_SCREEN에 종속
		DWORD					parameter4;			// EVENT_BACKGROUND에 종속
		std::vector<int>		m_StringsID;		// GameString의 StringID가 들어간다.

		const bool IsShowTime() const;
};

class MEventManager
{
	public:
		MEventManager();
		~MEventManager();

		void			ProcessEvent();

		void			AddEvent(MEvent &event);

		const bool		IsEmptyEvent() const				{ return m_Events.empty(); }
		const int		GetEventCount() const				{ return m_Events.size(); }
		const MEvent*	GetEvent(EVENT_ID id);

		const bool		IsEmptyEventByFlag(DWORD flag);
		const MEvent*	GetEventByFlag(DWORD flag, int count = 0);
		const int		GetEventCountByFlag(DWORD flag);

		bool			IsEvent(EVENT_ID id);

		void			RemoveEvent(EVENT_ID id);
		void			RemoveAllEvent();
		void			RemoveAllEventByType(EVENT_TYPE type);

		bool					AssertEventBackground(EVENTBACKGROUND_ID id);
		CDirectDrawSurface*		GetEventBackground(EVENTBACKGROUND_ID id) { AssertEventBackground(id); return &m_EventBackGround[id]; }
		
		typedef std::map<EVENT_ID, MEvent>		EVENT_MAP;
		
	protected :
		EVENT_MAP		m_Events;
		CTypeTable<CDirectDrawSurface>	m_EventBackGround;	// 이벤트 배경그림...
		
};

//----------------------------------------------------------------------
// global
//----------------------------------------------------------------------
extern MEventManager* g_pEventManager;

#endif