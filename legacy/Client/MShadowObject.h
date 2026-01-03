//----------------------------------------------------------------------
// MShadowObject.h
//----------------------------------------------------------------------
// ImageObject랑 같다고 보면 되는데..
// 출력되는 image는 Shadow다..
//----------------------------------------------------------------------

#ifndef	__MSHADOWOBJECT_H__
#define	__MSHADOWOBJECT_H__

class ofstream;
class ifstream;
#include "MTypeDef.h"
#include "DrawTypeDef.h"
#include "MImageObject.h"

//----------------------------------------------------------------------
//
// MShadowObject class
//
//----------------------------------------------------------------------
class MShadowObject : public MImageObject {
	public :
		MShadowObject();
		MShadowObject(TYPE_OBJECTID id, TYPE_OBJECTID shadowObjectID, TYPE_SPRITEID nSprite, int pX, int pY, TYPE_SECTORPOSITION viewpoint, bool trans);
		~MShadowObject();
};


#endif


