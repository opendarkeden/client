//////////////////////////////////////////////////////////////////////
// 
// Filename    : CLSelectPC.h 
// Written By  : reiot@ewestsoft.com
// Description : 
// 
//////////////////////////////////////////////////////////////////////

#ifndef __CL_SELECT_PC_H__
#define __CL_SELECT_PC_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"
#include "Player.h"

//////////////////////////////////////////////////////////////////////
//
// class CLSelectPC;
//
// 플레이할 PC 를 선택하는 패킷이다.
//
//////////////////////////////////////////////////////////////////////

class CLSelectPC : public Packet {

public :
	
    // 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
    void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
		    
    // 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );

	// execute packet's handler
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );

	// get packet id
	PacketID_t getPacketID () const throw () { return PACKET_CL_SELECT_PC; }
	
	// get packet's body size
	PacketSize_t getPacketSize () const throw () 
	{ 
		return szBYTE + m_PCName.size() 	// pc name
			+ szPCType; 					// pc type
	}

	#ifdef __DEBUG_OUTPUT__
		// get packet's name
		std::string getPacketName () const throw () { return "CLSelectPC"; }
		
		// get packet's debug std::string
		std::string toString () const throw ();
	#endif

public :

	// get/set creature's name
	std::string getPCName () const throw () { return m_PCName; }
	void setPCName ( std::string pcName ) throw () { m_PCName = pcName; }

	// get/set pc type
	PCType getPCType () const throw () { return m_PCType; }
	void setPCType ( PCType pcType ) throw () { m_PCType = pcType; }

private :
	
	// Name
	std::string m_PCName;

	// Slayer or Vampire?
	PCType m_PCType;

};


//////////////////////////////////////////////////////////////////////
//
// class CLSelectPCFactory;
//
// Factory for CLSelectPC
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CLSelectPCFactory : public PacketFactory {

public :
	
	// create packet
	Packet * createPacket () throw () { return new CLSelectPC(); }

	// get packet name
	std::string getPacketName () const throw () { return "CLSelectPC"; }
	
	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_CL_SELECT_PC; }

	// get packet's max body size
	PacketSize_t getPacketMaxSize () const throw ()
	{
		return szBYTE + 20		 	// name
			+ szPCType; 			// pc type
	}

};
#endif


//////////////////////////////////////////////////////////////////////
//
// class CLSelectPCHandler;
//
//////////////////////////////////////////////////////////////////////
#ifndef __GAME_CLIENT__
	class CLSelectPCHandler {

	public :

		// execute packet's handler
		static void execute ( CLSelectPC * pPacket , Player * pPlayer ) throw ( ProtocolException , Error );

	};
#endif

#endif
