//////////////////////////////////////////////////////////////////////
// 
// Filename    : GCAddMonsterFromBurrowing.cpp 
// Written By  : Reiot
// 
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "Client_PCH.h"
#include "GCAddMonsterFromBurrowing.h"

//--------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------
GCAddMonsterFromBurrowing::GCAddMonsterFromBurrowing()
	throw( Error )
:m_pEffectInfo(NULL)
{
	__BEGIN_TRY
	__END_CATCH
}

//--------------------------------------------------------------------
// Destructor
//--------------------------------------------------------------------
GCAddMonsterFromBurrowing::~GCAddMonsterFromBurrowing()
	throw( Error )
{
	__BEGIN_TRY

	if( m_pEffectInfo != NULL )
	{
		delete m_pEffectInfo;
		m_pEffectInfo = NULL;
	}

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////
// �Է½�Ʈ��(����)���κ��� ����Ÿ�� �о ��Ŷ�� �ʱ�ȭ�Ѵ�.
//////////////////////////////////////////////////////////////////////
void GCAddMonsterFromBurrowing::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	BYTE name_length = 0;
		
	iStream.read( m_ObjectID );
	iStream.read( m_MonsterType );

	iStream.read(name_length);
	if (name_length != 0) iStream.read(m_MonsterName, name_length);

	iStream.read( m_MainColor );
	iStream.read( m_SubColor );
	iStream.read( m_X );
	iStream.read( m_Y );
	iStream.read( m_Dir );

	m_pEffectInfo = new EffectInfo();
	m_pEffectInfo->read( iStream );

	iStream.read( m_CurrentHP );
	iStream.read( m_MaxHP );

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// ��½�Ʈ��(����)���� ��Ŷ�� ���̳ʸ� �̹����� ������.
//////////////////////////////////////////////////////////////////////
void GCAddMonsterFromBurrowing::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	BYTE name_length = m_MonsterName.size();

	oStream.write( m_ObjectID );
	oStream.write( m_MonsterType );

	oStream.write( name_length );
	if (m_MonsterName.size() != 0) oStream.write( m_MonsterName );

	oStream.write( m_MainColor );
	oStream.write( m_SubColor );
	oStream.write( m_X );
	oStream.write( m_Y );
	oStream.write( m_Dir );

	m_pEffectInfo->write( oStream );

	oStream.write( m_CurrentHP );
	oStream.write( m_MaxHP );

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// execute packet's handler
//////////////////////////////////////////////////////////////////////
void GCAddMonsterFromBurrowing::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	GCAddMonsterFromBurrowingHandler::execute( this , pPlayer );
		
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// get packet's debug std::string
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
std::string GCAddMonsterFromBurrowing::toString () const
       throw ()
{
	__BEGIN_TRY

	StringStream msg;

	msg << "GCAddMonsterFromBurrowing("
		<< "ObjectID:"     << (int)m_ObjectID
		<< ",MonsterType:" << (int)m_MonsterType
		<< ",MonsterName:" << m_MonsterName
		<< ",MainColor:"   << (int)m_MainColor
		<< ",SubColor:"    << (int)m_SubColor
		<< ",X:"           << (int)m_X 
		<< ",Y:"           << (int)m_Y 
		<< ",Dir:"         << Dir2String[m_Dir]
		<< ",Effects:"     << m_pEffectInfo->toString()
		<< ",CurrentHP:"   << (int)m_CurrentHP
		<< ",MaxHP:"       << (int)m_MaxHP
		<< ")" ;

	return msg.toString();

	__END_CATCH
}
#endif