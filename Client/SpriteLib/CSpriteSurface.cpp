//----------------------------------------------------------------------
// CSpriteSurface.cpp
//----------------------------------------------------------------------
#include "client_PCH.h"
#include "CSprite.h"
#include "CAlphaSprite.h"
#include "CIndexSprite.h"
#include "CShadowSprite.h"
#include "CSpriteOutlineManager.h"
#include "CFilter.h"
#include "CSpriteSurface.h"


//----------------------------------------------------------------------
// Static member
//----------------------------------------------------------------------
int	CSpriteSurface::s_Value1 = 1;
int	CSpriteSurface::s_Value2 = 31;
int	CSpriteSurface::s_Value3 = 1;

// memcpy Effect
FUNCTION_MEMCPYEFFECT 	CSpriteSurface::s_pMemcpyEffectFunction = memcpyEffectDarker;
FUNCTION_MEMCPYEFFECT	CSpriteSurface::s_pMemcpyEffectFunctionTable[MAX_EFFECT] =
{
	memcpyEffectDarker,
	memcpyEffectGrayScale,
	memcpyEffectLighten,
	memcpyEffectDarken,
	memcpyEffectColorDodge,
	memcpyEffectScreen,
	memcpyEffectDodgeBurn,
	memcpyEffectDifferent,
	memcpyEffectGradation,
	memcpyEffectSimpleOutline,
	memcpyEffectWipeOut,
	memcpyEffectNet,
	memcpyEffectGrayScaleVarious,
	memcpyEffectScreenAlpha,
};

FUNCTION_MEMCPYPALEFFECT 	CSpriteSurface::s_pMemcpyPalEffectFunction = memcpyPalEffectDarker;
FUNCTION_MEMCPYPALEFFECT	CSpriteSurface::s_pMemcpyPalEffectFunctionTable[MAX_EFFECT] =
{
	memcpyPalEffectDarker,
	memcpyPalEffectGrayScale,
	memcpyPalEffectLighten,
	memcpyPalEffectDarken,
	memcpyPalEffectColorDodge,
	memcpyPalEffectScreen,
	memcpyPalEffectDodgeBurn,
	memcpyPalEffectDifferent,
	memcpyPalEffectGradation,
	memcpyPalEffectSimpleOutline,
	memcpyPalEffectWipeOut,
	memcpyPalEffectNet,
	memcpyPalEffectGrayScaleVarious,
	memcpyPalEffectScreenAlpha,
};

WORD		CSpriteSurface::s_EffectScreenTableR[32][32];
WORD		CSpriteSurface::s_EffectScreenTableG[32][32];
WORD		CSpriteSurface::s_EffectScreenTableB[32][32];


//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------

CSpriteSurface::CSpriteSurface()
{
}

CSpriteSurface::~CSpriteSurface()
{

}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// VLine
//----------------------------------------------------------------------
// 세로 선 그리기 
//----------------------------------------------------------------------
void
CSpriteSurface::VLine(int x, int y, int length, WORD color)
{
	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x+1;
	rect.bottom = y+length;

	FillRect(&rect, color);
}

//----------------------------------------------------------------------
// HLine
//----------------------------------------------------------------------
// 가로선 그리기 
//----------------------------------------------------------------------
void
CSpriteSurface::HLine(int x, int y, int length, WORD color)
{
	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x+length;
	rect.bottom = y+1;

	FillRect(&rect, color);
}

//----------------------------------------------------------------------
// DrawRect
//----------------------------------------------------------------------
// 속이 빈 상자 그리기 
//----------------------------------------------------------------------
void
CSpriteSurface::DrawRect(RECT* pRect, WORD color)
{
	if (pRect->left > pRect->right) { int temp=pRect->left; pRect->left=pRect->right; pRect->right=temp; }
	if (pRect->top > pRect->bottom) { int temp=pRect->top; pRect->top=pRect->bottom; pRect->bottom=temp; }

	int width = abs(pRect->right - pRect->left)+1,
		height = abs(pRect->bottom - pRect->top)+1;

	HLine(pRect->left, pRect->top, width, color);
	HLine(pRect->left, pRect->bottom, width, color);

	VLine(pRect->left, pRect->top, height, color);
	VLine(pRect->right, pRect->top, height, color);
}

//----------------------------------------------------------------------
// 직선 그리기 
//----------------------------------------------------------------------
void
CSpriteSurface::Line(int x1, int y1, int x2, int y2, WORD color)
{
	// 한 점씩 찍어서 직선을 그린다.	
	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	lPitch		= m_ddsd.lPitch;

	WORD* lpSurfaceTemp;

	//
	// Bresenham algolithm
	//
	int x, y;
	int d, dx, dy;
	int Aincr, Bincr, xincr, yincr;
	int t;

    if (abs(x2 - x1) > abs(y2 - y1)) 
    {
        if (x1 > x2)
        {          
            t = x1;
            x1 = x2;
            x2 = t;
            t = y1;
            y1 = y2;
            y2 = t;
        }

        if (y2 > y1)
			yincr = 1;
        else 
			yincr = -1;

        dx = x2 - x1;
        dy = abs(y2 - y1);
        d = 2 * dy - dx;
        Aincr = 2 * (dy - dx);
        Bincr = 2 * dy;

        x = x1;
        y = y1;
        
		// PutPixel(x,y, color)
		if(x > m_ClipLeft && x < m_ClipRight && y > m_ClipTop && y < m_ClipBottom)
		{
			lpSurfaceTemp = (WORD*)((BYTE*)lpSurface + lPitch*y + (x<<1));
			*lpSurfaceTemp = color;
		}

        for (x++; x <= x2; x++)
        {
			if (d >= 0) 
            {
                y += yincr;
                d += Aincr;
            }
            else 
				d += Bincr;

            // PutPixel(x,y, color)
			if(x > m_ClipLeft && x < m_ClipRight && y > m_ClipTop && y < m_ClipBottom)
			{
				lpSurfaceTemp = (WORD*)((BYTE*)lpSurface + lPitch*y + (x<<1));
				*lpSurfaceTemp = color;
			}
        }            
    }    
	else
    {
        if (y1 > y2)
        {          
            t = x1;
            x1 = x2;
            x2 = t;
            t = y1;
            y1 = y2;
            y2 = t;
        }

        if (x2 > x1)
            xincr = 1;
        else 
			xincr = -1;
		
        dx = abs(x2 - x1);
        dy = y2 - y1;
        d = 2*dx - dy;
        Aincr = 2 * (dx - dy);
        Bincr = 2 * dx;
        x = x1;
        y = y1;

        // PutPixel(x,y, color)
		if(x > m_ClipLeft && x < m_ClipRight && y > m_ClipTop && y < m_ClipBottom)
		{
	        lpSurfaceTemp = (WORD*)((BYTE*)lpSurface + lPitch*y + (x<<1));
			*lpSurfaceTemp = color;
		}

        for (y++; y <= y2; y++)
        {
            if (d >= 0)
            {
                x += xincr;
                d += Aincr;
            }
            else
				d += Bincr;

            // PutPixel(x,y, color)
			if(x > m_ClipLeft && x < m_ClipRight && y > m_ClipTop && y < m_ClipBottom)
			{
				lpSurfaceTemp = (WORD*)((BYTE*)lpSurface + lPitch*y + (x<<1));
				*lpSurfaceTemp = color;
			}
        }  
    }
}

//----------------------------------------------------------------------
// Rect영역을 읽어서 Point위치에 찍을 경우
// 화면 크기를 넘지 않도록 좌표를 다시 설정해준다.
//----------------------------------------------------------------------
bool
CSpriteSurface::ClippingRectToPoint(RECT*& pRect, POINT*& pPoint)
{
	// 영역을 넘어가는지에 대한 체크
	if (pPoint->x>=(int)m_ClipRight || pPoint->y>=(int)m_ClipBottom)		
		return false;

	// width clipping
	if (pPoint->x<m_ClipLeft) 
	{
		pRect->left = pRect->left-pPoint->x + m_ClipLeft; //abs(pPoint->x);
		pPoint->x = m_ClipLeft;
	} 
	else if ((int)m_ClipRight-pPoint->x < pRect->right-pRect->left) 
	{
		pRect->right = pRect->left+ m_ClipRight-pPoint->x;
	}

	// height clipping
	if (pPoint->y<m_ClipTop) 
	{
		pRect->top = pRect->top-pPoint->y + m_ClipTop;
		pPoint->y = m_ClipTop;
	} 
	else if ((int)m_ClipBottom-pPoint->y < pRect->bottom-pRect->top) 
	{
		pRect->bottom = pRect->top+ m_ClipBottom-pPoint->y;
	}

	// 화면 영역을 넘어가면 안된다->
	if (pRect->left >= pRect->right || pRect->top >= pRect->bottom)
		return false;

	return true;
}


//----------------------------------------------------------------------
// Blt Darkness
//----------------------------------------------------------------------
// SourceSurface를 DarkBits만큼 어둡게 만들어서 *this에 출력
//----------------------------------------------------------------------
void
CSpriteSurface::BltHalf(POINT* pPoint, 
							   CDirectDrawSurface* SourceSurface, RECT*  pRect)
{
	//------------------------
	// Clipping
	//------------------------
	// TargetSurface의 영역을 넘어가는지에 대한 체크
	if (pPoint->x>=(int)m_ClipRight || pPoint->y>=(int)m_ClipBottom) 
		return;

	POINT pt = *pPoint;

	// width clipping
	if (pt.x<m_ClipLeft) 
	{
		pRect->left = pRect->left-pt.x + m_ClipLeft;
		pt.x = m_ClipLeft;
	} 
	else if ((int)m_ClipRight-pt.x < pRect->right-pRect->left) 
	{
		pRect->right = pRect->left+ m_ClipRight-pt.x;
	}

	// height clipping
	if (pt.y<m_ClipTop) 
	{
		pRect->top = pRect->top-pt.y + m_ClipTop;
		pt.y = m_ClipTop;
	} 
	else if ((int)m_ClipBottom-pt.y < pRect->bottom-pRect->top) 
	{
		pRect->bottom = pRect->top+ m_ClipBottom-pt.y;
	}
	
	
	void*	lpSurfaceTemp	= m_ddsd.lpSurface;
	long	lPitchTemp		= m_ddsd.lPitch;

	WORD*	lpTargetSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	lTargetPitch	= m_ddsd.lPitch;

	// 두 Surface에 Lock을 건다.	
	SourceSurface->Lock();

	WORD*	lpSourceSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	lSourcePitch	= m_ddsd.lPitch;

	lpTargetSurface = (WORD*)((BYTE*)lpTargetSurface + pt.y*lTargetPitch) + pt.x;
	lpSourceSurface = (WORD*)((BYTE*)lpSourceSurface + pRect->top*lSourcePitch) + pRect->left;

	int	len = pRect->right - pRect->left + 1;
	for (int register i=pRect->top; i<pRect->bottom; i++)
	{
		memcpyHalf(lpTargetSurface, lpSourceSurface, len);
		
		lpTargetSurface = (WORD*)((BYTE*)lpTargetSurface + lTargetPitch);
		lpSourceSurface = (WORD*)((BYTE*)lpSourceSurface + lSourcePitch);
	}

	// unlock
	//Unlock();
	
	SourceSurface->Unlock();
	m_ddsd.lpSurface	= lpSurfaceTemp;
	m_ddsd.lPitch		= lPitchTemp;
}


//----------------------------------------------------------------------
// Blt Darkness
//----------------------------------------------------------------------
// SourceSurface를 DarkBits만큼 어둡게 만들어서 *this에 출력
//----------------------------------------------------------------------
void
CSpriteSurface::BltDarkness(POINT* pPoint, 
							   CDirectDrawSurface* SourceSurface, RECT*  pRect,
							   BYTE DarkBits)
{
	//------------------------
	// Clipping
	//------------------------
	// TargetSurface의 영역을 넘어가는지에 대한 체크
	if (pPoint->x>=(int)m_ClipRight || pPoint->y>=(int)m_ClipBottom) 
		return;

	POINT pt = *pPoint;

	// width clipping
	if (pt.x<m_ClipLeft) 
	{
		pRect->left = pRect->left-pt.x + m_ClipLeft;
		pt.x = m_ClipLeft;
	} 
	else if ((int)m_ClipRight-pt.x < pRect->right-pRect->left) 
	{
		pRect->right = pRect->left+ m_ClipRight-pt.x;
	}

	// height clipping
	if (pt.y<m_ClipTop) 
	{
		pRect->top = pRect->top-pt.y + m_ClipTop;
		pt.y = m_ClipTop;
	} 
	else if ((int)m_ClipBottom-pt.y < pRect->bottom-pRect->top) 
	{
		pRect->bottom = pRect->top+ m_ClipBottom-pt.y;
	}
	
	
	void*	lpSurfaceTemp	= m_ddsd.lpSurface;
	long	lPitchTemp		= m_ddsd.lPitch;

	WORD*	lpTargetSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	lTargetPitch	= m_ddsd.lPitch;

	// 두 Surface에 Lock을 건다.	
	SourceSurface->Lock();

	WORD*	lpSourceSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	lSourcePitch	= m_ddsd.lPitch;

	lpTargetSurface = (WORD*)((BYTE*)lpTargetSurface + pt.y*lTargetPitch) + pt.x;
	lpSourceSurface = (WORD*)((BYTE*)lpSourceSurface + pRect->top*lSourcePitch) + pRect->left;

	s_Value1 = DarkBits;
	int	len = pRect->right - pRect->left + 1;
	for (int register i=pRect->top; i<pRect->bottom; i++)
	{
		memcpyDarkness(lpTargetSurface, lpSourceSurface, len);
		
		lpTargetSurface = (WORD*)((BYTE*)lpTargetSurface + lTargetPitch);
		lpSourceSurface = (WORD*)((BYTE*)lpSourceSurface + lSourcePitch);
	}

	// unlock
	//Unlock();
	//SourceSurface->Unlock();
	m_ddsd.lpSurface	= lpSurfaceTemp;
	m_ddsd.lPitch		= lPitchTemp;
}

//----------------------------------------------------------------------
// Blt Darkness
//----------------------------------------------------------------------
// SourceSurface를 DarkBits만큼 어둡게 만들어서 *this에 출력
//----------------------------------------------------------------------
void
CSpriteSurface::BltBrightness(POINT* pPoint, 
							   CDirectDrawSurface* SourceSurface, RECT*  pRect,
							   BYTE BrightBits)
{
	//------------------------
	// Clipping
	//------------------------
	// TargetSurface의 영역을 넘어가는지에 대한 체크
	if (pPoint->x>=(int)m_ClipRight || pPoint->y>=(int)m_ClipBottom) 
		return;

	POINT pt = *pPoint;

	// width clipping
	if (pt.x<m_ClipLeft) 
	{
		pRect->left = pRect->left-pt.x + m_ClipLeft;
		pt.x = m_ClipLeft;
	} 
	else if ((int)m_ClipRight-pt.x < pRect->right-pRect->left) 
	{
		pRect->right = pRect->left+ m_ClipRight-pt.x;
	}

	// height clipping
	if (pt.y<m_ClipTop) 
	{
		pRect->top = pRect->top-pt.y + m_ClipTop;
		pt.y = m_ClipTop;
	} 
	else if ((int)m_ClipBottom-pt.y < pRect->bottom-pRect->top) 
	{
		pRect->bottom = pRect->top+ m_ClipBottom-pt.y;
	}
	
			
	void*	lpSurfaceTemp	= m_ddsd.lpSurface;
	long	lPitchTemp		= m_ddsd.lPitch;

	WORD*	lpTargetSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	lTargetPitch	= m_ddsd.lPitch;

	// 두 Surface에 Lock을 건다.	
	SourceSurface->Lock();

	WORD*	lpSourceSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	lSourcePitch	= m_ddsd.lPitch;

	lpTargetSurface = (WORD*)((BYTE*)lpTargetSurface + pt.y*lTargetPitch) + pt.x;
	lpSourceSurface = (WORD*)((BYTE*)lpSourceSurface + pRect->top*lSourcePitch) + pRect->left;

	s_Value1 = BrightBits;
	int	len = pRect->right - pRect->left + 1;
	for (int register i=pRect->top; i<pRect->bottom; i++)
	{
		memcpyBrightness(lpTargetSurface, lpSourceSurface, len);
		
		lpTargetSurface = (WORD*)((BYTE*)lpTargetSurface + lTargetPitch);
		lpSourceSurface = (WORD*)((BYTE*)lpSourceSurface + lSourcePitch);
	}

	// unlock
	//Unlock();
	//SourceSurface->Unlock();
	m_ddsd.lpSurface	= lpSurfaceTemp;
	m_ddsd.lPitch		= lPitchTemp;
}

