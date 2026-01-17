
#include "Client_PCH.h"
#include "CAlphaSpritePal.h"

#include "CSpriteSurface.h"

int		CAlphaSpritePal::s_Value1	= 0;
int		CAlphaSpritePal::s_Value2	= 0;

// Constructor and destructor for vtable
CAlphaSpritePal::CAlphaSpritePal()
{
}

CAlphaSpritePal::~CAlphaSpritePal()
{
}


void CAlphaSpritePal::SetPixel(BYTE *pSource, WORD pitch, BYTE *pSourceAlpha, WORD alphaPitch, WORD width, WORD height)
{
	// memory해제
	Release();

	m_Width = width;
	m_Height = height;

	// 일단 memory를 적당히 잡아둔다.	
	BYTE*	data = new BYTE[m_Width*4+10];

	int		index;//,				// data의 index로 사용
//			lastColorIndex;		// 투명이 아닌색 개수의 최근 index
	int		count;				// 반복수
	int		trans,				// 투명색 개수
			color;				// 투명이 아닌색 개수

	BOOL	bCheckTrans;		// 최근에 검사한게 투명색인가?

	BYTE	*pSourceTemp;
	BYTE	*pAlphaTemp;


	// height줄 만큼 memory잡기
	m_pPixels = new BYTE* [height];
	BYTE **Pixels = new BYTE* [height];
	std::vector<int> PixelSize;

	register int i;
	register int j;

	for (int i=0; i<height; i++)
	{
		index = 0;
		count = 0;
		trans = 0;
		color = 0;
		bCheckTrans = TRUE;

		pSourceTemp = pSource;
		pAlphaTemp = pSourceAlpha;
		
		for( j=0; j<width;)
		{
			int trans_count = 0;
			int image_count = 0;
			
			while( *(pSourceTemp+j) == s_Colorkey || *(pAlphaTemp+j) == 0 )
			{
				if( j >= width  || trans_count == 255)
					break;
				trans_count++;
				j++;				
			}
			count++;
			data[index++] = trans_count;
			
			while( *(pSourceTemp + j) != s_Colorkey && *(pAlphaTemp+j) != 0)
			{
				if( j>= width || image_count == 255 )
					break;
				image_count ++;
				j++;
				
			}
			data[index++] = image_count;
			
			if( image_count > 0)
			{
				for( int t = 0; t < image_count; t++ )
				{
					BYTE alpha;
					
					alpha = *( pAlphaTemp + ( j - image_count ) + t);//(BYTE)(*pAlphaTemp & 0x001F);	// Alpha값(Blue값을 택한다.)
					data[index++] = alpha;
					data[index++] = *( pSourceTemp + ( j - image_count ) + t );
				}
			}			
		}
/*
		
		// 각 line에 대해서 압축~
		for (j=0; j<width; j++)
		{
			// 0번 color에 대해서 압축
			if (*pSourceTemp==s_Colorkey || *pAlphaTemp==0)
			{
				// 최근에 검사한게 투명색이 아니었다면
				if (!bCheckTrans)
				{
					// ' (투명,색깔수,색깔들) '의 한 set가 끝났음을 의미하므로
					// 색깔들은 (alpha,색깔들)의 집합을 의미한다.
					count++;
					
					// color개수 저장
					data[lastColorIndex] = color;
					color = 0;

					bCheckTrans = TRUE;
				}
				
				trans++;				
			}
			else
			{
				// 최근에 검사한게 투명색이었다면..
				if (bCheckTrans)
				{						
					data[index++] = trans;		// 상위 byte에 투명수를 넣는다.
					trans = 0;

					lastColorIndex=index++;			// 색깔수를 넣을 위치를 기억					

					bCheckTrans = FALSE;
				}
				// alpha값 만들기
				BYTE alpha;
				alpha = *pAlphaTemp;//(BYTE)(*pAlphaTemp & 0x001F);	// Alpha값(Blue값을 택한다.)

				data[index++] = alpha;					// Alpha 값을 저장한다.				
				data[index++] = *pSourceTemp;	// 실제 색깔을 저장한다.

				color++;								
			}

			pSourceTemp++;
			pAlphaTemp++;
		}
		*/
		
//		// 한 줄의 마지막 점이 투명색인가?
//		if (bCheckTrans)
//		{
//			// 투명색이면 별다른 처리를 안해줘도 될거 같다.
//		}	
//		// 투명색이 아닌 경우, 점의 개수를 저장시켜줘야 한다.
//		else
//		{			
//			count++;
//			data[lastColorIndex] = color;
//		}
		
		// memory를 다시 잡는다.
		Pixels[i] = new BYTE [index+1];
		m_Size += index+1;

		// m_pPixels[i]를 압축했으므로 data로 대체한다.
		// m_pPixels[i][0]에는 count를 넣어야 한다.
		Pixels[i][0] = count;
		memcpy(Pixels[i]+1, data, index);
		PixelSize.push_back(index+1);

		pSource += pitch;
		pSourceAlpha += alphaPitch;
	}

	m_pData = new BYTE[m_Size+sizeof(BYTE *)*height];
	m_pPixels = (BYTE **)(m_pData+m_Size);
	BYTE *TempData = m_pData;

	for(i = 0; i < height; i++)
	{
		memcpy(TempData, Pixels[i], PixelSize[i]);
		m_pPixels[i] = TempData;
		TempData += PixelSize[i];
		delete [] Pixels[i];
	}

	delete [] data;
	delete [] Pixels;

	m_bInit = true;
}

void CAlphaSpritePal::Blt(int x, int y, WORD* pDest, int pitch, MPalette &pal)
{
	int	count,			
		colorCount;
	
	WORD	*pDestTemp;
	BYTE	*pPixels;
	
	register int i;
	register int j;
	int rectBottom = m_Height;
	
	pDest = (WORD*)((BYTE*)pDest + y*pitch +x*2);
	
	if (rectBottom > 0)
	{
		i = 0;
		
		do
		{
			pPixels		= m_pPixels[i];
			pDestTemp = (WORD*)((BYTE*)pDest + i*pitch);
			
			// (투명수,색깔수,색깔들)의 반복 수		
			count	= *pPixels++;		
			// 한 줄 출력
			if (count > 0)
			{	
				j = count;
				do {
					pDestTemp += *pPixels++;			// 투명색만큼 건너 뛴다.
					colorCount = *pPixels++;		// 투명 아닌 색 수				
					
					// 투명이 아닌 색들을 Surface에 출력한다.
					memcpyAlpha(pDestTemp, pPixels, colorCount, pal);
					
					pDestTemp	+= colorCount;
					// 실제 점 개수는 alpha값 때문에 2배이다.
					pPixels		+= (colorCount<<1);
				} while (--j);
			}
			
		} while (i++ < rectBottom-1);
	}
}

//----------------------------------------------------------------------
// AlphaChannel Copy
//----------------------------------------------------------------------
// Alpha값 : 1~32
//----------------------------------------------------------------------
// pSource의 것을 pDest에 출력을 해야한다.
// pSource의 구성은 (alpha,색깔 하나)의 pixels만큼 반복이다.
//----------------------------------------------------------------------
void	
CAlphaSpritePal::memcpyAlpha(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register WORD		sTemp,dTemp;
	register int		sr,sg,sb,dr,dg,db;

	register int i = pixels;
	
	BYTE alpha;
	
	// Alpha Channel Blending
	// 한점씩 찍기
	while (i--)
	{	
		// Source에는 Alpha값이 포함되어 있다.
		alpha = *pSource;

		pSource++;
		
		// 한점 찍기
		sTemp = pal[*pSource];
		dTemp = *pDest;
		
		sr = CDirectDraw::Red(sTemp);
		sg = CDirectDraw::Green(sTemp);
		sb = CDirectDraw::Blue(sTemp);
		
		dr = CDirectDraw::Red(dTemp);
		dg = CDirectDraw::Green(dTemp);
		db = CDirectDraw::Blue(dTemp);		

		*pDest = ((((sb - db)*alpha >> 5) + db) |
		((((sg - dg)*alpha >> 5) + dg) << CDirectDraw::s_bSHIFT_G) |
		(((sr - dr)*alpha >> 5) + dr) << CDirectDraw::s_bSHIFT_R);
		
		pDest++;
		pSource++;
	}
	
}


