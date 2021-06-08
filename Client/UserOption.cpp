//----------------------------------------------------------------------
// UserOption.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "Client_PCH.h"
#include "UserOption.h"
#include "KeyAccelerator.h"
#include <DInput.h>


//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
UserOption*		g_pUserOption = NULL;

//----------------------------------------------------------------------
// define functions
//----------------------------------------------------------------------
#define READ_CHECK_EOF( value, temp, bytes )		\
		{											\
			file.read((char*)&temp, bytes);			\
			if (!file.eof())						\
			{										\
				value = temp;						\
			}										\
			else									\
			{										\
				file.close();						\
				return true;						\
			}										\
		}

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
	class ofstream file(filename, ios::binary);	

	DWORD flag = 0;
	file.write((const char*)&flag, 4);
	g_pKeyAccelerator->SaveToFile(file);

	file.write((const char*)&UseSmoothCursor, 4);
	file.write((const char*)&DrawMinimap, 4);
	file.write((const char*)&DrawZoneName, 4);
	file.write((const char*)&DrawGameTime, 4);
	file.write((const char*)&DrawInterface, 4);
	file.write((const char*)&DrawFPS, 4);
	file.write((const char*)&BlendingShadow, 4);
	file.write((const char*)&FilteringCurse, 4);
	file.write((const char*)&PlayMusic, 4);
	file.write((const char*)&PlaySound, 4);
	file.write((const char*)&VolumeMusic, 4);
	file.write((const char*)&VolumeSound, 4);
	file.write((const char*)&UseHelpEvent, 4);
	file.write((const char*)&PlayWaveMusic, 4);
	file.write((const char*)&BloodDrop, 4);
	file.write((const char*)&OpenQuickSlot, 4);	
	file.write((const char*)&UseHalfFrame, 4);	
	file.write((const char*)&Use3DHAL, 4);
	file.write((const char*)&DrawTransHPBar, 4);	
	file.write((const char*)&UseForceFeel, 4);
	file.write((const char*)&UseGammaControl, 4);	
	file.write((const char*)&GammaValue, 4);	
	file.write((const char*)&DrawChatBoxOutline, 4);
	
	// new interface
	file.write((const char*)BackupID, 15);
	file.write((const char*)&UseEnterChat, 4);
	file.write((const char*)&UseMouseSpeed, 4);
	file.write((const char*)&MouseSpeedValue, 4);
	file.write((const char*)&PlayYellSound, 4);
	file.write((const char*)&ShowChoboHelp, 4);
	file.write((const char*)&TribeChange, 4);
	file.write((const char*)&DenyPartyInvite, 4);
	file.write((const char*)&DenyPartyRequest, 4);
	file.write((const char*)&AutoHideSmoothScroll, 4);
	file.write((const char*)&ChattingColor, 4);
	file.write((const char*)&ALPHA_DEPTH, 1);
	file.write((const char*)&DefaultAlpha, 4);
	file.write((const char*)&IsPreLoadMonster, 4);
	file.write((const char*)&ChatWhite, 4);
	file.write((const char*)&UseTeenVersion, 4);
	file.write((const char*)&PopupChatByWhisper, 4);
	file.write((const char*)&NotSendMyInfo,4);
	file.write((const char*)&DoNotShowWarMsg,4);
	file.write((const char*)&DoNotShowLairMsg,4);
	file.write((const char*)&DoNotShowHolyLandMsg,4);
	file.write((const char*)&ShowGameMoneyWithHANGUL,4);
	file.write((const char*)&DoNotShowPersnalShopMsg,4);

	

	file.close();
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
bool	
UserOption::LoadFromFile(const char* filename)
{
	class ifstream file(filename, ios::binary | ios::nocreate);	

	if (!file || !file.is_open())
	{
		return false;
	}
	
	DWORD flag = 0;
	file.read((char*)&flag, 4);
	g_pKeyAccelerator->LoadFromFile(file);
	
//	if(flag == 0)
//	{
//		g_pKeyAccelerator->SetAcceleratorKey(ACCEL_GRADE1INFO, DIK_R);
//		file.seekg(-2, ios::cur);
//	}

	DWORD temp;
	
	READ_CHECK_EOF( UseSmoothCursor, temp, 4 );
	READ_CHECK_EOF( DrawMinimap, temp, 4 );
	READ_CHECK_EOF( DrawZoneName, temp, 4 );
	READ_CHECK_EOF( DrawGameTime, temp, 4 );
	READ_CHECK_EOF( DrawInterface, temp, 4 );
	READ_CHECK_EOF( DrawFPS, temp, 4 );
	READ_CHECK_EOF( BlendingShadow, temp, 4 );
	READ_CHECK_EOF( FilteringCurse, temp, 4 );
	READ_CHECK_EOF( PlayMusic, temp, 4 );
	READ_CHECK_EOF( PlaySound, temp, 4 );
	READ_CHECK_EOF( VolumeMusic, temp, 4 );
	READ_CHECK_EOF( VolumeSound, temp, 4 );	
	READ_CHECK_EOF( UseHelpEvent, temp, 4 );
	READ_CHECK_EOF( PlayWaveMusic, temp, 4 );
	READ_CHECK_EOF( BloodDrop, temp, 4 );	
	READ_CHECK_EOF( OpenQuickSlot, temp, 4 );		
	READ_CHECK_EOF( UseHalfFrame, temp, 4 );	
	READ_CHECK_EOF( Use3DHAL, temp, 4);
	READ_CHECK_EOF( DrawTransHPBar, temp, 4);	
	READ_CHECK_EOF( UseForceFeel, temp, 4);	
	READ_CHECK_EOF( UseGammaControl, temp, 4);		
	READ_CHECK_EOF( GammaValue, temp, 4);	
	READ_CHECK_EOF( DrawChatBoxOutline, temp, 4);
	
	// new interface
//	READ_CHECK_EOF( *BackupID, temp, 11);
	file.read((char *)BackupID, 15);
	READ_CHECK_EOF( UseEnterChat, temp, 4);
	READ_CHECK_EOF( UseMouseSpeed, temp, 4);
	READ_CHECK_EOF( MouseSpeedValue, temp, 4);
	READ_CHECK_EOF( PlayYellSound, temp, 4);
	READ_CHECK_EOF( ShowChoboHelp, temp, 4);
	READ_CHECK_EOF( TribeChange, temp, 4);
	READ_CHECK_EOF( DenyPartyInvite, temp, 4);
	READ_CHECK_EOF( DenyPartyRequest, temp, 4);
	READ_CHECK_EOF( AutoHideSmoothScroll, temp, 4);
	READ_CHECK_EOF( ChattingColor, temp, 4);
	READ_CHECK_EOF( ALPHA_DEPTH, temp, 1);
	READ_CHECK_EOF( DefaultAlpha, temp, 4);
	READ_CHECK_EOF( IsPreLoadMonster, temp, 4);
	READ_CHECK_EOF( ChatWhite, temp, 4);
	READ_CHECK_EOF( UseTeenVersion, temp, 4);
	READ_CHECK_EOF( PopupChatByWhisper, temp, 4);
	READ_CHECK_EOF( NotSendMyInfo, temp, 4);
	READ_CHECK_EOF( DoNotShowWarMsg, temp, 4);
	READ_CHECK_EOF( DoNotShowLairMsg, temp, 4);
	READ_CHECK_EOF( DoNotShowHolyLandMsg, temp, 4);
	READ_CHECK_EOF( ShowGameMoneyWithHANGUL, temp, 4);
	READ_CHECK_EOF( DoNotShowPersnalShopMsg, temp, 4);

	
	file.close();

	return true;
}
