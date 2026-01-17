//----------------------------------------------------------------------
// MPortal.h
//----------------------------------------------------------------------
// Zone 이동 정보
//----------------------------------------------------------------------

#ifndef	__MPORTAL_H__
#define	__MPORTAL_H__

#include "MObject.h"
#include <vector>

#define	FLAG_MIP_SAFE_SLAYER		0x01
#define	FLAG_MIP_SAFE_VAMPIRE		0x02
#define	FLAG_MIP_SAFE_COMMON		0x04
#define FLAG_MIP_SAFE_OUSTERS		0x10

struct B_RECT
{
	BYTE flag;	// 1-slayer, 2-vampire, 4-공통
	BYTE left;
	BYTE top;
	BYTE right;
	BYTE bottom;
};
#define SIZE_B_RECT sizeof(B_RECT)

struct P_RECT
{
	BYTE left;
	BYTE top;
	BYTE right;
	BYTE bottom;
};
#define SIZE_P_RECT sizeof(P_RECT)

class MPortal{
	public :
		enum
		{
			TYPE_NOMAL,
			TYPE_SLAYER_ONLY,
			TYPE_VAMPIRE_ONLY,
			TYPE_MULTI_PORTAL,
			TYPE_MULTI_PORTAL_HELIPORT,
			TYPE_GUILD_PORTAL,
			TYPE_BATTLE_PORTAL,
			TYPE_OUSTERS_ONLY,
			TYPE_CLIENT_SIEGE_POTAL_1 = 200,
			TYPE_CLIENT_SIEGE_POTAL_2 = 201,
			TYPE_CLIENT_SIEGE_POTAL_3 = 202,
			TYPE_CLIENT_SIEGE_POTAL_4 = 203,
			TYPE_CLIENT_ONLY = 255,
		};

		MPortal();
		MPortal(std::vector<WORD> ZoneID, P_RECT rect, BYTE Type);
		MPortal(std::vector<WORD> ZoneID, BYTE left, BYTE top, BYTE right, BYTE bottom, BYTE Type);
		~MPortal();

		//-------------------------------------------------------
		// Zone ID
		//-------------------------------------------------------
		void				SetZoneID(std::vector<WORD> id)			{ m_ZoneID = id; }
		std::vector<WORD>	GetZoneID() const			{ return m_ZoneID; }

		//-------------------------------------------------------
		// RECT
		//-------------------------------------------------------
		void		SetRect(BYTE left, BYTE top, BYTE right, BYTE bottom)
		{ m_Rect.left = left; m_Rect.top = top; m_Rect.right = right; m_Rect.bottom = bottom; }
		void		SetRect(P_RECT rect)		{ m_Rect = rect; }
		P_RECT		GetRect() const				{ return m_Rect; }

		BYTE		GetLeft() const				{ return m_Rect.left; }
		BYTE		GetTop() const				{ return m_Rect.top; }
		BYTE		GetRight() const			{ return m_Rect.right; }
		BYTE		GetBottom() const			{ return m_Rect.bottom; }

		void		SetLeft(BYTE left)			{ m_Rect.left = left; }
		void		SetTop(BYTE top)			{ m_Rect.top = top; }
		void		SetRight(BYTE right)		{ m_Rect.right = right; }
		void		SetBottom(BYTE bottom)		{ m_Rect.bottom = bottom; }

		//-------------------------------------------------------
		// Type & Data
		//-------------------------------------------------------
		BYTE		GetType() const				{ return m_Type; }

		void		SetType(BYTE Type)			{ m_Type = Type; }

		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		void		SaveToFile(std::ofstream& file);
		void		LoadFromFile(std::ifstream& file);

	protected :		
		
		// Portal의 종류
		std::vector<WORD>		m_ZoneID;		// 이동하게 되는 Zone ID
		P_RECT		m_Rect;
		BYTE		m_Type;
};

#endif
