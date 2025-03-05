//----------------------------------------------------------------------
// MParty.h
//----------------------------------------------------------------------
// creatureID나 creatureName이 둘 다 key가 될 수 있다.
// 파티원은 최대수는 6.. 언젠가 늘어날 수도 있겠지만..
// key값 두개인거도 애매하고 max가 6이므로 map은 안쓰기로.. ㅋㅋ..
//
// 누가 party에 있다는거만 알면 된다.
// 그리고.. 근처에 있는 경우(g_pZone에 있는 경우) m_ID를 통해서 
// HP나 기타 정보들을 출력해주면 되겠지.
//
// GetMemberXXX( n )의 n은 array의 index이다.
//----------------------------------------------------------------------

#ifndef __MParty_H__
#define __MParty_H__

#pragma warning(disable:4786)

#include "MString.h"
#include "MTypeDef.h"

class PARTY_INFO
{
public :
	MString			Name;		// 이름
	TYPE_OBJECTID	ID;			// object ID
	bool			bMale;
	BYTE			hairStyle;

	// guildID
	MString			guildName;
	WORD			guildID;
	
	// 현재 있는 곳의 정보
	int				zoneID;
	BYTE			zoneX;
	BYTE			zoneY;

	// 에노지..
	WORD			MaxHP;
	WORD			HP;
	
	// IP
	std::string		IP;
	bool			bInSight;

public :
	PARTY_INFO();
};

class MParty {
	public :
		typedef std::vector<PARTY_INFO*>	PARTY_VECTOR;

	public :
		MParty();
		~MParty();

		void		Release();

		//--------------------------------------------------------
		// SetJoinTime
		//--------------------------------------------------------
		DWORD		GetJoinTime() const		{ return m_JoinTime; }		
		void		SetJoinTime(DWORD time)	{ m_JoinTime = time; }
		void		SetJoinTime();		
		bool		IsKickAvailableTime() const;

		//--------------------------------------------------------
		// Get Size - 현재 파티원의 수 (나를 제외한)
		//--------------------------------------------------------
		int			GetSize() const		{ return m_pInfo.size(); }

		//--------------------------------------------------------
		// Add / Remove
		//--------------------------------------------------------
		bool		AddMember(PARTY_INFO* pInfo);
		bool		RemoveMember(const char* pName);
		bool		RemoveMember(int creatureID);	
		void		UnSetPlayerParty() const;

		//--------------------------------------------------------
		// Accept/Refuse Invite
		//--------------------------------------------------------
		bool		IsAcceptInvite() const		{ return m_bAccept; }
		void		AcceptInvite()				{ m_bAccept = true; }
		void		RefuseInvite()				{ m_bAccept = false; }

		//--------------------------------------------------------
		// Get ID / Name
		//--------------------------------------------------------
		PARTY_INFO*	GetMemberInfo(int n) const;	
		PARTY_INFO*	GetMemberInfo(const char* pName) const;	
		PARTY_INFO* GetMemberInfoByIP(const char* pIP) const;

		bool		HasMember(const char* pName) const;

	protected :
		int							m_Size;			// 파티원 수
		PARTY_VECTOR				m_pInfo;		// party 정보

		bool						m_bAccept;		// 초대를 받아들이는가?

		DWORD						m_JoinTime;
};

extern MParty*	g_pParty;

#endif