//----------------------------------------------------------------------
// Blt DarknessFilter
//----------------------------------------------------------------------
// SourceSurface를 Filter로 생각해서
// 현재 Surface의 각 점(R,G,B)에서 
// SourceSurface의 각 점(R,G,B) 만큼을 빼주어서 어둡게 만든다.
//----------------------------------------------------------------------
void
CSpriteSurface::BltDarknessFilter(POINT* pPoint, 
							   CDirectDrawSurface* SourceSurface, RECT*  pRect,
							   WORD TransColor)
{
	//------------------------
	// Clipping
	//------------------------
	// TargetSurface의 영역을 넘어가는지에 대한 체크
	if (pPoint->x>=(int)m_ClipRight || pPoint->y>=(int)m_ClipBottom) 
		return;

	POINT pt = *pPoint;
	
	// width clipping
	if (pt.x<m_ClipLeft) 
	{
		pRect->left = pRect->left-pt.x + m_ClipLeft;
		pt.x = m_ClipLeft;
	} 
	else if ((int)m_ClipRight-pt.x < pRect->right-pRect->left) 
	{
		pRect->right = pRect->left+ m_ClipRight-pt.x;
	}

	// height clipping
	if (pt.y<m_ClipTop) 
	{
		pRect->top = pRect->top-pt.y + m_ClipTop;
		pt.y = m_ClipTop;
	} 
	else if ((int)m_ClipBottom-pt.y < pRect->bottom-pRect->top) 
	{
		pRect->bottom = pRect->top+ m_ClipBottom-pt.y;
	}


	
	
	WORD	*lpTargetSurface, *lpSourceSurface,
			*lpTargetTemp, *lpSourceTemp;
	WORD	lTargetPitch, lSourcePitch;

	// 두 Surface에 Lock을 건다.
	LockW(lpTargetSurface, lTargetPitch);
	SourceSurface->LockW(lpSourceSurface, lSourcePitch);

	lpTargetSurface = (WORD*)((BYTE*)lpTargetSurface + pt.y*lTargetPitch) + pt.x;
	lpSourceSurface = (WORD*)((BYTE*)lpSourceSurface + pRect->top*lSourcePitch) + pRect->left;

	//------------------------------------------------------------	
	// Darker : BG - SPR_Filter
	//------------------------------------------------------------	
	BYTE	filter;

	for (int register i=pRect->top; i<pRect->bottom; i++)
	{
		lpTargetTemp = lpTargetSurface;
		lpSourceTemp = lpSourceSurface;

		for (int register j=pRect->left; j<pRect->right; j++)
		{	
			// 투명색이 아닐때 출력
			if (*lpTargetTemp != TransColor)
			{				
				filter = *lpSourceTemp & 0x1F;	// Blue값

				*lpTargetTemp = 
					(((((*lpTargetTemp >> 11) & 0x1F) * filter) >> 5) << 11)
					| (((((*lpTargetTemp >> 5) & 0x3F) * (filter<<1)) >> 6) << 5)
					| (((*lpTargetTemp & 0x1F) * filter) >> 5);
			}
			
			lpTargetTemp ++;
			lpSourceTemp ++;
		}
		
		lpTargetSurface = (WORD*)((BYTE*)lpTargetSurface + lTargetPitch);
		lpSourceSurface = (WORD*)((BYTE*)lpSourceSurface + lSourcePitch);
	}

	// unlock
	Unlock();
	SourceSurface->Unlock();
}

//----------------------------------------------------------------------
// Blt SpriteOutline
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteOutline(CSpriteOutlineManager* pSOM, WORD color)
{
	POINT	point = pSOM->GetPosition();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (point.x >= m_ClipRight || point.y >= m_ClipBottom)
		return;

	//WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	//WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSOM->GetWidth();
	rect.bottom = pSOM->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= point.x + pSOM->GetWidth();
	int yPlusSpriteHeight	= point.y + pSOM->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (point.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (point.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -point.x + m_ClipLeft;		
		point.x = m_ClipLeft;
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - point.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - point.y;					
				}				
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + point.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - point.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			//pSOM->BltClipWidth( (WORD*)m_ddsd.lpSurface, Pitch, color, &rect);
			pSOM->BltClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
				}				
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + point.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
				}				
			}

			// 왼쪽 Clip			
			//pSOM->BltClipLeft( (WORD*)m_ddsd.lpSurface, Pitch, color, &rect);
			pSOM->BltClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - point.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (point.x<<1));
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - point.y;
				}				
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + point.y*Pitch + (point.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - point.y;										
				}								
			}	
			
			// 오른쪽 Clip			
			//pSOM->BltClipRight( (WORD*)m_ddsd.lpSurface, Pitch, color, &rect);
			pSOM->BltClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (point.x<<1));
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
				}
				
				// Height만 Clip
				//pSOM->BltClipHeight( (WORD*)m_ddsd.lpSurface, Pitch, color, &rect);
				pSOM->BltClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + point.y*Pitch + (point.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
					// Height만 Clip
					//pSOM->BltClipHeight( (WORD*)m_ddsd.lpSurface, Pitch, color, &rect);
					pSOM->BltClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
				}
				else
				{
					// No Clip					
					pSOM->Blt( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt SpriteOutline Only
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteOutlineOnly(CSpriteOutlineManager* pSOM, WORD color)
{
	POINT	point = pSOM->GetPosition();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (point.x >= m_ClipRight || point.y >= m_ClipBottom)
		return;


	//WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	//WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSOM->GetWidth();
	rect.bottom = pSOM->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= point.x + pSOM->GetWidth();
	int yPlusSpriteHeight	= point.y + pSOM->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (point.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (point.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -point.x + m_ClipLeft;		
		point.x = m_ClipLeft;
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - point.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - point.y;					
				}				
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + point.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - point.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			//pSOM->BltOutlineClipWidth( (WORD*)m_ddsd.lpSurface, Pitch, color, &rect);
			pSOM->BltOutlineClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
				}				
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + point.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
				}				
			}

			// 왼쪽 Clip			
			//pSOM->BltOutlineClipLeft( (WORD*)m_ddsd.lpSurface, Pitch, color, &rect);
			pSOM->BltOutlineClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - point.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (point.x<<1));
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - point.y;
				}				
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + point.y*Pitch + (point.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - point.y;										
				}								
			}	
			
			// 오른쪽 Clip			
			//pSOM->BltOutlineClipRight( (WORD*)m_ddsd.lpSurface, Pitch, color, &rect);
			pSOM->BltOutlineClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (point.x<<1));
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
				}
				
				// Height만 Clip
				//pSOM->BltOutlineClipHeight( (WORD*)m_ddsd.lpSurface, Pitch, color, &rect);
				pSOM->BltOutlineClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + point.y*Pitch + (point.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
					// Height만 Clip
					//pSOM->BltOutlineClipHeight( (WORD*)m_ddsd.lpSurface, Pitch, color, &rect);
					pSOM->BltOutlineClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
				}
				else
				{
					// No Clip					
					pSOM->BltOutline( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt SpriteOutline
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteOutlineDarkness(CSpriteOutlineManager* pSOM, WORD color, BYTE DarkBits)
{
	POINT	point = pSOM->GetPosition();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (point.x >= m_ClipRight || point.y >= m_ClipBottom)
		return;


	//WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	//WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSOM->GetWidth();
	rect.bottom = pSOM->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= point.x + pSOM->GetWidth();
	int yPlusSpriteHeight	= point.y + pSOM->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (point.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (point.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -point.x + m_ClipLeft;		
		point.x = m_ClipLeft;
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - point.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - point.y;					
				}				
			}
			else
			{
				//lpSurface = (WORD*)( (BYTE*)lpSurface + point.y*m_ddsd.lPitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - point.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			//pSOM->BltClipWidth( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
			pSOM->BltDarknessClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, DarkBits, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
				}				
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + point.y*m_ddsd.lPitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
				}				
			}

			// 왼쪽 Clip			
			//pSOM->BltClipLeft( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
			pSOM->BltDarknessClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, DarkBits, &rect);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - point.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (point.x<<1));
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - point.y;
				}				
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + point.y*m_ddsd.lPitch + (point.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - point.y;										
				}								
			}	
			
			// 오른쪽 Clip			
			//pSOM->BltClipRight( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
			pSOM->BltDarknessClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, DarkBits, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (point.x<<1));
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
				}
				
				// Height만 Clip
				//pSOM->BltClipHeight( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
				pSOM->BltDarknessClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, DarkBits, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + point.y*m_ddsd.lPitch + (point.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
					// Height만 Clip
					//pSOM->BltClipHeight( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
					pSOM->BltDarknessClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, DarkBits, &rect);
				}
				else
				{
					// No Clip					
					pSOM->BltDarkness( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, DarkBits);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}


//----------------------------------------------------------------------
// Change Brightness
//----------------------------------------------------------------------
// m_pDDSurface의 pRect영역을 어둡게 한다. 
// DarkBits는 (0~5) bit단위로 1, 1/2, 1/4, 1/8, 1/16, 1/32, 0이다.
//----------------------------------------------------------------------
// pRect의 영역은 right, bottom의 좌표는 포함하지 않는다.  -1까지이다.
//----------------------------------------------------------------------
void	
CSpriteSurface::ChangeBrightnessBit(RECT*  pRect, BYTE DarkBits)
{
	// ClippingRect
	if (pRect->left < m_ClipLeft) pRect->left=m_ClipLeft;
	if ((WORD)pRect->right > m_ClipRight) pRect->right=m_ClipRight;

	if (pRect->top < m_ClipTop) pRect->top=m_ClipTop;
	if ((WORD)pRect->bottom > m_ClipBottom) pRect->bottom=m_ClipBottom;

	// 영역을 벗어나는 경우
	if (pRect->left >= pRect->right || pRect->top >= pRect->bottom)
		return;


	QWORD	*lpTargetTemp;
	QWORD	*lpTargetSurface	= (QWORD*)m_ddsd.lpSurface;
	WORD	Pitch			= m_ddsd.lPitch;
	
	// 출력 시작 위치
	lpTargetSurface = (QWORD*)((BYTE*)lpTargetSurface + pRect->top*Pitch + (pRect->left<<1));

	register int i;
	register int j;

	int rectBottom = pRect->bottom;

	int width = pRect->right - pRect->left;
	int qTimes = width >> 2;

	// Target Surface의 정해진 영역을 어둡게 한다.
	/*
	switch ( width & 0x03 )
	{
		//------------------
		// 4점씩
		//------------------
		case 0 :			
			for (i=pRect->top; i<rectBottom; i++)
			{
				lpTargetTemp = lpTargetSurface;	

				// 네점씩 찍기
				for (j=0; j<qTimes; j++)
				{
					*lpTargetTemp++ = (*lpTargetTemp >> DarkBits) & s_qwMASK_SHIFT[DarkBits];					
				}

				// 다음 줄
				lpTargetSurface = (QWORD*)((BYTE*)lpTargetSurface + Pitch);
			}
		break;

		//------------------
		// 1점 + 4점씩
		//------------------
		case 1 :
			for (i=pRect->top; i<rectBottom; i++)
			{
				lpTargetTemp = lpTargetSurface;	

				// 한점 찍기
				*((WORD*)lpTargetTemp) = 
					(*((WORD*)lpTargetTemp) >> DarkBits) & s_wMASK_SHIFT[DarkBits];
				
				lpTargetTemp = (QWORD*)((WORD*)lpTargetTemp + 1);

				// 네점씩 찍기
				for (j=0; j<qTimes; j++)
				{
					*lpTargetTemp++ = (*lpTargetTemp >> DarkBits) & s_qwMASK_SHIFT[DarkBits];
				}

				// 다음 줄
				lpTargetSurface = (QWORD*)((BYTE*)lpTargetSurface + Pitch);
			}
		break;

		//------------------
		// 2점 + 4점씩
		//------------------
		case 2 :
			for (i=pRect->top; i<rectBottom; i++)
			{
				lpTargetTemp = lpTargetSurface;	

				// 두점 찍기
				*((DWORD*)lpTargetTemp) = (*((DWORD*)lpTargetTemp) >> DarkBits) & s_dwMASK_SHIFT[DarkBits];
				lpTargetTemp = (QWORD*)((DWORD*)lpTargetTemp + 1);

				// 네점씩 찍기
				for (j=0; j<qTimes; j++)
				{
					*lpTargetTemp++ = (*lpTargetTemp >> DarkBits) & s_qwMASK_SHIFT[DarkBits];
				}

				// 다음 줄
				lpTargetSurface = (QWORD*)((BYTE*)lpTargetSurface + Pitch);
			}
		break;

		//------------------
		// 1점 + 2점 + 4점씩
		//------------------
		case 3 :
			for (i=pRect->top; i<rectBottom; i++)
			{
				lpTargetTemp = lpTargetSurface;	

				// 한점 찍기
				*((WORD*)lpTargetTemp) = 
					(*((WORD*)lpTargetTemp) >> DarkBits) & s_wMASK_SHIFT[DarkBits];
				lpTargetTemp = (QWORD*)((WORD*)lpTargetTemp + 1);

				// 두점 찍기
				*((DWORD*)lpTargetTemp) = (*((DWORD*)lpTargetTemp) >> DarkBits) & s_dwMASK_SHIFT[DarkBits];
				lpTargetTemp = (QWORD*)((DWORD*)lpTargetTemp + 1);

				// 네점씩 찍기
				for (j=0; j<qTimes; j++)
				{
					*lpTargetTemp++ = (*lpTargetTemp >> DarkBits) & s_qwMASK_SHIFT[DarkBits];
				}

				// 다음 줄
				lpTargetSurface = (QWORD*)((BYTE*)lpTargetSurface + Pitch);
			}
		break;
	}
	*/

///*
	// 흐름을 바꾼다.. 으흠..~	
	int maxWave = 8;		// 2, 4, 8, 16...

	if (maxWave >= width)
	{
		return;
	}

	int maxWave_1 = maxWave-1;
	
	int step = ((DarkBits & maxWave)? maxWave+maxWave_1-DarkBits : DarkBits);
	step = max( 0, min( maxWave_1, step ) );
	int stepInc = 1;	

	int height = pRect->bottom - pRect->top;
	int widthHalf = width>>1;
	int heightHalf = height >> 1;
	int width2 = maxWave;
	int width2Inc = width / heightHalf;
	int heightHalfValue = pRect->top + heightHalf;
	
	QWORD* pSourceTemp;

	for (i=pRect->top; i<rectBottom; i++)
	{			
		// step만큼 오른쪽에서 출력한다. 
		// 4-step만큼은 덜 출력한다.
		int widthPlus = (width - width2)>>1;

		int newWidth = width2 - maxWave + step;
		int qTimes = newWidth >> 2;

		pSourceTemp = (QWORD*)((WORD*)lpTargetSurface + widthPlus + step);
		lpTargetTemp = (QWORD*)((WORD*)lpTargetSurface + widthPlus);
		
		switch (newWidth & 0x03)
		{
			//------------------
			// 4점씩
			//------------------
			case 0 :
				for (j=0; j<qTimes; j++)
				{
					*lpTargetTemp++ = *pSourceTemp++;
				}
			break;			

			//------------------
			// 1점 + 4점씩
			//------------------
			case 1 :
				// 한점 찍기
				*((WORD*)lpTargetTemp) = *((WORD*)pSourceTemp);
				
				lpTargetTemp = (QWORD*)((WORD*)lpTargetTemp + 1);
				pSourceTemp = (QWORD*)((WORD*)pSourceTemp + 1);

				// 네점씩 찍기
				for (j=0; j<qTimes; j++)
				{
					*lpTargetTemp++ = *pSourceTemp++;
				}
			break;

			//------------------
			// 2점 + 4점씩
			//------------------
			case 2 :
				// 두점 찍기
				*((DWORD*)lpTargetTemp) = *((DWORD*)pSourceTemp);
				lpTargetTemp = (QWORD*)((DWORD*)lpTargetTemp + 1);
				pSourceTemp = (QWORD*)((DWORD*)pSourceTemp + 1);

				// 네점씩 찍기
				for (j=0; j<qTimes; j++)
				{
					*lpTargetTemp++ = *pSourceTemp++;
				}
			break;

			//------------------
			// 1점 + 2점 + 4점씩
			//------------------
			case 3 :			
				// 한점 찍기
				*((WORD*)lpTargetTemp) = *((WORD*)pSourceTemp);
				lpTargetTemp = (QWORD*)((WORD*)lpTargetTemp + 1);
				pSourceTemp = (QWORD*)((WORD*)pSourceTemp + 1);

				// 두점 찍기
				*((DWORD*)lpTargetTemp) = *((DWORD*)pSourceTemp);
				lpTargetTemp = (QWORD*)((DWORD*)lpTargetTemp + 1);
				pSourceTemp = (QWORD*)((DWORD*)pSourceTemp + 1);

				// 네점씩 찍기
				for (j=0; j<qTimes; j++)
				{
					*lpTargetTemp++ = *pSourceTemp++;
				}
			break;
		}

		// 다음 줄
		lpTargetSurface = (QWORD*)((BYTE*)lpTargetSurface + Pitch);

		step += stepInc;
		if (step < 0 || step >= maxWave)
		{
			if (step < 0)
			{
				step = 0;
				stepInc = 1;
			}
			else if (step >= maxWave)
			{
				step = maxWave_1;
				stepInc = -1;
			}
		}

		width2 += width2Inc;

		width2 = min( width, width2 );
		width2 = max( maxWave, width2 );

		if (i==heightHalfValue)
		{
			width2Inc = -width2Inc;
		}
	}
//	*/
}

