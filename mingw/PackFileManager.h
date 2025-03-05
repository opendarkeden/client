//--------------------------------------------------------------------------
// PackFileManager.h
//--------------------------------------------------------------------------
// 화일들을 묶어서 관리한다.
// 추가, 수정이 쉬워야 한다.
//
// [ 화일형식 ]
// 
//  DataFile  : [Data0]....[DataX][DataY][DataZ]
//	IndexFile : [ID, DataFP]....[ID, DataFP][ID, DataFP][ID, DataFP]
//
// Merge는 AppendPatch를 이용하면 된다.
//--------------------------------------------------------------------------

#ifndef __PACK_FILE_MANAGER_H__
#define __PACK_FILE_MANAGER_H__

#pragma warning(disable:4786)

#include <Windows.h>
#include <map>
#include <string>
#include <fstream>

//--------------------------------------------------------------------------
// Pack FileInfo
//--------------------------------------------------------------------------
// Data화일을 먼저 저장하고 Index를 저장해야 한다.
//--------------------------------------------------------------------------
class PackFileInfo {
	public :
		PackFileInfo(DWORD id=0, const char* pFilename=NULL);
		virtual ~PackFileInfo();

		//-------------------------------------------------------------
		// Set / Get
		//-------------------------------------------------------------
		void				SetID(DWORD id)						{ m_ID = id; }
		void				SetFilePosition(long fp)			{ m_FP = fp; }
		void				SetFilename(const char* pFilename)	{ m_Filename = pFilename; }

		DWORD				GetID()	const				{ return m_ID; }
		const std::string&	GetFilename() const			{ return m_Filename; }
		long				GetFilePosition() const		{ return m_FP; }

		//-------------------------------------------------------------
		// File I/O
		//-------------------------------------------------------------
		virtual void		SaveToFile(ofstream& file);
		virtual void		LoadFromFile(ifstream& file);
		virtual bool		SaveToFileData(ofstream& file);

	protected :
		DWORD				m_ID;			// ID
		std::string			m_Filename;		// 화일이름
		long				m_FP;			// File Position
};

//--------------------------------------------------------------------------
// PackFileManager
//--------------------------------------------------------------------------
template <class FileInfoType>
class PackFileManager {
	public :
		typedef std::map<DWORD, FileInfoType*>			FILEINFO_ID_MAP;
		typedef std::map<std::string, FileInfoType*>	FILEINFO_NAME_MAP;

	public :
		PackFileManager();
		virtual ~PackFileManager();

		void				Release();

		bool				AddFile( DWORD id, const char* pFilename );
		
		//---------------------------------------------------------------
		// Info File
		//---------------------------------------------------------------
		bool				SaveToFileInfo(const char* pFilename);
		bool				LoadFromFileInfo(const char* pFilename);

		//---------------------------------------------------------------
		// Save Data
		//---------------------------------------------------------------
		virtual bool		SaveToFileData(const char* pFilename);
		
		//---------------------------------------------------------------
		// Load Data
		//---------------------------------------------------------------
		void				SetDataFilename(const char* pFilename)	{ m_DataFilename = pFilename; }
		bool				GetInputFileStream(const char* pFilename, ifstream& file) const;
		bool				GetInputFileStream(DWORD id, ifstream& file) const;

		//---------------------------------------------------------------
		// Merge
		//---------------------------------------------------------------
		//static bool			Merge(const char* pInfoFilenameOrg,
		//								const char* pDataFilenameOrg,
		//								const char* pInfoFilenameApp,
		//								const char* pDataFilenameApp);


	protected :
		bool					AddInfo(DWORD id, FileInfoType* pInfo);
		bool					HasInfo(DWORD id);
		bool					HasInfo(const char* pFilename);
		FileInfoType*			GetInfo(DWORD id) const;
		FileInfoType*			GetInfo(const char* pFilename) const;
		bool					RemoveInfo(DWORD id);
		bool					RemoveInfo(const char* pFilename);

	protected :
		std::string				m_DataFilename;
		FILEINFO_ID_MAP			m_IDInfos;
		FILEINFO_NAME_MAP		m_NameInfos;
};


//--------------------------------------------------------------------------
//
//						PackFileManager
//
//--------------------------------------------------------------------------
template <class FileInfoType>
PackFileManager<FileInfoType>::PackFileManager()
{
}

template <class FileInfoType>
PackFileManager<FileInfoType>::~PackFileManager()
{
	Release();
}

