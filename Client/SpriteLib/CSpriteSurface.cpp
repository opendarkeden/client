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

/* Include SDL backend adapter if enabled */
#ifdef SPRITELIB_BACKEND_SDL
	#include "CSpriteSurface_Adapter.cpp"
#endif


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
#ifdef SPRITELIB_BACKEND_SDL
	: m_backend_surface(SPRITECTL_INVALID_SURFACE)
#endif
{
}

CSpriteSurface::~CSpriteSurface()
{
#ifdef SPRITELIB_BACKEND_SDL
	/* Cleanup backend surface */
	if (m_backend_surface != SPRITECTL_INVALID_SURFACE) {
		spritectl_destroy_surface(m_backend_surface);
		m_backend_surface = SPRITECTL_INVALID_SURFACE;
	}
#endif
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// VLine
//----------------------------------------------------------------------
// ���� �� �׸��� 
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
// ���μ� �׸��� 
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
// ���� �� ���� �׸��� 
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
// ���� �׸��� 
//----------------------------------------------------------------------
void
CSpriteSurface::Line(int x1, int y1, int x2, int y2, WORD color)
{
	// �� ���� �� ������ �׸���.	
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
// Rect������ �о Point��ġ�� ���� ���
// ȭ�� ũ�⸦ ���� �ʵ��� ��ǥ�� �ٽ� �������ش�.
//----------------------------------------------------------------------
bool
CSpriteSurface::ClippingRectToPoint(RECT*& pRect, POINT*& pPoint)
{
	// ������ �Ѿ������ ���� üũ
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

	// ȭ�� ������ �Ѿ�� �ȵȴ�->
	if (pRect->left >= pRect->right || pRect->top >= pRect->bottom)
		return false;

	return true;
}


//----------------------------------------------------------------------
// Blt Darkness
//----------------------------------------------------------------------
// SourceSurface�� DarkBits��ŭ ��Ӱ� ���� *this�� ���
//----------------------------------------------------------------------
void
CSpriteSurface::BltHalf(POINT* pPoint, 
							   CDirectDrawSurface* SourceSurface, RECT*  pRect)
{
	//------------------------
	// Clipping
	//------------------------
	// TargetSurface�� ������ �Ѿ������ ���� üũ
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

	// �� Surface�� Lock�� �Ǵ�.	
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
// SourceSurface�� DarkBits��ŭ ��Ӱ� ���� *this�� ���
//----------------------------------------------------------------------
void
CSpriteSurface::BltDarkness(POINT* pPoint, 
							   CDirectDrawSurface* SourceSurface, RECT*  pRect,
							   BYTE DarkBits)
{
	//------------------------
	// Clipping
	//------------------------
	// TargetSurface�� ������ �Ѿ������ ���� üũ
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

	// �� Surface�� Lock�� �Ǵ�.	
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
// SourceSurface�� DarkBits��ŭ ��Ӱ� ���� *this�� ���
//----------------------------------------------------------------------
void
CSpriteSurface::BltBrightness(POINT* pPoint, 
							   CDirectDrawSurface* SourceSurface, RECT*  pRect,
							   BYTE BrightBits)
{
	//------------------------
	// Clipping
	//------------------------
	// TargetSurface�� ������ �Ѿ������ ���� üũ
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

	// �� Surface�� Lock�� �Ǵ�.	
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
// SourceSurface�� Filter�� �����ؼ�
// ���� Surface�� �� ��(R,G,B)���� 
// SourceSurface�� �� ��(R,G,B) ��ŭ�� ���־ ��Ӱ� �����.
//----------------------------------------------------------------------
void
CSpriteSurface::BltDarknessFilter(POINT* pPoint, 
							   CDirectDrawSurface* SourceSurface, RECT*  pRect,
							   WORD TransColor)
{
	//------------------------
	// Clipping
	//------------------------
	// TargetSurface�� ������ �Ѿ������ ���� üũ
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

	// �� Surface�� Lock�� �Ǵ�.
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
			// �������� �ƴҶ� ���
			if (*lpTargetTemp != TransColor)
			{				
				filter = *lpSourceTemp & 0x1F;	// Blue��

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
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (point.x >= m_ClipRight || point.y >= m_ClipBottom)
		return;

	//WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	//WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSOM->GetWidth();
	rect.bottom = pSOM->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= point.x + pSOM->GetWidth();
	int yPlusSpriteHeight	= point.y + pSOM->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (point.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (point.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -point.x + m_ClipLeft;		
		point.x = m_ClipLeft;
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - point.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - point.y;										
				}								
			}

			// ���� + ������			
			//pSOM->BltClipWidth( (WORD*)m_ddsd.lpSurface, Pitch, color, &rect);
			pSOM->BltClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
				}				
			}

			// ���� Clip			
			//pSOM->BltClipLeft( (WORD*)m_ddsd.lpSurface, Pitch, color, &rect);
			pSOM->BltClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - point.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (point.x<<1));
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - point.y;										
				}								
			}	
			
			// ������ Clip			
			//pSOM->BltClipRight( (WORD*)m_ddsd.lpSurface, Pitch, color, &rect);
			pSOM->BltClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (point.x<<1));
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
				}
				
				// Height�� Clip
				//pSOM->BltClipHeight( (WORD*)m_ddsd.lpSurface, Pitch, color, &rect);
				pSOM->BltClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + point.y*Pitch + (point.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
					// Height�� Clip
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
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (point.x >= m_ClipRight || point.y >= m_ClipBottom)
		return;


	//WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	//WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSOM->GetWidth();
	rect.bottom = pSOM->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= point.x + pSOM->GetWidth();
	int yPlusSpriteHeight	= point.y + pSOM->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (point.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (point.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -point.x + m_ClipLeft;		
		point.x = m_ClipLeft;
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - point.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - point.y;										
				}								
			}

			// ���� + ������			
			//pSOM->BltOutlineClipWidth( (WORD*)m_ddsd.lpSurface, Pitch, color, &rect);
			pSOM->BltOutlineClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
				}				
			}

			// ���� Clip			
			//pSOM->BltOutlineClipLeft( (WORD*)m_ddsd.lpSurface, Pitch, color, &rect);
			pSOM->BltOutlineClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - point.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (point.x<<1));
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - point.y;										
				}								
			}	
			
			// ������ Clip			
			//pSOM->BltOutlineClipRight( (WORD*)m_ddsd.lpSurface, Pitch, color, &rect);
			pSOM->BltOutlineClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (point.x<<1));
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
				}
				
				// Height�� Clip
				//pSOM->BltOutlineClipHeight( (WORD*)m_ddsd.lpSurface, Pitch, color, &rect);
				pSOM->BltOutlineClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + point.y*Pitch + (point.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
					// Height�� Clip
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
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (point.x >= m_ClipRight || point.y >= m_ClipBottom)
		return;


	//WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	//WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSOM->GetWidth();
	rect.bottom = pSOM->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= point.x + pSOM->GetWidth();
	int yPlusSpriteHeight	= point.y + pSOM->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (point.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (point.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -point.x + m_ClipLeft;		
		point.x = m_ClipLeft;
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - point.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - point.y;										
				}								
			}

			// ���� + ������			
			//pSOM->BltClipWidth( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
			pSOM->BltDarknessClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, DarkBits, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
				}				
			}

			// ���� Clip			
			//pSOM->BltClipLeft( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
			pSOM->BltDarknessClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, DarkBits, &rect);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - point.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (point.x<<1));
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - point.y;										
				}								
			}	
			
			// ������ Clip			
			//pSOM->BltClipRight( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
			pSOM->BltDarknessClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, DarkBits, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (point.x<<1));
				rect.top = -point.y + m_ClipTop;
				point.y = m_ClipTop;

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
				}
				
				// Height�� Clip
				//pSOM->BltClipHeight( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, &rect);
				pSOM->BltDarknessClip( (WORD*)m_ddsd.lpSurface, m_ddsd.lPitch, color, DarkBits, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + point.y*m_ddsd.lPitch + (point.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - point.y;					
					// Height�� Clip
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
// m_pDDSurface�� pRect������ ��Ӱ� �Ѵ�. 
// DarkBits�� (0~5) bit������ 1, 1/2, 1/4, 1/8, 1/16, 1/32, 0�̴�.
//----------------------------------------------------------------------
// pRect�� ������ right, bottom�� ��ǥ�� �������� �ʴ´�.  -1�����̴�.
//----------------------------------------------------------------------
void	
CSpriteSurface::ChangeBrightnessBit(RECT*  pRect, BYTE DarkBits)
{
	// ClippingRect
	if (pRect->left < m_ClipLeft) pRect->left=m_ClipLeft;
	if ((WORD)pRect->right > m_ClipRight) pRect->right=m_ClipRight;

	if (pRect->top < m_ClipTop) pRect->top=m_ClipTop;
	if ((WORD)pRect->bottom > m_ClipBottom) pRect->bottom=m_ClipBottom;

	// ������ ����� ���
	if (pRect->left >= pRect->right || pRect->top >= pRect->bottom)
		return;


	QWORD	*lpTargetTemp;
	QWORD	*lpTargetSurface	= (QWORD*)m_ddsd.lpSurface;
	WORD	Pitch			= m_ddsd.lPitch;
	
	// ��� ���� ��ġ
	lpTargetSurface = (QWORD*)((BYTE*)lpTargetSurface + pRect->top*Pitch + (pRect->left<<1));

	register int i;
	register int j;

	int rectBottom = pRect->bottom;

	int width = pRect->right - pRect->left;
	int qTimes = width >> 2;

	// Target Surface�� ������ ������ ��Ӱ� �Ѵ�.
	/*
	switch ( width & 0x03 )
	{
		//------------------
		// 4����
		//------------------
		case 0 :			
			for (i=pRect->top; i<rectBottom; i++)
			{
				lpTargetTemp = lpTargetSurface;	

				// ������ ���
				for (j=0; j<qTimes; j++)
				{
					*lpTargetTemp++ = (*lpTargetTemp >> DarkBits) & s_qwMASK_SHIFT[DarkBits];					
				}

				// ���� ��
				lpTargetSurface = (QWORD*)((BYTE*)lpTargetSurface + Pitch);
			}
		break;

		//------------------
		// 1�� + 4����
		//------------------
		case 1 :
			for (i=pRect->top; i<rectBottom; i++)
			{
				lpTargetTemp = lpTargetSurface;	

				// ���� ���
				*((WORD*)lpTargetTemp) = 
					(*((WORD*)lpTargetTemp) >> DarkBits) & s_wMASK_SHIFT[DarkBits];
				
				lpTargetTemp = (QWORD*)((WORD*)lpTargetTemp + 1);

				// ������ ���
				for (j=0; j<qTimes; j++)
				{
					*lpTargetTemp++ = (*lpTargetTemp >> DarkBits) & s_qwMASK_SHIFT[DarkBits];
				}

				// ���� ��
				lpTargetSurface = (QWORD*)((BYTE*)lpTargetSurface + Pitch);
			}
		break;

		//------------------
		// 2�� + 4����
		//------------------
		case 2 :
			for (i=pRect->top; i<rectBottom; i++)
			{
				lpTargetTemp = lpTargetSurface;	

				// ���� ���
				*((DWORD*)lpTargetTemp) = (*((DWORD*)lpTargetTemp) >> DarkBits) & s_dwMASK_SHIFT[DarkBits];
				lpTargetTemp = (QWORD*)((DWORD*)lpTargetTemp + 1);

				// ������ ���
				for (j=0; j<qTimes; j++)
				{
					*lpTargetTemp++ = (*lpTargetTemp >> DarkBits) & s_qwMASK_SHIFT[DarkBits];
				}

				// ���� ��
				lpTargetSurface = (QWORD*)((BYTE*)lpTargetSurface + Pitch);
			}
		break;

		//------------------
		// 1�� + 2�� + 4����
		//------------------
		case 3 :
			for (i=pRect->top; i<rectBottom; i++)
			{
				lpTargetTemp = lpTargetSurface;	

				// ���� ���
				*((WORD*)lpTargetTemp) = 
					(*((WORD*)lpTargetTemp) >> DarkBits) & s_wMASK_SHIFT[DarkBits];
				lpTargetTemp = (QWORD*)((WORD*)lpTargetTemp + 1);

				// ���� ���
				*((DWORD*)lpTargetTemp) = (*((DWORD*)lpTargetTemp) >> DarkBits) & s_dwMASK_SHIFT[DarkBits];
				lpTargetTemp = (QWORD*)((DWORD*)lpTargetTemp + 1);

				// ������ ���
				for (j=0; j<qTimes; j++)
				{
					*lpTargetTemp++ = (*lpTargetTemp >> DarkBits) & s_qwMASK_SHIFT[DarkBits];
				}

				// ���� ��
				lpTargetSurface = (QWORD*)((BYTE*)lpTargetSurface + Pitch);
			}
		break;
	}
	*/

///*
	// �帧�� �ٲ۴�.. ����..~	
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
		// step��ŭ �����ʿ��� ����Ѵ�. 
		// 4-step��ŭ�� �� ����Ѵ�.
		int widthPlus = (width - width2)>>1;

		int newWidth = width2 - maxWave + step;
		int qTimes = newWidth >> 2;

		pSourceTemp = (QWORD*)((WORD*)lpTargetSurface + widthPlus + step);
		lpTargetTemp = (QWORD*)((WORD*)lpTargetSurface + widthPlus);
		
		switch (newWidth & 0x03)
		{
			//------------------
			// 4����
			//------------------
			case 0 :
				for (j=0; j<qTimes; j++)
				{
					*lpTargetTemp++ = *pSourceTemp++;
				}
			break;			

			//------------------
			// 1�� + 4����
			//------------------
			case 1 :
				// ���� ���
				*((WORD*)lpTargetTemp) = *((WORD*)pSourceTemp);
				
				lpTargetTemp = (QWORD*)((WORD*)lpTargetTemp + 1);
				pSourceTemp = (QWORD*)((WORD*)pSourceTemp + 1);

				// ������ ���
				for (j=0; j<qTimes; j++)
				{
					*lpTargetTemp++ = *pSourceTemp++;
				}
			break;

			//------------------
			// 2�� + 4����
			//------------------
			case 2 :
				// ���� ���
				*((DWORD*)lpTargetTemp) = *((DWORD*)pSourceTemp);
				lpTargetTemp = (QWORD*)((DWORD*)lpTargetTemp + 1);
				pSourceTemp = (QWORD*)((DWORD*)pSourceTemp + 1);

				// ������ ���
				for (j=0; j<qTimes; j++)
				{
					*lpTargetTemp++ = *pSourceTemp++;
				}
			break;

			//------------------
			// 1�� + 2�� + 4����
			//------------------
			case 3 :			
				// ���� ���
				*((WORD*)lpTargetTemp) = *((WORD*)pSourceTemp);
				lpTargetTemp = (QWORD*)((WORD*)lpTargetTemp + 1);
				pSourceTemp = (QWORD*)((WORD*)pSourceTemp + 1);

				// ���� ���
				*((DWORD*)lpTargetTemp) = *((DWORD*)pSourceTemp);
				lpTargetTemp = (QWORD*)((DWORD*)lpTargetTemp + 1);
				pSourceTemp = (QWORD*)((DWORD*)pSourceTemp + 1);

				// ������ ���
				for (j=0; j<qTimes; j++)
				{
					*lpTargetTemp++ = *pSourceTemp++;
				}
			break;
		}

		// ���� ��
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
// �ܻ�����(color) Surface�� pRect������
// alpha����ŭ alpha blending�Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltColorAlpha(RECT* pRect, WORD color, BYTE alpha2)
{
	// ClippingRect
	if (pRect->left < m_ClipLeft) pRect->left=m_ClipLeft;
	if ((WORD)pRect->right > m_ClipRight) pRect->right=m_ClipRight;

	if (pRect->top < m_ClipTop) pRect->top=m_ClipTop;
	if ((WORD)pRect->bottom > m_ClipBottom) pRect->bottom=m_ClipBottom;

	// ������ ����� ���
	if (pRect->left >= pRect->right || pRect->top >= pRect->bottom)
		return;

	QWORD	*lpTargetTemp;
	QWORD	*lpTargetSurface	= (QWORD*)m_ddsd.lpSurface;
	WORD	Pitch			= m_ddsd.lPitch;
	
	// ��� ���� ��ġ
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
	// Surface�� ������ ������ alpha blending				
	//-----------------------------------------------------
	switch ( width & 0x03 )
	{
		//-----------------------------------------------------
		// ��Ȯ�� 4�� ����� �Ǵ� ���..
		//-----------------------------------------------------
		case 0 :
			for (i=pRect->top; i<rectBottom; i++)
			{
				lpTargetTemp = lpTargetSurface;	

				int qTimesTemp = qTimes;

				//-----------------------------------
				// 4����...
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

				// ���� ��
				lpTargetSurface = (QWORD*)((BYTE*)lpTargetSurface + Pitch);
			}
		break;

		//-----------------------------------------------------
		// 1�� ���  4����...
		//-----------------------------------------------------
		case 1 :
			for (i=pRect->top; i<rectBottom; i++)
			{
				lpTargetTemp = lpTargetSurface;	

				//-----------------------------------
				// �� �� ���...
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
				// 4����..
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

				// ���� ��
				lpTargetSurface = (QWORD*)((BYTE*)lpTargetSurface + Pitch);
			}
		break;

		//-----------------------------------------------------
		// 2�����.. 4����...
		//-----------------------------------------------------
		case 2 :
			for (i=pRect->top; i<rectBottom; i++)
			{
				lpTargetTemp = lpTargetSurface;	
			
				//-----------------------------------
				// �� �� ���..
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
				// 4����..
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

				// ���� ��
				lpTargetSurface = (QWORD*)((BYTE*)lpTargetSurface + Pitch);
			}
		break;

		//-----------------------------------------------------
		// 1�� ��� 2�����.. 4����...
		//-----------------------------------------------------
		case 3 :
			for (i=pRect->top; i<rectBottom; i++)
			{
				lpTargetTemp = lpTargetSurface;	

				//-----------------------------------
				// �� �� ���...
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
				// �� �� ���..
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
				// 4����..
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

				// ���� ��
				lpTargetSurface = (QWORD*)((BYTE*)lpTargetSurface + Pitch);
			}
		break;
	}
}

//----------------------------------------------------------------------
// Blt Sprite
//----------------------------------------------------------------------
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSprite(POINT* pPoint, CSprite* pSprite)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = {pPoint->x, pPoint->y};

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;			
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;

	// Lock
	//LockW(lpSurface, Pitch);

	// ��� ���� ��ġ			
	lpSurface = (WORD*)((BYTE*)lpSurface + pPoint->y*Pitch + (pPoint->x<<1));

	// ���
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
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	
	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltHalfClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltHalfClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltHalfClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltHalfClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteAlpha(POINT* pPoint, CSprite* pSprite, BYTE alpha)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	
	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltAlphaClipWidth(lpSurface, Pitch, &rect, alpha);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltAlphaClipLeft(lpSurface, Pitch, &rect, alpha);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltAlphaClipRight(lpSurface, Pitch, &rect, alpha);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltAlphaClipHeight(lpSurface, Pitch, &rect, alpha);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteColor(POINT* pPoint, CSprite* pSprite, BYTE rgb)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltColorClipWidth(lpSurface, Pitch, &rect, rgb);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltColorClipLeft(lpSurface, Pitch, &rect, rgb);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltColorClipRight(lpSurface, Pitch, &rect, rgb);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltColorClipHeight(lpSurface, Pitch, &rect, rgb);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteColorSet(POINT* pPoint, CSprite* pSprite, WORD colorSet)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltColorSetClipWidth(lpSurface, Pitch, &rect, colorSet);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltColorSetClipLeft(lpSurface, Pitch, &rect, colorSet);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltColorSetClipRight(lpSurface, Pitch, &rect, colorSet);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltColorSetClipHeight(lpSurface, Pitch, &rect, colorSet);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteDarkness(POINT* pPoint, CSprite* pSprite, BYTE DarkBits)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltDarknessClipWidth(lpSurface, Pitch, &rect, DarkBits);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltDarknessClipLeft(lpSurface, Pitch, &rect, DarkBits);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltDarknessClipRight(lpSurface, Pitch, &rect, DarkBits);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltDarknessClipHeight(lpSurface, Pitch, &rect, DarkBits);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteEffect(POINT* pPoint, CSprite* pSprite)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltEffectClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltEffectClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltEffectClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
				pSprite->BltEffectClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpritePalEffect(POINT* pPoint, CSpritePal* pSprite, MPalette &pal)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;


	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	
	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltEffectClipWidth(lpSurface, Pitch, &rect, pal);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltEffectClipLeft(lpSurface, Pitch, &rect, pal);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltEffectClipRight(lpSurface, Pitch, &rect, pal);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
				pSprite->BltEffectClipHeight(lpSurface, Pitch, &rect, pal);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteScale(POINT* pPoint, CSprite* pSprite, BYTE scale)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	
	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->BltScaleClipWidth(lpSurface, Pitch, &rect, scale);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->BltScaleClipLeft(lpSurface, Pitch, &rect, scale);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
			pSprite->BltScaleClipRight(lpSurface, Pitch, &rect, scale);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
				pSprite->BltScaleClipHeight(lpSurface, Pitch, &rect, scale);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltAlphaSprite(POINT* pPoint, CAlphaSprite* pSprite)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->BltClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->BltClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
			pSprite->BltClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
				pSprite->BltClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltAlphaSpritePal(POINT* pPoint, CAlphaSpritePal* pSprite, MPalette &pal)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;
	

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->BltClipWidth(lpSurface, Pitch, &rect, pal);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->BltClipLeft(lpSurface, Pitch, &rect, pal);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
			pSprite->BltClipRight(lpSurface, Pitch, &rect, pal);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
				pSprite->BltClipHeight(lpSurface, Pitch, &rect, pal);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltAlphaSpriteAlpha(POINT* pPoint, CAlphaSprite* pSprite, BYTE alpha)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->BltAlphaClipWidth(lpSurface, Pitch, &rect, alpha);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->BltAlphaClipLeft(lpSurface, Pitch, &rect, alpha);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
			pSprite->BltAlphaClipRight(lpSurface, Pitch, &rect, alpha);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
				pSprite->BltAlphaClipHeight(lpSurface, Pitch, &rect, alpha);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltAlphaSpritePalAlpha(POINT* pPoint, CAlphaSpritePal* pSprite, BYTE alpha, MPalette &pal)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;


	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	


	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->BltAlphaClipWidth(lpSurface, Pitch, &rect, alpha, pal);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->BltAlphaClipLeft(lpSurface, Pitch, &rect, alpha, pal);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
			pSprite->BltAlphaClipRight(lpSurface, Pitch, &rect, alpha, pal);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));			
				pSprite->BltAlphaClipHeight(lpSurface, Pitch, &rect, alpha, pal);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltAlphaSprite4444(POINT* pPoint, CAlphaSprite* pSprite)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->Blt4444ClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->Blt4444ClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->Blt4444ClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->Blt4444ClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltAlphaSpritePal4444(POINT* pPoint, CAlphaSpritePal* pSprite, MPalette &pal)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;


	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->Blt4444ClipWidth(lpSurface, Pitch, &rect, pal);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));			
			pSprite->Blt4444ClipLeft(lpSurface, Pitch, &rect, pal);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->Blt4444ClipRight(lpSurface, Pitch, &rect, pal);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->Blt4444ClipHeight(lpSurface, Pitch, &rect, pal);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�. ������ �κ��� �˰� ĥ�Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltAlphaSprite4444NotTrans(POINT* pPoint, CAlphaSprite* pSprite)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->Blt4444NotTransClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->Blt4444NotTransClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->Blt4444NotTransClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->Blt4444NotTransClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�. ������ �κ��� �˰� ĥ�Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltAlphaSpritePal4444NotTrans(POINT* pPoint, CAlphaSpritePal* pSprite, MPalette &pal)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->Blt4444NotTransClipWidth(lpSurface, Pitch, &rect, pal);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->Blt4444NotTransClipLeft(lpSurface, Pitch, &rect, pal);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->Blt4444NotTransClipRight(lpSurface, Pitch, &rect, pal);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->Blt4444NotTransClipHeight(lpSurface, Pitch, &rect, pal);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�. ������ �κ��� �˰� ĥ�Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltAlphaSprite4444SmallNotTrans(POINT* pPoint, CAlphaSprite* pSprite, BYTE shift)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth() >> shift;
	rect.bottom = pSprite->GetHeight() >> shift;


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + (pSprite->GetWidth() >> shift);
	int yPlusSpriteHeight	= pt.y + (pSprite->GetHeight() >> shift);

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -(pt.x << shift);		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444SmallClipWidth(lpSurface, Pitch, &rect, shift);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444SmallClipLeft(lpSurface, Pitch, &rect, shift);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			//pSprite->Blt4444SmallClipRight(lpSurface, Pitch, &rect, shift);			
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -(pt.y << shift);

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//pSprite->Blt4444SmallClipHeight(lpSurface, Pitch, &rect, shift);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�. ������ �κ��� �˰� ĥ�Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltAlphaSpritePal4444SmallNotTrans(POINT* pPoint, CAlphaSpritePal* pSprite, BYTE shift, MPalette &pal)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;


	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth() >> shift;
	rect.bottom = pSprite->GetHeight() >> shift;


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + (pSprite->GetWidth() >> shift);
	int yPlusSpriteHeight	= pt.y + (pSprite->GetHeight() >> shift);

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -(pt.x << shift);		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444SmallClipWidth(lpSurface, Pitch, &rect, shift);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444SmallClipLeft(lpSurface, Pitch, &rect, shift);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			//pSprite->Blt4444SmallClipRight(lpSurface, Pitch, &rect, shift);			
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -(pt.y << shift);

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//pSprite->Blt4444SmallClipHeight(lpSurface, Pitch, &rect, shift);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltIndexSprite(POINT* pPoint, CIndexSprite* pSprite)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltIndexSpriteDarkness(POINT* pPoint, CIndexSprite* pSprite, BYTE DarkBits)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltDarknessClipWidth(lpSurface, Pitch, &rect, DarkBits);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltDarknessClipLeft(lpSurface, Pitch, &rect, DarkBits);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltDarknessClipRight(lpSurface, Pitch, &rect, DarkBits);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltDarknessClipHeight(lpSurface, Pitch, &rect, DarkBits);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltIndexSpriteAlpha(POINT* pPoint, CIndexSprite* pSprite, BYTE alpha)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltAlphaClipWidth(lpSurface, Pitch, &rect, alpha);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltAlphaClipLeft(lpSurface, Pitch, &rect, alpha);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltAlphaClipRight(lpSurface, Pitch, &rect, alpha);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltAlphaClipHeight(lpSurface, Pitch, &rect, alpha);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltIndexSpriteColor(POINT* pPoint, CIndexSprite* pSprite, BYTE rgb)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltColorClipWidth(lpSurface, Pitch, &rect, rgb);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltColorClipLeft(lpSurface, Pitch, &rect, rgb);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltColorClipRight(lpSurface, Pitch, &rect, rgb);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltColorClipHeight(lpSurface, Pitch, &rect, rgb);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltIndexSpriteColorSet(POINT* pPoint, CIndexSprite* pSprite, WORD colorSet)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltColorSetClipWidth(lpSurface, Pitch, &rect, colorSet);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltColorSetClipLeft(lpSurface, Pitch, &rect, colorSet);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltColorSetClipRight(lpSurface, Pitch, &rect, colorSet);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltColorSetClipHeight(lpSurface, Pitch, &rect, colorSet);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltIndexSpriteEffect(POINT* pPoint, CIndexSprite* pSprite)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltEffectClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltEffectClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltEffectClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltEffectClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// ���� Clipping�� �������� ����
//----------------------------------------------------------------------
/*
void	
CSpriteSurface::BltSpriteAlphaFilter(POINT* pPoint, CSprite* pSprite, CFilter* pFilter)
{
	WORD*	lpSurface;
	WORD	Pitch;

	// Lock
	LockW(lpSurface, Pitch);

	// ��� ���� ��ġ			
	//lpSurface = (WORD*)((BYTE*)lpSurface + pPoint->y*Pitch + (pPoint->x<<1));
	lpSurface = (WORD*)((BYTE*)lpSurface + pPoint->y*Pitch + (pPoint->x<<1));
	pSprite->BltAlphaFilter(lpSurface, Pitch, pFilter);

	Unlock();
}
*/


