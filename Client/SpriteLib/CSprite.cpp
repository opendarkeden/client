//----------------------------------------------------------------------
// CSprite.cpp
//----------------------------------------------------------------------
#include "client_PCH.h"
#include "CDirectDraw.h"
#include "CFilter.h"
#include "CSprite.h"


//----------------------------------------------------------------------
// Static member
//----------------------------------------------------------------------
WORD	CSprite::s_Colorkey = 0;
DWORD	CSprite::s_Value1 = 1;
DWORD	CSprite::s_Value2 = 31;
DWORD	CSprite::s_Value3 = 1;

short		CSprite::s_X = 0;
short		CSprite::s_Y = 0;
CFilter*	CSprite::s_pFilter = NULL;


//----------------------------------------------------------------------
// Init Static member
//----------------------------------------------------------------------


//----------------------------------------------------------------------
//
// constructor/destructore
//
//----------------------------------------------------------------------

CSprite::CSprite()
{
	m_Width		= 0;
	m_Height	= 0;
	m_Pixels	= NULL;	
	m_bInit		= false;
	m_bLoading	= false;
}

CSprite::~CSprite()
{
	Release();
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

/*
//----------------------------------------------------------------------
// s_Pixels의 memory를 잡는다.
//----------------------------------------------------------------------
void
CSprite::InitBuffer(WORD width, WORD height)
{
	if (width==0 || height==0)
		return;

	// memory 해제
	ReleaseBuffer();

	s_BufferWidth	= width;
	s_BufferHeight	= height;

	// 각 buffer[i]에 대한 길이 저장
	s_BufferLen = new WORD [s_BufferHeight];

	s_Buffer = new WORD* [s_BufferHeight];

	for (int i=0; i<s_BufferHeight; i++)
		s_Buffer[i] = new WORD [s_BufferWidth];
}

//----------------------------------------------------------------------
// s_Pixels의 memory를 해제한다.
//----------------------------------------------------------------------
void	
CSprite::ReleaseBuffer()
{
	if (s_Buffer!=NULL)
	{		
		delete [] s_BufferLen;

		for (int i=0; i<s_BufferHeight; i++)
			delete [] s_Buffer[i];
			
		delete [] s_Buffer;

		s_Buffer	= NULL;
		s_BufferWidth	= 0;
		s_BufferHeight	= 0;

		s_Width			= 0;
		s_Height		= 0;
	}
}		
*/

//----------------------------------------------------------------------
// m_Pixels의 memory를 해제한다.
//----------------------------------------------------------------------
void	
CSprite::Release()
{
	if (m_Pixels!=NULL)
	{		
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

	m_bInit		= false;
}

//----------------------------------------------------------------------
// Assign Operator
//----------------------------------------------------------------------
void
CSprite::operator = (const CSprite& Sprite)
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
	register int i;
	register int j;

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

			index += colorCount;	// 투명색 아닌것만큼 +				
		}

		// 메모리 잡기
		m_Pixels[i] = new WORD [index];
		memcpy(m_Pixels[i], Sprite.m_Pixels[i], index<<1);		
	}

	// 복사 완료
	m_bInit = true;
}

//----------------------------------------------------------------------
// Load From Buffer
//----------------------------------------------------------------------
// s_Buffer에 저장된 Sprite를 *this로 copy시킨다.
//----------------------------------------------------------------------
/*
void
CSprite::LoadFromBuffer()
{
	if (s_Width==0 || s_Height==0)
		return;

	// 현재 Sprite의 memory해제
	Release();

	m_Width	= s_Width;
	m_Height = s_Height;
	m_Pixels = new WORD* [s_Height];

	// copy
	for (int i=0; i<s_Height; i++)
	{
		m_Pixels[i] = new WORD [s_BufferLen[i]];
		memcpy(m_Pixels[i], s_Buffer[i], s_BufferLen[i]);
	}
}
*/

//----------------------------------------------------------------------
// CDirectDrawSurface의 (x,y)+(width, height)영역을 읽어서 m_Pixels에 저장한다.
//----------------------------------------------------------------------
// m_Pixels를 0번 압축 Format으로 바꾼다.
//
// 각 line마다 다음과 같은 구조를 가진다.
//
//    [반복수] (투명수,색깔수,색깔들)(투명수,색깔수,색깔들)......
//
// 반복수는 2 bytes이고
// 투명수와 색깔수는 각각 2 byte이고
// 색깔들은 각각 2 bytes씩이다.
//
//----------------------------------------------------------------------
// Smart Cut 을 지원해야 한다.
//           :  그림 화일에서 특정한 영역의 Sprite를 짜를 때
//              어느 영역을 선택하면 그 영역안의 외곽부분에서..
//              자동으로 투명색 부분을 제거하고 색깔이 있는 부분만을 
//              새 영역으로 선택하도록 하는 것.
//----------------------------------------------------------------------
void
CSprite::SetPixel(WORD *pSource, WORD pitch, WORD width, WORD height)
{
	// memory해제
	Release();

	m_Width = width;
	m_Height = height;

	// 일단 memory를 적당히 잡아둔다.	
	WORD*	data = new WORD[m_Width*2+10];

	int		index,				// data의 index로 사용
			lastColorIndex;		// 투명이 아닌색 개수의 최근 index
	int		count;				// 반복수
	int		trans,				// 투명색 개수
			color;				// 투명이 아닌색 개수

	BOOL	bCheckTrans;		// 최근에 검사한게 투명색인가?

	WORD	*pSourceTemp;


	// height줄 만큼 memory잡기
	m_Pixels = new WORD* [height];

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

		// 각 line에 대해서 압축~
		for (j=0; j<width; j++)
		{
			// 0번 color에 대해서 압축
			if (*pSourceTemp==s_Colorkey)
			{
				// 최근에 검사한게 투명색이 아니었다면
				if (!bCheckTrans)
				{
					// ' (투명,색깔수,색깔들) '의 한 set가 끝났음을 의미하므로
					count++;
					
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

				data[index++] = *pSourceTemp;	// 실제 색깔을 저장한다.

				color++;								
			}

			pSourceTemp++;
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

		pSource = (WORD*)((BYTE*)pSource + pitch);
	}

	delete [] data;

	m_bInit = true;
}

//----------------------------------------------------------------------
// Set Pixel No Colorkey
//----------------------------------------------------------------------
// 투명색 없이 저장한다.
//----------------------------------------------------------------------
void
CSprite::SetPixelNoColorkey(WORD *pSource, WORD pitch, WORD width, WORD height)
{
	// memory해제
	Release();

	m_Width = width;
	m_Height = height;

	// 일단 memory를 적당히 잡아둔다.	
	WORD*	data = new WORD[m_Width*2+10];

	int	index,				// data의 index로 사용
			lastColorIndex;		// 투명이 아닌색 개수의 최근 index
	int	count;				// 반복수
	int	trans,				// 투명색 개수
			color;				// 투명이 아닌색 개수

	BOOL	bCheckTrans;		// 최근에 검사한게 투명색인가?

	WORD	*pSourceTemp;


	// height줄 만큼 memory잡기
	m_Pixels = new WORD* [height];

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

		// 각 line에 대해서 압축~
		for (j=0; j<width; j++)
		{
			// 0번 color에 대해서 압축
			/*
			if (*pSourceTemp==s_Colorkey)
			{
				// 최근에 검사한게 투명색이 아니었다면
				if (!bCheckTrans)
				{
					// ' (투명,색깔수,색깔들) '의 한 set가 끝났음을 의미하므로
					count++;
					
					data[lastColorIndex] = color;
					color = 0;

					bCheckTrans = TRUE;
				}
				
				trans++;				
			}
			else
			*/
			{
				// 최근에 검사한게 투명색이었다면..
				if (bCheckTrans)
				{						
					data[index++] = trans;		// 상위 byte에 투명수를 넣는다.
					trans = 0;

					lastColorIndex=index++;			// 색깔수를 넣을 위치를 기억					

					bCheckTrans = FALSE;
				}

				data[index++] = *pSourceTemp;	// 실제 색깔을 저장한다.

				color++;								
			}

			pSourceTemp++;
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

		pSource = (WORD*)((BYTE*)pSource + pitch);
	}

	delete [] data;

	m_bInit = true;
}

/*
void		
CSprite::Uncompress()
{
	m_bCompress = FALSE;
}
*/

//----------------------------------------------------------------------
// Get ColorRect
//----------------------------------------------------------------------
// 지정된 surface의 투명색 부분을 제거하기 위해서 사용한다....
//
// pSource의 width*height만큼의 영역에서
// 색깔이 존재하는 부분의 최대 사각 영역을 구해서(투명색 제외)
// rect로 넘겨준다.
//----------------------------------------------------------------------
//
//	[ Example ]
//
//	x : 투명색,  O : 색깔
//	width = 13, height = 9
//
//
//		0123456789012
//  0	xxxxxxxxxxxxx
//	1	xxxOOOOxxxxxx
//  2	xxOOOxxOOxOxx
//  3	xOOxOOxOOOxxx
//  4	xxOOOOOOOxxxx
//  5	xxxOOxxxOOxxx
//  6	xxxxxxxOOxxxx
//  7	xxxxxxxxxxxxx
//  8	xxxxxxxxxxxxx
//
//
// ---> GetColorRect(...)를 하면
//
//		0123456789012
//  0	xxxxxxxxxxxxx
//	1	x+-OOOO---+xx
//  2	x|OOOxxOOxOxx
//  3	xOOxOOxOOO|xx
//  4	x|OOOOOOOx|xx
//  5	x|xOOxxxOO|xx
//  6	x+-----OO-+xx
//  7	xxxxxxxxxxxxx
//  8	xxxxxxxxxxxxx
//
//      rect.left	= 1
//      rect.top	= 1
//      rect.right	= 10 + 1 = 11
//      rect.bottom	= 6 + 1 = 7
//
//
// [ 외부에서 ] 
//
//	new시작점	= old시작점 + (rect.left, rect.top)
//	newWidth	= rect.right - rect.left
//	newHeight	= rect.bottom - rect.top
//
//----------------------------------------------------------------------
void
CSprite::GetTightColorRect(WORD *pSource, WORD pitch, WORD width, WORD height, WORD colorkey, RECT& rect)
{
	WORD	*pSourceTemp, *pSourceTemp2;
	register int i, j;

	rect.left = 0;
	rect.right = 0;
	rect.top = 0;
	rect.bottom = 0;

	//-------------------------------------------------------
	// left 구하기
	//-------------------------------------------------------
	pSourceTemp = pSource;
	for (j=0; j<width; j++)
	{
		pSourceTemp2 = pSourceTemp;

		for (i=0; i<height; i++)
		{
			if (*pSourceTemp2!=colorkey)
			{
				rect.left = j;

				j = width;
				break;
			}

			pSourceTemp2 = (WORD*)((BYTE*)pSourceTemp2 + pitch);
		}

		pSourceTemp++;
	}

	//-------------------------------------------------------
	// top 구하기
	//-------------------------------------------------------
	pSourceTemp = pSource;
	for (i=0; i<height; i++)
	{
		pSourceTemp2 = pSourceTemp;

		for (j=0; j<width; j++)
		{
			if (*pSourceTemp2!=colorkey)
			{
				rect.top = i;

				i = height;
				break;
			}

			pSourceTemp2++;
		}

		pSourceTemp = (WORD*)((BYTE*)pSourceTemp + pitch);
	}

	//-------------------------------------------------------
	// right 구하기
	//-------------------------------------------------------
	pSourceTemp = (WORD*)((BYTE*)pSource + ((width-1)<<1));
	for (j=width-1; j>=0; j--)
	{
		pSourceTemp2 = pSourceTemp;

		for (i=0; i<height; i++)
		{
			if (*pSourceTemp2!=colorkey)
			{
				rect.right = j + 1;	// right는 +1

				j = 0;
				break;
			}

			pSourceTemp2 = (WORD*)((BYTE*)pSourceTemp2 + pitch);
		}

		pSourceTemp--;
	}

	//-------------------------------------------------------
	// bottom 구하기
	//-------------------------------------------------------
	pSourceTemp = (WORD*)((BYTE*)pSource + (height-1)*pitch);
	for (i=height-1; i>=0; i--)
	{
		pSourceTemp2 = pSourceTemp;

		for (j=0; j<width; j++)
		{
			if (*pSourceTemp2!=colorkey)
			{
				rect.bottom = i + 1;	// bottom은 +1

				i = 0;
				break;
			}

			pSourceTemp2++;
		}
		pSourceTemp = (WORD*)((BYTE*)pSourceTemp - pitch);
	}
}

//----------------------------------------------------------------------
// Is ColorPixel ?
//----------------------------------------------------------------------
// Sprite안에서 (x,y)는 색깔이 있는가?(투명색이 아닌 경우)
//----------------------------------------------------------------------
bool		
CSprite::IsColorPixel(short x, short y)
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

		if (count > 0)
		{
			register int i = count;
			do {		
				transCount = *pPixels++;
				colorCount = *pPixels++;

				index += transCount;

				// 이번 loop안에 존재하는 점
				if (x < index+colorCount)
				{
					// 투명색까지보다 적은 경우
					if (x < index)
					{
						return false;
					}

					// 색깔에 속한다.
					return true;
				}

				pPixels += colorCount;
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
CSprite::GetPixel(int x, int y) const
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

		if (count > 0)
		{
			register int i = count;
			do {		
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
					// 색깔을 return
					return pPixels[x-index];					
				}

				pPixels += colorCount;
				index += colorCount;
			} while (--i);
		}
	}

	return 0;
}

//----------------------------------------------------------------------
// Is Intersect Filter
//----------------------------------------------------------------------
// 이 Sprite가 설정된 Filter에 영향을 받는지 check한다.
//----------------------------------------------------------------------
bool
CSprite::IsIntersectFilter()
{
	// s_X, s_Y는 Sprite 내부에서 Filter가 출력되는 시작위치이다.	
	if (IsNotInit() || s_pFilter->IsNotInit())
		return false;

	if (// 출력 시작위치(s_X,s_Y)가 Sprite의 끝점보다는 적을 경우
		s_X < m_Width && s_Y < m_Height
		// Filter의 끝점이 Sprite 첫점 보다 큰 경우
		 &&	s_X+s_pFilter->GetWidth() > 0 && s_Y+s_pFilter->GetHeight() > 0)
		{
			return true;
		}
	
	return false;
}

