//////////////////////////////////////////////////////////////////////////////
// Filename    : CGPartyInvite.h 
// Written By  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __CG_PARTY_INVITE_H__
#define __CG_PARTY_INVITE_H__

#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////////////
// 파티 가입 관련 코드
//////////////////////////////////////////////////////////////////////////////
enum
{
	CG_PARTY_INVITE_REQUEST = 0,
	CG_PARTY_INVITE_CANCEL,
	CG_PARTY_INVITE_ACCEPT,
	CG_PARTY_INVITE_REJECT,
	CG_PARTY_INVITE_BUSY,

	CG_PARTY_INVITE_MAX
};

//////////////////////////////////////////////////////////////////////////////
// class CGPartyInvite
//////////////////////////////////////////////////////////////////////////////

class CGPartyInvite : public Packet 
{
public:
	void read(SocketInputStream & iStream) throw(ProtocolException, Error);
	void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);
	PacketID_t getPacketID() const throw() { return PACKET_CG_PARTY_INVITE; }
	PacketSize_t getPacketSize() const throw() { return szObjectID + szBYTE; }

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName() const throw() { return "CGPartyInvite"; }
		std::string toString() const throw();
	#endif
	
public:
	ObjectID_t getTargetObjectID() const throw() { return m_TargetObjectID; }
	void setTargetObjectID(ObjectID_t id) throw() { m_TargetObjectID = id; }

	BYTE getCode(void) const throw() { return m_Code; }
	void setCode(BYTE code) throw() { m_Code = code; }

private:
	ObjectID_t m_TargetObjectID; // 상대방의 OID
	BYTE       m_Code;           // 코드

};


//////////////////////////////////////////////////////////////////////////////
// class CGPartyInviteFactory;
//////////////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CGPartyInviteFactory : public PacketFactory 
{
public:
	Packet* createPacket() throw() { return new CGPartyInvite(); }

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName() const throw() { return "CGPartyInvite"; }
	#endif

	PacketID_t getPacketID() const throw() { return Packet::PACKET_CG_PARTY_INVITE; }
	PacketSize_t getPacketMaxSize() const throw() { return szObjectID + szBYTE; }
};

#endif

//////////////////////////////////////////////////////////////////////////////
// class CGPartyInviteHandler
//////////////////////////////////////////////////////////////////////////////
#ifndef __GAME_CLIENT__
	class CGPartyInviteHandler 
	{
	public:
		static void execute(CGPartyInvite* pPacket, Player* player) throw(ProtocolException, Error);
		static void executeError(CGPartyInvite* pPacket, Player* player, BYTE ErrorCode) throw(ProtocolException, Error);
	};
#endif

#endif
