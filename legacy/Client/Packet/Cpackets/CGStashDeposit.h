////////////////////////////////////////////////////////////////////////////////
// Filename    : CGStashDeposit.h 
// Written By  : ±è¼º¹Î
// Description : 
////////////////////////////////////////////////////////////////////////////////

#ifndef __CG_STASH_DEPOSIT_H__
#define __CG_STASH_DEPOSIT_H__

#include "Packet.h"
#include "PacketFactory.h"

////////////////////////////////////////////////////////////////////////////////
//
// class CGStashDeposit;
//
////////////////////////////////////////////////////////////////////////////////

class CGStashDeposit : public Packet 
{
public :
    void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );
	PacketID_t getPacketID () const throw () { return PACKET_CG_STASH_DEPOSIT; }
	PacketSize_t getPacketSize () const throw () { return szGold; }

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName () const throw () { return "CGStashDeposit"; }
		std::string toString () const throw ();
	#endif
	
public :
	Gold_t getAmount(void) const throw() { return m_Amount;}
	void setAmount(Gold_t amount) throw() { m_Amount = amount;}

private :
	Gold_t m_Amount;
};


////////////////////////////////////////////////////////////////////////////////
//
// class CGStashDepositFactory;
//
////////////////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CGStashDepositFactory : public PacketFactory {
public :
	Packet * createPacket () throw () { return new CGStashDeposit(); }

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName () const throw () { return "CGStashDeposit"; }
	#endif

	PacketID_t getPacketID () const throw () { return Packet::PACKET_CG_STASH_DEPOSIT; }
	PacketSize_t getPacketMaxSize () const throw () { return szGold; }

};

#endif

////////////////////////////////////////////////////////////////////////////////
//
// class CGStashDepositHandler;
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __GAME_CLIENT__
	class CGStashDepositHandler {
		
	public :

		// execute packet's handler
		static void execute ( CGStashDeposit * pPacket , Player * player ) throw ( ProtocolException , Error );
	};
#endif
#endif
