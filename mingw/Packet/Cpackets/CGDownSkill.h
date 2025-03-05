//////////////////////////////////////////////////////////////////////////////
// Filename    : CGDownSkill.h 
// Written By  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __CG_DOWN_SKILL_H__
#define __CG_DOWN_SKILL_H__

#include "Types.h"
#include "Exception.h"
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////////////
// class CGDownSkill;
//////////////////////////////////////////////////////////////////////////////

class CGDownSkill : public Packet 
{
public:
	void read(SocketInputStream & iStream) throw(ProtocolException, Error);
	void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);
	PacketID_t getPacketID() const throw() { return PACKET_CG_DOWN_SKILL; }
	PacketSize_t getPacketSize() const throw() { return szSkillType; }
#ifdef __DEBUG_OUTPUT__
	std::string getPacketName() const throw() { return "CGDownSkill"; }
	std::string toString() const throw();
#endif

public:
	SkillType_t getSkillType() const throw()  { return m_SkillType; }
	void setSkillType(SkillType_t SkillType) throw() { m_SkillType = SkillType; }

private:
	SkillType_t       m_SkillType;  // 기술의 종류
};

//////////////////////////////////////////////////////////////////////
// class CGDownSkillFactory;
//////////////////////////////////////////////////////////////////////

class CGDownSkillFactory : public PacketFactory 
{
public:
	Packet* createPacket() throw() { return new CGDownSkill(); }
#ifdef __DEBUG_OUTPUT__
	std::string getPacketName() const throw() { return "CGDownSkill"; }
#endif
	PacketID_t getPacketID() const throw() { return Packet::PACKET_CG_DOWN_SKILL; }
	PacketSize_t getPacketMaxSize() const throw() { return szSkillType; }
};


#ifndef __GAME_CLIENT__
//////////////////////////////////////////////////////////////////////
// class CGDownSkillHandler;
//////////////////////////////////////////////////////////////////////

class CGDownSkillHandler 
{
public:
	static void execute(CGDownSkill* pCGDownSkill, Player* pPlayer) throw(ProtocolException, Error);
};
#endif


#endif
