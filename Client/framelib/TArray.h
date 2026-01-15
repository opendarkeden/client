//----------------------------------------------------------------------
// TArray.h
//----------------------------------------------------------------------
//
// Template Array class
//
//----------------------------------------------------------------------
//
// Data Type과  Size Type이 Template이다.
//
// File I/O를 하려면  Data가 되는 class에 
//      bool		SaveToFile(ofstream& file);
//		bool		LoadFromFile(ifstream& file);  이 구현되어야 한다.
//
//----------------------------------------------------------------------

#ifndef	__TARRAY_H__
#define	__TARRAY_H__


#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#else
#include "../../basic/Platform.h"
#endif
#include <fstream.h>
//class ofstream;
//class ifstream;

template <class DataType, class SizeType>
class TArray {
	public :
		TArray(SizeType size=0);
		~TArray();

		//--------------------------------------------------------
		// Init / Release
		//--------------------------------------------------------
		void		Init(SizeType size);
		void		Release();
	

		//--------------------------------------------------------
		// Get Size
		//--------------------------------------------------------
		SizeType	GetSize() const				{ return m_Size; }
	
		//--------------------------------------------------------
		// File I/O
		//--------------------------------------------------------
		bool		SaveToFile(class ofstream& file);
		bool		LoadFromFile(class ifstream& file);

		//--------------------------------------------------------
		// Operator overloading
		//--------------------------------------------------------
		DataType&	operator [] (SizeType n)		{ return m_pData[n]; }
		const DataType&	operator [] (SizeType n) const	{ return m_pData[n]; }
		void		operator = (const TArray<DataType, SizeType>& array);
		
		// 현재 Array에 다른 array를 더한다.
		void		operator += (const TArray<DataType, SizeType>& array);


	protected :
		SizeType		m_Size;
		DataType*		m_pData;

		// sizeof(SizeType) 의 값
		static BYTE		s_SIZEOF_SizeType;
};




//----------------------------------------------------------------------
//
// Initialize static data member
//
//----------------------------------------------------------------------
template <class DataType, class SizeType>
BYTE	TArray<DataType, SizeType>::s_SIZEOF_SizeType = sizeof(SizeType);


//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
template <class DataType, class SizeType> 
TArray<DataType, SizeType>::TArray(SizeType size)
{
	m_Size	= 0;
	m_pData = NULL;	

	Init(size);
}

template <class DataType, class SizeType> 
TArray<DataType, SizeType>::~TArray()
{
	Release();
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Init
//----------------------------------------------------------------------
template <class DataType, class SizeType> 
void	
TArray<DataType, SizeType>::Init(SizeType size)
{
	// 일단 해제
	Release();

	if (size==0) return;

	m_Size = size;
	m_pData = new DataType [m_Size];
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
template <class DataType, class SizeType> 
void	
TArray<DataType, SizeType>::Release()
{
	if (m_pData!=NULL)
	{
		delete [] m_pData;
		m_pData = NULL;
		
		m_Size = 0;
	}
}

//----------------------------------------------------------------------
// Add Array to *this
//----------------------------------------------------------------------
// 두 Array에 존재하는 data의 개수를 더한만큼의 
// memory를 *this에 다시 잡고 copy~~
//----------------------------------------------------------------------
template <class DataType, class SizeType>
void
TArray<DataType, SizeType>::operator += (const TArray<DataType, SizeType>& array)
{
	SizeType newSize = m_Size + array.m_Size;

	//------------------------------------------------
	// 두 Array를 더한 개수만큼의 memory를 잡는다.
	//------------------------------------------------
	DataType*	pTempData = new DataType [newSize];
	
	//------------------------------------------------
	// temp에 *this를 copy
	//------------------------------------------------
	SizeType k=0;
	for (SizeType i=0; i<m_Size; i++)
	{
		pTempData[k] = m_pData[i];		
		
		k++;
	}

	//------------------------------------------------
	// temp에 FramePack을 copy
	//------------------------------------------------
	for (i=0; i<array.m_Size; i++)
	{
		pTempData[k] = array.m_pData[i];
		
		k++;
	}

	//------------------------------------------------
	// memory해제한다.
	//------------------------------------------------
	Release();

	//------------------------------------------------
	// *this가 temp를 가리키도록 한다.
	//------------------------------------------------
	m_Size		= newSize;
	m_pData		= pTempData;	
}


//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
template <class DataType, class SizeType> 
bool
TArray<DataType, SizeType>::SaveToFile(class ofstream& file)
{
	// 0이라도 개수는 저장한다.
	file.write((const char*)&m_Size, s_SIZEOF_SizeType);

	// 아무것도 없으면..
	if (m_pData==NULL || m_Size==0) 
		return false;

	for (SizeType i=0; i<m_Size; i++)
		m_pData[i].SaveToFile(file);

	return true;
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
template <class DataType, class SizeType> 
bool
TArray<DataType, SizeType>::LoadFromFile(class ifstream& file)
{
	// frame 개수
	file.read((char*)&m_Size, s_SIZEOF_SizeType);

	if (m_Size==0) return false;
	
	// memory잡기
	Init(m_Size);

	for (SizeType i=0; i<m_Size; i++)
		m_pData[i].LoadFromFile(file);

	return true;
}

//----------------------------------------------------------------------
// assign
//----------------------------------------------------------------------
template <class DataType, class SizeType> 
void	
TArray<DataType, SizeType>::operator = (const TArray<DataType, SizeType>& array)
{
	// frameArray와 똑같이 해야 한다.
	Init( array.m_Size );

	// 모든 element를 copy해야 한다.
	for (SizeType i=0; i<m_Size; i++)
	{
		m_pData[i] = array.m_pData[i];
	}
}


#endif
