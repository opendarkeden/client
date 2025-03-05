//----------------------------------------------------------------------
// CFilter.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "CFilter.h"
//----------------------------------------------------------------------
//
// contructor/destructor
//
//----------------------------------------------------------------------
CFilter::CFilter()
{
	m_Width		= 0;
	m_Height	= 0;
	m_ppFilter	= NULL;
}

CFilter::~CFilter()
{
	Release();
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Init
//----------------------------------------------------------------------
void
CFilter::Init(WORD width, WORD height)
{
	if (width==0 || height==0)
		return;

	// 일단 해제
	Release();

	m_Width = width;
	m_Height = height;
	
	// linear로 memory생성
	m_ppFilter = new BYTE* [m_Height];

	for (int i=0; i<m_Height; i++)
	{
		m_ppFilter[i] = new BYTE [m_Width];
	}
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void
CFilter::Release()
{
	if (m_ppFilter != NULL)
	{
		for (int i=0; i<m_Height; i++)
			delete [] m_ppFilter[i];

		delete [] m_ppFilter;
		m_ppFilter = NULL;

		m_Width = 0;
		m_Height = 0;
	}
}

//----------------------------------------------------------------------
// Set Filter
//----------------------------------------------------------------------
// pSurface의 각 점을 읽어서 그에 따른 Filter값을 만들어준다.
// Filter는 한 점에 대해서 하나의 값인데,
// 한 점은 (R,G,B)로 이루어져 있으므로...
//
// 일반적으로 Filter로 사용될 (R,G,B)의 값에서 R==G==B이라고 가정하고..
// 그 중에서 B를 선택하도록 한다. 그냥.. *_*;
// 
//----------------------------------------------------------------------
void		
CFilter::SetFilter(WORD* pSurface, WORD pitch, WORD width, WORD height)
{
	//------------------------------------------------------------
	// 현재 잡혀있는 메모리와 용량이 다를 경우 memory잡기
	//------------------------------------------------------------
	if (width!=m_Width || height!=m_Height)
		Init(width, height);

	
	register int i;
	register int j;
			

	//------------------------------------------------------------
	// pSurface의 각 점에 대한 정보를 읽어서 m_pFilter에 저장
	//------------------------------------------------------------
	WORD*	pSurfaceTemp;
	BYTE*	pFilter;
	for (i=0; i<height; i++)
	{
		pSurfaceTemp = pSurface;

		pFilter = m_ppFilter[i];

		j = width;
		do
		{
			*pFilter = (BYTE)(*pSurfaceTemp & 0x1F);	// Blue값

			pFilter++;
			pSurfaceTemp++;
			
		} while (--j);

		// 다음 줄
		pSurface = (WORD*)((BYTE*)pSurface + pitch);
	}
}

//----------------------------------------------------------------------
// 다른 Filter를 대입한다.
//----------------------------------------------------------------------
void		
CFilter::SetFilter(const CFilter& filter)
{
	// size가 0이면 return
	if (filter.m_Width==0 || filter.m_Height==0)
		return;

	// memory 잡기
	Init( filter.m_Width, filter.m_Height );

	// byte수
	int width2 = m_Width << 1;

	for (int i=0; i<m_Height; i++)
	{
		memcpy((void*)m_ppFilter[i], (void*)filter.m_ppFilter[i], width2);		
	}
}

//----------------------------------------------------------------------
// 다른 Filter를 어둡게 해서.. 대입한다.
//----------------------------------------------------------------------
void		
CFilter::SetFilterDarkness(const CFilter& filter, BYTE DarkBits)
{
	// size가 0이면 return
	if (filter.m_Width==0 || filter.m_Height==0)
		return;

	// memory 잡기
	Init( filter.m_Width, filter.m_Height );

	register int i;
	register int j;

	// 각 filter값마다 DarkBits만큼 어둡게 한다.
	for (i=0; i<m_Height; i++)
	{
		for (j=0; j<m_Width; j++)
		{
			m_ppFilter[i][j] = (filter.m_ppFilter[i][j] >> DarkBits);
		}
	}
}

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
bool		
CFilter::SaveToFile(ofstream& file)
{
	// Size 저장
	file.write((const char*)&m_Width, 2);
	file.write((const char*)&m_Height, 2);

	// 저장된 내용이 없으면 return
	if (m_Width==0 || m_Height==0 || m_ppFilter==NULL)
		return false;

	// Filter 내용을 저장한다.
	for (int i=0; i<m_Height; i++)
	{
		file.write((const char*)m_ppFilter[i], m_Width);
	}
	return true;
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
bool		
CFilter::LoadFromFile(ifstream& file)
{
	// Size Load
	file.read((char*)&m_Width, 2);
	file.read((char*)&m_Height, 2);

	// size가 0이면 return
	if (m_Width==0 || m_Height==0)
		return false;

	// memory 잡기
	Init( m_Width, m_Height );	

	// Filter 내용을 읽어온다.
	for (int i=0; i<m_Height; i++)
	{
		file.read((char*)m_ppFilter[i], m_Width);
	}
	return true;
}

//----------------------------------------------------------------------
// Blt Filter
//----------------------------------------------------------------------
// (x,y)에 filter를 출력한다.
//----------------------------------------------------------------------
void		
CFilter::BltFilter(int x, int y, const CFilter& filter)
{
	BYTE	*pDest;			// 출력할려는 위치
	BYTE	*pSource;		// 출력할려는 위치

	int startY, endY;						// y관련
	int sourceStartX, destStartX, width;	// x관련
	
	register int sourceIndex;
	register int destIndex;

	//-------------------------------------------------
	// Y출력 범위 지정
	//-------------------------------------------------
	// 화면을 넘어가는 경우
	if (y > m_Height)
	{
		return;	
	}
	// 0보다 적은 경우
	if (y < 0)
	{		
		// 마지막줄도 화면에 나타나지 않는 경우
		if (y+filter.m_Height <= 0)
		{
			return;
		}

		// 위쪽 clip
		startY = -y;

		// 아래쪽 clip도 되는 경우
		if (y+filter.m_Height > m_Height)
		{
			endY = startY + m_Height;
		}
		else
		{
			endY = filter.m_Height;
		}

		destIndex = 0;
	}
	// 0이상
	// 아래쪽 clip
	else if (y+filter.m_Height > m_Height)
	{
		startY = 0;
		endY = m_Height - y;
		destIndex = y;
	}	
	// y clip이 없는 경우
	else 
	{
		startY = 0;
		endY = filter.m_Height;
		destIndex = y;
	}

	//-------------------------------------------------
	// X출력 범위 지정
	//-------------------------------------------------
	// 화면을 넘어가는 경우
	if (x > m_Width)
	{
		return;	
	}
	// 0보다 적은 경우
	if (x < 0)
	{		
		// 마지막줄도 화면에 나타나지 않는 경우
		if (x+filter.m_Width <= 0)
		{
			return;
		}

		// 오른쪽 Clip도 필요한 경우
		if (x+filter.m_Width > m_Width)
		{
			width = m_Width;
		}
		else
		{
			width = filter.m_Width + x;
		}

		// 왼쪽 clip
		sourceStartX = -x;
		destStartX = 0;		
	}
	// 0이상
	// 아래쪽 clip
	else if (x+filter.m_Width > m_Width)
	{
		sourceStartX = 0;
		destStartX = x;
		width = m_Width - x;		
	}	
	// X clip이 없는 경우
	else 
	{
		sourceStartX = 0;		
		destStartX = x;
		width = filter.m_Width;
	}


	//-------------------------------------------------
	// startY에서 endY까지의 source를 dest로...
	//-------------------------------------------------
	for (sourceIndex=startY; sourceIndex<endY; destIndex++, sourceIndex++)
	{
		pSource = filter.m_ppFilter[sourceIndex] + sourceStartX;
		pDest	= m_ppFilter[destIndex] + destStartX;

		// width만큼 pDest에 pSource를 blt한다.
		memcpy(pDest, pSource, width);		
	}
}

//----------------------------------------------------------------------
// Blt Filter Add (31까지)
//----------------------------------------------------------------------
// (x,y)에 filter를 Lighten으로 출력한다.
//----------------------------------------------------------------------
void		
CFilter::BltFilterAdd(int x, int y, const CFilter& filter)
{
	BYTE	*pDest;			// 출력할려는 위치
	BYTE	*pSource;		// 출력할려는 위치

	int startY, endY;						// y관련
	int sourceStartX, destStartX, width;	// x관련
	
	register int sourceIndex;
	register int destIndex;

	//-------------------------------------------------
	// Y출력 범위 지정
	//-------------------------------------------------
	// 화면을 넘어가는 경우
	if (y > m_Height)
	{
		return;	
	}
	// 0보다 적은 경우
	if (y < 0)
	{		
		// 마지막줄도 화면에 나타나지 않는 경우
		if (y+filter.m_Height <= 0)
		{
			return;
		}

		// 위쪽 clip
		startY = -y;

		// 아래쪽 clip도 되는 경우
		if (y+filter.m_Height > m_Height)
		{
			endY = startY + m_Height;
		}
		else
		{
			endY = filter.m_Height;
		}

		destIndex = 0;
	}
	// 0이상
	// 아래쪽 clip
	else if (y+filter.m_Height > m_Height)
	{
		startY = 0;
		endY = m_Height - y;
		destIndex = y;
	}	
	// y clip이 없는 경우
	else 
	{
		startY = 0;
		endY = filter.m_Height;
		destIndex = y;
	}

	//-------------------------------------------------
	// X출력 범위 지정
	//-------------------------------------------------
	// 화면을 넘어가는 경우
	if (x > m_Width)
	{
		return;	
	}
	// 0보다 적은 경우
	if (x < 0)
	{		
		// 마지막줄도 화면에 나타나지 않는 경우
		if (x+filter.m_Width <= 0)
		{
			return;
		}

		// 오른쪽 Clip도 필요한 경우
		if (x+filter.m_Width > m_Width)
		{
			width = m_Width;
		}
		else
		{
			width = filter.m_Width + x;
		}

		// 왼쪽 clip
		sourceStartX = -x;
		destStartX = 0;		
	}
	// 0이상
	// 아래쪽 clip
	else if (x+filter.m_Width > m_Width)
	{
		sourceStartX = 0;
		destStartX = x;
		width = m_Width - x;		
	}	
	// X clip이 없는 경우
	else 
	{
		sourceStartX = 0;		
		destStartX = x;
		width = filter.m_Width;
	}


	//int d, s, t;
	int t;
	//-------------------------------------------------
	// startY에서 endY까지의 source를 dest로...
	//-------------------------------------------------
	for (sourceIndex=startY; sourceIndex<endY; destIndex++, sourceIndex++)
	{
		pSource = filter.m_ppFilter[sourceIndex] + sourceStartX;
		pDest	= m_ppFilter[destIndex] + destStartX;

		//------------------------------------------------------------	
		// Lighten : min(source, dest)
		// filter값은 0이 옅은색이 된다. 0xFF가 최고...
		//------------------------------------------------------------
		// width만큼 pDest에 pSource를 blt한다.
		//memcpyLighten( pDest, pSource, width );		
		register int i = width;
		
		if (i>0)
		{
			do
			{
				///*
				//s = *pSource;
				//d = *pDest;	
				
				t = *pDest + *pSource;
				if (t > 31)
				{
					*pDest = 31;
				}
				else
				{
					*pDest = t;
				}		
					
				pDest++;
				pSource++;		
			} while(--i);
		}
	}

}

//----------------------------------------------------------------------
// Blt Filter Subtract
//----------------------------------------------------------------------
// (x,y)에 filter를 Lighten으로 출력한다.
//----------------------------------------------------------------------
void		
CFilter::BltFilterSub(int x, int y, const CFilter& filter)
{
	BYTE	*pDest;			// 출력할려는 위치
	BYTE	*pSource;		// 출력할려는 위치

	int startY, endY;						// y관련
	int sourceStartX, destStartX, width;	// x관련
	
	register int sourceIndex;
	register int destIndex;

	//-------------------------------------------------
	// Y출력 범위 지정
	//-------------------------------------------------
	// 화면을 넘어가는 경우
	if (y > m_Height)
	{
		return;	
	}
	// 0보다 적은 경우
	if (y < 0)
	{		
		// 마지막줄도 화면에 나타나지 않는 경우
		if (y+filter.m_Height <= 0)
		{
			return;
		}

		// 위쪽 clip
		startY = -y;

		// 아래쪽 clip도 되는 경우
		if (y+filter.m_Height > m_Height)
		{
			endY = startY + m_Height;
		}
		else
		{
			endY = filter.m_Height;
		}

		destIndex = 0;
	}
	// 0이상
	// 아래쪽 clip
	else if (y+filter.m_Height > m_Height)
	{
		startY = 0;
		endY = m_Height - y;
		destIndex = y;
	}	
	// y clip이 없는 경우
	else 
	{
		startY = 0;
		endY = filter.m_Height;
		destIndex = y;
	}

	//-------------------------------------------------
	// X출력 범위 지정
	//-------------------------------------------------
	// 화면을 넘어가는 경우
	if (x > m_Width)
	{
		return;	
	}
	// 0보다 적은 경우
	if (x < 0)
	{		
		// 마지막줄도 화면에 나타나지 않는 경우
		if (x+filter.m_Width <= 0)
		{
			return;
		}

		// 오른쪽 Clip도 필요한 경우
		if (x+filter.m_Width > m_Width)
		{
			width = m_Width;
		}
		else
		{
			width = filter.m_Width + x;
		}

		// 왼쪽 clip
		sourceStartX = -x;
		destStartX = 0;		
	}
	// 0이상
	// 아래쪽 clip
	else if (x+filter.m_Width > m_Width)
	{
		sourceStartX = 0;
		destStartX = x;
		width = m_Width - x;		
	}	
	// X clip이 없는 경우
	else 
	{
		sourceStartX = 0;		
		destStartX = x;
		width = filter.m_Width;
	}


	//int d, s, t;
	int t, d, s;
	//-------------------------------------------------
	// startY에서 endY까지의 source를 dest로...
	//-------------------------------------------------
	for (sourceIndex=startY; sourceIndex<endY; destIndex++, sourceIndex++)
	{
		pSource = filter.m_ppFilter[sourceIndex] + sourceStartX;
		pDest	= m_ppFilter[destIndex] + destStartX;

		//------------------------------------------------------------	
		// Lighten : min(source, dest)
		// filter값은 0이 옅은색이 된다. 0xFF가 최고...
		//------------------------------------------------------------
		// width만큼 pDest에 pSource를 blt한다.
		//memcpyLighten( pDest, pSource, width );		
		register int i = width;
		
		if (i>0)
		{
			do
			{
				///*
				//s = *pSource;
				//d = *pDest;	
				d = *pDest;
				s = *pSource;
				
				t = d - s;
				if (t < 0)
				{
					*pDest = 0;
				}
				else
				{
					*pDest = (BYTE)t;
				}

				/*
				// s가 밝다
                if (d > s)
                {
                        t = s - ((15 - d)>>1);
                        if (t < 0) t = 0;
                        *pDest = t;
                }
				// d가 밝다
                else
                {
                        t = d - ((15 - s)>>1);
                        if (t < 0) t = 0;
                        *pDest = t;
                }
				*/

				//*pDest = min( *pDest, *pSource );				
					
				pDest++;
				pSource++;		
			} while(--i);
		}
	}

}

//----------------------------------------------------------------------
// Blt 4444
//----------------------------------------------------------------------
// Texture용으로 사용할 수 있도록 Filter를 pSurface에 출력한다.
// Filter는 Alpha값을 의미하게 된다.
//
// (-_-;;) Surface와 Filter의 Width와 Height는 같다고 본다.
// 다를 경우는 ... 에러가 날 수도.. 우하.. 
//----------------------------------------------------------------------
void
CFilter::Blt4444(WORD* pSurface, WORD pitch)
{
	register int i;
	register int j;

	BYTE* pFilter;
	WORD* pSurfaceTemp;

	for (i=0; i<m_Height; i++)
	{
		pSurfaceTemp = pSurface;
		pFilter = m_ppFilter[i];

		j = m_Width;
		do 
		{
			// 기존의 RGB값을 살릴 경우..
			//*pSurfaceTemp = (*pFilter << 12) | (*pSurfaceTemp & 0x0FFF);

			*pSurfaceTemp = (*pFilter << 12);
			pSurfaceTemp ++;
			pFilter ++;
		} while (--j);

		pSurface = (WORD*)((BYTE*)pSurface + pitch);
	}
}

//----------------------------------------------------------------------
// Blt 4444
//----------------------------------------------------------------------
// Texture용으로 사용할 수 있도록 Filter를 pSurface에 출력한다.
// Filter는 Alpha값을 의미하게 된다.
//
// (-_-;;) Surface와 Filter의 Width와 Height는 같다고 본다.
// 다를 경우는 ... 에러가 날 수도.. 우하.. 
//----------------------------------------------------------------------
void
CFilter::Blt4444Color(WORD* pSurface, WORD pitch, WORD color)
{
	// R:G:B만 살린다.
	color &= 0x0FFF;

	register int i;
	register int j;

	BYTE* pFilter;
	WORD* pSurfaceTemp;

	for (i=0; i<m_Height; i++)
	{
		pSurfaceTemp = pSurface;
		pFilter = m_ppFilter[i];

		j = m_Width;
		do 
		{
			// 기존의 RGB값을 살릴 경우..
			*pSurfaceTemp = (*pFilter << 12) | color;

			//*pSurfaceTemp = (*pFilter << 12);
			pSurfaceTemp ++;
			pFilter ++;
		} while (--j);

		pSurface = (WORD*)((BYTE*)pSurface + pitch);
	}
}