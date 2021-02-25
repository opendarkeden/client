#ifndef __MPALETTE_PACK_H__
#define __MPALETTE_PACK_H__

#include "CTypePack.h"
#include "MPalette.h"

typedef CTypePack2<MPalette, MPalette555, MPalette565> MPalettePack;

//class MPalettePack: public CTypePack2<MPalette>
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
//	
//protected:
//	bool			m_b565;
//};

#endif