//-----------------------------------------------------------------------------
// MGameTime.h
//-----------------------------------------------------------------------------
// 
// 기준 시간으로부터 현재 시간을 구해낸다.
//
// 실제 시간과 게임 시간의 비율을 고려해야 한다..
//
// timeGetTime()의 값이 [기준시간], [현재시간]이 된다.
//
//-----------------------------------------------------------------------------

#ifndef __MGAMETIME_H__
#define	__MGAMETIME_H__

class MGameTime {
	public :
		MGameTime();
		~MGameTime();

		//-------------------------------------------------------------
		// 기준 시간 : y-m-d : h-m-s
		//-------------------------------------------------------------
		void	SetStartTime(DWORD time, WORD year, BYTE month, BYTE day, BYTE hour, BYTE minute, BYTE second);

		//-------------------------------------------------------------
		// game시간은 실제 시간의 몇 배인가?
		//-------------------------------------------------------------
		void	SetTimeRatio(int ratio)		{ m_TimeRatio = ratio; }
		int		GetTimeRatio()				{ return m_TimeRatio; }

		//-------------------------------------------------------------
		// 현재 시간
		//-------------------------------------------------------------
		void	SetCurrentTime(DWORD time);

		//-------------------------------------------------------------
		// Get
		//-------------------------------------------------------------
		WORD	GetYear() const			{ return m_Year; }
		BYTE	GetMonth() const		{ return m_Month; }
		BYTE	GetDay() const			{ return m_Day; }
		BYTE	GetHour() const			{ return m_Hour; }
		BYTE	GetMinute() const		{ return m_Minute; }
		BYTE	GetSecond() const		{ return m_Second; }


	protected :
		//-------------------------------------------------------------
		// 기준 시간	
		//-------------------------------------------------------------
		// YYYY-MM-DD
		WORD		m_StartYear;
		BYTE		m_StartMonth;
		BYTE		m_StartDay;

		// HH:MM:SS
		BYTE		m_StartHour;
		BYTE		m_StartMinute;
		BYTE		m_StartSecond;

		//-------------------------------------------------------------
		// 기준 시간 : timeGetTime()의 값
		//-------------------------------------------------------------
		DWORD		m_StartTime;

		//-------------------------------------------------------------
		// 게임 시간과 실제 시간의 비율 : 실제시간*비율 = 게임시간
		//-------------------------------------------------------------
		int			m_TimeRatio;

		//-------------------------------------------------------------
		// 현재 시간
		//-------------------------------------------------------------
		// YYYY-MM-DD
		WORD		m_Year;
		BYTE		m_Month;
		BYTE		m_Day;

		// HH:MM:SS
		BYTE		m_Hour;
		BYTE		m_Minute;
		BYTE		m_Second;
};


extern MGameTime*	g_pGameTime;

#endif

