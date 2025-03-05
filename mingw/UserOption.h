//----------------------------------------------------------------------
// UserOption.h
//----------------------------------------------------------------------
// 사용자가 바꿀 수 있는 것
//----------------------------------------------------------------------

#ifndef	__USEROPTION_H__
#define	__USEROPTION_H__

#include <Windows.h>

class UserOption {
	public :
		UserOption();
		~UserOption();

		//--------------------------------------------------------
		// File I/O
		//--------------------------------------------------------
		void	SaveToFile(const char* filename);
		bool	LoadFromFile(const char* filename);

	public :
		BOOL	Chinese;
		BOOL	Korean;
		BOOL	Japanese;
		BOOL	English;
		BOOL	Use3DHAL;					// 3D가속 사용할 수 있는지 테스트 한다.
		BOOL	UseSmoothCursor;			// 부드러운(?) cursor를 사용할까?
		BOOL	DrawMinimap;				// minimap을 그릴까?
		BOOL	DrawZoneName;				// Zone이름 출력
		BOOL	DrawGameTime;				/// 게임 시간 출력
		BOOL	DrawInterface;				// interface를 출력할까?
		BOOL	DrawFPS;					// FPS를 출력할까?
		BOOL	BlendingShadow;				// 그림자를 반투명해서 출력할까?
		BOOL	FilteringCurse;				// 나쁜 말 제거 할까?
		BOOL	PlayMusic;					// 노래 출력할까?
		BOOL	PlaySound;					// 효과음 출력할까?
		int		VolumeMusic;				// 음악 볼륨
		int		VolumeSound;				// 효과음 볼륨
		BOOL	UseHelpEvent;				// HelpEvent를 사용하는가?
		BOOL	PlayWaveMusic;				// wave로 음악을 듣는가?
		BOOL	BloodDrop;					// HP 낮을 때 피 흘리기
		BOOL	OpenQuickSlot;				// QuickSlot은 열려있는가?
		BOOL	UseHalfFrame;				// Frame 줄인걸 보여줄까?
		BOOL	DrawTransHPBar;				// 투명한 HP 박스인가?
		BOOL	UseForceFeel;				// 진동 마우스 사용?
		BOOL	UseGammaControl;			// 감마 값 변경하는걸 사용하는가?
		BOOL	GammaValue;					// 감마 값( 50 ~ 100(기본) ~ 150(밝음) )
		BOOL	DrawChatBoxOutline;			// 캐릭터 머리 위에 뜨는 채팅글자 외곽에 선 그리기
		
		// new interface
		char	BackupID[11];
		BOOL	UseEnterChat;
		BOOL	UseMouseSpeed;
		int		MouseSpeedValue;
		BOOL	PlayYellSound;
		BOOL	ShowChoboHelp;
		BOOL	TribeChange;
		BOOL	DenyPartyInvite;
		BOOL	DenyPartyRequest;
		BOOL	AutoHideSmoothScroll;		// 창 자동숨김 부드럽게 하는가?
		COLORREF ChattingColor;
		BYTE	ALPHA_DEPTH;
		BOOL	DefaultAlpha;
		BOOL	IsPreLoadMonster;
		BOOL	ChatWhite;					// 채팅 모두 흰색으로 보기
		BOOL	UseTeenVersion;				// 틴버전으로 게임하기
		BOOL	PopupChatByWhisper;			// 귓속말 왔을때 채팅창 잠깐 보이기
		BOOL	NotSendMyInfo;				// 내 정보 남에게 보이지 않기
		BOOL	DoNotShowWarMsg;			// 전쟁메시지 보지 않기
		BOOL	DoNotShowLairMsg;			// 레어마스터 메시지 보지 않기 
		BOOL	DoNotShowHolyLandMsg;		// 아담의 성지 메시지 보지 않기
		int		persnalShopupdatetime;
		// 2004, 12, 21, sobeit add start - 게임머니 보이는 방식
		BOOL	ShowGameMoneyWithHANGUL;
		// 2004, 12, 21, sobeit add end
		// 2005, 1, 20, sobeit add start - 개인상점 메세지 안보기
		BOOL	DoNotShowPersnalShopMsg;
		// 2005, 1, 20, sobeit add end
};


extern	UserOption*		g_pUserOption;

#endif

