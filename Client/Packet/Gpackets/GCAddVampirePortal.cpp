//////////////////////////////////////////////////////////////////////////////
// Filename    : GCAddVampirePortal.cpp
// Written By  : excel96
// Description :
// �����̾� ��Ż�� ���� ����Ʈ�� �������μ� �����Ǵµ�, �� ��Ŷ��
// Ŭ���̾�Ʈ���� �ٴڿ��� �����̾� ��Ż ����Ʈ�� ���̶�� ��Ŷ�̴�.
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "GCAddVampirePortal.h"

//////////////////////////////////////////////////////////////////////////////
// class GCAddVampirePortal member methods
//////////////////////////////////////////////////////////////////////////////
void GCAddVampirePortal::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	iStream.read(m_ObjectID);

	BYTE length = 0;
	iStream.read(length);

	if (length > 0)
	{
		iStream.read(m_OwnerID, length);
	}

	iStream.read(m_Duration);
	iStream.read(m_X);
	iStream.read(m_Y);
	iStream.read(m_TargetZoneID);
	iStream.read(m_TargetX);
	iStream.read(m_TargetY);
	iStream.read(m_CreateFlag);

	__END_CATCH
}
		    
void GCAddVampirePortal::write ( SocketOutputStream & oStream ) 
     const throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	oStream.write(m_ObjectID);

	BYTE length = m_OwnerID.size();
	oStream.write(length);

	if (length > 0)
	{
		oStream.write(m_OwnerID);
	}

	oStream.write(m_Duration);
	oStream.write(m_X);
	oStream.write(m_Y);
	oStream.write(m_TargetZoneID);
	oStream.write(m_TargetX);
	oStream.write(m_TargetY);
	oStream.write(m_CreateFlag);
		
	__END_CATCH
}

void GCAddVampirePortal::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	GCAddVampirePortalHandler::execute( this , pPlayer );
		
	__END_CATCH
}

#ifdef __DEBUG_OUTPUT__
	std::string GCAddVampirePortal::toString () 
		const throw ()
	{
		__BEGIN_TRY

		StringStream msg;
		msg << "GCAddVampirePortal( "
			<< ",ObjectID:" << (int)m_ObjectID
			<< ",OwnerID:"  << m_OwnerID
			<< ",Duration:" << (int)m_Duration
			<< ",X:"        << (int)m_X
			<< ",Y:"        << (int)m_Y
			<< ",TZID:"     << (int)m_TargetZoneID
			<< ",TX:"       << (int)m_TargetX
			<< ",TY:"       << (int)m_TargetY
			<< ",Flag:"     << (int)m_CreateFlag
			<< ")";
		return msg.toString();

		__END_CATCH
	}

#endif