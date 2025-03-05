//-----------------------------------------------------------------------------
// MGameTime.cpp
//-----------------------------------------------------------------------------
#include "Client_PCH.h"
#include "MGameTime.h"

//-----------------------------------------------------------------------------
// Global
//-----------------------------------------------------------------------------
MGameTime*	g_pGameTime = NULL;

extern DWORD		g_CurrentTime;

//-----------------------------------------------------------------------------
//
// constructor / destructor
//
//-----------------------------------------------------------------------------
MGameTime::MGameTime()
{
	m_StartYear		= 0;
	m_StartMonth	= 0;
	m_StartDay		= 0;

	m_StartHour		= 0;
	m_StartMinute	= 0;
	m_StartSecond	= 0;

	// timeGetTime
	m_StartTime		= 0;

	// 시간 비율
	m_TimeRatio		= 1;

	// 현재 시간
	m_Year			= 0;
	m_Month			= 0;
	m_Day			= 0;

	m_Hour			= 0;
	m_Minute		= 0;
	m_Second		= 0;
}

MGameTime::~MGameTime()
{
}

//-----------------------------------------------------------------------------
//
// member functions
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Set StartTime
//-----------------------------------------------------------------------------
// 기준 시간 : y-m-d : h-m-s
//-----------------------------------------------------------------------------
void	
MGameTime::SetStartTime(DWORD time, WORD year, BYTE month, BYTE day, BYTE hour, BYTE minute, BYTE second)
{
	// timeGetTime
	m_StartTime		= time;

	m_StartYear		= year;
	m_StartMonth	= month;
	m_StartDay		= day;

	m_StartHour		= hour;
	m_StartMinute	= minute;
	m_StartSecond	= second;	
}

//-----------------------------------------------------------------------------
// Set CurrentTime
//-----------------------------------------------------------------------------
// 기준시간과 시간비율을 고려해서 현재 시간을 저장한다.
//-----------------------------------------------------------------------------
void	
MGameTime::SetCurrentTime(DWORD time)
{
	//------------------------------------------------------------
	// 초(second)로 환산한 값
	//------------------------------------------------------------
	const DWORD Day	= 24*60*60;
	const DWORD Hour	= 60*60;
	const DWORD Min	= 60;
	
	//------------------------------------------------------------
	// 31일인 달인가?
	//------------------------------------------------------------
	const BYTE DaysPerMonth[13] = 
	{
		0, 31, 28, 31, 30, 31, 30, 
		31, 31, 30, 31, 30, 31
	};


	//------------------------------------------------------------
	// 기준시간과 현재시간의 차이 
	//------------------------------------------------------------
	// 1000 = 1초, 시간비율 고려..
	//------------------------------------------------------------
	DWORD gap = (time - m_StartTime) / 1000 * m_TimeRatio;

	int rem = gap;

	// 기준시간 더해줌..
	rem += m_StartSecond + m_StartMinute*Min + m_StartHour*Hour;

	//------------------------------------------------------------
	// 변화값	
	//------------------------------------------------------------
	int	d = rem/Day;			rem -= d*Day;	
	int	h = rem/Hour;			rem -= h*Hour;	
	int	m = rem/Min;			rem -= m*Min;
	int	s = rem;

	m_Hour		= h;
	m_Minute	= m;
	m_Second	= s;

	// 이 달의 날 수?
	int days = DaysPerMonth[m_StartMonth];

	// 2월
	if (m_StartMonth==2)
	{	
		// 100의 배수가 아니면서 4의 배수인 해, 400의 배수인 해.. 맞나?? - -;
		if ((m_StartYear&0x03)==0 && m_StartYear%100!=0 || m_StartYear%400==0)
		{
			days ++;
		}
	}
	
	// 날
	m_Day = m_StartDay + d;

	// 한달이 넘어간 경우
	if (m_StartDay+d > days)
	{
		// 다음달 날짜로 계산..
		m_Day -= days;

		// 기준달 + 1
		m_Month = m_StartMonth + 1;

		if (m_Month==13)
		{
			m_Year ++;
			m_Month = 1;
		}
		else
		{
			// 1년이 넘어간 경우, 기준년 + 1
			m_Year = m_StartYear;
		}

		// 날짜 다시 설정
		SetStartTime(g_CurrentTime, m_Year, m_Month, m_Day, m_Hour, m_Minute, m_Second);
	}
	else
	{
		m_Month = m_StartMonth;
		m_Year	= m_StartYear;
	}

}
