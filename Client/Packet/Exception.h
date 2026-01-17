//////////////////////////////////////////////////////////////////////
// 
// Filename    : Exception.h 
// Written By  : reiot@ewestsoft.com
// Description :
// 
//////////////////////////////////////////////////////////////////////

#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

// include files
#include "Types.h"
#include "StringStream.h"

#if __WINDOWS__
#pragma warning ( disable : 4786 )
#endif

#include <list>


//////////////////////////////////////////////////////////////////////
//
// class Throwable
//
// Exception °ú Error ÀÇ º£ÀÌ½º Å¬·¡½ºÀÌ´Ù. °ü·Ã ¸Þ½îµå ¹× µ¥ÀÌÅ¸¸¦
// ±¸ÇöÇØ³õ°í ÀÖ´Ù.
//
//////////////////////////////////////////////////////////////////////

class Throwable {

public :

	// constructor
	Throwable () throw () {}
	
	// constructor
	Throwable ( std::string message ) throw () : m_Message(message) {}

	// destructor
	virtual ~Throwable () throw () {}

	// return class's name
	virtual std::string getName () const throw () { return "Throwable"; }

	// add function name to throwable object's function stack
	void addStack ( const std::string & file, const int line ) throw ()
	{
		StringStream s;
		s << file << ":" << line;
		m_Stacks.push_front( s.toString());
	}

	// return debug std::string - throwable object's function stack trace
	std::string getStackTrace () const throw ()
	{
		StringStream buf;
		int i = 1;

		for ( std::list<std::string>::const_iterator itr = m_Stacks.begin() ;
			  itr != m_Stacks.end() ;
			  itr ++ , i ++ ) {
			for ( int j = 0 ; j < i ; j ++ )
				buf << " ";
			buf << *itr << '\n' ;
		}
		
		return std::string( buf.toString() );
	}

	// get throwable object's message
	std::string getMessage () const throw () { return m_Message; }
	
	// set throwable object's message
	void setMessage ( const std::string & message ) throw () { m_Message = message; }
	
	// return debug string - throwable object's detailed information
	virtual std::string toString () const throw ()
	{
		StringStream buf;
		buf << getName() << " : " << m_Message << '\n'
			<< getStackTrace () ;
		
		return std::string( buf.toString() );
	}

private :
	
	// message string
	std::string m_Message;
	
	// function stack 
	std::list<std::string> m_Stacks;
};

//--------------------------------------------------------------------------------
// macro definition
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//
// ThrowableÀÌ ÇÊ¿äÇÏ±â ¶§¹®¿¡ ¾Æ·¡¿¡ Á¤ÀÇÇß´Ù.
// Exception/Error¸¦ ´øÁö´Â ¸ðµç ¸Þ½îµåÀÇ À§/¾Æ·¡¿¡ ¸í½ÃµÇ¾î¾ß ÇÑ´Ù.
// __END_CATCH´Â ThrowableÀÇ ¸Þ¼Òµå ½ºÅÃ¿¡ µî·ÏÇÑ ÈÄ »óÀ§·Î ´øÁö´Â
// ¿ªÇÒÀ» ÇÑ´Ù.
//
//--------------------------------------------------------------------------------

#ifdef NDEBUG
	#define __BEGIN_TRY ((void)0);
	#define __END_CATCH ((void)0);
#else
	#define __BEGIN_TRY \
				try {
	#define __END_CATCH \
				} catch ( Throwable & t ) { \
					t.addStack(__FILE__, __LINE__); \
					throw; \
				}
#endif

//--------------------------------------------------------------------------------
//
// critical section
//
//--------------------------------------------------------------------------------
#define __ENTER_CRITICAL_SECTION(mutex) \
			mutex.lock(); \
			try {

#define __LEAVE_CRITICAL_SECTION(mutex) \
			} catch ( Throwable & t ) { \
				mutex.unlock(); \
				throw; \
			} \
			mutex.unlock();

