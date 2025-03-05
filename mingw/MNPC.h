//----------------------------------------------------------------------
// MNPC.h
//----------------------------------------------------------------------
// NPC는 상점을 가질 수 있다. - -;
//----------------------------------------------------------------------

#ifndef __MNPC_H__
#define	__MNPC_H__

#include "MCreatureWear.h"
#include "MShop.h"
#include "MemoryPool.h"
class NPC_INFO;

class MNPC : public MCreatureWear {
	public :
		MNPC();
		~MNPC();

		void* operator new( size_t size )
		{
			return g_NPCCreatureMemoryPool.Alloc();
		}

		void operator delete( void* pmem )
		{
			g_NPCCreatureMemoryPool.Free( pmem );
		}

		//------------------------------------------------------
		// class type
		//------------------------------------------------------
		virtual CLASS_TYPE	GetClassType() const	{ return CLASS_NPC; }

		//------------------------------------------------------
		// Action
		//------------------------------------------------------
		virtual void	Action();			// 현재 행동을 취하도록 한다.

		//------------------------------------------------------
		// NPC ID
		//------------------------------------------------------
		void				SetNPCID(TYPE_OBJECTID id)	{ m_NPCID = id; }
		TYPE_OBJECTID		GetNPCID() const			{ return m_NPCID; }

		//------------------------------------------------------
		// NPC 정보
		//------------------------------------------------------
		NPC_INFO*			GetNPCInfo(TYPE_OBJECTID id) const;

		//------------------------------------------------------
		// Shop
		//------------------------------------------------------
		BOOL				HasShop()			{ return m_pShop!=NULL; }
		MShop*				GetShop() const		{ return m_pShop; }
		void				SetShop(MShop* pShop);

		//------------------------------------------------------
		// normal item 선반 생성
		//------------------------------------------------------
		BOOL				CreateFixedShelf(bool bMysterious=false);

	protected :
		TYPE_OBJECTID		m_NPCID;		// NPC ID
		MShop*				m_pShop;		// 상점
};

#endif


