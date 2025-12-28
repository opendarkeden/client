//--------------------------------------------------------------------------------
// 
// Filename    : GCSearchMotorcycleFail.h 
// Written By  : 김성민
// Description : 플레이어에게 서버 측의 상점 버전을 알려줄 때 쓰이는 패킷이다.
// 
//--------------------------------------------------------------------------------

#ifndef __GC_SEARCH_MOTORCYCLE_FAIL_H__
#define __GC_SEARCH_MOTORCYCLE_FAIL_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"


//--------------------------------------------------------------------------------
//
// class GCSearchMotorcycleFail;
//
//--------------------------------------------------------------------------------

class GCSearchMotorcycleFail : public Packet 
{
public :
	void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
	void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );
	PacketID_t getPacketID () const throw () { return PACKET_GC_SEARCH_MOTORCYCLE_FAIL; }
	PacketSize_t getPacketSize () const throw () { return 0; }
	
	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName () const throw () { return "GCSearchMotorcycleFail"; }
		std::string toString () const throw ();
	#endif
};


//////////////////////////////////////////////////////////////////////
//
// class GCSearchMotorcycleFailFactory;
//
// Factory for GCSearchMotorcycleFail
//
//////////////////////////////////////////////////////////////////////

class GCSearchMotorcycleFailFactory : public PacketFactory 
{

public :
	
	Packet * createPacket () throw () { return new GCSearchMotorcycleFail(); }
	
	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName () const throw () { return "GCSearchMotorcycleFail"; }
	#endif

	PacketID_t getPacketID () const throw () { return Packet::PACKET_GC_SEARCH_MOTORCYCLE_FAIL; }
	PacketSize_t getPacketMaxSize () const throw () { return 0; }

};


//////////////////////////////////////////////////////////////////////
//
// class GCSearchMotorcycleFailHandler;
//
//////////////////////////////////////////////////////////////////////

class GCSearchMotorcycleFailHandler 
{
	
public :
	
	// execute packet's handler
	static void execute ( GCSearchMotorcycleFail * pPacket , Player * pPlayer ) throw ( ProtocolException , Error );

};

#endif
