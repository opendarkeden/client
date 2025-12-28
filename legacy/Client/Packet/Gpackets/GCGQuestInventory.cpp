//////////////////////////////////////////////////////////////////////
// 
// Filename    : GCGQuestInventory.cpp 
// Written By  : elca@ewestsoft.com
// Description : 자신에게 쓰는 기술의 성공을 알리기 위한 패킷 클래스의
//               멤버 정의.
// 
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "GCGQuestInventory.h"

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
GCGQuestInventory::GCGQuestInventory () 
     throw ()
{
	__BEGIN_TRY
	__END_CATCH
}
	
//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
GCGQuestInventory::~GCGQuestInventory () 
    throw ()
{
	__BEGIN_TRY
	__END_CATCH
}

void GCGQuestInventory::read(SocketInputStream & iStream) throw(ProtocolException, Error)
{
	__BEGIN_TRY

	BYTE size;
	iStream.read(size);

	for ( int i=0; i<size; ++i )
	{
		ItemType_t item;
		iStream.read(item);
		m_ItemList.push_back(item);
	}

	__END_CATCH
}

void GCGQuestInventory::write(SocketOutputStream & oStream) const throw(ProtocolException, Error)
{
	__BEGIN_TRY

	BYTE size = m_ItemList.size();
	oStream.write(size);

	for ( int i=0; i<size; ++i )
	{
		oStream.write( m_ItemList[i] );
	}

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////
//
// execute packet's handler
//
//////////////////////////////////////////////////////////////////////
void GCGQuestInventory::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	GCGQuestInventoryHandler::execute( this , pPlayer );
		
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// get packet's debug string
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
string GCGQuestInventory::toString () 
	const throw ()
{
	__BEGIN_TRY

	StringStream msg;
	msg << "GCGQuestInventory("
		<< ")";
	return msg.toString();

	__END_CATCH
}
#endif