//--------------------------------------------------------------------------------
//
// cout debugging
//
//--------------------------------------------------------------------------------
#if defined(NDEBUG) || defined(__WIN32__)
	#define __BEGIN_DEBUG ((void)0);
	#define __END_DEBUG ((void)0);
#elif defined(__LINUX__) || defined(__APPLE__) || defined(__macos__) || defined(__WIN_CONSOLE__)
	#define __BEGIN_DEBUG \
				try {
	#define __END_DEBUG  \
				} catch ( Throwable & t ) { \
					std::cout << t.toString() << std::endl; \
					throw; \
				} catch ( std::exception & e ) { \
					std::cout << e.what() << std::endl; \
					throw; \
				}
#elif defined(__MFC__)
	#define __BEGIN_DEBUG \
				try {
	#define __END_DEBUG \
				} catch ( Throwable & t ) { \
					AfxMessageBox(t.toString()); \
					throw; \
				}
#endif


//////////////////////////////////////////////////////////////////////
//
// Exception
//
//////////////////////////////////////////////////////////////////////
class Exception : public Throwable {
public :
	Exception () throw () : Throwable() {}
	Exception ( std::string msg ) throw () : Throwable(msg) {}
	std::string getName () const throw () { return "Exception"; }
};

	//////////////////////////////////////////////////////////////////////
	//
	// I/O Exception
	//
	// ÆÄÀÏ, ¼ÒÄÏ, IPC ÀÔÃâ·Â½Ã ¹ß»ýÇÒ ¼ö ÀÖ´Â ¿¹¿Ü
	//
	//////////////////////////////////////////////////////////////////////
	// ÆÄÀÏ, ¼ÒÄÏ, IPC ÀÔÃâ·Â½Ã ¹ß»ýÇÒ ¼ö ÀÖ´Â ¿¹¿Ü
	class IOException : public Exception {
	public :
		IOException () throw () : Exception () {}
		IOException ( std::string msg ) throw () : Exception (msg) {}
		std::string getName () const throw () { return "IOException"; }
	};

		//////////////////////////////////////////////////////////////////////
		//
		// Non Blocking I/O Exception
		//
		// I/O ½Ã nonblocking ÀÌ ¹ß»ýÇÒ °æ¿ì
		//
		//////////////////////////////////////////////////////////////////////
		class NonBlockingIOException : public IOException {
		public :
			NonBlockingIOException () throw () : IOException () {}
			NonBlockingIOException ( std::string msg ) throw () : IOException (msg) {}
			std::string getName () const throw () { return "NonBlockingIOException"; }
		};
	
		//////////////////////////////////////////////////////////////////////
		//
		// Interrupted I/O Exception
		//
		// I/O ½Ã ÀÎÅÍ·´Æ®°¡ °É¸° °æ¿ì
		//
		//////////////////////////////////////////////////////////////////////
		class InterruptedIOException : public IOException {
		public :
			InterruptedIOException () throw () : IOException () {}
			InterruptedIOException ( std::string msg ) throw () : IOException (msg) {}
			std::string getName () const throw () { return "InterruptedIOException"; }
		};
	
		//////////////////////////////////////////////////////////////////////
		//
		// EOF Exception
		//
		// I/O ½Ã EOF ¸¦ ¸¸³­ °æ¿ì
		//
		//////////////////////////////////////////////////////////////////////
		class EOFException : public IOException {
		public :
			EOFException () throw () : IOException () {}
			EOFException ( std::string msg ) throw () : IOException (msg) {}
			std::string getName () const throw () { return "EOFException"; }
		};
	
		//////////////////////////////////////////////////////////////////////
		//
		// File Not Opened Exception 
		//
		//////////////////////////////////////////////////////////////////////
		class FileNotOpenedException : public IOException {
		public :
			FileNotOpenedException () throw () : IOException() {}
			FileNotOpenedException ( std::string msg ) throw () : IOException(msg) {}
			std::string getName () const throw () { return "FileNotOpenedException"; }
		};

		//////////////////////////////////////////////////////////////////////
		//
		// File Already Exist Exception
		//
		//////////////////////////////////////////////////////////////////////
		class FileAlreadyExistException : public IOException {
		public :
			FileAlreadyExistException () throw () : IOException() {}
			FileAlreadyExistException ( std::string msg ) throw () : IOException(msg) {}
			std::string getName () const throw () { return "FileAlreadyExistException"; }
		};

		//////////////////////////////////////////////////////////////////////
		//
		// File Not Exist Exception
		//
		//////////////////////////////////////////////////////////////////////
		class FileNotExistException : public IOException {
		public :
			FileNotExistException () throw () : IOException() {}
			FileNotExistException ( std::string msg ) throw () : IOException(msg) {}
			std::string getName () const throw () { return "FileNotExistException"; }
		};

		//////////////////////////////////////////////////////////////////////
		//
		// Time out Exception
		//
		// ÁöÁ¤ ½Ã°£ÀÌ Áö³µÀ» °æ¿ì
		//
		//////////////////////////////////////////////////////////////////////
		class TimeoutException : public IOException {
		public :
			TimeoutException () throw () : IOException () {}
			TimeoutException ( std::string msg ) throw () : IOException (msg) {}
			std::string getName () const throw () { return "TimeoutException"; }
		};

		//////////////////////////////////////////////////////////////////////
		//
		// Socket Exception
		//
		// Æ¯È÷ ¼ÒÄÏ¿¡¼­ ¹ß»ýÇÏ´Â ¿¹¿Üµé
		//
		//////////////////////////////////////////////////////////////////////
		class SocketException : public IOException {
		public :
			SocketException () throw () : IOException () {}
			SocketException ( std::string msg ) throw () : IOException (msg) {}
			std::string getName () const throw () { return "SocketException"; }
		};
	
			//////////////////////////////////////////////////////////////////////
			//
			// Bind Exception
			//
			// bind()½Ã ¹ß»ýÇÏ´Â ¿¹¿Ü
			//
			//////////////////////////////////////////////////////////////////////
			class BindException : public SocketException {
			public :
				BindException () throw () : SocketException () {}
				BindException ( std::string msg ) throw () : SocketException (msg) {}
				std::string getName () const throw () { return "BindException"; }
			};
	
			//////////////////////////////////////////////////////////////////////
			//
			// Connect Exception
			//
			// ¼ÒÄÏ ¿¬°áÀÌ ²÷±æ °æ¿ì ( °¡Àå ¸¹ÀÌ ¹ß»ýÇÑ´Ù°í º¸¸é µÈ´Ù. )
			//
			//////////////////////////////////////////////////////////////////////
			class ConnectException : public SocketException {
			public :
				ConnectException () throw () : SocketException () {}
				ConnectException ( std::string msg ) throw () : SocketException (msg) {}
				std::string getName () const throw () { return "ConnectException"; }
			};
			
		//////////////////////////////////////////////////////////////////////
		//
		// Protocol Exception
		//
		// ÆÐÅ¶ ÆÄ½ÌÇÒ¶§ ¹ß»ýÇÏ´Â ¿¹¿Üµé
		//
		//////////////////////////////////////////////////////////////////////
		class ProtocolException : public IOException {
		public :
			ProtocolException () throw () : IOException () {}
			ProtocolException ( std::string msg ) throw () : IOException (msg) {}
			std::string getName () const throw () { return "ProtocolException"; }
		};
	
			//////////////////////////////////////////////////////////////////////
			//
			// Idle Exception
			//
			// ÀÏÁ¤ ½Ã°£µ¿¾È peer ·ÎºÎÅÍ ÀÔ·ÂÀÌ ¾ø´Â °æ¿ì
			//
			//////////////////////////////////////////////////////////////////////
			class IdleException : public ProtocolException {
			public :
				IdleException () throw () : ProtocolException () {}
				IdleException ( std::string msg ) throw () : ProtocolException (msg) {}
				std::string getName () const throw () { return "IdleException"; }
			};
	

			//////////////////////////////////////////////////////////////////////
			//
			// Invalid Protocol Exception
			//
			// Àß¸øµÈ ÇÁ·ÎÅäÄÝ
			//
			//////////////////////////////////////////////////////////////////////
			class InvalidProtocolException : public ProtocolException {
			public :
				InvalidProtocolException () throw () : ProtocolException () {}
				InvalidProtocolException ( std::string msg ) throw () : ProtocolException (msg) {}
				std::string getName () const throw () { return "InvalidProtocolException"; }
			};
	
			//////////////////////////////////////////////////////////////////////
			//
			// Insufficient Data Exception
			//
			// ¾ÆÁ÷ ÆÐÅ¶ µ¥ÀÌÅ¸°¡ ¿ÏÀüÇÏ°Ô µµÂøÇÏÁö ¾Ê¾ÒÀ» °æ¿ì
			//
			//////////////////////////////////////////////////////////////////////
			class InsufficientDataException : public ProtocolException {
			public :
				InsufficientDataException ( uint size = 0 ) throw () : ProtocolException () , m_Size(size) {}
				InsufficientDataException ( std::string msg , uint size = 0 ) throw () : ProtocolException (msg) , m_Size(size) {}
				std::string getName () const throw () { return "InsufficientDataException"; }
				uint getSize () const throw () { return m_Size; }
				std::string toString () const throw ()
				{
					StringStream buf;
					buf << getName() << " : " << getMessage();
					if ( m_Size > 0 ) {
						buf << " ( lack of " << m_Size << " bytes )\n";
					}
					buf << getStackTrace ();
			
					return buf.toString();
				}
	
			private :
				uint m_Size;
			};

			//////////////////////////////////////////////////////////////////////
			// 
			// ÇÁ·ÎÅäÄÝ ¿¹¿Ü, ½Ã½ºÅÛ ¿¹¿Ü µîÀ¸·ÎÀÎÇØ¼­ Á¢¼ÓÀ» Â©¶ó¾ß ÇÒ °æ¿ì
			// ÀÌ ¿¹¿Ü¸¦ »ç¿ëÇÑ´Ù.
			// 
			//////////////////////////////////////////////////////////////////////
			class DisconnectException : public ProtocolException {
			public :
				DisconnectException () throw () : ProtocolException () {}
				DisconnectException ( std::string msg ) throw () : ProtocolException (msg) {}
				std::string getName () const throw () { return "DisconnectException"; }
			};

			//////////////////////////////////////////////////////////////////////
			// 
			// Æ¯Á¤ »óÈ²¶§ ¹«½ÃÇØ¾ß µÇ´Â ÆÐÅ¶ÀÌ µé¾î¿ÔÀ» °æ¿ì
			// 
			//////////////////////////////////////////////////////////////////////
			class IgnorePacketException : public ProtocolException {
			public :
				IgnorePacketException () throw () : ProtocolException () {}
				IgnorePacketException ( std::string msg ) throw () : ProtocolException (msg) {}
				std::string getName () const throw () { return "IgnorePacketException"; }
			};


	//////////////////////////////////////////////////////////////////////
	//
	// Thread Exception
	//
	// ¾²·¹µå ¹× µ¿±âÈ­ µµ±¸µé¿¡¼­ ¹ß»ýÇÏ´Â ¿¹¿Üµé
	//
	//////////////////////////////////////////////////////////////////////
	class ThreadException : public Exception {
	public :
		ThreadException () throw () : Exception () {}
		ThreadException ( std::string msg ) throw () : Exception (msg) {}
		std::string getName () const throw () { return "ThreadException"; }
	};

		//////////////////////////////////////////////////////////////////////
		//
		// Mutex Exception
		//
		// ¹ÂÅØ½º¿¡¼­ ¹ß»ýÇÏ´Â ¿¹¿Üµé
		//
		//////////////////////////////////////////////////////////////////////
		class MutexException : public ThreadException {
		public :
			MutexException () throw () : ThreadException () {}
			MutexException ( std::string msg ) throw () : ThreadException (msg) {}
			std::string getName () const throw () { return "MutexException"; }
		};

			//////////////////////////////////////////////////////////////////////
			//
			// Mutex Attribute Exception
			//
			// ¹ÂÅØ½º ¼Ó¼º¿¡¼­ ¹ß»ýÇÏ´Â ¿¹¿Üµé
			//
			//////////////////////////////////////////////////////////////////////
			class MutexAttrException : public MutexException {
			public :
				MutexAttrException () throw () : MutexException () {}
				MutexAttrException ( std::string msg ) throw () : MutexException (msg) {}
				std::string getName () const throw () { return "MutexAttrException"; }
			};


		//////////////////////////////////////////////////////////////////////
		//
		// Conditional Variable Exception
		//
		// Conditional Variable ¿¡¼­ ¹ß»ýÇÏ´Â ¿¹¿Ü (ÀÌ¸§ÀÌ ³Ê¹« ±æ´Ù.. - -)
		//
		//////////////////////////////////////////////////////////////////////
		class CondVarException : public ThreadException {
		public :
			CondVarException () throw () : ThreadException () {}
			CondVarException ( std::string msg ) throw () : ThreadException (msg) {}
			std::string getName () const throw () { return "CondVarException"; }
		};

		//////////////////////////////////////////////////////////////////////
		//
		// Semaphore Exception
		//
		// Semaphore ¿¡¼­ ¹ß»ýÇÏ´Â ¿¹¿Ü
		//
		//////////////////////////////////////////////////////////////////////
		class SemaphoreException : public ThreadException {
		public :
			SemaphoreException () throw () : ThreadException () {}
			SemaphoreException ( std::string msg ) throw () : ThreadException (msg) {}
			std::string getName () const throw () { return "SemaphoreException"; }
		};


	//////////////////////////////////////////////////////////////////////
	//
	// SQL Exception 
	//
	// SQL °ü·Ã ¿¹¿Ü
	//
	//////////////////////////////////////////////////////////////////////
	class SQLException : public Exception {
	public :
		SQLException () throw () : Exception() {}
		SQLException ( std::string msg ) throw () : Exception(msg) {}
		std::string getName () const throw () { return "SQLException"; }
	};

		//////////////////////////////////////////////////////////////////////
		//
		// SQL Warning
		//
		// SQL °æ°í¹®À» ³ªÅ¸³»´Â ¿¹¿Ü~~
		//
		//////////////////////////////////////////////////////////////////////
		class SQLWarning : public SQLException {
		public :
			SQLWarning () throw () : SQLException() {}
			SQLWarning ( std::string msg ) throw () : SQLException(msg) {}
			std::string getName () const throw () { return "SQLWarning"; }
		};


		//////////////////////////////////////////////////////////////////////
		//
		// SQL Connect Exception
		//
		// SQL¿¡ ´ëÇÑ ¿¬°á ½Ãµµ°¡ ½ÇÆÐÇÑ °æ¿ì, ¿¬°áÀÌ ²÷¾îÁ³À» °æ¿ì µî
		//
		//////////////////////////////////////////////////////////////////////
		class SQLConnectException : public SQLException {
		public :
			SQLConnectException () throw () : SQLException() {}
			SQLConnectException ( std::string msg ) throw () : SQLException(msg) {}
			std::string getName () const throw () { return "SQLConnectException"; }
		};

		//////////////////////////////////////////////////////////////////////
		//
		// Query Exception
		//
		//////////////////////////////////////////////////////////////////////
		class SQLQueryException : public SQLException {
		public :
			SQLQueryException () throw () : SQLException() {}
			SQLQueryException ( std::string msg ) throw () : SQLException(msg) {}
			std::string getName () const throw () { return "SQLQueryException"; }
		};


	//////////////////////////////////////////////////////////////////////
	//
	// Runtime Exception
	//
	// ·±Å¸ÀÓ¿¡ ¹ß»ý°¡´ÉÇÑ generic ÇÑ ¿ëµµ·Î »ç¿ëµÉ ¼ö ÀÖ´Â ¿¹¿Üµé
	//
	//////////////////////////////////////////////////////////////////////
	class RuntimeException : public Exception {
	public :
		RuntimeException () throw () : Exception () {}
		RuntimeException ( std::string msg ) throw () : Exception (msg) {}
		std::string getName () const throw () { return "RuntimeException"; }
	};
	
		//////////////////////////////////////////////////////////////////////
		//
		// Invalid Arguemnt Exception
		//
		// ÇÔ¼ö, ¸â¹öÇÔ¼öÀÇ ÆÄ¶ó¹ÌÅÍ°¡ Àß¸øµÈ °æ¿ì 
		//
		//////////////////////////////////////////////////////////////////////
		class InvalidArgumentException : public RuntimeException {
		public :
			InvalidArgumentException () throw () : RuntimeException () {}
			InvalidArgumentException ( std::string msg ) throw () : RuntimeException (msg) {}
			std::string getName () const throw () { return "InvalidArgumentException"; }
		};

		//////////////////////////////////////////////////////////////////////
		//
		// Out Of Bound Exception
		//
		// ¸»±×´ë·Î. Out Of Bound!
		//
		//////////////////////////////////////////////////////////////////////
		class OutOfBoundException : public RuntimeException {
		public :
			OutOfBoundException () throw () : RuntimeException () {}
			OutOfBoundException ( std::string msg ) throw () : RuntimeException (msg) {}
			std::string getName () const throw () { return "OutOfBoundException"; }
		};

		//////////////////////////////////////////////////////////////////////
		//
		// Interrupted Exception
		//
		// System Call µîÀÌ ÀÎÅÍ·´Æ® ´çÇßÀ» °æ¿ì
		//
		//////////////////////////////////////////////////////////////////////
		class InterruptedException : public RuntimeException {
		public :
			InterruptedException () throw () : RuntimeException () {}
			InterruptedException ( std::string msg ) throw () : RuntimeException (msg) {}
			std::string getName () const throw () { return "InterruptedException"; }
		};

		//////////////////////////////////////////////////////////////////////
		//
		// No Such Element Exception
		//
		// ÄÃ·º¼Ç¿¡¼­ Æ¯Á¤ Å°°ªÀ» °Ë»öÇßÀ»¶§ ±×·± ¿¤¸®¸ÕÆ®°¡ ¾ø´Â °æ¿ì
		//
		//////////////////////////////////////////////////////////////////////
		class NoSuchElementException : public RuntimeException {
		public :
			NoSuchElementException () throw () : RuntimeException () {}
			NoSuchElementException ( std::string msg ) throw () : RuntimeException (msg) {}
			std::string getName () const throw () { return "NoSuchElementException"; }
		};

		//////////////////////////////////////////////////////////////////////
		//
		// Duplicated Exception
		//
		// ÄÃ·º¼ÇÀÇ Æ¯Á¤ Å°°¡ Áßº¹µÇ¾úÀ» ¶§ 
		//
		//////////////////////////////////////////////////////////////////////
		class DuplicatedException : public RuntimeException {
		public :
			DuplicatedException () throw () : RuntimeException () {}
			DuplicatedException ( std::string msg ) throw () : RuntimeException (msg) {}
			std::string getName () const throw () { return "DuplicatedException"; }
		};

	//////////////////////////////////////////////////////////////////////
	//
	// Game Exception
	//
	// °ÔÀÓ¿¡¼­ goto ¿ëµµ·Î »ç¿ëÇÏ´Â ¿¹¿Üµé.. -_-;
	//
	//////////////////////////////////////////////////////////////////////
	class GameException : public Exception {
	public :
		GameException () throw () : Exception () {}
		GameException ( std::string msg ) throw () : Exception (msg) {}
		std::string getName () const throw () { return "GameException"; }
	};
	
		//////////////////////////////////////////////////////////////////////
		//
		// Portal Exception
		//
		// PC °¡ Æ÷Å»À» ¹â¾ÒÀ»¶§...Ä

		//
		//////////////////////////////////////////////////////////////////////
		class PortalException : public GameException {
		public :
			PortalException () throw () : GameException () {}
			PortalException ( std::string msg ) throw () : GameException (msg) {}
			std::string getName () const throw () { return "PortalException"; }
		};


