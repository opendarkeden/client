#ifndef _SPRITE_H
#define _SPRITE_H
#include"stdafx.h"
#include"smartcutting.h"

#include "..\Client\SpriteLib\CSprite.h"
#include "..\Client\SpriteLib\CSprite555.h"
#include "..\Client\SpriteLib\Cspritepacklist555.h"
#include "..\Client\SpriteLib\CSprite565.h"
#include "..\Client\SpriteLib\Cspritepacklist565.h"
#include "..\Client\SpriteLib\CIndexSprite.h"
#include "..\Client\SpriteLib\CShadowSprite.h"


struct FRAME
{
	short sprite_id;
	short rx;
	short ry;
};

inline WORD RGB24to16_555( DWORD c)
{
	return (((WORD)(c>>3))&0x001F) | (((WORD)(c>>6))&0x03E0) | (((WORD)(c>>9))&0x7C00);
/*
	WORD res = 0;
	// get blue
	res |= (WORD)((c&0x000000ff)>>3);
	// get green
	res |= (WORD)((c&0x0000ff00)>>(3+8 -5));
	// get red
	res |= (WORD)((c&0x00ff0000)>>(3+16 -10));

	return res;
*/
}

inline WORD RGB24to16_565( DWORD c)
{
	return (((WORD)(c>>3))&0x001F) | (((WORD)(c>>5))&0x07E0) | (((WORD)(c>>8))&0xF800);
/*
	WORD res = 0;
	// get blue
	res |= (WORD)((c&0x000000ff)>>3);
	// get green
	res |= (WORD)((c&0x0000ff00)>>(3+8 -5));
	// get red
	res |= (WORD)((c&0x00ff0000)>>(3+16 -10));

	return res;
*/
}


DWORD getPixel24( S_SURFACEINFO* pSI, int x, int y);

//-----------------------------------------------------------------------------
// MakeFrame
//
// filename의 image file을 smart cutting하여 spk에 add하고, frame을 얻으며 
// 그것을 보정한다. 결과는 frame에 넣는다.
// bAdd가 false면 frame data만 얻는다...
// xOffset, yOffset은.. bmp 찍기 offset...
// bSplit가 true면 반으로 잘라서, 두개를 저장..
// pPt에는 두개인경우 위치 보간을 위한 offset을 저장
//-----------------------------------------------------------------------------

