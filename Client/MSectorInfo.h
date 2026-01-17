//----------------------------------------------------------------------
// MSectorInfo.h
//----------------------------------------------------------------------
// 안전지대 표시.. 등등..
// 자주 바뀔거 같아서.. map화일과 분리한다.
//----------------------------------------------------------------------


#ifndef	__MSECTORINFO_H__
#define	__MSECTORINFO_H__

#pragma warning(disable:4786)


#ifndef __GAME_CLIENT__
	#include <vector>

	extern std::vector<BYTE> gi_portal_x;
	extern std::vector<BYTE> gi_portal_y;
	extern std::vector<WORD> gi_portal_ZoneID;
	extern BYTE gi_portal_Type;
#endif

//----------------------------------------------------------------------
// Flag
//----------------------------------------------------------------------
#define FLAG_SECTOR_SAFETY					0x01	// safety zone

#ifndef __GAME_CLIENT__
	#define FLAG_SECTOR_PORTAL					0x02	// portal zone
#endif

class MSectorInfo {
	public :
		MSectorInfo();
		~MSectorInfo();

		void operator = (const MSectorInfo& s)
		{
			m_fProperty = s.m_fProperty;
		}


		//------------------------------------------------
		//
		//                  File I/O
		//
		//------------------------------------------------
		void	SaveToFile(std::ofstream& file);
		void	LoadFromFile(std::ifstream& file);

		//------------------------------------------------
		// Safety
		//------------------------------------------------
		BYTE	IsSafety() const		{ return m_fProperty & FLAG_SECTOR_SAFETY; }
		void	SetSafety()				{ m_fProperty |= FLAG_SECTOR_SAFETY; }
		void	UnSetSafety()			{ m_fProperty &= ~FLAG_SECTOR_SAFETY; }

		//------------------------------------------------
		//
		//                      Portal
		//
		//------------------------------------------------
		#ifndef __GAME_CLIENT__			
			BYTE			IsPortal() const	{ return m_fProperty & FLAG_SECTOR_PORTAL; }
			void			SetPortal()
			{
				m_fProperty |= FLAG_SECTOR_PORTAL;

					m_Portal_ZoneID = gi_portal_ZoneID;
					m_Portal_X = gi_portal_x;
					m_Portal_Y = gi_portal_y;
					m_Portal_Type = gi_portal_Type;			
			}
			void			UnSetPortal()		{ m_fProperty &= ~FLAG_SECTOR_PORTAL; }


			// get functions
			std::vector<WORD>			GetPortalZoneID() const		{ return m_Portal_ZoneID; }
			std::vector<BYTE>			GetPortalX() const			{ return m_Portal_X; }
			std::vector<BYTE>			GetPortalY() const			{ return m_Portal_Y; }
			BYTE						GetPortalType() const		{ return m_Portal_Type; }
		
			// set functions
			void			SetPortalZoneID(std::vector<WORD> ID)		{ m_Portal_ZoneID = ID; }
			void			SetPortalX(std::vector<BYTE> x)				{ m_Portal_X = x; }
			void			SetPortalY(std::vector<BYTE> y)				{ m_Portal_Y = y; }
			void			SetPortalType(BYTE Type)		{ m_Portal_Type = Type; }
		#endif

	protected :
		// 정보 Flag
		BYTE					m_fProperty;

		#ifndef __GAME_CLIENT__
			// 에디터용 변수들 클라이언트용 파일에는 들어가지 않음.
			std::vector<WORD>					m_Portal_ZoneID;
			std::vector<BYTE>					m_Portal_X;
			std::vector<BYTE>					m_Portal_Y;
			BYTE								m_Portal_Type;
		#endif
};

#endif


