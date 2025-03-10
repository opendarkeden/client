//////////////////////////////////////////////////////////////////////
// 
// Filename    : SocketInputStream.h 
// Written by  : reiot@ewestsoft.com
// Description :
// 
//////////////////////////////////////////////////////////////////////
//
// *Reiot's Notes*
//
// 시스템에서 가장 빈번하게 사용되는 클래스중의 하나이다.
// 속도에 무지막지한 영향을 미치므로, 만일 좀더 속도를 보강하고
// 싶다면, exception을 빼고 re-write 하라. 
//
// 현재 nonblocking 이 굉장히-억수로-졸라 많이 발생한다고 했을때,
// 이것이 NonBlockingIOException으로 wrapping될때 overhead가 발생할
// 확률이 높다고 추측된다.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SOCKET_INPUT_STREAM_H__
#define __SOCKET_INPUT_STREAM_H__

// include files
#include "Types.h"
#include "Exception.h"
#include "Socket.h"

// constant definitions
const uint DefaultSocketInputBufferSize = 8192;

// forward declaration
class Packet;

//////////////////////////////////////////////////////////////////////
//
// class SocketInputStream
//
//////////////////////////////////////////////////////////////////////

class SocketInputStream {

//////////////////////////////////////////////////
// constructor/destructor
//////////////////////////////////////////////////
public :
	
	// constructor
	SocketInputStream ( Socket * sock , uint BufferSize = DefaultSocketInputBufferSize ) throw ( );
	
	// destructor
	virtual ~SocketInputStream () throw ( );

	
//////////////////////////////////////////////////
// methods
//////////////////////////////////////////////////
public :
	
	// read data from stream (input buffer)
	uint read ( char * buf , uint len ) throw ( );
	uint read ( std::string & str , uint len ) throw ( );
	void read ( Packet * p ) throw ( );

	uint read ( bool   & buf ) throw ( ) { return read( (char*)&buf, szbool   ); }
	uint read ( char   & buf ) throw ( ) { return read( (char*)&buf, szchar   ); }
	uint read ( uchar  & buf ) throw ( ) { return read( (char*)&buf, szuchar  ); }
	uint read ( short  & buf ) throw ( ) { return read( (char*)&buf, szshort  ); }
	uint read ( ushort & buf ) throw ( ) { return read( (char*)&buf, szushort ); }
	uint read ( int    & buf ) throw ( ) { return read( (char*)&buf, szint    ); }
	uint read ( uint   & buf ) throw ( ) { return read( (char*)&buf, szuint   ); }
	uint read ( long   & buf ) throw ( ) { return read( (char*)&buf, szlong   ); }
	uint read ( ulong  & buf ) throw ( ) { return read( (char*)&buf, szulong  ); }

	// peek data from stream (input buffer)
	bool peek ( char * buf , uint len ) throw ( );
	
	// skip data from stream (input buffer)
	void skip ( uint len ) throw ( );
	
	// fill stream (input buffer) from socket
	uint fill () throw ( );
	uint fill_RAW () throw ( );

	// resize buffer
	void resize ( int size ) throw ( );
	
	// get buffer length
	uint capacity () const throw () { return m_BufferLen; }
	
	// get data length in buffer
	uint length () const throw ();
	uint size () const throw () { return length(); }

	// check if buffer is empty
	bool isEmpty () const throw () { return m_Head == m_Tail; }

	// get debug string
	std::string toString () const throw ();


//////////////////////////////////////////////////
// attributes
//////////////////////////////////////////////////
private :
	
	// socket
	Socket * m_pSocket;
	
	// buffer
	char * m_Buffer;
	
	// buffer length
	uint m_BufferLen;
	
	// buffer head/tail
	uint m_Head;
	uint m_Tail;
//add by viva 2008-12-31
public :
	WORD m_EncryptKey;
	BYTE* m_HashTable;
	void setKey(WORD EncryptKey, BYTE* HashTable) throw() {m_EncryptKey = EncryptKey; m_HashTable = HashTable; }
	WORD EncryptData(WORD EncryptKey, char* buf, int len) throw();
//end
};

#endif
