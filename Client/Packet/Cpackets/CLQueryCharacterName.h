//--------------------------------------------------------------------------------
// 
// Filename    : CLQueryCharacterName.h 
// Written By  : Reiot
// Description : 
// 
//--------------------------------------------------------------------------------

#ifndef __CL_QUERY_CHARACTER_NAME_H__
#define __CL_QUERY_CHARACTER_NAME_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"

//--------------------------------------------------------------------------------
//
// class CLQueryCharacterName;
//
// 클라이언트가 로그인 서버에게 최초에 전송하는 패킷이다.
// 아이디와 패스워드가 암호화되어 있다.
//
//--------------------------------------------------------------------------------

class CLQueryCharacterName : public Packet {

public:
	
    // 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
    void read(SocketInputStream & iStream) throw(ProtocolException, Error);
		    
    // 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
    void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);

	// execute packet's handler
	void execute(Player* pPlayer) throw(ProtocolException, Error);

	// get packet id
	PacketID_t getPacketID() const throw() { return Packet::PACKET_CL_QUERY_CHARACTER_NAME; }
	
	// get packet's body size
	PacketSize_t getPacketSize() const throw() 
	{ 
		return szBYTE + m_CharacterName.size(); 
	}

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName() const throw() { return "CLQueryCharacterName"; }
		
		// get packet's debug std::string
		std::string toString() const throw();
	#endif

public:

	// get/set player's id
	std::string getCharacterName() const throw() { return m_CharacterName; }
	void setCharacterName(const std::string & playerID) throw() { m_CharacterName = playerID; }

private :

	// 플레이어 아이디
	std::string m_CharacterName;

};


//--------------------------------------------------------------------------------
//
// class CLQueryCharacterNameFactory;
//
// Factory for CLQueryCharacterName
//
//--------------------------------------------------------------------------------

class CLQueryCharacterNameFactory : public PacketFactory {

public:
	
	// create packet
	Packet* createPacket() throw() { return new CLQueryCharacterName(); }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName() const throw() { return "CLQueryCharacterName"; }
	#endif
	
	// get packet id
	PacketID_t getPacketID() const throw() { return Packet::PACKET_CL_QUERY_CHARACTER_NAME; }

	// get packet's max body size
	PacketSize_t getPacketMaxSize() const throw() 
	{ 
		return szBYTE + 20; 
	}

};


//--------------------------------------------------------------------------------
//
// class CLQueryCharacterNameHandler;
//
//--------------------------------------------------------------------------------
#ifndef __GAME_CLIENT__
	class CLQueryCharacterNameHandler {

	public:

		// execute packet's handler
		static void execute(CLQueryCharacterName* pPacket, Player* pPlayer) throw(ProtocolException, Error);

	};
#endif

#endif
