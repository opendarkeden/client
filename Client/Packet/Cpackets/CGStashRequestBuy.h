////////////////////////////////////////////////////////////////////////////////
// 
// Filename    : CGStashRequestBuy.h 
// Written By  : 김성민
// Description : 
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef __CG_STASH_REQUEST_BUY_H__
#define __CG_STASH_REQUEST_BUY_H__

#include "Packet.h"
#include "PacketFactory.h"

////////////////////////////////////////////////////////////////////////////////
//
// class CGStashRequestBuy;
//
////////////////////////////////////////////////////////////////////////////////

class CGStashRequestBuy : public Packet 
{
public:
	void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
	void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );
	PacketID_t getPacketID () const throw () { return PACKET_CG_STASH_REQUEST_BUY; }
	PacketSize_t getPacketSize () const throw () { return 0; }

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName () const throw () { return "CGStashRequestBuy"; }
		std::string toString () const throw ();
	#endif
	
};


////////////////////////////////////////////////////////////////////////////////
//
// class CGStashRequestBuyFactory;
//
// Factory for CGStashRequestBuy
//
////////////////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CGStashRequestBuyFactory : public PacketFactory 
{
public:
	Packet * createPacket () throw () { return new CGStashRequestBuy(); }
	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName () const throw () { return "CGStashRequestBuy"; }
	#endif
	PacketID_t getPacketID () const throw () { return Packet::PACKET_CG_STASH_REQUEST_BUY; }
	PacketSize_t getPacketMaxSize () const throw () { return 0; }

};
#endif

////////////////////////////////////////////////////////////////////////////////
//
// class CGStashRequestBuyHandler;
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __GAME_CLIENT__
	class CGStashRequestBuyHandler 
	{
	public :
		static void execute ( CGStashRequestBuy * pPacket , Player * player ) throw ( ProtocolException , Error );
	};
#endif

#endif
