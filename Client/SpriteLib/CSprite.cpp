//----------------------------------------------------------------------
// CSprite.cpp
//----------------------------------------------------------------------
#include "client_PCH.h"
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

#ifdef SPRITELIB_BACKEND_SDL
	m_backend_sprite = SPRITECTL_INVALID_SPRITE;
	m_backend_dirty = false;
#endif
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
// s_Pixels�� memory�� ��´�.
//----------------------------------------------------------------------
void
CSprite::InitBuffer(WORD width, WORD height)
{
	if (width==0 || height==0)
		return;

	// memory ����
	ReleaseBuffer();

	s_BufferWidth	= width;
	s_BufferHeight	= height;

	// �� buffer[i]�� ���� ���� ����
	s_BufferLen = new WORD [s_BufferHeight];

	s_Buffer = new WORD* [s_BufferHeight];

	for (int i=0; i<s_BufferHeight; i++)
		s_Buffer[i] = new WORD [s_BufferWidth];
}

//----------------------------------------------------------------------
// s_Pixels�� memory�� �����Ѵ�.
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
// m_Pixels�� memory�� �����Ѵ�.
//----------------------------------------------------------------------
void	
CSprite::Release()
{
#ifdef SPRITELIB_BACKEND_SDL
	if (m_backend_sprite != SPRITECTL_INVALID_SPRITE) {
		spritectl_destroy_sprite(m_backend_sprite);
		m_backend_sprite = SPRITECTL_INVALID_SPRITE;
		m_backend_dirty = false;
	}
#endif
	for (int i=0; i<m_Height; i++)
		delete [] m_Pixels[i];
		//free(m_Pixels[i]);
	delete [] m_Pixels;
	//free(m_Pixels);

	//s_Delete++;

	m_Pixels	= NULL;
	m_Width		= 0;
	m_Height	= 0;

	m_bInit		= false;
}

//----------------------------------------------------------------------
// Assign Operator
//----------------------------------------------------------------------
void
CSprite::operator = (const CSprite& Sprite)
{
		// �޸� ����
	Release();


	// NULL�̸� �������� �ʴ´�.
	if (Sprite.m_Pixels==NULL || Sprite.m_Width==0 || Sprite.m_Height==0)
		return;

	// ũ�� ����
	m_Width = Sprite.m_Width;
	m_Height = Sprite.m_Height;
	
	// ���� �� �� ����
	int index;	
	register int i;
	register int j;

	// �޸� ���
	m_Pixels = new WORD* [m_Height];

	for (int i=0; i<m_Height; i++)
	{
		// �ݺ� ȸ���� 2 byte
		int	count = Sprite.m_Pixels[i][0], 
				colorCount;
		index	= 1;

		// �� line���� byte���� ��� �����ؾ��Ѵ�.
		for (j=0; j<count; j++)
		{
			//transCount = m_Pixels[i][index];
			colorCount = Sprite.m_Pixels[i][index+1];

			index+=2;	// �� count ��ŭ

			index += colorCount;	// ������ �ƴѰ͸�ŭ +				
		}

		// �޸� ���
		m_Pixels[i] = new WORD [index];
		memcpy(m_Pixels[i], Sprite.m_Pixels[i], index<<1);		
	}

	// ���� �Ϸ�
	m_bInit = true;
}