//----------------------------------------------------------------------
// BltClip
//----------------------------------------------------------------------
// pRect의 영역만 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltClip(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------	
	// 첫 점 (x,y)
	//--------------------------------------------
	pDest += pitch * pRect->top + pRect->left;
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
						memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
						break;
					}						

					// 모두 출력
					memcpy(pDestTemp, pPixels, colorCount<<1);
					pDestTemp += colorCount;
				}				
				// 출력하면 안 될 경우(줄의 왼쪽부분)에는 출력해도 되는지 확인해본다.
				else
				{
					// 투명색만으로 범위를 넘어갔으므로 모두 출력
					if (index > pRect->left)
					{	
						pDestTemp += index - pRect->left;
						memcpy(pDestTemp, pPixels, colorCount<<1);
						pDestTemp += colorCount;
						bPut = TRUE;
					}
					else
					if (index+colorCount > pRect->left)
					{
						dist = pRect->left - index;

						// 투명이 아닌 색들을 Surface에 출력한다.
						memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);
						pDestTemp += colorCount-dist;

						// 이제부터는 계속 출력한다.
						bPut = TRUE;
					}
				}				

				// 투명이 아닌 색만큼 index증가				
				pPixels += colorCount;		

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
CSprite::Blt(WORD *pDest, WORD pitch)
{
	int	count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;
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
				do {
					pDestTemp += *pPixels++;			// 투명색만큼 건너 뛴다.
					colorCount = *pPixels++;		// 투명 아닌 색 수				

					// 투명이 아닌 색들을 Surface에 출력한다.
					memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);

					//int colorCount2 = colorCount;
					/*
					if (colorCount >= 4) 
					{

						_asm
						{					
							mov eax, colorCount2
							shr	eax, 2

							mov edi, pDestTemp
							mov esi, pPixels
							mov	ecx, eax

						_loop_first_:
							movq		mm0, [esi]
							movq		[edi], mm0
							add			esi, 8
							add			edi, 8
							loop		_loop_first_

							emms // Empty MMX state
						}				
					}
					*/
			
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;
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
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltClipLeft(WORD* pDest, WORD pitch, RECT* pRect)
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

	register int i;
	register int j;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
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
						memcpy(pDestTemp, pPixels, colorCount<<1);
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
						memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
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
					memcpy(pDestTemp, pPixels, colorCount<<1);

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
// Blt ClipRight
//----------------------------------------------------------------------
// 오른쪽 clipping.  
// pRect->right개 까지의 점만 pDest에 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltClipRight(WORD* pDest, WORD pitch, RECT* pRect)
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

	register int	i;
	register int	j;

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
						memcpy(pDestTemp, pPixels, (rectRight - index)<<1);
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				// 출력
				memcpy(pDestTemp, pPixels, colorCount<<1);
				pDestTemp += colorCount;
				pPixels += colorCount;			
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
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력하다가
// pRect->Right까지만 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltClipWidth(WORD* pDest, WORD pitch, RECT* pRect)
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
								//memset((void*)pDestTemp, 0, (rectRight - index)<<1);
								memcpy(pDestTemp, pPixels, (rectRight - index)<<1);
							}

							j = 0;
							
							break;
						}

						// 이번 단계는 모두 출력
						memcpy(pDestTemp, pPixels, colorCount<<1);
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
							memcpy(pDestTemp, pPixels+dist, (rectRight - rectLeft)<<1);

							j = 0;						
							break;
						}		

						// 투명이 아닌 색들을 Surface에 출력한다.
						memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
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
							memcpy(pDestTemp, pPixels, (rectRight - index)<<1);
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					// 출력
					memcpy(pDestTemp, pPixels, colorCount<<1);
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
// Blt Clip Height
//----------------------------------------------------------------------
// pRect->top, pRect->bottom만큼만 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltClipHeight(WORD *pDest, WORD pitch, RECT* pRect)
{
	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	for (i=pRect->top; i<rectBottom; i++)
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
				memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
				
				pDestTemp	+= colorCount;
				pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}



//----------------------------------------------------------------------
// memcpy Darker
//----------------------------------------------------------------------
// Darker : Dest = Source - Filter
//----------------------------------------------------------------------
void	
CSprite::memcpyDarkerFilter(WORD* pDest, WORD* pSource, BYTE* pFilter, WORD pixels)
{
	register int i = pixels;
	
	///*
	BYTE	darker;			
			
	while (i--)
	{
		darker = *pFilter;		

		//if (darker==0)
			//continue;
		
		*pDest = 
			(((((*pSource >> CDirectDraw::s_bSHIFT_R) & 0x1F) * darker) >> 5) << CDirectDraw::s_bSHIFT_R)
			| (((((*pSource >> CDirectDraw::s_bSHIFT_G) & 0x1F) * darker) >> 5) << CDirectDraw::s_bSHIFT_G)
			| (((*pSource & 0x1F) * darker) >> 5);
			
		pDest++;
		pSource++;
		pFilter++;		
	}
}



//----------------------------------------------------------------------
// Alpha Copy
//----------------------------------------------------------------------
// source --> dest 로 pixels만큼 s_Value1값으로 출력을 한다.
//
// s_Value1 = 1~32
//
//----------------------------------------------------------------------
// 5:6:5 전용 code이다.
//----------------------------------------------------------------------
/*
void	
CSprite::memcpyAlphaFilter(WORD* pDest, WORD* pSource, WORD pixels)
{
	static WORD		sTemp,dTemp;
	static WORD		sr,sg,sb,dr,dg,db;

	register int j;

	// Alpha Channel Blending
	// 한점씩 찍기
	for (j=0; j<pixels; j++)
	{		
		//if (s_IndexX > 0 && s_IndexX < m_pFilter->GetWidth())
		{
			// 한점 찍기
			sTemp = *pSource;
			dTemp = *pDest;
			sb = sTemp & 0x1F;
			db = dTemp & 0x1F;
			sg = sTemp >> 5;
			sg &= 0x3F;
			dg = dTemp >> 5;
			dg &= 0x3F;
			sr = sTemp >> 11;
			sr &= 0x1F;
			dr = dTemp >> 11;
			dr &= 0x1F;
			*pDest = ((*s_pFilterLine * (sb - db) >> 5) + db |
						((*s_pFilterLine * (sg - dg) >> 5) + dg) << 5 |
						((*s_pFilterLine * (sr - dr) >> 5) + dr) << 11);
		}
		//else
		{
		//	*pDest = *pSource;
		}

		pDest++;
		pSource++;
		s_pFilterLine++;
		s_IndexX++;
	}
}
*/

//----------------------------------------------------------------------
// BltHalf
//----------------------------------------------------------------------
// Clipping하지 않는다.
//----------------------------------------------------------------------
void
CSprite::BltHalf(WORD *pDest, WORD pitch)
{
	int		count,
			colorCount;

	WORD	*pDestTemp,
			*pPixels;

	register int i=0;
	register int j=0;
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
					CSpriteSurface::memcpyHalf(pDestTemp, pPixels, colorCount);
					
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;
				} while (--j);
			}

			pDest = (WORD*)((BYTE*)pDest - pitch);
		} while (i--);
	}
}

//----------------------------------------------------------------------
// BltHalf ClipLeft
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltHalfClipLeft(WORD* pDest, WORD pitch, RECT* pRect)
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

	register int i;
	register int j;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
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
						CSpriteSurface::memcpyHalf(pDestTemp, pPixels, colorCount);
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
						CSpriteSurface::memcpyHalf(pDestTemp, pPixels+dist, colorCount-dist);
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
					CSpriteSurface::memcpyHalf(pDestTemp, pPixels, colorCount);

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
// BltHalf ClipRight
//----------------------------------------------------------------------
// 오른쪽 clipping.  
// pRect->right개 까지의 점만 pDest에 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltHalfClipRight(WORD* pDest, WORD pitch, RECT* pRect)
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

	register int	i;
	register int	j;

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
						CSpriteSurface::memcpyHalf(pDestTemp, pPixels, rectRight - index);
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				// 출력
				CSpriteSurface::memcpyHalf(pDestTemp, pPixels, colorCount);
				pDestTemp += colorCount;
				pPixels += colorCount;			
				index += colorCount;
			} while (--j);
		}
		
		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltHalf ClipWidth
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
// rectRight까지..
//----------------------------------------------------------------------
void
CSprite::BltHalfClipWidth(WORD* pDest, WORD pitch, RECT* pRect)
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
								CSpriteSurface::memcpyHalf(pDestTemp, pPixels, rectRight - index);
							}

							j = 0;
							
							break;
						}

						// 이번 단계는 모두 출력
						CSpriteSurface::memcpyHalf(pDestTemp, pPixels, colorCount);
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
							CSpriteSurface::memcpyHalf(pDestTemp, pPixels+dist, rectRight - rectLeft);

							j = 0;						
							break;
						}					

						// 투명이 아닌 색들을 Surface에 출력한다.
						CSpriteSurface::memcpyHalf(pDestTemp, pPixels+dist, colorCount-dist);
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
							CSpriteSurface::memcpyHalf(pDestTemp, pPixels, rectRight - index);
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					// 출력
					CSpriteSurface::memcpyHalf(pDestTemp, pPixels, colorCount);
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
// BltHalf Clip Height
//----------------------------------------------------------------------
// pRect->top, pRect->bottom만큼만 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltHalfClipHeight(WORD *pDest, WORD pitch, RECT* pRect)
{
	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	for (i=pRect->top; i<rectBottom; i++)
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
				CSpriteSurface::memcpyHalf(pDestTemp, pPixels, colorCount);
				
				pDestTemp	+= colorCount;
				pPixels		+= colorCount;
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
CSprite::BltAlpha(WORD *pDest, WORD pitch, BYTE alpha)
{
	// alpha Depth 설정
	CSpriteSurface::s_Value1 = alpha;
	CSpriteSurface::s_Value2 = 32 - alpha;

	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;

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
					CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, colorCount);
					
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;
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
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltAlphaClipLeft(WORD* pDest, WORD pitch, RECT* pRect, BYTE alpha)
{
	// alpha Depth 설정
	CSpriteSurface::s_Value1 = alpha;
	CSpriteSurface::s_Value2 = 32 - alpha;

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

	register int i;
	register int j;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
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
						CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, colorCount);
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
						CSpriteSurface::memcpyAlpha(pDestTemp, pPixels+dist, colorCount-dist);
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
					CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, colorCount);

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
// BltAlpha ClipRight
//----------------------------------------------------------------------
// 오른쪽 clipping.  
// pRect->right개 까지의 점만 pDest에 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltAlphaClipRight(WORD* pDest, WORD pitch, RECT* pRect, BYTE alpha)
{
	// alpha Depth 설정
	CSpriteSurface::s_Value1 = alpha;
	CSpriteSurface::s_Value2 = 32 - alpha;
		
	WORD	*pPixels,
			*pDestTemp;

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
						CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, rectRight - index);
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				// 출력
				CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, colorCount);
				pDestTemp += colorCount;
				pPixels += colorCount;			
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
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltAlphaClipWidth(WORD* pDest, WORD pitch, RECT* pRect, BYTE alpha)
{
	// alpha Depth 설정
	CSpriteSurface::s_Value1 = alpha;
	CSpriteSurface::s_Value2 = 32 - alpha;

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
								//memset((void*)pDestTemp, 0, (rectRight - index)<<1);
								CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, rectRight - index);
							}

							j = 0;
							
							break;
						}

						// 이번 단계는 모두 출력
						CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, colorCount);
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
							CSpriteSurface::memcpyAlpha(pDestTemp, pPixels+dist, rectRight - rectLeft);

							j = 0;						
							break;
						}		

						// 투명이 아닌 색들을 Surface에 출력한다.
						CSpriteSurface::memcpyAlpha(pDestTemp, pPixels+dist, colorCount-dist);
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
							CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, rectRight - index);
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					// 출력
					CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, colorCount);
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
// BltAlpha Clip Height
//----------------------------------------------------------------------
// pRect->top, pRect->bottom만큼만 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltAlphaClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha)
{
	// alpha Depth 설정
	CSpriteSurface::s_Value1 = alpha;
	CSpriteSurface::s_Value2 = 32 - alpha;

	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	for (i=pRect->top; i<rectBottom; i++)
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
				CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, colorCount);
				
				pDestTemp	+= colorCount;
				pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}


//----------------------------------------------------------------------
// BltColor
//----------------------------------------------------------------------
// Clipping하지 않는다.
//----------------------------------------------------------------------
void
CSprite::BltColor(WORD *pDest, WORD pitch, BYTE rgb)
{
	// rgb값 설정
	CSpriteSurface::s_Value1 = rgb;

	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;

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
					CSpriteSurface::memcpyColor(pDestTemp, pPixels, colorCount);
					
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;
				} while (--j);
			}

			pDest = (WORD*)((BYTE*)pDest - pitch);
		} while (i--);
	}
}

//----------------------------------------------------------------------
// BltColor ClipLeft
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltColorClipLeft(WORD* pDest, WORD pitch, RECT* pRect, BYTE rgb)
{
	// rgb값 설정
	CSpriteSurface::s_Value1 = rgb;

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

	register int i;
	register int j;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
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
						CSpriteSurface::memcpyColor(pDestTemp, pPixels, colorCount);
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
						CSpriteSurface::memcpyColor(pDestTemp, pPixels+dist, colorCount-dist);
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
					CSpriteSurface::memcpyColor(pDestTemp, pPixels, colorCount);

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
// BltColor ClipRight
//----------------------------------------------------------------------
// 오른쪽 clipping.  
// pRect->right개 까지의 점만 pDest에 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltColorClipRight(WORD* pDest, WORD pitch, RECT* pRect, BYTE rgb)
{
	// rgb값 설정
	CSpriteSurface::s_Value1 = rgb;

	WORD	*pPixels,
			*pDestTemp;

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
						CSpriteSurface::memcpyColor(pDestTemp, pPixels, rectRight - index);
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				// 출력
				CSpriteSurface::memcpyColor(pDestTemp, pPixels, colorCount);
				pDestTemp += colorCount;
				pPixels += colorCount;			
				index += colorCount;
			} while (--j);
		}
		
		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltColor ClipWidth
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
// pRect->Right까지..
//----------------------------------------------------------------------
void
CSprite::BltColorClipWidth(WORD* pDest, WORD pitch, RECT* pRect, BYTE rgb)
{
	// rgb값 설정
	CSpriteSurface::s_Value1 = rgb;

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
								CSpriteSurface::memcpyColor(pDestTemp, pPixels, rectRight - index);
							}

							j = 0;
							
							break;
						}

						// 이번 단계는 모두 출력
						CSpriteSurface::memcpyColor(pDestTemp, pPixels, colorCount);
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
							CSpriteSurface::memcpyColor(pDestTemp, pPixels+dist, rectRight - rectLeft);

							j = 0;						
							break;
						}		

						// 투명이 아닌 색들을 Surface에 출력한다.
						CSpriteSurface::memcpyColor(pDestTemp, pPixels+dist, colorCount-dist);
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
							CSpriteSurface::memcpyColor(pDestTemp, pPixels, rectRight - index);
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					// 출력
					CSpriteSurface::memcpyColor(pDestTemp, pPixels, colorCount);
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
// BltColor Clip Height
//----------------------------------------------------------------------
// pRect->top, pRect->bottom만큼만 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltColorClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE rgb)
{
	// rgb값 설정
	CSpriteSurface::s_Value1 = rgb;

	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	for (i=pRect->top; i<rectBottom; i++)
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
				CSpriteSurface::memcpyColor(pDestTemp, pPixels, colorCount);
				
				pDestTemp	+= colorCount;
				pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltScale
//----------------------------------------------------------------------
// Clipping하지 않는다.
//----------------------------------------------------------------------
void
CSprite::BltScale(WORD *pDest, WORD pitch, BYTE scale)
{
	// scale값 설정
	CSpriteSurface::s_Value1 = scale;

	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;

	int rectBottom = m_Height;
	for (i=0; i<rectBottom; i++)
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
				pDestTemp += (*pPixels * scale);		// 투명색만큼 건너 뛴다.
				pPixels++;
				colorCount = *pPixels++;		// 투명 아닌 색 수				

				// 투명이 아닌 색들을 Surface에 출력한다.
				CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels, colorCount);
				
				pDestTemp	+= colorCount*scale;
				pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch*scale);
	}
}

