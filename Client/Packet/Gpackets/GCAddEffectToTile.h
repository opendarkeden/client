//////////////////////////////////////////////////////////////////////
// 
// Filename    : GCAddEffectToTile.h 
// Written By  : elca@ewestsoft.com
// Description : 기술이 성공했을때 보내는 패킷을 위한 클래스 정의
// 
//////////////////////////////////////////////////////////////////////

#ifndef __GC_ADD_EFFECT_TO_TILE_H__
#define __GC_ADD_EFFECT_TO_TILE_H__

// include files
#include "Types.h"
#include "Exception.h"
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////
//
// class GCAddEffectToTile;
//
// 게임서버에서 클라이언트로 자신의 기술이 성공을 알려주기 위한 클래스
//
//////////////////////////////////////////////////////////////////////

class GCAddEffectToTile : public Packet {

public :
	
	// constructor
	GCAddEffectToTile () throw ();
	
	// destructor
	~GCAddEffectToTile () throw ();

	
public :
	
	
    // 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
    void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
		    
    // 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );

	// execute packet's handler
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );

	// get packet id
	PacketID_t getPacketID () const throw () { return PACKET_GC_ADD_EFFECT_TO_TILE; }
	
	// get packet's body size
	// 최적화시, 미리 계산된 정수를 사용한다.
	PacketSize_t getPacketSize () const throw () { return szObjectID + szCoord*2 + szEffectID + szDuration; }

	#ifdef __DEBUG_OUTPUT__
		// get packet's name
		std::string getPacketName () const throw () { return "GCAddEffectToTile"; }
		
		// get packet's debug std::string
		std::string toString () const throw ();
	#endif

	// get / set EffectID 
	EffectID_t getEffectID() const throw() { return m_EffectID; }
	void setEffectID( EffectID_t e ) throw() { m_EffectID = e; }
	

	// get / set Duration 
	Duration_t getDuration() const throw() { return m_Duration; }
	void setDuration( Duration_t d ) throw() { m_Duration = d; }

	// get / set ObjectID 
	ObjectID_t getObjectID() const throw() { return m_ObjectID; }
	void setObjectID( ObjectID_t d ) throw() { m_ObjectID = d; }

	// get & set X, Y
	Coord_t getX() const throw() { return m_X;}
	Coord_t getY() const throw() { return m_Y;}
	void setXY( Coord_t x, Coord_t y) throw() { m_X = x; m_Y = y;}
	
private :
	
	Coord_t  m_X, m_Y;
	ObjectID_t m_ObjectID;

	EffectID_t	m_EffectID;
	Duration_t	m_Duration;

};


//////////////////////////////////////////////////////////////////////
//
// class GCAddEffectToTileFactory;
//
// Factory for GCAddEffectToTile
//
//////////////////////////////////////////////////////////////////////

class GCAddEffectToTileFactory : public PacketFactory {

public :
	
	// constructor
	GCAddEffectToTileFactory () throw () {}
	
	// destructor
	virtual ~GCAddEffectToTileFactory () throw () {}

	
public :
	
	// create packet
	Packet * createPacket () throw () { return new GCAddEffectToTile(); }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "GCAddEffectToTile"; }
	#endif
	
	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_GC_ADD_EFFECT_TO_TILE; }

	// get Packet Max Size
	// PacketSize_t getPacketMaxSize() const throw() { return szSkillType + szCEffectID + szDuration + szBYTE + szBYTE * m_ListNum * 2 ; }
	PacketSize_t getPacketMaxSize() const throw() { return szObjectID + szCoord*2 + szEffectID + szDuration; }

};


//////////////////////////////////////////////////////////////////////
//
// class GCAddEffectToTileHandler;
//
//////////////////////////////////////////////////////////////////////

class GCAddEffectToTileHandler {

public :

	// execute packet's handler
	static void execute ( GCAddEffectToTile * pGCAddEffectToTile , Player * pPlayer ) throw ( Error );

};

#endif
