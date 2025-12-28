//////////////////////////////////////////////////////////////////////
// 
// Filename    : CLRegisterPlayer.h 
// Written By  : reiot@ewestsoft.com
// Description : 
// 
//////////////////////////////////////////////////////////////////////

#ifndef __CL_REGISTER_PLAYER_H__
#define __CL_REGISTER_PLAYER_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"
#include "PlayerInfo.h"

//////////////////////////////////////////////////////////////////////
//
// class CLRegisterPlayer;
//
// 클라이언트가 로그인 서버에게 최초에 전송하는 패킷이다.
// 아이디와 패스워드가 암호화되어 있다.
//
//////////////////////////////////////////////////////////////////////

class CLRegisterPlayer : public Packet {

public :
	
    // 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
    void read ( SocketInputStream & iStream ) throw ( ProtocolException , Error );
		    
    // 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
    void write ( SocketOutputStream & oStream ) const throw ( ProtocolException , Error );

	// execute packet's handler
	void execute ( Player * pPlayer ) throw ( ProtocolException , Error );

	// get packet id
	PacketID_t getPacketID () const throw () { return PACKET_CL_REGISTER_PLAYER; }
	
	// get packet's body size
	PacketSize_t getPacketSize () const throw ()
	{
		// 최적화시 미리 계산된 상수를 사용하도록 한다.
		return    szBYTE + m_ID.size() 			// 아이디
				+ szBYTE + m_Password.size() 	// 암호
				+ szBYTE + m_Name.size() 		// 이름
				+ szBYTE						// 성별
				+ szBYTE + m_SSN.size() 		// 주민등록번호
				+ szBYTE + m_Telephone.size() 	// 전화번호
				+ szBYTE + m_Cellular.size() 	// 휴대폰번호
				+ szBYTE + m_ZipCode.size() 	// 우편번호
				+ szBYTE + m_Address.size() 	// 주소
				+ szBYTE 						// 국가코드
				+ szBYTE + m_Email.size() 		// 전자메일
				+ szBYTE + m_Homepage.size() 	// 홈페이지
				+ szBYTE + m_Profile.size() 	// 자기소개글
				+ szBYTE;						// 공개여부
	}

#ifdef __DEBUG_OUTPUT__
	// get packet name
	std::string getPacketName () const throw () { return "CLRegisterPlayer"; }
	
	// get packet's debug string
	std::string toString () const throw ();
#endif

public :

	//----------------------------------------------------------------------
	// *CAUTION* 
	// 각 setXXX()들은 최대 길이를 체크해서 truncate 하지만, 최소길이는 
	// 체크하지 않는다. 최소 길이는 read()/write() 에서 체크된다. 
	//----------------------------------------------------------------------

    // get/set player's id
	std::string getID () const throw () { return m_ID; }
	void setID ( std::string id ) throw () { m_ID = ( id.size() > PlayerInfo::maxIDLength ) ? id.substr(0,PlayerInfo::maxIDLength) : id; }

    // get/set player's password
    std::string getPassword () const throw () { return m_Password; }
    void setPassword ( std::string password ) throw () { m_Password = ( password.size() > PlayerInfo::maxPasswordLength ) ? password.substr(0,PlayerInfo::maxPasswordLength) : password; }

    // get/set player's name
    std::string getName () const throw () { return m_Name; }
    void setName ( std::string name ) throw () { m_Name = ( name.size() > PlayerInfo::maxNameLength ) ? name.substr(0,PlayerInfo::maxNameLength) : name; }

    // get/set player's sex
    Sex getSex () const throw () { return m_Sex; }
    void setSex ( Sex sex ) throw () { m_Sex = sex; }

    // get/set player's ssn
    std::string getSSN () const throw () { return m_SSN; }
    void setSSN ( std::string ssn ) throw () { m_SSN = ( ssn.size() > PlayerInfo::maxSSNLength ) ? ssn.substr(0,PlayerInfo::maxSSNLength) : ssn; }

    // get/set player's telephone
    std::string getTelephone () const throw () { return m_Telephone; }
    void setTelephone ( std::string telephone ) throw () { m_Telephone = ( telephone.size() > PlayerInfo::maxTelephoneLength ) ? telephone.substr(0,PlayerInfo::maxTelephoneLength) : telephone; }

    // get/set player's cellular
    std::string getCellular () const throw () { return m_Cellular; }
    void setCellular ( std::string cellular ) throw () { m_Cellular = ( cellular.size() > PlayerInfo::maxCellularLength ) ? cellular.substr(0,PlayerInfo::maxCellularLength) : cellular; }

    // get/set player's zipcode
    std::string getZipCode () const throw () { return m_ZipCode; }
    void setZipCode ( std::string zipcode ) throw () { m_ZipCode = ( zipcode.size() > PlayerInfo::maxZipCodeLength ) ? zipcode.substr(0,PlayerInfo::maxZipCodeLength) : zipcode; }