S_RECT MakeFrame( CSpritePackList555& spkList, FRAME &frame, char * filename, int xOffset, int yOffset, BOOL bAdd = TRUE, BOOL bSplit = FALSE, S_RECT* pRect1 = NULL, S_RECT* pRect2 = NULL)
{

	S_SURFACEINFO	src_surface;
	S_RECT			rect; 
	rect.x = -1;
	rect.y = -1;
	rect.w = -1;
	rect.h = -1;

/*// MBCD At 00-07-04 오후 5:06:45		load from file.
	// confirm tga file that we want...
	ret = Get_TGA_ImageInfo(filename, &picinfo);
	if (!ret)
		return false;

	// load TGA & do smart cutting
	ret = LoadTGA2(filename, &picinfo);
	if (!ret)
		return false;
*/

	CDib dib;
	if( dib.LoadBitmapFile( filename, TRUE) == FALSE) return rect;


	BYTE*	pDibBits = dib.GetDibBitsPtr();
	RGBQUAD* pPalette = dib.GetDibRGBTablePtr();

	src_surface.p_surface = pDibBits;
	src_surface.pitch	= dib.GetLineOffsetByte();
	src_surface.width = dib.m_dwWidth;
	src_surface.height = dib.m_dwHeight;
	rect.x = 0;
	rect.y = 0;
	rect.w = dib.m_dwWidth;
	rect.h = dib.m_dwHeight;

	DWORD colorkey;
	if(dib.GetBPP() == 24)
		colorkey = getPixel24( &src_surface, 0, 0);
	else
		colorkey = getPixel16( &src_surface, 0, 0);
//	int colorkey = getPixel16(&src_surface, 0, 0);// MBCD At 00-07-04 오후 5:06:59, Get colorkey


	if( bSplit)
	{
		///////////////////////////////////////////////////////
		//	분리된 object인 경우
		///////////////////////////////////////////////////////
//		int x1, y1, w1;	// 첫번째 sprite의 bmp offset...
		for( int i = 0; i< 2; i++)
		{
			CDib newDib;
			DWORD width;
			DWORD startX;

			if( i == 0)
			{
				width = dib.m_dwWidth/2;
				startX = 0;
			}
			else
			{
				width = dib.m_dwWidth - dib.m_dwWidth/2;
				startX = dib.m_dwWidth/2;
			}


			newDib.MakeClone( dib, width, dib.m_dwHeight);
			newDib.Blt( dib, startX, 0, width, dib.m_dwHeight, 0, 0);

//			newDib.Save( "1.bmp");

			src_surface.p_surface = newDib.GetDibBitsPtr();
			src_surface.pitch	= newDib.GetLineOffsetByte();
			src_surface.width = newDib.m_dwWidth;
			src_surface.height = newDib.m_dwHeight;
			rect.x = 0;
			rect.y = 0;
			rect.w = newDib.m_dwWidth;
			rect.h = newDib.m_dwHeight;


			DoSmartCutting(&src_surface, &rect, colorkey);
			// offset계산
			if( i == 0)
			{
/*
				// 첫번째의 offset저장.
				x1 = rect.x;	
				y1 = rect.y;
				w1 = width;
*/
				if( pRect1)
				{
					*pRect1 = rect;
				}
			}
			else
			{
				if( pRect2)
				{
					*pRect2 = rect;
				}
			}
			

			if( bAdd)
			{

				// create Sprite
				CSprite * p_sprite = new CSprite555;


				p_sprite->SetColorkey( RGB24to16_555(colorkey));	

				//////////////////////////////////////////////
				// convert 24 bit to 16 bit..
				/////////////////////////////////////////////
				WORD * pSpr = new WORD[ rect.w*rect.h];
				for( int y = 0; y < rect.h; y++)
				{
					for( int x = 0; x < rect.w; x++)
					{
						DWORD c = getPixel24( &src_surface, x + rect.x, y + rect.y);
						pSpr[ y*rect.w +x ] = RGB24to16_555( c);
					}
				}

				p_sprite->SetPixel( pSpr, rect.w*2, rect.w, rect.h);
				spkList.AddSprite(p_sprite);


				if( i == 0)
					frame.sprite_id = spkList.GetSize()-1;	// 처음 것의 id기록.

				delete[] pSpr;
			}

			frame.rx = rect.x - xOffset;	// 무의미..
			frame.ry = rect.y - yOffset;	// 무의미..
		}
	}
	else
	{
		///////////////////////////////////////////////////////
		//	한개로 된 object인 경우
		///////////////////////////////////////////////////////
		DoSmartCutting(&src_surface, &rect, colorkey);

		if( bAdd)
		{

			// create Sprite
			CSprite * p_sprite = new CSprite555;


			p_sprite->SetColorkey( RGB24to16_555(colorkey));	

			//////////////////////////////////////////////
			// convert 24 bit to 16 bit..
			/////////////////////////////////////////////
			WORD * pSpr = new WORD[ rect.w*rect.h];
			for( int y = 0; y < rect.h; y++)
			{
				for( int x = 0; x < rect.w; x++)
				{
					DWORD c = getPixel24( &src_surface, x + rect.x, y + rect.y);
					pSpr[ y*rect.w +x ] = RGB24to16_555( c);
				}
			}

			p_sprite->SetPixel( pSpr, rect.w*2, rect.w, rect.h);
			spkList.AddSprite(p_sprite);


			frame.sprite_id = spkList.GetSize()-1;

			delete[] pSpr;
		}

	//	frame.rx = rect.x -= g_revision_x;// MBCD At 00-07-04 오후 5:03:57
	//	frame.ry = rect.y -= g_revision_y;

		frame.rx = rect.x - xOffset;
		frame.ry = rect.y - yOffset;
	}
	//cout << "-> " << gp_file_list_buf[i] << "(" << rect.x << ", " << rect.y << ")" << endl;


//	free(picinfo.p_data); // free image buf

	return rect;
}





