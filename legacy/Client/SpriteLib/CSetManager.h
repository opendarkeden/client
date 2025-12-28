//----------------------------------------------------------------------
// CSetManager.h
//----------------------------------------------------------------------
// Template Sorted List
// 내부적으로 stl의 list를 사용했다.
//----------------------------------------------------------------------
//
// 단지 DataType값만 저장한다.
// Ascending Sort이다.
//
//----------------------------------------------------------------------


#ifndef	__CSETMANAGER_H__
#define	__CSETMANAGER_H__

#pragma warning(disable:4786)

#include <Windows.h>
#include <list>
#include "DrawTypeDef.h"

template <class DataType, class SizeType>
class CSetManager {
	public :		
		typedef std::list<DataType>	DATA_LIST;

	public :
		CSetManager();
		~CSetManager();

		//--------------------------------------------------------
		// Init/Release		
		//--------------------------------------------------------
		void		Release();

		//--------------------------------------------------------
		// add / remove
		//--------------------------------------------------------
		bool		Add(const DataType data);		
		bool		Remove(const DataType data);

		//--------------------------------------------------------
		// file I/O		
		//--------------------------------------------------------		
		bool		SaveToFile(class ofstream& file);
		bool		LoadFromFile(class ifstream& file);

		//--------------------------------------------------------
		// Get functions
		//--------------------------------------------------------
		SizeType	GetSize() const	{ return m_List.size(); }
		
		// 첫번째 위치의 List Iterater를 넘겨준다.
		DATA_LIST::const_iterator	GetIterator() const	{ return m_List.begin(); }

	protected :			
		DATA_LIST			m_List;		// Data pointer들을 저장해둔다.

		// sizeof(SizeType) 의 값
		static BYTE			s_SIZEOF_SizeType;
};



//----------------------------------------------------------------------
//
// Initialize static data member
//
//----------------------------------------------------------------------
template <class DataType, class SizeType>
BYTE	CSetManager<DataType, SizeType>::s_SIZEOF_SizeType = sizeof(SizeType);


//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
template <class DataType, class SizeType> 
CSetManager<DataType, SizeType>::CSetManager()
{
}

template <class DataType, class SizeType> 
CSetManager<DataType, SizeType>::~CSetManager()
{
	Release();
}


//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
template <class DataType, class SizeType>
void
CSetManager<DataType, SizeType>::Release()
{
	m_List.clear();		
}

//----------------------------------------------------------------------
// Add
//----------------------------------------------------------------------
// list에 data를 추가한다.
// Sort되어 추가되고 중복을 허용하지 않는다.
// 
// 이미 존재하는 값이면 false를 return한다.
//----------------------------------------------------------------------
template <class DataType, class SizeType>
bool	
CSetManager<DataType, SizeType>::Add(const DataType data)
{
	DATA_LIST::iterator iData = m_List.begin();

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
// list에서 data를 지운다.
//
// 없는 값이면 return false
//----------------------------------------------------------------------
template <class DataType, class SizeType>
bool
CSetManager<DataType, SizeType>::Remove(const DataType data)
{
	DATA_LIST::iterator iData = m_List.begin();

	while (iData != m_List.end())
	{		
		// 같은 값이면 지운다.
		if (*iData==data)
		{
			m_List.erase(iData);
			return true;
		}
		
		// 현재 위치에 있는 값이 data보다 크다면
		// 계속 큰 값만 있으므로 data값이 없는 것이다.
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
// Save To File
//----------------------------------------------------------------------
//
// size를 저장하고
// 모든 list의 node들을 저장한다.
//
//----------------------------------------------------------------------
template <class DataType, class SizeType>
bool
CSetManager<DataType, SizeType>::SaveToFile(class ofstream& file)
{
	// size
	SizeType size = m_List.size();

	// size저장
	file.write((const char *)&size, s_SIZEOF_SizeType);

	// 아무 것도 없으면
	if (size==0)
	{
		return false;
	}

	DataType data;

	// 모든 Data들을 save한다.
	DATA_LIST::iterator iData = m_List.begin();

	int dataSize = sizeof(DataType);

	for (SizeType i=0; i<size; i++)
	{
		data = *iData;

		// file에 저장
		file.write((const char *)&data, dataSize);		

		iData++;
	}
	
	return true;
}

//----------------------------------------------------------------------
// Load from File
//----------------------------------------------------------------------
template <class DataType, class SizeType>
bool
CSetManager<DataType, SizeType>::LoadFromFile(class ifstream& file)
{
	// 이전에 있던 list를 지운다.
	Release();

	SizeType size;

	// file에서 size를 읽어온다.
	file.read((char*)&size, s_SIZEOF_SizeType);

	// 아무것도 저장된게 없을 경우
	if (size==0)
	{
		return false;
	}

	DataType	data;

	int dataSize = sizeof(DataType);

	// size개 만큼을 load한다.
	for (SizeType i=0; i<size; i++)
	{
		// file에서 load한다.
		file.read((char*)&data, dataSize);

		// list에 추가한다.
		Add( data );
	}
	
	return true;
}


#endif



