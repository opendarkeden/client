//----------------------------------------------------------------------
// MWeather.cpp
//----------------------------------------------------------------------
// 음.. 시간이 없는 관계로 
// 예전에 테스트용으로 만들어둔걸 그냥 써야하는 가슴 아픔이랄까. - -;;
// 아.. 아무리 봐도.. 허접한 구조.. 헐.. 어쩔 수 없지.. 음냐하..
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MWeather.h"
#include "MPlayer.h"
#include "MViewDef.h"
#include "DebugInfo.h"
extern BOOL g_MyFull;
extern RECT g_GameRect;
//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
MWeather*	g_pWeather;

//----------------------------------------------------------------------
//
//  constructor/destructor
//
//----------------------------------------------------------------------
MWeather::MWeather()
{
	m_WeatherType		= WEATHER_NULL;
	m_nMapEffect		= 0;
	m_nActiveMapEffect	= 0;
	m_pMapEffect		= NULL;
}

MWeather::~MWeather()
{
	Stop();
	Release();
}

//----------------------------------------------------------------------
//
//  member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Init
//----------------------------------------------------------------------
void
MWeather::Init(BYTE n)
{
	// 이미 잡혀있는 메모리와 같으면 return
	if (n==m_nMapEffect) 
		return;

	// 일단 제거
	Release();

	// memory잡기
	m_nMapEffect = n;
	m_pMapEffect = new MAP_EFFECT [ m_nMapEffect ];
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void
MWeather::Release()
{
	// 메모리에서 제거
	if (m_pMapEffect != NULL)
	{
		delete [] m_pMapEffect;
		m_nMapEffect = 0;
		m_pMapEffect = NULL;
		//m_nActiveMapEffect = 0;
		m_nActiveMapEffect = 0;
		m_WeatherType	= WEATHER_NULL;
	}
}

//----------------------------------------------------------------------
// Set Rain (비 개수)
//----------------------------------------------------------------------
// 비를 오게 한다.
//----------------------------------------------------------------------
void		
MWeather::SetRain(BYTE number)
{
	if (number==0) return;

	//------------------------------------------------
	// 이전에도 비가 내리고 있던 중이면..
	// 비의 개수를 조정해준다.
	//------------------------------------------------
	if (m_WeatherType == WEATHER_RAIN)
	{
		//----------------------------------------
		// 같은 수의 비가 오는 경우...
		//----------------------------------------
		if (number == m_nMapEffect)
			return;

		//----------------------------------------
		// 아닌 경우
		//----------------------------------------
		// 새로 메모리를 잡고 
		MAP_EFFECT*		pMapEffect = new MAP_EFFECT [ number ];

		//----------------------------------------
		// 비가 더 많이 오는 경우
		//----------------------------------------
		int i;
		if (number > m_nMapEffect)
		{
			// 현재 비를 새로운 메모리로 복사..
			for (i=0; i<m_nActiveMapEffect; i++)
			{
				pMapEffect[i] = m_pMapEffect[i];
			}	
			
			// 점차 비가 더 오게 한다.
			m_MoreEffectCount = 2;
		}
		//----------------------------------------
		// 비가 적게 오는 경우
		//----------------------------------------
		else
		{
			m_nActiveMapEffect = min( m_nActiveMapEffect, number );

			for (i=0; i<m_nActiveMapEffect; i++)
			{
				pMapEffect[i] = m_pMapEffect[i];
			}
		}

		// 현재 메모리 제거
		Release();

		// 위에서 생성한 비를 설정..
		m_WeatherType	= WEATHER_RAIN;	
		m_pMapEffect = pMapEffect;
		m_nMapEffect = number;
		
		return;
	}

	//------------------------------------------------
	//
	// 새롭게 모든 비를 생성한다.
	//
	//------------------------------------------------

	// number개의 빗줄기를 생성
	Init( number );

	// 비
	m_WeatherType	= WEATHER_RAIN;
	m_StartX		= g_pPlayer->GetX()*TILE_X;
	m_StartY		= g_pPlayer->GetY()*TILE_Y;

	// 현재 비의 개수
	m_nActiveMapEffect = 1;

	for (int i=0; i<m_nActiveMapEffect; i++)
	{
		GenerateRain( i );
	}

	// MoreEffectCount시간이 지나서야 비가 증가하기 시작한다.
	m_MoreEffectCount = 2;	
}

//----------------------------------------------------------------------
// Set Snow (눈 개수)
//----------------------------------------------------------------------
// 눈을 오게 한다.
//----------------------------------------------------------------------
void		
MWeather::SetSnow(BYTE number)
{
	if (number==0) return;

	//------------------------------------------------
	// 이전에도 눈이 내리고 있던 중이면..
	// 눈의 개수를 조정해준다.
	//------------------------------------------------
	if (m_WeatherType == WEATHER_SNOW)
	{
		//----------------------------------------
		// 같은 수의 눈이 오는 경우...
		//----------------------------------------
		if (number == m_nMapEffect)
			return;

		//----------------------------------------
		// 아닌 경우
		//----------------------------------------
		// 새로 메모리를 잡고 
		MAP_EFFECT*		pMapEffect = new MAP_EFFECT [ number ];

		//----------------------------------------
		// 눈이 더 많이 오는 경우
		//----------------------------------------
		int i;
		if (number > m_nMapEffect)
		{
			// 현재 눈을 새로운 메모리로 복사..
			for (i=0; i<m_nActiveMapEffect; i++)
			{
				pMapEffect[i] = m_pMapEffect[i];
			}	
			
			// 점차 눈이 더 오게 한다.
			m_MoreEffectCount = 2;
		}
		//----------------------------------------
		// 눈이 적게 오는 경우
		//----------------------------------------
		else
		{
			m_nActiveMapEffect = min( m_nActiveMapEffect, number );

			for (i=0; i<m_nActiveMapEffect; i++)
			{
				pMapEffect[i] = m_pMapEffect[i];
			}
		}

		// 현재 메모리 제거
		Release();

		// 위에서 생성한 눈을 설정..
		m_WeatherType	= WEATHER_SNOW;	
		m_pMapEffect = pMapEffect;
		m_nMapEffect = number;
		
		return;
	}

	//------------------------------------------------
	//
	// 새롭게 모든 눈을 생성한다.
	//
	//------------------------------------------------

	// number개의 눈송이 생성
	Init( number );

	// 눈
	m_WeatherType	= WEATHER_SNOW;
	m_StartX		= g_pPlayer->GetX()*TILE_X;
	m_StartY		= g_pPlayer->GetY()*TILE_Y;

	// 증가 개수
	m_nActiveMapEffect = 1;

	for (int i=0; i<number; i++)
	{
		GenerateSnow( i );
	}
	
	m_MoreEffectCount = 2;
}

//----------------------------------------------------------------------
// Set Spot (개수)
//----------------------------------------------------------------------
void		
MWeather::SetSpot(BYTE number)
{
	if (number==0) return;
	
	//------------------------------------------------
	// 이전에도 있던 중이면..
	// 눈의 개수를 조정해준다.
	//------------------------------------------------
	if (m_WeatherType == WEATHER_SPOT)
	{
		//----------------------------------------
		// 같은 수의 눈이 오는 경우...
		//----------------------------------------
		if (number == m_nMapEffect)
			return;
		
		//----------------------------------------
		// 아닌 경우
		//----------------------------------------
		// 새로 메모리를 잡고 
		MAP_EFFECT*		pMapEffect = new MAP_EFFECT [ number ];
		
		//----------------------------------------
		// 눈이 더 많이 오는 경우
		//----------------------------------------
		int i;
		if (number > m_nMapEffect)
		{
			// 현재 눈을 새로운 메모리로 복사..
			for (i=0; i<m_nActiveMapEffect; i++)
			{
				pMapEffect[i] = m_pMapEffect[i];
			}	
			
			// 점차 눈이 더 오게 한다.
			m_MoreEffectCount = 2;
		}
		//----------------------------------------
		// 눈이 적게 오는 경우
		//----------------------------------------
		else
		{
			m_nActiveMapEffect = min( m_nActiveMapEffect, number );
			
			for (i=0; i<m_nActiveMapEffect; i++)
			{
				pMapEffect[i] = m_pMapEffect[i];
			}
		}
		
		// 현재 메모리 제거
		Release();
		
		// 위에서 생성한 눈을 설정..
		m_WeatherType	= WEATHER_SPOT;	
		m_pMapEffect = pMapEffect;
		m_nMapEffect = number;
		
		return;
	}
	
	//------------------------------------------------
	//
	// 새롭게 모든 눈을 생성한다.
	//
	//------------------------------------------------
	
	// number개의 눈송이 생성
	Init( number );
	
	// 눈
	m_WeatherType	= WEATHER_SPOT;
	m_StartX		= g_pPlayer->GetX()*TILE_X;
	m_StartY		= g_pPlayer->GetY()*TILE_Y;
	
	// 증가 개수
	m_nActiveMapEffect = 1;
	
	for (int i=0; i<number; i++)
	{
		GenerateSpot( i );
	}
	
	m_MoreEffectCount = 2;
}


//----------------------------------------------------------------------
// Generate Rain 
//----------------------------------------------------------------------
// n번 째 비 하나를 생성
//----------------------------------------------------------------------
void
MWeather::GenerateRain(const BYTE& n)
{
	BYTE type;

	switch (m_pMapEffect[n].GetType())
	{			
		//--------------------------------------------------------
		// 도착한 비는 사라지고.. 다시 새로운 비를 생성..
		//--------------------------------------------------------
		case MAP_EFFECT::MAP_EFFECT_ARRIVE4 : 
		
			type = rand() & 0x01;	// %2

			switch (type)
			{				
				case 0 :
					m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_FALL,
								0, 
								rand()%g_GameRect.right, rand()%g_GameRect.bottom, 
								-1*((rand()%4)+1), rand()%30+20, 
								rand()%25);
				break;

				case 1 :
					m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_FALL,
								rand()%2+1, 
								rand()%g_GameRect.right, rand()%g_GameRect.bottom, 
								-1*((rand()%6)+3), rand()%28+17, 
								rand()%20);
				break;
			}
		break;

		//--------------------------------------------------------
		// 도착한 비..
		//--------------------------------------------------------
		case MAP_EFFECT::MAP_EFFECT_FALL :
			m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_ARRIVE1,
						3,						
						m_pMapEffect[n].GetX(), m_pMapEffect[n].GetY(), 
						0, 0, 
						1);	
		break;

		case MAP_EFFECT::MAP_EFFECT_ARRIVE1 :
			m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_ARRIVE2,
						4,						
						m_pMapEffect[n].GetX(), m_pMapEffect[n].GetY(), 
						0, 0, 
						1);	
		break;

		case MAP_EFFECT::MAP_EFFECT_ARRIVE2 :
			m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_ARRIVE3,
						5,						
						m_pMapEffect[n].GetX(), m_pMapEffect[n].GetY(), 
						0, 0, 
						1);	
		break;

		case MAP_EFFECT::MAP_EFFECT_ARRIVE3 :
			m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_ARRIVE4,
						6,						
						m_pMapEffect[n].GetX(), m_pMapEffect[n].GetY(), 
						0, 0, 
						1);	
		break;
	}

}