S_RECT GetFrame( FRAME &frame, char * filename, int xOffset, int yOffset)
{

	S_SURFACEINFO	src_surface;
	S_RECT			rect;
	rect.x = -1;
	rect.y = -1;
	rect.w = -1;
	rect.h = -1;


	CDib dib;
	if( dib.LoadBitmapFile( filename, TRUE) == FALSE) return rect;


	src_surface.p_surface = dib.GetDibBitsPtr();
	src_surface.pitch	= dib.GetLineOffsetByte();
	src_surface.width = dib.m_dwWidth;
	src_surface.height = dib.m_dwHeight;

	rect.x = 0;
	rect.y = 0;
	rect.w = dib.m_dwWidth;
	rect.h = dib.m_dwHeight;

	DWORD colorkey;
	colorkey = getPixel24( &src_surface, 0, 0);

	DoSmartCutting(&src_surface, &rect, colorkey);

	frame.rx = rect.x - xOffset;
	frame.ry = rect.y - yOffset;

	return rect;
}
BOOL MakeFrame( CIndexSprite& sprite, char * filename, CString *indexNameArray, int bl_dither = 0, int rotate = 0)
{

	S_SURFACEINFO	src_surface, src_surface2;
	S_RECT			rect;
	rect.x = -1;
	rect.y = -1;
	rect.w = -1;
	rect.h = -1;


	CDib dib, idib;
	if( dib.LoadBitmapFile( filename, TRUE) == FALSE) return FALSE;
	if(rotate)
	{
		for(int i = 0; i < rotate; i++)
			dib.Rotate(1);
	}

	src_surface.p_surface = dib.GetDibBitsPtr();
	src_surface.pitch	= dib.GetLineOffsetByte();
	src_surface.width = dib.m_dwWidth;
	src_surface.height = dib.m_dwHeight;

	rect.x = 0;
	rect.y = 0;
	rect.w = dib.m_dwWidth;
	rect.h = dib.m_dwHeight;


	DWORD colorkey16_1 = 0x001F, colorkey16_2 = 0x001F;
	DWORD colorkey16 = 0x001F;
	DWORD colorkey;
	if(dib.GetBPP() == 24)
	{
		colorkey = getPixel24( &src_surface, 0, 0);
		colorkey16 = RGB24to16_555(colorkey);
		CIndexSprite::SetColorkey( colorkey16 );
	}
	else
	{
		colorkey16 = colorkey = getPixel16( &src_surface, 0, 0);
		CIndexSprite::SetColorkey( colorkey16 );
	}
//	int colorkey = getPixel16(&src_surface, 0, 0);// MBCD At 00-07-04 오후 5:06:59, Get colorkey


	DoSmartCutting(&src_surface, &rect, colorkey);

	WORD *pSpr = NULL, *pIndexSpr = NULL, *pIndexSpr2 = NULL;
	int indexSprWidth = 0, indexSpr2Width = 0;

	///////////////////////////////////////////////////////////
	//	index sprite 준비.
	//

	int file_index = 0;

	// 여러개의 인덱스 파일을 겹쳐야 하므로 루프문 밖에서 생성
	WORD **ppIndexs = new WORD* [rect.h];
	WORD **ppColor = new WORD* [rect.h];

	register DWORD d;
	bool	b_fileload = idib.LoadBitmapFile( indexNameArray[0].GetBuffer(0), TRUE) == TRUE;
// 	if(b_fileload)
	{
		
		if(rotate && b_fileload)
		{
			for(int i = 0; i < rotate; i++)
				idib.Rotate(1);
		}

		for(int i=0; i<rect.h; i++)
		{
			ppIndexs[i] = new WORD[rect.w];
			ppColor[i] = new WORD[rect.w];

			for(int j=0; j<rect.w; j++)
			{			
				d = RGB24to16_555( getPixel24( &src_surface, j  + rect.x, i + rect.y) );
				if( d == colorkey16 )
				{
					if( !b_fileload )
					{
						ppIndexs[i][j] = INDEX_TRANS;
						ppColor[i][j] = d;
					}
					else ppIndexs[i][j] = 0;
				}
				else
				{
					if( !b_fileload )
					{
						ppColor[i][j] = d; //RGB24to16_555( getPixel24( &src_surface, j + rect.x, i + rect.y) );
						ppIndexs[i][j] = INDEX_NULL;
					}
					else ppIndexs[i][j] = 0;
				}		
			}
		}
		
		int x, y;

		while( idib.LoadBitmapFile( indexNameArray[ file_index++ ].GetBuffer(0), TRUE) )
		{
			if(rotate)
			{
				for(int i = 0; i < rotate; i++)
					idib.Rotate(1);
			}
			
			// 서페이스 셋팅
			src_surface2.p_surface = idib.GetDibBitsPtr();
			src_surface2.pitch	= idib.GetLineOffsetByte();
			src_surface2.width = idib.m_dwWidth;
			src_surface2.height = idib.m_dwHeight;

			indexSprWidth = rect.w*2;

			pIndexSpr = new WORD[ rect.w*rect.h];

			bool bl_dither1 = false;
			if(bl_dither)
			{
				S_RECT rect1;			
				DWORD colorkey_1;
				
				colorkey_1 = getPixel24( &src_surface2, 0, 0);
				colorkey16_1 = RGB24to16_555(colorkey_1);
				rect1.x = 0;
				rect1.y = 0;
				rect1.w = idib.m_dwWidth;
				rect1.h = idib.m_dwHeight;
				DoSmartCutting(&src_surface2, &rect1, colorkey_1);
				if(bl_dither == 1)
				{
					if(rect.w < rect1.w || rect.h < rect1.h) 
						bl_dither1 = true;
				}
				else
					bl_dither1 = true;			
			}
			
			// convert 24 bit to 16 bit..
			if( pSpr) delete[] pSpr;
			pSpr = new WORD[ rect.w*rect.h];

			int trance_count = 0;
			register DWORD c;
			for( y = 0; y < rect.h; y++)
			{
				for( x = 0; x < rect.w; x++)
				{
					c = getPixel24( &src_surface, x + rect.x, y + rect.y);
					pSpr[ y*rect.w +x ] = RGB24to16_555( c);
					if(bl_dither)
					{
						if(c == colorkey16)
							trance_count++;
					}
				}
			}

			int dither_color = 0;
			int dither_count = 0;
			int dither[65536];
			ZeroMemory(dither, 65536*sizeof(int));

			for( y = 0; y < rect.h; y++)
				for( x = 0; x < rect.w; x++)
				{
					c = RGB24to16_555(getPixel24( &src_surface2, x + rect.x, y + rect.y));
					pIndexSpr[ y*rect.w +x ] = c;
					if( bl_dither1 && c != colorkey16_1 )
					{
						dither[c]++;
						if(dither[c] > dither_count)
						{
							dither_color = c;
							dither_count = dither[c];
						}
					}
				}
				
			if(bl_dither1)
			{
				// 디더링 칼라가 투명색을 뺀 색의 25%이상일때
				if(dither_count > (rect.w*rect.h-trance_count)>>2)
					for(int y = 0; y < rect.h; y++)
						for(int x = 0; x < rect.w; x++)
							if(pIndexSpr[y*rect.w +x] == dither_color)
								pIndexSpr[y*rect.w +x] = colorkey16_1;						
			}
			// index color가 색으로 먹게
//			{
//				for( int y = 0; y < rect.h; y++)
//					for( int x = 0; x < rect.w; x++)
//						if(pIndexSpr[y*rect.w +x] != colorkey16_1)
//							pSpr[y*rect.w +x] = pIndexSpr[y*rect.w +x];				
//			}		

			//-----------------------------------------------------------------------------
			// CIndexSprite::SetPixel 함수를 가져와써 쓴다.
		
			WORD* pSource = pSpr;
			WORD  sourcePitch = rect.w*2;
			WORD* pIndex1 = pIndexSpr;
			WORD  indexPitch1 = indexSprWidth;
			WORD  width = rect.w;
			WORD  height = rect.h;
			WORD  ColorKey = sprite.GetColorkey();
			
			if (width==0 || height==0)
			{
				continue;
			}

			WORD	*pSourceTemp, *pIndexTemp1;
			WORD	transIndex1=0;		
			WORD	*pSourceTemp2;

			pSourceTemp = pSource;

			for (register i=0; i<height; i++)
			{
				pSourceTemp2 = pSourceTemp;

				for (register int j=0; j<width; j++)
				{				
					if (*pSourceTemp2 == ColorKey)
					{					
						if (pIndex1!=NULL)
						{
							transIndex1 = *(WORD*)((BYTE*)pIndex1 + i*indexPitch1 + (j<<1));
						}				

						i = height;
						break;
					}
					pSourceTemp2++;
				}
				pSourceTemp = (WORD*)((BYTE*)pSourceTemp + sourcePitch);
			}

			WORD **ppIndex = new WORD* [height];

			for (i=0; i<height; i++)
			{			
				ppIndex[i] = new WORD[rect.w];
			
				pSourceTemp = pSource;

				if (pIndex1!=NULL)
				{
					pIndexTemp1 = pIndex1;
				}

				for (register int j=0; j<width; j++)
				{
					if (*pSourceTemp==ColorKey)
					{
						ppIndex[i][j] = INDEX_TRANS;
					}
					else
					{
						// 여러 파일 읽어들이며 겹쳐서 저장

						// 인텍스가 있고 칼라값이 아니면 Index ColorSet값 저장
						if (pIndex1!=NULL && *pIndexTemp1!=transIndex1)
						{
							ppIndex[i][j] = ((file_index-1)<<8) | sprite.GetIndexColor( *pSourceTemp );
						}				
						else
						{
							ppIndex[i][j] = INDEX_NULL;
							ppColor[i][j] = *pSourceTemp;
						}
					}

					pSourceTemp++;
					
					if (pIndex1!=NULL)
					{
						pIndexTemp1++;
					}			
				}

				pSource = (WORD*)((BYTE*)pSource + sourcePitch);

				if (pIndex1!=NULL)
				{
					pIndex1 = (WORD*)((BYTE*)pIndex1 + indexPitch1);
				}		
			}

			for( i=0; i<height; i++)
			{
				for( int j=0; j<width; j++ )
				{
					if( ppIndexs[i][j] != INDEX_TRANS && ppIndex[i][j] != INDEX_TRANS )
					{
						//Index 겹침.						
					}

					if( file_index > 1 && (ppIndex[i][j] >> 8 ) > 0 )
					{
						int a=3;
					}

					if(ppIndexs[i][j]==0  || ppIndexs[i][j] == INDEX_TRANS )						
					{
						ppIndexs[i][j] = ppIndex[i][j];
					}
					else if( ppIndex[i][j] != INDEX_TRANS && ppIndex[i][j] != INDEX_NULL )
					{
						ppIndexs[i][j] = ppIndex[i][j];
					}
				}
			}

			if( pIndexSpr) delete[] pIndexSpr;
			for( i=0; i<height; i++)
			{
				if( ppIndex[i] )
					delete [] ppIndex[i];			
			}
		}
	}	
//	else
//	{
//		for(int i=0; i<rect.h; i++)
//		{
//			ppIndexs[i] = new WORD[rect.w];
//			ppColor[i] = new WORD[rect.w];
//			for(int j = 0; j < rect.w; j++)
//			{
//				ppIndexs[i][j] = INDEX_TRANS;
//			}
//		}
//	}

	sprite.GenerateFromIndex(ppColor, rect.w, rect.h, ppIndexs);	

	if( pSpr) delete[] pSpr;

	int i = 0;
	for( i=0; i<rect.h; i++)
	{
		if( ppIndexs[i] )
			delete [] ppIndexs[i];
		if( ppColor[i] )
			delete [] ppColor[i];
	}
	if( ppIndexs ) delete[] ppIndexs;
	if( ppColor ) delete[] ppColor;
	
	return TRUE;
}

