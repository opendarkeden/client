#ifndef __CTYPEPACK_H__
#define __CTYPEPACK_H__

#ifdef PLATFORM_WINDOWS
	#include <windows.h>
	#include <fstream.h>
#else
	#include "../basic/Platform.h"
	#include <fstream>
	#include <cstring>
	using namespace std;
#endif
#include "CSpriteSetManager.h"
#include "COrderedList.h"
#ifdef PLATFORM_WINDOWS
	#include "CDirectDraw.h"
#else
	#include "../DXLib/CDirectDraw.h"
#endif
#include <VECTOR>

template <class Type>
class CTypePack
{
public:
	CTypePack();
	virtual ~CTypePack();
	
	//--------------------------------------------------------
	// Init/Release
	//--------------------------------------------------------
	virtual void	Init(WORD size);
	virtual void	Release();
	
	//--------------------------------------------------------
	// Size
	//--------------------------------------------------------
	DWORD		GetSize() const { return m_Size; }
	
	//--------------------------------------------------------
	// operator
	//--------------------------------------------------------
	Type&		operator [] (WORD n);//		{ return m_pSpritePals[n]; }
	Type&		Get(WORD n);

	//--------------------------------------------------------
	// file I/O
	//--------------------------------------------------------
	virtual bool LoadFromFile(std::ifstream&file);
	virtual bool SaveToFile(std::ofstream&dataFile, std::ofstream&indexFile);
	
	virtual bool LoadFromFileRunning(LPCTSTR lpszFilename);
	virtual bool LoadFromFile(LPCTSTR lpszFilename);
	virtual bool LoadFromFilePart(int first, int last);
	virtual bool LoadFromFilePart(const CSpriteSetManager& SSM);
	virtual bool LoadFromFileData(int dataID, int fileID, LPCTSTR packFilename, LPCTSTR indexFilename);

	virtual bool ReleasePart(int first, int last);
	virtual bool ReleasePart(COrderedList<TYPE_SPRITEID> list);

	virtual bool SaveToFile(LPCTSTR lpszFilename);
	
protected:
	Type *			m_pData;
	WORD			m_Size;
	bool			m_bRunningLoad;

	// runtime loading
	WORD			m_nLoadData;	// Loading �� CSprite�� ����
	std::ifstream *m_file;
	int*			m_file_index;
};

template <class Type>
CTypePack<Type>::CTypePack()
{
	m_pData = NULL;
	m_Size = 0;
	m_bRunningLoad = false;
	m_nLoadData = 0;
	m_file_index = NULL;
	m_file = NULL;
}

template <class Type>
CTypePack<Type>::~CTypePack()
{
	Release();
}

template <class Type>
void CTypePack<Type>::Release()
{
	m_bRunningLoad = false;
	
	if(m_file != NULL)
	{
		delete m_file;
		m_file = NULL;
	}

	if(m_file_index != NULL)
	{
		delete []m_file_index;
		m_file_index = NULL;
	}

	if(m_pData != NULL)
	{
		delete []m_pData;
		m_pData = NULL;
		m_Size = 0;
	}
}

template <class Type>
void CTypePack<Type>::Init(WORD size)
{
	if(size == 0)
		return;
	
	Release();
	
	m_Size = size;
	
	m_pData = new Type[size];
}

template <class Type>
Type &CTypePack<Type>::operator [] (WORD n)
{
	return Get(n);
}

template <class Type>
Type &CTypePack<Type>::Get(WORD n)
{
	if(m_bRunningLoad && !m_pData[n].IsInit())
	{
		m_file->seekg(m_file_index[n]);
		// file�� �ִ� Sprite���� Load	
		m_pData[n].LoadFromFile(*m_file);	// Sprite �о����
		if(++m_nLoadData >= m_Size)
		{
			m_bRunningLoad = false;
			m_file->close();
			delete m_file;
			m_file = NULL;
			delete []m_file_index;
			m_file_index = NULL;
		}
	}
	
	return m_pData[n];
}

template <class Type>
bool CTypePack<Type>::LoadFromFile(LPCTSTR lpszFilename)
{
	std::ifstream file(lpszFilename, ios::binary);
	bool re = LoadFromFile(file);
	file.close();

	return re;
}

