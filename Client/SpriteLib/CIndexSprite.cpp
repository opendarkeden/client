//----------------------------------------------------------------------
// CIndexSprite.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "CSprite.h"
#include "CSpriteSurface.h"
#include "CIndexSprite.h"

#ifdef SPRITELIB_BACKEND_SDL
#include "SpriteLibBackend.h"
#endif

//-----------------------------------------------------------------------------
// Static Member
//-----------------------------------------------------------------------------
WORD	CIndexSprite::ColorSet[MAX_COLORSET][MAX_COLORGRADATION];
WORD	CIndexSprite::GradationValue[MAX_COLORGRADATION];
WORD	CIndexSprite::ColorSetDarkness[MAX_DARKBIT][MAX_COLORSET][MAX_COLORGRADATION];
BYTE	CIndexSprite::ColorToGradation[MAX_COLOR_TO_GRADATION];

//----------------------------------------------------------------------
// Static member
//----------------------------------------------------------------------
WORD	CIndexSprite::s_Colorkey = 0;
int		CIndexSprite::s_IndexValue[MAX_COLORSET_USE];

//-----------------------------------------------------------------------------
// Blt Color���� �⺻������ ����� ColorSet
//-----------------------------------------------------------------------------
const int defaultGradationColor = 384;

//-----------------------------------------------------------------------------
// memcpy Effect
//-----------------------------------------------------------------------------
FUNCTION_MEMCPYEFFECT 	CIndexSprite::s_pMemcpyEffectFunction = memcpyEffectDarker;
FUNCTION_MEMCPYEFFECT	CIndexSprite::s_pMemcpyEffectFunctionTable[MAX_EFFECT] =
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
	memcpyEffectGrayScaleVarious
};

//-----------------------------------------------------------------------------
//
// Static Functions
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Set ColorSet
//-----------------------------------------------------------------------------
void
CIndexSprite::SetColorSet()
{
	int i,j,k, set;
	WORD color;


	/*
	static BYTE rgbPoint[MAX_COLORSET_SEED][3] = 
	{
		//                     �⺻��  :  ����,����,��ο�,
		{ 16, 16, 16 },		// ���	
		{ 31, 0, 0 },		// ����	
		{ 31, 16, 0 },		// ��Ȳ	
		{ 31, 31, 0 },		// ���	
		{ 0, 31, 16 },		// ����	
		{ 0, 31, 0 },		// �ʷ�	
		{ 0, 0, 31 },		// �Ķ�	
		{ 31, 0, 31 },		// ����	
		{ 0, 31, 31 },		// �ϴ�	
		{ 16, 0, 31 },		// ����	
		{ 16, 16, 31 },		// £�� �ϴ�
		{ 31, 16, 16 }		// ���		
	};
	*/

	static BYTE rgbPoint[MAX_COLORSET_SEED][3] = 
	{
		{ 0, 0, 31 },
		{ 0, 31, 0 },
		{ 31, 0, 0 },
		{ 0, 31, 31 },
		{ 31, 0, 31 },
		{ 31, 31, 0 },

		{ 0, 0, 16 },
		{ 0, 16, 0 },
		{ 16, 0, 0 },
		{ 0, 16, 16 },
		{ 16, 0, 16 },
		{ 16, 16, 0 },	

		{ 16, 31, 0 },
		{ 16, 0, 31 },
		{ 31, 16, 0 },
		{ 0, 16, 31 },
		{ 31, 0, 16 },
		{ 0, 31, 16 },

		{ 16, 31, 16 },
		{ 16, 16, 31 },
		{ 31, 16, 16 },

		{ 16, 31, 31 },
		{ 31, 16, 31 },
		{ 31, 31, 16 },

		{ 16, 16, 16 }, // ȸ��
		{ 24, 24, 24 }, // ���� ȸ��
		{ 8, 8, 8 }, // ��ο� ȸ��		

		{ 30, 24, 18 }, // ���
		{ 25, 15, 11 },	// ����		
		{ 21, 12, 11 },				
		{ 19, 15, 13 }, // ������				

		{ 21, 18, 11 }, // ���� ���		

		{ 22, 16, 9 } // ���		
	};

		

	//----------------------------------------------------------------------
	// ColorIndex Table 
	//----------------------------------------------------------------------
	set = 0;
	int r,g,b;

	for (i=0; i<MAX_COLORSET_SEED; i++)
	{
		r = rgbPoint[i][0];
		g = rgbPoint[i][1];
		b = rgbPoint[i][2];
		
		// MAX_COLORGRADATION_HALF ~ 1
		for (j=MAX_COLORGRADATION_HALF; j>=1; j--)
		{
			// ù�ٸ�
			if (j==MAX_COLORGRADATION_HALF)
			{
				GetIndexColor(ColorSet[set], j, 
							31, 31, 31, 
							r,g,b);
			}
			else
			{
				WORD color = ColorSet[i*MAX_COLORSET_SEED_MODIFY][MAX_COLORGRADATION_HALF-j];
				int r0 = CDirectDraw::Red( color );
				int g0 = CDirectDraw::Green( color );
				int b0 = CDirectDraw::Blue( color );

				GetIndexColor(ColorSet[set], j, 
							r0, g0, b0, 
							r,g,b);
			}

			GetIndexColor(ColorSet[set]+j, MAX_COLORGRADATION-j, 
						r,g,b,
						0, 0, 0);

			set++;
		}
		/*
		// White ~ rgb ~ Black
		// White ~ rgb
		//GetIndexColor(ColorSet[set], MAX_COLORGRADATION, 
		//				31, 31, 31, 
		//				r,g,b);
		GetIndexColor(ColorSet[set], MAX_COLORGRADATION_HALF, 
						31, 31, 31, 
						r,g,b);

		GetIndexColor(ColorSet[set]+MAX_COLORGRADATION_HALF, MAX_COLORGRADATION_HALF, 
						r,g,b,
						0, 0, 0);
		
		set++;

		// White/2 ~ rgb ~~~ Black
		GetIndexColor(ColorSet[set], MAX_COLORGRADATION_1_DIV_3, 
						(31+r)/2, (31+g)/2, (31+b)/2, 
						r,g,b);

		GetIndexColor(ColorSet[set]+MAX_COLORGRADATION_1_DIV_3, MAX_COLORGRADATION_2_DIV_3, 
						r,g,b,
						0, 0, 0);		
		set++;		
		
		// rgb ~ Black		
		GetIndexColor(ColorSet[set], MAX_COLORGRADATION, 
						r,g,b,
						0, 0, 0);

		set++;
		*/
	}
	
	//----------------------------------------------------------------------
	// GradationValue��
	//----------------------------------------------------------------------
	for (j=0; j<MAX_COLORGRADATION; j++)
	{
		color = ColorSet[0][j];
		GradationValue[j] = CDirectDraw::Red(color) + CDirectDraw::Green(color) + CDirectDraw::Blue(color);
	}

	//----------------------------------------------------------------------
	// Darkness���� �����Ѵ�.
	//----------------------------------------------------------------------
	for (i=0; i<MAX_COLORSET; i++)
	{
		for (j=0; j<MAX_COLORGRADATION; j++)
		{
			color = ColorSet[i][j];
			//GradationValue[j] = CDirectDraw::Red(color) + CDirectDraw::Green(color) + CDirectDraw::Blue(color);

			// Darkness�� ���� ����
			for (k=0; k<MAX_DARKBIT; k++)
			{	
				r = ((color >> CDirectDraw::s_bSHIFT_R) >> k) << CDirectDraw::s_bSHIFT_R;
				g = (((color >> CDirectDraw::s_bSHIFT_G) & 0x1F) >> k) << CDirectDraw::s_bSHIFT_G;
				b = (color & 0x1F) >> k;
				ColorSetDarkness[k][i][j] = r | g | b;
			}
		}
	}

	//----------------------------------------------------------------------
	// Color to Gradation
	//----------------------------------------------------------------------
	for (BYTE cg=0; cg<MAX_COLOR_TO_GRADATION; cg++)
	{
		ColorToGradation[cg] = GetColorToGradation(cg);
	}

	//----------------------------------------------------------------------
	// ���� �׽�Ʈ..
	//----------------------------------------------------------------------
	/*
	int colorSet = MAX_COLORSET-1;

	int css[] = { 375, 44 };

	for (cg=0; cg<MAX_COLOR_TO_GRADATION; cg++)
	{
		ColorSet[colorSet][cg] = ColorSet[css[cg%2]][cg];
	}
	*/
}

//----------------------------------------------------------------------
// Save IndexTable To File
//----------------------------------------------------------------------
BOOL
CIndexSprite::SaveIndexTableToFile(std::ofstream& file)
{
	int i,j,k;

	int cg = MAX_COLORGRADATION;
	int cs = MAX_COLORSET;
	int db = MAX_DARKBIT;

	//----------------------------------------------------------------------
	// ������ ���ڵ�..
	//----------------------------------------------------------------------
	file.write((const char*)&cg, 4);
	file.write((const char*)&cs, 4);
	file.write((const char*)&db, 4);

	//----------------------------------------------------------------------
	// WORD	CIndexSprite::ColorSet[MAX_COLORSET][MAX_COLORGRADATION];
	//----------------------------------------------------------------------
	for (i=0; i<MAX_COLORSET; i++)
	{
		for (j=0; j<MAX_COLORGRADATION; j++)
		{
			file.write((const char*)&ColorSet[i][j], 2);
		}
	}

	//----------------------------------------------------------------------
	//	WORD	CIndexSprite::GradationValue[MAX_COLORGRADATION];
	//----------------------------------------------------------------------
	for (i=0; i<MAX_COLORSET; i++)
	{
		file.write((const char*)&GradationValue[i], 2);
	}

	//----------------------------------------------------------------------
	// WORD	CIndexSprite::ColorSetDarkness[MAX_DARKBIT][MAX_COLORSET][MAX_COLORGRADATION];
	//----------------------------------------------------------------------
	for (k=0; k<MAX_DARKBIT; k++)
	{
		for (i=0; i<MAX_COLORSET; i++)
		{
			for (j=0; j<MAX_COLORGRADATION; j++)
			{
				file.write((const char*)&ColorSetDarkness[k][i][j], 2);
			}
		}
	}

	return TRUE;
}

//----------------------------------------------------------------------
// Load IndexTable From File
//----------------------------------------------------------------------
BOOL
CIndexSprite::LoadIndexTableFromFile(std::ifstream& file)
{
	int i,j,k;

	int cg, cs, db;

	//----------------------------------------------------------------------
	// ������ ���ڵ�..
	//----------------------------------------------------------------------
	file.read((char*)&cg, 4);
	file.read((char*)&cs, 4);
	file.read((char*)&db, 4);

	if (cg != MAX_COLORGRADATION ||
		cs != MAX_COLORSET ||
		db != MAX_DARKBIT)
	{
		//return FALSE;
	}

	//----------------------------------------------------------------------
	// WORD	CIndexSprite::ColorSet[MAX_COLORSET][MAX_COLORGRADATION];
	//----------------------------------------------------------------------
	for (i=0; i<MAX_COLORSET; i++)
	{
		for (j=0; j<MAX_COLORGRADATION; j++)
		{
			file.read((char*)&ColorSet[i][j], 2);
		}
	}

	//----------------------------------------------------------------------
	//	WORD	CIndexSprite::GradationValue[MAX_COLORGRADATION];
	//----------------------------------------------------------------------
	for (i=0; i<MAX_COLORSET; i++)
	{
		file.read((char*)&GradationValue[i], 2);
	}

	//----------------------------------------------------------------------
	// WORD	CIndexSprite::ColorSetDarkness[MAX_DARKBIT][MAX_COLORSET][MAX_COLORGRADATION];
	//----------------------------------------------------------------------
	for (k=0; k<MAX_DARKBIT; k++)
	{
		for (i=0; i<MAX_COLORSET; i++)
		{
			for (j=0; j<MAX_COLORGRADATION; j++)
			{
				file.read((char*)&ColorSetDarkness[k][i][j], 2);
			}
		}
	}

	return TRUE;
}

//----------------------------------------------------------------------
// ��¿� ����� ColorSet�� �����Ѵ�.
//----------------------------------------------------------------------
void	
CIndexSprite::SetUsingColorSet(int set1, int set2)
{
	s_IndexValue[0] = set1;
	s_IndexValue[1] = set2;
}

//----------------------------------------------------------------------
// ��¿� ����� ColorSet�� �����Ѵ�.
//----------------------------------------------------------------------
void
CIndexSprite::SetUsingColorSetOnly(BYTE setNumber, int colorSet)
{
	s_IndexValue[setNumber] = colorSet;
}


//----------------------------------------------------------------------
// (r0,g0,b0) ~ (r1,g1,b1) ������ step������ŭ�� ���ڸ�
// pColor�� �־��ش�.
//----------------------------------------------------------------------
void
CIndexSprite::GetIndexColor(WORD* pColor, int step, 
							int r0, int g0, int b0, 
							int r1, int g1, int b1)
{
	float r = (float)r0;
	float g = (float)g0;
	float b = (float)b0;

	float step_1 = (float)step - 1.0f;
	float sr = (r1-r0) / (float)step_1;
	float sg = (g1-g0) / (float)step_1;
	float sb = (b1-b0) / (float)step_1;

	BYTE red, green, blue;

	for (int i=0; i<step; i++)
	{
		red		= (BYTE)r;
		green	= (BYTE)g;
		blue	= (BYTE)b;
		
		*pColor++ = CDirectDraw::Color(red, green, blue);		
		
		r += sr;
		g += sg;
		b += sb;
	}
}


//----------------------------------------------------------------------
//
// constructor/destructore
//
//----------------------------------------------------------------------

CIndexSprite::CIndexSprite()
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

CIndexSprite::~CIndexSprite()
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
CIndexSprite::Release()
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
// Assign Operator
//----------------------------------------------------------------------
void
CIndexSprite::operator = (const CIndexSprite& Sprite)
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

	int colorCount, transPair, indexCount;

	// �޸� ���
	m_Pixels = new WORD* [m_Height];

	//--------------------------------
	// 5:6:5
	//--------------------------------
	for (i=0; i<m_Height; i++)
	{
		// �ݺ� ȸ���� 2 byte
		transPair = Sprite.m_Pixels[i][0];
				
		index	= 1;

		// �� line���� byte���� ��� �����ؾ��Ѵ�.
		for (j=0; j<transPair; j++)
		{			
			// transCount = m_Pixels[i][index];
			index++;	// ���� ��
			indexCount = Sprite.m_Pixels[i][index++];	// indexPair ��

			index += indexCount;		// index���� ���� ���� �� ��ŭ

			// Normal�� ��
			colorCount = Sprite.m_Pixels[i][index++];

			// Normal�� ����ŭ ����
			index	+= colorCount;
		}

		// �޸� ���
		m_Pixels[i] = new WORD [index];
		memcpy(m_Pixels[i], Sprite.m_Pixels[i], index<<1);
	}

	// ���� �Ϸ�
	m_bInit = true;
}


