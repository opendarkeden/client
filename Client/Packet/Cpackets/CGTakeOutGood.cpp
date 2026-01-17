//////////////////////////////////////////////////////////////////////////////
// Filename    : CGTakeOutGood.cpp 
// Written By  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGTakeOutGood.h"
#include "Assert.h"


CGTakeOutGood::CGTakeOutGood () 
     throw ()
{
	__BEGIN_TRY
	__END_CATCH
}

CGTakeOutGood::~CGTakeOutGood () 
    throw ()
{
	__BEGIN_TRY
	__END_CATCH
}

void CGTakeOutGood::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	iStream.read(m_ObjectID);

	__END_CATCH
}

void CGTakeOutGood::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY

	oStream.write(m_ObjectID);

	__END_CATCH
}

void CGTakeOutGood::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY

#ifndef __GAME_CLIENT__
	CGTakeOutGoodHandler::execute (this , pPlayer);
#endif
		
	__END_CATCH
}

#ifdef __DEBUG_OUTPUT__
std::string CGTakeOutGood::toString () 
	const throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
	msg << "CGTakeOutGood(ObjectID : " << (int)m_ObjectID << ")";
	return msg.toString();

	__END_CATCH
}
#endif