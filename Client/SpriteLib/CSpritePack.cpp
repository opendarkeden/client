#include "client_PCH.h"
//#include "CSpritePack.h"
//#include "CSprite555.h"
//#include "CSprite565.h"
//#include <fstream.h>
//
//void CSpritePack::Init(WORD size, bool b565)
//{
//	if(size == 0)
//		return;
//	
//	Release();
//	
//	m_b565 = b565;
//	m_Size = size;
//	
//	if(m_b565)
//		m_pData = new CSprite565[size];
//	else
//		m_pData = new CSprite555[size];
//}
//
//bool CSpritePack::LoadFromFile(LPCTSTR lpszFilename, bool b565)
//{
//	ifstream file(lpszFilename, ios::binary);
//	bool re = LoadFromFile(file, b565);
//	file.close();
//	
//	return re;
//}
//
//bool CSpritePack::LoadFromFile(ifstream &file, bool b565)
//{
//	Release();
//	
//	file.read((char *)&m_Size, 2);
//	
//	Init(m_Size, b565);
//	
//	for(int i = 0; i < m_Size; i++)
//	{
//		m_pData[i].LoadFromFile(file);
//	}
//	
//	return true;
//}
//
////----------------------------------------------------------------------
//// Load From File Running
////----------------------------------------------------------------------
//// 실시간 로딩
////----------------------------------------------------------------------
//bool CSpritePack::LoadFromFileRunning(LPCTSTR lpszFilename, bool b565)
//{
//	//인덱스 파일 로딩
//	std::string filename = lpszFilename;
//	filename += 'i';
//	ifstream indexFile(filename.c_str(), ios::binary);
//	indexFile.read((char *)&m_Size, 2); 
//	Init(m_Size, b565);
//
//	if(m_file == NULL)
//	{
//		m_file = new ifstream;
//	}
//	
//	m_file_index = new int[m_Size];
//	for (int i = 0; i < m_Size; i++)
//	{
//		indexFile.read((char*)&m_file_index[i], 4);
//	}
//	indexFile.close();
//	
//	// file에서 sprite 개수를 읽어온다.	
//	m_file->open(lpszFilename, ios::binary);
//	
//	m_file->read((char*)&m_Size, 2);
//	
//	m_bRunningLoad = true;
//	m_nLoadData = 0;
//	
//	return true;
//}
