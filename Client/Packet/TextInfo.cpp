//----------------------------------------------------------------------
//
// Filename    : TextInfo.cpp
// Writen By   : beowulf
//
//----------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "TextInfo.h"

//----------------------------------------------------------------------
// read data from socket input stream
//----------------------------------------------------------------------
void TextInfo::read ( SocketInputStream & iStream ) 
	throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	iStream.read( m_ID );

	BYTE szWriter;
	
	iStream.read( szWriter );
	
	if ( szWriter == 0 )
		throw InvalidProtocolException("szWriter == 0" );
	if ( szWriter > 20 )
		throw InvalidProtocolException("too large Writer lenth");
		
	iStream.read( m_Writer , szWriter );

	BYTE szTopic;

	iStream.read ( szTopic );

 	if ( szTopic == 0 )
		throw InvalidProtocolException("szTopic == 0" );
	if ( szTopic > 255 )
		throw InvalidProtocolException("too large Topic lenth");
		
	iStream.read( m_Topic , szTopic );

	iStream.read( m_Hit );

	__END_CATCH
}


//----------------------------------------------------------------------
// write data to socket output stream
//----------------------------------------------------------------------
void TextInfo::write ( SocketOutputStream & oStream ) const 
	throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	oStream.write( m_ID );

	BYTE szWriter = m_Writer.size();

	if ( szWriter == 0 )
		throw InvalidProtocolException("empty BBS_ID");
	if ( szWriter > 20 )
		throw InvalidProtocolException("Too large lenth");

	oStream.write( szWriter );
	
	oStream.write( m_Writer );

	BYTE szTopic = m_Topic.size();
	
	if ( szTopic == 0 )
		throw InvalidProtocolException ("empty BBS_Topic");
	if ( szTopic > 255 )
		throw InvalidProtocolException ("too lorge Topic lenth");

	oStream.write( szTopic );
	
	oStream.write( m_Topic );
	
	oStream.write( m_Hit );

	__END_CATCH
}


//----------------------------------------------------------------------
// get debug string
//----------------------------------------------------------------------
std::string TextInfo::toString () const 
	throw ()
{
	StringStream msg;
	
	msg << "TextInfo("
		<< "ID:" << m_ID 
		<< ",Writer:"<< m_Writer
		<< ",Topic:"<< m_Topic
		<< ",Hit:"<< m_Hit
		<< ")";

	return msg.toString();
}
	
