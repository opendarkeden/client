//////////////////////////////////////////////////////////////////////////////
// Filename    : GCSelectQuestID.h 
// Written By  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////

#pragma warning(disable:4786)

#ifndef __GC_SELECT_QUEST_ID_H__
#define __GC_SELECT_QUEST_ID_H__

#include <list>
#include "Packet.h"
#include "PacketFactory.h"

class PlayerCreature;
const BYTE maxQuestNum = 255;

//////////////////////////////////////////////////////////////////////////////
// class GCSelectQuestID;
//////////////////////////////////////////////////////////////////////////////

class GCSelectQuestID : public Packet
{
public:
#ifdef __GAME_SERVER__
	GCSelectQuestID(PlayerCreature* pPC) throw();
	GCSelectQuestID() throw() { }
	virtual ~GCSelectQuestID() throw();
#endif

public:
	void read(SocketInputStream & iStream) throw(ProtocolException, Error);
	void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);
	PacketID_t getPacketID() const throw() { return PACKET_GC_SELECT_QUEST_ID; }
	PacketSize_t getPacketSize() const throw();
	
#ifdef __DEBUG_OUTPUT__
	std::string getPacketName() const throw() { return "GCSelectQuestID"; }
	std::string toString() const throw();
#endif

public:
	bool		empty() const { return m_QuestIDList.empty(); }
	QuestID_t	popQuestID() { QuestID_t qID = m_QuestIDList.front(); m_QuestIDList.pop_front(); return qID; }
	
private:
	std::list<QuestID_t>	m_QuestIDList;
};

//////////////////////////////////////////////////////////////////////////////
// class GCSelectQuestIDFactory;
//////////////////////////////////////////////////////////////////////////////

class GCSelectQuestIDFactory : public PacketFactory 
{
public:
	Packet* createPacket() throw() { return new GCSelectQuestID(); }
#ifdef __DEBUG_OUTPUT__
	std::string getPacketName() const throw() { return "GCSelectQuestID"; }
#endif
	PacketID_t getPacketID() const throw() { return Packet::PACKET_GC_SELECT_QUEST_ID; }
	PacketSize_t getPacketMaxSize() const throw()
	{
		return szBYTE
			 + szQuestID * maxQuestNum;
	}
};

//////////////////////////////////////////////////////////////////////////////
// class GCSelectQuestIDHandler;
//////////////////////////////////////////////////////////////////////////////

class GCSelectQuestIDHandler 
{
public:
	static void execute(GCSelectQuestID* pPacket, Player* pPlayer) throw(ProtocolException, Error);
};

#endif