//-----------------------------------------------------------------------------
// Get IndexColor
//-----------------------------------------------------------------------------
// ������ gradation�� ���ؼ�
// color���� �����ϴ� ������ index��(���� ���� �ƴ� index��ȣ)�� ã���ش�.
//-----------------------------------------------------------------------------
BYTE
CIndexSprite::GetIndexColor(WORD color)
{
	//-------------------------------------------------------
	// Gradition��
	//-------------------------------------------------------
	WORD spriteGradation = CDirectDraw::Red(color) 
						+ CDirectDraw::Green(color) 
						+ CDirectDraw::Blue(color);
			
	return GetColorToGradation( spriteGradation );
}

//-----------------------------------------------------------------------------
// GetColorToGradation
//-----------------------------------------------------------------------------
// color(0~93)���� gradation(0~MAX_GRADATION)������ �ٲ۴�.
//-----------------------------------------------------------------------------
BYTE
CIndexSprite::GetColorToGradation(BYTE spriteGradation)
{
	// 5:6:5���� code
	//WORD spriteGradation = (color >> 11) + ((color >> 6) & 0x1F) + (color & 0x1F);
	// 5:5:5���� code
	//WORD spriteGradation = (color >> 10) + ((color >> 5) & 0x1F) + (color & 0x1F);

	//-------------------------------------------------------
	// spriteGradation���� ���� ����� 
	// GradationValue�� ã�ƾ� �Ѵ�.
	//-------------------------------------------------------
	for (int g=0; g<MAX_COLORGRADATION; g++)
	{			
		if (spriteGradation > GradationValue[g])
		{
			break;
		}
	}

	// ���� ���� ������ ���
	if (g==0 ||	g==MAX_COLORGRADATION-1)
	{
		return g;
	}

	// ��� ����
	WORD value1 = GradationValue[g-1] - spriteGradation;
	WORD value2 = spriteGradation - GradationValue[g-1];
	
	// ���� ���� �����Ѵ�.
	if (value1 < value2)
	{
		return g-1;
	}
	else if (value1 > value2)
	{
		return g;
	}
	
	// ���� ����.. ??	
	return g-1;	
}

//-----------------------------------------------------------------------------
// Set Pixel
//-----------------------------------------------------------------------------
// pixel����(pSource)��
// �� ���� index ������ ������ 
// IndexSprite�� �����Ѵ�.
//
// �켱 ColorInfo�� IndexInfo�� �����ϰ�..
// GenerateFromIndex�� ȣ���Ѵ�..
//
// ȿ���� �� ���������� -_-; ����ȭ�ϱ� ������. reuse���.. 
// ������ ���� �����߿��� ���� ���� �����ϱ�.. ����.. -_-;;;; 
//-----------------------------------------------------------------------------
void		
CIndexSprite::SetPixel(WORD* pSource, WORD sourcePitch, 
							 WORD* pIndex1, WORD indexPitch1,	// ù��° index
							 WORD* pIndex2, WORD indexPitch2,	// �ι�° index
							 WORD width, WORD height)
{
	//--------------------------------------------------
	// ���̰� 0�϶�..
	//--------------------------------------------------
	if (width==0 || height==0)
	{
		Release();
		return;
	}

	WORD	*pSourceTemp, *pIndexTemp1, *pIndexTemp2;
	WORD	transIndex1=0, transIndex2=0;	// default �������� 0��(����)

	//--------------------------------------------------
	// Index������ �ִ� surface�� �������� �����Ѵ�.
	//--------------------------------------------------
	// pSource�� �������� �ִ� ��ġ��
	// pIndex1, pIndex2������ �������̴�.
	//--------------------------------------------------
	WORD	*pSourceTemp2;
	pSourceTemp = pSource;

	for (register i=0; i<height; i++)
	{
		pSourceTemp2 = pSourceTemp;

		for (register int j=0; j<width; j++)
		{
			// ������
			if (*pSourceTemp2==s_Colorkey)
			{
				// pIndex1�� pIndex2���� ���� �������� �����Ѵ�.
				if (pIndex1!=NULL)
				{
					transIndex1 = *(WORD*)((BYTE*)pIndex1 + i*indexPitch1 + (j<<1));
				}

				if (pIndex2!=NULL)
				{
					transIndex2 = *(WORD*)((BYTE*)pIndex2 + i*indexPitch2 + (j<<1));
				}

				i = height;
				break;
			}

			pSourceTemp2++;
		}

		pSourceTemp = (WORD*)((BYTE*)pSourceTemp + sourcePitch);
	}
	

	//--------------------------------------------------
	// ���� ������ ���� �޸� ���
	//--------------------------------------------------
	WORD** ppColor, **ppIndex;

	ppColor = new WORD* [height];
	ppIndex = new WORD* [height];

	//--------------------------------------------------
	// üũ üũ~
	//--------------------------------------------------
	for (i=0; i<height; i++)
	{
		//--------------------------------------------------
		// �޸� ���
		//--------------------------------------------------
		ppColor[i] = new WORD [width];
		ppIndex[i] = new WORD [width];

		//--------------------------------------------------
		// ù ��
		//--------------------------------------------------
		pSourceTemp = pSource;

		if (pIndex1!=NULL)
		{
			pIndexTemp1 = pIndex1;
		}

		if (pIndex2!=NULL)
		{
			pIndexTemp2 = pIndex2;
		}

		//--------------------------------------------------
		// �� ���� üũ..
		//--------------------------------------------------
		for (register int j=0; j<width; j++)
		{
			//--------------------------------------------------
			// �������̶��..
			//--------------------------------------------------
			if (*pSourceTemp==s_Colorkey)
			{
				ppIndex[i][j] = INDEX_TRANS;
			}
			//--------------------------------------------------
			// ���� Index�������� üũ�ϰ�, 
			// �ƴϸ�, ���� ���� ����̴�.
			//--------------------------------------------------
			else
			{
				//--------------------------------------------------
				// index1�� �ִ� ������ üũ�ؾ� �Ѵ�.
				//--------------------------------------------------
				if (pIndex1!=NULL && *pIndexTemp1!=transIndex1)
				{
					ppIndex[i][j] = GetIndexColor( *pSourceTemp );	// (0<<8) | gradation;
				}
				//--------------------------------------------------
				// index2�� �ִ� ������ üũ�ؾ� �Ѵ�.
				//--------------------------------------------------
				else if (pIndex2!=NULL && *pIndexTemp2!=transIndex2)
				{
					ppIndex[i][j] = (1<<8) | GetIndexColor( *pSourceTemp );
				}
				//--------------------------------------------------
				// �� �� ���� ��� = ���� ��
				//--------------------------------------------------
				else
				{
					ppIndex[i][j] = INDEX_NULL;

					// ���� ���� �����Ѵ�.
					ppColor[i][j] = *pSourceTemp;
				}				
			}

			pSourceTemp++;

			//--------------------------------------------------
			// ���� ��
			//--------------------------------------------------
			if (pIndex1!=NULL)
			{
				pIndexTemp1++;
			}

			if (pIndex2!=NULL)
			{
				pIndexTemp2++;
			}
		}

		//--------------------------------------------------
		// ���� ��
		//--------------------------------------------------
		pSource = (WORD*)((BYTE*)pSource + sourcePitch);

		if (pIndex1!=NULL)
		{
			pIndex1 = (WORD*)((BYTE*)pIndex1 + indexPitch1);
		}

		if (pIndex2!=NULL)
		{
			pIndex2 = (WORD*)((BYTE*)pIndex2 + indexPitch2);
		}
	}


	//--------------------------------------------------
	// ppColor�� ppIndex�� �ϼ��Ǿ����Ƿ�.. 
	// IndexSprite����!
	//--------------------------------------------------
	GenerateFromIndex(ppColor, width, height, ppIndex);


	//--------------------------------------------------
	// ppColor�� ppIndex�� �����ش�.
	//--------------------------------------------------
	for (i=0; i<height; i++)
	{
		delete [] ppColor[i];
		delete [] ppIndex[i];
	}
	
	delete [] ppColor;
	delete [] ppIndex;
}


//-----------------------------------------------------------------------------
// Generate Index 2
//-----------------------------------------------------------------------------
// 
// ppColor���� width*height��ŭ�� ���� �о Sprite�� ����µ�..
// ������ ���൵ �ؾ��ϰ�...
// Index Array�� ������ �����ؾ��Ѵ�.
//
//-----------------------------------------------------------------------------
// - ���������� ������ ������ ����Ѵ�.
// - IndexSet�� Gradation���򺰷� ����(?)�Ѵ�. (ȿ���� ���� ���� �� �ϴ�)
//
//
// [ �� ���� ���� ]
//
// (����Pair�ݺ���) 
//    ( (��������, Index����, 
//                 (indexSet��ȣ, gradation��ȣ), 
//                 (indexSet��ȣ, gradation��ȣ), 
//                              .     
//                              .     
//                              .     
//    ( (��������, Index����, 
//                 (indexSet��ȣ, gradation��ȣ), 
//                 (indexSet��ȣ, gradation��ȣ), 
//                              .     
//                              .     
//                              .     
//        �����Ȼ����, �����Ȼ����)
//    )
//
// WORD : �������� 
// WORD : ��Pair�ݺ���
// WORD : index����
// WORD : indexSet��ȣ | gradation��ȣ --> BYTE | BYTE
// WORD : �����Ȼ����
// WORD*: �����Ȼ����
//
// �������� Gradation Index���� ������ ���� �����Ѵ�.
//-----------------------------------------------------------------------------
// ppIndex���� �� 2byte�� 
// ((IndexSet��ȣ<<8) | Gradation��ȣ)�� ����Ǿ� �־�� �Ѵ�.
//-----------------------------------------------------------------------------
// Index�� �˻��ϸ鼭.. 
// �������� ���� ������ counter����..
// �ƴϸ�.. IndexSet���� ���� Gradation���� ����..
//-----------------------------------------------------------------------------
void			
CIndexSprite::GenerateFromIndex(WORD** ppColor,
								WORD width, WORD height,
								WORD** ppIndex)
{
	// memory����
	Release();

	m_Width = width;
	m_Height = height;

	// �ϴ� memory�� ������ ��Ƶд�.	
	WORD*	data = new WORD[m_Width*8+10];

	int	index;				// data�� index�� ���
			
	int	transPairCount,					// ����Pair ����
			transColorCount,				// ������ ����			
			indexColorCount,				// Index�� ����
			normalColorCount;				// NormalColor ����

	int	indexColorCountPosition,		// IndexColor���� ���� ��ġ
			normalColorCountPosition;		// NormalColor���� ���� ��ġ

	enum	COLORTYPE { COLORTYPE_TRANS, COLORTYPE_INDEX, COLORTYPE_NORMAL }
			lastColorType;			// �ֱٿ� �˻��� ���� ����
								

	// height�� ��ŭ memory���
	m_Pixels = new WORD* [height];


	//-----------------------------------
	// test code
	//-----------------------------------
	//int* m_pLineIndex = new int [height];


	for (register int  i=0; i<height; i++)
	{
		index = 0;
		transPairCount = 0;
		transColorCount = 0; 		
		lastColorType = COLORTYPE_TRANS;

		//------------------------------------------------------------
		// �� line�� ���ؼ� ����~
		//------------------------------------------------------------
		for (register int  j=0; j<width; j++)
		{
			//--------------------------------------------------
			// Index�� �������� ���
			//--------------------------------------------------
			if ((ppIndex[i][j] & 0xFF)==INDEX_TRANS)
			{
				//--------------------------------------------------
				// �ֱٿ� �˻��Ѱ� index���̾��ٸ�,
				//--------------------------------------------------
				if (lastColorType == COLORTYPE_INDEX)
				{
					// ���� index������ �����ؾ��Ѵ�.
					data[indexColorCountPosition] = indexColorCount;

					// ' (��������,��Pair�ݺ���, (��Pair��....)) '�� �� set�� �������� �ǹ��ϹǷ�
					// normal�� ���� 0�����ؼ� �����Ѵ�.
					transPairCount++;
					
					data[index++] = 0;					

					transColorCount = 0;
					lastColorType = COLORTYPE_TRANS;
				}
				//--------------------------------------------------
				// �ֱٿ� �˻��Ѱ� normal���̾��ٸ�
				//--------------------------------------------------
				else if (lastColorType == COLORTYPE_NORMAL)
				{
					// ' (��������,indexPair�ݺ���, (index�ݺ�), ��������)) '�� �� set�� �������� �ǹ��ϹǷ�
					transPairCount++;
					
					data[normalColorCountPosition] = normalColorCount;
					normalColorCount = 0;

					transColorCount = 0;
					lastColorType = COLORTYPE_TRANS;
				}

				
				transColorCount++;				
			}		
			//--------------------------------------------------				
			// Index ���� ���...
			//--------------------------------------------------
			else if ((ppIndex[i][j] & 0xFF)!=INDEX_NULL)
			{
				//--------------------------------------------------
				// ������ ���� �������̾��ٸ�..
				//--------------------------------------------------
				if (lastColorType == COLORTYPE_TRANS)
				{						
					data[index++] = transColorCount;		// ���� byte�� �������� �ִ´�.
					transColorCount = 0;
					
					// ���ο� index�� ���� ���� �ʱ�ȭ..
					// index���� ������ ������ ��ġ
					indexColorCountPosition = index++;
					indexColorCount	= 0;
					
					lastColorType = COLORTYPE_INDEX;
				}
				//--------------------------------------------------
				// ������ ���� �������ʴ� ���� ���
				//--------------------------------------------------
				else if (lastColorType == COLORTYPE_NORMAL)
				{
					// ������ �ʴ� �� ��
					// ' (��������,indexPair�ݺ���, (index�ݺ�), ��������)) '�� �� set�� �������� �ǹ��ϹǷ�
					transPairCount++;
					
					data[normalColorCountPosition] = normalColorCount;
					normalColorCount = 0;

					// ������ ����.. - -;
					data[index++] = 0;

					// index���� ������ ������ ��ġ
					indexColorCountPosition = index++;
					indexColorCount	= 0;					

					lastColorType = COLORTYPE_INDEX;						
				}	
				
				// index�� ���� ����
				indexColorCount++;

				// index�� ����				
				data[index++]	= ppIndex[i][j];
			}
			//--------------------------------------------------				
			// ������ �ʴ� ���� ��� (INDEX_NULL)
			//--------------------------------------------------
			else //if ((ppIndex[i][j] & 0xFF)==INDEX_NULL)
			{	
				//--------------------------------------------------
				// ���� ������ �������̸�..
				//--------------------------------------------------
				if (lastColorType == COLORTYPE_TRANS)
				{
					data[index++] = transColorCount;
					transColorCount = 0;
						
					// indexCount�� ����.
					data[index++] = 0;	
						
					// ��������� �������Ƿ� �ʱ�ȭ.
					normalColorCountPosition = index++;
					normalColorCount = 0;

					lastColorType = COLORTYPE_NORMAL;	
				}
				else
				//--------------------------------------------------
				// ���� ������ index���̸�..
				//--------------------------------------------------
				if (lastColorType == COLORTYPE_INDEX)
				{
					// ���� index������ �����ؾ��Ѵ�.
					data[indexColorCountPosition] = indexColorCount;
						
					// ��������� �������Ƿ� �ʱ�ȭ.
					normalColorCountPosition = index++;
					normalColorCount = 0;

					lastColorType = COLORTYPE_NORMAL;						
				}

				data[index++] = ppColor[i][j];	// ���� ������ �����Ѵ�.
				normalColorCount++;
			}
		}
		
		//--------------------------------------------------
		// �� ���� ������ ���� �������ΰ�?
		//--------------------------------------------------
		if (lastColorType == COLORTYPE_TRANS)
		{
			// �������̸� ���ٸ� ó���� �����൵ �ɰ� ����.
		}	
		//--------------------------------------------------
		// ������ ���� index���� ��� - indexPair����..
		//--------------------------------------------------
		else if (lastColorType == COLORTYPE_INDEX)
		{		
			// ���� index������ �����ؾ��Ѵ�.
			data[indexColorCountPosition] = indexColorCount;

			// normal���� ����.
			data[index++] = 0;

			// ' (��������,indexPair�ݺ���, (index�ݺ�), ��������)) '�� �� set�� �������� �ǹ��ϹǷ�
			transPairCount++;			
		}
		//--------------------------------------------------
		// ������ ���� normal���� ���
		//--------------------------------------------------
		else 
		{
			// ' (��������,indexPair�ݺ���, (index�ݺ�), ��������)) '�� �� set�� �������� �ǹ��ϹǷ�
			data[normalColorCountPosition] = normalColorCount;	

			transPairCount++;		
		}

		// memory�� ��´�.
		m_Pixels[i] = new WORD [index+1];

		// m_Pixels[i]�� ���������Ƿ� data�� ��ü�Ѵ�.
		// m_Pixels[i][0]���� count�� �־�� �Ѵ�.
		m_Pixels[i][0] = transPairCount;
		memcpy(m_Pixels[i]+1, data, index<<1);

		//m_pLineIndex[i] = index+1;
	}

	delete [] data;

	m_bInit = true;

	/*
	ofstream indexFile("index.txt");
	ofstream colorFile("color.txt");
	ofstream spriteFile("sprite.txt");

	for (i=0; i<m_Height; i++)
	{
		for (int j=0; j<m_Width; j++)
		{
			indexFile << (int)ppIndex[i][j] << " ";
			colorFile << (int)ppColor[i][j] << " ";
		}
		indexFile << endl;
		colorFile << endl;

		for (j=0; j<m_pLineIndex[i]; j++)
		{
			spriteFile << (int)m_Pixels[i][j] << " ";
		}

		spriteFile << endl;
	}
	delete [] m_pLineIndex;
	*/
}



