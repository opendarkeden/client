//----------------------------------------------------------------------
// MShadowObject.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MShadowObject.h"
#include "MShadowObject.h"

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
MShadowObject::MShadowObject()
{
	// type
	m_ObjectType	= TYPE_SHADOWOBJECT;	

	m_bAnimation	= false;
}

MShadowObject::MShadowObject(TYPE_OBJECTID id, TYPE_OBJECTID shadowObjectID, TYPE_SPRITEID SpriteID, int pX, int pY, TYPE_SECTORPOSITION viewpoint, bool trans)
{
	// instace ID¹ß±Þ
	m_ID			= id;

	// type
	m_ObjectType	= TYPE_SHADOWOBJECT;	
	m_ImageObjectID = shadowObjectID;

	m_bAnimation	= false;

	// data
	m_SpriteID		= SpriteID;
	m_PixelX		= pX;
	m_PixelY		= pY;	
	m_Viewpoint		= viewpoint;
	m_bTrans		= trans;
}

MShadowObject::~MShadowObject() 
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
