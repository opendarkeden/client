#include "client_PCH.h"
#include "CSpritePal.h"
#include "CSpriteSurface.h"


DWORD	CSpritePal::s_Value1 = 1;
DWORD	CSpritePal::s_Value2 = 31;
DWORD	CSpritePal::s_Value3 = 1;

void CSpritePal::SetPixel(BYTE *pSource, WORD pitch, WORD width, WORD height)
{
	// memory해제
	Release();

	m_Width = width;
	m_Height = height;

	// 일단 memory를 적당히 잡아둔다.	
	BYTE*	data = new BYTE[m_Width*2+10];

	int		index;				// data의 index로 사용
	int		count;				// 반복수
	int		trans,				// 투명색 개수
			color;				// 투명이 아닌색 개수

	BOOL	bCheckTrans;		// 최근에 검사한게 투명색인가?

	BYTE	*pSourceTemp;


	// height줄 만큼 memory잡기
	m_pPixels = new BYTE* [height];
	BYTE **Pixels = new BYTE* [height];
	std::vector<int> PixelSize;

	register int i;
	register int j;

	for (i=0; i<height; i++)
	{
		index = 0;
		count = 0;
		trans = 0;
		color = 0;
		bCheckTrans = TRUE;
		pSourceTemp = pSource;

//		// 각 line에 대해서 압축~
//		for (j=0; j<width; j++)
//		{
//			// 0번 color에 대해서 압축
//			if (*pSourceTemp==s_Colorkey)
//			{
//				// 최근에 검사한게 투명색이 아니었다면
//				if (!bCheckTrans)
//				{
//					// ' (투명,색깔수,색깔들) '의 한 set가 끝났음을 의미하므로
//					count++;					
//					data[lastColorIndex] = color;
//					color = 0;
//					bCheckTrans = TRUE;
//				}				
//				trans++;				
//			}
//			else
//			{
//				// 최근에 검사한게 투명색이었다면..
//				if (bCheckTrans)
//				{						
//					data[index++] = trans;		// 상위 byte에 투명수를 넣는다.
//					trans = 0;
//					lastColorIndex=index++;			// 색깔수를 넣을 위치를 기억
//					bCheckTrans = FALSE;
//				}
//				data[index++] = *pSourceTemp;	// 실제 색깔을 저장한다.
//				color++;								
//			}
//			pSourceTemp++;
//		}
		
		for( j=0; j<width;)
		{
			int trans_count = 0;
			int image_count = 0;
			
			while( *(pSourceTemp+j) == s_Colorkey )
			{
				if( j >= width  || trans_count == 255)
					break;
				trans_count++;
				j++;				
			}
			count++;
			data[index++] = trans_count;
			
			while( *(pSourceTemp + j) != s_Colorkey )
			{
				if( j>= width || image_count == 255 )
					break;
				image_count ++;
				j++;
				
			}
			data[index++] = image_count;
			if( image_count > 0)
			{
				memcpy(data+index, pSourceTemp+(j-image_count), image_count);
				index+=image_count;
			}			
		}
		
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

		pSource = pSource + pitch;
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

void CSpritePal::Blt(int x, int y, WORD* pDest, int pitch, MPalette &pal)
{
	int	count,			
		colorCount;
	
	WORD	*pDestTemp;
	BYTE	*pPixels;
	
	register int i;
	register int j;
	register int k;
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
					for(k = 0; k < colorCount; k++)
					{
						memcpy((void*)(pDestTemp+k), (void*)(&pal[*(pPixels+k)]), 2);
					}
					
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;
				} while (--j);
			}
			
		} while (i++ < rectBottom-1);
	}
}

//----------------------------------------------------------------------
// BltEffect
//----------------------------------------------------------------------
// Clipping하지 않는다.
//----------------------------------------------------------------------
void
CSpritePal::BltEffect(WORD *pDest, WORD pitch, MPalette &pal)
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
			if (count > 0)
			{
				j = count;
				do 
				{
					pDestTemp += *pPixels++;			// 투명색만큼 건너 뛴다.
					colorCount = *pPixels++;		// 투명 아닌 색 수				

					// 투명이 아닌 색들을 Surface에 출력한다.
					CSpriteSurface::memcpyPalEffect(pDestTemp, pPixels, colorCount, pal);
					
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;
				} while (--j);
			}

			pDest = (WORD*)((BYTE*)pDest - pitch);
		} while (i--);
	}
}

