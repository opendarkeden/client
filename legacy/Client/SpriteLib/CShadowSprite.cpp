//----------------------------------------------------------------------
// CShadowSprite.cpp
//----------------------------------------------------------------------
#include "client_PCH.h"
#include "CSpriteSurface.h"
#include "CFilter.h"
#include "CShadowSprite.h"


//----------------------------------------------------------------------
// Static member
//----------------------------------------------------------------------
WORD	CShadowSprite::s_Colorkey = 0;
BYTE	CShadowSprite::s_Value1 = 1;
BYTE	CShadowSprite::s_Value2 = 31;
WORD	CShadowSprite::s_wValue1 = 1;


//----------------------------------------------------------------------
// Init Static member
//----------------------------------------------------------------------


//----------------------------------------------------------------------
//
// constructor/destructore
//
//----------------------------------------------------------------------

CShadowSprite::CShadowSprite()
{
	m_Width		= 0;
	m_Height	= 0;
	m_Pixels	= NULL;	
	m_bInit		= false;
}

CShadowSprite::~CShadowSprite()
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
CShadowSprite::Release()
{
	if (m_Pixels!=NULL)
	{		
		m_bInit		= false;

		for (int i=0; i<m_Height; i++)
			delete [] m_Pixels[i];
		
		delete [] m_Pixels;

		m_Pixels	= NULL;
		m_Width		= 0;
		m_Height	= 0;	
	}
}

//----------------------------------------------------------------------
// operator = 
//----------------------------------------------------------------------
void		
CShadowSprite::operator = (const CShadowSprite& Sprite)
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
	WORD index;	

	// 메모리 잡기
	m_Pixels = new WORD* [m_Height];

	//--------------------------------
	// 5:6:5
	//--------------------------------
	for (int i=0; i<m_Height; i++)
	{
		// 반복수 + 반복수*2byte
		index	= 1 + m_Pixels[i][0]*2;	

		// 메모리 잡기
		m_Pixels[i] = new WORD [index];
		memcpy(m_Pixels[i], Sprite.m_Pixels[i], index<<1);
	}

}

//----------------------------------------------------------------------
// fstream에 save 한다.    ( file에는 5:6:5로 저장한다. )
//----------------------------------------------------------------------
bool	
CShadowSprite::SaveToFile(ofstream& file)
{
	// width와 height를 저장한다.
	file.write((const char*)&m_Width , 2);
	file.write((const char*)&m_Height, 2);

	// NULL이면 저장하지 않는다. 길이만 저장되는 것이다.
	if (m_Pixels==NULL || m_Width==0 || m_Height==0)
		return false;
	
	// 압축 된 것 저장
	WORD index;	

	//--------------------------------
	// 5:6:5
	//--------------------------------
	for (int i=0; i<m_Height; i++)
	{
		// 반복수 + 반복수*2byte
		index	= 1 + m_Pixels[i][0]*2;	

		// byte수와 실제 data를 저장한다.
		file.write((const char*)&index, 2);
		file.write((const char*)m_Pixels[i], index<<1);
	}


	return true;
}

//----------------------------------------------------------------------
// fstream에서 load한다.
//----------------------------------------------------------------------
bool	
CShadowSprite::LoadFromFile(ifstream& file)
{
	// 이미 잡혀있는 memory를 release한다.
	Release();

	// width와 height를 Load한다.
	file.read((char*)&m_Width , 2);
	file.read((char*)&m_Height, 2);	

	// 길이가 0이면 더 Load할게 없겠지..
	if (m_Width==0 || m_Height==0) 
	{	
		m_bInit = true;

		return true;
	}

	m_Pixels = NULL;
	m_Pixels = new WORD* [m_Height];	
	//m_Pixels = (WORD**)malloc(sizeof(WORD*)*m_Height);

	WORD len;

	//--------------------------------
	// 5:6:5
	//--------------------------------
	for (int i=0; i<m_Height; i++)
	{
		// byte수와 실제 data를 Load한다.
		file.read((char*)&len, 2);
		
		m_Pixels[i] = NULL;
		m_Pixels[i] = new WORD [len];		
	
		file.read((char*)m_Pixels[i], len<<1);
	}	
	
	m_bInit = true;

	return true;
}

