//----------------------------------------------------------------------
// CAlphaSprite565.h
//----------------------------------------------------------------------
// 5:6:5 video card에 대한 sprite class
//----------------------------------------------------------------------

#ifndef	__CALPHASPRITE565_H__
#define	__CALPHASPRITE565_H__

#include <Windows.h>
class ofstream;
class ifstream;


#include "CAlphaSprite.h"


class CAlphaSprite565 : public CAlphaSprite {
	public :
		//---------------------------------------------------------
		// fstream에서 save/load를 한다.
		//---------------------------------------------------------
		bool		SaveToFile(class ofstream& file);
		bool		LoadFromFile(class ifstream& file);
		//bool	LoadFromFileToBuffer(ifstream& file);
};


#endif



