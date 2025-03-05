//----------------------------------------------------------------------
// UserOption.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "Client_PCH.h"
#include "UserOption.h"
#include "KeyAccelerator.h"
#include <DInput.h>
#include <cstdio>


//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
UserOption*		g_pUserOption = NULL;

//----------------------------------------------------------------------
// 
// constructor
//
//----------------------------------------------------------------------
UserOption::UserOption()
{	
	Use3DHAL			= FALSE;
	UseSmoothCursor		= FALSE;//TRUE;
	DrawMinimap			= TRUE;		// minimap을 그릴까?
	DrawZoneName		= TRUE;		// Zone이름 출력
	DrawGameTime		= TRUE;		// 게임 시간 출력
	DrawInterface		= FALSE;		// interface
	DrawFPS				= FALSE;		// FPS
	BlendingShadow		= FALSE;//TRUE;			// 그림자 반투명
	FilteringCurse		= TRUE;		// 나쁜 말 제거
	PlayMusic			= TRUE;		// 음악 출력
	PlaySound			= TRUE;		// 효과음 출력
	VolumeMusic			= 15;		// 음악 소리크기
	VolumeSound			= 15;		// 효과음 소리 크기
	UseHelpEvent		= TRUE;		// 도움말 사용
	PlayWaveMusic		= TRUE;		// Wav로 음악 출력하기(아니면 MID로)
	BloodDrop			= TRUE;		// HP 낮을 때 피 흘리기
	OpenQuickSlot		= FALSE;
	UseHalfFrame		= FALSE;
	DrawTransHPBar		= TRUE;
	UseForceFeel		= FALSE;
	UseGammaControl		= TRUE;
	GammaValue			= 100;
	DrawChatBoxOutline	= TRUE;

	//new interface
	BackupID[0]			= '\0';
	UseEnterChat		= FALSE;
	UseMouseSpeed		= FALSE;
	MouseSpeedValue		= 50;
	PlayYellSound		= TRUE;
	ShowChoboHelp		= TRUE;
	TribeChange			= FALSE;
	DenyPartyInvite		= FALSE;
	DenyPartyRequest	= FALSE;
	AutoHideSmoothScroll = TRUE;
	ChattingColor		= RGB(198, 195, 198);
	ALPHA_DEPTH			= 23;
	DefaultAlpha		= FALSE;
	IsPreLoadMonster	= TRUE;
	ChatWhite			= FALSE;
	UseTeenVersion		= FALSE;				// 틴버전으로 게임하기
	PopupChatByWhisper	= TRUE;			// 귓속말 왔을때 채팅창 잠깐 보이기
	NotSendMyInfo = FALSE;
	DoNotShowWarMsg = FALSE;
	DoNotShowLairMsg = FALSE;
	DoNotShowHolyLandMsg = FALSE;
	Chinese = FALSE;
	Korean = TRUE;
	Japanese = FALSE;
	English = FALSE;
	persnalShopupdatetime = 0;
	ShowGameMoneyWithHANGUL = FALSE;
	DoNotShowPersnalShopMsg = FALSE;
}