//----------------------------------------------------------------------
// CDirectDrawSurface의 (x,y)+(width, height)영역을 읽어서 m_Pixels에 저장한다.
//----------------------------------------------------------------------
// m_Pixels를 0번 압축 Format으로 바꾼다.
//
// 각 line마다 다음과 같은 구조를 가진다.
//
//    [반복수] (투명수,색깔수)(투명수,색깔수)......
//
// 반복수는 2 bytes이고
// 투명수와 색깔수는 각각 2 byte
//
//----------------------------------------------------------------------
void
CShadowSprite::SetPixel(WORD *pSource, WORD pitch, WORD width, WORD height)
{
	// memory해제
	Release();

	m_Width = width;
	m_Height = height;

	// 일단 memory를 적당히 잡아둔다.	
	WORD*	data = new WORD[m_Width+2];

	int	index;				// data의 index로 사용			
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
			if (*pSourceTemp==s_Colorkey)
			{
				// 최근에 검사한게 투명색이 아니었다면
				if (!bCheckTrans)
				{
					// ' (투명,색깔수) '의 한 set가 끝났음을 의미하므로
					count++;
					
					data[index++] = color;
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

					bCheckTrans = FALSE;
				}

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
			data[index++] = color;
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
// SetPixel
//----------------------------------------------------------------------
// IndexSprite --> ShadowSprite
//----------------------------------------------------------------------
void		
CShadowSprite::SetPixel(CIndexSprite& ispr)
{
	// memory해제
	Release();

	m_Width = ispr.GetWidth();
	m_Height = ispr.GetHeight();

	int		transPair,	
			indexCount,			
			colorCount,
			index;
	
	WORD	*pPixels;

	register int i;
	register int j;	

	// height줄 만큼 memory잡기
	m_Pixels = new WORD* [m_Height];

	for (i=0; i<m_Height; i++)
	{			
		pPixels		= ispr.GetPixelLine( i );
		
		// (투명수,Index색,색깔들)의 반복 수		
		transPair	= *pPixels++;

		// 반복수 + (투명수,그림자수)*반복수
		m_Pixels[i] = new WORD [1 + (transPair<<1)];
		index = 0;
		m_Pixels[i][index++] = transPair;
		
 		// 한 줄 출력
		for (j=0; j<transPair; j++)
		{			
			//------------------------------------
			// 투명색 수
			//------------------------------------
			m_Pixels[i][index++] = *pPixels;	
			pPixels++;			

			indexCount = *pPixels++;	// Index반복 수			
			pPixels += indexCount;		// Index색들만큼 증가
			
			colorCount = *pPixels++;	// color 색깔 수			
			pPixels		+= colorCount;	// Color 색만큼 증가

			//------------------------------------
			// 그림자 수 = Index + Color수
			//------------------------------------
			m_Pixels[i][index++] = indexCount + colorCount;
		}
	}

	m_bInit = true;
}

//----------------------------------------------------------------------
// SetPixel
//----------------------------------------------------------------------
// Sprite --> ShadowSprite
//----------------------------------------------------------------------
void		
CShadowSprite::SetPixel(CSprite& spr)
{
	// memory해제
	Release();

	m_Width = spr.GetWidth();
	m_Height = spr.GetHeight();

	int		transPair,	
			colorCount,
			index;
	
	WORD	*pPixels;

	register int i;
	register int j;	

	// height줄 만큼 memory잡기
	m_Pixels = new WORD* [m_Height];

	for (i=0; i<m_Height; i++)
	{			
		pPixels		= spr.GetPixelLine( i );
		
		// (투명수,색깔들)의 반복 수		
		transPair	= *pPixels++;

		// 반복수 + (투명수,그림자수)*반복수
		m_Pixels[i] = new WORD [1 + (transPair<<1)];
		index = 0;
		m_Pixels[i][index++] = transPair;
		
 		// 한 줄 출력
		for (j=0; j<transPair; j++)
		{			
			//------------------------------------
			// 투명색 수
			//------------------------------------
			m_Pixels[i][index++] = *pPixels;	
			pPixels++;			

			colorCount = *pPixels++;	// color 색깔 수			
			pPixels		+= colorCount;	// Color 색만큼 증가

			//------------------------------------
			// 그림자 수 = Index + Color수
			//------------------------------------
			m_Pixels[i][index++] = colorCount;
		}
	}

	m_bInit = true;
}

/*
void		
CShadowSprite::Uncompress()
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
CShadowSprite::IsColorPixel(short x, short y)
{
	// 아직 초기화 되지 않은 경우
	if (m_Pixels==NULL)
		return false; 

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

	for (int register i=0; i<count; i++)
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
				return false;
			}

			// 색깔에 속한다.
			return true;
		}

		//pPixels += colorCount;
		index += colorCount;
	}

	return false;
}

//----------------------------------------------------------------------
// Blt
//----------------------------------------------------------------------
// Clipping하지 않는다.
//----------------------------------------------------------------------
void
CShadowSprite::Blt(WORD *pDest, WORD pitch)
{
	int	count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;

	for (i=0; i<m_Height; i++)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		// (투명수,색깔수)의 반복 수		
		count	= *pPixels++;		
 		// 한 줄 출력
		if (count > 0)
		{			
			j = count;
			do
			{
				pDestTemp += *pPixels++;			// 투명색만큼 건너 뛴다.
				colorCount = *pPixels++;		// 투명 아닌 색 수				

				// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
				memset((void*)pDestTemp, 0, colorCount<<1);
		
				pDestTemp	+= colorCount;
				//pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt ClipLeft
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CShadowSprite::BltClipLeft(WORD* pDest, WORD pitch, RECT* pRect)
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
	for (i=pRect->top; i<pRect->bottom; i++)
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
				if (index+colorCount > pRect->left)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > pRect->left)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - pRect->left;

						// 이번 단계는 모두 출력
						//memcpy(pDestTemp, pPixels, colorCount<<1);
						// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
						memset((void*)pDestTemp, 0, colorCount<<1);

						pDestTemp += colorCount;
						//pPixels += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우
					//---------------------------------------------
					else
					{
						dist = pRect->left - index;

						// 투명이 아닌 색들을 Surface에 출력한다.
						//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
						// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
						memset((void*)pDestTemp, 0, (colorCount-dist)<<1);

						pDestTemp += colorCount-dist;
						//pPixels += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}					

				// 투명이 아닌 색만큼 index증가				
				//pPixels += colorCount;
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
					
					// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
					memset((void*)pDestTemp, 0, colorCount<<1);

					// memory addr 증가
					pDestTemp += colorCount;
					//pPixels += colorCount;			
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
CShadowSprite::BltClipRight(WORD* pDest, WORD pitch, RECT* pRect)
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

	for (i=pRect->top; i<pRect->bottom; i++)
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
				if (index+colorCount > pRect->right)
				{
					// 투명색만으로 더 출력할 필요가 없을 때
					if (index > pRect->right)
					{
						break;
					}
					// 투명색 아닌 것을 조금 출력해야 할 경우
					else
					{
						pDestTemp += transCount;
					
						// 투명이 아닌 색들을 Surface에 출력한다.
						//memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
						// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
						memset((void*)pDestTemp, 0, (pRect->right - index)<<1);

						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				// 출력
				//memcpy(pDestTemp, pPixels, colorCount<<1);
				// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
				memset((void*)pDestTemp, 0, colorCount<<1);

				pDestTemp += colorCount;
				//pPixels += colorCount;			
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
CShadowSprite::BltClipWidth(WORD* pDest, WORD pitch, RECT* pRect)
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

	register short i;
	register short j;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	for (i=pRect->top; i<pRect->bottom; i++)
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
				if (index+colorCount > pRect->left)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > pRect->left)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - pRect->left;

						// 이번 단계는 모두 출력?
						// 오른쪽 끝을 넘어가는 경우..
						if (index+colorCount > pRect->right)
						{							
							// 투명색만으로 오른쪽 끝 넘어가는 경우
							if (index > pRect->right)
							{
							}
							else
							{
								memset((void*)pDestTemp, 0, (pRect->right - index)<<1);
							}

							j = count;
							
							break;
						}
					
						memset((void*)pDestTemp, 0, colorCount<<1);

						pDestTemp += colorCount;
						//pPixels += colorCount;
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
						dist = pRect->left - index;

						// 오른쪽 끝을 넘어가는 경우..
						if (index+colorCount > pRect->right)
						{						
							memset((void*)pDestTemp, 0, pRect->right - pRect->left);

							j = count;						
							break;
						}

						// 투명이 아닌 색들을 Surface에 출력한다.
						//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
						// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
						memset((void*)pDestTemp, 0, (colorCount-dist)<<1);

						pDestTemp += colorCount-dist;
						//pPixels += colorCount;		
						index += colorCount;

						// 이제부터는 계속 출력한다.					
						break;
					}
				}					

				// 투명이 아닌 색만큼 index증가				
				//pPixels += colorCount;
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
					if (index+colorCount > pRect->right)
					{
						// 투명색만으로 더 출력할 필요가 없을 때
						if (index > pRect->right)
						{
							break;
						}
						// 투명색 아닌 것을 조금 출력해야 할 경우
						else
						{
							pDestTemp += transCount;
						
							// 투명이 아닌 색들을 Surface에 출력한다.
							//memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
							// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
							memset((void*)pDestTemp, 0, (pRect->right - index)<<1);

							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					// 출력
					//memcpy(pDestTemp, pPixels, colorCount<<1);
					// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
					memset((void*)pDestTemp, 0, colorCount<<1);

					pDestTemp += colorCount;
					//pPixels += colorCount;			
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
CShadowSprite::BltClipHeight(WORD *pDest, WORD pitch, RECT* pRect)
{
	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int i;
	register int j;

	for (i=pRect->top; i<pRect->bottom; i++)
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
				//memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
				// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
				memset((void*)pDestTemp, 0, colorCount<<1);
				
				pDestTemp	+= colorCount;
				//pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}


//----------------------------------------------------------------------
// BltDarkness
//----------------------------------------------------------------------
// Clipping하지 않는다.
//----------------------------------------------------------------------
void
CShadowSprite::BltDarkness(WORD *pDest, WORD pitch, BYTE DarkBits)
{
	s_Value1 = DarkBits;

	int	count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;

	for (i=0; i<m_Height; i++)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		// (투명수,색깔수)의 반복 수		
		count	= *pPixels++;		
 		// 한 줄 출력
		if (count > 0)
		{			
			j = count;
			do
			{						
				pDestTemp += *pPixels++;			// 투명색만큼 건너 뛴다.
				colorCount = *pPixels++;		// 투명 아닌 색 수				

				// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
				//memset((void*)pDestTemp, 0, colorCount<<1);
				memcpyShadowDarkness(pDestTemp, colorCount);
		
				pDestTemp	+= colorCount;
				//pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltDarkness ClipLeft
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CShadowSprite::BltDarknessClipLeft(WORD* pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	s_Value1 = DarkBits;

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
	for (i=pRect->top; i<pRect->bottom; i++)
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
				if (index+colorCount > pRect->left)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > pRect->left)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - pRect->left;

						// 이번 단계는 모두 출력
						//memcpy(pDestTemp, pPixels, colorCount<<1);
						// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
						//memset((void*)pDestTemp, 0, colorCount<<1);
						memcpyShadowDarkness(pDestTemp, colorCount);

						pDestTemp += colorCount;
						//pPixels += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우
					//---------------------------------------------
					else
					{
						dist = pRect->left - index;

						// 투명이 아닌 색들을 Surface에 출력한다.
						//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
						// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
						//memset((void*)pDestTemp, 0, (colorCount-dist)<<1);
						memcpyShadowDarkness(pDestTemp, colorCount-dist);

						pDestTemp += colorCount-dist;
						//pPixels += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}					

				// 투명이 아닌 색만큼 index증가				
				//pPixels += colorCount;
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
					
					// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
					//memset((void*)pDestTemp, 0, colorCount<<1);
					memcpyShadowDarkness(pDestTemp, colorCount);

					// memory addr 증가
					pDestTemp += colorCount;
					//pPixels += colorCount;			
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
CShadowSprite::BltDarknessClipRight(WORD* pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	s_Value1 = DarkBits;

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

	for (i=pRect->top; i<pRect->bottom; i++)
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
				if (index+colorCount > pRect->right)
				{
					// 투명색만으로 더 출력할 필요가 없을 때
					if (index > pRect->right)
					{
						break;
					}
					// 투명색 아닌 것을 조금 출력해야 할 경우
					else
					{
						pDestTemp += transCount;
					
						// 투명이 아닌 색들을 Surface에 출력한다.
						//memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
						// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
						//memset((void*)pDestTemp, 0, (pRect->right - index)<<1);
						memcpyShadowDarkness(pDestTemp, pRect->right - index);

						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				// 출력
				//memcpy(pDestTemp, pPixels, colorCount<<1);
				// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
				//memset((void*)pDestTemp, 0, colorCount<<1);
				memcpyShadowDarkness(pDestTemp, colorCount);

				pDestTemp += colorCount;
				//pPixels += colorCount;			
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
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력하다가
// pRect->Right까지만 출력한다.
//----------------------------------------------------------------------
void
CShadowSprite::BltDarknessClipWidth(WORD* pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	s_Value1 = DarkBits;

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
	for (i=pRect->top; i<pRect->bottom; i++)
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
				if (index+colorCount > pRect->left)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > pRect->left)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - pRect->left;

						// 이번 단계는 모두 출력?
						// 오른쪽 끝을 넘어가는 경우..
						if (index+colorCount > pRect->right)
						{							
							// 투명색만으로 오른쪽 끝 넘어가는 경우
							if (index > pRect->right)
							{
							}
							else
							{
								//memset((void*)pDestTemp, 0, (pRect->right - index)<<1);
								memcpyShadowDarkness(pDestTemp, pRect->right - index);
							}

							j = count;
							
							break;
						}

						// 이번 단계는 모두 출력
						//memcpy(pDestTemp, pPixels, colorCount<<1);
						// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
						//memset((void*)pDestTemp, 0, colorCount<<1);
						memcpyShadowDarkness(pDestTemp, colorCount);

						pDestTemp += colorCount;
						//pPixels += colorCount;
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
						dist = pRect->left - index;

						// 오른쪽 끝을 넘어가는 경우..
						if (index+colorCount > pRect->right)
						{
							memcpyShadowDarkness(pDestTemp, pRect->right - pRect->left);

							j = count;						
							break;
						}

						// 투명이 아닌 색들을 Surface에 출력한다.
						//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
						// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
						//memset((void*)pDestTemp, 0, (colorCount-dist)<<1);
						memcpyShadowDarkness(pDestTemp, colorCount-dist);

						pDestTemp += colorCount-dist;
						//pPixels += colorCount;		
						index += colorCount;

						// 이제부터는 계속 출력한다.					
						break;
					}
				}					

				// 투명이 아닌 색만큼 index증가				
				//pPixels += colorCount;
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
					if (index+colorCount > pRect->right)
					{
						// 투명색만으로 더 출력할 필요가 없을 때
						if (index > pRect->right)
						{
							break;
						}
						// 투명색 아닌 것을 조금 출력해야 할 경우
						else
						{
							pDestTemp += transCount;
						
							// 투명이 아닌 색들을 Surface에 출력한다.
							//memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
							// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
							//memset((void*)pDestTemp, 0, (pRect->right - index)<<1);
							memcpyShadowDarkness(pDestTemp, pRect->right-index);

							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					// 출력
					//memcpy(pDestTemp, pPixels, colorCount<<1);
					// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
					//memset((void*)pDestTemp, 0, colorCount<<1);
					memcpyShadowDarkness(pDestTemp, colorCount);

					pDestTemp += colorCount;
					//pPixels += colorCount;			
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
CShadowSprite::BltDarknessClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	s_Value1 = DarkBits;

	int	count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int i;
	register int j;

	for (i=pRect->top; i<pRect->bottom; i++)
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
				//memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
				// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
				//memset((void*)pDestTemp, 0, colorCount<<1);
				memcpyShadowDarkness(pDestTemp, colorCount);
				
				pDestTemp	+= colorCount;
				//pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// ShadowDarkness Copy
//----------------------------------------------------------------------
// dest의 pixels개를 s_Value1만큼 어둡게 출력을 한다.
//----------------------------------------------------------------------
void	
CShadowSprite::memcpyShadowDarkness(WORD* pDest, WORD pixels)
{
	QWORD	*qpDest		= (QWORD*)pDest;

	register int j;

	BYTE qTimes = pixels >> 2;	// pixels / 4

	// 반투명
	switch ( pixels & 0x03 )	// pixels % 4
	{
		//------------------
		// 4점씩
		//------------------
		case 0 :			
			// 네점씩 찍기
			for (j=0; j<qTimes; j++)
			{
				*qpDest = ((*qpDest >> s_Value1) & CDirectDraw::s_qwMASK_SHIFT[s_Value1]);				

				*qpDest++;				
			}
		break;

		//------------------
		// 1점 + 4점씩
		//------------------
		case 1 :
			// 한점 찍기
			*(WORD*)qpDest = ((*(WORD*)qpDest >> s_Value1) & CDirectDraw::s_wMASK_SHIFT[s_Value1]);
			
				
			qpDest = (QWORD*)((WORD*)qpDest + 1);			

			// 네점씩 찍기
			for (j=0; j<qTimes; j++)
			{
				*qpDest = ((*qpDest >> s_Value1) & CDirectDraw::s_qwMASK_SHIFT[s_Value1]);

				*qpDest++;				
			}
			
		break;

		//------------------
		// 2점 + 4점씩
		//------------------
		case 2 :
			// 두점 찍기
			*(DWORD*)qpDest = ((*(DWORD*)qpDest >> s_Value1) & CDirectDraw::s_dwMASK_SHIFT[s_Value1]);			
				
			qpDest = (QWORD*)((DWORD*)qpDest + 1);			

			// 네점씩 찍기
			for (j=0; j<qTimes; j++)
			{
				*qpDest = ((*qpDest >> s_Value1) & CDirectDraw::s_qwMASK_SHIFT[s_Value1]);

				*qpDest++;				
			}
		break;

		//------------------
		// 1점 + 2점 + 4점씩
		//------------------
		case 3 :
			// 한점 찍기
			*(WORD*)qpDest = ((*(WORD*)qpDest >> s_Value1) & CDirectDraw::s_wMASK_SHIFT[s_Value1]);	

			// 두점 찍기
			*(DWORD*)qpDest = ((*(DWORD*)qpDest >> s_Value1) & CDirectDraw::s_dwMASK_SHIFT[s_Value1]);
				
			qpDest = (QWORD*)((DWORD*)qpDest + 1);			

			// 네점씩 찍기
			for (j=0; j<qTimes; j++)
			{
				*qpDest = ((*qpDest >> s_Value1) & CDirectDraw::s_qwMASK_SHIFT[s_Value1]);

				*qpDest++;				
			}
		break;
	}
}

//----------------------------------------------------------------------
// Blt4444
//----------------------------------------------------------------------
// Clipping하지 않는다.
//----------------------------------------------------------------------
void
CShadowSprite::Blt4444(WORD *pDest, WORD pitch, WORD pixel)
{
	s_wValue1 = pixel;

	int	count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;

	for (register int i=0; i<m_Height; i++)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		// (투명수,색깔수)의 반복 수		
		count	= *pPixels++;		
 		// 한 줄 출력
		for (register int j=0; j<count; j++)
		{				
			pDestTemp += *pPixels++;			// 투명색만큼 건너 뛴다.
			colorCount = *pPixels++;		// 투명 아닌 색 수				

			// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
			//memset((void*)pDestTemp, 0, colorCount<<1);
			memcpyShadow4444(pDestTemp, colorCount);
	
			pDestTemp	+= colorCount;
			//pPixels		+= colorCount;
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt4444 ClipLeft
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CShadowSprite::Blt4444ClipLeft(WORD* pDest, WORD pitch, RECT* pRect, WORD pixel)
{
	s_wValue1 = pixel;

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
	for (i=pRect->top; i<pRect->bottom; i++)
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
		for (j=0; j<count; j++)
		{
			transCount = *pPixels++;		// 투명색 수			
			colorCount = *pPixels++;		// 투명 아닌 색 수			
					
			// 투명색만큼 index증가			
			index += transCount;
			
		
			//---------------------------------------------
			// xxxx범위를 넘어가게 되는 경우
			//---------------------------------------------
			if (index+colorCount > pRect->left)
			{
				//---------------------------------------------
				// 투명색만으로 xxxx범위를 넘어갔을 경우
				//---------------------------------------------
				if (index > pRect->left)
				{	
					// 투명색부분 건너띔
					pDestTemp += index - pRect->left;

					// 이번 단계는 모두 출력
					//memcpy(pDestTemp, pPixels, colorCount<<1);
					// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
					//memset((void*)pDestTemp, 0, colorCount<<1);
					memcpyShadow4444(pDestTemp, colorCount);

					pDestTemp += colorCount;
					//pPixels += colorCount;

					// 이제부터는 계속 출력한다.
					break;
				}
				//---------------------------------------------
				// 투명색+투명아닌색의 일부까지 출력하면 
				// xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				else
				{
					dist = pRect->left - index;

					// 투명이 아닌 색들을 Surface에 출력한다.
					//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
					// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
					//memset((void*)pDestTemp, 0, (colorCount-dist)<<1);
					memcpyShadow4444(pDestTemp, colorCount-dist);

					pDestTemp += colorCount-dist;
					//pPixels += colorCount;

					// 이제부터는 계속 출력한다.
					break;
				}
			}					

			// 투명이 아닌 색만큼 index증가				
			//pPixels += colorCount;
			index += colorCount;
		}

		//---------------------------------------------
		// 이제부터는 계속 출력한다.		
		//---------------------------------------------		
		for (j++; j<count; j++)
		{
			transCount = *pPixels++;		// 투명색 수			
			colorCount = *pPixels++;		// 투명 아닌 색 수			
					
			// 투명색만큼 건너 뛴다.
			pDestTemp += transCount;			
			
			// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
			//memset((void*)pDestTemp, 0, colorCount<<1);
			memcpyShadow4444(pDestTemp, colorCount);

			// memory addr 증가
			pDestTemp += colorCount;
			//pPixels += colorCount;			
		}
		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt4444 ClipRight
//----------------------------------------------------------------------
// 오른쪽 clipping.  
// pRect->right개 까지의 점만 pDest에 출력한다.
//----------------------------------------------------------------------
void
CShadowSprite::Blt4444ClipRight(WORD* pDest, WORD pitch, RECT* pRect, WORD pixel)
{
	s_wValue1 = pixel;

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

	for (i=pRect->top; i<pRect->bottom; i++)
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
		for (j=0; j<count; j++)
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
			if (index+colorCount > pRect->right)
			{
				// 투명색만으로 더 출력할 필요가 없을 때
				if (index > pRect->right)
				{
					break;
				}
				// 투명색 아닌 것을 조금 출력해야 할 경우
				else
				{
					pDestTemp += transCount;
				
					// 투명이 아닌 색들을 Surface에 출력한다.
					//memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
					// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
					//memset((void*)pDestTemp, 0, (pRect->right - index)<<1);
					memcpyShadow4444(pDestTemp, pRect->right - index);

					break;
				}
			}

			// 투명색만큼 건너띄고
			pDestTemp += transCount;

			// 출력
			//memcpy(pDestTemp, pPixels, colorCount<<1);
			// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
			//memset((void*)pDestTemp, 0, colorCount<<1);
			memcpyShadow4444(pDestTemp, colorCount);

			pDestTemp += colorCount;
			//pPixels += colorCount;			
			index += colorCount;
		}
		
		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt4444 ClipWidth
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력하다가
// pRect->Right까지만 출력한다.
//----------------------------------------------------------------------
void
CShadowSprite::Blt4444ClipWidth(WORD* pDest, WORD pitch, RECT* pRect, WORD pixel)
{
	s_wValue1 = pixel;

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
	for (i=pRect->top; i<pRect->bottom; i++)
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
		for (j=0; j<count; j++)
		{
			transCount = *pPixels++;		// 투명색 수			
			colorCount = *pPixels++;		// 투명 아닌 색 수			
					
			// 투명색만큼 index증가			
			index += transCount;
			
		
			//---------------------------------------------
			// xxxx범위를 넘어가게 되는 경우
			//---------------------------------------------
			if (index+colorCount > pRect->left)
			{
				//---------------------------------------------
				// 투명색만으로 xxxx범위를 넘어갔을 경우
				//---------------------------------------------
				if (index > pRect->left)
				{	
					// 투명색부분 건너띔
					pDestTemp += index - pRect->left;

					// 이번 단계는 모두 출력?
					// 오른쪽 끝을 넘어가는 경우..
					if (index+colorCount > pRect->right)
					{							
						// 투명색만으로 오른쪽 끝 넘어가는 경우
						if (index > pRect->right)
						{
						}
						else
						{
							//memset((void*)pDestTemp, 0, (pRect->right - index)<<1);
							memcpyShadow4444(pDestTemp, pRect->right - index);
						}

						j = count;
						
						break;
					}

					// 이번 단계는 모두 출력
					//memcpy(pDestTemp, pPixels, colorCount<<1);
					// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
					//memset((void*)pDestTemp, 0, colorCount<<1);
					memcpyShadow4444(pDestTemp, colorCount);

					pDestTemp += colorCount;
					//pPixels += colorCount;
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
					dist = pRect->left - index;

					// 오른쪽 끝을 넘어가는 경우..
					if (index+colorCount > pRect->right)
					{
						memcpyShadow4444(pDestTemp, pRect->right - pRect->left);

						j = count;						
						break;
					}

					// 투명이 아닌 색들을 Surface에 출력한다.
					//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
					// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
					//memset((void*)pDestTemp, 0, (colorCount-dist)<<1);
					memcpyShadow4444(pDestTemp, colorCount-dist);

					pDestTemp += colorCount-dist;
					//pPixels += colorCount;		
					index += colorCount;

					// 이제부터는 계속 출력한다.					
					break;
				}
			}					

			// 투명이 아닌 색만큼 index증가				
			//pPixels += colorCount;
			index += colorCount;
		}

		//---------------------------------------------
		// 각 줄마다 Clipping을 해줘야 하는데...		
		// OOOOOOOOOOOOOOxxxxx 이런 경우이다.
		//---------------------------------------------
		// OOOOOOOOOOOOOO까지만 출력해주면 된다.
		//---------------------------------------------
		for (j++; j<count; j++)
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
			if (index+colorCount > pRect->right)
			{
				// 투명색만으로 더 출력할 필요가 없을 때
				if (index > pRect->right)
				{
					break;
				}
				// 투명색 아닌 것을 조금 출력해야 할 경우
				else
				{
					pDestTemp += transCount;
				
					// 투명이 아닌 색들을 Surface에 출력한다.
					//memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
					// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
					//memset((void*)pDestTemp, 0, (pRect->right - index)<<1);
					memcpyShadow4444(pDestTemp, pRect->right-index);

					break;
				}
			}

			// 투명색만큼 건너띄고
			pDestTemp += transCount;

			// 출력
			//memcpy(pDestTemp, pPixels, colorCount<<1);
			// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
			//memset((void*)pDestTemp, 0, colorCount<<1);
			memcpyShadow4444(pDestTemp, colorCount);

			pDestTemp += colorCount;
			//pPixels += colorCount;			
			index += colorCount;
		}
		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt4444 Clip Height
//----------------------------------------------------------------------
// pRect->top, pRect->bottom만큼만 출력한다.
//----------------------------------------------------------------------
void
CShadowSprite::Blt4444ClipHeight(WORD *pDest, WORD pitch, RECT* pRect, WORD pixel)
{
	s_wValue1 = pixel;

	int	count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int i;
	register int j;

	for (i=pRect->top; i<pRect->bottom; i++)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		// (투명수,색깔수,색깔들)의 반복 수		
		count	= *pPixels++;		

		// 한 줄 출력
		for (j=0; j<count; j++)
		{				
			pDestTemp += *pPixels++;		// 투명색만큼 건너 뛴다.
			colorCount = *pPixels++;		// 투명 아닌 색 수				

			// 투명이 아닌 색들을 Surface에 출력한다.
			//memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
			// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
			//memset((void*)pDestTemp, 0, colorCount<<1);
			memcpyShadow4444(pDestTemp, colorCount);
			
			pDestTemp	+= colorCount;
			//pPixels		+= colorCount;
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// ShadowDarkness Copy
//----------------------------------------------------------------------
// dest의 pixels개를 s_wValue1로 한다.
//----------------------------------------------------------------------
void	
CShadowSprite::memcpyShadow4444(WORD* pDest, WORD pixels)
{
	register int i = pixels;

	if (i>0)
	{
		do {	
			*pDest = s_wValue1;
			pDest++;
		} while (--i);
	}
}


//----------------------------------------------------------------------
// Blt Small
//----------------------------------------------------------------------
// 2^shift byte수당 한 점씩만 출력한다.
//
// 100*100 Sprite일 경우 
// shift = 0 , 100*100
// shift = 1 , 50*50
// shift = 2 , 25*25
// shift = 3 , 12*12
// shift = 4 , 6*6
// ...
// Clipping하지 않는다.
//----------------------------------------------------------------------
// 길이 보정해야되는데...
// clipping 함수들에서는 안한다. - -; 귀찮아서.. 음하하.
//----------------------------------------------------------------------
void
CShadowSprite::BltSmall(WORD *pDest, WORD pitch, BYTE shift)
{
	int		count,		
			transCount,
			transCountShift,
			colorCount,
			colorCountShift,
			totalCount,
			totalShiftCount,
			pixelGap,
			pixelGapShift;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;

	if (m_Height > 0)
	{
		i = m_Height-1;
		int stepY = 1 << shift;		// y줄 건너띄는 pixel수
		pDest = (WORD*)((BYTE*)pDest + (i>>shift)*pitch);

		do {	
			pPixels		= m_Pixels[i];
			pDestTemp	= pDest;

			// (투명수,색깔수)의 반복 수		
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
					colorCount = *pPixels++;				// 투명 아닌 색 수
					
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
					// 투명색만큼 건너 뛴다.
					//--------------------------------------------------
					pDestTemp += transCountShift;		

					
					//--------------------------------------------------
					//				그림자 부분 길이 보정
					//--------------------------------------------------
					totalCount += colorCount;
					totalShiftCount += colorCountShift;
					
					//--------------------------------------------------
					// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
					//--------------------------------------------------
					memset((void*)pDestTemp, 0, colorCount);
			
					pDestTemp	+= colorCountShift;
					//pPixels		+= colorCount;
				} while (--j);
			}

			pDest = (WORD*)((BYTE*)pDest - pitch);
			
			i-=stepY;

		} while (i>0);
	}
}

//----------------------------------------------------------------------
// BltSmall ClipLeft
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CShadowSprite::BltSmallClipLeft(WORD* pDest, WORD pitch, RECT* pRect, BYTE shift)
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
	int stepY = 1 << shift;		// y줄 건너띄는 pixel수
	int endY = pRect->bottom << shift;
	for (i=pRect->top; i<endY; i+=stepY)
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
				if (index+colorCount > pRect->left)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > pRect->left)
					{	
						// 투명색부분 건너띔
						pDestTemp += (index - pRect->left) >> shift;

						// shift만큼 줄여버린다.
						colorCount >>= shift;

						// 이번 단계는 모두 출력
						//memcpy(pDestTemp, pPixels, colorCount<<1);
						// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
						memset((void*)pDestTemp, 0, colorCount<<1);

						pDestTemp += colorCount;
						//pPixels += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우
					//---------------------------------------------
					else
					{
						dist = pRect->left - index;

						// shift만큼 줄여버린다.
						colorCount = (colorCount-dist) >> shift;

						// 투명이 아닌 색들을 Surface에 출력한다.
						//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
						// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
						memset((void*)pDestTemp, 0, colorCount<<1);

						pDestTemp += colorCount;
						//pPixels += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}					

				// 투명이 아닌 색만큼 index증가				
				//pPixels += colorCount;
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
					pDestTemp += transCount>>shift;			

					// shift만큼 줄여버린다.
					colorCount >>= shift;
					
					// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
					memset((void*)pDestTemp, 0, colorCount<<1);

					// memory addr 증가
					pDestTemp += colorCount;
					//pPixels += colorCount;			
				} while (--j);
			}
		}
		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltSmall ClipRight
//----------------------------------------------------------------------
// 오른쪽 clipping.  
// pRect->right개 까지의 점만 pDest에 출력한다.
//----------------------------------------------------------------------
void
CShadowSprite::BltSmallClipRight(WORD* pDest, WORD pitch, RECT* pRect, BYTE shift)
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

	int stepY = 1 << shift;		// y줄 건너띄는 pixel수
	int endY = pRect->bottom << shift;
	for (i=pRect->top; i<endY; i+=stepY)
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
				transCount = *pPixels++ >> shift;		// 투명색 수			
				colorCount = *pPixels++ >> shift;		// 투명 아닌 색 수		
						
				// 투명색만큼 index증가
				index += transCount;
				
				// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
				// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

				// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

				//---------------------------------------------
				// 오른쪽 끝까지 도달했을 경우
				//---------------------------------------------			
				if (index+colorCount > pRect->right)
				{
					// 투명색만으로 더 출력할 필요가 없을 때
					if (index > pRect->right)
					{
						break;
					}
					// 투명색 아닌 것을 조금 출력해야 할 경우
					else
					{
						pDestTemp += transCount;
					
						// 투명이 아닌 색들을 Surface에 출력한다.
						//memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
						// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
						memset((void*)pDestTemp, 0, (pRect->right - index)<<1);

						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				index += colorCount;
				
				// 출력
				//memcpy(pDestTemp, pPixels, colorCount<<1);
				// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
				memset((void*)pDestTemp, 0, colorCount<<1);

				pDestTemp += colorCount;
				//pPixels += colorCount;							
			} while (--j);
		}
		
		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltSmall ClipWidth
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력하다가
// pRect->Right까지만 출력한다.
//----------------------------------------------------------------------
void
CShadowSprite::BltSmallClipWidth(WORD* pDest, WORD pitch, RECT* pRect, BYTE shift)
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

	register short i;
	register short j;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	int stepY = 1 << shift;		// y줄 건너띄는 pixel수
	int endY = pRect->bottom << shift;
	for (i=pRect->top; i<endY; i+=stepY)
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
				if (index+colorCount > pRect->left)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > pRect->left)
					{	
						// 투명색부분 건너띔
						pDestTemp += (index - pRect->left)>>shift;

						// 이번 단계는 모두 출력?
						// 오른쪽 끝을 넘어가는 경우..
						if (index+colorCount > pRect->right)
						{							
							// 투명색만으로 오른쪽 끝 넘어가는 경우
							if (index > pRect->right)
							{
							}
							else
							{
								memset((void*)pDestTemp, 0, ((pRect->right - index)>>shift)<<1);
							}

							j = count;
							
							break;
						}

						// shift만큼 줄여버린다.
						colorCount >>= shift;
					
						memset((void*)pDestTemp, 0, colorCount<<1);

						pDestTemp += colorCount;
						//pPixels += colorCount;
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
						dist = pRect->left - index;

						// 오른쪽 끝을 넘어가는 경우..
						if (index+colorCount > pRect->right)
						{						
							memset((void*)pDestTemp, 0, (pRect->right - pRect->left)>>shift);

							j = count;						
							break;
						}

						// shift만큼 줄여버린다.
						index += colorCount;
						colorCount = (colorCount-dist) >> shift;

						// 투명이 아닌 색들을 Surface에 출력한다.
						//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
						// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
						memset((void*)pDestTemp, 0, colorCount<<1);

						pDestTemp += colorCount;
						//pPixels += colorCount;		
						

						// 이제부터는 계속 출력한다.					
						break;
					}
				}					

				// 투명이 아닌 색만큼 index증가				
				//pPixels += colorCount;
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
					if (index+colorCount > pRect->right)
					{
						// 투명색만으로 더 출력할 필요가 없을 때
						if (index > pRect->right)
						{
							break;
						}
						// 투명색 아닌 것을 조금 출력해야 할 경우
						else
						{
							pDestTemp += transCount>>shift;
						
							// 투명이 아닌 색들을 Surface에 출력한다.
							//memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
							// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
							memset((void*)pDestTemp, 0, ((pRect->right - index)>>shift)<<1);

							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount>>shift;


					index += colorCount;
					// shift만큼 줄여준다.
					colorCount >>= shift;

					// 출력
					//memcpy(pDestTemp, pPixels, colorCount<<1);
					// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
					memset((void*)pDestTemp, 0, colorCount<<1);

					pDestTemp += colorCount;
					//pPixels += colorCount;								
				} while (--j);
			}
		}
		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltSmall Clip Height
//----------------------------------------------------------------------
// pRect->top, pRect->bottom만큼만 출력한다.
//----------------------------------------------------------------------
void
CShadowSprite::BltSmallClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE shift)
{
	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int i;
	register int j;
	
	int stepY = 1 << shift;		// y줄 건너띄는 pixel수
	int endY = pRect->bottom << shift;
	for (i=pRect->top; i<endY; i+=stepY)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		// (투명수,색깔수,색깔들)의 반복 수		
		count	= *pPixels++;		

		if (count > 0)
		{			
			j = count;
			do
			{
				pDestTemp += ((*pPixels++)>>shift);		// 투명색만큼 건너 뛴다.
				colorCount = *pPixels++;				// 투명 아닌 색 수
				
				colorCount >>= shift;

				// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
				memset((void*)pDestTemp, 0, colorCount<<1);
		
				pDestTemp	+= colorCount;
				//pPixels		+= colorCount;
			} while (--j);
		}
		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt Small4444
//----------------------------------------------------------------------
// 2^shift byte수당 한 점씩만 출력한다.
//
// 100*100 Sprite일 경우 
// shift = 0 , 100*100
// shift = 1 , 50*50
// shift = 2 , 25*25
// shift = 3 , 12*12
// shift = 4 , 6*6
// ...
// Clipping하지 않는다.
//----------------------------------------------------------------------
// 길이 보정해야되는데...
// clipping 함수들에서는 안한다. - -; 귀찮아서.. 음하하.
//----------------------------------------------------------------------
void
CShadowSprite::BltSmall4444(WORD *pDest, WORD pitch, WORD pixel, BYTE shift)
{
	// Shadow색깔 설정
	s_wValue1 = pixel;

	int		count,		
			transCount,
			transCountShift,
			colorCount,
			colorCountShift,
			totalCount,
			totalShiftCount,
			pixelGap,
			pixelGapShift;


	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;

	/*
	int stepY = 1 << shift;		// y줄 건너띄는 pixel수		
	int m_Height_1 = m_Height - 1;

	if (m_Height > 0)
	{
		i = m_Height_1;
		pDest = (WORD*)((BYTE*)pDest + (i>>shift)*pitch);

		do {	
			pPixels		= m_Pixels[i];
			pDestTemp	= pDest;

			// (투명수,색깔수)의 반복 수		
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
					colorCount = *pPixels++;				// 투명 아닌 색 수
					
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
					// 투명색만큼 건너 뛴다.
					//--------------------------------------------------
					pDestTemp += transCountShift;		

					
					//--------------------------------------------------
					//				그림자 부분 길이 보정
					//--------------------------------------------------
					totalCount += colorCount;
					totalShiftCount += colorCountShift;
					
					//--------------------------------------------------
					// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
					//--------------------------------------------------
					memcpyShadow4444(pDestTemp, colorCountShift);
			
					pDestTemp	+= colorCountShift;
					//pPixels		+= colorCount;
				} while (--j);
			}

			pDest = (WORD*)((BYTE*)pDest - pitch);
			
			i-=stepY;

		} while (i>0);
	}
	*/

	int stepY = 1 << shift;		// y줄 건너띄는 pixel수		
	
	for (i=0; i<m_Height; i+=stepY)
	{
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		// (투명수,색깔수)의 반복 수		
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
				colorCount = *pPixels++;				// 투명 아닌 색 수
				
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
				// 투명색만큼 건너 뛴다.
				//--------------------------------------------------
				pDestTemp += transCountShift;		
			
				//--------------------------------------------------
				//				그림자 부분 길이 보정
				//--------------------------------------------------
				totalCount += colorCount;
				totalShiftCount += colorCountShift;
				
				//--------------------------------------------------
				// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
				//--------------------------------------------------
				memcpyShadow4444(pDestTemp, colorCountShift);

				pDestTemp	+= colorCountShift;

				//pPixels		+= colorCount;
			} while (--j);

		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltSmall4444 ClipLeft
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CShadowSprite::BltSmall4444ClipLeft(WORD* pDest, WORD pitch, RECT* pRect, WORD pixel, BYTE shift)
{
	// Shadow색깔 설정
	s_wValue1 = pixel;

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
	int stepY = 1 << shift;		// y줄 건너띄는 pixel수
	int endY = pRect->bottom << shift;
	for (i=pRect->top; i<endY; i+=stepY)
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
				if (index+colorCount > pRect->left)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > pRect->left)
					{	
						// 투명색부분 건너띔
						pDestTemp += (index - pRect->left) >> shift;

						// shift만큼 줄여버린다.
						colorCount >>= shift;

						// 이번 단계는 모두 출력
						//memcpy(pDestTemp, pPixels, colorCount<<1);
						// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
						memcpyShadow4444(pDestTemp, colorCount);

						pDestTemp += colorCount;
						//pPixels += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우
					//---------------------------------------------
					else
					{
						dist = pRect->left - index;

						// shift만큼 줄여버린다.
						colorCount = (colorCount-dist) >> shift;

						// 투명이 아닌 색들을 Surface에 출력한다.
						//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
						// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
						memcpyShadow4444(pDestTemp, colorCount);

						pDestTemp += colorCount;
						//pPixels += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}					

				// 투명이 아닌 색만큼 index증가				
				//pPixels += colorCount;
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
					pDestTemp += transCount>>shift;			

					// shift만큼 줄여버린다.
					colorCount >>= shift;
					
					// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
					memcpyShadow4444(pDestTemp, colorCount);

					// memory addr 증가
					pDestTemp += colorCount;
					//pPixels += colorCount;			
				} while (--j);
			}
		}
		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltSmall4444 ClipRight
//----------------------------------------------------------------------
// 오른쪽 clipping.  
// pRect->right개 까지의 점만 pDest에 출력한다.
//----------------------------------------------------------------------
void
CShadowSprite::BltSmall4444ClipRight(WORD* pDest, WORD pitch, RECT* pRect, WORD pixel, BYTE shift)
{
	// Shadow색깔 설정
	s_wValue1 = pixel;

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

	int stepY = 1 << shift;		// y줄 건너띄는 pixel수
	int endY = pRect->bottom << shift;
	for (i=pRect->top; i<endY; i+=stepY)
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
				transCount = *pPixels++ >> shift;		// 투명색 수			
				colorCount = *pPixels++ >> shift;		// 투명 아닌 색 수		
						
				// 투명색만큼 index증가
				index += transCount;
				
				// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
				// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

				// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

				//---------------------------------------------
				// 오른쪽 끝까지 도달했을 경우
				//---------------------------------------------			
				if (index+colorCount > pRect->right)
				{
					// 투명색만으로 더 출력할 필요가 없을 때
					if (index > pRect->right)
					{
						break;
					}
					// 투명색 아닌 것을 조금 출력해야 할 경우
					else
					{
						pDestTemp += transCount;
					
						// 투명이 아닌 색들을 Surface에 출력한다.
						//memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
						// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
						memcpyShadow4444(pDestTemp, (pRect->right - index));

						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				index += colorCount;
				
				// 출력
				//memcpy(pDestTemp, pPixels, colorCount<<1);
				// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
				memcpyShadow4444(pDestTemp, colorCount);

				pDestTemp += colorCount;
				//pPixels += colorCount;							
			} while (--j);
		}
		
		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltSmall4444 ClipWidth
//----------------------------------------------------------------------
// 왼쪽 clipping.  
// pRect->left개의 점을 건너띈 다음부터 pDest에 출력하다가
// pRect->Right까지만 출력한다.
//----------------------------------------------------------------------
void
CShadowSprite::BltSmall4444ClipWidth(WORD* pDest, WORD pitch, RECT* pRect, WORD pixel, BYTE shift)
{
	// Shadow색깔 설정
	s_wValue1 = pixel;

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

	register short i;
	register short j;

	//---------------------------------------------
	// 출력해야하는 모든 줄에 대해서..
	//---------------------------------------------
	int stepY = 1 << shift;		// y줄 건너띄는 pixel수
	int endY = pRect->bottom << shift;
	for (i=pRect->top; i<endY; i+=stepY)
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
				if (index+colorCount > pRect->left)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > pRect->left)
					{	
						// 투명색부분 건너띔
						pDestTemp += (index - pRect->left)>>shift;

						// 이번 단계는 모두 출력?
						// 오른쪽 끝을 넘어가는 경우..
						if (index+colorCount > pRect->right)
						{							
							// 투명색만으로 오른쪽 끝 넘어가는 경우
							if (index > pRect->right)
							{
							}
							else
							{
								memcpyShadow4444(pDestTemp, (pRect->right - index)>>shift);
							}

							j = count;
							
							break;
						}

						// shift만큼 줄여버린다.
						colorCount >>= shift;
					
						memcpyShadow4444(pDestTemp, colorCount);

						pDestTemp += colorCount;
						//pPixels += colorCount;
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
						dist = pRect->left - index;

						// 오른쪽 끝을 넘어가는 경우..
						if (index+colorCount > pRect->right)
						{						
							memcpyShadow4444(pDestTemp, (pRect->right - pRect->left)>>shift);

							j = count;						
							break;
						}

						// shift만큼 줄여버린다.
						index += colorCount;
						colorCount = (colorCount-dist) >> shift;

						// 투명이 아닌 색들을 Surface에 출력한다.
						//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
						// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
						memcpyShadow4444(pDestTemp, colorCount);

						pDestTemp += colorCount;
						//pPixels += colorCount;		
						

						// 이제부터는 계속 출력한다.					
						break;
					}
				}					

				// 투명이 아닌 색만큼 index증가				
				//pPixels += colorCount;
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
					if (index+colorCount > pRect->right)
					{
						// 투명색만으로 더 출력할 필요가 없을 때
						if (index > pRect->right)
						{
							break;
						}
						// 투명색 아닌 것을 조금 출력해야 할 경우
						else
						{
							pDestTemp += transCount>>shift;
						
							// 투명이 아닌 색들을 Surface에 출력한다.
							//memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
							// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
							memcpyShadow4444(pDestTemp, (pRect->right - index)>>shift);

							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount>>shift;


					index += colorCount;
					// shift만큼 줄여준다.
					colorCount >>= shift;

					// 출력
					//memcpy(pDestTemp, pPixels, colorCount<<1);
					// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
					memcpyShadow4444(pDestTemp, colorCount);

					pDestTemp += colorCount;
					//pPixels += colorCount;								
				} while (--j);
			}
		}
		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// BltSmall4444 Clip Height
//----------------------------------------------------------------------
// pRect->top, pRect->bottom만큼만 출력한다.
//----------------------------------------------------------------------
void
CShadowSprite::BltSmall4444ClipHeight(WORD *pDest, WORD pitch, RECT* pRect, WORD pixel, BYTE shift)
{
	// Shadow색깔 설정
	s_wValue1 = pixel;

	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int i;
	register int j;
	
	int stepY = 1 << shift;		// y줄 건너띄는 pixel수
	int endY = pRect->bottom << shift;
	for (i=pRect->top; i<endY; i+=stepY)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		// (투명수,색깔수,색깔들)의 반복 수		
		count	= *pPixels++;		

		if (count > 0)
		{			
			j = count;
			do
			{
				pDestTemp += ((*pPixels++)>>shift);		// 투명색만큼 건너 뛴다.
				colorCount = *pPixels++;				// 투명 아닌 색 수
				
				colorCount >>= shift;

				// 0번(검정색, 5:5:5나 5:6:5나 똑같다)색으로 출력
				memcpyShadow4444(pDestTemp, colorCount);
		
				pDestTemp	+= colorCount;
				//pPixels		+= colorCount;
			} while (--j);
		}
		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