template <class Type>
bool CTypePack<Type>::SaveToFile(LPCTSTR lpszFilename)
{
	char szIndexFilename[512];
	sprintf(szIndexFilename, "%si", lpszFilename);

	std::ofstream dataFile(lpszFilename, ios::binary);
	std::ofstream indexFile(szIndexFilename, ios::binary);

	bool re = SaveToFile(dataFile, indexFile);

	dataFile.close();
	indexFile.close();

	return re;
}


template <class Type>
bool CTypePack<Type>::LoadFromFile(std::ifstream&file)
{
//	Release();

	file.read((char *)&m_Size, 2);
	
	Init(m_Size);
	
	int i;

	for(i = 0; i < m_Size; i++)
	{
		m_pData[i].LoadFromFile(file);
	}
	
	return true;
}

//----------------------------------------------------------------------
// Load From File Running
//----------------------------------------------------------------------
// �ǽð� �ε�
//----------------------------------------------------------------------
template <class Type>
bool CTypePack<Type>::LoadFromFileRunning(LPCTSTR lpszFilename)
{
	//�ε��� ���� �ε�
	std::string filename = lpszFilename;
	filename += 'i';
	std::ifstream indexFile(filename.c_str(), ios::binary);
	indexFile.read((char *)&m_Size, 2); 
	Init(m_Size);

	if(m_file == NULL)
	{
		m_file = new std::ifstream;
	}
	
	m_file_index = new int[m_Size];
	for (int i = 0; i < m_Size; i++)
	{
		indexFile.read((char*)&m_file_index[i], 4);
	}
	indexFile.close();
	
	// file���� sprite ������ �о�´�.	
	m_file->open(lpszFilename, ios::binary);
	
	m_file->read((char*)&m_Size, 2);
	
	m_bRunningLoad = true;
	m_nLoadData = 0;
	
	return true;
}

template <class Type>
bool CTypePack<Type>::SaveToFile(std::ofstream&dataFile, std::ofstream&indexFile)
{
	//--------------------------------------------------
	// index file�� �����ϱ� ���� ����
	//--------------------------------------------------
//	long*	pIndex = new long [m_Size];
	std::vector<DWORD> vIndex;

	//--------------------------------------------------
	// Size ����
	//--------------------------------------------------
	dataFile.write((const char *)&m_Size, 2);
	indexFile.write((const char *)&m_Size, 2);
	WORD realSize = m_Size;
	DWORD index = 0;
	int i;  // Declare at function scope for both loops

	for(i = 0; i < m_Size; i++)
	{
		index = dataFile.tellp();
		if(m_pData[i].SaveToFile(dataFile) == false)
		{
			realSize--;
		}
		else
		{
			vIndex.push_back(index);
		}
	}
	
	if(realSize != m_Size)
	{
		char szTemp[512];
		sprintf(szTemp, "real size : %d size : %d", realSize, m_Size);
		MessageBox(NULL, szTemp, "CTypePack", MB_OK);

		dataFile.seekp(0);
		dataFile.write((const char *)&realSize, 2);
		indexFile.seekp(0);
		indexFile.write((const char *)&realSize, 2);
	}

	//--------------------------------------------------
	// index ����
	//--------------------------------------------------
	for (i=0; i<vIndex.size(); i++)
	{
		indexFile.write((const char*)&vIndex[i], 4);
	}
	
//	delete [] pIndex;
	
//	indexFile.close();
//	dataFile.close();

	if(m_bRunningLoad)
	{
		m_bRunningLoad = false;
		m_file->close();
		delete m_file;
		m_file = NULL;
		delete []m_file_index;
		m_file_index = NULL;
	}

	return true;
}

template <class Type>
bool CTypePack<Type>::LoadFromFilePart(int first, int last)
{
	last = min(last, 0xFFFE);
	for(int i = first; i <= last; i++)
		operator[](i);

	return true;
}

template <class Type>
bool CTypePack<Type>::LoadFromFilePart(const CSpriteSetManager& SSM)
{
	CSpriteSetManager::DATA_LIST::const_iterator iID = SSM.GetIterator();
	for (int t=0; t<SSM.GetSize(); t++)
	{
		if(*iID != 0xFFFF)
			Get(*iID);
	}

	return true;
}

