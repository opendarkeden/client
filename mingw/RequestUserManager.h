//----------------------------------------------------------------------
// RequestUserManager.h
//----------------------------------------------------------------------
// 다른 Client의 Name, IP 등의 정보를 갖는다.
//----------------------------------------------------------------------

#ifndef __REQUEST_USER_MANAGER_H__
#define __REQUEST_USER_MANAGER_H__


#pragma warning(disable:4786)

#include <Windows.h>
#include <map>
#include <string>
#include <list>

//----------------------------------------------------------------------
// RequestUserInfo
//----------------------------------------------------------------------
// 다른 Client에 대한 정보
//----------------------------------------------------------------------
class RequestUserInfo {
	public :
		enum USER_STATUS
		{
			USER_STATUS_NORMAL,			// 정상 상태
			USER_STATUS_UNABLE			// 접속 불가
		};

	public :
		RequestUserInfo()
		{
			Status = USER_STATUS_NORMAL;
			UDPPort = 0;
			TCPPort = 0;
		}

		void			SetStatusNormal()		{ Status = USER_STATUS_NORMAL; }
		void			SetStatusUnable()		{ Status = USER_STATUS_UNABLE; }
		bool			IsStatusUnable() const	{ return Status == USER_STATUS_UNABLE; }

	public :
		std::string		Name;		
		std::string		IP;
		int				UDPPort;		// client communication UDP port
		int				TCPPort;		// request server TCP port
		USER_STATUS		Status;
};

//----------------------------------------------------------------------
// RequestUserManager
//----------------------------------------------------------------------
class RequestUserManager {
	public :
		enum REQUESTING_FOR
		{
			REQUESTING_FOR_NULL,
			REQUESTING_FOR_WHISPER,
			REQUESTING_FOR_PROFILE,
			
			REQUESTING_FOR_CHARACTER_INFO,
		};

	public :
		typedef std::map<std::string, RequestUserInfo*>		REQUEST_USER_MAP;
		typedef std::map<std::string, REQUESTING_FOR>		REQUESTING_USER_MAP;
		typedef std::list<std::string>						REMOVE_USER_LIST;

	public :
		RequestUserManager();
		~RequestUserManager();

		//-------------------------------------------------------------
		// Release
		//-------------------------------------------------------------
		void				Release();

		//-------------------------------------------------------------
		// Add / Remove User
		//-------------------------------------------------------------
		bool				HasRequestUser(const char* pName) const;
		void				AddRequestUser(const char* pName, const char* pIP, int UDPPort=0);
		bool				RemoveRequestUser(const char* pName);

		//-------------------------------------------------------------
		// Add / Remove User
		//-------------------------------------------------------------
		bool				HasRequestingUser(const char* pName) const;
		void				AddRequestingUser(const char* pName, REQUESTING_FOR rf);
		bool				RemoveRequestingUser(const char* pName);

		//-------------------------------------------------------------
		// RemoveUser - 다른 thread에서.. 냠..
		//-------------------------------------------------------------
		void				RemoveRequestUserLater(const char* pName);

		//-------------------------------------------------------------
		// Get
		//-------------------------------------------------------------
		RequestUserInfo*	GetUserInfo(const char* pName) const;
		REQUESTING_FOR		GetRequestingFor(const char* pName) const;

		//-------------------------------------------------------------
		// Update
		//-------------------------------------------------------------
		void				Update();
		
	protected :
		//----------------------------------------------------------------------
		// Lock / Unlock
		//----------------------------------------------------------------------
		void		Lock()					{ EnterCriticalSection(&m_Lock); }
		void		Unlock()				{ LeaveCriticalSection(&m_Lock); }

	private :
		CRITICAL_SECTION		m_Lock;

		REQUEST_USER_MAP		m_RequestUsers;		// IP를 알고 있는 애덜

		REQUESTING_USER_MAP		m_RequestingUsers;	// 서버에 IP를 요청중인 애덜

		REMOVE_USER_LIST		m_RemoveUsers;
};

extern RequestUserManager*		g_pRequestUserManager;


#endif

