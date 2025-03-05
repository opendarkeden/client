//----------------------------------------------------------------------
// CAlphaSprite.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "CSpriteSurface.h"
#include "CAlphaSprite.h"


//----------------------------------------------------------------------
// Static member
//----------------------------------------------------------------------
WORD	CAlphaSprite::s_Colorkey = 0;
int		CAlphaSprite::s_Value1	= 0;
int		CAlphaSprite::s_Value2	= 0;
//BYTE	CAlphaSprite::s_Value2 = 31;



//----------------------------------------------------------------------
//
// constructor/destructore
//
//----------------------------------------------------------------------

CAlphaSprite::CAlphaSprite()
{
	m_Width		= 0;
	m_Height	= 0;
	m_Pixels	= NULL;	
	m_bInit		= false;
}

CAlphaSprite::~CAlphaSprite()
{
	Release();
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// m_Pixels의 memory를 해제한다.
//----------------------------------------------------------------------
void	
CAlphaSprite::Release()
{
	if (m_Pixels!=NULL)
	{		
		m_bInit		= false;

		for (int i=0; i<m_Height; i++)
			delete [] m_Pixels[i];
			//free(m_Pixels[i]);
		delete [] m_Pixels;
		//free(m_Pixels);

		//s_Delete++;

		m_Pixels	= NULL;
		m_Width		= 0;
		m_Height	= 0;	
	}
}

//----------------------------------------------------------------------
// Assign Operator
//----------------------------------------------------------------------
void
CAlphaSprite::operator = (const CAlphaSprite& Sprite)
{
		// 메모리 해제
	Release();


	// NULL이면 저장하지 않는다.
	if (Sprite.m_Pixels==NULL || Sprite.m_Width==0 || Sprite.m_Height==0)
		return;

	// 크기 설정
	m_Width = Sprite.m_Width;
	m_Height = Sprite.m_Height;
	
	// 압축 된 것 저장
	int index;	
	int i;
	int j;

	// 메모리 잡기
	m_Pixels = new WORD* [m_Height];

	for (i=0; i<m_Height; i++)
	{
		// 반복 회수의 2 byte
		int	count = Sprite.m_Pixels[i][0], 
				colorCount;
		index	= 1;

		// 각 line마다 byte수를 세어서 저장해야한다.
		for (j=0; j<count; j++)
		{
			//transCount = m_Pixels[i][index];
			colorCount = Sprite.m_Pixels[i][index+1];

			index+=2;	// 두 count 만큼

			index += (colorCount<<1);	// 투명색 아닌것만큼 +				
		}

		// 메모리 잡기
		m_Pixels[i] = new WORD [index];
		memcpy(m_Pixels[i], Sprite.m_Pixels[i], index<<1);
	}

	// 복사 완료
	m_bInit = true;
}

//----------------------------------------------------------------------
// CDirectDrawSurface의 (x,y)+(width, height)영역을 읽어서 m_Pixels에 저장한다.
//----------------------------------------------------------------------
// m_Pixels를 0번 압축 Format으로 바꾼다.
//
// 각 line마다 다음과 같은 구조를 가진다.
//
// [반복수] (투명수,색깔수,(alpha,색깔)(alpha,색깔)....)(투명수,색깔수,(alpha,색깔)(alpha,색깔)....)........
//
// 반복수는 2 bytes이고
// 투명수와 색깔수는 각각 2 byte이고
// 색깔들은 각각 2 bytes씩이다.
//
// alpha값은 2byte인데
// 상위 1byte는 원래 alpha값
// 하위 1byte는 32-alpha값
// (이렇게 하는 이유는... 2byte씩 맞추는데 1byte가 남다보니.. *_*)
//----------------------------------------------------------------------
// Source에서 투명색 압축을 하고
// Filter를 Alpha값으로 해서 함께 저장한다. 
//----------------------------------------------------------------------
void
CAlphaSprite::SetPixel(WORD *pSource, WORD sourcePitch, 
						WORD *pFilter, WORD filterPitch, 
						WORD width, WORD height)
{
	// memory해제
	Release();

	m_Width = width;
	m_Height = height;

	// 일단 memory를 적당히 잡아둔다.	
	WORD*	data = new WORD[m_Width*4+10];

	int	index,				// data의 index로 사용
			lastColorIndex;		// 투명이 아닌색 개수의 최근 index
	int	count;				// 반복수
	int	trans,				// 투명색 개수
			color;				// 투명이 아닌색 개수

	BOOL	bCheckTrans;		// 최근에 검사한게 투명색인가?

	WORD	*pSourceTemp, *pFilterTemp;

	// height줄 만큼 memory잡기
	m_Pixels = new WORD* [height];

	for (int i=0; i<height; i++)
	{
		index = 0;
		count = 0;
		trans = 0;
		color = 0;
		bCheckTrans = TRUE;

		pSourceTemp = pSource;
		pFilterTemp = pFilter;

		// 각 line에 대해서 압축~
		for (int j=0; j<width; j++)
		{
			// 0번 color에 대해서 압축			
			if (*pSourceTemp==s_Colorkey)
				// alpha값이 0인 것은 투명색으로 봐도 될거 같았는데.. 불안해서.. -_-;
				//|| (*pFilterTemp & 0x001F)==0)	// 2002.3.26 추가
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
				WORD alpha2;				
				alpha = (BYTE)(*pFilterTemp & 0x001F);	// Alpha값(Blue값을 택한다.)
				alpha2 = (alpha << 8) | (32-alpha);	// (Alpha:32-Alpha)

				// 저장
				data[index++] = alpha2;					// Alpha 값을 저장한다.				
				data[index++] = *pSourceTemp;			// 실제 색깔을 저장한다.

				color++;								
			}

			pSourceTemp++;
			pFilterTemp++;
		}
		
		// 한 줄의 마지막 점이 투명색인가?
		if (bCheckTrans)
		{
			// 투명색이면 별다른 처리를 안해줘도 될거 같다.
		}	
		// 투명색이 아닌 경우, 점의 개수를 저장시켜줘야 한다.
		else
		{			
			count++;
			data[lastColorIndex] = color;
		}
		
		// memory를 다시 잡는다.
		m_Pixels[i] = new WORD [index+1];

		// m_Pixels[i]를 압축했으므로 data로 대체한다.
		// m_Pixels[i][0]에는 count를 넣어야 한다.
		m_Pixels[i][0] = count;
		memcpy(m_Pixels[i]+1, data, index<<1);

		pSource = (WORD*)((BYTE*)pSource + sourcePitch);
		pFilter = (WORD*)((BYTE*)pFilter + filterPitch);
	}

	delete [] data;

	m_bInit = true;
}

