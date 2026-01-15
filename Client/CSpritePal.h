#ifndef __CSPRITE_PAL__
#define __CSPRITE_PAL__

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#else
#include "../../basic/Platform.h"
#endif
#include "CTypeTable.h"

class MPalette
{
public:
	MPalette();
	~MPalette();

	//--------------------------------------------------------
	// Init/Release
	//--------------------------------------------------------
	void	Release();
	void	Init(BYTE size);
 
	//--------------------------------------------------------
	// Size
	//--------------------------------------------------------
	BYTE		GetSize() const { return m_Size; }

	//--------------------------------------------------------
	// operator
	//--------------------------------------------------------
	WORD&		operator [] (BYTE n)		{ return m_pColor[n]; }
	WORD&		operator [] (BYTE n) const { return m_pColor[n]; }
	void		operator = (const MPalette& pal);

	//--------------------------------------------------------
	// file I/O
	//--------------------------------------------------------
	virtual bool	SaveToFile(class ofstream& file) = 0;
	virtual bool	LoadFromFile(class ifstream& file) = 0;		

protected:
	WORD *		m_pColor;
	BYTE		m_Size;
};

class MPalette555 : public MPalette
{
public:
	//--------------------------------------------------------
	// file I/O
	//--------------------------------------------------------
	bool LoadFromFile(class ifstream &file);
	bool SaveToFile(class ofstream &file);
};

class MPalette565 : public MPalette
{
public:
	//--------------------------------------------------------
	// file I/O
	//--------------------------------------------------------
	bool LoadFromFile(class ifstream &file);
	bool SaveToFile(class ofstream &file);
};

class MPalettePack
{
public:
	MPalettePack();
	~MPalettePack();

	//--------------------------------------------------------
	// Init/Release
	//--------------------------------------------------------
	void	Init(WORD size, bool b565 = true);
	void	Release();

	//--------------------------------------------------------
	// Size
	//--------------------------------------------------------
	DWORD		GetSize() const { return m_Size; }

	//--------------------------------------------------------
	// operator
	//--------------------------------------------------------
	MPalette&		operator [] (int n)			{ return m_pPalette[n]; }
	MPalette&		operator [] (int n) const	{ return m_pPalette[n]; }

	//--------------------------------------------------------
	// file I/O
	//--------------------------------------------------------
	bool LoadFromFile(class ifstream &file, bool b565 = true);
	bool SaveToFile(class ofstream &file);

	bool LoadFromFile(LPCTSTR lpszFilename, bool b565 = true);
	bool SaveToFile(LPCTSTR lpszFilename);
	
protected:
	bool			m_b565;
	MPalette *		m_pPalette;
	WORD			m_Size;
};

class CSpritePal
{
public:
	CSpritePal();
	virtual ~CSpritePal();

	void SetPixel(BYTE *pSource, WORD pitch, WORD width, WORD height);
	void SetEmptySprite();

	//--------------------------------------------------------
	// Init/Release
	//--------------------------------------------------------
	bool		IsNotInit() const	{ return !m_bInit; }
	bool		IsInit() const		{ return m_bInit; }
	bool		IsLoading() const	{ return m_bLoading; }

	//---------------------------------------------------------
	// m_pData의 memory를 Release한다.		
	//---------------------------------------------------------
	void		Release();

	//--------------------------------------------------------
	// file I/O
	//--------------------------------------------------------
	bool LoadFromFile(class ifstream &file);
	bool SaveToFile(class ofstream &file);

	//--------------------------------------------------------
	// Get Functions
	//--------------------------------------------------------
	WORD	GetWidth() const	{ return m_Width; }
	WORD	GetHeight() const	{ return m_Height; }

	static void	SetColorKey(BYTE color)		{ s_Colorkey = color; }
	static BYTE	GetColorKey()				{ return s_Colorkey; }

	//--------------------------------------------------------
	// operator
	//--------------------------------------------------------
	void		operator = (const CSpritePal& Sprite);

	//---------------------------------------------------------
	// Blt functions
	//---------------------------------------------------------
	void Blt(int x, int y, WORD* pDest, int pitch, MPalette &pal);
	

protected:
	WORD			m_Width;		// 가로 pixel수
	WORD			m_Height;		// 세로 pixel수		
	DWORD			m_Size;			// 스프라이트의 size

	BYTE**			m_pPixels;		// pixels point array
	BYTE*			m_pData;			// data

	bool			m_bInit;		// data가 있는가?
	bool			m_bLoading;		// Loading중인가?

	static BYTE		s_Colorkey;

};

class CSpritePalPack
{
public:
	CSpritePalPack();
	~CSpritePalPack();
	
	//--------------------------------------------------------
	// Init/Release
	//--------------------------------------------------------
	void	Init(WORD size);
	void	Release();
	
	//--------------------------------------------------------
	// Size
	//--------------------------------------------------------
	DWORD		GetSize() const { return m_Size; }
	
	//--------------------------------------------------------
	// operator
	//--------------------------------------------------------
	CSpritePal&		operator [] (int n)			{ return m_pSpritePals[n]; }
	CSpritePal&		operator [] (int n) const	{ return m_pSpritePals[n]; }
	
	//--------------------------------------------------------
	// file I/O
	//--------------------------------------------------------
	bool LoadFromFile(class ifstream &file);
	bool SaveToFile(class ofstream &file);
	
	bool LoadFromFile(LPCTSTR lpszFilename);
	bool SaveToFile(LPCTSTR lpszFilename);
	
protected:
	CSpritePal *	m_pSpritePals;
	WORD			m_Size;
};

#endif