//----------------------------------------------------------------------
// CShadowSprite.cpp
//----------------------------------------------------------------------
#include "client_PCH.h"
#include "CSpriteSurface.h"
#include "CFilter.h"
#include "CShadowSprite.h"

#ifdef SPRITELIB_BACKEND_SDL
#include "SpriteLibBackend.h"
#endif


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

#ifdef SPRITELIB_BACKEND_SDL
	m_backend_sprite = SPRITECTL_INVALID_SPRITE;
	m_backend_dirty = false;
#endif
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
// m_Pixels�� memory�� �����Ѵ�.
//----------------------------------------------------------------------
void	
CShadowSprite::Release()
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
	// �޸� ����
	Release();

	// NULL�̸� �������� �ʴ´�.
	if (Sprite.m_Pixels==NULL || Sprite.m_Width==0 || Sprite.m_Height==0)
		return;

	// ũ�� ����
	m_Width = Sprite.m_Width;
	m_Height = Sprite.m_Height;	

	// ���� �� �� ����
	WORD index;	

	// �޸� ���
	m_Pixels = new WORD* [m_Height];

	//--------------------------------
	// 5:6:5
	//--------------------------------
	for (int i=0; i<m_Height; i++)
	{
		// �ݺ��� + �ݺ���*2byte
		index	= 1 + m_Pixels[i][0]*2;	

		// �޸� ���
		m_Pixels[i] = new WORD [index];
		memcpy(m_Pixels[i], Sprite.m_Pixels[i], index<<1);
	}

}

//----------------------------------------------------------------------
// fstream�� save �Ѵ�.    ( file���� 5:6:5�� �����Ѵ�. )
//----------------------------------------------------------------------
bool	
CShadowSprite::SaveToFile(ofstream& file)
{
	// width�� height�� �����Ѵ�.
	file.write((const char*)&m_Width , 2);
	file.write((const char*)&m_Height, 2);

	// NULL�̸� �������� �ʴ´�. ���̸� ����Ǵ� ���̴�.
	if (m_Pixels==NULL || m_Width==0 || m_Height==0)
		return false;
	
	// ���� �� �� ����
	WORD index;	

	//--------------------------------
	// 5:6:5
	//--------------------------------
	for (int i=0; i<m_Height; i++)
	{
		// �ݺ��� + �ݺ���*2byte
		index	= 1 + m_Pixels[i][0]*2;	

		// byte���� ���� data�� �����Ѵ�.
		file.write((const char*)&index, 2);
		file.write((const char*)m_Pixels[i], index<<1);
	}


	return true;
}

//----------------------------------------------------------------------
// fstream���� load�Ѵ�.
//----------------------------------------------------------------------
bool	
CShadowSprite::LoadFromFile(ifstream& file)
{
	// �̹� �����ִ� memory�� release�Ѵ�.
	Release();

	// width�� height�� Load�Ѵ�.
	file.read((char*)&m_Width , 2);
	file.read((char*)&m_Height, 2);	

	// ���̰� 0�̸� �� Load�Ұ� ������..
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
		// byte���� ���� data�� Load�Ѵ�.
		file.read((char*)&len, 2);
		
		m_Pixels[i] = NULL;
		m_Pixels[i] = new WORD [len];		
	
		file.read((char*)m_Pixels[i], len<<1);
	}	
	
	m_bInit = true;

	return true;
}