//----------------------------------------------------------------------
// Blt Color Alpha
//----------------------------------------------------------------------
// 단색으로(color) Surface의 pRect영역을
// alpha값만큼 alpha blending한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltColorAlpha(RECT* pRect, WORD color, BYTE alpha2)
{
	// ClippingRect
	if (pRect->left < m_ClipLeft) pRect->left=m_ClipLeft;
	if ((WORD)pRect->right > m_ClipRight) pRect->right=m_ClipRight;

	if (pRect->top < m_ClipTop) pRect->top=m_ClipTop;
	if ((WORD)pRect->bottom > m_ClipBottom) pRect->bottom=m_ClipBottom;

	// 영역을 벗어나는 경우
	if (pRect->left >= pRect->right || pRect->top >= pRect->bottom)
		return;

	QWORD	*lpTargetTemp;
	QWORD	*lpTargetSurface	= (QWORD*)m_ddsd.lpSurface;
	WORD	Pitch			= m_ddsd.lPitch;
	
	// 출력 시작 위치
	lpTargetSurface = (QWORD*)((BYTE*)lpTargetSurface + pRect->top*Pitch + (pRect->left<<1));

	register int i;
	//register int j;

	DWORD	srcColor2, destColor2;
	DWORD	first2, second2;

	QWORD	srcColor, destColor;
	QWORD	first, second;

	int rectBottom = pRect->bottom;

	int width = pRect->right - pRect->left;
	int qTimes = width >> 2;	//  / 4
	
	DWORD dwColor = ((DWORD)color << 16) | color;
	QWORD qwColor = ((QWORD)dwColor << 32) | dwColor;

	// 31 - alpha2
	BYTE alpha = 31 - alpha2;
		
	//-----------------------------------------------------
	// Surface의 정해진 영역을 alpha blending				
	//-----------------------------------------------------
	switch ( width & 0x03 )
	{
		//-----------------------------------------------------
		// 정확히 4의 배수가 되는 경우..
		//-----------------------------------------------------
		case 0 :
			for (i=pRect->top; i<rectBottom; i++)
			{
				lpTargetTemp = lpTargetSurface;	

				int qTimesTemp = qTimes;

				//-----------------------------------
				// 4점씩...
				//-----------------------------------
				while (qTimesTemp--)
				{
					destColor = *lpTargetTemp;
				
					first   =   qwColor & CDirectDraw::s_qwMASK_ALPHA0;
					first   >>= 5;
					first   *=  alpha;
					first   &=  CDirectDraw::s_qwMASK_ALPHA0;
					
					second  =   qwColor & CDirectDraw::s_qwMASK_ALPHA1;
					second  *=  alpha;
					second  >>= 5;
					second  &=  CDirectDraw::s_qwMASK_ALPHA1;
					
					srcColor =  first | second;

					first   =   destColor & CDirectDraw::s_qwMASK_ALPHA0;
					first   >>= 5;
					first   *=  alpha2;
					first   &=  CDirectDraw::s_qwMASK_ALPHA0;

					second  =   destColor & CDirectDraw::s_qwMASK_ALPHA1;
					second  *=  alpha2;
					second  >>= 5;
					second  &=  CDirectDraw::s_qwMASK_ALPHA1;

					destColor=  first | second;
					
					*lpTargetTemp = srcColor + destColor;

					lpTargetTemp++;					
				}

				// 다음 줄
				lpTargetSurface = (QWORD*)((BYTE*)lpTargetSurface + Pitch);
			}
		break;

		//-----------------------------------------------------
		// 1점 찍고  4점씩...
		//-----------------------------------------------------
		case 1 :
			for (i=pRect->top; i<rectBottom; i++)
			{
				lpTargetTemp = lpTargetSurface;	

				//-----------------------------------
				// 한 점 찍기...
				//-----------------------------------
				destColor2 = *(WORD*)lpTargetTemp;

				first2   =   color & CDirectDraw::s_wMASK_ALPHA1;
				first2   >>= 5;
				first2   *=  alpha;
				first2   &=  CDirectDraw::s_wMASK_ALPHA1;

				second2  =   color & CDirectDraw::s_wMASK_ALPHA0;
				second2  *=  alpha;
				second2  >>= 5;
				second2  &=  CDirectDraw::s_wMASK_ALPHA0;

				srcColor2 =  first2 | second2;

				first2   =   destColor2 & CDirectDraw::s_wMASK_ALPHA1;
				first2   >>= 5;
				first2   *=  alpha2;
				first2   &=  CDirectDraw::s_wMASK_ALPHA1;

				second2  =   destColor2 & CDirectDraw::s_wMASK_ALPHA0;
				second2  *=  alpha2;
				second2  >>= 5;
				second2  &=  CDirectDraw::s_wMASK_ALPHA0;

				destColor2=  first2 | second2;

				*(WORD*)lpTargetTemp = (WORD)(srcColor2 + destColor2);		
					
				lpTargetTemp = (QWORD*)((WORD*)lpTargetTemp + 1);	


				int qTimesTemp = qTimes;
				
				//-----------------------------------
				// 4점씩..
				//-----------------------------------
				while (qTimesTemp--)
				{
					destColor = *lpTargetTemp;
				
					first   =   qwColor & CDirectDraw::s_qwMASK_ALPHA0;
					first   >>= 5;
					first   *=  alpha;
					first   &=  CDirectDraw::s_qwMASK_ALPHA0;
					
					second  =   qwColor & CDirectDraw::s_qwMASK_ALPHA1;
					second  *=  alpha;
					second  >>= 5;
					second  &=  CDirectDraw::s_qwMASK_ALPHA1;
					
					srcColor =  first | second;

					first   =   destColor & CDirectDraw::s_qwMASK_ALPHA0;
					first   >>= 5;
					first   *=  alpha2;
					first   &=  CDirectDraw::s_qwMASK_ALPHA0;

					second  =   destColor & CDirectDraw::s_qwMASK_ALPHA1;
					second  *=  alpha2;
					second  >>= 5;
					second  &=  CDirectDraw::s_qwMASK_ALPHA1;

					destColor=  first | second;
					
					*lpTargetTemp = srcColor + destColor;

					lpTargetTemp++;					
				}

				// 다음 줄
				lpTargetSurface = (QWORD*)((BYTE*)lpTargetSurface + Pitch);
			}
		break;

		//-----------------------------------------------------
		// 2점찍고.. 4점씩...
		//-----------------------------------------------------
		case 2 :
			for (i=pRect->top; i<rectBottom; i++)
			{
				lpTargetTemp = lpTargetSurface;	
			
				//-----------------------------------
				// 두 점 찍기..
				//-----------------------------------
				destColor2 = *(DWORD*)lpTargetTemp;

				first2   =   dwColor & CDirectDraw::s_dwMASK_ALPHA0;
				first2   >>= 5;
				first2   *=  alpha;
				first2   &=  CDirectDraw::s_dwMASK_ALPHA0;

				second2  =   dwColor & CDirectDraw::s_dwMASK_ALPHA1;
				second2  *=  alpha;
				second2  >>= 5;
				second2  &=  CDirectDraw::s_dwMASK_ALPHA1;

				srcColor2 =  first2 | second2;

				first2   =   destColor2 & CDirectDraw::s_dwMASK_ALPHA0;
				first2   >>= 5;
				first2   *=  alpha2;
				first2   &=  CDirectDraw::s_dwMASK_ALPHA0;

				second2  =   destColor2 & CDirectDraw::s_dwMASK_ALPHA1;
				second2  *=  alpha2;
				second2  >>= 5;
				second2  &=  CDirectDraw::s_dwMASK_ALPHA1;

				destColor2=  first2 | second2;

				*(DWORD*)lpTargetTemp = srcColor2 + destColor2;
					
				lpTargetTemp = (QWORD*)((DWORD*)lpTargetTemp + 1);


				int qTimesTemp = qTimes;
				
				//-----------------------------------
				// 4점씩..
				//-----------------------------------
				while (qTimesTemp--)
				{
					destColor = *lpTargetTemp;
				
					first   =   qwColor & CDirectDraw::s_qwMASK_ALPHA0;
					first   >>= 5;
					first   *=  alpha;
					first   &=  CDirectDraw::s_qwMASK_ALPHA0;
					
					second  =   qwColor & CDirectDraw::s_qwMASK_ALPHA1;
					second  *=  alpha;
					second  >>= 5;
					second  &=  CDirectDraw::s_qwMASK_ALPHA1;
					
					srcColor =  first | second;

					first   =   destColor & CDirectDraw::s_qwMASK_ALPHA0;
					first   >>= 5;
					first   *=  alpha2;
					first   &=  CDirectDraw::s_qwMASK_ALPHA0;

					second  =   destColor & CDirectDraw::s_qwMASK_ALPHA1;
					second  *=  alpha2;
					second  >>= 5;
					second  &=  CDirectDraw::s_qwMASK_ALPHA1;

					destColor=  first | second;
					
					*lpTargetTemp = srcColor + destColor;

					lpTargetTemp++;					
				}

				// 다음 줄
				lpTargetSurface = (QWORD*)((BYTE*)lpTargetSurface + Pitch);
			}
		break;

		//-----------------------------------------------------
		// 1점 찍고 2점찍고.. 4점씩...
		//-----------------------------------------------------
		case 3 :
			for (i=pRect->top; i<rectBottom; i++)
			{
				lpTargetTemp = lpTargetSurface;	

				//-----------------------------------
				// 한 점 찍기...
				//-----------------------------------
				destColor2 = *(WORD*)lpTargetTemp;

				first2   =   color & CDirectDraw::s_wMASK_ALPHA1;
				first2   >>= 5;
				first2   *=  alpha;
				first2   &=  CDirectDraw::s_wMASK_ALPHA1;

				second2  =   color & CDirectDraw::s_wMASK_ALPHA0;
				second2  *=  alpha;
				second2  >>= 5;
				second2  &=  CDirectDraw::s_wMASK_ALPHA0;

				srcColor2 =  first2 | second2;

				first2   =   destColor2 & CDirectDraw::s_wMASK_ALPHA1;
				first2   >>= 5;
				first2   *=  alpha2;
				first2   &=  CDirectDraw::s_wMASK_ALPHA1;

				second2  =   destColor2 & CDirectDraw::s_wMASK_ALPHA0;
				second2  *=  alpha2;
				second2  >>= 5;
				second2  &=  CDirectDraw::s_wMASK_ALPHA0;

				destColor2=  first2 | second2;

				*(WORD*)lpTargetTemp = (WORD)(srcColor2 + destColor2);		
					
				lpTargetTemp = (QWORD*)((WORD*)lpTargetTemp + 1);	

				//-----------------------------------
				// 두 점 찍기..
				//-----------------------------------
				destColor2 = *(DWORD*)lpTargetTemp;

				first2   =   dwColor & CDirectDraw::s_dwMASK_ALPHA0;
				first2   >>= 5;
				first2   *=  alpha;
				first2   &=  CDirectDraw::s_dwMASK_ALPHA0;

				second2  =   dwColor & CDirectDraw::s_dwMASK_ALPHA1;
				second2  *=  alpha;
				second2  >>= 5;
				second2  &=  CDirectDraw::s_dwMASK_ALPHA1;

				srcColor2 =  first2 | second2;

				first2   =   destColor2 & CDirectDraw::s_dwMASK_ALPHA0;
				first2   >>= 5;
				first2   *=  alpha2;
				first2   &=  CDirectDraw::s_dwMASK_ALPHA0;

				second2  =   destColor2 & CDirectDraw::s_dwMASK_ALPHA1;
				second2  *=  alpha2;
				second2  >>= 5;
				second2  &=  CDirectDraw::s_dwMASK_ALPHA1;

				destColor2=  first2 | second2;

				*(DWORD*)lpTargetTemp = srcColor2 + destColor2;
					
				lpTargetTemp = (QWORD*)((DWORD*)lpTargetTemp + 1);


				int qTimesTemp = qTimes;
				
				//-----------------------------------
				// 4점씩..
				//-----------------------------------
				while (qTimesTemp--)
				{
					destColor = *lpTargetTemp;
				
					first   =   qwColor & CDirectDraw::s_qwMASK_ALPHA0;
					first   >>= 5;
					first   *=  alpha;
					first   &=  CDirectDraw::s_qwMASK_ALPHA0;
					
					second  =   qwColor & CDirectDraw::s_qwMASK_ALPHA1;
					second  *=  alpha;
					second  >>= 5;
					second  &=  CDirectDraw::s_qwMASK_ALPHA1;
					
					srcColor =  first | second;

					first   =   destColor & CDirectDraw::s_qwMASK_ALPHA0;
					first   >>= 5;
					first   *=  alpha2;
					first   &=  CDirectDraw::s_qwMASK_ALPHA0;

					second  =   destColor & CDirectDraw::s_qwMASK_ALPHA1;
					second  *=  alpha2;
					second  >>= 5;
					second  &=  CDirectDraw::s_qwMASK_ALPHA1;

					destColor=  first | second;
					
					*lpTargetTemp = srcColor + destColor;

					lpTargetTemp++;					
				}

				// 다음 줄
				lpTargetSurface = (QWORD*)((BYTE*)lpTargetSurface + Pitch);
			}
		break;
	}
}

