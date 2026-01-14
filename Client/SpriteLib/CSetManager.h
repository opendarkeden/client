//----------------------------------------------------------------------
// CSetManager.h
//----------------------------------------------------------------------
// Template Sorted List
// ���������� stl�� list�� ����ߴ�.
//----------------------------------------------------------------------
//
// ���� DataType���� �����Ѵ�.
// Ascending Sort�̴�.
//
//----------------------------------------------------------------------


#ifndef	__CSETMANAGER_H__
#define	__CSETMANAGER_H__

#pragma warning(disable:4786)

#ifdef PLATFORM_WINDOWS
	#include <Windows.h>
#else
	#include "../basic/Platform.h"
#endif
#include <list>
#include <fstream>
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
		bool		SaveToFile(std::ofstream& file);
		bool		LoadFromFile(std::ifstream& file);

		//--------------------------------------------------------
		// Get functions
		//--------------------------------------------------------
		SizeType	GetSize() const	{ return m_List.size(); }

		// ù��° ��ġ�� List Iterater�� �Ѱ��ش�.
		typename DATA_LIST::const_iterator	GetIterator() const	{ return m_List.begin(); }

	protected :			
		DATA_LIST			m_List;		// Data pointer���� �����صд�.

		// sizeof(SizeType) �� ��
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
// list�� data�� �߰��Ѵ�.
// Sort�Ǿ� �߰��ǰ� �ߺ��� ������� �ʴ´�.
// 
// �̹� �����ϴ� ���̸� false�� return�Ѵ�.
//----------------------------------------------------------------------
template <class DataType, class SizeType>
bool	
CSetManager<DataType, SizeType>::Add(const DataType data)
{
	typename DATA_LIST::iterator iData = m_List.begin();

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
// list���� data�� �����.
//
// ���� ���̸� return false
//----------------------------------------------------------------------
template <class DataType, class SizeType>
bool
CSetManager<DataType, SizeType>::Remove(const DataType data)
{
	typename DATA_LIST::iterator iData = m_List.begin();

	while (iData != m_List.end())
	{		
		// ���� ���̸� �����.
		if (*iData==data)
		{
			m_List.erase(iData);
			return true;
		}
		
		// ���� ��ġ�� �ִ� ���� data���� ũ�ٸ�
		// ��� ū ���� �����Ƿ� data���� ���� ���̴�.
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
// Save To File
//----------------------------------------------------------------------
//
// size�� �����ϰ�
// ��� list�� node���� �����Ѵ�.
//
//----------------------------------------------------------------------
template <class DataType, class SizeType>
bool
CSetManager<DataType, SizeType>::SaveToFile(std::ofstream& file)
{
	// size
	SizeType size = m_List.size();

	// size����
	file.write((const char *)&size, s_SIZEOF_SizeType);

	// �ƹ� �͵� ������
	if (size==0)
	{
		return false;
	}

	DataType data;

	// ��� Data���� save�Ѵ�.
	typename DATA_LIST::iterator iData = m_List.begin();

	int dataSize = sizeof(DataType);

	for (SizeType i=0; i<size; i++)
	{
		data = *iData;

		// file�� ����
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
CSetManager<DataType, SizeType>::LoadFromFile(std::ifstream& file)
{
	// ������ �ִ� list�� �����.
	Release();

	SizeType size;

	// file���� size�� �о�´�.
	file.read((char*)&size, s_SIZEOF_SizeType);

	// �ƹ��͵� ����Ȱ� ���� ���
	if (size==0)
	{
		return false;
	}

	DataType	data;

	int dataSize = sizeof(DataType);

	// size�� ��ŭ�� load�Ѵ�.
	for (SizeType i=0; i<size; i++)
	{
		// file���� load�Ѵ�.
		file.read((char*)&data, dataSize);

		// list�� �߰��Ѵ�.
		Add( data );
	}
	
	return true;
}


#endif



