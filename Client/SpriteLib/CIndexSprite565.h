//----------------------------------------------------------------------
// CIndexSprite565.h
//----------------------------------------------------------------------
// 5:6:5 video card에 대한 sprite class
//----------------------------------------------------------------------

#ifndef	__CINDEXSPRITE565_H__
#define	__CINDEXSPRITE565_H__

#ifdef PLATFORM_WINDOWS
	#include <Windows.h>
#else
	#include "../basic/Platform.h"
#endif
std::ofstream;
std::ifstream;


#include "CIndexSprite.h"


class CIndexSprite565 : public CIndexSprite {
	public :
		//---------------------------------------------------------
		// fstream에서 save/load를 한다.
		//---------------------------------------------------------
		bool		SaveToFile(std::ofstream& file);
		bool		LoadFromFile(std::ifstream& file);		

};


#endif



