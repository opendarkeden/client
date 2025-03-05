#include "DirectXlib_Pch.h"
#include "Client_PCH.h"
#include <windows.h>
#include "dxlib\CDirectSound.h"
#include "CMP3.h"
#include "DebugInfo.h"

//#include "mp3lib\mp3.h"
//
//MP3 *CMP3::mp3 = NULL ;
//LPDIRECTSOUNDBUFFER CMP3::mp3Buf = NULL ;
//
////extern HWND BaseWnd ;
//
////DWORD WINAPI PrepareMP3Buf (void* param) ;
////DWORD WINAPI PlayMP3Buf (void* param) ;
//bool CMP3::m_bLoop = false;
//
//HANDLE g_hPlayThread = NULL;
//HANDLE g_hLoadThread = NULL;
//int		CMP3::m_Volume = 0;
//std::string CMP3::m_szFilename;
//CMP3::MP3_STATUS CMP3::m_status = CMP3::STATUS_NULL;
//
//CMP3::CMP3(LPCSTR lpcszFileName) 
//{
//	if(lpcszFileName != NULL)
//		Open(lpcszFileName);
//
//}
//
//CMP3::~CMP3()
//{
//	Release();
//}
//
//LPDIRECTSOUNDBUFFER CMP3::GetBuffer()
//{
//	return mp3Buf;
//}
//
//void CMP3::Release()
//{
//	DEBUG_ADD("[CMP3] Release TerminateThread( g_hPlayThread, 0 );");
//	if(g_hPlayThread != NULL && WaitForSingleObject(g_hPlayThread, 0) == WAIT_TIMEOUT)
//	{
//		TerminateThread( g_hPlayThread, 0 );
//		CloseHandle( g_hPlayThread );
//		g_hPlayThread = NULL;
//	}
//	DEBUG_ADD("[CMP3] Release TerminateThread( g_hLoadThread, 0 );");
//	if(g_hLoadThread != NULL && WaitForSingleObject(g_hLoadThread, 0) == WAIT_TIMEOUT)
//	{
//		TerminateThread( g_hLoadThread, 0 );
//		CloseHandle( g_hLoadThread );
//		g_hLoadThread = NULL;
//	}
//	DEBUG_ADD("[CMP3] Release TerminateThread OK");
//	if ( mp3 ) { /*MP3_Close(mp3) ;*/ mp3 = NULL ;} ;
//	DEBUG_ADD("[CMP3] Release MP3Close OK");
//	if ( mp3Buf ) { g_DXSound.Release(mp3Buf) ; mp3Buf = NULL; } ;
//	DEBUG_ADD("[CMP3] Release mp3buf Release OK");
//}
//
//DWORD WINAPI CMP3::PrepareMP3Buf (void* param)
//{
//	DEBUG_ADD("[CMP3] PrepareMP3Buf");
//
////	Sleep(1000) ;
//	char *mp3FileName = (char*) param ;
//	
//	DEBUG_ADD("[CMP3] MP3_Load");
//	mp3 = MP3_Load(mp3FileName) ;
//	DEBUG_ADD("[CMP3] MP3_Load OK");
//
//	if ( mp3 == NULL )
//	{
//		DEBUG_ADD("[CMP3] mp3 == NULL");
//		m_szFilename = "";
//		ExitThread(100) ;
//	}
//	
////	if(g_hPlayThread != NULL && WaitForSingleObject(g_hPlayThread, 0) == WAIT_TIMEOUT)
////	{
////		DEBUG_ADD("[CMP3] TerminateThread PlayThread");
////		TerminateThread( g_hPlayThread, 0 );
////		CloseHandle( g_hPlayThread );
////		g_hPlayThread = NULL;
////	}
////	
////	DWORD threadID;
////	DEBUG_ADD("[CMP3] Create PlayThread");
////	CreateThread ( NULL, 0, PlayMP3Buf, NULL, 0, &threadID) ;
////	DEBUG_ADD("[CMP3] Create PlayThread OK");
//
////	if(mp3Buf != NULL)
////	{
////		g_DXSound.Release(mp3Buf) ; mp3Buf = NULL;
////	}
//
//	DEBUG_ADD("[CMP3] MP3_CreateDSBuffer");
//	mp3Buf= MP3_CreateDSBuffer(mp3) ;
//	DEBUG_ADD("[CMP3] MP3_CreateDSBuffer OK");
////	if ( mp3Buf == NULL ) DispDebugMsg("MP3 Direct Sound Buffer 생성에 실패했습니다." ) ;
////	mp3 = NULL ;
////	SendMessage(BaseWnd, UM_MP3LOADED, 0, 0) ;
//	if(mp3Buf == NULL)
//	{
//		m_szFilename = "";
//		ExitThread(0) ;
//		return 0 ;
//	}
//	
//	DEBUG_ADD("[CMP3] Close mp3 in Prepare MP3Buf");
//	if ( mp3 ) { /*MP3_Close(mp3) ;*/ mp3 = NULL ;} ;
//	DEBUG_ADD("[CMP3] Close mp3 in Prepare MP3Buf OK");
//
//	if(m_status == STATUS_PLAYING)
//	{
//		// 로딩 끝나면 무조건 play
//		g_pMP3->Play(false);
//	}
//	ExitThread(0) ;
//	return 0 ;
//}
//
//
//// DSBUFFER에 놓인 MP3 음악을 재생한다. 끝나면 메시지를 보낸다.
//DWORD WINAPI CMP3::PlayMP3Buf (LPVOID param)
//{
////	DispDebugMsg("현재 로드된 MP3파일을 연주합니다.") ;
//	DEBUG_ADD("[CMP3] PlayMP3Buf");
////	while(mp3Buf == NULL)
////	{
////		Sleep(1000);
////	}
//
//	g_DXSound.Play ( mp3Buf, false, false ) ;
//
//	BOOL bEnd = FALSE ;
//	DWORD status ;
//
//	while ( !bEnd )
//	{
////		status = DS_GetStatus(mp3Buf) ;
//		if(mp3Buf == NULL)
//			ExitThread(0);
//
//		mp3Buf->GetStatus(&status);
//
//		if ( status & DSBSTATUS_PLAYING )
//		{
//			Sleep(100) ;
//			if(m_status == STATUS_STOP)
//				mp3Buf->Stop();
//		}
//		else
//		{
//			Sleep(100) ;
////			if(m_status != STATUS_STOP)
//			bEnd = TRUE ;
//		}
//
//		
//	}
//
////	DEBUG_ADD("[CMP3] Release mp3buf inPlayMP3Buf");
////	mp3Buf = NULL;
////	if ( mp3Buf ) { g_DXSound.Release(mp3Buf) ; mp3Buf = NULL; } ;
////	DEBUG_ADD("[CMP3] Release mp3buf inPlayMP3Buf OK");
//	// DS_Release(mp3Buf) ;
////	mp3Buf = NULL ;
//
////	SendMessage(BaseWnd, UM_MP3PLAYEND, 0, 0) ;
////	DispDebugMsg("MP3연주가 끝나서 메시지를 보냅니다.") ;
//	m_status = STATUS_NULL;
////	m_szFilename = "";
//
//	DEBUG_ADD("[CMP3] PlayMP3Buf OK");
//	ExitThread(0) ;
//	return 0 ;
//}
//
//int CMP3::Play(bool bLoop)
//{
//	DEBUG_ADD("[CMP3] Play");
//	m_status = STATUS_PLAYING;
//	m_bLoop = bLoop;
//	
//	if ( mp3Buf != NULL )
//	{
//		SetVolume(m_Volume);
//
//		DWORD threadID ;
//		if(g_hPlayThread != NULL && WaitForSingleObject(g_hPlayThread, 0) == WAIT_TIMEOUT)
//		{
//			TerminateThread( g_hPlayThread, 0 );
//			CloseHandle( g_hPlayThread );
//			g_hPlayThread = NULL;
//		}
//
//		CreateThread ( NULL, 0, PlayMP3Buf, NULL, 0, &threadID) ;
//		return 0 ;
//	}
//	else
//	{
//		return -1 ;
//	}
//	DEBUG_ADD("[CMP3] Play OK");
//}
//
//int CMP3::Open(LPCSTR filename)
//{
//	if(!m_szFilename.empty() && strcmp(filename, m_szFilename.c_str()) == 0)
//		return -1;
//
//	m_szFilename = filename;
//
//	CMP3::Release () ;
//
//	DEBUG_ADD("[CMP3] Open");
//	if ( mp3Buf != NULL )
//	{
////		DispDebugMsg("MP3 파일 %s을 로드하려 할 때 mp3Buf가 NULL값이 아닙니다.", filename ) ;
//		return -1 ;
////		Release();
//	}
//	else
//	{
////		DispDebugMsg("MP3 파일 %s을 로드합니다.", filename ) ;
//		DWORD threadID ;
//
//		g_hLoadThread = CreateThread ( NULL, 0, PrepareMP3Buf, (void *)filename, 0, &threadID) ;
//		DEBUG_ADD("[CMP3] Open OK");
//		return 0 ;
//	}
//	DEBUG_ADD("[CMP3] Open OK 2");
//}
//
//void CMP3::Stop()
//{
////	g_DXSound.Stop(mp3Buf);
////	Release();
//	m_status = STATUS_STOP;
//	if(mp3Buf != NULL)mp3Buf->Stop();
//}
//
//void CMP3::SetVolume(int volume)
//{
//	DEBUG_ADD("[CMP3] SetVolume");
//	m_Volume = volume;
//	if(mp3Buf != NULL)
//	{
////		LONG volume = volume*SOUND_DEGREE + SOUND_MIN;
//		
//		LONG maxVolume = g_DXSound.GetVolumeLimit();
//		
//		g_DXSound.SetVolumeLimit( volume );
//		g_DXSound.AddVolume( mp3Buf, 0 );
//		
//		// 원래의 max volume으로 돌린다.
//		g_DXSound.SetVolumeLimit( maxVolume );
//		mp3Buf->SetVolume(volume);
//	}
//	DEBUG_ADD("[CMP3] SetVolume OK");
////	g_DXSound.SetVolumeLimit(volume);
//}

