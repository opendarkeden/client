//----------------------------------------------------------------------
// MEffect.h
//----------------------------------------------------------------------
// 화면에 출력되는 Effect
//----------------------------------------------------------------------
//
// < Effect란? >
//   - 화면에서 보여지는 효과.
//   - 캐릭터, 아이템, 건물, 장애물.. 등을 제외한 것들 중(뭐가 있을래나)에서
//     Animation이 되는 것들을 말한다.
//   - Effect는 모두 CAlphaSprite를 이용해서 알파채널블렌딩~으로 표현한다.
//   - 캐릭터가 칼~을 휘둘렀을 때의 검기나 신부의 오오라 같은건
//     여기서 말하는 Effect에 포함되지 않는다.
//   - Effect는 독립적인 하나의 객체라고 할 수 있다.
//
//
// < Effect의 종류 >
//   = 미사일 류
//     - Pixel단위의 움직임.
//     - 발사위치부터 --> 목표위치까지 Animation되면서 
//       일정 속도(pixel)로 이동함
//     - 유도미사일인 경우와 아닌 경우가 있음.
//     - 장애물, 캐릭터, 건물 등에 부딪히면 소멸될 수도 있음.
//     - 목표위치에 도착하면 소멸.
//            
//   = Tile마법 류
//     - 특정 Tile에서 Animation됨.
//     - 크기는 Tile보다 커도 상관없음.
//     - 출력 시점은 그 Tile에서 가장 나중에 출력한다.
//     - 일정 Frame이 지나면 소멸.
//         
//----------------------------------------------------------------------
// - 모든 Effect는 Zone의 한 Sector에는 속해야 할 것이다.
//   출력 순서가 y좌표로 Sort돼야하기 때문에, Sector의 Object출력에 맞추어
//   출력하기 위해서.. Sector에 넣기로 결정..
//
// - Effect는 일정 시간 지속된다. 
//   즉, Counter를 두어서 0이 되면 끝나도록...
//----------------------------------------------------------------------
//
//              Effect는 File로 저장되지 않는다.
//
//----------------------------------------------------------------------
/* class hierarchy


      MEffect --+-- MMovingEffect ---- ....	            
								


   MEffect : Tile에 고정된 Effect
*/
//----------------------------------------------------------------------

#ifndef	__MEFFECT_H__
#define	__MEFFECT_H__

#pragma warning(disable:4786)

#include "framelib/CAnimationFrame.h"
#include "MTypeDef.h"
#include "MObject.h"
#include "MEffectTarget.h"

class ofstream;
class ifstream;


extern DWORD	g_CurrentFrame;


class MEffect : public MObject, public CAnimationFrame {
	
	public :
		enum EFFECT_TYPE 
		{
			EFFECT_SECTOR = 0,		// Sector에 고정 
			EFFECT_MOVING,			// 움직이는 Effect
			EFFECT_LINEAR,			// 직선 이동
			EFFECT_GUIDANCE,		// 추적(유도)하고 끝남
			EFFECT_HOMING,			// 곡선형의 유도탄
			EFFECT_PARABOLA,		// 포물선
			EFFECT_ATTACH,			// (캐릭터에) 붙음
			EFFECT_SCREEN,			// 화면좌표로 표현
			EFFECT_CHASE,			// 추적하면서 계속 붙어 다님
			EFFECT_ATTACH_ORBIT,	// 캐릭터에 붙어서 빙빙~ 돌면서 붙어다님
		};

	public :
		MEffect(BYTE bltType);
		~MEffect();

		//--------------------------------------------------------
		// Set FrameID
		//--------------------------------------------------------
		void			SetFrameID(TYPE_FRAMEID FrameID, BYTE max);

		//--------------------------------------------------------
		// GetEffectType
		//--------------------------------------------------------
		virtual EFFECT_TYPE		GetEffectType()	const	{ return EFFECT_SECTOR; }

		void SetEst( int est ) { m_est = est; }
		int GetEst() const { return m_est; }
		//--------------------------------------------------------
		//	Is Selectable
		//--------------------------------------------------------
		virtual bool		IsSelectable() const		{ return false; }		

		//--------------------------------------------------------
		// 끝나는 시간...
		//--------------------------------------------------------		
		// 현재 시간 + last까지 작동		
		void			SetCount(DWORD last, DWORD linkCount=0xFFFF);
		DWORD			GetEndFrame() const			{ return m_EndFrame; }
		DWORD			GetEndLinkFrame() const		{ return m_EndLinkFrame; }
		bool			IsEnd() const				{ return g_CurrentFrame >= m_EndFrame; }

		
		//--------------------------------------------------------
		// 한 번의 Update에 호출될 함수..
		//--------------------------------------------------------
		virtual bool	Update();
		
