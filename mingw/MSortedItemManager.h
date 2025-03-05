//----------------------------------------------------------------------
// MSortedItemManager.h
//----------------------------------------------------------------------

#ifndef __MSORTEDITEMMANAGER_H__
#define __MSORTEDITEMMANAGER_H__

#pragma warning(disable:4786)

class MItem;

class MSortedItemManager : public std::map<ULONGLONG, MItem*> {
	public :
		typedef std::map<ULONGLONG, MItem*>		ITEM_MAP;
		typedef ITEM_MAP::iterator				iterator;
		typedef ITEM_MAP::const_iterator		const_iterator;

	public :
		MSortedItemManager();
		~MSortedItemManager();

		void		Release();											// 모두 delete하고 map을 지운다.
		void		Clear()		{ m_NumTwoByTwo = 0; clear(); }			// map에서 그냥 지운다.

		//----------------------------------------------------------
		// 2 x 2의 개수
		//----------------------------------------------------------
		void		SetTwoByTwoNumber(int n);
		int			GetTwoByTwoNumber() const	{ return m_NumTwoByTwo; }

		//----------------------------------------------------------
		// item 추가
		//----------------------------------------------------------
		bool		AddItem(MItem* pItem);

		//----------------------------------------------------------
		// map추가
		//----------------------------------------------------------
		//void		Merge(const MSortedItemManager& sim);

	protected :
		//void		ChangeTwoByTwoPriority(int n);

	protected :
		ULONGLONG	GetKey(MItem* pItem);
		int			m_NumTwoByTwo;
		int			m_NumTwoByTwoTemp;				// 임시 - GetKey에서 필수!
};

#endif


