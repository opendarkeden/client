//////////////////////////////////////////////////////////////////////
// 
// Filename    : CGSkillToTile.h 
// Written By  : elca@ewestsoft.com
// Description : 
// 
//////////////////////////////////////////////////////////////////////

#ifndef __CG_SKILL_TO_TILE_H__
#define __CG_SKILL_TO_TILE_H__

// include files
#include "Types.h"
#include "Exception.h"
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////
//
// class CGSkillToTile;
//
//////////////////////////////////////////////////////////////////////

class CGSkillToTile : public Packet {

public :
	
	// constructor
	CGSkillToTile () throw ();
	
	// destructor
	~CGSkillToTile () throw ();

	
public :
	
	
    // 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
    void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
		    
    // 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );

	// execute packet's handler
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );

	// get packet id
	PacketID_t getPacketID () const throw () { return PACKET_CG_SKILL_TO_TILE; }
	
	// get packet's body size
	PacketSize_t getPacketSize () const throw () { return szSkillType + szCEffectID + szCoord + szCoord ; }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "CGSkillToTile"; }
	#endif

	// get/set SkillType
	SkillType_t getSkillType() const throw()  { return m_SkillType; }
	void setSkillType( SkillType_t SkillType ) throw() { m_SkillType = SkillType; }

	// get/set CEffectID
	CEffectID_t getCEffectID() const throw() { return m_CEffectID; }
	void setCEffectID( CEffectID_t CEffectID ) throw() { m_CEffectID = CEffectID; }

	// get/set X
	Coord_t getX() const throw() { return m_X; }
	void setX( Coord_t X ) throw() { m_X = X; }

	// get/set Y
	Coord_t getY() const throw() { return m_Y; }
	void setY( Coord_t Y ) throw() { m_Y = Y; }
	
	#ifdef __DEBUG_OUTPUT__
		// get packet's debug std::string
		std::string toString () const throw ();
	#endif

private :

	// SkillType
	SkillType_t m_SkillType;

	// Client EffectID
	CEffectID_t m_CEffectID;

	// Coord X
	Coord_t m_X;

	// Coord Y
	Coord_t m_Y;
	
};


//////////////////////////////////////////////////////////////////////
//
// class CGSkillToTileFactory;
//
// Factory for CGSkillToTile
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CGSkillToTileFactory : public PacketFactory {

public :
	
	// constructor
	CGSkillToTileFactory () throw () {}
	
	// destructor
	virtual ~CGSkillToTileFactory () throw () {}

	
public :
	
	// create packet
	Packet * createPacket () throw () { return new CGSkillToTile(); }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "CGSkillToTile"; }
	#endif
	
	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_CG_SKILL_TO_TILE; }

	// get Packet Max Size
	PacketSize_t getPacketMaxSize() const throw() { return szSkillType + szCEffectID + szCoord + szCoord; }

};

#endif


//////////////////////////////////////////////////////////////////////
//
// class CGSkillToTileHandler;
//
//////////////////////////////////////////////////////////////////////

#ifndef __GAME_CLIENT__
class CGSkillToTileHandler {

public :

	// execute packet's handler
	static void execute ( CGSkillToTile * pCGSkillToTile , Player * pPlayer ) throw ( Error );

};
#endif

#endif