//-----------------------------------------------------------------------------
// Generate Index (Old version)
//-----------------------------------------------------------------------------
// 
// ppColor���� width*height��ŭ�� ���� �о Sprite�� ����µ�..
// ������ ���൵ �ؾ��ϰ�...
// Index Array�� ������ �����ؾ��Ѵ�.
//
//-----------------------------------------------------------------------------
// - ���������� ������ ������ ����Ѵ�.
// - IndexSet�� Gradation���򺰷� ����(?)�Ѵ�. (ȿ���� ���� ���� �� �ϴ�)
//
//
// [ �� ���� ���� ]
//
// (����Pair�ݺ���) 
//    ( (��������, IndexPair�ݺ���, 
//                 (index����, indexSet��ȣ, gradation��ȣ), 
//                 (index����, indexSet��ȣ, gradation��ȣ), 
//                              .     
//                              .     
//                              .     
//        �����Ȼ����, �����Ȼ����)
//       (��������, IndexPair�ݺ���, 
//                 (index����, indexSet��ȣ, gradation��ȣ), 
//                 (index����, indexSet��ȣ, gradation��ȣ), 
//                              .     
//                              .     
//                              .     
//        �����Ȼ����, �����Ȼ����)
//    )
//
// WORD : �������� 
// WORD : ��Pair�ݺ���
// WORD : index����
// WORD : indexSet��ȣ | gradation��ȣ --> BYTE | BYTE
// WORD : �����Ȼ����
// WORD*: �����Ȼ����
//
// �������� Gradation Index���� ������ ���� �����Ѵ�.
//-----------------------------------------------------------------------------
// ppIndex���� �� 2byte�� 
// ((IndexSet��ȣ<<8) | Gradation��ȣ)�� ����Ǿ� �־�� �Ѵ�.
//-----------------------------------------------------------------------------
// Index�� �˻��ϸ鼭.. 
// �������� ���� ������ counter����..
// �ƴϸ�.. IndexSet���� ���� Gradation���� ����..
//-----------------------------------------------------------------------------
/*
void			
CIndexSprite::GenerateFromIndex(WORD** ppColor,
								WORD width, WORD height,
								WORD** ppIndex)
{
	// memory����
	Release();

	m_Width = width;
	m_Height = height;

	// �ϴ� memory�� ������ ��Ƶд�.	
	WORD*	data = new WORD[m_Width*8];

	WORD	index;				// data�� index�� ���
			
	WORD	transPairCount,					// ����Pair ����
			transColorCount,				// ������ ����
			indexColorPairCount,			// IndexPair����
			indexColorCount,				// Index�� ����
			normalColorCount;				// NormalColor ����

	WORD	indexColorPairCountPosition,	// IndexPair���� ���� ��ġ
			indexColorCountPosition,		// IndexColor���� ���� ��ġ
			normalColorCountPosition;		// NormalColor���� ���� ��ġ

	enum	COLORTYPE { COLORTYPE_TRANS, COLORTYPE_INDEX, COLORTYPE_NORMAL }
			lastColorType;			// �ֱٿ� �˻��� ���� ����
								
	BYTE	lastIndexSet;		// �ֱٿ� �˻��� IndexSet
	BYTE	lastIndexColor;		// �ֱٿ� �˻��� IndexGradation


	// height�� ��ŭ memory���
	m_Pixels = new WORD* [height];


	// test code
	//int* m_pLineIndex = new int [height];


	for (register int  i=0; i<height; i++)
	{
		index = 0;
		transPairCount = 0;
		transColorCount = 0; 		
		lastColorType = COLORTYPE_TRANS;

		//------------------------------------------------------------
		// �� line�� ���ؼ� ����~
		//------------------------------------------------------------
		for (register int  j=0; j<width; j++)
		{
			//--------------------------------------------------
			// Index�� �������� ���
			//--------------------------------------------------
			if ((ppIndex[i][j] & 0xFF)==INDEX_TRANS)
			{
				//--------------------------------------------------
				// �ֱٿ� �˻��Ѱ� index���̾��ٸ�,
				//--------------------------------------------------
				if (lastColorType == COLORTYPE_INDEX)
				{
					// ���� index������ �����ؾ��Ѵ�.
					data[indexColorCountPosition] = indexColorCount;

					// indexPair���� �ϳ� ����
					indexColorPairCount++;

					// indexPair�� ��� �������Ƿ�.. �����ؾ��Ѵ�.
					data[indexColorPairCountPosition] = indexColorPairCount;
					indexColorPairCount = 0;

					// ' (��������,��Pair�ݺ���, (��Pair��....)) '�� �� set�� �������� �ǹ��ϹǷ�
					// normal�� ���� 0�����ؼ� �����Ѵ�.
					transPairCount++;
					
					data[index++] = 0;					

					transColorCount = 0;
					lastColorType = COLORTYPE_TRANS;
				}
				//--------------------------------------------------
				// �ֱٿ� �˻��Ѱ� normal���̾��ٸ�
				//--------------------------------------------------
				else if (lastColorType == COLORTYPE_NORMAL)
				{
					// ' (��������,indexPair�ݺ���, (index�ݺ�), ��������)) '�� �� set�� �������� �ǹ��ϹǷ�
					transPairCount++;
					
					data[normalColorCountPosition] = normalColorCount;
					normalColorCount = 0;

					transColorCount = 0;
					lastColorType = COLORTYPE_TRANS;
				}

				
				transColorCount++;				
			}		
			//--------------------------------------------------				
			// Index ���� ���...
			//--------------------------------------------------
			else if ((ppIndex[i][j] & 0xFF)!=INDEX_NULL)
			{
				//--------------------------------------------------
				// ������ ���� �������̾��ٸ�..
				//--------------------------------------------------
				if (lastColorType == COLORTYPE_TRANS)
				{						
					data[index++] = transColorCount;		// ���� byte�� �������� �ִ´�.
					transColorCount = 0;
					
					// ������ [IndexPair����]�� ���� ��ġ�� ���
					indexColorPairCountPosition = index++;
					indexColorPairCount = 0;		// index����

					// ���ο� index�� ���� ���� �ʱ�ȭ..
					// index���� ������ ������ ��ġ
					indexColorCountPosition = index++;
					indexColorCount	= 1;

					// index���� ó�� �����Ƿ�.. �ʱ�ȭ..
					lastIndexSet	= (ppIndex[i][j]>>8) & 0xFF;
					lastIndexColor	= ppIndex[i][j] & 0xFF;	

					// ����
					data[index++]	= (lastIndexSet<<8) | lastIndexColor;
					
					lastColorType = COLORTYPE_INDEX;
				}
				//--------------------------------------------------
				// ������ ���� �������ʴ� ���� ���
				//--------------------------------------------------
				else if (lastColorType == COLORTYPE_NORMAL)
				{
					// ������ �ʴ� �� ��
					// ' (��������,indexPair�ݺ���, (index�ݺ�), ��������)) '�� �� set�� �������� �ǹ��ϹǷ�
					transPairCount++;
					
					data[normalColorCountPosition] = normalColorCount;
					normalColorCount = 0;

					// ������ ����.. - -;
					data[index++] = 0;

					// ������ [IndexPair����]�� ���� ��ġ�� ���
					indexColorPairCountPosition = index++;
					indexColorPairCount = 0;		// index����

					// index���� ������ ������ ��ġ
					indexColorCountPosition = index++;
					indexColorCount	= 1;

					// index���� ó�� �����Ƿ�.. �ʱ�ȭ..
					lastIndexSet	= (ppIndex[i][j]>>8) & 0xFF;
					lastIndexColor	= ppIndex[i][j] & 0xFF;

					// ����
					data[index++]	= (lastIndexSet<<8) | lastIndexColor;

					lastColorType = COLORTYPE_INDEX;						
				}	
				//--------------------------------------------------
				// ������ ���� index���� ���
				//--------------------------------------------------
				else 			
				{
					//--------------------------------------------------
					// ���� index���̶� Set���� Gradation���� ���� ���
					//--------------------------------------------------
					if (lastIndexSet == ((ppIndex[i][j]>>8) & 0xFF)
						&& lastIndexColor == (ppIndex[i][j] & 0xFF))
					{
						// ���� index���̹Ƿ� count����
						indexColorCount++;
					}
					//--------------------------------------------------
					// �ٸ� index���� ��Ÿ����.
					//--------------------------------------------------
					else
					{	
						// ���� index������ �����ؾ��Ѵ�.
						data[indexColorCountPosition] = indexColorCount;						

						// indexPair���� �ϳ� ����
						indexColorPairCount++;

						// ���ο� index�� ���� ���� �ʱ�ȭ..
						// index���� ������ ������ ��ġ
						indexColorCountPosition = index++;
						indexColorCount	= 1;

						// index���� ó�� �����Ƿ�.. �ʱ�ȭ..
						lastIndexSet	= (ppIndex[i][j]>>8) & 0xFF;
						lastIndexColor	= ppIndex[i][j] & 0xFF;	

						// ����
						data[index++]	= (lastIndexSet<<8) | lastIndexColor;
					}							
				}
			}
			//--------------------------------------------------				
			// ������ �ʴ� ���� ��� (INDEX_NULL)
			//--------------------------------------------------
			else //if ((ppIndex[i][j] & 0xFF)==INDEX_NULL)
			{	
				//--------------------------------------------------
				// ���� ������ �������̸�..
				//--------------------------------------------------
				if (lastColorType == COLORTYPE_TRANS)
				{
					data[index++] = transColorCount;
					transColorCount = 0;
						
					// indexPair�� ����.
					data[index++] = 0;	
						
					// ��������� �������Ƿ� �ʱ�ȭ.
					normalColorCountPosition = index++;
					normalColorCount = 0;

					lastColorType = COLORTYPE_NORMAL;	
				}
				else
				//--------------------------------------------------
				// ���� ������ index���̸�..
				//--------------------------------------------------
				if (lastColorType == COLORTYPE_INDEX)
				{
					// ���� index������ �����ؾ��Ѵ�.
					data[indexColorCountPosition] = indexColorCount;

					// indexPair���� �ϳ� ����
					indexColorPairCount++;

					// indexPair�� ��� �������Ƿ�.. �����ؾ��Ѵ�.
					data[indexColorPairCountPosition] = indexColorPairCount;
					indexColorPairCount = 0;

						
					// ��������� �������Ƿ� �ʱ�ȭ.
					normalColorCountPosition = index++;
					normalColorCount = 0;

					lastColorType = COLORTYPE_NORMAL;						
				}

				data[index++] = ppColor[i][j];	// ���� ������ �����Ѵ�.
				normalColorCount++;
			}
		}
		
		//--------------------------------------------------
		// �� ���� ������ ���� �������ΰ�?
		//--------------------------------------------------
		if (lastColorType == COLORTYPE_TRANS)
		{
			// �������̸� ���ٸ� ó���� �����൵ �ɰ� ����.
		}	
		//--------------------------------------------------
		// ������ ���� index���� ��� - indexPair����..
		//--------------------------------------------------
		else if (lastColorType == COLORTYPE_INDEX)
		{		
			// ���� index������ �����ؾ��Ѵ�.
			data[indexColorCountPosition] = indexColorCount;

			// indexPair���� �ϳ� ����
			indexColorPairCount++;

			// indexPair�� ��� �������Ƿ�.. �����ؾ��Ѵ�.
			data[indexColorPairCountPosition] = indexColorPairCount;
			indexColorPairCount = 0;

			// normal���� ����.
			data[index++] = 0;

			// ' (��������,indexPair�ݺ���, (index�ݺ�), ��������)) '�� �� set�� �������� �ǹ��ϹǷ�
			transPairCount++;			
		}
		//--------------------------------------------------
		// ������ ���� normal���� ���
		//--------------------------------------------------
		else 
		{
			// ' (��������,indexPair�ݺ���, (index�ݺ�), ��������)) '�� �� set�� �������� �ǹ��ϹǷ�
			data[normalColorCountPosition] = normalColorCount;	

			transPairCount++;		
		}

		// memory�� ��´�.
		m_Pixels[i] = new WORD [index+1];

		// m_Pixels[i]�� ���������Ƿ� data�� ��ü�Ѵ�.
		// m_Pixels[i][0]���� count�� �־�� �Ѵ�.
		m_Pixels[i][0] = transPairCount;
		memcpy(m_Pixels[i]+1, data, index<<1);

		//m_pLineIndex[i] = index+1;
	}

	delete [] data;


	
	//ofstream indexFile("index.txt");
	//ofstream colorFile("color.txt");
	//ofstream spriteFile("sprite.txt");

	//for (i=0; i<m_Height; i++)
	//{
		//for (int j=0; j<m_Width; j++)
		//{
			//indexFile << (int)ppIndex[i][j] << " ";
			//colorFile << (int)ppColor[i][j] << " ";
		//}
		//indexFile << endl;
		//colorFile << endl;

		//for (j=0; j<m_pLineIndex[i]; j++)
		//{
			//spriteFile << (int)m_Pixels[i][j] << " ";
		//}

		//spriteFile << endl;
	//}
	//delete [] m_pLineIndex;
	
}
*/


