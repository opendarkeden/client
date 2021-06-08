#include "StdAfx.h"

#include "cdib.h"
#include "stdio.h"

#ifdef _G3D
#include "G3D.h"
#endif

#define SWAP(x, y) { x^=y; y^=x; x^=y; }

// bitmap의 성질에 따라 imageSize를 구한다.
DWORD GetDibImageSize( DWORD width, DWORD height, DWORD bpp)
{
	if( bpp >= 8)
	{
		// width의 data길이가 DWORD의 배수가 되어야 하므로..
		DWORD addedByte = (width*bpp/8)%4;
		if( addedByte != 0)
			addedByte = (4-addedByte);

		return (width*bpp/8 + addedByte)*height;
	}
	else
	{
		// not yet implemented......
		return ((width+4*8-1)/8)*height;
	}
}


CDib::CDib()
{
	m_pDib = NULL;
	m_pBmInfo = NULL;
	m_pBmInfoHeader = NULL;
	m_pRGBTable = NULL;
	m_pDibBits = NULL;
	m_numColors = 0;

	m_dwDibSize = 0;
	m_dwWidth = 0;
	m_dwHeight = 0;

	m_bFlip = FALSE;

	memset( m_strFileName, 0, sizeof( m_strFileName));

	memset( &m_bmFileHeader, 0, sizeof(m_bmFileHeader));
}


CDib::CDib( const char* fileName, BOOL bFlip)
{
	m_pDib = NULL;
	m_pBmInfo = NULL;
	m_pBmInfoHeader = NULL;
	m_pRGBTable = NULL;
	m_pDibBits = NULL;
	m_numColors = 0;

	m_dwDibSize = 0;
	m_dwWidth = 0;
	m_dwHeight = 0;

	memset( m_strFileName, 0, sizeof( m_strFileName));

	memset( &m_bmFileHeader, 0, sizeof(m_bmFileHeader));

	LoadBitmapFile( fileName, bFlip);
}

CDib::CDib( const CDib& dib)
{
	m_pDib = NULL;
	m_pBmInfo = NULL;
	m_pBmInfoHeader = NULL;
	m_pRGBTable = NULL;
	m_pDibBits = NULL;
	m_numColors = 0;

	m_dwDibSize = 0;
	m_dwWidth = 0;
	m_dwHeight = 0;

	memset( m_strFileName, 0, sizeof( m_strFileName));

	memset( &m_bmFileHeader, 0, sizeof(m_bmFileHeader));

	*this = dib;

}

void CDib::operator =( const CDib& dib)
{

	MakeClone( dib, dib.m_dwWidth, dib.m_dwHeight);
	Blt( dib, 0, 0, dib.m_dwWidth, dib.m_dwHeight, 0, 0);
	strcpy( m_strFileName, dib.m_strFileName);
}



BOOL CDib::operator ==( const CDib& dib)
{
	if( m_strFileName == "") return FALSE;

	return  (strcmp( m_strFileName, dib.GetFileName()) == 0);
}



CDib::~CDib()
{
//	if( m_pDib) delete[] m_pDib;	// 이렇게 하면 memory leak이 발생하는 것 같슴.
	Release();
}

