//----------------------------------------------------------------------
// COrderedList.h
//----------------------------------------------------------------------
// 
// 값 하나를 갖고 있는 class이다.
//
// 순서를 가지고 중복을 허용하지 않는다.
//
// list로 구현한다.
//
//----------------------------------------------------------------------

#ifndef	__CORDEREDLIST_H__
#define	__CORDEREDLIST_H__

#include <Windows.h>
#include <list>




//----------------------------------------------------------------------
//
// COrderedList class
//
//----------------------------------------------------------------------
template <class Type>
class COrderedList {
	public :
		//----------------------------------------------------------------------
		// BUILDING_INFO_LIST를 정의한다.
		//----------------------------------------------------------------------
		typedef	std::list<Type>	DATA_LIST;

	public :
		COrderedList();
		~COrderedList(); 

		//--------------------------------------------------------------
		// Release
		//--------------------------------------------------------------
		void		Release();

		//--------------------------------------------------------------
		// Add    : data를 추가. 이미 있는지 check해봐야 한다.
		//--------------------------------------------------------------
		bool		Add(Type data);

		//--------------------------------------------------------------
		// Remove : data를 제거
		//--------------------------------------------------------------
		bool		Remove(Type data);

		//--------------------------------------------------------------
		// Get 
		//--------------------------------------------------------------
		int								GetSize() const		{ return m_List.size(); }
		DATA_LIST::const_iterator	GetIterator() const	{ return m_List.begin(); }

		//--------------------------------------------------------------
		// operator
		//--------------------------------------------------------------
		void		operator	-= (const COrderedList<Type>& listSub);

	protected :
		DATA_LIST			m_List;
};


//----------------------------------------------------------------------
// COrderedList.cpp
//----------------------------------------------------------------------

//#include "COrderedList.h"

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
template <class Type>
COrderedList<Type>::COrderedList()
{
}

template <class Type>
COrderedList<Type>::~COrderedList()
{
	Release();
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
template <class Type>
void
COrderedList<Type>::Release()
{
	m_List.clear();
}

//----------------------------------------------------------------------
// Add(x,y)
//----------------------------------------------------------------------
// (x,y)를 추가 : 이미 있는지 check해봐야 한다.
// 편의상.. X, Y순으로 Sort해서 추가한다.
//----------------------------------------------------------------------
template <class Type>
bool
COrderedList<Type>::Add(Type data)
{
	DATA_LIST::iterator iData = m_List.begin();

	// 이미 list에 들어있는 모든 Data과 비교해보고
	// 같은게 없을 때 추가한다.
	while (iData != m_List.end())
	{
		// 현재 들어있는게 추가할려는것보다 클 경우,
		// 현재 위치에 추가하면 된다.
		if (*iData > data)
		{
			m_List.insert(iData, data);
			return true;
		}

		// 이미 있는 값이면
		// 추가하지 않는다.
		if (*iData==data)
		{
			return false;
		}

		iData++;
	}	

	// list의 모든 원소들보다 크므로 
	// list의 끝에 추가한다.
	m_List.push_back( data );

	return true;
}

//----------------------------------------------------------------------
// Remove
//----------------------------------------------------------------------
// (x,y)를 제거
//----------------------------------------------------------------------
template <class Type>
bool		
COrderedList<Type>::Remove(Type data)
{
	DATA_LIST::iterator iData = m_List.begin();

	// list의 Node에서 같은게 있으면 지운다.
	while (iData != m_List.end())
	{		
		// 같은 값이면 지운다.
		if (*iData==data)
		{
			m_List.erase(iData);
			return true;
		}
		
		// 현재 위치에 있는 값이 Data보다 크다면
		// 계속 큰 값만 있으므로 Data값이 없는 것이다.
		if (*iData > data)
		{			
			return false;
		}

		iData++;
	}	

	// 없는 경우
	return false;
}

//----------------------------------------------------------------------
// operator -=
//----------------------------------------------------------------------
// 현재의 list에서 listSub에 있는것 들을 제거한다.
//----------------------------------------------------------------------
template <class Type>
void		
COrderedList<Type>::operator -= (const COrderedList<Type>& listSub)
{
	DATA_LIST::iterator iDataThis = m_List.begin();
	DATA_LIST::const_iterator iDataSub = listSub.m_List.begin();
	DATA_LIST::iterator iDataTemp;

	// 둘 중에 하나라도 list가 끝이면.. 더 계산할게 없다.
	while (iDataThis != m_List.end() && iDataSub != listSub.m_List.end())
	{		
		// 같은 것이면.. this에서 지운다.
		if (*iDataThis == *iDataSub)
		{
			iDataTemp = iDataThis;		// 임시로 기억

			// 다음 것
			iDataThis++;
			iDataSub++;

			// 삭제
			m_List.erase( iDataTemp );
		}
		// this가 더 작은 경우..
		else if (*iDataThis < *iDataSub)
		{
			// this를 다음 것으로..
			iDataThis++;
		}
		// this가 더 큰 경우
		else
		{
			iDataSub++;
		}
	}	
}

#endif