//-----------------------------------------------------------------------------
// Get IndexInfo
//-----------------------------------------------------------------------------
// ���� ����� m_Pixels�� ������ IndexInfo�� �����ؾ� �Ѵ�.
// ppIndex���� ��� INDEX_TRANS���� �� �ִٰ� �����Ѵ�.
//-----------------------------------------------------------------------------
void			
CIndexSprite::GetIndexInfo(WORD**& ppIndex)
{
	int	transPair,	
			indexCount,			
			colorCount;

	int	colorSet,
			colorGradation;

	WORD	*pDest,
			*pPixels;

	register int i;
	register int j;
	register int k;
	register int l;

	for (i=0; i<m_Height; i++)
	{			
		pPixels	= m_Pixels[i];
		pDest	= ppIndex[i];

		// (������,Index�ݺ�,�����)�� �ݺ� ��		
		transPair	= *pPixels++;
		
 		// �� �� ���
		if (transPair > 0)
		{
			j = transPair;
			do {		
				pDest += *pPixels++;	// ��������ŭ �ǳ� �ڴ�.
				indexCount = *pPixels++;		// Index�ݺ� ��

				// indexCount����ŭ index������ ����Ѵ�.
				if (indexCount > 0)
				{
					k = indexCount;
					do {			
						colorSet		= *pPixels >> 8;
						colorSet		&= 0xFF;				// set
						colorGradation	= *pPixels & 0xFF;		// gradation
						pPixels++;

						// (colorSet<<8) | colorGradation
						*pDest		= (colorSet << 8) | colorGradation;
						pDest++;
					} while (--k);
				}

				// Normal ���� ��
				colorCount = *pPixels++;
				
				// Normal ���� ����ŭ INDEX_NULL�� ���.
				for (l=0; l<colorCount; l++)
				{					
					*pDest++ = (INDEXSET_NULL<<8) | INDEX_NULL;
				}
				
				pPixels		+= colorCount;
			} while (--j);
		}
	}
}

