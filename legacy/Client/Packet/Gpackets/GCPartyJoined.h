//////////////////////////////////////////////////////////////////////////////
// Filename    : GCPartyJoined.h 
// Written By  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __GC_PARTY_JOINED_H__
#define __GC_PARTY_JOINED_H__

#include "Packet.h"
#include "PacketFactory.h"
#include <string>

typedef struct
{
	std::string name;
	BYTE   sex;
	BYTE   hair_style;
	IP_t   ip;
} PARTY_MEMBER_INFO;

// 구조체 맥스 크기(14) + 이름 길이 (1)
const uint PARTY_MEMBER_INFO_MAX_SIZE = 17; 

//////////////////////////////////////////////////////////////////////////////
// class GCPartyJoined;
//////////////////////////////////////////////////////////////////////////////

class GCPartyJoined : public Packet 
{
public:
	GCPartyJoined();
	~GCPartyJoined();

public:
	void read(SocketInputStream & iStream) throw(ProtocolException, Error);
	void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);
	PacketID_t getPacketID() const throw() { return PACKET_GC_PARTY_JOINED; }
	PacketSize_t getPacketSize() const throw();

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName() const throw() { return "GCPartyJoined"; }
		std::string toString() const throw();
	#endif

public:
	BYTE getMemberInfoCount(void) { return m_MemberCount; }

	void addMemberInfo(PARTY_MEMBER_INFO* pInfo) throw();
	PARTY_MEMBER_INFO* popMemberInfo(void) throw();

	void clear(void) throw();

private:
	BYTE                     m_MemberCount;
	std::list<PARTY_MEMBER_INFO*> m_MemberInfoList;
};


//////////////////////////////////////////////////////////////////////////////
// class GCPartyJoinedFactory;
//////////////////////////////////////////////////////////////////////////////

class GCPartyJoinedFactory : public PacketFactory 
{
public:
	Packet* createPacket() throw() { return new GCPartyJoined(); }

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName() const throw() { return "GCPartyJoined"; }
	#endif

	PacketID_t getPacketID() const throw() { return Packet::PACKET_GC_PARTY_JOINED; }
	PacketSize_t getPacketMaxSize() const throw() { return szBYTE + PARTY_MEMBER_INFO_MAX_SIZE * 6; }
};


//////////////////////////////////////////////////////////////////////////////
// class GCPartyJoinedHandler;
//////////////////////////////////////////////////////////////////////////////

class GCPartyJoinedHandler 
{
public:
	static void execute(GCPartyJoined* pPacket, Player* pPlayer) throw(ProtocolException, Error);
};

#endif