CMP3::CMP3(LPCSTR lpcszFileName) 
{
	muiDeviceID = 0;

	if(lpcszFileName != NULL)
		Open(lpcszFileName);

	m_bLoop = false;
	m_oldVolume = 0xffffffff;
}

CMP3::~CMP3()
{
	Stop();
	Close();
	SetVolume(m_oldVolume);
}

void CMP3::Close()
{
	if (muiDeviceID != 0) {

		mciSendCommand(muiDeviceID, MCI_CLOSE, 0, (DWORD)NULL);
	}

	return;
}


DWORD CMP3::Open(LPCSTR lpcszFileName)
{
/*  MCI_OPEN_PARMS mciOpenParms;
  MCI_SET_PARMS mciSetParms;
	mciOpenParms.lpstrElementName = lpcszFileName;
	mciOpenParms.lpstrDeviceType = "MPEGVideo";
	mciSendCommand(NULL,MCI_OPEN,MCI_OPEN_ELEMENT|MCI_OPEN_TYPE,(DWORD)(LPVOID)&mciOpenParms);
	muiDeviceID = mciOpenParms.wDeviceID;
	MCI_STATUS_PARMS mciStatusParms;
	mciStatusParms.dwItem = MCI_STATUS_LENGTH;
	mciSendCommand(muiDeviceID,MCI_STATUS,MCI_STATUS_ITEM,(DWORD)(LPVOID)&mciStatusParms);
//	m_nTotalTime = (LONG)mciStatusParms.dwReturn;
	return 0;
*/

	
	
	
  DWORD dwResult;
  MCI_OPEN_PARMS mciOpenParms;
  MCI_SET_PARMS mciSetParms;


	if (StatusMode() != MCI_MODE_NOT_READY) {

		Close();
	}

	


	// 장치오픈
	mciOpenParms.lpstrDeviceType = "MPEGVideo";
	mciOpenParms.lpstrElementName = lpcszFileName;

	dwResult = mciSendCommand(0,
		                      MCI_OPEN,
							  MCI_OPEN_ELEMENT|MCI_OPEN_TYPE,
							  (DWORD)(LPVOID)&mciOpenParms);

	// 정상적으로 오픈되었으면 ID 발급
	if (dwResult == 0) {

		muiDeviceID = mciOpenParms.wDeviceID;

		// Set Time Format
		mciSetParms.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
		mciSendCommand(muiDeviceID, MCI_SET, 
			           MCI_SET_TIME_FORMAT, (DWORD)(LPVOID)&mciSetParms);
	}

	return dwResult;
	
}


