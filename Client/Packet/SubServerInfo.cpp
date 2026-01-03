//----------------------------------------------------------------------
//
// Filename    : SubServerInfo.cpp
// Written By  : elca
// Description :
//
//----------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "SubServerInfo.h"

//----------------------------------------------------------------------
// read data from socket input stream
//----------------------------------------------------------------------
void SubServerInfo::read ( SocketInputStream & iStream ) 
	throw ( ProtocolException , Error )
{
	__BEGIN_TRY
	try {

	// 이름 크기
	BYTE szServerName;
	iStream.read( m_ServerID );

	// 이름 크기를 읽는다.
	iStream.read( szServerName );

	// 실제 이름을 읽는다.
	iStream.read( m_ServerName, szServerName );

	// 서버 상태를 읽는다.
	iStream.read( m_ServerStat );

	} catch( Throwable & t ) {
		cout << t.toString().c_str() << endl;
	}

	__END_CATCH
}

//----------------------------------------------------------------------
// write data to socket output stream
//----------------------------------------------------------------------
void SubServerInfo::write ( SocketOutputStream & oStream ) const 
	throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	try {

	// 이름 크기
	BYTE szServerName = m_ServerName.size();

	oStream.write( m_ServerID );

	// 이름 크기를 쓴다
	oStream.write( szServerName );

	// 실제 이름을 쓴다
	oStream.write( m_ServerName );

	// 서버 상태를 쓴다
	oStream.write( m_ServerStat );

	} catch (Throwable & t ) {
		cout << t.toString().c_str() << endl;
	}

	__END_CATCH
}

//----------------------------------------------------------------------
// get debug string
//----------------------------------------------------------------------
#ifdef __DEBUG_OUTPUT__
	std::string SubServerInfo::toString () const 
		throw ()
	{
		StringStream msg;

		msg << "SubServerInfo("
			<< "ServerID :" << (int)m_ServerID
			<< ",ServerName :" << m_ServerName
			<< ",ServerStat :" << (int)m_ServerStat
			<< ")";
		
		return msg.toString();
	}
#endif