template <class Type>
bool CTypePack<Type>::ReleasePart(int first, int last)
{
	last = min(last, 0xFFFE);
	for(int i = first; i <= last; i++)
		m_pData[i].Release();

	return true;
}

template <class Type>
bool CTypePack<Type>::ReleasePart(COrderedList<TYPE_SPRITEID> list)
{
	COrderedList<TYPE_SPRITEID>::DATA_LIST::const_iterator iID = list.GetIterator();
	for (int t=0; t<list.GetSize(); t++)
	{
		if(*iID != 0xFFFF)
			m_pData[*iID].Release();
	}

	return true;
}

template <class Type>
bool CTypePack<Type>::LoadFromFileData(int dataID, int fileID, LPCTSTR packFilename, LPCTSTR indexFilename)
{
	if (dataID < 0 || dataID >= m_Size)
	{
		return false;
	}

	std::ifstream dataFile(packFilename, ios::binary);
	
	if (!dataFile.is_open())
	{
		return false;
	}
	
	std::ifstream indexFile(indexFilename, ios::binary);
	
	if (!indexFile.is_open())
	{
		return false;
	}
	
	//-------------------------------------------------------------------
	// index�� ������ üũ�Ѵ�. fileID�� �ִ���..?
	//-------------------------------------------------------------------
	TYPE_SPRITEID num;
	indexFile.read((char*)&num, sizeof(WORD));
	
	if (fileID >= num)
	{
		return false;
	}
	
	//-------------------------------------------------------------------
	// load�� data�� file pointer�� �д´�.
	//-------------------------------------------------------------------
	long fp;	
	indexFile.seekg( 2 + fileID*4 );		// 2(num) + spriteID * (4 bytes)
	indexFile.read((char*)&fp, 4);
	
	dataFile.seekg( fp );	
	
	m_pData[dataID].LoadFromFile( dataFile );
	
	return true;
}

// CTypePack2
template <class TypeBase, class Type1, class Type2>
class CTypePack2
{
private:
	// Disable copy constructor and copy assignment to prevent issues with m_file pointer
	CTypePack2(const CTypePack2&) = delete;
	CTypePack2& operator=(const CTypePack2&) = delete;

	// Disable move constructor and move assignment to prevent m_file pointer from being moved
	CTypePack2(CTypePack2&&) = delete;
	CTypePack2& operator=(CTypePack2&&) = delete;

public:
	CTypePack2();
	virtual ~CTypePack2();
	
	//--------------------------------------------------------
	// Init/Release
	//--------------------------------------------------------
	virtual void	Init(WORD size );
	virtual void	Release();
	
	//--------------------------------------------------------
	// Size
	//--------------------------------------------------------
	DWORD		GetSize() const { return m_Size; }
	
	//--------------------------------------------------------
	// operator
	//--------------------------------------------------------
	TypeBase&		operator [] (WORD n);//		{ return m_pSpritePals[n]; }
	TypeBase&		Get(WORD n);

	//--------------------------------------------------------
	// file I/O
	//--------------------------------------------------------
	virtual bool LoadFromFile(std::ifstream&file);
	virtual bool SaveToFile(std::ofstream&dataFile, std::ofstream&indexFile);
	
	virtual bool LoadFromFileRunning(LPCTSTR lpszFilename);
	virtual bool LoadFromFile(LPCTSTR lpszFilename);
	virtual bool LoadFromFilePart(int first, int last);
	virtual bool LoadFromFilePart(const CSpriteSetManager& SSM);
	virtual bool LoadFromFileData(int dataID, int fileID, LPCTSTR packFilename, LPCTSTR indexFilename);

	virtual bool ReleasePart(int first, int last);
	virtual bool ReleasePart(COrderedList<TYPE_SPRITEID> list);

	virtual bool SaveToFile(LPCTSTR lpszFilename);

	virtual bool Is565() { return m_bSecond; }
	
protected:
	TypeBase *			m_pData;
	WORD			m_Size;
	bool			m_bRunningLoad;