//----------------------------------------------------------------------
// BltScale ClipLeft
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltScaleClipLeft(WORD* pDest, WORD pitch, RECT* pRect, BYTE scale)
{
	// scale값 설정
	CSpriteSurface::s_Value1 = scale;

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

	register int i;
	register int j;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
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
						pDestTemp += (index - rectLeft)*scale;

						// 이번 단계는 모두 출력
						CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels, colorCount);
						pDestTemp += (colorCount*scale);
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
						CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels+dist, colorCount-dist);
						pDestTemp += (colorCount-dist)*scale;
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
					pDestTemp += transCount*scale;			
					
					// 투명색이 아닌만큼 출력해준다.
					CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels, colorCount);

					// memory addr 증가
					pDestTemp += colorCount*scale;
					pPixels += colorCount;			
				} while (--j);
			}
		}
			

		pDest = (WORD*)((BYTE*)pDest + pitch*scale);
	}
}

//----------------------------------------------------------------------
// BltScale ClipRight
//----------------------------------------------------------------------
// 오른쪽 clipping.  
// pRect->right개 까지의 점만 pDest에 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltScaleClipRight(WORD* pDest, WORD pitch, RECT* pRect, BYTE scale)
{
	// scale값 설정
	CSpriteSurface::s_Value1 = scale;

	WORD	*pPixels,
			*pDestTemp;

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
						pDestTemp += transCount*scale;
					
						// 투명이 아닌 색들을 Surface에 출력한다.
						CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels, rectRight - index);
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount*scale;

				// 출력
				CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels, colorCount);
				pDestTemp += colorCount*scale;
				pPixels += colorCount;			
				index += colorCount;
			} while (--j);
		}
		
		pDest = (WORD*)((BYTE*)pDest + pitch*scale);
	}
}

//----------------------------------------------------------------------
// BltScale ClipWidth
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
// pRect->Right까지..
//----------------------------------------------------------------------
void
CSprite::BltScaleClipWidth(WORD* pDest, WORD pitch, RECT* pRect, BYTE scale)
{
	// scale값 설정
	CSpriteSurface::s_Value1 = scale;

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
						pDestTemp += (index - rectLeft)*scale;

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
								CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels, rectRight - index);
							}

							j = 0;
							
							break;
						}

						// 이번 단계는 모두 출력
						CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels, colorCount);
						pDestTemp += (colorCount*scale);
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
							CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels+dist, rectRight - rectLeft);

							j = 0;						
							break;
						}		

						// 투명이 아닌 색들을 Surface에 출력한다.
						CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels+dist, colorCount-dist);
						pDestTemp += (colorCount-dist)*scale;
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
							pDestTemp += transCount*scale;
						
							// 투명이 아닌 색들을 Surface에 출력한다.
							CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels, rectRight - index);
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount*scale;

					// 출력
					CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels, colorCount);
					pDestTemp += colorCount*scale;
					pPixels += colorCount;			
					index += colorCount;
				} while (--j);
			}
		}
		

		pDest = (WORD*)((BYTE*)pDest + pitch*scale);
	}
}

//----------------------------------------------------------------------
// BltScale Clip Height
//----------------------------------------------------------------------
// pRect->top, pRect->bottom만큼만 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltScaleClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE scale)
{
	// scale값 설정
	CSpriteSurface::s_Value1 = scale;

	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	for (i=pRect->top; i<rectBottom; i++)
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
				pDestTemp += *pPixels * scale;		// 투명색만큼 건너 뛴다.
				pPixels++;
				colorCount = *pPixels++;		// 투명 아닌 색 수				

				// 투명이 아닌 색들을 Surface에 출력한다.
				CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels, colorCount);
				
				pDestTemp	+= colorCount*scale;
				pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch*scale);
	}
}


//----------------------------------------------------------------------
// BltDarkness
//----------------------------------------------------------------------
// Clipping하지 않는다.
//----------------------------------------------------------------------
void
CSprite::BltDarkness(WORD *pDest, WORD pitch, BYTE DarkBits)
{
	// DarkBits값 설정
	CSpriteSurface::s_Value1 = DarkBits;

	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;

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
					CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
					
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;
				} while (--j);
			}

			pDest = (WORD*)((BYTE*)pDest - pitch);
		} while (i--);
	}
}

//----------------------------------------------------------------------
// BltDarkness ClipLeft
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltDarknessClipLeft(WORD* pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	// DarkBits값 설정
	CSpriteSurface::s_Value1 = DarkBits;

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

	register int i;
	register int j;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
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
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
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
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, colorCount-dist);
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
					CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);

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
// BltDarkness ClipRight
//----------------------------------------------------------------------
// 오른쪽 clipping.  
// pRect->right개 까지의 점만 pDest에 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltDarknessClipRight(WORD* pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	// DarkBits값 설정
	CSpriteSurface::s_Value1 = DarkBits;

	WORD	*pPixels,
			*pDestTemp;

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
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, rectRight - index);
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				// 출력
				CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
				pDestTemp += colorCount;
				pPixels += colorCount;			
				index += colorCount;
			} while (--j);
		}
		
		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltDarkness ClipWidth
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
// rectRight까지..
//----------------------------------------------------------------------
void
CSprite::BltDarknessClipWidth(WORD* pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	// DarkBits값 설정
	CSpriteSurface::s_Value1 = DarkBits;

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
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, rectRight - index);
							}

							j = 0;
							
							break;
						}

						// 이번 단계는 모두 출력
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
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
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, rectRight - rectLeft);

							j = 0;						
							break;
						}		

						// 투명이 아닌 색들을 Surface에 출력한다.
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, colorCount-dist);
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
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, rectRight - index);
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					// 출력
					CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
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
// BltDarkness Clip Height
//----------------------------------------------------------------------
// pRect->top, pRect->bottom만큼만 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltDarknessClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	// DarkBits값 설정
	CSpriteSurface::s_Value1 = DarkBits;

	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	for (i=pRect->top; i<rectBottom; i++)
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
				CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
				
				pDestTemp	+= colorCount;
				pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltColorSet
//----------------------------------------------------------------------
// Clipping하지 않는다.
//----------------------------------------------------------------------
void
CSprite::BltColorSet(WORD *pDest, WORD pitch, WORD colorSet)
{
	// colorSet값 설정
	CSpriteSurface::s_Value1 = colorSet;

	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;

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
					CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, colorCount);
					
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;
				} while (--j);
			}

			pDest = (WORD*)((BYTE*)pDest - pitch);
		} while (i--);
	}
}

//----------------------------------------------------------------------
// BltColorSet ClipLeft
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltColorSetClipLeft(WORD* pDest, WORD pitch, RECT* pRect, WORD colorSet)
{
	// colorSet값 설정
	CSpriteSurface::s_Value1 = colorSet;

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

	register int i;
	register int j;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
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
						CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, colorCount);
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
						CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels+dist, colorCount-dist);
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
					CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, colorCount);

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
// BltColorSet ClipRight
//----------------------------------------------------------------------
// 오른쪽 clipping.  
// pRect->right개 까지의 점만 pDest에 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltColorSetClipRight(WORD* pDest, WORD pitch, RECT* pRect, WORD colorSet)
{
	// colorSet값 설정
	CSpriteSurface::s_Value1 = colorSet;

	WORD	*pPixels,
			*pDestTemp;

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
						CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, rectRight - index);
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				// 출력
				CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, colorCount);
				pDestTemp += colorCount;
				pPixels += colorCount;			
				index += colorCount;
			} while (--j);
		}
		
		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltColorSet ClipWidth
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
// rectRight까지..
//----------------------------------------------------------------------
void
CSprite::BltColorSetClipWidth(WORD* pDest, WORD pitch, RECT* pRect, WORD colorSet)
{
	// colorSet값 설정
	CSpriteSurface::s_Value1 = colorSet;

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
								CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, rectRight - index);
							}

							j = 0;
							
							break;
						}

						// 이번 단계는 모두 출력
						CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, colorCount);
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
							CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels+dist, rectRight - rectLeft);

							j = 0;						
							break;
						}		

						// 투명이 아닌 색들을 Surface에 출력한다.
						CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels+dist, colorCount-dist);
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
							CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, rectRight - index);
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					// 출력
					CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, colorCount);
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
// BltColorSet Clip Height
//----------------------------------------------------------------------
// pRect->top, pRect->bottom만큼만 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltColorSetClipHeight(WORD *pDest, WORD pitch, RECT* pRect, WORD colorSet)
{
	// colorSet값 설정
	CSpriteSurface::s_Value1 = colorSet;

	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	for (i=pRect->top; i<rectBottom; i++)
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
				CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, colorCount);
				
				pDestTemp	+= colorCount;
				pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltEffect
//----------------------------------------------------------------------
// Clipping하지 않는다.
//----------------------------------------------------------------------
void
CSprite::BltEffect(WORD *pDest, WORD pitch)
{
	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;

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
					pDestTemp += *pPixels++;			// 투명색만큼 건너 뛴다.
					colorCount = *pPixels++;		// 투명 아닌 색 수				

					// 투명이 아닌 색들을 Surface에 출력한다.
					CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
					
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
CSprite::BltEffectClipLeft(WORD* pDest, WORD pitch, RECT* pRect)
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

	register int i;
	register int j;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
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
						CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
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
						CSpriteSurface::memcpyEffect(pDestTemp, pPixels+dist, colorCount-dist);					
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
					CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);

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
CSprite::BltEffectClipRight(WORD* pDest, WORD pitch, RECT* pRect)
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

	register int	i;
	register int	j;

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
						CSpriteSurface::memcpyEffect(pDestTemp, pPixels, rectRight - index);
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				// 출력
				CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
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
CSprite::BltEffectClipWidth(WORD* pDest, WORD pitch, RECT* pRect)
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
								CSpriteSurface::memcpyEffect(pDestTemp, pPixels, rectRight - index);
							}

							j = 0;
							
							break;
						}

						// 이번 단계는 모두 출력
						CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
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
							CSpriteSurface::memcpyEffect(pDestTemp, pPixels+dist, rectRight - rectLeft);

							j = 0;						
							break;
						}		

						// 투명이 아닌 색들을 Surface에 출력한다.
						CSpriteSurface::memcpyEffect(pDestTemp, pPixels+dist, colorCount-dist);					
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
							CSpriteSurface::memcpyEffect(pDestTemp, pPixels, rectRight - index);
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					// 출력
					CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
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
CSprite::BltEffectClipHeight(WORD *pDest, WORD pitch, RECT* pRect)
{
	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	for (i=pRect->top; i<rectBottom; i++)
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
				CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
				
				pDestTemp	+= colorCount;
				pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
} 


//----------------------------------------------------------------------
// BltAlphaFilter
//----------------------------------------------------------------------
// Clipping하지 않는다.
//----------------------------------------------------------------------
/*
void
CSprite::BltAlphaFilter(WORD *pDest, WORD pitch, CFilter* pFilter)
{
	m_pFilter = pFilter;

	// 출력에 이용할 Filter가 정의되어 있지 않은 경우
	if (m_pFilter==NULL)
		return;

	WORD	count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;			

	
	s_IndexY		= -s_FilterCY;

	for (register int i=0; i<m_Height; i++)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;
		

		s_IndexY++;
		// i번째줄의 Filter를 읽어온다.		
		//if (s_IndexY >= 0 && s_IndexY < m_pFilter->GetHeight())
		{
			s_pFilterLine	= m_pFilter->GetFilter(s_IndexY);

			s_IndexX		= -s_FilterCX;
			s_pFilterLine += -s_FilterCX;
		}
		

		// (투명수,색깔수,색깔들)의 반복 수		
		count	= *pPixels++;		
	

		s_bPutFilter = false;

		// 한 줄 출력
		for (register int j=0; j<count; j++)
		{				
			s_pFilterLine += *pPixels;		// filter도 투명색만큼 건너띈다.			
			s_IndexX			+= *pPixels;

			pDestTemp += *pPixels++;		// 투명색만큼 건너 뛴다.
			colorCount = *pPixels++;		// 투명 아닌 색 수
			
			if	(s_IndexY >= 0 && s_IndexY < m_pFilter->GetHeight())
				// Filter를 이용해서 투명이 아닌 색들을 Surface에 출력한다.
				memcpyAlphaFilter(pDestTemp, pPixels, colorCount);
			else
				memcpy(pDestTemp, pPixels, colorCount<<1);

			pDestTemp	+= colorCount;
			pPixels		+= colorCount;

		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}
*/


