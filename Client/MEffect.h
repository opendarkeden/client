//----------------------------------------------------------------------
// MEffect.h
//-------------------------------------------------------------------------
// Effects displayed on the screen
//----------------------------------------------------------------------------------
//
// < What is an Effect?
// - Effects displayed on the screen.
// - Excluding characters, items, buildings, obstacles, etc. (what else?),
// these are animations.
// - All Effects are expressed using CAlphaSprite and alpha channel blending.
// - Things like a character swinging a sword or a priest's aura
// are not included in the Effects discussed here.
// - An Effect can be considered an independent object.
///
// < Effect Types >
// = Missile
// - Pixel-based movement.
// - Animated from the launch location to the target location
// / / Moves at a constant speed (pixels).
// - May or may not be guided missiles. // - May be destroyed by collision with obstacles, characters, buildings, etc.
// - Disappears upon reaching the target location.
//
// = Tile Magic
// - Animated on a specific Tile.
// - Size may be larger than the Tile.
// - Output is displayed last on the Tile.
// - Disappears after a certain number of frames.
//
//----------------------------------------------------------------------
// - All Effects must belong to a Sector within a Zone.
// Since the output order must be sorted by y-coordinate,
// we decided to place them in a Sector to output them according to the Sector's Object output.
//
// - The Effect lasts for a certain amount of time.
// That is, we set a counter so that it ends when it reaches 0.
//----------------------------------------------------------------------
//
// The Effect is not saved to a file. //
//----------------------------------------------------------------------
/* class hierarchy

MEffect --+-- MMovingEffect ---- ....

MEffect: Effect anchored to a Tile
*/
//----------------------------------------------------------------------

#ifndef	__MEFFECT_H__
#define	__MEFFECT_H__

#pragma warning(disable:4786)

#include "framelib/CAnimationFrame.h"
#include "MTypeDef.h"
#include "MObject.h"
#include "MEffectTarget.h"
#include "EffectResourceContainer.h"

#include <fstream>
using namespace std;



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
		// New constructor: supports dependency injection (requires explicit resource container)
		MEffect(BYTE bltType, EffectResourceContainer* resources);

		// Old constructor: maintain backward compatibility (no resource container)
		MEffect(BYTE bltType);

		~MEffect();

		//--------------------------------------------------------
		// Resource container management (newly added)
		//--------------------------------------------------------
		// Set resource container (for dependency injection)
		void SetResourceContainer(EffectResourceContainer* resources);

		// Get resource container
		EffectResourceContainer* GetResourceContainer() const { return m_pResources; }

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

		// Newly added: resource container (dependency injection)
		EffectResourceContainer*	m_pResources;
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