	// runtime loading
	WORD			m_nLoadData;	// Loading �� CSprite�� ����
	std::ifstream *m_file;
	int*			m_file_index;
	bool			m_bSecond;
};

template <class TypeBase, class Type1, class Type2>
CTypePack2<TypeBase, Type1, Type2>::CTypePack2()
{
	m_pData = NULL;
	m_Size = 0;
	m_bRunningLoad = false;
	m_nLoadData = 0;
	m_file_index = NULL;
	m_file = NULL;
}

template <class TypeBase, class Type1, class Type2>
CTypePack2<TypeBase, Type1, Type2>::~CTypePack2()
{
	Release();
}

template <class TypeBase, class Type1, class Type2>
void CTypePack2<TypeBase, Type1, Type2>::Release()
{
	printf("DEBUG Release: this=%p, m_file=%p, m_bRunningLoad=%d\n", this, m_file, m_bRunningLoad);
	m_bRunningLoad = false;

	if(m_file != NULL)
	{
		printf("DEBUG Release: this=%p, deleting m_file=%p\n", this, m_file);
		delete m_file;
		m_file = NULL;
	}

	if(m_file_index != NULL)
	{
		delete []m_file_index;
		m_file_index = NULL;
	}

	if(m_pData != NULL)
	{
		// IMPORTANT: Delete with correct type to match new Type1[size] or new Type2[size]
		// Using base class pointer to delete derived class array is UB even with virtual destructor
		if(m_bSecond)
			delete [] ((Type2*)m_pData);
		else
			delete [] ((Type1*)m_pData);
		m_pData = NULL;
		m_Size = 0;
	}
}

template <class TypeBase, class Type1, class Type2>
void CTypePack2<TypeBase, Type1, Type2>::Init(WORD size)
{
	if(size == 0)
		return;
	
	Release();
	
	m_Size = size;
	m_bSecond = CDirectDraw::Is565();

	if( m_bSecond == true )
		m_pData = new Type2[size];
	else
		m_pData = new Type1[size];
}

template <class TypeBase, class Type1, class Type2>
TypeBase &CTypePack2<TypeBase, Type1, Type2>::operator [] (WORD n)
{
	return Get(n);
}

template <class TypeBase, class Type1, class Type2>
TypeBase &CTypePack2<TypeBase, Type1, Type2>::Get(WORD n)
{
	if(m_bRunningLoad && !m_pData[n].IsInit())
	{
		// Safety check: disable lazy loading if file pointer is invalid
		if (m_file == NULL)
		{
			m_bRunningLoad = false;
			return m_pData[n];
		}

		// Validate sprite index
		if (n >= m_Size)
		{
			printf("WARNING Get[%d]: this=%p, sprite index %d out of range (size=%d)\n",
			       n, this, n, m_Size);
			m_bRunningLoad = false;
			return m_pData[n];
		}

		// Debug: print object and file pointer info BEFORE using m_file
		printf("DEBUG Get[%d]: this=%p, m_file=%p, m_nLoadData=%d, m_Size=%d\n",
		       n, this, m_file, m_nLoadData, m_Size);

		// Try to load sprite - use exception handler to detect file corruption
		try {
			// Check if file stream is valid before using it
			if (!m_file->good())
			{
				printf("WARNING Get[%d]: this=%p, m_file=%p is not good(), disabling lazy loading\n",
				       n, this, m_file);
				m_bRunningLoad = false;
				return m_pData[n];
			}
			m_file->seekg(m_file_index[n]);
			m_pData[n].LoadFromFile(*m_file);	// Sprite �о����
		}
		catch (...)
		{
			// File operation failed, disable lazy loading
			printf("WARNING: Failed to load sprite %d from file, disabling lazy loading\n", n);
			m_bRunningLoad = false;
			return m_pData[n];
		}

		if(++m_nLoadData >= m_Size)
		{
			m_bRunningLoad = false;
			m_file->close();
			delete m_file;
			m_file = NULL;
			delete []m_file_index;
			m_file_index = NULL;
		}
	}

	return m_pData[n];
}

