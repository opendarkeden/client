//----------------------------------------------------------------------
// UserInformation.h
//----------------------------------------------------------------------
// Client의 정보 중에서 변할 수 있는 것
//----------------------------------------------------------------------

#ifndef	__USERINFORMATION_H__
#define	__USERINFORMATION_H__

#include <Windows.h>
#include <vector>
#include <bitset>
#include "MString.h"
#include "RaceType.h"

struct WAR_INFO
{
	std::string zone_name;
	std::string attack_guild_name, defense_guild_name;
	WORD	zone_id;
	DWORD left_time;
	BYTE war_type;
};

class MPetItem;

class UserInformation {	
	public :
		UserInformation();
		~UserInformation();

		//--------------------------------------------------------
		// File I/O
		//--------------------------------------------------------
		//void	SaveToFile(const char* filename);
		//bool	LoadFromFile(const char* filename);

	public :

		// 조합형 한글
		MString			UserID;				// 사용자의 ID
		MString			Character[3];		// 캐릭터 이름
		Race			Race[3];		// slayer 캐릭터인가?
		MString			CharacterID;		// 선택된 Character ID
		MString			GuildName;			// 내 길드 이름
		BYTE			GuildGrade;			// 내 길드 안에서의 등급
		int				PCSNumber;			// 내 pcs 번호
		int				FaceStyle;			// 얼굴 모양
		int				FaceStyleSlot[3];	// 선택창에서 얼굴 스타일
		int				HairColor;
		int				SkinColor;
		int				BatColor;
		MString			WhisperID;			// 방금 전의 귓속말 대상

		MString			PCSUserName[3];		// 사용자 이름
		int				OtherPCSNumber[3];		// 접속중인 번호

		// 완성형 한글
		//MString			UserIDW;			// 사용자의 ID
		//MString			CharacterW[3];		// 사용자의 ID		
		//MString			CharacterIDW;		// 사용자의 Character ID

		//
		int				Slot;				// 선택할려는 Slot
//		bool			Invisible;			// 투명 캐릭터(Super User용?)

		DWORD			GlobalSayTime;		// 마지막으로 외치기~한 시간.

		bool			GoreLevel;			// 피 튀게 할까?

		BOOL			KeepConnection;		// 접속 유지해야하는 상태인가?
		BOOL			IsMaster;			// 운영자인가?
		DWORD			ItemDropEnableTime;	// item을 떨어뜨려도 되는 시간
		bool			HasSkillRestore;	// Restore 사용할 수 있는가?
		DWORD			LogoutTime;			// Logout이 가능한 시간
		DWORD			GameVersion;		// Game의 Version
		bool			HasMagicGroundAttack;	// 불기둥 사용할 수 있는가?
		bool			HasMagicHallu;			// Hallu 사용할 수 있는가?
		bool			HasMagicBloodySnake;			// Bloody Snake 사용할 수 있는가?
		bool			HasMagicBloodyWarp;			// Bloody Warp 사용할 수 있는가?

		// 넷마블용
		bool			IsNetmarble;		// NetMarble 용 클라이언트 인가
		MString			NetmarbleID;
		MString			NetmarblePassword;
		bool			bNetmarbleGoreLevel;

		int				WorldID;
		int				ServerID;

		std::vector<WAR_INFO>		WarInfo;
		
		bool			bMetrotech;
		MString			WorldName;

		bool			bChinese;
		bool			bKorean;
		bool			bJapanese;
		bool			bEnglish;

		void			SetKorean() { bKorean = true; bChinese=bJapanese=bEnglish=false;}
		void			SetChinese() { bChinese = true; bKorean=bJapanese=bEnglish=false;}
		void			SetJapanese() { bJapanese = true; bKorean=bChinese=bEnglish=false;}
		
		// 머리가격 조정용
		int				HeadPrice;

		bool			bCompetence;
		bool			bCompetenceShape;
		
		// 2004, 7, 15, sobeit add start
		bool			IsAutoLogIn;		// 자동 로그인(웹로그인) 모드인지 아닌지
		MString			AutoLogInKeyValue;	// 자동 로그인(웹로그인) 모드일때 key 값
		void*			pLogInClientPlayer;
		// 2004, 7, 15, sobeit add end
		
		// 2004, 9, 14, sobeit add start
		bool			IsNonPK;
		// 2004, 9, 14, sobeit add end
		// 2004, 10, 18, sobeit add start
		DWORD			dwUnionID;
		BYTE			bUnionGrade; // 0: 유니온 마스트 1: 일반 마스터 3: nothing
		// 2004, 10, 18, sobeit add end
		
		// 2004, 12, 14, sobeit add start
		bool			IsTestServer;
		// 2004, 12, 14, sobeit add end

};


extern	UserInformation	*	g_pUserInformation;

#endif

