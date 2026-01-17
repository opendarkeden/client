#include <fstream>
//----------------------------------------------------------------------
// CSpriteDef.h
//----------------------------------------------------------------------
#ifndef	__CSPRITEDEF_H__
#define	__CSPRITEDEF_H__

#pragma warning(disable:4786)

#include <list>
#include "DrawTypeDef.h"
#include "TArray.h"
std::ifstream;
std::ofstream;

// int list
typedef	std::list<int>	INT_LIST;

//----------------------------------------------------------------------
// (SpriteID, FilePosition)의 List를 받아서 Load한다.
//----------------------------------------------------------------------
class SPRITE_FILEPOSITION_NODE
{
	public :
		TYPE_SPRITEID		SpriteID;
		long				FilePosition;

	public :
		void		SaveToFile(std::ofstream& file);
		void		LoadFromFile(std::ifstream& file);

};

//----------------------------------------------------------------------
// FilePosition Array 정의
//----------------------------------------------------------------------
typedef	TArray<SPRITE_FILEPOSITION_NODE, TYPE_SPRITEID>	CSpriteFilePositionArray;



#endif
