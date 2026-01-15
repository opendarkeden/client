//----------------------------------------------------------------------
// CDataTable.h
//----------------------------------------------------------------------

#ifndef	__CDATATABLE_H__
#define	__CDATATABLE_H__

//#include "DebugInfo.h"
//#define	 new DEBUG_NEW

//----------------------------------------------------------------------
//
// Info에 대한 정보 Table
//
//----------------------------------------------------------------------
template <class Type>
class CDataTable {
	public :
		CDataTable();
		~CDataTable();

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

	protected :		
		int			m_Size;					// Type 종류 수
		Type*		m_pTypeInfo;			// Type 정보

		static int	s_SizeOfData;
};

//----------------------------------------------------------------------
// static
//----------------------------------------------------------------------
template <class Type>
int CDataTable<Type>::s_SizeOfData = sizeof(Type);

//----------------------------------------------------------------------
//
//    constructor/destructor
//
//----------------------------------------------------------------------
template <class Type>
CDataTable<Type>::CDataTable()
{
	m_pTypeInfo	= NULL;
	m_Size		= 0;
}

template <class Type>
CDataTable<Type>::~CDataTable()
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
CDataTable<Type>::Init(int size)
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
CDataTable<Type>::Release()
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
CDataTable<Type>::SaveToFile(ofstream& file)
{
	// size 저장
	file.write((const char*)&m_Size, 4);

	// 아무 것도 없는 경우
	if (m_pTypeInfo==NULL)
		return;

	// 각각의 정보 저장
	for (int i=0; i<m_Size; i++)
	{
		file.write((const char*)&m_pTypeInfo[i], s_SizeOfData);		
	}
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
template <class Type>
void			
CDataTable<Type>::LoadFromFile(ifstream& file)
{
	int numSize;

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
 		file.read((char*)&m_pTypeInfo[i], s_SizeOfData);
	}
}


#endif