template <class TypeBase, class Type1, class Type2>
bool CTypePack2<TypeBase, Type1, Type2>::LoadFromFile(LPCTSTR lpszFilename)
{
	std::ifstream file(lpszFilename, ios::binary);
	bool re = LoadFromFile(file);
	file.close();

	return re;
}

template <class TypeBase, class Type1, class Type2>
bool CTypePack2<TypeBase, Type1, Type2>::SaveToFile(LPCTSTR lpszFilename)
{
	char szIndexFilename[512];
	sprintf(szIndexFilename, "%si", lpszFilename);

	std::ofstream dataFile(lpszFilename, ios::binary);
	std::ofstream indexFile(szIndexFilename, ios::binary);

	bool re = SaveToFile(dataFile, indexFile);

	dataFile.close();
	indexFile.close();

	return re;
}


template <class TypeBase, class Type1, class Type2>
bool CTypePack2<TypeBase, Type1, Type2>::LoadFromFile(std::ifstream&file)
{
//	Release();

	file.read((char *)&m_Size, 2);
	
	Init(m_Size);
	
	int i;

	for(i = 0; i < m_Size; i++)
	{
		m_pData[i].LoadFromFile(file);
	}
	
	return true;
}

//----------------------------------------------------------------------
// Load From File Running
//----------------------------------------------------------------------
// �ǽð� �ε�
//----------------------------------------------------------------------
template <class TypeBase, class Type1, class Type2>
bool CTypePack2<TypeBase, Type1, Type2>::LoadFromFileRunning(LPCTSTR lpszFilename)
{
	//�ε��� ���� �ε�
	std::string filename = lpszFilename;
	filename += 'i';
	std::ifstream indexFile(filename.c_str(), ios::binary);

	// Check if index file opened successfully
	if (!indexFile.is_open() || !indexFile.good())
	{
		printf("ERROR: Failed to open index file: %s\n", filename.c_str());
		return false;
	}

	indexFile.read((char *)&m_Size, 2);

	if (!indexFile.good())
	{
		printf("ERROR: Failed to read size from index file: %s\n", filename.c_str());
		indexFile.close();
		return false;
	}

	Init(m_Size);

	if(m_file == NULL)
	{
		m_file = new std::ifstream;
		printf("DEBUG LoadFromFileRunning: Created new m_file=%p for %s\n", (void*)m_file, lpszFilename);
	}
	else
	{
		printf("DEBUG LoadFromFileRunning: Reusing existing m_file=%p for %s\n", (void*)m_file, lpszFilename);
	}

	m_file_index = new int[m_Size];
	for (int i = 0; i < m_Size; i++)
	{
		indexFile.read((char*)&m_file_index[i], 4);
	}
	indexFile.close();

	// file���� sprite ������ �о�´�.
	m_file->open(lpszFilename, ios::binary);

	// Check if data file opened successfully
	if (!m_file->is_open() || !m_file->good())
	{
		printf("ERROR: Failed to open data file: %s\n", lpszFilename);
		delete []m_file_index;
		m_file_index = NULL;
		m_bRunningLoad = false;
		return false;
	}

	m_file->read((char*)&m_Size, 2);

	if (!m_file->good())
	{
		printf("ERROR: Failed to read size from data file: %s\n", lpszFilename);
		m_file->close();
		delete m_file;
		m_file = NULL;
		delete []m_file_index;
		m_file_index = NULL;
		m_bRunningLoad = false;
		return false;
	}

	m_bRunningLoad = true;
	m_nLoadData = 0;

	printf("DEBUG LoadFromFileRunning: Successfully loaded %s (size=%d)\n", lpszFilename, m_Size);

	return true;
}