BOOL CDib::LoadBitmapFile( const char* fileName, BOOL bFlip)
{
	if( fileName == NULL || fileName[0] == 0) return false;
	Release();

	FILE* fp = NULL;

#ifdef _G3D
	fp = G3Dfopen( fileName, "rb");
#else
	fp = fopen( fileName, "rb");
#endif

	if( fp == NULL) return FALSE;

	strcpy( m_strFileName, fileName);

//	BITMAPFILEHEADER bmFileHeader;

	fread( &m_bmFileHeader, sizeof( m_bmFileHeader), 1, fp);

	if( m_bmFileHeader.bfType != 0x4d42)
	{
/*
		m_pBmInfo = 0;
		m_pBmInfoHeader = 0;
		m_pRGBTable = 0;
		m_pDibBits = 0;
		m_numColors = 0;
*/
		return FALSE;
	}
	else
	{
		DWORD preOffset = ftell( fp);
		// get file length
		fseek( fp, 0, SEEK_END);
		DWORD fileLength = ftell( fp);
		m_dwDibSize = fileLength - sizeof(m_bmFileHeader);
		
		fseek( fp, preOffset, SEEK_SET);

		BYTE* pDib = new BYTE[ m_dwDibSize];

		fread( pDib, m_dwDibSize, 1, fp);
		fclose( fp);

		// 
		//
		m_pDib = pDib;
		m_pBmInfo = (LPBITMAPINFO)m_pDib;
		m_pBmInfoHeader = (LPBITMAPINFOHEADER)m_pDib;
		m_pRGBTable = (RGBQUAD*)(m_pDib+m_pBmInfoHeader->biSize);

		m_numColors = GetDibNumColors();

//		m_pBmInfoHeader->biSizeImage = GetDibImageSizeImage();

		if( m_pBmInfoHeader->biClrUsed == 0)
		{
//			m_pBmInfoHeader->biClrUsed = m_numColors;
		}

		m_pDibBits = m_pDib + m_bmFileHeader.bfOffBits - sizeof( BITMAPFILEHEADER);


		m_dwWidth = m_pBmInfoHeader->biWidth;
		m_dwHeight = m_pBmInfoHeader->biHeight;

		m_bFlip = bFlip;

		if( bFlip)
		{
			// bmp는 upside down
			DWORD swapNo = (m_dwHeight+1)/2;
			DWORD bpp = m_pBmInfoHeader->biBitCount;
			DWORD widthByteLength = m_dwWidth*bpp/8;
			// width의 data길이가 DWORD의 배수가 되므로..
			DWORD addedByte = (m_dwWidth*GetBytesPerPixel())%4;
			if( addedByte != 0)
				widthByteLength += (4-addedByte);
			


			if( bpp != 1) // 2 color bitmap은 아직 지원 안됩니다.
			{
				BYTE* temp = new BYTE[widthByteLength];
				for( DWORD i=0; i< swapNo; i++)	
				{
					// swap
					MoveMemory( temp, m_pDibBits+(m_dwHeight-i-1)*widthByteLength, widthByteLength);
					MoveMemory( m_pDibBits+(m_dwHeight-i-1)*widthByteLength, m_pDibBits+i*widthByteLength, widthByteLength);
					MoveMemory( m_pDibBits+i*widthByteLength, temp, widthByteLength);
				}
				delete[] temp;
			} 
		}

		return TRUE;
	}
}
 


//=======================================================================================
//		
//		사용되기 전에 dib가 MakeClone등으로 생성되어 있어야 된다
//		
//=======================================================================================
void CDib::Blt( const CDib& srcDib, DWORD srcX, DWORD srcY, DWORD width, DWORD height, DWORD destX, DWORD destY)
{
	if( width == 0) width = srcDib.m_dwWidth;
	if( height == 0) height = srcDib.m_dwHeight;

	if( srcX >= srcDib.m_dwWidth || ( srcX+width) > srcDib.m_dwWidth 
		|| srcY >= srcDib.m_dwHeight || ( srcY+height) > srcDib.m_dwHeight
		|| destX >= m_dwWidth || destY >= m_dwHeight)
		return;





	DWORD dwBytesPerPixel = GetBytesPerPixel();

	DWORD srcBytesPerLine = srcDib.GetLineOffsetByte();
	DWORD destBytesPerLine = GetLineOffsetByte();

	for( DWORD yCount = 0; yCount< height; yCount++)
	{
		BYTE* pSourcePoint = srcDib.m_pDibBits + srcX*dwBytesPerPixel + (srcDib.m_dwHeight - (yCount+srcY) - 1)*srcBytesPerLine;
		BYTE* pDestPoint = m_pDibBits + destX*dwBytesPerPixel + (m_dwHeight - (yCount+destY) - 1)*destBytesPerLine;

//		memset( dib.m_pDibBits + yCount*bytesPerLine, 0, bytesPerLine);

		memcpy( pDestPoint, pSourcePoint, width*dwBytesPerPixel);
//		memcpy( dib.m_pDibBits + yCount, pSourcePoint, bytesPerLine);
	}
}