//----------------------------------------------------------------------
// BltEffect ClipLeft
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CSpritePal::BltEffectClipLeft(WORD* pDest, WORD pitch, RECT* pRect, MPalette &pal)
{
	WORD	*pDestTemp;
	BYTE	*pPixels;

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

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	for (i=pRect->top; i<rectBottom; i++)
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
						CSpriteSurface::memcpyPalEffect(pDestTemp, pPixels, colorCount, pal);
						pDestTemp += colorCount;
						pPixels += colorCount;

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
						CSpriteSurface::memcpyPalEffect(pDestTemp, pPixels+dist, colorCount-dist, pal);					
						pDestTemp += colorCount-dist;
						pPixels += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}					

				// 투명이 아닌 색만큼 index증가				
				pPixels += colorCount;
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
					CSpriteSurface::memcpyPalEffect(pDestTemp, pPixels, colorCount, pal);

					// memory addr 증가
					pDestTemp += colorCount;
					pPixels += colorCount;			
				} while (--j);
			}
		}
		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltEffect ClipRight
//----------------------------------------------------------------------
// 오른쪽 clipping.  
// pRect->right개 까지의 점만 pDest에 출력한다.
//----------------------------------------------------------------------
void
CSpritePal::BltEffectClipRight(WORD* pDest, WORD pitch, RECT* pRect, MPalette &pal)
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
	for (i=pRect->top; i<rectBottom; i++)
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
						CSpriteSurface::memcpyPalEffect(pDestTemp, pPixels, rectRight - index, pal);
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				// 출력
				CSpriteSurface::memcpyPalEffect(pDestTemp, pPixels, colorCount, pal);
				pDestTemp += colorCount;
				pPixels += colorCount;			
				index += colorCount;
			} while (--j);
		}
		
		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltEffect ClipWidth
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
// rectRight까지..
//----------------------------------------------------------------------
void
CSpritePal::BltEffectClipWidth(WORD* pDest, WORD pitch, RECT* pRect, MPalette &pal)
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

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;
	for (i=pRect->top; i<rectBottom; i++)
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

						// 이번 단계는 모두 출력?
						// 오른쪽 끝을 넘어가는 경우..
						if (index+colorCount > rectRight)
						{							
							// 투명색만으로 오른쪽 끝 넘어가는 경우
							if (index > rectRight)
							{
							}
							else
							{
								CSpriteSurface::memcpyPalEffect(pDestTemp, pPixels, rectRight - index, pal);
							}

							j = 0;
							
							break;
						}

						// 이번 단계는 모두 출력
						CSpriteSurface::memcpyPalEffect(pDestTemp, pPixels, colorCount, pal);
						pDestTemp += colorCount;
						pPixels += colorCount;
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

						// 오른쪽 끝을 넘어가는 경우..
						if (index+colorCount > rectRight)
						{
							CSpriteSurface::memcpyPalEffect(pDestTemp, pPixels+dist, rectRight - rectLeft, pal);

							j = 0;						
							break;
						}		

						// 투명이 아닌 색들을 Surface에 출력한다.
						CSpriteSurface::memcpyPalEffect(pDestTemp, pPixels+dist, colorCount-dist, pal);					
						pDestTemp += colorCount-dist;
						pPixels += colorCount;
						index += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}					

				// 투명이 아닌 색만큼 index증가				
				pPixels += colorCount;
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
							CSpriteSurface::memcpyPalEffect(pDestTemp, pPixels, rectRight - index, pal);
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					// 출력
					CSpriteSurface::memcpyPalEffect(pDestTemp, pPixels, colorCount, pal);
					pDestTemp += colorCount;
					pPixels += colorCount;			
					index += colorCount;
				} while (--j);
			}		
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltEffect Clip Height
//----------------------------------------------------------------------
// pRect->top, pRect->bottom만큼만 출력한다.
//----------------------------------------------------------------------
void
CSpritePal::BltEffectClipHeight(WORD *pDest, WORD pitch, RECT* pRect, MPalette &pal)
{
	int		count,			
			colorCount;

	WORD	*pDestTemp;
	BYTE	*pPixels;


	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	for (i=pRect->top; i<rectBottom; i++)
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
				CSpriteSurface::memcpyPalEffect(pDestTemp, pPixels, colorCount, pal);
				
				pDestTemp	+= colorCount;
				pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
} 

//----------------------------------------------------------------------
// Blt1555NotTrans
//----------------------------------------------------------------------
// Clipping하지 않는다.
//----------------------------------------------------------------------
void
CSpritePal::Blt1555NotTrans(WORD *pDest, WORD pitch, MPalette &pal)
{
	//s_Value1 = alpha >> 4;	// 1 bit이므로
	
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
					memcpy1555(pDestTemp, pPixels, colorCount, pal);
					
					pDestTemp	+= colorCount;
					
					pPixels		+= colorCount;
				} while (--j);
			}
			
			pDest = (WORD*)((BYTE*)pDest - pitch);
		} while (i--);
	}
}

