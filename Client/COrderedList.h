//----------------------------------------------------------------------
// COrderedList.h
//----------------------------------------------------------------------
// 
// �� �ϳ��� ���� �ִ� class�̴�.
//
// ������ ������ �ߺ��� ������� �ʴ´�.
//
// list�� �����Ѵ�.
//
//----------------------------------------------------------------------

#ifndef	__CORDEREDLIST_H__
#define	__CORDEREDLIST_H__

#ifdef PLATFORM_WINDOWS
	#include <Windows.h>
#else
	#include "../basic/Platform.h"
#endif
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
		// BUILDING_INFO_LIST�� �����Ѵ�.
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
		// Add    : data�� �߰�. �̹� �ִ��� check�غ��� �Ѵ�.
		//--------------------------------------------------------------
		bool		Add(Type data);

		//--------------------------------------------------------------
		// Remove : data�� ����
		//--------------------------------------------------------------
		bool		Remove(Type data);

		//--------------------------------------------------------------
		// Get 
		//--------------------------------------------------------------
		int								GetSize() const		{ return m_List.size(); }
		typename DATA_LIST::const_iterator	GetIterator() const	{ return m_List.begin(); }

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
// (x,y)�� �߰� : �̹� �ִ��� check�غ��� �Ѵ�.
// ���ǻ�.. X, Y������ Sort�ؼ� �߰��Ѵ�.
//----------------------------------------------------------------------
template <class Type>
bool
COrderedList<Type>::Add(Type data)
{
	typename DATA_LIST::iterator iData = m_List.begin();

	// �̹� list�� ����ִ� ��� Data�� ���غ���
	// ������ ���� �� �߰��Ѵ�.
	while (iData != m_List.end())
	{
		// ���� ����ִ°� �߰��ҷ��°ͺ��� Ŭ ���,
		// ���� ��ġ�� �߰��ϸ� �ȴ�.
		if (*iData > data)
		{
			m_List.insert(iData, data);
			return true;
		}

		// �̹� �ִ� ���̸�
		// �߰����� �ʴ´�.
		if (*iData==data)
		{
			return false;
		}

		iData++;
	}	

	// list�� ��� ���ҵ麸�� ũ�Ƿ� 
	// list�� ���� �߰��Ѵ�.
	m_List.push_back( data );

	return true;
}

//----------------------------------------------------------------------
// Remove
//----------------------------------------------------------------------
// (x,y)�� ����
//----------------------------------------------------------------------
template <class Type>
bool		
COrderedList<Type>::Remove(Type data)
{
	typename DATA_LIST::iterator iData = m_List.begin();

	// list�� Node���� ������ ������ �����.
	while (iData != m_List.end())
	{		
		// ���� ���̸� �����.
		if (*iData==data)
		{
			m_List.erase(iData);
			return true;
		}
		
		// ���� ��ġ�� �ִ� ���� Data���� ũ�ٸ�
		// ��� ū ���� �����Ƿ� Data���� ���� ���̴�.
		if (*iData > data)
		{			
			return false;
		}

		iData++;
	}	

	// ���� ���
	return false;
}

//----------------------------------------------------------------------
// operator -=
//----------------------------------------------------------------------
// ������ list���� listSub�� �ִ°� ���� �����Ѵ�.
//----------------------------------------------------------------------
template <class Type>
void		
COrderedList<Type>::operator -= (const COrderedList<Type>& listSub)
{
	typename DATA_LIST::iterator iDataThis = m_List.begin();
	typename DATA_LIST::const_iterator iDataSub = listSub.m_List.begin();
	typename DATA_LIST::iterator iDataTemp;

	// �� �߿� �ϳ��� list�� ���̸�.. �� ����Ұ� ����.
	while (iDataThis != m_List.end() && iDataSub != listSub.m_List.end())
	{		
		// ���� ���̸�.. this���� �����.
		if (*iDataThis == *iDataSub)
		{
			iDataTemp = iDataThis;		// �ӽ÷� ���

			// ���� ��
			iDataThis++;
			iDataSub++;

			// ����
			m_List.erase( iDataTemp );
		}
		// this�� �� ���� ���..
		else if (*iDataThis < *iDataSub)
		{
			// this�� ���� ������..
			iDataThis++;
		}
		// this�� �� ū ���
		else
		{
			iDataSub++;
		}
	}	
}

#endif