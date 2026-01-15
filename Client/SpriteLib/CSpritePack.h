//----------------------------------------------------------------------
// CSpritePack.h
//----------------------------------------------------------------------
//
// SpritePack의 Sprite에 대한 File Pointer Index가 필요하다.
// SpriteSet에서 읽기 위해서.
//
//----------------------------------------------------------------------

#ifndef	__CSPRITEPACK_H__
#define	__CSPRITEPACK_H__

#include "CTypePack.h"
#include "CSprite555.h"
#include "CSprite565.h"

typedef CTypePack2<CSprite, CSprite555, CSprite565> CSpritePack;

//class CSpritePack : public CTypePack<CSprite>
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
//	bool LoadFromFile(std::ifstream &file, bool b565 = true);
//	bool LoadFromFile(LPCTSTR lpszFilename, bool b565 = true);
//	bool LoadFromFileRunning(LPCTSTR lpszFilename, bool b565 = true);
//	
//protected:
//	bool			m_b565;
//};

#endif


