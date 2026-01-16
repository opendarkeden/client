//////////////////////////////////////////////////////////////////////////////
// Filename    : GCOtherModifyInfo.h 
// Written By  : excel96
// Description :
//////////////////////////////////////////////////////////////////////////////

#ifndef __GC_OTHER_MODIFY_INFO_H__
#define __GC_OTHER_MODIFY_INFO_H__

#include "ModifyInfo.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////////////
// class GCOtherModifyInfo;
//////////////////////////////////////////////////////////////////////////////

class GCOtherModifyInfo : public ModifyInfo 
{
public:
    void read(SocketInputStream & iStream) throw(ProtocolException, Error);
    void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);
	PacketID_t getPacketID() const throw() { return PACKET_GC_OTHER_MODIFY_INFO; }
	PacketSize_t getPacketSize() const throw() { return szObjectID + ModifyInfo::getPacketSize(); }

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName() const throw() { return "GCOtherModifyInfo"; }
		std::string toString() const throw();
	#endif

public:
	ObjectID_t getObjectID() const throw() { return m_ObjectID; }
	void setObjectID(ObjectID_t ObjectID) throw() { m_ObjectID = ObjectID; }

private:
	ObjectID_t m_ObjectID;
};


//////////////////////////////////////////////////////////////////////////////
// class GCOtherModifyInfoFactory;
//////////////////////////////////////////////////////////////////////////////

class GCOtherModifyInfoFactory : public PacketFactory 
{
public:
	Packet* createPacket() throw() { return new GCOtherModifyInfo(); }

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName() const throw() { return "GCOtherModifyInfo"; }
	#endif

	PacketID_t getPacketID() const throw() { return Packet::PACKET_GC_OTHER_MODIFY_INFO; }
	PacketSize_t getPacketMaxSize() const throw() { return szObjectID + ModifyInfo::getPacketMaxSize(); }
};


//////////////////////////////////////////////////////////////////////////////
// class GCOtherModifyInfoHandler;
//////////////////////////////////////////////////////////////////////////////

class GCOtherModifyInfoHandler 
{
public:
	static void execute(GCOtherModifyInfo* pGCOtherModifyInfo, Player* pPlayer) throw ( ProtocolException , Error );
};

#endif