//--------------------------------------------------------------------------
// Release
//--------------------------------------------------------------------------
template <class FileInfoType>
void
PackFileManager<FileInfoType>::Release()
{
	//----------------------------------------------------------
	// ID map
	// 같은 pointer를 사용하므로...
	//----------------------------------------------------------
	FILEINFO_ID_MAP::iterator iInfo = m_IDInfos.begin();

	while (iInfo != m_IDInfos.end())
	{
		PackFileInfo* pFileInfo = iInfo->second;

		if (pFileInfo!=NULL)
		{
			delete pFileInfo;
		}

		iInfo ++;
	}

	m_IDInfos.clear();
	m_NameInfos.clear();
}

//--------------------------------------------------------------------------
// Release
//--------------------------------------------------------------------------
template <class FileInfoType>
bool
PackFileManager<FileInfoType>::AddInfo(DWORD id, FileInfoType* pInfo)
{
	FILEINFO_ID_MAP::iterator iInfo = m_IDInfos.find( id );

	if (iInfo != m_IDInfos.end())
	{
		RemoveInfo( id );		
	}
	
	m_IDInfos[id] = pInfo;

	if (pInfo->GetFilename().c_str()!=NULL)
	{
		m_NameInfos[pInfo->GetFilename()] = pInfo;
	}

	return true;
}