//----------------------------------------------------------------------
// Blt Sprite
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSprite(POINT* pPoint, CSprite* pSprite)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = {pPoint->x, pPoint->y};

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;			
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}	

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltClipHeight(lpSurface, Pitch, &rect);
				}
				else
				{
					// No Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->Blt(lpSurface, Pitch);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt Sprite NoClip
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteNoClip(POINT* pPoint, CSprite* pSprite)
{
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;

	// Lock
	//LockW(lpSurface, Pitch);

	// 출력 시작 위치			
	lpSurface = (WORD*)((BYTE*)lpSurface + pPoint->y*Pitch + (pPoint->x<<1));

	// 출력
	pSprite->Blt(lpSurface, Pitch);	

	// Unlock
	//Unlock();
}

//----------------------------------------------------------------------
// Blt SpriteHalf
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteHalf(POINT* pPoint, CSprite* pSprite)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	
	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltHalfClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltHalfClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltHalfClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltHalfClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltHalfClipHeight(lpSurface, Pitch, &rect);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltHalf(lpSurface, Pitch);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt Sprite Alpha
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteAlpha(POINT* pPoint, CSprite* pSprite, BYTE alpha)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	
	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltAlphaClipWidth(lpSurface, Pitch, &rect, alpha);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltAlphaClipLeft(lpSurface, Pitch, &rect, alpha);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltAlphaClipRight(lpSurface, Pitch, &rect, alpha);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltAlphaClipHeight(lpSurface, Pitch, &rect, alpha);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltAlphaClipHeight(lpSurface, Pitch, &rect, alpha);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltAlpha(lpSurface, Pitch, alpha);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}


//----------------------------------------------------------------------
// Blt Sprite Color
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteColor(POINT* pPoint, CSprite* pSprite, BYTE rgb)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltColorClipWidth(lpSurface, Pitch, &rect, rgb);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltColorClipLeft(lpSurface, Pitch, &rect, rgb);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltColorClipRight(lpSurface, Pitch, &rect, rgb);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltColorClipHeight(lpSurface, Pitch, &rect, rgb);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltColorClipHeight(lpSurface, Pitch, &rect, rgb);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltColor(lpSurface, Pitch, rgb);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}


//----------------------------------------------------------------------
// Blt Sprite ColorSet
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteColorSet(POINT* pPoint, CSprite* pSprite, WORD colorSet)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltColorSetClipWidth(lpSurface, Pitch, &rect, colorSet);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltColorSetClipLeft(lpSurface, Pitch, &rect, colorSet);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltColorSetClipRight(lpSurface, Pitch, &rect, colorSet);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltColorSetClipHeight(lpSurface, Pitch, &rect, colorSet);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltColorSetClipHeight(lpSurface, Pitch, &rect, colorSet);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltColorSet(lpSurface, Pitch, colorSet);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt Sprite Darkness
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteDarkness(POINT* pPoint, CSprite* pSprite, BYTE DarkBits)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltDarknessClipWidth(lpSurface, Pitch, &rect, DarkBits);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltDarknessClipLeft(lpSurface, Pitch, &rect, DarkBits);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltDarknessClipRight(lpSurface, Pitch, &rect, DarkBits);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltDarknessClipHeight(lpSurface, Pitch, &rect, DarkBits);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltDarknessClipHeight(lpSurface, Pitch, &rect, DarkBits);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltDarkness(lpSurface, Pitch, DarkBits);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt Sprite Effect
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteEffect(POINT* pPoint, CSprite* pSprite)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltEffectClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltEffectClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltEffectClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
				pSprite->BltEffectClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
					pSprite->BltEffectClipHeight(lpSurface, Pitch, &rect);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
					pSprite->BltEffect(lpSurface, Pitch);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt Sprite Effect
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpritePalEffect(POINT* pPoint, CSpritePal* pSprite, MPalette &pal)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;


	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	
	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltEffectClipWidth(lpSurface, Pitch, &rect, pal);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltEffectClipLeft(lpSurface, Pitch, &rect, pal);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltEffectClipRight(lpSurface, Pitch, &rect, pal);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
				pSprite->BltEffectClipHeight(lpSurface, Pitch, &rect, pal);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
					pSprite->BltEffectClipHeight(lpSurface, Pitch, &rect, pal);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
					pSprite->BltEffect(lpSurface, Pitch, pal);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt Sprite Scale 
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteScale(POINT* pPoint, CSprite* pSprite, BYTE scale)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	
	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->BltScaleClipWidth(lpSurface, Pitch, &rect, scale);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->BltScaleClipLeft(lpSurface, Pitch, &rect, scale);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
			pSprite->BltScaleClipRight(lpSurface, Pitch, &rect, scale);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
				pSprite->BltScaleClipHeight(lpSurface, Pitch, &rect, scale);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
					pSprite->BltScaleClipHeight(lpSurface, Pitch, &rect, scale);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
					pSprite->BltScale(lpSurface, Pitch, scale);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}



//----------------------------------------------------------------------
// Blt AlphaSprite
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltAlphaSprite(POINT* pPoint, CAlphaSprite* pSprite)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->BltClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->BltClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
			pSprite->BltClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
				pSprite->BltClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
					pSprite->BltClipHeight(lpSurface, Pitch, &rect);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
					pSprite->Blt(lpSurface, Pitch);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt AlphaSprite
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltAlphaSpritePal(POINT* pPoint, CAlphaSpritePal* pSprite, MPalette &pal)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;
	

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->BltClipWidth(lpSurface, Pitch, &rect, pal);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->BltClipLeft(lpSurface, Pitch, &rect, pal);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
			pSprite->BltClipRight(lpSurface, Pitch, &rect, pal);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
				pSprite->BltClipHeight(lpSurface, Pitch, &rect, pal);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
					pSprite->BltClipHeight(lpSurface, Pitch, &rect, pal);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
					pSprite->Blt(lpSurface, Pitch, pal);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}


//----------------------------------------------------------------------
// Blt AlphaSprite Alpha
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltAlphaSpriteAlpha(POINT* pPoint, CAlphaSprite* pSprite, BYTE alpha)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->BltAlphaClipWidth(lpSurface, Pitch, &rect, alpha);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->BltAlphaClipLeft(lpSurface, Pitch, &rect, alpha);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
			pSprite->BltAlphaClipRight(lpSurface, Pitch, &rect, alpha);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
				pSprite->BltAlphaClipHeight(lpSurface, Pitch, &rect, alpha);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
					pSprite->BltAlphaClipHeight(lpSurface, Pitch, &rect, alpha);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
					pSprite->BltAlpha(lpSurface, Pitch, alpha);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt AlphaSprite Alpha
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltAlphaSpritePalAlpha(POINT* pPoint, CAlphaSpritePal* pSprite, BYTE alpha, MPalette &pal)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;


	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	


	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->BltAlphaClipWidth(lpSurface, Pitch, &rect, alpha, pal);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->BltAlphaClipLeft(lpSurface, Pitch, &rect, alpha, pal);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
			pSprite->BltAlphaClipRight(lpSurface, Pitch, &rect, alpha, pal);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
				pSprite->BltAlphaClipHeight(lpSurface, Pitch, &rect, alpha, pal);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
					pSprite->BltAlphaClipHeight(lpSurface, Pitch, &rect, alpha, pal);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
					pSprite->BltAlpha(lpSurface, Pitch, alpha, pal);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}


//----------------------------------------------------------------------
// Blt AlphaSprite4444
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltAlphaSprite4444(POINT* pPoint, CAlphaSprite* pSprite)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->Blt4444ClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->Blt4444ClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->Blt4444ClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->Blt4444ClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->Blt4444ClipHeight(lpSurface, Pitch, &rect);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->Blt4444(lpSurface, Pitch);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt AlphaSprite4444
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltAlphaSpritePal4444(POINT* pPoint, CAlphaSpritePal* pSprite, MPalette &pal)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;


	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->Blt4444ClipWidth(lpSurface, Pitch, &rect, pal);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->Blt4444ClipLeft(lpSurface, Pitch, &rect, pal);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->Blt4444ClipRight(lpSurface, Pitch, &rect, pal);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->Blt4444ClipHeight(lpSurface, Pitch, &rect, pal);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->Blt4444ClipHeight(lpSurface, Pitch, &rect, pal);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->Blt4444(lpSurface, Pitch, pal);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}


//----------------------------------------------------------------------
// Blt AlphaSprite4444 NotTrans
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다. 투명색 부분은 검게 칠한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltAlphaSprite4444NotTrans(POINT* pPoint, CAlphaSprite* pSprite)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->Blt4444NotTransClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->Blt4444NotTransClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->Blt4444NotTransClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->Blt4444NotTransClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->Blt4444NotTransClipHeight(lpSurface, Pitch, &rect);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->Blt4444NotTrans(lpSurface, Pitch);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt AlphaSprite4444 NotTrans
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다. 투명색 부분은 검게 칠한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltAlphaSpritePal4444NotTrans(POINT* pPoint, CAlphaSpritePal* pSprite, MPalette &pal)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->Blt4444NotTransClipWidth(lpSurface, Pitch, &rect, pal);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->Blt4444NotTransClipLeft(lpSurface, Pitch, &rect, pal);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->Blt4444NotTransClipRight(lpSurface, Pitch, &rect, pal);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->Blt4444NotTransClipHeight(lpSurface, Pitch, &rect, pal);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->Blt4444NotTransClipHeight(lpSurface, Pitch, &rect, pal);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->Blt4444NotTrans(lpSurface, Pitch, pal);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}


//----------------------------------------------------------------------
// Blt AlphaSprite4444 NotTrans
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다. 투명색 부분은 검게 칠한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltAlphaSprite4444SmallNotTrans(POINT* pPoint, CAlphaSprite* pSprite, BYTE shift)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth() >> shift;
	rect.bottom = pSprite->GetHeight() >> shift;


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + (pSprite->GetWidth() >> shift);
	int yPlusSpriteHeight	= pt.y + (pSprite->GetHeight() >> shift);

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -(pt.x << shift);		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444SmallClipWidth(lpSurface, Pitch, &rect, shift);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444SmallClipLeft(lpSurface, Pitch, &rect, shift);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			//pSprite->Blt4444SmallClipRight(lpSurface, Pitch, &rect, shift);			
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -(pt.y << shift);

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//pSprite->Blt4444SmallClipHeight(lpSurface, Pitch, &rect, shift);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					//pSprite->Blt4444SmallClipHeight(lpSurface, Pitch, &rect, shift);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->Blt4444SmallNotTrans(lpSurface, Pitch, shift);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt AlphaSprite4444 NotTrans
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다. 투명색 부분은 검게 칠한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltAlphaSpritePal4444SmallNotTrans(POINT* pPoint, CAlphaSpritePal* pSprite, BYTE shift, MPalette &pal)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;


	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth() >> shift;
	rect.bottom = pSprite->GetHeight() >> shift;


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + (pSprite->GetWidth() >> shift);
	int yPlusSpriteHeight	= pt.y + (pSprite->GetHeight() >> shift);

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -(pt.x << shift);		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444SmallClipWidth(lpSurface, Pitch, &rect, shift);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444SmallClipLeft(lpSurface, Pitch, &rect, shift);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			//pSprite->Blt4444SmallClipRight(lpSurface, Pitch, &rect, shift);			
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -(pt.y << shift);

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//pSprite->Blt4444SmallClipHeight(lpSurface, Pitch, &rect, shift);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					//pSprite->Blt4444SmallClipHeight(lpSurface, Pitch, &rect, shift);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->Blt4444SmallNotTrans(lpSurface, Pitch, shift, pal);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}


//----------------------------------------------------------------------
// Blt IndexSprite
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltIndexSprite(POINT* pPoint, CIndexSprite* pSprite)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltClipHeight(lpSurface, Pitch, &rect);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->Blt(lpSurface, Pitch);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt IndexSprite Darkness
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltIndexSpriteDarkness(POINT* pPoint, CIndexSprite* pSprite, BYTE DarkBits)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltDarknessClipWidth(lpSurface, Pitch, &rect, DarkBits);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltDarknessClipLeft(lpSurface, Pitch, &rect, DarkBits);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltDarknessClipRight(lpSurface, Pitch, &rect, DarkBits);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltDarknessClipHeight(lpSurface, Pitch, &rect, DarkBits);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltDarknessClipHeight(lpSurface, Pitch, &rect, DarkBits);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltDarkness(lpSurface, Pitch, DarkBits);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt IndexSprite Alpha
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltIndexSpriteAlpha(POINT* pPoint, CIndexSprite* pSprite, BYTE alpha)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltAlphaClipWidth(lpSurface, Pitch, &rect, alpha);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltAlphaClipLeft(lpSurface, Pitch, &rect, alpha);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltAlphaClipRight(lpSurface, Pitch, &rect, alpha);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltAlphaClipHeight(lpSurface, Pitch, &rect, alpha);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltAlphaClipHeight(lpSurface, Pitch, &rect, alpha);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltAlpha(lpSurface, Pitch, alpha);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}



//----------------------------------------------------------------------
// Blt IndexSprite Color
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltIndexSpriteColor(POINT* pPoint, CIndexSprite* pSprite, BYTE rgb)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltColorClipWidth(lpSurface, Pitch, &rect, rgb);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltColorClipLeft(lpSurface, Pitch, &rect, rgb);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltColorClipRight(lpSurface, Pitch, &rect, rgb);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltColorClipHeight(lpSurface, Pitch, &rect, rgb);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltColorClipHeight(lpSurface, Pitch, &rect, rgb);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltColor(lpSurface, Pitch, rgb);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt IndexSprite ColorSet
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltIndexSpriteColorSet(POINT* pPoint, CIndexSprite* pSprite, WORD colorSet)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltColorSetClipWidth(lpSurface, Pitch, &rect, colorSet);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltColorSetClipLeft(lpSurface, Pitch, &rect, colorSet);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltColorSetClipRight(lpSurface, Pitch, &rect, colorSet);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltColorSetClipHeight(lpSurface, Pitch, &rect, colorSet);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltColorSetClipHeight(lpSurface, Pitch, &rect, colorSet);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltColorSet(lpSurface, Pitch, colorSet);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}


//----------------------------------------------------------------------
// Blt IndexSpriteEffect
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltIndexSpriteEffect(POINT* pPoint, CIndexSprite* pSprite)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltEffectClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltEffectClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltEffectClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltEffectClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltEffectClipHeight(lpSurface, Pitch, &rect);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltEffect(lpSurface, Pitch);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt SpriteAlpha Filter
//----------------------------------------------------------------------
// 아직 Clipping이 지원되지 않음
//----------------------------------------------------------------------
/*
void	
CSpriteSurface::BltSpriteAlphaFilter(POINT* pPoint, CSprite* pSprite, CFilter* pFilter)
{
	WORD*	lpSurface;
	WORD	Pitch;

	// Lock
	LockW(lpSurface, Pitch);

	// 출력 시작 위치			
	//lpSurface = (WORD*)((BYTE*)lpSurface + pPoint->y*Pitch + (pPoint->x<<1));
	lpSurface = (WORD*)((BYTE*)lpSurface + pPoint->y*Pitch + (pPoint->x<<1));
	pSprite->BltAlphaFilter(lpSurface, Pitch, pFilter);

	Unlock();
}
*/