//=======================================================================================
//	Name:	BltDC
//	Desc:	dc에 bit blit한다.
//	Params:	nDestWidth, nDestHeight, nSrcWidth, nSrcHeight가 0이면 dib의 size로 바꿔서 사용.
//=======================================================================================
void CDib::BltDC( HDC hDC, int XDest, int YDest, int nDestWidth, int nDestHeight, 
								int XSrc, int YSrc, int nSrcWidth, int nSrcHeight, DWORD dwRop)
{
	if( hDC == NULL) return;

	if( nDestWidth == 0) nDestWidth = m_dwWidth;
	if( nDestHeight == 0) nDestHeight = m_dwHeight;
	if( nSrcWidth == 0) nSrcWidth = m_dwWidth;
	if( nSrcHeight == 0) nSrcHeight = m_dwHeight;

	int iUsage = DIB_RGB_COLORS;
	if( m_pRGBTable != NULL) iUsage = DIB_PAL_COLORS;

	StretchDIBits( hDC, XDest, YDest, nDestWidth, nDestHeight, XSrc, YSrc, nSrcWidth, nSrcHeight, 
		m_pDibBits, m_pBmInfo, iUsage, dwRop);
}



//=======================================================================================
//	Name:	TransparentBltDC
//	Desc:	transparet blit
//	Params:	nDestWidth, nDestHeight, nSrcWidth, nSrcHeight가 0이면 dib의 size로 바꿔서 사용.
//=======================================================================================
void CDib::TransparentBltDC( HDC hDC, int XDest, int YDest, int nDestWidth, int nDestHeight, 
								int XSrc, int YSrc, int nSrcWidth, int nSrcHeight, COLORREF tpColor)
{

	if( hDC == NULL) return;

	if( nDestWidth == 0) nDestWidth = m_dwWidth;
	if( nDestHeight == 0) nDestHeight = m_dwHeight;
	if( nSrcWidth == 0) nSrcWidth = m_dwWidth;
	if( nSrcHeight == 0) nSrcHeight = m_dwHeight;

	int iUsage = DIB_RGB_COLORS;
	if( m_pRGBTable != NULL) iUsage = DIB_PAL_COLORS;


	HDC	hMemDC = NULL, hMaskDC = NULL;

	// create dcs
	hMaskDC = CreateCompatibleDC( hDC);
	hMemDC = CreateCompatibleDC( hDC);


	HBITMAP hBMP = NULL;	// hMemDC가 select할 bitmap
	HBITMAP hOldBMP = NULL, hOldBMP2;
	HBITMAP hMaskBMP = NULL;	// mask bitmap

	// create bitmap for memory dc
	hBMP = CreateCompatibleBitmap( hDC, nSrcWidth, nSrcHeight);
	// select bitmap
	hOldBMP = (HBITMAP)SelectObject( hMemDC, hBMP);
	// copy source to memory dc
	BltDC( hMemDC, 0, 0, nSrcWidth, nSrcHeight, XSrc, YSrc, nSrcWidth, nSrcHeight);

	// create mask bitmap
	hMaskBMP = CreateBitmap( nSrcWidth, nSrcHeight, 1, 1, NULL);
	// select mask bitmap
	hOldBMP2 = (HBITMAP)SelectObject( hMaskDC, hMaskBMP);

	// set background color
	SetBkColor( hMemDC, tpColor);

	// make mask dc from memory dc.
	BitBlt( hMaskDC, 0, 0, nSrcWidth, nSrcHeight, hMemDC, 0, 0, SRCCOPY);

	// copy mask dc to memory dc
	SetBkColor( hMemDC, RGB( 0, 0, 0));
	SetTextColor( hMemDC, RGB( 255, 255, 255));
	BitBlt( hMemDC, 0, 0, nDestWidth, nDestHeight, hMaskDC, 0, 0, SRCAND);


	// 
	SetBkColor( hDC, RGB( 255, 255, 255));
	SetTextColor( hDC, RGB( 0, 0, 0));
	BitBlt( hDC, XDest, YDest, nDestWidth, nDestHeight, hMaskDC, 0, 0, SRCAND);


	// Combine the foreground with the background
	BitBlt( hDC, XDest, YDest, nDestWidth, nDestHeight, hMemDC, 
		0, 0, SRCPAINT);

	if( hOldBMP) 
		::SelectObject( hMemDC, hOldBMP);
	if( hOldBMP2)
		::SelectObject( hMaskDC, hOldBMP2);

	DeleteObject( hBMP);
	DeleteObject( hMaskBMP);

	DeleteDC( hMemDC);
	DeleteDC( hMaskDC);

}


