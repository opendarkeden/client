//////////////////////////////////////////////////////////////////////
// 
// Filename    : CRWhisper.cpp 
// Written By  : elca@ewestsoft.com
// Description : 
// 
//////////////////////////////////////////////////////////////////////

// include files

#include "Client_PCH.h"
#include "CRWhisper.h"

CRWhisper::CRWhisper()
{
}

CRWhisper::~CRWhisper()
{
	std::list<WHISPER_MESSAGE*>::iterator iString = m_Messages.begin();

	while (iString != m_Messages.end())
	{
		WHISPER_MESSAGE* pString = *iString;

		delete pString;

		iString++;
	}

	m_Messages.clear();
}

//////////////////////////////////////////////////////////////////////
// getPacketSize
//////////////////////////////////////////////////////////////////////
PacketSize_t CRWhisper::getPacketSize () const throw ()
{ 
	int messageBytes = 0;

	int num = m_Messages.size();

	std::list<WHISPER_MESSAGE*>::const_iterator iString = m_Messages.begin();

	while (iString != m_Messages.end())
	{
		WHISPER_MESSAGE* pString = *iString;

		messageBytes += szBYTE + pString->msg.size();
		messageBytes += szDWORD;

		iString++;
	}

	return szBYTE + m_Name.size() 
			+ szBYTE + m_TargetName.size() 
			+ szBYTE + messageBytes + szBYTE +szBYTE; 
}

//////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////
void CRWhisper::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

		// Name
	BYTE szName;

	iStream.read( szName );

	if ( szName == 0 )
		throw InvalidProtocolException("szName == 0");

	if ( szName > 10 )
		throw InvalidProtocolException("too large name length");

	iStream.read( m_Name, szName );

	// targetName
	iStream.read( szName );

	if ( szName == 0 )
		throw InvalidProtocolException("szName == 0");

	if ( szName > 10 )
		throw InvalidProtocolException("too large name length");

	iStream.read( m_TargetName, szName );

	BYTE numMessage;

	iStream.read( numMessage );

	for (int i=0; i<numMessage; i++)
	{
		BYTE szMessage;

		iStream.read( szMessage );

		if ( szMessage == 0 )
			throw InvalidProtocolException("szMessage == 0");

		if ( szMessage > 128 )
			throw InvalidProtocolException("too large message length");
		
		WHISPER_MESSAGE* pStr = new WHISPER_MESSAGE;
		iStream.read( pStr->msg , szMessage );
		iStream.read( pStr->color );

		m_Messages.push_back( pStr );
	}

	iStream.read( m_eRace );

	iStream.read( m_WorldID );

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////
void CRWhisper::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	// Name
	BYTE szName = m_Name.size();

	if ( szName == 0 )
		throw InvalidProtocolException("szName == 0");

	if ( szName > 10 )
		throw InvalidProtocolException("too large name length");

	oStream.write( szName );

	oStream.write( m_Name );

	// TargetName
	szName = m_TargetName.size();

	if ( szName == 0 )
		throw InvalidProtocolException("szName == 0");

	if ( szName > 10 )
		throw InvalidProtocolException("too large name length");

	oStream.write( szName );

	oStream.write( m_TargetName );
		
	// message
	BYTE numMessage = m_Messages.size();

	oStream.write( numMessage );

	std::list<WHISPER_MESSAGE*>::const_iterator iString = m_Messages.begin();

	while (iString != m_Messages.end())
	{
		WHISPER_MESSAGE* pString = *iString;

		BYTE szMessage = pString->msg.size();

		if ( szMessage == 0 )
			throw InvalidProtocolException("szMessage == 0");

		if ( szMessage > 128 )
			throw InvalidProtocolException("too large message length");

		oStream.write( szMessage );

		oStream.write( pString->msg );

		oStream.write( pString->color );

		iString++;
	}

	oStream.write( m_eRace );

	oStream.write( m_WorldID );

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// execute packet's handler
//////////////////////////////////////////////////////////////////////
void CRWhisper::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	CRWhisperHandler::execute( this , pPlayer );

	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
// get packet's debug std::string
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
std::string CRWhisper::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
	
	msg << "CRWhisper("
		<< "Name :" << m_Name
		<< "TargetName :" << m_TargetName;

		
	std::list<WHISPER_MESSAGE*>::const_iterator iString = m_Messages.begin();

	int i=0;
	while (iString != m_Messages.end())
	{
		WHISPER_MESSAGE* pString = *iString;

		msg << ",[" << i << "] " << pString->msg.c_str();
		msg << ",[" << pString->color;

		iString++;
		i++;
	}

	msg	<< ", Race :" << m_eRace
		<< ")" ;

	
	return msg.toString();
		
	__END_CATCH
}


#endif