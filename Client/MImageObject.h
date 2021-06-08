//----------------------------------------------------------------------
// MImageObject.h
//----------------------------------------------------------------------
//
// 하나의 그림에 대한 정보
// 
//  - 그림의 그림은 Surface에 저장되는 것이 아니라 
//    Sprite로 저장된다.  크기가 다양하기 때문이다.
//    당연한 얘기지만, 그림Sprite는 사각형이다.
//    투명색을 포함하기 때문에 마름모꼴 그림이기는 하지만...
//
//  - 그림의 위치는 Sector좌표로 표시하기가 곤란하다.
//    사각형 그림만 있는 것이 아니기 때문에 사각영역도 안된다.
//    그냥 Zone에서 Pixel단위의 좌표로 표시해야 한다.
//    즉, 그림 Sprite의 좌측상단이 위치는 지점이 그림의 위치이다.
//
// 그림 그림은 그저 Zone에서 어떤 위치(pixel)에 그려질 뿐이다.
// 그러므로, 충돌 check 같은 정보들은 Zone의 Sector에 표현해야 한다.
//
// 그림의 문이 있을 경우는 MTransfer로 Sector에 표현되는 것이지
// 특별히 다른 처리를 하는 것이 아니다.
//
// 그림의 그림은 같은 시점에서 출력되는 것이 하나의 Sprite로 표현되어야 한다.
//
//----------------------------------------------------------------------
//
// [ Sector의 정보 설정 ]
//
// 그림이 화면에 출력되는 Sector에는 FLAG_SECTOR_BUILDING를 표시한다.
// 그림의 바닥이 위치하는 Sector에는 FLAG_SECTOR_BLOCK를 표시한다.
//
// FLAG_SECTOR_BUILDING은 표시되었지만, FLAG_SECTOR_BLOCK이 표시되지
// 않은 곳은 Player가 그림에 가려지는 부분이므로 그림을 반투명 처리 해야한다.
//----------------------------------------------------------------------
//
// 그림은 결과적으로 하나의 그림으로 표현되기는 하지만,
// 내부적으로는 몇개의 Sprite로 나뉠 수 있다.
// 단, 반투명 처리를 할 때, 여러개의 Sprite가 동시에 투명이 되어야 한다.
// 
// 
//----------------------------------------------------------------------
// - 두개의 Image로 하나의 Object를 표현해야할 경우가 있다.
//   이때 Object를 구분하는게 ImageObjectID이다.
//
// - m_ID는 같은 Object라도 다르다. 같을 수 없다.
//----------------------------------------------------------------------

#ifndef	__MIMAGEOBJECT_H__
#define	__MIMAGEOBJECT_H__

class ofstream;
class ifstream;
#include "MTypeDef.h"
#include "MObject.h"
#include "DrawTypeDef.h"

#define	IMAGEOBJECT_NULL	0xFFFF
#define OBJECT_TRANS_FLAG	1
#define OBJECT_HALF_FLAG	2
//----------------------------------------------------------------------
//
// ImageObject class
//
//----------------------------------------------------------------------
class MImageObject : public MObject {
	public :
		// 이거는 벽같은거.. 방향을 설정할때 쓴다.
		enum WALL_DIRECTION
		{
			WALL_RIGHTDOWN = 1,		// 오른쪽으로 가면서 아래로 내려가는 벽
			WALL_RIGHTUP,			// 오른쪽으로 가면서 위로 올라가는 벽			

			WALL_NULL = SECTORPOSITION_NULL,			// 벽 같은게 아니다. - -;			
		};

	public :
		MImageObject();
		MImageObject(TYPE_OBJECTID id, TYPE_OBJECTID ImageObjectID, TYPE_SPRITEID nSprite, int pX, int pY, TYPE_SECTORPOSITION viewpoint, BYTE trans);
		~MImageObject();
		
		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		virtual void	SaveToFile(class ofstream& file);
		virtual void	LoadFromFile(class ifstream& file);

		//-------------------------------------------------------
		// Is...
		//-------------------------------------------------------
		bool	IsAnimation() const		{ return m_bAnimation; }
		bool	IsSame(const MImageObject* pImageObject) const	{ return m_ImageObjectID==pImageObject->m_ImageObjectID; }
		//bool	IsTrans() const			{ return m_bTrans; }
		bool	IsTrans() const			{ return (m_bTrans&OBJECT_TRANS_FLAG)?true:false; }
		bool	IsHalf() const			{ return (m_bTrans&OBJECT_HALF_FLAG)?true:false; }
		//-------------------------------------------------------
		// set				
		//-------------------------------------------------------
		void	Set(TYPE_OBJECTID ImageObjectID, TYPE_SPRITEID SpriteID, int pX, int pY, TYPE_SECTORPOSITION viewpoint, BYTE trans);
		void	SetPixelX(int px)		{ m_PixelX = px; }
		void	SetPixelY(int py)		{ m_PixelY = py; }
		void	SetViewpoint(TYPE_SECTORPOSITION vp)	{ m_Viewpoint = vp; }
		//void	SetTrans()				{ m_bTrans = true; }
		//void	UnSetTrans()				{ m_bTrans = false; }
		void	SetTrans()				{ m_bTrans |= OBJECT_TRANS_FLAG; }
		void	UnSetTrans()				{ m_bTrans &= ~OBJECT_TRANS_FLAG; }
		void	SetHalf()				{ m_bTrans |= OBJECT_HALF_FLAG; }
		void	UnSetHalf()				{ m_bTrans &= ~OBJECT_HALF_FLAG; }

		//-------------------------------------------------------
		// get
		//-------------------------------------------------------
		TYPE_OBJECTID			GetImageObjectID()		{ return m_ImageObjectID; }
		TYPE_SPRITEID	GetSpriteID() const		{ return m_SpriteID; }
		int				GetPixelX()		const	{ return m_PixelX; }
		int				GetPixelY()		const	{ return m_PixelY; }
		TYPE_SECTORPOSITION			GetViewpoint() const	{ return m_Viewpoint; }
		
		//-------------------------------------------------------
		// 그림이 있는 Sector에 대한 정보
		//-------------------------------------------------------

		//-------------------------------------------------------
		// 벽 같은거(-_-;)
		//-------------------------------------------------------
		void				SetWall(WALL_DIRECTION wd);
		bool				IsWall() const;
		WALL_DIRECTION		GetWallDirection() const;		
		int					GetWallValue() const;
		bool				IsWallTransPosition(int sX, int sY) const;
		

	protected :

		// Sprite가 여러개일때 한 그림은 하나의 ID를 갖도록 해야할 것이다.
		// 같은 그림임을 나타내는 ID가 있어야 할 것이다.
		TYPE_OBJECTID			m_ImageObjectID;

		// SpritePack에 있는 Sprite ID
		TYPE_SPRITEID			m_SpriteID;

		// Animation되는 것인가?
		bool					m_bAnimation;

		// 그림이 출력될 Zone상의 Pixel위치
		int						m_PixelX;
		int						m_PixelY;

		// 출력될 Sector 상의 세로 위치
		TYPE_SECTORPOSITION					m_Viewpoint;
		
		// Player를 가릴 때 반투명이 되어야 하는가?
		BYTE					m_bTrans;
};


#endif


