//////////////////////////////////////////////////////////////////////
// 
// Filename    : CRDisconnect.h 
// Written By  : crazydog
// Description : Effect 제거.
// 
//////////////////////////////////////////////////////////////////////

#ifndef __CR_DISCONNECT_H__
#define __CR_DISCONNECT_H__

// include files
#include "Types.h"
#include "Exception.h"
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////
//
// class CRDisconnect;
//
// 다른 클라이언트와의 접속을 강제로 끊는다.
//
//////////////////////////////////////////////////////////////////////

class CRDisconnect : public Packet
{

public :
	
	// constructor
	CRDisconnect () throw ();
	
	// destructor
	~CRDisconnect () throw ();
	
public :
    PacketID_t getPacketID () const throw () { return PACKET_CR_DISCONNECT; }

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName () const throw () { return "CRDisconnect"; }
		
		// get packet's debug std::string
		std::string toString () const throw ();
	#endif


    // 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
    void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
		    
    // 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );
	// execute packet's handler
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );

	// get packet's body size
	// 최적화시, 미리 계산된 정수를 사용한다.
	PacketSize_t getPacketSize () const throw () { return 0; }
	static PacketSize_t getPacketMaxSize() throw() { return 0;}

protected :

};

//////////////////////////////////////////////////////////////////////
//
// class CRDisconnectFactory;
//
// Factory for CRDisconnect
//
//////////////////////////////////////////////////////////////////////

class CRDisconnectFactory : public PacketFactory {

public :
	
	// constructor
	CRDisconnectFactory () throw () {}
	
	// destructor
	virtual ~CRDisconnectFactory () throw () {}

	
public :
	
	// create packet
	Packet * createPacket () throw () { return new CRDisconnect(); }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "CRDisconnect"; }
	#endif
	
	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_CR_DISCONNECT; }

	// get Packet Max Size
	PacketSize_t getPacketMaxSize () const throw () { return 0;}

};


//////////////////////////////////////////////////////////////////////
//
// class CRDisconnectHandler;
//
//////////////////////////////////////////////////////////////////////

class CRDisconnectHandler {

public :

	// execute packet's handler
	static void execute ( CRDisconnect * pCRDisconnect , Player * pPlayer ) throw ( ProtocolException , Error );

};


#endif