//----------------------------------------------------------------------
// Blt SpriteAlphaFilter
//----------------------------------------------------------------------
// pPoint�� pSprite�� ����Ѵ�.
// Filter�� CSprite::SetFilter(...)�� ���ؼ� �����ȴ�.
//
// Filter�� Sprite�� ��ġ�� �ʴ� ���...
// BltSprite�� ȣ���ϵ��� �Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteAlphaFilter(POINT* pPoint, CSprite* pSprite)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit() || CSprite::GetFilter()->IsNotInit())
		return;

	//-------------------------------------------------------------
	// Filter�� Sprite�� ��ġ�� �ʴ� ���...
	// BltSprite�� ȣ���ϵ��� �Ѵ�.
	//-------------------------------------------------------------
	if (!pSprite->IsIntersectFilter())
	{
		BltSprite( pPoint, pSprite );
		return;
	}

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltAlphaFilterClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltAlphaFilterClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltAlphaFilterClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltAlphaFilterClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
// Filter�� CSprite::SetFilter(...)�� ���ؼ� �����ȴ�.
//
// Filter�� Sprite�� ��ġ�� �ʴ� ���...
// BltSpriteDarkness�� ȣ���ϵ��� �Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteAlphaFilterDarkness(POINT* pPoint, CSprite* pSprite, BYTE DarkBits)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit() || CSprite::GetFilter()->IsNotInit())
		return;

	//-------------------------------------------------------------
	// Filter�� Sprite�� ��ġ�� �ʴ� ���...
	// BltSprite�� ȣ���ϵ��� �Ѵ�.
	//-------------------------------------------------------------
	if (!pSprite->IsIntersectFilter())
	{
		BltSpriteDarkness( pPoint, pSprite, DarkBits );
		return;
	}

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltAlphaFilterDarknessClipWidth(lpSurface, Pitch, &rect, DarkBits);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltAlphaFilterDarknessClipLeft(lpSurface, Pitch, &rect, DarkBits);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltAlphaFilterDarknessClipRight(lpSurface, Pitch, &rect, DarkBits);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltAlphaFilterDarknessClipHeight(lpSurface, Pitch, &rect, DarkBits);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�. ������ �κ��� �˰� ĥ�Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteAlpha4444NotTrans(POINT* pPoint, CSprite* pSprite, BYTE alpha)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444NotTransClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444NotTransClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			//pSprite->Blt4444NotTransClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//pSprite->Blt4444NotTransClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�. ������ �κ��� �˰� ĥ�Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteAlpha4444SmallNotTrans(POINT* pPoint, CSprite* pSprite, BYTE alpha, BYTE shift)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth() >> shift;
	rect.bottom = pSprite->GetHeight() >> shift;


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + (pSprite->GetWidth() >> shift);
	int yPlusSpriteHeight	= pt.y + (pSprite->GetHeight() >> shift);

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -(pt.x << shift);		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444SmallClipWidth(lpSurface, Pitch, &rect, shift);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444SmallClipLeft(lpSurface, Pitch, &rect, shift);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			//pSprite->Blt4444SmallClipRight(lpSurface, Pitch, &rect, shift);			
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -(pt.y << shift);

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//pSprite->Blt4444SmallClipHeight(lpSurface, Pitch, &rect, shift);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�. ������ �κ��� �˰� ĥ�Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSprite1555NotTrans(POINT* pPoint, CSprite* pSprite)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444NotTransClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444NotTransClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			//pSprite->Blt4444NotTransClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//pSprite->Blt4444NotTransClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�. ������ �κ��� �˰� ĥ�Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpritePal1555NotTrans(POINT* pPoint, CSpritePal* pSprite, MPalette &pal)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;


	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444NotTransClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444NotTransClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			//pSprite->Blt4444NotTransClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//pSprite->Blt4444NotTransClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�. ������ �κ��� �˰� ĥ�Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSprite1555SmallNotTrans(POINT* pPoint, CSprite* pSprite, BYTE shift)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth() >> shift;
	rect.bottom = pSprite->GetHeight() >> shift;


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + (pSprite->GetWidth() >> shift);
	int yPlusSpriteHeight	= pt.y + (pSprite->GetHeight() >> shift);

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -(pt.x << shift);		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444SmallClipWidth(lpSurface, Pitch, &rect, shift);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444SmallClipLeft(lpSurface, Pitch, &rect, shift);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			//pSprite->Blt4444SmallClipRight(lpSurface, Pitch, &rect, shift);			
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -(pt.y << shift);

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//pSprite->Blt4444SmallClipHeight(lpSurface, Pitch, &rect, shift);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�. ������ �κ��� �˰� ĥ�Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpritePal1555SmallNotTrans(POINT* pPoint, CSpritePal* pSprite, BYTE shift, MPalette &pal)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;


	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth() >> shift;
	rect.bottom = pSprite->GetHeight() >> shift;


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + (pSprite->GetWidth() >> shift);
	int yPlusSpriteHeight	= pt.y + (pSprite->GetHeight() >> shift);

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -(pt.x << shift);		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444SmallClipWidth(lpSurface, Pitch, &rect, shift);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->Blt4444SmallClipLeft(lpSurface, Pitch, &rect, shift);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			//pSprite->Blt4444SmallClipRight(lpSurface, Pitch, &rect, shift);			
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -(pt.y << shift);

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//pSprite->Blt4444SmallClipHeight(lpSurface, Pitch, &rect, shift);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
// Filter�� CSprite::SetFilter(...)�� ���ؼ� �����ȴ�.
//
// Filter�� Sprite�� ��ġ�� �ʴ� ���...
// BltSprite�� ȣ���ϵ��� �Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltSpriteDarkerFilter(POINT* pPoint, CSprite* pSprite)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit() || CSprite::GetFilter()->IsNotInit())
		return;

	//-------------------------------------------------------------
	// Filter�� Sprite�� ��ġ�� �ʴ� ���...
	// BltSprite�� ȣ���ϵ��� �Ѵ�.
	//-------------------------------------------------------------
	// Darker�� Tile������ ����� �����̹Ƿ�..
	// �ϴ���... 
	/*
	if (!pSprite->IsIntersectFilter())
	{
		BltSprite( pPoint, pSprite );
		return;
	}
	*/

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltDarkerFilterClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltDarkerFilterClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltDarkerFilterClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltDarkerFilterClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltShadowSprite(POINT* pPoint, CShadowSprite* pSprite)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltClipWidth(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltClipLeft(lpSurface, Pitch, &rect);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltClipRight(lpSurface, Pitch, &rect);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltClipHeight(lpSurface, Pitch, &rect);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltShadowSpriteDarkness(POINT* pPoint, CShadowSprite* pSprite, BYTE DarkBits)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltDarknessClipWidth(lpSurface, Pitch, &rect, DarkBits);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltDarknessClipLeft(lpSurface, Pitch, &rect, DarkBits);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltDarknessClipRight(lpSurface, Pitch, &rect, DarkBits);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltDarknessClipHeight(lpSurface, Pitch, &rect, DarkBits);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltShadowSpriteSmall(POINT* pPoint, CShadowSprite* pSprite, BYTE shift)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth() >> shift;
	rect.bottom = pSprite->GetHeight() >> shift;


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + (pSprite->GetWidth() >> shift);
	int yPlusSpriteHeight	= pt.y + (pSprite->GetHeight() >> shift);

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -(pt.x << shift);		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltSmallClipWidth(lpSurface, Pitch, &rect, shift);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltSmallClipLeft(lpSurface, Pitch, &rect, shift);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltSmallClipRight(lpSurface, Pitch, &rect, shift);			
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -(pt.y << shift);

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltSmallClipHeight(lpSurface, Pitch, &rect, shift);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltShadowSpriteSmall4444(POINT* pPoint, CShadowSprite* pSprite, WORD pixel, BYTE shift)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth() >> shift;
	rect.bottom = pSprite->GetHeight() >> shift;


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + (pSprite->GetWidth() >> shift);
	int yPlusSpriteHeight	= pt.y + (pSprite->GetHeight() >> shift);

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -(pt.x << shift);		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->BltSmall4444ClipWidth(lpSurface, Pitch, &rect, pixel, shift);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			//pSprite->BltSmall4444ClipLeft(lpSurface, Pitch, &rect, pixel, shift);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
//			pSprite->BltSmall4444ClipRight(lpSurface, Pitch, &rect, pixel, shift);			
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -(pt.y << shift);

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
//				pSprite->BltSmall4444ClipHeight(lpSurface, Pitch, &rect, pixel, shift);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltShadowSprite4444(POINT* pPoint, CShadowSprite* pSprite, WORD pixel)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->Blt4444ClipWidth(lpSurface, Pitch, &rect, pixel);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->Blt4444ClipLeft(lpSurface, Pitch, &rect, pixel);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->Blt4444ClipRight(lpSurface, Pitch, &rect, pixel);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->Blt4444ClipHeight(lpSurface, Pitch, &rect, pixel);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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
// source --> dest �� pixels��ŭ ������ ����� �Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyHalf(WORD* pDest, WORD* pSource, WORD pixels)
{
	QWORD	*qpDest		= (QWORD*)pDest, 
			*qpSource	= (QWORD*)pSource;


	register int qTimes = pixels >> 2;	// pixels / 4

	// ������
	switch ( pixels & 0x03 )	// pixels % 4
	{
		//------------------
		// 4����
		//------------------
		case 0 :			
			// ������ ���
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
		// 1�� + 4����
		//------------------
		case 1 :
			// ���� ���
			*(WORD*)qpDest = 
					((*(WORD*)qpDest >> 1) & CDirectDraw::s_wMASK_SHIFT[1]) +
					((*(WORD*)qpSource >> 1) & CDirectDraw::s_wMASK_SHIFT[1]);
				
			qpDest = (QWORD*)((WORD*)qpDest + 1);
			qpSource = (QWORD*)((WORD*)qpSource + 1);

			// ������ ���
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
		// 2�� + 4����
		//------------------
		case 2 :
			// ���� ���
			*(DWORD*)qpDest = 
					((*(DWORD*)qpDest >> 1) & CDirectDraw::s_dwMASK_SHIFT[1]) +
					((*(DWORD*)qpSource >> 1) & CDirectDraw::s_dwMASK_SHIFT[1]);
				
			qpDest = (QWORD*)((DWORD*)qpDest + 1);
			qpSource = (QWORD*)((DWORD*)qpSource + 1);


			// ������ ���
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
		// 1�� + 2�� + 4����
		//------------------
		case 3 :
			// ���� ���
			*(WORD*)qpDest = 
					((*(WORD*)qpDest >> 1) & CDirectDraw::s_wMASK_SHIFT[1]) +
					((*(WORD*)qpSource >> 1) & CDirectDraw::s_wMASK_SHIFT[1]);
				
			qpDest = (QWORD*)((WORD*)qpDest + 1);
			qpSource = (QWORD*)((WORD*)qpSource + 1);


			// ���� ���
			*(DWORD*)qpDest = 
					((*(DWORD*)qpDest >> 1) & CDirectDraw::s_dwMASK_SHIFT[1]) +
					((*(DWORD*)qpSource >> 1) & CDirectDraw::s_dwMASK_SHIFT[1]);
				
			qpDest = (QWORD*)((DWORD*)qpDest + 1);
			qpSource = (QWORD*)((DWORD*)qpSource + 1);

			// ������ ���
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
// source --> dest �� pixels��ŭ s_Value1������ ����� �Ѵ�.
//
// s_Value1 = 1~32
//
//----------------------------------------------------------------------
/*
WORD
CSpriteSurface::memcpyAlpha1Pixel(WORD pDest, WORD pSource)
{
	WORD sr,sg,sb, dr,dg,db;

	// ���� ���
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
// source --> dest �� pixels��ŭ s_Value1������ ����� �Ѵ�.
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
//	// ���� ���
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
		// 4����
		//------------------
		case 0 :			
			// ������ ���
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
		// 1�� + 4����
		//------------------
		case 1 :
			// �� �� ���
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
			

			// ������ ���
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
		// 2�� + 4����
		//------------------
		case 2 :
			// ���� ���
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


			// ������ ���
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
		// 1�� + 2�� + 4����
		//------------------
		case 3 :
			// ���� ���
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
			// ���� ���		
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
			

			// ���� ���
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

			// ������ ���
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
// source --> dest �� pixels��ŭ Ư���� Color��(s_Value1)���� ����� �Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyColor(WORD* pDest, WORD* pSource, WORD pixels)
{
	QWORD	*qpDest		= (QWORD*)pDest, 
			*qpSource	= (QWORD*)pSource;


	register int qTimes = pixels >> 2;	// pixels / 4

	// rgb�� �� �ϳ��� ����Ѵ�.
	switch ( pixels & 0x03 )	// pixels % 4
	{
		//------------------
		// 4����
		//------------------
		case 0 :			
			// ������ ���
			while (qTimes--)
			{
				*qpDest = (*qpSource & CDirectDraw::s_qwMASK_RGB[s_Value1]);

				*qpDest++;
				*qpSource++;
			}
		break;

		//------------------
		// 1�� + 4����
		//------------------
		case 1 :
			// ���� ���
			*(WORD*)qpDest = *(WORD*)qpSource & CDirectDraw::s_wMASK_RGB[s_Value1];
				
			qpDest = (QWORD*)((WORD*)qpDest + 1);
			qpSource = (QWORD*)((WORD*)qpSource + 1);

			// ������ ���
			while (qTimes--)
			{
				*qpDest = (*qpSource & CDirectDraw::s_qwMASK_RGB[s_Value1]);

				*qpDest++;
				*qpSource++;
			}
			
		break;

		//------------------
		// 2�� + 4����
		//------------------
		case 2 :
			// ���� ���
			*(DWORD*)qpDest = *(DWORD*)qpSource & CDirectDraw::s_dwMASK_RGB[s_Value1];
				
			qpDest = (QWORD*)((WORD*)qpDest + 1);
			qpSource = (QWORD*)((WORD*)qpSource + 1);

			// ������ ���
			while (qTimes--)
			{
				*qpDest = (*qpSource & CDirectDraw::s_qwMASK_RGB[s_Value1]);

				*qpDest++;
				*qpSource++;
			}
		break;

		//------------------
		// 1�� + 2�� + 4����
		//------------------
		case 3 :
			// ���� ���
			*(WORD*)qpDest = *(WORD*)qpSource & CDirectDraw::s_wMASK_RGB[s_Value1];
				
			qpDest = (QWORD*)((WORD*)qpDest + 1);
			qpSource = (QWORD*)((WORD*)qpSource + 1);

			// ���� ���
			*(DWORD*)qpDest = *(DWORD*)qpSource & CDirectDraw::s_dwMASK_RGB[s_Value1];

			qpDest = (QWORD*)((WORD*)qpDest + 1);
			qpSource = (QWORD*)((WORD*)qpSource + 1);

			// ������ ���
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
// source --> dest �� pixels��ŭ Scale(s_Value1)�� �ٲ㼭 
// ����Ѵ�. 
//
// �� �Լ��� ���� ���ӿ��� ������� �����Ƿ� 
// �ӵ��� �Ű��� �Ƚᵵ �� ���̴�. - -;;;
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyScale(WORD* pDest, WORD destPitch, WORD* pSource, WORD pixels)
{
	WORD*	pDestTemp, *pDestTemp2;

	register int n=pixels,i,j;

	// n���� ���� ����Ѵ�.
	while (n--)
	{
		pDestTemp = pDest;
		// �� ���� ���ؼ� scale*scale�� ũ��� ����Ѵ�.

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
// source --> dest �� pixels��ŭ Ư���� Darkness��(s_Value1)��ŭ
// ��Ӱ� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyDarkness(WORD* pDest, WORD* pSource, WORD pixels)
{
	QWORD	*qpDest		= (QWORD*)pDest, 
			*qpSource	= (QWORD*)pSource;


	register int qTimes = pixels >> 2;	// pixels / 4

	// ��Ӱ� ����Ѵ�.
	switch ( pixels & 0x03 )	// pixels % 4
	{
		//------------------
		// 4����
		//------------------
		case 0 :			
			// ������ ���
			while (qTimes--)
			{					
				*qpDest = (*qpSource >> s_Value1) & CDirectDraw::s_qwMASK_SHIFT[s_Value1];

				*qpDest++;
				*qpSource++;
			}
		break;

		//------------------
		// 1�� + 4����
		//------------------
		case 1 :
			// ���� ���
			*(WORD*)qpDest = (*(WORD*)qpSource >> s_Value1) & CDirectDraw::s_wMASK_SHIFT[s_Value1];
				
			qpDest = (QWORD*)((WORD*)qpDest + 1);
			qpSource = (QWORD*)((WORD*)qpSource + 1);

			// ������ ���
			while (qTimes--)
			{
				*qpDest = (*qpSource >> s_Value1) & CDirectDraw::s_qwMASK_SHIFT[s_Value1];

				*qpDest++;
				*qpSource++;
			}
			
		break;

		//------------------
		// 2�� + 4����
		//------------------
		case 2 :
			// ���� ���
			*(DWORD*)qpDest = (*(DWORD*)qpSource >> s_Value1) & CDirectDraw::s_dwMASK_SHIFT[s_Value1];
				
			qpDest = (QWORD*)((DWORD*)qpDest + 1);
			qpSource = (QWORD*)((DWORD*)qpSource + 1);

			// ������ ���
			while (qTimes--)
			{
				*qpDest = (*qpSource >> s_Value1) & CDirectDraw::s_qwMASK_SHIFT[s_Value1];

				*qpDest++;
				*qpSource++;
			}
		break;

		//------------------
		// 1�� + 2�� + 4����
		//------------------
		case 3 :
			// ���� ���
			*(WORD*)qpDest = (*(WORD*)qpSource >> s_Value1) & CDirectDraw::s_wMASK_SHIFT[s_Value1];
				
			qpDest = (QWORD*)((WORD*)qpDest + 1);
			qpSource = (QWORD*)((WORD*)qpSource + 1);

			// ���� ���
			*(DWORD*)qpDest = (*(DWORD*)qpSource >> s_Value1) & CDirectDraw::s_dwMASK_SHIFT[s_Value1];

			qpDest = (QWORD*)((DWORD*)qpDest + 1);
			qpSource = (QWORD*)((DWORD*)qpSource + 1);

			// ������ ���
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
// source --> dest �� pixels��ŭ Ư���� Brightness��(s_Value1)��ŭ
// ��� ����Ѵ�.
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
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
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
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
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
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
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
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
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
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
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
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
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
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
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
		// ���� ���
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
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
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
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
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
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
//
// s_Value1�� ColorSet��ȣ�̴�.
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
// �� �������� �ܰ��� ���(������ ������ �� �� ���� ���)
// pixels�� 1�̻��̶�� �����Ѵ�.
//----------------------------------------------------------------------
void		
CSpriteSurface::memcpyEffectSimpleOutline(WORD* pDest, WORD* pSource, WORD pixels)
{
	// ù ��
	*pDest = *pSource;
	
	int pixels_1 = pixels-1;

	// �� �� - pixels�� 1�϶� ���ϴ°ź��� �׳� ��°� �� ������?
	*(pDest+pixels_1) = *(pSource+pixels_1);	
}

//----------------------------------------------------------------------
// Effect WipeOut
//----------------------------------------------------------------------
// s_Value1 : �� ����� ����? 
//				64 - ����
//				0 - �� ���
//
// �߽ɿ������� �������� ����������.
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
	
	// [1] drawPixels��ŭ ����ϰ�..
	// [2] skipPixels ��ŭ �ǳʶ��
	// [3] drawPixels2��ŭ ���
	
	//------------------------------------------------------------	
	// drawPixels��ŭ ���
	//------------------------------------------------------------	
	memcpy(pDest, pSource, (drawPixels<<1));
	pDest += drawPixels;
	pSource += drawPixels;

	//------------------------------------------------------------	
	// skipPixels��ŭ �ǳʶ�
	//------------------------------------------------------------	
	pDest += skipPixels;
	pSource += skipPixels;

	//------------------------------------------------------------	
	// drawPixels2��ŭ ���
	//------------------------------------------------------------	
	memcpy(pDest, pSource, (drawPixels2<<1));
	//pDest += drawPixels2;
	//pSource += drawPixels2;
}

//----------------------------------------------------------------------
// Effect Net
//----------------------------------------------------------------------
// s_Value1 : �ǳʶ�� ��
//
// �� �� ���.. s_Value1�� �� ����ŭ �ǳʶ��.
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
	// drawPixels��ŭ ���
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
// s_Value1 �� �󸶳� gray�Ǵ°�?(-_-;)�̴�..
// s_Value1���� 0~31.. 32�ΰ�?.. - -
// 0�̸� ���� gray
// 32�̸� ��������
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
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
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
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
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
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
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
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
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
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
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
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
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
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::memcpyPalEffectScreenAlpha(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
}

//----------------------------------------------------------------------
// Effect Copy - DodgeBurn
//----------------------------------------------------------------------
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
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
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
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
// source --> dest �� pixels��ŭ Ư��ȿ�� ó���� �Ѵ�.
//
// s_Value1�� ColorSet��ȣ�̴�.
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
// �� �������� �ܰ��� ���(������ ������ �� �� ���� ���)
// pixels�� 1�̻��̶�� �����Ѵ�.
//----------------------------------------------------------------------
void		
CSpriteSurface::memcpyPalEffectSimpleOutline(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	// ù ��
	*pDest = pal[*pSource];
	
	int pixels_1 = pixels-1;

	// �� �� - pixels�� 1�϶� ���ϴ°ź��� �׳� ��°� �� ������?
	*(pDest+pixels_1) = *(pSource+pixels_1);	
}

//----------------------------------------------------------------------
// Effect WipeOut
//----------------------------------------------------------------------
// s_Value1 : �� ����� ����? 
//				64 - ����
//				0 - �� ���
//
// �߽ɿ������� �������� ����������.
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
	
	// [1] drawPixels��ŭ ����ϰ�..
	// [2] skipPixels ��ŭ �ǳʶ��
	// [3] drawPixels2��ŭ ���
	
	//------------------------------------------------------------	
	// drawPixels��ŭ ���
	//------------------------------------------------------------	
	memcpy(pDest, pSource, (drawPixels<<1));
	pDest += drawPixels;
	pSource += drawPixels;

	//------------------------------------------------------------	
	// skipPixels��ŭ �ǳʶ�
	//------------------------------------------------------------	
	pDest += skipPixels;
	pSource += skipPixels;

	//------------------------------------------------------------	
	// drawPixels2��ŭ ���
	//------------------------------------------------------------	
	memcpy(pDest, pSource, (drawPixels2<<1));
	//pDest += drawPixels2;
	//pSource += drawPixels2;
}

//----------------------------------------------------------------------
// Effect Net
//----------------------------------------------------------------------
// s_Value1 : �ǳʶ�� ��
//
// �� �� ���.. s_Value1�� �� ����ŭ �ǳʶ��.
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
	// drawPixels��ŭ ���
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
// s_Value1 �� �󸶳� gray�Ǵ°�?(-_-;)�̴�..
// s_Value1���� 0~31.. 32�ΰ�?.. - -
// 0�̸� ���� gray
// 32�̸� ��������
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
// pPoint�� pSprite�� ����Ѵ�.
//----------------------------------------------------------------------
void	
CSpriteSurface::BltIndexSpriteBrightness(POINT* pPoint, CIndexSprite* pSprite, BYTE BrightBits)
{	
	// Sprite�� ���� �������� ���� ���..
	if (pSprite->IsNotInit())
		return;

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 1
	//-------------------------------------------------------------
	if (pPoint->x >= m_ClipRight || pPoint->y >= m_ClipBottom)
		return;

	POINT pt = *pPoint;
	

	WORD*	lpSurface	= (WORD*)m_ddsd.lpSurface;
	WORD	Pitch		= m_ddsd.lPitch;
	RECT	rect;

	// sprite������ ��� ����
	rect.left = 0;
	rect.top = 0;
	rect.right = pSprite->GetWidth();
	rect.bottom = pSprite->GetHeight();


	// Lock
	//LockW(lpSurface, Pitch);

	int xPlusSpriteWidth	= pt.x + pSprite->GetWidth();
	int yPlusSpriteHeight	= pt.y + pSprite->GetHeight();

	//-------------------------------------------------------------
	// ȭ�鿡 ������ �ʴ� ��� 2
	//-------------------------------------------------------------
	if (xPlusSpriteWidth <= m_ClipLeft || yPlusSpriteHeight <= m_ClipTop)
		return;

	//-------------------------------------------------------------
	// Clipping�� �ʿ��� �κп� ���� check
	//-------------------------------------------------------------
	bool	leftClip	= (pt.x < m_ClipLeft) && (xPlusSpriteWidth > m_ClipLeft);
	bool	rightClip	= (xPlusSpriteWidth > m_ClipRight);
	bool	topClip		= (pt.y < m_ClipTop) && (yPlusSpriteHeight > m_ClipTop);
	bool	bottomClip	= (yPlusSpriteHeight > m_ClipBottom);

	//------------------------------------------
	// ���� Clipping�� �ʿ��� ����..
	//------------------------------------------
	if (leftClip)
	{
		// �������� ����ŭ�� �����ؼ� ����ؾ� �Ѵ�.
		rect.left = -pt.x + m_ClipLeft;    
		//pt.x = m_ClipLeft;		
		
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;			

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}

			// ���� + ������			
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltBrightnessClipWidth(lpSurface, Pitch, &rect, BrightBits);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}				
			}

			// ���� Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (m_ClipLeft<<1));
			pSprite->BltBrightnessClipLeft(lpSurface, Pitch, &rect, BrightBits);
		}
	}
	//------------------------------------------
	// ���� Clipping�� �ʿ� ���� ����..
	//------------------------------------------
	else
	{
		//------------------------------------------
		// ������ Clipping�� �ʿ��� ����
		//------------------------------------------
		if (rightClip)
		{	
			// ���� ���̸� ���δ�.
			rect.right = m_ClipRight - pt.x;

			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{				
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
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
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{					
					rect.bottom = m_ClipBottom - pt.y;										
				}								
			}	
			
			// ������ Clip
			lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
			pSprite->BltBrightnessClipRight(lpSurface, Pitch, &rect, BrightBits);
		}
		//------------------------------------------
		// ������ Clipping�� �ʿ� ���� ����
		//------------------------------------------
		else
		{
			//------------------------------------------
			// ���� Clipping�� �ʿ��� ����
			//------------------------------------------
			if (topClip)
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + (pt.x<<1));
				rect.top = -pt.y + m_ClipTop;   				

				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
				}

				pt.y = m_ClipTop;
				
				// Height�� Clip
				lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				pSprite->BltBrightnessClipHeight(lpSurface, Pitch, &rect, BrightBits);
			}
			else
			{
				//lpSurface = (WORD*)((BYTE*)lpSurface + pt.y*Pitch + (pt.x<<1));
				//------------------------------------------
				// �Ʒ��� Clipping�� �ʿ��� ����
				//------------------------------------------
				if (bottomClip)
				{
					rect.bottom = m_ClipBottom - pt.y;					
					// Height�� Clip
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

//----------------------------------------------------------------------
// DirectX Compatibility Methods (SDL Backend Stubs)
//----------------------------------------------------------------------

bool CSpriteSurface::Restore()
{
#ifdef SPRITELIB_BACKEND_SDL
	// SDL backend: Surface doesn't need restoration
	return true;
#else
	return false;
#endif
}

void* CSpriteSurface::Lock(RECT* rect, DWORD* pitch)
{
#ifdef SPRITELIB_BACKEND_SDL
	(void)rect;
	if (pitch) {
		*pitch = m_width * 2; // 16-bit color
	}
	return spritectl_surface_lock(m_backend_surface);
#else
	(void)rect; (void)pitch;
	return NULL;
#endif
}

void CSpriteSurface::Unlock()
{
#ifdef SPRITELIB_BACKEND_SDL
	spritectl_surface_unlock(m_backend_surface);
#endif
}

int CSpriteSurface::GetSurfacePitch() const
{
#ifdef SPRITELIB_BACKEND_SDL
	return m_width * 2; // 16-bit color
#else
	return 0;
#endif
}

void* CSpriteSurface::GetSurfacePointer()
{
#ifdef SPRITELIB_BACKEND_SDL
	return spritectl_surface_lock(m_backend_surface);
#else
	return NULL;
#endif
}

int CSpriteSurface::GetWidth() const
{
#ifdef SPRITELIB_BACKEND_SDL
	return m_width;
#else
	return 0;
#endif
}

int CSpriteSurface::GetHeight() const
{
#ifdef SPRITELIB_BACKEND_SDL
	return m_height;
#else
	return 0;
#endif
}

bool CSpriteSurface::InitTextureSurface(int width, int height, void* pixels, void* pixelFormat)
{
#ifdef SPRITELIB_BACKEND_SDL
	(void)pixels;
	(void)pixelFormat;  // Store for future use if needed
	// Create or resize the surface
	if (m_backend_surface) {
		spritectl_surface_release(m_backend_surface);
	}
	m_backend_surface = spritectl_surface_create(width, height);
	if (!m_backend_surface) {
		return false;
	}
	m_width = width;
	m_height = height;
	return true;
#else
	(void)width; (void)height; (void)pixels; (void)pixelFormat;
	return false;
#endif
}
