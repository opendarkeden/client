//----------------------------------------------------------------------
// MShopShelf.h
//----------------------------------------------------------------------
// 상점의 선반 하나.. - -;
//
// 선반 하나마다 run-time version이 있다.
//----------------------------------------------------------------------

#ifndef __MSHOPSHELF_H__
#define	__MSHOPSHELF_H__

#include "MGameDef.h"
#include "MItem.h"

class MShopShelf {
	public :
		enum SHELF_TYPE
		{			
			SHELF_FIXED,			// 고정된 item list를 가진 shelf
			SHELF_SPECIAL,			// special item
			SHELF_UNKNOWN,			// unknown item
			MAX_SHELF,				// max값
			SHELF_BASE				// 사용 안 함
			
		};

	public :
		MShopShelf();
		virtual ~MShopShelf();

		//----------------------------------------------------------
		// class
		//----------------------------------------------------------
		virtual SHELF_TYPE	GetShelfType() const	{ return SHELF_BASE; }

		//----------------------------------------------------------
		// Release
		//----------------------------------------------------------
		void		Release();

		//----------------------------------------------------------
		// Name
		//----------------------------------------------------------
		void		SetName(const char*name)		{ m_Name = name; }
		const char*	GetName() const					{ return m_Name.GetString(); }

		//----------------------------------------------------------
		// Is...
		//----------------------------------------------------------
		bool		IsEnable() const		{ return m_bShelfEnable; }
		bool		IsEmptySlot(unsigned int  slot);

		void		SetEnable()				{ m_bShelfEnable = true; }
		void		SetDisable()			{ m_bShelfEnable = false; }
		
		//----------------------------------------------------------
		// Get 
		//----------------------------------------------------------
		int			GetVersion() const			{ return m_Version; }
		MItem*		GetItem(unsigned int  slot) const;

		
		//----------------------------------------------------------
		// Set
		//----------------------------------------------------------
		void		SetVersion(int version)		{ m_Version = version; }
		bool		AddItem(MItem* pItem);		// 빈 곳에 추가한다.
		bool		SetItem(unsigned int  slot, MItem* pItem);	// 기존에 있던건 지워준다.

	
		//----------------------------------------------------------
		// Remove - Shelf에서 제거해서 return해준다.(외부에서 delete해야한다.)
		//----------------------------------------------------------
		MItem*		RemoveItem(unsigned int slot);

		//----------------------------------------------------------
		// Delete - Shelf 내부에 있는걸 지워준다.
		//----------------------------------------------------------
		void		DeleteItem(unsigned int slot);

	protected :
		bool		m_bShelfEnable;				// 현재 사용가능한 Shelf인가?

		MString		m_Name;						// Shelf Name

		int			m_Version;					// run-time version
	
		MItem*		m_pItems[SHOP_SHELF_SLOT];


	public :
		// NewItem()에 대한 functions pointer
		typedef MShopShelf* (*FUNCTION_NEWSHELFCLASS)();

		//-------------------------------------------------------
		// 원하는 class의 Item의 객체를 생성한다.
		//-------------------------------------------------------
		static MShopShelf*		NewShelf(MShopShelf::SHELF_TYPE shelfType);

		// 각 class별로 NewItem()에 대한 function pointer
		static FUNCTION_NEWSHELFCLASS		s_NewShelfClassTable[MAX_SHELF];
};


//----------------------------------------------------------------------
// MShopFixedShelf.h
//----------------------------------------------------------------------
// 고정된 Item List를 가진 상점 선반
//----------------------------------------------------------------------
class MShopFixedShelf : public MShopShelf {
	public :
		MShopFixedShelf();
		~MShopFixedShelf();

		//----------------------------------------------------------
		// class
		//----------------------------------------------------------
		virtual SHELF_TYPE	GetShelfType() const	{ return SHELF_FIXED; }

		//------------------------------------------------------------
		// Init
		//------------------------------------------------------------
		//bool		InitFromFixedItemTable(unsigned int tableID);

		//------------------------------------------------------------
		// new Shelf
		//------------------------------------------------------------
		static MShopShelf*	NewShelf()				{ return new MShopFixedShelf; }

	protected :

};

//----------------------------------------------------------------------
// MShopSpecialShelf.h
//----------------------------------------------------------------------
// 고정된 Item List를 가진 상점 선반
//----------------------------------------------------------------------
class MShopSpecialShelf : public MShopShelf {
	public :
		MShopSpecialShelf();
		~MShopSpecialShelf();

		//----------------------------------------------------------
		// class
		//----------------------------------------------------------
		virtual SHELF_TYPE	GetShelfType() const	{ return SHELF_SPECIAL; }

		//------------------------------------------------------------
		// new Shelf
		//------------------------------------------------------------
		static MShopShelf*	NewShelf()				{ return new MShopSpecialShelf; }

	protected :

};


//----------------------------------------------------------------------
// MShopUnknownShelf.h
//----------------------------------------------------------------------
// 고정된 Item List를 가진 상점 선반
//----------------------------------------------------------------------
class MShopUnknownShelf : public MShopShelf {
	public :
		MShopUnknownShelf();
		~MShopUnknownShelf();

		//----------------------------------------------------------
		// class
		//----------------------------------------------------------
		virtual SHELF_TYPE	GetShelfType() const	{ return SHELF_UNKNOWN; }

		//------------------------------------------------------------
		// new Shelf
		//------------------------------------------------------------
		static MShopShelf*	NewShelf()				{ return new MShopUnknownShelf; }

	protected :

};

#endif
