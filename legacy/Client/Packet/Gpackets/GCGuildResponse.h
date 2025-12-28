//////////////////////////////////////////////////////////////////////////////
// Filename    : GCGuildResponse.h 
// Written By  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __GC_GUILD_RESPONSE_H__
#define __GC_GUILD_RESPONSE_H__

#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////////////
// class GCGuildResponse
//////////////////////////////////////////////////////////////////////////////
enum UNION_ERROR
{
		OK = 0,
        ALREADY_IN_UNION,		
        ALREADY_OFFER_SOMETHING,
        TARGET_IS_NOT_MASTER,   // 상대방이 마스터가 아니라네;;
        NOT_IN_UNION,           // 연합에 가입된 상태가 아님
        MASTER_CANNOT_QUIT,     // 연합의 마스터 길드는 스스로 탈퇴불가(하위길드가 없을때만 가능-자동이래)
        NO_TARGET_UNION,        // 해당하는 연합이 없음
        NOT_YOUR_UNION,         // 소속된 연합이 아니다.
        SOURCE_IS_NOT_MASTER,   // 신청한 사람이 마스터가 아니다    
		YOU_HAVE_PENALTY,        // 강제로 탈퇴한 기록이 있다.
		NOT_ENOUGH_SLOT,			// 남아있는 슬롯이 없다.
		TOO_MANY_MEMBER,		// 멤버가 50명이상이라 가입할 수 없습니다.
};

class GCGuildResponse : public Packet 
{

public:
	GCGuildResponse() throw() { m_Code = 0; m_Parameter = 0;}
	virtual ~GCGuildResponse() throw() {}

public:
	void read(SocketInputStream & iStream) throw(ProtocolException, Error);
	void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);

	PacketID_t getPacketID() const throw() { return PACKET_GC_GUILD_RESPONSE; }
	PacketSize_t getPacketSize() const throw();
#ifdef __DEBUG_OUTPUT__
	string getPacketName() const throw() { return "GCGuildResponse"; }
	string toString() const throw();
#endif	
public:
	BYTE getCode(void) const throw() { return m_Code;}
	void setCode(WORD code) throw() { m_Code = code;}

	uint getParameter(void) const throw() { return m_Parameter; }
	void setParameter(uint parameter) throw() { m_Parameter = parameter; }

private: 
	WORD m_Code;
	uint m_Parameter;

};


//////////////////////////////////////////////////////////////////////////////
// class GCGuildResponseFactory;
//////////////////////////////////////////////////////////////////////////////

class GCGuildResponseFactory : public PacketFactory 
{
public:
	Packet* createPacket() throw() { return new GCGuildResponse(); }
	string getPacketName() const throw() { return "GCGuildResponse"; }
	PacketID_t getPacketID() const throw() { return Packet::PACKET_GC_GUILD_RESPONSE; }
	PacketSize_t getPacketMaxSize() const throw() { return szWORD + szuint; }
};


//////////////////////////////////////////////////////////////////////////////
// class GCGuildResponseHandler
//////////////////////////////////////////////////////////////////////////////

class GCGuildResponseHandler 
{
public:
	static void execute( GCGuildResponse* pPacket, Player* pPlayer) throw(ProtocolException, Error);
};

#endif
