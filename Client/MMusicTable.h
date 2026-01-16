//----------------------------------------------------------------------
// MMusicTable.h
//----------------------------------------------------------------------
// Music Loading시에 Music마다 필요한 정보를 저장하는 class
//----------------------------------------------------------------------
//
// [ Music별로 Loadind에 필요한 정보 ] 
//
// - file이름
// 
//----------------------------------------------------------------------

#ifndef	__MMUSICTABLE_H__
#define	__MMUSICTABLE_H__

#include "MTypeDef.h"
#include "CTypeTable.h"
#include "MString.h"

//----------------------------------------------------------------------
//
// 한 종류의 Music에 대한 정보
//
//----------------------------------------------------------------------
class MUSICTABLE_INFO {
	public :
		MString					Filename;		// Music File이름(MID)
		MString					FilenameWav;	// wave Music File이름

	public :
		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		void			SaveToFile(std::ofstream& file);
		void			LoadFromFile(std::ifstream& file);
};

typedef CTypeTable<MUSICTABLE_INFO>	MUSIC_TABLE;
extern 	MUSIC_TABLE*	g_pMusicTable;

#endif