//----------------------------------------------------------------------
// Blt AlphaFilter
//----------------------------------------------------------------------
// Sprite의 (x,y)에 pFilter를 ...
//
//----------------------------------------------------------------------
void
CSprite::BltAlphaFilter(WORD *pDest, WORD pitch)
{
	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;
	BYTE	*pFilter;
	int		dist;

	register int	yIndex = -s_Y + m_Height - 1;
	register int	xIndex;
	register int i;
	register int j;

	int rectBottom = m_Height;
	if (rectBottom > 0)
	{
		i = rectBottom-1;
		pDest = (WORD*)((BYTE*)pDest + i*pitch);

		do
		{
			pPixels		= m_Pixels[i];
			pDestTemp	= pDest;

			//------------------------------------------
			// Filter를 출력하지 않는 경우
			//------------------------------------------
			if (yIndex < 0 || yIndex >= s_pFilter->GetHeight())
			{
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
						memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);

						pDestTemp	+= colorCount;
						pPixels		+= colorCount;
					} while (--j);
				}
			}
			//------------------------------------------
			// Filter를 출력해야 하는 경우
			//------------------------------------------
			else
			{
				// 출력할려는 줄에 관련된 Filter를 얻는다.
				pFilter = s_pFilter->GetFilter( yIndex );
				pFilter += -s_X;
				xIndex = -s_X;		// xIndex가 x를 이상인 경우 출력하면 된다.

				// (투명수,색깔수,색깔들)의 반복 수		
				count	= *pPixels++;		
 				// 한 줄 출력
				if (count > 0)
				{
					j = count;
					do 
					{				
						pDestTemp	+= *pPixels;			// 투명색만큼 건너 뛴다.
						xIndex		+= *pPixels;
						pFilter		+= *pPixels;
						pPixels ++;
						colorCount = *pPixels++;			// 투명 아닌 색 수				

						//------------------------------------------
						// Filter를 출력해야 하는 경우
						//------------------------------------------
						if (xIndex >= 0)
						{			
							dist  = s_pFilter->GetWidth() - xIndex;
							if (dist > 0)
							{						
								// Filter가 넉넉한 경우
								if (dist >= colorCount)
								{
									// 투명이 아닌 색들을 Surface에 출력한다.
									memcpyAlphaFilter(pDestTemp, pPixels, pFilter, colorCount);
								}
								// Filter가 부족한 경우
								else
								{
									// dist만큼만 Filter출력
									memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist);
									memcpy((void*)(pDestTemp+dist), (void*)(pPixels+dist), (colorCount-dist)<<1);
								}
							}
							else
							{
								memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
							}
						}
						// xIndex가 음수(-)인 경우
						else
						{					
							dist = s_pFilter->GetWidth() - xIndex ;

							// 몇 점을 건너서.. Filter를 출력해야 하는 경우
							if (xIndex+colorCount > 0)
							{							
								// Filter가 부족한 경우
								if (s_pFilter->GetWidth() < xIndex+colorCount)
								{	
									memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
									// Filter출력
									memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
									
									// - -;;
									memcpy((void*)(pDestTemp+dist), (void*)(pPixels+dist), (colorCount-dist)<<1);
								}
								// Filter가 넉넉한 경우
								else
								{							
									memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
									// dist만큼만 Filter출력
									memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
								}						
							}
							else
							{
								// 투명이 아닌 색들을 Surface에 출력한다.
								memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
							}
						}

						pDestTemp	+= colorCount;
						pPixels		+= colorCount;
						xIndex		+= colorCount;
						pFilter		+= colorCount;
					} while (--j);
				}
			}

			pDest = (WORD*)((BYTE*)pDest - pitch);

			yIndex--;
		} while (i--);
	}
}

//----------------------------------------------------------------------
// Blt AlphaFilter Clip Left
//----------------------------------------------------------------------
void		
CSprite::BltAlphaFilterClipLeft(WORD *pDest, WORD pitch, RECT* pRect)
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
			

	BYTE	*pFilter;
	int		dist, dist2;

	register int	yIndex = -s_Y + pRect->top;
	register int	xIndex;

	register int i;
	register int j;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	for (i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;	
		
		//------------------------------------------
		// Filter를 출력하지 않는 경우
		//------------------------------------------
		if (yIndex < 0 || yIndex >= s_pFilter->GetHeight())
		{
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
							memcpy(pDestTemp, pPixels, colorCount<<1);
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
							memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
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
						memcpy(pDestTemp, pPixels, colorCount<<1);

						// memory addr 증가
						pDestTemp += colorCount;
						pPixels += colorCount;			
					} while (--j);
				}
			}
		}
		//------------------------------------------
		// Filter를 출력해야 하는 경우
		//------------------------------------------
		else
		{
			// 출력할려는 줄에 관련된 Filter를 얻는다.
			pFilter = s_pFilter->GetFilter( yIndex );
			pFilter += -s_X;
			xIndex = -s_X;

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
							xIndex += transCount;//index - rectLeft;
							pFilter += transCount;//index - rectLeft;

							// 이번 단계는 모두 출력
							//memcpy(pDestTemp, pPixels, colorCount<<1);
							//------------------------------------------
							// Filter를 출력해야 하는 경우
							//------------------------------------------
							if (xIndex >= 0)
							{			
								dist2  = s_pFilter->GetWidth() - xIndex;
								if (dist2 > 0)
								{						
									// Filter가 넉넉한 경우
									if (dist2 >= colorCount)
									{
										// 투명이 아닌 색들을 Surface에 출력한다.
										memcpyAlphaFilter(pDestTemp, pPixels, pFilter, colorCount);
									}
									// Filter가 부족한 경우
									else
									{
										// dist2만큼만 Filter출력
										memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist2);
										memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (colorCount-dist2)<<1);
									}
								}
								else
								{
									memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
								}
							}
							// xIndex가 음수(-)인 경우
							else
							{					
								dist2 = s_pFilter->GetWidth() - xIndex ;

								// 몇 점을 건너서.. Filter를 출력해야 하는 경우
								if (xIndex+colorCount > 0)
								{							
									// Filter가 부족한 경우
									if (s_pFilter->GetWidth() < xIndex+colorCount)
									{	
										memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
										// Filter출력
										memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
										
										// - -;;
										memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (colorCount-dist2)<<1);
									}
									// Filter가 넉넉한 경우
									else
									{							
										memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
										// dist2만큼만 Filter출력
										memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
									}						
								}
								else
								{
									// 투명이 아닌 색들을 Surface에 출력한다.
									memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
								}
							}
							pDestTemp	+= colorCount;
							pPixels		+= colorCount;
							xIndex		+= colorCount;
							pFilter		+= colorCount;


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

							xIndex	+= transCount + dist;
							pFilter	+= transCount + dist;

							// 투명이 아닌 색들을 Surface에 출력한다.
							//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
							//pDestTemp += colorCount-dist;
							//pPixels += colorCount;
							
							//------------------------------------------
							// Filter를 출력해야 하는 경우
							//------------------------------------------
							if (xIndex >= 0)
							{			
								dist2  = s_pFilter->GetWidth() - xIndex;
								if (dist2 > 0)
								{						
									// Filter가 넉넉한 경우
									if (dist2 >= colorCount-dist)
									{
										// 투명이 아닌 색들을 Surface에 출력한다.
										memcpyAlphaFilter(pDestTemp, pPixels+dist, pFilter, colorCount-dist);
									}
									// Filter가 부족한 경우
									else
									{
										// dist2만큼만 Filter출력
										memcpyAlphaFilter(pDestTemp, pPixels+dist, pFilter, dist2);
										memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist+dist2), (colorCount-dist - dist2)<<1);
									}								
								}
								else
								{
									memcpy((void*)pDestTemp, (void*)(pPixels+dist), (colorCount-dist)<<1);
								}							
							}
							// xIndex가 음수(-)인 경우
							else
							{					
								dist2 = s_pFilter->GetWidth() - xIndex ;

								// 몇 점을 건너서.. Filter를 출력해야 하는 경우
								if (xIndex+colorCount-dist > 0)
								{							
									// Filter가 부족한 경우
									if (s_pFilter->GetWidth() < xIndex+(colorCount-dist))
									{	
										memcpy((void*)pDestTemp, (void*)(pPixels+dist), (-xIndex)<<1);
										// Filter출력
										memcpyAlphaFilter(pDestTemp-xIndex, pPixels+dist-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
										
										// - -;;
										memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist+dist2), (colorCount-dist - dist2)<<1);
									}
									// Filter가 넉넉한 경우
									else
									{							
										memcpy((void*)pDestTemp, (void*)(pPixels+dist), (-xIndex)<<1);
										// dist2만큼만 Filter출력
										memcpyAlphaFilter(pDestTemp-xIndex, pPixels+dist-xIndex, pFilter-xIndex, (colorCount-dist)+xIndex);
									}						
								}
								else
								{
									// 투명이 아닌 색들을 Surface에 출력한다.
									memcpy((void*)pDestTemp, (void*)(pPixels+dist), (colorCount-dist)<<1);
								}
							}

							pDestTemp += colorCount-dist;
							pPixels += colorCount;
							xIndex	+= colorCount-dist;
							pFilter += colorCount-dist;
							
							// 이제부터는 계속 출력한다.
							break;
						}
						
							
					}	
					
					// 투명색 부분 증가
					xIndex += transCount;
					pFilter += transCount;

					// 투명이 아닌 색만큼 index증가				
					pPixels += colorCount;
					index	+= colorCount;
					xIndex	+= colorCount;
					pFilter += colorCount;
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
						pDestTemp	+= transCount;			
						xIndex		+= transCount;
						pFilter		+= transCount;

						//------------------------------------------
						// Filter를 출력해야 하는 경우
						//------------------------------------------
						if (xIndex >= 0)
						{			
							dist2  = s_pFilter->GetWidth() - xIndex;
							if (dist2 > 0)
							{						
								// Filter가 넉넉한 경우
								if (dist2 >= colorCount)
								{
									// 투명이 아닌 색들을 Surface에 출력한다.
									memcpyAlphaFilter(pDestTemp, pPixels, pFilter, colorCount);
								}
								// Filter가 부족한 경우
								else
								{
									// dist2만큼만 Filter출력
									memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist2);
									memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (colorCount-dist2)<<1);
								}
							}
							else
							{
								memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
							}
						}
						// xIndex가 음수(-)인 경우
						else
						{					
							dist2 = s_pFilter->GetWidth() - xIndex ;

							// 몇 점을 건너서.. Filter를 출력해야 하는 경우
							if (xIndex+colorCount > 0)
							{							
								// Filter가 부족한 경우
								if (s_pFilter->GetWidth() < xIndex+colorCount)
								{	
									memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
									// Filter출력
									memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
									
									// - -;;
									memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (colorCount-dist2)<<1);
								}
								// Filter가 넉넉한 경우
								else
								{							
									memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
									// dist2만큼만 Filter출력
									memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
								}						
							}
							else
							{
								// 투명이 아닌 색들을 Surface에 출력한다.
								memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
							}
						}

						// memory addr 증가
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;		
						xIndex		+= colorCount;
						pFilter		+= colorCount;
					} while (--j);
				}	
			}
		}		

		pDest = (WORD*)((BYTE*)pDest + pitch);
		yIndex ++;
	}
}

//----------------------------------------------------------------------
// Blt AlphaFilter Clip Right
//----------------------------------------------------------------------
void		
CSprite::BltAlphaFilterClipRight(WORD *pDest, WORD pitch, RECT* pRect)
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

	BYTE	*pFilter;
	int		dist, dist2;

	register int	yIndex = -s_Y + pRect->top;
	register int	xIndex;

	register int	i;
	register int	j;

	int rectBottom = pRect->bottom;
	int rectRight = pRect->right;
	for (i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;	
		
		//------------------------------------------
		// Filter를 출력하지 않는 경우
		//------------------------------------------
		if (yIndex < 0 || yIndex >= s_pFilter->GetHeight())
		{
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
							memcpy((void*)pDestTemp, (void*)pPixels, (rectRight-index)<<1);
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					// 출력
					memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
					pDestTemp += colorCount;
					pPixels += colorCount;			
					index += colorCount;
				} while (--j);
			}
		}
		//------------------------------------------
		// Filter를 출력하는 경우
		//------------------------------------------
		else
		{
			// 출력할려는 줄에 관련된 Filter를 얻는다.
			pFilter = s_pFilter->GetFilter( yIndex );
			pFilter += -s_X;
			xIndex = -s_X;		

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
							pDestTemp	+= transCount;
							xIndex		+= transCount;
							pFilter		+= transCount;

							dist		= rectRight - index;
						
							// 투명이 아닌 색들을 Surface에 출력한다.
							//memcpy((void*)pDestTemp, (void*)pPixels, (rectRight - index)<<1);
							//------------------------------------------
							// Filter를 출력해야 하는 경우
							//------------------------------------------
							if (xIndex >= 0)
							{	
								dist2  = s_pFilter->GetWidth() - xIndex;
								if (dist2 > 0)
								{						
									// Filter가 넉넉한 경우
									if (dist2 >= dist)
									{
										// 투명이 아닌 색들을 Surface에 출력한다.
										memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist);
									}
									// Filter가 부족한 경우
									else
									{
										// dist2만큼만 Filter출력
										memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist2);
										memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (dist - dist2)<<1);
									}
								}
								else
								{
									memcpy((void*)pDestTemp, (void*)pPixels, dist<<1);
								}
							}
							// xIndex가 음수(-)인 경우
							else
							{					
								dist2 = s_pFilter->GetWidth() - xIndex ;

								// 몇 점을 건너서.. Filter를 출력해야 하는 경우
								if (xIndex + dist > 0)
								{							
									// Filter가 부족한 경우
									if (s_pFilter->GetWidth() < xIndex+colorCount)
									{	
										memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
										// Filter출력
										memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
										
										// - -;;
										int di = dist - dist2;
										if (di>0)
										{
											memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), di<<1);
										}
									}
									// Filter가 넉넉한 경우
									else
									{							
										memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
										// dist2만큼만 Filter출력
										memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, dist + xIndex);
									}						
								}
								else
								{
									// 투명이 아닌 색들을 Surface에 출력한다.
									memcpy((void*)pDestTemp, (void*)pPixels, dist<<1);
								}
							}
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp	+= transCount;
					xIndex		+= transCount;
					pFilter		+= transCount;

					// 출력
					//------------------------------------------
					// Filter를 출력해야 하는 경우
					//------------------------------------------
					if (xIndex >= 0)
					{			
						dist2  = s_pFilter->GetWidth() - xIndex;
						if (dist2 > 0)
						{						
							// Filter가 넉넉한 경우
							if (dist2 >= colorCount)
							{
								// 투명이 아닌 색들을 Surface에 출력한다.
								memcpyAlphaFilter(pDestTemp, pPixels, pFilter, colorCount);
							}
							// Filter가 부족한 경우
							else
							{
								// dist2만큼만 Filter출력
								memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist2);
								memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (colorCount-dist2)<<1);
							}
						}
						else
						{
							memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
						}
					}
					// xIndex가 음수(-)인 경우
					else
					{					
						dist2 = s_pFilter->GetWidth() - xIndex ;

						// 몇 점을 건너서.. Filter를 출력해야 하는 경우
						if (xIndex+colorCount > 0)
						{							
							// Filter가 부족한 경우
							if (s_pFilter->GetWidth() < xIndex+colorCount)
							{	
								memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
								// Filter출력
								memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
								
								// - -;;
								memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (colorCount-dist2)<<1);
							}
							// Filter가 넉넉한 경우
							else
							{							
								memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
								// dist2만큼만 Filter출력
								memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
							}						
						}
						else
						{
							// 투명이 아닌 색들을 Surface에 출력한다.
							memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
						}
					}

					pDestTemp	+= colorCount;
					pPixels		+= colorCount;	
					xIndex		+= colorCount;
					pFilter		+= colorCount;
					index		+= colorCount;
				} while (--j);
			}
		}
		
		pDest = (WORD*)((BYTE*)pDest + pitch);
		yIndex ++;
	}
}