//--------------------------------------------------------------------------
// HasInfo
//--------------------------------------------------------------------------
template <class FileInfoType>
bool		
PackFileManager<FileInfoType>::HasInfo(DWORD id)
{
	FILEINFO_ID_MAP::iterator iInfo = m_IDInfos.find( id );

	if (iInfo != m_IDInfos.end())
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------
// HasInfo
//--------------------------------------------------------------------------
template <class FileInfoType>
bool		
PackFileManager<FileInfoType>::HasInfo(const char* pFilename)
{
	FILEINFO_ID_MAP::iterator iInfo = m_NameInfos.find( std::string(pFilename) );

	if (iInfo != m_IDInfos.end())
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------
// Get Info
//--------------------------------------------------------------------------
template <class FileInfoType>
FileInfoType*	
PackFileManager<FileInfoType>::GetInfo(DWORD id) const
{
	FILEINFO_ID_MAP::const_iterator iInfo = m_IDInfos.find( id );

	if (iInfo != m_IDInfos.end())
	{
		return iInfo->second;
	}

	return NULL;
}

//--------------------------------------------------------------------------
// Get Info
//--------------------------------------------------------------------------
template <class FileInfoType>
FileInfoType*	
PackFileManager<FileInfoType>::GetInfo(const char* pFilename) const
{
	FILEINFO_NAME_MAP::const_iterator iInfo = m_IDInfos.find( std::string(pFilename) );

	if (iInfo != m_IDInfos.end())
	{
		return iInfo->second;
	}

	return NULL;
}

//--------------------------------------------------------------------------
// Remove Info
//--------------------------------------------------------------------------
template <class FileInfoType>
bool					
PackFileManager<FileInfoType>::RemoveInfo(DWORD id)
{
	FILEINFO_ID_MAP::iterator iInfo = m_IDInfos.find( id );

	if (iInfo != m_IDInfos.end())
	{
		FileInfoType* pInfo = iInfo->second;

		FILEINFO_NAME_MAP::iterator iNameInfo = m_NameInfos.find( pInfo->GetFilename() );

		if (iNameInfo!=m_NameInfos.end())
		{
			m_NameInfos.erase( iNameInfo );
		}

		m_IDInfos.erase( iInfo );

		if (pInfo!=NULL)
		{
			delete pInfo;
		}		

		return true;
	}

	return false;
}

//--------------------------------------------------------------------------
// Remove Info
//--------------------------------------------------------------------------
template <class FileInfoType>
bool					
PackFileManager<FileInfoType>::RemoveInfo(const char* pFilename)
{
	if (pFilename==NULL)
	{
		return false;
	}

	FILEINFO_NAME_MAP::iterator iNameInfo = m_NameInfos.find( std::string(pFilename) );

	if (iInfo != m_NameInfos.end())
	{
		FileInfoType* pInfo = iInfo->second;

		FILEINFO_ID_MAP::iterator iInfo = m_IDInfos.find( pInfo->GetID() );

		if (iInfo!=m_IDInfos.end())
		{
			m_IDInfos.erase( iInfo );
		}

		m_NameInfos.erase( iNameInfo );

		if (pInfo!=NULL)
		{
			delete pInfo;
		}		

		return true;
	}

	return false;
}

//--------------------------------------------------------------------------
// Add File
//--------------------------------------------------------------------------
template <class FileInfoType>
bool					
PackFileManager<FileInfoType>::AddFile( DWORD id, const char* pFilename )
{
	FileInfoType* pInfo = new FileInfoType;

	pInfo->SetID( id );
	pInfo->SetFilename( pFilename );

	AddInfo( id, pInfo );

	return true;
}

//--------------------------------------------------------------------------
// Save To File Info
//--------------------------------------------------------------------------
template <class FileInfoType>
bool					
PackFileManager<FileInfoType>::SaveToFileInfo(const char* pFilename)
{
	ofstream file(pFilename, ios::binary | ios::trunc);

	// 개수
	WORD num = m_IDInfos.size();
	file.write((const char*)&num, 2);

	// Header 저장
	FILEINFO_ID_MAP::iterator iInfo = m_IDInfos.begin();

	while (iInfo != m_IDInfos.end())
	{
		FileInfoType* pInfo = iInfo->second;

		pInfo->SaveToFile( file );

		iInfo ++;
	}

	file.close();
	
	return true;
}

//--------------------------------------------------------------------------
// Load From File Info
//--------------------------------------------------------------------------
template <class FileInfoType>
bool					
PackFileManager<FileInfoType>::LoadFromFileInfo(const char* pFilename)
{
	Release();

	ifstream file(pFilename, ios::binary | ios::nocreate);

	// 개수
	if (file.is_open())
	{
		WORD num;
		file.read((char*)&num, 2);

		for (int i=0; i<num; i++)
		{
			// Header 읽기
			FileInfoType* pInfo = new FileInfoType;

			pInfo->LoadFromFile( file );

			AddInfo( pInfo->GetID(), pInfo );
		}

		file.close();
	
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------
// Save To File Data
//--------------------------------------------------------------------------
template <class FileInfoType>
bool					
PackFileManager<FileInfoType>::SaveToFileData(const char* pFilename)
{
	ofstream file(pFilename, ios::binary | ios::trunc);

	// 개수
	WORD num = m_IDInfos.size();
	file.write((const char*)&num, 2);	

	// data
	FILEINFO_ID_MAP::iterator iInfo = m_IDInfos.begin();

	while  (iInfo != m_IDInfos.end())
	{
		FileInfoType* pInfo = iInfo->second;

		if (pInfo!=NULL)
		{
			long fp = file.tellp();

			pInfo->SetFilePosition( fp );

			pInfo->SaveToFileData( file );
		}

		iInfo ++;
	}

	return true;
}

//--------------------------------------------------------------------------
// Get InputFileStream
//--------------------------------------------------------------------------
template <class FileInfoType>
bool
PackFileManager<FileInfoType>::GetInputFileStream(const char* pFilename, ifstream& file) const
{
	if (m_DataFilename.c_str()==NULL 
		|| pFilename==NULL)
	{
		return false;
	}

	FILEINFO_NAME_MAP::iterator iInfo = m_NameInfos.find( std::string(pFilename) );

	if (iInfo != m_NameInfos.end())
	{
		FileInfoType* pInfo = iInfo->second;

		file.open(m_DataFilename.c_str(), ios::binary);

		file.seekg( pInfo->GetFilePosition() );

		return true;				
	}

	return false;
}

//--------------------------------------------------------------------------
// Get InputFileStream
//--------------------------------------------------------------------------
template <class FileInfoType>
bool
PackFileManager<FileInfoType>::GetInputFileStream(DWORD id, ifstream& file) const
{
	if (m_DataFilename.c_str()==NULL)
	{
		return;
	}

	FILEINFO_ID_MAP::iterator iInfo = m_IDInfos.find( id );

	if (iInfo != m_IDInfos.end())
	{
		FileInfoType* pInfo = iInfo->second;

		file.open(m_DataFilename.c_str(), ios::binary);

		file.seekg( pInfo->GetFilePosition() );

		return true;
	}

	return false;
}

//--------------------------------------------------------------------------
// Merge
//--------------------------------------------------------------------------
/*
template <class FileInfoType>
bool
PackFileManager<FileInfoType>::Merge(const char* pInfoFilenameOrg,
						const char* pDataFilenameOrg,
						const char* pInfoFilenameApp,
						const char* pDataFilenameApp)
{
	//class fstream fileInfoOrg(pInfoFilenameOrg, ios::in | ios::out | ios::binary | ios::ate);	
	//class fstream fileDataOrg(pDataFilenameOrg, ios::in | ios::out | ios::binary | ios::ate);
	//ifstream fileInfoApp(pInfoFilenameApp, ios::binary);
	//ifstream fileDataApp(pDataFilenameApp, ios::binary);

	// pInfoFilenameOrg의 개수 변경
	// pInfoFilenameOrg의 끝에 pInfoFilenameApp를 붙인다.

	// pDataFilenameOrg의 개수 변경
	// pDataFilenameOrg의 끝에 pDataFilenameApp를 붙인다.

	return true;
}
*/
#endif