//////////////////////////////////////////////////////////////////////////////
// Filename    : CGPartyInvite.cpp 
// Written By  : 김성민
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGPartyInvite.h"

//////////////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////////////
void CGPartyInvite::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY

	iStream.read(m_TargetObjectID);
	iStream.read(m_Code);

	__END_CATCH
}
		    
//////////////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////////////
void CGPartyInvite::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY

	oStream.write(m_TargetObjectID);
	oStream.write(m_Code);

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
// execute packet's handler
//////////////////////////////////////////////////////////////////////////////
void CGPartyInvite::execute (Player * pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY

	#ifndef __GAME_CLIENT__
		CGPartyInviteHandler::execute (this , pPlayer);
	#endif
		
	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
// get debug std::string
//////////////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
std::string CGPartyInvite::toString () 
	const throw ()
{
	__BEGIN_TRY

	StringStream msg;
	msg << "CGPartyInvite(" 
		<< "TargetObjectID : " << (int)m_TargetObjectID << ","
		<< "CODE : "           << (int)m_Code 
		<< ")";
	return msg.toString();

	__END_CATCH
}
#endif