//----------------------------------------------------------------------
// MEffectStatusTable.h
//----------------------------------------------------------------------
// 캐릭터에 붙는 Effect 중에서
// 특별한 상태를 나타내는 것들에 대한 EffectSpriteType의 table.. 
//
// 하나의 상태는 하나의 EffectSpriteType으로 나타내어 진다..라고 가정.
//
// * 상태 --> EffectSpriteType
//
//----------------------------------------------------------------------

#ifndef	__MEFFECTSTATUSTABLE_H__
#define	__MEFFECTSTATUSTABLE_H__

#include "CTypeTable.h"
#include "MTypeDef.h"
//#include "EffectSpriteTypeDef.h"
#include "MEffectStatusDef.h"
#include <fstream>

using namespace std;

//----------------------------------------------------------------------
//
// 한 종류의 EffectStatus --> EffectSpriteType에 대한 정보
//
//----------------------------------------------------------------------
class EFFECTSTATUS_NODE {
	public :
		EFFECTSTATUS_NODE();

	public :
		bool						bUseEffectSprite;		// EffectSprite를 사용하나?
		bool						bAttachGround;			// 캐릭터 바닥에 붙나?

		// union으로 해도 되는데.. - -
		TYPE_EFFECTSPRITETYPE		EffectSpriteType;		// sprite붙이기
		WORD						EffectColor;			// 색깔 바꾸기
		ADDON						EffectColorPart;		// 색깔 바뀌는 부위
		
		TYPE_ACTIONINFO				ActionInfo;				// actionInfo의 effect사용
		TYPE_ACTIONINFO				OriginalActionInfo;		// EffectStatus와 ActionInfo를 이어준다.
		int							SoundID;				// SoundID
		
	public :
		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		void			SaveToFile(ofstream& file);		
		void			LoadFromFile(ifstream& file);		
};


typedef CTypeTable<EFFECTSTATUS_NODE>	EFFECTSTATUS_TABLE;
extern 	EFFECTSTATUS_TABLE*		g_pEffectStatusTable;


#endif