//-----------------------------------------------------------------------------
// MakeFrame
//	shadow sprite pack을 만들기용.
//-----------------------------------------------------------------------------
BOOL MakeFrame( CShadowSprite& sprite, char * filename)
{

	S_SURFACEINFO	src_surface;
	S_RECT			rect;
	rect.x = -1;
	rect.y = -1;
	rect.w = -1;
	rect.h = -1;


	CDib dib;
	if( dib.LoadBitmapFile( filename, TRUE) == FALSE) return FALSE;


	src_surface.p_surface = dib.GetDibBitsPtr();
	src_surface.pitch	= dib.GetLineOffsetByte();
	src_surface.width = dib.m_dwWidth;
	src_surface.height = dib.m_dwHeight;

	rect.x = 0;
	rect.y = 0;
	rect.w = dib.m_dwWidth;
	rect.h = dib.m_dwHeight;

	DWORD colorkey;
	if(dib.GetBPP() == 24)
		colorkey = getPixel24( &src_surface, 0, 0);
	else
		colorkey = getPixel16( &src_surface, 0, 0);
//	int colorkey = getPixel16(&src_surface, 0, 0);// MBCD At 00-07-04 오후 5:06:59, Get colorkey

	CShadowSprite::SetColorkey( RGB24to16_555( colorkey) );

	DoSmartCutting(&src_surface, &rect, colorkey);


	//////////////////////////////////////////////
	// convert 24 bit to 16 bit..
	/////////////////////////////////////////////
	WORD * pSpr = new WORD[ rect.w*rect.h];
	for( int y = 0; y < rect.h; y++)
	{
		for( int x = 0; x < rect.w; x++)
		{
			DWORD c = getPixel24( &src_surface, x + rect.x, y + rect.y);
			pSpr[ y*rect.w +x ] = RGB24to16_555( c);
		}
	}


	sprite.SetPixel( pSpr, rect.w*2, rect.w, rect.h);

	


	delete[] pSpr;

	return TRUE;
}



