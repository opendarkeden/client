//----------------------------------------------------------------------
// DrawTypeDef.h
//----------------------------------------------------------------------

#ifndef	__DRAWTYPEDEF_H__
#define	__DRAWTYPEDEF_H__

// sizeof( TYPE_... )
#define	SIZE_SPRITEID		2
#define	SIZE_FRAMEID		2
#define	SIZE_FILTERID		2

#define	SPRITEID_NULL		0xFFFF
#define	FRAMEID_NULL		0xFFFF
#define	FILTERID_NULL		0xFFFF


typedef	unsigned short		TYPE_SPRITEID;
typedef unsigned short		TYPE_FRAMEID;
typedef unsigned short		TYPE_FILTERID;


enum BLT_TYPE 
{
	BLT_NORMAL = 0,
	BLT_EFFECT,
	BLT_SHADOW,
	BLT_SCREEN
};


#endif