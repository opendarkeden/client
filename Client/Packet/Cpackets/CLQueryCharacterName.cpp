//////////////////////////////////////////////////////////////////////////////
// Filename    : CLQueryCharacterName.cpp 
// Written By  : reiot@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CLQueryCharacterName.h"

void CLQueryCharacterName::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY

	// read player id
	BYTE szCharacterName;

	iStream.read(szCharacterName);

	if (szCharacterName == 0)
		throw InvalidProtocolException("szCharacterName == 0");

	if (szCharacterName > 20)
		throw InvalidProtocolException("too long CharacterName length");

	iStream.read(m_CharacterName , szCharacterName);

	__END_CATCH
}
		    
void CLQueryCharacterName::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY

	// write player id
	BYTE szCharacterName = m_CharacterName.size();

	if (szCharacterName == 0)
		throw InvalidProtocolException("empty CharacterName");

	if (szCharacterName > 20)
		throw InvalidProtocolException("too long CharacterName length");

	oStream.write(szCharacterName);

	oStream.write(m_CharacterName);

	__END_CATCH
}

void CLQueryCharacterName::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	#ifndef __GAME_CLIENT__
		CLQueryCharacterNameHandler::execute (this , pPlayer);
	#endif
		
	__END_CATCH
}

#ifdef __DEBUG_OUTPUT__
	std::string CLQueryCharacterName::toString () const
		throw ()
	{
		__BEGIN_TRY
			
		StringStream msg;
		msg << "CLQueryCharacterName("
			<< "CharacterName:" << m_CharacterName 
			<< ")";
		return msg.toString();
			
		__END_CATCH
	}
#endif