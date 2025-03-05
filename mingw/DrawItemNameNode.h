//----------------------------------------------------------------------
// DrawItemNameNode.h
//----------------------------------------------------------------------
// 화면에 String을 출력하기 위한 정보이다.
//----------------------------------------------------------------------

#ifndef	__DRAWITEMNAMENODE_H__
#define	__DRAWITEMNAMENODE_H__

#include <Windows.h>
#include "MItem.h"

class DRAWITEMNAME_NODE {
	public :
		DRAWITEMNAME_NODE(DWORD dist, MItem* pItem)
		{
			m_Distance	= dist;
			m_ID		= pItem->GetID();
			m_pItem		= pItem;
		}

		//-------------------------------------------------------
		// Get
		//-------------------------------------------------------
		int				GetDistance() const		{ return m_Distance; }		
		TYPE_OBJECTID	GetID() const			{ return m_ID; }
		MItem*			GetItem() const			{ return m_pItem; }		

		//-------------------------------------------------------
		// Rect
		//-------------------------------------------------------
		void			SetRect(const RECT& rect)	{ m_Rect = rect; }
		BOOL			IsPointInRect(int x, int y) const
		{
			if (x >= m_Rect.left && x < m_Rect.right
				&& y >= m_Rect.top && y < m_Rect.bottom)
			{
				return TRUE;
			}

			return FALSE;
		}

	protected :
		DWORD			m_Distance;		// player와의 거리
		TYPE_OBJECTID	m_ID;			// Item ID
		MItem*			m_pItem;		// item
		RECT			m_Rect;
};

#endif