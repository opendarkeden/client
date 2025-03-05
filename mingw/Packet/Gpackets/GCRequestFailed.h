//////////////////////////////////////////////////////////////////////////////
// Filename    : GCRequestFailed.h 
// Written By  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __GC_REQUEST_FAILED_H__
#define __GC_REQUEST_FAILED_H__

#include "Packet.h"
#include "PacketFactory.h"

enum 
{
	REQUEST_FAILED_NULL,
	REQUEST_FAILED_IP,
};

//////////////////////////////////////////////////////////////////////////////
// class GCRequestFailed
//////////////////////////////////////////////////////////////////////////////

class GCRequestFailed : public Packet 
{

public:
	GCRequestFailed() throw() { m_Code = REQUEST_FAILED_NULL; }
	virtual ~GCRequestFailed() throw() {}

public:
	void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
	void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );

	PacketID_t getPacketID () const throw () { return PACKET_GC_REQUEST_FAILED; }
	PacketSize_t getPacketSize () const throw ()	{ return szBYTE + szBYTE + m_Name.size(); }

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName () const throw () { return "GCRequestFailed"; }
		std::string toString () const throw ();
	#endif
	
public:
	BYTE getCode(void) const throw() { return m_Code;}
	void setCode(WORD code) throw() { m_Code = code;}

	const std::string& getName(void) const throw() { return m_Name;}
	void setName(const char* Name) throw() { m_Name = Name;}

private: 
	BYTE m_Code;	

	std::string m_Name;
};


//////////////////////////////////////////////////////////////////////////////
// class GCRequestFailedFactory;
//////////////////////////////////////////////////////////////////////////////

class GCRequestFailedFactory : public PacketFactory 
{
public:
	Packet * createPacket () throw () { return new GCRequestFailed(); }

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName () const throw () { return "GCRequestFailed"; }
	#endif

	PacketID_t getPacketID () const throw () { return Packet::PACKET_GC_REQUEST_FAILED; }
	PacketSize_t getPacketMaxSize () const throw () { return szBYTE + szBYTE + 10; }
};


//////////////////////////////////////////////////////////////////////////////
// class GCRequestFailedHandler
//////////////////////////////////////////////////////////////////////////////

class GCRequestFailedHandler 
{
public:
	static void execute (  GCRequestFailed * pPacket , Player * pPlayer ) throw ( ProtocolException , Error );
};

#endif