DWORD CMP3::Play(bool bLoop)
{
  DWORD          dwResult, dwStatus;
  MCI_PLAY_PARMS mciPlayParms;

	dwStatus = StatusMode();

	Stop();

	if (dwStatus != MCI_MODE_NOT_READY) {
	
//		mciPlayParms.dwFrom = dwStart;
//		mciPlayParms.dwCallback = (DWORD)hWnddNotify;
		dwResult = mciSendCommand(muiDeviceID, MCI_PLAY, 
			                      MCI_NOTIFY, (DWORD)(LPVOID)&mciPlayParms);

		if (dwResult) {
			Close();
		}
	}

	m_bLoop = bLoop;
	return dwResult;
}


DWORD CMP3::Pause()
{
  DWORD             dwResult;
  MCI_GENERIC_PARMS mciGenericParms;

	dwResult = mciSendCommand(muiDeviceID, MCI_PAUSE, 
		                      MCI_WAIT,(DWORD)(LPVOID)&mciGenericParms);
	return dwResult;
}


DWORD CMP3::Stop()
{
  DWORD             dwResult;
  MCI_GENERIC_PARMS mciGenericParms;

	dwResult = mciSendCommand(muiDeviceID, MCI_STOP, 
		                      0,(DWORD)(LPVOID)&mciGenericParms);

	m_bLoop = false;

	return dwResult;
}

