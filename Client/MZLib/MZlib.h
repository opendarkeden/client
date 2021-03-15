//--------------------------------------------------------------------------
// MZLib.h
//--------------------------------------------------------------------------

#ifndef __MZLIB_H__
#define __MZLIB_H__

#pragma warning(disable:4786)

#include <Windows.h>
#include <list>
#include "MString.h"

using std::ifstream;
using std::ofstream;

#define MZLIB_HEADER_SIZE	10

//--------------------------------------------------------------------------
// MFileInfo
//--------------------------------------------------------------------------
class MFileInfo {
	public :
		MFileInfo();
		~MFileInfo();

		bool			Set( const char* filename );
		void			SetBufferInfo(long startBuffer, long bufferSize)	{ m_StartBuffer = startBuffer; m_CompressSize=bufferSize; }
		
		const char*		GetFilename() const		{ return m_Filename.GetString(); }
		long			GetFilesize() const		{ return m_Filesize; }
		long			GetStartBuffer() const	{ return m_StartBuffer; }
		long			GetCompressSize() const	{ return m_CompressSize; }

		void			SaveToFile(ofstream& file);
		void			LoadFromFile(ifstream& file);

	protected :
		MString			m_Filename;
		long			m_Filesize;
		long			m_StartBuffer;
		long			m_CompressSize;
};

//--------------------------------------------------------------------------
// MZLib
//--------------------------------------------------------------------------
class MZLib {
	public :
		typedef	std::list<MFileInfo*>	FILEINFO_LIST;

	public :
		MZLib();
		~MZLib();

		bool				AddFile(const char* filename);
		bool				Compress(const char* filename);

		bool				Uncompress(const char* filename);
		
	protected :
		static void			InitPackBuffer(long bytes);
		static void			InitFileBuffer(long bytes);
		static void			ReleasePackBuffer();
		static void			ReleaseFileBuffer();
		static bool			ReadBufferFromFile(const char* filename);
		static void			WriteBufferToFile( ofstream& packFile, long bytes);
		static void			ReadBufferFromFile( ifstream& packFile, long bytes);

		void				ReleaseFileInfo();
		void				AddFileInfo(MFileInfo* pInfo);
		void				WriteFileInfoToFile( ofstream& packFile);
		bool				ReadFileInfoFromFile( ifstream& packFile);

	protected :
		FILEINFO_LIST		m_listFileInfo;
		static char*		m_pPackBuffer;
		static char*		m_pFileBuffer;
		static char			m_ZLibHeader[MZLIB_HEADER_SIZE+1];	
};


#endif