//----------------------------------------------------------------------
// CIndexSprite.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "CSprite.h"
#include "CSpriteSurface.h"
#include "CIndexSprite.h"

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
// Blt Color에서 기본색으로 사용할 ColorSet
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
		//                     기본색  :  밝음,보통,어두움,
		{ 16, 16, 16 },		// 흰색	
		{ 31, 0, 0 },		// 빨강	
		{ 31, 16, 0 },		// 주황	
		{ 31, 31, 0 },		// 노랑	
		{ 0, 31, 16 },		// 연두	
		{ 0, 31, 0 },		// 초록	
		{ 0, 0, 31 },		// 파랑	
		{ 31, 0, 31 },		// 보라	
		{ 0, 31, 31 },		// 하늘	
		{ 16, 0, 31 },		// 남색	
		{ 16, 16, 31 },		// 짙은 하늘
		{ 31, 16, 16 }		// 살색		
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

		{ 16, 16, 16 }, // 회색
		{ 24, 24, 24 }, // 밝은 회색
		{ 8, 8, 8 }, // 어두운 회색		

		{ 30, 24, 18 }, // 살색
		{ 25, 15, 11 },	// 갈색		
		{ 21, 12, 11 },				
		{ 19, 15, 13 }, // 고동색				

		{ 21, 18, 11 }, // 연한 살색		

		{ 22, 16, 9 } // 살색		
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
			// 첫줄만
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
	// GradationValue와
	//----------------------------------------------------------------------
	for (j=0; j<MAX_COLORGRADATION; j++)
	{
		color = ColorSet[0][j];
		GradationValue[j] = CDirectDraw::Red(color) + CDirectDraw::Green(color) + CDirectDraw::Blue(color);
	}

	//----------------------------------------------------------------------
	// Darkness색을 결정한다.
	//----------------------------------------------------------------------
	for (i=0; i<MAX_COLORSET; i++)
	{
		for (j=0; j<MAX_COLORGRADATION; j++)
		{
			color = ColorSet[i][j];
			//GradationValue[j] = CDirectDraw::Red(color) + CDirectDraw::Green(color) + CDirectDraw::Blue(color);

			// Darkness를 위한 색값
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
	// 색깔 테스트..
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
CIndexSprite::SaveIndexTableToFile(ofstream& file)
{
	int i,j,k;

	int cg = MAX_COLORGRADATION;
	int cs = MAX_COLORSET;
	int db = MAX_DARKBIT;

	//----------------------------------------------------------------------
	// 검증용 숫자들..
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
CIndexSprite::LoadIndexTableFromFile(ifstream& file)
{
	int i,j,k;

	int cg, cs, db;

	//----------------------------------------------------------------------
	// 검증용 숫자들..
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
// 출력에 사용할 ColorSet을 선택한다.
//----------------------------------------------------------------------
void	
CIndexSprite::SetUsingColorSet(int set1, int set2)
{
	s_IndexValue[0] = set1;
	s_IndexValue[1] = set2;
}

//----------------------------------------------------------------------
// 출력에 사용할 ColorSet을 선택한다.
//----------------------------------------------------------------------
void
CIndexSprite::SetUsingColorSetOnly(BYTE setNumber, int colorSet)
{
	s_IndexValue[setNumber] = colorSet;
}


//----------------------------------------------------------------------
// (r0,g0,b0) ~ (r1,g1,b1) 사이의 step개수만큼의 숫자를
// pColor에 넣어준다.
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
// m_Pixels의 memory를 해제한다.
//----------------------------------------------------------------------
void	
CIndexSprite::Release()
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
// Assign Operator
//----------------------------------------------------------------------
void
CIndexSprite::operator = (const CIndexSprite& Sprite)
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

	int colorCount, transPair, indexCount;

	// 메모리 잡기
	m_Pixels = new WORD* [m_Height];

	//--------------------------------
	// 5:6:5
	//--------------------------------
	for (i=0; i<m_Height; i++)
	{
		// 반복 회수의 2 byte
		transPair = Sprite.m_Pixels[i][0];
				
		index	= 1;

		// 각 line마다 byte수를 세어서 저장해야한다.
		for (j=0; j<transPair; j++)
		{			
			// transCount = m_Pixels[i][index];
			index++;	// 투명 수
			indexCount = Sprite.m_Pixels[i][index++];	// indexPair 수

			index += indexCount;		// index색에 대한 정보 수 만큼

			// Normal색 수
			colorCount = Sprite.m_Pixels[i][index++];

			// Normal색 수만큼 증가
			index	+= colorCount;
		}

		// 메모리 잡기
		m_Pixels[i] = new WORD [index];
		memcpy(m_Pixels[i], Sprite.m_Pixels[i], index<<1);
	}

	// 복사 완료
	m_bInit = true;
}


//-----------------------------------------------------------------------------
// Get IndexColor
//-----------------------------------------------------------------------------
// 정해진 gradation에 의해서
// color색에 대응하는 적절한 index색(실제 색이 아닌 index번호)을 찾아준다.
//-----------------------------------------------------------------------------
BYTE
CIndexSprite::GetIndexColor(WORD color)
{
	//-------------------------------------------------------
	// Gradition값
	//-------------------------------------------------------
	WORD spriteGradation = CDirectDraw::Red(color) 
						+ CDirectDraw::Green(color) 
						+ CDirectDraw::Blue(color);
			
	return GetColorToGradation( spriteGradation );
}

//-----------------------------------------------------------------------------
// GetColorToGradation
//-----------------------------------------------------------------------------
// color(0~93)값을 gradation(0~MAX_GRADATION)값으로 바꾼다.
//-----------------------------------------------------------------------------
BYTE
CIndexSprite::GetColorToGradation(BYTE spriteGradation)
{
	// 5:6:5전용 code
	//WORD spriteGradation = (color >> 11) + ((color >> 6) & 0x1F) + (color & 0x1F);
	// 5:5:5전용 code
	//WORD spriteGradation = (color >> 10) + ((color >> 5) & 0x1F) + (color & 0x1F);

	//-------------------------------------------------------
	// spriteGradation값과 가장 가까운 
	// GradationValue를 찾아야 한다.
	//-------------------------------------------------------
  int g = 0;
	for (; g<MAX_COLORGRADATION; g++)
	{			
		if (spriteGradation > GradationValue[g])
		{
			break;
		}
	}

	// 제일 끝의 색깔인 경우
	if (g==0 ||	g==MAX_COLORGRADATION-1)
	{
		return g;
	}

	// 가운데 색깔
	WORD value1 = GradationValue[g-1] - spriteGradation;
	WORD value2 = spriteGradation - GradationValue[g-1];
	
	// 적은 값을 선택한다.
	if (value1 < value2)
	{
		return g-1;
	}
	else if (value1 > value2)
	{
		return g;
	}
	
	// 같은 경우는.. ??	
	return g-1;	
}

//-----------------------------------------------------------------------------
// Set Pixel
//-----------------------------------------------------------------------------
// pixel정보(pSource)와
// 두 개의 index 정보를 가지고 
// IndexSprite를 생성한다.
//
// 우선 ColorInfo와 IndexInfo를 생성하고..
// GenerateFromIndex를 호출한다..
//
// 효율은 좀 떨어지지만 -_-; 최적화하기 귀찮다. reuse라고.. 
// 어차피 게임 실행중에는 쓰일 일이 없으니깐.. 음하.. -_-;;;; 
//-----------------------------------------------------------------------------
void		
CIndexSprite::SetPixel(WORD* pSource, WORD sourcePitch, 
							 WORD* pIndex1, WORD indexPitch1,	// 첫번째 index
							 WORD* pIndex2, WORD indexPitch2,	// 두번째 index
							 WORD width, WORD height)
{
	//--------------------------------------------------
	// 길이가 0일때..
	//--------------------------------------------------
	if (width==0 || height==0)
	{
		Release();
		return;
	}

	WORD	*pSourceTemp, *pIndexTemp1, *pIndexTemp2;
	WORD	transIndex1=0, transIndex2=0;	// default 투명색은 0번(검정)

	//--------------------------------------------------
	// Index정보가 있는 surface의 투명색을 결정한다.
	//--------------------------------------------------
	// pSource의 투명색이 있는 위치가
	// pIndex1, pIndex2에서도 투명색이다.
	//--------------------------------------------------
	WORD	*pSourceTemp2;
	pSourceTemp = pSource;

	for (int i=0; i<height; i++)
	{
		pSourceTemp2 = pSourceTemp;

		for (int j=0; j<width; j++)
		{
			// 투명색
			if (*pSourceTemp2==s_Colorkey)
			{
				// pIndex1과 pIndex2에서 각각 투명색을 결정한다.
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
	// 정보 생성을 위한 메모리 잡기
	//--------------------------------------------------
	WORD** ppColor, **ppIndex;

	ppColor = new WORD* [height];
	ppIndex = new WORD* [height];

	//--------------------------------------------------
	// 체크 체크~
	//--------------------------------------------------
	for (int i=0; i<height; i++)
	{
		//--------------------------------------------------
		// 메모리 잡기
		//--------------------------------------------------
		ppColor[i] = new WORD [width];
		ppIndex[i] = new WORD [width];

		//--------------------------------------------------
		// 첫 점
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
		// 줄 단위 체크..
		//--------------------------------------------------
		for (int j=0; j<width; j++)
		{
			//--------------------------------------------------
			// 투명색이라면..
			//--------------------------------------------------
			if (*pSourceTemp==s_Colorkey)
			{
				ppIndex[i][j] = INDEX_TRANS;
			}
			//--------------------------------------------------
			// 먼저 Index색인지를 체크하고, 
			// 아니면, 실제 색인 경우이다.
			//--------------------------------------------------
			else
			{
				//--------------------------------------------------
				// index1이 있는 것인지 체크해야 한다.
				//--------------------------------------------------
				if (pIndex1!=NULL && *pIndexTemp1!=transIndex1)
				{
					ppIndex[i][j] = GetIndexColor( *pSourceTemp );	// (0<<8) | gradation;
				}
				//--------------------------------------------------
				// index2가 있는 것인지 체크해야 한다.
				//--------------------------------------------------
				else if (pIndex2!=NULL && *pIndexTemp2!=transIndex2)
				{
					ppIndex[i][j] = (1<<8) | GetIndexColor( *pSourceTemp );
				}
				//--------------------------------------------------
				// 둘 다 없는 경우 = 실제 색
				//--------------------------------------------------
				else
				{
					ppIndex[i][j] = INDEX_NULL;

					// 실제 색을 저장한다.
					ppColor[i][j] = *pSourceTemp;
				}				
			}

			pSourceTemp++;

			//--------------------------------------------------
			// 다음 점
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
		// 다음 줄
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
	// ppColor와 ppIndex가 완성되었으므로.. 
	// IndexSprite생성!
	//--------------------------------------------------
	GenerateFromIndex(ppColor, width, height, ppIndex);


	//--------------------------------------------------
	// ppColor와 ppIndex를 지워준다.
	//--------------------------------------------------
	for (int i=0; i<height; i++)
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
// ppColor에서 width*height만큼의 점을 읽어서 Sprite를 만드는데..
// 투명색 압축도 해야하고...
// Index Array의 정보도 참조해야한다.
//
//-----------------------------------------------------------------------------
// - 내부적으로 투명색 압축을 사용한다.
// - IndexSet의 Gradation색깔별로 압축(?)한다. (효율은 높지 않을 듯 하다)
//
//
// [ 한 줄의 정보 ]
//
// (투명Pair반복수) 
//    ( (투명색수, Index색수, 
//                 (indexSet번호, gradation번호), 
//                 (indexSet번호, gradation번호), 
//                              .     
//                              .     
//                              .     
//    ( (투명색수, Index색수, 
//                 (indexSet번호, gradation번호), 
//                 (indexSet번호, gradation번호), 
//                              .     
//                              .     
//                              .     
//        고정된색깔수, 고정된색깔들)
//    )
//
// WORD : 투명색수 
// WORD : 색Pair반복수
// WORD : index색수
// WORD : indexSet번호 | gradation번호 --> BYTE | BYTE
// WORD : 고정된색깔수
// WORD*: 고정된색깔들
//
// 투명색과 Gradation Index값과 색깔값을 같이 저장한다.
//-----------------------------------------------------------------------------
// ppIndex에는 각 2byte에 
// ((IndexSet번호<<8) | Gradation번호)가 저장되어 있어야 한다.
//-----------------------------------------------------------------------------
// Index를 검색하면서.. 
// 투명색인 경우는 투명색 counter증가..
// 아니면.. IndexSet별로 같은 Gradation끼리 압축..
//-----------------------------------------------------------------------------
void			
CIndexSprite::GenerateFromIndex(WORD** ppColor,
								WORD width, WORD height,
								WORD** ppIndex)
{
	// memory해제
	Release();

	m_Width = width;
	m_Height = height;

	// 일단 memory를 적당히 잡아둔다.	
	WORD*	data = new WORD[m_Width*8+10];

	int	index;				// data의 index로 사용
			
	int	transPairCount,					// 투명Pair 개수
			transColorCount,				// 투명색 개수			
			indexColorCount,				// Index색 개수
			normalColorCount;				// NormalColor 개수

	int	indexColorCountPosition,		// IndexColor개수 저장 위치
			normalColorCountPosition;		// NormalColor개수 저장 위치

	enum	COLORTYPE { COLORTYPE_TRANS, COLORTYPE_INDEX, COLORTYPE_NORMAL }
			lastColorType;			// 최근에 검사한 색의 종류
								

	// height줄 만큼 memory잡기
	m_Pixels = new WORD* [height];


	//-----------------------------------
	// test code
	//-----------------------------------
	//int* m_pLineIndex = new int [height];


	for (int  i=0; i<height; i++)
	{
		index = 0;
		transPairCount = 0;
		transColorCount = 0; 		
		lastColorType = COLORTYPE_TRANS;

		//------------------------------------------------------------
		// 각 line에 대해서 압축~
		//------------------------------------------------------------
		for ( int  j=0; j<width; j++)
		{
			//--------------------------------------------------
			// Index가 투명색인 경우
			//--------------------------------------------------
			if ((ppIndex[i][j] & 0xFF)==INDEX_TRANS)
			{
				//--------------------------------------------------
				// 최근에 검사한게 index색이었다면,
				//--------------------------------------------------
				if (lastColorType == COLORTYPE_INDEX)
				{
					// 이전 index색수를 저장해야한다.
					data[indexColorCountPosition] = indexColorCount;

					// ' (투명색수,색Pair반복수, (색Pair들....)) '의 한 set가 끝났음을 의미하므로
					// normal색 수는 0으로해서 저장한다.
					transPairCount++;
					
					data[index++] = 0;					

					transColorCount = 0;
					lastColorType = COLORTYPE_TRANS;
				}
				//--------------------------------------------------
				// 최근에 검사한게 normal색이었다면
				//--------------------------------------------------
				else if (lastColorType == COLORTYPE_NORMAL)
				{
					// ' (투명색수,indexPair반복수, (index반복), 고정색들)) '의 한 set가 끝났음을 의미하므로
					transPairCount++;
					
					data[normalColorCountPosition] = normalColorCount;
					normalColorCount = 0;

					transColorCount = 0;
					lastColorType = COLORTYPE_TRANS;
				}

				
				transColorCount++;				
			}		
			//--------------------------------------------------				
			// Index 색인 경우...
			//--------------------------------------------------
			else if ((ppIndex[i][j] & 0xFF)!=INDEX_NULL)
			{
				//--------------------------------------------------
				// 이전의 것이 투명색이었다면..
				//--------------------------------------------------
				if (lastColorType == COLORTYPE_TRANS)
				{						
					data[index++] = transColorCount;		// 상위 byte에 투명수를 넣는다.
					transColorCount = 0;
					
					// 새로운 index색 값에 대한 초기화..
					// index색의 개수를 저장할 위치
					indexColorCountPosition = index++;
					indexColorCount	= 0;
					
					lastColorType = COLORTYPE_INDEX;
				}
				//--------------------------------------------------
				// 이전에 것이 변하지않는 색인 경우
				//--------------------------------------------------
				else if (lastColorType == COLORTYPE_NORMAL)
				{
					// 변하지 않는 색 끝
					// ' (투명색수,indexPair반복수, (index반복), 고정색들)) '의 한 set가 끝났음을 의미하므로
					transPairCount++;
					
					data[normalColorCountPosition] = normalColorCount;
					normalColorCount = 0;

					// 투명색 없다.. - -;
					data[index++] = 0;

					// index색의 개수를 저장할 위치
					indexColorCountPosition = index++;
					indexColorCount	= 0;					

					lastColorType = COLORTYPE_INDEX;						
				}	
				
				// index색 개수 증가
				indexColorCount++;

				// index색 저장				
				data[index++]	= ppIndex[i][j];
			}
			//--------------------------------------------------				
			// 변하지 않는 색인 경우 (INDEX_NULL)
			//--------------------------------------------------
			else //if ((ppIndex[i][j] & 0xFF)==INDEX_NULL)
			{	
				//--------------------------------------------------
				// 이전 색깔이 투명색이면..
				//--------------------------------------------------
				if (lastColorType == COLORTYPE_TRANS)
				{
					data[index++] = transColorCount;
					transColorCount = 0;
						
					// indexCount는 없다.
					data[index++] = 0;	
						
					// 정상색깔이 나왔으므로 초기화.
					normalColorCountPosition = index++;
					normalColorCount = 0;

					lastColorType = COLORTYPE_NORMAL;	
				}
				else
				//--------------------------------------------------
				// 이전 색깔이 index색이면..
				//--------------------------------------------------
				if (lastColorType == COLORTYPE_INDEX)
				{
					// 이전 index색수를 저장해야한다.
					data[indexColorCountPosition] = indexColorCount;
						
					// 정상색깔이 나왔으므로 초기화.
					normalColorCountPosition = index++;
					normalColorCount = 0;

					lastColorType = COLORTYPE_NORMAL;						
				}

				data[index++] = ppColor[i][j];	// 실제 색깔을 저장한다.
				normalColorCount++;
			}
		}
		
		//--------------------------------------------------
		// 한 줄의 마지막 점이 투명색인가?
		//--------------------------------------------------
		if (lastColorType == COLORTYPE_TRANS)
		{
			// 투명색이면 별다른 처리를 안해줘도 될거 같다.
		}	
		//--------------------------------------------------
		// 마지막 점이 index색인 경우 - indexPair정리..
		//--------------------------------------------------
		else if (lastColorType == COLORTYPE_INDEX)
		{		
			// 이전 index색수를 저장해야한다.
			data[indexColorCountPosition] = indexColorCount;

			// normal색은 없다.
			data[index++] = 0;

			// ' (투명색수,indexPair반복수, (index반복), 고정색들)) '의 한 set가 끝났음을 의미하므로
			transPairCount++;			
		}
		//--------------------------------------------------
		// 마지막 점이 normal색인 경우
		//--------------------------------------------------
		else 
		{
			// ' (투명색수,indexPair반복수, (index반복), 고정색들)) '의 한 set가 끝났음을 의미하므로
			data[normalColorCountPosition] = normalColorCount;	

			transPairCount++;		
		}

		// memory를 잡는다.
		m_Pixels[i] = new WORD [index+1];

		// m_Pixels[i]를 압축했으므로 data로 대체한다.
		// m_Pixels[i][0]에는 count를 넣어야 한다.
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
// ppColor에서 width*height만큼의 점을 읽어서 Sprite를 만드는데..
// 투명색 압축도 해야하고...
// Index Array의 정보도 참조해야한다.
//
//-----------------------------------------------------------------------------
// - 내부적으로 투명색 압축을 사용한다.
// - IndexSet의 Gradation색깔별로 압축(?)한다. (효율은 높지 않을 듯 하다)
//
//
// [ 한 줄의 정보 ]
//
// (투명Pair반복수) 
//    ( (투명색수, IndexPair반복수, 
//                 (index색수, indexSet번호, gradation번호), 
//                 (index색수, indexSet번호, gradation번호), 
//                              .     
//                              .     
//                              .     
//        고정된색깔수, 고정된색깔들)
//       (투명색수, IndexPair반복수, 
//                 (index색수, indexSet번호, gradation번호), 
//                 (index색수, indexSet번호, gradation번호), 
//                              .     
//                              .     
//                              .     
//        고정된색깔수, 고정된색깔들)
//    )
//
// WORD : 투명색수 
// WORD : 색Pair반복수
// WORD : index색수
// WORD : indexSet번호 | gradation번호 --> BYTE | BYTE
// WORD : 고정된색깔수
// WORD*: 고정된색깔들
//
// 투명색과 Gradation Index값과 색깔값을 같이 저장한다.
//-----------------------------------------------------------------------------
// ppIndex에는 각 2byte에 
// ((IndexSet번호<<8) | Gradation번호)가 저장되어 있어야 한다.
//-----------------------------------------------------------------------------
// Index를 검색하면서.. 
// 투명색인 경우는 투명색 counter증가..
// 아니면.. IndexSet별로 같은 Gradation끼리 압축..
//-----------------------------------------------------------------------------
/*
void			
CIndexSprite::GenerateFromIndex(WORD** ppColor,
								WORD width, WORD height,
								WORD** ppIndex)
{
	// memory해제
	Release();

	m_Width = width;
	m_Height = height;

	// 일단 memory를 적당히 잡아둔다.	
	WORD*	data = new WORD[m_Width*8];

	WORD	index;				// data의 index로 사용
			
	WORD	transPairCount,					// 투명Pair 개수
			transColorCount,				// 투명색 개수
			indexColorPairCount,			// IndexPair개수
			indexColorCount,				// Index색 개수
			normalColorCount;				// NormalColor 개수

	WORD	indexColorPairCountPosition,	// IndexPair개수 저장 위치
			indexColorCountPosition,		// IndexColor개수 저장 위치
			normalColorCountPosition;		// NormalColor개수 저장 위치

	enum	COLORTYPE { COLORTYPE_TRANS, COLORTYPE_INDEX, COLORTYPE_NORMAL }
			lastColorType;			// 최근에 검사한 색의 종류
								
	BYTE	lastIndexSet;		// 최근에 검사한 IndexSet
	BYTE	lastIndexColor;		// 최근에 검사한 IndexGradation


	// height줄 만큼 memory잡기
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
		// 각 line에 대해서 압축~
		//------------------------------------------------------------
		for (register int  j=0; j<width; j++)
		{
			//--------------------------------------------------
			// Index가 투명색인 경우
			//--------------------------------------------------
			if ((ppIndex[i][j] & 0xFF)==INDEX_TRANS)
			{
				//--------------------------------------------------
				// 최근에 검사한게 index색이었다면,
				//--------------------------------------------------
				if (lastColorType == COLORTYPE_INDEX)
				{
					// 이전 index색수를 저장해야한다.
					data[indexColorCountPosition] = indexColorCount;

					// indexPair개수 하나 증가
					indexColorPairCount++;

					// indexPair가 모두 끝났으므로.. 저장해야한다.
					data[indexColorPairCountPosition] = indexColorPairCount;
					indexColorPairCount = 0;

					// ' (투명색수,색Pair반복수, (색Pair들....)) '의 한 set가 끝났음을 의미하므로
					// normal색 수는 0으로해서 저장한다.
					transPairCount++;
					
					data[index++] = 0;					

					transColorCount = 0;
					lastColorType = COLORTYPE_TRANS;
				}
				//--------------------------------------------------
				// 최근에 검사한게 normal색이었다면
				//--------------------------------------------------
				else if (lastColorType == COLORTYPE_NORMAL)
				{
					// ' (투명색수,indexPair반복수, (index반복), 고정색들)) '의 한 set가 끝났음을 의미하므로
					transPairCount++;
					
					data[normalColorCountPosition] = normalColorCount;
					normalColorCount = 0;

					transColorCount = 0;
					lastColorType = COLORTYPE_TRANS;
				}

				
				transColorCount++;				
			}		
			//--------------------------------------------------				
			// Index 색인 경우...
			//--------------------------------------------------
			else if ((ppIndex[i][j] & 0xFF)!=INDEX_NULL)
			{
				//--------------------------------------------------
				// 이전의 것이 투명색이었다면..
				//--------------------------------------------------
				if (lastColorType == COLORTYPE_TRANS)
				{						
					data[index++] = transColorCount;		// 상위 byte에 투명수를 넣는다.
					transColorCount = 0;
					
					// 다음에 [IndexPair개수]를 넣을 위치를 기억
					indexColorPairCountPosition = index++;
					indexColorPairCount = 0;		// index색수

					// 새로운 index색 값에 대한 초기화..
					// index색의 개수를 저장할 위치
					indexColorCountPosition = index++;
					indexColorCount	= 1;

					// index색이 처음 나오므로.. 초기화..
					lastIndexSet	= (ppIndex[i][j]>>8) & 0xFF;
					lastIndexColor	= ppIndex[i][j] & 0xFF;	

					// 저장
					data[index++]	= (lastIndexSet<<8) | lastIndexColor;
					
					lastColorType = COLORTYPE_INDEX;
				}
				//--------------------------------------------------
				// 이전에 것이 변하지않는 색인 경우
				//--------------------------------------------------
				else if (lastColorType == COLORTYPE_NORMAL)
				{
					// 변하지 않는 색 끝
					// ' (투명색수,indexPair반복수, (index반복), 고정색들)) '의 한 set가 끝났음을 의미하므로
					transPairCount++;
					
					data[normalColorCountPosition] = normalColorCount;
					normalColorCount = 0;

					// 투명색 없다.. - -;
					data[index++] = 0;

					// 다음에 [IndexPair개수]를 넣을 위치를 기억
					indexColorPairCountPosition = index++;
					indexColorPairCount = 0;		// index색수

					// index색의 개수를 저장할 위치
					indexColorCountPosition = index++;
					indexColorCount	= 1;

					// index색이 처음 나오므로.. 초기화..
					lastIndexSet	= (ppIndex[i][j]>>8) & 0xFF;
					lastIndexColor	= ppIndex[i][j] & 0xFF;

					// 저장
					data[index++]	= (lastIndexSet<<8) | lastIndexColor;

					lastColorType = COLORTYPE_INDEX;						
				}	
				//--------------------------------------------------
				// 이전에 것이 index색인 경우
				//--------------------------------------------------
				else 			
				{
					//--------------------------------------------------
					// 이전 index색이랑 Set값과 Gradation값이 같은 경우
					//--------------------------------------------------
					if (lastIndexSet == ((ppIndex[i][j]>>8) & 0xFF)
						&& lastIndexColor == (ppIndex[i][j] & 0xFF))
					{
						// 같은 index색이므로 count증가
						indexColorCount++;
					}
					//--------------------------------------------------
					// 다른 index색이 나타난다.
					//--------------------------------------------------
					else
					{	
						// 이전 index색수를 저장해야한다.
						data[indexColorCountPosition] = indexColorCount;						

						// indexPair개수 하나 증가
						indexColorPairCount++;

						// 새로운 index색 값에 대한 초기화..
						// index색의 개수를 저장할 위치
						indexColorCountPosition = index++;
						indexColorCount	= 1;

						// index색이 처음 나오므로.. 초기화..
						lastIndexSet	= (ppIndex[i][j]>>8) & 0xFF;
						lastIndexColor	= ppIndex[i][j] & 0xFF;	

						// 저장
						data[index++]	= (lastIndexSet<<8) | lastIndexColor;
					}							
				}
			}
			//--------------------------------------------------				
			// 변하지 않는 색인 경우 (INDEX_NULL)
			//--------------------------------------------------
			else //if ((ppIndex[i][j] & 0xFF)==INDEX_NULL)
			{	
				//--------------------------------------------------
				// 이전 색깔이 투명색이면..
				//--------------------------------------------------
				if (lastColorType == COLORTYPE_TRANS)
				{
					data[index++] = transColorCount;
					transColorCount = 0;
						
					// indexPair는 없다.
					data[index++] = 0;	
						
					// 정상색깔이 나왔으므로 초기화.
					normalColorCountPosition = index++;
					normalColorCount = 0;

					lastColorType = COLORTYPE_NORMAL;	
				}
				else
				//--------------------------------------------------
				// 이전 색깔이 index색이면..
				//--------------------------------------------------
				if (lastColorType == COLORTYPE_INDEX)
				{
					// 이전 index색수를 저장해야한다.
					data[indexColorCountPosition] = indexColorCount;

					// indexPair개수 하나 증가
					indexColorPairCount++;

					// indexPair가 모두 끝났으므로.. 저장해야한다.
					data[indexColorPairCountPosition] = indexColorPairCount;
					indexColorPairCount = 0;

						
					// 정상색깔이 나왔으므로 초기화.
					normalColorCountPosition = index++;
					normalColorCount = 0;

					lastColorType = COLORTYPE_NORMAL;						
				}

				data[index++] = ppColor[i][j];	// 실제 색깔을 저장한다.
				normalColorCount++;
			}
		}
		
		//--------------------------------------------------
		// 한 줄의 마지막 점이 투명색인가?
		//--------------------------------------------------
		if (lastColorType == COLORTYPE_TRANS)
		{
			// 투명색이면 별다른 처리를 안해줘도 될거 같다.
		}	
		//--------------------------------------------------
		// 마지막 점이 index색인 경우 - indexPair정리..
		//--------------------------------------------------
		else if (lastColorType == COLORTYPE_INDEX)
		{		
			// 이전 index색수를 저장해야한다.
			data[indexColorCountPosition] = indexColorCount;

			// indexPair개수 하나 증가
			indexColorPairCount++;

			// indexPair가 모두 끝났으므로.. 저장해야한다.
			data[indexColorPairCountPosition] = indexColorPairCount;
			indexColorPairCount = 0;

			// normal색은 없다.
			data[index++] = 0;

			// ' (투명색수,indexPair반복수, (index반복), 고정색들)) '의 한 set가 끝났음을 의미하므로
			transPairCount++;			
		}
		//--------------------------------------------------
		// 마지막 점이 normal색인 경우
		//--------------------------------------------------
		else 
		{
			// ' (투명색수,indexPair반복수, (index반복), 고정색들)) '의 한 set가 끝났음을 의미하므로
			data[normalColorCountPosition] = normalColorCount;	

			transPairCount++;		
		}

		// memory를 잡는다.
		m_Pixels[i] = new WORD [index+1];

		// m_Pixels[i]를 압축했으므로 data로 대체한다.
		// m_Pixels[i][0]에는 count를 넣어야 한다.
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
// 현재 저장된 m_Pixels의 값으로 IndexInfo를 생성해야 한다.
// ppIndex에는 모두 INDEX_TRANS값이 들어가 있다고 가정한다.
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

		// (투명수,Index반복,색깔들)의 반복 수		
		transPair	= *pPixels++;
		
 		// 한 줄 출력
		if (transPair > 0)
		{
			j = transPair;
			do {		
				pDest += *pPixels++;	// 투명색만큼 건너 뛴다.
				indexCount = *pPixels++;		// Index반복 수

				// indexCount수만큼 index색들을 출력한다.
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

				// Normal 색깔 수
				colorCount = *pPixels++;
				
				// Normal 색깔 수만큼 INDEX_NULL을 출력.
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
// Sprite안에서 (x,y)는 색깔이 있는가?(투명색이 아닌 경우)
//----------------------------------------------------------------------
bool		
CIndexSprite::IsColorPixel(short x, short y)
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

		int	index = 0;

		//    ( (투명색수, Index색수, 
		//                 (indexSet번호, gradation번호), 
		//                 (indexSet번호, gradation번호), 

		register int i;

		if (count > 0)
		{
			i = count;
			do {
				index += *pPixels++;

				// 투명색까지보다 적은 경우
				if (x < index)
				{
					return false;
				}

				// index색수만큼 증가		
				index += *pPixels;

				// index색 정보만큼 증가.
				pPixels += *pPixels;

				pPixels++;


				// 고정된 색깔수만큼 증가
				index += *pPixels;

				// 고정색 정보만큼 증가.
				pPixels += *pPixels;

				pPixels++;


				// index+고정색까지보다 적은 경우
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
// Sprite안에서 (x,y)는 색깔을 얻는다.(투명색이 아닌 경우)
//----------------------------------------------------------------------
WORD		
CIndexSprite::GetPixel(int x, int y) const
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

		int	index = 0;

		//    ( (투명색수, Index색수, 
		//                 (indexSet번호, gradation번호), 
		//                 (indexSet번호, gradation번호), 

		register int i;

		if (count > 0)
		{
			i = count;
			do {
				index += *pPixels++;

				// 투명색까지보다 적은 경우
				if (x < index)
				{
					return 0;
				}

				// index색수만큼 증가		
				index += *pPixels;

				// index색 정보만큼 증가.
				pPixels += *pPixels;

				pPixels++;


				// 고정된 색깔수만큼 증가
				index += *pPixels;

				// 고정색 정보만큼 증가.
				pPixels += *pPixels;

				pPixels++;


				// index+고정색까지보다 적은 경우
				if (x < index)
				{
					// [ TEST CODE ]
					// 아직 완성된게 아니다.
					// 현재의 IndexColor를 참조해서 색을 넘겨줘야 한다?
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
//    ( (투명색수, Index색수, 
//                 (indexSet번호, gradation번호), 
//                 (indexSet번호, gradation번호), 
//-----------------------------------------------------------------------------
// Clipping하지 않는다.
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

			// (투명수,Index색,색깔들)의 반복 수		
			transPair	= *pPixels++;
			
 			// 한 줄 출력		
			if (transPair > 0)
			{
				j = transPair;
				do
				{
					pDestTemp += *pPixels++;	// 투명색만큼 건너 뛴다.
					indexCount = *pPixels++;	// Index반복 수

					// indexCount수만큼 index색들을 출력한다.
					// 한 줄 출력		
					if (indexCount > 0)
					{
						k = indexCount;
						do
						{
							colorSet		= (*pPixels >> 8) & 0xFF;						
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;
							
							// s_IndexValue와 관련된 색을 선택해서 출력한다.
							*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
							pDestTemp ++;
						} while (--k);
					}

					// Normal 색깔 수
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
//    ( (투명색수, Index색수, 
//                 (indexSet번호, gradation번호), 
//                 (indexSet번호, gradation번호), 
//-----------------------------------------------------------------------------
// 왼쪽 clipping.  
// rectLeft개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CIndexSprite::BltClipLeft(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
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
		if (count > 0)
		{
			j = count;
			do
			{
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;			
			
				//---------------------------------------------
				// index색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - rectLeft;

						// index색 모두 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do
							{
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우, index일부 출력
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// 왼쪽에서 skip되는 점들 수
						
						pPixels += dist;
						indexCount -= dist;
						
						// index색 일부 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do
							{
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}		

				
				// index색수만큼 index증가
				index += indexCount;

				// index색깔만큼 skip
				pPixels += indexCount;

				// normal 색깔 수
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index색까지해서는 xxxx범위를 안 넘어가고..
				// normal색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// 투명이 아닌 색들을 Surface에 출력한다.
					memcpy(pDestTemp, pPixels+dist, colorCount2<<1);					
					pDestTemp += colorCount2;
					pPixels += colorCount;

					// 이제부터는 계속 출력한다.
					break;			
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
				do {
					pDestTemp += *pPixels++;	// 투명색만큼 건너 뛴다.
					indexCount = *pPixels++;	// Index반복 수

					// indexCount수만큼 index색들을 출력한다.
					if (indexCount > 0)
					{
						k = indexCount;
						do
						{				
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue와 관련된 색을 선택해서 출력한다.				
							*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
							pDestTemp ++;
						} while (--k);
					}

					// Normal 색깔 수
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
// 오른쪽 clipping.  
// rectRight개 까지의 점만 pDest에 출력한다.
//----------------------------------------------------------------------
void
CIndexSprite::BltClipRight(WORD* pDest, WORD pitch, RECT* pRect)
{	
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
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
			do {
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가
				index += transCount;
				
				// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
				// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

				// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

				//---------------------------------------------
				// index색까지 오른쪽 끝까지 도달했을 경우
				//---------------------------------------------			
				if (index+indexCount > rectRight)
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

						// 오른쪽으로 skip되는 점들을 빼준다.
						indexCount = rectRight - index;

						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}						
						
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				//-------------------------
				// index색 모두 출력
				//-------------------------
				// indexCount수만큼 index색들을 출력한다.
				if (indexCount > 0)
				{
					k = indexCount;
					do {				
						colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue와 관련된 색을 선택해서 출력한다.				
						*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
						pDestTemp ++;
					} while (--k);
				}		
				// 출력한 indexCount만큼 index증가
				index += indexCount;
				

				// Normal 색깔 수
				colorCount = *pPixels++;

				//---------------------------------------------
				// normal색까지 오른쪽 끝까지 도달했을 경우
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// 투명이 아닌 색들을 Surface에 출력한다.
					memcpy(pDestTemp, pPixels, (rectRight - index)<<1);
					break;			
				}			
				
				//--------------------
				// normal색 모두 출력
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
//    ( (투명색수, Index색수, 
//                 (indexSet번호, gradation번호), 
//                 (indexSet번호, gradation번호), 
//-----------------------------------------------------------------------------
// 왼쪽 clipping.  
// rectLeft개의 점을 건너띈 다음부터 pDest에 출력한다.
// rectRight까지..
//----------------------------------------------------------------------
void
CIndexSprite::BltClipWidth(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
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
			do {		
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;			
			
				//---------------------------------------------
				// index색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - rectLeft;

						// index색 모두 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {				
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우, index일부 출력
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// 왼쪽에서 skip되는 점들 수
						
						pPixels += dist;
						indexCount -= dist;
						
						// index색 일부 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						memcpy((void*)pDestTemp, (void*)pPixels, colorCount<<1);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;
						index += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}		

				
				// index색수만큼 index증가
				index += indexCount;

				// index색깔만큼 skip
				pPixels += indexCount;

				// normal 색깔 수
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index색까지해서는 xxxx범위를 안 넘어가고..
				// normal색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// 투명이 아닌 색들을 Surface에 출력한다.
					memcpy(pDestTemp, pPixels+dist, colorCount2<<1);					
					pDestTemp += colorCount2;
					pPixels += colorCount;
					index += colorCount;

					// 이제부터는 계속 출력한다.
					break;			
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
				do {
					transCount = *pPixels++;		// 투명색 수			
					indexCount = *pPixels++;		// 투명 아닌 색 수			
							
					// 투명색만큼 index증가
					index += transCount;
					
					// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
					// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

					// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

					//---------------------------------------------
					// index색까지 오른쪽 끝까지 도달했을 경우
					//---------------------------------------------			
					if (index+indexCount > rectRight)
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

							// 오른쪽으로 skip되는 점들을 빼준다.
							indexCount = rectRight - index;

							// indexCount수만큼 index색들을 출력한다.
							if (indexCount > 0)
							{
								k = indexCount;
								do {
									colorSet		= (*pPixels >> 8) & 0xFF;	// set
									colorGradation	= (*pPixels & 0xFF);			// gradation
									pPixels++;

									// s_IndexValue와 관련된 색을 선택해서 출력한다.				
									*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
									pDestTemp ++;
								} while (--k);
							}						
							
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					//-------------------------
					// index색 모두 출력
					//-------------------------
					// indexCount수만큼 index색들을 출력한다.
					if (indexCount > 0)
					{
						k = indexCount;
						do {
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue와 관련된 색을 선택해서 출력한다.				
							*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
							pDestTemp ++;
						} while (--k);
					}		
					// 출력한 indexCount만큼 index증가
					index += indexCount;
					

					// Normal 색깔 수
					colorCount = *pPixels++;

					//---------------------------------------------
					// normal색까지 오른쪽 끝까지 도달했을 경우
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// 투명이 아닌 색들을 Surface에 출력한다.
						memcpy(pDestTemp, pPixels, (rectRight - index)<<1);
						break;			
					}			
					
					//--------------------
					// normal색 모두 출력
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
// pRect->top, rectBottom만큼만 출력한다.
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

		// (투명수,색깔수,색깔들)의 반복 수		
		count	= *pPixels++;		

		// 한 줄 출력
		if (count > 0)
		{
			j = count;
			do {		
				pDestTemp += *pPixels++;		// 투명색만큼 건너 뛴다.
				indexCount = *pPixels++;	// Index반복 수

				// indexCount수만큼 index색들을 출력한다.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue와 관련된 색을 선택해서 출력한다.				
						*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
						pDestTemp ++;
					} while (--k);
				}

				// Normal 색깔 수
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

			// (투명수,Index색,색깔들)의 반복 수		
			transPair	= *pPixels++;
			
 			// 한 줄 출력
			if (transPair > 0)
			{
				j = transPair;
				do {
					pDestTemp += *pPixels++;	// 투명색만큼 건너 뛴다.
					indexCount = *pPixels++;	// Index반복 수

					// indexCount수만큼 index색들을 출력한다.
					if (indexCount > 0)
					{
						k = indexCount;
						do {
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue와 관련된 색을 선택해서 출력한다.				
							*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
							pDestTemp ++;
						} while (--k);
					}

					// Normal 색깔 수
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
	// pRect만큼의 점을 출력한다.
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
			do {		
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;			
			
				//---------------------------------------------
				// index색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - rectLeft;

						// index색 모두 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {						
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우, index일부 출력
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// 왼쪽에서 skip되는 점들 수
						
						pPixels += dist;
						indexCount -= dist;
						
						// index색 일부 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}		

				
				// index색수만큼 index증가
				index += indexCount;

				// index색깔만큼 skip
				pPixels += indexCount;

				// normal 색깔 수
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index색까지해서는 xxxx범위를 안 넘어가고..
				// normal색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// 투명이 아닌 색들을 Surface에 출력한다.
					CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, colorCount2);
					pDestTemp += colorCount2;
					pPixels += colorCount;

					// 이제부터는 계속 출력한다.
					break;			
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
				do {		
					pDestTemp += *pPixels++;	// 투명색만큼 건너 뛴다.
					indexCount = *pPixels++;	// Index반복 수

					// indexCount수만큼 index색들을 출력한다.
					if (indexCount > 0)
					{
						k = indexCount;
						do {			
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue와 관련된 색을 선택해서 출력한다.				
							*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
							pDestTemp ++;
						} while (--k);
					}

					// Normal 색깔 수
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
	// pRect만큼의 점을 출력한다.
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
			do {
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가
				index += transCount;
				
				// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
				// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

				// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

				//---------------------------------------------
				// index색까지 오른쪽 끝까지 도달했을 경우
				//---------------------------------------------			
				if (index+indexCount > rectRight)
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

						// 오른쪽으로 skip되는 점들을 빼준다.
						indexCount = rectRight - index;

						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}						
						
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				//-------------------------
				// index색 모두 출력
				//-------------------------
				// indexCount수만큼 index색들을 출력한다.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue와 관련된 색을 선택해서 출력한다.				
						*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
						pDestTemp ++;
					} while (--k);
				}		
				// 출력한 indexCount만큼 index증가
				index += indexCount;
				

				// Normal 색깔 수
				colorCount = *pPixels++;

				//---------------------------------------------
				// normal색까지 오른쪽 끝까지 도달했을 경우
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// 투명이 아닌 색들을 Surface에 출력한다.
					CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, rectRight - index);
					break;			
				}			
				
				//--------------------
				// normal색 모두 출력
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
	// pRect만큼의 점을 출력한다.
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
			do {
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;			
			
				//---------------------------------------------
				// index색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - rectLeft;

						// index색 모두 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우, index일부 출력
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// 왼쪽에서 skip되는 점들 수
						
						pPixels += dist;
						indexCount -= dist;
						
						// index색 일부 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;
						index += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}		

				
				// index색수만큼 index증가
				index += indexCount;

				// index색깔만큼 skip
				pPixels += indexCount;

				// normal 색깔 수
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index색까지해서는 xxxx범위를 안 넘어가고..
				// normal색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// 투명이 아닌 색들을 Surface에 출력한다.
					CSpriteSurface::memcpyDarkness(pDestTemp, pPixels+dist, colorCount2);
					pDestTemp += colorCount2;
					pPixels += colorCount;
					index += colorCount;

					// 이제부터는 계속 출력한다.
					break;			
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
				do {
					transCount = *pPixels++;		// 투명색 수			
					indexCount = *pPixels++;		// 투명 아닌 색 수			
							
					// 투명색만큼 index증가
					index += transCount;
					
					// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
					// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

					// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

					//---------------------------------------------
					// index색까지 오른쪽 끝까지 도달했을 경우
					//---------------------------------------------			
					if (index+indexCount > rectRight)
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

							// 오른쪽으로 skip되는 점들을 빼준다.
							indexCount = rectRight - index;

							// indexCount수만큼 index색들을 출력한다.
							if (indexCount > 0)
							{
								k = indexCount;
								do {
									colorSet		= (*pPixels >> 8) & 0xFF;	// set
									colorGradation	= (*pPixels & 0xFF);			// gradation
									pPixels++;

									// s_IndexValue와 관련된 색을 선택해서 출력한다.				
									*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
									pDestTemp ++;
								} while (--k);
							}						
							
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					//-------------------------
					// index색 모두 출력
					//-------------------------
					// indexCount수만큼 index색들을 출력한다.
					if (indexCount > 0)
					{
						k = indexCount;
						do {
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue와 관련된 색을 선택해서 출력한다.				
							*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
							pDestTemp ++;
						} while (--k);
					}
					// 출력한 indexCount만큼 index증가
					index += indexCount;
					

					// Normal 색깔 수
					colorCount = *pPixels++;

					//---------------------------------------------
					// normal색까지 오른쪽 끝까지 도달했을 경우
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// 투명이 아닌 색들을 Surface에 출력한다.
						CSpriteSurface::memcpyDarkness(pDestTemp, pPixels, rectRight - index);
						break;			
					}			
					
					//--------------------
					// normal색 모두 출력
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

		// (투명수,색깔수,색깔들)의 반복 수		
		count	= *pPixels++;		

		// 한 줄 출력
		if (count > 0)
		{
			j = count;
			do {
				pDestTemp += *pPixels++;		// 투명색만큼 건너 뛴다.
				indexCount = *pPixels++;	// Index반복 수

				// indexCount수만큼 index색들을 출력한다.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue와 관련된 색을 선택해서 출력한다.				
						*pDestTemp		= ColorSetDarkness[DarkBits][s_IndexValue[colorSet]][colorGradation];
						pDestTemp ++;
					} while (--k);
				}

				// Normal 색깔 수
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

			// (투명수,Index색,색깔들)의 반복 수		
			transPair	= *pPixels++;
			
 			// 한 줄 출력
			if (transPair > 0)
			{
				j = transPair;
				do {				
					pDestTemp += *pPixels++;	// 투명색만큼 건너 뛴다.
					indexCount = *pPixels++;	// Index반복 수

					// indexCount수만큼 index색들을 출력한다.
					if (indexCount > 0)
					{
						k = indexCount;
						do {
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue와 관련된 색을 선택해서 출력한다.				
							*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
							pDestTemp ++;
						} while (--k);
					}

					// Normal 색깔 수
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
	// pRect만큼의 점을 출력한다.
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
			do {
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;			
			
				//---------------------------------------------
				// index색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - rectLeft;

						// index색 모두 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우, index일부 출력
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// 왼쪽에서 skip되는 점들 수
						
						pPixels += dist;
						indexCount -= dist;
						
						// index색 일부 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}		

				
				// index색수만큼 index증가
				index += indexCount;

				// index색깔만큼 skip
				pPixels += indexCount;

				// normal 색깔 수
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index색까지해서는 xxxx범위를 안 넘어가고..
				// normal색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// 투명이 아닌 색들을 Surface에 출력한다.
					CSpriteSurface::memcpyAlpha(pDestTemp, pPixels+dist, colorCount2);
					pDestTemp += colorCount2;
					pPixels += colorCount;

					// 이제부터는 계속 출력한다.
					break;			
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
				do {
					pDestTemp += *pPixels++;	// 투명색만큼 건너 뛴다.
					indexCount = *pPixels++;	// Index반복 수

					// indexCount수만큼 index색들을 출력한다.
					if (indexCount > 0)
					{
						k = indexCount;
						do {
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue와 관련된 색을 선택해서 출력한다.				
							*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
							pDestTemp ++;
						} while (--k);
					}

					// Normal 색깔 수
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
	// pRect만큼의 점을 출력한다.
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
			do {
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가
				index += transCount;
				
				// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
				// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

				// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

				//---------------------------------------------
				// index색까지 오른쪽 끝까지 도달했을 경우
				//---------------------------------------------			
				if (index+indexCount > rectRight)
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

						// 오른쪽으로 skip되는 점들을 빼준다.
						indexCount = rectRight - index;

						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
								pDestTemp ++;
							} while (--k);
						}						
						
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				//-------------------------
				// index색 모두 출력
				//-------------------------
				// indexCount수만큼 index색들을 출력한다.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue와 관련된 색을 선택해서 출력한다.				
						*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
						pDestTemp ++;
					} while (--k);
				}		
				// 출력한 indexCount만큼 index증가
				index += indexCount;
				

				// Normal 색깔 수
				colorCount = *pPixels++;

				//---------------------------------------------
				// normal색까지 오른쪽 끝까지 도달했을 경우
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// 투명이 아닌 색들을 Surface에 출력한다.
					CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, rectRight - index);
					break;			
				}			
				
				//--------------------
				// normal색 모두 출력
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
	// pRect만큼의 점을 출력한다.
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
			do {
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;			
			
				//---------------------------------------------
				// index색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - rectLeft;

						// index색 모두 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우, index일부 출력
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// 왼쪽에서 skip되는 점들 수
						
						pPixels += dist;
						indexCount -= dist;
						
						// index색 일부 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;
						index += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}		

				
				// index색수만큼 index증가
				index += indexCount;

				// index색깔만큼 skip
				pPixels += indexCount;

				// normal 색깔 수
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index색까지해서는 xxxx범위를 안 넘어가고..
				// normal색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// 투명이 아닌 색들을 Surface에 출력한다.
					CSpriteSurface::memcpyAlpha(pDestTemp, pPixels+dist, colorCount2);
					pDestTemp += colorCount2;
					pPixels += colorCount;
					index += colorCount;

					// 이제부터는 계속 출력한다.
					break;			
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
				do {
					transCount = *pPixels++;		// 투명색 수			
					indexCount = *pPixels++;		// 투명 아닌 색 수			
							
					// 투명색만큼 index증가
					index += transCount;
					
					// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
					// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

					// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

					//---------------------------------------------
					// index색까지 오른쪽 끝까지 도달했을 경우
					//---------------------------------------------			
					if (index+indexCount > rectRight)
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

							// 오른쪽으로 skip되는 점들을 빼준다.
							indexCount = rectRight - index;

							// indexCount수만큼 index색들을 출력한다.
							if (indexCount > 0)
							{
								k = indexCount;
								do {
									colorSet		= (*pPixels >> 8) & 0xFF;	// set
									colorGradation	= (*pPixels & 0xFF);			// gradation
									pPixels++;

									// s_IndexValue와 관련된 색을 선택해서 출력한다.				
									*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
									pDestTemp ++;
								} while (--k);
							}						
							
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					//-------------------------
					// index색 모두 출력
					//-------------------------
					// indexCount수만큼 index색들을 출력한다.
					if (indexCount > 0)
					{
						k = indexCount;
						do {
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue와 관련된 색을 선택해서 출력한다.				
							*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
							pDestTemp ++;
						} while (--k);
					}		
					// 출력한 indexCount만큼 index증가
					index += indexCount;
					

					// Normal 색깔 수
					colorCount = *pPixels++;

					//---------------------------------------------
					// normal색까지 오른쪽 끝까지 도달했을 경우
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// 투명이 아닌 색들을 Surface에 출력한다.
						CSpriteSurface::memcpyAlpha(pDestTemp, pPixels, rectRight - index);
						break;			
					}			
					
					//--------------------
					// normal색 모두 출력
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

		// (투명수,색깔수,색깔들)의 반복 수		
		count	= *pPixels++;		

		// 한 줄 출력
		if (count > 0)
		{
			j = count;
			do {						
				pDestTemp += *pPixels++;		// 투명색만큼 건너 뛴다.
				indexCount = *pPixels++;	// Index반복 수

				// indexCount수만큼 index색들을 출력한다.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue와 관련된 색을 선택해서 출력한다.				
						*pDestTemp		= CSpriteSurface::memcpyAlpha1Pixel(*pDestTemp, ColorSet[s_IndexValue[colorSet]][colorGradation]);
						pDestTemp ++;
					} while (--k);
				}

				// Normal 색깔 수
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
// Clipping하지 않는다.
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

			// (투명수,Index색,색깔들)의 반복 수		
			transPair	= *pPixels++;
			
 			// 한 줄 출력		
			if (transPair > 0)
			{
				j = transPair;
				do
				{
					pDestTemp += *pPixels++;	// 투명색만큼 건너 뛴다.
					indexCount = *pPixels++;	// Index반복 수

					// indexCount수만큼 index색들을 출력한다.
					// 한 줄 출력		
					if (indexCount > 0)
					{
						k = indexCount;
						do
						{
							colorSet		= (*pPixels >> 8) & 0xFF;						
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue와 관련된 색을 선택해서 출력한다.				
							//*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];

							WORD color = GradationValue[s_IndexValue[colorSet]][colorGradation];//*pPixels;							
							int change = color >> 4;	// 0~96 --> 0~6

							*pDestTemp = *(pDestTemp + change);

							
							pDestTemp ++;
						} while (--k);
					}

					// Normal 색깔 수
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

			// (투명수,Index색,색깔들)의 반복 수		
			transPair	= *pPixels++;
			
 			// 한 줄 출력
			if (transPair > 0)
			{
				j = transPair;
				do {
					pDestTemp += *pPixels++;	// 투명색만큼 건너 뛴다.
					indexCount = *pPixels++;	// Index반복 수

					// indexCount수만큼 index색들을 출력한다.
					if (indexCount > 0)
					{
						k = indexCount;
						do 
						{
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue와 관련된 색을 선택해서 출력한다.				
							*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
							pDestTemp ++;
						} while (--k);
					}

					// Normal 색깔 수
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
	// pRect만큼의 점을 출력한다.
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
			do {		
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;			
			
				//---------------------------------------------
				// index색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - rectLeft;

						// index색 모두 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {						
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyColor(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우, index일부 출력
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// 왼쪽에서 skip되는 점들 수
						
						pPixels += dist;
						indexCount -= dist;
						
						// index색 일부 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyColor(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}		

				
				// index색수만큼 index증가
				index += indexCount;

				// index색깔만큼 skip
				pPixels += indexCount;

				// normal 색깔 수
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index색까지해서는 xxxx범위를 안 넘어가고..
				// normal색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// 투명이 아닌 색들을 Surface에 출력한다.
					CSpriteSurface::memcpyColor(pDestTemp, pPixels+dist, colorCount2);
					pDestTemp += colorCount2;
					pPixels += colorCount;

					// 이제부터는 계속 출력한다.
					break;			
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
				do {		
					pDestTemp += *pPixels++;	// 투명색만큼 건너 뛴다.
					indexCount = *pPixels++;	// Index반복 수

					// indexCount수만큼 index색들을 출력한다.
					if (indexCount > 0)
					{
						k = indexCount;
						do {			
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue와 관련된 색을 선택해서 출력한다.				
							*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
							pDestTemp ++;
						} while (--k);
					}

					// Normal 색깔 수
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
	// rgb값 설정
	CSpriteSurface::s_Value1 = rgb;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
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
			do {
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가
				index += transCount;
				
				// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
				// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

				// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

				//---------------------------------------------
				// index색까지 오른쪽 끝까지 도달했을 경우
				//---------------------------------------------			
				if (index+indexCount > rectRight)
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

						// 오른쪽으로 skip되는 점들을 빼준다.
						indexCount = rectRight - index;

						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
								pDestTemp ++;
							} while (--k);
						}						
						
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				//-------------------------
				// index색 모두 출력
				//-------------------------
				// indexCount수만큼 index색들을 출력한다.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue와 관련된 색을 선택해서 출력한다.				
						*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
						pDestTemp ++;
					} while (--k);
				}		
				// 출력한 indexCount만큼 index증가
				index += indexCount;
				

				// Normal 색깔 수
				colorCount = *pPixels++;

				//---------------------------------------------
				// normal색까지 오른쪽 끝까지 도달했을 경우
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// 투명이 아닌 색들을 Surface에 출력한다.
					CSpriteSurface::memcpyColor(pDestTemp, pPixels, rectRight - index);
					break;			
				}			
				
				//--------------------
				// normal색 모두 출력
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
	// rgb값 설정
	CSpriteSurface::s_Value1 = rgb;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
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
			do {
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;			
			
				//---------------------------------------------
				// index색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - rectLeft;

						// index색 모두 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyColor(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우, index일부 출력
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// 왼쪽에서 skip되는 점들 수
						
						pPixels += dist;
						indexCount -= dist;
						
						// index색 일부 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyColor(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;
						index += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}		

				
				// index색수만큼 index증가
				index += indexCount;

				// index색깔만큼 skip
				pPixels += indexCount;

				// normal 색깔 수
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index색까지해서는 xxxx범위를 안 넘어가고..
				// normal색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// 투명이 아닌 색들을 Surface에 출력한다.
					CSpriteSurface::memcpyColor(pDestTemp, pPixels+dist, colorCount2);
					pDestTemp += colorCount2;
					pPixels += colorCount;
					index += colorCount;

					// 이제부터는 계속 출력한다.
					break;			
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
				do {
					transCount = *pPixels++;		// 투명색 수			
					indexCount = *pPixels++;		// 투명 아닌 색 수			
							
					// 투명색만큼 index증가
					index += transCount;
					
					// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
					// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

					// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

					//---------------------------------------------
					// index색까지 오른쪽 끝까지 도달했을 경우
					//---------------------------------------------			
					if (index+indexCount > rectRight)
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

							// 오른쪽으로 skip되는 점들을 빼준다.
							indexCount = rectRight - index;

							// indexCount수만큼 index색들을 출력한다.
							if (indexCount > 0)
							{
								k = indexCount;
								do {
									colorSet		= (*pPixels >> 8) & 0xFF;	// set
									colorGradation	= (*pPixels & 0xFF);			// gradation
									pPixels++;

									// s_IndexValue와 관련된 색을 선택해서 출력한다.				
									*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
									pDestTemp ++;
								} while (--k);
							}						
							
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					//-------------------------
					// index색 모두 출력
					//-------------------------
					// indexCount수만큼 index색들을 출력한다.
					if (indexCount > 0)
					{
						k = indexCount;
						do {
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue와 관련된 색을 선택해서 출력한다.				
							*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
							pDestTemp ++;
						} while (--k);
					}
					// 출력한 indexCount만큼 index증가
					index += indexCount;
					

					// Normal 색깔 수
					colorCount = *pPixels++;

					//---------------------------------------------
					// normal색까지 오른쪽 끝까지 도달했을 경우
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// 투명이 아닌 색들을 Surface에 출력한다.
						CSpriteSurface::memcpyColor(pDestTemp, pPixels, rectRight - index);
						break;			
					}			
					
					//--------------------
					// normal색 모두 출력
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
	// rgb값 설정
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

		// (투명수,색깔수,색깔들)의 반복 수		
		count	= *pPixels++;		

		// 한 줄 출력
		if (count > 0)
		{
			j = count;
			do {
				pDestTemp += *pPixels++;		// 투명색만큼 건너 뛴다.
				indexCount = *pPixels++;	// Index반복 수

				// indexCount수만큼 index색들을 출력한다.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue와 관련된 색을 선택해서 출력한다.				
						*pDestTemp		= ColorSet[defaultGradationColor][colorGradation]
												& CDirectDraw::s_wMASK_RGB[rgb];
						pDestTemp ++;
					} while (--k);
				}

				// Normal 색깔 수
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

			// (투명수,Index색,색깔들)의 반복 수		
			transPair	= *pPixels++;
			
 			// 한 줄 출력
			if (transPair > 0)
			{
				j = transPair;
				do {
					pDestTemp += *pPixels++;	// 투명색만큼 건너 뛴다.
					indexCount = *pPixels++;	// Index반복 수

					// indexCount수만큼 index색들을 출력한다.
					if (indexCount > 0)
					{
						k = indexCount;
						do 
						{
							//colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue와 관련된 색을 선택해서 출력한다.				
							*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
							pDestTemp ++;
						} while (--k);
					}

					// Normal 색깔 수
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
	// pRect만큼의 점을 출력한다.
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
			do {		
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;			
			
				//---------------------------------------------
				// index색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - rectLeft;

						// index색 모두 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {						
								//colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우, index일부 출력
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// 왼쪽에서 skip되는 점들 수
						
						pPixels += dist;
						indexCount -= dist;
						
						// index색 일부 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								//colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}		

				
				// index색수만큼 index증가
				index += indexCount;

				// index색깔만큼 skip
				pPixels += indexCount;

				// normal 색깔 수
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index색까지해서는 xxxx범위를 안 넘어가고..
				// normal색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// 투명이 아닌 색들을 Surface에 출력한다.
					CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels+dist, colorCount2);
					pDestTemp += colorCount2;
					pPixels += colorCount;

					// 이제부터는 계속 출력한다.
					break;			
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
				do {		
					pDestTemp += *pPixels++;	// 투명색만큼 건너 뛴다.
					indexCount = *pPixels++;	// Index반복 수

					// indexCount수만큼 index색들을 출력한다.
					if (indexCount > 0)
					{
						k = indexCount;
						do {			
							//colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue와 관련된 색을 선택해서 출력한다.				
							*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
							pDestTemp ++;
						} while (--k);
					}

					// Normal 색깔 수
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
	// colorSet값 설정
	CSpriteSurface::s_Value1 = colorSet;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
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
			do {
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가
				index += transCount;
				
				// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
				// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

				// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

				//---------------------------------------------
				// index색까지 오른쪽 끝까지 도달했을 경우
				//---------------------------------------------			
				if (index+indexCount > rectRight)
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

						// 오른쪽으로 skip되는 점들을 빼준다.
						indexCount = rectRight - index;

						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								//colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
								pDestTemp ++;
							} while (--k);
						}						
						
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				//-------------------------
				// index색 모두 출력
				//-------------------------
				// indexCount수만큼 index색들을 출력한다.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						//colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue와 관련된 색을 선택해서 출력한다.				
						*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
						pDestTemp ++;
					} while (--k);
				}		
				// 출력한 indexCount만큼 index증가
				index += indexCount;
				

				// Normal 색깔 수
				colorCount = *pPixels++;

				//---------------------------------------------
				// normal색까지 오른쪽 끝까지 도달했을 경우
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// 투명이 아닌 색들을 Surface에 출력한다.
					CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, rectRight - index);
					break;			
				}			
				
				//--------------------
				// normal색 모두 출력
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
	// colorSet값 설정
	CSpriteSurface::s_Value1 = colorSet;

	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
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
			do {
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;			
			
				//---------------------------------------------
				// index색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - rectLeft;

						// index색 모두 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								//colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우, index일부 출력
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// 왼쪽에서 skip되는 점들 수
						
						pPixels += dist;
						indexCount -= dist;
						
						// index색 일부 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								//colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;
						index += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}		

				
				// index색수만큼 index증가
				index += indexCount;

				// index색깔만큼 skip
				pPixels += indexCount;

				// normal 색깔 수
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index색까지해서는 xxxx범위를 안 넘어가고..
				// normal색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// 투명이 아닌 색들을 Surface에 출력한다.
					CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels+dist, colorCount2);
					pDestTemp += colorCount2;
					pPixels += colorCount;
					index += colorCount;

					// 이제부터는 계속 출력한다.
					break;			
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
				do {
					transCount = *pPixels++;		// 투명색 수			
					indexCount = *pPixels++;		// 투명 아닌 색 수			
							
					// 투명색만큼 index증가
					index += transCount;
					
					// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
					// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

					// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

					//---------------------------------------------
					// index색까지 오른쪽 끝까지 도달했을 경우
					//---------------------------------------------			
					if (index+indexCount > rectRight)
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

							// 오른쪽으로 skip되는 점들을 빼준다.
							indexCount = rectRight - index;

							// indexCount수만큼 index색들을 출력한다.
							if (indexCount > 0)
							{
								k = indexCount;
								do {
									//colorSet		= (*pPixels >> 8) & 0xFF;	// set
									colorGradation	= (*pPixels & 0xFF);			// gradation
									pPixels++;

									// s_IndexValue와 관련된 색을 선택해서 출력한다.				
									*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
									pDestTemp ++;
								} while (--k);
							}						
							
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					//-------------------------
					// index색 모두 출력
					//-------------------------
					// indexCount수만큼 index색들을 출력한다.
					if (indexCount > 0)
					{
						k = indexCount;
						do {
							//colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue와 관련된 색을 선택해서 출력한다.				
							*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
							pDestTemp ++;
						} while (--k);
					}
					// 출력한 indexCount만큼 index증가
					index += indexCount;
					

					// Normal 색깔 수
					colorCount = *pPixels++;

					//---------------------------------------------
					// normal색까지 오른쪽 끝까지 도달했을 경우
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// 투명이 아닌 색들을 Surface에 출력한다.
						CSpriteSurface::memcpyEffectGradation(pDestTemp, pPixels, rectRight - index);
						break;			
					}			
					
					//--------------------
					// normal색 모두 출력
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
	// colorSet값 설정
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

		// (투명수,색깔수,색깔들)의 반복 수		
		count	= *pPixels++;		

		// 한 줄 출력
		if (count > 0)
		{
			j = count;
			do {
				pDestTemp += *pPixels++;		// 투명색만큼 건너 뛴다.
				indexCount = *pPixels++;	// Index반복 수

				// indexCount수만큼 index색들을 출력한다.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						//colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue와 관련된 색을 선택해서 출력한다.				
						*pDestTemp		= ColorSet[colorSet][colorGradation];
												//& CDirectDraw::s_wMASK_RGB[colorSet];
						pDestTemp ++;
					} while (--k);
				}

				// Normal 색깔 수
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
//    ( (투명색수, Index색수, 
//                 (indexSet번호, gradation번호), 
//                 (indexSet번호, gradation번호), 
//-----------------------------------------------------------------------------
// Clipping하지 않는다.
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

			// (투명수,Index색,색깔들)의 반복 수		
			transPair	= *pPixels++;
			
 			// 한 줄 출력		
			if (transPair > 0)
			{
				j = transPair;
				do
				{
					pDestTemp += *pPixels++;	// 투명색만큼 건너 뛴다.
					indexCount = *pPixels++;	// Index반복 수

					if (indexCount > 0)
					{
						// indexCount수만큼 index색들을 출력한다.
						memcpyEffect( pDestTemp, pPixels, indexCount );

						pDestTemp += indexCount;
						pPixels += indexCount;
					}

					// Normal 색깔 수
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
//    ( (투명색수, Index색수, 
//                 (indexSet번호, gradation번호), 
//                 (indexSet번호, gradation번호), 
//-----------------------------------------------------------------------------
// 왼쪽 clipping.  
// rectLeft개의 점을 건너띈 다음부터 pDest에 출력한다.
//----------------------------------------------------------------------
void
CIndexSprite::BltEffectClipLeft(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
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
		if (count > 0)
		{
			j = count;
			do
			{
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;			
			
				//---------------------------------------------
				// index색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - rectLeft;

						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							memcpyEffect( pDestTemp, pPixels, indexCount );

							pDestTemp += indexCount;
							pPixels += indexCount;
						}


						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						if (colorCount > 0)
						{
							CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
							
							pDestTemp	+= colorCount;
							pPixels		+= colorCount;					
						}

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우, index일부 출력
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// 왼쪽에서 skip되는 점들 수
						
						pPixels += dist;
						indexCount -= dist;
						
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							memcpyEffect( pDestTemp, pPixels, indexCount );

							pDestTemp += indexCount;
							pPixels += indexCount;
						}


						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						if (colorCount > 0)
						{
							CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
							
							pDestTemp	+= colorCount;
							pPixels		+= colorCount;
						}

						// 이제부터는 계속 출력한다.
						break;
					}
				}		

				
				// index색수만큼 index증가
				index += indexCount;

				// index색깔만큼 skip
				pPixels += indexCount;

				// normal 색깔 수
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index색까지해서는 xxxx범위를 안 넘어가고..
				// normal색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// 투명이 아닌 색들을 Surface에 출력한다.
					if (colorCount2 > 0)
					{
						CSpriteSurface::memcpyEffect(pDestTemp, pPixels+dist, colorCount2);
						pDestTemp += colorCount2;
						pPixels += colorCount;
					}

					// 이제부터는 계속 출력한다.
					break;			
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
				do {
					pDestTemp += *pPixels++;	// 투명색만큼 건너 뛴다.
					indexCount = *pPixels++;	// Index반복 수

					// indexCount수만큼 index색들을 출력한다.
					if (indexCount > 0)
					{
						memcpyEffect( pDestTemp, pPixels, indexCount );

						pDestTemp += indexCount;
						pPixels += indexCount;
					}

					// Normal 색깔 수
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
// 오른쪽 clipping.  
// rectRight개 까지의 점만 pDest에 출력한다.
//----------------------------------------------------------------------
void
CIndexSprite::BltEffectClipRight(WORD* pDest, WORD pitch, RECT* pRect)
{	
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
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
			do {
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가
				index += transCount;
				
				// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
				// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

				// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

				//---------------------------------------------
				// index색까지 오른쪽 끝까지 도달했을 경우
				//---------------------------------------------			
				if (index+indexCount > rectRight)
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

						// 오른쪽으로 skip되는 점들을 빼준다.
						indexCount = rectRight - index;

						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							memcpyEffect( pDestTemp, pPixels, indexCount );

							pDestTemp += indexCount;
							pPixels += indexCount;
						}

						
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				//-------------------------
				// index색 모두 출력
				//-------------------------
				// indexCount수만큼 index색들을 출력한다.
				if (indexCount > 0)
				{
					memcpyEffect( pDestTemp, pPixels, indexCount );

					pDestTemp += indexCount;
					pPixels += indexCount;
				}

				// 출력한 indexCount만큼 index증가
				index += indexCount;
				

				// Normal 색깔 수
				colorCount = *pPixels++;

				//---------------------------------------------
				// normal색까지 오른쪽 끝까지 도달했을 경우
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// 투명이 아닌 색들을 Surface에 출력한다.
					int tempCount = rectRight - index;
					if (tempCount > 0)
					{
						CSpriteSurface::memcpyEffect(pDestTemp, pPixels, tempCount);
					}
					break;			
				}			
				
				//--------------------
				// normal색 모두 출력
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
//    ( (투명색수, Index색수, 
//                 (indexSet번호, gradation번호), 
//                 (indexSet번호, gradation번호), 
//-----------------------------------------------------------------------------
// 왼쪽 clipping.  
// rectLeft개의 점을 건너띈 다음부터 pDest에 출력한다.
// rectRight까지..
//----------------------------------------------------------------------
void
CIndexSprite::BltEffectClipWidth(WORD* pDest, WORD pitch, RECT* pRect)
{
	WORD	*pPixels,
			*pDestTemp;

	//--------------------------------------------
	// pRect만큼의 점을 출력한다.
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
			do {		
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;			
			
				//---------------------------------------------
				// index색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - rectLeft;

						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							memcpyEffect( pDestTemp, pPixels, indexCount );

							pDestTemp += indexCount;
							pPixels += indexCount;
						}


						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						if (colorCount > 0)
						{
							CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
							
							pDestTemp	+= colorCount;
							pPixels		+= colorCount;					
						}

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우, index일부 출력
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// 왼쪽에서 skip되는 점들 수
						
						pPixels += dist;
						indexCount -= dist;
						
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							memcpyEffect( pDestTemp, pPixels, indexCount );

							pDestTemp += indexCount;
							pPixels += indexCount;
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						if (colorCount > 0)
						{
							CSpriteSurface::memcpyEffect(pDestTemp, pPixels, colorCount);
							
							pDestTemp	+= colorCount;
							pPixels		+= colorCount;
							index += colorCount;
						}

						// 이제부터는 계속 출력한다.
						break;
					}
				}		

				
				// index색수만큼 index증가
				index += indexCount;

				// index색깔만큼 skip
				pPixels += indexCount;

				// normal 색깔 수
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index색까지해서는 xxxx범위를 안 넘어가고..
				// normal색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// 투명이 아닌 색들을 Surface에 출력한다.
					if (colorCount2 > 0)
					{
						CSpriteSurface::memcpyEffect(pDestTemp, pPixels+dist, colorCount2);
						pDestTemp += colorCount2;
					}
					pPixels += colorCount;
					index += colorCount;					

					// 이제부터는 계속 출력한다.
					break;			
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
				do {
					transCount = *pPixels++;		// 투명색 수			
					indexCount = *pPixels++;		// 투명 아닌 색 수			
							
					// 투명색만큼 index증가
					index += transCount;
					
					// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
					// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

					// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

					//---------------------------------------------
					// index색까지 오른쪽 끝까지 도달했을 경우
					//---------------------------------------------			
					if (index+indexCount > rectRight)
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

							// 오른쪽으로 skip되는 점들을 빼준다.
							indexCount = rectRight - index;

							// indexCount수만큼 index색들을 출력한다.
							if (indexCount > 0)
							{
								memcpyEffect( pDestTemp, pPixels, indexCount );

								pDestTemp += indexCount;
								pPixels += indexCount;
							}

							
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					//-------------------------
					// index색 모두 출력
					//-------------------------
					// indexCount수만큼 index색들을 출력한다.
					if (indexCount > 0)
					{
						memcpyEffect( pDestTemp, pPixels, indexCount );

						pDestTemp += indexCount;
						pPixels += indexCount;
					}

					// 출력한 indexCount만큼 index증가
					index += indexCount;
					

					// Normal 색깔 수
					colorCount = *pPixels++;

					//---------------------------------------------
					// normal색까지 오른쪽 끝까지 도달했을 경우
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// 투명이 아닌 색들을 Surface에 출력한다.
						int tempCount = rectRight - index;
						if (tempCount > 0)
						{
							CSpriteSurface::memcpyEffect(pDestTemp, pPixels, tempCount);
						}
						break;			
					}			
					
					//--------------------
					// normal색 모두 출력
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
// pRect->top, rectBottom만큼만 출력한다.
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

		// (투명수,색깔수,색깔들)의 반복 수		
		count	= *pPixels++;		

		// 한 줄 출력
		if (count > 0)
		{
			j = count;
			do {		
				pDestTemp += *pPixels++;		// 투명색만큼 건너 뛴다.
				indexCount = *pPixels++;	// Index반복 수

				// indexCount수만큼 index색들을 출력한다.
				if (indexCount > 0)
				{
					memcpyEffect( pDestTemp, pPixels, indexCount );

					pDestTemp += indexCount;
					pPixels += indexCount;
				}

				// Normal 색깔 수
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

	// 첫 점
	colorSet		= (*pSource >> 8) & 0xFF;						
	colorGradation	= *pSource & 0xFF;			// gradation
	*pDest			= ColorSet[s_IndexValue[colorSet]][colorGradation];

	int pixels_1 = pixels-1;
	
	pDest += pixels_1;
	pSource += pixels_1;
	
	// 끝 점
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
	
	// [1] drawPixels만큼 출력하고..
	// [2] skipPixels 만큼 건너띄고
	// [3] drawPixels2만큼 출력
	
	//------------------------------------------------------------	
	// drawPixels만큼 출력
	//------------------------------------------------------------		
	if (drawPixels > 0)
	{
		do
		{
			colorSet		= (*pSource >> 8) & 0xFF;						
			colorGradation	= *pSource & 0xFF;			// gradation
			pSource++;

			// s_IndexValue와 관련된 색을 선택해서 출력한다.				
			*pDest		= ColorSet[s_IndexValue[colorSet]][colorGradation];
			pDest ++;
		} while (--drawPixels);
	}

	//------------------------------------------------------------	
	// skipPixels만큼 건너띔
	//------------------------------------------------------------	
	pDest += skipPixels;
	pSource += skipPixels;

	//------------------------------------------------------------	
	// drawPixels2만큼 출력
	//------------------------------------------------------------	
	if (drawPixels2 > 0)
	{
		do
		{
			colorSet		= (*pSource >> 8) & 0xFF;						
			colorGradation	= *pSource & 0xFF;			// gradation
			pSource++;

			// s_IndexValue와 관련된 색을 선택해서 출력한다.				
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
// 그물망
//----------------------------------------------------------------------
void		
CIndexSprite::memcpyEffectNet(WORD* pDest, WORD* pSource, WORD pixels)
{
	// 찍는점(1) + 건너띄는점(s_Value1)
	register int i = pixels;
	int skipPixels = 1 + CSpriteSurface::s_Value1;

	int colorSet, colorGradation;	
	
	// 한점 찍고 , skipPixels만큼 건너띄기
	do
	{
		colorSet		= (*pSource >> 8) & 0xFF;						
		colorGradation	= *pSource & 0xFF;			// gradation
		
		// s_IndexValue와 관련된 색을 선택해서 출력한다.				
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

			// (투명수,Index색,색깔들)의 반복 수		
			transPair	= *pPixels++;
			
 			// 한 줄 출력
			if (transPair > 0)
			{
				j = transPair;
				do {
					pDestTemp += *pPixels++;	// 투명색만큼 건너 뛴다.
					indexCount = *pPixels++;	// Index반복 수

					// indexCount수만큼 index색들을 출력한다.
					if (indexCount > 0)
					{
						k = indexCount;
						do {
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue와 관련된 색을 선택해서 출력한다.				
							*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
							pDestTemp ++;
						} while (--k);
						
					}

					// Normal 색깔 수
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
	// pRect만큼의 점을 출력한다.
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
			do {		
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;			
			
				//---------------------------------------------
				// index색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - rectLeft;

						// index색 모두 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {						
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyBrightness(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우, index일부 출력
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// 왼쪽에서 skip되는 점들 수
						
						pPixels += dist;
						indexCount -= dist;
						
						// index색 일부 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyBrightness(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}		

				
				// index색수만큼 index증가
				index += indexCount;

				// index색깔만큼 skip
				pPixels += indexCount;

				// normal 색깔 수
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index색까지해서는 xxxx범위를 안 넘어가고..
				// normal색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// 투명이 아닌 색들을 Surface에 출력한다.
					CSpriteSurface::memcpyBrightness(pDestTemp, pPixels+dist, colorCount2);
					pDestTemp += colorCount2;
					pPixels += colorCount;

					// 이제부터는 계속 출력한다.
					break;			
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
				do {		
					pDestTemp += *pPixels++;	// 투명색만큼 건너 뛴다.
					indexCount = *pPixels++;	// Index반복 수

					// indexCount수만큼 index색들을 출력한다.
					if (indexCount > 0)
					{
						k = indexCount;
						do {			
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue와 관련된 색을 선택해서 출력한다.				
							*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
							pDestTemp ++;
						} while (--k);
					}

					// Normal 색깔 수
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
	// pRect만큼의 점을 출력한다.
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
			do {
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가
				index += transCount;
				
				// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
				// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

				// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

				//---------------------------------------------
				// index색까지 오른쪽 끝까지 도달했을 경우
				//---------------------------------------------			
				if (index+indexCount > rectRight)
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

						// 오른쪽으로 skip되는 점들을 빼준다.
						indexCount = rectRight - index;

						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}						
						
						break;
					}
				}

				// 투명색만큼 건너띄고
				pDestTemp += transCount;

				//-------------------------
				// index색 모두 출력
				//-------------------------
				// indexCount수만큼 index색들을 출력한다.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue와 관련된 색을 선택해서 출력한다.				
						*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
						pDestTemp ++;
					} while (--k);
				}		
				// 출력한 indexCount만큼 index증가
				index += indexCount;
				

				// Normal 색깔 수
				colorCount = *pPixels++;

				//---------------------------------------------
				// normal색까지 오른쪽 끝까지 도달했을 경우
				//---------------------------------------------			
				if (index+colorCount > rectRight)
				{
					// 투명이 아닌 색들을 Surface에 출력한다.
					CSpriteSurface::memcpyBrightness(pDestTemp, pPixels, rectRight - index);
					break;			
				}			
				
				//--------------------
				// normal색 모두 출력
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
	// pRect만큼의 점을 출력한다.
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
			do {
				transCount = *pPixels++;		// 투명색 수			
				indexCount = *pPixels++;		// 투명 아닌 색 수			
						
				// 투명색만큼 index증가			
				index += transCount;			
			
				//---------------------------------------------
				// index색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+indexCount > rectLeft)
				{
					//---------------------------------------------
					// 투명색만으로 xxxx범위를 넘어갔을 경우
					//---------------------------------------------
					if (index > rectLeft)
					{	
						// 투명색부분 건너띔
						pDestTemp += index - rectLeft;

						// index색 모두 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyBrightness(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;					

						// 이제부터는 계속 출력한다.
						break;
					}
					//---------------------------------------------
					// 투명색+투명아닌색의 일부까지 출력하면 
					// xxxx범위를 넘어가게 되는 경우, index일부 출력
					//---------------------------------------------
					else
					{					
						dist = rectLeft - index;		// 왼쪽에서 skip되는 점들 수
						
						pPixels += dist;
						indexCount -= dist;
						
						// index색 일부 출력
						// indexCount수만큼 index색들을 출력한다.
						if (indexCount > 0)
						{
							k = indexCount;
							do {
								colorSet		= (*pPixels >> 8) & 0xFF;	// set
								colorGradation	= (*pPixels & 0xFF);			// gradation
								pPixels++;

								// s_IndexValue와 관련된 색을 선택해서 출력한다.				
								*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
								pDestTemp ++;
							} while (--k);
						}

						// Normal 색 모두 출력
						colorCount = *pPixels++;
						
						CSpriteSurface::memcpyBrightness(pDestTemp, pPixels, colorCount);
						
						pDestTemp	+= colorCount;
						pPixels		+= colorCount;
						index += colorCount;

						// 이제부터는 계속 출력한다.
						break;
					}
				}		

				
				// index색수만큼 index증가
				index += indexCount;

				// index색깔만큼 skip
				pPixels += indexCount;

				// normal 색깔 수
				colorCount = *pPixels++;			
				
				//---------------------------------------------
				// index색까지해서는 xxxx범위를 안 넘어가고..
				// normal색까지하면 xxxx범위를 넘어가게 되는 경우
				//---------------------------------------------
				if (index+colorCount > rectLeft)
				{
					dist = rectLeft - index;				
					colorCount2 = colorCount - dist;

					// 투명이 아닌 색들을 Surface에 출력한다.
					CSpriteSurface::memcpyBrightness(pDestTemp, pPixels+dist, colorCount2);
					pDestTemp += colorCount2;
					pPixels += colorCount;
					index += colorCount;

					// 이제부터는 계속 출력한다.
					break;			
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
				do {
					transCount = *pPixels++;		// 투명색 수			
					indexCount = *pPixels++;		// 투명 아닌 색 수			
							
					// 투명색만큼 index증가
					index += transCount;
					
					// 출력하고 있다가 오른쪽부분부터 출력하지 말아야 할 경우가 있다.
					// 현재 출력하는 줄은 모두 출력한 것이므로 break해야 한다.

					// 투명색까지 출력하는것만으로 더 이상 출력할 필요가 없을 경우

					//---------------------------------------------
					// index색까지 오른쪽 끝까지 도달했을 경우
					//---------------------------------------------			
					if (index+indexCount > rectRight)
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

							// 오른쪽으로 skip되는 점들을 빼준다.
							indexCount = rectRight - index;

							// indexCount수만큼 index색들을 출력한다.
							if (indexCount > 0)
							{
								k = indexCount;
								do {
									colorSet		= (*pPixels >> 8) & 0xFF;	// set
									colorGradation	= (*pPixels & 0xFF);			// gradation
									pPixels++;

									// s_IndexValue와 관련된 색을 선택해서 출력한다.				
									*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
									pDestTemp ++;
								} while (--k);
							}						
							
							break;
						}
					}

					// 투명색만큼 건너띄고
					pDestTemp += transCount;

					//-------------------------
					// index색 모두 출력
					//-------------------------
					// indexCount수만큼 index색들을 출력한다.
					if (indexCount > 0)
					{
						k = indexCount;
						do {
							colorSet		= (*pPixels >> 8) & 0xFF;	// set
							colorGradation	= (*pPixels & 0xFF);			// gradation
							pPixels++;

							// s_IndexValue와 관련된 색을 선택해서 출력한다.				
							*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
							pDestTemp ++;
						} while (--k);
					}
					// 출력한 indexCount만큼 index증가
					index += indexCount;
					

					// Normal 색깔 수
					colorCount = *pPixels++;

					//---------------------------------------------
					// normal색까지 오른쪽 끝까지 도달했을 경우
					//---------------------------------------------			
					if (index+colorCount > rectRight)
					{
						// 투명이 아닌 색들을 Surface에 출력한다.
						CSpriteSurface::memcpyBrightness(pDestTemp, pPixels, rectRight - index);
						break;			
					}			
					
					//--------------------
					// normal색 모두 출력
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

		// (투명수,색깔수,색깔들)의 반복 수		
		count	= *pPixels++;		

		// 한 줄 출력
		if (count > 0)
		{
			j = count;
			do {
				pDestTemp += *pPixels++;		// 투명색만큼 건너 뛴다.
				indexCount = *pPixels++;	// Index반복 수

				// indexCount수만큼 index색들을 출력한다.
				if (indexCount > 0)
				{
					k = indexCount;
					do {
						colorSet		= (*pPixels >> 8) & 0xFF;	// set
						colorGradation	= (*pPixels & 0xFF);			// gradation
						pPixels++;

						// s_IndexValue와 관련된 색을 선택해서 출력한다.				
						*pDestTemp		= ColorSet[s_IndexValue[colorSet]][colorGradation];
						pDestTemp ++;
					} while (--k);
				}

				// Normal 색깔 수
				colorCount = *pPixels++;
				
				CSpriteSurface::memcpyBrightness(pDestTemp, pPixels, colorCount);
				
				pDestTemp	+= colorCount;
				pPixels		+= colorCount;
			} while (--j);
		}

		pDest = (WORD*)((BYTE*)pDest + pitch);
	}
}