//----------------------------------------------------------------------
// Blt AlphaFilter Clip Width
//----------------------------------------------------------------------
//
// [BUG]
//
//   "투명부분+색깔"이 한 화면(800)을 넘어갈 경우..
//   그 사이에 Filter까지 포함해서 출력할때.. 복잡..복잡.. - -;;
//
//----------------------------------------------------------------------

void		
CSprite::BltAlphaFilterClipWidth(WORD *pDest, WORD pitch, RECT* pRect)
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
			

	BYTE	*pFilter;
	int		dist, dist2;

	register int	yIndex = -s_Y + pRect->top;
	register int	xIndex;

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
		pPixels = m_Pixels[i];
		pDestTemp = pDest;	
		
		//------------------------------------------
		// Filter를 출력하지 않는 경우
		//------------------------------------------
		if (yIndex < 0 || yIndex >= s_pFilter->GetHeight())
		{
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
									//memset((void*)pDestTemp, 0, (rectRight - index)<<1);
									memcpy(pDestTemp, pPixels, (rectRight - index)<<1);
								}

								j = 0;
								
								break;
							}

							// 이번 단계는 모두 출력
							memcpy(pDestTemp, pPixels, colorCount<<1);
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
								memcpy(pDestTemp, pPixels+dist, (rectRight - rectLeft)<<1);

								j = 0;						
								break;
							}		

							// 투명이 아닌 색들을 Surface에 출력한다.
							memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
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
								memcpy((void*)pDestTemp, (void*)pPixels, (rectRight-index)<<1);
								break;
							}
						}

						// 투명색만큼 건너띄고
						pDestTemp += transCount;

						// 출력
						memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
						pDestTemp += colorCount;
						pPixels += colorCount;			
						index += colorCount;
					} while (--j);
				}
			}
		}
		//------------------------------------------
		// Filter를 출력해야 하는 경우
		//------------------------------------------
		else
		{
			// 출력할려는 줄에 관련된 Filter를 얻는다.
			pFilter = s_pFilter->GetFilter( yIndex );
			pFilter += -s_X;
			xIndex = -s_X;

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
							//---------------------------------------------
							// 오른쪽을 넘어가는지 check해야 한다.
							//---------------------------------------------
							//if (index+colorCount)


							// 투명색부분 건너띔
							pDestTemp += index - rectLeft;
							xIndex += transCount;//index - rectLeft;
							pFilter += transCount;//index - rectLeft;							

							// 이번 단계는 모두 출력
							//memcpy(pDestTemp, pPixels, colorCount<<1);
							//------------------------------------------
							// Filter를 출력해야 하는 경우
							//------------------------------------------
							if (xIndex >= 0)
							{			
								dist2  = s_pFilter->GetWidth() - xIndex;
								if (dist2 > 0)
								{						
									// Filter가 넉넉한 경우
									if (dist2 >= colorCount)
									{
										// 투명이 아닌 색들을 Surface에 출력한다.
										memcpyAlphaFilter(pDestTemp, pPixels, pFilter, colorCount);
									}
									// Filter가 부족한 경우
									else
									{
										// dist2만큼만 Filter출력
										memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist2);
										memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (colorCount-dist2)<<1);
									}
								}
								else
								{
									memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
								}
							}
							// xIndex가 음수(-)인 경우
							else
							{					
								dist2 = s_pFilter->GetWidth() - xIndex ;

								// 몇 점을 건너서.. Filter를 출력해야 하는 경우
								if (xIndex+colorCount > 0)
								{							
									// Filter가 부족한 경우
									if (s_pFilter->GetWidth() < xIndex+colorCount)
									{	
										memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
										// Filter출력
										memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
										
										// - -;;
										memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (colorCount-dist2)<<1);
									}
									// Filter가 넉넉한 경우
									else
									{							
										memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
										// dist2만큼만 Filter출력
										memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
									}						
								}
								else
								{
									// 투명이 아닌 색들을 Surface에 출력한다.
									memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
								}
							}
							pDestTemp	+= colorCount;
							pPixels		+= colorCount;
							xIndex		+= colorCount;
							pFilter		+= colorCount;
							index		+= colorCount;

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

							xIndex	+= transCount + dist;
							pFilter	+= transCount + dist;

							// 투명이 아닌 색들을 Surface에 출력한다.
							//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
							//pDestTemp += colorCount-dist;
							//pPixels += colorCount;
							
							//------------------------------------------
							// Filter를 출력해야 하는 경우
							//------------------------------------------
							if (xIndex >= 0)
							{			
								dist2  = s_pFilter->GetWidth() - xIndex;
								if (dist2 > 0)
								{						
									// Filter가 넉넉한 경우
									if (dist2 >= colorCount-dist)
									{
										// 투명이 아닌 색들을 Surface에 출력한다.
										memcpyAlphaFilter(pDestTemp, pPixels+dist, pFilter, colorCount-dist);
									}
									// Filter가 부족한 경우
									else
									{
										// dist2만큼만 Filter출력
										memcpyAlphaFilter(pDestTemp, pPixels+dist, pFilter, dist2);
										memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist+dist2), (colorCount-dist - dist2)<<1);
									}								
								}
								else
								{
									memcpy((void*)pDestTemp, (void*)(pPixels+dist), (colorCount-dist)<<1);
								}							
							}
							// xIndex가 음수(-)인 경우
							else
							{					
								dist2 = s_pFilter->GetWidth() - xIndex ;

								// 몇 점을 건너서.. Filter를 출력해야 하는 경우
								if (xIndex+colorCount-dist > 0)
								{							
									// Filter가 부족한 경우
									if (s_pFilter->GetWidth() < xIndex+(colorCount-dist))
									{	
										memcpy((void*)pDestTemp, (void*)(pPixels+dist), (-xIndex)<<1);
										// Filter출력
										memcpyAlphaFilter(pDestTemp-xIndex, pPixels+dist-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
										
										// - -;;
										memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist+dist2), (colorCount-dist - dist2)<<1);
									}
									// Filter가 넉넉한 경우
									else
									{							
										memcpy((void*)pDestTemp, (void*)(pPixels+dist), (-xIndex)<<1);
										// dist2만큼만 Filter출력
										memcpyAlphaFilter(pDestTemp-xIndex, pPixels+dist-xIndex, pFilter-xIndex, (colorCount-dist)+xIndex);
									}						
								}
								else
								{
									// 투명이 아닌 색들을 Surface에 출력한다.
									memcpy((void*)pDestTemp, (void*)(pPixels+dist), (colorCount-dist)<<1);
								}
							}

							pDestTemp += colorCount-dist;
							pPixels		+= colorCount;
							xIndex		+= colorCount-dist;
							pFilter		+= colorCount-dist;
							index		+= colorCount;
							
							// 이제부터는 계속 출력한다.
							break;
						}
					}	
					
					// 투명색 부분 증가
					xIndex += transCount;
					pFilter += transCount;

					// 투명이 아닌 색만큼 index증가				
					pPixels += colorCount;
					index	+= colorCount;
					xIndex	+= colorCount;
					pFilter += colorCount;
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
								pDestTemp	+= transCount;
								xIndex		+= transCount;
								pFilter		+= transCount;

								dist		= rectRight - index;
							
								// 투명이 아닌 색들을 Surface에 출력한다.
								//memcpy((void*)pDestTemp, (void*)pPixels, (rectRight - index)<<1);
								//------------------------------------------
								// Filter를 출력해야 하는 경우
								//------------------------------------------
								if (xIndex >= 0)
								{	
									dist2  = s_pFilter->GetWidth() - xIndex;
									if (dist2 > 0)
									{						
										// Filter가 넉넉한 경우
										if (dist2 >= dist)
										{
											// 투명이 아닌 색들을 Surface에 출력한다.
											memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist);
										}
										// Filter가 부족한 경우
										else
										{
											// dist2만큼만 Filter출력
											memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist2);
											memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (dist - dist2)<<1);
										}
									}
									else
									{
										memcpy((void*)pDestTemp, (void*)pPixels, dist<<1);
									}
								}
								// xIndex가 음수(-)인 경우
								else
								{					
									dist2 = s_pFilter->GetWidth() - xIndex ;

									// 몇 점을 건너서.. Filter를 출력해야 하는 경우
									if (xIndex + dist > 0)
									{							
										// Filter가 부족한 경우
										if (s_pFilter->GetWidth() < xIndex+colorCount)
										{	
											memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
											// Filter출력
											memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
											
											// - -;;
											memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (dist - dist2)<<1);
										}
										// Filter가 넉넉한 경우
										else
										{							
											memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
											// dist2만큼만 Filter출력
											memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, dist + xIndex);
										}						
									}
									else
									{
										// 투명이 아닌 색들을 Surface에 출력한다.
										memcpy((void*)pDestTemp, (void*)pPixels, dist<<1);
									}
								}
								break;
							}
						}

						// 투명색만큼 건너띄고
						pDestTemp	+= transCount;
						xIndex		+= transCount;
						pFilter		+= transCount;

						// 출력
						//------------------------------------------
						// Filter를 출력해야 하는 경우
						//------------------------------------------
						if (xIndex >= 0)
						{			
							dist2  = s_pFilter->GetWidth() - xIndex;
							if (dist2 > 0)
							{						
								// Filter가 넉넉한 경우
								if (dist2 >= colorCount)
								{
									// 투명이 아닌 색들을 Surface에 출력한다.
									memcpyAlphaFilter(pDestTemp, pPixels, pFilter, colorCount);
								}
								// Filter가 부족한 경우
								else
								{
									// dist2만큼만 Filter출력
									memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist2);
									memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (colorCount-dist2)<<1);
								}
							}
							else
							{
								memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
							}
						}
						// xIndex가 음수(-)인 경우
						else
						{					
							dist2 = s_pFilter->GetWidth() - xIndex ;

							// 몇 점을 건너서.. Filter를 출력해야 하는 경우
							if (xIndex+colorCount > 0)
							{							
								// Filter가 부족한 경우
								if (s_pFilter->GetWidth() < xIndex+colorCount)
								{	
									memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
									// Filter출력
									memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
									
									// - -;;
									memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (colorCount-dist2)<<1);
								}
								// Filter가 넉넉한 경우
								else
								{							
									memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
									// dist2만큼만 Filter출력
									memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
								}						
							}
							else
							{
								// 투명이 아닌 색들을 Surface에 출력한다.
								memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
							}
						}

						pDestTemp	+= colorCount;
						pPixels		+= colorCount;	
						xIndex		+= colorCount;
						pFilter		+= colorCount;
						index		+= colorCount;
					} while (--j);
				}
			}
		}		

		pDest = (WORD*)((BYTE*)pDest + pitch);
		yIndex ++;
	}
}

//----------------------------------------------------------------------
// Blt AlphaFilter Clip Height
//----------------------------------------------------------------------
// pRect->top, pRect->bottom만큼만 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltAlphaFilterClipHeight(WORD *pDest, WORD pitch, RECT* pRect)
{
	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;
	BYTE	*pFilter;
	int		dist;

	register int	yIndex = -s_Y + pRect->top;
	register int	xIndex;

	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	for (i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		//------------------------------------------
		// Filter를 출력하지 않는 경우
		//------------------------------------------
		if (yIndex < 0 || yIndex >= s_pFilter->GetHeight())
		{
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
					memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
					
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;
				} while (--j);
			}
		}
		//------------------------------------------
		// Filter를 출력해야 하는 경우
		//------------------------------------------
		else
		{
			// 출력할려는 줄에 관련된 Filter를 얻는다.
			pFilter = s_pFilter->GetFilter( yIndex );
			pFilter += -s_X;
			xIndex = -s_X;		// xIndex가 x를 이상인 경우 출력하면 된다.

			// (투명수,색깔수,색깔들)의 반복 수		
			count	= *pPixels++;		
 			// 한 줄 출력
			if (count > 0)
			{
				j = count;
				do 
				{
					pDestTemp	+= *pPixels;			// 투명색만큼 건너 뛴다.
					xIndex		+= *pPixels;
					pFilter		+= *pPixels;
					pPixels ++;
					colorCount = *pPixels++;			// 투명 아닌 색 수				

					//------------------------------------------
					// Filter를 출력해야 하는 경우
					//------------------------------------------
					if (xIndex >= 0)
					{			
						dist  = s_pFilter->GetWidth() - xIndex;
						if (dist > 0)
						{						
							// Filter가 넉넉한 경우
							if (dist >= colorCount)
							{
								// 투명이 아닌 색들을 Surface에 출력한다.
								memcpyAlphaFilter(pDestTemp, pPixels, pFilter, colorCount);
							}
							// Filter가 부족한 경우
							else
							{
								// dist만큼만 Filter출력
								memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist);
								memcpy((void*)(pDestTemp+dist), (void*)(pPixels+dist), (colorCount-dist)<<1);
							}
						}
						else
						{
							memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
						}
					}
					// xIndex가 음수(-)인 경우
					else
					{					
						dist = s_pFilter->GetWidth() - xIndex ;

						// 몇 점을 건너서.. Filter를 출력해야 하는 경우
						if (xIndex+colorCount > 0)
						{							
							// Filter가 부족한 경우
							if (s_pFilter->GetWidth() < xIndex+colorCount)
							{	
								memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
								// Filter출력
								memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
								
								// - -;;
								memcpy((void*)(pDestTemp+dist), (void*)(pPixels+dist), (colorCount-dist)<<1);
							}
							// Filter가 넉넉한 경우
							else
							{							
								memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
								// dist만큼만 Filter출력
								memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
							}						
						}
						else
						{
							// 투명이 아닌 색들을 Surface에 출력한다.
							memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
						}
					}

					pDestTemp	+= colorCount;
					pPixels		+= colorCount;
					xIndex		+= colorCount;
					pFilter		+= colorCount;
				} while (--j);
			}
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
		yIndex++;
	}
}