//=======================================================================================
//	Name:	TransparentBltDC
//	Desc:	transparet blit
//	Params:	nDestWidth, nDestHeight, nSrcWidth, nSrcHeight가 0이면 dib의 size로 바꿔서 사용.
//=======================================================================================
void CDib::TransparentBltDC( HDC hDC, int XDest, int YDest, int nDestWidth, int nDestHeight, 
								int XSrc, int YSrc, int nSrcWidth, int nSrcHeight)
{
	if( m_pDibBits == NULL) return;
	// 구석의 한점을 color key로 사용.
	COLORREF tpColor = 0;

	switch( GetBPP())
	{
	case 8:
		tpColor = RGB( m_pRGBTable[ m_pDibBits[0]].rgbRed, m_pRGBTable[ m_pDibBits[0]].rgbGreen, m_pRGBTable[ m_pDibBits[0]].rgbBlue);
		break;
	case 24:
//		tpColor = RGB( m_pDibBits[2], m_pDibBits[1], m_pDibBits[0]);
		tpColor = RGB( m_pDibBits[2], m_pDibBits[1], m_pDibBits[0]);
		break;
	}

	TransparentBltDC( hDC, XDest, YDest, nDestWidth, nDestHeight, 
		XSrc, YSrc, nSrcWidth, nSrcHeight, tpColor);
}





int CDib::GetColorPixelNum()
{
	if( m_pDibBits == NULL) return 0;
	// 구석의 한점을 color key로 사용.

	DWORD color_key = 0;

	switch( GetBPP())
	{
	case 8:
//		color_key = *((char *)m_pDibBits);
		memcpy(&color_key, m_pDibBits, 1);
		break;
	case 24:
//		color_key = (((char *)m_pDibBits)[0])<<16 | (((char *)m_pDibBits)[1])<<8 | ((char *)m_pDibBits)[2];
		memcpy(&color_key, m_pDibBits, 3);
		break;
	}

	int size = 0;

	for(DWORD i = 0; i < m_dwHeight; i++)
	{
		for(DWORD j = 0; j < m_dwWidth; j++)
		{
			if(color_key != GetPixel(j, i))size++;
		}
	}
	return size;
}