//----------------------------------------------------------------------
// CDirectDrawSurface�� (x,y)+(width, height)������ �о m_Pixels�� �����Ѵ�.
//----------------------------------------------------------------------
// m_Pixels�� 0�� ���� Format���� �ٲ۴�.
//
// �� line���� ������ ���� ������ ������.
//
//    [�ݺ���] (������,�����)(������,�����)......
//
// �ݺ����� 2 bytes�̰�
// �������� ������� ���� 2 byte
//
//----------------------------------------------------------------------
void
CShadowSprite::SetPixel(WORD *pSource, WORD pitch, WORD width, WORD height)
{
	// memory����
	Release();

	m_Width = width;
	m_Height = height;

	// �ϴ� memory�� ������ ��Ƶд�.	
	WORD*	data = new WORD[m_Width+2];

	int	index;				// data�� index�� ���			
	int	count;				// �ݺ���
	int	trans,				// ������ ����
			color;				// ������ �ƴѻ� ����

	BOOL	bCheckTrans;		// �ֱٿ� �˻��Ѱ� �������ΰ�?

	WORD	*pSourceTemp;


	// height�� ��ŭ memory���
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

		// �� line�� ���ؼ� ����~
		for (j=0; j<width; j++)
		{
			// 0�� color�� ���ؼ� ����
			if (*pSourceTemp==s_Colorkey)
			{
				// �ֱٿ� �˻��Ѱ� �������� �ƴϾ��ٸ�
				if (!bCheckTrans)
				{
					// ' (����,�����) '�� �� set�� �������� �ǹ��ϹǷ�
					count++;
					
					data[index++] = color;
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

					bCheckTrans = FALSE;
				}

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
			data[index++] = color;
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
// SetPixel
//----------------------------------------------------------------------
// IndexSprite --> ShadowSprite
//----------------------------------------------------------------------
void		
CShadowSprite::SetPixel(CIndexSprite& ispr)
{
	// memory����
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

	// height�� ��ŭ memory���
	m_Pixels = new WORD* [m_Height];

	for (i=0; i<m_Height; i++)
	{			
		pPixels		= ispr.GetPixelLine( i );
		
		// (������,Index��,�����)�� �ݺ� ��		
		transPair	= *pPixels++;

		// �ݺ��� + (������,�׸��ڼ�)*�ݺ���
		m_Pixels[i] = new WORD [1 + (transPair<<1)];
		index = 0;
		m_Pixels[i][index++] = transPair;
		
 		// �� �� ���
		for (j=0; j<transPair; j++)
		{			
			//------------------------------------
			// ������ ��
			//------------------------------------
			m_Pixels[i][index++] = *pPixels;	
			pPixels++;			

			indexCount = *pPixels++;	// Index�ݺ� ��			
			pPixels += indexCount;		// Index���鸸ŭ ����
			
			colorCount = *pPixels++;	// color ���� ��			
			pPixels		+= colorCount;	// Color ����ŭ ����

			//------------------------------------
			// �׸��� �� = Index + Color��
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
	// memory����
	Release();

	m_Width = spr.GetWidth();
	m_Height = spr.GetHeight();

	int		transPair,	
			colorCount,
			index;
	
	WORD	*pPixels;

	register int i;
	register int j;	

	// height�� ��ŭ memory���
	m_Pixels = new WORD* [m_Height];

	for (i=0; i<m_Height; i++)
	{			
		pPixels		= spr.GetPixelLine( i );
		
		// (������,�����)�� �ݺ� ��		
		transPair	= *pPixels++;

		// �ݺ��� + (������,�׸��ڼ�)*�ݺ���
		m_Pixels[i] = new WORD [1 + (transPair<<1)];
		index = 0;
		m_Pixels[i][index++] = transPair;
		
 		// �� �� ���
		for (j=0; j<transPair; j++)
		{			
			//------------------------------------
			// ������ ��
			//------------------------------------
			m_Pixels[i][index++] = *pPixels;	
			pPixels++;			

			colorCount = *pPixels++;	// color ���� ��			
			pPixels		+= colorCount;	// Color ����ŭ ����

			//------------------------------------
			// �׸��� �� = Index + Color��
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
// Sprite�ȿ��� (x,y)�� ������ �ִ°�?(�������� �ƴ� ���)
//----------------------------------------------------------------------
bool		
CShadowSprite::IsColorPixel(short x, short y)
{
	// ���� �ʱ�ȭ ���� ���� ���
	if (m_Pixels==NULL)
		return false; 

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

	for (int register i=0; i<count; i++)
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
				return false;
			}

			// ���� ���Ѵ�.
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
// Clipping���� �ʴ´�.
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

		// (������,�����)�� �ݺ� ��		
		count	= *pPixels++;		
 		// �� �� ���
		if (count > 0)
		{			
			j = count;
			do
			{
				pDestTemp += *pPixels++;			// ��������ŭ �ǳ� �ڴ�.
				colorCount = *pPixels++;		// ���� �ƴ� �� ��				

				// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CShadowSprite::BltClipLeft(WORD* pDest, WORD pitch, RECT* pRect)
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
	for (i=pRect->top; i<pRect->bottom; i++)
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
				if (index+colorCount > pRect->left)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > pRect->left)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - pRect->left;

						// �̹� �ܰ�� ��� ���
						//memcpy(pDestTemp, pPixels, colorCount<<1);
						// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
						memset((void*)pDestTemp, 0, colorCount<<1);

						pDestTemp += colorCount;
						//pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = pRect->left - index;

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
						// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
						memset((void*)pDestTemp, 0, (colorCount-dist)<<1);

						pDestTemp += colorCount-dist;
						//pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				//pPixels += colorCount;
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
					
					// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
					memset((void*)pDestTemp, 0, colorCount<<1);

					// memory addr ����
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
// ������ clipping.  
// pRect->right�� ������ ���� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CShadowSprite::BltClipRight(WORD* pDest, WORD pitch, RECT* pRect)
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

	for (i=pRect->top; i<pRect->bottom; i++)
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
				if (index+colorCount > pRect->right)
				{
					// ������������ �� ����� �ʿ䰡 ���� ��
					if (index > pRect->right)
					{
						break;
					}
					// ������ �ƴ� ���� ���� ����ؾ� �� ���
					else
					{
						pDestTemp += transCount;
					
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						//memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
						// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
						memset((void*)pDestTemp, 0, (pRect->right - index)<<1);

						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				// ���
				//memcpy(pDestTemp, pPixels, colorCount<<1);
				// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����ϴٰ�
// pRect->Right������ ����Ѵ�.
//----------------------------------------------------------------------
void
CShadowSprite::BltClipWidth(WORD* pDest, WORD pitch, RECT* pRect)
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

	register short i;
	register short j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	for (i=pRect->top; i<pRect->bottom; i++)
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
				if (index+colorCount > pRect->left)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > pRect->left)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - pRect->left;

						// �̹� �ܰ�� ��� ���?
						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > pRect->right)
						{							
							// ������������ ������ �� �Ѿ�� ���
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

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{		
						dist = pRect->left - index;

						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > pRect->right)
						{						
							memset((void*)pDestTemp, 0, pRect->right - pRect->left);

							j = count;						
							break;
						}

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
						// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
						memset((void*)pDestTemp, 0, (colorCount-dist)<<1);

						pDestTemp += colorCount-dist;
						//pPixels += colorCount;		
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.					
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				//pPixels += colorCount;
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
					if (index+colorCount > pRect->right)
					{
						// ������������ �� ����� �ʿ䰡 ���� ��
						if (index > pRect->right)
						{
							break;
						}
						// ������ �ƴ� ���� ���� ����ؾ� �� ���
						else
						{
							pDestTemp += transCount;
						
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							//memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
							// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
							memset((void*)pDestTemp, 0, (pRect->right - index)<<1);

							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					// ���
					//memcpy(pDestTemp, pPixels, colorCount<<1);
					// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
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
// pRect->top, pRect->bottom��ŭ�� ����Ѵ�.
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
				//memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
				// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
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
// Clipping���� �ʴ´�.
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

		// (������,�����)�� �ݺ� ��		
		count	= *pPixels++;		
 		// �� �� ���
		if (count > 0)
		{			
			j = count;
			do
			{						
				pDestTemp += *pPixels++;			// ��������ŭ �ǳ� �ڴ�.
				colorCount = *pPixels++;		// ���� �ƴ� �� ��				

				// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CShadowSprite::BltDarknessClipLeft(WORD* pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	s_Value1 = DarkBits;

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
	for (i=pRect->top; i<pRect->bottom; i++)
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
				if (index+colorCount > pRect->left)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > pRect->left)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - pRect->left;

						// �̹� �ܰ�� ��� ���
						//memcpy(pDestTemp, pPixels, colorCount<<1);
						// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
						//memset((void*)pDestTemp, 0, colorCount<<1);
						memcpyShadowDarkness(pDestTemp, colorCount);

						pDestTemp += colorCount;
						//pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = pRect->left - index;

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
						// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
						//memset((void*)pDestTemp, 0, (colorCount-dist)<<1);
						memcpyShadowDarkness(pDestTemp, colorCount-dist);

						pDestTemp += colorCount-dist;
						//pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				//pPixels += colorCount;
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
					
					// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
					//memset((void*)pDestTemp, 0, colorCount<<1);
					memcpyShadowDarkness(pDestTemp, colorCount);

					// memory addr ����
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
// ������ clipping.  
// pRect->right�� ������ ���� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CShadowSprite::BltDarknessClipRight(WORD* pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	s_Value1 = DarkBits;

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

	for (i=pRect->top; i<pRect->bottom; i++)
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
				if (index+colorCount > pRect->right)
				{
					// ������������ �� ����� �ʿ䰡 ���� ��
					if (index > pRect->right)
					{
						break;
					}
					// ������ �ƴ� ���� ���� ����ؾ� �� ���
					else
					{
						pDestTemp += transCount;
					
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						//memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
						// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
						//memset((void*)pDestTemp, 0, (pRect->right - index)<<1);
						memcpyShadowDarkness(pDestTemp, pRect->right - index);

						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				// ���
				//memcpy(pDestTemp, pPixels, colorCount<<1);
				// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����ϴٰ�
// pRect->Right������ ����Ѵ�.
//----------------------------------------------------------------------
void
CShadowSprite::BltDarknessClipWidth(WORD* pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	s_Value1 = DarkBits;

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
	for (i=pRect->top; i<pRect->bottom; i++)
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
				if (index+colorCount > pRect->left)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > pRect->left)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - pRect->left;

						// �̹� �ܰ�� ��� ���?
						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > pRect->right)
						{							
							// ������������ ������ �� �Ѿ�� ���
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

						// �̹� �ܰ�� ��� ���
						//memcpy(pDestTemp, pPixels, colorCount<<1);
						// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
						//memset((void*)pDestTemp, 0, colorCount<<1);
						memcpyShadowDarkness(pDestTemp, colorCount);

						pDestTemp += colorCount;
						//pPixels += colorCount;
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
						dist = pRect->left - index;

						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > pRect->right)
						{
							memcpyShadowDarkness(pDestTemp, pRect->right - pRect->left);

							j = count;						
							break;
						}

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
						// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
						//memset((void*)pDestTemp, 0, (colorCount-dist)<<1);
						memcpyShadowDarkness(pDestTemp, colorCount-dist);

						pDestTemp += colorCount-dist;
						//pPixels += colorCount;		
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.					
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				//pPixels += colorCount;
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
					if (index+colorCount > pRect->right)
					{
						// ������������ �� ����� �ʿ䰡 ���� ��
						if (index > pRect->right)
						{
							break;
						}
						// ������ �ƴ� ���� ���� ����ؾ� �� ���
						else
						{
							pDestTemp += transCount;
						
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							//memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
							// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
							//memset((void*)pDestTemp, 0, (pRect->right - index)<<1);
							memcpyShadowDarkness(pDestTemp, pRect->right-index);

							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					// ���
					//memcpy(pDestTemp, pPixels, colorCount<<1);
					// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
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
// pRect->top, pRect->bottom��ŭ�� ����Ѵ�.
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
				//memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
				// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
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
// dest�� pixels���� s_Value1��ŭ ��Ӱ� ����� �Ѵ�.
//----------------------------------------------------------------------
void	
CShadowSprite::memcpyShadowDarkness(WORD* pDest, WORD pixels)
{
	QWORD	*qpDest		= (QWORD*)pDest;

	register int j;

	BYTE qTimes = pixels >> 2;	// pixels / 4

	// ������
	switch ( pixels & 0x03 )	// pixels % 4
	{
		//------------------
		// 4����
		//------------------
		case 0 :			
			// ������ ���
			for (j=0; j<qTimes; j++)
			{
				*qpDest = ((*qpDest >> s_Value1) & CDirectDraw::s_qwMASK_SHIFT[s_Value1]);				

				*qpDest++;				
			}
		break;

		//------------------
		// 1�� + 4����
		//------------------
		case 1 :
			// ���� ���
			*(WORD*)qpDest = ((*(WORD*)qpDest >> s_Value1) & CDirectDraw::s_wMASK_SHIFT[s_Value1]);
			
				
			qpDest = (QWORD*)((WORD*)qpDest + 1);			

			// ������ ���
			for (j=0; j<qTimes; j++)
			{
				*qpDest = ((*qpDest >> s_Value1) & CDirectDraw::s_qwMASK_SHIFT[s_Value1]);

				*qpDest++;				
			}
			
		break;

		//------------------
		// 2�� + 4����
		//------------------
		case 2 :
			// ���� ���
			*(DWORD*)qpDest = ((*(DWORD*)qpDest >> s_Value1) & CDirectDraw::s_dwMASK_SHIFT[s_Value1]);			
				
			qpDest = (QWORD*)((DWORD*)qpDest + 1);			

			// ������ ���
			for (j=0; j<qTimes; j++)
			{
				*qpDest = ((*qpDest >> s_Value1) & CDirectDraw::s_qwMASK_SHIFT[s_Value1]);

				*qpDest++;				
			}
		break;

		//------------------
		// 1�� + 2�� + 4����
		//------------------
		case 3 :
			// ���� ���
			*(WORD*)qpDest = ((*(WORD*)qpDest >> s_Value1) & CDirectDraw::s_wMASK_SHIFT[s_Value1]);	

			// ���� ���
			*(DWORD*)qpDest = ((*(DWORD*)qpDest >> s_Value1) & CDirectDraw::s_dwMASK_SHIFT[s_Value1]);
				
			qpDest = (QWORD*)((DWORD*)qpDest + 1);			

			// ������ ���
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
// Clipping���� �ʴ´�.
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

		// (������,�����)�� �ݺ� ��		
		count	= *pPixels++;		
 		// �� �� ���
		for (register int j=0; j<count; j++)
		{				
			pDestTemp += *pPixels++;			// ��������ŭ �ǳ� �ڴ�.
			colorCount = *pPixels++;		// ���� �ƴ� �� ��				

			// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CShadowSprite::Blt4444ClipLeft(WORD* pDest, WORD pitch, RECT* pRect, WORD pixel)
{
	s_wValue1 = pixel;

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
	for (i=pRect->top; i<pRect->bottom; i++)
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
		for (j=0; j<count; j++)
		{
			transCount = *pPixels++;		// ������ ��			
			colorCount = *pPixels++;		// ���� �ƴ� �� ��			
					
			// ��������ŭ index����			
			index += transCount;
			
		
			//---------------------------------------------
			// xxxx������ �Ѿ�� �Ǵ� ���
			//---------------------------------------------
			if (index+colorCount > pRect->left)
			{
				//---------------------------------------------
				// ������������ xxxx������ �Ѿ�� ���
				//---------------------------------------------
				if (index > pRect->left)
				{	
					// �������κ� �ǳʶ�
					pDestTemp += index - pRect->left;

					// �̹� �ܰ�� ��� ���
					//memcpy(pDestTemp, pPixels, colorCount<<1);
					// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
					//memset((void*)pDestTemp, 0, colorCount<<1);
					memcpyShadow4444(pDestTemp, colorCount);

					pDestTemp += colorCount;
					//pPixels += colorCount;

					// �������ʹ� ��� ����Ѵ�.
					break;
				}
				//---------------------------------------------
				// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
				// xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				else
				{
					dist = pRect->left - index;

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
					// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
					//memset((void*)pDestTemp, 0, (colorCount-dist)<<1);
					memcpyShadow4444(pDestTemp, colorCount-dist);

					pDestTemp += colorCount-dist;
					//pPixels += colorCount;

					// �������ʹ� ��� ����Ѵ�.
					break;
				}
			}					

			// ������ �ƴ� ����ŭ index����				
			//pPixels += colorCount;
			index += colorCount;
		}

		//---------------------------------------------
		// �������ʹ� ��� ����Ѵ�.		
		//---------------------------------------------		
		for (j++; j<count; j++)
		{
			transCount = *pPixels++;		// ������ ��			
			colorCount = *pPixels++;		// ���� �ƴ� �� ��			
					
			// ��������ŭ �ǳ� �ڴ�.
			pDestTemp += transCount;			
			
			// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
			//memset((void*)pDestTemp, 0, colorCount<<1);
			memcpyShadow4444(pDestTemp, colorCount);

			// memory addr ����
			pDestTemp += colorCount;
			//pPixels += colorCount;			
		}
		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt4444 ClipRight
//----------------------------------------------------------------------
// ������ clipping.  
// pRect->right�� ������ ���� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CShadowSprite::Blt4444ClipRight(WORD* pDest, WORD pitch, RECT* pRect, WORD pixel)
{
	s_wValue1 = pixel;

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

	for (i=pRect->top; i<pRect->bottom; i++)
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
		for (j=0; j<count; j++)
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
			if (index+colorCount > pRect->right)
			{
				// ������������ �� ����� �ʿ䰡 ���� ��
				if (index > pRect->right)
				{
					break;
				}
				// ������ �ƴ� ���� ���� ����ؾ� �� ���
				else
				{
					pDestTemp += transCount;
				
					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					//memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
					// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
					//memset((void*)pDestTemp, 0, (pRect->right - index)<<1);
					memcpyShadow4444(pDestTemp, pRect->right - index);

					break;
				}
			}

			// ��������ŭ �ǳʶ��
			pDestTemp += transCount;

			// ���
			//memcpy(pDestTemp, pPixels, colorCount<<1);
			// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����ϴٰ�
// pRect->Right������ ����Ѵ�.
//----------------------------------------------------------------------
void
CShadowSprite::Blt4444ClipWidth(WORD* pDest, WORD pitch, RECT* pRect, WORD pixel)
{
	s_wValue1 = pixel;

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
	for (i=pRect->top; i<pRect->bottom; i++)
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
		for (j=0; j<count; j++)
		{
			transCount = *pPixels++;		// ������ ��			
			colorCount = *pPixels++;		// ���� �ƴ� �� ��			
					
			// ��������ŭ index����			
			index += transCount;
			
		
			//---------------------------------------------
			// xxxx������ �Ѿ�� �Ǵ� ���
			//---------------------------------------------
			if (index+colorCount > pRect->left)
			{
				//---------------------------------------------
				// ������������ xxxx������ �Ѿ�� ���
				//---------------------------------------------
				if (index > pRect->left)
				{	
					// �������κ� �ǳʶ�
					pDestTemp += index - pRect->left;

					// �̹� �ܰ�� ��� ���?
					// ������ ���� �Ѿ�� ���..
					if (index+colorCount > pRect->right)
					{							
						// ������������ ������ �� �Ѿ�� ���
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

					// �̹� �ܰ�� ��� ���
					//memcpy(pDestTemp, pPixels, colorCount<<1);
					// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
					//memset((void*)pDestTemp, 0, colorCount<<1);
					memcpyShadow4444(pDestTemp, colorCount);

					pDestTemp += colorCount;
					//pPixels += colorCount;
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
					dist = pRect->left - index;

					// ������ ���� �Ѿ�� ���..
					if (index+colorCount > pRect->right)
					{
						memcpyShadow4444(pDestTemp, pRect->right - pRect->left);

						j = count;						
						break;
					}

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
					// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
					//memset((void*)pDestTemp, 0, (colorCount-dist)<<1);
					memcpyShadow4444(pDestTemp, colorCount-dist);

					pDestTemp += colorCount-dist;
					//pPixels += colorCount;		
					index += colorCount;

					// �������ʹ� ��� ����Ѵ�.					
					break;
				}
			}					

			// ������ �ƴ� ����ŭ index����				
			//pPixels += colorCount;
			index += colorCount;
		}

		//---------------------------------------------
		// �� �ٸ��� Clipping�� ����� �ϴµ�...		
		// OOOOOOOOOOOOOOxxxxx �̷� ����̴�.
		//---------------------------------------------
		// OOOOOOOOOOOOOO������ ������ָ� �ȴ�.
		//---------------------------------------------
		for (j++; j<count; j++)
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
			if (index+colorCount > pRect->right)
			{
				// ������������ �� ����� �ʿ䰡 ���� ��
				if (index > pRect->right)
				{
					break;
				}
				// ������ �ƴ� ���� ���� ����ؾ� �� ���
				else
				{
					pDestTemp += transCount;
				
					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					//memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
					// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
					//memset((void*)pDestTemp, 0, (pRect->right - index)<<1);
					memcpyShadow4444(pDestTemp, pRect->right-index);

					break;
				}
			}

			// ��������ŭ �ǳʶ��
			pDestTemp += transCount;

			// ���
			//memcpy(pDestTemp, pPixels, colorCount<<1);
			// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
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
// pRect->top, pRect->bottom��ŭ�� ����Ѵ�.
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

		// (������,�����,�����)�� �ݺ� ��		
		count	= *pPixels++;		

		// �� �� ���
		for (j=0; j<count; j++)
		{				
			pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
			colorCount = *pPixels++;		// ���� �ƴ� �� ��				

			// ������ �ƴ� ������ Surface�� ����Ѵ�.
			//memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
			// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
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
// dest�� pixels���� s_wValue1�� �Ѵ�.
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
// 2^shift byte���� �� ������ ����Ѵ�.
//
// 100*100 Sprite�� ��� 
// shift = 0 , 100*100
// shift = 1 , 50*50
// shift = 2 , 25*25
// shift = 3 , 12*12
// shift = 4 , 6*6
// ...
// Clipping���� �ʴ´�.
//----------------------------------------------------------------------
// ���� �����ؾߵǴµ�...
// clipping �Լ��鿡���� ���Ѵ�. - -; �����Ƽ�.. ������.
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
		int stepY = 1 << shift;		// y�� �ǳʶ�� pixel��
		pDest = (WORD*)((BYTE*)pDest + (i>>shift)*pitch);

		do {	
			pPixels		= m_Pixels[i];
			pDestTemp	= pDest;

			// (������,�����)�� �ݺ� ��		
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
					colorCount = *pPixels++;				// ���� �ƴ� �� ��
					
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
					// ��������ŭ �ǳ� �ڴ�.
					//--------------------------------------------------
					pDestTemp += transCountShift;		

					
					//--------------------------------------------------
					//				�׸��� �κ� ���� ����
					//--------------------------------------------------
					totalCount += colorCount;
					totalShiftCount += colorCountShift;
					
					//--------------------------------------------------
					// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CShadowSprite::BltSmallClipLeft(WORD* pDest, WORD pitch, RECT* pRect, BYTE shift)
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
	int stepY = 1 << shift;		// y�� �ǳʶ�� pixel��
	int endY = pRect->bottom << shift;
	for (i=pRect->top; i<endY; i+=stepY)
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
				if (index+colorCount > pRect->left)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > pRect->left)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += (index - pRect->left) >> shift;

						// shift��ŭ �ٿ�������.
						colorCount >>= shift;

						// �̹� �ܰ�� ��� ���
						//memcpy(pDestTemp, pPixels, colorCount<<1);
						// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
						memset((void*)pDestTemp, 0, colorCount<<1);

						pDestTemp += colorCount;
						//pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = pRect->left - index;

						// shift��ŭ �ٿ�������.
						colorCount = (colorCount-dist) >> shift;

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
						// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
						memset((void*)pDestTemp, 0, colorCount<<1);

						pDestTemp += colorCount;
						//pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				//pPixels += colorCount;
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
					pDestTemp += transCount>>shift;			

					// shift��ŭ �ٿ�������.
					colorCount >>= shift;
					
					// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
					memset((void*)pDestTemp, 0, colorCount<<1);

					// memory addr ����
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
// ������ clipping.  
// pRect->right�� ������ ���� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CShadowSprite::BltSmallClipRight(WORD* pDest, WORD pitch, RECT* pRect, BYTE shift)
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

	int stepY = 1 << shift;		// y�� �ǳʶ�� pixel��
	int endY = pRect->bottom << shift;
	for (i=pRect->top; i<endY; i+=stepY)
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
				transCount = *pPixels++ >> shift;		// ������ ��			
				colorCount = *pPixels++ >> shift;		// ���� �ƴ� �� ��		
						
				// ��������ŭ index����
				index += transCount;
				
				// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
				// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

				// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

				//---------------------------------------------
				// ������ ������ �������� ���
				//---------------------------------------------			
				if (index+colorCount > pRect->right)
				{
					// ������������ �� ����� �ʿ䰡 ���� ��
					if (index > pRect->right)
					{
						break;
					}
					// ������ �ƴ� ���� ���� ����ؾ� �� ���
					else
					{
						pDestTemp += transCount;
					
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						//memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
						// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
						memset((void*)pDestTemp, 0, (pRect->right - index)<<1);

						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				index += colorCount;
				
				// ���
				//memcpy(pDestTemp, pPixels, colorCount<<1);
				// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����ϴٰ�
// pRect->Right������ ����Ѵ�.
//----------------------------------------------------------------------
void
CShadowSprite::BltSmallClipWidth(WORD* pDest, WORD pitch, RECT* pRect, BYTE shift)
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

	register short i;
	register short j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int stepY = 1 << shift;		// y�� �ǳʶ�� pixel��
	int endY = pRect->bottom << shift;
	for (i=pRect->top; i<endY; i+=stepY)
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
				if (index+colorCount > pRect->left)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > pRect->left)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += (index - pRect->left)>>shift;

						// �̹� �ܰ�� ��� ���?
						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > pRect->right)
						{							
							// ������������ ������ �� �Ѿ�� ���
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

						// shift��ŭ �ٿ�������.
						colorCount >>= shift;
					
						memset((void*)pDestTemp, 0, colorCount<<1);

						pDestTemp += colorCount;
						//pPixels += colorCount;
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
						dist = pRect->left - index;

						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > pRect->right)
						{						
							memset((void*)pDestTemp, 0, (pRect->right - pRect->left)>>shift);

							j = count;						
							break;
						}

						// shift��ŭ �ٿ�������.
						index += colorCount;
						colorCount = (colorCount-dist) >> shift;

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
						// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
						memset((void*)pDestTemp, 0, colorCount<<1);

						pDestTemp += colorCount;
						//pPixels += colorCount;		
						

						// �������ʹ� ��� ����Ѵ�.					
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				//pPixels += colorCount;
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
					if (index+colorCount > pRect->right)
					{
						// ������������ �� ����� �ʿ䰡 ���� ��
						if (index > pRect->right)
						{
							break;
						}
						// ������ �ƴ� ���� ���� ����ؾ� �� ���
						else
						{
							pDestTemp += transCount>>shift;
						
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							//memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
							// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
							memset((void*)pDestTemp, 0, ((pRect->right - index)>>shift)<<1);

							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount>>shift;


					index += colorCount;
					// shift��ŭ �ٿ��ش�.
					colorCount >>= shift;

					// ���
					//memcpy(pDestTemp, pPixels, colorCount<<1);
					// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
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
// pRect->top, pRect->bottom��ŭ�� ����Ѵ�.
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
	
	int stepY = 1 << shift;		// y�� �ǳʶ�� pixel��
	int endY = pRect->bottom << shift;
	for (i=pRect->top; i<endY; i+=stepY)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		// (������,�����,�����)�� �ݺ� ��		
		count	= *pPixels++;		

		if (count > 0)
		{			
			j = count;
			do
			{
				pDestTemp += ((*pPixels++)>>shift);		// ��������ŭ �ǳ� �ڴ�.
				colorCount = *pPixels++;				// ���� �ƴ� �� ��
				
				colorCount >>= shift;

				// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
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
// 2^shift byte���� �� ������ ����Ѵ�.
//
// 100*100 Sprite�� ��� 
// shift = 0 , 100*100
// shift = 1 , 50*50
// shift = 2 , 25*25
// shift = 3 , 12*12
// shift = 4 , 6*6
// ...
// Clipping���� �ʴ´�.
//----------------------------------------------------------------------
// ���� �����ؾߵǴµ�...
// clipping �Լ��鿡���� ���Ѵ�. - -; �����Ƽ�.. ������.
//----------------------------------------------------------------------
void
CShadowSprite::BltSmall4444(WORD *pDest, WORD pitch, WORD pixel, BYTE shift)
{
	// Shadow���� ����
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
	int stepY = 1 << shift;		// y�� �ǳʶ�� pixel��		
	int m_Height_1 = m_Height - 1;

	if (m_Height > 0)
	{
		i = m_Height_1;
		pDest = (WORD*)((BYTE*)pDest + (i>>shift)*pitch);

		do {	
			pPixels		= m_Pixels[i];
			pDestTemp	= pDest;

			// (������,�����)�� �ݺ� ��		
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
					colorCount = *pPixels++;				// ���� �ƴ� �� ��
					
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
					// ��������ŭ �ǳ� �ڴ�.
					//--------------------------------------------------
					pDestTemp += transCountShift;		

					
					//--------------------------------------------------
					//				�׸��� �κ� ���� ����
					//--------------------------------------------------
					totalCount += colorCount;
					totalShiftCount += colorCountShift;
					
					//--------------------------------------------------
					// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
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

	int stepY = 1 << shift;		// y�� �ǳʶ�� pixel��		
	
	for (i=0; i<m_Height; i+=stepY)
	{
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		// (������,�����)�� �ݺ� ��		
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
				colorCount = *pPixels++;				// ���� �ƴ� �� ��
				
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
				// ��������ŭ �ǳ� �ڴ�.
				//--------------------------------------------------
				pDestTemp += transCountShift;		
			
				//--------------------------------------------------
				//				�׸��� �κ� ���� ����
				//--------------------------------------------------
				totalCount += colorCount;
				totalShiftCount += colorCountShift;
				
				//--------------------------------------------------
				// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CShadowSprite::BltSmall4444ClipLeft(WORD* pDest, WORD pitch, RECT* pRect, WORD pixel, BYTE shift)
{
	// Shadow���� ����
	s_wValue1 = pixel;

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
	int stepY = 1 << shift;		// y�� �ǳʶ�� pixel��
	int endY = pRect->bottom << shift;
	for (i=pRect->top; i<endY; i+=stepY)
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
				if (index+colorCount > pRect->left)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > pRect->left)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += (index - pRect->left) >> shift;

						// shift��ŭ �ٿ�������.
						colorCount >>= shift;

						// �̹� �ܰ�� ��� ���
						//memcpy(pDestTemp, pPixels, colorCount<<1);
						// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
						memcpyShadow4444(pDestTemp, colorCount);

						pDestTemp += colorCount;
						//pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���
					//---------------------------------------------
					else
					{
						dist = pRect->left - index;

						// shift��ŭ �ٿ�������.
						colorCount = (colorCount-dist) >> shift;

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
						// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
						memcpyShadow4444(pDestTemp, colorCount);

						pDestTemp += colorCount;
						//pPixels += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				//pPixels += colorCount;
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
					pDestTemp += transCount>>shift;			

					// shift��ŭ �ٿ�������.
					colorCount >>= shift;
					
					// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
					memcpyShadow4444(pDestTemp, colorCount);

					// memory addr ����
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
// ������ clipping.  
// pRect->right�� ������ ���� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CShadowSprite::BltSmall4444ClipRight(WORD* pDest, WORD pitch, RECT* pRect, WORD pixel, BYTE shift)
{
	// Shadow���� ����
	s_wValue1 = pixel;

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

	int stepY = 1 << shift;		// y�� �ǳʶ�� pixel��
	int endY = pRect->bottom << shift;
	for (i=pRect->top; i<endY; i+=stepY)
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
				transCount = *pPixels++ >> shift;		// ������ ��			
				colorCount = *pPixels++ >> shift;		// ���� �ƴ� �� ��		
						
				// ��������ŭ index����
				index += transCount;
				
				// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
				// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

				// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

				//---------------------------------------------
				// ������ ������ �������� ���
				//---------------------------------------------			
				if (index+colorCount > pRect->right)
				{
					// ������������ �� ����� �ʿ䰡 ���� ��
					if (index > pRect->right)
					{
						break;
					}
					// ������ �ƴ� ���� ���� ����ؾ� �� ���
					else
					{
						pDestTemp += transCount;
					
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						//memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
						// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
						memcpyShadow4444(pDestTemp, (pRect->right - index));

						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				index += colorCount;
				
				// ���
				//memcpy(pDestTemp, pPixels, colorCount<<1);
				// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
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
// ���� clipping.  
// pRect->left���� ���� �ǳʶ� �������� pDest�� ����ϴٰ�
// pRect->Right������ ����Ѵ�.
//----------------------------------------------------------------------
void
CShadowSprite::BltSmall4444ClipWidth(WORD* pDest, WORD pitch, RECT* pRect, WORD pixel, BYTE shift)
{
	// Shadow���� ����
	s_wValue1 = pixel;

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

	register short i;
	register short j;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int stepY = 1 << shift;		// y�� �ǳʶ�� pixel��
	int endY = pRect->bottom << shift;
	for (i=pRect->top; i<endY; i+=stepY)
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
				if (index+colorCount > pRect->left)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > pRect->left)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += (index - pRect->left)>>shift;

						// �̹� �ܰ�� ��� ���?
						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > pRect->right)
						{							
							// ������������ ������ �� �Ѿ�� ���
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

						// shift��ŭ �ٿ�������.
						colorCount >>= shift;
					
						memcpyShadow4444(pDestTemp, colorCount);

						pDestTemp += colorCount;
						//pPixels += colorCount;
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
						dist = pRect->left - index;

						// ������ ���� �Ѿ�� ���..
						if (index+colorCount > pRect->right)
						{						
							memcpyShadow4444(pDestTemp, (pRect->right - pRect->left)>>shift);

							j = count;						
							break;
						}

						// shift��ŭ �ٿ�������.
						index += colorCount;
						colorCount = (colorCount-dist) >> shift;

						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						//memcpy(pDestTemp, pPixels+dist, (colorCount-dist)<<1);					
						// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
						memcpyShadow4444(pDestTemp, colorCount);

						pDestTemp += colorCount;
						//pPixels += colorCount;		
						

						// �������ʹ� ��� ����Ѵ�.					
						break;
					}
				}					

				// ������ �ƴ� ����ŭ index����				
				//pPixels += colorCount;
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
					if (index+colorCount > pRect->right)
					{
						// ������������ �� ����� �ʿ䰡 ���� ��
						if (index > pRect->right)
						{
							break;
						}
						// ������ �ƴ� ���� ���� ����ؾ� �� ���
						else
						{
							pDestTemp += transCount>>shift;
						
							// ������ �ƴ� ������ Surface�� ����Ѵ�.
							//memcpy(pDestTemp, pPixels, (pRect->right - index)<<1);
							// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
							memcpyShadow4444(pDestTemp, (pRect->right - index)>>shift);

							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount>>shift;


					index += colorCount;
					// shift��ŭ �ٿ��ش�.
					colorCount >>= shift;

					// ���
					//memcpy(pDestTemp, pPixels, colorCount<<1);
					// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
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
// pRect->top, pRect->bottom��ŭ�� ����Ѵ�.
//----------------------------------------------------------------------
void
CShadowSprite::BltSmall4444ClipHeight(WORD *pDest, WORD pitch, RECT* pRect, WORD pixel, BYTE shift)
{
	// Shadow���� ����
	s_wValue1 = pixel;

	int		count,			
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int i;
	register int j;
	
	int stepY = 1 << shift;		// y�� �ǳʶ�� pixel��
	int endY = pRect->bottom << shift;
	for (i=pRect->top; i<endY; i+=stepY)
	{			
		pPixels		= m_Pixels[i];
		pDestTemp	= pDest;

		// (������,�����,�����)�� �ݺ� ��		
		count	= *pPixels++;		

		if (count > 0)
		{			
			j = count;
			do
			{
				pDestTemp += ((*pPixels++)>>shift);		// ��������ŭ �ǳ� �ڴ�.
				colorCount = *pPixels++;				// ���� �ƴ� �� ��
				
				colorCount >>= shift;

				// 0��(������, 5:5:5�� 5:6:5�� �Ȱ���)������ ���
				memcpyShadow4444(pDestTemp, colorCount);
		
				pDestTemp	+= colorCount;
				//pPixels		+= colorCount;
			} while (--j);
		}
		

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

