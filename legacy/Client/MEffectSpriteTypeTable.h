//----------------------------------------------------------------------
// MEffectSpriteTypeTable.h
//----------------------------------------------------------------------
// EffectSpriteType의 종류에 따른 정보를 저장하는 class
//----------------------------------------------------------------------
//
// [ 종류별로 있는 정보 ] 
//
// - Tile에서의 FrameID
// - Inventory에서의 SpriteID
// - 이름... 등등...
// 
//----------------------------------------------------------------------

#ifndef	__MEFFECTSPRITETYPETABLE_H__
#define	__MEFFECTSPRITETYPETABLE_H__

#pragma warning(disable:4786)

#include "MTypeDef.h"
#include "DrawTypeDef.h"
#include "CTypeTable.h"
//#include "EffectSpriteTypeDef.h"
class ifstream;
class ofstream;

//----------------------------------------------------------------------
//
// 한 종류의 EffectSpriteType에 대한 정보
//
//----------------------------------------------------------------------
class EFFECTSPRITETYPETABLE_INFO {
	public :
		typedef std::list<TYPE_FRAMEID>		FRAMEID_LIST;

	public :
		BLT_TYPE					BltType;				// 출력형식
		TYPE_FRAMEID				FrameID;				// Frame ID	
		bool						RepeatFrame;			// 반복되는 frame인가?
		TYPE_FRAMEID				ActionEffectFrameID;	// action 마다 다른 frame을 보여줘야하는 effect인가?
		//TYPE_FRAMEID				PairFrameID;			// 동시에 출력되어야 하는 짝~이다.
		FRAMEID_LIST				PairFrameIDList;		// 동시에 출력되어야 하는 이펙트들~이다.
		bool						bPairFrameBack;			// 동시에 출력되는 이펙트가 뒤에 출력되는가
		TYPE_EFFECTSPRITETYPE		FemaleEffectSpriteType;	// 여자용..

	public :
		EFFECTSPRITETYPETABLE_INFO();
		//-------------------------------------------------------
		// Save
		//-------------------------------------------------------
		void			SaveToFile(class ofstream& file);	
		void			LoadFromFile(class ifstream& file);
		
};

typedef CTypeTable<EFFECTSPRITETYPETABLE_INFO>	EFFECTSPRITETYPE_TABLE;


//----------------------------------------------------------------------
//
// Action에 맞는 EffectSpriteType
//
//----------------------------------------------------------------------
class FRAME_TYPE {
	public :
		TYPE_FRAMEID		FrameID;

	public :
		void		SaveToFile(class ofstream& file);
		void		LoadFromFile(class ifstream& file);
};

typedef CTypeTable<FRAME_TYPE>	FRAMEID_ARRAY;
class ACTION_FRAMEID_ARRAY : public FRAMEID_ARRAY {
	public :
		ACTION_FRAMEID_ARRAY();
		~ACTION_FRAMEID_ARRAY();
};

typedef CTypeTable<ACTION_FRAMEID_ARRAY>	ACTION_FRAMEID_TABLE;

class MActionEffectSpriteTypeTable : public ACTION_FRAMEID_TABLE {
	public :
		MActionEffectSpriteTypeTable();
		~MActionEffectSpriteTypeTable();
};


extern	EFFECTSPRITETYPE_TABLE*			g_pEffectSpriteTypeTable;
extern	MActionEffectSpriteTypeTable*	g_pActionEffectSpriteTypeTable;

#endif

