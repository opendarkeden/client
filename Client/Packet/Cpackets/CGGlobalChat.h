//////////////////////////////////////////////////////////////////////
// 
// Filename    : CGGlobalChat.h 
// Written By  : reiot@ewestsoft.com
// Description : 
// 
//////////////////////////////////////////////////////////////////////

#ifndef __CG_GLOBAL_CHAT_H__
#define __CG_GLOBAL_CHAT_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////
//
// class CGGlobalChat;
//
// 클라이언트가 서버에게 보내는 GlobalChat 패킷이다.
// 내부에 GlobalChat String 만을 데이타 필드로 가진다.
//
//////////////////////////////////////////////////////////////////////

class CGGlobalChat : public Packet {

public:
	
    // 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
    void read(SocketInputStream & iStream) throw(ProtocolException, Error);
		    
    // 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
    void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);

	// execute packet's handler
	void execute(Player* pPlayer) throw(ProtocolException, Error);

	// get packet id
	PacketID_t getPacketID() const throw() { return PACKET_CG_GLOBAL_CHAT; }
	
	// get packet's body size
	PacketSize_t getPacketSize() const throw() { return szuint + szBYTE + m_Message.size(); }

#ifdef __DEBUG_OUTPUT__
	// get packet name
	std::string getPacketName() const throw() { return "CGGlobalChat"; }
	
	// get packet's debug std::string
	std::string toString() const throw();
#endif

	// get/set text color
	uint getColor() const throw() { return m_Color; }
	void setColor( uint color ) throw() { m_Color = color; }

	// get/set chatting message
	std::string getMessage() const throw() { return m_Message; }
	void setMessage(const std::string & msg) throw() { m_Message = msg; }
	

private :
	
	// text color
	uint m_Color;

	// chatting message
	std::string m_Message;
	
};


#ifdef __DEBUG_OUTPUT__
//////////////////////////////////////////////////////////////////////
//
// class CGGlobalChatFactory;
//
// Factory for CGGlobalChat
//
//////////////////////////////////////////////////////////////////////

class CGGlobalChatFactory : public PacketFactory {

public:
	
	// create packet
	Packet* createPacket() throw() { return new CGGlobalChat(); }

	// get packet name
	std::string getPacketName() const throw() { return "CGGlobalChat"; }
	
	// get packet id
	PacketID_t getPacketID() const throw() { return Packet::PACKET_CG_GLOBAL_CHAT; }

	// get packet's max body size
	// message 의 최대 크기에 대한 설정이 필요하다.
	PacketSize_t getPacketMaxSize() const throw() { return szuint + szBYTE + 128; }

};
#endif


#ifndef __GAME_CLIENT__
//////////////////////////////////////////////////////////////////////
//
// class CGGlobalChatHandler;
//
//////////////////////////////////////////////////////////////////////

class CGGlobalChatHandler {

public:

	// execute packet's handler
	static void execute(CGGlobalChat* pPacket, Player* pPlayer) throw(ProtocolException, Error);

};
#endif

#endif
