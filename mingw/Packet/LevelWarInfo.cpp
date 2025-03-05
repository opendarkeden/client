//////////////////////////////////////////////////////////////////////
// 
// Filename    : WarInfo.cpp 
// Written By  : 
// Description : 
// 
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "LevelWarInfo.h"

//////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////
void LevelWarInfo::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	iStream.read( m_Level );
	WarInfo::read( iStream );

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////
void LevelWarInfo::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	oStream.write( m_Level );
	WarInfo::write( oStream );

	__END_CATCH
}

#ifdef __DEBUG_OUTPUT__
//////////////////////////////////////////////////////////////////////
// get debug string
//////////////////////////////////////////////////////////////////////
std::string LevelWarInfo::toString () const
	throw ()
{
	StringStream msg;

	msg << "LevelWarInfo("
		<< "Level: " << m_Level
		<< ",RemainTime:" << (int)m_RemainTime
		<< ")";

	return msg.toString();
}

#endif