template <class TypeBase, class Type1, class Type2>
bool CTypePack2<TypeBase, Type1, Type2>::SaveToFile(std::ofstream&dataFile, std::ofstream&indexFile)
{
	//--------------------------------------------------
	// index file�� �����ϱ� ���� ����
	//--------------------------------------------------
//	long*	pIndex = new long [m_Size];
	std::vector<DWORD> vIndex;

	//--------------------------------------------------
	// Size ����
	//--------------------------------------------------
	dataFile.write((const char *)&m_Size, 2);
	indexFile.write((const char *)&m_Size, 2);
	WORD realSize = m_Size;
	DWORD index = 0;
	int i;  // Declare at function scope for both loops

	for(i = 0; i < m_Size; i++)
	{
		index = dataFile.tellp();
		if(m_pData[i].SaveToFile(dataFile) == false)
		{
			realSize--;
		}
		else
		{
			vIndex.push_back(index);
		}
	}
	
	if(realSize != m_Size)
	{
		char szTemp[512];
		sprintf(szTemp, "real size : %d size : %d", realSize, m_Size);
		MessageBox(NULL, szTemp, "CTypePack2", MB_OK);

		dataFile.seekp(0);
		dataFile.write((const char *)&realSize, 2);
		indexFile.seekp(0);
		indexFile.write((const char *)&realSize, 2);
	}

	//--------------------------------------------------
	// index ����
	//--------------------------------------------------
	for (i=0; i<vIndex.size(); i++)
	{
		indexFile.write((const char*)&vIndex[i], 4);
	}
	
//	delete [] pIndex;
	
//	indexFile.close();
//	dataFile.close();

	if(m_bRunningLoad)
	{
		m_bRunningLoad = false;
		m_file->close();
		delete m_file;
		m_file = NULL;
		delete []m_file_index;
		m_file_index = NULL;
	}

	return true;
}

template <class TypeBase, class Type1, class Type2>
bool CTypePack2<TypeBase, Type1, Type2>::LoadFromFilePart(int first, int last)
{
	last = min(last, 0xFFFE);
	for(int i = first; i <= last; i++)
		operator[](i);

	return true;
}

template <class TypeBase, class Type1, class Type2>
bool CTypePack2<TypeBase, Type1, Type2>::LoadFromFilePart(const CSpriteSetManager& SSM)
{
	CSpriteSetManager::DATA_LIST::const_iterator iID = SSM.GetIterator();
	for (int t=0; t<SSM.GetSize(); t++)
	{
		if(*iID != 0xFFFF)
			Get(*iID);
	}

	return true;
}

template <class TypeBase, class Type1, class Type2>
bool CTypePack2<TypeBase, Type1, Type2>::ReleasePart(int first, int last)
{
	last = min(last, 0xFFFE);
	for(int i = first; i <= last; i++)
		m_pData[i].Release();

	return true;
}

template <class TypeBase, class Type1, class Type2>
bool CTypePack2<TypeBase, Type1, Type2>::ReleasePart(COrderedList<TYPE_SPRITEID> list)
{
	COrderedList<TYPE_SPRITEID>::DATA_LIST::const_iterator iID = list.GetIterator();
	for (int t=0; t<list.GetSize(); t++)
	{
		if(*iID != 0xFFFF)
			m_pData[*iID].Release();
	}

	return true;
}

template <class TypeBase, class Type1, class Type2>
bool CTypePack2<TypeBase, Type1, Type2>::LoadFromFileData(int dataID, int fileID, LPCTSTR packFilename, LPCTSTR indexFilename)
{
	if (dataID < 0 || dataID >= m_Size)
	{
		return false;
	}

	std::ifstream dataFile(packFilename, ios::binary);
	
	if (!dataFile.is_open())
	{
		return false;
	}
	
	std::ifstream indexFile(indexFilename, ios::binary);
	
	if (!indexFile.is_open())
	{
		return false;
	}
	
	//-------------------------------------------------------------------
	// index�� ������ üũ�Ѵ�. fileID�� �ִ���..?
	//-------------------------------------------------------------------
	TYPE_SPRITEID num;
	indexFile.read((char*)&num, sizeof(WORD));
	
	if (fileID >= num)
	{
		return false;
	}
	
	//-------------------------------------------------------------------
	// load�� data�� file pointer�� �д´�.
	//-------------------------------------------------------------------
	long fp;	
	indexFile.seekg( 2 + fileID*4 );		// 2(num) + spriteID * (4 bytes)
	indexFile.read((char*)&fp, 4);
	
	dataFile.seekg( fp );	
	
	m_pData[dataID].LoadFromFile( dataFile );
	
	return true;
}

#endif
