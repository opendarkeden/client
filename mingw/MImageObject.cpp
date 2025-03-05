//----------------------------------------------------------------------
// MImageObject.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MObject.h"
#include "MImageObject.h"

#ifdef __GAME_CLIENT__
	#include "MTopView.h"
#endif


//----------------------------------------------------------------------
// Init Static Members
//----------------------------------------------------------------------
//TYPE_OBJECTID	MImageObject::s_ID	= 0;

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
MImageObject::MImageObject()
{
	// type
	m_ObjectType	= TYPE_IMAGEOBJECT;	

	m_bAnimation	= false;

	// ImageObject는 m_X를 안 쓰기 때문에.. 임시로(아마 영원히.. -_-;) 사용한다.
	m_X	= WALL_NULL;
}

MImageObject::MImageObject(TYPE_OBJECTID id, TYPE_OBJECTID ImageObjectID, TYPE_SPRITEID SpriteID, int pX, int pY, TYPE_SECTORPOSITION viewpoint, BYTE trans)
{
	// instace ID발급
	m_ID			= id;

	// type
	m_ObjectType	= TYPE_IMAGEOBJECT;	
	m_ImageObjectID = ImageObjectID;

	m_bAnimation	= false;

	// data
	m_SpriteID		= SpriteID;
	m_PixelX		= pX;
	m_PixelY		= pY;	
	m_Viewpoint		= viewpoint;
	m_bTrans		= trans;
}

MImageObject::~MImageObject() 
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
MImageObject::Set(TYPE_OBJECTID ImageObjectID, TYPE_SPRITEID SpriteID, int pX, int pY, TYPE_SECTORPOSITION viewpoint, BYTE trans)
{
	m_ImageObjectID = ImageObjectID;
	m_SpriteID		= SpriteID;
	m_PixelX		= pX;
	m_PixelY		= pY;	
	m_Viewpoint		= viewpoint;
	m_bTrans		= trans;
}

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
void	
MImageObject::SaveToFile(ofstream& file)
{
	MObject::SaveToFile(file);	

	file.write((const char*)&m_ImageObjectID, SIZE_OBJECTID);
	file.write((const char*)&m_SpriteID, SIZE_SPRITEID);
	file.write((const char*)&m_PixelX, 4);
	file.write((const char*)&m_PixelY, 4);
	file.write((const char*)&m_Viewpoint, SIZE_SECTORPOSITION);
	file.write((const char*)&m_bAnimation, 1);
	file.write((const char*)&m_bTrans, 1);
}
		
//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
void	
MImageObject::LoadFromFile(ifstream& file)
{
	MObject::LoadFromFile(file);

	file.read((char*)&m_ImageObjectID, SIZE_OBJECTID);
	file.read((char*)&m_SpriteID, SIZE_SPRITEID);
	file.read((char*)&m_PixelX, 4);
	file.read((char*)&m_PixelY, 4);
	file.read((char*)&m_Viewpoint, SIZE_SECTORPOSITION);
	file.read((char*)&m_bAnimation, 1);
	file.read((char*)&m_bTrans, 1);
}

//----------------------------------------------------------------------
// Set Wall
//----------------------------------------------------------------------
// m_X에는 WallDirection
// m_Y에는 WallValue를 저장한다.
//----------------------------------------------------------------------
void				
MImageObject::SetWall(WALL_DIRECTION wd)//, int x, int y)
{
	m_X = wd;

	/*
	switch (m_X)
	{
		//-------------------------------------------------------------
		// 오른쪽으로 가면서 아래로 내려가는 벽
		//-------------------------------------------------------------
		case WALL_RIGHTDOWN :
			// x - y 값이 저장되어있으므로..
			// 값이 음수일수도 있다. (m_Y는 unsigned이다. - -;)
			m_Y = x - y;
		break;

		//-------------------------------------------------------------
		// 오른쪽으로 가면서 위로 올라가는 벽
		//-------------------------------------------------------------
		case WALL_RIGHTUP :
			// x + y 의 값
			m_Y = x + y;
		break;
	}
	*/
}

//----------------------------------------------------------------------
// Is Wall
//----------------------------------------------------------------------
bool				
MImageObject::IsWall() const
{
	return m_X != WALL_NULL;
}

//----------------------------------------------------------------------
// Get Wall Direction
//----------------------------------------------------------------------
MImageObject::WALL_DIRECTION		
MImageObject::GetWallDirection() const
{
	return (MImageObject::WALL_DIRECTION)m_X;
}

//----------------------------------------------------------------------
// Is Trans Position
//----------------------------------------------------------------------
// player가 sector좌표 (sX,sY)에 있을때 이 Object는 투명이 되는가?
//
// WALL_RIGHTDOWN 방향일 때,
//			value = x-y
//			sX-sY가 value보다 큰 경우가 투명이 된다.
//
// WALL_RIGHTUP 방향일 때,
//			value = x+y
//			sX+sY가 value보다 적은 경우 투명이 된다.
//
//----------------------------------------------------------------------
bool
MImageObject::IsWallTransPosition(int sX, int sY) const
{
	#ifdef __GAME_CLIENT__			
		switch (m_X)
		{
			//-------------------------------------------------------------
			// 오른쪽으로 가면서 아래로 내려가는 벽
			//-------------------------------------------------------------
			case WALL_RIGHTDOWN :
			{
				// imageObject의 sector X좌표
				int objectSX = MTopView::PixelToMapX( m_PixelX );
		
				// x - y
				int value = objectSX - (int)m_Viewpoint;			
				
				if (sX-sY > value)
				{
					return true;
				}

				return false;
			}
			break;

			//-------------------------------------------------------------
			// 오른쪽으로 가면서 위로 올라가는 벽
			//-------------------------------------------------------------
			case WALL_RIGHTUP :
			{
				// imageObject의 sector X좌표
				int objectSX = MTopView::PixelToMapX( m_PixelX );
				
				// x + y
				int value = objectSX + (int)m_Viewpoint;			

				if (sX+sY < value)
				{
					return true;
				}

				return false;
			}
			break;
		}
	#endif

	return true;
}