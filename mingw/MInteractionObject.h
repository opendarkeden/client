//----------------------------------------------------------------------
// MInteractionObject.h
//----------------------------------------------------------------------
//
// Action이 있는 ImageObject
// 
//----------------------------------------------------------------------
//
// 단, 각각의 Action에 대해서는 Animation이 없다.
// 하나의 Action은 하나의 Frame에 대응한다.
//
//----------------------------------------------------------------------

#ifndef	__MINTERACTIONOBJECT_H__
#define	__MINTERACTIONOBJECT_H__



#include "MTypeDef.h"
#include "MAnimationObject.h"


//----------------------------------------------------------------------
//
// InteractionObject class
//
//----------------------------------------------------------------------
class MInteractionObject : public MAnimationObject {
	public :
		MInteractionObject();
		MInteractionObject(TYPE_INTERACTIONOBJECTTYPE iaotype, TYPE_OBJECTID id, TYPE_OBJECTID ImageObjectID, TYPE_SPRITEID nSprite, int pX, int pY, TYPE_SECTORPOSITION viewpoint, bool trans, BYTE type=BLT_NORMAL);
		~MInteractionObject();

		//--------------------------------------------------------
		// Action에 관련된 함수
		//--------------------------------------------------------		
		void			SetAction(BYTE action);
		BYTE			GetAction() const			{ return m_CurrentFrame; }

		//--------------------------------------------------------		
		// action의 다음 action을 설정한다.
		//--------------------------------------------------------		
		void			SetNextAction(BYTE action);

		//--------------------------------------------------------		
		// InteractionObject type
		//--------------------------------------------------------		
		BYTE			GetInteractionObjectType() const	{ return m_InteractionObjectType; }

		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		virtual void	SaveToFile(ofstream& file);
		virtual void	LoadFromFile(ifstream& file);

	public :
		enum INTERACTIONOBJECT_TYPE 
		{
			TYPE_DOOR = 0,
			TYPE_TRAP,
			TYPE_SWITCH,
			TYPE_MAX
		};
		
	protected :
		// Action을 바꾼다.
		void			ChangeActionDoor();
		void			ChangeActionTrap();
		void			ChangeActionSwitch();

	protected :
		BYTE			m_InteractionObjectType;
};


#endif