//-----------------------------------------------------------------------------
// MakeFrame
//-----------------------------------------------------------------------------
int MakeFrame( CSpritePackList555& spkList, const CDib& dib, S_RECT& rect, bool bCutting = true)
{
	S_SURFACEINFO	src_surface;
	int sprite_id = -1;

	rect.x = -1;
	rect.y = -1;
	rect.w = -1;
	rect.h = -1;



	BYTE*	pDibBits = dib.GetDibBitsPtr();
	RGBQUAD* pPalette = dib.GetDibRGBTablePtr();

	src_surface.p_surface = pDibBits;
	src_surface.pitch	= dib.GetLineOffsetByte();
	src_surface.width = dib.m_dwWidth;
	src_surface.height = dib.m_dwHeight;
	rect.x = 0;
	rect.y = 0;
	rect.w = dib.m_dwWidth;
	rect.h = dib.m_dwHeight;

	DWORD colorkey;
	if(dib.GetBPP() == 24)
		colorkey = getPixel24( &src_surface, 0, 0);
	else
		colorkey = getPixel16( &src_surface, 0, 0);

	if(bCutting)
		DoSmartCutting(&src_surface, &rect, colorkey);

	// create Sprite
	CSprite * p_sprite = new CSprite555;


	p_sprite->SetColorkey( RGB24to16_555(colorkey));	

	//////////////////////////////////////////////
	// convert 24 bit to 16 bit..
	/////////////////////////////////////////////
	WORD * pSpr = new WORD[ rect.w*rect.h];
	for( int y = 0; y < rect.h; y++)
	{
		for( int x = 0; x < rect.w; x++)
		{
			DWORD c = getPixel24( &src_surface, x + rect.x, y + rect.y);
			pSpr[ y*rect.w +x ] = RGB24to16_555( c);
		}
	}

	if(bCutting)
		p_sprite->SetPixel( pSpr, rect.w*2, rect.w, rect.h);
	else
		p_sprite->SetPixelNoColorkey( pSpr, rect.w*2, rect.w, rect.h);
	spkList.AddSprite(p_sprite);


	sprite_id = spkList.GetSize()-1;
	delete[] pSpr;

	return sprite_id;
}






