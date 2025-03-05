//----------------------------------------------------------------------
// MEffectGenerator.h
//----------------------------------------------------------------------
//
// = 임의의 순간에 동시에 발생하는 Effect들을 생성해서 Zone에 등록하는 일을 한다.
//
// = 여러개의 Effect들을 생성하고 Player에게 영향을 줄 수도 있다.
//
// = 단지, 생성해서 Zone에 등록하는 역할만 한다. 
//
// = 하나의 Effect를 생성하는데 필요한 정보들
//    - 출력형식(BltType)
//    - 그림(FrameID)
//    - 시작(pX,pY,pZ)
//    - 목표(pX,pY,pZ)
//    - 목표Creature(CreatureID)
//    - 방향(Direction)
//    - 속도(Step)
//    - 지속시간(Count)
//    - 위력(Power)
//
//----------------------------------------------------------------------

#ifndef	__MEFFECTGENERATOR_H__
#define	__MEFFECTGENERATOR_H__

#include "MTypeDef.h"
#include "MEffectGeneratorDef.h"
#include "DrawTypeDef.h"
#include "MEffectTarget.h"
class MEffect;

//----------------------------------------------------------------------
// effect를 생성하는데 필요한 정보들
//----------------------------------------------------------------------
class EFFECTGENERATOR_INFO
{
	public :
		TYPE_ACTIONINFO			nActionInfo;				// 해당하는 액션 인포
		MEffectTarget*			pEffectTarget;				// 이펙트의 목표
		TYPE_EFFECTSPRITETYPE	effectSpriteType;			// 스프라이트 타입
		int						x0, y0, z0;					// 시작점
		int						x1, y1, z1;					// 목표지점		
		TYPE_OBJECTID			creatureID;					// 대상 CreatureID
		BYTE					direction;					// 방향
		BYTE					step;						// 진행 속도
		WORD					count;						// 지속시간
		WORD					linkCount;					// 다음 이펙트로 연결될 시간
		BYTE					power;						// 위력
		MEffect*				pPreviousEffect;			// 이전 이펙트
		BYTE					temp1,temp2;				// 기타
};

//----------------------------------------------------------------------
//
//		 Effect Generator
//
//----------------------------------------------------------------------
class MEffectGenerator {
	public :
		MEffectGenerator()	{}
		~MEffectGenerator() {}

		virtual TYPE_EFFECTGENERATORID		GetID()		{ return EFFECTGENERATORID_NULL; }

		virtual bool	Generate(	const EFFECTGENERATOR_INFO& egInfo	) = 0;

	protected :
};

#endif
