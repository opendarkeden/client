//////////////////////////////////////////////////////////////////////
// 
// Filename    : CLGetServerList.h 
// Written By  : reiot@ewestsoft.com
// Description : 
// 
//////////////////////////////////////////////////////////////////////

#ifndef __CL_GET_SERVER_LIST_H__
#define __CL_GET_SERVER_LIST_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////
//
// class CLGetServerList;
//
//////////////////////////////////////////////////////////////////////

class CLGetServerList : public Packet {

public :
	
    // �Է½�Ʈ��(����)���κ��� ����Ÿ�� �о ��Ŷ�� �ʱ�ȭ�Ѵ�.
    void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
		    
    // ��½�Ʈ��(����)���� ��Ŷ�� ���̳ʸ� �̹����� ������.
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );

	// execute packet's handler
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );

	// get packet id
	PacketID_t getPacketID () const throw () { return PACKET_CL_GET_SERVER_LIST; }
	
	// get packet's body size
	PacketSize_t getPacketSize () const throw () { return 0; }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "CLGetServerList"; }

		// get packet's debug std::string
		std::string toString () const throw () { return "CLGetServerList"; }
	#endif

private :

};


//////////////////////////////////////////////////////////////////////
//
// class CLGetServerListFactory;
//
// Factory for CLGetServerList
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CLGetServerListFactory : public PacketFactory {

public :
	
	// create packet
	Packet * createPacket () throw () { return new CLGetServerList(); }

	// get packet name
	std::string getPacketName () const throw () { return "CLGetServerList"; }
	
	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_CL_GET_SERVER_LIST; }

	// get packet's max body size
	PacketSize_t getPacketMaxSize () const throw () { return 0; }

};

#endif


//////////////////////////////////////////////////////////////////////
//
// class CLGetServerListHandler;
//
//////////////////////////////////////////////////////////////////////

#ifndef __GAME_CLIENT__
	class CLGetServerListHandler {

	public :

		// execute packet's handler
		static void execute ( CLGetServerList * pPacket , Player * player ) throw ( ProtocolException , Error );

	};
#endif

#endif