typedef WORD	color_t;
#define BPP_BYTE					2
#define BPP							16
#define IMAGE_BACKGROUND		0xFFFF
/*
//-----------------------------------------------------------------------------
// MakeImgFile - BMP로부터 MapEditor에서 사용되는 img화일을 만드는 함수.
// MBCD At 00-09-20 오후 4:52:17
//-----------------------------------------------------------------------------
int MakeImgFile( const CDib& dib, char* filepathname)
{
	S_RECT rect;
	S_SURFACEINFO	src_surface;
	int sprite_id = -1;

	rect.x = -1;
	rect.y = -1;
	rect.w = -1;
	rect.h = -1;



	BYTE*	pDibBits = dib.GetDibBitsPtr();
	RGBQUAD* pPalette = dib.GetDibRGBTablePtr();

	src_surface.p_surface = pDibBits;
	src_surface.pitch	= dib.GetLineOffsetByte();
	src_surface.width = dib.m_dwWidth;
	src_surface.height = dib.m_dwHeight;
	rect.x = 0;
	rect.y = 0;
	rect.w = dib.m_dwWidth;
	rect.h = dib.m_dwHeight;

	DWORD colorkey;
	colorkey = getPixel24( &src_surface, 0, 0);

	DoSmartCutting(&src_surface, &rect, colorkey);

	// create Sprite
	CSprite * p_sprite = new CSprite565;


	p_sprite->SetColorkey( RGB24to16_565(colorkey));	

	//////////////////////////////////////////////
	// convert 24 bit to 16 bit..
	/////////////////////////////////////////////
	WORD * pSpr = new WORD[ rect.w*rect.h];
	for( int y = 0; y < rect.h; y++)
	{
		for( int x = 0; x < rect.w; x++)
		{
			DWORD c = getPixel24( &src_surface, x + rect.x, y + rect.y);
			pSpr[ y*rect.w +x ] = RGB24to16_565( c);
		}
	}

	p_sprite->SetPixel( pSpr, rect.w*2, rect.w, rect.h);




	// -------------------------------------------------
	// Make img file
	// -------------------------------------------------

	const int zoom_width = 100, zoom_height = 100;

	int sprite_width = p_sprite->GetWidth();
	int sprite_height = p_sprite->GetHeight();


	// create image buf
	color_t p_image_buf[zoom_width*zoom_height];

	// make resized image

	S_SURFACEINFO	image_surface;
	S_RECT			image_rect;
	SetSurfaceInfo(image_surface, p_image_buf, zoom_width, zoom_height, zoom_width*BPP_BYTE);
	SetRect(image_rect, 0, 0, zoom_width, zoom_height);


	// fill background
//	filledRect(&image_surface, &image_rect, IMAGE_BACKGROUND);
	memset( p_image_buf, 0xff, sizeof( p_image_buf));

	// zoom out
	float f_scale;
	float s1 = (float)zoom_width/sprite_width;
	float s2 = (float)zoom_height/sprite_height;
	if (s1 > s2)
		f_scale = s2;
	else
		f_scale = s1;

	// 확대되는 경우라면...
	if (f_scale > 1)
		f_scale = 1;

	for (int h = 0; h < sprite_height; h++)
		for (int w = 0; w < sprite_width; w++)
		{
			int zx = w*f_scale;
			int zy = h*f_scale;

			if (p_sprite->IsColorPixel(w, h))
			{
//				putPixel(&image_surface, zx, zy, p_sprite->GetPixel(w, h));
				((color_t*)(image_surface.p_surface))[ zy*image_surface.width+zx] = p_sprite->GetPixel(w, h);
//				((color_t*)(image_surface.p_surface))[ zy*image_surface.width+zx] = 0x7c1f;
			}
		}

	// make image file
	C_IMG			img;
	S_PICINFO	picinfo;
	picinfo.p_data = (BYTE *)p_image_buf;
	picinfo.width	= zoom_width;
	picinfo.height = zoom_height;
	picinfo.bpp		= BPP;

	img.LoadIMG(picinfo);
	img.SaveIMG( filepathname);



	delete p_sprite;

	delete[] pSpr;

	return sprite_id;
}
*/



