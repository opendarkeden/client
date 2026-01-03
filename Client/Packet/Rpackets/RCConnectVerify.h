//////////////////////////////////////////////////////////////////////
// 
// Filename    : RCConnectVerify.h 
// Written By  : crazydog
// Description : Effect 제거.
// 
//////////////////////////////////////////////////////////////////////

#ifndef __RC_CONNECT_VERIFY_H__
#define __RC_CONNECT_VERIFY_H__

// include files
#include "Types.h"
#include "Exception.h"
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////
//
// class RCConnectVerify;
//
// 게임서버에서 클라이언트로 자신의 변형된 데이터를 알려주기 위한 객채
// RemoveEffectrmation, SkillToObjectOK 등에 실려서 날아간다.
//
//////////////////////////////////////////////////////////////////////
enum RC_CONNECT_VERIFY_CODE
{
	RC_CONNECT_NULL,			// 잘못된 값

	RC_CONNECT_OK,				// 허용
	RC_CONNECT_REJECT,			// 거부 - -; 
	RC_CONNECT_WRONG,			// 잘못된 접속(RequestServerName이 다르다던가..)
};

class RCConnectVerify : public Packet
{

public :
	
	// constructor
	RCConnectVerify () throw ();
	
	// destructor
	~RCConnectVerify () throw ();
	
public :
    PacketID_t getPacketID () const throw () { return PACKET_RC_CONNECT_VERIFY; }

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName () const throw () { return "RCConnectVerify"; }
		
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
	PacketSize_t getPacketSize () const throw () { return szBYTE; }
	static PacketSize_t getPacketMaxSize() throw() { return szBYTE;}

	// get / set ListNumber
	RC_CONNECT_VERIFY_CODE	getCode() const throw() { return m_Code; }
	void					setCode( RC_CONNECT_VERIFY_CODE code ) throw() { m_Code = code; }

protected :
	
	RC_CONNECT_VERIFY_CODE		m_Code;

};

//////////////////////////////////////////////////////////////////////
//
// class RCConnectVerifyFactory;
//
// Factory for RCConnectVerify
//
//////////////////////////////////////////////////////////////////////

class RCConnectVerifyFactory : public PacketFactory {

public :
	
	// constructor
	RCConnectVerifyFactory () throw () {}
	
	// destructor
	virtual ~RCConnectVerifyFactory () throw () {}

	
public :
	
	// create packet
	Packet * createPacket () throw () { return new RCConnectVerify(); }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "RCConnectVerify"; }
	#endif
	
	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_RC_CONNECT_VERIFY; }

	// get Packet Max Size
	PacketSize_t getPacketMaxSize () const throw () { return 255;}

};


//////////////////////////////////////////////////////////////////////
//
// class RCConnectVerifyHandler;
//
//////////////////////////////////////////////////////////////////////

class RCConnectVerifyHandler {

public :

	// execute packet's handler
	static void execute ( RCConnectVerify * pRCConnectVerify , Player * pPlayer ) throw ( Error );

};


#endif
