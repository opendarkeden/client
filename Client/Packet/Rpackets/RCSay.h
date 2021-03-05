//----------------------------------------------------------------------
// 
// Filename    : RCSay.h 
// Written By  : Reiot
// Description : 
// 
//----------------------------------------------------------------------

#ifndef __RC_SAY_H__
#define __RC_SAY_H__

// include files
#include "DatagramPacket.h"
#include "PacketFactory.h"


//----------------------------------------------------------------------
//
// class RCSay;
//
// 내가 다른 클라이언트에게 말하는 패킷
//
//----------------------------------------------------------------------

class RCSay : public DatagramPacket {

public :
	
    // Datagram 객체에서부터 데이타를 읽어서 패킷을 초기화한다.
    void read ( Datagram & iDatagram ) throw ( ProtocolException , Error );
		    
    // Datagram 객체로 패킷의 바이너리 이미지를 보낸다.
    void write ( Datagram & oDatagram ) const throw ( ProtocolException , Error );

	// execute packet's handler
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );

	// get packet id
	PacketID_t getPacketID () const throw () { return PACKET_RC_SAY; }
	
	// get packet's body size
	PacketSize_t getPacketSize () const throw () 
	{ 
		return szBYTE + m_Name.size()
				+ szBYTE + m_Message.size()
				+ szDWORD;			
	}

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "RCSay"; }
		
		// get packet's debug std::string
		std::string toString () const throw ();
	#endif

public :

	// get/set Name
	std::string getName () const throw () { return m_Name; }
	void setName ( std::string msg ) throw () { m_Name = msg; }

	// get/set Message
	std::string getMessage () const throw () { return m_Message; }
	void setMessage ( std::string msg ) throw () { m_Message = msg; }
	
	//get/set text color
	uint getColor() const throw() { return m_Color; }
	void setColor( uint color ) throw() { m_Color = color; }
	
private :
	std::string m_Name;
	std::string m_Message;	
	// text color
	uint m_Color;

};


//////////////////////////////////////////////////////////////////////
//
// class RCSayFactory;
//
// Factory for RCSay
//
//////////////////////////////////////////////////////////////////////

class RCSayFactory : public PacketFactory {

public :
	
	// create packet
	Packet * createPacket () throw () { return new RCSay(); }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "RCSay"; }
	#endif
	
	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_RC_SAY; }

	PacketSize_t getPacketMaxSize () const throw () 
	{ 
		return szBYTE + 128 + szBYTE + 20 + szDWORD;			
	}

};


//////////////////////////////////////////////////////////////////////
//
// class RCSayHandler;
//
//////////////////////////////////////////////////////////////////////

class RCSayHandler {
	
public :

	// execute packet's handler
	static void execute ( RCSay * pPacket ) throw ( ProtocolException , Error );

};

#endif
