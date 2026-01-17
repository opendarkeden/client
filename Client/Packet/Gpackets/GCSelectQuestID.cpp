//////////////////////////////////////////////////////////////////////////////
// Filename    : GCSelectQuestID.cpp 
// Written By  : Reiot
// Description : 
//////////////////////////////////////////////////////////////////////////////

#include "Client_PCH.h"
#include "GCSelectQuestID.h"
#include "Assert.h"

#ifdef __GAME_SERVER__
#include "mission/QuestInfoManager.h"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
GCSelectQuestID::GCSelectQuestID(PlayerCreature* pPC)
	throw ()
{
	__BEGIN_TRY 

	g_pQuestInfoManager->getPossibleQuestIDs( pPC, back_inserter( m_QuestIDList ) );

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
GCSelectQuestID::~GCSelectQuestID() 
	throw ()
{
	__BEGIN_TRY 

	__END_CATCH
}

#endif

//////////////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////////////
void GCSelectQuestID::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	BYTE num;

	iStream.read( num );

	for ( int i=0; i<num; ++i )
	{
		QuestID_t qID;
		iStream.read( qID );

		m_QuestIDList.push_back( qID );
	}

	__END_CATCH
}
		    
//////////////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////////////
void GCSelectQuestID::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	Assert( m_QuestIDList.size() <= maxQuestNum );

	BYTE num = m_QuestIDList.size();

	oStream.write(num);

	std::list<QuestID_t>::const_iterator itr = m_QuestIDList.begin();

	for ( int i=0; i<num; i++ )
	{
		oStream.write( *itr++ );
	}

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////////////
// execute packet's handler
//////////////////////////////////////////////////////////////////////////////
void GCSelectQuestID::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	GCSelectQuestIDHandler::execute( this , pPlayer );

	__END_CATCH
}

PacketSize_t GCSelectQuestID::getPacketSize() const
	throw()
{
	__BEGIN_TRY

	PacketSize_t result = 0;

	result += szBYTE + szQuestID * m_QuestIDList.size();

	return result;

	__END_CATCH
}

#ifdef __DEBUG_OUTPUT__
//////////////////////////////////////////////////////////////////////////////
// get packet's debug std::string
//////////////////////////////////////////////////////////////////////////////
std::string GCSelectQuestID::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
	msg << "GCSelectQuestID("
		<< "Parameters: (" ;

	std::list<QuestID_t>::const_iterator itr = m_QuestIDList.begin();
	for( ; itr != m_QuestIDList.end() ; itr++ )
	{
		msg << *itr << ", ";
	}
	msg << ") )";

	return msg.toString();
		
	__END_CATCH
}

#endif