//----------------------------------------------------------------------
// Blt1555SmallNotTrans
//----------------------------------------------------------------------
// 축소해서 출력.. 
// Clipping하지 않는다.
//
// alpha값은 50%(반투명)으로 한다.
//----------------------------------------------------------------------
void
CSpritePal::Blt1555SmallNotTrans(WORD *pDest, WORD pitch, BYTE shift, MPalette &pal)
{
	//s_Value1 = alpha >> 4;	// 1 bit이므로
	s_Value2 = shift;
	// memcpy에서 건너띄는 값
	s_Value3 = 1 << shift;
	
	
	int		count,	
		transCount,
		colorCount,
		transCountShift,
		colorCountShift;
	
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
			if (count > 0)
			{			
				j = count;
				do
				{		
					transCount = *pPixels++;					
					colorCount = *pPixels++;		// 투명 아닌 색 수	
					
					// shift만큼 줄여진 값을 계산한다.
					transCountShift = transCount >> shift;
					colorCountShift = colorCount >> shift;
					
					// 0을 출력한다.
					memset(pDestTemp, 0, transCountShift<<1);
					pDestTemp += transCountShift;		// 투명색만큼 건너 뛴다.
					
					// 투명이 아닌 색들을 Surface에 출력한다.
					memcpy1555Small(pDestTemp, pPixels, colorCount, pal);
					
					pDestTemp	+= colorCountShift;
					
					pPixels		+= colorCount;
				} while (--j);
			}
			
			pDest = (WORD*)((BYTE*)pDest - pitch);
			
			i-=stepY;
			
		} while (i>0);
	}
}

//----------------------------------------------------------------------
// AlphaChannel Copy  1555
//----------------------------------------------------------------------
// Alpha값 : 의미없다.
//----------------------------------------------------------------------
// pSource의 것을 pDest에 출력을 해야한다.
// pSource의 구성은 (alpha,색깔 하나)의 pixels만큼 반복이다.
//
// A:R:G:B = 1:5:5:5 Texture를 위한 것이다.
//----------------------------------------------------------------------
void	
CSpritePal::memcpy1555(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int		sTemp;
	register int		sr,sg,sb;
	//static WORD		temp, temp2;
	
	register int i = pixels;
	
	// Alpha Channel Blending
	// 한점씩 찍기
	while (i--)
	{	
		// 한점 찍기
		sTemp = pal[*pSource];
		
		sr = (sTemp >> CDirectDraw::s_bSHIFT_R);// & 0x0F;
		sg = (sTemp >> CDirectDraw::s_bSHIFT_G) & 0x0000001F;
		sb = (sTemp >> CDirectDraw::s_bSHIFT_B) & 0x0000001F;
		
		*pDest =	0x8000 |
			(sr << 10) |
			(sg << 5) |
			(sb);
		
		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// Alpha Copy  1555 Small
//----------------------------------------------------------------------
// Alpha값 : 의미없다 - -;
//----------------------------------------------------------------------
// pSource의 것을 pDest에 출력을 해야한다.
// pSource의 구성은 (alpha,색깔 하나)의 pixels만큼 반복이다.
//
// A:R:G:B = 1:5:5:5 Texture를 위한 것이다.
//
// s_Value2는 shift값
// s_Value3은 건너띄는 값(맞나? 기억이 가물가물. - -;;)
//----------------------------------------------------------------------
void	
CSpritePal::memcpy1555Small(WORD* pDest, BYTE* pSource, WORD pixels, MPalette &pal)
{
	register int		sTemp;
	register int		sr,sg,sb;
	//static WORD		temp, temp2;
	
	register int i = pixels >> s_Value2;
	
	// 한점씩 찍기
	while (i--)
	{	
		// 한점 찍기
		sTemp = pal[*pSource];
		
		sr = (sTemp >> CDirectDraw::s_bSHIFT_R);// & 0x0F;
		sg = (sTemp >> CDirectDraw::s_bSHIFT_G) & 0x0000001F;
		sb = (sTemp >> CDirectDraw::s_bSHIFT_B) & 0x0000001F;
		
		*pDest =	0x8000 | //(s_Value1 << 15) |
			(sr << 10) |
			(sg << 5) |
			(sb);
		
		pDest++;
		pSource += s_Value3;
	}
}

bool
CSpritePal::IsColorPixel( short x, short y )
{
	if( x >= m_Width || x< 0 || y>= m_Height || y< 0)
		return false;

	int	count, colorCount , offset=0;
	
	const BYTE	*pPixels = m_pPixels[y];
	
	register int j;
	
	count	= *pPixels++;		
	// 한 줄 출력
	if (count > 0)
	{	
		j = count;
		do {
			offset += *pPixels++;			// 투명색만큼 건너 뛴다.
			if( offset > x ) return false;
			colorCount = *pPixels++;	// 투명 아닌 색 수						
			offset += colorCount;
			if( offset > x ) return true;
			pPixels	+= colorCount;
		} while (--j);
	}	
	return false;
}

WORD
CSpritePal::GetPixel( short x, short y, MPalette &pal )
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
				pPixels += (x - offset);
				return pal[*pPixels];
			}
			pPixels += colorCount;
		} while(--j);
	}

	return 0;
}