/*
void		
CAlphaSprite::Uncompress()
{
	m_bCompress = FALSE;
}
*/

//----------------------------------------------------------------------
// Is ColorPixel ?
//----------------------------------------------------------------------
// Sprite안에서 (x,y)는 색깔이 있는가?(투명색이 아닌 경우)
//----------------------------------------------------------------------
bool		
CAlphaSprite::IsColorPixel(short x, short y)
{
	// 초기화 된 경우
	if (m_bInit)
	{
		// Sprite의 영역을 벗어나면 false
		if (x<0 || y<0 || x>=m_Width || y>=m_Height)
			return false;

		// y번째 줄
		WORD	*pPixels = m_Pixels[y];

		// y번째 줄의 반복 수
		int	count = *pPixels++;

		int	transCount, 
				colorCount,
				index = 0;

		// 한 줄 출력
		if (count > 0)
		{			
			int i = count;
			do
			{
				transCount = *pPixels++;
				colorCount = *pPixels++;

				index += transCount;

				// 이번 loop안에 존재하는 점
				if (x < index+colorCount)
				{
					// 투명색까지보다 적은 경우
					if (x < index)
					{
						int n = index - x;

						// [pixel + alpha] * n 만큼 뒤의 점을 읽는다.
						pPixels += n<<1;

						// alpha값을 읽는다.
						if ((*pPixels >> 8)!= 0)
						{
							return true;
						}

						return false;
					}

					// 색깔에 속한다.
					return true;
				}
				
				pPixels += (colorCount<<1);
				index += colorCount;
			} while (--i);
		}
	}

	return false;
}

