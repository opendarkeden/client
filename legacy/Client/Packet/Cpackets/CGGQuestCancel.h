//////////////////////////////////////////////////////////////////////////////
// Filename    : CGGQuestCancel.h 
// Written By  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __CG_GQUEST_CANCEL_H__
#define __CG_GQUEST_CANCEL_H__

#include "Types.h"
#include "Exception.h"
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////////////
// class CGGQuestCancel;
//////////////////////////////////////////////////////////////////////////////

class CGGQuestCancel : public Packet 
{
public:
	void read(SocketInputStream & iStream) throw(ProtocolException, Error);
	void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);
	PacketID_t getPacketID() const throw() { return PACKET_CG_GQUEST_CANCEL; }
	PacketSize_t getPacketSize() const throw() { return szDWORD; }
#ifdef __DEBUG_OUTPUT__
	string getPacketName() const throw() { return "CGGQuestCancel"; }
	string toString() const throw();
#endif
public:
	DWORD getQuestID() const throw()  { return m_QuestID; }
	void setQuestID(DWORD QuestID) throw() { m_QuestID = QuestID; }

private:
	DWORD       m_QuestID;  // 기술의 종류
};

//////////////////////////////////////////////////////////////////////
// class CGGQuestCancelFactory;
//////////////////////////////////////////////////////////////////////

class CGGQuestCancelFactory : public PacketFactory 
{
public:
	Packet* createPacket() throw() { return new CGGQuestCancel(); }
#ifdef __DEBUG_OUTPUT__
	string getPacketName() const throw() { return "CGGQuestCancel"; }
#endif
	PacketID_t getPacketID() const throw() { return Packet::PACKET_CG_GQUEST_CANCEL; }
	PacketSize_t getPacketMaxSize() const throw() { return szDWORD; }
};


//////////////////////////////////////////////////////////////////////
// class CGGQuestCancelHandler;
//////////////////////////////////////////////////////////////////////
#ifndef __GAME_CLIENT__
class CGGQuestCancelHandler 
{
public:
	static void execute(CGGQuestCancel* pCGGQuestCancel, Player* pPlayer) throw(ProtocolException, Error);
};
#endif
#endif
