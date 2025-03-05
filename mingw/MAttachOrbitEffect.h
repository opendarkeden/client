//----------------------------------------------------------------------
// MAttachOrbitEffect.h
//----------------------------------------------------------------------
//
// 캐릭터에 붙어서 원형 궤도로 붙어다닌다.
//
// 초당 16 frame으로 그려주니까..
// 16 step으로 돌면... 1초에 한 바퀴이다.
// 2초에 한 바퀴 정도가 어떨까?
//
// 속도 조절도 따로 하면 좋겠지만.. 일단 간다~
// 속도 조절 할려면.. step을 좀 많이 만들어놓고.. 
// 한 번에 몇 step을 이동하는가?의 값을 두고.. 
// NextOrbitStep()에서 그만큼 증가시키면 될 것이다.
//
// 돌아가는 방향을 바꿀려면 NextOrbitStep()에서 + 대신 - 를 해주면 될 것이당.
//
// 돌아가는 크기를 바꿀려면 InitOrbitPosition()에서
// ...widthHalf/...heightHalf를 바꿔야한다.
// 실행중에 계산하면 부하가 있으므로.. 여러가지 크기를 쓰고 싶으면
// 아예 table을 여러개 두는개 좋지 싶다.
//
//----------------------------------------------------------------------

#ifndef	__MATTACH_ORBIT_EFFECT_H__
#define	__MATTACH_ORBIT_EFFECT_H__

#pragma warning(disable:4786)

// 이 값 바꿔줄때 NextOrbitStep()도 고려해야 한다.
#define	MAX_EFFECT_ORBIT_TYPE		3
#define	MAX_EFFECT_ORBIT_STEP		64

#include "MAttachEffect.h"



class MAttachOrbitEffect : public MAttachEffect {
	public :
		MAttachOrbitEffect(TYPE_EFFECTSPRITETYPE type, DWORD last, int orbit_type, int orbit_step = -1, DWORD linkCount=MAX_LINKCOUNT);
		~MAttachOrbitEffect();

		virtual EFFECT_TYPE		GetEffectType()	const	{ return EFFECT_ATTACH_ORBIT; }
		
		//--------------------------------------------------------
		// 한 번의 Update에 호출될 함수..
		//--------------------------------------------------------
		virtual bool			Update();

		//void					NextOrbitStep()		{ m_OrbitStep = ++m_OrbitStep % MAX_EFFECT_ORBIT_STEP; }
		void					NextOrbitStep()		{ m_OrbitStep = ++m_OrbitStep & 0x0000003F; }

		//--------------------------------------------------------
		// 좌표값 + Orbit보정값
		//--------------------------------------------------------
		virtual int		GetPixelX() const	{ return (int)m_PixelX + s_OrbitPosition[m_Type][m_OrbitStep].x; }
		virtual int		GetPixelY() const	{ return (int)m_PixelY + s_OrbitPosition[m_Type][m_OrbitStep].y; }

		//--------------------------------------------------------
		// Get Orbit X,Y
		//--------------------------------------------------------
		const POINT&	GetOrbitPosition() const	{ return s_OrbitPosition[m_Type][m_OrbitStep]; }
		
		//--------------------------------------------------------
		// Init OrbitPosition
		//--------------------------------------------------------
		static void		InitOrbitPosition();
	
	public :
		bool					m_bRun;
		int						m_OrbitStep;		// 몇번째 궤도 좌표인가?
		int						m_Type;

		// 보정 좌표
		static POINT			s_OrbitPosition[MAX_EFFECT_ORBIT_TYPE][MAX_EFFECT_ORBIT_STEP];		
};

// list정의
typedef	std::list<MAttachEffect*>	ATTACHEFFECT_LIST;

#endif

