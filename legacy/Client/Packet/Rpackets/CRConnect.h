//////////////////////////////////////////////////////////////////////
// 
// Filename    : CRConnect.h 
// Written By  : crazydog
// Description : Effect 제거.
// 
//////////////////////////////////////////////////////////////////////

#ifndef __CR_CONNECT_H__
#define __CR_CONNECT_H__

// include files
#include "Types.h"
#include "Exception.h"
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////
//
// class CRConnect;
//
// 클라이언트에서 다른 클라이언트로 접속을 요청하는거다.
//
// 자기 캐릭터 이름과 상대의 캐릭터 이름을 알아야 한다.
//
//////////////////////////////////////////////////////////////////////

class CRConnect : public Packet
{

public :
	
	// constructor
	CRConnect () throw ();
	
	// destructor
	~CRConnect () throw ();
	
public :
    PacketID_t getPacketID () const throw () { return PACKET_CR_CONNECT; }

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName () const throw () { return "CRConnect"; }
		
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
	PacketSize_t getPacketSize () const throw () { return szBYTE + m_RequestServerName.size() + szBYTE + m_RequestClientName.size(); }
	static PacketSize_t getPacketMaxSize() throw() { return  szBYTE + 10 + szBYTE + 10;}

	// get / set ListNumber
	const std::string& getRequestServerName() const throw() { return m_RequestServerName; }
	void setRequestServerName(const char* pName) throw() { m_RequestServerName = pName; }

	const std::string& getRequestClientName() const throw() { return m_RequestClientName; }
	void setRequestClientName(const char* pName) throw() { m_RequestClientName = pName; }
	

protected :
	
	std::string		m_RequestServerName;
	std::string		m_RequestClientName;
};

//////////////////////////////////////////////////////////////////////
//
// class CRConnectFactory;
//
// Factory for CRConnect
//
//////////////////////////////////////////////////////////////////////

class CRConnectFactory : public PacketFactory {

public :
	
	// constructor
	CRConnectFactory () throw () {}
	
	// destructor
	virtual ~CRConnectFactory () throw () {}

	
public :
	
	// create packet
	Packet * createPacket () throw () { return new CRConnect(); }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "CRConnect"; }
	#endif
	
	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_CR_CONNECT; }

	// get Packet Max Size
	PacketSize_t getPacketMaxSize () const throw () { return  szBYTE + 10 + szBYTE + 10; }

};


//////////////////////////////////////////////////////////////////////
//
// class CRConnectHandler;
//
//////////////////////////////////////////////////////////////////////

class CRConnectHandler {

public :

	// execute packet's handler
	static void execute ( CRConnect * pCRConnect , Player * pPlayer ) throw ( Error );

};


#endif
