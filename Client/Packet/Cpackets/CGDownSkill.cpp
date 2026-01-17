//////////////////////////////////////////////////////////////////////////////
// Filename    : CGDownSkill.cpp 
// Written By  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////
#include "Client_PCH.h"
#include "CGDownSkill.h"

void CGDownSkill::read (SocketInputStream & iStream) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	iStream.read(m_SkillType);

	__END_CATCH
}
		    
void CGDownSkill::write (SocketOutputStream & oStream) const 
     throw (ProtocolException , Error)
{
	__BEGIN_TRY
		
	oStream.write(m_SkillType);

	__END_CATCH
}

void CGDownSkill::execute (Player* pPlayer) 
	 throw (ProtocolException , Error)
{
	__BEGIN_TRY

#ifndef __GAME_CLIENT__
	CGDownSkillHandler::execute(this , pPlayer);
#endif

	__END_CATCH
}

#ifdef __DEBUG_OUTPUT__
std::string CGDownSkill::toString () const
       throw ()
{
	__BEGIN_TRY
		
	StringStream msg;
	msg << "CGDownSkill("
	    << "SkillType:"  << (int)m_SkillType << ","
	    << ")";
	
	return msg.toString();

	__END_CATCH
}
#endif