//----------------------------------------------------------------------
// Generate Snow 
//----------------------------------------------------------------------
// n번 째 눈 하나를 생성
//----------------------------------------------------------------------
void
MWeather::GenerateSnow(const BYTE& n)
{
	BYTE type;

	switch (m_pMapEffect[n].GetType())
	{			
		//--------------------------------------------------------
		// 도착한 눈은 사라지고.. 다시 새로운 눈을 생성..
		//--------------------------------------------------------
		case MAP_EFFECT::MAP_EFFECT_ARRIVE6 : 
		
			type = rand() & 0x01; //%2;

			switch (type)
			{				
				case 0 :
					m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_FALL,
								rand()%5+7, 
								rand()%g_GameRect.right, rand()%g_GameRect.bottom, 
								-1*((rand()%3)+1), rand()%7+4, 
								rand()%120);
				break;

				case 1 :
					m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_FALL,
								rand()%5+7,  
								rand()%g_GameRect.right, rand()%g_GameRect.bottom, 
								-1*((rand()%3)+3), rand()%7+4, 
								rand()%120);
				break;
			}
		break;

		//--------------------------------------------------------
		// 도착한 눈..
		//--------------------------------------------------------
		case MAP_EFFECT::MAP_EFFECT_FALL :
			m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_ARRIVE1,
						12,						
						m_pMapEffect[n].GetX(), m_pMapEffect[n].GetY(), 
						0, 0, 
						(rand() & 0x01)+2);	
		break;

		case MAP_EFFECT::MAP_EFFECT_ARRIVE1 :
			m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_ARRIVE2,
						13,						
						m_pMapEffect[n].GetX(), m_pMapEffect[n].GetY(), 
						0, 0, 
						(rand() & 0x01)+2);	
		break;

		case MAP_EFFECT::MAP_EFFECT_ARRIVE2 :
			m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_ARRIVE3,
						14,						
						m_pMapEffect[n].GetX(), m_pMapEffect[n].GetY(), 
						0, 0, 
						(rand() & 0x01)+2);	
		break;

		case MAP_EFFECT::MAP_EFFECT_ARRIVE3 :
			m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_ARRIVE4,
						15,						
						m_pMapEffect[n].GetX(), m_pMapEffect[n].GetY(), 
						0, 0, 
						(rand() & 0x01)+2);	
		break;

		case MAP_EFFECT::MAP_EFFECT_ARRIVE4 :
			m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_ARRIVE5,
						16,						
						m_pMapEffect[n].GetX(), m_pMapEffect[n].GetY(), 
						0, 0, 
						(rand() & 0x01)+2);	
		break;

		case MAP_EFFECT::MAP_EFFECT_ARRIVE5 :
			m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_ARRIVE6,
						17,						
						m_pMapEffect[n].GetX(), m_pMapEffect[n].GetY(), 
						0, 0, 
						(rand() & 0x01)+2);	
		break;
	}

}

