//----------------------------------------------------------------------
// MObject.h
//----------------------------------------------------------------------
// 화면에 출력되는 Object
//----------------------------------------------------------------------
/* class hierachy

      MObject --+-- MCreature ---- MCreatureWear -- MPlayer
				|
				+-- MItem
	            |				
				+-- MImageObject -- MAnimationObject -- MInteractionObject
				|
				+-- MEffect --- ....
	            


*/
//----------------------------------------------------------------------

#ifndef	__MOBJECT_H__
#define	__MOBJECT_H__

class ofstream;
class ifstream;
#include "MTypeDef.h"


class MObject {
	public :
		MObject();
		~MObject()	{}

		BYTE	GetObjectType()	const	{ return m_ObjectType; }

		//--------------------------------------------------------
		// id
		//--------------------------------------------------------
		void			SetID(TYPE_OBJECTID id)	{ m_ID = id; }
		TYPE_OBJECTID	GetID()	const			{ return m_ID; }

		//--------------------------------------------------------
		// Sector에서의 좌표
		//--------------------------------------------------------
		void		SetPosition(TYPE_SECTORPOSITION x, TYPE_SECTORPOSITION y)	{ m_X = x; m_Y = y; }
		void		SetX(TYPE_SECTORPOSITION x)		{ m_X = x; }
		void		SetY(TYPE_SECTORPOSITION y)		{ m_Y = y; }
		TYPE_SECTORPOSITION		GetX()	const		{ return m_X; }		// 좌표X
		TYPE_SECTORPOSITION		GetY()	const		{ return m_Y; }		// 좌표Y

		//------------------------------------------------------
		// 화면에서의 영역
		//------------------------------------------------------
		//extern BOOL g_MyFull;
		/*  edit by coffee 2007-01-01
		void				ClearScreenRect()	
		{ 
			// 최대/최소값을 구하기 위한 것이므로.. 거꾸로 넣어야 한다.
			// add by Sonic 2006.9.26
			if(g_MyFull)
			{
				m_ScreenRect.left = 1024;	// 바로 값을 써버린당.
				m_ScreenRect.top = 768;		// -_-; 이러면 안되지만 고치기 짱난다~
				m_ScreenRect.right = 0;
				m_ScreenRect.bottom = 0;
			}
			else
			{
				m_ScreenRect.left = 800;	// 바로 값을 써버린당.
				m_ScreenRect.top = 600;		// -_-; 이러면 안되지만 고치기 짱난다~
				m_ScreenRect.right = 0;
				m_ScreenRect.bottom = 0;
			}
			// end
		}
		*/  //end edit
		void				ClearScreenRect()	
		{
				m_ScreenRect.left = 800;	// 바로 값을 써버린당.
				m_ScreenRect.top = 600;		// -_-; 이러면 안되지만 고치기 짱난다~
				m_ScreenRect.right = 0;
				m_ScreenRect.bottom = 0;
		}
		void				AddScreenRect(RECT* pRect);
		void				SetScreenRect(RECT* pRect)	{ m_ScreenRect = *pRect; }
		const RECT&			GetScreenRect() const	{ return m_ScreenRect; }
		BOOL				IsPointInScreenRect(int x, int y) const
		{
			if (x >= m_ScreenRect.left && x < m_ScreenRect.right
				&& y >= m_ScreenRect.top && y < m_ScreenRect.bottom)
			{
				return TRUE;
			}

			return FALSE;
		}

		//--------------------------------------------------------
		// file I/O
		//--------------------------------------------------------
		virtual void	SaveToFile(class ofstream& file);
		virtual void	LoadFromFile(class ifstream& file);

	public :
		//--------------------------------------------------------
		// Object 종류에 대한 정의 : class ID
		//--------------------------------------------------------
		enum OBJECT_TYPE 
		{
			TYPE_OBJECT = 0,
			TYPE_CREATURE,	
			TYPE_ITEM,			
			TYPE_IMAGEOBJECT,
			TYPE_SHADOWOBJECT,
			TYPE_ANIMATIONOBJECT,
			TYPE_SHADOWANIMATIONOBJECT,
			TYPE_INTERACTIONOBJECT,
			TYPE_PORTAL,
			TYPE_EFFECT			
		};

	protected :
		BYTE				m_ObjectType;	// Object의 종류
		TYPE_OBJECTID		m_ID;			// Object's instance ID
		
		// 위치
		TYPE_SECTORPOSITION				m_X, m_Y;		// Sector 좌표 (pixel단위 아님!)

		// 화면에서의 Sprite충돌 사각 영역
		RECT					m_ScreenRect;
};

#endif