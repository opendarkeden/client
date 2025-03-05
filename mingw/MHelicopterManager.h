//----------------------------------------------------------------------
// MHelicopterManager.h
//----------------------------------------------------------------------
// MHelicopter는 한 Player를 쫓아가는 Effect로 표현된다.
// MHelicopterManager는 화면상에 존재하는 MHelicopter를 다 처리한다.
//----------------------------------------------------------------------

#ifndef __MHELICOPTERMANAGER_H__
#define __MHELICOPTERMANAGER_H__

#include "MTypeDef.h"
#include "CTypeMap2.h"
class ZONESOUND_NODE;

//----------------------------------------------------------------------
// MHelicopter
//----------------------------------------------------------------------
class MHelicopter {
	public :
		MHelicopter();
		~MHelicopter();

		void			Release();

		bool			SetChaseCreature(TYPE_OBJECTID id, int x0, int y0);		

		TYPE_OBJECTID	GetCreatureID() const	{ return m_CreatureID; }
		TYPE_OBJECTID	GetEffectID() const		{ return m_EffectID; }

		void			UpdateSound(int x, int y);

	protected :
		TYPE_OBJECTID			m_CreatureID;	// 호출한 사람의 id
		TYPE_OBJECTID			m_EffectID;		// 헬기 그림자 Effect의 ID
		ZONESOUND_NODE*			m_pSound;		// 헬기 소리
};

//----------------------------------------------------------------------
// MHelicopterManager - MHelicopter*의 map
//----------------------------------------------------------------------
class MHelicopterManager : private CTypeMap2<MHelicopter> {
	public :
		MHelicopterManager();
		~MHelicopterManager();

		void			Release();

		bool			AddHelicopter(MHelicopter* pHelicopter);
		MHelicopter*	GetHelicopter(TYPE_OBJECTID creatureID) const;
		void			RemoveHelicopter(TYPE_OBJECTID creatureID);
		void			RemoveHelicopterSoon(TYPE_OBJECTID creatureID);

		void			Update();		
};

#endif
