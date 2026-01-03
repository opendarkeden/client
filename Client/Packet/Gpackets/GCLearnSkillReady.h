//////////////////////////////////////////////////////////////////////
// 
// Filename    :  GCLearnSkillReady.h 
// Written By  :  elca@ewestsoft.com
// Description :  Å

//                
// 
//////////////////////////////////////////////////////////////////////

#ifndef __GC_LEARN_SKILL_READY_H__
#define __GC_LEARN_SKILL_READY_H__

// include files
#include "Types.h"
#include "Exception.h"
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////
//
// class  GCLearnSkillReady;
//
//////////////////////////////////////////////////////////////////////

class GCLearnSkillReady : public Packet {

public :
	
	// constructor
	GCLearnSkillReady () throw ();
	
	// destructor
	~GCLearnSkillReady () throw ();

	
public :
	
	
    // ÀÔ·Â½ºÆ®¸²(¹öÆÛ)À¸·ÎºÎÅÍ µ¥ÀÌÅ¸¸¦ ÀÐ¾î¼­ ÆÐÅ¶À» ÃÊ±âÈ­ÇÑ´Ù.
    void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
		    
    // Ãâ·Â½ºÆ®¸²(¹öÆÛ)À¸·Î ÆÐÅ¶ÀÇ ¹ÙÀÌ³Ê¸® ÀÌ¹ÌÁö¸¦ º¸³½´Ù.
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );

	// execute packet's handler
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );

	// get packet id
	PacketID_t getPacketID () const throw () { return PACKET_GC_LEARN_SKILL_READY; }
	
	// get packet size
	PacketSize_t getPacketSize () const throw () { return szSkillDomainType; }
	
	#ifdef __DEBUG_OUTPUT__
		// get packet's name
		std::string getPacketName () const throw () { return "GCLearnSkillReady"; }
		
		// get packet's debug std::string
		std::string toString () const throw ();
	#endif
	
	SkillDomainType_t getSkillDomainType() const throw() { return m_SkillDomainType;}
	void setSkillDomainType( SkillDomainType_t SkillDomainType) throw()
	{
		m_SkillDomainType = SkillDomainType;
	}
private : 

	// ½ºÅ³ Å¸ÀÔ
	SkillDomainType_t m_SkillDomainType;

};


//////////////////////////////////////////////////////////////////////
//
// class  GCLearnSkillReadyFactory;
//
// Factory for  GCLearnSkillReady
//
//////////////////////////////////////////////////////////////////////

class  GCLearnSkillReadyFactory : public PacketFactory {

public :
	
	// constructor
	 GCLearnSkillReadyFactory () throw () {}
	
	// destructor
	virtual ~GCLearnSkillReadyFactory () throw () {}

	
public :
	
	// create packet
	Packet * createPacket () throw () { return new GCLearnSkillReady(); }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "GCLearnSkillReady"; }	
	#endif
	
	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_GC_LEARN_SKILL_READY; }

	// get Packet Max Size
	PacketSize_t getPacketMaxSize() const throw() { return szSkillDomainType;}

};


//////////////////////////////////////////////////////////////////////
//
// class  GCLearnSkillReadyHandler;
//
//////////////////////////////////////////////////////////////////////

class  GCLearnSkillReadyHandler {

public :

	// execute packet's handler
	static void execute (  GCLearnSkillReady * pGCLearnSkillReady , Player * pPlayer ) throw ( Error );

};

#endif	// __GC_LEARN_SKILL_READY_H__