//----------------------------------------------------------------------
// memcpy Filter
//----------------------------------------------------------------------
// pFilter를 이용해서 출력한다.
// pFilter를 alpha 값으로 이용한다.
//----------------------------------------------------------------------
void
CSprite::memcpyAlphaFilter(WORD* pDest, WORD* pSource, BYTE* pFilter, WORD pixels)
{
	//register WORD		sTemp,dTemp;
	//register int		sr,sg,sb,dr,dg,db;
	//static WORD		temp, temp2;

	register int j = pixels;
	
	//BYTE alpha;

	// Alpha Channel Blending
	// 한점씩 찍기
	while (j--)
	{			
		//alpha = *pFilter;		

		// 한점 찍기
		/*
		sTemp = *pSource;
		dTemp = *pDest;

		sr = CDirectDraw::Red(sTemp);
		sg = CDirectDraw::Green(sTemp);
		sb = CDirectDraw::Blue(sTemp);

		dr = CDirectDraw::Red(dTemp);
		dg = CDirectDraw::Green(dTemp);
		db = CDirectDraw::Blue(dTemp);		
	
		*pDest = ((alpha * (sb - db) >> 5) + db |
					((alpha * (sg - dg) >> 5) + dg) << CDirectDraw::s_bSHIFT_G |
					((alpha * (sr - dr) >> 5) + dr) << CDirectDraw::s_bSHIFT_R);
		*/

		///*
		//if (*pFilter > 26) 
		if (*pFilter) 
		{
			*pDest = *pSource; 
		}
		/*
		/*
		else
		{
			*pDest = 
				((*pDest >> 1) & CDirectDraw::s_wMASK_SHIFT[1]) +
				((*pSource >> 1) & CDirectDraw::s_wMASK_SHIFT[1]);
		}
		//*/
		

		pDest++;
		pSource++;
		pFilter++;
	}
}


//----------------------------------------------------------------------
// Blt AlphaFilter Darkness
//----------------------------------------------------------------------
// Sprite의 (x,y)에 pFilter를 ...
//
//----------------------------------------------------------------------
void
CSprite::BltAlphaFilterDarkness(WORD *pDest, WORD pitch, BYTE DarkBits)
{
	// 어둡게 하는 bit값 설정
	s_Value1 = DarkBits;
	CSpriteSurface::s_Value1 = DarkBits;

	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;
	BYTE	*pFilter;
	int		dist;

	register int	yIndex = -s_Y + m_Height - 1;
	register int	xIndex;

	register int i;
	register int j;

	int rectBottom = m_Height;
	for (i=0; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		//------------------------------------------
		// Filter를 출력하지 않는 경우
		//------------------------------------------
		if (yIndex < 0 || yIndex >= s_pFilter->GetHeight())
		{
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
					CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);

					pDestTemp	+= colorCount;
					pPixels		+= colorCount;
				} while (--j);
			}
		}
		//------------------------------------------
		// Filter를 출력해야 하는 경우
		//------------------------------------------
		else
		{
			// 출력할려는 줄에 관련된 Filter를 얻는다.
			pFilter = s_pFilter->GetFilter( yIndex );
			pFilter += -s_X;
			xIndex = -s_X;		// xIndex가 x를 이상인 경우 출력하면 된다.

			// (투명수,색깔수,색깔들)의 반복 수		
			count	= *pPixels++;		
 			// 한 줄 출력
			if (count > 0)
			{
				j = count;
				do 
				{			
					pDestTemp	+= *pPixels;			// 투명색만큼 건너 뛴다.
					xIndex		+= *pPixels;
					pFilter		+= *pPixels;
					pPixels ++;
					colorCount = *pPixels++;			// 투명 아닌 색 수				

					//------------------------------------------
					// Filter를 출력해야 하는 경우
					//------------------------------------------
					if (xIndex >= 0)
					{			
						dist  = s_pFilter->GetWidth() - xIndex;
						if (dist > 0)
						{						
							// Filter가 넉넉한 경우
							if (dist >= colorCount)
							{
								// 투명이 아닌 색들을 Surface에 출력한다.
								memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, colorCount);
							}
							// Filter가 부족한 경우
							else
							{
								// dist만큼만 Filter출력
								memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist);
								CSpriteSurface::memcpyDarkness(pDestTemp+dist, pPixels+dist, colorCount-dist);
							}
						}
						else
						{
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						}
					}
					// xIndex가 음수(-)인 경우
					else
					{					
						dist = s_pFilter->GetWidth() - xIndex ;

						// 몇 점을 건너서.. Filter를 출력해야 하는 경우
						if (xIndex+colorCount > 0)
						{							
							// Filter가 부족한 경우
							if (s_pFilter->GetWidth() < xIndex+colorCount)
							{	
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
								// Filter출력
								memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
								
								// - -;;
								CSpriteSurface::memcpyDarkness(pDestTemp+dist, pPixels+dist, colorCount-dist);
							}
							// Filter가 넉넉한 경우
							else
							{							
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
								// dist만큼만 Filter출력
								memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
							}						
						}
						else
						{
							// 투명이 아닌 색들을 Surface에 출력한다.
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						}
					}

					pDestTemp	+= colorCount;
					pPixels		+= colorCount;
					xIndex		+= colorCount;
					pFilter		+= colorCount;				
				} while (--j);
			}
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);

		yIndex--;
	}
}

//----------------------------------------------------------------------
// Blt AlphaFilter Darkness Clip Left
//----------------------------------------------------------------------
void		
CSprite::BltAlphaFilterDarknessClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	// 어둡게 하는 bit값 설정
	s_Value1 = DarkBits;
	CSpriteSurface::s_Value1 = DarkBits;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index;
			

	BYTE	*pFilter;
	int		dist, dist2;

	register int	yIndex = -s_Y + pRect->top;
	register int	xIndex;

	register int i;
	register int j;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	for (i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;	
		
		//------------------------------------------
		// Filter를 출력하지 않는 경우
		//------------------------------------------
		if (yIndex < 0 || yIndex >= s_pFilter->GetHeight())
		{
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
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
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
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, colorCount-dist);					
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
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);

						// memory addr 증가
						pDestTemp += colorCount;
						pPixels += colorCount;			
					} while (--j);
				}
			}
		}
		//------------------------------------------
		// Filter를 출력해야 하는 경우
		//------------------------------------------
		else
		{
			// 출력할려는 줄에 관련된 Filter를 얻는다.
			pFilter = s_pFilter->GetFilter( yIndex );
			pFilter += -s_X;
			xIndex = -s_X;

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
							xIndex += transCount;//index - rectLeft;
							pFilter += transCount;//index - rectLeft;

							// 이번 단계는 모두 출력
							//memcpy(pDestTemp, pPixels, colorCount<<1);
							//------------------------------------------
							// Filter를 출력해야 하는 경우
							//------------------------------------------
							if (xIndex >= 0)
							{			
								dist2  = s_pFilter->GetWidth() - xIndex;
								if (dist2 > 0)
								{						
									// Filter가 넉넉한 경우
									if (dist2 >= colorCount)
									{
										// 투명이 아닌 색들을 Surface에 출력한다.
										memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, colorCount);
									}
									// Filter가 부족한 경우
									else
									{
										// dist2만큼만 Filter출력
										memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist2);
										CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, colorCount-dist2);
									}
								}
								else
								{
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
								}
							}
							// xIndex가 음수(-)인 경우
							else
							{					
								dist2 = s_pFilter->GetWidth() - xIndex ;

								// 몇 점을 건너서.. Filter를 출력해야 하는 경우
								if (xIndex+colorCount > 0)
								{							
									// Filter가 부족한 경우
									if (s_pFilter->GetWidth() < xIndex+colorCount)
									{	
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
										// Filter출력
										memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
										
										// - -;;
										CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, colorCount-dist2);
									}
									// Filter가 넉넉한 경우
									else
									{							
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
										// dist2만큼만 Filter출력
										memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
									}						
								}
								else
								{
									// 투명이 아닌 색들을 Surface에 출력한다.
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
								}
							}
							pDestTemp	+= colorCount;
							pPixels		+= colorCount;
							xIndex		+= colorCount;
							pFilter		+= colorCount;


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

							xIndex	+= transCount + dist;
							pFilter	+= transCount + dist;

							// 투명이 아닌 색들을 Surface에 출력한다.
							//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
							//pDestTemp += colorCount-dist;
							//pPixels += colorCount;
							
							//------------------------------------------
							// Filter를 출력해야 하는 경우
							//------------------------------------------
							if (xIndex >= 0)
							{			
								dist2  = s_pFilter->GetWidth() - xIndex;
								if (dist2 > 0)
								{						
									// Filter가 넉넉한 경우
									if (dist2 >= colorCount-dist)
									{
										// 투명이 아닌 색들을 Surface에 출력한다.
										memcpyAlphaFilterDarkness(pDestTemp, pPixels+dist, pFilter, colorCount-dist);
									}
									// Filter가 부족한 경우
									else
									{
										// dist2만큼만 Filter출력
										memcpyAlphaFilterDarkness(pDestTemp, pPixels+dist, pFilter, dist2);
										CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist+dist2, colorCount-dist - dist2);
									}								
								}
								else
								{
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, colorCount-dist);
								}							
							}
							// xIndex가 음수(-)인 경우
							else
							{					
								dist2 = s_pFilter->GetWidth() - xIndex ;

								// 몇 점을 건너서.. Filter를 출력해야 하는 경우
								if (xIndex+colorCount-dist > 0)
								{							
									// Filter가 부족한 경우
									if (s_pFilter->GetWidth() < xIndex+(colorCount-dist))
									{	
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, -xIndex);
										// Filter출력
										memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels+dist-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
										
										// - -;;
										CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist+dist2, colorCount-dist - dist2);
									}
									// Filter가 넉넉한 경우
									else
									{							
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, -xIndex);
										// dist2만큼만 Filter출력
										memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels+dist-xIndex, pFilter-xIndex, (colorCount-dist)+xIndex);
									}						
								}
								else
								{
									// 투명이 아닌 색들을 Surface에 출력한다.
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, colorCount-dist);
								}
							}

							pDestTemp += colorCount-dist;
							pPixels += colorCount;
							xIndex	+= colorCount-dist;
							pFilter += colorCount-dist;
							
							// 이제부터는 계속 출력한다.
							break;
						}
						
							
					}	
					
					// 투명색 부분 증가
					xIndex += transCount;
					pFilter += transCount;

					// 투명이 아닌 색만큼 index증가				
					pPixels += colorCount;
					index	+= colorCount;
					xIndex	+= colorCount;
					pFilter += colorCount;
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
						pDestTemp	+= transCount;			
						xIndex		+= transCount;
						pFilter		+= transCount;

						//------------------------------------------
						// Filter를 출력해야 하는 경우
						//------------------------------------------
						if (xIndex >= 0)
						{			
							dist2  = s_pFilter->GetWidth() - xIndex;
							if (dist2 > 0)
							{						
								// Filter가 넉넉한 경우
								if (dist2 >= colorCount)
								{
									// 투명이 아닌 색들을 Surface에 출력한다.
									memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, colorCount);
								}
								// Filter가 부족한 경우
								else
								{
									// dist2만큼만 Filter출력
									memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist2);
									CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, colorCount-dist2);
								}
							}
							else
							{
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
							}
						}
						// xIndex가 음수(-)인 경우
						else
						{					
							dist2 = s_pFilter->GetWidth() - xIndex ;

							// 몇 점을 건너서.. Filter를 출력해야 하는 경우
							if (xIndex+colorCount > 0)
							{							
								// Filter가 부족한 경우
								if (s_pFilter->GetWidth() < xIndex+colorCount)
								{	
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
									// Filter출력
									memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
									
									// - -;;
									CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, colorCount-dist2);
								}
								// Filter가 넉넉한 경우
								else
								{							
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
									// dist2만큼만 Filter출력
									memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
								}						
							}
							else
							{
								// 투명이 아닌 색들을 Surface에 출력한다.
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
							}
						}

						// memory addr 증가
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;		
						xIndex		+= colorCount;
						pFilter		+= colorCount;
					} while (--j);
				}					
			}
		}		

		pDest = (WORD*)((BYTE*)pDest + pitch);
		yIndex ++;
	}
}

//----------------------------------------------------------------------
// Blt AlphaFilter Darkness Clip Right
//----------------------------------------------------------------------
void		
CSprite::BltAlphaFilterDarknessClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	// 어둡게 하는 bit값 설정
	s_Value1 = DarkBits;
	CSpriteSurface::s_Value1 = DarkBits;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index;

	BYTE	*pFilter;
	int		dist, dist2;

	register int	yIndex = -s_Y + pRect->top;
	register int	xIndex;

	register int	i;
	register int	j;

	int rectBottom = pRect->bottom;
	int rectRight = pRect->right;
	for (i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;	
		
		//------------------------------------------
		// Filter를 출력하지 않는 경우
		//------------------------------------------
		if (yIndex < 0 || yIndex >= s_pFilter->GetHeight())
		{
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
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, rectRight-index);
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					// 출력
					CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
					pDestTemp += colorCount;
					pPixels += colorCount;			
					index += colorCount;
				} while (--j);
			}
		}
		//------------------------------------------
		// Filter를 출력하는 경우
		//------------------------------------------
		else
		{
			// 출력할려는 줄에 관련된 Filter를 얻는다.
			pFilter = s_pFilter->GetFilter( yIndex );
			pFilter += -s_X;
			xIndex = -s_X;		

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
							pDestTemp	+= transCount;
							xIndex		+= transCount;
							pFilter		+= transCount;

							dist		= rectRight - index;
						
							// 투명이 아닌 색들을 Surface에 출력한다.
							//memcpy((void*)pDestTemp, (void*)pPixels, (rectRight - index)<<1);
							//------------------------------------------
							// Filter를 출력해야 하는 경우
							//------------------------------------------
							if (xIndex >= 0)
							{	
								dist2  = s_pFilter->GetWidth() - xIndex;
								if (dist2 > 0)
								{						
									// Filter가 넉넉한 경우
									if (dist2 >= dist)
									{
										// 투명이 아닌 색들을 Surface에 출력한다.
										memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist);
									}
									// Filter가 부족한 경우
									else
									{
										// dist2만큼만 Filter출력
										memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist2);
										CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, dist - dist2);
									}
								}
								else
								{
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, dist);
								}
							}
							// xIndex가 음수(-)인 경우
							else
							{					
								dist2 = s_pFilter->GetWidth() - xIndex ;

								// 몇 점을 건너서.. Filter를 출력해야 하는 경우
								if (xIndex + dist > 0)
								{							
									// Filter가 부족한 경우
									if (s_pFilter->GetWidth() < xIndex+colorCount)
									{	
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
										// Filter출력
										memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
										
										// - -;;
										CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, dist - dist2);
									}
									// Filter가 넉넉한 경우
									else
									{							
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
										// dist2만큼만 Filter출력
										memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, dist + xIndex);
									}						
								}
								else
								{
									// 투명이 아닌 색들을 Surface에 출력한다.
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, dist);
								}
							}
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp	+= transCount;
					xIndex		+= transCount;
					pFilter		+= transCount;

					// 출력
					//------------------------------------------
					// Filter를 출력해야 하는 경우
					//------------------------------------------
					if (xIndex >= 0)
					{			
						dist2  = s_pFilter->GetWidth() - xIndex;
						if (dist2 > 0)
						{						
							// Filter가 넉넉한 경우
							if (dist2 >= colorCount)
							{
								// 투명이 아닌 색들을 Surface에 출력한다.
								memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, colorCount);
							}
							// Filter가 부족한 경우
							else
							{
								// dist2만큼만 Filter출력
								memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist2);
								CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, colorCount-dist2);
							}
						}
						else
						{
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						}
					}
					// xIndex가 음수(-)인 경우
					else
					{					
						dist2 = s_pFilter->GetWidth() - xIndex ;

						// 몇 점을 건너서.. Filter를 출력해야 하는 경우
						if (xIndex+colorCount > 0)
						{							
							// Filter가 부족한 경우
							if (s_pFilter->GetWidth() < xIndex+colorCount)
							{	
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
								// Filter출력
								memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
								
								// - -;;
								CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, colorCount-dist2);
							}
							// Filter가 넉넉한 경우
							else
							{							
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
								// dist2만큼만 Filter출력
								memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
							}						
						}
						else
						{
							// 투명이 아닌 색들을 Surface에 출력한다.
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						}
					}

					pDestTemp	+= colorCount;
					pPixels		+= colorCount;	
					xIndex		+= colorCount;
					pFilter		+= colorCount;
					index		+= colorCount;
				} while (--j);
			}
		}
		
		pDest = (WORD*)((BYTE*)pDest + pitch);
		yIndex ++;
	}
}


