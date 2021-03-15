#ifndef __CTYPEPACK_H__
#define __CTYPEPACK_H__

#include <windows.h>
#include <fstream>
#include "CSpriteSetManager.h"
#include "COrderedList.h"
#include "CDirectDraw.h"
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
	virtual bool LoadFromFile(ifstream &file);
	virtual bool SaveToFile(ofstream &dataFile, ofstream &indexFile);
	
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
	WORD			m_nLoadData;	// Loading 된 CSprite의 개수
	ifstream	*m_file;
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
		// file에 있는 Sprite들을 Load	
		m_pData[n].LoadFromFile(*m_file);	// Sprite 읽어오기
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
	ifstream file(lpszFilename, std::ios::binary);
	bool re = LoadFromFile(file);
	file.close();

	return re;
}

template <class Type>
bool CTypePack<Type>::SaveToFile(LPCTSTR lpszFilename)
{
	char szIndexFilename[512];
	sprintf(szIndexFilename, "%si", lpszFilename);

	ofstream dataFile(lpszFilename, std::ios::binary);
	ofstream indexFile(szIndexFilename, std::ios::binary);

	bool re = SaveToFile(dataFile, indexFile);

	dataFile.close();
	indexFile.close();

	return re;
}


template <class Type>
bool CTypePack<Type>::LoadFromFile(ifstream &file)
{
//	Release();

	file.read((char *)&m_Size, 2);
	
	Init(m_Size);
	
	register int i;

	for(i = 0; i < m_Size; i++)
	{
		m_pData[i].LoadFromFile(file);
	}
	
	return true;
}

//----------------------------------------------------------------------
// Load From File Running
//----------------------------------------------------------------------
// 실시간 로딩
//----------------------------------------------------------------------
template <class Type>
bool CTypePack<Type>::LoadFromFileRunning(LPCTSTR lpszFilename)
{
	//인덱스 파일 로딩
	std::string filename = lpszFilename;
	filename += 'i';
	ifstream indexFile(filename.c_str(), std::ios::binary);
	indexFile.read((char *)&m_Size, 2); 
	Init(m_Size);

	if(m_file == NULL)
	{
		m_file = new ifstream;
	}
	
	m_file_index = new int[m_Size];
	for (int i = 0; i < m_Size; i++)
	{
		indexFile.read((char*)&m_file_index[i], 4);
	}
	indexFile.close();
	
	// file에서 sprite 개수를 읽어온다.	
	m_file->open(lpszFilename, std::ios::binary);
	
	m_file->read((char*)&m_Size, 2);
	
	m_bRunningLoad = true;
	m_nLoadData = 0;
	
	return true;
}

template <class Type>
bool CTypePack<Type>::SaveToFile(ofstream &dataFile, ofstream &indexFile)
{
	//--------------------------------------------------
	// index file을 생성하기 위한 정보
	//--------------------------------------------------
//	long*	pIndex = new long [m_Size];
	std::vector<DWORD> vIndex;

	//--------------------------------------------------
	// Size 저장
	//--------------------------------------------------
	dataFile.write((const char *)&m_Size, 2); 
	indexFile.write((const char *)&m_Size, 2); 
	WORD realSize = m_Size;
	DWORD index = 0;

	for(int i = 0; i < m_Size; i++)
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
	// index 저장
	//--------------------------------------------------
	for (int i=0; i<vIndex.size(); i++)
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

	ifstream dataFile(packFilename, std::ios::binary | std::ios::in);
	
	if (!dataFile.is_open())
	{
		return false;
	}
	
	ifstream indexFile(indexFilename, std::ios::binary | std::ios::in);
	
	if (!indexFile.is_open())
	{
		return false;
	}
	
	//-------------------------------------------------------------------
	// index의 개수를 체크한다. fileID가 있는지..?
	//-------------------------------------------------------------------
	TYPE_SPRITEID num;
	indexFile.read((char*)&num, sizeof(WORD));
	
	if (fileID >= num)
	{
		return false;
	}
	
	//-------------------------------------------------------------------
	// load할 data의 file pointer를 읽는다.
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
	virtual bool LoadFromFile(ifstream &file);
	virtual bool SaveToFile(ofstream &dataFile, ofstream &indexFile);
	
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
	WORD			m_nLoadData;	// Loading 된 CSprite의 개수
	ifstream	*m_file;
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
		m_file->seekg(m_file_index[n]);
		// file에 있는 Sprite들을 Load	
		m_pData[n].LoadFromFile(*m_file);	// Sprite 읽어오기
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
	ifstream file(lpszFilename, std::ios::binary);
	bool re = LoadFromFile(file);
	file.close();

	return re;
}

