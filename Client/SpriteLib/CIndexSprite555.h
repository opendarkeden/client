//----------------------------------------------------------------------
// CIndexSprite555.h
//----------------------------------------------------------------------
// 5:5:5 video card에 대한 sprite class
//----------------------------------------------------------------------

#ifndef	__CINDEXSPRITE555_H__
#define	__CINDEXSPRITE555_H__

#include <Windows.h>
class ofstream;
class ifstream;


#include "CIndexSprite.h"


class CIndexSprite555 : public CIndexSprite {
	public :
		//---------------------------------------------------------
		// fstream에서 save/load를 한다.
		//---------------------------------------------------------
		bool		SaveToFile(class ofstream& file);
		bool		LoadFromFile(class ifstream& file);		

};


#endif



