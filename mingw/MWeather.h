//----------------------------------------------------------------------
// MWeather.h
//----------------------------------------------------------------------
// 날씨 효과를 표현..
//----------------------------------------------------------------------

#ifndef	__MWEATHER_H__
#define	__MWEATHER_H__

#include "MTypeDef.h"
#include "DrawTypeDef.h"


//----------------------------------------------------------------------
// 하나의 움직이는 효과를 나타낸다.
//----------------------------------------------------------------------
class MAP_EFFECT {
	public :		
		MAP_EFFECT() { m_Type = MAP_EFFECT_ARRIVE3; }
		~MAP_EFFECT() {}

		void	Set(BYTE type, TYPE_SPRITEID sid, short x, short y, char sx, char sy, BYTE count)
		{
			m_Type		= type;
			m_SpriteID	= sid;
			m_X			= x;
			m_Y			= y;
			m_StepX		= sx;
			m_StepY		= sy;
			m_Count		= count;
			m_maxCount	= count;
		}

		//------------------------------------------------------------
		// 한 번 이동하기..
		//------------------------------------------------------------
		bool		Move()
		{	
			if (m_Count==0)
				return false;

			m_X += m_StepX;
			m_Y += m_StepY;

			m_Count--;

			return true;
		}

		//------------------------------------------------------------
		// Get Functions
		//------------------------------------------------------------
		BYTE			IsActive() const		{ return m_Count; }
		BYTE			GetType() const			{ return m_Type; }
		TYPE_SPRITEID	GetSpriteID() const		{ return m_SpriteID; }
		short			GetX() const			{ return m_X; }
		short			GetY() const			{ return m_Y; }		
		short			GetCount() const		{ return m_Count; }
		short			GetMaxCount() const		{ return m_maxCount; }

		//------------------------------------------------------------
		// Set
		//------------------------------------------------------------
		void			SetSX(char sx)			{ m_StepX = sx; }
		void			SetSY(char sy)			{ m_StepY = sy; }

		//------------------------------------------------------------
		// operator = 
		//------------------------------------------------------------
		void			operator = (const MAP_EFFECT& effect)
		{
			m_Type		= effect.m_Type;
			m_SpriteID	= effect.m_SpriteID;
			m_X			= effect.m_X;
			m_Y			= effect.m_Y;
			m_StepX		= effect.m_StepX;
			m_StepY		= effect.m_StepY;
			m_Count		= effect.m_Count;
			m_maxCount	= effect.m_maxCount;
		}

	public :
		enum WEATHER_EFFECT_TYPE
		{
			MAP_EFFECT_FALL,
			MAP_EFFECT_ARRIVE1,
			MAP_EFFECT_ARRIVE2,
			MAP_EFFECT_ARRIVE3,
			MAP_EFFECT_ARRIVE4,
			MAP_EFFECT_ARRIVE5,
			MAP_EFFECT_ARRIVE6
		};

	protected :
		BYTE			m_Type;		// 종류

		TYPE_SPRITEID	m_SpriteID;

		// 화면에서의 좌표
		short			m_X, m_Y; 

		// 한 번에 움직이는 Pixel수
		char			m_StepX, m_StepY;

		// 움직이는 회수 남은거..
		BYTE			m_Count, m_maxCount;
};


//----------------------------------------------------------------------
// 날씨 효과를 나타낸다.
//----------------------------------------------------------------------
class MWeather {
	public :
		MWeather();
		~MWeather();

		//-------------------------------
		// 작동중인가?
		//-------------------------------
		bool		IsActive() const		{ return m_WeatherType!=WEATHER_NULL; }
		BYTE		GetWeatherType() const	{ return m_WeatherType; }		

		//-------------------------------
		// 완전히 제거
		//-------------------------------
		void		Release();			// MapEffec를 메모리에서 제거

		//-------------------------------
		// 날씨 효과 중단하기.. (바로 중단되는건 아님)
		//-------------------------------
		void		Stop()					{ m_WeatherType = WEATHER_STOP; }

		//-------------------------------
		// 날씨 효과를 생성한다.
		//-------------------------------
		void		SetRain(BYTE number);			// 비를 오게 한다.
		void		SetSnow(BYTE number);			// 눈을 오게 한다.
		void		SetSpot(BYTE number);			// 눈을 오게 한다.

		//-------------------------------
		// 전체적으로 한 번 움직인다..
		//-------------------------------
		void		Action();

		//-------------------------------
		// Get
		//-------------------------------
		int					GetStartX() const		{ return m_StartX; }
		int					GetStartY() const		{ return m_StartY; }
		BYTE				GetSize() const			{ return m_nActiveMapEffect; }
		const MAP_EFFECT&	operator [] (BYTE n)	{ return m_pMapEffect[n]; }

	protected :
		void		Init(BYTE n);		// n개의 MapEffect 메모리 잡기
		
		void		GenerateRain(const BYTE& n);
		void		GenerateSnow(const BYTE& n);
		void		GenerateSpot(const BYTE& n);
	

	public :
		// 날씨 효과 종류에 따른 정의
		enum WEATHER_TYPE
		{
			WEATHER_NULL = 0,
			WEATHER_STOP,		// 멈추고 있는 중
			WEATHER_RAIN,
			WEATHER_SNOW,
			WEATHER_SPOT,
		};


	
	protected :
		// 날씨가 시작된때의 캐릭터의 Zone에서의 좌표
		int				m_StartX;
		int				m_StartY;

		// 날씨 정보 
		BYTE			m_WeatherType;
		BYTE			m_nMapEffect;
		BYTE			m_nActiveMapEffect;
		MAP_EFFECT*		m_pMapEffect;

		// temp(?)
		BYTE			m_MoreEffectCount;
};


extern	MWeather*	g_pWeather;

#endif