//----------------------------------------------------------------------
// Blt SpriteAlphaFilter
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
// Filter는 CSprite::SetFilter(...)를 통해서 설정된다.
//
// Filter가 Sprite와 겹치지 않는 경우...
// BltSprite를 호출하도록 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteAlphaFilter(POINT* pPoint, CSprite* pSprite)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit() || CSprite::GetFilter()->IsNotInit())
		return;

	//-------------------------------------------------------------
	// Filter가 Sprite와 겹치지 않는 경우...
	// BltSprite를 호출하도록 한다.
	//-------------------------------------------------------------
	if (!pSprite->IsIntersectFilter())
	{
		BltSprite( pPoint, pSprite );
		return;
	}

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltAlphaFilterClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltAlphaFilterClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltAlphaFilterClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltAlphaFilterClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltAlphaFilterClipHeight(lpSurface, Pitch, &rect);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltAlphaFilter(lpSurface, Pitch);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}


//----------------------------------------------------------------------
// Blt SpriteAlphaFilter Darkness
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
// Filter는 CSprite::SetFilter(...)를 통해서 설정된다.
//
// Filter가 Sprite와 겹치지 않는 경우...
// BltSpriteDarkness를 호출하도록 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteAlphaFilterDarkness(POINT* pPoint, CSprite* pSprite, BYTE DarkBits)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit() || CSprite::GetFilter()->IsNotInit())
		return;

	//-------------------------------------------------------------
	// Filter가 Sprite와 겹치지 않는 경우...
	// BltSprite를 호출하도록 한다.
	//-------------------------------------------------------------
	if (!pSprite->IsIntersectFilter())
	{
		BltSpriteDarkness( pPoint, pSprite, DarkBits );
		return;
	}

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltAlphaFilterDarknessClipWidth(lpSurface, Pitch, &rect, DarkBits);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltAlphaFilterDarknessClipLeft(lpSurface, Pitch, &rect, DarkBits);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltAlphaFilterDarknessClipRight(lpSurface, Pitch, &rect, DarkBits);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltAlphaFilterDarknessClipHeight(lpSurface, Pitch, &rect, DarkBits);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltAlphaFilterDarknessClipHeight(lpSurface, Pitch, &rect, DarkBits);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltAlphaFilterDarkness(lpSurface, Pitch, DarkBits);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt Sprite Alpha4444 NotTrans
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다. 투명색 부분은 검게 칠한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteAlpha4444NotTrans(POINT* pPoint, CSprite* pSprite, BYTE alpha)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444NotTransClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444NotTransClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			//pSprite->Blt4444NotTransClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//pSprite->Blt4444NotTransClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					//pSprite->Blt4444NotTransClipHeight(lpSurface, Pitch, &rect);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltAlpha4444NotTrans(lpSurface, Pitch, alpha);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt AlphaSprite4444 NotTrans
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다. 투명색 부분은 검게 칠한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteAlpha4444SmallNotTrans(POINT* pPoint, CSprite* pSprite, BYTE alpha, BYTE shift)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth() >> shift;
	rect.bottom = pSprite->GetHeight() >> shift;


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + (pSprite->GetWidth() >> shift);
	int yPlusSpriteHeight	= pt.y + (pSprite->GetHeight() >> shift);

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -(pt.x << shift);		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444SmallClipWidth(lpSurface, Pitch, &rect, shift);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444SmallClipLeft(lpSurface, Pitch, &rect, shift);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			//pSprite->Blt4444SmallClipRight(lpSurface, Pitch, &rect, shift);			
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -(pt.y << shift);

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//pSprite->Blt4444SmallClipHeight(lpSurface, Pitch, &rect, shift);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					//pSprite->Blt4444SmallClipHeight(lpSurface, Pitch, &rect, shift);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltAlpha4444SmallNotTrans(lpSurface, Pitch, alpha, shift);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt Sprite 1555 NotTrans
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다. 투명색 부분은 검게 칠한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSprite1555NotTrans(POINT* pPoint, CSprite* pSprite)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444NotTransClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444NotTransClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			//pSprite->Blt4444NotTransClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//pSprite->Blt4444NotTransClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					//pSprite->Blt4444NotTransClipHeight(lpSurface, Pitch, &rect);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->Blt1555NotTrans(lpSurface, Pitch);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}


//----------------------------------------------------------------------
// Blt Sprite 1555 NotTrans
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다. 투명색 부분은 검게 칠한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpritePal1555NotTrans(POINT* pPoint, CSpritePal* pSprite, MPalette &pal)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;


	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444NotTransClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444NotTransClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			//pSprite->Blt4444NotTransClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//pSprite->Blt4444NotTransClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					//pSprite->Blt4444NotTransClipHeight(lpSurface, Pitch, &rect);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->Blt1555NotTrans(lpSurface, Pitch, pal);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt AlphaSprite4444 NotTrans
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다. 투명색 부분은 검게 칠한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSprite1555SmallNotTrans(POINT* pPoint, CSprite* pSprite, BYTE shift)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth() >> shift;
	rect.bottom = pSprite->GetHeight() >> shift;


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + (pSprite->GetWidth() >> shift);
	int yPlusSpriteHeight	= pt.y + (pSprite->GetHeight() >> shift);

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -(pt.x << shift);		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444SmallClipWidth(lpSurface, Pitch, &rect, shift);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444SmallClipLeft(lpSurface, Pitch, &rect, shift);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			//pSprite->Blt4444SmallClipRight(lpSurface, Pitch, &rect, shift);			
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -(pt.y << shift);

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//pSprite->Blt4444SmallClipHeight(lpSurface, Pitch, &rect, shift);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					//pSprite->Blt4444SmallClipHeight(lpSurface, Pitch, &rect, shift);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->Blt1555SmallNotTrans(lpSurface, Pitch, shift);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}


//----------------------------------------------------------------------
// Blt AlphaSprite4444 NotTrans
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다. 투명색 부분은 검게 칠한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpritePal1555SmallNotTrans(POINT* pPoint, CSpritePal* pSprite, BYTE shift, MPalette &pal)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;


	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth() >> shift;
	rect.bottom = pSprite->GetHeight() >> shift;


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + (pSprite->GetWidth() >> shift);
	int yPlusSpriteHeight	= pt.y + (pSprite->GetHeight() >> shift);

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -(pt.x << shift);		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444SmallClipWidth(lpSurface, Pitch, &rect, shift);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444SmallClipLeft(lpSurface, Pitch, &rect, shift);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			//pSprite->Blt4444SmallClipRight(lpSurface, Pitch, &rect, shift);			
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -(pt.y << shift);

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//pSprite->Blt4444SmallClipHeight(lpSurface, Pitch, &rect, shift);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					//pSprite->Blt4444SmallClipHeight(lpSurface, Pitch, &rect, shift);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->Blt1555SmallNotTrans(lpSurface, Pitch, shift, pal);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}


//----------------------------------------------------------------------
// Blt SpriteDarkerFilter
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
// Filter는 CSprite::SetFilter(...)를 통해서 설정된다.
//
// Filter가 Sprite와 겹치지 않는 경우...
// BltSprite를 호출하도록 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteDarkerFilter(POINT* pPoint, CSprite* pSprite)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit() || CSprite::GetFilter()->IsNotInit())
		return;

	//-------------------------------------------------------------
	// Filter가 Sprite와 겹치지 않는 경우...
	// BltSprite를 호출하도록 한다.
	//-------------------------------------------------------------
	// Darker는 Tile에서만 사용할 예정이므로..
	// 일단은... 
	/*
	if (!pSprite->IsIntersectFilter())
	{
		BltSprite( pPoint, pSprite );
		return;
	}
	*/

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltDarkerFilterClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltDarkerFilterClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltDarkerFilterClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltDarkerFilterClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltDarkerFilterClipHeight(lpSurface, Pitch, &rect);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltDarkerFilter(lpSurface, Pitch);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}


//----------------------------------------------------------------------
// Blt Sprite
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltShadowSprite(POINT* pPoint, CShadowSprite* pSprite)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltClipHeight(lpSurface, Pitch, &rect);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->Blt(lpSurface, Pitch);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}


//----------------------------------------------------------------------
// BltDarkness Sprite
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltShadowSpriteDarkness(POINT* pPoint, CShadowSprite* pSprite, BYTE DarkBits)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltDarknessClipWidth(lpSurface, Pitch, &rect, DarkBits);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltDarknessClipLeft(lpSurface, Pitch, &rect, DarkBits);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltDarknessClipRight(lpSurface, Pitch, &rect, DarkBits);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltDarknessClipHeight(lpSurface, Pitch, &rect, DarkBits);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltDarknessClipHeight(lpSurface, Pitch, &rect, DarkBits);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltDarkness(lpSurface, Pitch, DarkBits);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// BltSmall Sprite
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltShadowSpriteSmall(POINT* pPoint, CShadowSprite* pSprite, BYTE shift)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth() >> shift;
	rect.bottom = pSprite->GetHeight() >> shift;


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + (pSprite->GetWidth() >> shift);
	int yPlusSpriteHeight	= pt.y + (pSprite->GetHeight() >> shift);

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -(pt.x << shift);		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltSmallClipWidth(lpSurface, Pitch, &rect, shift);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltSmallClipLeft(lpSurface, Pitch, &rect, shift);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltSmallClipRight(lpSurface, Pitch, &rect, shift);			
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -(pt.y << shift);

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltSmallClipHeight(lpSurface, Pitch, &rect, shift);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltSmallClipHeight(lpSurface, Pitch, &rect, shift);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltSmall(lpSurface, Pitch, shift);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}


//----------------------------------------------------------------------
// BltSmall4444 Sprite
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltShadowSpriteSmall4444(POINT* pPoint, CShadowSprite* pSprite, WORD pixel, BYTE shift)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth() >> shift;
	rect.bottom = pSprite->GetHeight() >> shift;


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + (pSprite->GetWidth() >> shift);
	int yPlusSpriteHeight	= pt.y + (pSprite->GetHeight() >> shift);

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -(pt.x << shift);		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->BltSmall4444ClipWidth(lpSurface, Pitch, &rect, pixel, shift);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->BltSmall4444ClipLeft(lpSurface, Pitch, &rect, pixel, shift);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
//			pSprite->BltSmall4444ClipRight(lpSurface, Pitch, &rect, pixel, shift);			
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -(pt.y << shift);

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
//				pSprite->BltSmall4444ClipHeight(lpSurface, Pitch, &rect, pixel, shift);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
//					pSprite->BltSmall4444ClipHeight(lpSurface, Pitch, &rect, pixel, shift);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltSmall4444(lpSurface, Pitch, pixel, shift);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}

//----------------------------------------------------------------------
// Blt ShadowSprite4444
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltShadowSprite4444(POINT* pPoint, CShadowSprite* pSprite, WORD pixel)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->Blt4444ClipWidth(lpSurface, Pitch, &rect, pixel);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->Blt4444ClipLeft(lpSurface, Pitch, &rect, pixel);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->Blt4444ClipRight(lpSurface, Pitch, &rect, pixel);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->Blt4444ClipHeight(lpSurface, Pitch, &rect, pixel);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->Blt4444ClipHeight(lpSurface, Pitch, &rect, pixel);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->Blt4444(lpSurface, Pitch, pixel);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}



//----------------------------------------------------------------------
// Half Copy
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 반투명 출력을 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyHalf(WORD* pDest, WORD* pSource, WORD pixels)
{
	QWORD	*qpDest		= (QWORD*)pDest, 
			*qpSource	= (QWORD*)pSource;


	register int qTimes = pixels >> 2;	// pixels / 4

	// 반투명
	switch ( pixels & 0x03 )	// pixels % 4
	{
		//------------------
		// 4점씩
		//------------------
		case 0 :			
			// 네점씩 찍기
			while (qTimes--)
			{
				*qpDest = 
					((*qpDest >> 1) & CDirectDraw::s_qwMASK_SHIFT[1]) +
					((*qpSource >> 1) & CDirectDraw::s_qwMASK_SHIFT[1]);
				

				*qpDest++;
				*qpSource++;
			}
		break;

		//------------------
		// 1점 + 4점씩
		//------------------
		case 1 :
			// 한점 찍기
			*(WORD*)qpDest = 
					((*(WORD*)qpDest >> 1) & CDirectDraw::s_wMASK_SHIFT[1]) +
					((*(WORD*)qpSource >> 1) & CDirectDraw::s_wMASK_SHIFT[1]);
				
			qpDest = (QWORD*)((WORD*)qpDest + 1);
			qpSource = (QWORD*)((WORD*)qpSource + 1);

			// 네점씩 찍기
			while (qTimes--)
			{
				*qpDest = 
					((*qpDest >> 1) & CDirectDraw::s_qwMASK_SHIFT[1]) +
					((*qpSource >> 1) & CDirectDraw::s_qwMASK_SHIFT[1]);

				*qpDest++;
				*qpSource++;
			}
			
		break;

		//------------------
		// 2점 + 4점씩
		//------------------
		case 2 :
			// 두점 찍기
			*(DWORD*)qpDest = 
					((*(DWORD*)qpDest >> 1) & CDirectDraw::s_dwMASK_SHIFT[1]) +
					((*(DWORD*)qpSource >> 1) & CDirectDraw::s_dwMASK_SHIFT[1]);
				
			qpDest = (QWORD*)((DWORD*)qpDest + 1);
			qpSource = (QWORD*)((DWORD*)qpSource + 1);


			// 네점씩 찍기
			while (qTimes--)
			{
				*qpDest = 
					((*qpDest >> 1) & CDirectDraw::s_qwMASK_SHIFT[1]) +
					((*qpSource >> 1) & CDirectDraw::s_qwMASK_SHIFT[1]);

				*qpDest++;
				*qpSource++;
			}
		break;

		//------------------
		// 1점 + 2점 + 4점씩
		//------------------
		case 3 :
			// 한점 찍기
			*(WORD*)qpDest = 
					((*(WORD*)qpDest >> 1) & CDirectDraw::s_wMASK_SHIFT[1]) +
					((*(WORD*)qpSource >> 1) & CDirectDraw::s_wMASK_SHIFT[1]);
				
			qpDest = (QWORD*)((WORD*)qpDest + 1);
			qpSource = (QWORD*)((WORD*)qpSource + 1);


			// 두점 찍기
			*(DWORD*)qpDest = 
					((*(DWORD*)qpDest >> 1) & CDirectDraw::s_dwMASK_SHIFT[1]) +
					((*(DWORD*)qpSource >> 1) & CDirectDraw::s_dwMASK_SHIFT[1]);
				
			qpDest = (QWORD*)((DWORD*)qpDest + 1);
			qpSource = (QWORD*)((DWORD*)qpSource + 1);

			// 네점씩 찍기
			while (qTimes--)
			{
				*qpDest = 
					((*qpDest >> 1) & CDirectDraw::s_qwMASK_SHIFT[1]) +
					((*qpSource >> 1) & CDirectDraw::s_qwMASK_SHIFT[1]);

				*qpDest++;
				*qpSource++;
			}
		break;
	}
}


//----------------------------------------------------------------------
// Alpha Copy 1Pixel
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 s_Value1값으로 출력을 한다.
//
// s_Value1 = 1~32
//
//----------------------------------------------------------------------
/*
WORD
CSpriteSurface::memcpyAlpha1Pixel(WORD pDest, WORD pSource)
{
	WORD sr,sg,sb, dr,dg,db;

	// 한점 찍기
	sr = CDirectDraw::Red(pSource);
	sg = CDirectDraw::Green(pSource);
	sb = CDirectDraw::Blue(pSource);

	dr = CDirectDraw::Red(pDest);
	dg = CDirectDraw::Green(pDest);
	db = CDirectDraw::Blue(pDest);		
	
	return ((s_Value1 * (sb - db) >> 5) + db |
				((s_Value1 * (sg - dg) >> 5) + dg) << CDirectDraw::s_bSHIFT_G |
				((s_Value1 * (sr - dr) >> 5) + dr) << CDirectDraw::s_bSHIFT_R);
}
*/

