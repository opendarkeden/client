
//////////////////////////////////////////////////////////////////////////////
// Filename    : CGPetGamble.h 
// Written By  : reiot@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __CG_PET_GAMBLE_H__
#define __CG_PET_GAMBLE_H__

#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////////////
// class CGPetGamble;
//////////////////////////////////////////////////////////////////////////////

class CGPetGamble : public Packet 
{
public:
	CGPetGamble() throw();
	~CGPetGamble() throw();

public:
    void read(SocketInputStream & iStream) throw(ProtocolException, Error);
    void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);
	PacketID_t getPacketID() const throw() { return PACKET_CG_PET_GAMBLE; }
	PacketSize_t getPacketSize() const throw() { return 0; }

#ifdef __DEBUG_OUTPUT__
	std::string getPacketName() const throw() { return "CGPetGamble"; }
	std::string toString() const throw();
#endif
	
public:
private :
};

//////////////////////////////////////////////////////////////////////////////
// class CGPetGambleFactory;
//////////////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CGPetGambleFactory : public PacketFactory 
{
public:
	Packet* createPacket() throw() { return new CGPetGamble(); }
	std::string getPacketName() const throw() { return "CGPetGamble"; }
	PacketID_t getPacketID() const throw() { return Packet::PACKET_CG_PET_GAMBLE; }
	PacketSize_t getPacketMaxSize() const throw() { return 0; }
};
#endif

//////////////////////////////////////////////////////////////////////////////
// class CGPetGambleHandler;
//////////////////////////////////////////////////////////////////////////////
#ifndef __GAME_CLIENT__
class CGPetGambleHandler 
{
public:
	static void execute(CGPetGamble* pPacket, Player* player) throw(ProtocolException, Error);
};
#endif

#endif