//----------------------------------------------------------------------
// Load From Buffer
//----------------------------------------------------------------------
// s_Buffer�� ����� Sprite�� *this�� copy��Ų��.
//----------------------------------------------------------------------
/*
void
CSprite::LoadFromBuffer()
{
	if (s_Width==0 || s_Height==0)
		return;

	// ���� Sprite�� memory����
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
// CDirectDrawSurface�� (x,y)+(width, height)������ �о m_Pixels�� �����Ѵ�.
//----------------------------------------------------------------------
// m_Pixels�� 0�� ���� Format���� �ٲ۴�.
//
// �� line���� ������ ���� ������ ������.
//
//    [�ݺ���] (������,�����,�����)(������,�����,�����)......
//
// �ݺ����� 2 bytes�̰�
// �������� ������� ���� 2 byte�̰�
// ������� ���� 2 bytes���̴�.
//
//----------------------------------------------------------------------
// Smart Cut �� �����ؾ� �Ѵ�.
//           :  �׸� ȭ�Ͽ��� Ư���� ������ Sprite�� ¥�� ��
//              ��� ������ �����ϸ� �� �������� �ܰ��κп���..
//              �ڵ����� ������ �κ��� �����ϰ� ������ �ִ� �κи��� 
//              �� �������� �����ϵ��� �ϴ� ��.
//----------------------------------------------------------------------
void
CSprite::SetPixel(WORD *pSource, WORD pitch, WORD width, WORD height)
{
	// memory����
	Release();

	m_Width = width;
	m_Height = height;

	// �ϴ� memory�� ������ ��Ƶд�.	
	WORD*	data = new WORD[m_Width*2+10];

	int		index,				// data�� index�� ���
			lastColorIndex;		// ������ �ƴѻ� ������ �ֱ� index
	int		count;				// �ݺ���
	int		trans,				// ������ ����
			color;				// ������ �ƴѻ� ����

	BOOL	bCheckTrans;		// �ֱٿ� �˻��Ѱ� �������ΰ�?

	WORD	*pSourceTemp;


	// height�� ��ŭ memory���
	m_Pixels = new WORD* [height];

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

		// �� line�� ���ؼ� ����~
		for (j=0; j<width; j++)
		{
			// 0�� color�� ���ؼ� ����
			if (*pSourceTemp==s_Colorkey)
			{
				// �ֱٿ� �˻��Ѱ� �������� �ƴϾ��ٸ�
				if (!bCheckTrans)
				{
					// ' (����,�����,�����) '�� �� set�� �������� �ǹ��ϹǷ�
					count++;
					
					data[lastColorIndex] = color;
					color = 0;

					bCheckTrans = TRUE;
				}
				
				trans++;				
			}
			else
			{
				// �ֱٿ� �˻��Ѱ� �������̾��ٸ�..
				if (bCheckTrans)
				{						
					data[index++] = trans;		// ���� byte�� �������� �ִ´�.
					trans = 0;

					lastColorIndex=index++;			// ������� ���� ��ġ�� ���					

					bCheckTrans = FALSE;
				}

				data[index++] = *pSourceTemp;	// ���� ������ �����Ѵ�.

				color++;								
			}

			pSourceTemp++;
		}
		
		// �� ���� ������ ���� �������ΰ�?
		if (bCheckTrans)
		{
			// �������̸� ���ٸ� ó���� �����൵ �ɰ� ����.
		}	
		// �������� �ƴ� ���, ���� ������ ���������� �Ѵ�.
		else
		{			
			count++;
			data[lastColorIndex] = color;
		}
		
		// memory�� �ٽ� ��´�.
		m_Pixels[i] = new WORD [index+1];

		// m_Pixels[i]�� ���������Ƿ� data�� ��ü�Ѵ�.
		// m_Pixels[i][0]���� count�� �־�� �Ѵ�.
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
// ������ ���� �����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::SetPixelNoColorkey(WORD *pSource, WORD pitch, WORD width, WORD height)
{
	// memory����
	Release();

	m_Width = width;
	m_Height = height;

	// �ϴ� memory�� ������ ��Ƶд�.	
	WORD*	data = new WORD[m_Width*2+10];

	int	index,				// data�� index�� ���
			lastColorIndex;		// ������ �ƴѻ� ������ �ֱ� index
	int	count;				// �ݺ���
	int	trans,				// ������ ����
			color;				// ������ �ƴѻ� ����

	BOOL	bCheckTrans;		// �ֱٿ� �˻��Ѱ� �������ΰ�?

	WORD	*pSourceTemp;


	// height�� ��ŭ memory���
	m_Pixels = new WORD* [height];

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

		// �� line�� ���ؼ� ����~
		for (j=0; j<width; j++)
		{
			// 0�� color�� ���ؼ� ����
			/*
			if (*pSourceTemp==s_Colorkey)
			{
				// �ֱٿ� �˻��Ѱ� �������� �ƴϾ��ٸ�
				if (!bCheckTrans)
				{
					// ' (����,�����,�����) '�� �� set�� �������� �ǹ��ϹǷ�
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
				// �ֱٿ� �˻��Ѱ� �������̾��ٸ�..
				if (bCheckTrans)
				{						
					data[index++] = trans;		// ���� byte�� �������� �ִ´�.
					trans = 0;

					lastColorIndex=index++;			// ������� ���� ��ġ�� ���					

					bCheckTrans = FALSE;
				}

				data[index++] = *pSourceTemp;	// ���� ������ �����Ѵ�.

				color++;								
			}

			pSourceTemp++;
		}
		
		// �� ���� ������ ���� �������ΰ�?
		if (bCheckTrans)
		{
			// �������̸� ���ٸ� ó���� �����൵ �ɰ� ����.
		}	
		// �������� �ƴ� ���, ���� ������ ���������� �Ѵ�.
		else
		{			
			count++;
			data[lastColorIndex] = color;
		}
		
		// memory�� �ٽ� ��´�.
		m_Pixels[i] = new WORD [index+1];

		// m_Pixels[i]�� ���������Ƿ� data�� ��ü�Ѵ�.
		// m_Pixels[i][0]���� count�� �־�� �Ѵ�.
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
// ������ surface�� ������ �κ��� �����ϱ� ���ؼ� ����Ѵ�....
//
// pSource�� width*height��ŭ�� ��������
// ������ �����ϴ� �κ��� �ִ� �簢 ������ ���ؼ�(������ ����)
// rect�� �Ѱ��ش�.
//----------------------------------------------------------------------
//
//	[ Example ]
//
//	x : ������,  O : ����
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
// ---> GetColorRect(...)�� �ϸ�
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
// [ �ܺο��� ] 
//
//	new������	= old������ + (rect.left, rect.top)
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
	// left ���ϱ�
	//-------------------------------------------------------
	pSourceTemp = pSource;
	for (j=0; j<width; j++)
	{
		pSourceTemp2 = pSourceTemp;

		for (int i=0; i<height; i++)
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
	// top ���ϱ�
	//-------------------------------------------------------
	pSourceTemp = pSource;
	for (int i=0; i<height; i++)
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
	// right ���ϱ�
	//-------------------------------------------------------
	pSourceTemp = (WORD*)((BYTE*)pSource + ((width-1)<<1));
	for (j=width-1; j>=0; j--)
	{
		pSourceTemp2 = pSourceTemp;

		for (int i=0; i<height; i++)
		{
			if (*pSourceTemp2!=colorkey)
			{
				rect.right = j + 1;	// right�� +1

				j = 0;
				break;
			}

			pSourceTemp2 = (WORD*)((BYTE*)pSourceTemp2 + pitch);
		}

		pSourceTemp--;
	}

	//-------------------------------------------------------
	// bottom ���ϱ�
	//-------------------------------------------------------
	pSourceTemp = (WORD*)((BYTE*)pSource + (height-1)*pitch);
	for (int i=height-1; i>=0; i--)
	{
		pSourceTemp2 = pSourceTemp;

		for (j=0; j<width; j++)
		{
			if (*pSourceTemp2!=colorkey)
			{
				rect.bottom = i + 1;	// bottom�� +1

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
// Sprite�ȿ��� (x,y)�� ������ �ִ°�?(�������� �ƴ� ���)
//----------------------------------------------------------------------
bool		
CSprite::IsColorPixel(short x, short y)
{
	// �ʱ�ȭ �� ���
	if (m_bInit)
	{

		// Sprite�� ������ ����� false
		if (x<0 || y<0 || x>=m_Width || y>=m_Height)
			return false;

		// y��° ��
		WORD	*pPixels = m_Pixels[y];

		// y��° ���� �ݺ� ��
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

				// �̹� loop�ȿ� �����ϴ� ��
				if (x < index+colorCount)
				{
					// �������������� ���� ���
					if (x < index)
					{
						return false;
					}

					// ���� ���Ѵ�.
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
// Sprite�ȿ��� (x,y)�� ������ ��´�.(�������� �ƴ� ���)
//----------------------------------------------------------------------
WORD		
CSprite::GetPixel(int x, int y) const
{
	// �ʱ�ȭ �� ���
	if (m_bInit)
	{
		// Sprite�� ������ ����� false
		if (x<0 || y<0 || x>=m_Width || y>=m_Height)
			return 0;

		// y��° ��
		WORD	*pPixels = m_Pixels[y];

		// y��° ���� �ݺ� ��
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

				// �̹� loop�ȿ� �����ϴ� ��
				if (x < index+colorCount)
				{
					// �������������� ���� ���
					if (x < index)
					{
						return 0;
					}

					// ���� ���Ѵ�.
					// ������ return
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
// �� Sprite�� ������ Filter�� ������ �޴��� check�Ѵ�.
//----------------------------------------------------------------------
bool
CSprite::IsIntersectFilter()
{
	// s_X, s_Y�� Sprite ���ο��� Filter�� ��µǴ� ������ġ�̴�.	
	if (IsNotInit() || s_pFilter->IsNotInit())
		return false;

	if (// ��� ������ġ(s_X,s_Y)�� Sprite�� �������ٴ� ���� ���
		s_X < m_Width && s_Y < m_Height
		// Filter�� ������ Sprite ù�� ���� ū ���
		 &&	s_X+s_pFilter->GetWidth() > 0 && s_Y+s_pFilter->GetHeight() > 0)
		{
			return true;
		}
	
	return false;
}

//----------------------------------------------------------------------
// BltClip
//----------------------------------------------------------------------
// pRect�� ������ ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltClip(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------	
	// ù �� (x,y)
	//--------------------------------------------
	pDest += pitch * pRect->top + pRect->left;
	//WORD width = ((pRect->right - pRect->left)<<1);

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
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
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���
		bPut = (pRect->left==0)? TRUE:FALSE;
		index = 0;
			
		// �� �ٸ��� Clipping�� ����� �ϴµ�...
		// xxxxxOOOOOOOOOOOOOO �̰ų�  (x:��¾���, O:�����)
		// OOOOOOOOOOOOOOxxxxx �̰�.. �� ���� ����.			
		if (count > 0)
		{
			j = count;
			do 
			{				
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ �ǳ� �ڴ�.
				//lpSurfaceTemp += transCount;
				index += transCount;

				// ����ص� �Ǵ� ��쿡�� ����Ѵ�.
				if (bPut)
				{
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���
					if (index > pRect->right)
						break;

					pDestTemp += transCount;

					// ������ �ƴ� ���� ���� ����ؾ� �� ���
					if (index+colorCount > pRect->right)
					{							
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
						break;
					}						

					// ��� ���
					memcpy(pDestTemp, pPixels, colorCount<<1);
					pDestTemp += colorCount;
				}				
				// ����ϸ� �� �� ���(���� ���ʺκ�)���� ����ص� �Ǵ��� Ȯ���غ���.
				else
				{
					// ������������ ������ �Ѿ���Ƿ� ��� ���
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

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);
						pDestTemp += colorCount-dist;

						// �������ʹ� ��� ����Ѵ�.
						bPut = TRUE;
					}
				}				

				// ������ �ƴ� ����ŭ index����				
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
// Clipping���� �ʴ´�.
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

			// (������,�����,�����)�� �ݺ� ��		
			count	= *pPixels++;		
 			// �� �� ���
			if (count > 0)
			{	
				j = count;
				do {
					pDestTemp += *pPixels++;			// ��������ŭ �ǳ� �ڴ�.
					colorCount = *pPixels++;		// ���� �ƴ� �� ��				

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltClipLeft(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
		
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// �̹� �ܰ�� ��� ���
						memcpy(pDestTemp, pPixels, colorCount<<1);
						pDestTemp += colorCount;
						pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
						pDestTemp += colorCount-dist;
						pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += colorCount;
				index += colorCount;
			} while (--j);

			//---------------------------------------------
			// �������ʹ� ��� ����Ѵ�.		
			//---------------------------------------------	
			if (--j > 0)
			{			
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ �ǳ� �ڴ�.
					pDestTemp += transCount;			
					
					// �������� �ƴѸ�ŭ ������ش�.
					memcpy(pDestTemp, pPixels, colorCount<<1);

					// memory addr ����
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
// ������ clipping.  
// pRect->right�� ������ ���� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltClipRight(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
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
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
			
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...		
		// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
		//---------------------------------------------
		// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
		//---------------------------------------------
		if (count > 0)		
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����
				index += transCount;
				
				// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
				// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

				// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

				//---------------------------------------------
				// ������ ������ �������� ���
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// ������������ �� ����� �ʿ䰡 ���� ��
					if (index > rectRight)
					{
						break;
					}
					// ������ �ƴ� ���� ���� ����ؾ� �� ���
					else
					{
						pDestTemp += transCount;
					
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						memcpy(pDestTemp, pPixels, (rectRight - index)<<1);
						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				// ���
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����ϴٰ�
// pRect->Right������ ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltClipWidth(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
		
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{		
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// �̹� �ܰ�� ��� ���?
						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > rectRight)
						{							
							// ������������ ������ �� �Ѿ�� ���
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

						// �̹� �ܰ�� ��� ���
						memcpy(pDestTemp, pPixels, colorCount<<1);
						pDestTemp += colorCount;
						pPixels += colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > rectRight)
						{
							memcpy(pDestTemp, pPixels+dist, (rectRight - rectLeft)<<1);

							j = 0;						
							break;
						}		

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
						pDestTemp += colorCount-dist;
						pPixels += colorCount;		
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.					
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += colorCount;
				index += colorCount;
			} while (--j);

			
			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...		
			// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
			//---------------------------------------------
			// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
			//---------------------------------------------
			if (--j > 0)
			{
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����
					index += transCount;
					
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

					//---------------------------------------------
					// ������ ������ �������� ���
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// ������������ �� ����� �ʿ䰡 ���� ��
						if (index > rectRight)
						{
							break;
						}
						// ������ �ƴ� ���� ���� ����ؾ� �� ���
						else
						{
							pDestTemp += transCount;
						
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							memcpy(pDestTemp, pPixels, (rectRight - index)<<1);
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					// ���
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
// pRect->top, pRect->bottom��ŭ�� ����Ѵ�.
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
	for (int i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		// (������,�����,�����)�� �ݺ� ��		
		count	= *pPixels++;		

		// �� �� ���
		if (count > 0)
		{
			j = count;
			do 
			{
				pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
				colorCount = *pPixels++;		// ���� �ƴ� �� ��				

				// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
			(((((*pSource >> ColorDraw::s_bSHIFT_R) & 0x1F) * darker) >> 5) << ColorDraw::s_bSHIFT_R)
			| (((((*pSource >> ColorDraw::s_bSHIFT_G) & 0x1F) * darker) >> 5) << ColorDraw::s_bSHIFT_G)
			| (((*pSource & 0x1F) * darker) >> 5);
			
		pDest++;
		pSource++;
		pFilter++;		
	}
}



//----------------------------------------------------------------------
// Alpha Copy
//----------------------------------------------------------------------
// source --> dest �� pixels��ŭ s_Value1������ ����� �Ѵ�.
//
// s_Value1 = 1~32
//
//----------------------------------------------------------------------
// 5:6:5 ���� code�̴�.
//----------------------------------------------------------------------
/*
void	
CSprite::memcpyAlphaFilter(WORD* pDest, WORD* pSource, WORD pixels)
{
	static WORD		sTemp,dTemp;
	static WORD		sr,sg,sb,dr,dg,db;

	register int j;

	// Alpha Channel Blending
	// ������ ���
	for (j=0; j<pixels; j++)
	{		
		//if (s_IndexX > 0 && s_IndexX < m_pFilter->GetWidth())
		{
			// ���� ���
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
// Clipping���� �ʴ´�.
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

			// (������,�����,�����)�� �ݺ� ��		
			count	= *pPixels++;		

			// �� �� ���
			if (count > 0)
			{
				j = count;
				do 
				{		
					pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
					colorCount = *pPixels++;		// ���� �ƴ� �� ��				

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltHalfClipLeft(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
		
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{		
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// �̹� �ܰ�� ��� ���
						CSpriteSurface::memcpyHalf(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyHalf(pDestTemp, pPixels+dist, colorCount-dist);
						pDestTemp += colorCount-dist;
						pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += colorCount;
				index += colorCount;
			} while (--j);
		
			//---------------------------------------------
			// �������ʹ� ��� ����Ѵ�.		
			//---------------------------------------------		
			if (--j > 0)
			{
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ �ǳ� �ڴ�.
					pDestTemp += transCount;			
					
					// �������� �ƴѸ�ŭ ������ش�.
					CSpriteSurface::memcpyHalf(pDestTemp, pPixels, colorCount);

					// memory addr ����
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
// ������ clipping.  
// pRect->right�� ������ ���� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltHalfClipRight(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
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
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
			
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...		
		// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
		//---------------------------------------------
		// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����
				index += transCount;
				
				// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
				// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

				// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

				//---------------------------------------------
				// ������ ������ �������� ���
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// ������������ �� ����� �ʿ䰡 ���� ��
					if (index > rectRight)
					{
						break;
					}
					// ������ �ƴ� ���� ���� ����ؾ� �� ���
					else
					{
						pDestTemp += transCount;
					
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyHalf(pDestTemp, pPixels, rectRight - index);
						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				// ���
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
// rectRight����..
//----------------------------------------------------------------------
void
CSprite::BltHalfClipWidth(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
		
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{		
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// �̹� �ܰ�� ��� ���?
						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > rectRight)
						{							
							// ������������ ������ �� �Ѿ�� ���
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

						// �̹� �ܰ�� ��� ���
						CSpriteSurface::memcpyHalf(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > rectRight)
						{
							CSpriteSurface::memcpyHalf(pDestTemp, pPixels+dist, rectRight - rectLeft);

							j = 0;						
							break;
						}					

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyHalf(pDestTemp, pPixels+dist, colorCount-dist);
						pDestTemp += colorCount-dist;
						pPixels += colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += colorCount;
				index += colorCount;
			} while (--j);
		

			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...		
			// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
			//---------------------------------------------
			// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
			//---------------------------------------------
			if (--j > 0)
			{
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����
					index += transCount;
					
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

					//---------------------------------------------
					// ������ ������ �������� ���
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// ������������ �� ����� �ʿ䰡 ���� ��
						if (index > rectRight)
						{
							break;
						}
						// ������ �ƴ� ���� ���� ����ؾ� �� ���
						else
						{
							pDestTemp += transCount;
						
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							CSpriteSurface::memcpyHalf(pDestTemp, pPixels, rectRight - index);
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					// ���
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
// pRect->top, pRect->bottom��ŭ�� ����Ѵ�.
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
	for (int i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		// (������,�����,�����)�� �ݺ� ��		
		count	= *pPixels++;		

		// �� �� ���
		if (count > 0)
		{
			j = count;
			do 
			{
				pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
				colorCount = *pPixels++;		// ���� �ƴ� �� ��				

				// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// Clipping���� �ʴ´�.
//----------------------------------------------------------------------
void
CSprite::BltAlpha(WORD *pDest, WORD pitch, BYTE alpha)
{
	// alpha Depth ����
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

			// (������,�����,�����)�� �ݺ� ��		
			count	= *pPixels++;		

			// �� �� ���
			if (count > 0)
			{
				j = count;
				do 
				{
					pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
					colorCount = *pPixels++;		// ���� �ƴ� �� ��				

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltAlphaClipLeft(WORD* pDest, WORD pitch, RECT* pRect, BYTE alpha)
{
	// alpha Depth ����
	CSpriteSurface::s_Value1 = alpha;
	CSpriteSurface::s_Value2 = 32 - alpha;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
		
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// �̹� �ܰ�� ��� ���
						CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyAlpha(pDestTemp, pPixels+dist, colorCount-dist);
						pDestTemp += colorCount-dist;
						pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += colorCount;
				index += colorCount;
			} while (--j);		

			//---------------------------------------------
			// �������ʹ� ��� ����Ѵ�.		
			//---------------------------------------------	
			if (--j > 0)			
			{			
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ �ǳ� �ڴ�.
					pDestTemp += transCount;			
					
					// �������� �ƴѸ�ŭ ������ش�.
					CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, colorCount);

					// memory addr ����
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
// ������ clipping.  
// pRect->right�� ������ ���� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltAlphaClipRight(WORD* pDest, WORD pitch, RECT* pRect, BYTE alpha)
{
	// alpha Depth ����
	CSpriteSurface::s_Value1 = alpha;
	CSpriteSurface::s_Value2 = 32 - alpha;
		
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
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
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
			
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...		
		// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
		//---------------------------------------------
		// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{		
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����
				index += transCount;
				
				// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
				// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

				// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

				//---------------------------------------------
				// ������ ������ �������� ���
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// ������������ �� ����� �ʿ䰡 ���� ��
					if (index > rectRight)
					{
						break;
					}
					// ������ �ƴ� ���� ���� ����ؾ� �� ���
					else
					{
						pDestTemp += transCount;
					
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, rectRight - index);
						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				// ���
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltAlphaClipWidth(WORD* pDest, WORD pitch, RECT* pRect, BYTE alpha)
{
	// alpha Depth ����
	CSpriteSurface::s_Value1 = alpha;
	CSpriteSurface::s_Value2 = 32 - alpha;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
		
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// �̹� �ܰ�� ��� ���?
						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > rectRight)
						{							
							// ������������ ������ �� �Ѿ�� ���
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

						// �̹� �ܰ�� ��� ���
						CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > rectRight)
						{
							CSpriteSurface::memcpyAlpha(pDestTemp, pPixels+dist, rectRight - rectLeft);

							j = 0;						
							break;
						}		

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyAlpha(pDestTemp, pPixels+dist, colorCount-dist);
						pDestTemp += colorCount-dist;
						pPixels += colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += colorCount;
				index += colorCount;
			} while (--j);		

			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...		
			// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
			//---------------------------------------------
			// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
			//---------------------------------------------
			if (--j > 0)
			{
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����
					index += transCount;
					
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

					//---------------------------------------------
					// ������ ������ �������� ���
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// ������������ �� ����� �ʿ䰡 ���� ��
						if (index > rectRight)
						{
							break;
						}
						// ������ �ƴ� ���� ���� ����ؾ� �� ���
						else
						{
							pDestTemp += transCount;
						
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, rectRight - index);
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					// ���
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
// pRect->top, pRect->bottom��ŭ�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltAlphaClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha)
{
	// alpha Depth ����
	CSpriteSurface::s_Value1 = alpha;
	CSpriteSurface::s_Value2 = 32 - alpha;

	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	for (int i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		// (������,�����,�����)�� �ݺ� ��		
		count	= *pPixels++;		

		// �� �� ���
		if (count > 0)
		{
			j = count;
			do 
			{
				pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
				colorCount = *pPixels++;		// ���� �ƴ� �� ��				

				// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// Clipping���� �ʴ´�.
//----------------------------------------------------------------------
void
CSprite::BltColor(WORD *pDest, WORD pitch, BYTE rgb)
{
	// rgb�� ����
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

			// (������,�����,�����)�� �ݺ� ��		
			count	= *pPixels++;		

			// �� �� ���
			if (count > 0)
			{
				j = count;
				do 
				{						
					pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
					colorCount = *pPixels++;		// ���� �ƴ� �� ��				

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltColorClipLeft(WORD* pDest, WORD pitch, RECT* pRect, BYTE rgb)
{
	// rgb�� ����
	CSpriteSurface::s_Value1 = rgb;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
		
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// �̹� �ܰ�� ��� ���
						CSpriteSurface::memcpyColor(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyColor(pDestTemp, pPixels+dist, colorCount-dist);
						pDestTemp += colorCount-dist;
						pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += colorCount;
				index += colorCount;
			} while (--j);
		

			//---------------------------------------------
			// �������ʹ� ��� ����Ѵ�.		
			//---------------------------------------------		
			if (--j > 0)
			{
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ �ǳ� �ڴ�.
					pDestTemp += transCount;			
					
					// �������� �ƴѸ�ŭ ������ش�.
					CSpriteSurface::memcpyColor(pDestTemp, pPixels, colorCount);

					// memory addr ����
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
// ������ clipping.  
// pRect->right�� ������ ���� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltColorClipRight(WORD* pDest, WORD pitch, RECT* pRect, BYTE rgb)
{
	// rgb�� ����
	CSpriteSurface::s_Value1 = rgb;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
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
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
			
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...		
		// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
		//---------------------------------------------
		// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����
				index += transCount;
				
				// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
				// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

				// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

				//---------------------------------------------
				// ������ ������ �������� ���
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// ������������ �� ����� �ʿ䰡 ���� ��
					if (index > rectRight)
					{
						break;
					}
					// ������ �ƴ� ���� ���� ����ؾ� �� ���
					else
					{
						pDestTemp += transCount;
					
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyColor(pDestTemp, pPixels, rectRight - index);
						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				// ���
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
// pRect->Right����..
//----------------------------------------------------------------------
void
CSprite::BltColorClipWidth(WORD* pDest, WORD pitch, RECT* pRect, BYTE rgb)
{
	// rgb�� ����
	CSpriteSurface::s_Value1 = rgb;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
		
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// �̹� �ܰ�� ��� ���?
						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > rectRight)
						{							
							// ������������ ������ �� �Ѿ�� ���
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

						// �̹� �ܰ�� ��� ���
						CSpriteSurface::memcpyColor(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > rectRight)
						{
							CSpriteSurface::memcpyColor(pDestTemp, pPixels+dist, rectRight - rectLeft);

							j = 0;						
							break;
						}		

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyColor(pDestTemp, pPixels+dist, colorCount-dist);
						pDestTemp += colorCount-dist;
						pPixels += colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += colorCount;
				index += colorCount;
			} while (--j);

			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...		
			// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
			//---------------------------------------------
			// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
			//---------------------------------------------
			if (--j > 0)
			{
				do
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����
					index += transCount;
					
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

					//---------------------------------------------
					// ������ ������ �������� ���
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// ������������ �� ����� �ʿ䰡 ���� ��
						if (index > rectRight)
						{
							break;
						}
						// ������ �ƴ� ���� ���� ����ؾ� �� ���
						else
						{
							pDestTemp += transCount;
						
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							CSpriteSurface::memcpyColor(pDestTemp, pPixels, rectRight - index);
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					// ���
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
// pRect->top, pRect->bottom��ŭ�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltColorClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE rgb)
{
	// rgb�� ����
	CSpriteSurface::s_Value1 = rgb;

	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	for (int i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		// (������,�����,�����)�� �ݺ� ��		
		count	= *pPixels++;		

		// �� �� ���
		if (count > 0)
		{
			j = count;
			do 
			{
				pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
				colorCount = *pPixels++;		// ���� �ƴ� �� ��				

				// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// Clipping���� �ʴ´�.
//----------------------------------------------------------------------
void
CSprite::BltScale(WORD *pDest, WORD pitch, BYTE scale)
{
	// scale�� ����
	CSpriteSurface::s_Value1 = scale;

	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;

	int rectBottom = m_Height;
	for (int i=0; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		// (������,�����,�����)�� �ݺ� ��		
		count	= *pPixels++;		

		// �� �� ���
		if (count > 0)
		{
			j = count;
			do 
			{
				pDestTemp += (*pPixels * scale);		// ��������ŭ �ǳ� �ڴ�.
				pPixels++;
				colorCount = *pPixels++;		// ���� �ƴ� �� ��				

				// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltScaleClipLeft(WORD* pDest, WORD pitch, RECT* pRect, BYTE scale)
{
	// scale�� ����
	CSpriteSurface::s_Value1 = scale;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
		
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += (index - rectLeft)*scale;

						// �̹� �ܰ�� ��� ���
						CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels, colorCount);
						pDestTemp += (colorCount*scale);
						pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels+dist, colorCount-dist);
						pDestTemp += (colorCount-dist)*scale;
						pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += colorCount;
				index += colorCount;
			} while (--j);

			//---------------------------------------------
			// �������ʹ� ��� ����Ѵ�.		
			//---------------------------------------------		
			if (--j > 0)
			{
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ �ǳ� �ڴ�.
					pDestTemp += transCount*scale;			
					
					// �������� �ƴѸ�ŭ ������ش�.
					CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels, colorCount);

					// memory addr ����
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
// ������ clipping.  
// pRect->right�� ������ ���� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltScaleClipRight(WORD* pDest, WORD pitch, RECT* pRect, BYTE scale)
{
	// scale�� ����
	CSpriteSurface::s_Value1 = scale;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
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
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
			
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...		
		// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
		//---------------------------------------------
		// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����
				index += transCount;
				
				// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
				// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

				// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

				//---------------------------------------------
				// ������ ������ �������� ���
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// ������������ �� ����� �ʿ䰡 ���� ��
					if (index > rectRight)
					{
						break;
					}
					// ������ �ƴ� ���� ���� ����ؾ� �� ���
					else
					{
						pDestTemp += transCount*scale;
					
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels, rectRight - index);
						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount*scale;

				// ���
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
// pRect->Right����..
//----------------------------------------------------------------------
void
CSprite::BltScaleClipWidth(WORD* pDest, WORD pitch, RECT* pRect, BYTE scale)
{
	// scale�� ����
	CSpriteSurface::s_Value1 = scale;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
		
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += (index - rectLeft)*scale;

						// �̹� �ܰ�� ��� ���?
						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > rectRight)
						{							
							// ������������ ������ �� �Ѿ�� ���
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

						// �̹� �ܰ�� ��� ���
						CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels, colorCount);
						pDestTemp += (colorCount*scale);
						pPixels += colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > rectRight)
						{
							CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels+dist, rectRight - rectLeft);

							j = 0;						
							break;
						}		

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels+dist, colorCount-dist);
						pDestTemp += (colorCount-dist)*scale;
						pPixels += colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += colorCount;
				index += colorCount;
			} while (--j);

			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...		
			// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
			//---------------------------------------------
			// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
			//---------------------------------------------
			if (--j > 0)
			{
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����
					index += transCount;
					
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

					//---------------------------------------------
					// ������ ������ �������� ���
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// ������������ �� ����� �ʿ䰡 ���� ��
						if (index > rectRight)
						{
							break;
						}
						// ������ �ƴ� ���� ���� ����ؾ� �� ���
						else
						{
							pDestTemp += transCount*scale;
						
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							CSpriteSurface::memcpyScale(pDestTemp, pitch, pPixels, rectRight - index);
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount*scale;

					// ���
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
// pRect->top, pRect->bottom��ŭ�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltScaleClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE scale)
{
	// scale�� ����
	CSpriteSurface::s_Value1 = scale;

	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	for (int i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		// (������,�����,�����)�� �ݺ� ��		
		count	= *pPixels++;		

		// �� �� ���
		if (count > 0)
		{
			j = count;
			do 
			{
				pDestTemp += *pPixels * scale;		// ��������ŭ �ǳ� �ڴ�.
				pPixels++;
				colorCount = *pPixels++;		// ���� �ƴ� �� ��				

				// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// Clipping���� �ʴ´�.
//----------------------------------------------------------------------
void
CSprite::BltDarkness(WORD *pDest, WORD pitch, BYTE DarkBits)
{
	// DarkBits�� ����
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

			// (������,�����,�����)�� �ݺ� ��		
			count	= *pPixels++;		

			// �� �� ���
			if (count > 0)
			{
				j = count;
				do 
				{
					pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
					colorCount = *pPixels++;		// ���� �ƴ� �� ��				

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltDarknessClipLeft(WORD* pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	// DarkBits�� ����
	CSpriteSurface::s_Value1 = DarkBits;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
		
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// �̹� �ܰ�� ��� ���
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, colorCount-dist);
						pDestTemp += colorCount-dist;
						pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += colorCount;
				index += colorCount;
			} while (--j);

			//---------------------------------------------
			// �������ʹ� ��� ����Ѵ�.		
			//---------------------------------------------		
			if (--j > 0)
			{
				do
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ �ǳ� �ڴ�.
					pDestTemp += transCount;			
					
					// �������� �ƴѸ�ŭ ������ش�.
					CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);

					// memory addr ����
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
// ������ clipping.  
// pRect->right�� ������ ���� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltDarknessClipRight(WORD* pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	// DarkBits�� ����
	CSpriteSurface::s_Value1 = DarkBits;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
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
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
			
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...		
		// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
		//---------------------------------------------
		// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����
				index += transCount;
				
				// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
				// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

				// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

				//---------------------------------------------
				// ������ ������ �������� ���
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// ������������ �� ����� �ʿ䰡 ���� ��
					if (index > rectRight)
					{
						break;
					}
					// ������ �ƴ� ���� ���� ����ؾ� �� ���
					else
					{
						pDestTemp += transCount;
					
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, rectRight - index);
						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				// ���
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
// rectRight����..
//----------------------------------------------------------------------
void
CSprite::BltDarknessClipWidth(WORD* pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	// DarkBits�� ����
	CSpriteSurface::s_Value1 = DarkBits;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
		
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// �̹� �ܰ�� ��� ���?
						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > rectRight)
						{							
							// ������������ ������ �� �Ѿ�� ���
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

						// �̹� �ܰ�� ��� ���
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > rectRight)
						{
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, rectRight - rectLeft);

							j = 0;						
							break;
						}		

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, colorCount-dist);
						pDestTemp += colorCount-dist;
						pPixels += colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += colorCount;
				index += colorCount;
			} while (--j);

			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...		
			// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
			//---------------------------------------------
			// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
			//---------------------------------------------
			if (--j > 0)
			{
				do
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����
					index += transCount;
					
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

					//---------------------------------------------
					// ������ ������ �������� ���
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// ������������ �� ����� �ʿ䰡 ���� ��
						if (index > rectRight)
						{
							break;
						}
						// ������ �ƴ� ���� ���� ����ؾ� �� ���
						else
						{
							pDestTemp += transCount;
						
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, rectRight - index);
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					// ���
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
// pRect->top, pRect->bottom��ŭ�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltDarknessClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	// DarkBits�� ����
	CSpriteSurface::s_Value1 = DarkBits;

	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	for (int i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		// (������,�����,�����)�� �ݺ� ��		
		count	= *pPixels++;		

		// �� �� ���
		if (count > 0)
		{
			j = count;
			do 
			{				
				pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
				colorCount = *pPixels++;		// ���� �ƴ� �� ��				

				// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// Clipping���� �ʴ´�.
//----------------------------------------------------------------------
void
CSprite::BltColorSet(WORD *pDest, WORD pitch, WORD colorSet)
{
	// colorSet�� ����
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

			// (������,�����,�����)�� �ݺ� ��		
			count	= *pPixels++;		

			// �� �� ���
			if (count > 0)
			{
				j = count;
				do 
				{
					pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
					colorCount = *pPixels++;		// ���� �ƴ� �� ��				

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltColorSetClipLeft(WORD* pDest, WORD pitch, RECT* pRect, WORD colorSet)
{
	// colorSet�� ����
	CSpriteSurface::s_Value1 = colorSet;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
		
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// �̹� �ܰ�� ��� ���
						CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels+dist, colorCount-dist);
						pDestTemp += colorCount-dist;
						pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += colorCount;
				index += colorCount;
			} while (--j);

			//---------------------------------------------
			// �������ʹ� ��� ����Ѵ�.		
			//---------------------------------------------		
			if (--j > 0)
			{
				do
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ �ǳ� �ڴ�.
					pDestTemp += transCount;			
					
					// �������� �ƴѸ�ŭ ������ش�.
					CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, colorCount);

					// memory addr ����
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
// ������ clipping.  
// pRect->right�� ������ ���� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltColorSetClipRight(WORD* pDest, WORD pitch, RECT* pRect, WORD colorSet)
{
	// colorSet�� ����
	CSpriteSurface::s_Value1 = colorSet;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
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
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
			
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...		
		// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
		//---------------------------------------------
		// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����
				index += transCount;
				
				// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
				// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

				// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

				//---------------------------------------------
				// ������ ������ �������� ���
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// ������������ �� ����� �ʿ䰡 ���� ��
					if (index > rectRight)
					{
						break;
					}
					// ������ �ƴ� ���� ���� ����ؾ� �� ���
					else
					{
						pDestTemp += transCount;
					
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, rectRight - index);
						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				// ���
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
// rectRight����..
//----------------------------------------------------------------------
void
CSprite::BltColorSetClipWidth(WORD* pDest, WORD pitch, RECT* pRect, WORD colorSet)
{
	// colorSet�� ����
	CSpriteSurface::s_Value1 = colorSet;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
		
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// �̹� �ܰ�� ��� ���?
						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > rectRight)
						{							
							// ������������ ������ �� �Ѿ�� ���
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

						// �̹� �ܰ�� ��� ���
						CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > rectRight)
						{
							CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels+dist, rectRight - rectLeft);

							j = 0;						
							break;
						}		

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels+dist, colorCount-dist);
						pDestTemp += colorCount-dist;
						pPixels += colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += colorCount;
				index += colorCount;
			} while (--j);

			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...		
			// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
			//---------------------------------------------
			// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
			//---------------------------------------------
			if (--j > 0)
			{
				do
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����
					index += transCount;
					
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

					//---------------------------------------------
					// ������ ������ �������� ���
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// ������������ �� ����� �ʿ䰡 ���� ��
						if (index > rectRight)
						{
							break;
						}
						// ������ �ƴ� ���� ���� ����ؾ� �� ���
						else
						{
							pDestTemp += transCount;
						
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, rectRight - index);
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					// ���
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
// pRect->top, pRect->bottom��ŭ�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltColorSetClipHeight(WORD *pDest, WORD pitch, RECT* pRect, WORD colorSet)
{
	// colorSet�� ����
	CSpriteSurface::s_Value1 = colorSet;

	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int i;
	register int j;

	int rectBottom = pRect->bottom;
	for (int i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		// (������,�����,�����)�� �ݺ� ��		
		count	= *pPixels++;		

		// �� �� ���
		if (count > 0)
		{
			j = count;
			do 
			{				
				pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
				colorCount = *pPixels++;		// ���� �ƴ� �� ��				

				// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// Clipping���� �ʴ´�.
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

			// (������,�����,�����)�� �ݺ� ��		
			count	= *pPixels++;		

			// �� �� ���
			if (count > 0)
			{
				j = count;
				do 
				{
					pDestTemp += *pPixels++;			// ��������ŭ �ǳ� �ڴ�.
					colorCount = *pPixels++;		// ���� �ƴ� �� ��				

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltEffectClipLeft(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
		
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// �̹� �ܰ�� ��� ���
						CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyEffect(pDestTemp, pPixels+dist, colorCount-dist);					
						pDestTemp += colorCount-dist;
						pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += colorCount;
				index += colorCount;
			} while (--j);

			//---------------------------------------------
			// �������ʹ� ��� ����Ѵ�.		
			//---------------------------------------------		
			if (--j > 0)
			{
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ �ǳ� �ڴ�.
					pDestTemp += transCount;			
					
					// �������� �ƴѸ�ŭ ������ش�.
					CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);

					// memory addr ����
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
// ������ clipping.  
// pRect->right�� ������ ���� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltEffectClipRight(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
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
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
			
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...		
		// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
		//---------------------------------------------
		// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����
				index += transCount;
				
				// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
				// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

				// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

				//---------------------------------------------
				// ������ ������ �������� ���
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// ������������ �� ����� �ʿ䰡 ���� ��
					if (index > rectRight)
					{
						break;
					}
					// ������ �ƴ� ���� ���� ����ؾ� �� ���
					else
					{
						pDestTemp += transCount;
					
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyEffect(pDestTemp, pPixels, rectRight - index);
						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				// ���
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
// rectRight����..
//----------------------------------------------------------------------
void
CSprite::BltEffectClipWidth(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
		
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// �̹� �ܰ�� ��� ���?
						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > rectRight)
						{							
							// ������������ ������ �� �Ѿ�� ���
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

						// �̹� �ܰ�� ��� ���
						CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > rectRight)
						{
							CSpriteSurface::memcpyEffect(pDestTemp, pPixels+dist, rectRight - rectLeft);

							j = 0;						
							break;
						}		

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyEffect(pDestTemp, pPixels+dist, colorCount-dist);					
						pDestTemp += colorCount-dist;
						pPixels += colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += colorCount;
				index += colorCount;
			} while (--j);

			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...		
			// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
			//---------------------------------------------
			// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
			//---------------------------------------------
			if (--j > 0)
			{
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����
					index += transCount;
					
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

					//---------------------------------------------
					// ������ ������ �������� ���
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// ������������ �� ����� �ʿ䰡 ���� ��
						if (index > rectRight)
						{
							break;
						}
						// ������ �ƴ� ���� ���� ����ؾ� �� ���
						else
						{
							pDestTemp += transCount;
						
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							CSpriteSurface::memcpyEffect(pDestTemp, pPixels, rectRight - index);
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					// ���
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
// pRect->top, pRect->bottom��ŭ�� ����Ѵ�.
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
	for (int i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		// (������,�����,�����)�� �ݺ� ��		
		count	= *pPixels++;		

		// �� �� ���
		if (count > 0)
		{
			j = count;
			do 
			{				
				pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
				colorCount = *pPixels++;		// ���� �ƴ� �� ��				

				// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// Clipping���� �ʴ´�.
//----------------------------------------------------------------------
/*
void
CSprite::BltAlphaFilter(WORD *pDest, WORD pitch, CFilter* pFilter)
{
	m_pFilter = pFilter;

	// ��¿� �̿��� Filter�� ���ǵǾ� ���� ���� ���
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
		// i��°���� Filter�� �о�´�.		
		//if (s_IndexY >= 0 && s_IndexY < m_pFilter->GetHeight())
		{
			s_pFilterLine	= m_pFilter->GetFilter(s_IndexY);

			s_IndexX		= -s_FilterCX;
			s_pFilterLine += -s_FilterCX;
		}
		

		// (������,�����,�����)�� �ݺ� ��		
		count	= *pPixels++;		
	

		s_bPutFilter = false;

		// �� �� ���
		for (register int j=0; j<count; j++)
		{				
			s_pFilterLine += *pPixels;		// filter�� ��������ŭ �ǳʶ��.			
			s_IndexX			+= *pPixels;

			pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
			colorCount = *pPixels++;		// ���� �ƴ� �� ��
			
			if	(s_IndexY >= 0 && s_IndexY < m_pFilter->GetHeight())
				// Filter�� �̿��ؼ� ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// Sprite�� (x,y)�� pFilter�� ...
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
			// Filter�� ������� �ʴ� ���
			//------------------------------------------
			if (yIndex < 0 || yIndex >= s_pFilter->GetHeight())
			{
				// (������,�����,�����)�� �ݺ� ��		
				count	= *pPixels++;		
 				// �� �� ���
				if (count > 0)
				{
					j = count;
					do 
					{			
						pDestTemp += *pPixels++;			// ��������ŭ �ǳ� �ڴ�.
						colorCount = *pPixels++;		// ���� �ƴ� �� ��				

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);

						pDestTemp	+= colorCount;
						pPixels		+= colorCount;
					} while (--j);
				}
			}
			//------------------------------------------
			// Filter�� ����ؾ� �ϴ� ���
			//------------------------------------------
			else
			{
				// ����ҷ��� �ٿ� ���õ� Filter�� ��´�.
				pFilter = s_pFilter->GetFilter( yIndex );
				pFilter += -s_X;
				xIndex = -s_X;		// xIndex�� x�� �̻��� ��� ����ϸ� �ȴ�.

				// (������,�����,�����)�� �ݺ� ��		
				count	= *pPixels++;		
 				// �� �� ���
				if (count > 0)
				{
					j = count;
					do 
					{				
						pDestTemp	+= *pPixels;			// ��������ŭ �ǳ� �ڴ�.
						xIndex		+= *pPixels;
						pFilter		+= *pPixels;
						pPixels ++;
						colorCount = *pPixels++;			// ���� �ƴ� �� ��				

						//------------------------------------------
						// Filter�� ����ؾ� �ϴ� ���
						//------------------------------------------
						if (xIndex >= 0)
						{			
							dist  = s_pFilter->GetWidth() - xIndex;
							if (dist > 0)
							{						
								// Filter�� �˳��� ���
								if (dist >= colorCount)
								{
									// ������ �ƴ� ������ Surface�� ����Ѵ�.
									memcpyAlphaFilter(pDestTemp, pPixels, pFilter, colorCount);
								}
								// Filter�� ������ ���
								else
								{
									// dist��ŭ�� Filter���
									memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist);
									memcpy((void*)(pDestTemp+dist), (void*)(pPixels+dist), (colorCount-dist)<<1);
								}
							}
							else
							{
								memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
							}
						}
						// xIndex�� ����(-)�� ���
						else
						{					
							dist = s_pFilter->GetWidth() - xIndex ;

							// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
							if (xIndex+colorCount > 0)
							{							
								// Filter�� ������ ���
								if (s_pFilter->GetWidth() < xIndex+colorCount)
								{	
									memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
									// Filter���
									memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
									
									// - -;;
									memcpy((void*)(pDestTemp+dist), (void*)(pPixels+dist), (colorCount-dist)<<1);
								}
								// Filter�� �˳��� ���
								else
								{							
									memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
									// dist��ŭ�� Filter���
									memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
								}						
							}
							else
							{
								// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
	// pRect��ŭ�� ���� ����Ѵ�.
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
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;	
		
		//------------------------------------------
		// Filter�� ������� �ʴ� ���
		//------------------------------------------
		if (yIndex < 0 || yIndex >= s_pFilter->GetHeight())
		{
			// (������,�����,�����)�� �ݺ� ��
			count = *pPixels++;		

			// �� �� ���		
			index = 0;
			
			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...
			// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
			//---------------------------------------------
			// xxxx�κб��� check���ִ� ��ƾ
			//---------------------------------------------
			if (count > 0)
			{
				j = count;
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����			
					index += transCount;
					
				
					//---------------------------------------------
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					if (index+colorCount > rectLeft)
					{
						//---------------------------------------------
						// ������������ xxxx������ �Ѿ�� ���
						//---------------------------------------------
						if (index > rectLeft)
						{	
							// �������κ� �ǳʶ�
							pDestTemp += index - rectLeft;

							// �̹� �ܰ�� ��� ���
							memcpy(pDestTemp, pPixels, colorCount<<1);
							pDestTemp += colorCount;
							pPixels += colorCount;

							// �������ʹ� ��� ����Ѵ�.
							break;
						}
						//---------------------------------------------
						// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
						// xxxx������ �Ѿ�� �Ǵ� ���
						//---------------------------------------------
						else
						{
							dist = rectLeft - index;

							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
							pDestTemp += colorCount-dist;
							pPixels += colorCount;

							// �������ʹ� ��� ����Ѵ�.
							break;
						}
					}					

					// ������ �ƴ� ����ŭ index����				
					pPixels += colorCount;
					index += colorCount;
				} while (--j);
			

				//---------------------------------------------
				// �������ʹ� ��� ����Ѵ�.		
				//---------------------------------------------		
				if (--j > 0)
				{
					do 
					{
						transCount = *pPixels++;		// ������ ��			
						colorCount = *pPixels++;		// ���� �ƴ� �� ��			
								
						// ��������ŭ �ǳ� �ڴ�.
						pDestTemp += transCount;			
						
						// �������� �ƴѸ�ŭ ������ش�.
						memcpy(pDestTemp, pPixels, colorCount<<1);

						// memory addr ����
						pDestTemp += colorCount;
						pPixels += colorCount;			
					} while (--j);
				}
			}
		}
		//------------------------------------------
		// Filter�� ����ؾ� �ϴ� ���
		//------------------------------------------
		else
		{
			// ����ҷ��� �ٿ� ���õ� Filter�� ��´�.
			pFilter = s_pFilter->GetFilter( yIndex );
			pFilter += -s_X;
			xIndex = -s_X;

			// (������,�����,�����)�� �ݺ� ��
			count = *pPixels++;		

			// �� �� ���		
			index = 0;
			
			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...
			// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
			//---------------------------------------------
			// xxxx�κб��� check���ִ� ��ƾ
			//---------------------------------------------
			if (count > 0)
			{
				j = count;
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��					
							
					// ��������ŭ index����			
					index += transCount;				
				
					//---------------------------------------------
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					if (index+colorCount > rectLeft)
					{
						//---------------------------------------------
						// ������������ xxxx������ �Ѿ�� ���
						//---------------------------------------------
						if (index > rectLeft)
						{	
							// �������κ� �ǳʶ�
							pDestTemp += index - rectLeft;
							xIndex += transCount;//index - rectLeft;
							pFilter += transCount;//index - rectLeft;

							// �̹� �ܰ�� ��� ���
							//memcpy(pDestTemp, pPixels, colorCount<<1);
							//------------------------------------------
							// Filter�� ����ؾ� �ϴ� ���
							//------------------------------------------
							if (xIndex >= 0)
							{			
								dist2  = s_pFilter->GetWidth() - xIndex;
								if (dist2 > 0)
								{						
									// Filter�� �˳��� ���
									if (dist2 >= colorCount)
									{
										// ������ �ƴ� ������ Surface�� ����Ѵ�.
										memcpyAlphaFilter(pDestTemp, pPixels, pFilter, colorCount);
									}
									// Filter�� ������ ���
									else
									{
										// dist2��ŭ�� Filter���
										memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist2);
										memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (colorCount-dist2)<<1);
									}
								}
								else
								{
									memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
								}
							}
							// xIndex�� ����(-)�� ���
							else
							{					
								dist2 = s_pFilter->GetWidth() - xIndex ;

								// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
								if (xIndex+colorCount > 0)
								{							
									// Filter�� ������ ���
									if (s_pFilter->GetWidth() < xIndex+colorCount)
									{	
										memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
										// Filter���
										memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
										
										// - -;;
										memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (colorCount-dist2)<<1);
									}
									// Filter�� �˳��� ���
									else
									{							
										memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
										// dist2��ŭ�� Filter���
										memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
									}						
								}
								else
								{
									// ������ �ƴ� ������ Surface�� ����Ѵ�.
									memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
								}
							}
							pDestTemp	+= colorCount;
							pPixels		+= colorCount;
							xIndex		+= colorCount;
							pFilter		+= colorCount;


							// �������ʹ� ��� ����Ѵ�.
							break;
						}
						//---------------------------------------------
						// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
						// xxxx������ �Ѿ�� �Ǵ� ���
						//---------------------------------------------
						else
						{	
							dist = rectLeft - index;

							xIndex	+= transCount + dist;
							pFilter	+= transCount + dist;

							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
							//pDestTemp += colorCount-dist;
							//pPixels += colorCount;
							
							//------------------------------------------
							// Filter�� ����ؾ� �ϴ� ���
							//------------------------------------------
							if (xIndex >= 0)
							{			
								dist2  = s_pFilter->GetWidth() - xIndex;
								if (dist2 > 0)
								{						
									// Filter�� �˳��� ���
									if (dist2 >= colorCount-dist)
									{
										// ������ �ƴ� ������ Surface�� ����Ѵ�.
										memcpyAlphaFilter(pDestTemp, pPixels+dist, pFilter, colorCount-dist);
									}
									// Filter�� ������ ���
									else
									{
										// dist2��ŭ�� Filter���
										memcpyAlphaFilter(pDestTemp, pPixels+dist, pFilter, dist2);
										memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist+dist2), (colorCount-dist - dist2)<<1);
									}								
								}
								else
								{
									memcpy((void*)pDestTemp, (void*)(pPixels+dist), (colorCount-dist)<<1);
								}							
							}
							// xIndex�� ����(-)�� ���
							else
							{					
								dist2 = s_pFilter->GetWidth() - xIndex ;

								// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
								if (xIndex+colorCount-dist > 0)
								{							
									// Filter�� ������ ���
									if (s_pFilter->GetWidth() < xIndex+(colorCount-dist))
									{	
										memcpy((void*)pDestTemp, (void*)(pPixels+dist), (-xIndex)<<1);
										// Filter���
										memcpyAlphaFilter(pDestTemp-xIndex, pPixels+dist-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
										
										// - -;;
										memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist+dist2), (colorCount-dist - dist2)<<1);
									}
									// Filter�� �˳��� ���
									else
									{							
										memcpy((void*)pDestTemp, (void*)(pPixels+dist), (-xIndex)<<1);
										// dist2��ŭ�� Filter���
										memcpyAlphaFilter(pDestTemp-xIndex, pPixels+dist-xIndex, pFilter-xIndex, (colorCount-dist)+xIndex);
									}						
								}
								else
								{
									// ������ �ƴ� ������ Surface�� ����Ѵ�.
									memcpy((void*)pDestTemp, (void*)(pPixels+dist), (colorCount-dist)<<1);
								}
							}

							pDestTemp += colorCount-dist;
							pPixels += colorCount;
							xIndex	+= colorCount-dist;
							pFilter += colorCount-dist;
							
							// �������ʹ� ��� ����Ѵ�.
							break;
						}
						
							
					}	
					
					// ������ �κ� ����
					xIndex += transCount;
					pFilter += transCount;

					// ������ �ƴ� ����ŭ index����				
					pPixels += colorCount;
					index	+= colorCount;
					xIndex	+= colorCount;
					pFilter += colorCount;
				} while (--j);
			
			
				//---------------------------------------------
				// �������ʹ� ��� ����Ѵ�.		
				//---------------------------------------------		
				if (--j > 0)
				{
					do
					{
						transCount = *pPixels++;		// ������ ��			
						colorCount = *pPixels++;		// ���� �ƴ� �� ��			
								
						// ��������ŭ �ǳ� �ڴ�.
						pDestTemp	+= transCount;			
						xIndex		+= transCount;
						pFilter		+= transCount;

						//------------------------------------------
						// Filter�� ����ؾ� �ϴ� ���
						//------------------------------------------
						if (xIndex >= 0)
						{			
							dist2  = s_pFilter->GetWidth() - xIndex;
							if (dist2 > 0)
							{						
								// Filter�� �˳��� ���
								if (dist2 >= colorCount)
								{
									// ������ �ƴ� ������ Surface�� ����Ѵ�.
									memcpyAlphaFilter(pDestTemp, pPixels, pFilter, colorCount);
								}
								// Filter�� ������ ���
								else
								{
									// dist2��ŭ�� Filter���
									memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist2);
									memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (colorCount-dist2)<<1);
								}
							}
							else
							{
								memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
							}
						}
						// xIndex�� ����(-)�� ���
						else
						{					
							dist2 = s_pFilter->GetWidth() - xIndex ;

							// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
							if (xIndex+colorCount > 0)
							{							
								// Filter�� ������ ���
								if (s_pFilter->GetWidth() < xIndex+colorCount)
								{	
									memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
									// Filter���
									memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
									
									// - -;;
									memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (colorCount-dist2)<<1);
								}
								// Filter�� �˳��� ���
								else
								{							
									memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
									// dist2��ŭ�� Filter���
									memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
								}						
							}
							else
							{
								// ������ �ƴ� ������ Surface�� ����Ѵ�.
								memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
							}
						}

						// memory addr ����
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
	// pRect��ŭ�� ���� ����Ѵ�.
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
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;	
		
		//------------------------------------------
		// Filter�� ������� �ʴ� ���
		//------------------------------------------
		if (yIndex < 0 || yIndex >= s_pFilter->GetHeight())
		{
			// (������,�����,�����)�� �ݺ� ��
			count = *pPixels++;		

			// �� �� ���		
			index = 0;
				
			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...		
			// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
			//---------------------------------------------
			// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
			//---------------------------------------------
			if (count > 0)
			{
				j = count;
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����
					index += transCount;
					
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

					//---------------------------------------------
					// ������ ������ �������� ���
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// ������������ �� ����� �ʿ䰡 ���� ��
						if (index > rectRight)
						{
							break;
						}
						// ������ �ƴ� ���� ���� ����ؾ� �� ���
						else
						{
							pDestTemp += transCount;
						
							// ������ �ƴ� ������ Surface�� ����Ѵ�.						
							memcpy((void*)pDestTemp, (void*)pPixels, (rectRight-index)<<1);
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					// ���
					memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
					pDestTemp += colorCount;
					pPixels += colorCount;			
					index += colorCount;
				} while (--j);
			}
		}
		//------------------------------------------
		// Filter�� ����ϴ� ���
		//------------------------------------------
		else
		{
			// ����ҷ��� �ٿ� ���õ� Filter�� ��´�.
			pFilter = s_pFilter->GetFilter( yIndex );
			pFilter += -s_X;
			xIndex = -s_X;		

			// (������,�����,�����)�� �ݺ� ��
			count = *pPixels++;		

			// �� �� ���		
			index = 0;
				
			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...		
			// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
			//---------------------------------------------
			// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
			//---------------------------------------------
			if (count > 0)
			{
				j = count;
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����
					index += transCount;
					
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

					//---------------------------------------------
					// ������ ������ �������� ���
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// ������������ �� ����� �ʿ䰡 ���� ��
						if (index > rectRight)
						{
							break;
						}
						// ������ �ƴ� ���� ���� ����ؾ� �� ���
						else
						{
							pDestTemp	+= transCount;
							xIndex		+= transCount;
							pFilter		+= transCount;

							dist		= rectRight - index;
						
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							//memcpy((void*)pDestTemp, (void*)pPixels, (rectRight - index)<<1);
							//------------------------------------------
							// Filter�� ����ؾ� �ϴ� ���
							//------------------------------------------
							if (xIndex >= 0)
							{	
								dist2  = s_pFilter->GetWidth() - xIndex;
								if (dist2 > 0)
								{						
									// Filter�� �˳��� ���
									if (dist2 >= dist)
									{
										// ������ �ƴ� ������ Surface�� ����Ѵ�.
										memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist);
									}
									// Filter�� ������ ���
									else
									{
										// dist2��ŭ�� Filter���
										memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist2);
										memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (dist - dist2)<<1);
									}
								}
								else
								{
									memcpy((void*)pDestTemp, (void*)pPixels, dist<<1);
								}
							}
							// xIndex�� ����(-)�� ���
							else
							{					
								dist2 = s_pFilter->GetWidth() - xIndex ;

								// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
								if (xIndex + dist > 0)
								{							
									// Filter�� ������ ���
									if (s_pFilter->GetWidth() < xIndex+colorCount)
									{	
										memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
										// Filter���
										memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
										
										// - -;;
										int di = dist - dist2;
										if (di>0)
										{
											memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), di<<1);
										}
									}
									// Filter�� �˳��� ���
									else
									{							
										memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
										// dist2��ŭ�� Filter���
										memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, dist + xIndex);
									}						
								}
								else
								{
									// ������ �ƴ� ������ Surface�� ����Ѵ�.
									memcpy((void*)pDestTemp, (void*)pPixels, dist<<1);
								}
							}
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp	+= transCount;
					xIndex		+= transCount;
					pFilter		+= transCount;

					// ���
					//------------------------------------------
					// Filter�� ����ؾ� �ϴ� ���
					//------------------------------------------
					if (xIndex >= 0)
					{			
						dist2  = s_pFilter->GetWidth() - xIndex;
						if (dist2 > 0)
						{						
							// Filter�� �˳��� ���
							if (dist2 >= colorCount)
							{
								// ������ �ƴ� ������ Surface�� ����Ѵ�.
								memcpyAlphaFilter(pDestTemp, pPixels, pFilter, colorCount);
							}
							// Filter�� ������ ���
							else
							{
								// dist2��ŭ�� Filter���
								memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist2);
								memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (colorCount-dist2)<<1);
							}
						}
						else
						{
							memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
						}
					}
					// xIndex�� ����(-)�� ���
					else
					{					
						dist2 = s_pFilter->GetWidth() - xIndex ;

						// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
						if (xIndex+colorCount > 0)
						{							
							// Filter�� ������ ���
							if (s_pFilter->GetWidth() < xIndex+colorCount)
							{	
								memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
								// Filter���
								memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
								
								// - -;;
								memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (colorCount-dist2)<<1);
							}
							// Filter�� �˳��� ���
							else
							{							
								memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
								// dist2��ŭ�� Filter���
								memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
							}						
						}
						else
						{
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
//   "�����κ�+����"�� �� ȭ��(800)�� �Ѿ ���..
//   �� ���̿� Filter���� �����ؼ� ����Ҷ�.. ����..����.. - -;;
//
//----------------------------------------------------------------------

void		
CSprite::BltAlphaFilterClipWidth(WORD *pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
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
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;	
		
		//------------------------------------------
		// Filter�� ������� �ʴ� ���
		//------------------------------------------
		if (yIndex < 0 || yIndex >= s_pFilter->GetHeight())
		{
			// (������,�����,�����)�� �ݺ� ��
			count = *pPixels++;		

			// �� �� ���		
			index = 0;
			
			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...
			// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
			//---------------------------------------------
			// xxxx�κб��� check���ִ� ��ƾ
			//---------------------------------------------
			if (count > 0)
			{
				j = count;
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����			
					index += transCount;
					
				
					//---------------------------------------------
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					if (index+colorCount > rectLeft)
					{
						//---------------------------------------------
						// ������������ xxxx������ �Ѿ�� ���
						//---------------------------------------------
						if (index > rectLeft)
						{	
							// �������κ� �ǳʶ�
							pDestTemp += index - rectLeft;

							// �̹� �ܰ�� ��� ���?
							// ������ ���� �Ѿ�� ���..
							if (index+colorCount > rectRight)
							{							
								// ������������ ������ �� �Ѿ�� ���
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

							// �̹� �ܰ�� ��� ���
							memcpy(pDestTemp, pPixels, colorCount<<1);
							pDestTemp += colorCount;
							pPixels += colorCount;
							index += colorCount;

							// �������ʹ� ��� ����Ѵ�.
							break;
						}
						//---------------------------------------------
						// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
						// xxxx������ �Ѿ�� �Ǵ� ���
						//---------------------------------------------
						else
						{
							dist = rectLeft - index;

							// ������ ���� �Ѿ�� ���..
							if (index+colorCount > rectRight)
							{
								memcpy(pDestTemp, pPixels+dist, (rectRight - rectLeft)<<1);

								j = 0;						
								break;
							}		

							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
							pDestTemp += colorCount-dist;
							pPixels += colorCount;
							index += colorCount;

							// �������ʹ� ��� ����Ѵ�.
							break;
						}
					}					

					// ������ �ƴ� ����ŭ index����				
					pPixels += colorCount;
					index += colorCount;
				} while (--j);
			
				//---------------------------------------------
				// �� �ٸ��� Clipping�� ����� �ϴµ�...		
				// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
				//---------------------------------------------
				// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
				//---------------------------------------------
				if (--j > 0)
				{
					do 
					{
						transCount = *pPixels++;		// ������ ��			
						colorCount = *pPixels++;		// ���� �ƴ� �� ��			
								
						// ��������ŭ index����
						index += transCount;
						
						// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
						// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

						// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

						//---------------------------------------------
						// ������ ������ �������� ���
						//---------------------------------------------			
						if (index+colorCount > rectRight)
						{
							// ������������ �� ����� �ʿ䰡 ���� ��
							if (index > rectRight)
							{
								break;
							}
							// ������ �ƴ� ���� ���� ����ؾ� �� ���
							else
							{
								pDestTemp += transCount;
							
								// ������ �ƴ� ������ Surface�� ����Ѵ�.						
								memcpy((void*)pDestTemp, (void*)pPixels, (rectRight-index)<<1);
								break;
							}
						}

						// ��������ŭ �ǳʶ��
						pDestTemp += transCount;

						// ���
						memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
						pDestTemp += colorCount;
						pPixels += colorCount;			
						index += colorCount;
					} while (--j);
				}
			}
		}
		//------------------------------------------
		// Filter�� ����ؾ� �ϴ� ���
		//------------------------------------------
		else
		{
			// ����ҷ��� �ٿ� ���õ� Filter�� ��´�.
			pFilter = s_pFilter->GetFilter( yIndex );
			pFilter += -s_X;
			xIndex = -s_X;

			// (������,�����,�����)�� �ݺ� ��
			count = *pPixels++;		

			// �� �� ���		
			index = 0;
			
			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...
			// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
			//---------------------------------------------
			// xxxx�κб��� check���ִ� ��ƾ
			//---------------------------------------------
			if (count > 0)
			{
				j = count;
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��					
							
					// ��������ŭ index����			
					index += transCount;				
				
					//---------------------------------------------
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					if (index+colorCount > rectLeft)
					{
						//---------------------------------------------
						// ������������ xxxx������ �Ѿ�� ���
						//---------------------------------------------
						if (index > rectLeft)
						{	
							//---------------------------------------------
							// �������� �Ѿ���� check�ؾ� �Ѵ�.
							//---------------------------------------------
							//if (index+colorCount)


							// �������κ� �ǳʶ�
							pDestTemp += index - rectLeft;
							xIndex += transCount;//index - rectLeft;
							pFilter += transCount;//index - rectLeft;							

							// �̹� �ܰ�� ��� ���
							//memcpy(pDestTemp, pPixels, colorCount<<1);
							//------------------------------------------
							// Filter�� ����ؾ� �ϴ� ���
							//------------------------------------------
							if (xIndex >= 0)
							{			
								dist2  = s_pFilter->GetWidth() - xIndex;
								if (dist2 > 0)
								{						
									// Filter�� �˳��� ���
									if (dist2 >= colorCount)
									{
										// ������ �ƴ� ������ Surface�� ����Ѵ�.
										memcpyAlphaFilter(pDestTemp, pPixels, pFilter, colorCount);
									}
									// Filter�� ������ ���
									else
									{
										// dist2��ŭ�� Filter���
										memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist2);
										memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (colorCount-dist2)<<1);
									}
								}
								else
								{
									memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
								}
							}
							// xIndex�� ����(-)�� ���
							else
							{					
								dist2 = s_pFilter->GetWidth() - xIndex ;

								// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
								if (xIndex+colorCount > 0)
								{							
									// Filter�� ������ ���
									if (s_pFilter->GetWidth() < xIndex+colorCount)
									{	
										memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
										// Filter���
										memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
										
										// - -;;
										memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (colorCount-dist2)<<1);
									}
									// Filter�� �˳��� ���
									else
									{							
										memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
										// dist2��ŭ�� Filter���
										memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
									}						
								}
								else
								{
									// ������ �ƴ� ������ Surface�� ����Ѵ�.
									memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
								}
							}
							pDestTemp	+= colorCount;
							pPixels		+= colorCount;
							xIndex		+= colorCount;
							pFilter		+= colorCount;
							index		+= colorCount;

							// �������ʹ� ��� ����Ѵ�.
							break;
						}
						//---------------------------------------------
						// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
						// xxxx������ �Ѿ�� �Ǵ� ���
						//---------------------------------------------
						else
						{	
							dist = rectLeft - index;

							xIndex	+= transCount + dist;
							pFilter	+= transCount + dist;

							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
							//pDestTemp += colorCount-dist;
							//pPixels += colorCount;
							
							//------------------------------------------
							// Filter�� ����ؾ� �ϴ� ���
							//------------------------------------------
							if (xIndex >= 0)
							{			
								dist2  = s_pFilter->GetWidth() - xIndex;
								if (dist2 > 0)
								{						
									// Filter�� �˳��� ���
									if (dist2 >= colorCount-dist)
									{
										// ������ �ƴ� ������ Surface�� ����Ѵ�.
										memcpyAlphaFilter(pDestTemp, pPixels+dist, pFilter, colorCount-dist);
									}
									// Filter�� ������ ���
									else
									{
										// dist2��ŭ�� Filter���
										memcpyAlphaFilter(pDestTemp, pPixels+dist, pFilter, dist2);
										memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist+dist2), (colorCount-dist - dist2)<<1);
									}								
								}
								else
								{
									memcpy((void*)pDestTemp, (void*)(pPixels+dist), (colorCount-dist)<<1);
								}							
							}
							// xIndex�� ����(-)�� ���
							else
							{					
								dist2 = s_pFilter->GetWidth() - xIndex ;

								// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
								if (xIndex+colorCount-dist > 0)
								{							
									// Filter�� ������ ���
									if (s_pFilter->GetWidth() < xIndex+(colorCount-dist))
									{	
										memcpy((void*)pDestTemp, (void*)(pPixels+dist), (-xIndex)<<1);
										// Filter���
										memcpyAlphaFilter(pDestTemp-xIndex, pPixels+dist-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
										
										// - -;;
										memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist+dist2), (colorCount-dist - dist2)<<1);
									}
									// Filter�� �˳��� ���
									else
									{							
										memcpy((void*)pDestTemp, (void*)(pPixels+dist), (-xIndex)<<1);
										// dist2��ŭ�� Filter���
										memcpyAlphaFilter(pDestTemp-xIndex, pPixels+dist-xIndex, pFilter-xIndex, (colorCount-dist)+xIndex);
									}						
								}
								else
								{
									// ������ �ƴ� ������ Surface�� ����Ѵ�.
									memcpy((void*)pDestTemp, (void*)(pPixels+dist), (colorCount-dist)<<1);
								}
							}

							pDestTemp += colorCount-dist;
							pPixels		+= colorCount;
							xIndex		+= colorCount-dist;
							pFilter		+= colorCount-dist;
							index		+= colorCount;
							
							// �������ʹ� ��� ����Ѵ�.
							break;
						}
					}	
					
					// ������ �κ� ����
					xIndex += transCount;
					pFilter += transCount;

					// ������ �ƴ� ����ŭ index����				
					pPixels += colorCount;
					index	+= colorCount;
					xIndex	+= colorCount;
					pFilter += colorCount;
				} while (--j);
				
			
				//---------------------------------------------
				// �� �ٸ��� Clipping�� ����� �ϴµ�...		
				// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
				//---------------------------------------------
				// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
				//---------------------------------------------
				if (--j > 0)
				{
					do 
					{
						transCount = *pPixels++;		// ������ ��			
						colorCount = *pPixels++;		// ���� �ƴ� �� ��			
								
						// ��������ŭ index����
						index += transCount;
						
						// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
						// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

						// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

						//---------------------------------------------
						// ������ ������ �������� ���
						//---------------------------------------------			
						if (index+colorCount > rectRight)
						{
							// ������������ �� ����� �ʿ䰡 ���� ��
							if (index > rectRight)
							{
								break;
							}
							// ������ �ƴ� ���� ���� ����ؾ� �� ���
							else
							{
								pDestTemp	+= transCount;
								xIndex		+= transCount;
								pFilter		+= transCount;

								dist		= rectRight - index;
							
								// ������ �ƴ� ������ Surface�� ����Ѵ�.
								//memcpy((void*)pDestTemp, (void*)pPixels, (rectRight - index)<<1);
								//------------------------------------------
								// Filter�� ����ؾ� �ϴ� ���
								//------------------------------------------
								if (xIndex >= 0)
								{	
									dist2  = s_pFilter->GetWidth() - xIndex;
									if (dist2 > 0)
									{						
										// Filter�� �˳��� ���
										if (dist2 >= dist)
										{
											// ������ �ƴ� ������ Surface�� ����Ѵ�.
											memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist);
										}
										// Filter�� ������ ���
										else
										{
											// dist2��ŭ�� Filter���
											memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist2);
											memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (dist - dist2)<<1);
										}
									}
									else
									{
										memcpy((void*)pDestTemp, (void*)pPixels, dist<<1);
									}
								}
								// xIndex�� ����(-)�� ���
								else
								{					
									dist2 = s_pFilter->GetWidth() - xIndex ;

									// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
									if (xIndex + dist > 0)
									{							
										// Filter�� ������ ���
										if (s_pFilter->GetWidth() < xIndex+colorCount)
										{	
											memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
											// Filter���
											memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
											
											// - -;;
											memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (dist - dist2)<<1);
										}
										// Filter�� �˳��� ���
										else
										{							
											memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
											// dist2��ŭ�� Filter���
											memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, dist + xIndex);
										}						
									}
									else
									{
										// ������ �ƴ� ������ Surface�� ����Ѵ�.
										memcpy((void*)pDestTemp, (void*)pPixels, dist<<1);
									}
								}
								break;
							}
						}

						// ��������ŭ �ǳʶ��
						pDestTemp	+= transCount;
						xIndex		+= transCount;
						pFilter		+= transCount;

						// ���
						//------------------------------------------
						// Filter�� ����ؾ� �ϴ� ���
						//------------------------------------------
						if (xIndex >= 0)
						{			
							dist2  = s_pFilter->GetWidth() - xIndex;
							if (dist2 > 0)
							{						
								// Filter�� �˳��� ���
								if (dist2 >= colorCount)
								{
									// ������ �ƴ� ������ Surface�� ����Ѵ�.
									memcpyAlphaFilter(pDestTemp, pPixels, pFilter, colorCount);
								}
								// Filter�� ������ ���
								else
								{
									// dist2��ŭ�� Filter���
									memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist2);
									memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (colorCount-dist2)<<1);
								}
							}
							else
							{
								memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
							}
						}
						// xIndex�� ����(-)�� ���
						else
						{					
							dist2 = s_pFilter->GetWidth() - xIndex ;

							// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
							if (xIndex+colorCount > 0)
							{							
								// Filter�� ������ ���
								if (s_pFilter->GetWidth() < xIndex+colorCount)
								{	
									memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
									// Filter���
									memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
									
									// - -;;
									memcpy((void*)(pDestTemp+dist2), (void*)(pPixels+dist2), (colorCount-dist2)<<1);
								}
								// Filter�� �˳��� ���
								else
								{							
									memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
									// dist2��ŭ�� Filter���
									memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
								}						
							}
							else
							{
								// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// pRect->top, pRect->bottom��ŭ�� ����Ѵ�.
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
	for (int i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		//------------------------------------------
		// Filter�� ������� �ʴ� ���
		//------------------------------------------
		if (yIndex < 0 || yIndex >= s_pFilter->GetHeight())
		{
			// (������,�����,�����)�� �ݺ� ��		
			count	= *pPixels++;		

			// �� �� ���
			if (count > 0)
			{
				j = count;
				do 
				{				
					pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
					colorCount = *pPixels++;		// ���� �ƴ� �� ��				

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
					
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;
				} while (--j);
			}
		}
		//------------------------------------------
		// Filter�� ����ؾ� �ϴ� ���
		//------------------------------------------
		else
		{
			// ����ҷ��� �ٿ� ���õ� Filter�� ��´�.
			pFilter = s_pFilter->GetFilter( yIndex );
			pFilter += -s_X;
			xIndex = -s_X;		// xIndex�� x�� �̻��� ��� ����ϸ� �ȴ�.

			// (������,�����,�����)�� �ݺ� ��		
			count	= *pPixels++;		
 			// �� �� ���
			if (count > 0)
			{
				j = count;
				do 
				{
					pDestTemp	+= *pPixels;			// ��������ŭ �ǳ� �ڴ�.
					xIndex		+= *pPixels;
					pFilter		+= *pPixels;
					pPixels ++;
					colorCount = *pPixels++;			// ���� �ƴ� �� ��				

					//------------------------------------------
					// Filter�� ����ؾ� �ϴ� ���
					//------------------------------------------
					if (xIndex >= 0)
					{			
						dist  = s_pFilter->GetWidth() - xIndex;
						if (dist > 0)
						{						
							// Filter�� �˳��� ���
							if (dist >= colorCount)
							{
								// ������ �ƴ� ������ Surface�� ����Ѵ�.
								memcpyAlphaFilter(pDestTemp, pPixels, pFilter, colorCount);
							}
							// Filter�� ������ ���
							else
							{
								// dist��ŭ�� Filter���
								memcpyAlphaFilter(pDestTemp, pPixels, pFilter, dist);
								memcpy((void*)(pDestTemp+dist), (void*)(pPixels+dist), (colorCount-dist)<<1);
							}
						}
						else
						{
							memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
						}
					}
					// xIndex�� ����(-)�� ���
					else
					{					
						dist = s_pFilter->GetWidth() - xIndex ;

						// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
						if (xIndex+colorCount > 0)
						{							
							// Filter�� ������ ���
							if (s_pFilter->GetWidth() < xIndex+colorCount)
							{	
								memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
								// Filter���
								memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
								
								// - -;;
								memcpy((void*)(pDestTemp+dist), (void*)(pPixels+dist), (colorCount-dist)<<1);
							}
							// Filter�� �˳��� ���
							else
							{							
								memcpy((void*)pDestTemp, (void*)pPixels, (-xIndex)<<1);
								// dist��ŭ�� Filter���
								memcpyAlphaFilter(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
							}						
						}
						else
						{
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// pFilter�� �̿��ؼ� ����Ѵ�.
// pFilter�� alpha ������ �̿��Ѵ�.
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
	// ������ ���
	while (j--)
	{			
		//alpha = *pFilter;		

		// ���� ���
		/*
		sTemp = *pSource;
		dTemp = *pDest;

		sr = ColorDraw::Red(sTemp);
		sg = ColorDraw::Green(sTemp);
		sb = ColorDraw::Blue(sTemp);

		dr = ColorDraw::Red(dTemp);
		dg = ColorDraw::Green(dTemp);
		db = ColorDraw::Blue(dTemp);		
	
		*pDest = ((alpha * (sb - db) >> 5) + db |
					((alpha * (sg - dg) >> 5) + dg) << ColorDraw::s_bSHIFT_G |
					((alpha * (sr - dr) >> 5) + dr) << ColorDraw::s_bSHIFT_R);
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
				((*pDest >> 1) & ColorDraw::s_wMASK_SHIFT[1]) +
				((*pSource >> 1) & ColorDraw::s_wMASK_SHIFT[1]);
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
// Sprite�� (x,y)�� pFilter�� ...
//
//----------------------------------------------------------------------
void
CSprite::BltAlphaFilterDarkness(WORD *pDest, WORD pitch, BYTE DarkBits)
{
	// ��Ӱ� �ϴ� bit�� ����
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
	for (int i=0; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		//------------------------------------------
		// Filter�� ������� �ʴ� ���
		//------------------------------------------
		if (yIndex < 0 || yIndex >= s_pFilter->GetHeight())
		{
			// (������,�����,�����)�� �ݺ� ��		
			count	= *pPixels++;		
 			// �� �� ���
			if (count > 0)
			{
				j = count;
				do 
				{
					pDestTemp += *pPixels++;			// ��������ŭ �ǳ� �ڴ�.
					colorCount = *pPixels++;		// ���� �ƴ� �� ��				

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);

					pDestTemp	+= colorCount;
					pPixels		+= colorCount;
				} while (--j);
			}
		}
		//------------------------------------------
		// Filter�� ����ؾ� �ϴ� ���
		//------------------------------------------
		else
		{
			// ����ҷ��� �ٿ� ���õ� Filter�� ��´�.
			pFilter = s_pFilter->GetFilter( yIndex );
			pFilter += -s_X;
			xIndex = -s_X;		// xIndex�� x�� �̻��� ��� ����ϸ� �ȴ�.

			// (������,�����,�����)�� �ݺ� ��		
			count	= *pPixels++;		
 			// �� �� ���
			if (count > 0)
			{
				j = count;
				do 
				{			
					pDestTemp	+= *pPixels;			// ��������ŭ �ǳ� �ڴ�.
					xIndex		+= *pPixels;
					pFilter		+= *pPixels;
					pPixels ++;
					colorCount = *pPixels++;			// ���� �ƴ� �� ��				

					//------------------------------------------
					// Filter�� ����ؾ� �ϴ� ���
					//------------------------------------------
					if (xIndex >= 0)
					{			
						dist  = s_pFilter->GetWidth() - xIndex;
						if (dist > 0)
						{						
							// Filter�� �˳��� ���
							if (dist >= colorCount)
							{
								// ������ �ƴ� ������ Surface�� ����Ѵ�.
								memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, colorCount);
							}
							// Filter�� ������ ���
							else
							{
								// dist��ŭ�� Filter���
								memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist);
								CSpriteSurface::memcpyDarkness(pDestTemp+dist, pPixels+dist, colorCount-dist);
							}
						}
						else
						{
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						}
					}
					// xIndex�� ����(-)�� ���
					else
					{					
						dist = s_pFilter->GetWidth() - xIndex ;

						// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
						if (xIndex+colorCount > 0)
						{							
							// Filter�� ������ ���
							if (s_pFilter->GetWidth() < xIndex+colorCount)
							{	
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
								// Filter���
								memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
								
								// - -;;
								CSpriteSurface::memcpyDarkness(pDestTemp+dist, pPixels+dist, colorCount-dist);
							}
							// Filter�� �˳��� ���
							else
							{							
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
								// dist��ŭ�� Filter���
								memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
							}						
						}
						else
						{
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
	// ��Ӱ� �ϴ� bit�� ����
	s_Value1 = DarkBits;
	CSpriteSurface::s_Value1 = DarkBits;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
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
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;	
		
		//------------------------------------------
		// Filter�� ������� �ʴ� ���
		//------------------------------------------
		if (yIndex < 0 || yIndex >= s_pFilter->GetHeight())
		{
			// (������,�����,�����)�� �ݺ� ��
			count = *pPixels++;		

			// �� �� ���		
			index = 0;
			
			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...
			// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
			//---------------------------------------------
			// xxxx�κб��� check���ִ� ��ƾ
			//---------------------------------------------
			if (count > 0)
			{
				j = count;
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����			
					index += transCount;
					
				
					//---------------------------------------------
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					if (index+colorCount > rectLeft)
					{
						//---------------------------------------------
						// ������������ xxxx������ �Ѿ�� ���
						//---------------------------------------------
						if (index > rectLeft)
						{	
							// �������κ� �ǳʶ�
							pDestTemp += index - rectLeft;

							// �̹� �ܰ�� ��� ���
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
							pDestTemp += colorCount;
							pPixels += colorCount;

							// �������ʹ� ��� ����Ѵ�.
							break;
						}
						//---------------------------------------------
						// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
						// xxxx������ �Ѿ�� �Ǵ� ���
						//---------------------------------------------
						else
						{
							dist = rectLeft - index;

							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, colorCount-dist);					
							pDestTemp += colorCount-dist;
							pPixels += colorCount;

							// �������ʹ� ��� ����Ѵ�.
							break;
						}
					}					

					// ������ �ƴ� ����ŭ index����				
					pPixels += colorCount;
					index += colorCount;
				} while (--j);

				//---------------------------------------------
				// �������ʹ� ��� ����Ѵ�.		
				//---------------------------------------------		
				if (--j > 0)
				{
					do
					{
						transCount = *pPixels++;		// ������ ��			
						colorCount = *pPixels++;		// ���� �ƴ� �� ��			
								
						// ��������ŭ �ǳ� �ڴ�.
						pDestTemp += transCount;			
						
						// �������� �ƴѸ�ŭ ������ش�.
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);

						// memory addr ����
						pDestTemp += colorCount;
						pPixels += colorCount;			
					} while (--j);
				}
			}
		}
		//------------------------------------------
		// Filter�� ����ؾ� �ϴ� ���
		//------------------------------------------
		else
		{
			// ����ҷ��� �ٿ� ���õ� Filter�� ��´�.
			pFilter = s_pFilter->GetFilter( yIndex );
			pFilter += -s_X;
			xIndex = -s_X;

			// (������,�����,�����)�� �ݺ� ��
			count = *pPixels++;		

			// �� �� ���		
			index = 0;
			
			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...
			// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
			//---------------------------------------------
			// xxxx�κб��� check���ִ� ��ƾ
			//---------------------------------------------
			if (count > 0)
			{
				j = count;
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��					
							
					// ��������ŭ index����			
					index += transCount;				
				
					//---------------------------------------------
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					if (index+colorCount > rectLeft)
					{
						//---------------------------------------------
						// ������������ xxxx������ �Ѿ�� ���
						//---------------------------------------------
						if (index > rectLeft)
						{	
							// �������κ� �ǳʶ�
							pDestTemp += index - rectLeft;
							xIndex += transCount;//index - rectLeft;
							pFilter += transCount;//index - rectLeft;

							// �̹� �ܰ�� ��� ���
							//memcpy(pDestTemp, pPixels, colorCount<<1);
							//------------------------------------------
							// Filter�� ����ؾ� �ϴ� ���
							//------------------------------------------
							if (xIndex >= 0)
							{			
								dist2  = s_pFilter->GetWidth() - xIndex;
								if (dist2 > 0)
								{						
									// Filter�� �˳��� ���
									if (dist2 >= colorCount)
									{
										// ������ �ƴ� ������ Surface�� ����Ѵ�.
										memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, colorCount);
									}
									// Filter�� ������ ���
									else
									{
										// dist2��ŭ�� Filter���
										memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist2);
										CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, colorCount-dist2);
									}
								}
								else
								{
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
								}
							}
							// xIndex�� ����(-)�� ���
							else
							{					
								dist2 = s_pFilter->GetWidth() - xIndex ;

								// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
								if (xIndex+colorCount > 0)
								{							
									// Filter�� ������ ���
									if (s_pFilter->GetWidth() < xIndex+colorCount)
									{	
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
										// Filter���
										memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
										
										// - -;;
										CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, colorCount-dist2);
									}
									// Filter�� �˳��� ���
									else
									{							
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
										// dist2��ŭ�� Filter���
										memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
									}						
								}
								else
								{
									// ������ �ƴ� ������ Surface�� ����Ѵ�.
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
								}
							}
							pDestTemp	+= colorCount;
							pPixels		+= colorCount;
							xIndex		+= colorCount;
							pFilter		+= colorCount;


							// �������ʹ� ��� ����Ѵ�.
							break;
						}
						//---------------------------------------------
						// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
						// xxxx������ �Ѿ�� �Ǵ� ���
						//---------------------------------------------
						else
						{	
							dist = rectLeft - index;

							xIndex	+= transCount + dist;
							pFilter	+= transCount + dist;

							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
							//pDestTemp += colorCount-dist;
							//pPixels += colorCount;
							
							//------------------------------------------
							// Filter�� ����ؾ� �ϴ� ���
							//------------------------------------------
							if (xIndex >= 0)
							{			
								dist2  = s_pFilter->GetWidth() - xIndex;
								if (dist2 > 0)
								{						
									// Filter�� �˳��� ���
									if (dist2 >= colorCount-dist)
									{
										// ������ �ƴ� ������ Surface�� ����Ѵ�.
										memcpyAlphaFilterDarkness(pDestTemp, pPixels+dist, pFilter, colorCount-dist);
									}
									// Filter�� ������ ���
									else
									{
										// dist2��ŭ�� Filter���
										memcpyAlphaFilterDarkness(pDestTemp, pPixels+dist, pFilter, dist2);
										CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist+dist2, colorCount-dist - dist2);
									}								
								}
								else
								{
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, colorCount-dist);
								}							
							}
							// xIndex�� ����(-)�� ���
							else
							{					
								dist2 = s_pFilter->GetWidth() - xIndex ;

								// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
								if (xIndex+colorCount-dist > 0)
								{							
									// Filter�� ������ ���
									if (s_pFilter->GetWidth() < xIndex+(colorCount-dist))
									{	
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, -xIndex);
										// Filter���
										memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels+dist-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
										
										// - -;;
										CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist+dist2, colorCount-dist - dist2);
									}
									// Filter�� �˳��� ���
									else
									{							
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, -xIndex);
										// dist2��ŭ�� Filter���
										memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels+dist-xIndex, pFilter-xIndex, (colorCount-dist)+xIndex);
									}						
								}
								else
								{
									// ������ �ƴ� ������ Surface�� ����Ѵ�.
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, colorCount-dist);
								}
							}

							pDestTemp += colorCount-dist;
							pPixels += colorCount;
							xIndex	+= colorCount-dist;
							pFilter += colorCount-dist;
							
							// �������ʹ� ��� ����Ѵ�.
							break;
						}
						
							
					}	
					
					// ������ �κ� ����
					xIndex += transCount;
					pFilter += transCount;

					// ������ �ƴ� ����ŭ index����				
					pPixels += colorCount;
					index	+= colorCount;
					xIndex	+= colorCount;
					pFilter += colorCount;
				} while (--j);
			
				//---------------------------------------------
				// �������ʹ� ��� ����Ѵ�.		
				//---------------------------------------------		
				if (--j > 0)
				{
					do 
					{
						transCount = *pPixels++;		// ������ ��			
						colorCount = *pPixels++;		// ���� �ƴ� �� ��			
								
						// ��������ŭ �ǳ� �ڴ�.
						pDestTemp	+= transCount;			
						xIndex		+= transCount;
						pFilter		+= transCount;

						//------------------------------------------
						// Filter�� ����ؾ� �ϴ� ���
						//------------------------------------------
						if (xIndex >= 0)
						{			
							dist2  = s_pFilter->GetWidth() - xIndex;
							if (dist2 > 0)
							{						
								// Filter�� �˳��� ���
								if (dist2 >= colorCount)
								{
									// ������ �ƴ� ������ Surface�� ����Ѵ�.
									memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, colorCount);
								}
								// Filter�� ������ ���
								else
								{
									// dist2��ŭ�� Filter���
									memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist2);
									CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, colorCount-dist2);
								}
							}
							else
							{
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
							}
						}
						// xIndex�� ����(-)�� ���
						else
						{					
							dist2 = s_pFilter->GetWidth() - xIndex ;

							// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
							if (xIndex+colorCount > 0)
							{							
								// Filter�� ������ ���
								if (s_pFilter->GetWidth() < xIndex+colorCount)
								{	
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
									// Filter���
									memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
									
									// - -;;
									CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, colorCount-dist2);
								}
								// Filter�� �˳��� ���
								else
								{							
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
									// dist2��ŭ�� Filter���
									memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
								}						
							}
							else
							{
								// ������ �ƴ� ������ Surface�� ����Ѵ�.
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
							}
						}

						// memory addr ����
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
	// ��Ӱ� �ϴ� bit�� ����
	s_Value1 = DarkBits;
	CSpriteSurface::s_Value1 = DarkBits;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
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
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;	
		
		//------------------------------------------
		// Filter�� ������� �ʴ� ���
		//------------------------------------------
		if (yIndex < 0 || yIndex >= s_pFilter->GetHeight())
		{
			// (������,�����,�����)�� �ݺ� ��
			count = *pPixels++;		

			// �� �� ���		
			index = 0;
				
			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...		
			// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
			//---------------------------------------------
			// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
			//---------------------------------------------
			if (count > 0)
			{
				j = count;
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����
					index += transCount;
					
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

					//---------------------------------------------
					// ������ ������ �������� ���
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// ������������ �� ����� �ʿ䰡 ���� ��
						if (index > rectRight)
						{
							break;
						}
						// ������ �ƴ� ���� ���� ����ؾ� �� ���
						else
						{
							pDestTemp += transCount;
						
							// ������ �ƴ� ������ Surface�� ����Ѵ�.						
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, rectRight-index);
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					// ���
					CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
					pDestTemp += colorCount;
					pPixels += colorCount;			
					index += colorCount;
				} while (--j);
			}
		}
		//------------------------------------------
		// Filter�� ����ϴ� ���
		//------------------------------------------
		else
		{
			// ����ҷ��� �ٿ� ���õ� Filter�� ��´�.
			pFilter = s_pFilter->GetFilter( yIndex );
			pFilter += -s_X;
			xIndex = -s_X;		

			// (������,�����,�����)�� �ݺ� ��
			count = *pPixels++;		

			// �� �� ���		
			index = 0;
				
			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...		
			// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
			//---------------------------------------------
			// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
			//---------------------------------------------
			if (count > 0)
			{
				j = count;
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����
					index += transCount;
					
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

					//---------------------------------------------
					// ������ ������ �������� ���
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// ������������ �� ����� �ʿ䰡 ���� ��
						if (index > rectRight)
						{
							break;
						}
						// ������ �ƴ� ���� ���� ����ؾ� �� ���
						else
						{
							pDestTemp	+= transCount;
							xIndex		+= transCount;
							pFilter		+= transCount;

							dist		= rectRight - index;
						
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							//memcpy((void*)pDestTemp, (void*)pPixels, (rectRight - index)<<1);
							//------------------------------------------
							// Filter�� ����ؾ� �ϴ� ���
							//------------------------------------------
							if (xIndex >= 0)
							{	
								dist2  = s_pFilter->GetWidth() - xIndex;
								if (dist2 > 0)
								{						
									// Filter�� �˳��� ���
									if (dist2 >= dist)
									{
										// ������ �ƴ� ������ Surface�� ����Ѵ�.
										memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist);
									}
									// Filter�� ������ ���
									else
									{
										// dist2��ŭ�� Filter���
										memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist2);
										CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, dist - dist2);
									}
								}
								else
								{
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, dist);
								}
							}
							// xIndex�� ����(-)�� ���
							else
							{					
								dist2 = s_pFilter->GetWidth() - xIndex ;

								// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
								if (xIndex + dist > 0)
								{							
									// Filter�� ������ ���
									if (s_pFilter->GetWidth() < xIndex+colorCount)
									{	
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
										// Filter���
										memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
										
										// - -;;
										CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, dist - dist2);
									}
									// Filter�� �˳��� ���
									else
									{							
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
										// dist2��ŭ�� Filter���
										memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, dist + xIndex);
									}						
								}
								else
								{
									// ������ �ƴ� ������ Surface�� ����Ѵ�.
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, dist);
								}
							}
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp	+= transCount;
					xIndex		+= transCount;
					pFilter		+= transCount;

					// ���
					//------------------------------------------
					// Filter�� ����ؾ� �ϴ� ���
					//------------------------------------------
					if (xIndex >= 0)
					{			
						dist2  = s_pFilter->GetWidth() - xIndex;
						if (dist2 > 0)
						{						
							// Filter�� �˳��� ���
							if (dist2 >= colorCount)
							{
								// ������ �ƴ� ������ Surface�� ����Ѵ�.
								memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, colorCount);
							}
							// Filter�� ������ ���
							else
							{
								// dist2��ŭ�� Filter���
								memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist2);
								CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, colorCount-dist2);
							}
						}
						else
						{
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						}
					}
					// xIndex�� ����(-)�� ���
					else
					{					
						dist2 = s_pFilter->GetWidth() - xIndex ;

						// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
						if (xIndex+colorCount > 0)
						{							
							// Filter�� ������ ���
							if (s_pFilter->GetWidth() < xIndex+colorCount)
							{	
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
								// Filter���
								memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
								
								// - -;;
								CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, colorCount-dist2);
							}
							// Filter�� �˳��� ���
							else
							{							
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
								// dist2��ŭ�� Filter���
								memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
							}						
						}
						else
						{
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
	// ��Ӱ� �ϴ� bit�� ����
	s_Value1 = DarkBits;
	CSpriteSurface::s_Value1 = DarkBits;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
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
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pDestTemp = pDest;	
		
		//------------------------------------------
		// Filter�� ������� �ʴ� ���
		//------------------------------------------
		if (yIndex < 0 || yIndex >= s_pFilter->GetHeight())
		{
			// (������,�����,�����)�� �ݺ� ��
			count = *pPixels++;		

			// �� �� ���		
			index = 0;
			
			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...
			// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
			//---------------------------------------------
			// xxxx�κб��� check���ִ� ��ƾ
			//---------------------------------------------
			if (count > 0)
			{
				j = count;
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����			
					index += transCount;
					
				
					//---------------------------------------------
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					if (index+colorCount > rectLeft)
					{						
						//---------------------------------------------
						// ������������ xxxx������ �Ѿ�� ���
						//---------------------------------------------
						if (index > rectLeft)
						{	
							// �������κ� �ǳʶ�
							pDestTemp += index - rectLeft;

							// �̹� �ܰ�� ��� ���?
							// ������ ���� �Ѿ�� ���..
							if (index+colorCount > rectRight)
							{							
								// ������������ ������ �� �Ѿ�� ���
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

							// �̹� �ܰ�� ��� ���
							memcpy(pDestTemp, pPixels, colorCount<<1);
							pDestTemp += colorCount;
							pPixels += colorCount;
							index += colorCount;

							// �������ʹ� ��� ����Ѵ�.
							break;
						}
						//---------------------------------------------
						// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
						// xxxx������ �Ѿ�� �Ǵ� ���
						//---------------------------------------------
						else
						{
							dist = rectLeft - index;

							// ������ ���� �Ѿ�� ���..
							if (index+colorCount > rectRight)
							{
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, rectRight - rectLeft);

								j = 0;						
								break;
							}		

							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, colorCount-dist);
							pDestTemp += colorCount-dist;
							pPixels += colorCount;
							index += colorCount;

							// �������ʹ� ��� ����Ѵ�.
							break;
						}
					}					

					// ������ �ƴ� ����ŭ index����				
					pPixels += colorCount;
					index += colorCount;
				} while (--j);

				//---------------------------------------------
				// �� �ٸ��� Clipping�� ����� �ϴµ�...		
				// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
				//---------------------------------------------
				// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
				//---------------------------------------------
				if (--j > 0)
				{
					do
					{
						transCount = *pPixels++;		// ������ ��			
						colorCount = *pPixels++;		// ���� �ƴ� �� ��			
								
						// ��������ŭ index����
						index += transCount;
						
						// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
						// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

						// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

						//---------------------------------------------
						// ������ ������ �������� ���
						//---------------------------------------------			
						if (index+colorCount > rectRight)
						{
							// ������������ �� ����� �ʿ䰡 ���� ��
							if (index > rectRight)
							{
								break;
							}
							// ������ �ƴ� ���� ���� ����ؾ� �� ���
							else
							{
								pDestTemp += transCount;
							
								// ������ �ƴ� ������ Surface�� ����Ѵ�.						
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, rectRight-index);
								break;
							}
						}

						// ��������ŭ �ǳʶ��
						pDestTemp += transCount;

						// ���
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += colorCount;			
						index += colorCount;
					} while (--j);
				}
			}
		}
		//------------------------------------------
		// Filter�� ����ؾ� �ϴ� ���
		//------------------------------------------
		else
		{
			// ����ҷ��� �ٿ� ���õ� Filter�� ��´�.
			pFilter = s_pFilter->GetFilter( yIndex );
			pFilter += -s_X;
			xIndex = -s_X;

			// (������,�����,�����)�� �ݺ� ��
			count = *pPixels++;		

			// �� �� ���		
			index = 0;
			
			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...
			// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
			//---------------------------------------------
			// xxxx�κб��� check���ִ� ��ƾ
			//---------------------------------------------
			if (count > 0)
			{
				j = count;
				do 
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��					
							
					// ��������ŭ index����			
					index += transCount;				
				
					//---------------------------------------------
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					if (index+colorCount > rectLeft)
					{
						//---------------------------------------------
						// ������������ xxxx������ �Ѿ�� ���
						//---------------------------------------------
						if (index > rectLeft)
						{	
							// �������κ� �ǳʶ�
							pDestTemp += index - rectLeft;
							xIndex += transCount;//index - rectLeft;
							pFilter += transCount;//index - rectLeft;

							// �̹� �ܰ�� ��� ���
							//memcpy(pDestTemp, pPixels, colorCount<<1);
							//------------------------------------------
							// Filter�� ����ؾ� �ϴ� ���
							//------------------------------------------
							if (xIndex >= 0)
							{			
								dist2  = s_pFilter->GetWidth() - xIndex;
								if (dist2 > 0)
								{						
									// Filter�� �˳��� ���
									if (dist2 >= colorCount)
									{
										// ������ �ƴ� ������ Surface�� ����Ѵ�.
										memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, colorCount);
									}
									// Filter�� ������ ���
									else
									{
										// dist2��ŭ�� Filter���
										memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist2);
										CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, colorCount-dist2);
									}
								}
								else
								{
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
								}
							}
							// xIndex�� ����(-)�� ���
							else
							{					
								dist2 = s_pFilter->GetWidth() - xIndex ;

								// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
								if (xIndex+colorCount > 0)
								{							
									// Filter�� ������ ���
									if (s_pFilter->GetWidth() < xIndex+colorCount)
									{	
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
										// Filter���
										memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
										
										// - -;;
										CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, colorCount-dist2);
									}
									// Filter�� �˳��� ���
									else
									{							
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
										// dist2��ŭ�� Filter���
										memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
									}						
								}
								else
								{
									// ������ �ƴ� ������ Surface�� ����Ѵ�.
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
								}
							}
							pDestTemp	+= colorCount;
							pPixels		+= colorCount;
							xIndex		+= colorCount;
							pFilter		+= colorCount;
							index		+= colorCount;

							// �������ʹ� ��� ����Ѵ�.
							break;
						}
						//---------------------------------------------
						// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
						// xxxx������ �Ѿ�� �Ǵ� ���
						//---------------------------------------------
						else
						{	
							dist = rectLeft - index;

							xIndex	+= transCount + dist;
							pFilter	+= transCount + dist;

							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
							//pDestTemp += colorCount-dist;
							//pPixels += colorCount;
							
							//------------------------------------------
							// Filter�� ����ؾ� �ϴ� ���
							//------------------------------------------
							if (xIndex >= 0)
							{			
								dist2  = s_pFilter->GetWidth() - xIndex;
								if (dist2 > 0)
								{						
									// Filter�� �˳��� ���
									if (dist2 >= colorCount-dist)
									{
										// ������ �ƴ� ������ Surface�� ����Ѵ�.
										memcpyAlphaFilterDarkness(pDestTemp, pPixels+dist, pFilter, colorCount-dist);
									}
									// Filter�� ������ ���
									else
									{
										// dist2��ŭ�� Filter���
										memcpyAlphaFilterDarkness(pDestTemp, pPixels+dist, pFilter, dist2);
										CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist+dist2, colorCount-dist - dist2);
									}								
								}
								else
								{
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, colorCount-dist);
								}							
							}
							// xIndex�� ����(-)�� ���
							else
							{					
								dist2 = s_pFilter->GetWidth() - xIndex ;

								// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
								if (xIndex+colorCount-dist > 0)
								{							
									// Filter�� ������ ���
									if (s_pFilter->GetWidth() < xIndex+(colorCount-dist))
									{	
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, -xIndex);
										// Filter���
										memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels+dist-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
										
										// - -;;
										CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist+dist2, colorCount-dist - dist2);
									}
									// Filter�� �˳��� ���
									else
									{							
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, -xIndex);
										// dist2��ŭ�� Filter���
										memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels+dist-xIndex, pFilter-xIndex, (colorCount-dist)+xIndex);
									}						
								}
								else
								{
									// ������ �ƴ� ������ Surface�� ����Ѵ�.
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, colorCount-dist);
								}
							}

							pDestTemp += colorCount-dist;
							pPixels		+= colorCount;
							xIndex		+= colorCount-dist;
							pFilter		+= colorCount-dist;
							index		+= colorCount;
							
							// �������ʹ� ��� ����Ѵ�.
							break;
						}
					}	
					
					// ������ �κ� ����
					xIndex += transCount;
					pFilter += transCount;

					// ������ �ƴ� ����ŭ index����				
					pPixels += colorCount;
					index	+= colorCount;
					xIndex	+= colorCount;
					pFilter += colorCount;
				} while (--j);
			
				//---------------------------------------------
				// �� �ٸ��� Clipping�� ����� �ϴµ�...		
				// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
				//---------------------------------------------
				// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
				//---------------------------------------------
				if (--j > 0)
				{
					do
					{
						transCount = *pPixels++;		// ������ ��			
						colorCount = *pPixels++;		// ���� �ƴ� �� ��			
								
						// ��������ŭ index����
						index += transCount;
						
						// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
						// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

						// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

						//---------------------------------------------
						// ������ ������ �������� ���
						//---------------------------------------------			
						if (index+colorCount > rectRight)
						{
							// ������������ �� ����� �ʿ䰡 ���� ��
							if (index > rectRight)
							{
								break;
							}
							// ������ �ƴ� ���� ���� ����ؾ� �� ���
							else
							{
								pDestTemp	+= transCount;
								xIndex		+= transCount;
								pFilter		+= transCount;

								dist		= rectRight - index;
							
								// ������ �ƴ� ������ Surface�� ����Ѵ�.
								//memcpy((void*)pDestTemp, (void*)pPixels, (rectRight - index)<<1);
								//------------------------------------------
								// Filter�� ����ؾ� �ϴ� ���
								//------------------------------------------
								if (xIndex >= 0)
								{	
									dist2  = s_pFilter->GetWidth() - xIndex;
									if (dist2 > 0)
									{						
										// Filter�� �˳��� ���
										if (dist2 >= dist)
										{
											// ������ �ƴ� ������ Surface�� ����Ѵ�.
											memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist);
										}
										// Filter�� ������ ���
										else
										{
											// dist2��ŭ�� Filter���
											memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist2);
											CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, dist - dist2);
										}
									}
									else
									{
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, dist);
									}
								}
								// xIndex�� ����(-)�� ���
								else
								{					
									dist2 = s_pFilter->GetWidth() - xIndex ;

									// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
									if (xIndex + dist > 0)
									{							
										// Filter�� ������ ���
										if (s_pFilter->GetWidth() < xIndex+colorCount)
										{	
											CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
											// Filter���
											memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
											
											// - -;;
											CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, dist - dist2);
										}
										// Filter�� �˳��� ���
										else
										{							
											CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
											// dist2��ŭ�� Filter���
											memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, dist + xIndex);
										}						
									}
									else
									{
										// ������ �ƴ� ������ Surface�� ����Ѵ�.
										CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, dist);
									}
								}
								break;
							}
						}

						// ��������ŭ �ǳʶ��
						pDestTemp	+= transCount;
						xIndex		+= transCount;
						pFilter		+= transCount;

						// ���
						//------------------------------------------
						// Filter�� ����ؾ� �ϴ� ���
						//------------------------------------------
						if (xIndex >= 0)
						{			
							dist2  = s_pFilter->GetWidth() - xIndex;
							if (dist2 > 0)
							{						
								// Filter�� �˳��� ���
								if (dist2 >= colorCount)
								{
									// ������ �ƴ� ������ Surface�� ����Ѵ�.
									memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, colorCount);
								}
								// Filter�� ������ ���
								else
								{
									// dist2��ŭ�� Filter���
									memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist2);
									CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, colorCount-dist2);
								}
							}
							else
							{
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
							}
						}
						// xIndex�� ����(-)�� ���
						else
						{					
							dist2 = s_pFilter->GetWidth() - xIndex ;

							// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
							if (xIndex+colorCount > 0)
							{							
								// Filter�� ������ ���
								if (s_pFilter->GetWidth() < xIndex+colorCount)
								{	
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
									// Filter���
									memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
									
									// - -;;
									CSpriteSurface::memcpyDarkness(pDestTemp+dist2, pPixels+dist2, colorCount-dist2);
								}
								// Filter�� �˳��� ���
								else
								{							
									CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
									// dist2��ŭ�� Filter���
									memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
								}						
							}
							else
							{
								// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// pRect->top, pRect->bottom��ŭ�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltAlphaFilterDarknessClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	// ��Ӱ� �ϴ� bit�� ����
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
	for (int i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		//------------------------------------------
		// Filter�� ������� �ʴ� ���
		//------------------------------------------
		if (yIndex < 0 || yIndex >= s_pFilter->GetHeight())
		{
			// (������,�����,�����)�� �ݺ� ��		
			count	= *pPixels++;		

			// �� �� ���
			if (count > 0)
			{
				j = count;
				do 
				{				
					pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
					colorCount = *pPixels++;		// ���� �ƴ� �� ��				

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
					
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;
				} while (--j);
			}
		}
		//------------------------------------------
		// Filter�� ����ؾ� �ϴ� ���
		//------------------------------------------
		else
		{
			// ����ҷ��� �ٿ� ���õ� Filter�� ��´�.
			pFilter = s_pFilter->GetFilter( yIndex );
			pFilter += -s_X;
			xIndex = -s_X;		// xIndex�� x�� �̻��� ��� ����ϸ� �ȴ�.

			// (������,�����,�����)�� �ݺ� ��		
			count	= *pPixels++;		
 			// �� �� ���
			if (count > 0)
			{
				j = count;
				do 
				{
					pDestTemp	+= *pPixels;			// ��������ŭ �ǳ� �ڴ�.
					xIndex		+= *pPixels;
					pFilter		+= *pPixels;
					pPixels ++;
					colorCount = *pPixels++;			// ���� �ƴ� �� ��				

					//------------------------------------------
					// Filter�� ����ؾ� �ϴ� ���
					//------------------------------------------
					if (xIndex >= 0)
					{			
						dist  = s_pFilter->GetWidth() - xIndex;
						if (dist > 0)
						{						
							// Filter�� �˳��� ���
							if (dist >= colorCount)
							{
								// ������ �ƴ� ������ Surface�� ����Ѵ�.
								memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, colorCount);
							}
							// Filter�� ������ ���
							else
							{
								// dist��ŭ�� Filter���
								memcpyAlphaFilterDarkness(pDestTemp, pPixels, pFilter, dist);
								CSpriteSurface::memcpyDarkness(pDestTemp+dist, pPixels+dist, colorCount-dist);
							}
						}
						else
						{
							CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						}
					}
					// xIndex�� ����(-)�� ���
					else
					{					
						dist = s_pFilter->GetWidth() - xIndex ;

						// �� ���� �ǳʼ�.. Filter�� ����ؾ� �ϴ� ���
						if (xIndex+colorCount > 0)
						{							
							// Filter�� ������ ���
							if (s_pFilter->GetWidth() < xIndex+colorCount)
							{	
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
								// Filter���
								memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, s_pFilter->GetWidth());
								
								// - -;;
								CSpriteSurface::memcpyDarkness(pDestTemp+dist, pPixels+dist, colorCount-dist);
							}
							// Filter�� �˳��� ���
							else
							{							
								CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, -xIndex);
								// dist��ŭ�� Filter���
								memcpyAlphaFilterDarkness(pDestTemp-xIndex, pPixels-xIndex, pFilter-xIndex, colorCount+xIndex);
							}						
						}
						else
						{
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// pFilter�� �̿��ؼ� ����Ѵ�.
// pFilter�� alpha ������ �̿��Ѵ�.
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
	// ������ ���
	while (j--)
	{			
		alpha = *pFilter;		

		// ���� ���
		///*
		sTemp = *pSource;
		dTemp = *pDest;

		sr = ColorDraw::Red(sTemp);
		sg = ColorDraw::Green(sTemp);
		sb = ColorDraw::Blue(sTemp);

		dr = ColorDraw::Red(dTemp);
		dg = ColorDraw::Green(dTemp);
		db = ColorDraw::Blue(dTemp);		
		
		*pDest = ((alpha * (sb - db) >> 5) + db |
					((alpha * (sg - dg) >> 5) + dg) << ColorDraw::s_bSHIFT_G |
					((alpha * (sr - dr) >> 5) + dr) << ColorDraw::s_bSHIFT_R);

		*pDest = (*pDest >> s_Value1) & ColorDraw::s_wMASK_SHIFT[s_Value1];
		//*/

		//if (alpha > 26) 
		{
			//*pDest = *pSource; 
		//	*pDest = (*pSource >> s_Value1) & ColorDraw::s_wMASK_SHIFT[s_Value1];
		}


		pDest++;
		pSource++;
		pFilter++;
	}
}


