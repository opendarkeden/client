//----------------------------------------------------------------------
// CMessageArray.h
//----------------------------------------------------------------------
// String Array이다.
// 
// 초기화 할 때 : Init(String수, 한String의 최대길이, log File);
//
// Init(...)할때 메모리를 다 잡아버린다.
// filename을 써주면 자동으로 string이 추가될때 log한다.
//----------------------------------------------------------------------

#ifndef	__CMESSAGEARRAY_H__
#define	__CMESSAGEARRAY_H__

class CMessageArray {
	public :
		CMessageArray();
		~CMessageArray();

		//--------------------------------------------------
		// Init / Release
		//--------------------------------------------------
		void		Init(int max, int length, const char* filename=NULL);
		void		Release();

		//--------------------------------------------------
		// Add / Get
		//--------------------------------------------------
		void		Add(const char *str);
		//void		Add(std::string str)			{ Add(str.c_str()); }
		void		AddToFile(const char *str);
		//void		AddToFile(std::string str)	{ AddToFile(str.c_str()); }
		void		AddFormat(const char* format, ...);
		void		AddFormatVL(const char* format, va_list& vl);
		const char*	operator [] (int i);

		// 외부에서 편집..
		char*&		GetCurrent()		{ return m_ppMessage[m_Current]; }

		// 다음 것
		void		Next();

		// size
		int			GetSize() const			{ return m_Max; }

		// clear
		void		Clear();

		// filename
		const char*	GetFilename() const		{ return m_Filename; }


	protected :		
		int			m_Length;		// Message 하나의 길이
		int			m_Max;			// Message 개수

		char**		m_ppMessage;	// 입력된 Message
		int			m_Current;		// 입력할려는 Message 

		// file Log
		bool			m_bLog;
		int				m_LogFile;
		char*			m_Filename;


};

#endif