//----------------------------------------------------------------------
// Alpha Copy
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 s_Value1값으로 출력을 한다.
//
// s_Value1 = 1~32
//
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyAlpha(WORD* pDest, WORD* pSource, WORD pixels)
{
//	WORD sr,sg,sb, dr,dg,db;
//	
//	while(pixels--)
//	{
//	// 한점 찍기
//	sr = CDirectDraw::Red(*pSource);
//	sg = CDirectDraw::Green(*pSource);
//	sb = CDirectDraw::Blue(*pSource);
//	
//	dr = CDirectDraw::Red(*pDest);
//	dg = CDirectDraw::Green(*pDest);
//	db = CDirectDraw::Blue(*pDest);		
//	
//	*pDest = ((s_Value1 * (sb - db) >> 5) + db |
//		((s_Value1 * (sg - dg) >> 5) + dg) << CDirectDraw::s_bSHIFT_G |
//		((s_Value1 * (sr - dr) >> 5) + dr) << CDirectDraw::s_bSHIFT_R);
//	
//	pDest++;
//	pSource++;
//	}
//	return;

	QWORD	*qpDest		= (QWORD*)pDest, 
			*qpSource	= (QWORD*)pSource;

	//static WORD		sTemp,dTemp;
	//static WORD		sr,sg,sb,dr,dg,db;

	DWORD	srcColor2, destColor2;
	DWORD	first2, second2;

	QWORD	srcColor, destColor;
	QWORD	first, second;

	register int qTimes = pixels >> 2;	// pixels / 4

	// Alpha Blending
	switch ( pixels & 0x03 )	// pixels % 4
	{
		//------------------
		// 4점씩
		//------------------
		case 0 :			
			// 네점씩 찍기
			/*
			while (qTimes--)
			{
				srcColor  = *qpSource;
				destColor = *qpDest;
			
				first   =   (((srcColor & CDirectDraw::s_qwMASK_ALPHA0) >> 5) * s_Value1) &  CDirectDraw::s_qwMASK_ALPHA0;				
				second  =   (((srcColor & CDirectDraw::s_qwMASK_ALPHA1) * s_Value1) >> 5) & CDirectDraw::s_qwMASK_ALPHA1;
				
				srcColor =  first | second;

				first   =   (((destColor & CDirectDraw::s_qwMASK_ALPHA0) >> 5) * s_Value2) & CDirectDraw::s_qwMASK_ALPHA0;				
				second  =   (((destColor & CDirectDraw::s_qwMASK_ALPHA1) * s_Value2) >> 5) & CDirectDraw::s_qwMASK_ALPHA1;				

				destColor=  first | second;
				
				*qpDest = srcColor + destColor;

				qpDest++;
				qpSource++;
			}
			*/
			///*
			while (qTimes--)
			{
				if(*qpSource != *qpDest)
				{
					srcColor  = *qpSource;
					destColor = *qpDest;
				
					first   =   srcColor & CDirectDraw::s_qwMASK_ALPHA0;
					first   >>= 5;
					first   *=  s_Value1;
					first   &=  CDirectDraw::s_qwMASK_ALPHA0;
					
					second  =   srcColor & CDirectDraw::s_qwMASK_ALPHA1;
					second  *=  s_Value1;
					second  >>= 5;
					second  &=  CDirectDraw::s_qwMASK_ALPHA1;
					
					srcColor =  first | second;

					first   =   destColor & CDirectDraw::s_qwMASK_ALPHA0;
					first   >>= 5;
					first   *=  s_Value2;
					first   &=  CDirectDraw::s_qwMASK_ALPHA0;

					second  =   destColor & CDirectDraw::s_qwMASK_ALPHA1;
					second  *=  s_Value2;
					second  >>= 5;
					second  &=  CDirectDraw::s_qwMASK_ALPHA1;

					destColor=  first | second;
					
					*qpDest = srcColor + destColor;
				}

				qpDest++;
				qpSource++;
			}
			//*/
		break;

		//------------------
		// 1점 + 4점씩
		//------------------
		case 1 :
			// 한 점 찍기
			srcColor2  = *(WORD*)qpSource;
			destColor2 = *(WORD*)qpDest;

			if(srcColor2 != destColor2)
			{
				first2   =   srcColor2 & CDirectDraw::s_wMASK_ALPHA1;
				first2   >>= 5;
				first2   *=  s_Value1;
				first2   &=  CDirectDraw::s_wMASK_ALPHA1;

				second2  =   srcColor2 & CDirectDraw::s_wMASK_ALPHA0;
				second2  *=  s_Value1;
				second2  >>= 5;
				second2  &=  CDirectDraw::s_wMASK_ALPHA0;

				srcColor2 =  first2 | second2;

				first2   =   destColor2 & CDirectDraw::s_wMASK_ALPHA1;
				first2   >>= 5;
				first2   *=  s_Value2;
				first2   &=  CDirectDraw::s_wMASK_ALPHA1;

				second2  =   destColor2 & CDirectDraw::s_wMASK_ALPHA0;
				second2  *=  s_Value2;
				second2  >>= 5;
				second2  &=  CDirectDraw::s_wMASK_ALPHA0;

				destColor2=  first2 | second2;

				*(WORD*)qpDest = (WORD)(srcColor2 + destColor2);
			}
				
			qpDest = (QWORD*)((WORD*)qpDest + 1);
			qpSource = (QWORD*)((WORD*)qpSource + 1);
			

			// 네점씩 찍기
			while (qTimes--)
			{
				if(*qpSource != *qpDest)
				{
					srcColor  = *qpSource;
					destColor = *qpDest;

					first   =   srcColor & CDirectDraw::s_qwMASK_ALPHA0;
					first   >>= 5;
					first   *=  s_Value1;
					first   &=  CDirectDraw::s_qwMASK_ALPHA0;

					second  =   srcColor & CDirectDraw::s_qwMASK_ALPHA1;
					second  *=  s_Value1;
					second  >>= 5;
					second  &=  CDirectDraw::s_qwMASK_ALPHA1;

					srcColor =  first | second;

					first   =   destColor & CDirectDraw::s_qwMASK_ALPHA0;
					first   >>= 5;
					first   *=  s_Value2;
					first   &=  CDirectDraw::s_qwMASK_ALPHA0;

					second  =   destColor & CDirectDraw::s_qwMASK_ALPHA1;
					second  *=  s_Value2;
					second  >>= 5;
					second  &=  CDirectDraw::s_qwMASK_ALPHA1;

					destColor=  first | second;

					*qpDest = srcColor + destColor;
				}

				qpDest++;
				qpSource++;
			}
			
		break;

		//------------------
		// 2점 + 4점씩
		//------------------
		case 2 :
			// 두점 찍기
			srcColor2  = *(DWORD*)qpSource;
			destColor2 = *(DWORD*)qpDest;

			if(srcColor2 != destColor2)
			{
				first2   =   srcColor2 & CDirectDraw::s_dwMASK_ALPHA0;
				first2   >>= 5;
				first2   *=  s_Value1;
				first2   &=  CDirectDraw::s_dwMASK_ALPHA0;

				second2  =   srcColor2 & CDirectDraw::s_dwMASK_ALPHA1;
				second2  *=  s_Value1;
				second2  >>= 5;
				second2  &=  CDirectDraw::s_dwMASK_ALPHA1;

				srcColor2 =  first2 | second2;

				first2   =   destColor2 & CDirectDraw::s_dwMASK_ALPHA0;
				first2   >>= 5;
				first2   *=  s_Value2;
				first2   &=  CDirectDraw::s_dwMASK_ALPHA0;

				second2  =   destColor2 & CDirectDraw::s_dwMASK_ALPHA1;
				second2  *=  s_Value2;
				second2  >>= 5;
				second2  &=  CDirectDraw::s_dwMASK_ALPHA1;

				destColor2=  first2 | second2;

				*(DWORD*)qpDest = srcColor2 + destColor2;
			}
			
			qpDest = (QWORD*)((DWORD*)qpDest + 1);
			qpSource = (QWORD*)((DWORD*)qpSource + 1);


			// 네점씩 찍기
			while (qTimes--)
			{
				if(*qpSource != *qpDest)
				{
					srcColor  = *qpSource;
					destColor = *qpDest;

					first   =   srcColor & CDirectDraw::s_qwMASK_ALPHA0;
					first   >>= 5;
					first   *=  s_Value1;
					first   &=  CDirectDraw::s_qwMASK_ALPHA0;

					second  =   srcColor & CDirectDraw::s_qwMASK_ALPHA1;
					second  *=  s_Value1;
					second  >>= 5;
					second  &=  CDirectDraw::s_qwMASK_ALPHA1;

					srcColor =  first | second;

					first   =   destColor & CDirectDraw::s_qwMASK_ALPHA0;
					first   >>= 5;
					first   *=  s_Value2;
					first   &=  CDirectDraw::s_qwMASK_ALPHA0;

					second  =   destColor & CDirectDraw::s_qwMASK_ALPHA1;
					second  *=  s_Value2;
					second  >>= 5;
					second  &=  CDirectDraw::s_qwMASK_ALPHA1;

					destColor=  first | second;

					*qpDest = srcColor + destColor;
				}
				qpDest++;
				qpSource++;
			}
		break;

		//------------------
		// 1점 + 2점 + 4점씩
		//------------------
		case 3 :
			// 한점 찍기
			/*
			sTemp = *((WORD*)qpSource);
			dTemp = *((WORD*)qpDest);
			sb = sTemp & 0x1F;
			db = dTemp & 0x1F;
			sg = (sTemp >> 5) & 0x3F;
			dg = (dTemp >> 5) & 0x3F;
			sr = (sTemp >> 11) & 0x1F;
			dr = (dTemp >> 11) & 0x1F;

			*((WORD*)qpDest) = (WORD)((s_Value1 * (sb - db) >> 5) + db |
									((s_Value1 * (sg - dg) >> 5) + dg) << 5 |
									((s_Value1 * (sr - dr) >> 5) + dr) << 11);
			//*(WORD*)qpDest = (((sr*s_Value1 + dr*s_Value2) >> 5) << 11) |
							//(((sg*s_Value1 + dg*s_Value2) >> 5) << 5) |
							//	((sb*s_Value1 + db*s_Value2) >> 5);

			qpDest = (QWORD*)((WORD*)qpDest + 1);
			qpSource = (QWORD*)((WORD*)qpSource + 1);
			*/
			// 한점 찍기		
			srcColor2  = *(WORD*)qpSource;
			destColor2 = *(WORD*)qpDest;

			if(srcColor2 != destColor2)
			{
				first2   =   srcColor2 & CDirectDraw::s_wMASK_ALPHA1;
				first2   >>= 5;
				first2   *=  s_Value1;
				first2   &=  CDirectDraw::s_wMASK_ALPHA1;

				second2  =   srcColor2 & CDirectDraw::s_wMASK_ALPHA0;
				second2  *=  s_Value1;
				second2  >>= 5;
				second2  &=  CDirectDraw::s_wMASK_ALPHA0;

				srcColor2 =  first2 | second2;

				first2   =   destColor2 & CDirectDraw::s_wMASK_ALPHA1;
				first2   >>= 5;
				first2   *=  s_Value2;
				first2   &=  CDirectDraw::s_wMASK_ALPHA1;

				second2  =   destColor2 & CDirectDraw::s_wMASK_ALPHA0;
				second2  *=  s_Value2;
				second2  >>= 5;
				second2  &=  CDirectDraw::s_wMASK_ALPHA0;

				destColor2=  first2 | second2;

				*(WORD*)qpDest = (WORD)(srcColor2 + destColor2);
			}

			qpDest = (QWORD*)((WORD*)qpDest + 1);
			qpSource = (QWORD*)((WORD*)qpSource + 1);				
			

			// 두점 찍기
			srcColor2  = *(DWORD*)qpSource;
			destColor2 = *(DWORD*)qpDest;

			if(srcColor2 != destColor2)
			{
				// 1111 1000 0001 1111 
				// 0000 0111 1110 0000
				first2   =   srcColor2 & CDirectDraw::s_dwMASK_ALPHA0;
				first2   >>= 5;
				first2   *=  s_Value1;
				first2   &=  CDirectDraw::s_dwMASK_ALPHA0;

				second2  =   srcColor2 & CDirectDraw::s_dwMASK_ALPHA1;
				second2  *=  s_Value1;
				second2  >>= 5;
				second2  &=  CDirectDraw::s_dwMASK_ALPHA1;

				srcColor2 =  first2 | second2;

				first2   =   destColor2 & CDirectDraw::s_dwMASK_ALPHA0;
				first2   >>= 5;
				first2   *=  s_Value2;
				first2   &=  CDirectDraw::s_dwMASK_ALPHA0;

				second2  =   destColor2 & CDirectDraw::s_dwMASK_ALPHA1;
				second2  *=  s_Value2;
				second2  >>= 5;
				second2  &=  CDirectDraw::s_dwMASK_ALPHA1;

				destColor2=  first2 | second2;

				*(DWORD*)qpDest = srcColor2 + destColor2;
			}

			qpDest = (QWORD*)((DWORD*)qpDest + 1);
			qpSource = (QWORD*)((DWORD*)qpSource + 1);

			// 네점씩 찍기
			while (qTimes--)
			{
				if(*qpSource != *qpDest)
				{
					srcColor  = *qpSource;
					destColor = *qpDest;

					first   =   srcColor & CDirectDraw::s_qwMASK_ALPHA0;
					first   >>= 5;
					first   *=  s_Value1;
					first   &=  CDirectDraw::s_qwMASK_ALPHA0;

					second  =   srcColor & CDirectDraw::s_qwMASK_ALPHA1;
					second  *=  s_Value1;
					second  >>= 5;
					second  &=  CDirectDraw::s_qwMASK_ALPHA1;

					srcColor =  first | second;

					first   =   destColor & CDirectDraw::s_qwMASK_ALPHA0;
					first   >>= 5;
					first   *=  s_Value2;
					first   &=  CDirectDraw::s_qwMASK_ALPHA0;

					second  =   destColor & CDirectDraw::s_qwMASK_ALPHA1;
					second  *=  s_Value2;
					second  >>= 5;
					second  &=  CDirectDraw::s_qwMASK_ALPHA1;

					destColor=  first | second;

					*qpDest = srcColor + destColor;
				}

				qpDest++;
				qpSource++;
			}
		break;
	}
}


//----------------------------------------------------------------------
// Color Copy
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특정한 Color값(s_Value1)으로 출력을 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyColor(WORD* pDest, WORD* pSource, WORD pixels)
{
	QWORD	*qpDest		= (QWORD*)pDest, 
			*qpSource	= (QWORD*)pSource;


	register int qTimes = pixels >> 2;	// pixels / 4

	// rgb값 중 하나만 출력한다.
	switch ( pixels & 0x03 )	// pixels % 4
	{
		//------------------
		// 4점씩
		//------------------
		case 0 :			
			// 네점씩 찍기
			while (qTimes--)
			{
				*qpDest = (*qpSource & CDirectDraw::s_qwMASK_RGB[s_Value1]);

				*qpDest++;
				*qpSource++;
			}
		break;

		//------------------
		// 1점 + 4점씩
		//------------------
		case 1 :
			// 한점 찍기
			*(WORD*)qpDest = *(WORD*)qpSource & CDirectDraw::s_wMASK_RGB[s_Value1];
				
			qpDest = (QWORD*)((WORD*)qpDest + 1);
			qpSource = (QWORD*)((WORD*)qpSource + 1);

			// 네점씩 찍기
			while (qTimes--)
			{
				*qpDest = (*qpSource & CDirectDraw::s_qwMASK_RGB[s_Value1]);

				*qpDest++;
				*qpSource++;
			}
			
		break;

		//------------------
		// 2점 + 4점씩
		//------------------
		case 2 :
			// 두점 찍기
			*(DWORD*)qpDest = *(DWORD*)qpSource & CDirectDraw::s_dwMASK_RGB[s_Value1];
				
			qpDest = (QWORD*)((WORD*)qpDest + 1);
			qpSource = (QWORD*)((WORD*)qpSource + 1);

			// 네점씩 찍기
			while (qTimes--)
			{
				*qpDest = (*qpSource & CDirectDraw::s_qwMASK_RGB[s_Value1]);

				*qpDest++;
				*qpSource++;
			}
		break;

		//------------------
		// 1점 + 2점 + 4점씩
		//------------------
		case 3 :
			// 한점 찍기
			*(WORD*)qpDest = *(WORD*)qpSource & CDirectDraw::s_wMASK_RGB[s_Value1];
				
			qpDest = (QWORD*)((WORD*)qpDest + 1);
			qpSource = (QWORD*)((WORD*)qpSource + 1);

			// 두점 찍기
			*(DWORD*)qpDest = *(DWORD*)qpSource & CDirectDraw::s_dwMASK_RGB[s_Value1];

			qpDest = (QWORD*)((WORD*)qpDest + 1);
			qpSource = (QWORD*)((WORD*)qpSource + 1);

			// 네점씩 찍기
			while (qTimes--)
			{
				*qpDest = (*qpSource & CDirectDraw::s_qwMASK_RGB[s_Value1]);

				*qpDest++;
				*qpSource++;
			}
		break;
	}
}

