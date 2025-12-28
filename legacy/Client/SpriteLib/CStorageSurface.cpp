//----------------------------------------------------------------------
// CStorageSurface.cpp
//----------------------------------------------------------------------
#include "client_PCH.h"
#include "CDirectDrawSurface.h"
#include "CStorageSurface.h"
//#include "DebugInfo.h"

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
CStorageSurface::CStorageSurface()
{
	m_Size				= 0;
	m_pPoint			= NULL;
	m_pStorageSurface	= NULL;	
}

CStorageSurface::~CStorageSurface()
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
// (사용할Surface개수, surface가로크기, 세로크기)
//----------------------------------------------------------------------
void
CStorageSurface::Init(int size, int width, int height)
{
	// parameter가 잘못된 경우..
	if (size==0 || width==0 || height==0)
		return;

	// 일단 이전에 있던걸 해제한 후에..
	Release();	

	// 새로 설정.. 메모리 잡기
	m_Size = size;
	m_pPoint = new POINT [m_Size];	
	m_pStorageSurface = new CDirectDrawSurface [m_Size];

	
	for (int i=0; i<m_Size; i++)	
	{
		// 각 surface의 기억 위치를 초기화한다.
		m_pPoint[i].x = 0;
		m_pPoint[i].y = 0;

		// 각 surface의 크기를 결정해둔다.
		m_pStorageSurface[i].InitOffsurface(width, height);
		m_pStorageSurface[i].SetTransparency(0);
	}
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
// 메모리 제거
//----------------------------------------------------------------------
void
CStorageSurface::Release()
{
	if (m_pStorageSurface!=NULL)
	{
		delete [] m_pStorageSurface;
		m_pStorageSurface = NULL;
	}

	if (m_pPoint!=NULL)
	{
		delete [] m_pPoint;
		m_pPoint = NULL;
	}

	m_Size = 0;
}

//----------------------------------------------------------------------
// Store
//----------------------------------------------------------------------
// pSurface의 pPoint부분을 읽어서 i번째 surface에 기억시켜둔다.
//----------------------------------------------------------------------
void				
CStorageSurface::Store(int i, CDirectDrawSurface* pSurface, POINT* pPoint)
{
	// i가 없는 surface번호일 경우..
	if (i<0 || i>=m_Size)
		return;

	POINT	origin = {0, 0};

	int width = pPoint->x + m_pStorageSurface[i].GetWidth();
	int height = pPoint->y + m_pStorageSurface[i].GetHeight();

	// clipping
	if (pPoint->x < 0)
	{
		width += -pPoint->x;
		pPoint->x = 0;		
	}
	else if (width > pSurface->GetWidth())
	{
		pPoint->x -= width - pSurface->GetWidth();
		width = pSurface->GetWidth();
	}	

	// clipping
	if (pPoint->y < 0)
	{
		height += -pPoint->y;
		pPoint->y = 0;
	}
	else if (height > pSurface->GetHeight())
	{
		pPoint->y -= height - pSurface->GetHeight();
		height = pSurface->GetHeight();
	}	

	// pSurface에서 기억시켜둘 영역
	RECT	rect = 
			{
				pPoint->x, 
				pPoint->y,
				width,
				height
			};	
	
	
	// 기억시킨 위치를 저장해둔다.
	m_pPoint[i] = *pPoint;

	// m_pStorageSurface[i]의 (0,0)에 
	// pSurface의 영역을 기억시켜둔다.
	m_pStorageSurface[i].BltNoColorkey(&origin, pSurface, &rect);

	//DEBUG_ADD_FORMAT("Store : [%d] (%d, %d)", i, m_pPoint[i].x, m_pPoint[i].y);
}

//----------------------------------------------------------------------
// Restore
//----------------------------------------------------------------------
// i번째 surface를 pSurface의 원래위치(m_pPoint)로 출력해준다.
//----------------------------------------------------------------------
void				
CStorageSurface::Restore(int i, CDirectDrawSurface* pSurface, POINT* pPoint) const
{
	// i가 없는 surface번호일 경우..
	if (i<0 || i>=m_Size)
		return;

	// pSurface에서 기억시켜둘 영역
	RECT	rect = 
			{
				0, 
				0, 
				m_pStorageSurface[i].GetWidth(),
				m_pStorageSurface[i].GetHeight()
			};


	// pSurface의 원래 위치(m_pPoint)에 
	// m_pStorageSurface[i]의 기억된 영역을 출력시킨다.	
	if (pPoint==NULL)
	{
		POINT point = m_pPoint[i];
		pSurface->BltNoColorkey(&point, &m_pStorageSurface[i], &rect);
	}
	else
	{
		pSurface->BltNoColorkey(pPoint, &m_pStorageSurface[i], &rect);
	}

	//DEBUG_ADD_FORMAT("Restore : [%d] (%d, %d)", i, m_pPoint[i].x, m_pPoint[i].y);
}

