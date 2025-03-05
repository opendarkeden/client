//----------------------------------------------------------------------
// MAnimationObject.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MAnimationObject.h"

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
MAnimationObject::MAnimationObject()
: CAnimationFrame(BLT_NORMAL)
{
	// type
	m_ObjectType	= TYPE_ANIMATIONOBJECT;	

	m_bAnimation	= true;
	
	m_Direction		= 0;
}

MAnimationObject::MAnimationObject(TYPE_OBJECTID id, TYPE_OBJECTID ImageObjectID, TYPE_SPRITEID SpriteID, int pX, int pY, TYPE_SECTORPOSITION viewpoint, bool trans, BYTE bltType)
: CAnimationFrame(bltType)
{
	// instace ID발급
	m_ID			= id;

	// type
	m_ObjectType	= TYPE_ANIMATIONOBJECT;	
	m_ImageObjectID = ImageObjectID;

	m_bAnimation	= true;

	// data
	m_SpriteID		= SpriteID;
	m_PixelX		= pX;
	m_PixelY		= pY;	
	m_Viewpoint		= viewpoint;
	m_bTrans		= trans;	

	m_Direction		= 0;
}

MAnimationObject::~MAnimationObject() 
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Set
//----------------------------------------------------------------------
void
MAnimationObject::Set(TYPE_OBJECTID id, TYPE_OBJECTID ImageObjectID, TYPE_SPRITEID SpriteID, int pX, int pY, TYPE_SECTORPOSITION viewpoint, bool trans, BYTE bltType)
{
	// instace ID발급
	m_ID			= id;

	// type
	m_ObjectType	= TYPE_ANIMATIONOBJECT;	
	m_ImageObjectID = ImageObjectID;

	m_bAnimation	= true;

	// data
	m_SpriteID		= SpriteID;
	m_PixelX		= pX;
	m_PixelY		= pY;	
	m_Viewpoint		= viewpoint;
	m_bTrans		= trans;
	
	m_BltType		= bltType;
}
//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
void	
MAnimationObject::SaveToFile(ofstream& file)
{
	MImageObject::SaveToFile(file);	
	CAnimationFrame::SaveToFile(file);	

	file.write((const char*)&m_BltType, 1);
	file.write((const char*)&m_Direction, 1);
	file.write((const char*)&m_SoundFrame, 1);
	file.write((const char*)&m_SoundID, SIZE_SOUNDID);

	ShowTimeChecker::SaveToFile(file);
}
		
//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
void	
MAnimationObject::LoadFromFile(ifstream& file)
{
	MImageObject::LoadFromFile(file);
	CAnimationFrame::LoadFromFile(file);
//
////	m_ImageObjectFPK[ frameID ]
////	g_CurrentFrame % g_pTopView->m_ImageObjectFPK[ frameID ].GetSize();
//	CAnimationFrame::SetFrameID(CAnimationFrame::GetFrameID(), g_pTopView->m_ImageObjectFPK[ CAnimationFrame::GetFrameID()].GetSize());

	file.read((char*)&m_BltType, 1);
	file.read((char*)&m_Direction, 1);
	file.read((char*)&m_SoundFrame, 1);
	file.read((char*)&m_SoundID, SIZE_SOUNDID);

	ShowTimeChecker::LoadFromFile(file);

}

DWORD	MAnimationObject::LoopFrameCount = 0;

//----------------------------------------------------------------------
// Next Frame
//----------------------------------------------------------------------
void	
MAnimationObject::NextFrame()
{ 
	if(Loop)
	{
		if(m_MaxFrame > 0)
			m_CurrentFrame = (BYTE)(LoopFrameCount % m_MaxFrame);
	}
	else
	{
		if (++m_CurrentFrame==m_MaxFrame) 
		{
	//		if (IsShowTime())
	//		{
	//			m_CurrentFrame = 0; 
	//		}
	//		else
	//		{
				m_CurrentFrame = 0;
	//		}
		}
	}
}