    // get/set player's address
    std::string getAddress () const throw () { return m_Address; }
    void setAddress ( std::string address ) throw () { m_Address = ( address.size() > PlayerInfo::maxAddressLength ) ? address.substr(0,PlayerInfo::maxAddressLength) : address; }

    // get/set player's nation
    Nation getNation () const throw () { return m_Nation; }
    void setNation ( Nation nation ) throw () { m_Nation = nation; }

    // get/set player's email
    std::string getEmail () const throw () { return m_Email; }
    void setEmail ( std::string email ) throw () { m_Email = ( email.size() > PlayerInfo::maxEmailLength ) ? email.substr(0,PlayerInfo::maxEmailLength) : email; }

    // get/set player's homepage
    std::string getHomepage () const throw () { return m_Homepage; }
    void setHomepage ( std::string homepage ) throw () { m_Homepage = ( homepage.size() > PlayerInfo::maxHomepageLength ) ? homepage.substr(0,PlayerInfo::maxHomepageLength) : homepage; }

    // get/set player's profile
    std::string getProfile () const throw () { return m_Profile; }
    void setProfile ( std::string profile ) throw () { m_Profile = ( profile.size() > PlayerInfo::maxProfileLength ) ? profile.substr(0,PlayerInfo::maxProfileLength) : profile; }

	// get/set player info's publicability (?) 
	bool getPublic () const throw () { return m_bPublic; }
	void setPublic ( bool bPublic ) throw () { m_bPublic = bPublic; }

private :

	//--------------------------------------------------
	// 플레이어 기본 정보
	//--------------------------------------------------
    std::string m_ID; 			// 아이디
    std::string m_Password; 		// 패스워드
	//--------------------------------------------------
	// 플레이어 개인 정보
	//--------------------------------------------------
    std::string m_Name; 			// 이름
    Sex m_Sex; 				// 성별
    std::string m_SSN; 			// 주민등록번호
	//--------------------------------------------------
	// 플레이어 연락처/주소
	//--------------------------------------------------
    std::string m_Telephone; 	// 전화번호
    std::string m_Cellular; 		// 핸드폰
    std::string m_ZipCode; 		// 우편번호
    std::string m_Address; 		// 주소
    Nation m_Nation; 		// 국가 코드
	//--------------------------------------------------
	// 플레이어 전자정보
	//--------------------------------------------------
    std::string m_Email; 		// 전자메일
    std::string m_Homepage; 		// 홈페이지
	//--------------------------------------------------
	// 기타 
	//--------------------------------------------------
    std::string m_Profile; 		// 하고픈말
	bool m_bPublic; 		// 공개 여부

};


//////////////////////////////////////////////////////////////////////
//
// class CLRegisterPlayerFactory;
//
// Factory for CLRegisterPlayer
//
//////////////////////////////////////////////////////////////////////
#ifdef __DEBUG_OUTPUT__
class CLRegisterPlayerFactory : public PacketFactory {

public :
	
	// create packet
	Packet * createPacket () throw () { return new CLRegisterPlayer(); }

	// get packet name
	std::string getPacketName () const throw () { return "CLRegisterPlayer"; }

	// get packet id
	PacketID_t getPacketID () const throw () { return Packet::PACKET_CL_REGISTER_PLAYER; }

	// get packet's max body size
	PacketSize_t getPacketMaxSize () const throw ()
	{
		// 최적화시 미리 계산된 상수를 사용하도록 한다.
		return    szBYTE + PlayerInfo::maxIDLength 			// 아이디
				+ szBYTE + PlayerInfo::maxPasswordLength 	// 패스워드
				+ szBYTE + PlayerInfo::maxNameLength 		// 이름
				+ szBYTE									// 성별
				+ szBYTE + PlayerInfo::maxSSNLength 		// 주민등록번호
				+ szBYTE + PlayerInfo::maxTelephoneLength 	// 전화번호
				+ szBYTE + PlayerInfo::maxCellularLength 	// 휴대폰번호
				+ szBYTE + PlayerInfo::maxZipCodeLength 	// 우편번호
				+ szBYTE + PlayerInfo::maxAddressLength 	// 주소
				+ szBYTE 									// 국가코드
				+ szBYTE + PlayerInfo::maxEmailLength 		// 전자메일
				+ szBYTE + PlayerInfo::maxHomepageLength 	// 홈페이지
				+ szBYTE + PlayerInfo::maxProfileLength 	// 자기소개
				+ szBYTE;									// 공개여부
	}


};

#endif


//////////////////////////////////////////////////////////////////////
//
// class CLRegisterPlayerHandler;
//
//////////////////////////////////////////////////////////////////////
#ifndef __GAME_CLIENT__
	class CLRegisterPlayerHandler {

	public :

		// execute packet's handler
		static void execute ( CLRegisterPlayer * pPacket , Player * pPlayer ) throw ( ProtocolException , Error );

	};
#endif

#endif