//----------------------------------------------------------------------
// Scale Copy
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 Scale(s_Value1)을 바꿔서 
// 출력한다. 
//
// 이 함수는 실제 게임에서 사용하지 않으므로 
// 속도에 신경을 안써도 될 것이다. - -;;;
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyScale(WORD* pDest, WORD destPitch, WORD* pSource, WORD pixels)
{
	WORD*	pDestTemp, *pDestTemp2;

	register int n=pixels,i,j;

	// n개의 점을 출력한다.
	while (n--)
	{
		pDestTemp = pDest;
		// 각 점에 대해서 scale*scale의 크기로 출력한다.

		i = s_Value1;
		while (i--)
		{
			pDestTemp2 = pDestTemp;

			j=s_Value1;
			while (j--)
			{
				*pDestTemp2++ = *pSource;
			}
			pDestTemp = (WORD*)((BYTE*)pDestTemp + destPitch);
		}

		pDest += s_Value1;
		pSource++;
	}
}


//----------------------------------------------------------------------
// Darkness Copy
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특정한 Darkness값(s_Value1)만큼
// 어둡게 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyDarkness(WORD* pDest, WORD* pSource, WORD pixels)
{
	QWORD	*qpDest		= (QWORD*)pDest, 
			*qpSource	= (QWORD*)pSource;


	register int qTimes = pixels >> 2;	// pixels / 4

	// 어둡게 출력한다.
	switch ( pixels & 0x03 )	// pixels % 4
	{
		//------------------
		// 4점씩
		//------------------
		case 0 :			
			// 네점씩 찍기
			while (qTimes--)
			{					
				*qpDest = (*qpSource >> s_Value1) & CDirectDraw::s_qwMASK_SHIFT[s_Value1];

				*qpDest++;
				*qpSource++;
			}
		break;

		//------------------
		// 1점 + 4점씩
		//------------------
		case 1 :
			// 한점 찍기
			*(WORD*)qpDest = (*(WORD*)qpSource >> s_Value1) & CDirectDraw::s_wMASK_SHIFT[s_Value1];
				
			qpDest = (QWORD*)((WORD*)qpDest + 1);
			qpSource = (QWORD*)((WORD*)qpSource + 1);

			// 네점씩 찍기
			while (qTimes--)
			{
				*qpDest = (*qpSource >> s_Value1) & CDirectDraw::s_qwMASK_SHIFT[s_Value1];

				*qpDest++;
				*qpSource++;
			}
			
		break;

		//------------------
		// 2점 + 4점씩
		//------------------
		case 2 :
			// 두점 찍기
			*(DWORD*)qpDest = (*(DWORD*)qpSource >> s_Value1) & CDirectDraw::s_dwMASK_SHIFT[s_Value1];
				
			qpDest = (QWORD*)((DWORD*)qpDest + 1);
			qpSource = (QWORD*)((DWORD*)qpSource + 1);

			// 네점씩 찍기
			while (qTimes--)
			{
				*qpDest = (*qpSource >> s_Value1) & CDirectDraw::s_qwMASK_SHIFT[s_Value1];

				*qpDest++;
				*qpSource++;
			}
		break;

		//------------------
		// 1점 + 2점 + 4점씩
		//------------------
		case 3 :
			// 한점 찍기
			*(WORD*)qpDest = (*(WORD*)qpSource >> s_Value1) & CDirectDraw::s_wMASK_SHIFT[s_Value1];
				
			qpDest = (QWORD*)((WORD*)qpDest + 1);
			qpSource = (QWORD*)((WORD*)qpSource + 1);

			// 두점 찍기
			*(DWORD*)qpDest = (*(DWORD*)qpSource >> s_Value1) & CDirectDraw::s_dwMASK_SHIFT[s_Value1];

			qpDest = (QWORD*)((DWORD*)qpDest + 1);
			qpSource = (QWORD*)((DWORD*)qpSource + 1);

			// 네점씩 찍기
			while (qTimes--)
			{
				*qpDest = (*qpSource >> s_Value1) & CDirectDraw::s_qwMASK_SHIFT[s_Value1];

				*qpDest++;
				*qpSource++;
			}
		break;
	}
}

//----------------------------------------------------------------------
// Brightness Copy
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특정한 Brightness값(s_Value1)만큼
// 밝게 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyBrightness(WORD* pDest, WORD* pSource, WORD pixels)
{
	int R,G,B;

	int register i=pixels;

	while (i--)
	{
		R = ((CDirectDraw::Red(*pSource) + s_Value1) & 0x001F) << CDirectDraw::s_bSHIFT_R;
		G = ((CDirectDraw::Green(*pSource) + s_Value1) & 0x001F) << CDirectDraw::s_bSHIFT_G;
		B = (CDirectDraw::Blue(*pSource) + s_Value1) & 0x001F;
		
		*pDest = R | G | B;			
			
		pDest++;
		pSource++;
	}

	//*pDest = (R << CDirectDraw::s_bSHIFT_R) 
				//| (G << CDirectDraw::s_bSHIFT_G) 
				//| B;
	
}


