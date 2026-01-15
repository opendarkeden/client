//-----------------------------------------------------------------------------
// ShowTimeChecker.h
//-----------------------------------------------------------------------------

#ifndef __SHOWTIMECHECKER_H__
#define __SHOWTIMECHECKER_H__

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#else
#include "../../basic/Platform.h"

#include <fstream>
using namespace std;
#endif

class ShowTimeChecker {
	public :
		ShowTimeChecker();
		~ShowTimeChecker();

		bool			IsShowTime() const;		// 다 고려한 상황에서.. ShowTime인가?
		bool			IsShowHour() const;		// startHour~EndHour와 관련..

		void			SetNextShowTime();

		//---------------------------------------------------------------
		// File I/O
		//---------------------------------------------------------------
		void			SaveToFile(ofstream& file);
		void			LoadFromFile(ifstream& file);

	public :
		// 반복적인가?
		bool			Loop;

		// MinDelay ~ MaxDelay 사이에는 꼭 한 번
		DWORD			MinDelay;
		DWORD			MaxDelay;
		
		// StartHour부터 EndHour 사이에만 (0~24시면 종일?)
		BYTE			StartHour;
		BYTE			EndHour;

		DWORD			NextPlayTime;		// 다음에 show할 시간
};

#endif


