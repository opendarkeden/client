//////////////////////////////////////////////////////////////////////////////
// Filename    : CGSelectTileEffect.h 
// Written By  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __CG_SELECT_TILE_EFFECT_H__
#define __CG_SELECT_TILE_EFFECT_H__

#include "Types.h"
#include "Exception.h"
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////////////
// class CGSelectTileEffect
//////////////////////////////////////////////////////////////////////////////

class CGSelectTileEffect : public Packet 
{
public:
    void read(SocketInputStream & iStream) throw(ProtocolException, Error);
    void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);
	PacketID_t getPacketID() const throw() { return PACKET_CG_SELECT_TILE_EFFECT; }
	PacketSize_t getPacketSize() const throw() { return szObjectID; }

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName() const throw() { return "CGSelectTileEffect"; }
		std::string toString() const throw();
	#endif

public:
	ObjectID_t getEffectObjectID(void) const { return m_EffectObjectID; }
	void setEffectObjectID(ObjectID_t id) { m_EffectObjectID = id; }

private:
	ObjectID_t m_EffectObjectID; // 선택한 이펙트의 오브젝트 ID
};


//////////////////////////////////////////////////////////////////////////////
// class CGSelectTileEffectFactory
//////////////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CGSelectTileEffectFactory : public PacketFactory {
	Packet* createPacket() throw() { return new CGSelectTileEffect(); }

	#ifdef __DEBUG_OUTPUT__
		std::string getPacketName() const throw() { return "CGSelectTileEffect"; }
	#endif

	PacketID_t getPacketID() const throw() { return Packet::PACKET_CG_SELECT_TILE_EFFECT; }
	PacketSize_t getPacketMaxSize() const throw() { return szObjectID; }
};
#endif

//////////////////////////////////////////////////////////////////////////////
// class CGSelectTileEffectHandler
//////////////////////////////////////////////////////////////////////////////

class Effect;

#ifndef __GAME_CLIENT__
	class CGSelectTileEffectHandler 
	{
	public:
		static void execute(CGSelectTileEffect* pCGSelectTileEffect, Player* pPlayer) throw ( ProtocolException , Error );
		static void executeVampirePortal(CGSelectTileEffect* pCGSelectTileEffect, Player* pPlayer, Effect* pEffect) throw ( ProtocolException , Error );
	};
#endif

#endif
