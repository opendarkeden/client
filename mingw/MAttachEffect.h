//----------------------------------------------------------------------
// MAttachEffect.h
//----------------------------------------------------------------------
//
// Effect중에서 Object에 붙어 있는 Effect를 의미한다.
//
// 매 Update마다 Object의 좌표를 체크해서 붙어야~ 한다.
//
// Object라고는 하지만 아마.. Creature밖에 없지 않을까.
//
//----------------------------------------------------------------------

#ifndef	__MATTACHEFFECT_H__
#define	__MATTACHEFFECT_H__

#pragma warning(disable:4786)

#include "MMovingEffect.h"
#include "MTypeDef.h"
//#include "EffectSpriteTypeDef.h"
#include <list>
class MCreature;

class MAttachEffect : public MMovingEffect {
	public :
		MAttachEffect(TYPE_EFFECTSPRITETYPE type, DWORD last, DWORD linkCount=MAX_LINKCOUNT);
		~MAttachEffect();

		virtual EFFECT_TYPE		GetEffectType()	const	{ return EFFECT_ATTACH; }

		//--------------------------------------------------------
		// 목표 설정
		//--------------------------------------------------------
		void					SetAttachCreatureID(TYPE_OBJECTID id);	// zone에서 찾는다.
		bool					SetAttachCreature(MCreature* pCreature);		// 캐릭터의 좌표로 이동
		TYPE_OBJECTID			GetAttachCreatureID()					{ return m_CreatureID; }

		//--------------------------------------------------------
		// 종류
		//--------------------------------------------------------
		bool					IsEffectSprite() const	{ return m_bEffectSprite; }
		bool					IsEffectColor() const	{ return !m_bEffectSprite; }
		void					SetEffectSprite(TYPE_EFFECTSPRITETYPE es)	{ m_EffectSpriteType = es; m_bEffectSprite = true; }
		void					SetEffectColor(WORD colorSet)				{ m_EffectColor = colorSet; m_bEffectSprite = false; }
		
		void					SetEffectColorPart(ADDON part)				{ m_bEffectColorPart = part; }
		ADDON					GetEffectColorPart() const					{ return m_bEffectColorPart; }

		TYPE_EFFECTSPRITETYPE	GetEffectSpriteType() const	{ return m_EffectSpriteType; }
		WORD					GetEffectColor() const		{ return m_EffectColor; }		
		
		//--------------------------------------------------------
		// 한 번의 Update에 호출될 함수..
		//--------------------------------------------------------
		virtual bool			Update();

		
	protected :
		TYPE_OBJECTID				m_CreatureID;

		bool						m_bEffectSprite;		// 그림 종류를 사용하나?

		union {
			TYPE_EFFECTSPRITETYPE	m_EffectSpriteType;		// 그림 종류
			WORD					m_EffectColor;			// Effect 색깔
		};

		ADDON						m_bEffectColorPart;		// 색깔 바뀌는 위치
};

// list정의
typedef	std::list<MAttachEffect*>	ATTACHEFFECT_LIST;

#endif

