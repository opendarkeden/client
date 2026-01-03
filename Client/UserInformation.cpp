//----------------------------------------------------------------------
// UserInformation.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "UserInformation.h"
//#include <fstream.h>

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
UserInformation	*	g_pUserInformation  = NULL;

//----------------------------------------------------------------------
// 
// constructor
//
//----------------------------------------------------------------------
UserInformation::UserInformation()
{
	Slot		= 0;
//	Invisible	= false;
	
	PCSNumber	= 0;
	OtherPCSNumber[0] = 0;
	OtherPCSNumber[1] = 0;
	OtherPCSNumber[2] = 0;

	FaceStyleSlot[0] = 0;
	FaceStyleSlot[1] = 0;
	FaceStyleSlot[2] = 0;

	HairColor = 0;
	SkinColor = 0;

	FaceStyle = 0;

	GlobalSayTime = 0;

	GoreLevel = true;

	KeepConnection = FALSE;
	IsMaster = FALSE;
	ItemDropEnableTime = 0;
	HasSkillRestore = false;
	HasMagicGroundAttack = false;
	HasMagicHallu = false;
	HasMagicBloodyWarp = false;
	HasMagicBloodySnake = false;

	// 0이면 시간이 설정되어 있지 않는거다.
	LogoutTime = 0;

	GameVersion = 0;

	// 넷마블용
	IsNetmarble = false;
	WorldID = 0;
	ServerID = 0;
	
	bMetrotech = false;

	WarInfo.clear();

	bChinese = false;
	bKorean = true;
	bJapanese = false;
	bEnglish = false;
	
	// 머리가격 기본값 100%다.
	HeadPrice = 100;
	bCompetence = false;
	bCompetenceShape = false;
	
	IsAutoLogIn = false;
	pLogInClientPlayer = NULL;
	IsNonPK = false;
	dwUnionID = 0;
	bUnionGrade = 0;
	IsTestServer = false;
}

UserInformation::~UserInformation()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