//-----------------------------------------------------------------------------
// MakeFrame
// pColor는 타일 자를시 color key를 위한 
//-----------------------------------------------------------------------------
int MakeFrame( CSpritePackList565& spkList, const CDib& dib, S_RECT& rect, DWORD* pColor = NULL, BOOL bTile = false)
{
	S_SURFACEINFO	src_surface;
	int sprite_id = -1;

	rect.x = -1;
	rect.y = -1;
	rect.w = -1;
	rect.h = -1;



	BYTE*	pDibBits = dib.GetDibBitsPtr();
	RGBQUAD* pPalette = dib.GetDibRGBTablePtr();

	src_surface.p_surface = pDibBits;
	src_surface.pitch	= dib.GetLineOffsetByte();
	src_surface.width = dib.m_dwWidth;
	src_surface.height = dib.m_dwHeight;
	rect.x = 0;
	rect.y = 0;
	rect.w = dib.m_dwWidth;
	rect.h = dib.m_dwHeight;

	DWORD colorkey;
	if( pColor != NULL)
		colorkey = *pColor;
	else
	{
		if(dib.GetBPP() == 24)
			colorkey = getPixel24( &src_surface, 0, 0);
		else
			colorkey = getPixel16( &src_surface, 0, 0);
	}

	if( bTile == false)
		DoSmartCutting(&src_surface, &rect, colorkey);

	// create Sprite
	CSprite * p_sprite = new CSprite565;


	p_sprite->SetColorkey( RGB24to16_565(colorkey));	

	//////////////////////////////////////////////
	// convert 24 bit to 16 bit..
	/////////////////////////////////////////////
	WORD * pSpr = new WORD[ rect.w*rect.h];
	for( int y = 0; y < rect.h; y++)
	{
		for( int x = 0; x < rect.w; x++)
		{
			DWORD c = getPixel24( &src_surface, x + rect.x, y + rect.y);
			pSpr[ y*rect.w +x ] = RGB24to16_565( c);
		}
	}

	if(bTile)
		p_sprite->SetPixelNoColorkey( pSpr, rect.w*2, rect.w, rect.h);
	else
		p_sprite->SetPixel( pSpr, rect.w*2, rect.w, rect.h);
	spkList.AddSprite(p_sprite);


	sprite_id = spkList.GetSize()-1;
	delete[] pSpr;

	return sprite_id;
}




