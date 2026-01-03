//////////////////////////////////////////////////////////////////////////////
// Filename    : CGRequestPowerPoint.h 
// Written By  : bezz
// Description :
//////////////////////////////////////////////////////////////////////////////

#ifndef __CG_REQUEST_POWER_POINT_H__
#define __CG_REQUEST_POWER_POINT_H__

#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////////////
// class CGRequestPowerPoint;
//////////////////////////////////////////////////////////////////////////////

class CGRequestPowerPoint : public Packet
{
public:
	CGRequestPowerPoint () throw ();
	~CGRequestPowerPoint () throw ();
	
public:
    void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );
    PacketID_t getPacketID () const throw () { return PACKET_CG_REQUEST_POWER_POINT; }
	PacketSize_t getPacketSize () const throw () { return szBYTE + m_CellNum.size(); }
#ifdef __DEBUG_OUTPUT__
	string getPacketName () const throw () { return "CGRequestPowerPoint"; }
	string toString () const throw ();
#endif
public:
	// get / set cell number
	const string& getCellNum() const { return m_CellNum; }
	void setCellNum( const string& cellnum ) { m_CellNum = cellnum; }

protected:
	string			m_CellNum;
};

//////////////////////////////////////////////////////////////////////////////
// class CGRequestPowerPointFactory;
//////////////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CGRequestPowerPointFactory : public PacketFactory 
{
public:
	Packet * createPacket () throw () { return new CGRequestPowerPoint(); }
	string getPacketName () const throw () { return "CGRequestPowerPoint"; }
	PacketID_t getPacketID () const throw () { return Packet::PACKET_CG_REQUEST_POWER_POINT; }
	PacketSize_t getPacketMaxSize () const throw () { return szBYTE + 12;}
};
#endif
//////////////////////////////////////////////////////////////////////////////
// class CGRequestPowerPointHandler;
//////////////////////////////////////////////////////////////////////////////
#ifndef __GAME_CLIENT__
class CGRequestPowerPointHandler 
{
public:
	static void execute ( CGRequestPowerPoint * pCGRequestPowerPoint , Player * pPlayer ) throw (ProtocolException, Error);
};
#endif

#endif