//----------------------------------------------------------------------
// Generate Spot
//----------------------------------------------------------------------
// n번 째 눈 하나를 생성
//----------------------------------------------------------------------
void
MWeather::GenerateSpot(const BYTE& n)
{
// 	BYTE type;
//	
//	switch (m_pMapEffect[n].GetType())
//	{			
//		//--------------------------------------------------------
//		// 도착한 눈은 사라지고.. 다시 새로운 눈을 생성..
//		//--------------------------------------------------------
//	case MAP_EFFECT::MAP_EFFECT_ARRIVE6 : 
//		
//		type = rand() & 0x01; //%2;
//		
//		switch (type)
//		{				
//		case 0 :
			m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_FALL,
				18+rand()%6,
				rand()%g_GameRect.right, rand()%g_GameRect.bottom, 
				0, -((rand()%4)+1),
				30+rand()%30);
//			break;
//			
//		case 1 :
//			m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_FALL,
//				18,  
//				rand()%800, rand()%600, 
//				((rand()%7)+4), 0, 
//				60);
//			break;
//		}
//		break;
		
//		//--------------------------------------------------------
//		// 도착한 눈..
//		//--------------------------------------------------------
//		case MAP_EFFECT::MAP_EFFECT_FALL :
//			m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_ARRIVE1,
//				18,						
//				m_pMapEffect[n].GetX(), m_pMapEffect[n].GetY(), 
//				0, 0, 
//				(rand() & 0x01)+2);	
//			break;
//			
//		case MAP_EFFECT::MAP_EFFECT_ARRIVE1 :
//			m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_ARRIVE2,
//				18,						
//				m_pMapEffect[n].GetX(), m_pMapEffect[n].GetY(), 
//				0, 0, 
//				(rand() & 0x01)+2);	
//			break;
//			
//		case MAP_EFFECT::MAP_EFFECT_ARRIVE2 :
//			m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_ARRIVE3,
//				18,						
//				m_pMapEffect[n].GetX(), m_pMapEffect[n].GetY(), 
//				0, 0, 
//				(rand() & 0x01)+2);	
//			break;
//			
//		case MAP_EFFECT::MAP_EFFECT_ARRIVE3 :
//			m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_ARRIVE4,
//				18,						
//				m_pMapEffect[n].GetX(), m_pMapEffect[n].GetY(), 
//				0, 0, 
//				(rand() & 0x01)+2);	
//			break;
//			
//		case MAP_EFFECT::MAP_EFFECT_ARRIVE4 :
//			m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_ARRIVE5,
//				18,						
//				m_pMapEffect[n].GetX(), m_pMapEffect[n].GetY(), 
//				0, 0, 
//				(rand() & 0x01)+2);	
//			break;
//			
//		case MAP_EFFECT::MAP_EFFECT_ARRIVE5 :
//			m_pMapEffect[n].Set(MAP_EFFECT::MAP_EFFECT_ARRIVE6,
//				18,						
//				m_pMapEffect[n].GetX(), m_pMapEffect[n].GetY(), 
//				0, 0, 
//				(rand() & 0x01)+2);	
//			break;

