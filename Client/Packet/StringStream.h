//////////////////////////////////////////////////////////////////////
//
// Filename    : StringStream.h
// Written By  : reiot@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

#ifndef __STRING_STREAM_H__
#define __STRING_STREAM_H__

#pragma warning(disable:4786)

// include files
#include "Types/SystemTypes.h"
#include <list>
#include <string>

// end of stream
const char eos = '\n';

class StringStream {
	
public :
	
	// constructor
	StringStream () throw ();
	
	// destructor
	~StringStream () throw ();
	
	
public :
	
	// add string to stream
	StringStream & operator << ( bool T ) throw ();
	StringStream & operator << ( char T ) throw ();
	StringStream & operator << ( unsigned char T ) throw ();
	StringStream & operator << ( short T ) throw ();
	StringStream & operator << ( unsigned short T ) throw ();
	StringStream & operator << ( int T ) throw ();
	StringStream & operator << ( unsigned int T ) throw ();
	StringStream & operator << ( long T ) throw ();
	StringStream & operator << ( unsigned long T ) throw ();
	StringStream & operator << ( float T ) throw ();
	StringStream & operator << ( double T ) throw ();
	StringStream & operator << ( const char * str ) throw ();
	StringStream & operator << ( const std::string & str ) throw ();

	// make string
	std::string toString () const throw ();
	
	// true if stream is empty
	bool isEmpty () const throw () { return m_Size == 0; }

private :
	
	// list of string
	std::list<std::string> m_Strings;

	// size of string which will be generated
	unsigned short m_Size;
	
	// inserted flag 
	mutable bool m_bInserted;
	
	// buffer for string will be generated
	mutable std::string m_Buffer;

};

#endif
