//--------------------------------------------------------------------------------
// 
// Filename    : CLSelectPC.cpp 
// Written By  : reiot@ewestsoft.com
// Description : 
// 
//--------------------------------------------------------------------------------

// include files
#include "Client_PCH.h"
#include "CLSelectPC.h"


//--------------------------------------------------------------------------------
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//--------------------------------------------------------------------------------
void CLSelectPC::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	//--------------------------------------------------
	// read creature's name
	//--------------------------------------------------
	BYTE szPCName;

	iStream.read( szPCName );

	if ( szPCName == 0 ) 
		throw InvalidProtocolException("szPCName == 0");

	if ( szPCName > 20 ) 
		throw InvalidProtocolException("too long pc name length");

	iStream.read( m_PCName , szPCName );

	cout << "Select PC Name : " << m_PCName.c_str() << endl;

	//--------------------------------------------------
	// read pc type
	//--------------------------------------------------
	BYTE pcType;
	iStream.read( pcType );
	m_PCType = PCType(pcType);

	cout << "Select PC Type : " << (int)m_PCType << endl;

	if ( m_PCType != PC_SLAYER && m_PCType != PC_VAMPIRE && m_PCType != PC_OUSTERS )
		throw InvalidProtocolException("invalid pc type");

	__END_CATCH
}

		    
//--------------------------------------------------------------------------------
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//--------------------------------------------------------------------------------
void CLSelectPC::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	cout << "Select PC Name : " << m_PCName.c_str() << endl;
	cout << "Select PC Type : " << (int)m_PCType << endl;

	//--------------------------------------------------
	// write creature's name
	//--------------------------------------------------
	BYTE szPCName = m_PCName.size();

	if ( szPCName == 0 ) 
		throw InvalidProtocolException("szPCName == 0");

	if ( szPCName > 20 ) 
		throw InvalidProtocolException("too long pc name length");

	oStream.write( szPCName );

	oStream.write( m_PCName );

	//--------------------------------------------------
	// write pc type
	//--------------------------------------------------
	if ( m_PCType != PC_SLAYER && m_PCType != PC_VAMPIRE && m_PCType != PC_OUSTERS )
		throw InvalidProtocolException("invalid pc type");

	oStream.write( (BYTE)m_PCType );

	__END_CATCH
}


//--------------------------------------------------------------------------------
// execute packet's handler
//--------------------------------------------------------------------------------
void CLSelectPC::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	#ifndef __GAME_CLIENT__
		CLSelectPCHandler::execute( this , pPlayer );
	#endif
		
	__END_CATCH
}


//--------------------------------------------------------------------------------
// get packet's debug std::string
//--------------------------------------------------------------------------------
#ifdef __DEBUG_OUTPUT__
	std::string CLSelectPC::toString () const
		   throw ()
	{
		__BEGIN_TRY

		StringStream msg;
		
		msg << "CLSelectPC("
			<< "PCName:" << m_PCName 
			<< ",PCType:" << PCType2String[m_PCType] 
			<< ")" ;
		
		return msg.toString();

		__END_CATCH
	}


#endif
