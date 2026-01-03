////////////////////////////////////////////////////////////////////////////////
// 
// Filename    : CGStashToMouse.h 
// Written By  : 김성민 
// Description : 
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef __CG_STASH_TO_MOUSE_H__
#define __CG_STASH_TO_MOUSE_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"

////////////////////////////////////////////////////////////////////////////////
//
// class CGStashToMouse;
//
////////////////////////////////////////////////////////////////////////////////

class CGStashToMouse : public Packet 
{
public :
    void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );
	PacketID_t getPacketID () const throw () { return PACKET_CG_STASH_TO_MOUSE; }
	PacketSize_t getPacketSize () const throw () { return szObjectID + szBYTE*2; }

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName () const throw () { return "CGStashToMouse"; }
		std::string toString () const throw ();
	#endif
	
public :
	ObjectID_t getObjectID() throw () { return m_ObjectID; }
	void       setObjectID( ObjectID_t ObjectID ) throw() { m_ObjectID = ObjectID; }
	BYTE       getRack(void) throw() { return m_Rack;}
	void       setRack(BYTE rack) throw() { m_Rack = rack;}
	BYTE       getIndex(void) throw() { return m_Index;}
	void       setIndex(BYTE index) throw() { m_Index = index;}

private:
	ObjectID_t m_ObjectID;
	BYTE       m_Rack;
	BYTE       m_Index;

};


////////////////////////////////////////////////////////////////////////////////
//
// class CGStashToMouseFactory;
//
////////////////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CGStashToMouseFactory : public PacketFactory 
{
public :
	Packet * createPacket () throw () { return new CGStashToMouse(); }

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName () const throw () { return "CGStashToMouse"; }
	#endif

	PacketID_t getPacketID () const throw () { return Packet::PACKET_CG_STASH_TO_MOUSE; }
	PacketSize_t getPacketMaxSize () const throw () { return szObjectID + szBYTE*2; }

};
#endif

////////////////////////////////////////////////////////////////////////////////
//
// class CGStashToMouseHandler;
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __GAME_CLIENT__
	class CGStashToMouseHandler {
		
	public :

		// execute packet's handler
		static void execute ( CGStashToMouse * pPacket , Player * player ) throw ( ProtocolException , Error );
		static void executeSlayer ( CGStashToMouse * pPacket , Player * player ) throw ( ProtocolException , Error );
		static void executeVampire ( CGStashToMouse * pPacket , Player * player ) throw ( ProtocolException , Error );
	};
#endif

#endif
