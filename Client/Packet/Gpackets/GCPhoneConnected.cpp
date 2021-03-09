//////////////////////////////////////////////////////////////////////
// 
// Filename    : GCPhoneConnected.cpp 
// Written By  : elca@ewestsoft.com
// Description : 
// 
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCPhoneConnected.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"


//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
GCPhoneConnected::GCPhoneConnected () 
     throw ()
{
	__BEGIN_TRY
	__END_CATCH
}

	
//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
GCPhoneConnected::~GCPhoneConnected () 
    throw ()
{
	__BEGIN_TRY
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// �Է½�Ʈ��(����)���κ��� ����Ÿ�� �о ��Ŷ�� �ʱ�ȭ�Ѵ�.
//////////////////////////////////////////////////////////////////////
void GCPhoneConnected::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	iStream.read( m_PhoneNumber );
	iStream.read( m_SlotID );

	BYTE szName;
	iStream.read( szName );

	if ( szName == 0 )
		throw InvalidProtocolException("szName == 0");
	if ( szName > 20 )
		throw InvalidProtocolException("too long name length");

	iStream.read( m_Name , szName );

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// ��½�Ʈ��(����)���� ��Ŷ�� ���̳ʸ� �̹����� ������.
//////////////////////////////////////////////////////////////////////
void GCPhoneConnected::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	oStream.write( m_PhoneNumber );
	oStream.write( m_SlotID );

	BYTE szName = m_Name.size();

	if ( szName == 0 )
		throw InvalidProtocolException("szName == 0");

	if ( szName > 20 )
		throw InvalidProtocolException("too long name length");

	oStream.write( szName );
	oStream.write( m_Name );

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// execute packet's handler
//////////////////////////////////////////////////////////////////////
void GCPhoneConnected::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	GCPhoneConnectedHandler::execute ( this , pPlayer );
		
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// toString()
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
	std::string GCPhoneConnected::toString () const
		throw ()
	{
		__BEGIN_TRY
			
		StringStream msg;

		msg << "GCPhoneConnected( PhoneNumber : " << (int)m_PhoneNumber
			<< ", SlotID : " << (int)m_SlotID
			<< ", Name : " << m_Name;
			//<< " )" << endl;

		return msg.toString();

		__END_CATCH
	}
#endif