//----------------------------------------------------------------------
// Effect Copy - Darker
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특수효과 처리를 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyEffectDarker(WORD* pDest, WORD* pSource, WORD pixels)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Darker : BG - SPR_Filter
	//------------------------------------------------------------
	int		darkR,
			darkG,
			darkB;
			
	while (i--)
	{
		darkR = Red(*pSource);
		darkG = Green(*pSource);
		darkB = Blue(*pSource);;

		*pDest = 
			(((Red(*pDest) * darkR) >> 5) << s_bSHIFT_R)
			| (((Green(*pDest) * darkG) >> 5) << s_bSHIFT_G)
			| ((Blue(*pDest) * darkB) >> 5);
			
		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// Effect Copy - GrayScale
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특수효과 처리를 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyEffectGrayScale(WORD* pDest, WORD* pSource, WORD pixels)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Gray Scale : color value = (R+G+B)/3
	//------------------------------------------------------------
	BYTE average;

	while (i--)
	{
		average = ( Red(*pSource) +
					Green(*pSource) +
					Blue(*pSource) ) / 3;

		*pDest = (average << s_bSHIFT_R) 
				| (average << s_bSHIFT_G)
				| average;			
		
		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// Effect Copy - Lighten
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특수효과 처리를 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyEffectLighten(WORD* pDest, WORD* pSource, WORD pixels)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Lighten : max(BG, SPR)
	//------------------------------------------------------------
	while (i--)
	{
		*pDest = 
			(max( Red(*pDest), Red(*pSource) ) << s_bSHIFT_R)
			| (max( Green(*pDest), Green(*pSource) ) << s_bSHIFT_G)
			| (max( Blue(*pDest), Blue(*pSource) ));
		
		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// Effect Copy - Darken
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특수효과 처리를 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyEffectDarken(WORD* pDest, WORD* pSource, WORD pixels)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Darken : min(BG, SPR)
	//------------------------------------------------------------	
	while (i--)
	{

		*pDest = 
			(min( Red(*pDest), Red(*pSource) ) << s_bSHIFT_R)
			| (min( Green(*pDest), Green(*pSource) ) << s_bSHIFT_G)
			| (min( Blue(*pDest), Blue(*pSource) ));

		pDest++;
		pSource++;
	}
	
}

//----------------------------------------------------------------------
// Effect Copy - ColorDodge
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특수효과 처리를 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyEffectColorDodge(WORD* pDest, WORD* pSource, WORD pixels)
{
	register int i = pixels;
	
	//------------------------------------------------------------	
	// Color Dodge : (BG*MAXDAC)/(MAXDAC-SPR)
	//------------------------------------------------------------	
	while (i--)
	{

		*pDest = 
			((Red(*pDest) << 5) / (32-Red(*pSource)) << s_bSHIFT_R)
			| ((Green(*pDest) << 5) / (32-Green(*pSource)) << s_bSHIFT_G)
			| (Blue(*pDest) << 5) / (32-Blue(*pSource));
			

		pDest++;
		pSource++;
	}
	
}

//----------------------------------------------------------------------
// Effect Copy - Screen
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특수효과 처리를 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyEffectScreen(WORD* pDest, WORD* pSource, WORD pixels)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Screen :  TempSum=(MAXDAC-max(BG,SPR))/MAXDAC*min(BG,SPR)
	//           Screen=max(BG,SPR)+TempSum
	//------------------------------------------------------------	
	WORD temp;
	int d, s;
	//int m; 
	while (i--)
	{
		// R
		d = Red(*pDest);
		s = Red(*pSource);	

		//m = max(d,s);
		//temp = (((32 - m) * min(d,s) >> 5) + m) << s_bSHIFT_R;
		temp = s_EffectScreenTableR[d][s];

		// G
		d = Green(*pDest);
		s = Green(*pSource);

		//m = max(d,s);
		//temp |= (((32 - m) * min(d,s) >> 5) + m) << s_bSHIFT_G;
		temp |= s_EffectScreenTableG[d][s];
		
		// B
		d = Blue(*pDest);
		s = Blue(*pSource);

		//m = max(d,s);
		//temp |= ((32 - m) * min(d,s) >> 5) + m;
		temp |= s_EffectScreenTableB[d][s];

		// 
		*pDest = temp;

		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// Effect Copy - Screen
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특수효과 처리를 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyEffectScreenAlpha(WORD* pDest, WORD* pSource, WORD pixels)
{
	register int i = pixels;
	
	//------------------------------------------------------------	
	// Screen :  TempSum=(MAXDAC-max(BG,SPR))/MAXDAC*min(BG,SPR)
	//           Screen=max(BG,SPR)+TempSum
	//------------------------------------------------------------	
	WORD temp;
	int d, s;
	int sr,sg,sb, dr,dg,db;
	//int m; 
	while (i--)
	{
		// R
		d = Red(*pDest);
		s = Red(*pSource);	
		
		//m = max(d,s);
		//temp = (((32 - m) * min(d,s) >> 5) + m) << s_bSHIFT_R;
		temp = s_EffectScreenTableR[d][s];
		
		// G
		d = Green(*pDest);
		s = Green(*pSource);
		
		//m = max(d,s);
		//temp |= (((32 - m) * min(d,s) >> 5) + m) << s_bSHIFT_G;
		temp |= s_EffectScreenTableG[d][s];
		
		// B
		d = Blue(*pDest);
		s = Blue(*pSource);
		
		//m = max(d,s);
		//temp |= ((32 - m) * min(d,s) >> 5) + m;
		temp |= s_EffectScreenTableB[d][s];
		
		// 
		// 한점 찍기
		sr = CDirectDraw::Red(temp);
		sg = CDirectDraw::Green(temp);
		sb = CDirectDraw::Blue(temp);
		
		dr = CDirectDraw::Red(*pDest);
		dg = CDirectDraw::Green(*pDest);
		db = CDirectDraw::Blue(*pDest);		
		
		*pDest = ((s_Value1 * (sb - db) >> 5) + db |
			((s_Value1 * (sg - dg) >> 5) + dg) << CDirectDraw::s_bSHIFT_G |
			((s_Value1 * (sr - dr) >> 5) + dr) << CDirectDraw::s_bSHIFT_R);
		
		pDest++;
		pSource++;
	}
}


//----------------------------------------------------------------------
// Effect Copy - DodgeBurn
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특수효과 처리를 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyEffectDodgeBurn(WORD* pDest, WORD* pSource, WORD pixels)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// DodgeBurn = (BG*(MAXDAC-SPR))/MAXDAC
	//------------------------------------------------------------	
	while (i--)
	{

		*pDest = 		
			 (( (Red(*pDest) * (32 - Red(*pSource)) ) >> 5) << s_bSHIFT_R)
			| ((( Green(*pDest) * (32 - Green(*pSource))) >> 5) << s_bSHIFT_G)
			| ((Blue(*pDest) * (32 - Blue(*pSource))) >> 5);
			

		pDest++;
		pSource++;
	}

}

//----------------------------------------------------------------------
// Effect Copy - Different
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특수효과 처리를 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyEffectDifferent(WORD* pDest, WORD* pSource, WORD pixels)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Different=(max(BG,SPR)-min(BG,SPR))
	//------------------------------------------------------------	
	int temp;
	WORD d, s;
	
	while (i--)
	{
		temp = 0;

		d = Red(*pDest);
		s = Red(*pSource);
		temp |= (max(d,s)-min(d,s)) << 11;

		d = Green(*pDest);
		s = Green(*pSource);
		temp |= (max(d,s)-min(d,s)) << 5;

		d = Blue(*pDest);
		s = Blue(*pSource);
		temp |= max(d,s)-min(d,s);
		
		*pDest = temp;

		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// Effect Copy - Gradation
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특수효과 처리를 한다.
//
// s_Value1이 ColorSet번호이다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyEffectGradation(WORD* pDest, WORD* pSource, WORD pixels)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Different=(max(BG,SPR)-min(BG,SPR))
	//------------------------------------------------------------	
	WORD s;
	BYTE r, g, b;
	BYTE gradation;

	while (i--)
	{
		s = *pSource;

		r = s >> s_bSHIFT_R;		
		g = (s >> s_bSHIFT_G) & 0x1F;
		b = s & 0x1F;

		gradation = CIndexSprite::ColorToGradation[ r + g + b ];

		*pDest = CIndexSprite::ColorSet[s_Value1][gradation];

		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// Effect SimpleOutline
//----------------------------------------------------------------------
// 좀 띄엄띄엄한 외곽선 출력(투명색 압축의 양 끝 점에 출력)
// pixels는 1이상이라고 가정한다.
//----------------------------------------------------------------------
void		
CSpriteSurface::memcpyEffectSimpleOutline(WORD* pDest, WORD* pSource, WORD pixels)
{
	// 첫 점
	*pDest = *pSource;
	
	int pixels_1 = pixels-1;

	// 끝 점 - pixels가 1일때 비교하는거보다 그냥 찍는게 더 빠를까?
	*(pDest+pixels_1) = *(pSource+pixels_1);	
}

//----------------------------------------------------------------------
// Effect WipeOut
//----------------------------------------------------------------------
// s_Value1 : 덜 출력할 수준? 
//				64 - 투명
//				0 - 다 출력
//
// 중심에서부터 양쪽으로 지워져간다.
//
// ***************
// ******   ******
//----------------------------------------------------------------------
void		
CSpriteSurface::memcpyEffectWipeOut(WORD* pDest, WORD* pSource, WORD pixels)
{
	register int i = pixels;

	int skipPixels = (pixels * s_Value1) >> 6;	// / 64
	int drawPixels = (pixels - skipPixels)>>1;
	int drawPixels2 = pixels - drawPixels - skipPixels;
	
	// [1] drawPixels만큼 출력하고..
	// [2] skipPixels 만큼 건너띄고
	// [3] drawPixels2만큼 출력
	
	//------------------------------------------------------------	
	// drawPixels만큼 출력
	//------------------------------------------------------------	
	memcpy(pDest, pSource, (drawPixels<<1));
	pDest += drawPixels;
	pSource += drawPixels;

	//------------------------------------------------------------	
	// skipPixels만큼 건너띔
	//------------------------------------------------------------	
	pDest += skipPixels;
	pSource += skipPixels;

	//------------------------------------------------------------	
	// drawPixels2만큼 출력
	//------------------------------------------------------------	
	memcpy(pDest, pSource, (drawPixels2<<1));
	//pDest += drawPixels2;
	//pSource += drawPixels2;
}

//----------------------------------------------------------------------
// Effect Net
//----------------------------------------------------------------------
// s_Value1 : 건너띄는 점
//
// 한 점 찍고.. s_Value1의 점 수만큼 건너띈다.
//
// ***************
// * * * * * * * *	: s_Value1 = 1
// *  *  *  *  *    : s_Value1 = 2
// *   *   *   *    : s_Value1 = 3
//----------------------------------------------------------------------
void		
CSpriteSurface::memcpyEffectNet(WORD* pDest, WORD* pSource, WORD pixels)
{
	register int i = pixels;

	int skipPixels = 1 + s_Value1;
	
	//------------------------------------------------------------	
	// drawPixels만큼 출력
	//------------------------------------------------------------	
	do {
		//memcpy(pDest, pSource, (drawPixels<<1));
		*pDest = *pSource;

		pDest += skipPixels;
		pSource += skipPixels;

		i -= skipPixels;
	} while (i > 0);
}

//----------------------------------------------------------------------
// Effect Copy - GrayScaleVarious
//----------------------------------------------------------------------
// s_Value1 은 얼마나 gray되는가?(-_-;)이다..
// s_Value1값은 0~31.. 32인가?.. - -
// 0이면 완전 gray
// 32이면 원래색깔
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyEffectGrayScaleVarious(WORD* pDest, WORD* pSource, WORD pixels)
{
	register int i = pixels;

	//int grayValue = s_Value1;

	//------------------------------------------------------------	
	// Gray Scale : color value = (R+G+B)/3
	//------------------------------------------------------------
	int average;
	int r, g, b;

	while (i--)
	{	
		r = Red(*pSource);
		g = Green(*pSource);
		b = Blue(*pSource);

		average = ( r + g + b ) / 3;		// 0 ~ 31

		/*
		if (average==0)
		{
			*pDest = 0;
		}
		else
		{
		*/
			r = average + ((r-average)*s_Value1 >> 5);
			g = average + ((g-average)*s_Value1 >> 5);
			b = average + ((b-average)*s_Value1 >> 5);
		
			*pDest = (r << s_bSHIFT_R) 
					| (g << s_bSHIFT_G)
					| b;			
		//}
		
		pDest++;
		pSource++;
	}
}

















//----------------------------------------------------------------------
// Effect Copy - Darker
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특수효과 처리를 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectDarker(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Darker : BG - SPR_Filter
	//------------------------------------------------------------
	int		darkR,
			darkG,
			darkB;
			
	while (i--)
	{
		darkR = Red(pal[*pSource]);
		darkG = Green(pal[*pSource]);
		darkB = Blue(pal[*pSource]);;

		*pDest = 
			(((Red(*pDest) * darkR) >> 5) << s_bSHIFT_R)
			| (((Green(*pDest) * darkG) >> 5) << s_bSHIFT_G)
			| ((Blue(*pDest) * darkB) >> 5);
			
		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// Effect Copy - GrayScale
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특수효과 처리를 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectGrayScale(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Gray Scale : color value = (R+G+B)/3
	//------------------------------------------------------------
	BYTE average;

	while (i--)
	{
		average = ( Red(pal[*pSource]) +
					Green(pal[*pSource]) +
					Blue(pal[*pSource]) ) / 3;

		*pDest = (average << s_bSHIFT_R) 
				| (average << s_bSHIFT_G)
				| average;			
		
		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// Effect Copy - Lighten
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특수효과 처리를 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectLighten(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Lighten : max(BG, SPR)
	//------------------------------------------------------------
	while (i--)
	{
		*pDest = 
			(max( Red(*pDest), Red(pal[*pSource]) ) << s_bSHIFT_R)
			| (max( Green(*pDest), Green(pal[*pSource]) ) << s_bSHIFT_G)
			| (max( Blue(*pDest), Blue(pal[*pSource]) ));
		
		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// Effect Copy - Darken
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특수효과 처리를 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectDarken(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Darken : min(BG, SPR)
	//------------------------------------------------------------	
	while (i--)
	{

		*pDest = 
			(min( Red(*pDest), Red(pal[*pSource]) ) << s_bSHIFT_R)
			| (min( Green(*pDest), Green(pal[*pSource]) ) << s_bSHIFT_G)
			| (min( Blue(*pDest), Blue(pal[*pSource]) ));

		pDest++;
		pSource++;
	}
	
}

//----------------------------------------------------------------------
// Effect Copy - ColorDodge
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특수효과 처리를 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectColorDodge(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;
	
	//------------------------------------------------------------	
	// Color Dodge : (BG*MAXDAC)/(MAXDAC-SPR)
	//------------------------------------------------------------	
	while (i--)
	{

		*pDest = 
			((Red(*pDest) << 5) / (32-Red(pal[*pSource])) << s_bSHIFT_R)
			| ((Green(*pDest) << 5) / (32-Green(pal[*pSource])) << s_bSHIFT_G)
			| (Blue(*pDest) << 5) / (32-Blue(pal[*pSource]));
			

		pDest++;
		pSource++;
	}
	
}

//----------------------------------------------------------------------
// Effect Copy - Screen
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특수효과 처리를 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectScreen(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Screen :  TempSum=(MAXDAC-max(BG,SPR))/MAXDAC*min(BG,SPR)
	//           Screen=max(BG,SPR)+TempSum
	//------------------------------------------------------------	
	WORD temp;
	int d, s;
	//int m; 
	while (i--)
	{
		// R
		d = Red(*pDest);
		s = Red(pal[*pSource]);	

		//m = max(d,s);
		//temp = (((32 - m) * min(d,s) >> 5) + m) << s_bSHIFT_R;
		temp = s_EffectScreenTableR[d][s];

		// G
		d = Green(*pDest);
		s = Green(pal[*pSource]);

		//m = max(d,s);
		//temp |= (((32 - m) * min(d,s) >> 5) + m) << s_bSHIFT_G;
		temp |= s_EffectScreenTableG[d][s];
		
		// B
		d = Blue(*pDest);
		s = Blue(pal[*pSource]);

		//m = max(d,s);
		//temp |= ((32 - m) * min(d,s) >> 5) + m;
		temp |= s_EffectScreenTableB[d][s];

		// 
		*pDest = temp;

		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// Effect Copy - Screen
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특수효과 처리를 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectScreenAlpha(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
}

//----------------------------------------------------------------------
// Effect Copy - DodgeBurn
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특수효과 처리를 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectDodgeBurn(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// DodgeBurn = (BG*(MAXDAC-SPR))/MAXDAC
	//------------------------------------------------------------	
	while (i--)
	{

		*pDest = 		
			 (( (Red(*pDest) * (32 - Red(pal[*pSource])) ) >> 5) << s_bSHIFT_R)
			| ((( Green(*pDest) * (32 - Green(pal[*pSource]))) >> 5) << s_bSHIFT_G)
			| ((Blue(*pDest) * (32 - Blue(pal[*pSource]))) >> 5);
			

		pDest++;
		pSource++;
	}

}

//----------------------------------------------------------------------
// Effect Copy - Different
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특수효과 처리를 한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectDifferent(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Different=(max(BG,SPR)-min(BG,SPR))
	//------------------------------------------------------------	
	int temp;
	WORD d, s;
	
	while (i--)
	{
		temp = 0;

		d = Red(*pDest);
		s = Red(pal[*pSource]);
		temp |= (max(d,s)-min(d,s)) << 11;

		d = Green(*pDest);
		s = Green(pal[*pSource]);
		temp |= (max(d,s)-min(d,s)) << 5;

		d = Blue(*pDest);
		s = Blue(pal[*pSource]);
		temp |= max(d,s)-min(d,s);
		
		*pDest = temp;

		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// Effect Copy - Gradation
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 특수효과 처리를 한다.
//
// s_Value1이 ColorSet번호이다.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectGradation(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	//------------------------------------------------------------	
	// Different=(max(BG,SPR)-min(BG,SPR))
	//------------------------------------------------------------	
	WORD s;
	BYTE r, g, b;
	BYTE gradation;

	while (i--)
	{
		s = pal[*pSource];

		r = s >> s_bSHIFT_R;		
		g = (s >> s_bSHIFT_G) & 0x1F;
		b = s & 0x1F;

		gradation = CIndexSprite::ColorToGradation[ r + g + b ];

		*pDest = CIndexSprite::ColorSet[s_Value1][gradation];

		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// Effect SimpleOutline
//----------------------------------------------------------------------
// 좀 띄엄띄엄한 외곽선 출력(투명색 압축의 양 끝 점에 출력)
// pixels는 1이상이라고 가정한다.
//----------------------------------------------------------------------
void		
CSpriteSurface::memcpyPalEffectSimpleOutline(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	// 첫 점
	*pDest = pal[*pSource];
	
	int pixels_1 = pixels-1;

	// 끝 점 - pixels가 1일때 비교하는거보다 그냥 찍는게 더 빠를까?
	*(pDest+pixels_1) = *(pSource+pixels_1);	
}

//----------------------------------------------------------------------
// Effect WipeOut
//----------------------------------------------------------------------
// s_Value1 : 덜 출력할 수준? 
//				64 - 투명
//				0 - 다 출력
//
// 중심에서부터 양쪽으로 지워져간다.
//
// ***************
// ******   ******
//----------------------------------------------------------------------
void		
CSpriteSurface::memcpyPalEffectWipeOut(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	int skipPixels = (pixels * s_Value1) >> 6;	// / 64
	int drawPixels = (pixels - skipPixels)>>1;
	int drawPixels2 = pixels - drawPixels - skipPixels;
	
	// [1] drawPixels만큼 출력하고..
	// [2] skipPixels 만큼 건너띄고
	// [3] drawPixels2만큼 출력
	
	//------------------------------------------------------------	
	// drawPixels만큼 출력
	//------------------------------------------------------------	
	memcpy(pDest, pSource, (drawPixels<<1));
	pDest += drawPixels;
	pSource += drawPixels;

	//------------------------------------------------------------	
	// skipPixels만큼 건너띔
	//------------------------------------------------------------	
	pDest += skipPixels;
	pSource += skipPixels;

	//------------------------------------------------------------	
	// drawPixels2만큼 출력
	//------------------------------------------------------------	
	memcpy(pDest, pSource, (drawPixels2<<1));
	//pDest += drawPixels2;
	//pSource += drawPixels2;
}

//----------------------------------------------------------------------
// Effect Net
//----------------------------------------------------------------------
// s_Value1 : 건너띄는 점
//
// 한 점 찍고.. s_Value1의 점 수만큼 건너띈다.
//
// ***************
// * * * * * * * *	: s_Value1 = 1
// *  *  *  *  *    : s_Value1 = 2
// *   *   *   *    : s_Value1 = 3
//----------------------------------------------------------------------
void		
CSpriteSurface::memcpyPalEffectNet(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	int skipPixels = 1 + s_Value1;
	
	//------------------------------------------------------------	
	// drawPixels만큼 출력
	//------------------------------------------------------------	
	do {
		//memcpy(pDest, pSource, (drawPixels<<1));
		*pDest = pal[*pSource];

		pDest += skipPixels;
		pSource += skipPixels;

		i -= skipPixels;
	} while (i > 0);
}

//----------------------------------------------------------------------
// Effect Copy - GrayScaleVarious
//----------------------------------------------------------------------
// s_Value1 은 얼마나 gray되는가?(-_-;)이다..
// s_Value1값은 0~31.. 32인가?.. - -
// 0이면 완전 gray
// 32이면 원래색깔
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectGrayScaleVarious(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int i = pixels;

	//int grayValue = s_Value1;

	//------------------------------------------------------------	
	// Gray Scale : color value = (R+G+B)/3
	//------------------------------------------------------------
	int average;
	int r, g, b;

	while (i--)
	{	
		r = Red(pal[*pSource]);
		g = Green(pal[*pSource]);
		b = Blue(pal[*pSource]);

		average = ( r + g + b ) / 3;		// 0 ~ 31

		/*
		if (average==0)
		{
			*pDest = 0;
		}
		else
		{
		*/
			r = average + ((r-average)*s_Value1 >> 5);
			g = average + ((g-average)*s_Value1 >> 5);
			b = average + ((b-average)*s_Value1 >> 5);
		
			*pDest = (r << s_bSHIFT_R) 
					| (g << s_bSHIFT_G)
					| b;			
		//}
		
		pDest++;
		pSource++;
	}
}





















//----------------------------------------------------------------------
// Init EffectTable
//----------------------------------------------------------------------
void		
CSpriteSurface::InitEffectTable()
{
	WORD temp;
	WORD m1, m2;

	for (int d=0; d<32; d++)
	{
		for (int s=0; s<32; s++)
		{
			m1 = max(d,s);
			m2 = min(d,s);

			// R			
			temp = (((32 - m1) * m2 >> 5) + m1) << s_bSHIFT_R;
			s_EffectScreenTableR[d][s] = temp;

			// G
			temp = (((32 - m1) * m2 >> 5) + m1) << s_bSHIFT_G;
			s_EffectScreenTableG[d][s] = temp;
			
			// B
			temp = ((32 - m1) * m2 >> 5) + m1;
			s_EffectScreenTableB[d][s] = temp;
		}
	}

}


//----------------------------------------------------------------------
// Blt IndexSprite Darkness
//----------------------------------------------------------------------
// pPoint에 pSprite를 출력한다.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltIndexSpriteBrightness(POINT* pPoint, CIndexSprite* pSprite, BYTE BrightBits)
{	
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite내부의 출력 영역
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// 화면에 보이지 않는 경우 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping이 필요한 부분에 대한 check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// 왼쪽 Clipping이 필요한 상태..
	//------------------------------------------
	if (leftClip)
	{
		// 왼쪽으로 들어간만큼은 생략해서 출력해야 한다.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// 왼쪽 + 오른쪽			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltBrightnessClipWidth(lpSurface, Pitch, &rect, BrightBits);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch);
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// 왼쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltBrightnessClipLeft(lpSurface, Pitch, &rect, BrightBits);
		}
	}
	//------------------------------------------
	// 왼쪽 Clipping이 필요 없는 상태..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// 오른쪽 Clipping이 필요한 상태
		//------------------------------------------
		if (rightClip)
		{	
			// 가로 길이를 줄인다.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{						
					rect.bottom = m_ClipBottom - pt.y;
				}				

				pt.y = m_ClipTop;
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// 오른쪽 Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltBrightnessClipRight(lpSurface, Pitch, &rect, BrightBits);
		}
		//------------------------------------------
		// 오른쪽 Clipping이 필요 없는 상태
		//------------------------------------------
		else
		{
			//------------------------------------------
			// 위쪽 Clipping이 필요한 상태
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height만 Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltBrightnessClipHeight(lpSurface, Pitch, &rect, BrightBits);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// 아래쪽 Clipping이 필요한 상태
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height만 Clip
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltBrightnessClipHeight(lpSurface, Pitch, &rect, BrightBits);
				}
				else
				{
					// No Clip					
					lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
					pSprite->BltBrightness(lpSurface, Pitch, BrightBits);
				}				
			}			
			
		}		
	}
	

	//Unlock();
}