//-----------------------------------------------------------------------------
// Update sprite( tile전용)
// pColor는 타일 자를시 color key를 위한 
//-----------------------------------------------------------------------------
int UpdateSprite( CSpritePackList565& spkList, const CDib& dib, DWORD* pColor, int iIndex)
{
	S_SURFACEINFO	src_surface;
	int sprite_id = -1;


	BYTE*	pDibBits = dib.GetDibBitsPtr();
	RGBQUAD* pPalette = dib.GetDibRGBTablePtr();

	src_surface.p_surface = pDibBits;
	src_surface.pitch	= dib.GetLineOffsetByte();
	src_surface.width = dib.m_dwWidth;
	src_surface.height = dib.m_dwHeight;

	S_RECT rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = dib.m_dwWidth;
	rect.h = dib.m_dwHeight;

	DWORD colorkey;
	if( pColor != NULL)
		colorkey = *pColor;
	else
		colorkey = getPixel24( &src_surface, 0, 0);


	// create Sprite
	CSprite * p_sprite = new CSprite565;


	p_sprite->SetColorkey( RGB24to16_565(colorkey));	

	//////////////////////////////////////////////
	// convert 24 bit to 16 bit..
	/////////////////////////////////////////////
	WORD * pSpr = new WORD[ rect.w*rect.h];
	for( int y = 0; y < rect.h; y++)
	{
		for( int x = 0; x < rect.w; x++)
		{
			DWORD c = getPixel24( &src_surface, x + rect.x, y + rect.y);
			pSpr[ y*rect.w +x ] = RGB24to16_565( c);
		}
	}

	p_sprite->SetPixel( pSpr, rect.w*2, rect.w, rect.h);
	spkList.AddSprite( p_sprite);
	spkList.ChangeSprite( iIndex, spkList.GetSize()-1);
	spkList.RemoveSprite( spkList.GetSize()-1);


	sprite_id = spkList.GetSize()-1;
	delete[] pSpr;

	return sprite_id;
}







S_RECT GetSmartCuttingRect( const CDib& dib)
{

	S_SURFACEINFO	src_surface;
	S_RECT rect;


	BYTE*	pDibBits = dib.GetDibBitsPtr();

	src_surface.p_surface = pDibBits;
	src_surface.pitch	= dib.GetLineOffsetByte();
	src_surface.width = dib.m_dwWidth;
	src_surface.height = dib.m_dwHeight;
	rect.x = 0;
	rect.y = 0;
	rect.w = dib.m_dwWidth;
	rect.h = dib.m_dwHeight;

//	int colorkey = getPixel16(&src_surface, 0, 0);// MBCD At 00-07-04 오후 5:06:59, Get colorkey
	DWORD colorkey;

	colorkey = getPixel24( &src_surface, 0, 0);

	DoSmartCutting( &src_surface, &rect, colorkey);
	return rect;
}








#endif