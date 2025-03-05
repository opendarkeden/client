//----------------------------------------------------------------------
// CTypeTable.h
//----------------------------------------------------------------------
//행행 326220963
#ifndef	__CTYPETABLE_H__
#define	__CTYPETABLE_H__

//#include "DebugInfo.h"
//#define	 new DEBUG_NEW

#include <fstream>
using std::ifstream, std::ofstream;
using std::ios;

//----------------------------------------------------------------------
//
// Info에 대한 정보 Table
//
//----------------------------------------------------------------------
template <class Type>
class CTypeTable {
	public :
		CTypeTable();
		~CTypeTable();

		//-------------------------------------------------------
		// Init / Release
		//-------------------------------------------------------
		void			Init(int size);
		void			Release();

		//-------------------------------------------------------
		// Size
		//-------------------------------------------------------
		int				GetSize() const		{ return m_Size; }
		
		//-------------------------------------------------------
		// Reference
		//-------------------------------------------------------		
		const Type&	operator [] (int type) const	{ return m_pTypeInfo[type]; }
		Type&	operator [] (int type)				{ return m_pTypeInfo[type]; }
		Type&	Get(int type)						{ return m_pTypeInfo[type]; }


		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		void			SaveToFile(ofstream& file);
		void			LoadFromFile(ifstream& file);
		void			SaveToFile(const char *filename);
		void			LoadFromFile(const char *filename);
		bool			LoadFromFile_NickNameString(ifstream& file);
	protected :		
		int			m_Size;					// Type 종류 수
		Type*		m_pTypeInfo;			// Type 정보

};


//----------------------------------------------------------------------
//
//    constructor/destructor
//
//----------------------------------------------------------------------
template <class Type>
CTypeTable<Type>::CTypeTable()
{
	m_pTypeInfo	= NULL;
	m_Size		= 0;
}

template <class Type>
CTypeTable<Type>::~CTypeTable()
{
	Release();
}

//----------------------------------------------------------------------
//
//  member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Init
//----------------------------------------------------------------------
template <class Type>
void
CTypeTable<Type>::Init(int size)
{
	// 개수가 없을 경우 
	if (size==0) 
		return;

	// 일단 해제
	Release();

	// 메모리 잡기
	m_Size = size;
	
	m_pTypeInfo = new Type [m_Size];	
}


//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
template <class Type>
void
CTypeTable<Type>::Release()
{
	if (m_pTypeInfo != NULL)
	{
		// 모든 CSprite를 지운다.
		delete [] m_pTypeInfo;
		m_pTypeInfo = NULL;
		
		m_Size = 0;
	}
}

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
template <class Type>
void			
CTypeTable<Type>::SaveToFile(ofstream& file)
{
	// size 저장
	file.write((const char*)&m_Size, 4);

	// 아무 것도 없는 경우
	if (m_pTypeInfo==NULL)
		return;

	// 각각의 정보 저장
	for (int i=0; i<m_Size; i++)
	{
		if (i==557)//柯庫럿쀼槻벎
		{
			i=i;
		}
		m_pTypeInfo[i].SaveToFile(file);
	}
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
template <class Type>
void			
CTypeTable<Type>::LoadFromFile(ifstream& file)
{
	int numSize=0;

	// size 읽어오기
	file.read((char*)&numSize, 4);

	// 현재 잡혀있는 메모리와 다르면 다시 메모리를 잡는다.
	if (m_Size != numSize)
	{
		// 메모리 해제
		Release();

		// 메모리 잡기
		Init( numSize );
	}

	// file에서 각각의 정보를 읽어들인다.
	for (int i=0; i<m_Size; i++)
	{
		if (i==700)
		{
			i=i;
		}
 		m_pTypeInfo[i].LoadFromFile( file );
	}
}

template <class Type>
void
CTypeTable<Type>::LoadFromFile(const char* lpszFilename)
{
	ifstream file(lpszFilename, ios::binary | ios::in);
	if(file.is_open())
	{
		LoadFromFile(file);
		file.close();
	}
}

template <class Type>
void
CTypeTable<Type>::SaveToFile(const char* lpszFilename)
{
	ofstream file(lpszFilename, ios::binary);
	SaveToFile(file);
	file.close();
}
// 2004, 6, 18 sobeit add start - nick name - 파일 구조가 쩜 틀려서 전용으로 만듬..^^:
//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
template <class Type>
bool			
CTypeTable<Type>::LoadFromFile_NickNameString(ifstream& file)
{
	int numSize;
	WORD wIndex;
	// size 읽어오기
	file.read((char*)&numSize, 4);

	// 현재 잡혀있는 메모리와 다르면 다시 메모리를 잡는다.
	if (m_Size != numSize)
	{
		// 메모리 해제
		Release();

		// 메모리 잡기
		Init( numSize );
	}
	
	
	for (int i=0; i<m_Size; i++)
	{
		file.read((char*)&wIndex, 2);
		if(wIndex>=numSize)
			return false;
 		m_pTypeInfo[wIndex].LoadFromFile( file );
	}
	return true;
}
// 2004, 6, 18 sobeit add start - nick name - 파일 구조가 쩜 틀려서 전용으로 만듬..^^:
#endif
