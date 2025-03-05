//----------------------------------------------------------------------
// MShadowAnimationObject.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MShadowAnimationObject.h"
#include "MShadowAnimationObject.h"

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
MShadowAnimationObject::MShadowAnimationObject()
{
	// type
	m_ObjectType	= TYPE_SHADOWANIMATIONOBJECT;	

	m_bAnimation	= true;
}

MShadowAnimationObject::MShadowAnimationObject(TYPE_OBJECTID id, TYPE_OBJECTID shadowObjectID, TYPE_SPRITEID SpriteID, int pX, int pY, TYPE_SECTORPOSITION viewpoint, bool trans)
: MAnimationObject(id, shadowObjectID, SpriteID, pX, pY, viewpoint, trans, BLT_NORMAL)
{
	m_ID			= id;

	// type
	m_ObjectType	= TYPE_SHADOWANIMATIONOBJECT;	
	m_ImageObjectID = shadowObjectID;

	m_bAnimation	= false;

	// data
	m_SpriteID		= SpriteID;
	m_PixelX		= pX;
	m_PixelY		= pY;	
	m_Viewpoint		= viewpoint;
	m_bTrans		= trans;
}

MShadowAnimationObject::~MShadowAnimationObject() 
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