template <class TypeBase, class Type1, class Type2>
bool CTypePack2<TypeBase, Type1, Type2>::SaveToFile(LPCTSTR lpszFilename)
{
	char szIndexFilename[512];
	sprintf(szIndexFilename, "%si", lpszFilename);

	ofstream dataFile(lpszFilename, std::ios::binary);
	ofstream indexFile(szIndexFilename, std::ios::binary);

	bool re = SaveToFile(dataFile, indexFile);

	dataFile.close();
	indexFile.close();

	return re;
}


template <class TypeBase, class Type1, class Type2>
bool CTypePack2<TypeBase, Type1, Type2>::LoadFromFile(ifstream &file)
{
//	Release();

	file.read((char *)&m_Size, 2);
	
	Init(m_Size);
	
	register int i;

	for(i = 0; i < m_Size; i++)
	{
		m_pData[i].LoadFromFile(file);
	}
	
	return true;
}

//----------------------------------------------------------------------
// Load From File Running
//----------------------------------------------------------------------
// 실시간 로딩
//----------------------------------------------------------------------
template <class TypeBase, class Type1, class Type2>
bool CTypePack2<TypeBase, Type1, Type2>::LoadFromFileRunning(LPCTSTR lpszFilename)
{
	//인덱스 파일 로딩
	std::string filename = lpszFilename;
	filename += 'i';
	ifstream indexFile(filename.c_str(), std::ios::binary);
	indexFile.read((char *)&m_Size, 2); 
	Init(m_Size);

	if(m_file == NULL)
	{
		m_file = new ifstream;
	}
	
	m_file_index = new int[m_Size];
	for (int i = 0; i < m_Size; i++)
	{
		indexFile.read((char*)&m_file_index[i], 4);
	}
	indexFile.close();
	
	// file에서 sprite 개수를 읽어온다.	
	m_file->open(lpszFilename, std::ios::binary);
	
	m_file->read((char*)&m_Size, 2);
	
	m_bRunningLoad = true;
	m_nLoadData = 0;
	
	return true;
}

template <class TypeBase, class Type1, class Type2>
bool CTypePack2<TypeBase, Type1, Type2>::SaveToFile(ofstream &dataFile, ofstream &indexFile)
{
	//--------------------------------------------------
	// index file을 생성하기 위한 정보
	//--------------------------------------------------
//	long*	pIndex = new long [m_Size];
	std::vector<DWORD> vIndex;

	//--------------------------------------------------
	// Size 저장
	//--------------------------------------------------
	dataFile.write((const char *)&m_Size, 2); 
	indexFile.write((const char *)&m_Size, 2); 
	WORD realSize = m_Size;
	DWORD index = 0;

	for(int i = 0; i < m_Size; i++)
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
	// index 저장
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

	ifstream dataFile(packFilename, std::ios::binary | ios::nocreate);
	
	if (!dataFile.is_open())
	{
		return false;
	}
	
	ifstream indexFile(indexFilename, std::ios::binary | ios::nocreate);
	
	if (!indexFile.is_open())
	{
		return false;
	}
	
	//-------------------------------------------------------------------
	// index의 개수를 체크한다. fileID가 있는지..?
	//-------------------------------------------------------------------
	TYPE_SPRITEID num;
	indexFile.read((char*)&num, sizeof(WORD));
	
	if (fileID >= num)
	{
		return false;
	}
	
	//-------------------------------------------------------------------
	// load할 data의 file pointer를 읽는다.
	//-------------------------------------------------------------------
	long fp;	
	indexFile.seekg( 2 + fileID*4 );		// 2(num) + spriteID * (4 bytes)
	indexFile.read((char*)&fp, 4);
	
	dataFile.seekg( fp );	
	
	m_pData[dataID].LoadFromFile( dataFile );
	
	return true;
}

#endif