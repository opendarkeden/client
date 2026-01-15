//-----------------------------------------------------------------------------
// ShowTimeChecker.cpp
//-----------------------------------------------------------------------------
#include "Client_PCH.h"
#include "ShowTimeChecker.h"
#include "Client_PCH.h"
#ifdef __GAME_CLIENT__
	#include "MGameTime.h"
#endif

extern DWORD	g_CurrentTime;

//-----------------------------------------------------------------------------
//
// constructor / destructor
//
//-----------------------------------------------------------------------------
ShowTimeChecker::ShowTimeChecker()
{
	Loop = false;

	// MinDelay ~ MaxDelay 사이에는 꼭 한 번 소리가 나야한다.
	MinDelay = 60000;
	MaxDelay = 60000;

	// StartHour부터 EndHour 사이에만 소리가 난다. (0~24시면 종일?)
	StartHour = 0;
	EndHour = 24;

	NextPlayTime = 0;
}

ShowTimeChecker::~ShowTimeChecker()
{
}

//-----------------------------------------------------------------------------
// Is Show Time
//-----------------------------------------------------------------------------
// MinDelay~MaxDelay사이..
// StartHour~EndHour사이에 연주..
//-----------------------------------------------------------------------------
bool					
ShowTimeChecker::IsShowTime() const
{
	if (IsShowHour())
	{
		//----------------------------------------------------------------
		// 반복 출력이거나..
		// 출력할 시간이 지났나?
		//----------------------------------------------------------------
		return Loop || g_CurrentTime >= NextPlayTime;		
	}

	return false;		
}

//-----------------------------------------------------------------------------
// IsShowHour
//-----------------------------------------------------------------------------
// StartHour~EndHour사이가 아닌 경우..
// false : x
// true : 보여준다. 소리낸다.. 등등.
//-----------------------------------------------------------------------------
bool					
ShowTimeChecker::IsShowHour() const
{
	if (g_pGameTime!=NULL)
	{
		int hour = g_pGameTime->GetHour();

		//----------------------------------------------------------------
		// 시간대 체크
		//----------------------------------------------------------------
		if (StartHour <= EndHour)
		{
			// ......[Start]xxxxx[End]......
			if (hour >= StartHour && hour <= EndHour)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		// xxxxx[End]......[Start]xxxxx
		if (hour >= StartHour || hour <= EndHour)
		{
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// Set NextShowTime
//-----------------------------------------------------------------------------
// 다음에 출력할 시간을 결정한다.
//-----------------------------------------------------------------------------
void					
ShowTimeChecker::SetNextShowTime()
{
	DWORD delayGap = MaxDelay - MinDelay;

	if (delayGap==0)
	{
		NextPlayTime = g_CurrentTime;
	}
	else
	{
		NextPlayTime = g_CurrentTime + MinDelay + rand()%delayGap;
	}
}


//-----------------------------------------------------------------------------
// Save To File
//-----------------------------------------------------------------------------
void					
ShowTimeChecker::SaveToFile(class ofstream& file)
{
	file.write((const char*)&Loop, 1);

	// MinDelay ~ MaxDelay 사이에는 꼭 한 번 소리가 나야한다.
	file.write((const char*)&MinDelay, 4);
	file.write((const char*)&MaxDelay, 4);
	
	// StartHour부터 EndHour 사이에만 소리가 난다. (0~24시면 종일?)
	file.write((const char*)&StartHour, 1);
	file.write((const char*)&EndHour, 1);
}

//-----------------------------------------------------------------------------
// Load From File
//-----------------------------------------------------------------------------
void					
ShowTimeChecker::LoadFromFile(class ifstream& file)
{
	file.read((char*)&Loop, 1);

	// MinDelay ~ MaxDelay 사이에는 꼭 한 번 소리가 나야한다.
	file.read((char*)&MinDelay, 4);
	file.read((char*)&MaxDelay, 4);
	
	// StartHour부터 EndHour 사이에만 소리가 난다. (0~24시면 종일?)
	file.read((char*)&StartHour, 1);
	file.read((char*)&EndHour, 1);
}