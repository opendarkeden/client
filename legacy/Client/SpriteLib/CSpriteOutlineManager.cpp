//----------------------------------------------------------------------
// CSpriteOutlineManager.cpp
//----------------------------------------------------------------------
#include "client_PCH.h"
#include "CSpriteOutlineManager.h"
#include "CSprite.h"
#include "CAlphaSprite.h"
#include "CIndexSprite.h"

//----------------------------------------------------------------------
// 
// constructor/destructor
//
//----------------------------------------------------------------------
CSpriteOutlineManager::CSpriteOutlineManager()
{
	m_Width			= 0;
	m_Height		= 0;
	m_ppPixelInfo	= NULL;
	m_ppColorInfo	= NULL;

	Clear();
}

CSpriteOutlineManager::~CSpriteOutlineManager()
{
	ReleaseInfo();
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
CSpriteOutlineManager::Clear()
{
	// 등록된 sprite가 없게 한다.
	for (int i=0; i<MAX_SPRITE_MERGE; i++)
	{
		m_Type[i]		= SPRITETYPE_NULL;
		m_pSprite[i]	= NULL;		
	}

	m_nSprite = 0;

	m_rectOutline.left		= 999;
	m_rectOutline.right		= -999;
	m_rectOutline.top		= 999;
	m_rectOutline.bottom	= -999;

	m_pointOutput.x			= 0;
	m_pointOutput.y			= 0;

	// 외곽선 정보 관련
	ReleaseInfo();

}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void
CSpriteOutlineManager::InitInfo(int width, int height)
{
	// 일단 메모리 해제
	ReleaseInfo();

	// 길이가 없는 경우
	if (width==0 || height==0)
		return;

	m_Width = width;
	m_Height = height;

	// 메모리 잡기..
	m_ppPixelInfo = new BYTE* [m_Height];
	m_ppColorInfo = new WORD* [m_Height];

	for (int i=0; i<m_Height; i++)
	{
		m_ppPixelInfo[i] = new BYTE [m_Width];
		m_ppColorInfo[i] = new WORD [m_Width];

		// 일단 모든 점을 투명색으로 초기화한다.
		//for (int j=0; j<m_Width; j++)
		//{
		//	m_ppPixelInfo[i][j] = PIXELTYPE_TRANS;
		//}
		memset((void*)m_ppPixelInfo[i], PIXELTYPE_TRANS, m_Width);
	}
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void
CSpriteOutlineManager::ReleaseInfo()
{
	// m_ppPixelInfo제거
	if (m_ppPixelInfo != NULL)
	{		
		for (int i=0; i<m_Height; i++)
		{
			delete [] m_ppPixelInfo[i];
		}

		delete [] m_ppPixelInfo;
		m_ppPixelInfo	= NULL;
	}

	// m_ppColorInfo제거
	if (m_ppColorInfo != NULL)
	{				
		for (int i=0; i<m_Height; i++)
		{
			delete [] m_ppColorInfo[i];
		}

		delete [] m_ppColorInfo;
		m_ppColorInfo	= NULL;
	}

	m_Width		= 0;
	m_Height	= 0;

}

//----------------------------------------------------------------------
// Add Sprite( CSprite* )
//----------------------------------------------------------------------
void		
CSpriteOutlineManager::Add(int x, int y, CSprite* pSprite)
{
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	// MAX_SPRITE_MERGE을 넘지 않도록 한다.
	if (m_nSprite < MAX_SPRITE_MERGE)
	{
		m_Type[m_nSprite]		= SPRITETYPE_NORMAL;	
		m_Position[m_nSprite].x	= x;
		m_Position[m_nSprite].y	= y;
		m_pSprite[m_nSprite]	= (void*)pSprite;

		m_nSprite++;

		
		// 외곽선 정보의 크기를 결정한다.
		int temp;

		if (x < m_rectOutline.left)
			m_rectOutline.left = x;

		temp = x+pSprite->GetWidth();
		if (temp > m_rectOutline.right)
			m_rectOutline.right = temp;

		if (y < m_rectOutline.top)
			m_rectOutline.top = y;

		temp = y+pSprite->GetHeight();
		if (temp > m_rectOutline.bottom)
			m_rectOutline.bottom = temp;
	}
}

//----------------------------------------------------------------------
// Add Sprite( CIndexSprite* )
//----------------------------------------------------------------------
void		
CSpriteOutlineManager::Add(int x, int y, CIndexSprite* pSprite, WORD colorSet)
{
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	register int i;
	// MAX_SPRITE_MERGE을 넘지 않도록 한다.
	if (m_nSprite < MAX_SPRITE_MERGE)
	{
		m_Type[m_nSprite]		= SPRITETYPE_INDEX;	
		m_Position[m_nSprite].x	= x;
		m_Position[m_nSprite].y	= y;
		m_ChangeColorSet[m_nSprite] = colorSet;

		for(i = 0; i < 256; i++)
		{
			m_Value[m_nSprite][i]	= CIndexSprite::GetUsingColorSet( i );
		}
		m_pSprite[m_nSprite]	= (void*)pSprite;

		m_nSprite++;

		
		// 외곽선 정보의 크기를 결정한다.
		int temp;

		if (x < m_rectOutline.left)
			m_rectOutline.left = x;

		temp = x+pSprite->GetWidth();
		if (temp > m_rectOutline.right)
			m_rectOutline.right = temp;

		if (y < m_rectOutline.top)
			m_rectOutline.top = y;

		temp = y+pSprite->GetHeight();
		if (temp > m_rectOutline.bottom)
			m_rectOutline.bottom = temp;
	}
}

//----------------------------------------------------------------------
// Add Sprite( CAlphaSprite* )
//----------------------------------------------------------------------
void		
CSpriteOutlineManager::Add(int x, int y, CAlphaSprite* pSprite)
{
	// Sprite가 아직 생성되지 않은 경우..
	if (pSprite->IsNotInit())
		return;

	// MAX_SPRITE_MERGE을 넘지 않도록 한다.
	if (m_nSprite < MAX_SPRITE_MERGE)
	{
		m_Type[m_nSprite]		= SPRITETYPE_ALPHA;	
		m_Position[m_nSprite].x	= x;
		m_Position[m_nSprite].y	= y;
		m_pSprite[m_nSprite]	= (void*)pSprite;

		m_nSprite++;

		
		// 외곽선 정보의 크기를 결정한다.
		int temp;

		if (x < m_rectOutline.left)
			m_rectOutline.left = x;

		temp = x+pSprite->GetWidth();
		if (temp > m_rectOutline.right)
			m_rectOutline.right = temp;

		if (y < m_rectOutline.top)
			m_rectOutline.top = y;

		temp = y+pSprite->GetHeight();
		if (temp > m_rectOutline.bottom)
			m_rectOutline.bottom = temp;
	}
}

//----------------------------------------------------------------------
// Generate
//----------------------------------------------------------------------
// 외곽선에 대한 정보를 생성한다.
//----------------------------------------------------------------------
void		
CSpriteOutlineManager::Generate(int opt)
{	
	if (m_nSprite==0)
		return;

	register int i;
	register int j;

	//------------------------------------------------------
	// 외곽선 정보의 크기(m_rectOutline)에 따라 정보 생성.
	//------------------------------------------------------
	//
	// ex) (100,100) ~ (200,200) : 등록된 Sprite들의 외곽
	//
	// --> (0,0) ~ (100,100)     : (0,0)을 시작점으로.. 
	// --> (0,0) ~ (102,102)     : 외곽에 한줄씩 더 추가.
	//
	// 출력위치 : (100,100) - (1,1) = (99,99)
	//
	//------------------------------------------------------
	m_pointOutput.x		= m_rectOutline.left - 1;
	m_pointOutput.y		= m_rectOutline.top - 1;

	m_rectOutline.right -= m_rectOutline.left;
	m_rectOutline.bottom -= m_rectOutline.top;
	//m_rectOutline.left = 0;
	//m_rectOutline.top = 0;
	m_rectOutline.right += 1;	// +2	아래쪽에서 계산의 편의상 +1만 g음
	m_rectOutline.bottom += 1;	// +2
	
	// 정보 생성
	InitInfo( m_rectOutline.right+1, m_rectOutline.bottom+1 );


	//------------------------------------------------------
	//
	// 각 Sprite에 따라서 외곽선 정보를 변경시켜 준다.
	//
	//------------------------------------------------------
	for (i=0; i<m_nSprite; i++)
	{
		// 외곽선 정보에서 이 Sprite가 위치하는 position
		m_Position[i].x	-= m_rectOutline.left;
		m_Position[i].x	+= 1;
		m_Position[i].y	-= m_rectOutline.top;
		m_Position[i].y	+= 1;

		switch (m_Type[i])
		{
			case SPRITETYPE_NORMAL :
				MergeNormalSprite( i );
			break;

			case SPRITETYPE_INDEX :
				if (m_ChangeColorSet[i] < MAX_COLORSET)
				{
					MergeIndexSpriteColorSet( i );					
				}
				else
				{
					MergeIndexSprite( i );
				}
			break;

			case SPRITETYPE_ALPHA :
				MergeAlphaSprite( i );
			break;
		}		
	}

	//------------------------------------------------------
	//
	// 생성된 외곽선 정보로 -->  외곽선을 추출한다.
	//
	//
	// m_ppPixelInfo에 PIXELTYPE_OUTLINE을 추가한다.
	// 
	//------------------------------------------------------
	//------------------------------------------------------
	// 세로로 Scan
	//------------------------------------------------------
	BOOL bPreviousTrans;
	for (i=1; i<m_rectOutline.right; i++)
	{
		bPreviousTrans = TRUE;

		for (j=1; j<m_rectOutline.bottom; j++)
		{
			// 투명색인 경우
			if (m_ppPixelInfo[j][i] == PIXELTYPE_TRANS)
			{
				// 이전의 것이 투명색이 아닌 경우
				if (!bPreviousTrans)
				{
					m_ppPixelInfo[j][i] = PIXELTYPE_OUTLINE;
					bPreviousTrans = TRUE;
				}
			}
			// 투명색이 아닌 경우
			else
			{
				// 이전의 것이 투명색인 경우
				if (bPreviousTrans)
				{
					m_ppPixelInfo[j-1][i] = PIXELTYPE_OUTLINE;
					bPreviousTrans = FALSE;
				}
			}	
		}

		// 마지막 것이 투명색이 아닌 경우
		if (!bPreviousTrans)
		{
			m_ppPixelInfo[j][i] = PIXELTYPE_OUTLINE;
		}
	}
	
	//------------------------------------------------------
	// 가로로 Scan
	//------------------------------------------------------
	if(opt != GENERATE_EXCEPT_SIDE)
	{
		int	transCount;		// 투명색 개수
		int transIndex;
		int	colorCount;		// 색 개수
		int colorIndex;
		for (i=1; i<m_rectOutline.bottom; i++)
		{
			transCount		= 0;
			transIndex		= 0;
			colorCount		= 0;
			colorIndex		= 0;
			bPreviousTrans	= TRUE;

			for (j=1; j<m_rectOutline.right; j++)
			{
				// 투명색인 경우
				if (m_ppPixelInfo[i][j] == PIXELTYPE_TRANS)
				{
					// 이전의 것이 투명색이 아닌 경우
					if (!bPreviousTrans)
					{
						// 이전의 것이 outline이면..
						if (m_ppPixelInfo[i][j-1]==PIXELTYPE_OUTLINE)
						{
							// 투명색 개수를 저장할 위치
							transIndex		= j;
							bPreviousTrans = TRUE;
						}
						// 이전의 것이 outline이 아니면(색깔이면)..
						else
						{				
							if(opt != GENERATE_EXCEPT_RIGHT)
								m_ppPixelInfo[i][j] = PIXELTYPE_OUTLINE;
							continue;
						}										
					}
					transCount++;
				}
				// 투명색이 아닌 경우
				else
				{
					// 이전의 것이 투명색인 경우
					if (bPreviousTrans)
					{
						// 이미 세로scan에서 outline체크가 되어 있다면
						if (m_ppPixelInfo[i][j]==PIXELTYPE_OUTLINE)
						{
						}
						// 아니면..
						else
						{
							if(opt != GENERATE_EXCEPT_LEFT)
							{
								transCount --;
								m_ppPixelInfo[i][j-1] = PIXELTYPE_OUTLINE;						
							}
						}
						
						bPreviousTrans = FALSE;

						// 투명색 개수를 저장해둔다.
						//
						// (!) transCount가 200이상일 경우.. check
						//
						// 0이면 저장해서는 안된다.
						if (transCount > 0)
						{
							m_ppPixelInfo[i][transIndex] = transCount;
							transCount = 0;
						}					
					}				
				}	
			}

			// 마지막 것이 투명색인 경우
			if (bPreviousTrans)
			{
				// 뒤로는 모두 투명색이라는 의미.
				if (transCount > 0)
				{
					m_ppPixelInfo[i][transIndex] = PIXELTYPE_TRANSEND;
				}
			}
			// 마지막 것이 투명색이 아닌 경우
			else if (m_ppPixelInfo[i][j-1] != PIXELTYPE_OUTLINE)
			{
				if(opt != GENERATE_EXCEPT_RIGHT)
					m_ppPixelInfo[i][j] = PIXELTYPE_OUTLINE;
			}
		}
	}	
}

//----------------------------------------------------------------------
// Blt
//----------------------------------------------------------------------
// 외곽선을 화면에 출력한다. 
//
// pSurface에.. 외곽선은 color로 
//----------------------------------------------------------------------
void		
CSpriteOutlineManager::Blt(WORD* pSurface, WORD Pitch, WORD color)
{
	if (m_nSprite==0)
		return;

	// 좌표 설정
	pSurface = (WORD*)((BYTE*)pSurface + Pitch*m_pointOutput.y + (m_pointOutput.x<<1));

	register int i;
	register int j;

	WORD	*pSurfaceTemp;	

	int rectBottom = m_rectOutline.bottom;
	int rectRight = m_rectOutline.right;

	int info;

	// Clipping도 해야한다. T_T;;;
	for (i=0; i<=rectBottom; i++)
	{
		pSurfaceTemp = pSurface;

		//for (j=0; j<=m_rectOutline.right; j++)
		j = 0;
		BYTE* pPixelInfo = m_ppPixelInfo[i];
		while (j <= rectRight)
		{		
			info = *pPixelInfo;

			// 투명색만큼 건너뛴다.
			if (info < PIXELTYPE_MAXCOUNT)
			{				
				pSurfaceTemp += info;
				pPixelInfo += info;
				j += info;
			}

			// 외곽선인 경우 
			if (info == PIXELTYPE_OUTLINE)
			{
				*pSurfaceTemp = color;
			}
			// 색깔인 경우
			else if (info == PIXELTYPE_COLOR)
			{
				*pSurfaceTemp = m_ppColorInfo[i][j];
			}
			// 끝까지 투명색인 경우
			else if (info==PIXELTYPE_TRANSEND)
			{
				break;
			}

			pSurfaceTemp++;
			pPixelInfo++;
			j++;
		}

		// 다음 줄
		pSurface = (WORD*)((BYTE*)pSurface + Pitch);
	}
}

//----------------------------------------------------------------------
// Blt Clip
//----------------------------------------------------------------------
// 외곽선을 화면에 출력한다. 
//
// pSurface에.. 외곽선은 color로 
//----------------------------------------------------------------------
void		
CSpriteOutlineManager::BltClip(WORD* pSurface, WORD Pitch, WORD color, RECT* pRect)
{
	if (m_nSprite==0)
		return;

	// 좌표 설정
	pSurface = (WORD*)((BYTE*)pSurface + Pitch*m_pointOutput.y + (m_pointOutput.x<<1));

	pSurface = (WORD*)((BYTE*)pSurface + Pitch*pRect->top + (pRect->left<<1));

	register int i;
	register int j;

	WORD	*pSurfaceTemp;	

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;

	// Clipping도 해야한다. T_T;;;
	for (i=pRect->top; i<rectBottom; i++)
	{
		pSurfaceTemp = pSurface;

		//for (j=0; j<=m_rectOutline.right; j++)
		j = rectLeft;
		while (j < rectRight)
		{		
			// 투명색만큼 건너뛴다.
			if (m_ppPixelInfo[i][j] < PIXELTYPE_MAXCOUNT)
			{				
				pSurfaceTemp += m_ppPixelInfo[i][j];
				j += m_ppPixelInfo[i][j];

				continue;
			}

			// 외곽선인 경우 
			if (m_ppPixelInfo[i][j] == PIXELTYPE_OUTLINE)
			{
				*pSurfaceTemp = color;
			}
			// 색깔인 경우
			else if (m_ppPixelInfo[i][j] == PIXELTYPE_COLOR)
			{
				*pSurfaceTemp = m_ppColorInfo[i][j];
			}
			// 끝까지 투명색인 경우
			else if (m_ppPixelInfo[i][j]==PIXELTYPE_TRANSEND)
			{
				break;
			}



			pSurfaceTemp++;
			j++;
		}

		// 다음 줄
		pSurface = (WORD*)((BYTE*)pSurface + Pitch);
	}
}

//----------------------------------------------------------------------
// BltOutline
//----------------------------------------------------------------------
// 외곽선을 화면에 출력한다. 
//
// pSurface에.. 외곽선은 color로 
//----------------------------------------------------------------------
void		
CSpriteOutlineManager::BltOutline(WORD* pSurface, WORD Pitch, WORD color)
{
	if (m_nSprite==0)
		return;

	// 좌표 설정
	pSurface = (WORD*)((BYTE*)pSurface + Pitch*m_pointOutput.y + (m_pointOutput.x<<1));

	register int i;
	register int j;

	WORD	*pSurfaceTemp;	

	int rectBottom = m_rectOutline.bottom;
	int rectRight = m_rectOutline.right;

	// Clipping도 해야한다. T_T;;;
	for (i=0; i<=rectBottom; i++)
	{
		pSurfaceTemp = pSurface;

		//for (j=0; j<=m_rectOutline.right; j++)
		j = 0;
		while (j <= rectRight)
		{		
			// 투명색만큼 건너뛴다.
			if (m_ppPixelInfo[i][j] < PIXELTYPE_MAXCOUNT)
			{				
				pSurfaceTemp += m_ppPixelInfo[i][j];
				j += m_ppPixelInfo[i][j];
			}

			// 외곽선인 경우 
			if (m_ppPixelInfo[i][j] == PIXELTYPE_OUTLINE)
			{
				*pSurfaceTemp = color;
			}
			// 색깔인 경우
			//else if (m_ppPixelInfo[i][j] == PIXELTYPE_COLOR)
			//{
			//	*pSurfaceTemp = m_ppColorInfo[i][j];
			//}
			// 끝까지 투명색인 경우
			else if (m_ppPixelInfo[i][j]==PIXELTYPE_TRANSEND)
			{
				break;
			}



			pSurfaceTemp++;
			j++;
		}

		// 다음 줄
		pSurface = (WORD*)((BYTE*)pSurface + Pitch);
	}
}

//----------------------------------------------------------------------
// BltOutline Clip
//----------------------------------------------------------------------
// 외곽선을 화면에 출력한다. 
//
// pSurface에.. 외곽선은 color로 
//----------------------------------------------------------------------
void		
CSpriteOutlineManager::BltOutlineClip(WORD* pSurface, WORD Pitch, WORD color, RECT* pRect)
{
	if (m_nSprite==0)
		return;

	// 좌표 설정
	pSurface = (WORD*)((BYTE*)pSurface + Pitch*m_pointOutput.y + (m_pointOutput.x<<1));

	pSurface = (WORD*)((BYTE*)pSurface + Pitch*pRect->top + (pRect->left<<1));

	register int i;
	register int j;

	WORD	*pSurfaceTemp;	

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;

	// Clipping도 해야한다. T_T;;;
	for (i=pRect->top; i<rectBottom; i++)
	{
		pSurfaceTemp = pSurface;

		//for (j=0; j<=m_rectOutline.right; j++)
		j = rectLeft;
		while (j < rectRight)
		{		
			// 투명색만큼 건너뛴다.
			if (m_ppPixelInfo[i][j] < PIXELTYPE_MAXCOUNT)
			{				
				pSurfaceTemp += m_ppPixelInfo[i][j];
				j += m_ppPixelInfo[i][j];

				continue;
			}

			// 외곽선인 경우 
			if (m_ppPixelInfo[i][j] == PIXELTYPE_OUTLINE)
			{
				*pSurfaceTemp = color;
			}
			// 색깔인 경우
			//else if (m_ppPixelInfo[i][j] == PIXELTYPE_COLOR)
			//{
				//*pSurfaceTemp = m_ppColorInfo[i][j];
			//}
			// 끝까지 투명색인 경우
			else if (m_ppPixelInfo[i][j]==PIXELTYPE_TRANSEND)
			{
				break;
			}



			pSurfaceTemp++;
			j++;
		}

		// 다음 줄
		pSurface = (WORD*)((BYTE*)pSurface + Pitch);
	}
}


//----------------------------------------------------------------------
// Blt Darkness
//----------------------------------------------------------------------
// 외곽선을 화면에 출력한다. 
//
// pSurface에.. 외곽선은 color로 
//----------------------------------------------------------------------
void		
CSpriteOutlineManager::BltDarkness(WORD* pSurface, WORD Pitch, WORD color, BYTE DarkBits)
{
	if (m_nSprite==0)
		return;

	// 좌표 설정
	pSurface = (WORD*)((BYTE*)pSurface + Pitch*m_pointOutput.y + (m_pointOutput.x<<1));

	register int i;
	register int j;

	WORD	*pSurfaceTemp;
	
	int rectBottom = m_rectOutline.bottom;
	int rectRight = m_rectOutline.right;

	// Clipping도 해야한다. T_T;;;
	for (i=0; i<=rectBottom; i++)
	{
		pSurfaceTemp = pSurface;

		//for (j=0; j<=m_rectOutline.right; j++)
		j = 0;
		while (j <= rectRight)
		{		
			// 투명색만큼 건너뛴다.
			if (m_ppPixelInfo[i][j] < PIXELTYPE_MAXCOUNT)
			{				
				pSurfaceTemp += m_ppPixelInfo[i][j];
				j += m_ppPixelInfo[i][j];
			}

			// 외곽선인 경우 
			if (m_ppPixelInfo[i][j] == PIXELTYPE_OUTLINE)
			{
				*pSurfaceTemp = color;
			}
			// 색깔인 경우
			else if (m_ppPixelInfo[i][j] == PIXELTYPE_COLOR)
			{
				*pSurfaceTemp = (m_ppColorInfo[i][j] >> DarkBits) & CDirectDraw::s_wMASK_SHIFT[DarkBits];				
			}
			// 끝까지 투명색인 경우
			else if (m_ppPixelInfo[i][j]==PIXELTYPE_TRANSEND)
			{
				break;
			}



			pSurfaceTemp++;
			j++;
		}

		// 다음 줄
		pSurface = (WORD*)((BYTE*)pSurface + Pitch);
	}
}


//----------------------------------------------------------------------
// Blt Darkness Clip
//----------------------------------------------------------------------
// 외곽선을 화면에 출력한다. 
//
// pSurface에.. 외곽선은 color로 
//----------------------------------------------------------------------
void		
CSpriteOutlineManager::BltDarknessClip(WORD* pSurface, WORD Pitch, WORD color, BYTE DarkBits, RECT* pRect)
{
	if (m_nSprite==0)
		return;

	// 좌표 설정
	pSurface = (WORD*)((BYTE*)pSurface + Pitch*m_pointOutput.y + (m_pointOutput.x<<1));

	pSurface = (WORD*)((BYTE*)pSurface + Pitch*pRect->top + (pRect->left<<1));

	register int i;
	register int j;

	WORD	*pSurfaceTemp;	

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;

	// Clipping도 해야한다. T_T;;;
	for (i=pRect->top; i<rectBottom; i++)
	{
		pSurfaceTemp = pSurface;

		//for (j=0; j<=m_rectOutline.right; j++)
		j = rectLeft;
		while (j < rectRight)
		{		
			// 투명색만큼 건너뛴다.
			if (m_ppPixelInfo[i][j] < PIXELTYPE_MAXCOUNT)
			{				
				pSurfaceTemp += m_ppPixelInfo[i][j];
				j += m_ppPixelInfo[i][j];

				continue;
			}

			// 외곽선인 경우 
			if (m_ppPixelInfo[i][j] == PIXELTYPE_OUTLINE)
			{
				*pSurfaceTemp = color;
			}
			// 색깔인 경우
			else if (m_ppPixelInfo[i][j] == PIXELTYPE_COLOR)
			{
				*pSurfaceTemp = (m_ppColorInfo[i][j] >> DarkBits) & CDirectDraw::s_wMASK_SHIFT[DarkBits];
			}
			// 끝까지 투명색인 경우
			else if (m_ppPixelInfo[i][j]==PIXELTYPE_TRANSEND)
			{
				break;
			}



			pSurfaceTemp++;
			j++;
		}

		// 다음 줄
		pSurface = (WORD*)((BYTE*)pSurface + Pitch);
	}
}

//----------------------------------------------------------------------
// Merge NormalSprite
//----------------------------------------------------------------------
// n번째 Sprite는 Normal Sprite이므로 
// Normal Sprite 정보를 읽어서 외곽선 정보에 추가한다.
//----------------------------------------------------------------------
void
CSpriteOutlineManager::MergeNormalSprite( int n )
{
	CSprite*	pSprite = (CSprite*)m_pSprite[n];

	register int i;
	register int j;

	int		count, 
			colorCount;

	WORD	*pPixels;
	BYTE	*pPixelInfoTemp;
	WORD	*pColorInfoTemp;

	int px = m_Position[n].x;
	int py = m_Position[n].y;

	// 각 줄에 대해서...
	int height = pSprite->GetHeight();
	for (i=0; i<height; i++)
	{
		// i번째 줄을 읽어들인다.
		pPixels			= pSprite->GetPixelLine( i );

		// 외곽선 정보에서 이 Sprite가 저장될 위치..
		pPixelInfoTemp	= m_ppPixelInfo[py] + px;
		pColorInfoTemp	= m_ppColorInfo[py] + px;

		// (투명수,색깔수,색깔들)의 반복 수		
		count	= *pPixels++;

 		// 한 줄 출력
		if (count > 0)
		{
			j = count;
			do {		
				pPixelInfoTemp += *pPixels;			// 투명색만큼 건너 뛴다.
				pColorInfoTemp += *pPixels;			// 투명색만큼 건너 뛴다.
				pPixels ++;	

				colorCount = *pPixels++;		// 투명 아닌 색 수				

				// 투명이 아닌 색들을 ColorInfo에 추가
				memcpy((void*)pColorInfoTemp, (void*)pPixels, colorCount << 1);
				
				// PixelInfo에는 색깔이라는 표시를 해둔다.
				memset((void*)pPixelInfoTemp, PIXELTYPE_COLOR, colorCount);
				
				// 출력한 색깔만큼 pointer증가
				pPixels			+= colorCount;
				pPixelInfoTemp	+= colorCount;
				pColorInfoTemp	+= colorCount;
			} while (--j);
		} 

		py++;
	}
}


//----------------------------------------------------------------------
// Merge AlphaSprite
//----------------------------------------------------------------------
void
CSpriteOutlineManager::MergeAlphaSprite( int n )
{
	CAlphaSprite*	pSprite = (CAlphaSprite*)m_pSprite[n];

	register int i;
	register int j;
	register int k;

	int		count, 
			colorCount;

	WORD	*pPixels;
	BYTE	*pPixelInfoTemp;
	WORD	*pColorInfoTemp;

	int px = m_Position[n].x;
	int py = m_Position[n].y;

	// 각 줄에 대해서...
	int height = pSprite->GetHeight();
	for (i=0; i<height; i++)
	{
		// i번째 줄을 읽어들인다.
		pPixels			= pSprite->GetPixelLine( i );

		// 외곽선 정보에서 이 Sprite가 저장될 위치..
		pPixelInfoTemp	= m_ppPixelInfo[py] + px;
		pColorInfoTemp	= m_ppColorInfo[py] + px;

		// (투명수,색깔수,색깔들)의 반복 수		
		count	= *pPixels++;

 		// 한 줄 출력
		if (count > 0)
		{
			j = count;
			do {				
				pPixelInfoTemp += *pPixels;			// 투명색만큼 건너 뛴다.
				pColorInfoTemp += *pPixels;			// 투명색만큼 건너 뛴다.
				pPixels ++;	

				colorCount = *pPixels++;		// 투명 아닌 색 수				

				// 투명이 아닌 색들을 ColorInfo에 추가
				//memcpy((void*)pColorInfoTemp, (void*)pPixels, colorCount << 1);
				for (k=0; k<colorCount; k++)
				{
					*pColorInfoTemp = *pPixels;
					
					pColorInfoTemp++;
					pPixels += 2;
				}
				
				// PixelInfo에는 색깔이라는 표시를 해둔다.
				memset((void*)pPixelInfoTemp, PIXELTYPE_COLOR, colorCount);
				
				// 출력한 색깔만큼 pointer증가
				//pPixels			+= colorCount;
				//pPixelInfoTemp	+= colorCount;
				pColorInfoTemp	+= colorCount;
			} while (--j);
		}
		
		py++;
	}
}

//----------------------------------------------------------------------
// Merge IndexSprite
//----------------------------------------------------------------------
void
CSpriteOutlineManager::MergeIndexSprite( int n )
{
	CIndexSprite*	pSprite = (CIndexSprite*)m_pSprite[n];

	register int i;
	register int j;
	register int k;

	int		count,
			indexCount,
			colorCount;
	
	BYTE	colorSet,
			colorGradation;

	WORD	*pPixels;
	BYTE	*pPixelInfoTemp;
	WORD	*pColorInfoTemp;

	int px = m_Position[n].x;
	int py = m_Position[n].y;

	// 각 줄에 대해서...
	int height = pSprite->GetHeight();
	for (i=0; i<height; i++)
	{
		// i번째 줄을 읽어들인다.
		pPixels			= pSprite->GetPixelLine( i );

		// 외곽선 정보에서 이 Sprite가 저장될 위치..
		pPixelInfoTemp	= m_ppPixelInfo[py] + px;
		pColorInfoTemp	= m_ppColorInfo[py] + px;

		// (투명수,색깔수,색깔들)의 반복 수		
		count	= *pPixels++;

 		// 한 줄 출력
		if (count > 0)
		{
			j = count;
			do {				
				pPixelInfoTemp += *pPixels;			// 투명색만큼 건너 뛴다.
				pColorInfoTemp += *pPixels;			// 투명색만큼 건너 뛴다.
				pPixels ++;	

				indexCount = *pPixels++;		// 투명 아닌 색 수
				
				// index색을 ColorInfo에 추가			
				for (k=0; k<indexCount; k++)
				{
					colorSet		= (*pPixels >> 8) & 0xFF;	// set
					colorGradation	= (BYTE)*pPixels;			// gradation
					pPixels++;

					// s_Value와 관련된 색을 선택해서 출력한다.
					*pColorInfoTemp		= CIndexSprite::ColorSet[m_Value[n][colorSet]][colorGradation];
					pColorInfoTemp ++;
				}

				// Normal 색깔 수
				colorCount = *pPixels++;

				// Normal색을 ColorInfo에 추가
				memcpy((void*)pColorInfoTemp, (void*)pPixels, colorCount << 1);
				
				// PixelInfo에는 색깔이라는 표시를 해둔다.
				memset((void*)pPixelInfoTemp, PIXELTYPE_COLOR, indexCount+colorCount);
				
				// 출력한 색깔만큼 pointer증가
				pPixels			+= colorCount;			
				pColorInfoTemp	+= colorCount;
				pPixelInfoTemp	+= indexCount + colorCount;
			} while (--j);
		}

		py++;
	}
}

//----------------------------------------------------------------------
// Merge IndexSprite ColorSet
//----------------------------------------------------------------------
// 특정 ColorSet으로 출력한다.
//----------------------------------------------------------------------
void
CSpriteOutlineManager::MergeIndexSpriteColorSet( int n )
{
	int colorSet = m_ChangeColorSet[n];
	CSpriteSurface::s_Value1 = colorSet;	
	for(int ic = 0; ic < 256; ic++)
	{
		m_Value[n][ic] = 0;
	}

	CIndexSprite*	pSprite = (CIndexSprite*)m_pSprite[n];

	register int i;
	register int j;
	register int k;

	int		count,
			indexCount,
			colorCount;
	
	int	colorGradation;

	WORD	*pPixels;
	BYTE	*pPixelInfoTemp;
	WORD	*pColorInfoTemp;

	int px = m_Position[n].x;
	int py = m_Position[n].y;

	// 각 줄에 대해서...
	int height = pSprite->GetHeight();
	for (i=0; i<height; i++)
	{
		// i번째 줄을 읽어들인다.
		pPixels			= pSprite->GetPixelLine( i );

		// 외곽선 정보에서 이 Sprite가 저장될 위치..
		pPixelInfoTemp	= m_ppPixelInfo[py] + px;
		pColorInfoTemp	= m_ppColorInfo[py] + px;

		// (투명수,색깔수,색깔들)의 반복 수		
		count	= *pPixels++;

 		// 한 줄 출력
		if (count > 0)
		{
			j = count;
			do {				
				pPixelInfoTemp += *pPixels;			// 투명색만큼 건너 뛴다.
				pColorInfoTemp += *pPixels;			// 투명색만큼 건너 뛴다.
				pPixels ++;	

				indexCount = *pPixels++;		// 투명 아닌 색 수
				
				// index색을 ColorInfo에 추가			
				for (k=0; k<indexCount; k++)
				{
					//colorSet		= (*pPixels >> 8) & 0xFF;	// set
					colorGradation	= (*pPixels & 0xFF);			// gradation
					pPixels++;

					// s_Value와 관련된 색을 선택해서 출력한다.				
					*pColorInfoTemp		= CIndexSprite::ColorSet[colorSet][colorGradation];
					pColorInfoTemp ++;
				}

				// Normal 색깔 수
				colorCount = *pPixels++;

				// Normal색을 ColorInfo에 추가
				CSpriteSurface::memcpyEffectGradation(pColorInfoTemp, pPixels, colorCount);
				//memcpy((void*)pColorInfoTemp, (void*)pPixels, colorCount << 1);
				
				// PixelInfo에는 색깔이라는 표시를 해둔다.
				memset((void*)pPixelInfoTemp, PIXELTYPE_COLOR, indexCount+colorCount);
				
				// 출력한 색깔만큼 pointer증가
				pPixels			+= colorCount;			
				pColorInfoTemp	+= colorCount;
				pPixelInfoTemp	+= indexCount + colorCount;
			} while (--j);
		}

		py++;
	}
}
