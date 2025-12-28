//////////////////////////////////////////////////////////////////////////////
// Filename    : GCAddItemToItemVerify.h 
// Written By  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __GC_ADD_ITEM_TO_ITEM_VERIFY_H__
#define __GC_ADD_ITEM_TO_ITEM_VERIFY_H__

#include "Packet.h"
#include "PacketFactory.h"

enum
{
    ADD_ITEM_TO_ITEM_VERIFY_ERROR ,                  // error

    ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_IMPOSSIBLE,     // enchant 완전 불가
    ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_FAIL_CRASH,     // enchant 실패 아이템 깨짐
    ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_FAIL_DECREASE,  // enchant 실패 옵션 한단계 내려감
    ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_OK,             // enchant 성공
	ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_IMPOSSIBLE_NO_PREMIUM	,// 프리미엄사용자가 아니라 무효
	ADD_ITEM_TO_ITEM_VERIFY_TRANS_OK,				// 성전환 성공
	ADD_ITEM_TO_ITEM_VERIFY_TRANS_IMPOSSIBLE,		// 성전환 실패
	ADD_ITEM_TO_ITEM_VERIFY_MIXING_OK,				// 믹싱포지 성공
	ADD_ITEM_TO_ITEM_VERIFY_DETACHING_OK,			// 아이템 옵션 삭제 성공
	ADD_ITEM_TO_ITEM_VERIFY_MIXING_FAILED_SAME_OPTION_GROUP,    // 아이템의 옵션 그룹이 같아서 섞을 수 없습니다.
	ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_FAIL,			// 인첸트 걍 실패
    ADD_ITEM_TO_ITEM_VERIFY_REVIVAL_OK,                 // 펫 부활 성공

	ADD_ITEM_TO_ITEM_VERIFY_CLEAR_OPTION_OK,            // option 제거 성공
	ADD_ITEM_TO_ITEM_VERIFY_UP_GRADE_OK,                // grade올리기 성공

	//add by Coffee 2006.11.3  
	ADD_ITEM_TO_ITEM_VERIFY_THREE_ENCHANT_OK,

	//add by svi 2009-07-15
	ADD_ITEM_TO_ITEM_REMOVE_OPTION_OK,



    ADD_ITEM_TO_ITEM_VERIFY_MAX
};

//////////////////////////////////////////////////////////////////////////////
// class GCAddItemToItemVerify
//////////////////////////////////////////////////////////////////////////////

class GCAddItemToItemVerify : public Packet 
{

public:
	GCAddItemToItemVerify() throw() { m_Code = ADD_ITEM_TO_ITEM_VERIFY_MAX; m_Parameter = 0;}
	virtual ~GCAddItemToItemVerify() throw() {}

public:
	void read(SocketInputStream & iStream) throw(ProtocolException, Error);
	void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);

	PacketID_t getPacketID() const throw() { return PACKET_GC_ADD_ITEM_TO_ITEM_VERIFY; }
	PacketSize_t getPacketSize() const throw();

#ifdef __DEBUG_OUTPUT__
	std::string getPacketName() const throw() { return "GCAddItemToItemVerify"; }
	std::string toString() const throw();
#endif
	
public:
	BYTE getCode(void) const throw() { return m_Code;}
	void setCode(BYTE code) throw() { m_Code = code;}

	uint getParameter(void) const throw() { return m_Parameter; }
	void setParameter(uint parameter) throw() { m_Parameter = parameter; }

	uint getParameter2(void) const throw() { return m_Parameter2; }
	void setParameter2(uint parameter) throw() { m_Parameter2 = parameter; }

private: 
	BYTE m_Code;
	uint m_Parameter;
	// add by Coffee 2006.11.3 힛橄昑
	uint m_Parameter2;

};


//////////////////////////////////////////////////////////////////////////////
// class GCAddItemToItemVerifyFactory;
//////////////////////////////////////////////////////////////////////////////

class GCAddItemToItemVerifyFactory : public PacketFactory 
{
public:
	Packet* createPacket() throw() { return new GCAddItemToItemVerify(); }
	std::string getPacketName() const throw() { return "GCAddItemToItemVerify"; }
	PacketID_t getPacketID() const throw() { return Packet::PACKET_GC_ADD_ITEM_TO_ITEM_VERIFY; }
	PacketSize_t getPacketMaxSize() const throw() { return szBYTE + szuint +szuint; }
};


//////////////////////////////////////////////////////////////////////////////
// class GCAddItemToItemVerifyHandler
//////////////////////////////////////////////////////////////////////////////

class GCAddItemToItemVerifyHandler 
{
public:
	static void execute( GCAddItemToItemVerify* pPacket, Player* pPlayer) throw(ProtocolException, Error);
};

#endif
