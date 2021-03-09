//--------------------------------------------------------------------------------
// 
// Filename    : UCRequestLoginMode.h 
// Written By  : Reiot
// 
//--------------------------------------------------------------------------------

#ifndef __UC_REQUEST_LOGIN_MODE_H__
#define __UC_REQUEST_LOGIN_MODE_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"

//--------------------------------------------------------------------------------
//
// class UCRequestLoginMode;
//
//--------------------------------------------------------------------------------

class UCRequestLoginMode : public Packet {

public :

	// �Է½�Ʈ��(����)���κ��� �����͸� �о ��Ŷ�� �ʱ�ȭ�Ѵ�.
	void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error ) { throw UnsupportedError(); }

    // �������κ��� ���� �����͸� �о ��Ŷ�� �ʱ�ȭ�Ѵ�.
    void read ( Socket * pSocket ) throw ( ProtocolException , Error );
		    
    // ��½�Ʈ��(����)���� ��Ŷ�� ���̳ʸ� �̹����� ������.
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error ) { throw UnsupportedError(); }

    // �������� ���� ��Ŷ�� ���̳ʸ� �̹����� ������.
    void write ( Socket * pSocket ) const throw ( ProtocolException , Error );

	// execute packet's handler
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );

	// get packet id
	PacketID_t getPacketID () const throw () { return PACKET_UC_REQUEST_LOGIN_MODE; }
	
	// get packet body size
	// *OPTIMIZATION HINT*
	// const static UCRequestLoginModePacketSize �� ����, �����϶�.
	PacketSize_t getPacketSize () const throw () 
	{ 
		return szBYTE;
	}
	
	// �ƹ��� Ŀ�� ��ް��� ���� ���Ѵ�.
	static PacketSize_t getPacketMaxSize () throw () 
	{ 
		return szBYTE;
	}

	// get packet's name
#ifdef __DEBUG_OUTPUT__
	string getPacketName () const throw () { return "UCRequestLoginMode"; }

	// get packet's debug string
	string toString () const throw ();
#endif

public :

	// get/set login mode
	BYTE getLoginMode() const { return m_LoginMode; }
	void setLoginMode( BYTE loginMode ) { m_LoginMode = loginMode; }

private :

	BYTE m_LoginMode;
	
};


//--------------------------------------------------------------------------------
//
// class UCRequestLoginModeFactory;
//
// Factory for UCRequestLoginMode
//
//--------------------------------------------------------------------------------

class UCRequestLoginModeFactory : public PacketFactory {

public :
	
	// create packet
	Packet * createPacket () throw () { return new UCRequestLoginMode(); }

	// get packet name
	string getPacketName () const throw () { return "UCRequestLoginMode"; }
	
	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_UC_REQUEST_LOGIN_MODE; }

	// get packet's max body size
	PacketSize_t getPacketMaxSize () const throw () { return szBYTE; }
	
};


//--------------------------------------------------------------------------------
//
// class UCRequestLoginModeHandler;
//
//--------------------------------------------------------------------------------

class UCRequestLoginModeHandler {

public :

	// execute packet's handler
	static void execute ( UCRequestLoginMode * pPacket , Player * pPlayer ) throw ( ProtocolException , Error );

};

#endif