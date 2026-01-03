//----------------------------------------------------------------------
//
// Filename    : TextInfo.h
// Writen By   : Reiot
//
//----------------------------------------------------------------------

#ifndef __TEXT_INFO_H__
#define __TEXT_INFO_H__

// include files
#include "Types.h"
#include "Exception.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

const uint maxTexts = 10;

//----------------------------------------------------------------------
//
// class TextInfo;
//
//----------------------------------------------------------------------

class TextInfo {

public :

	//----------------------------------------------------------------------
	// 패킷에 embedding 되려는 객체는 아래와 같은 데이타를 명시해야 한다.
	//----------------------------------------------------------------------

	// read data from socket input stream
	void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );

	// write data to socket output stream
	void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );

	// get size of object
	uint getSize () const throw () 
	{ 
		return szDWORD 					// text id
			+ szBYTE + m_Writer.size() 	// writer
			+ szBYTE + m_Topic.size() 	// topic
			+ szDWORD; 					// hits
	}

	// get debug std::string
	std::string toString () const throw ();

	// get max size()
	static uint getMaxSize () throw () 
	{ 
		return szDWORD		// text id
			+ szBYTE + 20 	// writer
			+ szBYTE + 255	// topic
			+ szDWORD; 		// hits
	} 
	
public :

	uint getID () const throw () { return m_ID; }
	void setID ( uint id ) throw () { m_ID = id; }
	
	std::string getWriter () const throw () { return m_Writer ; }
	void setWriter( std::string writer ) throw () { m_Writer = writer ; }
	
	std::string getTopic () const throw () { return m_Topic ; }
	void setTopic ( std::string topic ) throw () { m_Topic = topic ; }
	
	uint getHit () const throw () { return m_Hit ; }
	void setHit ( uint hit) throw () { m_Hit = hit ; }
	
private :

	DWORD 	m_ID;
	std::string 	m_Writer;
	std::string 	m_Topic;
	DWORD 	m_Hit;

};

#endif
