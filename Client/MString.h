//--------------------------------------------------------------------------
// MString.h
//--------------------------------------------------------------------------

#ifndef	__MSTRING_H__
#define	__MSTRING_H__

#pragma warning(disable:4786)

#define	MAX_BUFFER_LENGTH		1024

#include <string.h>
#include <fstream>
class MString {
	public :
		MString();
		MString(const MString& str);
		MString(const char* str);
		virtual ~MString();

		//----------------------------------------------------
		// Init / Release
		//----------------------------------------------------
		void	Init(int len);
		void	Release();

		//----------------------------------------------------
		// assign
		//----------------------------------------------------
		void	operator = (const MString& str);
		void	operator = (const char* str);
		void	Format(const char* format, ...);

		//----------------------------------------------------
		// get
		//----------------------------------------------------
		size_t		GetLength()	const		{ return m_Length; }
		char*		GetString() const		{ return m_pString; }
		operator const char*() const		{ return m_pString; }
		operator	char*() const		{ return m_pString; }		

		//----------------------------------------------------
		// other operator
		//----------------------------------------------------
		bool		operator == (const char* str)		{ return strcmp(m_pString, str)==0; }
		bool		operator == (const MString& str)	{ return strcmp(m_pString, str.m_pString)==0; }
		bool		operator >	(const char* str)		{ return strcmp(m_pString, str)>0; }
		bool		operator >	(const MString& str)	{ return strcmp(m_pString, str.m_pString)>0; }
		bool		operator <	(const char* str)		{ return strcmp(m_pString, str)<0; }
		bool		operator <	(const MString& str)	{ return strcmp(m_pString, str.m_pString)<0; }
		bool		operator != (const char* str)		{ return strcmp(m_pString, str)!=0; }
		bool		operator != (const MString& str)	{ return strcmp(m_pString, str.m_pString)!=0; }

		//----------------------------------------------------
		// File I/O
		//----------------------------------------------------
		virtual void	SaveToFile(std::ofstream& file);
		virtual void	LoadFromFile(std::ifstream& file);

	protected :
		size_t	m_Length;
		char*	m_pString;

		static	char		s_pBuffer[MAX_BUFFER_LENGTH];    
};


#endif

