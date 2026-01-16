//////////////////////////////////////////////////////////////////////////////
// Filename    : GCSkillFailed1.cc 
// Written By  : elca@ewestsoft.com
// Description : Skill이 실패 했을때 날려주는 패킷의 멤버 정의
//////////////////////////////////////////////////////////////////////////////

#include "Client_PCH.h"
#include "GCSkillFailed1.h"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

GCSkillFailed1::GCSkillFailed1 () 
     throw ()
{
	__BEGIN_TRY
	__END_CATCH
}
	
GCSkillFailed1::~GCSkillFailed1 () 
    throw ()
{
	__BEGIN_TRY
	__END_CATCH
}

void GCSkillFailed1::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	iStream.read(m_SkillType);
	iStream.read(m_Grade);
	ModifyInfo::read(iStream);
	
	__END_CATCH
}
		    
void GCSkillFailed1::write ( SocketOutputStream & oStream ) const 
     throw ( ProtocolException , Error )
{
	__BEGIN_TRY

	oStream.write(m_SkillType);
	oStream.write(m_Grade);
	ModifyInfo::write( oStream);

	__END_CATCH
}

void GCSkillFailed1::execute ( Player * pPlayer ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	GCSkillFailed1Handler::execute( this , pPlayer );
		
	__END_CATCH
}

#ifdef __DEBUG_OUTPUT__
std::string GCSkillFailed1::toString () const
       throw ()
{
	__BEGIN_TRY

	StringStream msg;
	msg << "GCSkillFailed1(SkillType:" << (int)m_SkillType << " )" ;
	msg << "GRADE : " <<(int)m_Grade;
	msg << ModifyInfo::toString();	
	return msg.toString();

	__END_CATCH
}
#endif