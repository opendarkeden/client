//////////////////////////////////////////////////////////////////////
// 
// SocketEncryptInputStream.h 
// 
// by Reiot
// 
//////////////////////////////////////////////////////////////////////

#ifndef __SOCKET_ENCRYPT_INPUT_STREAM_H__
#define __SOCKET_ENCRYPT_INPUT_STREAM_H__

// include files
#include "Types.h"
#include "Exception.h"
#include "SocketInputStream.h"
#include "Encrypter.h"

const unsigned int DefaultSocketEncryptInputBufferSize = 81920;

//////////////////////////////////////////////////////////////////////
//
// class SocketEncryptInputStream
//
//////////////////////////////////////////////////////////////////////

class SocketEncryptInputStream : public SocketInputStream {

//////////////////////////////////////////////////
// constructor/destructor
//////////////////////////////////////////////////
public :
	
	// constructor
	SocketEncryptInputStream (Socket* sock, uint BufferSize = DefaultSocketEncryptInputBufferSize) throw (Error);
	~SocketEncryptInputStream() {}
	
//////////////////////////////////////////////////
// methods
//////////////////////////////////////////////////
public :
	
	// read data to stream (output buffer)
	// *CAUTION*
	// std::string 을 버퍼에 writing 할 때, 자동으로 size 를 앞에 붙일 수도 있다.
	// 그러나, std::string 의 크기를 BYTE/WORD 중 어느 것으로 할 건지는 의문이다.
	// 패킷의 크기는 작을 수록 좋다는 정책하에서 필요에 따라서 std::string size 값을
	// BYTE 또는 WORD 를 수동으로 사용하도록 한다.
    uint readEncrypt (bool   &buf) throw (ProtocolException, Error) { uint re = read((char*)&buf, szbool  ); buf = m_Encrypter.convert(buf); return re; }
    uint readEncrypt (char   &buf) throw (ProtocolException, Error) { uint re = read((char*)&buf, szchar  ); buf = m_Encrypter.convert(buf); return re; }
    uint readEncrypt (uchar  &buf) throw (ProtocolException, Error) { uint re = read((char*)&buf, szuchar ); buf = m_Encrypter.convert(buf); return re; }
    uint readEncrypt (short  &buf) throw (ProtocolException, Error) { uint re = read((char*)&buf, szshort ); buf = m_Encrypter.convert(buf); return re; }
    uint readEncrypt (ushort &buf) throw (ProtocolException, Error) { uint re = read((char*)&buf, szushort); buf = m_Encrypter.convert(buf); return re; }
    uint readEncrypt (int    &buf) throw (ProtocolException, Error) { uint re = read((char*)&buf, szint   ); buf = m_Encrypter.convert(buf); return re; }
    uint readEncrypt (uint   &buf) throw (ProtocolException, Error) { uint re = read((char*)&buf, szuint  ); buf = m_Encrypter.convert(buf); return re; }
    uint readEncrypt (long   &buf) throw (ProtocolException, Error) { uint re = read((char*)&buf, szlong  ); buf = m_Encrypter.convert(buf); return re; }
    uint readEncrypt (ulong  &buf) throw (ProtocolException, Error) { uint re = read((char*)&buf, szulong ); buf = m_Encrypter.convert(buf); return re; }

	void	setEncryptCode(uchar code)	{ m_Encrypter.setCode(code); }
	uchar	getEncryptCode() const		{ return m_Encrypter.getCode(); }

//////////////////////////////////////////////////
// attributes
//////////////////////////////////////////////////
private :
	Encrypter m_Encrypter;
	
};

#endif
