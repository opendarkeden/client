//////////////////////////////////////////////////////////////////////
// 
// Filename    : CGCrashReport.h 
// Written By  : reiot@ewestsoft.com
// Description : 
// 
//////////////////////////////////////////////////////////////////////

#ifndef __CG_CRASH_REPORT_H__
#define __CG_CRASH_REPORT_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"


//////////////////////////////////////////////////////////////////////
//
// class CGCrashReport;
//
//////////////////////////////////////////////////////////////////////

class CGCrashReport : public Packet {
public:
	CGCrashReport() throw();
	~CGCrashReport() throw();

public:
    void read(SocketInputStream & iStream) throw(ProtocolException, Error);
    void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);
	PacketID_t getPacketID() const throw() { return PACKET_CG_CRASH_REPORT; }
	PacketSize_t getPacketSize() const throw() {
		return
			m_ExecutableTime.size()
			+ szWORD
			+ m_Address.size()
			+ szWORD + m_OS.size()
			+ szWORD + m_CallStack.size()
			+ szWORD + m_Message.size();
	}
#ifdef __DEBUG_OUTPUT__
	std::string getPacketName() const throw() { return "CGCrashReport"; }
	std::string toString() const throw();
#endif
	
public:
	const std::string&	getExecutableTime() const { return m_ExecutableTime; }
	void			setExecutableTime( const std::string& time ) { m_ExecutableTime = time; }

	WORD			getVersion() const { return m_Version; }
	void			setVersion( WORD ver ) { m_Version = ver; }

	const std::string&	getAddress() const { return m_Address; }
	void			setAddress( const std::string& address ) { m_Address = address; }

	const std::string&	getOS() const { return m_OS; }
	void			setOS( const std::string& os ) { m_OS = os; }

	const std::string&	getCallStack() const { return m_CallStack; }
	void			setCallStack( const std::string& callStack ) { m_CallStack = callStack; }

	const std::string&	getMessage() const { return m_Message; }
	void			setMessage( const std::string& message ) { m_Message = message; }

private:
	std::string	m_ExecutableTime;
	WORD	m_Version;
	std::string	m_Address;
	std::string	m_OS;
	std::string	m_CallStack;
	std::string	m_Message;
};


//////////////////////////////////////////////////////////////////////
//
// class CGCrashReportFactory;
//
// Factory for CGCrashReport
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CGCrashReportFactory : public PacketFactory {

public:
	
	// create packet
	Packet* createPacket() throw() { return new CGCrashReport(); }

	// get packet name
	std::string getPacketName() const throw() { return "CGCrashReport"; }
	
	// get packet id
	PacketID_t getPacketID() const throw() { return Packet::PACKET_CG_CRASH_REPORT; }

	// get packet's max body size
	// *OPTIMIZATION HINT*
	// const static CGCrashReportPacketSize 를 정의해서 리턴하라.
	PacketSize_t getPacketMaxSize() const throw() {
		return
			19 + szWORD + 10 + szWORD + 100 + szWORD + 1024 + szWORD + 1024;
	}

};
#endif

#ifndef __GAME_CLIENT__
//////////////////////////////////////////////////////////////////////
//
// class CGCrashReportHandler;
//
//////////////////////////////////////////////////////////////////////

class CGCrashReportHandler {
	
public:

	// execute packet's handler
	static void execute(CGCrashReport* pPacket, Player* player) throw(ProtocolException, Error);
};
#endif

#endif
