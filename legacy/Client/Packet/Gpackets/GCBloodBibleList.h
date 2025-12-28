//////////////////////////////////////////////////////////////////////////////
// Filename    : GCBloodBibleList.h 
// Written By  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __GC_BLOOD_BIBLE_LIST_H__
#define __GC_BLOOD_BIBLE_LIST_H__

#include <vector>
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////////////
// class GCBloodBibleList;
//////////////////////////////////////////////////////////////////////////////

class GCBloodBibleList : public Packet
{
public:
	GCBloodBibleList() throw() { }
	virtual ~GCBloodBibleList() throw();

public:
	void read(SocketInputStream & iStream) throw(ProtocolException, Error);
	void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);
	PacketID_t getPacketID() const throw() { return PACKET_GC_BLOOD_BIBLE_LIST; }
	PacketSize_t getPacketSize() const throw();
#ifdef __DEBUG_OUTPUT__
	string getPacketName() const throw() { return "GCBloodBibleList"; }
	string toString() const throw();
#endif
public:
	std::vector<ItemType_t>&	getList() { return m_BloodBibleList; }
	const std::vector<ItemType_t>&	getList() const { return m_BloodBibleList; }

private:
	std::vector<ItemType_t>	m_BloodBibleList;
};

//////////////////////////////////////////////////////////////////////////////
// class GCBloodBibleListFactory;
//////////////////////////////////////////////////////////////////////////////

class GCBloodBibleListFactory : public PacketFactory 
{
public:
	Packet* createPacket() throw() { return new GCBloodBibleList(); }
#ifdef __DEBUG_OUTPUT__
	std::string getPacketName() const throw() { return "GCBloodBibleList"; }
#endif
	PacketID_t getPacketID() const throw() { return Packet::PACKET_GC_BLOOD_BIBLE_LIST; }
	PacketSize_t getPacketMaxSize() const throw()
	{
		return szBYTE
			 + szItemType * 12;
	}
};

//////////////////////////////////////////////////////////////////////////////
// class GCBloodBibleListHandler;
//////////////////////////////////////////////////////////////////////////////

class GCBloodBibleListHandler 
{
public:
	static void execute(GCBloodBibleList* pPacket, Player* pPlayer) throw(ProtocolException, Error);
};

#endif