BOOL CMP3::Home()
{
	mciSendCommand(muiDeviceID,MCI_SEEK,MCI_SEEK_TO_START,(DWORD)(LPVOID)NULL);
	return TRUE;
}

BOOL CMP3::End()
{
	mciSendCommand(muiDeviceID,MCI_SEEK,MCI_SEEK_TO_END,(DWORD)(LPVOID)NULL);
	return TRUE;
}

DWORD CMP3::GetPosition()
{
	DWORD nTime;
	MCI_STATUS_PARMS mciStatusParms;
	mciStatusParms.dwItem = MCI_STATUS_POSITION;
	mciSendCommand(muiDeviceID,MCI_STATUS,MCI_STATUS_ITEM,(DWORD)(LPVOID)&mciStatusParms);
	nTime = (LONG)mciStatusParms.dwReturn;
	return nTime;
}

void CMP3::SetPosition(DWORD dwPos)
{
	MCI_SEEK_PARMS mciSeekParms;
	mciSeekParms.dwTo = dwPos;
	
	mciSendCommand(muiDeviceID,MCI_SEEK,MCI_TO,(DWORD)(LPVOID)&mciSeekParms);
//	Play();
}


// 전체 길이를 밀리초단위로 반환
DWORD CMP3::TotalLength()
{
  DWORD dwResult;

	dwResult = GetStatus(MCI_STATUS_LENGTH);
	return dwResult;
}


// 전체 길이를 mm:ss 형식의 문자열과 함께 밀리초단위로 반환
DWORD CMP3::TotalLength(LPSTR lpszReturn)
{
  DWORD dwResult;

	dwResult = TotalLength();
	MinsFromSecs(dwResult, lpszReturn);
	return dwResult;
}

	
// 재생 위치를 밀리초단위로 반환
DWORD CMP3::CurrentLength()
{
  DWORD dwResult;

	dwResult = GetStatus(MCI_STATUS_POSITION);
	return dwResult;
}