//----------------------------------------------------------------------
// Blt AlphaFilter Darkness Clip Width
//----------------------------------------------------------------------
void		
CSprite::BltAlphaFilterDarknessClipWidth(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	// 어둡게 하는 bit값 설정
	s_Value1 = DarkBits;
	CSpriteSurface::s_Value1 = DarkBits;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index;
			

	BYTE	*pFilter;
	int		dist, dist2;

	register int	yIndex = -s_Y + pRect->top;
	register int	xIndex;

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
		pPixels = m_Pixels[i];
		pDestTemp = pDest;	
		
		//------------------------------------------
		// Filter를 출력하지 않는 경우
		//------------------------------------------
		if (yIndex < 0 || yIndex >= s_pFilter->GetHeight())
		{
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
									//memset((void*)pDestTemp, 0, (rectRight - index)<<1);
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, (rectRight - index)<<1);									
								}

								j = 0;
								
								break;
							}

							// 이번 단계는 모두 출력
							memcpy(pDestTemp, pPixels, colorCount<<1);
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
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, rectRight - rectLeft);

								j = 0;						
								break;
							}		

							// 투명이 아닌 색들을 Surface에 출력한다.
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, colorCount-dist);
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
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, rectRight-index);
								break;
							}
						}

						// 투명색만큼 건너띄고
						pDestTemp += transCount;

						// 출력
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += colorCount;			
						index += colorCount;
					} while (--j);
				}
			}
		}
		//------------------------------------------
		// Filter를 출력해야 하는 경우
		//------------------------------------------
		else
		{
			// 출력할려는 줄에 관련된 Filter를 얻는다.
			pFilter = s_pFilter->GetFilter( yIndex );
			pFilter += -s_X;
			xIndex = -s_X;

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
							xIndex += transCount;//index - rectLeft;
							pFilter += transCount;//index - rectLeft;

							// 이번 단계는 모두 출력
							//memcpy(pDestTemp, pPixels, colorCount<<1);
							//------------------------------------------
							// Filter를 출력해야 하는 경우
							//------------------------------------------
							if (xIndex >= 0)
							{			
								dist2  = s_pFilter->GetWidth() - xIndex;
								if (dist2 > 0)
								{						
									// Filter가 넉넉한 경우
									if (dist2 >= colorCount)
									{
										// 투명이 아닌 색들을 Surface에 출력한다.
										memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, colorCount);
									}
									// Filter가 부족한 경우
									else
									{
										// dist2만큼만 Filter출력
										memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist2);
										CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, colorCount-dist2);
									}
								}
								else
								{
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
								}
							}
							// xIndex가 음수(-)인 경우
							else
							{					
								dist2 = s_pFilter->GetWidth() - xIndex ;

								// 몇 점을 건너서.. Filter를 출력해야 하는 경우
								if (xIndex+colorCount > 0)
								{							
									// Filter가 부족한 경우
									if (s_pFilter->GetWidth() < xIndex+colorCount)
									{	
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
										// Filter출력
										memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
										
										// - -;;
										CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, colorCount-dist2);
									}
									// Filter가 넉넉한 경우
									else
									{							
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
										// dist2만큼만 Filter출력
										memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
									}						
								}
								else
								{
									// 투명이 아닌 색들을 Surface에 출력한다.
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
								}
							}
							pDestTemp	+= colorCount;
							pPixels		+= colorCount;
							xIndex		+= colorCount;
							pFilter		+= colorCount;
							index		+= colorCount;

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

							xIndex	+= transCount + dist;
							pFilter	+= transCount + dist;

							// 투명이 아닌 색들을 Surface에 출력한다.
							//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
							//pDestTemp += colorCount-dist;
							//pPixels += colorCount;
							
							//------------------------------------------
							// Filter를 출력해야 하는 경우
							//------------------------------------------
							if (xIndex >= 0)
							{			
								dist2  = s_pFilter->GetWidth() - xIndex;
								if (dist2 > 0)
								{						
									// Filter가 넉넉한 경우
									if (dist2 >= colorCount-dist)
									{
										// 투명이 아닌 색들을 Surface에 출력한다.
										memcpyAlphaFilterDarkness(pDestTemp, pPixels+dist, pFilter, colorCount-dist);
									}
									// Filter가 부족한 경우
									else
									{
										// dist2만큼만 Filter출력
										memcpyAlphaFilterDarkness(pDestTemp, pPixels+dist, pFilter, dist2);
										CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist+dist2, colorCount-dist - dist2);
									}								
								}
								else
								{
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, colorCount-dist);
								}							
							}
							// xIndex가 음수(-)인 경우
							else
							{					
								dist2 = s_pFilter->GetWidth() - xIndex ;

								// 몇 점을 건너서.. Filter를 출력해야 하는 경우
								if (xIndex+colorCount-dist > 0)
								{							
									// Filter가 부족한 경우
									if (s_pFilter->GetWidth() < xIndex+(colorCount-dist))
									{	
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, -xIndex);
										// Filter출력
										memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels+dist-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
										
										// - -;;
										CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist+dist2, colorCount-dist - dist2);
									}
									// Filter가 넉넉한 경우
									else
									{							
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, -xIndex);
										// dist2만큼만 Filter출력
										memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels+dist-xIndex, pFilter-xIndex, (colorCount-dist)+xIndex);
									}						
								}
								else
								{
									// 투명이 아닌 색들을 Surface에 출력한다.
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, colorCount-dist);
								}
							}

							pDestTemp += colorCount-dist;
							pPixels		+= colorCount;
							xIndex		+= colorCount-dist;
							pFilter		+= colorCount-dist;
							index		+= colorCount;
							
							// 이제부터는 계속 출력한다.
							break;
						}
					}	
					
					// 투명색 부분 증가
					xIndex += transCount;
					pFilter += transCount;

					// 투명이 아닌 색만큼 index증가				
					pPixels += colorCount;
					index	+= colorCount;
					xIndex	+= colorCount;
					pFilter += colorCount;
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
								pDestTemp	+= transCount;
								xIndex		+= transCount;
								pFilter		+= transCount;

								dist		= rectRight - index;
							
								// 투명이 아닌 색들을 Surface에 출력한다.
								//memcpy((void*)pDestTemp, (void*)pPixels, (rectRight - index)<<1);
								//------------------------------------------
								// Filter를 출력해야 하는 경우
								//------------------------------------------
								if (xIndex >= 0)
								{	
									dist2  = s_pFilter->GetWidth() - xIndex;
									if (dist2 > 0)
									{						
										// Filter가 넉넉한 경우
										if (dist2 >= dist)
										{
											// 투명이 아닌 색들을 Surface에 출력한다.
											memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist);
										}
										// Filter가 부족한 경우
										else
										{
											// dist2만큼만 Filter출력
											memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist2);
											CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, dist - dist2);
										}
									}
									else
									{
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, dist);
									}
								}
								// xIndex가 음수(-)인 경우
								else
								{					
									dist2 = s_pFilter->GetWidth() - xIndex ;

									// 몇 점을 건너서.. Filter를 출력해야 하는 경우
									if (xIndex + dist > 0)
									{							
										// Filter가 부족한 경우
										if (s_pFilter->GetWidth() < xIndex+colorCount)
										{	
											CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
											// Filter출력
											memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
											
											// - -;;
											CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, dist - dist2);
										}
										// Filter가 넉넉한 경우
										else
										{							
											CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
											// dist2만큼만 Filter출력
											memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, dist + xIndex);
										}						
									}
									else
									{
										// 투명이 아닌 색들을 Surface에 출력한다.
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, dist);
									}
								}
								break;
							}
						}

						// 투명색만큼 건너띄고
						pDestTemp	+= transCount;
						xIndex		+= transCount;
						pFilter		+= transCount;

						// 출력
						//------------------------------------------
						// Filter를 출력해야 하는 경우
						//------------------------------------------
						if (xIndex >= 0)
						{			
							dist2  = s_pFilter->GetWidth() - xIndex;
							if (dist2 > 0)
							{						
								// Filter가 넉넉한 경우
								if (dist2 >= colorCount)
								{
									// 투명이 아닌 색들을 Surface에 출력한다.
									memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, colorCount);
								}
								// Filter가 부족한 경우
								else
								{
									// dist2만큼만 Filter출력
									memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist2);
									CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, colorCount-dist2);
								}
							}
							else
							{
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
							}
						}
						// xIndex가 음수(-)인 경우
						else
						{					
							dist2 = s_pFilter->GetWidth() - xIndex ;

							// 몇 점을 건너서.. Filter를 출력해야 하는 경우
							if (xIndex+colorCount > 0)
							{							
								// Filter가 부족한 경우
								if (s_pFilter->GetWidth() < xIndex+colorCount)
								{	
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
									// Filter출력
									memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
									
									// - -;;
									CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, colorCount-dist2);
								}
								// Filter가 넉넉한 경우
								else
								{							
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
									// dist2만큼만 Filter출력
									memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
								}						
							}
							else
							{
								// 투명이 아닌 색들을 Surface에 출력한다.
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
							}
						}

						pDestTemp	+= colorCount;
						pPixels		+= colorCount;	
						xIndex		+= colorCount;
						pFilter		+= colorCount;
						index		+= colorCount;
					} while (--j);
				}
			}
		}		

		pDest = (WORD*)((BYTE*)pDest + pitch);
		yIndex ++;
	}
}

//----------------------------------------------------------------------
// Blt AlphaFilter Darkness Clip Height
//----------------------------------------------------------------------
// pRect->top, pRect->bottom만큼만 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltAlphaFilterDarknessClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	// 어둡게 하는 bit값 설정
	s_Value1 = DarkBits;
	CSpriteSurface::s_Value1 = DarkBits;

	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;
	BYTE	*pFilter;
	int		dist;

	register int	yIndex = -s_Y + pRect->top;
	register int	xIndex;

	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	for (i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		//------------------------------------------
		// Filter를 출력하지 않는 경우
		//------------------------------------------
		if (yIndex < 0 || yIndex >= s_pFilter->GetHeight())
		{
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
					CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
					
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;
				} while (--j);
			}
		}
		//------------------------------------------
		// Filter를 출력해야 하는 경우
		//------------------------------------------
		else
		{
			// 출력할려는 줄에 관련된 Filter를 얻는다.
			pFilter = s_pFilter->GetFilter( yIndex );
			pFilter += -s_X;
			xIndex = -s_X;		// xIndex가 x를 이상인 경우 출력하면 된다.

			// (투명수,색깔수,색깔들)의 반복 수		
			count	= *pPixels++;		
 			// 한 줄 출력
			if (count > 0)
			{
				j = count;
				do 
				{
					pDestTemp	+= *pPixels;			// 투명색만큼 건너 뛴다.
					xIndex		+= *pPixels;
					pFilter		+= *pPixels;
					pPixels ++;
					colorCount = *pPixels++;			// 투명 아닌 색 수				

					//------------------------------------------
					// Filter를 출력해야 하는 경우
					//------------------------------------------
					if (xIndex >= 0)
					{			
						dist  = s_pFilter->GetWidth() - xIndex;
						if (dist > 0)
						{						
							// Filter가 넉넉한 경우
							if (dist >= colorCount)
							{
								// 투명이 아닌 색들을 Surface에 출력한다.
								memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, colorCount);
							}
							// Filter가 부족한 경우
							else
							{
								// dist만큼만 Filter출력
								memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist);
								CSpriteSurface::memcpyDarkness(pDestTemp+dist, pPixels+dist, colorCount-dist);
							}
						}
						else
						{
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						}
					}
					// xIndex가 음수(-)인 경우
					else
					{					
						dist = s_pFilter->GetWidth() - xIndex ;

						// 몇 점을 건너서.. Filter를 출력해야 하는 경우
						if (xIndex+colorCount > 0)
						{							
							// Filter가 부족한 경우
							if (s_pFilter->GetWidth() < xIndex+colorCount)
							{	
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
								// Filter출력
								memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
								
								// - -;;
								CSpriteSurface::memcpyDarkness(pDestTemp+dist, pPixels+dist, colorCount-dist);
							}
							// Filter가 넉넉한 경우
							else
							{							
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
								// dist만큼만 Filter출력
								memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
							}						
						}
						else
						{
							// 투명이 아닌 색들을 Surface에 출력한다.
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						}
					}

					pDestTemp	+= colorCount;
					pPixels		+= colorCount;
					xIndex		+= colorCount;
					pFilter		+= colorCount;
				} while (--j);
			}
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
		yIndex++;
	}
}