//	}
	
}


//----------------------------------------------------------------------
// Action
//----------------------------------------------------------------------
// 전체적으로 한 번 움직인다..
//----------------------------------------------------------------------
void		
MWeather::Action()
{
	if (m_pMapEffect==NULL)
		return;

	//---------------------------------------------------------
	// 표현되고 있는 날씨 효과가 없을 경우..
	//---------------------------------------------------------
	if (m_WeatherType == WEATHER_NULL)
	{
		return;
	}

	//---------------------------------------------------------
	// 멈추고 있는 중이면..
	//---------------------------------------------------------
	if (m_WeatherType == WEATHER_STOP)
	{
		int k=0;
		for (int i=0; i<m_nMapEffect; i++)
		{
			if (m_pMapEffect[i].Move())
			{
			}
			else
			{
				//m_pMapEffect[i].Set(0, -100, -100, 0,0,0);
				k++;
			}
		}

		if (k==m_nMapEffect)
		{		
			m_WeatherType = WEATHER_NULL;
		}

		return;
	}

	//---------------------------------------------------------
	// 현재 작동중인 Effect개수가 
	// 전체 Effect의 개수보다 적은 경우 --> 더 생성
	//---------------------------------------------------------
	if (m_nActiveMapEffect < m_nMapEffect)
	{
		if (m_MoreEffectCount==0)
		{
			// 
			BYTE start = m_nActiveMapEffect;

			m_nActiveMapEffect += 5;

			if (m_nActiveMapEffect > m_nMapEffect)
				m_nActiveMapEffect = m_nMapEffect;

			BYTE i;
			
			// 날씨 종류에 따라서 남은 Effect 몇개를 더 생성한다.
			switch (m_WeatherType)
			{
				case WEATHER_RAIN :
					for (i=start; i<m_nActiveMapEffect; i++)
					{
						GenerateRain( i );
					}
				break;

				case WEATHER_SNOW :
					for (i=start; i<m_nActiveMapEffect; i++)
					{
						GenerateSnow( i );
					}
				break;

				case WEATHER_SPOT :
					for (i=start; i<m_nActiveMapEffect; i++)
					{
						GenerateSpot( i );
					}
					break;
			}

			m_MoreEffectCount = 10;
		}
		else
		{
			m_MoreEffectCount--;
		}
	}

	//---------------------------------------------------------
	// 각각의 MAP_EFFECT에 대해서 한번의 움직임~~
	//---------------------------------------------------------
	for (int i=0; i<m_nActiveMapEffect; i++)
	{
		// 움직인다.
		if (m_pMapEffect[i].Move())
		{
			switch (m_WeatherType)
			{
				case WEATHER_RAIN :
				break;

				// 눈인 경우.. 하늘하늘~거리게...
				case WEATHER_SNOW :
//				case WEATHER_SPOT :
					if (m_pMapEffect[i].GetCount() & 0x08)
					{
						m_pMapEffect[i].SetSX( -1*(rand()%3-1) * ((rand()%3)+1) );
					}
				break;
			}
		}
		// 다 움직인 경우
		if(!m_pMapEffect[i].IsActive())
		{
			// 날씨 효과 종류에 따라서 하나만 다시 생성
			switch (m_WeatherType)
			{
				case WEATHER_RAIN :
				{
					GenerateRain( i );
				}
				break;
				
				case WEATHER_SNOW :
				{
					GenerateSnow( i );
				}
				break;

				case WEATHER_SPOT :
				{
					GenerateSpot( i );
				}
				break;
			}
		}
	}
}
	