// 재생 위치를 mm:ss 형식의 문자열과 함께 밀리초단위로 반환
DWORD CMP3::CurrentLength(LPSTR lpszReturn)
{
  DWORD dwResult;

	dwResult = CurrentLength();
	MinsFromSecs(dwResult, lpszReturn);
	return dwResult;
}


// 전달받은 Flag에 따른 Player의 상태 값을 반환
DWORD CMP3::GetStatus(DWORD dwFlag)
{
  MCI_STATUS_PARMS mciStatusParms;

	mciStatusParms.dwItem = dwFlag;
	mciSendCommand(muiDeviceID, MCI_STATUS, 
		           MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatusParms);

	return mciStatusParms.dwReturn;
}


// 전달 받은 밀리초를 mm:ss 형식의 문자열로 변환해서 반환
void CMP3::MinsFromSecs(DWORD dwLength, LPSTR lpszReturn)
{
  double dMilliSeconds;
  int    iMinutes, iSeconds;

	dMilliSeconds = (double)dwLength / 1000 / 60;
	iMinutes      = (int)dMilliSeconds;
	dMilliSeconds -= iMinutes;
	iSeconds      = (int)(dMilliSeconds * 60);

	wsprintf(lpszReturn, "%.2d:%.2d", iMinutes, iSeconds);
	return;
}


// 전달 받은 코드에 해당하는 에러 메시지를 반환
void CMP3::GetErrorString(DWORD dwErrCode, LPSTR lpszErrString)
{
	mciGetErrorString(dwErrCode, lpszErrString, sizeof(lpszErrString));
	return;
}


// 준비가 되었으면 true
bool CMP3::IsReady()
{
  bool bReturn;

	bReturn = (GetStatus(MCI_STATUS_MODE) == MCI_MODE_NOT_READY);

	return bReturn;
}


// 일시정지 상태이면 true
bool CMP3::IsPause()
{
  bool bReturn;

	bReturn = (GetStatus(MCI_STATUS_MODE) == MCI_MODE_PAUSE);

	return bReturn;
}

// 플레이 상태이면 true
bool CMP3::IsPlay()
{
  bool bReturn;

	bReturn = (GetStatus(MCI_STATUS_MODE) == MCI_MODE_PLAY);

	return bReturn;
}

// 정지 상태이면 true
bool CMP3::IsStop()
{
  bool bReturn;
	
	bReturn = (GetStatus(MCI_STATUS_MODE) == MCI_MODE_STOP);

	return bReturn;
}


// 레코딩 상태이면 true
bool CMP3::IsRecord()
{
  bool bReturn;

	bReturn = (GetStatus(MCI_STATUS_MODE) == MCI_MODE_RECORD);

	return bReturn;
}

/* Player의 진행 상태를 반환
		MCI_MODE_NOT_READY
		MCI_MODE_PAUSE
		MCI_MODE_PLAY
		MCI_MODE_STOP
		MCI_MODE_OPEN
		MCI_MODE_RECORD
		MCI_MODE_SEEK		*/
DWORD CMP3::StatusMode()
{
  DWORD dwResult;
	
	dwResult = GetStatus(MCI_STATUS_MODE);
	return dwResult;
}


DWORD CMP3::GetVolume()
{
  DWORD    dwVolume, dwReturn;
  MMRESULT mmsError;

	mmsError = waveOutGetVolume(0, (LPDWORD)&dwVolume);
	dwReturn = LOWORD(dwVolume);

	return dwReturn;
}


void CMP3::SetVolume(DWORD dwVolume)
{
  MMRESULT mmsError;
  long     lLeftVol, lRightVol;

  if(m_oldVolume == 0xffffffff)
	  m_oldVolume = GetVolume();

	lLeftVol  = dwVolume;
	lRightVol = dwVolume;
	dwVolume  = (dwVolume << 16) + lRightVol;

	mmsError = waveOutSetVolume(0, dwVolume);
	return;
}