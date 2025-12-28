//////////////////////////////////////////////////////////////////////////////
// Filename    : GCNPCAskVariable.h 
// Written By  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __GC_NPC_ASK_VARIABLE_H__
#define __GC_NPC_ASK_VARIABLE_H__

#include <map>
#include "Packet.h"
#include "PacketFactory.h"
#include "ScriptParameter.h"

//////////////////////////////////////////////////////////////////////////////
// class GCNPCAskVariable;
// NPC 의 대사를 주변의 PC 들에게 전송한다.
//////////////////////////////////////////////////////////////////////////////

typedef std::map<std::string,ScriptParameter*>			HashMapScriptParameter;
typedef HashMapScriptParameter::iterator		HashMapScriptParameterItor;
typedef HashMapScriptParameter::const_iterator	HashMapScriptParameterConstItor;

class GCNPCAskVariable : public Packet
{
public:
	GCNPCAskVariable() throw();
	virtual ~GCNPCAskVariable() throw();

public:
	void read(SocketInputStream & iStream) throw(ProtocolException, Error);
	void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);
	PacketID_t getPacketID() const throw() { return PACKET_GC_NPC_ASK_VARIABLE; }
	PacketSize_t getPacketSize() const throw();
#ifdef __DEBUG_OUTPUT__
	std::string getPacketName() const throw() { return "GCNPCAskVariable"; }
	std::string toString() const throw();
#endif 

public:
	ObjectID_t getObjectID(void) const throw() { return m_ObjectID; }
#ifndef __GAME_CLIENT__
	void setObjectID(ObjectID_t creatureID) throw() { m_ObjectID = creatureID; }
#endif

	ScriptID_t getScriptID(void) const throw() { return m_ScriptID; }
#ifndef __GAME_CLIENT__
	void setScriptID(ScriptID_t id) throw() { m_ScriptID = id; }
#endif

	void addScriptParameter( ScriptParameter* pParam ) throw(DuplicatedException);
	void clearScriptParameters() throw();
	HashMapScriptParameter& getScriptParameters() { return m_ScriptParameters; }
	std::string getValue( const std::string& name ) const throw(NoSuchElementException);

private:
	ObjectID_t m_ObjectID; // NPC's object id
	ScriptID_t m_ScriptID; // script id
	HashMapScriptParameter m_ScriptParameters; // 스크립트의 변수 파라미터들
};

//////////////////////////////////////////////////////////////////////////////
// class GCNPCAskVariableFactory;
//////////////////////////////////////////////////////////////////////////////

class GCNPCAskVariableFactory : public PacketFactory 
{
public:
	Packet* createPacket() throw() { return new GCNPCAskVariable(); }
	std::string getPacketName() const throw() { return "GCNPCAskVariable"; }
	PacketID_t getPacketID() const throw() { return Packet::PACKET_GC_NPC_ASK_VARIABLE; }
	PacketSize_t getPacketMaxSize() const throw()
	{
		return szObjectID
			 + szScriptID
			 + szBYTE
			 + ScriptParameter::getMaxSize() * 255;
	}
};

//////////////////////////////////////////////////////////////////////////////
// class GCNPCAskVariableHandler;
//////////////////////////////////////////////////////////////////////////////

class GCNPCAskVariableHandler 
{
public:
	static void execute(GCNPCAskVariable* pPacket, Player* pPlayer) throw(ProtocolException, Error);
};

#endif
