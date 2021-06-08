#ifndef __CDIB_H
#define __CDIB_H

#include <windows.h>
 
class CDib
{
//
//	Member Variables
//
protected:
	LPBITMAPINFO		m_pBmInfo;
	LPBITMAPINFOHEADER	m_pBmInfoHeader;
	RGBQUAD*			m_pRGBTable;
	BYTE*				m_pDibBits;
	UINT				m_numColors;
	BYTE*				m_pDib;
	char				m_strFileName[256];
	BOOL				m_bFlip;
public:
	DWORD				m_dwWidth, m_dwHeight;
	DWORD				m_dwDibSize;			// BMP화일에서 BITMAPFILEHEADER의 크기를 제외한 크기
	BITMAPFILEHEADER	m_bmFileHeader;

//
//	Member Functions
//
public:
	void	Rotate(int num);
	void Release();
	void PutPixel( DWORD x, DWORD y, RGBQUAD c);
	DWORD GetPixel( DWORD x, DWORD y);
	CDib();
	CDib( const char* fileName, BOOL bFlip = TRUE);
	CDib( const CDib& dib);
	~CDib();

	void operator=( const CDib& dib);
	BOOL operator==( const CDib& dib);

	DWORD				GetDibSizeImage() const ;
	UINT				GetDibNumColors() const ;
	LPBITMAPINFOHEADER	GetDibInfoHeaderPtr() const ;
	LPBITMAPINFO		GetDibInfoPtr() const ;
	LPRGBQUAD			GetDibRGBTablePtr() const ;
	BYTE*				GetDibBitsPtr() const ;
	BOOL				LoadBitmapFile( const char* fileName, BOOL bFlip = TRUE);
	BOOL				MakeClone( const CDib& srcDib, DWORD width, DWORD height);
	DWORD				GetBytesPerPixel() const ;
	DWORD				GetBPP() const { return m_pBmInfoHeader->biBitCount;};

	BOOL				Save( const char* filename);
	void				Blt( const CDib& srcDib, DWORD srcX, DWORD srcY, DWORD width, DWORD height, DWORD destX, DWORD destY);
	void				BltDC( HDC hDC, int XDest, int YDest, int nDestWidth, int nDestHeight, 
								int XSrc, int YSrc, int nSrcWidth, int nSrcHeight, DWORD dwRop = SRCCOPY);
	void				TransparentBltDC( HDC hDC, int XDest, int YDest, int nDestWidth, int nDestHeight, 
								int XSrc, int YSrc, int nSrcWidth, int nSrcHeight, COLORREF tpColor);
	void				TransparentBltDC( HDC hDC, int XDest, int YDest, int nDestWidth, int nDestHeight, 
								int XSrc, int YSrc, int nSrcWidth, int nSrcHeight);
	DWORD				GetLineOffsetByte() const ;

	LPCSTR				GetFileName() const { return m_strFileName;} 
	void				Clear( RGBQUAD c);

	int					GetColorPixelNum();
};
 
#endif