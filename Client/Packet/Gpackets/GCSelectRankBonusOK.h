//////////////////////////////////////////////////////////////////////
// 
// Filename    :  GCSelectRankBonusOK.h 
// Written By  :  elca@ewestsoft.com
// Description :  Å
//                
// 
//////////////////////////////////////////////////////////////////////

#ifndef __GC_SELECT_RANK_BONUS_OK_H__
#define __GC_SELECT_RANK_BONUS_OK_H__

// include files
#include "Types.h"
#include "Exception.h"
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////
//
// class  GCSelectRankBonusOK;
//
//////////////////////////////////////////////////////////////////////

class GCSelectRankBonusOK : public Packet {

public :
	
	// constructor
	GCSelectRankBonusOK() throw();
	
	// destructor
	~GCSelectRankBonusOK() throw();

	
public :
	
	
    // ÀÔ·Â½ºÆ®¸²(¹öÆÛ)À¸·ÎºÎÅÍ µ¥ÀÌÅ¸¸¦ ÀÐ¾î¼­ ÆÐÅ¶À» ÃÊ±âÈ­ÇÑ´Ù.
    void read(SocketInputStream & iStream) throw(ProtocolException, Error);
		    
    // Ãâ·Â½ºÆ®¸²(¹öÆÛ)À¸·Î ÆÐÅ¶ÀÇ ¹ÙÀÌ³Ê¸® ÀÌ¹ÌÁö¸¦ º¸³½´Ù.
    void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);

	// execute packet's handler
	void execute(Player* pPlayer) throw(ProtocolException, Error);

	// get packet id
	PacketID_t getPacketID() const throw() { return PACKET_GC_SELECT_RANK_BONUS_OK; }
	
	// get packet size
	PacketSize_t getPacketSize() const throw() { return szDWORD; }
	
#ifdef __DEBUG_OUTPUT__
	// get packet's name
	std::string getPacketName() const throw() { return "GCSelectRankBonusOK"; }
	
	// get packet's debug std::string
	std::string toString() const throw();
#endif
	
	// get/set m_RankBonusType
	DWORD getRankBonusType() const throw() { return m_RankBonusType; }
	void setRankBonusType(DWORD rankBonusType) throw() { m_RankBonusType = rankBonusType; }

private : 

	// RankBonusType
	DWORD m_RankBonusType;
};


//////////////////////////////////////////////////////////////////////
//
// class  GCSelectRankBonusOKFactory;
//
// Factory for  GCSelectRankBonusOK
//
//////////////////////////////////////////////////////////////////////

class  GCSelectRankBonusOKFactory : public PacketFactory {

public :
	
	// constructor
	 GCSelectRankBonusOKFactory() throw() {}
	
	// destructor
	virtual ~GCSelectRankBonusOKFactory() throw() {}

	
public :
	
	// create packet
	Packet* createPacket() throw() { return new GCSelectRankBonusOK(); }

	// get packet name
	std::string getPacketName() const throw() { return "GCSelectRankBonusOK"; }
	
	// get packet id
	PacketID_t getPacketID() const throw() { return Packet::PACKET_GC_SELECT_RANK_BONUS_OK; }

	// get Packet Max Size
	PacketSize_t getPacketMaxSize() const throw() { return szDWORD; }

};


//////////////////////////////////////////////////////////////////////
//
// class  GCSelectRankBonusOKHandler;
//
//////////////////////////////////////////////////////////////////////

class  GCSelectRankBonusOKHandler {

public :

	// execute packet's handler
	static void execute( GCSelectRankBonusOK* pGCSelectRankBonusOK, Player* pPlayer) throw ( ProtocolException , Error );

};

#endif
