//////////////////////////////////////////////////////////////////////////////
// Filename    : GearSlotInfo.cpp
// Written By  : elca
// Description :
//////////////////////////////////////////////////////////////////////////////

#include "Client_PCH.h"
#include "GearSlotInfo.h"

//////////////////////////////////////////////////////////////////////////////
// read data from socket input stream
//////////////////////////////////////////////////////////////////////////////
void GearSlotInfo::read ( SocketInputStream & iStream ) 
	throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	// ���� ���� Ŭ�������� �о���̰�...
	PCItemInfo::read(iStream);

	// �� Ŭ������ �����͸� �о���δ�.
	iStream.read(m_SlotID);

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
// write data to socket output stream
//////////////////////////////////////////////////////////////////////////////
void GearSlotInfo::write ( SocketOutputStream & oStream ) const 
	throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	// ���� ���� Ŭ�������� �о���̰�...
	PCItemInfo::write(oStream);

	// �� Ŭ������ �����͸� �о���δ�.
	oStream.write(m_SlotID);

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
// get debug string
//////////////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
std::string GearSlotInfo::toString () const 
	throw ()
{
	__BEGIN_TRY

	StringStream msg;
	msg << "GearSlotInfo("
		<< PCItemInfo::toString()
		<< "SlotID:" << (int)m_SlotID
		<< ")";
	return msg.toString();

	__END_CATCH
}
#endif