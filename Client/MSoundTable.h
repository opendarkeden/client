//----------------------------------------------------------------------
// MSoundTable.h
//----------------------------------------------------------------------
// Sound Loading시에 Sound마다 필요한 정보를 저장하는 class
//----------------------------------------------------------------------
//
// [ Sound별로 Loadind에 필요한 정보 ] 
//
// - file이름
// 
//----------------------------------------------------------------------

#ifndef	__MSOUNDTABLE_H__
#define	__MSOUNDTABLE_H__

#include "MTypeDef.h"
#include "CTypeTable.h"
#include "MString.h"
#include "SoundDef.h"


//----------------------------------------------------------------------
//
// 한 종류의 Sound에 대한 정보
//
//----------------------------------------------------------------------
class SOUNDTABLE_INFO {
	public :
		SOUNDTABLE_INFO()
		{
			//pDSBuffer = NULL;			
		}

		~SOUNDTABLE_INFO()
		{
			//if (pDSBuffer != NULL)			
			//{
			//	delete pDSBuffer;
			//}			
		}

	public :
		MString						Filename;	// Sound File이름
		//LPDIRECTSOUNDBUFFER		pDSBuffer;		// load된 wav

	public :
		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		void			SaveToFile(std::ofstream& file);		
		void			LoadFromFile(ifstream& file);		
};

typedef CTypeTable<SOUNDTABLE_INFO>		SOUND_TABLE;
extern 	SOUND_TABLE*		g_pSoundTable;


#endif
