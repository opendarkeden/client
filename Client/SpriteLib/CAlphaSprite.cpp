//----------------------------------------------------------------------
// CAlphaSprite.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "CSpriteSurface.h"
#include "CAlphaSprite.h"

#ifdef SPRITELIB_BACKEND_SDL
#include "SpriteLibBackend.h"
#endif


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

#ifdef SPRITELIB_BACKEND_SDL
	m_backend_sprite = SPRITECTL_INVALID_SPRITE;
	m_backend_dirty = false;
#endif
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
// m_Pixels�� memory�� �����Ѵ�.
//----------------------------------------------------------------------
void	
CAlphaSprite::Release()
{
#ifdef SPRITELIB_BACKEND_SDL
	if (m_backend_sprite != SPRITECTL_INVALID_SPRITE) {
		spritectl_destroy_sprite(m_backend_sprite);
		m_backend_sprite = SPRITECTL_INVALID_SPRITE;
		m_backend_dirty = false;
	}
#endif
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

	for (i=0; i<m_Height; i++)
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

			index += (colorCount<<1);	// ������ �ƴѰ͸�ŭ +				
		}

		// �޸� ���
		m_Pixels[i] = new WORD [index];
		memcpy(m_Pixels[i], Sprite.m_Pixels[i], index<<1);
	}

	// ���� �Ϸ�
	m_bInit = true;
}