//==========================================================================
//	Name: CDib::MakeClone
//	blt과 비슷한 형식으로 새로운 bmp를 만든다.
//	2000/04/19
//	
//==========================================================================
BOOL CDib::MakeClone( const CDib& srcDib, DWORD width, DWORD height)
{
	if( srcDib.m_pDib == NULL || width == 0 || height == 0 ||
//		( ( x+width) > m_dwWidth) || ( ( y+height) > m_dwHeight) ||	// 범위를 벗어나지 않도록..
//		width > m_dwWidth || height > m_dwHeight ||
		srcDib.m_pBmInfoHeader->biCompression != BI_RGB) return FALSE;
	Release();

	m_dwWidth = width;
	m_dwHeight = height;
	m_numColors = srcDib.m_numColors;

	m_bmFileHeader = srcDib.m_bmFileHeader;

	DWORD dwBytesPerPixel = srcDib.GetBytesPerPixel();

	m_dwDibSize = ::GetDibImageSize( m_dwWidth, m_dwHeight, srcDib.GetBPP()) + srcDib.m_bmFileHeader.bfOffBits - sizeof( BITMAPFILEHEADER);

	m_bFlip = srcDib.m_bFlip;


	
	m_pDib = new BYTE[ m_dwDibSize];


	// copy BITMAPFILEHEADER & BITMAPINFO
	memcpy( m_pDib, srcDib.m_pDib, m_bmFileHeader.bfOffBits - sizeof( BITMAPFILEHEADER));	


	// 재설정 bitmap info..
	m_pBmInfo = (LPBITMAPINFO)m_pDib;
	m_pBmInfoHeader = (LPBITMAPINFOHEADER)m_pDib;
	m_pRGBTable = (RGBQUAD*)(m_pDib+m_pBmInfoHeader->biSize);

	m_pDibBits = m_pDib + m_bmFileHeader.bfOffBits - sizeof( BITMAPFILEHEADER);

		
	m_pBmInfoHeader->biWidth = m_dwWidth;
	m_pBmInfoHeader->biHeight = m_dwHeight;

	m_pBmInfoHeader->biSizeImage = GetDibSizeImage();	// maybe 0
	m_bmFileHeader.bfSize = sizeof( BITMAPFILEHEADER) + m_dwDibSize;



	// image data초기화..
	memset( m_pDibBits, 0, GetDibSizeImage());
	


	return TRUE;

}

BOOL CDib::Save( const char* filename)
{
	FILE* fp = fopen( filename, "wb");

	if( fp == NULL) return FALSE;

	fwrite( &m_bmFileHeader, sizeof( m_bmFileHeader), 1, fp);
	fwrite( m_pDib, m_dwDibSize, 1, fp);

	if( fclose(  fp) == 0) return FALSE;

	return TRUE;
		
}


////////////////////////////////////////////////////////////////////////////////////
//
//	bitmap의 길이, 높이, bpp으로 bitmap의 크기를 구하는 함수.
//
////////////////////////////////////////////////////////////////////////////////////
DWORD CDib::GetDibSizeImage() const
{
	if( m_pBmInfoHeader->biSizeImage != 0)	// 이 값이 0인 경우도 있고 아닌 경우도 있다.???
	{
		return ::GetDibImageSize( m_dwWidth, m_dwHeight, m_pBmInfoHeader->biBitCount);
	}
	else
		return m_pBmInfoHeader->biSizeImage;
}

UINT CDib::GetDibNumColors() const
{

	return 1<<m_pBmInfoHeader->biBitCount;


		/*
	if( (m_pBmInfoHeader->biClrUsed == 0) &&
			(m_pBmInfoHeader->biBitCount < 9 ))
		return (1<<m_pBmInfoHeader->biBitCount);
	else
		return m_pBmInfoHeader->biClrUsed;
		*/
}


DWORD CDib::GetBytesPerPixel()  const
{
	return m_pBmInfoHeader->biBitCount/8;
/*
	DWORD dwBytesPerPixel;

	if( GetDibNumColors() == 0x100) dwBytesPerPixel = 1;
	else if( GetDibNumColors() == 0x10000) dwBytesPerPixel = 2;
	else if( GetDibNumColors() == 0x1000000) dwBytesPerPixel = 3;
	else if( GetDibNumColors() == 0x100000000) dwBytesPerPixel = 4;

	return dwBytesPerPixel;
	*/
}

LPBITMAPINFOHEADER CDib::GetDibInfoHeaderPtr() const
{
	return m_pBmInfoHeader;
}

LPBITMAPINFO CDib::GetDibInfoPtr() const
{
	return m_pBmInfo;
}

LPRGBQUAD CDib::GetDibRGBTablePtr() const
{
	return m_pRGBTable;
}

BYTE* CDib::GetDibBitsPtr() const
{
	return m_pDibBits;
}

