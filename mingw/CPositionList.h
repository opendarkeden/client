//----------------------------------------------------------------------
// CPositionList.h
//----------------------------------------------------------------------
// 
// (X,Y)좌표를 갖고 있는 class이다.
//
// (X,Y)들이 
// 뭔가를 의미하겠지만,
// PositionList class는 단지 그 좌표를 모아둔 것일 뿐이다.
//
// PositionList는 list로 구현한다.
//
//----------------------------------------------------------------------

#ifndef	__CPOSITIONLIST_H__
#define	__CPOSITIONLIST_H__

#include <Windows.h>
#include <list>
//#include <fstream>



//----------------------------------------------------------------------
//
// POSITION_NODE class  -  (X,Y) 좌표를 의미한다.
//
//----------------------------------------------------------------------
template <class Type>
class POSITION_NODE {
	public :
		void	operator = (const POSITION_NODE& node)
		{
			X = node.X;
			Y = node.Y;
		}

		bool	operator == (const POSITION_NODE& node)
		{
			return X==node.X && Y==node.Y;
		}

		bool	operator > (const POSITION_NODE& node)
		{
			if (X > node.X) return true;
			if (X < node.X) return false;
			if (Y > node.Y) return true;
			return false;
		}

		bool	operator < (const POSITION_NODE& node)
		{
			if (X < node.X) return true;
			if (X > node.X) return false;
			if (Y < node.Y) return true;
			return false;
		}

	public :
		Type			X;
		Type			Y;
};



//----------------------------------------------------------------------
//
// CPositionList class  -  POSITION_NODE의 list이다.
//
//----------------------------------------------------------------------
template <class Type>
class CPositionList {
	public :
		//----------------------------------------------------------------------
		// IMAGEOBJECT_INFO_LIST를 정의한다.
		//----------------------------------------------------------------------
		typedef	POSITION_NODE<Type>				POSITION_NODE_TYPE;
		typedef	std::list<POSITION_NODE_TYPE>	POSITION_LIST;

	public :
		CPositionList();
		~CPositionList(); 

		//--------------------------------------------------------------
		// Release
		//--------------------------------------------------------------
		void		Release();

		//--------------------------------------------------------------
		// Add    : (x,y)를 추가. 이미 있는지 check해봐야 한다.
		//--------------------------------------------------------------
		bool		Add(Type x, Type y);

		//--------------------------------------------------------------
		// Remove : (x,y)를 제거
		//--------------------------------------------------------------
		bool		Remove(Type x, Type y);

		//--------------------------------------------------------------
		// Get 
		//--------------------------------------------------------------
		int								GetSize() const		{ return m_listPosition.size(); }
		POSITION_LIST::const_iterator	GetIterator() const	{ return m_listPosition.begin(); }


		//--------------------------------------------------------------
		// File I/O
		//--------------------------------------------------------------
		void		SaveToFile(ofstream& file);
		void		LoadFromFile(ifstream& file);

	

	protected :
		POSITION_LIST		m_listPosition;

		
		static BYTE			s_SizeOfPositionType;
};


//----------------------------------------------------------------------
// CPositionList.cpp
//----------------------------------------------------------------------

//#include "CPositionList.h"

//----------------------------------------------------------------------
// Init Static Member
//----------------------------------------------------------------------
template <class Type>
BYTE	CPositionList<Type>::s_SizeOfPositionType	= sizeof(Type);

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
template <class Type>
CPositionList<Type>::CPositionList()
{
}

