//////////////////////////////////////////////////////////////////////////////
// Filename    : GCNicknameVerify.h 
// Written By  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __GC_NICKNAME_VERIFY_H__
#define __GC_NICKNAME_VERIFY_H__

#include "Packet.h"
#include "PacketFactory.h"

enum 
{
	NICKNAME_MODIFY_FAIL_NO_ITEM,
	NICKNAME_MODIFY_OK,

	NICKNAME_SELECT_FAIL_NO_SUCH,
	NICKNAME_SELECT_FAIL_FORCED,
	NICKNAME_SELECT_OK,

	NICKNAME_VERIFY_MAX
};

//////////////////////////////////////////////////////////////////////////////
// class GCNicknameVerify
//////////////////////////////////////////////////////////////////////////////

class GCNicknameVerify : public Packet 
{

public:
	GCNicknameVerify() throw() { m_Code = NICKNAME_VERIFY_MAX; m_Parameter = 0;}
	virtual ~GCNicknameVerify() throw() {}

public:
	void read(SocketInputStream & iStream) throw(ProtocolException, Error);
	void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);

	PacketID_t getPacketID() const throw() { return PACKET_GC_NICKNAME_VERIFY; }
	PacketSize_t getPacketSize() const throw();
#ifdef __DEBUG_OUTPUT__
	string getPacketName() const throw() { return "GCNicknameVerify"; }
	string toString() const throw();
#endif
public:
	BYTE getCode(void) const throw() { return m_Code;}
	void setCode(BYTE code) throw() { m_Code = code;}

	uint getParameter(void) const throw() { return m_Parameter; }
	void setParameter(uint parameter) throw() { m_Parameter = parameter; }

private: 
	BYTE m_Code;
	uint m_Parameter;
};


//////////////////////////////////////////////////////////////////////////////
// class GCNicknameVerifyFactory;
//////////////////////////////////////////////////////////////////////////////

class GCNicknameVerifyFactory : public PacketFactory 
{
public:
	Packet* createPacket() throw() { return new GCNicknameVerify(); }
#ifdef __DEBUG_OUTPUT__
	string getPacketName() const throw() { return "GCNicknameVerify"; }
#endif
	PacketID_t getPacketID() const throw() { return Packet::PACKET_GC_NICKNAME_VERIFY; }
	PacketSize_t getPacketMaxSize() const throw() { return szBYTE + szuint; }
};


//////////////////////////////////////////////////////////////////////////////
// class GCNicknameVerifyHandler
//////////////////////////////////////////////////////////////////////////////

class GCNicknameVerifyHandler 
{
public:
	static void execute( GCNicknameVerify* pPacket, Player* pPlayer) throw(ProtocolException, Error);
};

#endif