//////////////////////////////////////////////
//
// 1 line이 증가할때의 byte offset
// 즉, 다음 line으로 가기위한 byte offset...
//////////////////////////////////////////////
DWORD CDib::GetLineOffsetByte() const
{
	DWORD bpp = GetBPP();
	if( bpp >= 8)
	{
		// width의 data길이가 DWORD의 배수가 되어야 하므로..
		DWORD addedByte = (m_dwWidth*bpp/8)%4;
		if( addedByte != 0)
			addedByte = (4-addedByte);

		return (m_dwWidth*bpp/8 + addedByte);
	}
	else	// 1 bit.
	{
		// not yet implemented......
		return ((m_dwWidth+4*8-1)/8);
	}

}

void CDib::PutPixel( DWORD x, DWORD y, RGBQUAD c)
{
	if( GetBPP() != 24 || m_pDibBits == NULL)
		return;

	if( !m_bFlip)
		y = m_dwHeight - y - 1;
		

	DWORD bytePerLine = GetLineOffsetByte();

	// BLUE, GREEN, RED이런 순서로 저장
	m_pDibBits[ 3*x + y*bytePerLine+0] = c.rgbBlue;
	m_pDibBits[ 3*x + y*bytePerLine+1] = c.rgbGreen;
	m_pDibBits[ 3*x + y*bytePerLine+2] = c.rgbRed;

}



DWORD CDib::GetPixel( DWORD x, DWORD y)
{
	if( GetBPP() != 24 || m_pDibBits == NULL)
		return 0xffffffff;

	if( !m_bFlip)
		y = m_dwHeight - y - 1;
		

	DWORD bytePerLine = GetLineOffsetByte();

	// BLUE, GREEN, RED이런 순서로 저장
	DWORD blue = m_pDibBits[ 3*x + y*bytePerLine+0];
	DWORD green = m_pDibBits[ 3*x + y*bytePerLine+1];
	DWORD red = m_pDibBits[ 3*x + y*bytePerLine+2];
	return (red<<16) | (green<<8) | blue;
}

///////////////////////////////////////////
//	color c로 image를 채운다.
///////////////////////////////////////////
void CDib::Clear( RGBQUAD c)
{
	if( GetBPP() != 24 || m_pDibBits == NULL)
		return;

	DWORD bytePerLine = GetLineOffsetByte();
	DWORD x, y;
	for( y = 0; y< m_dwHeight; y++)
	{
		for( x = 0; x< m_dwWidth; x++)
		{
			m_pDibBits[ 3*x + y*bytePerLine+0] = c.rgbBlue;
			m_pDibBits[ 3*x + y*bytePerLine+1] = c.rgbGreen;
			m_pDibBits[ 3*x + y*bytePerLine+2] = c.rgbRed;
		}
	}
}


void CDib::Release()
{
	if( m_pDib ) delete m_pDib;
//	if( m_pBmInfo) delete m_pBmInfo;

	m_pDib = NULL;
	m_pBmInfo = NULL;
	m_pBmInfoHeader = NULL;
	m_pRGBTable = NULL;
	m_pDibBits = NULL;
	m_numColors = 0;

	m_dwDibSize = 0;
	m_dwWidth = 0;
	m_dwHeight = 0;

	m_bFlip = FALSE;

	memset( m_strFileName, 0, sizeof( m_strFileName));

	memset( &m_bmFileHeader, 0, sizeof(m_bmFileHeader));
}