//----------------------------------------------------------------------
// BltDarkerFilter
//----------------------------------------------------------------------
// Clipping���� �ʴ´�.
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

			// (������,�����,�����)�� �ݺ� ��		
			count	= *pPixels++;		

			// �� �� ���
			if (count > 0)
			{
				j = count;
				do 
				{
					pDestTemp	+= *pPixels;		// ��������ŭ �ǳ� �ڴ�.
					pFilter		+= *pPixels;
					pPixels++;
					colorCount = *pPixels++;		// ���� �ƴ� �� ��				

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltDarkerFilterClipLeft(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;
	BYTE	*pFilter;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pFilter		= s_pFilter->GetFilter( i );
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
		
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;			
			
				//---------------------------------------------
				// xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp	+= index - rectLeft;
						pFilter		+= index - rectLeft;

						// �̹� �ܰ�� ��� ���
						memcpyDarkerFilter(pDestTemp, pPixels, pFilter, colorCount);
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;
						pFilter		+= colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						memcpyDarkerFilter(pDestTemp, pPixels+dist, pFilter+dist, colorCount-dist);
						pDestTemp += colorCount-dist;
						pPixels += colorCount;
						pFilter	+= colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += colorCount;
				index += colorCount;
			} while (--j);

			//---------------------------------------------
			// �������ʹ� ��� ����Ѵ�.		
			//---------------------------------------------		
			if (--j > 0)
			{
				do
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��
							
					// ��������ŭ �ǳ� �ڴ�.			
					pDestTemp	+= transCount;
					pFilter		+= transCount;
					
					// �������� �ƴѸ�ŭ ������ش�.
					memcpyDarkerFilter(pDestTemp, pPixels, pFilter, colorCount);

					// memory addr ����
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
// ������ clipping.  
// pRect->right�� ������ ���� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltDarkerFilterClipRight(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;
	BYTE	*pFilter;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
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
		pPixels = m_Pixels[i];
		pFilter		= s_pFilter->GetFilter( i );
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
			
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...		
		// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
		//---------------------------------------------
		// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����
				index += transCount;
				
				// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
				// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

				// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

				//---------------------------------------------
				// ������ ������ �������� ���
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// ������������ �� ����� �ʿ䰡 ���� ��
					if (index > rectRight)
					{
						break;
					}
					// ������ �ƴ� ���� ���� ����ؾ� �� ���
					else
					{
						pDestTemp	+= transCount;
						pFilter		+= transCount;
					
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						memcpyDarkerFilter(pDestTemp, pPixels, pFilter, rectRight - index);
						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp	+= transCount;
				pFilter		+= transCount;

				// ���
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltDarkerFilterClipWidth(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;
	BYTE	*pFilter;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int		count,
			transCount, 
			colorCount,
			index,
			dist;

	register int i;
	register int j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;
	for (int i=pRect->top; i<rectBottom; i++)
	{
		pPixels = m_Pixels[i];
		pFilter		= s_pFilter->GetFilter( i );
		pDestTemp = pDest;		

		// (������,�����,�����)�� �ݺ� ��
		count = *pPixels++;		

		// �� �� ���		
		index = 0;
		
		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do 
			{
				transCount = *pPixels++;		// ������ ��			
				colorCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;
				
			
				//---------------------------------------------
				// xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp	+= index - rectLeft;
						pFilter		+= index - rectLeft;

						// �̹� �ܰ�� ��� ���?
						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > rectRight)
						{							
							// ������������ ������ �� �Ѿ�� ���
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

						// �̹� �ܰ�� ��� ���
						memcpyDarkerFilter(pDestTemp, pPixels, pFilter, colorCount);
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;
						pFilter		+= colorCount;
						index		+= colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = rectLeft - index;

						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > rectRight)
						{
							memcpyDarkerFilter(pDestTemp, pPixels+dist, pFilter+dist, rectRight - rectLeft);

							j = 0;						
							break;
						}		

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						memcpyDarkerFilter(pDestTemp, pPixels+dist, pFilter+dist, colorCount-dist);
						pDestTemp	+= colorCount-dist;
						pPixels		+= colorCount;
						pFilter		+= colorCount;
						index		+= colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += colorCount;
				pFilter	+= colorCount;
				index += colorCount;
			} while (--j);

			//---------------------------------------------
			// �� �ٸ��� Clipping�� ����� �ϴµ�...		
			// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
			//---------------------------------------------
			// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
			//---------------------------------------------
			if (--j > 0)
			{
				do
				{
					transCount = *pPixels++;		// ������ ��			
					colorCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����
					index += transCount;
					
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

					//---------------------------------------------
					// ������ ������ �������� ���
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// ������������ �� ����� �ʿ䰡 ���� ��
						if (index > rectRight)
						{
							break;
						}
						// ������ �ƴ� ���� ���� ����ؾ� �� ���
						else
						{
							pDestTemp	+= transCount;
							pFilter		+= transCount;
						
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							memcpyDarkerFilter(pDestTemp, pPixels, pFilter, rectRight - index);
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp	+= transCount;
					pFilter		+= transCount;

					// ���
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
// pRect->top, pRect->bottom��ŭ�� ����Ѵ�.
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
	for (int i=pRect->top; i<rectBottom; i++)
	{			
		pPixels		= m_Pixels[i];
		pFilter		= s_pFilter->GetFilter( i );
		pDestTemp	= pDest;

		// (������,�����,�����)�� �ݺ� ��		
		count	= *pPixels++;		

		// �� �� ���
		if (count > 0)
		{
			j = count;
			do 
			{				
				pDestTemp	+= *pPixels;		// ��������ŭ �ǳ� �ڴ�.
				pFilter		+= *pPixels;
				pPixels++;
				colorCount = *pPixels++;		// ���� �ƴ� �� ��				

				// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// Clipping���� �ʴ´�.
//----------------------------------------------------------------------
void
CSprite::BltAlpha4444NotTrans(WORD *pDest, WORD pitch, BYTE alpha)
{
	s_Value1 = alpha >> 1;	// 4 bit�̹Ƿ�

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

			// (������,�����,�����)�� �ݺ� ��		
			count	= *pPixels++;		

			// �� �� ���
			if (count > 0)
			{			
				j = count;
				do
				{		
					transCount = *pPixels++;					
					colorCount = *pPixels++;		// ���� �ƴ� �� ��				

					// 0�� ����Ѵ�.
					memset(pDestTemp, 0, transCount<<1);
					pDestTemp += transCount;		// ��������ŭ �ǳ� �ڴ�.

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// Alpha�� : 1~32
//----------------------------------------------------------------------
// pSource�� ���� pDest�� ����� �ؾ��Ѵ�.
// pSource�� ������ (alpha,���� �ϳ�)�� pixels��ŭ �ݺ��̴�.
//
// A:R:G:B = 4:4:4:4 Texture�� ���� ���̴�.
//----------------------------------------------------------------------
void	
CSprite::memcpyAlpha4444(WORD* pDest, WORD* pSource, WORD pixels)
{
	register WORD		sTemp;
	register int		sr,sg,sb;
	//static WORD		temp, temp2;

	register int i = pixels;

	// Alpha Channel Blending
	// ������ ���
	while (i--)
	{	
		// ���� ���
		sTemp = *pSource;
	
		sr = (sTemp >> ColorDraw::s_bSHIFT4_R);// & 0x0F;
		sg = (sTemp >> ColorDraw::s_bSHIFT4_G) & 0x0F;
		sb = (sTemp >> ColorDraw::s_bSHIFT4_B) & 0x0F;
		
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
// ����ؼ� ���.. 
// Clipping���� �ʴ´�.
//
// alpha���� 50%(������)���� �Ѵ�.
//----------------------------------------------------------------------
void
CSprite::BltAlpha4444SmallNotTrans(WORD *pDest, WORD pitch, BYTE alpha, BYTE shift)
{
	s_Value1 = alpha >> 1;	// 4 bit�̹Ƿ�
	s_Value2 = shift;
	// memcpy���� �ǳʶ�� ��
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
		int stepY = 1 << shift;		// y�� �ǳʶ�� pixel��
		pDest = (WORD*)((BYTE*)pDest + (i>>shift)*pitch);

		do
		{
			pPixels		= m_Pixels[i];
			pDestTemp	= pDest;

			// (������,�����,�����)�� �ݺ� ��		
			count	= *pPixels++;		

			// �� �� ���
			if (count > 0)
			{			
				j = count;
				do
				{		
					transCount = *pPixels++;					
					colorCount = *pPixels++;		// ���� �ƴ� �� ��	

					// shift��ŭ �ٿ��� ���� ����Ѵ�.
					transCountShift = transCount >> shift;
					colorCountShift = colorCount >> shift;

					// 0�� ����Ѵ�.
					memset(pDestTemp, 0, transCountShift<<1);
					pDestTemp += transCountShift;		// ��������ŭ �ǳ� �ڴ�.

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// Alpha�� : 1~32
//----------------------------------------------------------------------
// pSource�� ���� pDest�� ����� �ؾ��Ѵ�.
// pSource�� ������ (alpha,���� �ϳ�)�� pixels��ŭ �ݺ��̴�.
//
// A:R:G:B = 4:4:4:4 Texture�� ���� ���̴�.
//
// s_Value1�� alpha��
// s_Value2�� shift��
// s_Value3�� �ǳʶ�� ��(�³�? ����� ��������. - -;;)
//----------------------------------------------------------------------
void	
CSprite::memcpyAlpha4444Small(WORD* pDest, WORD* pSource, WORD pixels)
{
	register WORD		sTemp;
	register int		sr,sg,sb;
	//static WORD		temp, temp2;

	register int i = pixels >> s_Value2;

	// �� ������ Alpha Blending
	// ������ ���
	while (i--)
	{	
		// ���� ���
		sTemp = *pSource;
	
		sr = (sTemp >> ColorDraw::s_bSHIFT4_R);// & 0x0F;
		sg = (sTemp >> ColorDraw::s_bSHIFT4_G) & 0x0F;
		sb = (sTemp >> ColorDraw::s_bSHIFT4_B) & 0x0F;
		
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
// Clipping���� �ʴ´�.
//----------------------------------------------------------------------
void
CSprite::Blt1555NotTrans(WORD *pDest, WORD pitch)
{
	//s_Value1 = alpha >> 4;	// 1 bit�̹Ƿ�

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

			// (������,�����,�����)�� �ݺ� ��		
			count	= *pPixels++;		

			// �� �� ���
			if (count > 0)
			{			
				j = count;
				do
				{		
					transCount = *pPixels++;					
					colorCount = *pPixels++;		// ���� �ƴ� �� ��				

					// 0�� ����Ѵ�.
					memset(pDestTemp, 0, transCount<<1);
					pDestTemp += transCount;		// ��������ŭ �ǳ� �ڴ�.

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// Alpha�� : �ǹ̾���.
//----------------------------------------------------------------------
// pSource�� ���� pDest�� ����� �ؾ��Ѵ�.
// pSource�� ������ (alpha,���� �ϳ�)�� pixels��ŭ �ݺ��̴�.
//
// A:R:G:B = 1:5:5:5 Texture�� ���� ���̴�.
//----------------------------------------------------------------------
void	
CSprite::memcpy1555(WORD* pDest, WORD* pSource, WORD pixels)
{
	register int		sTemp;
	register int		sr,sg,sb;
	//static WORD		temp, temp2;

	register int i = pixels;

	// Alpha Channel Blending
	// ������ ���
	while (i--)
	{	
		// ���� ���
		sTemp = *pSource;
	
		sr = (sTemp >> ColorDraw::s_bSHIFT_R);// & 0x0F;
		sg = (sTemp >> ColorDraw::s_bSHIFT_G) & 0x0000001F;
		sb = (sTemp >> ColorDraw::s_bSHIFT_B) & 0x0000001F;
		
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
// ����ؼ� ���.. 
// Clipping���� �ʴ´�.
//
// alpha���� 50%(������)���� �Ѵ�.
//----------------------------------------------------------------------
void
CSprite::Blt1555SmallNotTrans(WORD *pDest, WORD pitch, BYTE shift)
{
	//s_Value1 = alpha >> 4;	// 1 bit�̹Ƿ�
	s_Value2 = shift;
	// memcpy���� �ǳʶ�� ��
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
		int stepY = 1 << shift;		// y�� �ǳʶ�� pixel��
		pDest = (WORD*)((BYTE*)pDest + (i>>shift)*pitch);

		do
		{
			pPixels		= m_Pixels[i];
			pDestTemp	= pDest;

			// (������,�����,�����)�� �ݺ� ��		
			count	= *pPixels++;		

			// �� �� ���
			if (count > 0)
			{			
				j = count;
				do
				{		
					transCount = *pPixels++;					
					colorCount = *pPixels++;		// ���� �ƴ� �� ��	

					// shift��ŭ �ٿ��� ���� ����Ѵ�.
					transCountShift = transCount >> shift;
					colorCountShift = colorCount >> shift;

					// 0�� ����Ѵ�.
					memset(pDestTemp, 0, transCountShift<<1);
					pDestTemp += transCountShift;		// ��������ŭ �ǳ� �ڴ�.

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// Alpha�� : �ǹ̾��� - -;
//----------------------------------------------------------------------
// pSource�� ���� pDest�� ����� �ؾ��Ѵ�.
// pSource�� ������ (alpha,���� �ϳ�)�� pixels��ŭ �ݺ��̴�.
//
// A:R:G:B = 1:5:5:5 Texture�� ���� ���̴�.
//
// s_Value2�� shift��
// s_Value3�� �ǳʶ�� ��(�³�? ����� ��������. - -;;)
//----------------------------------------------------------------------
void	
CSprite::memcpy1555Small(WORD* pDest, WORD* pSource, WORD pixels)
{
	register int		sTemp;
	register int		sr,sg,sb;
	//static WORD		temp, temp2;

	register int i = pixels >> s_Value2;

	// ������ ���
	while (i--)
	{	
		// ���� ���
		sTemp = *pSource;
	
		sr = (sTemp >> ColorDraw::s_bSHIFT_R);// & 0x0F;
		sg = (sTemp >> ColorDraw::s_bSHIFT_G) & 0x0000001F;
		sb = (sTemp >> ColorDraw::s_bSHIFT_B) & 0x0000001F;
		
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

	// width�� height�� �����Ѵ�.
	fileSize += 2;
	fileSize += 2;
	
	// NULL�̸� �������� �ʴ´�. ���̸� ����Ǵ� ���̴�.
	if (m_Pixels==NULL || m_Width==0 || m_Height==0)
		return 0;
	
	// ���� �� �� ����
	WORD index;	
	
	register int i;
	register int j;
	
	//--------------------------------
	// 5:6:5
	//--------------------------------
	for (int i=0; i<m_Height; i++)
	{
		// �ݺ� ȸ���� 2 byte
		int	count = m_Pixels[i][0], 
			colorCount;
		index	= 1;
		
		// �� line���� byte���� ��� �����ؾ��Ѵ�.
		for (j=0; j<count; j++)
		{
			//transCount = m_Pixels[i][index];
			colorCount = m_Pixels[i][index+1];
			
			index+=2;	// �� count ��ŭ
			
			index += colorCount;	// ������ �ƴѰ͸�ŭ +				
		}
		
		// byte���� ���� data�� �����Ѵ�.
		fileSize += 2;
		fileSize += index<<1;
	}

	return fileSize;
}