//----------------------------------------------------------------------
// Get Pixel ?
//----------------------------------------------------------------------
// Sprite안에서 (x,y)는 색깔을 얻는다.(투명색이 아닌 경우)
//----------------------------------------------------------------------
WORD		
CAlphaSprite::GetPixel(int x, int y, int bColor) const
{
	// 초기화 된 경우
	if (m_bInit)
	{
		// Sprite의 영역을 벗어나면 false
		if (x<0 || y<0 || x>=m_Width || y>=m_Height)
			return 0;

		// y번째 줄
		WORD	*pPixels = m_Pixels[y];

		// y번째 줄의 반복 수
		int	count = *pPixels++;

		int	transCount, 
				colorCount,
				index = 0;

		// 한 줄 출력
		if (count > 0)
		{			
			int i = count;
			do
			{
				transCount = *pPixels++;
				colorCount = *pPixels++;

				index += transCount;

				// 이번 loop안에 존재하는 점
				if (x < index+colorCount)
				{
					// 투명색까지보다 적은 경우
					if (x < index)
					{
						return 0;
					}

					// 색깔에 속한다.
					// AlphaChannel값이 있기 때문에...
					return pPixels[((x-index)<<1)+bColor];
				}
				
				pPixels += (colorCount<<1);
				index += colorCount;
			} while (--i);
		}
	}

	return 0;
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
CAlphaSprite::memcpyAlpha(WORD* pDest, WORD* pSource, WORD pixels)
{
	WORD		sTemp,dTemp;
	int		sr,sg,sb,dr,dg,db;
	//static WORD		temp, temp2;

	int i = pixels;

	BYTE alpha;

	// Alpha Channel Blending
	// 한점씩 찍기
	while (i--)
	{	
		// Source에는 Alpha값이 포함되어 있다.
		alpha = *pSource >> 8;
		pSource++;

		// 한점 찍기
		sTemp = *pSource;
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
CAlphaSprite::memcpyAlpha4444(WORD* pDest, WORD* pSource, WORD pixels)
{
	WORD		sTemp;
	int		sr,sg,sb;
	//static WORD		temp, temp2;

	int i = pixels;

	BYTE alpha;

	// Alpha Channel Blending
	// 한점씩 찍기
	while (i--)
	{	
		// Source에는 Alpha값이 포함되어 있다.
		alpha = *pSource >> 9;	//	alpha = (*pSource >> 8) >> 1;
		pSource++;

		// 한점 찍기
		sTemp = *pSource;
	
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
CAlphaSprite::BltClip(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

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

	int i;
	int j;
	int rectBottom = pRect->bottom;	

	for (i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
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
						memcpyAlpha(pDestTemp, pPixels, pRect->right - index);
						break;
					}						

					// 모두 출력
					memcpyAlpha(pDestTemp, pPixels, colorCount);
					pDestTemp += colorCount;
				}				
				// 출력하면 안 될 경우(줄의 왼쪽부분)에는 출력해도 되는지 확인해본다.
				else
				{
					// 투명색만으로 범위를 넘어갔으므로 모두 출력
					if (index > pRect->left)
					{	
						pDestTemp += index - pRect->left;
						memcpyAlpha(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						bPut = TRUE;
					}
					else
					if (index+colorCount > pRect->left)
					{
						dist = pRect->left - index;

						// 투명이 아닌 색들을 Surface에 출력한다.
						memcpyAlpha(pDestTemp, pPixels+dist, colorCount-dist);
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
CAlphaSprite::Blt(WORD *pDest, WORD pitch)
{
	int	count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;

	int i;
	int j;
	int rectBottom = m_Height;

	if (rectBottom > 0)
	{
		i = rectBottom-1;
		pDest = (WORD*)((BYTE*)pDest + i*pitch);

		do
		{
			pPixels		= m_Pixels[i];
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
					memcpyAlpha(pDestTemp, pPixels, colorCount);
					
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
CAlphaSprite::BltClipLeft(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int	count,
			transCount, 
			colorCount,
			index,
			dist;

	int i;
	int j;

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	for (i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
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
						memcpyAlpha(pDestTemp, pPixels, colorCount);
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
						memcpyAlpha(pDestTemp, pPixels+(dist<<1), colorCount-dist);
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
					memcpyAlpha(pDestTemp, pPixels, colorCount);

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
CAlphaSprite::BltClipRight(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int	count,
			transCount, 
			colorCount,
			index;

	int	i;
	int	j;

	int rectBottom = pRect->bottom;
	int rectRight = pRect->right;

	for (i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
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
						memcpyAlpha(pDestTemp, pPixels, rectRight - index);
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				// 출력
				memcpyAlpha(pDestTemp, pPixels, colorCount);
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
CAlphaSprite::BltClipWidth(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int	count,
			transCount, 
			colorCount,
			index,
			dist;

	int i;
	int j;

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	for (i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
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
								memcpyAlpha(pDestTemp, pPixels, (rectRight - index));
							}

							j = 0;
							
							break;
						}
						memcpyAlpha(pDestTemp, pPixels, colorCount);
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
							memcpyAlpha(pDestTemp, pPixels+(dist<<1), (rectRight - rectLeft));

							j = 0;						
							break;
						}		
						memcpyAlpha(pDestTemp, pPixels+(dist<<1), colorCount-dist);
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
							memcpyAlpha(pDestTemp, pPixels, rectRight - index);
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					// 출력
					memcpyAlpha(pDestTemp, pPixels, colorCount);
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
CAlphaSprite::BltClipHeight(WORD *pDest, WORD pitch, RECT* pRect)
{
	int	count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	int i;
	int j;

	int rectBottom = pRect->bottom;

	for (i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
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
				memcpyAlpha(pDestTemp, pPixels, colorCount);
				
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
CAlphaSprite::Blt4444(WORD *pDest, WORD pitch)
{
	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;

	int i;
	int j;

	int rectBottom = m_Height;

	if (rectBottom > 0)
	{
		i = rectBottom-1;
		pDest = (WORD*)((BYTE*)pDest + i*pitch);

		do
		{
			pPixels		= m_Pixels[i];
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
					memcpyAlpha4444(pDestTemp, pPixels, colorCount);
					
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
CAlphaSprite::Blt4444ClipLeft(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	int i;
	int j;

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	for (i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
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
						memcpyAlpha4444(pDestTemp, pPixels, colorCount);
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
						memcpyAlpha4444(pDestTemp, pPixels+(dist<<1), colorCount-dist);
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
					memcpyAlpha4444(pDestTemp, pPixels, colorCount);

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
CAlphaSprite::Blt4444ClipRight(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index;

	int	i;
	int	j;

	int rectBottom = pRect->bottom;
	int rectRight = pRect->right;

	for (i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
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
						memcpyAlpha4444(pDestTemp, pPixels, rectRight - index);
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				// 출력
				memcpyAlpha4444(pDestTemp, pPixels, colorCount);
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
CAlphaSprite::Blt4444ClipWidth(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	int i;
	int j;

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	for (i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
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
						memcpyAlpha4444(pDestTemp, pPixels, colorCount);
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
						memcpyAlpha4444(pDestTemp, pPixels+(dist<<1), colorCount-dist);
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
							memcpyAlpha4444(pDestTemp, pPixels, rectRight - index);
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					// 출력
					memcpyAlpha4444(pDestTemp, pPixels, colorCount);
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
CAlphaSprite::Blt4444ClipHeight(WORD *pDest, WORD pitch, RECT* pRect)
{
	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	int i;
	int j;

	int rectBottom = pRect->bottom;

	for (i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
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
				memcpyAlpha4444(pDestTemp, pPixels, colorCount);
				
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
CAlphaSprite::Blt4444NotTrans(WORD *pDest, WORD pitch)
{
	int		count,	
			transCount,
			colorCount;

	WORD	*pDestTemp,
			*pPixels;

	int i;
	int j;

	int rectBottom = m_Height;

	if (rectBottom > 0)
	{
		i = rectBottom-1;
		pDest = (WORD*)((BYTE*)pDest + i*pitch);

		do
		{
			pPixels		= m_Pixels[i];
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
					memcpyAlpha4444(pDestTemp, pPixels, colorCount);
					
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
CAlphaSprite::Blt4444NotTransClipLeft(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	int i;
	int j;

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	for (i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
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
						memcpyAlpha4444(pDestTemp, pPixels, colorCount);
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
						memcpyAlpha4444(pDestTemp, pPixels+(dist<<1), colorCount-dist);
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
					memcpyAlpha4444(pDestTemp, pPixels, colorCount);

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
CAlphaSprite::Blt4444NotTransClipRight(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index;

	int	i;
	int	j;

	int rectBottom = pRect->bottom;
	int rectRight = pRect->right;

	for (i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
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
						memcpyAlpha4444(pDestTemp, pPixels, rectRight - index);
						break;
					}
				}

				// 0을 출력한다.
				memset(pDestTemp, 0, transCount<<1);
				pDestTemp += transCount;		// 투명색만큼 건너 뛴다.

				// 출력
				memcpyAlpha4444(pDestTemp, pPixels, colorCount);
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
CAlphaSprite::Blt4444NotTransClipWidth(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	int i;
	int j;

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	for (i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
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
						memcpyAlpha4444(pDestTemp, pPixels, colorCount);
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
						memcpyAlpha4444(pDestTemp, pPixels+(dist<<1), colorCount-dist);
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
							memcpyAlpha4444(pDestTemp, pPixels, rectRight - index);
							break;
						}
					}

					// 0을 출력한다.
					memset(pDestTemp, 0, transCount<<1);
					pDestTemp += transCount;		// 투명색만큼 건너 뛴다.

					// 출력
					memcpyAlpha4444(pDestTemp, pPixels, colorCount);
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
CAlphaSprite::Blt4444NotTransClipHeight(WORD *pDest, WORD pitch, RECT* pRect)
{
	int		count,
			transCount,
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	int i;
	int j;

	int rectBottom = pRect->bottom;

	for (i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
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
				memcpyAlpha4444(pDestTemp, pPixels, colorCount);
				
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
CAlphaSprite::BltAlpha(WORD *pDest, WORD pitch, BYTE alpha)
{
	s_Value1 = alpha;

	int	count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;

	int i;
	int j;
	int rectBottom = m_Height;

	if (rectBottom > 0)
	{
		i = rectBottom-1;
		pDest = (WORD*)((BYTE*)pDest + i*pitch);

		do
		{
			pPixels		= m_Pixels[i];
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
					memcpyAlphaValue(pDestTemp, pPixels, colorCount);
					
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
CAlphaSprite::BltAlphaClipLeft(WORD* pDest, WORD pitch, RECT* pRect, BYTE alpha)
{
	s_Value1 = alpha;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int	count,
			transCount, 
			colorCount,
			index,
			dist;

	int i;
	int j;

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	for (i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
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
						memcpyAlphaValue(pDestTemp, pPixels, colorCount);
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
						memcpyAlphaValue(pDestTemp, pPixels+(dist<<1), colorCount-dist);
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
					memcpyAlphaValue(pDestTemp, pPixels, colorCount);

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
CAlphaSprite::BltAlphaClipRight(WORD* pDest, WORD pitch, RECT* pRect, BYTE alpha)
{
	s_Value1 = alpha;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int	count,
			transCount, 
			colorCount,
			index;

	int	i;
	int	j;

	int rectBottom = pRect->bottom;
	int rectRight = pRect->right;

	for (i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
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
						memcpyAlphaValue(pDestTemp, pPixels, rectRight - index);
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				// 출력
				memcpyAlphaValue(pDestTemp, pPixels, colorCount);
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
CAlphaSprite::BltAlphaClipWidth(WORD* pDest, WORD pitch, RECT* pRect, BYTE alpha)
{
	s_Value1 = alpha;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int	count,
			transCount, 
			colorCount,
			index,
			dist;

	int i;
	int j;

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	for (i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
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
								memcpyAlphaValue(pDestTemp, pPixels, (rectRight - index));
							}

							j = 0;
							
							break;
						}

						memcpyAlphaValue(pDestTemp, pPixels, colorCount);
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
							memcpyAlphaValue(pDestTemp, pPixels+(dist<<1), (rectRight - rectLeft));

							j = 0;						
							break;
						}		

						memcpyAlphaValue(pDestTemp, pPixels+(dist<<1), colorCount-dist);
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
							memcpyAlphaValue(pDestTemp, pPixels, rectRight - index);
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					// 출력
					memcpyAlphaValue(pDestTemp, pPixels, colorCount);
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
CAlphaSprite::BltAlphaClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha)
{
	s_Value1 = alpha;

	int	count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	int i;
	int j;

	int rectBottom = pRect->bottom;

	for (i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
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
				memcpyAlphaValue(pDestTemp, pPixels, colorCount);
				
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
CAlphaSprite::memcpyAlphaValue(WORD* pDest, WORD* pSource, WORD pixels)
{
	WORD		sTemp,dTemp;
	int		sr,sg,sb,dr,dg,db;
	//static WORD		temp, temp2;

	int i = pixels;

	//BYTE alpha;

	// Alpha Channel Blending
	// 한점씩 찍기
	while (i--)
	{	
		// Source에는 Alpha값이 포함되어 있다.
		//alpha = *pSource >> 8;
		pSource++;

		// 한점 찍기
		sTemp = *pSource;
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
CAlphaSprite::Blt4444SmallNotTrans(WORD *pDest, WORD pitch, BYTE shift)
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

	WORD	*pDestTemp,
			*pPixels;

	int i;
	int j;

	int rectBottom = m_Height;

	if (rectBottom > 0)
	{
		i = rectBottom-1;
		int stepY = 1 << shift;		// y줄 건너띄는 pixel수
		pDest = (WORD*)((BYTE*)pDest + (i>>shift)*pitch);

		do
		{
			pPixels		= m_Pixels[i];
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
					memcpyAlpha4444Small(pDestTemp, pPixels, colorCount);
					
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
CAlphaSprite::memcpyAlpha4444Small(WORD* pDest, WORD* pSource, WORD pixels)
{
	WORD		sTemp;
	int		sr,sg,sb;
	//static WORD		temp, temp2;

	int i = pixels >> s_Value1;

	BYTE alpha;
	
	// Alpha Channel Blending
	// 한점씩 찍기
	while (i--)
	{	
		// Source에는 Alpha값이 포함되어 있다.
		alpha = *pSource >> 9;	//	alpha = (*pSource >> 8) >> 1;	4 bit이기 때문에..
		pSource++;

		// 한점 찍기
		sTemp = *pSource;
	
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
