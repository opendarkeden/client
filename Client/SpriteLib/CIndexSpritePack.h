//----------------------------------------------------------------------
// CIndexSpritePack.h
//----------------------------------------------------------------------
//
// SpritePack의 Sprite에 대한 File Pointer Index가 필요하다.
// SpriteSet에서 읽기 위해서.
//
//----------------------------------------------------------------------

#ifndef	__CINDEXSPRITEPACK_H__
#define	__CINDEXSPRITEPACK_H__

#include "CIndexSprite555.h"
#include "CIndexSprite565.h"
#include "CTypePack.h"

typedef CTypePack2<CIndexSprite, CIndexSprite555, CIndexSprite565> CIndexSpritePack;

//class CIndexSpritePack : public CTypePack<CIndexSprite>
//{
//public:
//	//--------------------------------------------------------
//	// Init/Release
//	//--------------------------------------------------------
//	void	Init(WORD size, bool b565 = true);
//	
//	//--------------------------------------------------------
//	// file I/O
//	//--------------------------------------------------------
//	bool LoadFromFile(class ifstream &file, bool b565 = true);
//	bool LoadFromFile(LPCTSTR lpszFilename, bool b565 = true);
//	bool LoadFromFileRunning(LPCTSTR lpszFilename, bool b565 = true);
//	
//protected:
//	bool			m_b565;
//};

#endif


