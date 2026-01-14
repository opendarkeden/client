#ifndef __CSPRITEPALBASE_H__
#define __CSPRITEPALBASE_H__

#include "MPalette.h"
#ifdef PLATFORM_WINDOWS
	#include <windows.h>
#else
	#include "../basic/Platform.h"
#endif

class CSpritePalBase
{
public:
	CSpritePalBase();
	virtual ~CSpritePalBase();
	
	void SetEmptySprite();
	bool IsEmptySprite() const		{ if(IsInit() && m_Size == 0) return true; return false; }
	
	//--------------------------------------------------------
	// Init/Release
	//--------------------------------------------------------
	bool		IsNotInit() const	{ return !m_bInit; }
	bool		IsInit() const		{ return m_bInit; }
	bool		IsLoading() const	{ return m_bLoading; }
	
	//---------------------------------------------------------
	// m_pData�� memory�� Release�Ѵ�.		
	//---------------------------------------------------------
	void		Release();
	
	//--------------------------------------------------------
	// file I/O
	//--------------------------------------------------------
	bool LoadFromFile(std::ifstream &file);
	bool SaveToFile(std::ofstream &file);
	
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
	void		operator = (const CSpritePalBase& Sprite);
	
	//---------------------------------------------------------
	// Blt functions
	//---------------------------------------------------------
	virtual void Blt(int x, int y, WORD* pDest, int pitch, MPalette &pal) = 0;
	
	//---------------------------------------------------------
	// Pixel
	//---------------------------------------------------------
	virtual bool	IsColorPixel(short x,short y) = 0;
	virtual WORD	GetPixel(short x, short y, MPalette &pal) = 0;
	
protected:
	WORD			m_Width;		// ���� pixel��
	WORD			m_Height;		// ���� pixel��		
	DWORD			m_Size;			// ��������Ʈ�� size
	
	BYTE**			m_pPixels;		// pixels point array
	BYTE*			m_pData;			// data
	
	bool			m_bInit;		// data�� �ִ°�?
	bool			m_bLoading;		// Loading���ΰ�?
	
	static BYTE		s_Colorkey;
	
};

#endif