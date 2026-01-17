//////////////////////////////////////////////////////////////////////////////
// Filename    : CGAddMouseToZone.cpp 
// Written By  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGAddMouseToZone.h"
#include "SocketEncryptInputStream.h"
#include "SocketEncryptOutputStream.h"
#include "Assert.h"


CGAddMouseToZone::CGAddMouseToZone () 
     throw ()
{
	__BEGIN_TRY
	__END_CATCH
}

CGAddMouseToZone::~CGAddMouseToZone () 
    throw ()
{
	__BEGIN_TRY
	__END_CATCH
}

void CGAddMouseToZone::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
#ifdef __USE_ENCRYPTER__
	SocketEncryptInputStream* pEIStream = dynamic_cast<SocketEncryptInputStream*>(&iStream);
    Assert(pEIStream!=NULL);

	if (pEIStream->getEncryptCode()!=0)
	{
		pEIStream->readEncrypt(m_ObjectID);
	}
	else
#endif
	{
		iStream.read(m_ObjectID);
	}

	__END_CATCH
}

void CGAddMouseToZone::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY

#ifdef __USE_ENCRYPTER__
	SocketEncryptOutputStream* pEOStream = dynamic_cast<SocketEncryptOutputStream*>(&oStream);
    Assert(pEOStream!=NULL);

	if (pEOStream->getEncryptCode()!=0)
	{
		pEOStream->writeEncrypt(m_ObjectID);
	}
	else
#endif
	{
		oStream.write(m_ObjectID);
	}

	__END_CATCH
}

void CGAddMouseToZone::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY

#ifndef __GAME_CLIENT__
	CGAddMouseToZoneHandler::execute (this , pPlayer);
#endif
		
	__END_CATCH
}

#ifdef __DEBUG_OUTPUT__
std::string CGAddMouseToZone::toString () 
	const throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
	msg << "CGAddMouseToZone(ObjectID : " << (int)m_ObjectID << ")";
	return msg.toString();

	__END_CATCH
}
#endif