template <class Type>
CPositionList<Type>::~CPositionList()
{
	Release();
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
template <class Type>
void
CPositionList<Type>::Release()
{
	m_listPosition.clear();
}

//----------------------------------------------------------------------
// Add(x,y)
//----------------------------------------------------------------------
// (x,y)를 추가 : 이미 있는지 check해봐야 한다.
// 편의상.. X, Y순으로 Sort해서 추가한다.
//----------------------------------------------------------------------
template <class Type>
bool
CPositionList<Type>::Add(Type x, Type y)
{
	// node생성
	POSITION_NODE<Type>	node;
	node.X = x;
	node.Y = y;

	POSITION_LIST::iterator iPosition = m_listPosition.begin();

	// 이미 list에 들어있는 모든 position과 비교해보고
	// 같은게 없을 때 추가한다.
	while (iPosition != m_listPosition.end())
	{
		// 현재 들어있는게 추가할려는것보다 클 경우,
		// 현재 위치에 추가하면 된다.
		if (*iPosition > node)
		{
			m_listPosition.insert(iPosition, node);
			return true;
		}

		// 이미 있는 값이면
		// 추가하지 않는다.
		if (*iPosition==node)
		{
			return false;
		}

		iPosition++;
	}	

	// list의 모든 원소들보다 크므로 
	// list의 끝에 추가한다.
	m_listPosition.push_back( node );

	return true;
}

//----------------------------------------------------------------------
// Remove
//----------------------------------------------------------------------
// (x,y)를 제거
//----------------------------------------------------------------------
template <class Type>
bool		
CPositionList<Type>::Remove(Type x, Type y)
{
	// node생성
	POSITION_NODE<Type>	node;
	node.X = x;
	node.Y = y;

	POSITION_LIST::iterator iPosition = m_listPosition.begin();

	// list의 POSITION_NODE에서 같은게 있으면 지운다.
	while (iPosition != m_listPosition.end())
	{		
		// 같은 값이면 지운다.
		if (*iPosition==node)
		{
			m_listPosition.erase(iPosition);
			return true;
		}
		
		// 현재 위치에 있는 값이 Position보다 크다면
		// 계속 큰 값만 있으므로 Position값이 없는 것이다.
		if (*iPosition > node)
		{			
			return false;
		}

		iPosition++;
	}	

	// 없는 경우
	return false;
}

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
// list를 따라가면서 모든 position들을 file에 저장한다.
//----------------------------------------------------------------------
template <class Type>
void		
CPositionList<Type>::SaveToFile(ofstream& file)
{
	//----------------------------------------
	// Size저장
	//----------------------------------------
	WORD size = m_listPosition.size();

	file.write((const char*)&size, 2);

	// 아무것도 없으면 return
	if (size==0)
		return;


	POSITION_LIST::iterator iPosition = m_listPosition.begin();

	POSITION_NODE<Type>	node;

	//----------------------------------------
	// 각각의 POSITION_NODE를 저장한다.
	//----------------------------------------
	while (iPosition != m_listPosition.end())
	{		
		node = *iPosition;

		file.write((const char*)&node.X, s_SizeOfPositionType);
		file.write((const char*)&node.Y, s_SizeOfPositionType);

		iPosition++;
	}	
	
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
template <class Type>
void		
CPositionList<Type>::LoadFromFile(ifstream& file)
{
	// 이전에 있던 list를 제거
	Release();

	WORD size;

	//----------------------------------------
	// size를 읽어온다.
	//----------------------------------------
	file.read((char*)&size, 2);

	// 아무것도 없으면 return
	if (size==0)
		return;

	POSITION_NODE<Type> node;

	//----------------------------------------
	// size개수만큼 POSITION_NODE를 읽어오면서
	// list에 추가시킨다.
	//----------------------------------------
	for (int i=0; i<size; i++)
	{		
		file.read((char*)&node.X, s_SizeOfPositionType);
		file.read((char*)&node.Y, s_SizeOfPositionType);

		m_listPosition.push_back( node );
	}
}


//----------------------------------------------------------------------
// Test Code
//----------------------------------------------------------------------
/*
template <class Type>
void		
CPositionList<Type>::Print()
{

	POSITION_LIST::iterator iPosition = m_listPosition.begin();

	POSITION_NODE<Type> node;

	while (iPosition != m_listPosition.end())
	{
		node = *iPosition;

		cout << "( " << node.X << ", " << node.Y << " )" << endl;

		iPosition++;
	}
}


void main()
{
	CPositionList<int>	list;
	
	list.Add(10,10);	
	list.Add(16,17);
	list.Add(10,12);
	list.Add(12,17);
	list.Remove(12,16);
	list.Add(10,12);	
	list.Add(11,15);
	list.Add(10,11);
	list.Remove(10,10);
	list.Add(11,15);
	list.Add(10,11);
	list.Remove(12,17);
	list.Add(11,9);
	list.Remove(11,9);
	list.Add(11,9);	
	list.Add(10,7);

	list.Print();
}


//----------
// Result 
//----------
( 10, 7 )
( 10, 11 )
( 10, 12 )
( 11, 9 )
( 11, 15 )
( 16, 17 )


*/


#endif