//////////////////////////////////////////////////////////////////////
// 
// Filename    :  GCLearnSkillFailed.h 
// Written By  :  elca@ewestsoft.com
// Description :  Å
//                
// 
//////////////////////////////////////////////////////////////////////

#ifndef __GC_LEARN_SKILL_FAILED_H__
#define __GC_LEARN_SKILL_FAILED_H__

// include files
#include "Types.h"
#include "Exception.h"
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////
//
// class  GCLearnSkillFailed;
//
//////////////////////////////////////////////////////////////////////

class GCLearnSkillFailed : public Packet 
{

public: 

	GCLearnSkillFailed() throw();
	virtual ~GCLearnSkillFailed() throw();

	
public:
	
	// ÀÔ·Â½ºÆ®¸²(¹öÆÛ)À¸·ÎºÎÅÍ µ¥ÀÌÅ¸¸¦ ÀÐ¾î¼­ ÆÐÅ¶À» ÃÊ±âÈ­ÇÑ´Ù.
	void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
			
	// Ãâ·Â½ºÆ®¸²(¹öÆÛ)À¸·Î ÆÐÅ¶ÀÇ ¹ÙÀÌ³Ê¸® ÀÌ¹ÌÁö¸¦ º¸³½´Ù.
	void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );

	// execute packet's handler
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );

	// get packet id
	PacketID_t getPacketID () const throw () { return PACKET_GC_LEARN_SKILL_FAILED; }
	
	// get packet size
	PacketSize_t getPacketSize () const throw () { return szSkillType+szBYTE; }
	
	#ifdef __DEBUG_OUTPUT__
		// get packet's name
		std::string getPacketName () const throw () { return "GCLearnSkillFailed"; }
		
		// get packet's debug std::string
		std::string toString () const throw ();
	#endif
	
	// get/set skill type
	SkillType_t getSkillType(void) const throw () { return m_SkillType; }
	void setSkillType( SkillType_t SkillType ) throw () { m_SkillType = SkillType; }

	// get/set description
	BYTE getDesc(void) const throw() { return m_Desc;}
	void setDesc(BYTE desc) throw() { m_Desc = desc;}

private:

	SkillType_t m_SkillType;
	BYTE        m_Desc;

};


//////////////////////////////////////////////////////////////////////
//
// class  GCLearnSkillFailedFactory;
//
// Factory for  GCLearnSkillFailed
//
//////////////////////////////////////////////////////////////////////

class  GCLearnSkillFailedFactory : public PacketFactory {

public :
	
	// constructor
	 GCLearnSkillFailedFactory () throw () {}
	
	// destructor
	virtual ~GCLearnSkillFailedFactory () throw () {}

	
public :
	
	// create packet
	Packet * createPacket () throw () { return new GCLearnSkillFailed(); }

	#ifdef __DEBUG_OUTPUT__
		// get packet name
		std::string getPacketName () const throw () { return "GCLearnSkillFailed"; }
	#endif
	
	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_GC_LEARN_SKILL_FAILED; }

	// get Packet Max Size
	PacketSize_t getPacketMaxSize() const throw() { return szSkillType+szBYTE; }

};


//////////////////////////////////////////////////////////////////////
//
// class  GCLearnSkillFailedHandler;
//
//////////////////////////////////////////////////////////////////////

class  GCLearnSkillFailedHandler {

public :

	// execute packet's handler
	static void execute (  GCLearnSkillFailed * pGCLearnSkillFailed , Player * pPlayer ) throw ( Error );

};

#endif	// __GC_LEARN_SKILL_FAILED_H__
