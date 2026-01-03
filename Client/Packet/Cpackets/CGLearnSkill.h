//////////////////////////////////////////////////////////////////////////////
// Filename    : CGLearnSkill.h 
// Written By  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __CG_LEARN_SKILL_H__
#define __CG_LEARN_SKILL_H__

#include "Types.h"
#include "Exception.h"
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////////////
// class CGLearnSkill;
//////////////////////////////////////////////////////////////////////////////

class CGLearnSkill : public Packet 
{
public:
	void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
	void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );
	PacketID_t getPacketID () const throw () { return PACKET_CG_LEARN_SKILL; }
	PacketSize_t getPacketSize () const throw () { return szSkillType+szSkillDomainType; }

#ifdef __DEBUG_OUTPUT__
	std::string getPacketName () const throw () { return "CGLearnSkill"; }
	std::string toString () const throw ();
#endif

public:
	SkillType_t getSkillType() const throw()  { return m_SkillType; }
	void setSkillType( SkillType_t SkillType ) throw() { m_SkillType = SkillType; }

	SkillDomainType_t getSkillDomainType() const throw() { return m_DomainType;}
	void setSkillDomainType( SkillDomainType_t DomainType) throw() { m_DomainType = DomainType;}

private:
	SkillType_t       m_SkillType;  // 기술의 종류
	SkillDomainType_t m_DomainType; // 기술의 도메인
};

//////////////////////////////////////////////////////////////////////
// class CGLearnSkillFactory;
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CGLearnSkillFactory : public PacketFactory 
{
public:
	Packet * createPacket () throw () { return new CGLearnSkill(); }

	std::string getPacketName () const throw () { return "CGLearnSkill"; }
	PacketID_t getPacketID () const throw () { return Packet::PACKET_CG_LEARN_SKILL; }
	PacketSize_t getPacketMaxSize () const throw () { return szSkillType+szSkillDomainType; }
};
#endif


//////////////////////////////////////////////////////////////////////
// class CGLearnSkillHandler;
//////////////////////////////////////////////////////////////////////
#ifndef __GAME_CLIENT__
class CGLearnSkillHandler 
{
public:
	static void execute ( CGLearnSkill * pCGLearnSkill , Player * pPlayer ) throw ( ProtocolException, Error );
	static void executeSlayerSkill ( CGLearnSkill * pCGLearnSkill , Player * pPlayer ) throw ( ProtocolException, Error );
	static void executeVampireSkill ( CGLearnSkill * pCGLearnSkill , Player * pPlayer ) throw ( ProtocolException, Error );
};
#endif
#endif
