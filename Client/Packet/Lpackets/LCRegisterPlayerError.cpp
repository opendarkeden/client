//////////////////////////////////////////////////////////////////////
// 
// Filename    : LCRegisterPlayerError.cpp 
// Written By  : Reiot
// Description : �α��� ���н� �α��� �������� Ŭ���̾�Ʈ���� �����ϴ� ��Ŷ
// 
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "LCRegisterPlayerError.h"

//////////////////////////////////////////////////////////////////////
// �Է½�Ʈ��(����)���κ��� ����Ÿ�� �о ��Ŷ�� �ʱ�ȭ�Ѵ�.
//////////////////////////////////////////////////////////////////////
void LCRegisterPlayerError::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	iStream.read( m_ErrorID );

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// ��½�Ʈ��(����)���� ��Ŷ�� ���̳ʸ� �̹����� ������.
//////////////////////////////////////////////////////////////////////
void LCRegisterPlayerError::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	oStream.write( m_ErrorID );

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// execute packet's handler
//////////////////////////////////////////////////////////////////////
void LCRegisterPlayerError::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	LCRegisterPlayerErrorHandler::execute( this , pPlayer );
		
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// get packet's debug string
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
	std::string LCRegisterPlayerError::toString () const
		   throw ()
	{
		__BEGIN_TRY

		StringStream msg;
		msg << "LCRegisterPlayerError( " << (int)m_ErrorID << ")";
		return msg.toString();

		__END_CATCH
	}
#endif
