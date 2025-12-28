//////////////////////////////////////////////////////////////////////
// 
// Filename    : BloodBibleBonusInfo.h 
// Written By  : 
// Description :
// 
//////////////////////////////////////////////////////////////////////

#ifndef __BLOOD_BIBLE_BONUS_INFO_H__
#define __BLOOD_BIBLE_BONUS_INFO_H__

#pragma warning(disable:4786)

// include files
#include "Types.h"
#include "Exception.h"
#include "Packet.h"
#include <list>


//////////////////////////////////////////////////////////////////////
//
// class BloodBibleBonusInfo;
//
//////////////////////////////////////////////////////////////////////

typedef std::list<OptionType_t> OptionTypeList;
typedef OptionTypeList::const_iterator OptionTypeListConstItor;

class BloodBibleBonusInfo {

public :
	
	// constructor
	BloodBibleBonusInfo () throw ();
	
	// destructor
	~BloodBibleBonusInfo () throw ();

public :
	
	
    // 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
    void read (SocketInputStream & iStream) throw (ProtocolException, Error);
		    
    // 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
    void write (SocketOutputStream & oStream) const throw (ProtocolException, Error);

	// get packet's body size
	PacketSize_t getSize () throw () { return szRace; }//szBYTE + szBYTE+ szBYTE + m_OptionTypeList.size(); }

	// get packet's max size
	static uint getMaxSize() throw() { return szRace; }//szBYTE + szBYTE + szBYTE + 255; }

#ifdef __DEBUG_OUTPUT__
	// get packet's debug std::string
	std::string toString () const throw ();
#endif

	// get/set BloodBibleBonus Type
//	BYTE getType() const { return m_Type; }
//	void setType( BYTE type ) { m_Type = type; }

	// get/set Owner Race
	Race_t getRace() const { return m_Race; }
	void setRace( Race_t race ) { m_Race = race; }

//	void clearOptionTypeList() { m_OptionTypeList.clear(); }
//	void addOptionType( OptionType_t optionType ) { m_OptionTypeList.push_back( optionType ); }
//	void setOptionType( OptionTypeList& optionTypeList ) { m_OptionTypeList = optionTypeList; }
//	int getOptionTypeSize() const { return m_OptionTypeList.size(); }
//	OptionType_t popOptionType()
//	{
//		if ( m_OptionTypeList.empty() ) return 0;
//		OptionType_t optionType = m_OptionTypeList.front();
//		m_OptionTypeList.pop_front();
//		return optionType;
//	}

private :

	// Blood Bible Bonus type
//	BYTE m_Type;

	// Owner Race
	Race_t m_Race;

	// Option Type List;
//	OptionTypeList m_OptionTypeList;

};

#endif