//----------------------------------------------------------------------
// AlphaChannel Copy  4444
//----------------------------------------------------------------------
// Alpha값 : 1~32
//----------------------------------------------------------------------
// pSource의 것을 pDest에 출력을 해야한다.
// pSource의 구성은 (alpha,색깔 하나)의 pixels만큼 반복이다.
//
// A:R:G:B = 4:4:4:4 Texture를 위한 것이다.
//----------------------------------------------------------------------
void	
CAlphaSpritePal::memcpyAlpha4444(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register WORD		sTemp;
	register int		sr,sg,sb;
	//static WORD		temp, temp2;

	register int i = pixels;

	BYTE alpha;

	// Alpha Channel Blending
	// 한점씩 찍기
	while (i--)
	{	
		// Source에는 Alpha값이 포함되어 있다.
		alpha = *pSource >> 1;	//	alpha = (*pSource >> 8) >> 1;
		pSource++;

		// 한점 찍기
		sTemp = pal[*pSource];
	
		sr = (sTemp >> CDirectDraw::s_bSHIFT4_R);// & 0x0F;
		sg = (sTemp >> CDirectDraw::s_bSHIFT4_G) & 0x0F;
		sb = (sTemp >> CDirectDraw::s_bSHIFT4_B) & 0x0F;
		
		*pDest =	(alpha << 12) |
					(sr << 8) |
					(sg << 4) |
					(sb);
	
		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// BltClip
//----------------------------------------------------------------------
// pRect의 영역만 출력한다.
//----------------------------------------------------------------------
void
CAlphaSpritePal::BltClip(WORD* pDest, WORD pitch, RECT* pRect, MPalette &pal)
{
	BYTE	*pPixels;
	WORD	*pDestTemp;

	//--------------------------------------------	
	// 첫 점 (x,y)
	//--------------------------------------------
	pDest = (WORD*)((BYTE*)pDest + pitch*pRect->top + pRect->left);
	//WORD width = ((pRect->right - pRect->left)<<1);

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int	count,
			transCount, 
			colorCount,
			index,
			dist;

	BOOL	bPut;		

	register int i;
	register int j;
	int rectBottom = pRect->bottom;	

	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_pPixels[i];
		pDestTemp = pDest;		

		// (투명수,색깔수,색깔들)의 반복 수
		count = *pPixels++;		

		// 한 줄 출력
		bPut = (pRect->left==0)? TRUE:FALSE;
		index = 0;
			
		// 각 줄마다 Clipping을 해줘야 하는데...
		// xxxxxOOOOOOOOOOOOOO 이거나  (x:출력안함, O:출력함)
		// OOOOOOOOOOOOOOxxxxx 이거.. 두 가지 경우다.			
		if (count > 0)
		{
			j = count;
			do 
			{				
				transCount = *pPixels++;		// 투명색 수			
				colorCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 건너 뛴다.
				//lpSurfaceTemp += transCount;
				index += transCount;

				// 출력해도 되는 경우에는 출력한다.
				if (bPut)
				{
					// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
					// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

					// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우
					if (index > pRect->right)
						break;

					pDestTemp += transCount;

					// 투명색 아닌 것을 조금 출력해야 할 경우
					if (index+colorCount > pRect->right)
					{							
						// 투명이 아닌 색들을 Surface에 출력한다.
						memcpyAlpha(pDestTemp, pPixels, pRect->right - index, pal);
						break;
					}						

					// 모두 출력
					memcpyAlpha(pDestTemp, pPixels, colorCount, pal);
					pDestTemp += colorCount;
				}				
				// 출력하면 안 될 경우(줄의 왼쪽부분)에는 출력해도 되는지 확인해본다.
				else
				{
					// 투명색만으로 범위를 넘어갔으므로 모두 출력
					if (index > pRect->left)
					{	
						pDestTemp += index - pRect->left;
						memcpyAlpha(pDestTemp, pPixels, colorCount, pal);
						pDestTemp += colorCount;
						bPut = TRUE;
					}
					else
					if (index+colorCount > pRect->left)
					{
						dist = pRect->left - index;

						// 투명이 아닌 색들을 Surface에 출력한다.
						memcpyAlpha(pDestTemp, pPixels+dist, colorCount-dist, pal);
						pDestTemp += colorCount-dist;

						// 이제부터는 계속 출력한다.
						bPut = TRUE;
					}
				}				

				// 투명이 아닌 색만큼 index증가				
				pPixels += (colorCount<<1);		

				index += colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt
//----------------------------------------------------------------------
// Clipping하지 않는다.
//----------------------------------------------------------------------
void
CAlphaSpritePal::Blt(WORD *pDest, WORD pitch, MPalette &pal)
{
	int	count,			
			colorCount;

	WORD	*pDestTemp;
	BYTE	*pPixels;

	register int i;
	register int j;
	int rectBottom = m_Height;

	if (rectBottom > 0)
	{
		i = rectBottom-1;
		pDest = (WORD*)((BYTE*)pDest + i*pitch);

		do
		{
			pPixels		= m_pPixels[i];
			pDestTemp	= pDest;

			// (투명수,색깔수,색깔들)의 반복 수		
			count	= *pPixels++;		

			// 한 줄 출력
			if (count > 0)
			{			
				j = count;
				do
				{
					pDestTemp += *pPixels++;		// 투명색만큼 건너 뛴다.
					colorCount = *pPixels++;		// 투명 아닌 색 수				

					// 투명이 아닌 색들을 Surface에 출력한다.
					memcpyAlpha(pDestTemp, pPixels, colorCount, pal);
					
					pDestTemp	+= colorCount;

					// 실제 점 개수는 alpha값 때문에 2배이다.
					pPixels		+= (colorCount<<1);
				} while (--j);
			}

			pDest = (WORD*)((BYTE*)pDest - pitch);
		} while (i--);
	}
}

//----------------------------------------------------------------------
// Blt ClipLeft
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// rectLeft개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CAlphaSpritePal::BltClipLeft(WORD* pDest, WORD pitch, RECT* pRect, MPalette &pal)
{
	BYTE	*pPixels;
	WORD	*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int	count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_pPixels[i];
		pDestTemp = pDest;		

		// (투명수,색깔수,색깔들)의 반복 수
		count = *pPixels++;		

		// 한 줄 출력		
		index = 0;
		
		//---------------------------------------------
		// 각 줄마다 Clipping을 해줘야 하는데...
		// xxxxOOOOOOOOOOOOOO인 경우이므로..
		//---------------------------------------------
		// xxxx부분까지 check해주는 루틴
		//---------------------------------------------
		// 한 줄 출력
		if (count > 0)
		{			
			j = count;
			do
			{
				transCount = *pPixels++;		// 투명색 수			
				colorCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - rectLeft;

						// 이번 단계는 모두 출력
						memcpyAlpha(pDestTemp, pPixels, colorCount, pal);
						pDestTemp += colorCount;
						pPixels += (colorCount<<1);

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// 투명이 아닌 색들을 Surface에 출력한다.
						memcpyAlpha(pDestTemp, pPixels+(dist<<1), colorCount-dist, pal);
						pDestTemp += colorCount-dist;
						pPixels += (colorCount<<1);

						// 이제부터는 계속 출력한다.
						break;
					}
				}					

				// 투명이 아닌 색만큼 index증가				
				pPixels += (colorCount<<1);
				index += colorCount;
			} while (--j);

			//---------------------------------------------
			// 이제부터는 계속 출력한다.		
			//---------------------------------------------		
			if (--j > 0)
			{
				do
				{
					transCount = *pPixels++;		// 투명색 수			
					colorCount = *pPixels++;		// 투명 아닌 색 수			
							
					// 투명색만큼 건너 뛴다.
					pDestTemp += transCount;			
					
					// 투명색이 아닌만큼 출력해준다.
					memcpyAlpha(pDestTemp, pPixels, colorCount, pal);

					// memory addr 증가
					pDestTemp += colorCount;
					pPixels += (colorCount<<1);
				} while (--j);
			}
		}
		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt ClipRight
//----------------------------------------------------------------------
// 오른쪽 clipping.  
// rectRight개 까지의 점만 pDest에 출력한다.
//----------------------------------------------------------------------
void
CAlphaSpritePal::BltClipRight(WORD* pDest, WORD pitch, RECT* pRect, MPalette &pal)
{
	BYTE	*pPixels;
	WORD	*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int	count,
			transCount, 
			colorCount,
			index;

	register int	i;
	register int	j;

	int rectBottom = pRect->bottom;
	int rectRight = pRect->right;

	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_pPixels[i];
		pDestTemp = pDest;		

		// (투명수,색깔수,색깔들)의 반복 수
		count = *pPixels++;		

		// 한 줄 출력		
		index = 0;
			
		//---------------------------------------------
		// 각 줄마다 Clipping을 해줘야 하는데...		
		// OOOOOOOOOOOOOOxxxxx 이런 경우이다.
		//---------------------------------------------
		// OOOOOOOOOOOOOO까지만 출력해주면 된다.
		//---------------------------------------------
		// 한 줄 출력
		if (count > 0)
		{			
			j = count;
			do
			{
				transCount = *pPixels++;		// 투명색 수			
				colorCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가
				index += transCount;
				
				// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
				// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

				// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

				//---------------------------------------------
				// 오른쪽 끝까지 도달했을 경우
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// 투명색만으로 더 출력할 필요가 없을 때
					if (index > rectRight)
					{
						break;
					}
					// 투명색 아닌 것을 조금 출력해야 할 경우
					else
					{
						pDestTemp += transCount;
					
						// 투명이 아닌 색들을 Surface에 출력한다.
						memcpyAlpha(pDestTemp, pPixels, rectRight - index, pal);
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				// 출력
				memcpyAlpha(pDestTemp, pPixels, colorCount, pal);
				pDestTemp += colorCount;
				pPixels += (colorCount<<1);
				index += colorCount;
			} while (--j);
		}
		
		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt ClipWidth
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// rectLeft개의 점을 건너띈 다음부터 pDest에 출력한다.
// rectRight까지..
//----------------------------------------------------------------------
void
CAlphaSpritePal::BltClipWidth(WORD* pDest, WORD pitch, RECT* pRect, MPalette &pal)
{
	BYTE	*pPixels;
	WORD	*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int	count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_pPixels[i];
		pDestTemp = pDest;		

		// (투명수,색깔수,색깔들)의 반복 수
		count = *pPixels++;		

		// 한 줄 출력		
		index = 0;
		
		//---------------------------------------------
		// 각 줄마다 Clipping을 해줘야 하는데...
		// xxxxOOOOOOOOOOOOOO인 경우이므로..
		//---------------------------------------------
		// xxxx부분까지 check해주는 루틴
		//---------------------------------------------
		// 한 줄 출력
		if (count > 0)
		{			
			j = count;
			do
			{
				transCount = *pPixels++;		// 투명색 수			
				colorCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - rectLeft;

						// 이번 단계는 모두 출력
						// 오른쪽 끝을 넘어가는 경우..
						if (index+colorCount > rectRight)
						{							
							// 투명색만으로 오른쪽 끝 넘어가는 경우
							if (index > rectRight)
							{
							}
							else
							{
								//memset((void*)pDestTemp, 0, (rectRight - index)<<1);
								memcpyAlpha(pDestTemp, pPixels, (rectRight - index), pal);
							}

							j = 0;
							
							break;
						}
						memcpyAlpha(pDestTemp, pPixels, colorCount, pal);
						pDestTemp += colorCount;
						pPixels += (colorCount<<1);
						index += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// 투명이 아닌 색들을 Surface에 출력한다.
						// 오른쪽 끝을 넘어가는 경우..
						if (index+colorCount > rectRight)
						{
							memcpyAlpha(pDestTemp, pPixels+(dist<<1), (rectRight - rectLeft), pal);

							j = 0;						
							break;
						}		
						memcpyAlpha(pDestTemp, pPixels+(dist<<1), colorCount-dist, pal);
						pDestTemp += colorCount-dist;
						pPixels += (colorCount<<1);
						index += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}					

				// 투명이 아닌 색만큼 index증가				
				pPixels += (colorCount<<1);
				index += colorCount;
			} while (--j);

			//---------------------------------------------
			// 각 줄마다 Clipping을 해줘야 하는데...		
			// OOOOOOOOOOOOOOxxxxx 이런 경우이다.
			//---------------------------------------------
			// OOOOOOOOOOOOOO까지만 출력해주면 된다.
			//---------------------------------------------
			if (--j > 0)
			{
				do
				{
					transCount = *pPixels++;		// 투명색 수			
					colorCount = *pPixels++;		// 투명 아닌 색 수			
							
					// 투명색만큼 index증가
					index += transCount;
					
					// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
					// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

					// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

					//---------------------------------------------
					// 오른쪽 끝까지 도달했을 경우
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// 투명색만으로 더 출력할 필요가 없을 때
						if (index > rectRight)
						{
							break;
						}
						// 투명색 아닌 것을 조금 출력해야 할 경우
						else
						{
							pDestTemp += transCount;
						
							// 투명이 아닌 색들을 Surface에 출력한다.
							memcpyAlpha(pDestTemp, pPixels, rectRight - index, pal);
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					// 출력
					memcpyAlpha(pDestTemp, pPixels, colorCount, pal);
					pDestTemp += colorCount;
					pPixels += (colorCount<<1);
					index += colorCount;
				} while (--j);
			}
		}		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt Clip Height
//----------------------------------------------------------------------
// pRect->top, rectBottom만큼만 출력한다.
//----------------------------------------------------------------------
void
CAlphaSpritePal::BltClipHeight(WORD *pDest, WORD pitch, RECT* pRect, MPalette &pal)
{
	int	count,			
			colorCount;

	WORD	*pDestTemp;
	BYTE	*pPixels;


	register int i;
	register int j;

	int rectBottom = pRect->bottom;

	for (int i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_pPixels[i];
		pDestTemp	= pDest;

		// (투명수,색깔수,색깔들)의 반복 수		
		count	= *pPixels++;		

		// 한 줄 출력
		// 한 줄 출력
		if (count > 0)
		{			
			j = count;
			do
			{
				pDestTemp += *pPixels++;		// 투명색만큼 건너 뛴다.
				colorCount = *pPixels++;		// 투명 아닌 색 수				

				// 투명이 아닌 색들을 Surface에 출력한다.
				memcpyAlpha(pDestTemp, pPixels, colorCount, pal);
				
				pDestTemp	+= colorCount;
				pPixels		+= (colorCount<<1);
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt4444
//----------------------------------------------------------------------
// Clipping하지 않는다.
//----------------------------------------------------------------------
void
CAlphaSpritePal::Blt4444(WORD *pDest, WORD pitch, MPalette &pal)
{
	int		count,			
			colorCount;

	WORD	*pDestTemp;
	BYTE	*pPixels;

	register int i;
	register int j;

	int rectBottom = m_Height;

	if (rectBottom > 0)
	{
		i = rectBottom-1;
		pDest = (WORD*)((BYTE*)pDest + i*pitch);

		do
		{
			pPixels		= m_pPixels[i];
			pDestTemp	= pDest;

			// (투명수,색깔수,색깔들)의 반복 수		
			count	= *pPixels++;		

			// 한 줄 출력
			// 한 줄 출력
			if (count > 0)
			{			
				j = count;
				do
				{				
					pDestTemp += *pPixels++;		// 투명색만큼 건너 뛴다.
					colorCount = *pPixels++;		// 투명 아닌 색 수				

					// 투명이 아닌 색들을 Surface에 출력한다.
					memcpyAlpha4444(pDestTemp, pPixels, colorCount, pal);
					
					pDestTemp	+= colorCount;

					// 실제 점 개수는 alpha값 때문에 2배이다.
					pPixels		+= (colorCount<<1);
				} while (--j);
			}

			pDest = (WORD*)((BYTE*)pDest - pitch);
		} while (i--);
	}
}

//----------------------------------------------------------------------
// Blt4444 ClipLeft
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// rectLeft개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CAlphaSpritePal::Blt4444ClipLeft(WORD* pDest, WORD pitch, RECT* pRect, MPalette &pal)
{
	BYTE	*pPixels;
	WORD	*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_pPixels[i];
		pDestTemp = pDest;		

		// (투명수,색깔수,색깔들)의 반복 수
		count = *pPixels++;		

		// 한 줄 출력		
		index = 0;
		
		//---------------------------------------------
		// 각 줄마다 Clipping을 해줘야 하는데...
		// xxxxOOOOOOOOOOOOOO인 경우이므로..
		//---------------------------------------------
		// xxxx부분까지 check해주는 루틴
		//---------------------------------------------
		// 한 줄 출력
		if (count > 0)
		{			
			j = count;
			do
			{
				transCount = *pPixels++;		// 투명색 수			
				colorCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - rectLeft;

						// 이번 단계는 모두 출력
						memcpyAlpha4444(pDestTemp, pPixels, colorCount, pal);
						pDestTemp += colorCount;
						pPixels += (colorCount<<1);

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// 투명이 아닌 색들을 Surface에 출력한다.
						memcpyAlpha4444(pDestTemp, pPixels+(dist<<1), colorCount-dist, pal);
						pDestTemp += colorCount-dist;
						pPixels += (colorCount<<1);

						// 이제부터는 계속 출력한다.
						break;
					}
				}					

				// 투명이 아닌 색만큼 index증가				
				pPixels += (colorCount<<1);
				index += colorCount;
			} while (--j);

			//---------------------------------------------
			// 이제부터는 계속 출력한다.		
			//---------------------------------------------		
			if (--j > 0)
			{
				do
				{
					transCount = *pPixels++;		// 투명색 수			
					colorCount = *pPixels++;		// 투명 아닌 색 수			
							
					// 투명색만큼 건너 뛴다.
					pDestTemp += transCount;			
					
					// 투명색이 아닌만큼 출력해준다.
					memcpyAlpha4444(pDestTemp, pPixels, colorCount, pal);

					// memory addr 증가
					pDestTemp += colorCount;
					pPixels += (colorCount<<1);
				} while (--j);
			}
		}
		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt4444 ClipRight
//----------------------------------------------------------------------
// 오른쪽 clipping.  
// rectRight개 까지의 점만 pDest에 출력한다.
//----------------------------------------------------------------------
void
CAlphaSpritePal::Blt4444ClipRight(WORD* pDest, WORD pitch, RECT* pRect, MPalette &pal)
{
	BYTE	*pPixels;
	WORD	*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index;

	register int	i;
	register int	j;

	int rectBottom = pRect->bottom;
	int rectRight = pRect->right;

	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_pPixels[i];
		pDestTemp = pDest;		

		// (투명수,색깔수,색깔들)의 반복 수
		count = *pPixels++;		

		// 한 줄 출력		
		index = 0;
			
		//---------------------------------------------
		// 각 줄마다 Clipping을 해줘야 하는데...		
		// OOOOOOOOOOOOOOxxxxx 이런 경우이다.
		//---------------------------------------------
		// OOOOOOOOOOOOOO까지만 출력해주면 된다.
		//---------------------------------------------
		// 한 줄 출력
		if (count > 0)
		{			
			j = count;
			do
			{
				transCount = *pPixels++;		// 투명색 수			
				colorCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가
				index += transCount;
				
				// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
				// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

				// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

				//---------------------------------------------
				// 오른쪽 끝까지 도달했을 경우
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// 투명색만으로 더 출력할 필요가 없을 때
					if (index > rectRight)
					{
						break;
					}
					// 투명색 아닌 것을 조금 출력해야 할 경우
					else
					{
						pDestTemp += transCount;
					
						// 투명이 아닌 색들을 Surface에 출력한다.
						memcpyAlpha4444(pDestTemp, pPixels, rectRight - index, pal);
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				// 출력
				memcpyAlpha4444(pDestTemp, pPixels, colorCount, pal);
				pDestTemp += colorCount;
				pPixels += (colorCount<<1);
				index += colorCount;
			} while (--j);
		}
		
		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt4444 ClipWidth
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// rectLeft개의 점을 건너띈 다음부터 pDest에 출력한다.
// rectRight까지..
//----------------------------------------------------------------------
void
CAlphaSpritePal::Blt4444ClipWidth(WORD* pDest, WORD pitch, RECT* pRect, MPalette &pal)
{
	BYTE	*pPixels;
	WORD	*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_pPixels[i];
		pDestTemp = pDest;		

		// (투명수,색깔수,색깔들)의 반복 수
		count = *pPixels++;		

		// 한 줄 출력		
		index = 0;
		
		//---------------------------------------------
		// 각 줄마다 Clipping을 해줘야 하는데...
		// xxxxOOOOOOOOOOOOOO인 경우이므로..
		//---------------------------------------------
		// xxxx부분까지 check해주는 루틴
		//---------------------------------------------
		// 한 줄 출력
		if (count > 0)
		{			
			j = count;
			do
			{
				transCount = *pPixels++;		// 투명색 수			
				colorCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - rectLeft;

						// 이번 단계는 모두 출력
						memcpyAlpha4444(pDestTemp, pPixels, colorCount, pal);
						pDestTemp += colorCount;
						pPixels += (colorCount<<1);
						index += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// 투명이 아닌 색들을 Surface에 출력한다.
						memcpyAlpha4444(pDestTemp, pPixels+(dist<<1), colorCount-dist, pal);
						pDestTemp += colorCount-dist;
						pPixels += (colorCount<<1);
						index += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}					

				// 투명이 아닌 색만큼 index증가				
				pPixels += (colorCount<<1);
				index += colorCount;
			} while (--j);

			//---------------------------------------------
			// 각 줄마다 Clipping을 해줘야 하는데...		
			// OOOOOOOOOOOOOOxxxxx 이런 경우이다.
			//---------------------------------------------
			// OOOOOOOOOOOOOO까지만 출력해주면 된다.
			//---------------------------------------------
			if (--j > 0)
			{
				do
				{
					transCount = *pPixels++;		// 투명색 수			
					colorCount = *pPixels++;		// 투명 아닌 색 수			
							
					// 투명색만큼 index증가
					index += transCount;
					
					// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
					// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

					// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

					//---------------------------------------------
					// 오른쪽 끝까지 도달했을 경우
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// 투명색만으로 더 출력할 필요가 없을 때
						if (index > rectRight)
						{
							break;
						}
						// 투명색 아닌 것을 조금 출력해야 할 경우
						else
						{
							pDestTemp += transCount;
						
							// 투명이 아닌 색들을 Surface에 출력한다.
							memcpyAlpha4444(pDestTemp, pPixels, rectRight - index, pal);
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					// 출력
					memcpyAlpha4444(pDestTemp, pPixels, colorCount, pal);
					pDestTemp += colorCount;
					pPixels += (colorCount<<1);
					index += colorCount;
				} while (--j);
			}
		}		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt4444 Clip Height
//----------------------------------------------------------------------
// pRect->top, rectBottom만큼만 출력한다.
//----------------------------------------------------------------------
void
CAlphaSpritePal::Blt4444ClipHeight(WORD *pDest, WORD pitch, RECT* pRect, MPalette &pal)
{
	int		count,			
			colorCount;

	WORD	*pDestTemp;
	BYTE	*pPixels;


	register int i;
	register int j;

	int rectBottom = pRect->bottom;

	for (int i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_pPixels[i];
		pDestTemp	= pDest;

		// (투명수,색깔수,색깔들)의 반복 수		
		count	= *pPixels++;		

		// 한 줄 출력
		// 한 줄 출력
		if (count > 0)
		{			
			j = count;
			do
			{	
				pDestTemp += *pPixels++;		// 투명색만큼 건너 뛴다.
				colorCount = *pPixels++;		// 투명 아닌 색 수				

				// 투명이 아닌 색들을 Surface에 출력한다.
				memcpyAlpha4444(pDestTemp, pPixels, colorCount, pal);
				
				pDestTemp	+= colorCount;
				pPixels		+= (colorCount<<1);
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}


//----------------------------------------------------------------------
// Blt4444NotTrans
//----------------------------------------------------------------------
// Clipping하지 않는다.
//----------------------------------------------------------------------
void
CAlphaSpritePal::Blt4444NotTrans(WORD *pDest, WORD pitch, MPalette &pal)
{
	int		count,	
			transCount,
			colorCount;

	WORD	*pDestTemp;
	BYTE	*pPixels;

	register int i;
	register int j;

	int rectBottom = m_Height;

	if (rectBottom > 0)
	{
		i = rectBottom-1;
		pDest = (WORD*)((BYTE*)pDest + i*pitch);

		do
		{
			pPixels		= m_pPixels[i];
			pDestTemp	= pDest;

			// (투명수,색깔수,색깔들)의 반복 수		
			count	= *pPixels++;		

			// 한 줄 출력
			if (count > 0)
			{			
				j = count;
				do
				{		
					transCount = *pPixels++;					
					colorCount = *pPixels++;		// 투명 아닌 색 수				

					// 0을 출력한다.
					memset(pDestTemp, 0, transCount<<1);
					pDestTemp += transCount;		// 투명색만큼 건너 뛴다.

					// 투명이 아닌 색들을 Surface에 출력한다.
					memcpyAlpha4444(pDestTemp, pPixels, colorCount, pal);
					
					pDestTemp	+= colorCount;

					// 실제 점 개수는 alpha값 때문에 2배이다.
					pPixels		+= (colorCount<<1);
				} while (--j);
			}

			pDest = (WORD*)((BYTE*)pDest - pitch);
		} while (i--);
	}
}

//----------------------------------------------------------------------
// Blt4444NotTrans ClipLeft
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// rectLeft개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CAlphaSpritePal::Blt4444NotTransClipLeft(WORD* pDest, WORD pitch, RECT* pRect, MPalette &pal)
{
	BYTE	*pPixels;
	WORD	*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_pPixels[i];
		pDestTemp = pDest;		

		// (투명수,색깔수,색깔들)의 반복 수
		count = *pPixels++;		

		// 한 줄 출력		
		index = 0;
		
		//---------------------------------------------
		// 각 줄마다 Clipping을 해줘야 하는데...
		// xxxxOOOOOOOOOOOOOO인 경우이므로..
		//---------------------------------------------
		// xxxx부분까지 check해주는 루틴
		//---------------------------------------------
		// 한 줄 출력
		if (count > 0)
		{			
			j = count;
			do
			{
				transCount = *pPixels++;		// 투명색 수			
				colorCount = *pPixels++;		// 투명 아닌 색 수							
						
				// 투명색만큼 index증가			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						// 0을 출력한다.
						transCount = index - rectLeft;
						memset(pDestTemp, 0, transCount<<1);
						pDestTemp += transCount;		// 투명색만큼 건너 뛴다.
						
						// 이번 단계는 모두 출력
						memcpyAlpha4444(pDestTemp, pPixels, colorCount, pal);
						pDestTemp += colorCount;
						pPixels += (colorCount<<1);

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// 투명이 아닌 색들을 Surface에 출력한다.
						memcpyAlpha4444(pDestTemp, pPixels+(dist<<1), colorCount-dist, pal);
						pDestTemp += colorCount-dist;
						pPixels += (colorCount<<1);

						// 이제부터는 계속 출력한다.
						break;
					}
				}					

				// 투명이 아닌 색만큼 index증가				
				pPixels += (colorCount<<1);
				index += colorCount;
			} while (--j);

			//---------------------------------------------
			// 이제부터는 계속 출력한다.		
			//---------------------------------------------		
			if (--j > 0)
			{
				do
				{
					transCount = *pPixels++;		// 투명색 수			
					colorCount = *pPixels++;		// 투명 아닌 색 수			
							
					// 0을 출력한다.
					memset(pDestTemp, 0, transCount<<1);
					pDestTemp += transCount;		// 투명색만큼 건너 뛴다.
					
					// 투명색이 아닌만큼 출력해준다.
					memcpyAlpha4444(pDestTemp, pPixels, colorCount, pal);

					// memory addr 증가
					pDestTemp += colorCount;
					pPixels += (colorCount<<1);
				} while (--j);
			}
		}
		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt4444NotTrans ClipRight
//----------------------------------------------------------------------
// 오른쪽 clipping.  
// rectRight개 까지의 점만 pDest에 출력한다.
//----------------------------------------------------------------------
void
CAlphaSpritePal::Blt4444NotTransClipRight(WORD* pDest, WORD pitch, RECT* pRect, MPalette &pal)
{
	BYTE	*pPixels;
	WORD	*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index;

	register int	i;
	register int	j;

	int rectBottom = pRect->bottom;
	int rectRight = pRect->right;

	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_pPixels[i];
		pDestTemp = pDest;		

		// (투명수,색깔수,색깔들)의 반복 수
		count = *pPixels++;		

		// 한 줄 출력		
		index = 0;
			
		//---------------------------------------------
		// 각 줄마다 Clipping을 해줘야 하는데...		
		// OOOOOOOOOOOOOOxxxxx 이런 경우이다.
		//---------------------------------------------
		// OOOOOOOOOOOOOO까지만 출력해주면 된다.
		//---------------------------------------------
		// 한 줄 출력
		if (count > 0)
		{			
			j = count;
			do
			{
				transCount = *pPixels++;		// 투명색 수			
				colorCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가
				index += transCount;
				
				// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
				// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

				// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

				//---------------------------------------------
				// 오른쪽 끝까지 도달했을 경우
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// 투명색만으로 더 출력할 필요가 없을 때
					if (index > rectRight)
					{
						break;
					}
					// 투명색 아닌 것을 조금 출력해야 할 경우
					else
					{
						// 0을 출력한다.
						memset(pDestTemp, 0, transCount<<1);
						pDestTemp += transCount;		// 투명색만큼 건너 뛴다.
					
						// 투명이 아닌 색들을 Surface에 출력한다.
						memcpyAlpha4444(pDestTemp, pPixels, rectRight - index, pal);
						break;
					}
				}

				// 0을 출력한다.
				memset(pDestTemp, 0, transCount<<1);
				pDestTemp += transCount;		// 투명색만큼 건너 뛴다.

				// 출력
				memcpyAlpha4444(pDestTemp, pPixels, colorCount, pal);
				pDestTemp += colorCount;
				pPixels += (colorCount<<1);
				index += colorCount;
			} while (--j);
		}
		
		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt4444NotTrans ClipWidth
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// rectLeft개의 점을 건너띈 다음부터 pDest에 출력한다.
// rectRight까지..
//----------------------------------------------------------------------
void
CAlphaSpritePal::Blt4444NotTransClipWidth(WORD* pDest, WORD pitch, RECT* pRect, MPalette &pal)
{
	BYTE	*pPixels;
	WORD	*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_pPixels[i];
		pDestTemp = pDest;		

		// (투명수,색깔수,색깔들)의 반복 수
		count = *pPixels++;		

		// 한 줄 출력		
		index = 0;
		
		//---------------------------------------------
		// 각 줄마다 Clipping을 해줘야 하는데...
		// xxxxOOOOOOOOOOOOOO인 경우이므로..
		//---------------------------------------------
		// xxxx부분까지 check해주는 루틴
		//---------------------------------------------
		// 한 줄 출력
		if (count > 0)
		{			
			j = count;
			do
			{
				transCount = *pPixels++;		// 투명색 수			
				colorCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						transCount = index - rectLeft;
						// 0을 출력한다.
						memset(pDestTemp, 0, transCount<<1);
						pDestTemp += transCount;		// 투명색만큼 건너 뛴다.

						// 이번 단계는 모두 출력
						memcpyAlpha4444(pDestTemp, pPixels, colorCount, pal);
						pDestTemp += colorCount;
						pPixels += (colorCount<<1);
						index += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// 투명이 아닌 색들을 Surface에 출력한다.
						memcpyAlpha4444(pDestTemp, pPixels+(dist<<1), colorCount-dist, pal);
						pDestTemp += colorCount-dist;
						pPixels += (colorCount<<1);
						index += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}					

				// 투명이 아닌 색만큼 index증가				
				pPixels += (colorCount<<1);
				index += colorCount;
			} while (--j);

			//---------------------------------------------
			// 각 줄마다 Clipping을 해줘야 하는데...		
			// OOOOOOOOOOOOOOxxxxx 이런 경우이다.
			//---------------------------------------------
			// OOOOOOOOOOOOOO까지만 출력해주면 된다.
			//---------------------------------------------
			if (--j > 0)
			{
				do
				{
					transCount = *pPixels++;		// 투명색 수			
					colorCount = *pPixels++;		// 투명 아닌 색 수			
							
					// 투명색만큼 index증가
					index += transCount;
					
					// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
					// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

					// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

					//---------------------------------------------
					// 오른쪽 끝까지 도달했을 경우
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// 투명색만으로 더 출력할 필요가 없을 때
						if (index > rectRight)
						{
							break;
						}
						// 투명색 아닌 것을 조금 출력해야 할 경우
						else
						{
							// 0을 출력한다.
							memset(pDestTemp, 0, transCount<<1);
							pDestTemp += transCount;		// 투명색만큼 건너 뛴다.
						
							// 투명이 아닌 색들을 Surface에 출력한다.
							memcpyAlpha4444(pDestTemp, pPixels, rectRight - index, pal);
							break;
						}
					}

					// 0을 출력한다.
					memset(pDestTemp, 0, transCount<<1);
					pDestTemp += transCount;		// 투명색만큼 건너 뛴다.

					// 출력
					memcpyAlpha4444(pDestTemp, pPixels, colorCount, pal);
					pDestTemp += colorCount;
					pPixels += (colorCount<<1);
					index += colorCount;
				} while (--j);
			}
		}		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt4444NotTrans Clip Height
//----------------------------------------------------------------------
// pRect->top, rectBottom만큼만 출력한다.
//----------------------------------------------------------------------
void
CAlphaSpritePal::Blt4444NotTransClipHeight(WORD *pDest, WORD pitch, RECT* pRect, MPalette &pal)
{
	int		count,
			transCount,
			colorCount;

	WORD	*pDestTemp;
	BYTE	*pPixels;


	register int i;
	register int j;

	int rectBottom = pRect->bottom;

	for (int i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_pPixels[i];
		pDestTemp	= pDest;

		// (투명수,색깔수,색깔들)의 반복 수		
		count	= *pPixels++;		

		// 한 줄 출력
		// 한 줄 출력
		if (count > 0)
		{			
			j = count;
			do
			{	
				transCount = *pPixels++;		// 투명색만큼 건너 뛴다.
				colorCount = *pPixels++;		// 투명 아닌 색 수				

				// 0을 출력한다.
				memset(pDestTemp, 0, transCount<<1);
				pDestTemp += transCount;		// 투명색만큼 건너 뛴다.

				// 투명이 아닌 색들을 Surface에 출력한다.
				memcpyAlpha4444(pDestTemp, pPixels, colorCount, pal);
				
				pDestTemp	+= colorCount;
				pPixels		+= (colorCount<<1);
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}


//----------------------------------------------------------------------
// BltAlpha
//----------------------------------------------------------------------
// Clipping하지 않는다.
//----------------------------------------------------------------------
void
CAlphaSpritePal::BltAlpha(WORD *pDest, WORD pitch, BYTE alpha, MPalette &pal)
{
	s_Value1 = alpha;

	int	count,			
			colorCount;

	WORD	*pDestTemp;
	BYTE	*pPixels;

	register int i;
	register int j;
	int rectBottom = m_Height;

	if (rectBottom > 0)
	{
		i = rectBottom-1;
		pDest = (WORD*)((BYTE*)pDest + i*pitch);

		do
		{
			pPixels		= m_pPixels[i];
			pDestTemp	= pDest;

			// (투명수,색깔수,색깔들)의 반복 수		
			count	= *pPixels++;		

			// 한 줄 출력
			if (count > 0)
			{			
				j = count;
				do
				{
					pDestTemp += *pPixels++;		// 투명색만큼 건너 뛴다.
					colorCount = *pPixels++;		// 투명 아닌 색 수				

					// 투명이 아닌 색들을 Surface에 출력한다.
					memcpyAlphaValue(pDestTemp, pPixels, colorCount, pal);
					
					pDestTemp	+= colorCount;

					// 실제 점 개수는 alpha값 때문에 2배이다.
					pPixels		+= (colorCount<<1);
				} while (--j);
			}

			pDest = (WORD*)((BYTE*)pDest - pitch);
		} while (i--);
	}
}

//----------------------------------------------------------------------
// BltAlpha ClipLeft
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// rectLeft개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CAlphaSpritePal::BltAlphaClipLeft(WORD* pDest, WORD pitch, RECT* pRect, BYTE alpha, MPalette &pal)
{
	s_Value1 = alpha;

	BYTE	*pPixels;
	WORD	*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int	count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_pPixels[i];
		pDestTemp = pDest;		

		// (투명수,색깔수,색깔들)의 반복 수
		count = *pPixels++;		

		// 한 줄 출력		
		index = 0;
		
		//---------------------------------------------
		// 각 줄마다 Clipping을 해줘야 하는데...
		// xxxxOOOOOOOOOOOOOO인 경우이므로..
		//---------------------------------------------
		// xxxx부분까지 check해주는 루틴
		//---------------------------------------------
		// 한 줄 출력
		if (count > 0)
		{			
			j = count;
			do
			{
				transCount = *pPixels++;		// 투명색 수			
				colorCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - rectLeft;

						// 이번 단계는 모두 출력
						memcpyAlphaValue(pDestTemp, pPixels, colorCount, pal);
						pDestTemp += colorCount;
						pPixels += (colorCount<<1);

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// 투명이 아닌 색들을 Surface에 출력한다.
						memcpyAlphaValue(pDestTemp, pPixels+(dist<<1), colorCount-dist, pal);
						pDestTemp += colorCount-dist;
						pPixels += (colorCount<<1);

						// 이제부터는 계속 출력한다.
						break;
					}
				}					

				// 투명이 아닌 색만큼 index증가				
				pPixels += (colorCount<<1);
				index += colorCount;
			} while (--j);

			//---------------------------------------------
			// 이제부터는 계속 출력한다.		
			//---------------------------------------------		
			if (--j > 0)
			{
				do
				{
					transCount = *pPixels++;		// 투명색 수			
					colorCount = *pPixels++;		// 투명 아닌 색 수			
							
					// 투명색만큼 건너 뛴다.
					pDestTemp += transCount;			
					
					// 투명색이 아닌만큼 출력해준다.
					memcpyAlphaValue(pDestTemp, pPixels, colorCount, pal);

					// memory addr 증가
					pDestTemp += colorCount;
					pPixels += (colorCount<<1);
				} while (--j);
			}
		}
		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltAlpha ClipRight
//----------------------------------------------------------------------
// 오른쪽 clipping.  
// rectRight개 까지의 점만 pDest에 출력한다.
//----------------------------------------------------------------------
void
CAlphaSpritePal::BltAlphaClipRight(WORD* pDest, WORD pitch, RECT* pRect, BYTE alpha, MPalette &pal)
{
	s_Value1 = alpha;

	BYTE	*pPixels;
	WORD	*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int	count,
			transCount, 
			colorCount,
			index;

	register int	i;
	register int	j;

	int rectBottom = pRect->bottom;
	int rectRight = pRect->right;

	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_pPixels[i];
		pDestTemp = pDest;		

		// (투명수,색깔수,색깔들)의 반복 수
		count = *pPixels++;		

		// 한 줄 출력		
		index = 0;
			
		//---------------------------------------------
		// 각 줄마다 Clipping을 해줘야 하는데...		
		// OOOOOOOOOOOOOOxxxxx 이런 경우이다.
		//---------------------------------------------
		// OOOOOOOOOOOOOO까지만 출력해주면 된다.
		//---------------------------------------------
		// 한 줄 출력
		if (count > 0)
		{			
			j = count;
			do
			{
				transCount = *pPixels++;		// 투명색 수			
				colorCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가
				index += transCount;
				
				// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
				// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

				// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

				//---------------------------------------------
				// 오른쪽 끝까지 도달했을 경우
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// 투명색만으로 더 출력할 필요가 없을 때
					if (index > rectRight)
					{
						break;
					}
					// 투명색 아닌 것을 조금 출력해야 할 경우
					else
					{
						pDestTemp += transCount;
					
						// 투명이 아닌 색들을 Surface에 출력한다.
						memcpyAlphaValue(pDestTemp, pPixels, rectRight - index, pal);
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				// 출력
				memcpyAlphaValue(pDestTemp, pPixels, colorCount, pal);
				pDestTemp += colorCount;
				pPixels += (colorCount<<1);
				index += colorCount;
			} while (--j);
		}
		
		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltAlpha ClipWidth
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// rectLeft개의 점을 건너띈 다음부터 pDest에 출력한다.
// rectRight까지..
//----------------------------------------------------------------------
void
CAlphaSpritePal::BltAlphaClipWidth(WORD* pDest, WORD pitch, RECT* pRect, BYTE alpha, MPalette &pal)
{
	s_Value1 = alpha;

	BYTE	*pPixels;
	WORD	*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int	count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_pPixels[i];
		pDestTemp = pDest;		

		// (투명수,색깔수,색깔들)의 반복 수
		count = *pPixels++;		

		// 한 줄 출력		
		index = 0;
		
		//---------------------------------------------
		// 각 줄마다 Clipping을 해줘야 하는데...
		// xxxxOOOOOOOOOOOOOO인 경우이므로..
		//---------------------------------------------
		// xxxx부분까지 check해주는 루틴
		//---------------------------------------------
		// 한 줄 출력
		if (count > 0)
		{			
			j = count;
			do
			{
				transCount = *pPixels++;		// 투명색 수			
				colorCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - rectLeft;

						// 이번 단계는 모두 출력
						// 오른쪽 끝을 넘어가는 경우..
						if (index+colorCount > rectRight)
						{							
							// 투명색만으로 오른쪽 끝 넘어가는 경우
							if (index > rectRight)
							{
							}
							else
							{
								//memset((void*)pDestTemp, 0, (rectRight - index)<<1);
								memcpyAlphaValue(pDestTemp, pPixels, (rectRight - index), pal);
							}

							j = 0;
							
							break;
						}

						memcpyAlphaValue(pDestTemp, pPixels, colorCount, pal);
						pDestTemp += colorCount;
						pPixels += (colorCount<<1);
						index += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// 투명이 아닌 색들을 Surface에 출력한다.
						// 오른쪽 끝을 넘어가는 경우..
						if (index+colorCount > rectRight)
						{
							memcpyAlphaValue(pDestTemp, pPixels+(dist<<1), (rectRight - rectLeft), pal);

							j = 0;						
							break;
						}		

						memcpyAlphaValue(pDestTemp, pPixels+(dist<<1), colorCount-dist, pal);
						pDestTemp += colorCount-dist;
						pPixels += (colorCount<<1);
						index += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}					

				// 투명이 아닌 색만큼 index증가				
				pPixels += (colorCount<<1);
				index += colorCount;
			} while (--j);

			//---------------------------------------------
			// 각 줄마다 Clipping을 해줘야 하는데...		
			// OOOOOOOOOOOOOOxxxxx 이런 경우이다.
			//---------------------------------------------
			// OOOOOOOOOOOOOO까지만 출력해주면 된다.
			//---------------------------------------------
			if (--j > 0)
			{
				do
				{
					transCount = *pPixels++;		// 투명색 수			
					colorCount = *pPixels++;		// 투명 아닌 색 수			
							
					// 투명색만큼 index증가
					index += transCount;
					
					// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
					// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

					// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

					//---------------------------------------------
					// 오른쪽 끝까지 도달했을 경우
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// 투명색만으로 더 출력할 필요가 없을 때
						if (index > rectRight)
						{
							break;
						}
						// 투명색 아닌 것을 조금 출력해야 할 경우
						else
						{
							pDestTemp += transCount;
						
							// 투명이 아닌 색들을 Surface에 출력한다.
							memcpyAlphaValue(pDestTemp, pPixels, rectRight - index, pal);
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					// 출력
					memcpyAlphaValue(pDestTemp, pPixels, colorCount, pal);
					pDestTemp += colorCount;
					pPixels += (colorCount<<1);
					index += colorCount;
				} while (--j);
			}
		}		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltAlpha Clip Height
//----------------------------------------------------------------------
// pRect->top, rectBottom만큼만 출력한다.
//----------------------------------------------------------------------
void
CAlphaSpritePal::BltAlphaClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha, MPalette &pal)
{
	s_Value1 = alpha;

	int	count,			
			colorCount;

	WORD	*pDestTemp;
	BYTE	*pPixels;


	register int i;
	register int j;

	int rectBottom = pRect->bottom;

	for (int i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_pPixels[i];
		pDestTemp	= pDest;

		// (투명수,색깔수,색깔들)의 반복 수		
		count	= *pPixels++;		

		// 한 줄 출력
		// 한 줄 출력
		if (count > 0)
		{			
			j = count;
			do
			{
				pDestTemp += *pPixels++;		// 투명색만큼 건너 뛴다.
				colorCount = *pPixels++;		// 투명 아닌 색 수				

				// 투명이 아닌 색들을 Surface에 출력한다.
				memcpyAlphaValue(pDestTemp, pPixels, colorCount, pal);
				
				pDestTemp	+= colorCount;
				pPixels		+= (colorCount<<1);
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}


//----------------------------------------------------------------------
// AlphaChannel Copy
//----------------------------------------------------------------------
// Alpha값 : 1~32
//----------------------------------------------------------------------
// pSource의 것을 pDest에 출력을 해야한다.
// pSource의 구성은 (alpha,색깔 하나)의 pixels만큼 반복이다.
//----------------------------------------------------------------------
void	
CAlphaSpritePal::memcpyAlphaValue(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register WORD		sTemp,dTemp;
	register int		sr,sg,sb,dr,dg,db;
	//static WORD		temp, temp2;

	register int i = pixels;

	//BYTE alpha;

	// Alpha Channel Blending
	// 한점씩 찍기
	while (i--)
	{	
		// Source에는 Alpha값이 포함되어 있다.
		//alpha = *pSource >> 8;
		pSource++;

		// 한점 찍기
		sTemp = pal[*pSource];
		dTemp = *pDest;

		sr = CDirectDraw::Red(sTemp);
		sg = CDirectDraw::Green(sTemp);
		sb = CDirectDraw::Blue(sTemp);

		dr = CDirectDraw::Red(dTemp);
		dg = CDirectDraw::Green(dTemp);
		db = CDirectDraw::Blue(dTemp);		
		
		*pDest = ((s_Value1 * (sb - db) >> 5) + db |
					((s_Value1 * (sg - dg) >> 5) + dg) << CDirectDraw::s_bSHIFT_G |
					((s_Value1 * (sr - dr) >> 5) + dr) << CDirectDraw::s_bSHIFT_R);
	
		/*
		// 잉... 이게 더 느리다.. 왜 그렇지.. - -;;;
		temp = sb-db;
		temp *= alpha;
		temp >>= 5;
		temp += db;

		temp2 = sg-dg;
		temp2 *= alpha;
		temp2 >>= 5;
		temp2 += dg;
		temp2 <<= 5;

		temp |= temp2;

		temp2 = sr-dr;
		temp2 *= alpha;
		temp2 >>= 5;
		temp2 += dr;
		temp2 <<= CDirectDraw::s_bSHIFT_R;

		temp |= temp2;

		*pDest = temp;
		*/

		pDest++;
		pSource++;
	}
}


//----------------------------------------------------------------------
// Blt4444SmallNotTrans
//----------------------------------------------------------------------
// 축소해서 출력.. 
// Clipping하지 않는다.
//----------------------------------------------------------------------
// shift로 인해서 빠진 점들이 쌓이다보면
// 길이 차이가 많이 나게 되는데,
// 그걸 보정해줘야 한다.
// 계산이 좀 많은데.. T_T;;
// 줄여진 길이를 다시 늘리면(-_-;)
// 부족한 점의 개수를 알 수 있다.					
//----------------------------------------------------------------------
void
CAlphaSpritePal::Blt4444SmallNotTrans(WORD *pDest, WORD pitch, BYTE shift, MPalette &pal)
{
	s_Value1 = shift;
	// memcpy에서 하나의 alpha값을 제외하고 +하는 값
	s_Value2 = (2 << shift) - 1; //((1 << s_Value1) << 1) - 1;	


	int		count,	
			transCount,
			colorCount,
			transCountShift,
			colorCountShift,
			totalCount,
			totalShiftCount,
			pixelGap,
			pixelGapShift;

	WORD	*pDestTemp;
	BYTE	*pPixels;

	register int i;
	register int j;

	int rectBottom = m_Height;

	if (rectBottom > 0)
	{
		i = rectBottom-1;
		int stepY = 1 << shift;		// y줄 건너띄는 pixel수
		pDest = (WORD*)((BYTE*)pDest + (i>>shift)*pitch);

		do
		{
			pPixels		= m_pPixels[i];
			pDestTemp	= pDest;

			// (투명수,색깔수,색깔들)의 반복 수		
			count	= *pPixels++;		

			// 한 줄 출력
			totalCount = 0;
			totalShiftCount = 0;
			if (count > 0)
			{			
				j = count;
				do
				{		
					transCount = *pPixels++;					
					colorCount = *pPixels++;		// 투명 아닌 색 수	
					
					//--------------------------------------------------
					// shift만큼 줄여진 값을 계산한다.
					//--------------------------------------------------
					transCountShift = transCount >> shift;
					colorCountShift = colorCount >> shift;

					//--------------------------------------------------
					//				투명 부분 길이 보정
					//--------------------------------------------------
					// 실제 size의 pixel수..
					//--------------------------------------------------
					totalCount += transCount;
					totalShiftCount += transCountShift;

					// 실제pixel - shift해서 늘린 pixel(-_-;)
					pixelGap = totalCount - (totalShiftCount << shift);

					// gap을 다시 shift해서 더해준다.
					pixelGapShift = pixelGap >> shift;
					transCountShift += pixelGapShift;
					totalShiftCount += pixelGapShift;

					
					//--------------------------------------------------
					// 0을 출력한다. 투명색이다..
					//--------------------------------------------------
					memset(pDestTemp, 0, transCountShift<<1);
					pDestTemp += transCountShift;		// 투명색만큼 건너 뛴다.


					//--------------------------------------------------
					//				색깔 부분 길이 보정
					//--------------------------------------------------
					// (!!!) 근데 여기서 문제가 있다.
					// 투명색이야 그냥 투명색인데..
					// 색깔이 있는 부분에서는 
					// 점 몇개를 더 찍어야될 경우..
					// 어떤 점을 출력할까?? 흠..
					// 이대로라면.. 
					// 부족한 색깔만큼 그냥 건너띌 뿐.. 흠..- -;
					//
					// 그래서, 일단 제외.. - -;
					//--------------------------------------------------
					totalCount += colorCount;
					totalShiftCount += colorCountShift;

					//--------------------------------------------------
					// 투명이 아닌 색들을 Surface에 출력한다.
					//--------------------------------------------------					
					memcpyAlpha4444Small(pDestTemp, pPixels, colorCount, pal);
					
					pDestTemp	+= colorCountShift;

					// 실제 점 개수는 alpha값 때문에 2배이다.
					pPixels		+= (colorCount<<1);
				} while (--j);
			}

			pDest = (WORD*)((BYTE*)pDest - pitch);

			i-=stepY;

		} while (i>0);
	}
}


//----------------------------------------------------------------------
// AlphaChannel Copy  4444 Small
//----------------------------------------------------------------------
// Alpha값 : 1~32
//----------------------------------------------------------------------
// pSource의 것을 pDest에 출력을 해야한다.
// pSource의 구성은 (alpha,색깔 하나)의 pixels만큼 반복이다.
//
// A:R:G:B = 4:4:4:4 Texture를 위한 것이다.
//----------------------------------------------------------------------
void	
CAlphaSpritePal::memcpyAlpha4444Small(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register WORD		sTemp;
	register int		sr,sg,sb;
	//static WORD		temp, temp2;

	register int i = pixels >> s_Value1;

	BYTE alpha;
	
	// Alpha Channel Blending
	// 한점씩 찍기
	while (i--)
	{	
		// Source에는 Alpha값이 포함되어 있다.
		alpha = *pSource >> 1;	//	alpha = (*pSource >> 8) >> 1;	4 bit이기 때문에..
		pSource++;

		// 한점 찍기
		sTemp = pal[*pSource];
	
		sr = (sTemp >> CDirectDraw::s_bSHIFT4_R);// & 0x0F;
		sg = (sTemp >> CDirectDraw::s_bSHIFT4_G) & 0x0F;
		sb = (sTemp >> CDirectDraw::s_bSHIFT4_B) & 0x0F;
		
		*pDest =	(alpha << 12) |
					(sr << 8) |
					(sg << 4) |
					(sb);
	
		pDest++;
		pSource += s_Value2;
	}
}

bool
CAlphaSpritePal::IsColorPixel(short x, short y)
{
	if( y >= m_Height || y < 0 || x < 0 || x >= m_Width )
		return false;

	int	colorCount, count;
	int offset=0;

	const BYTE	*pPixels = m_pPixels[y];
	
	register int j;
	count	= *pPixels++;		
	if (count > 0)
	{	
		j = count;
		do {
			offset += *pPixels++;
			if( offset > x ) return false;
			colorCount = *pPixels++;		// 투명 아닌 색 수				
			offset += colorCount;
			if( offset > x ) return true;
			pPixels		+= (colorCount<<1);
		} while (--j);
	}
	return false;
}

WORD
CAlphaSpritePal::GetPixel( short x, short y, MPalette &pal )
{
	if( y >= m_Height || y < 0 || x < 0 || x >= m_Width )
		return 0;
	
	const BYTE *pPixels = m_pPixels[y];
	int offset=0, colorCount,count;
	register int j;

	count = *pPixels++;

	if(count > 0 )
	{
		j = count;

		do {
			offset += *pPixels++;			// 투명색부분 건너 뜀
			if( offset > x ) return 0;			//  투명색 부분이면 0을..
			colorCount = *pPixels++;
			offset += colorCount;
			if( offset > x )		// 이 안에 색이 있으면
			{
				offset -= colorCount;
				pPixels ++;		// 알파값은 건너뛰고				
				pPixels += ((x - offset)<<1);
				return pal[*pPixels];
			}
			pPixels += (colorCount<<1);
		} while(--j);
	}

	return 0;
}