		//--------------------------------------------------------
		// Sector에서의 좌표
		//--------------------------------------------------------	
		void			SetPosition(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y);
		void			SetX(TYPE_SECTORPOSITION x);
		void			SetY(TYPE_SECTORPOSITION y);
		void			SetZ(int z)						{ m_PixelZ = (float)z; }
		void			SetDirection(BYTE d)			{ m_Direction = d; }
		BYTE			GetDirection() const			{ return m_Direction; }
		void			SetLight(char light)			{ m_Light = light; }

	
		//--------------------------------------------------------
		// Pixel 좌표 설정
		//--------------------------------------------------------
		void			SetPixelPosition(int x, int y, int z);

		//--------------------------------------------------------
		// Get Functions
		//--------------------------------------------------------
		char			GetLight() const	{ return m_Light; }
		virtual int		GetPixelX() const	{ return (int)m_PixelX; }
		virtual int		GetPixelY() const	{ return (int)m_PixelY; }
		virtual int		GetPixelZ() const	{ return (int)m_PixelZ; }

		//--------------------------------------------------------
		// Power
		//--------------------------------------------------------
		void			SetPower(BYTE power)	{ m_Power = power; }
		BYTE			GetPower() const		{ return m_Power; }

		//--------------------------------------------------------
		// Step
		//--------------------------------------------------------
		void			SetStepPixel(WORD step)	{ m_StepPixel=step; }
		WORD			GetStepPixel() const	{ return m_StepPixel; }

		//--------------------------------------------------------
		// Effect 연결을 위한 정보
		//--------------------------------------------------------
		void			SetLink(TYPE_ACTIONINFO nActionInfo, MEffectTarget* pEffectTarget);		

		void			SetEffectTargetNULL();

		// 몇 번째 ActionInfo인가?
		TYPE_ACTIONINFO	GetActionInfo()	const { return m_nActionInfo; }

		// 남은 Effect개수를 return한다.
		MEffectTarget*	GetEffectTarget()		{ return m_pEffectTarget; }
		int				GetLinkSize()			{ return (m_pEffectTarget==NULL || m_pEffectTarget->IsEnd())? 0 : m_pEffectTarget->GetCurrentPhase(); }	

		void			SetMulti(bool bMulti)	{ m_bMulti = bMulti; }
		const bool		IsMulti()				{ return m_bMulti; }

		void			SetDelayFrame(DWORD frame);
		bool			IsDelayFrame() const;

		// 2004, 10, 15, sobeit add start - 이펙트 생성을 기다린다.
		void			SetWaitFrame(DWORD frame);
		bool			IsWaitFrame() const;
		// 2004, 10, 15, sobeit add end

		// 2004, 9, 30, sobeit add start - 보여줄지 말지..
		bool			IsSkipDraw() const			{ return m_bDrawSkip; }
		void			SetDrawSkip(bool bSkip)		{ m_bDrawSkip = bSkip;}
		// 2004, 9, 30, sobeit add end
	protected :
		// PixelPositon으로서 Sector좌표를 설정한다.
		void			AffectPosition();



	protected :
		DWORD					m_DelayFrame;
		
		int	m_est;

		// 끝나는 시간
		DWORD					m_EndFrame;
		DWORD					m_EndLinkFrame;	// 다음 link로 넘어가는 frame
		
		BYTE					m_Direction;	// 방향

		// Effect의 밝기 
		char					m_Light;

		// Effect가 존재하는 Zone에서의 Pixel좌표(현재 위치)
		float			m_PixelX;
		float			m_PixelY;
		float			m_PixelZ;
		WORD			m_StepPixel;

		BYTE			m_Power;

		// 다음 Effect로의 연결을 위한 정보
		TYPE_ACTIONINFO			m_nActionInfo;
		MEffectTarget*			m_pEffectTarget;

		static TYPE_OBJECTID	s_ID;			// ID발급을 위한...

		// 중복 가능한가
		bool			m_bMulti;

		bool			m_bDrawSkip;
		DWORD			m_dwWaitFrame;
};

//----------------------------------------------------------------------
// MSelectableEffect - 마우스로 선택되는 effect
//----------------------------------------------------------------------
class MSelectableEffect : public MEffect {
	public :
		MSelectableEffect(BYTE bltType) : MEffect(bltType) {}
		~MSelectableEffect() {}

		//--------------------------------------------------------
		//	Is Selectable
		//--------------------------------------------------------
		bool		IsSelectable() const		{ return true; }		
};

#endif



