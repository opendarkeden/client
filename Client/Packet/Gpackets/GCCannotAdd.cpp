//////////////////////////////////////////////////////////////////////
// 
// Filename    : GCCannotAdd.cc 
// Written By  : elca@ewestsoft.com
// Description : CGMove�� ���� ������ �ڱ� �ڽſ��� OK ������ ������
//               ���� ��Ŷ Ŭ���� �Լ� ����
// 
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCCannotAdd.h"


//////////////////////////////////////////////////////////////////////
// �Է½�Ʈ��(����)���κ��� ����Ÿ�� �о ��Ŷ�� �ʱ�ȭ�Ѵ�.
//////////////////////////////////////////////////////////////////////
void GCCannotAdd::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	iStream.read( m_ObjectID );

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// ��½�Ʈ��(����)���� ��Ŷ�� ���̳ʸ� �̹����� ������.
//////////////////////////////////////////////////////////////////////
void GCCannotAdd::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	oStream.write( m_ObjectID );

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////
// execute packet's handler
//////////////////////////////////////////////////////////////////////
void GCCannotAdd::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	GCCannotAddHandler::execute( this , pPlayer );
		
	__END_CATCH
}

//////////////////////////////////////////////////////////////////////
//
// get packet's debug std::string
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
	std::string GCCannotAdd::toString () const
		   throw ()
	{
		__BEGIN_TRY

		StringStream msg;
		msg << "GCCannotAdd("
			<< "ObjectId:" << (int)m_ObjectID
			<< ")";
		return msg.toString();

		__END_CATCH
	}
#endif