//----------------------------------------------------------------------
// CDirectDrawSurface�� (x,y)+(width, height)������ �о m_Pixels�� �����Ѵ�.
//----------------------------------------------------------------------
// m_Pixels�� 0�� ���� Format���� �ٲ۴�.
//
// �� line���� ������ ���� ������ ������.
//
// [�ݺ���] (������,�����,(alpha,����)(alpha,����)....)(������,�����,(alpha,����)(alpha,����)....)........
//
// �ݺ����� 2 bytes�̰�
// �������� ������� ���� 2 byte�̰�
// ������� ���� 2 bytes���̴�.
//
// alpha���� 2byte�ε�
// ���� 1byte�� ���� alpha��
// ���� 1byte�� 32-alpha��
// (�̷��� �ϴ� ������... 2byte�� ���ߴµ� 1byte�� ���ٺ���.. *_*)
//----------------------------------------------------------------------
// Source���� ������ ������ �ϰ�
// Filter�� Alpha������ �ؼ� �Բ� �����Ѵ�. 
//----------------------------------------------------------------------
void
CAlphaSprite::SetPixel(WORD *pSource, WORD sourcePitch, 
						WORD *pFilter, WORD filterPitch, 
						WORD width, WORD height)
{
	// memory����
	Release();

	m_Width = width;
	m_Height = height;

	// �ϴ� memory�� ������ ��Ƶд�.	
	WORD*	data = new WORD[m_Width*4+10];

	int	index,				// data�� index�� ���
			lastColorIndex;		// ������ �ƴѻ� ������ �ֱ� index
	int	count;				// �ݺ���
	int	trans,				// ������ ����
			color;				// ������ �ƴѻ� ����

	BOOL	bCheckTrans;		// �ֱٿ� �˻��Ѱ� �������ΰ�?

	WORD	*pSourceTemp, *pFilterTemp;

	// height�� ��ŭ memory���
	m_Pixels = new WORD* [height];

	for (register int i=0; i<height; i++)
	{
		index = 0;
		count = 0;
		trans = 0;
		color = 0;
		bCheckTrans = TRUE;

		pSourceTemp = pSource;
		pFilterTemp = pFilter;

		// �� line�� ���ؼ� ����~
		for (register int j=0; j<width; j++)
		{
			// 0�� color�� ���ؼ� ����			
			if (*pSourceTemp==s_Colorkey)
				// alpha���� 0�� ���� ���������� ���� �ɰ� ���Ҵµ�.. �Ҿ��ؼ�.. -_-;
				//|| (*pFilterTemp & 0x001F)==0)	// 2002.3.26 �߰�
			{
				// �ֱٿ� �˻��Ѱ� �������� �ƴϾ��ٸ�
				if (!bCheckTrans)
				{
					// ' (����,�����,�����) '�� �� set�� �������� �ǹ��ϹǷ�
					// ������� (alpha,�����)�� ������ �ǹ��Ѵ�.
					count++;
					
					// color���� ����
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

				// alpha�� �����
				BYTE alpha;
				WORD alpha2;				
				alpha = (BYTE)(*pFilterTemp & 0x001F);	// Alpha��(Blue���� ���Ѵ�.)
				alpha2 = (alpha << 8) | (32-alpha);	// (Alpha:32-Alpha)

				// ����
				data[index++] = alpha2;					// Alpha ���� �����Ѵ�.				
				data[index++] = *pSourceTemp;			// ���� ������ �����Ѵ�.

				color++;								
			}

			pSourceTemp++;
			pFilterTemp++;
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
// Sprite�ȿ��� (x,y)�� ������ �ִ°�?(�������� �ƴ� ���)
//----------------------------------------------------------------------
bool		
CAlphaSprite::IsColorPixel(short x, short y)
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

		// �� �� ���
		if (count > 0)
		{			
			register int i = count;
			do
			{
				transCount = *pPixels++;
				colorCount = *pPixels++;

				index += transCount;

				// �̹� loop�ȿ� �����ϴ� ��
				if (x < index+colorCount)
				{
					// �������������� ���� ���
					if (x < index)
					{
						int n = index - x;

						// [pixel + alpha] * n ��ŭ ���� ���� �д´�.
						pPixels += n<<1;

						// alpha���� �д´�.
						if ((*pPixels >> 8)!= 0)
						{
							return true;
						}

						return false;
					}

					// ���� ���Ѵ�.
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
// Sprite�ȿ��� (x,y)�� ������ ��´�.(�������� �ƴ� ���)
//----------------------------------------------------------------------
WORD		
CAlphaSprite::GetPixel(int x, int y, int bColor) const
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

		// �� �� ���
		if (count > 0)
		{			
			register int i = count;
			do
			{
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
					// AlphaChannel���� �ֱ� ������...
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
// Alpha�� : 1~32
//----------------------------------------------------------------------
// pSource�� ���� pDest�� ����� �ؾ��Ѵ�.
// pSource�� ������ (alpha,���� �ϳ�)�� pixels��ŭ �ݺ��̴�.
//----------------------------------------------------------------------
void	
CAlphaSprite::memcpyAlpha(WORD* pDest, WORD* pSource, WORD pixels)
{
	register WORD		sTemp,dTemp;
	register int		sr,sg,sb,dr,dg,db;
	//static WORD		temp, temp2;

	register int i = pixels;

	BYTE alpha;

	// Alpha Channel Blending
	// ������ ���
	while (i--)
	{	
		// Source���� Alpha���� ���ԵǾ� �ִ�.
		alpha = *pSource >> 8;
		pSource++;

		// ���� ���
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
		// ��... �̰� �� ������.. �� �׷���.. - -;;;
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
// Alpha�� : 1~32
//----------------------------------------------------------------------
// pSource�� ���� pDest�� ����� �ؾ��Ѵ�.
// pSource�� ������ (alpha,���� �ϳ�)�� pixels��ŭ �ݺ��̴�.
//
// A:R:G:B = 4:4:4:4 Texture�� ���� ���̴�.
//----------------------------------------------------------------------
void	
CAlphaSprite::memcpyAlpha4444(WORD* pDest, WORD* pSource, WORD pixels)
{
	register WORD		sTemp;
	register int		sr,sg,sb;
	//static WORD		temp, temp2;

	register int i = pixels;

	BYTE alpha;

	// Alpha Channel Blending
	// ������ ���
	while (i--)
	{	
		// Source���� Alpha���� ���ԵǾ� �ִ�.
		alpha = *pSource >> 9;	//	alpha = (*pSource >> 8) >> 1;
		pSource++;

		// ���� ���
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
// pRect�� ������ ����Ѵ�.
//----------------------------------------------------------------------
void
CAlphaSprite::BltClip(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------	
	// ù �� (x,y)
	//--------------------------------------------
	pDest = (WORD*)((BYTE*)pDest + pitch*pRect->top + pRect->left);
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

	for (i=pRect->top; i<rectBottom; i++)
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
						memcpyAlpha(pDestTemp, pPixels, pRect->right - index);
						break;
					}						

					// ��� ���
					memcpyAlpha(pDestTemp, pPixels, colorCount);
					pDestTemp += colorCount;
				}				
				// ����ϸ� �� �� ���(���� ���ʺκ�)���� ����ص� �Ǵ��� Ȯ���غ���.
				else
				{
					// ������������ ������ �Ѿ���Ƿ� ��� ���
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

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						memcpyAlpha(pDestTemp, pPixels+dist, colorCount-dist);
						pDestTemp += colorCount-dist;

						// �������ʹ� ��� ����Ѵ�.
						bPut = TRUE;
					}
				}				

				// ������ �ƴ� ����ŭ index����				
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
// Clipping���� �ʴ´�.
//----------------------------------------------------------------------
void
CAlphaSprite::Blt(WORD *pDest, WORD pitch)
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
				do
				{
					pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
					colorCount = *pPixels++;		// ���� �ƴ� �� ��				

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					memcpyAlpha(pDestTemp, pPixels, colorCount);
					
					pDestTemp	+= colorCount;

					// ���� �� ������ alpha�� ������ 2���̴�.
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
// ���� clipping.  
// rectLeft���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CAlphaSprite::BltClipLeft(WORD* pDest, WORD pitch, RECT* pRect)
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

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	for (i=pRect->top; i<rectBottom; i++)
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
		// �� �� ���
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
						memcpyAlpha(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += (colorCount<<1);

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
						memcpyAlpha(pDestTemp, pPixels+(dist<<1), colorCount-dist);
						pDestTemp += colorCount-dist;
						pPixels += (colorCount<<1);

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += (colorCount<<1);
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
					memcpyAlpha(pDestTemp, pPixels, colorCount);

					// memory addr ����
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
// ������ clipping.  
// rectRight�� ������ ���� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CAlphaSprite::BltClipRight(WORD* pDest, WORD pitch, RECT* pRect)
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

	for (i=pRect->top; i<rectBottom; i++)
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
		// �� �� ���
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
						memcpyAlpha(pDestTemp, pPixels, rectRight - index);
						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				// ���
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
// ���� clipping.  
// rectLeft���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
// rectRight����..
//----------------------------------------------------------------------
void
CAlphaSprite::BltClipWidth(WORD* pDest, WORD pitch, RECT* pRect)
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

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	for (i=pRect->top; i<rectBottom; i++)
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
		// �� �� ���
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
								memcpyAlpha(pDestTemp, pPixels, (rectRight - index));
							}

							j = 0;
							
							break;
						}
						memcpyAlpha(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += (colorCount<<1);
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

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						// ������ ���� �Ѿ�� ���..
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

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += (colorCount<<1);
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
							memcpyAlpha(pDestTemp, pPixels, rectRight - index);
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					// ���
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
// pRect->top, rectBottom��ŭ�� ����Ѵ�.
//----------------------------------------------------------------------
void
CAlphaSprite::BltClipHeight(WORD *pDest, WORD pitch, RECT* pRect)
{
	int	count,			
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

		// (������,�����,�����)�� �ݺ� ��		
		count	= *pPixels++;		

		// �� �� ���
		// �� �� ���
		if (count > 0)
		{			
			j = count;
			do
			{
				pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
				colorCount = *pPixels++;		// ���� �ƴ� �� ��				

				// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// Clipping���� �ʴ´�.
//----------------------------------------------------------------------
void
CAlphaSprite::Blt4444(WORD *pDest, WORD pitch)
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
			// �� �� ���
			if (count > 0)
			{			
				j = count;
				do
				{				
					pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
					colorCount = *pPixels++;		// ���� �ƴ� �� ��				

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					memcpyAlpha4444(pDestTemp, pPixels, colorCount);
					
					pDestTemp	+= colorCount;

					// ���� �� ������ alpha�� ������ 2���̴�.
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
// ���� clipping.  
// rectLeft���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CAlphaSprite::Blt4444ClipLeft(WORD* pDest, WORD pitch, RECT* pRect)
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

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	for (i=pRect->top; i<rectBottom; i++)
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
		// �� �� ���
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
						memcpyAlpha4444(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += (colorCount<<1);

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
						memcpyAlpha4444(pDestTemp, pPixels+(dist<<1), colorCount-dist);
						pDestTemp += colorCount-dist;
						pPixels += (colorCount<<1);

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += (colorCount<<1);
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
					memcpyAlpha4444(pDestTemp, pPixels, colorCount);

					// memory addr ����
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
// ������ clipping.  
// rectRight�� ������ ���� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CAlphaSprite::Blt4444ClipRight(WORD* pDest, WORD pitch, RECT* pRect)
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

	for (i=pRect->top; i<rectBottom; i++)
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
		// �� �� ���
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
						memcpyAlpha4444(pDestTemp, pPixels, rectRight - index);
						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				// ���
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
// ���� clipping.  
// rectLeft���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
// rectRight����..
//----------------------------------------------------------------------
void
CAlphaSprite::Blt4444ClipWidth(WORD* pDest, WORD pitch, RECT* pRect)
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

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	for (i=pRect->top; i<rectBottom; i++)
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
		// �� �� ���
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
						memcpyAlpha4444(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += (colorCount<<1);
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

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						memcpyAlpha4444(pDestTemp, pPixels+(dist<<1), colorCount-dist);
						pDestTemp += colorCount-dist;
						pPixels += (colorCount<<1);
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += (colorCount<<1);
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
							memcpyAlpha4444(pDestTemp, pPixels, rectRight - index);
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					// ���
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
// pRect->top, rectBottom��ŭ�� ����Ѵ�.
//----------------------------------------------------------------------
void
CAlphaSprite::Blt4444ClipHeight(WORD *pDest, WORD pitch, RECT* pRect)
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

		// (������,�����,�����)�� �ݺ� ��		
		count	= *pPixels++;		

		// �� �� ���
		// �� �� ���
		if (count > 0)
		{			
			j = count;
			do
			{	
				pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
				colorCount = *pPixels++;		// ���� �ƴ� �� ��				

				// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// Clipping���� �ʴ´�.
//----------------------------------------------------------------------
void
CAlphaSprite::Blt4444NotTrans(WORD *pDest, WORD pitch)
{
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

					// ���� �� ������ alpha�� ������ 2���̴�.
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
// ���� clipping.  
// rectLeft���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CAlphaSprite::Blt4444NotTransClipLeft(WORD* pDest, WORD pitch, RECT* pRect)
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

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	for (i=pRect->top; i<rectBottom; i++)
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
		// �� �� ���
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
						// 0�� ����Ѵ�.
						transCount = index - rectLeft;
						memset(pDestTemp, 0, transCount<<1);
						pDestTemp += transCount;		// ��������ŭ �ǳ� �ڴ�.
						
						// �̹� �ܰ�� ��� ���
						memcpyAlpha4444(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += (colorCount<<1);

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
						memcpyAlpha4444(pDestTemp, pPixels+(dist<<1), colorCount-dist);
						pDestTemp += colorCount-dist;
						pPixels += (colorCount<<1);

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += (colorCount<<1);
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
							
					// 0�� ����Ѵ�.
					memset(pDestTemp, 0, transCount<<1);
					pDestTemp += transCount;		// ��������ŭ �ǳ� �ڴ�.
					
					// �������� �ƴѸ�ŭ ������ش�.
					memcpyAlpha4444(pDestTemp, pPixels, colorCount);

					// memory addr ����
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
// ������ clipping.  
// rectRight�� ������ ���� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CAlphaSprite::Blt4444NotTransClipRight(WORD* pDest, WORD pitch, RECT* pRect)
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

	for (i=pRect->top; i<rectBottom; i++)
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
		// �� �� ���
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
						// 0�� ����Ѵ�.
						memset(pDestTemp, 0, transCount<<1);
						pDestTemp += transCount;		// ��������ŭ �ǳ� �ڴ�.
					
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						memcpyAlpha4444(pDestTemp, pPixels, rectRight - index);
						break;
					}
				}

				// 0�� ����Ѵ�.
				memset(pDestTemp, 0, transCount<<1);
				pDestTemp += transCount;		// ��������ŭ �ǳ� �ڴ�.

				// ���
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
// ���� clipping.  
// rectLeft���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
// rectRight����..
//----------------------------------------------------------------------
void
CAlphaSprite::Blt4444NotTransClipWidth(WORD* pDest, WORD pitch, RECT* pRect)
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

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	for (i=pRect->top; i<rectBottom; i++)
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
		// �� �� ���
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
						transCount = index - rectLeft;
						// 0�� ����Ѵ�.
						memset(pDestTemp, 0, transCount<<1);
						pDestTemp += transCount;		// ��������ŭ �ǳ� �ڴ�.

						// �̹� �ܰ�� ��� ���
						memcpyAlpha4444(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += (colorCount<<1);
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

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						memcpyAlpha4444(pDestTemp, pPixels+(dist<<1), colorCount-dist);
						pDestTemp += colorCount-dist;
						pPixels += (colorCount<<1);
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += (colorCount<<1);
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
							// 0�� ����Ѵ�.
							memset(pDestTemp, 0, transCount<<1);
							pDestTemp += transCount;		// ��������ŭ �ǳ� �ڴ�.
						
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							memcpyAlpha4444(pDestTemp, pPixels, rectRight - index);
							break;
						}
					}

					// 0�� ����Ѵ�.
					memset(pDestTemp, 0, transCount<<1);
					pDestTemp += transCount;		// ��������ŭ �ǳ� �ڴ�.

					// ���
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
// pRect->top, rectBottom��ŭ�� ����Ѵ�.
//----------------------------------------------------------------------
void
CAlphaSprite::Blt4444NotTransClipHeight(WORD *pDest, WORD pitch, RECT* pRect)
{
	int		count,
			transCount,
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

		// (������,�����,�����)�� �ݺ� ��		
		count	= *pPixels++;		

		// �� �� ���
		// �� �� ���
		if (count > 0)
		{			
			j = count;
			do
			{	
				transCount = *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
				colorCount = *pPixels++;		// ���� �ƴ� �� ��				

				// 0�� ����Ѵ�.
				memset(pDestTemp, 0, transCount<<1);
				pDestTemp += transCount;		// ��������ŭ �ǳ� �ڴ�.

				// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// Clipping���� �ʴ´�.
//----------------------------------------------------------------------
void
CAlphaSprite::BltAlpha(WORD *pDest, WORD pitch, BYTE alpha)
{
	s_Value1 = alpha;

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
				do
				{
					pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
					colorCount = *pPixels++;		// ���� �ƴ� �� ��				

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					memcpyAlphaValue(pDestTemp, pPixels, colorCount);
					
					pDestTemp	+= colorCount;

					// ���� �� ������ alpha�� ������ 2���̴�.
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
// ���� clipping.  
// rectLeft���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CAlphaSprite::BltAlphaClipLeft(WORD* pDest, WORD pitch, RECT* pRect, BYTE alpha)
{
	s_Value1 = alpha;

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

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	for (i=pRect->top; i<rectBottom; i++)
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
		// �� �� ���
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
						memcpyAlphaValue(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += (colorCount<<1);

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
						memcpyAlphaValue(pDestTemp, pPixels+(dist<<1), colorCount-dist);
						pDestTemp += colorCount-dist;
						pPixels += (colorCount<<1);

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += (colorCount<<1);
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
					memcpyAlphaValue(pDestTemp, pPixels, colorCount);

					// memory addr ����
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
// ������ clipping.  
// rectRight�� ������ ���� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CAlphaSprite::BltAlphaClipRight(WORD* pDest, WORD pitch, RECT* pRect, BYTE alpha)
{
	s_Value1 = alpha;

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

	for (i=pRect->top; i<rectBottom; i++)
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
		// �� �� ���
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
						memcpyAlphaValue(pDestTemp, pPixels, rectRight - index);
						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				// ���
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
// ���� clipping.  
// rectLeft���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
// rectRight����..
//----------------------------------------------------------------------
void
CAlphaSprite::BltAlphaClipWidth(WORD* pDest, WORD pitch, RECT* pRect, BYTE alpha)
{
	s_Value1 = alpha;

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

	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
	int rectRight = pRect->right;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	for (i=pRect->top; i<rectBottom; i++)
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
		// �� �� ���
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
								memcpyAlphaValue(pDestTemp, pPixels, (rectRight - index));
							}

							j = 0;
							
							break;
						}

						memcpyAlphaValue(pDestTemp, pPixels, colorCount);
						pDestTemp += colorCount;
						pPixels += (colorCount<<1);
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

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						// ������ ���� �Ѿ�� ���..
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

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				pPixels += (colorCount<<1);
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
							memcpyAlphaValue(pDestTemp, pPixels, rectRight - index);
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					// ���
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
// pRect->top, rectBottom��ŭ�� ����Ѵ�.
//----------------------------------------------------------------------
void
CAlphaSprite::BltAlphaClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha)
{
	s_Value1 = alpha;

	int	count,			
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

		// (������,�����,�����)�� �ݺ� ��		
		count	= *pPixels++;		

		// �� �� ���
		// �� �� ���
		if (count > 0)
		{			
			j = count;
			do
			{
				pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
				colorCount = *pPixels++;		// ���� �ƴ� �� ��				

				// ������ �ƴ� ������ Surface�� ����Ѵ�.
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
// Alpha�� : 1~32
//----------------------------------------------------------------------
// pSource�� ���� pDest�� ����� �ؾ��Ѵ�.
// pSource�� ������ (alpha,���� �ϳ�)�� pixels��ŭ �ݺ��̴�.
//----------------------------------------------------------------------
void	
CAlphaSprite::memcpyAlphaValue(WORD* pDest, WORD* pSource, WORD pixels)
{
	register WORD		sTemp,dTemp;
	register int		sr,sg,sb,dr,dg,db;
	//static WORD		temp, temp2;

	register int i = pixels;

	//BYTE alpha;

	// Alpha Channel Blending
	// ������ ���
	while (i--)
	{	
		// Source���� Alpha���� ���ԵǾ� �ִ�.
		//alpha = *pSource >> 8;
		pSource++;

		// ���� ���
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
		// ��... �̰� �� ������.. �� �׷���.. - -;;;
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
// ����ؼ� ���.. 
// Clipping���� �ʴ´�.
//----------------------------------------------------------------------
// shift�� ���ؼ� ���� ������ ���̴ٺ���
// ���� ���̰� ���� ���� �Ǵµ�,
// �װ� ��������� �Ѵ�.
// ����� �� ������.. T_T;;
// �ٿ��� ���̸� �ٽ� �ø���(-_-;)
// ������ ���� ������ �� �� �ִ�.					
//----------------------------------------------------------------------
void
CAlphaSprite::Blt4444SmallNotTrans(WORD *pDest, WORD pitch, BYTE shift)
{
	s_Value1 = shift;
	// memcpy���� �ϳ��� alpha���� �����ϰ� +�ϴ� ��
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
			totalCount = 0;
			totalShiftCount = 0;
			if (count > 0)
			{			
				j = count;
				do
				{		
					transCount = *pPixels++;					
					colorCount = *pPixels++;		// ���� �ƴ� �� ��	
					
					//--------------------------------------------------
					// shift��ŭ �ٿ��� ���� ����Ѵ�.
					//--------------------------------------------------
					transCountShift = transCount >> shift;
					colorCountShift = colorCount >> shift;

					//--------------------------------------------------
					//				���� �κ� ���� ����
					//--------------------------------------------------
					// ���� size�� pixel��..
					//--------------------------------------------------
					totalCount += transCount;
					totalShiftCount += transCountShift;

					// ����pixel - shift�ؼ� �ø� pixel(-_-;)
					pixelGap = totalCount - (totalShiftCount << shift);

					// gap�� �ٽ� shift�ؼ� �����ش�.
					pixelGapShift = pixelGap >> shift;
					transCountShift += pixelGapShift;
					totalShiftCount += pixelGapShift;

					
					//--------------------------------------------------
					// 0�� ����Ѵ�. �������̴�..
					//--------------------------------------------------
					memset(pDestTemp, 0, transCountShift<<1);
					pDestTemp += transCountShift;		// ��������ŭ �ǳ� �ڴ�.


					//--------------------------------------------------
					//				���� �κ� ���� ����
					//--------------------------------------------------
					// (!!!) �ٵ� ���⼭ ������ �ִ�.
					// �������̾� �׳� �������ε�..
					// ������ �ִ� �κп����� 
					// �� ��� �� ���ߵ� ���..
					// � ���� ����ұ�?? ��..
					// �̴�ζ��.. 
					// ������ ����ŭ �׳� �ǳʶ� ��.. ��..- -;
					//
					// �׷���, �ϴ� ����.. - -;
					//--------------------------------------------------
					totalCount += colorCount;
					totalShiftCount += colorCountShift;

					//--------------------------------------------------
					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					//--------------------------------------------------					
					memcpyAlpha4444Small(pDestTemp, pPixels, colorCount);
					
					pDestTemp	+= colorCountShift;

					// ���� �� ������ alpha�� ������ 2���̴�.
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
// Alpha�� : 1~32
//----------------------------------------------------------------------
// pSource�� ���� pDest�� ����� �ؾ��Ѵ�.
// pSource�� ������ (alpha,���� �ϳ�)�� pixels��ŭ �ݺ��̴�.
//
// A:R:G:B = 4:4:4:4 Texture�� ���� ���̴�.
//----------------------------------------------------------------------
void	
CAlphaSprite::memcpyAlpha4444Small(WORD* pDest, WORD* pSource, WORD pixels)
{
	register WORD		sTemp;
	register int		sr,sg,sb;
	//static WORD		temp, temp2;

	register int i = pixels >> s_Value1;

	BYTE alpha;
	
	// Alpha Channel Blending
	// ������ ���
	while (i--)
	{	
		// Source���� Alpha���� ���ԵǾ� �ִ�.
		alpha = *pSource >> 9;	//	alpha = (*pSource >> 8) >> 1;	4 bit�̱� ������..
		pSource++;

		// ���� ���
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