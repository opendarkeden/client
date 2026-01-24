//----------------------------------------------------------------------
// CTypeTable.h
//----------------------------------------------------------------------
//���� 326220963
#ifndef	__CTYPETABLE_H__
#define	__CTYPETABLE_H__

//#include "DebugInfo.h"
//#define	 new DEBUG_NEW
#include <fstream>

//----------------------------------------------------------------------
//
// Info�� ���� ���� Table
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
		// Debug/Internal access
		//-------------------------------------------------------
		Type*			GetInternalPointer() const { return m_pTypeInfo; }
		
		//-------------------------------------------------------
		// Reference
		//-------------------------------------------------------
		const Type&	operator [] (int type) const {
#ifdef _DEBUG
			if (type < 0 || type >= m_Size) {
				static Type dummy;
				return dummy;
			}
#endif
			return m_pTypeInfo[type];
		}
		Type&	operator [] (int type) {
#ifdef _DEBUG
			if (type < 0 || type >= m_Size) {
				static Type dummy;
				return dummy;
			}
#endif
			return m_pTypeInfo[type];
		}
		Type&	Get(int type) {
#ifdef _DEBUG
			if (type < 0 || type >= m_Size) {
				static Type dummy;
				return dummy;
			}
#endif
			return m_pTypeInfo[type];
		}


		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		void			SaveToFile(std::ofstream& file);
		void			LoadFromFile(std::ifstream& file);
		void			SaveToFile(const char *filename);
		void			LoadFromFile(const char *filename);
		bool			LoadFromFile_NickNameString(std::ifstream& file);
	protected :		
		int			m_Size;					// Type ���� ��
		Type*		m_pTypeInfo;			// Type ����

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
	// ������ ���� ��� 
	if (size==0) 
		return;

	// �ϴ� ����
	Release();

	// �޸� ���
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
		// ��� CSprite�� �����.
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
CTypeTable<Type>::SaveToFile(std::ofstream& file)
{
	// size ����
	file.write((const char*)&m_Size, 4);

	// �ƹ� �͵� ���� ���
	if (m_pTypeInfo==NULL)
		return;

	// ������ ���� ����
	for (int i=0; i<m_Size; i++)
	{
		if (i==557)//ʯͷ����Ч��
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
CTypeTable<Type>::LoadFromFile(std::ifstream& file)
{
	int numSize=0;

	// size �о����
	file.read((char*)&numSize, 4);

	// ���� �����ִ� �޸𸮿� �ٸ��� �ٽ� �޸𸮸� ��´�.
	if (m_Size != numSize)
	{
		// �޸� ����
		Release();

		// �޸� ���
		Init( numSize );
	}

	// file���� ������ ������ �о���δ�.
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
	std::ifstream file(lpszFilename, std::ios::binary );
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
	std::ofstream file(lpszFilename, std::ios::binary);
	SaveToFile(file);
	file.close();
}
// 2004, 6, 18 sobeit add start - nick name - ���� ������ �� Ʋ���� �������� ����..^^:
//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
template <class Type>
bool			
CTypeTable<Type>::LoadFromFile_NickNameString(std::ifstream& file)
{
	int numSize;
	WORD wIndex;
	// size �о����
	file.read((char*)&numSize, 4);

	// ���� �����ִ� �޸𸮿� �ٸ��� �ٽ� �޸𸮸� ��´�.
	if (m_Size != numSize)
	{
		// �޸� ����
		Release();

		// �޸� ���
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
// 2004, 6, 18 sobeit add start - nick name - ���� ������ �� Ʋ���� �������� ����..^^:
#endif