//----------------------------------------------------------------------
// memcpy Filter Darkness
//----------------------------------------------------------------------
// pFilter를 이용해서 출력한다.
// pFilter를 alpha 값으로 이용한다.
//----------------------------------------------------------------------
void
CSprite::memcpyAlphaFilterDarkness(WORD* pDest, WORD* pSource, BYTE* pFilter, WORD pixels)
{
	register WORD		sTemp,dTemp;
	register int		sr,sg,sb,dr,dg,db;
	//static WORD		temp, temp2;

	register int j = pixels;

	BYTE alpha;

	// Alpha Channel Blending
	// 한점씩 찍기
	while (j--)
	{			
		alpha = *pFilter;		

		// 한점 찍기
		///*
		sTemp = *pSource;
		dTemp = *pDest;

		sr = CDirectDraw::Red(sTemp);
		sg = CDirectDraw::Green(sTemp);
		sb = CDirectDraw::Blue(sTemp);

		dr = CDirectDraw::Red(dTemp);
		dg = CDirectDraw::Green(dTemp);
		db = CDirectDraw::Blue(dTemp);		
		
		*pDest = ((alpha * (sb - db) >> 5) + db |
					((alpha * (sg - dg) >> 5) + dg) << CDirectDraw::s_bSHIFT_G |
					((alpha * (sr - dr) >> 5) + dr) << CDirectDraw::s_bSHIFT_R);

		*pDest = (*pDest >> s_Value1) & CDirectDraw::s_wMASK_SHIFT[s_Value1];
		//*/

		//if (alpha > 26) 
		{
			//*pDest = *pSource; 
		//	*pDest = (*pSource >> s_Value1) & CDirectDraw::s_wMASK_SHIFT[s_Value1];
		}


		pDest++;
		pSource++;
		pFilter++;
	}
}


//----------------------------------------------------------------------
// BltDarkerFilter
//----------------------------------------------------------------------
// Clipping하지 않는다.
//----------------------------------------------------------------------
void
CSprite::BltDarkerFilter(WORD *pDest, WORD pitch)
{
	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;
	BYTE	*pFilter;

	register int i;
	register int j;

	int rectBottom = m_Height;
	
	if (rectBottom > 0)
	{
		i = rectBottom-1;
		pDest = (WORD*)((BYTE*)pDest + i*pitch);

		do
		{
			pPixels		= m_Pixels[i];
			pFilter		= s_pFilter->GetFilter( i );
			pDestTemp	= pDest;

			// (투명수,색깔수,색깔들)의 반복 수		
			count	= *pPixels++;		

			// 한 줄 출력
			if (count > 0)
			{
				j = count;
				do 
				{
					pDestTemp	+= *pPixels;		// 투명색만큼 건너 뛴다.
					pFilter		+= *pPixels;
					pPixels++;
					colorCount = *pPixels++;		// 투명 아닌 색 수				

					// 투명이 아닌 색들을 Surface에 출력한다.
					memcpyDarkerFilter(pDestTemp, pPixels, pFilter, colorCount);
					
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;
					pFilter		+= colorCount;
				} while (--j);
			}

			pDest = (WORD*)((BYTE*)pDest - pitch);
		} while (i--);
	}
}

//----------------------------------------------------------------------
// BltDarkerFilter ClipLeft
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltDarkerFilterClipLeft(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;
	BYTE	*pFilter;

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
		pPixels = m_Pixels[i];
		pFilter		= s_pFilter->GetFilter( i );
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
						pDestTemp	+= index - rectLeft;
						pFilter		+= index - rectLeft;

						// 이번 단계는 모두 출력
						memcpyDarkerFilter(pDestTemp, pPixels, pFilter, colorCount);
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;
						pFilter		+= colorCount;

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
						memcpyDarkerFilter(pDestTemp, pPixels+dist, pFilter+dist, colorCount-dist);
						pDestTemp += colorCount-dist;
						pPixels += colorCount;
						pFilter	+= colorCount;

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
					pDestTemp	+= transCount;
					pFilter		+= transCount;
					
					// 투명색이 아닌만큼 출력해준다.
					memcpyDarkerFilter(pDestTemp, pPixels, pFilter, colorCount);

					// memory addr 증가
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;	
					pFilter		+= colorCount;
				} while (--j);
			}
		}
		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltDarkerFilter ClipRight
//----------------------------------------------------------------------
// 오른쪽 clipping.  
// pRect->right개 까지의 점만 pDest에 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltDarkerFilterClipRight(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;
	BYTE	*pFilter;

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
		pPixels = m_Pixels[i];
		pFilter		= s_pFilter->GetFilter( i );
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
						pDestTemp	+= transCount;
						pFilter		+= transCount;
					
						// 투명이 아닌 색들을 Surface에 출력한다.
						memcpyDarkerFilter(pDestTemp, pPixels, pFilter, rectRight - index);
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp	+= transCount;
				pFilter		+= transCount;

				// 출력
				memcpyDarkerFilter(pDestTemp, pPixels, pFilter, colorCount);
				pDestTemp	+= colorCount;
				pPixels		+= colorCount;
				pFilter		+= colorCount;
				index		+= colorCount;
			} while (--j);
		}
		
		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltDarkerFilter ClipWidth
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltDarkerFilterClipWidth(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;
	BYTE	*pFilter;

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
		pPixels = m_Pixels[i];
		pFilter		= s_pFilter->GetFilter( i );
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
						pDestTemp	+= index - rectLeft;
						pFilter		+= index - rectLeft;

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
								//memset((void*)pDestTemp, 0, (rectRight - index)<<1);
								memcpyDarkerFilter(pDestTemp, pPixels, pFilter, rectRight - index);
							}

							j = 0;
							
							break;
						}

						// 이번 단계는 모두 출력
						memcpyDarkerFilter(pDestTemp, pPixels, pFilter, colorCount);
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;
						pFilter		+= colorCount;
						index		+= colorCount;

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
							memcpyDarkerFilter(pDestTemp, pPixels+dist, pFilter+dist, rectRight - rectLeft);

							j = 0;						
							break;
						}		

						// 투명이 아닌 색들을 Surface에 출력한다.
						memcpyDarkerFilter(pDestTemp, pPixels+dist, pFilter+dist, colorCount-dist);
						pDestTemp	+= colorCount-dist;
						pPixels		+= colorCount;
						pFilter		+= colorCount;
						index		+= colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}					

				// 투명이 아닌 색만큼 index증가				
				pPixels += colorCount;
				pFilter	+= colorCount;
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
							pDestTemp	+= transCount;
							pFilter		+= transCount;
						
							// 투명이 아닌 색들을 Surface에 출력한다.
							memcpyDarkerFilter(pDestTemp, pPixels, pFilter, rectRight - index);
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp	+= transCount;
					pFilter		+= transCount;

					// 출력
					memcpyDarkerFilter(pDestTemp, pPixels, pFilter, colorCount);
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;			
					pFilter		+= colorCount;
					index		+= colorCount;				
				} while (--j);
			}
		}
		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltDarkerFilter Clip Height
//----------------------------------------------------------------------
// pRect->top, pRect->bottom만큼만 출력한다.
//----------------------------------------------------------------------
void
CSprite::BltDarkerFilterClipHeight(WORD *pDest, WORD pitch, RECT* pRect)
{
	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;
	BYTE	*pFilter;


	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	for (i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
		pFilter		= s_pFilter->GetFilter( i );
		pDestTemp	= pDest;

		// (투명수,색깔수,색깔들)의 반복 수		
		count	= *pPixels++;		

		// 한 줄 출력
		if (count > 0)
		{
			j = count;
			do 
			{				
				pDestTemp	+= *pPixels;		// 투명색만큼 건너 뛴다.
				pFilter		+= *pPixels;
				pPixels++;
				colorCount = *pPixels++;		// 투명 아닌 색 수				

				// 투명이 아닌 색들을 Surface에 출력한다.
				memcpyDarkerFilter(pDestTemp, pPixels, pFilter, colorCount);
				
				pDestTemp	+= colorCount;
				pPixels		+= colorCount;
				pFilter		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltAlpha4444NotTrans
//----------------------------------------------------------------------
// Clipping하지 않는다.
//----------------------------------------------------------------------
void
CSprite::BltAlpha4444NotTrans(WORD *pDest, WORD pitch, BYTE alpha)
{
	s_Value1 = alpha >> 1;	// 4 bit이므로

	int		count,	
			transCount,
			colorCount;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;

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

					pPixels		+= colorCount;
				} while (--j);
			}

			pDest = (WORD*)((BYTE*)pDest - pitch);
		} while (i--);
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
CSprite::memcpyAlpha4444(WORD* pDest, WORD* pSource, WORD pixels)
{
	register WORD		sTemp;
	register int		sr,sg,sb;
	//static WORD		temp, temp2;

	register int i = pixels;

	// Alpha Channel Blending
	// 한점씩 찍기
	while (i--)
	{	
		// 한점 찍기
		sTemp = *pSource;
	
		sr = (sTemp >> CDirectDraw::s_bSHIFT4_R);// & 0x0F;
		sg = (sTemp >> CDirectDraw::s_bSHIFT4_G) & 0x0F;
		sb = (sTemp >> CDirectDraw::s_bSHIFT4_B) & 0x0F;
		
		*pDest =	(s_Value1 << 12) |
					(sr << 8) |
					(sg << 4) |
					(sb);
	
		pDest++;
		pSource++;
	}
}

//----------------------------------------------------------------------
// BltAlpha4444SmallNotTrans
//----------------------------------------------------------------------
// 축소해서 출력.. 
// Clipping하지 않는다.
//
// alpha값은 50%(반투명)으로 한다.
//----------------------------------------------------------------------
void
CSprite::BltAlpha4444SmallNotTrans(WORD *pDest, WORD pitch, BYTE alpha, BYTE shift)
{
	s_Value1 = alpha >> 1;	// 4 bit이므로
	s_Value2 = shift;
	// memcpy에서 건너띄는 값
	s_Value3 = 1 << shift;


	int		count,	
			transCount,
			colorCount,
			transCountShift,
			colorCountShift;

	WORD	*pDestTemp,
			*pPixels;

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

					// shift만큼 줄여진 값을 계산한다.
					transCountShift = transCount >> shift;
					colorCountShift = colorCount >> shift;

					// 0을 출력한다.
					memset(pDestTemp, 0, transCountShift<<1);
					pDestTemp += transCountShift;		// 투명색만큼 건너 뛴다.

					// 투명이 아닌 색들을 Surface에 출력한다.
					memcpyAlpha4444Small(pDestTemp, pPixels, colorCount);
					
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
// Alpha Copy  4444 Small
//----------------------------------------------------------------------
// Alpha값 : 1~32
//----------------------------------------------------------------------
// pSource의 것을 pDest에 출력을 해야한다.
// pSource의 구성은 (alpha,색깔 하나)의 pixels만큼 반복이다.
//
// A:R:G:B = 4:4:4:4 Texture를 위한 것이다.
//
// s_Value1은 alpha값
// s_Value2는 shift값
// s_Value3은 건너띄는 값(맞나? 기억이 가물가물. - -;;)
//----------------------------------------------------------------------
void	
CSprite::memcpyAlpha4444Small(WORD* pDest, WORD* pSource, WORD pixels)
{
	register WORD		sTemp;
	register int		sr,sg,sb;
	//static WORD		temp, temp2;

	register int i = pixels >> s_Value2;

	// 한 값으로 Alpha Blending
	// 한점씩 찍기
	while (i--)
	{	
		// 한점 찍기
		sTemp = *pSource;
	
		sr = (sTemp >> CDirectDraw::s_bSHIFT4_R);// & 0x0F;
		sg = (sTemp >> CDirectDraw::s_bSHIFT4_G) & 0x0F;
		sb = (sTemp >> CDirectDraw::s_bSHIFT4_B) & 0x0F;
		
		*pDest =	(s_Value1 << 12) |
					(sr << 8) |
					(sg << 4) |
					(sb);
	
		pDest++;
		pSource += s_Value3;
	}
}


//----------------------------------------------------------------------
// Blt1555NotTrans
//----------------------------------------------------------------------
// Clipping하지 않는다.
//----------------------------------------------------------------------
void
CSprite::Blt1555NotTrans(WORD *pDest, WORD pitch)
{
	//s_Value1 = alpha >> 4;	// 1 bit이므로

	int		count,	
			transCount,
			colorCount;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;

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
					memcpy1555(pDestTemp, pPixels, colorCount);
					
					pDestTemp	+= colorCount;

					pPixels		+= colorCount;
				} while (--j);
			}

			pDest = (WORD*)((BYTE*)pDest - pitch);
		} while (i--);
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
CSprite::memcpy1555(WORD* pDest, WORD* pSource, WORD pixels)
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
		sTemp = *pSource;
	
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
// Blt1555SmallNotTrans
//----------------------------------------------------------------------
// 축소해서 출력.. 
// Clipping하지 않는다.
//
// alpha값은 50%(반투명)으로 한다.
//----------------------------------------------------------------------
void
CSprite::Blt1555SmallNotTrans(WORD *pDest, WORD pitch, BYTE shift)
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

	WORD	*pDestTemp,
			*pPixels;

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

					// shift만큼 줄여진 값을 계산한다.
					transCountShift = transCount >> shift;
					colorCountShift = colorCount >> shift;

					// 0을 출력한다.
					memset(pDestTemp, 0, transCountShift<<1);
					pDestTemp += transCountShift;		// 투명색만큼 건너 뛴다.

					// 투명이 아닌 색들을 Surface에 출력한다.
					memcpy1555Small(pDestTemp, pPixels, colorCount);
					
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
CSprite::memcpy1555Small(WORD* pDest, WORD* pSource, WORD pixels)
{
	register int		sTemp;
	register int		sr,sg,sb;
	//static WORD		temp, temp2;

	register int i = pixels >> s_Value2;

	// 한점씩 찍기
	while (i--)
	{	
		// 한점 찍기
		sTemp = *pSource;
	
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

DWORD
CSprite::GetFileSize()
{
	DWORD fileSize = 0;

	// width와 height를 저장한다.
	fileSize += 2;
	fileSize += 2;
	
	// NULL이면 저장하지 않는다. 길이만 저장되는 것이다.
	if (m_Pixels==NULL || m_Width==0 || m_Height==0)
		return 0;
	
	// 압축 된 것 저장
	WORD index;	
	
	register int i;
	register int j;
	
	//--------------------------------
	// 5:6:5
	//--------------------------------
	for (i=0; i<m_Height; i++)
	{
		// 반복 회수의 2 byte
		int	count = m_Pixels[i][0], 
			colorCount;
		index	= 1;
		
		// 각 line마다 byte수를 세어서 저장해야한다.
		for (j=0; j<count; j++)
		{
			//transCount = m_Pixels[i][index];
			colorCount = m_Pixels[i][index+1];
			
			index+=2;	// 두 count 만큼
			
			index += colorCount;	// 투명색 아닌것만큼 +				
		}
		
		// byte수와 실제 data를 저장한다.
		fileSize += 2;
		fileSize += index<<1;
	}

	return fileSize;
}