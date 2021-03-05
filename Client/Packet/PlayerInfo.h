//----------------------------------------------------------------------
//
// Filename    : PlayerInfo.h
// Written By  : Reiot
// Description :
//
//----------------------------------------------------------------------

#ifndef __PLAYER_INFO_H__
#define __PLAYER_INFO_H__

// include files
#include "Types.h"
#include "Exception.h"

//----------------------------------------------------------------------
//
// class PlayerInfo;
//
// 플레이어 등록시 각 데이타 멤버들의 최대 최소 길이 같은 정보를 담는다.
//
//----------------------------------------------------------------------
namespace PlayerInfo {

	// 플레이어 아이디
	const uint minIDLength = 4;
	const uint maxIDLength = 10;

	// 플레이어 암호
	const uint minPasswordLength = 6;
	const uint maxPasswordLength = 10;

	// 플레이어 이름
	const uint maxNameLength = 20;

	// 주민등록번호 ( '-' 를 포함해야 한다. )
	const uint maxSSNLength = 20;

	// 집전화
	const uint maxTelephoneLength = 15;

	// 휴대폰
	const uint maxCellularLength = 15;

	// 우편번호
	const uint maxZipCodeLength = 7;

	// 집주소
	const uint maxAddressLength = 100;

	// 전자메일
	const uint maxEmailLength = 50;

	// 홈페이지
	const uint maxHomepageLength = 50;

	// 자기소개글
	const uint maxProfileLength = 200;

};

#endif
