//----------------------------------------------------------------------
// MCreatureWear.h
//----------------------------------------------------------------------
// 옷입고 움직이는 생물에 대한 class
//----------------------------------------------------------------------

#ifndef	__MCREATUREWEAR_H__
#define	__MCREATUREWEAR_H__

#include "MTypeDef.h"
#include "MCreature.h"
#include "MItem.h"
#include "MemoryPool.h"

//----------------------------------------------------------------------
// define
//----------------------------------------------------------------------

class MCreatureWear : public MCreature {
	public :
		//----------------------------------------------------
		// Creature의 그림과 복장에 대한 정보 
		//----------------------------------------------------
		struct ADDON_INFO 
		{
			BOOL				bAddon;			// 착용했나?	
			TYPE_FRAMEID		FrameID;		// 복장
			ITEM_CLASS	ItemClass;		// item class
			TYPE_ITEMTYPE		ItemType;		// item type
			WORD				ColorSet1;		// colorset1
			WORD				ColorSet2;		// colorset2
			BOOL				bEffectColor;	// EffectColor를 사용하는가?
			WORD				EffectColorSet;	// EffectColor			
		};

	public :
		MCreatureWear();
		virtual ~MCreatureWear();
		
		void* operator new( size_t size )
		{
			return g_CreatureWearMemoryPool.Alloc();
		}

		void operator delete( void* pmem )
		{
			g_CreatureWearMemoryPool.Free( pmem );
		}

		virtual CLASS_TYPE	GetClassType() const	{ return CLASS_CREATUREWEAR; }

		BOOL		IsWear() const					{ return TRUE; }

		//------------------------------------------------------
		// type설정
		//------------------------------------------------------
		virtual void	SetCreatureType(TYPE_CREATURETYPE type);


		//------------------------------------------------------
		// 복장 바꾸기 관련
		//------------------------------------------------------			
		// 머리카락
		void			SetAddonHair(TYPE_FRAMEID id, WORD cs1);
		TYPE_FRAMEID	GetHairFrameID()			{ return m_HairFrameID; }
		
		// 복장 벗기기
		bool			RemoveAddon(int Addon);

		// 복장 있는가?
		//BOOL			IsExistAddon(int Addon) const		{ return m_Addon[Addon].bAddon; }
		//TYPE_FRAMEID	GetAddonFrameID(int Addon) const	{ return m_Addon[Addon].FrameID; }
		//ITEM_CLASS	GetAddonItemClass(int Addon) const	{ return m_Addon[Addon].ItemClass; }
		//TYPE_ITEMTYPE	GetAddonItemType(int Addon) const	{ return m_Addon[Addon].ItemType; }
		const ADDON_INFO&	GetAddonInfo(int Addon) const	{ return m_Addon[Addon]; }
		const ADDON_INFO&	GetACAddonInfo(int Addon) const;

		//------------------------------------------------------			
		// ColorSet을 설정한다.
		//------------------------------------------------------			
		void			SetAddonColorSet1(int Addon, WORD colorSet);
		void			SetAddonColorSet2(int Addon, WORD colorSet);	
		
		//------------------------------------------------------
		// AddonItem을 착용/해제한다.
		//------------------------------------------------------
		virtual	bool		SetAddonItem(MItem* pItem);
		virtual bool		RemoveAddonItem(MItem* pItem);

		//------------------------------------------------------
		// SetSameWear
		//------------------------------------------------------
		void				SetSameWear(const MCreatureWear* pCreature);

		//------------------------------------------------------
		// Update AttachEffect
		//------------------------------------------------------
		virtual bool		RemoveEffectStatus(EFFECTSTATUS status);
		virtual void		UpdateAttachEffect();

	protected :
		void			ClearAddonInfo(int Addon);		
		MItem*			NewItemFromAddonInfo(int Addon);

	protected :
		// 복장의 정보	
		ADDON_INFO			m_Addon[ ADDON_MAX ];
//		ADDON_INFO			m_ACAddon[ AC_ADDON_MAX ];
		ADDON_INFO			m_ShoulderAddon;

		// 모자 썼을때 짧은 머리는 안 보여주게 하기 위해서..
		// 머리카락 정보만 따로 저장한다. -- 임시.. 그러나.. --;;
		TYPE_FRAMEID		m_HairFrameID;
		WORD				m_HairColorSet;

	public :
		// 8방향에 따른 옷의 출력 순서
		static int			s_AddonOrder[DIRECTION_MAX][ADDON_MAX];
		static int			s_ACAddonOrder[ DIRECTION_MAX ][ AC_ADDON_MAX ];
};

#endif