//----------------------------------------------------------------------
// Is ColorPixel ?
//----------------------------------------------------------------------
// Sprite�ȿ��� (x,y)�� ������ �ִ°�?(�������� �ƴ� ���)
//----------------------------------------------------------------------
bool		
CIndexSprite::IsColorPixel(short x, short y)
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

		int	index = 0;

		//    ( (��������, Index����, 
		//                 (indexSet��ȣ, gradation��ȣ), 
		//                 (indexSet��ȣ, gradation��ȣ), 

		register int i;

		if (count > 0)
		{
			i = count;
			do {
				index += *pPixels++;

				// �������������� ���� ���
				if (x < index)
				{
					return false;
				}

				// index������ŭ ����		
				index += *pPixels;

				// index�� ������ŭ ����.
				pPixels += *pPixels;

				pPixels++;


				// ������ �������ŭ ����
				index += *pPixels;

				// ������ ������ŭ ����.
				pPixels += *pPixels;

				pPixels++;


				// index+�������������� ���� ���
				if (x < index)
				{
					return true;
				}
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
CIndexSprite::GetPixel(int x, int y) const
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

		int	index = 0;

		//    ( (��������, Index����, 
		//                 (indexSet��ȣ, gradation��ȣ), 
		//                 (indexSet��ȣ, gradation��ȣ), 

		register int i;

		if (count > 0)
		{
			i = count;
			do {
				index += *pPixels++;

				// �������������� ���� ���
				if (x < index)
				{
					return 0;
				}

				// index������ŭ ����		
				index += *pPixels;

				// index�� ������ŭ ����.
				pPixels += *pPixels;

				pPixels++;


				// ������ �������ŭ ����
				index += *pPixels;

				// ������ ������ŭ ����.
				pPixels += *pPixels;

				pPixels++;


				// index+�������������� ���� ���
				if (x < index)
				{
					// [ TEST CODE ]
					// ���� �ϼ��Ȱ� �ƴϴ�.
					// ������ IndexColor�� �����ؼ� ���� �Ѱ���� �Ѵ�?
					return pPixels[x-index];
				}
			} while (--i);
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// Blt Index
//-----------------------------------------------------------------------------
// s_Value1 - Main Color
// s_Value2 - Sub Color
//-----------------------------------------------------------------------------
//    ( (��������, Index����, 
//                 (indexSet��ȣ, gradation��ȣ), 
//                 (indexSet��ȣ, gradation��ȣ), 
//-----------------------------------------------------------------------------
// Clipping���� �ʴ´�.
//-----------------------------------------------------------------------------
void
CIndexSprite::Blt(WORD *pDest, WORD pitch)
{
	int	transPair,	
			indexCount,			
			colorCount;

	int	colorSet,
			colorGradation;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;
	register int k;
	int rectBottom = m_Height;	

	
	//for (i=0; i<rectBottom; i++)	
	if (rectBottom > 0)
	{
		i = rectBottom-1;
		pDest = (WORD*)((BYTE*)pDest + i*pitch);

		do
		{			
			pPixels		= m_Pixels[i];
			pDestTemp	= pDest;

			// (������,Index��,�����)�� �ݺ� ��		
			transPair	= *pPixels++;
			
 			// �� �� ���		
			if (transPair > 0)
			{
				j = transPair;
				do
				{
					pDestTemp += *pPixels++;	// ��������ŭ �ǳ� �ڴ�.
					indexCount = *pPixels++;	// Index�ݺ� ��

					// indexCount����ŭ index������ ����Ѵ�.
					// �� �� ���		
					if (indexCount > 0)
					{
						k = indexCount;
						do
						{
							colorSet		= (*pPixels >> 8) & 0xFF;						
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;
							
							// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.
							*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
							pDestTemp ++;
						} while (--k);
					}

					// Normal ���� ��
					colorCount = *pPixels++;
					
					memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
					
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
// s_Value1 - Main Color
// s_Value2 - Sub Color
//-----------------------------------------------------------------------------
//    ( (��������, Index����, 
//                 (indexSet��ȣ, gradation��ȣ), 
//                 (indexSet��ȣ, gradation��ȣ), 
//-----------------------------------------------------------------------------
// ���� clipping.  
// rectLeft���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CIndexSprite::BltClipLeft(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int		count,
			transCount, 
			indexCount,
			colorCount,
			colorCount2,
			index,
			dist;

	register int i;
	register int j;
	register int k;

	int colorSet, colorGradation;
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
		if (count > 0)
		{
			j = count;
			do
			{
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;			
			
				//---------------------------------------------
				// index�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// index�� ��� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do
							{
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���, index�Ϻ� ���
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// ���ʿ��� skip�Ǵ� ���� ��
						
						pPixels += dist;
						indexCount -= dist;
						
						// index�� �Ϻ� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do
							{
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}		

				
				// index������ŭ index����
				index += indexCount;

				// index����ŭ skip
				pPixels += indexCount;

				// normal ���� ��
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index�������ؼ��� xxxx������ �� �Ѿ��..
				// normal�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					memcpy(pDestTemp, pPixels+dist, colorCount2<<1);					
					pDestTemp += colorCount2;
					pPixels += colorCount;

					// �������ʹ� ��� ����Ѵ�.
					break;			
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
				do {
					pDestTemp += *pPixels++;	// ��������ŭ �ǳ� �ڴ�.
					indexCount = *pPixels++;	// Index�ݺ� ��

					// indexCount����ŭ index������ ����Ѵ�.
					if (indexCount > 0)
					{
						k = indexCount;
						do
						{				
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
							*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
							pDestTemp ++;
						} while (--k);
					}

					// Normal ���� ��
					colorCount = *pPixels++;
					
					memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
					
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;		
				
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
CIndexSprite::BltClipRight(WORD* pDest, WORD pitch, RECT* pRect)
{	
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			indexCount,
			colorCount,
			index;

	register int	i;
	register int	j;
	register int	k;

	int colorSet, colorGradation;

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
		if (count > 0)
		{
			j = count;
			do {
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����
				index += transCount;
				
				// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
				// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

				// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

				//---------------------------------------------
				// index������ ������ ������ �������� ���
				//---------------------------------------------			
				if (index+indexCount > rectRight)
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

						// ���������� skip�Ǵ� ������ ���ش�.
						indexCount = rectRight - index;

						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}						
						
						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				//-------------------------
				// index�� ��� ���
				//-------------------------
				// indexCount����ŭ index������ ����Ѵ�.
				if (indexCount > 0)
				{
					k = indexCount;
					do {				
						colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
						*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
						pDestTemp ++;
					} while (--k);
				}		
				// ����� indexCount��ŭ index����
				index += indexCount;
				

				// Normal ���� ��
				colorCount = *pPixels++;

				//---------------------------------------------
				// normal������ ������ ������ �������� ���
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					memcpy(pDestTemp, pPixels, (rectRight - index)<<1);
					break;			
				}			
				
				//--------------------
				// normal�� ��� ���
				//--------------------
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
// s_Value1 - Main Color
// s_Value2 - Sub Color
//-----------------------------------------------------------------------------
//    ( (��������, Index����, 
//                 (indexSet��ȣ, gradation��ȣ), 
//                 (indexSet��ȣ, gradation��ȣ), 
//-----------------------------------------------------------------------------
// ���� clipping.  
// rectLeft���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
// rectRight����..
//----------------------------------------------------------------------
void
CIndexSprite::BltClipWidth(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			indexCount,
			colorCount,
			colorCount2,
			index,
			dist;

	register int i;
	register int j;
	register int k;

	int colorSet, colorGradation;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
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
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do {		
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;			
			
				//---------------------------------------------
				// index�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// index�� ��� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {				
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���, index�Ϻ� ���
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// ���ʿ��� skip�Ǵ� ���� ��
						
						pPixels += dist;
						indexCount -= dist;
						
						// index�� �Ϻ� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}		

				
				// index������ŭ index����
				index += indexCount;

				// index����ŭ skip
				pPixels += indexCount;

				// normal ���� ��
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index�������ؼ��� xxxx������ �� �Ѿ��..
				// normal�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					memcpy(pDestTemp, pPixels+dist, colorCount2<<1);					
					pDestTemp += colorCount2;
					pPixels += colorCount;
					index += colorCount;

					// �������ʹ� ��� ����Ѵ�.
					break;			
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
				do {
					transCount = *pPixels++;		// ������ ��			
					indexCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����
					index += transCount;
					
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

					//---------------------------------------------
					// index������ ������ ������ �������� ���
					//---------------------------------------------			
					if (index+indexCount > rectRight)
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

							// ���������� skip�Ǵ� ������ ���ش�.
							indexCount = rectRight - index;

							// indexCount����ŭ index������ ����Ѵ�.
							if (indexCount > 0)
							{
								k = indexCount;
								do {
									colorSet		= (*pPixels >> 8) & 0xFF;	// set
									colorGradation	= (*pPixels & 0xFF);			// gradation
									pPixels++;

									// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
									*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
									pDestTemp ++;
								} while (--k);
							}						
							
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					//-------------------------
					// index�� ��� ���
					//-------------------------
					// indexCount����ŭ index������ ����Ѵ�.
					if (indexCount > 0)
					{
						k = indexCount;
						do {
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
							*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
							pDestTemp ++;
						} while (--k);
					}		
					// ����� indexCount��ŭ index����
					index += indexCount;
					

					// Normal ���� ��
					colorCount = *pPixels++;

					//---------------------------------------------
					// normal������ ������ ������ �������� ���
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						memcpy(pDestTemp, pPixels, (rectRight - index)<<1);
						break;			
					}			
					
					//--------------------
					// normal�� ��� ���
					//--------------------
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
// pRect->top, rectBottom��ŭ�� ����Ѵ�.
//----------------------------------------------------------------------
void
CIndexSprite::BltClipHeight(WORD *pDest, WORD pitch, RECT* pRect)
{
	int	count,
			indexCount,
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int  i;
	register int  j;
	register int  k;

	int colorSet, colorGradation;
	int rectBottom = pRect->bottom;
	for (i=pRect->top; i<rectBottom; i++)
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
				pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
				indexCount = *pPixels++;	// Index�ݺ� ��

				// indexCount����ŭ index������ ����Ѵ�.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
						*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
						pDestTemp ++;
					} while (--k);
				}

				// Normal ���� ��
				colorCount = *pPixels++;
				
				memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
				
				pDestTemp	+= colorCount;
				pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}


//----------------------------------------------------------------------
// Blt Darkness
//----------------------------------------------------------------------
void		
CIndexSprite::BltDarkness(WORD *pDest, WORD pitch, BYTE DarkBits)
{
	CSpriteSurface::s_Value1 = DarkBits;

	int	transPair,	
			indexCount,			
			colorCount;

	int	colorSet,
			colorGradation;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;
	register int k;
	int rectBottom = m_Height;
	
	if (rectBottom > 0)
	{
		i = rectBottom-1;
		pDest = (WORD*)((BYTE*)pDest + i*pitch);

		do
		{
			pPixels		= m_Pixels[i];
			pDestTemp	= pDest;

			// (������,Index��,�����)�� �ݺ� ��		
			transPair	= *pPixels++;
			
 			// �� �� ���
			if (transPair > 0)
			{
				j = transPair;
				do {
					pDestTemp += *pPixels++;	// ��������ŭ �ǳ� �ڴ�.
					indexCount = *pPixels++;	// Index�ݺ� ��

					// indexCount����ŭ index������ ����Ѵ�.
					if (indexCount > 0)
					{
						k = indexCount;
						do {
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
							*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
							pDestTemp ++;
						} while (--k);
					}

					// Normal ���� ��
					colorCount = *pPixels++;
					
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
// Blt Darkness Clip Left
//----------------------------------------------------------------------
void		
CIndexSprite::BltDarknessClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	CSpriteSurface::s_Value1 = DarkBits;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			indexCount,
			colorCount,
			colorCount2,
			index,
			dist;

	register int i;
	register int j;
	register int k;

	int colorSet, colorGradation;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
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
		if (count > 0)
		{
			j = count;
			do {		
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;			
			
				//---------------------------------------------
				// index�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// index�� ��� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {						
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���, index�Ϻ� ���
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// ���ʿ��� skip�Ǵ� ���� ��
						
						pPixels += dist;
						indexCount -= dist;
						
						// index�� �Ϻ� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}		

				
				// index������ŭ index����
				index += indexCount;

				// index����ŭ skip
				pPixels += indexCount;

				// normal ���� ��
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index�������ؼ��� xxxx������ �� �Ѿ��..
				// normal�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, colorCount2);
					pDestTemp += colorCount2;
					pPixels += colorCount;

					// �������ʹ� ��� ����Ѵ�.
					break;			
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
				do {		
					pDestTemp += *pPixels++;	// ��������ŭ �ǳ� �ڴ�.
					indexCount = *pPixels++;	// Index�ݺ� ��

					// indexCount����ŭ index������ ����Ѵ�.
					if (indexCount > 0)
					{
						k = indexCount;
						do {			
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
							*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
							pDestTemp ++;
						} while (--k);
					}

					// Normal ���� ��
					colorCount = *pPixels++;
					
					CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
					
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;		
				} while (--j);
			}
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt Darkness Clip Right
//----------------------------------------------------------------------
void		
CIndexSprite::BltDarknessClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	CSpriteSurface::s_Value1 = DarkBits;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			indexCount,
			colorCount,
			index;

	register int	i;
	register int	j;
	register int	k;

	int colorSet, colorGradation;

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
		if (count > 0)
		{
			j = count;
			do {
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����
				index += transCount;
				
				// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
				// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

				// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

				//---------------------------------------------
				// index������ ������ ������ �������� ���
				//---------------------------------------------			
				if (index+indexCount > rectRight)
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

						// ���������� skip�Ǵ� ������ ���ش�.
						indexCount = rectRight - index;

						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}						
						
						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				//-------------------------
				// index�� ��� ���
				//-------------------------
				// indexCount����ŭ index������ ����Ѵ�.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
						*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
						pDestTemp ++;
					} while (--k);
				}		
				// ����� indexCount��ŭ index����
				index += indexCount;
				

				// Normal ���� ��
				colorCount = *pPixels++;

				//---------------------------------------------
				// normal������ ������ ������ �������� ���
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, rectRight - index);
					break;			
				}			
				
				//--------------------
				// normal�� ��� ���
				//--------------------
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
// Blt Darkness Clip Width
//----------------------------------------------------------------------
void		
CIndexSprite::BltDarknessClipWidth(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	CSpriteSurface::s_Value1 = DarkBits;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			indexCount,
			colorCount,
			colorCount2,
			index,
			dist;

	register int i;
	register int j;
	register int k;

	int colorSet, colorGradation;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
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
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do {
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;			
			
				//---------------------------------------------
				// index�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// index�� ��� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���, index�Ϻ� ���
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// ���ʿ��� skip�Ǵ� ���� ��
						
						pPixels += dist;
						indexCount -= dist;
						
						// index�� �Ϻ� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}		

				
				// index������ŭ index����
				index += indexCount;

				// index����ŭ skip
				pPixels += indexCount;

				// normal ���� ��
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index�������ؼ��� xxxx������ �� �Ѿ��..
				// normal�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, colorCount2);
					pDestTemp += colorCount2;
					pPixels += colorCount;
					index += colorCount;

					// �������ʹ� ��� ����Ѵ�.
					break;			
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
				do {
					transCount = *pPixels++;		// ������ ��			
					indexCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����
					index += transCount;
					
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

					//---------------------------------------------
					// index������ ������ ������ �������� ���
					//---------------------------------------------			
					if (index+indexCount > rectRight)
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

							// ���������� skip�Ǵ� ������ ���ش�.
							indexCount = rectRight - index;

							// indexCount����ŭ index������ ����Ѵ�.
							if (indexCount > 0)
							{
								k = indexCount;
								do {
									colorSet		= (*pPixels >> 8) & 0xFF;	// set
									colorGradation	= (*pPixels & 0xFF);			// gradation
									pPixels++;

									// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
									*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
									pDestTemp ++;
								} while (--k);
							}						
							
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					//-------------------------
					// index�� ��� ���
					//-------------------------
					// indexCount����ŭ index������ ����Ѵ�.
					if (indexCount > 0)
					{
						k = indexCount;
						do {
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
							*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
							pDestTemp ++;
						} while (--k);
					}
					// ����� indexCount��ŭ index����
					index += indexCount;
					

					// Normal ���� ��
					colorCount = *pPixels++;

					//---------------------------------------------
					// normal������ ������ ������ �������� ���
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, rectRight - index);
						break;			
					}			
					
					//--------------------
					// normal�� ��� ���
					//--------------------
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
// Blt Darkness Clip Height
//----------------------------------------------------------------------
void		
CIndexSprite::BltDarknessClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	CSpriteSurface::s_Value1 = DarkBits;

	int	count,
			indexCount,
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int  i;
	register int  j;
	register int  k;

	int colorSet, colorGradation;
	int rectBottom = pRect->bottom;
	for (i=pRect->top; i<rectBottom; i++)
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
				pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
				indexCount = *pPixels++;	// Index�ݺ� ��

				// indexCount����ŭ index������ ����Ѵ�.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
						*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
						pDestTemp ++;
					} while (--k);
				}

				// Normal ���� ��
				colorCount = *pPixels++;
				
				CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
				
				pDestTemp	+= colorCount;
				pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt Darkness
//----------------------------------------------------------------------
void		
CIndexSprite::BltAlpha(WORD *pDest, WORD pitch, BYTE alpha)
{
	CSpriteSurface::s_Value1 = alpha;

	int	transPair,	
			indexCount,			
			colorCount;

	int	colorSet,
			colorGradation;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;
	register int k;
	int rectBottom = m_Height;
	
	if (rectBottom > 0)
	{
		i = rectBottom-1;
		pDest = (WORD*)((BYTE*)pDest + i*pitch);

		do
		{
			pPixels		= m_Pixels[i];
			pDestTemp	= pDest;

			// (������,Index��,�����)�� �ݺ� ��		
			transPair	= *pPixels++;
			
 			// �� �� ���
			if (transPair > 0)
			{
				j = transPair;
				do {				
					pDestTemp += *pPixels++;	// ��������ŭ �ǳ� �ڴ�.
					indexCount = *pPixels++;	// Index�ݺ� ��

					// indexCount����ŭ index������ ����Ѵ�.
					if (indexCount > 0)
					{
						k = indexCount;
						do {
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
							*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
							pDestTemp ++;
						} while (--k);
					}

					// Normal ���� ��
					colorCount = *pPixels++;
					
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
// Blt Darkness Clip Left
//----------------------------------------------------------------------
void		
CIndexSprite::BltAlphaClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha)
{
	CSpriteSurface::s_Value1 = alpha;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			indexCount,
			colorCount,
			colorCount2,
			index,
			dist;

	register int i;
	register int j;
	register int k;

	int colorSet, colorGradation;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
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
		if (count > 0)
		{
			j = count;
			do {
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;			
			
				//---------------------------------------------
				// index�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// index�� ��� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���, index�Ϻ� ���
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// ���ʿ��� skip�Ǵ� ���� ��
						
						pPixels += dist;
						indexCount -= dist;
						
						// index�� �Ϻ� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}		

				
				// index������ŭ index����
				index += indexCount;

				// index����ŭ skip
				pPixels += indexCount;

				// normal ���� ��
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index�������ؼ��� xxxx������ �� �Ѿ��..
				// normal�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					CSpriteSurface::memcpyAlpha(pDestTemp, pPixels+dist, colorCount2);
					pDestTemp += colorCount2;
					pPixels += colorCount;

					// �������ʹ� ��� ����Ѵ�.
					break;			
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
				do {
					pDestTemp += *pPixels++;	// ��������ŭ �ǳ� �ڴ�.
					indexCount = *pPixels++;	// Index�ݺ� ��

					// indexCount����ŭ index������ ����Ѵ�.
					if (indexCount > 0)
					{
						k = indexCount;
						do {
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
							*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
							pDestTemp ++;
						} while (--k);
					}

					// Normal ���� ��
					colorCount = *pPixels++;
					
					CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, colorCount);
					
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;		
				} while (--j);
			}
		}
		
		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt Darkness Clip Right
//----------------------------------------------------------------------
void		
CIndexSprite::BltAlphaClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha)
{
	CSpriteSurface::s_Value1 = alpha;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			indexCount,
			colorCount,
			index;

	register int	i;
	register int	j;
	register int	k;

	int colorSet, colorGradation;

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
		if (count > 0)
		{
			j = count;
			do {
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����
				index += transCount;
				
				// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
				// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

				// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

				//---------------------------------------------
				// index������ ������ ������ �������� ���
				//---------------------------------------------			
				if (index+indexCount > rectRight)
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

						// ���������� skip�Ǵ� ������ ���ش�.
						indexCount = rectRight - index;

						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
								pDestTemp ++;
							} while (--k);
						}						
						
						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				//-------------------------
				// index�� ��� ���
				//-------------------------
				// indexCount����ŭ index������ ����Ѵ�.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
						*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
						pDestTemp ++;
					} while (--k);
				}		
				// ����� indexCount��ŭ index����
				index += indexCount;
				

				// Normal ���� ��
				colorCount = *pPixels++;

				//---------------------------------------------
				// normal������ ������ ������ �������� ���
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, rectRight - index);
					break;			
				}			
				
				//--------------------
				// normal�� ��� ���
				//--------------------
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
// Blt Darkness Clip Width
//----------------------------------------------------------------------
void		
CIndexSprite::BltAlphaClipWidth(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha)
{
	CSpriteSurface::s_Value1 = alpha;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			indexCount,
			colorCount,
			colorCount2,
			index,
			dist;

	register int i;
	register int j;
	register int k;

	int colorSet, colorGradation;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
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
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do {
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;			
			
				//---------------------------------------------
				// index�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// index�� ��� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���, index�Ϻ� ���
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// ���ʿ��� skip�Ǵ� ���� ��
						
						pPixels += dist;
						indexCount -= dist;
						
						// index�� �Ϻ� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}		

				
				// index������ŭ index����
				index += indexCount;

				// index����ŭ skip
				pPixels += indexCount;

				// normal ���� ��
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index�������ؼ��� xxxx������ �� �Ѿ��..
				// normal�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					CSpriteSurface::memcpyAlpha(pDestTemp, pPixels+dist, colorCount2);
					pDestTemp += colorCount2;
					pPixels += colorCount;
					index += colorCount;

					// �������ʹ� ��� ����Ѵ�.
					break;			
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
				do {
					transCount = *pPixels++;		// ������ ��			
					indexCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����
					index += transCount;
					
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

					//---------------------------------------------
					// index������ ������ ������ �������� ���
					//---------------------------------------------			
					if (index+indexCount > rectRight)
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

							// ���������� skip�Ǵ� ������ ���ش�.
							indexCount = rectRight - index;

							// indexCount����ŭ index������ ����Ѵ�.
							if (indexCount > 0)
							{
								k = indexCount;
								do {
									colorSet		= (*pPixels >> 8) & 0xFF;	// set
									colorGradation	= (*pPixels & 0xFF);			// gradation
									pPixels++;

									// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
									*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
									pDestTemp ++;
								} while (--k);
							}						
							
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					//-------------------------
					// index�� ��� ���
					//-------------------------
					// indexCount����ŭ index������ ����Ѵ�.
					if (indexCount > 0)
					{
						k = indexCount;
						do {
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
							*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
							pDestTemp ++;
						} while (--k);
					}		
					// ����� indexCount��ŭ index����
					index += indexCount;
					

					// Normal ���� ��
					colorCount = *pPixels++;

					//---------------------------------------------
					// normal������ ������ ������ �������� ���
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, rectRight - index);
						break;			
					}			
					
					//--------------------
					// normal�� ��� ���
					//--------------------
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
// Blt Darkness Clip Height
//----------------------------------------------------------------------
void		
CIndexSprite::BltAlphaClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha)
{
	CSpriteSurface::s_Value1 = alpha;

	int	count,
			indexCount,
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int  i;
	register int  j;
	register int  k;

	int colorSet, colorGradation;
	int rectBottom = pRect->bottom;
	for (i=pRect->top; i<rectBottom; i++)
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
				pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
				indexCount = *pPixels++;	// Index�ݺ� ��

				// indexCount����ŭ index������ ����Ѵ�.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
						*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
						pDestTemp ++;
					} while (--k);
				}

				// Normal ���� ��
				colorCount = *pPixels++;
				
				CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, colorCount);
				
				pDestTemp	+= colorCount;
				pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}




//----------------------------------------------------------------------
// BltWave
//----------------------------------------------------------------------
// Clipping���� �ʴ´�.
//----------------------------------------------------------------------
/*
void
CIndexSprite::BltWave(WORD *pDest, WORD pitch)
{
	int	transPair,	
			indexCount,			
			colorCount;

	int	colorSet,
			colorGradation;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;
	register int k;
	int rectBottom = m_Height;	

	
	//for (i=0; i<rectBottom; i++)	
	if (rectBottom > 0)
	{
		i = rectBottom-1;
		pDest = (WORD*)((BYTE*)pDest + i*pitch);

		do
		{			
			pPixels		= m_Pixels[i];
			pDestTemp	= pDest;

			// (������,Index��,�����)�� �ݺ� ��		
			transPair	= *pPixels++;
			
 			// �� �� ���		
			if (transPair > 0)
			{
				j = transPair;
				do
				{
					pDestTemp += *pPixels++;	// ��������ŭ �ǳ� �ڴ�.
					indexCount = *pPixels++;	// Index�ݺ� ��

					// indexCount����ŭ index������ ����Ѵ�.
					// �� �� ���		
					if (indexCount > 0)
					{
						k = indexCount;
						do
						{
							colorSet		= (*pPixels >> 8) & 0xFF;						
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
							//*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];

							WORD color = GradationValue[s_IndexValue[colorSet]][colorGradation];//*pPixels;							
							int change = color >> 4;	// 0~96 --> 0~6

							*pDestTemp = *(pDestTemp + change);

							
							pDestTemp ++;
						} while (--k);
					}

					// Normal ���� ��
					colorCount = *pPixels++;
					
					//memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
					// WAVE
					for (int l=0; l<colorCount; l++)
					{
						WORD color = *pPixels;
						BYTE r = CDirectDraw::Red( color );
						BYTE g = CDirectDraw::Green( color );
						BYTE b = CDirectDraw::Blue( color );
						int change = (r+g+b) >> 4;	// 0~96 --> 0~6

						*pDestTemp = *(pDestTemp + change);

						pDestTemp++;
						pPixels++;
					}

					
					//pDestTemp	+= colorCount;
					//pPixels		+= colorCount;
				} while (--j);
			}

			pDest = (WORD*)((BYTE*)pDest - pitch);
		} while (i--);
	}
}
*/



//----------------------------------------------------------------------
// Blt Darkness
//----------------------------------------------------------------------
void		
CIndexSprite::BltColor(WORD *pDest, WORD pitch, BYTE rgb)
{
	CSpriteSurface::s_Value1 = rgb;

	int	transPair,	
			indexCount,			
			colorCount;

	int	colorSet,
			colorGradation;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;
	register int k;
	int rectBottom = m_Height;
	
	if (rectBottom > 0)
	{
		i = rectBottom-1;
		pDest = (WORD*)((BYTE*)pDest + i*pitch);

		do
		{
			pPixels		= m_Pixels[i];
			pDestTemp	= pDest;

			// (������,Index��,�����)�� �ݺ� ��		
			transPair	= *pPixels++;
			
 			// �� �� ���
			if (transPair > 0)
			{
				j = transPair;
				do {
					pDestTemp += *pPixels++;	// ��������ŭ �ǳ� �ڴ�.
					indexCount = *pPixels++;	// Index�ݺ� ��

					// indexCount����ŭ index������ ����Ѵ�.
					if (indexCount > 0)
					{
						k = indexCount;
						do 
						{
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
							*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
							pDestTemp ++;
						} while (--k);
					}

					// Normal ���� ��
					colorCount = *pPixels++;
					
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
// Blt Darkness Clip Left
//----------------------------------------------------------------------
void		
CIndexSprite::BltColorClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE rgb)
{
	CSpriteSurface::s_Value1 = rgb;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			indexCount,
			colorCount,
			colorCount2,
			index,
			dist;

	register int i;
	register int j;
	register int k;

	int colorSet, colorGradation;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
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
		if (count > 0)
		{
			j = count;
			do {		
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;			
			
				//---------------------------------------------
				// index�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// index�� ��� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {						
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyColor(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���, index�Ϻ� ���
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// ���ʿ��� skip�Ǵ� ���� ��
						
						pPixels += dist;
						indexCount -= dist;
						
						// index�� �Ϻ� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyColor(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}		

				
				// index������ŭ index����
				index += indexCount;

				// index����ŭ skip
				pPixels += indexCount;

				// normal ���� ��
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index�������ؼ��� xxxx������ �� �Ѿ��..
				// normal�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					CSpriteSurface::memcpyColor(pDestTemp, pPixels+dist, colorCount2);
					pDestTemp += colorCount2;
					pPixels += colorCount;

					// �������ʹ� ��� ����Ѵ�.
					break;			
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
				do {		
					pDestTemp += *pPixels++;	// ��������ŭ �ǳ� �ڴ�.
					indexCount = *pPixels++;	// Index�ݺ� ��

					// indexCount����ŭ index������ ����Ѵ�.
					if (indexCount > 0)
					{
						k = indexCount;
						do {			
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
							*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
							pDestTemp ++;
						} while (--k);
					}

					// Normal ���� ��
					colorCount = *pPixels++;
					
					CSpriteSurface::memcpyColor(pDestTemp, pPixels, colorCount);
					
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;		
				} while (--j);
			}
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt Darkness Clip Right
//----------------------------------------------------------------------
void		
CIndexSprite::BltColorClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE rgb)
{
	// rgb�� ����
	CSpriteSurface::s_Value1 = rgb;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			indexCount,
			colorCount,
			index;

	register int	i;
	register int	j;
	register int	k;

	int colorSet, colorGradation;

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
		if (count > 0)
		{
			j = count;
			do {
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����
				index += transCount;
				
				// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
				// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

				// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

				//---------------------------------------------
				// index������ ������ ������ �������� ���
				//---------------------------------------------			
				if (index+indexCount > rectRight)
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

						// ���������� skip�Ǵ� ������ ���ش�.
						indexCount = rectRight - index;

						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
								pDestTemp ++;
							} while (--k);
						}						
						
						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				//-------------------------
				// index�� ��� ���
				//-------------------------
				// indexCount����ŭ index������ ����Ѵ�.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
						*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
						pDestTemp ++;
					} while (--k);
				}		
				// ����� indexCount��ŭ index����
				index += indexCount;
				

				// Normal ���� ��
				colorCount = *pPixels++;

				//---------------------------------------------
				// normal������ ������ ������ �������� ���
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					CSpriteSurface::memcpyColor(pDestTemp, pPixels, rectRight - index);
					break;			
				}			
				
				//--------------------
				// normal�� ��� ���
				//--------------------
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
// Blt Darkness Clip Width
//----------------------------------------------------------------------
void		
CIndexSprite::BltColorClipWidth(WORD *pDest, WORD pitch, RECT* pRect, BYTE rgb)
{
	// rgb�� ����
	CSpriteSurface::s_Value1 = rgb;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			indexCount,
			colorCount,
			colorCount2,
			index,
			dist;

	register int i;
	register int j;
	register int k;

	int colorSet, colorGradation;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
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
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do {
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;			
			
				//---------------------------------------------
				// index�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// index�� ��� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyColor(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���, index�Ϻ� ���
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// ���ʿ��� skip�Ǵ� ���� ��
						
						pPixels += dist;
						indexCount -= dist;
						
						// index�� �Ϻ� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyColor(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}		

				
				// index������ŭ index����
				index += indexCount;

				// index����ŭ skip
				pPixels += indexCount;

				// normal ���� ��
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index�������ؼ��� xxxx������ �� �Ѿ��..
				// normal�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					CSpriteSurface::memcpyColor(pDestTemp, pPixels+dist, colorCount2);
					pDestTemp += colorCount2;
					pPixels += colorCount;
					index += colorCount;

					// �������ʹ� ��� ����Ѵ�.
					break;			
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
				do {
					transCount = *pPixels++;		// ������ ��			
					indexCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����
					index += transCount;
					
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

					//---------------------------------------------
					// index������ ������ ������ �������� ���
					//---------------------------------------------			
					if (index+indexCount > rectRight)
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

							// ���������� skip�Ǵ� ������ ���ش�.
							indexCount = rectRight - index;

							// indexCount����ŭ index������ ����Ѵ�.
							if (indexCount > 0)
							{
								k = indexCount;
								do {
									colorSet		= (*pPixels >> 8) & 0xFF;	// set
									colorGradation	= (*pPixels & 0xFF);			// gradation
									pPixels++;

									// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
									*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
									pDestTemp ++;
								} while (--k);
							}						
							
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					//-------------------------
					// index�� ��� ���
					//-------------------------
					// indexCount����ŭ index������ ����Ѵ�.
					if (indexCount > 0)
					{
						k = indexCount;
						do {
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
							*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
							pDestTemp ++;
						} while (--k);
					}
					// ����� indexCount��ŭ index����
					index += indexCount;
					

					// Normal ���� ��
					colorCount = *pPixels++;

					//---------------------------------------------
					// normal������ ������ ������ �������� ���
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyColor(pDestTemp, pPixels, rectRight - index);
						break;			
					}			
					
					//--------------------
					// normal�� ��� ���
					//--------------------
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
// Blt Darkness Clip Height
//----------------------------------------------------------------------
void		
CIndexSprite::BltColorClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE rgb)
{
	// rgb�� ����
	CSpriteSurface::s_Value1 = rgb;

	int	count,
			indexCount,
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int  i;
	register int  j;
	register int  k;

	int colorSet, colorGradation;
	int rectBottom = pRect->bottom;
	for (i=pRect->top; i<rectBottom; i++)
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
				pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
				indexCount = *pPixels++;	// Index�ݺ� ��

				// indexCount����ŭ index������ ����Ѵ�.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
						*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
						pDestTemp ++;
					} while (--k);
				}

				// Normal ���� ��
				colorCount = *pPixels++;
				
				CSpriteSurface::memcpyColor(pDestTemp, pPixels, colorCount);
				
				pDestTemp	+= colorCount;
				pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}


//----------------------------------------------------------------------
// Blt ColorSet
//----------------------------------------------------------------------
void		
CIndexSprite::BltColorSet(WORD *pDest, WORD pitch, WORD colorSet)
{
	CSpriteSurface::s_Value1 = colorSet;

	int	transPair,	
			indexCount,			
			colorCount;

	int	//colorSet,
			colorGradation;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;
	register int k;
	int rectBottom = m_Height;
	
	if (rectBottom > 0)
	{
		i = rectBottom-1;
		pDest = (WORD*)((BYTE*)pDest + i*pitch);

		do
		{
			pPixels		= m_Pixels[i];
			pDestTemp	= pDest;

			// (������,Index��,�����)�� �ݺ� ��		
			transPair	= *pPixels++;
			
 			// �� �� ���
			if (transPair > 0)
			{
				j = transPair;
				do {
					pDestTemp += *pPixels++;	// ��������ŭ �ǳ� �ڴ�.
					indexCount = *pPixels++;	// Index�ݺ� ��

					// indexCount����ŭ index������ ����Ѵ�.
					if (indexCount > 0)
					{
						k = indexCount;
						do 
						{
							//colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
							*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
							pDestTemp ++;
						} while (--k);
					}

					// Normal ���� ��
					colorCount = *pPixels++;
					
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
// Blt ColorSet Clip Left
//----------------------------------------------------------------------
void		
CIndexSprite::BltColorSetClipLeft(WORD *pDest, WORD pitch, RECT* pRect, WORD colorSet)
{
	CSpriteSurface::s_Value1 = colorSet;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			indexCount,
			colorCount,
			colorCount2,
			index,
			dist;

	register int i;
	register int j;
	register int k;

	int //colorSet, 
		colorGradation;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
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
		if (count > 0)
		{
			j = count;
			do {		
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;			
			
				//---------------------------------------------
				// index�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// index�� ��� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {						
								//colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���, index�Ϻ� ���
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// ���ʿ��� skip�Ǵ� ���� ��
						
						pPixels += dist;
						indexCount -= dist;
						
						// index�� �Ϻ� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								//colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}		

				
				// index������ŭ index����
				index += indexCount;

				// index����ŭ skip
				pPixels += indexCount;

				// normal ���� ��
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index�������ؼ��� xxxx������ �� �Ѿ��..
				// normal�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels+dist, colorCount2);
					pDestTemp += colorCount2;
					pPixels += colorCount;

					// �������ʹ� ��� ����Ѵ�.
					break;			
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
				do {		
					pDestTemp += *pPixels++;	// ��������ŭ �ǳ� �ڴ�.
					indexCount = *pPixels++;	// Index�ݺ� ��

					// indexCount����ŭ index������ ����Ѵ�.
					if (indexCount > 0)
					{
						k = indexCount;
						do {			
							//colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
							*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
							pDestTemp ++;
						} while (--k);
					}

					// Normal ���� ��
					colorCount = *pPixels++;
					
					CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, colorCount);
					
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;		
				} while (--j);
			}
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt ColorSet Clip Right
//----------------------------------------------------------------------
void		
CIndexSprite::BltColorSetClipRight(WORD *pDest, WORD pitch, RECT* pRect, WORD colorSet)
{
	// colorSet�� ����
	CSpriteSurface::s_Value1 = colorSet;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			indexCount,
			colorCount,
			index;

	register int	i;
	register int	j;
	register int	k;

	int //colorSet, 
		colorGradation;

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
		if (count > 0)
		{
			j = count;
			do {
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����
				index += transCount;
				
				// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
				// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

				// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

				//---------------------------------------------
				// index������ ������ ������ �������� ���
				//---------------------------------------------			
				if (index+indexCount > rectRight)
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

						// ���������� skip�Ǵ� ������ ���ش�.
						indexCount = rectRight - index;

						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								//colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
								pDestTemp ++;
							} while (--k);
						}						
						
						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				//-------------------------
				// index�� ��� ���
				//-------------------------
				// indexCount����ŭ index������ ����Ѵ�.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						//colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
						*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
						pDestTemp ++;
					} while (--k);
				}		
				// ����� indexCount��ŭ index����
				index += indexCount;
				

				// Normal ���� ��
				colorCount = *pPixels++;

				//---------------------------------------------
				// normal������ ������ ������ �������� ���
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, rectRight - index);
					break;			
				}			
				
				//--------------------
				// normal�� ��� ���
				//--------------------
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
// Blt ColorSet Clip Width
//----------------------------------------------------------------------
void		
CIndexSprite::BltColorSetClipWidth(WORD *pDest, WORD pitch, RECT* pRect, WORD colorSet)
{
	// colorSet�� ����
	CSpriteSurface::s_Value1 = colorSet;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			indexCount,
			colorCount,
			colorCount2,
			index,
			dist;

	register int i;
	register int j;
	register int k;

	int //colorSet, 
		colorGradation;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
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
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do {
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;			
			
				//---------------------------------------------
				// index�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// index�� ��� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								//colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���, index�Ϻ� ���
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// ���ʿ��� skip�Ǵ� ���� ��
						
						pPixels += dist;
						indexCount -= dist;
						
						// index�� �Ϻ� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								//colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}		

				
				// index������ŭ index����
				index += indexCount;

				// index����ŭ skip
				pPixels += indexCount;

				// normal ���� ��
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index�������ؼ��� xxxx������ �� �Ѿ��..
				// normal�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels+dist, colorCount2);
					pDestTemp += colorCount2;
					pPixels += colorCount;
					index += colorCount;

					// �������ʹ� ��� ����Ѵ�.
					break;			
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
				do {
					transCount = *pPixels++;		// ������ ��			
					indexCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����
					index += transCount;
					
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

					//---------------------------------------------
					// index������ ������ ������ �������� ���
					//---------------------------------------------			
					if (index+indexCount > rectRight)
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

							// ���������� skip�Ǵ� ������ ���ش�.
							indexCount = rectRight - index;

							// indexCount����ŭ index������ ����Ѵ�.
							if (indexCount > 0)
							{
								k = indexCount;
								do {
									//colorSet		= (*pPixels >> 8) & 0xFF;	// set
									colorGradation	= (*pPixels & 0xFF);			// gradation
									pPixels++;

									// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
									*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
									pDestTemp ++;
								} while (--k);
							}						
							
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					//-------------------------
					// index�� ��� ���
					//-------------------------
					// indexCount����ŭ index������ ����Ѵ�.
					if (indexCount > 0)
					{
						k = indexCount;
						do {
							//colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
							*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
							pDestTemp ++;
						} while (--k);
					}
					// ����� indexCount��ŭ index����
					index += indexCount;
					

					// Normal ���� ��
					colorCount = *pPixels++;

					//---------------------------------------------
					// normal������ ������ ������ �������� ���
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, rectRight - index);
						break;			
					}			
					
					//--------------------
					// normal�� ��� ���
					//--------------------
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
// Blt ColorSet Clip Height
//----------------------------------------------------------------------
void		
CIndexSprite::BltColorSetClipHeight(WORD *pDest, WORD pitch, RECT* pRect, WORD colorSet)
{
	// colorSet�� ����
	CSpriteSurface::s_Value1 = colorSet;

	int	count,
			indexCount,
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int  i;
	register int  j;
	register int  k;

	int //colorSet, 
		colorGradation;

	int rectBottom = pRect->bottom;
	for (i=pRect->top; i<rectBottom; i++)
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
				pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
				indexCount = *pPixels++;	// Index�ݺ� ��

				// indexCount����ŭ index������ ����Ѵ�.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						//colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
						*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
						pDestTemp ++;
					} while (--k);
				}

				// Normal ���� ��
				colorCount = *pPixels++;
				
				CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, colorCount);
				
				pDestTemp	+= colorCount;
				pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}


//-----------------------------------------------------------------------------
// BltEffect Index
//-----------------------------------------------------------------------------
// s_Value1 - Main Color
// s_Value2 - Sub Color
//-----------------------------------------------------------------------------
//    ( (��������, Index����, 
//                 (indexSet��ȣ, gradation��ȣ), 
//                 (indexSet��ȣ, gradation��ȣ), 
//-----------------------------------------------------------------------------
// Clipping���� �ʴ´�.
//-----------------------------------------------------------------------------
void
CIndexSprite::BltEffect(WORD *pDest, WORD pitch)
{
	int	transPair,	
			indexCount,			
			colorCount;

	//int	colorSet,
	//		colorGradation;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;
	//register int k;
	int rectBottom = m_Height;	

	
	//for (i=0; i<rectBottom; i++)	
	if (rectBottom > 0)
	{
		i = rectBottom-1;
		pDest = (WORD*)((BYTE*)pDest + i*pitch);

		do
		{			
			pPixels		= m_Pixels[i];
			pDestTemp	= pDest;

			// (������,Index��,�����)�� �ݺ� ��		
			transPair	= *pPixels++;
			
 			// �� �� ���		
			if (transPair > 0)
			{
				j = transPair;
				do
				{
					pDestTemp += *pPixels++;	// ��������ŭ �ǳ� �ڴ�.
					indexCount = *pPixels++;	// Index�ݺ� ��

					if (indexCount > 0)
					{
						// indexCount����ŭ index������ ����Ѵ�.
						memcpyEffect( pDestTemp, pPixels, indexCount );

						pDestTemp += indexCount;
						pPixels += indexCount;
					}

					// Normal ���� ��
					colorCount = *pPixels++;
					
					if (colorCount > 0)
					{
						CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;
					}
				} while (--j);
			}

			pDest = (WORD*)((BYTE*)pDest - pitch);
		} while (i--);
	}
}

//----------------------------------------------------------------------
// BltEffect ClipLeft
//----------------------------------------------------------------------
// s_Value1 - Main Color
// s_Value2 - Sub Color
//-----------------------------------------------------------------------------
//    ( (��������, Index����, 
//                 (indexSet��ȣ, gradation��ȣ), 
//                 (indexSet��ȣ, gradation��ȣ), 
//-----------------------------------------------------------------------------
// ���� clipping.  
// rectLeft���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CIndexSprite::BltEffectClipLeft(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int		count,
			transCount, 
			indexCount,
			colorCount,
			colorCount2,
			index,
			dist;

	register int i;
	register int j;
//	register int k;

	//int colorSet, colorGradation;
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
		if (count > 0)
		{
			j = count;
			do
			{
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;			
			
				//---------------------------------------------
				// index�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							memcpyEffect( pDestTemp, pPixels, indexCount );

							pDestTemp += indexCount;
							pPixels += indexCount;
						}


						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						if (colorCount > 0)
						{
							CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
							
							pDestTemp	+= colorCount;
							pPixels		+= colorCount;					
						}

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���, index�Ϻ� ���
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// ���ʿ��� skip�Ǵ� ���� ��
						
						pPixels += dist;
						indexCount -= dist;
						
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							memcpyEffect( pDestTemp, pPixels, indexCount );

							pDestTemp += indexCount;
							pPixels += indexCount;
						}


						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						if (colorCount > 0)
						{
							CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
							
							pDestTemp	+= colorCount;
							pPixels		+= colorCount;
						}

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}		

				
				// index������ŭ index����
				index += indexCount;

				// index����ŭ skip
				pPixels += indexCount;

				// normal ���� ��
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index�������ؼ��� xxxx������ �� �Ѿ��..
				// normal�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					if (colorCount2 > 0)
					{
						CSpriteSurface::memcpyEffect(pDestTemp, pPixels+dist, colorCount2);
						pDestTemp += colorCount2;
						pPixels += colorCount;
					}

					// �������ʹ� ��� ����Ѵ�.
					break;			
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
				do {
					pDestTemp += *pPixels++;	// ��������ŭ �ǳ� �ڴ�.
					indexCount = *pPixels++;	// Index�ݺ� ��

					// indexCount����ŭ index������ ����Ѵ�.
					if (indexCount > 0)
					{
						memcpyEffect( pDestTemp, pPixels, indexCount );

						pDestTemp += indexCount;
						pPixels += indexCount;
					}

					// Normal ���� ��
					colorCount = *pPixels++;
					
					if (colorCount > 0)
					{
						CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;		
					}
				
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
// rectRight�� ������ ���� pDest�� ����Ѵ�.
//----------------------------------------------------------------------
void
CIndexSprite::BltEffectClipRight(WORD* pDest, WORD pitch, RECT* pRect)
{	
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			indexCount,
			colorCount,
			index;

	register int	i;
	register int	j;
	//register int	k;

	//int colorSet, colorGradation;

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
		if (count > 0)
		{
			j = count;
			do {
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����
				index += transCount;
				
				// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
				// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

				// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

				//---------------------------------------------
				// index������ ������ ������ �������� ���
				//---------------------------------------------			
				if (index+indexCount > rectRight)
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

						// ���������� skip�Ǵ� ������ ���ش�.
						indexCount = rectRight - index;

						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							memcpyEffect( pDestTemp, pPixels, indexCount );

							pDestTemp += indexCount;
							pPixels += indexCount;
						}

						
						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				//-------------------------
				// index�� ��� ���
				//-------------------------
				// indexCount����ŭ index������ ����Ѵ�.
				if (indexCount > 0)
				{
					memcpyEffect( pDestTemp, pPixels, indexCount );

					pDestTemp += indexCount;
					pPixels += indexCount;
				}

				// ����� indexCount��ŭ index����
				index += indexCount;
				

				// Normal ���� ��
				colorCount = *pPixels++;

				//---------------------------------------------
				// normal������ ������ ������ �������� ���
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					int tempCount = rectRight - index;
					if (tempCount > 0)
					{
						CSpriteSurface::memcpyEffect(pDestTemp, pPixels, tempCount);
					}
					break;			
				}			
				
				//--------------------
				// normal�� ��� ���
				//--------------------
				if (colorCount > 0)
				{
					CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
				}

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
// s_Value1 - Main Color
// s_Value2 - Sub Color
//-----------------------------------------------------------------------------
//    ( (��������, Index����, 
//                 (indexSet��ȣ, gradation��ȣ), 
//                 (indexSet��ȣ, gradation��ȣ), 
//-----------------------------------------------------------------------------
// ���� clipping.  
// rectLeft���� ���� �ǳʶ� �������� pDest�� ����Ѵ�.
// rectRight����..
//----------------------------------------------------------------------
void
CIndexSprite::BltEffectClipWidth(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			indexCount,
			colorCount,
			colorCount2,
			index,
			dist;

	register int i;
	register int j;
//	register int k;

	//int colorSet, colorGradation;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
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
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do {		
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;			
			
				//---------------------------------------------
				// index�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							memcpyEffect( pDestTemp, pPixels, indexCount );

							pDestTemp += indexCount;
							pPixels += indexCount;
						}


						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						if (colorCount > 0)
						{
							CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
							
							pDestTemp	+= colorCount;
							pPixels		+= colorCount;					
						}

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���, index�Ϻ� ���
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// ���ʿ��� skip�Ǵ� ���� ��
						
						pPixels += dist;
						indexCount -= dist;
						
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							memcpyEffect( pDestTemp, pPixels, indexCount );

							pDestTemp += indexCount;
							pPixels += indexCount;
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						if (colorCount > 0)
						{
							CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
							
							pDestTemp	+= colorCount;
							pPixels		+= colorCount;
							index += colorCount;
						}

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}		

				
				// index������ŭ index����
				index += indexCount;

				// index����ŭ skip
				pPixels += indexCount;

				// normal ���� ��
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index�������ؼ��� xxxx������ �� �Ѿ��..
				// normal�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					if (colorCount2 > 0)
					{
						CSpriteSurface::memcpyEffect(pDestTemp, pPixels+dist, colorCount2);
						pDestTemp += colorCount2;
					}
					pPixels += colorCount;
					index += colorCount;					

					// �������ʹ� ��� ����Ѵ�.
					break;			
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
				do {
					transCount = *pPixels++;		// ������ ��			
					indexCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����
					index += transCount;
					
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

					//---------------------------------------------
					// index������ ������ ������ �������� ���
					//---------------------------------------------			
					if (index+indexCount > rectRight)
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

							// ���������� skip�Ǵ� ������ ���ش�.
							indexCount = rectRight - index;

							// indexCount����ŭ index������ ����Ѵ�.
							if (indexCount > 0)
							{
								memcpyEffect( pDestTemp, pPixels, indexCount );

								pDestTemp += indexCount;
								pPixels += indexCount;
							}

							
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					//-------------------------
					// index�� ��� ���
					//-------------------------
					// indexCount����ŭ index������ ����Ѵ�.
					if (indexCount > 0)
					{
						memcpyEffect( pDestTemp, pPixels, indexCount );

						pDestTemp += indexCount;
						pPixels += indexCount;
					}

					// ����� indexCount��ŭ index����
					index += indexCount;
					

					// Normal ���� ��
					colorCount = *pPixels++;

					//---------------------------------------------
					// normal������ ������ ������ �������� ���
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						int tempCount = rectRight - index;
						if (tempCount > 0)
						{
							CSpriteSurface::memcpyEffect(pDestTemp, pPixels, tempCount);
						}
						break;			
					}			
					
					//--------------------
					// normal�� ��� ���
					//--------------------
					if (colorCount > 0)
					{
						CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
					}

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
// pRect->top, rectBottom��ŭ�� ����Ѵ�.
//----------------------------------------------------------------------
void
CIndexSprite::BltEffectClipHeight(WORD *pDest, WORD pitch, RECT* pRect)
{
	int	count,
			indexCount,
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int  i;
	register int  j;
//	register int  k;

	//int colorSet, colorGradation;
	int rectBottom = pRect->bottom;
	for (i=pRect->top; i<rectBottom; i++)
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
				pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
				indexCount = *pPixels++;	// Index�ݺ� ��

				// indexCount����ŭ index������ ����Ѵ�.
				if (indexCount > 0)
				{
					memcpyEffect( pDestTemp, pPixels, indexCount );

					pDestTemp += indexCount;
					pPixels += indexCount;
				}

				// Normal ���� ��
				colorCount = *pPixels++;
				
				if (colorCount > 0)
				{
					CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
				}
				
				pDestTemp	+= colorCount;
				pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}


//----------------------------------------------------------------------
// memcpy EffectDarker
//----------------------------------------------------------------------
void		
CIndexSprite::memcpyEffectDarker(WORD* pDest, WORD* pSource, WORD pixels)
{
}

//----------------------------------------------------------------------
// memcpy EffectGrayScale
//----------------------------------------------------------------------
void		
CIndexSprite::memcpyEffectGrayScale(WORD* pDest, WORD* pSource, WORD pixels)
{
}

//----------------------------------------------------------------------
// memcpy EffectLighten
//----------------------------------------------------------------------
void		
CIndexSprite::memcpyEffectLighten(WORD* pDest, WORD* pSource, WORD pixels)
{
}

//----------------------------------------------------------------------
// memcpy EffectDarken
//----------------------------------------------------------------------
void		
CIndexSprite::memcpyEffectDarken(WORD* pDest, WORD* pSource, WORD pixels)
{
}

//----------------------------------------------------------------------
// memcpy EffectColorDodge
//----------------------------------------------------------------------
void		
CIndexSprite::memcpyEffectColorDodge(WORD* pDest, WORD* pSource, WORD pixels)
{
}

//----------------------------------------------------------------------
// memcpy EffectScreen
//----------------------------------------------------------------------
void		
CIndexSprite::memcpyEffectScreen(WORD* pDest, WORD* pSource, WORD pixels)
{
}

//----------------------------------------------------------------------
// memcpy EffectDodgeBurn
//----------------------------------------------------------------------
void		
CIndexSprite::memcpyEffectDodgeBurn(WORD* pDest, WORD* pSource, WORD pixels)
{
}

//----------------------------------------------------------------------
// memcpy EffectDifferent
//----------------------------------------------------------------------
void		
CIndexSprite::memcpyEffectDifferent(WORD* pDest, WORD* pSource, WORD pixels)
{
}

//----------------------------------------------------------------------
// memcpy EffectGradation
//----------------------------------------------------------------------
void		
CIndexSprite::memcpyEffectGradation(WORD* pDest, WORD* pSource, WORD pixels)
{
}

//----------------------------------------------------------------------
// memcpy EffectSimpleOutline
//----------------------------------------------------------------------
void		
CIndexSprite::memcpyEffectSimpleOutline(WORD* pDest, WORD* pSource, WORD pixels)
{
	int colorSet, colorGradation;

	// ù ��
	colorSet		= (*pSource >> 8) & 0xFF;						
	colorGradation	= *pSource & 0xFF;			// gradation
	*pDest			= ColorSet[s_IndexValue[colorSet]][colorGradation];

	int pixels_1 = pixels-1;
	
	pDest += pixels_1;
	pSource += pixels_1;
	
	// �� ��
	colorSet		= (*pSource >> 8) & 0xFF;						
	colorGradation	= *pSource & 0xFF;			// gradation
	//*pDest			= ColorSet[s_IndexValue[colorSet]][colorGradation];
}

//----------------------------------------------------------------------
// memcpy EffectWipeOut
//----------------------------------------------------------------------
void		
CIndexSprite::memcpyEffectWipeOut(WORD* pDest, WORD* pSource, WORD pixels)
{
	int colorSet, colorGradation;	

	int skipPixels = (pixels * CSpriteSurface::s_Value1) >> 6;	// / 64
	int drawPixels = (pixels - skipPixels)>>1;
	int drawPixels2 = pixels - drawPixels - skipPixels;
	
	// [1] drawPixels��ŭ ����ϰ�..
	// [2] skipPixels ��ŭ �ǳʶ��
	// [3] drawPixels2��ŭ ���
	
	//------------------------------------------------------------	
	// drawPixels��ŭ ���
	//------------------------------------------------------------		
	if (drawPixels > 0)
	{
		do
		{
			colorSet		= (*pSource >> 8) & 0xFF;						
			colorGradation	= *pSource & 0xFF;			// gradation
			pSource++;

			// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
			*pDest		= ColorSet[s_IndexValue[colorSet]][colorGradation];
			pDest ++;
		} while (--drawPixels);
	}

	//------------------------------------------------------------	
	// skipPixels��ŭ �ǳʶ�
	//------------------------------------------------------------	
	pDest += skipPixels;
	pSource += skipPixels;

	//------------------------------------------------------------	
	// drawPixels2��ŭ ���
	//------------------------------------------------------------	
	if (drawPixels2 > 0)
	{
		do
		{
			colorSet		= (*pSource >> 8) & 0xFF;						
			colorGradation	= *pSource & 0xFF;			// gradation
			pSource++;

			// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
			*pDest		= ColorSet[s_IndexValue[colorSet]][colorGradation];
			pDest ++;
		} while (--drawPixels2);
	}

	//pDest += drawPixels2;
	//pSource += drawPixels2;	
}

//----------------------------------------------------------------------
// memcpy EffectNet
//----------------------------------------------------------------------
// �׹���
//----------------------------------------------------------------------
void		
CIndexSprite::memcpyEffectNet(WORD* pDest, WORD* pSource, WORD pixels)
{
	// �����(1) + �ǳʶ����(s_Value1)
	register int i = pixels;
	int skipPixels = 1 + CSpriteSurface::s_Value1;

	int colorSet, colorGradation;	
	
	// ���� ��� , skipPixels��ŭ �ǳʶ��
	do
	{
		colorSet		= (*pSource >> 8) & 0xFF;						
		colorGradation	= *pSource & 0xFF;			// gradation
		
		// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
		*pDest		= ColorSet[s_IndexValue[colorSet]][colorGradation];

		pSource += skipPixels;
		pDest += skipPixels;

		i -= skipPixels;

	} while (i > 0);

}


//----------------------------------------------------------------------
// memcpy 
//----------------------------------------------------------------------
void		
CIndexSprite::memcpyEffectGrayScaleVarious(WORD* pDest, WORD* pSource, WORD pixels)
{
	register int i = pixels;
	int colorSet, colorGradation;	
	//int grayValue = s_Value1;

	//------------------------------------------------------------	
	// Gray Scale : color value = (R+G+B)/3
	//------------------------------------------------------------
	int average;
	int r, g, b;
	int color;
	
	while (i--)
	{
		colorSet		= (*pSource >> 8) & 0xFF;						
		colorGradation	= *pSource & 0xFF;			// gradation
		
		color = ColorSet[s_IndexValue[colorSet]][colorGradation];

		r = CDirectDraw::Red(color);
		g = CDirectDraw::Green(color);
		b = CDirectDraw::Blue(color);

		average = ( r + g + b ) / 3;		// 0 ~ 31

		r = average + ((r-average)*CSpriteSurface::s_Value1 >> 5);
		g = average + ((g-average)*CSpriteSurface::s_Value1 >> 5);
		b = average + ((b-average)*CSpriteSurface::s_Value1 >> 5);
	
		*pDest = (r << CDirectDraw::s_bSHIFT_R) 
				| (g << CDirectDraw::s_bSHIFT_G)
				| b;				
		
		pDest++;
		pSource++;
	}
}



//----------------------------------------------------------------------
// Blt Brightness
//----------------------------------------------------------------------
void		
CIndexSprite::BltBrightness(WORD *pDest, WORD pitch, BYTE DarkBits)
{
	CSpriteSurface::s_Value1 = DarkBits;

	int	transPair,	
			indexCount,			
			colorCount;

	int	colorSet,
			colorGradation;

	WORD	*pDestTemp,
			*pPixels;

	register int i;
	register int j;
	register int k;
	int rectBottom = m_Height;
	
	if (rectBottom > 0)
	{
		i = rectBottom-1;
		pDest = (WORD*)((BYTE*)pDest + i*pitch);

		do
		{
			pPixels		= m_Pixels[i];
			pDestTemp	= pDest;

			// (������,Index��,�����)�� �ݺ� ��		
			transPair	= *pPixels++;
			
 			// �� �� ���
			if (transPair > 0)
			{
				j = transPair;
				do {
					pDestTemp += *pPixels++;	// ��������ŭ �ǳ� �ڴ�.
					indexCount = *pPixels++;	// Index�ݺ� ��

					// indexCount����ŭ index������ ����Ѵ�.
					if (indexCount > 0)
					{
						k = indexCount;
						do {
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
							*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
							pDestTemp ++;
						} while (--k);
						
					}

					// Normal ���� ��
					colorCount = *pPixels++;
					
					CSpriteSurface::memcpyBrightness(pDestTemp, pPixels, colorCount);					
					
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;
				} while (--j);
			}

			pDest = (WORD*)((BYTE*)pDest - pitch);
		} while (i--);
	}
}

//----------------------------------------------------------------------
// Blt Brightness Clip Left
//----------------------------------------------------------------------
void		
CIndexSprite::BltBrightnessClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	CSpriteSurface::s_Value1 = DarkBits;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			indexCount,
			colorCount,
			colorCount2,
			index,
			dist;

	register int i;
	register int j;
	register int k;

	int colorSet, colorGradation;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
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
		if (count > 0)
		{
			j = count;
			do {		
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;			
			
				//---------------------------------------------
				// index�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// index�� ��� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {						
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyBrightness(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���, index�Ϻ� ���
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// ���ʿ��� skip�Ǵ� ���� ��
						
						pPixels += dist;
						indexCount -= dist;
						
						// index�� �Ϻ� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyBrightness(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}		

				
				// index������ŭ index����
				index += indexCount;

				// index����ŭ skip
				pPixels += indexCount;

				// normal ���� ��
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index�������ؼ��� xxxx������ �� �Ѿ��..
				// normal�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					CSpriteSurface::memcpyBrightness(pDestTemp, pPixels+dist, colorCount2);
					pDestTemp += colorCount2;
					pPixels += colorCount;

					// �������ʹ� ��� ����Ѵ�.
					break;			
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
				do {		
					pDestTemp += *pPixels++;	// ��������ŭ �ǳ� �ڴ�.
					indexCount = *pPixels++;	// Index�ݺ� ��

					// indexCount����ŭ index������ ����Ѵ�.
					if (indexCount > 0)
					{
						k = indexCount;
						do {			
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
							*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
							pDestTemp ++;
						} while (--k);
					}

					// Normal ���� ��
					colorCount = *pPixels++;
					
					CSpriteSurface::memcpyBrightness(pDestTemp, pPixels, colorCount);
					
					pDestTemp	+= colorCount;
					pPixels		+= colorCount;		
				} while (--j);
			}
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt Brightness Clip Right
//----------------------------------------------------------------------
void		
CIndexSprite::BltBrightnessClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	CSpriteSurface::s_Value1 = DarkBits;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			indexCount,
			colorCount,
			index;

	register int	i;
	register int	j;
	register int	k;

	int colorSet, colorGradation;

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
		if (count > 0)
		{
			j = count;
			do {
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����
				index += transCount;
				
				// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
				// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

				// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

				//---------------------------------------------
				// index������ ������ ������ �������� ���
				//---------------------------------------------			
				if (index+indexCount > rectRight)
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

						// ���������� skip�Ǵ� ������ ���ش�.
						indexCount = rectRight - index;

						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}						
						
						break;
					}
				}

				// ��������ŭ �ǳʶ��
				pDestTemp += transCount;

				//-------------------------
				// index�� ��� ���
				//-------------------------
				// indexCount����ŭ index������ ����Ѵ�.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
						*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
						pDestTemp ++;
					} while (--k);
				}		
				// ����� indexCount��ŭ index����
				index += indexCount;
				

				// Normal ���� ��
				colorCount = *pPixels++;

				//---------------------------------------------
				// normal������ ������ ������ �������� ���
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					CSpriteSurface::memcpyBrightness(pDestTemp, pPixels, rectRight - index);
					break;			
				}			
				
				//--------------------
				// normal�� ��� ���
				//--------------------
				CSpriteSurface::memcpyBrightness(pDestTemp, pPixels, colorCount);

				pDestTemp += colorCount;
				pPixels += colorCount;			
				index += colorCount;
			} while (--j);
		}
		
		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}

//----------------------------------------------------------------------
// Blt Brightness Clip Width
//----------------------------------------------------------------------
void		
CIndexSprite::BltBrightnessClipWidth(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	CSpriteSurface::s_Value1 = DarkBits;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect��ŭ�� ���� ����Ѵ�.
	//--------------------------------------------
	int	count,
			transCount, 
			indexCount,
			colorCount,
			colorCount2,
			index,
			dist;

	register int i;
	register int j;
	register int k;

	int colorSet, colorGradation;

	//---------------------------------------------
	// ����ؾ��ϴ� ��� �ٿ� ���ؼ�..
	//---------------------------------------------
	int rectBottom = pRect->bottom;
	int rectLeft = pRect->left;
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
		// xxxxOOOOOOOOOOOOOO�� ����̹Ƿ�..
		//---------------------------------------------
		// xxxx�κб��� check���ִ� ��ƾ
		//---------------------------------------------
		if (count > 0)
		{
			j = count;
			do {
				transCount = *pPixels++;		// ������ ��			
				indexCount = *pPixels++;		// ���� �ƴ� �� ��			
						
				// ��������ŭ index����			
				index += transCount;			
			
				//---------------------------------------------
				// index�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// ������������ xxxx������ �Ѿ�� ���
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// �������κ� �ǳʶ�
						pDestTemp += index - rectLeft;

						// index�� ��� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyBrightness(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
					//---------------------------------------------
					// ������+�����ƴѻ��� �Ϻα��� ����ϸ� 
					// xxxx������ �Ѿ�� �Ǵ� ���, index�Ϻ� ���
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// ���ʿ��� skip�Ǵ� ���� ��
						
						pPixels += dist;
						indexCount -= dist;
						
						// index�� �Ϻ� ���
						// indexCount����ŭ index������ ����Ѵ�.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
								*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal �� ��� ���
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyBrightness(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;
						index += colorCount;

						// �������ʹ� ��� ����Ѵ�.
						break;
					}
				}		

				
				// index������ŭ index����
				index += indexCount;

				// index����ŭ skip
				pPixels += indexCount;

				// normal ���� ��
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index�������ؼ��� xxxx������ �� �Ѿ��..
				// normal�������ϸ� xxxx������ �Ѿ�� �Ǵ� ���
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// ������ �ƴ� ������ Surface�� ����Ѵ�.
					CSpriteSurface::memcpyBrightness(pDestTemp, pPixels+dist, colorCount2);
					pDestTemp += colorCount2;
					pPixels += colorCount;
					index += colorCount;

					// �������ʹ� ��� ����Ѵ�.
					break;			
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
				do {
					transCount = *pPixels++;		// ������ ��			
					indexCount = *pPixels++;		// ���� �ƴ� �� ��			
							
					// ��������ŭ index����
					index += transCount;
					
					// ����ϰ� �ִٰ� �����ʺκк��� ������� ���ƾ� �� ��찡 �ִ�.
					// ���� ����ϴ� ���� ��� ����� ���̹Ƿ� break�ؾ� �Ѵ�.

					// ���������� ����ϴ°͸����� �� �̻� ����� �ʿ䰡 ���� ���

					//---------------------------------------------
					// index������ ������ ������ �������� ���
					//---------------------------------------------			
					if (index+indexCount > rectRight)
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

							// ���������� skip�Ǵ� ������ ���ش�.
							indexCount = rectRight - index;

							// indexCount����ŭ index������ ����Ѵ�.
							if (indexCount > 0)
							{
								k = indexCount;
								do {
									colorSet		= (*pPixels >> 8) & 0xFF;	// set
									colorGradation	= (*pPixels & 0xFF);			// gradation
									pPixels++;

									// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
									*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
									pDestTemp ++;
								} while (--k);
							}						
							
							break;
						}
					}

					// ��������ŭ �ǳʶ��
					pDestTemp += transCount;

					//-------------------------
					// index�� ��� ���
					//-------------------------
					// indexCount����ŭ index������ ����Ѵ�.
					if (indexCount > 0)
					{
						k = indexCount;
						do {
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
							*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
							pDestTemp ++;
						} while (--k);
					}
					// ����� indexCount��ŭ index����
					index += indexCount;
					

					// Normal ���� ��
					colorCount = *pPixels++;

					//---------------------------------------------
					// normal������ ������ ������ �������� ���
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// ������ �ƴ� ������ Surface�� ����Ѵ�.
						CSpriteSurface::memcpyBrightness(pDestTemp, pPixels, rectRight - index);
						break;			
					}			
					
					//--------------------
					// normal�� ��� ���
					//--------------------
					CSpriteSurface::memcpyBrightness(pDestTemp, pPixels, colorCount);

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
// Blt Brightness Clip Height
//----------------------------------------------------------------------
void		
CIndexSprite::BltBrightnessClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits)
{
	CSpriteSurface::s_Value1 = DarkBits;

	int	count,
			indexCount,
			colorCount;

	WORD	*pDestTemp,
			*pPixels;


	register int  i;
	register int  j;
	register int  k;

	int colorSet, colorGradation;
	int rectBottom = pRect->bottom;
	for (i=pRect->top; i<rectBottom; i++)
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
				pDestTemp += *pPixels++;		// ��������ŭ �ǳ� �ڴ�.
				indexCount = *pPixels++;	// Index�ݺ� ��

				// indexCount����ŭ index������ ����Ѵ�.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue�� ���õ� ���� �����ؼ� ����Ѵ�.				
						*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
						pDestTemp ++;
					} while (--k);
				}

				// Normal ���� ��
				colorCount = *pPixels++;
				
				CSpriteSurface::memcpyBrightness(pDestTemp, pPixels, colorCount);
				
				pDestTemp	+= colorCount;
				pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}