//////////////////////////////////////////////////////////////////////
//
// Error
//
//////////////////////////////////////////////////////////////////////
class Error : public Throwable {
public :
	Error () throw () : Throwable() {}
	Error ( const std::string & msg ) throw () : Throwable(msg) {}
	std::string getName () const throw () { return "Error"; }
};	
	//////////////////////////////////////////////////////////////////////
	//
	// Game Error
	//
	//////////////////////////////////////////////////////////////////////
	class GameError : public Error {
	public :
		GameError () throw () : Error () {}
		GameError ( std::string msg ) throw () : Error(msg) {}
		std::string getName () const throw () { return "GameError"; }
	};


	//////////////////////////////////////////////////////////////////////
	//
	// Assertion Error
	//
	//////////////////////////////////////////////////////////////////////
	class AssertionError : public Error {
	public :
		AssertionError () throw () : Error () {}
		AssertionError ( std::string msg ) throw () : Error(msg) {}
		std::string getName () const throw () { return "AssertionError"; }
	};

	//////////////////////////////////////////////////////////////////////
	//
	// Unsupported Error
	//
	//////////////////////////////////////////////////////////////////////
	class UnsupportedError : public Error {
	public :
		UnsupportedError () throw () : Error () {}
		UnsupportedError ( std::string msg ) throw () : Error(msg) {}
		std::string getName () const throw () { return "UnsupportedError"; }
	};

	//////////////////////////////////////////////////////////////////////
	//
	// Log Error
	//
	// ÀÏ¹ÝÀûÀÎ ¿¡·¯¿Í´Â ´Þ¸® LogError´Â µðÆúÆ® ·Î±×ÆÄÀÏ¿¡ ·Î±×µÉ ¼ö ¾ø´Ù.
	// (»ý°¢ÇØº¸¶ó. ·Î±×¸Å´ÏÀú ÀÚÃ¼ÀÇ ¿¡·¯¸¦ ¾î¶»°Ô ·Î±×ÇÑ´Ù´Â ¸»ÀÎ°¡?)
	//
	//////////////////////////////////////////////////////////////////////
	class LogError : public Error {
	public :
		LogError () throw () : Error () {}
		LogError ( std::string msg ) throw () : Error(msg) {}
		std::string getName () const throw () { return "LogError"; }
	};

	//////////////////////////////////////////////////////////////////////
	//
	// Unknown Error
	//
	//////////////////////////////////////////////////////////////////////
	class UnknownError : public Error {
	public :
		UnknownError () throw () : Error() {}
		UnknownError ( const std::string & msg ) throw () : Error(msg) {}
		UnknownError ( const std::string & msg , uint ErrorCode ) throw () : Error(msg), m_ErrorCode(ErrorCode) {}
		std::string getName () const throw () { return "UnknownError"; }
		uint getErrorCode () const throw () { return m_ErrorCode; }
		std::string toString () const throw ()
		{
			StringStream buf;
			buf << getName() << " : " << getMessage() << " ( " << getErrorCode() << " ) \n"
				<< getStackTrace () ;
			
			return buf.toString();
		}
	private :
		uint m_ErrorCode;
	};

#endif
