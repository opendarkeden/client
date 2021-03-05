//////////////////////////////////////////////////////////////////////
// 
// Filename    : RequestServerPacketFactoryManager.h 
// Written By  : reiot@ewestsoft.com
// Description :
// 
//////////////////////////////////////////////////////////////////////

#ifndef __REQUEST_SERVER_PACKET_FACTORY_MANAGER_H__
#define __REQUEST_SERVER_PACKET_FACTORY_MANAGER_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"


//////////////////////////////////////////////////////////////////////
//
// class RequestServerPacketFactoryManager
//
//////////////////////////////////////////////////////////////////////

class RequestServerPacketFactoryManager {

public :
	
	// constructor
	RequestServerPacketFactoryManager () throw ();
	
	// destructor
	~RequestServerPacketFactoryManager () throw ();

	// 패킷팩토리매니저를 초기화한다.
	// 게임서버객체의 init()에서 호출된다.
	void init () throw ( Error );
	
	// 팩토리 객체를 특정 인덱스에 추가한다.
	void addFactory ( PacketFactory * pFactory ) throw ( Error );
	
	// 패킷아이디로 패킷객체를 생성한다.
	Packet * createPacket ( PacketID_t packetID ) throw ( InvalidProtocolException , Error );

	#ifdef	__DEBUG_OUTPUT__
		// 특정 패킷의 최대 크기를 리턴한다.
		std::string getPacketName ( PacketID_t packetID ) throw ( InvalidProtocolException , Error );

		// get debug string
		std::string toString () const throw ();
	#endif

	// 특정 패킷의 최대 크기를 리턴한다.
	PacketSize_t getPacketMaxSize ( PacketID_t packetID ) throw ( InvalidProtocolException , Error );

	

private :
	
	// 패킷팩토리의 배열
	PacketFactory ** m_Factories;
	
	// 패킷팩토리배열의 크기
	ushort m_Size;

};

extern RequestServerPacketFactoryManager * g_pRequestServerPacketFactoryManager;

#endif
