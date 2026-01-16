//////////////////////////////////////////////////////////////////////////////
// Filename    : GCSystemAvailabilities.h 
// Written By  : elca@ewestsoft.com
// Description : 
// 기술이 성공했을때 보내는 패킷을 위한 클래스 정의
//////////////////////////////////////////////////////////////////////////////

#ifndef __GC_SYSTEM_AVAILABILITIES_H__
#define __GC_SYSTEM_AVAILABILITIES_H__

#include "Types.h"
#include "Exception.h"
#include "Packet.h"
#include "PacketFactory.h"

enum SystemKind
{
	SYSTEM_PARTY = 0,
	SYSTEM_GAMBLE,
	SYSTEM_RANK_BONUS,
	SYSTEM_ENCHANT,
	SYSTEM_GUILD,
	SYSTEM_MASTER_LAIR,
	SYSTEM_PK_ZONE,
	SYSTEM_MARKET,
	SYSTEM_GRAND_MASTER_EFFECT,
	SYSTEM_COUPLE,
	SYSTEM_HOLY_LAND_WAR,
	SYSTEM_GUILD_WAR,
	SYSTEM_RACE_WAR,
	SYSTEM_FLAG_WAR,

	SYSTEM_MAX
};

//////////////////////////////////////////////////////////////////////////////
// class GCSystemAvailabilities;
// 게임서버에서 클라이언트로 자신의 기술이 성공을 알려주기 위한 클래스
//////////////////////////////////////////////////////////////////////////////

class GCSystemAvailabilities : public Packet 
{
public:
	GCSystemAvailabilities() throw();
	~GCSystemAvailabilities() throw();
	
public:
    void read(SocketInputStream & iStream) throw(ProtocolException, Error);
    void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);
	PacketID_t getPacketID() const throw() { return PACKET_GC_SYSTEM_AVAILABILITIES; }
	PacketSize_t getPacketSize() const throw() { return szDWORD + szBYTE * 2; }

#ifdef __DEBUG_OUTPUT__
	std::string getPacketName() const throw() { return "GCSystemAvailabilities"; }
	std::string toString() const throw();
#endif

public:
	DWORD	getFlag() const { return m_Flag; }
	void	setFlag(DWORD flag ) { m_Flag = flag; }
	BYTE	getDegree() const { return m_Degree; }
	BYTE	getSkillLimit() const { return m_SkillLimit; }

private:
	DWORD	m_Flag;
	BYTE	m_Degree;
	BYTE	m_SkillLimit;
};


//////////////////////////////////////////////////////////////////////////////
// class GCSystemAvailabilitiesFactory;
//////////////////////////////////////////////////////////////////////////////

class GCSystemAvailabilitiesFactory : public PacketFactory 
{
public :
	GCSystemAvailabilitiesFactory() throw() {}
	virtual ~GCSystemAvailabilitiesFactory() throw() {}
	
public:
	Packet* createPacket() throw() { return new GCSystemAvailabilities(); }
#ifdef __DEBUG_OUTPUT__
	std::string getPacketName() const throw() { return "GCSystemAvailabilities"; }
#endif
	PacketID_t getPacketID() const throw() { return Packet::PACKET_GC_SYSTEM_AVAILABILITIES; }
	PacketSize_t getPacketMaxSize() const throw() { return szDWORD + szBYTE * 2; }
};

//////////////////////////////////////////////////////////////////////////////
// class GCSystemAvailabilitiesHandler;
//////////////////////////////////////////////////////////////////////////////

class GCSystemAvailabilitiesHandler 
{
public:
	static void execute(GCSystemAvailabilities* pGCSystemAvailabilities, Player* pPlayer) throw ( ProtocolException , Error );

};

#endif
