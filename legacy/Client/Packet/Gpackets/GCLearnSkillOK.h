//////////////////////////////////////////////////////////////////////
// 
// Filename    :  GCLearnSkillOK.h 
// Written By  :  elca@ewestsoft.com
// Description :  Å

//                
// 
//////////////////////////////////////////////////////////////////////

#ifndef __GC_LEARN_SKILL_OK_H__
#define __GC_LEARN_SKILL_OK_H__

// include files
#include "Types.h"
#include "Exception.h"
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////
//
// class  GCLearnSkillOK;
//
//////////////////////////////////////////////////////////////////////

class GCLearnSkillOK : public Packet {

public :
	
	// constructor
	GCLearnSkillOK () throw ();
	
	// destructor
	~GCLearnSkillOK () throw ();

	
public :
	
	
    // ÀÔ·Â½ºÆ®¸²(¹öÆÛ)À¸·ÎºÎÅÍ µ¥ÀÌÅ¸¸¦ ÀÐ¾î¼­ ÆÐÅ¶À» ÃÊ±âÈ­ÇÑ´Ù.
    void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
		    
    // Ãâ·Â½ºÆ®¸²(¹öÆÛ)À¸·Î ÆÐÅ¶ÀÇ ¹ÙÀÌ³Ê¸® ÀÌ¹ÌÁö¸¦ º¸³½´Ù.
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );

	// execute packet's handler
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );

	// get packet id
	PacketID_t getPacketID () const throw () { return PACKET_GC_LEARN_SKILL_OK; }
	
	// get packet size
	PacketSize_t getPacketSize () const throw () { return szSkillType+szSkillDomainType; }
	
	#ifdef __DEBUG_OUTPUT__
		// get packet's name
		std::string getPacketName () const throw () { return "GCLearnSkillOK"; }
		
		// get packet's debug std::string
		std::string toString () const throw ();
	#endif
	
	// get/set m_SkillType
	SkillType_t getSkillType() const throw () { return m_SkillType; }
	void setSkillType( SkillType_t SkillType ) throw () { m_SkillType = SkillType; }

	// get/set m_SkillDomainType
	SkillDomainType_t getSkillDomainType() const throw() { return m_DomainType;}
	void setSkillDomainType( SkillDomainType_t DomainType) throw() { m_DomainType = DomainType;}

private : 

	// SkillType
	SkillType_t m_SkillType; 

	// DomainType
	SkillDomainType_t m_DomainType;
};


//////////////////////////////////////////////////////////////////////
//
// class  GCLearnSkillOKFactory;
//
// Factory for  GCLearnSkillOK
//
//////////////////////////////////////////////////////////////////////

class  GCLearnSkillOKFactory : public PacketFactory {

public :
	
	// constructor
	 GCLearnSkillOKFactory () throw () {}
	
	// destructor
	virtual ~GCLearnSkillOKFactory () throw () {}

	
public :
	
	// create packet
	Packet * createPacket () throw () { return new GCLearnSkillOK(); }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "GCLearnSkillOK"; }
	#endif
	
	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_GC_LEARN_SKILL_OK; }

	// get Packet Max Size
	PacketSize_t getPacketMaxSize() const throw() { return szSkillType+szSkillDomainType; }

};


//////////////////////////////////////////////////////////////////////
//
// class  GCLearnSkillOKHandler;
//
//////////////////////////////////////////////////////////////////////

class  GCLearnSkillOKHandler {

public :

	// execute packet's handler
	static void execute (  GCLearnSkillOK * pGCLearnSkillOK , Player * pPlayer ) throw ( Error );

};

#endif	// __GC_LEARN_SKILL_OK_H__