void	CDib::Rotate(int num)
{
	int bytePerLine = GetLineOffsetByte();

	int bpp = GetBPP();

	DWORD addedByte = (m_dwHeight*bpp/8)%4;
	if( addedByte != 0)
		addedByte = (4-addedByte);
	
	int targetBytePerLine = (m_dwHeight*bpp/8 + addedByte);
	if(num == 2)
		targetBytePerLine = bytePerLine;
	
	BYTE *tempBits;
	if(num == 2)
		tempBits = new BYTE[targetBytePerLine*m_dwHeight];
	else
		tempBits = new BYTE[targetBytePerLine*m_dwWidth];

	int targetX, targetY;
	switch(num)
	{
	case 1:
		{
			for(DWORD y = 0; y < m_dwHeight; y++)
			{
				for(DWORD x = 0; x < m_dwWidth; x++)
				{
					targetX = m_dwHeight-y-1;
					targetY = x;
					tempBits[3*targetX + targetY*targetBytePerLine+0] = m_pDibBits[3*x+y*bytePerLine+0];
					tempBits[3*targetX + targetY*targetBytePerLine+1] = m_pDibBits[3*x+y*bytePerLine+1];
					tempBits[3*targetX + targetY*targetBytePerLine+2] = m_pDibBits[3*x+y*bytePerLine+2];
				}
			}
		}
		break;

	case 2:
		{
			for(DWORD y = 0; y < m_dwHeight; y++)
			{
				for(DWORD x = 0; x < m_dwWidth; x++)
				{
					targetX = m_dwWidth-x-1;
					targetY = m_dwHeight-y-1;
					tempBits[3*targetX + targetY*targetBytePerLine+0] = m_pDibBits[3*x+y*bytePerLine+0];
					tempBits[3*targetX + targetY*targetBytePerLine+1] = m_pDibBits[3*x+y*bytePerLine+1];
					tempBits[3*targetX + targetY*targetBytePerLine+2] = m_pDibBits[3*x+y*bytePerLine+2];
				}
			}
		}
		break;

	case 3:
		{
			for(DWORD y = 0; y < m_dwHeight; y++)
			{
				for(DWORD x = 0; x < m_dwWidth; x++)
				{
					targetX = y;
					targetY = m_dwWidth-x-1;
					tempBits[3*targetX + targetY*targetBytePerLine+0] = m_pDibBits[3*x+y*bytePerLine+0];
					tempBits[3*targetX + targetY*targetBytePerLine+1] = m_pDibBits[3*x+y*bytePerLine+1];
					tempBits[3*targetX + targetY*targetBytePerLine+2] = m_pDibBits[3*x+y*bytePerLine+2];
				}
			}
		}
		break;
	}

//	if(targetBytePerLine*m_dwWidth != bytePerLine*m_dwHeight)
	{
		BYTE* pDib;
		if(num == 2)
			pDib = new BYTE[ m_dwDibSize ];
		else
			pDib = new BYTE[ m_dwDibSize+(targetBytePerLine*m_dwWidth-bytePerLine*m_dwHeight)];

		if(num != 2)
			SWAP(m_pBmInfoHeader->biWidth, m_pBmInfoHeader->biHeight);
		memcpy(pDib, m_pDib, m_bmFileHeader.bfOffBits - sizeof( BITMAPFILEHEADER));

		memcpy(pDib+m_bmFileHeader.bfOffBits - sizeof( BITMAPFILEHEADER), tempBits, targetBytePerLine*m_dwWidth);

		m_dwWidth = m_pBmInfoHeader->biWidth;
		m_dwHeight = m_pBmInfoHeader->biHeight;
		
		m_dwDibSize = ::GetDibImageSize( m_dwWidth, m_dwHeight, bpp) + m_bmFileHeader.bfOffBits - sizeof( BITMAPFILEHEADER);

		delete m_pDib;

		m_pDib = pDib;
		
		m_pBmInfo = (LPBITMAPINFO)m_pDib;
		m_pBmInfoHeader = (LPBITMAPINFOHEADER)m_pDib;
		m_pRGBTable = (RGBQUAD*)(m_pDib+m_pBmInfoHeader->biSize);
		
		m_numColors = GetDibNumColors();
		
		//		m_pBmInfoHeader->biSizeImage = GetDibImageSizeImage();
		
		if( m_pBmInfoHeader->biClrUsed == 0)
		{
			//			m_pBmInfoHeader->biClrUsed = m_numColors;
		}
		
		m_pDibBits = m_pDib + m_bmFileHeader.bfOffBits - sizeof( BITMAPFILEHEADER);
		
		
	}

	delete tempBits;
}