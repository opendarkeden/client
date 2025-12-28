//////////////////////////////////////////////////////////////////////////////
// Filename    : CGPartyLeave.h 
// Written By  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __CG_PARTY_LEAVE_H__
#define __CG_PARTY_LEAVE_H__

#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////////////
// class CGPartyLeave
//////////////////////////////////////////////////////////////////////////////

class CGPartyLeave : public Packet 
{
public:
	void read(SocketInputStream & iStream) throw(ProtocolException, Error);
	void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);
	PacketID_t getPacketID() const throw() { return PACKET_CG_PARTY_LEAVE; }
	PacketSize_t getPacketSize() const throw() { return szBYTE + m_TargetName.size(); }

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName() const throw() { return "CGPartyLeave"; }
		std::string toString() const throw();
	#endif
	
public:
	std::string getTargetName(void) const throw() { return m_TargetName; }
	void setTargetName(const std::string& name) { m_TargetName = name; }

private:
	std::string m_TargetName;
};


//////////////////////////////////////////////////////////////////////////////
// class CGPartyLeaveFactory;
//////////////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CGPartyLeaveFactory : public PacketFactory 
{
public:
	Packet* createPacket() throw() { return new CGPartyLeave(); }

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName() const throw() { return "CGPartyLeave"; }
	#endif

	PacketID_t getPacketID() const throw() { return Packet::PACKET_CG_PARTY_LEAVE; }
	PacketSize_t getPacketMaxSize() const throw() { return szBYTE + 10; }
};
#endif

//////////////////////////////////////////////////////////////////////////////
// class CGPartyLeaveHandler
//////////////////////////////////////////////////////////////////////////////
#ifndef __GAME_CLIENT__
	class CGPartyLeaveHandler 
	{
	public:
		static void execute(CGPartyLeave* pPacket, Player* player) throw(ProtocolException, Error);
	};
#endif

#endif