UserOption::~UserOption()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
void	
UserOption::SaveToFile(const char* filename)
{
	// ofstream file(filename, ios::binary);	
	FILE* file = fopen(filename, "w");

	DWORD flag = 0;
	fwrite((void*)&flag, 1, 4, file);
	g_pKeyAccelerator->SaveToFile(file);

	fprintf(file, "\n========\n");

	fprintf(file, "%d	UseSmoothCursor\n", UseSmoothCursor);
	fprintf(file, "%d	DrawMinimap\n", DrawMinimap);
	fprintf(file, "%d	DrawGameTime\n", DrawGameTime);
	fprintf(file, "%d	DrawInterface\n", DrawInterface);
	fprintf(file, "%d	DrawFPS\n", DrawFPS);
	fprintf(file, "%d	BlendingShadow\n", BlendingShadow);
	fprintf(file, "%d	FilteringCurse\n", FilteringCurse);
	fprintf(file, "%d	PlayMusic\n", PlayMusic);
	fprintf(file, "%d	PlaySound\n", PlaySound);
	fprintf(file, "%d	VolumeMusic\n", VolumeMusic);
	fprintf(file, "%d	VolumeSound\n", VolumeSound);
	fprintf(file, "%d	UseHelpEvent\n", UseHelpEvent);
	fprintf(file, "%d	PlayWaveMusic\n", PlayWaveMusic);
	fprintf(file, "%d	BloodDrop\n", BloodDrop);
	fprintf(file, "%d	OpenQuickSlot\n", OpenQuickSlot);
	fprintf(file, "%d	UseHalfFrame\n", UseHalfFrame);
	fprintf(file, "%d	Use3DHAL\n", Use3DHAL);
	fprintf(file, "%d	DrawTransHPBar\n", DrawTransHPBar);
	fprintf(file, "%d	UseForceFeel\n", UseForceFeel);
	fprintf(file, "%d	GammaValue\n", GammaValue);
	fprintf(file, "%d	DrawChatBoxOutline\n", DrawChatBoxOutline);

	// new interface
	fwrite((const void*)BackupID, 15, 1, file);
	fprintf(file, "%d	UseEnterChat\n", UseEnterChat);
	fprintf(file, "%d	MouseSpeedValue\n", MouseSpeedValue);
	fprintf(file, "%d	PlayYellSound\n", PlayYellSound);
	fprintf(file, "%d	ShowChoboHelp\n", ShowChoboHelp);
	fprintf(file, "%d	TribeChange\n", TribeChange);
	fprintf(file, "%d	DenyPartyInvite\n", DenyPartyInvite);
	fprintf(file, "%d	DenyPartyRequest\n", DenyPartyRequest);
	fprintf(file, "%d	AutoHideSmoothScroll\n", AutoHideSmoothScroll);
	fprintf(file, "%d	ChattingColor\n", ChattingColor);
	fprintf(file, "%d	ALPHA_DEPTH\n", ALPHA_DEPTH);
	fprintf(file, "%d	DefaultAlpha\n", DefaultAlpha);
	fprintf(file, "%d	IsPreLoadMonster\n", IsPreLoadMonster);
	fprintf(file, "%d	ChatWhite\n", ChatWhite);
	fprintf(file, "%d	UseTeenVersion\n", UseTeenVersion);
	fprintf(file, "%d	PopupChatByWhisper\n", PopupChatByWhisper);
	fprintf(file, "%d	NotSendMyInfo\n", NotSendMyInfo);
	fprintf(file, "%d	DoNotShowWarMsg\n", DoNotShowWarMsg);
	fprintf(file, "%d	DoNotShowLairMsg\n", DoNotShowLairMsg);
	fprintf(file, "%d	DoNotShowHolyLandMsg\n", DoNotShowHolyLandMsg);
	fprintf(file, "%d	ShowGameMoneyWithHANGUL\n", ShowGameMoneyWithHANGUL);
	fprintf(file, "%d	DoNotShowPersnalShopMsg\n", DoNotShowPersnalShopMsg);

	fclose(file);
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
bool	
UserOption::LoadFromFile(const char* filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		return false;
	}
	
	DWORD flag = 0;
	fread((void*)&flag, 1, 4, file);
	g_pKeyAccelerator->LoadFromFile(file);
	
//	if(flag == 0)
//	{
//		g_pKeyAccelerator->SetAcceleratorKey(ACCEL_GRADE1INFO, DIK_R);
//		file.seekg(-2, ios::cur);
//	}

	char ignore[256];
	fscanf(file, "\n%s\n", ignore); // ignore =======

	fscanf(file, "%d	%s\n", &UseSmoothCursor, ignore);
	fscanf(file, "%d	%s\n", &DrawMinimap, ignore);
	fscanf(file, "%d	%s\n", &DrawGameTime, ignore);
	fscanf(file, "%d	%s\n", &DrawInterface, ignore);
	fscanf(file, "%d	%s\n", &DrawFPS, ignore);
	fscanf(file, "%d	%s\n", &BlendingShadow, ignore);
	fscanf(file, "%d	%s\n", &FilteringCurse, ignore);
	fscanf(file, "%d	%s\n", &PlayMusic, ignore);
	fscanf(file, "%d	%s\n", &PlaySound, ignore);
	fscanf(file, "%d	%s\n", &VolumeMusic, ignore);
	fscanf(file, "%d	%s\n", &VolumeSound, ignore);
	fscanf(file, "%d	%s\n", &UseHelpEvent, ignore);
	fscanf(file, "%d	%s\n", &PlayWaveMusic, ignore);
	fscanf(file, "%d	%s\n", &BloodDrop, ignore);
	fscanf(file, "%d	%s\n", &OpenQuickSlot, ignore);
	fscanf(file, "%d	%s\n", &UseHalfFrame, ignore);
	fscanf(file, "%d	%s\n", &Use3DHAL, ignore);
	fscanf(file, "%d	%s\n", &DrawTransHPBar, ignore);
	fscanf(file, "%d	%s\n", &UseForceFeel, ignore);
	fscanf(file, "%d	%s\n", &GammaValue, ignore);
	fscanf(file, "%d	%s\n", &DrawChatBoxOutline, ignore);

	// new interface
	fread((void*)BackupID, 15, 1, file);
	fscanf(file, "%d %s\n", &UseEnterChat, ignore);
	fscanf(file, "%d %s\n", &MouseSpeedValue, ignore);
	fscanf(file, "%d %s\n", &PlayYellSound, ignore);
	fscanf(file, "%d %s\n", &ShowChoboHelp, ignore);
	fscanf(file, "%d %s\n", &TribeChange, ignore);
	fscanf(file, "%d %s\n", &DenyPartyInvite, ignore);
	fscanf(file, "%d %s\n", &DenyPartyRequest, ignore);
	fscanf(file, "%d %s\n", &AutoHideSmoothScroll, ignore);
	fscanf(file, "%d %s\n", &ChattingColor, ignore);
	fscanf(file, "%d %s\n", &ALPHA_DEPTH, ignore);
	fscanf(file, "%d %s\n", &DefaultAlpha, ignore);
	fscanf(file, "%d %s\n", &IsPreLoadMonster, ignore);
	fscanf(file, "%d %s\n", &ChatWhite, ignore);
	fscanf(file, "%d %s\n", &UseTeenVersion, ignore);
	fscanf(file, "%d %s\n", &PopupChatByWhisper, ignore);
	fscanf(file, "%d %s\n", &NotSendMyInfo, ignore);
	fscanf(file, "%d %s\n", &DoNotShowWarMsg, ignore);
	fscanf(file, "%d %s\n", &DoNotShowLairMsg, ignore);
	fscanf(file, "%d %s\n", &DoNotShowHolyLandMsg, ignore);
	fscanf(file, "%d %s\n", &ShowGameMoneyWithHANGUL, ignore);
	fscanf(file, "%d %s\n", &DoNotShowPersnalShopMsg, ignore);

	fclose(file);
	return true;
}
