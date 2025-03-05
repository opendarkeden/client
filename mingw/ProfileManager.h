//----------------------------------------------------------------------
// ProfileManager.h
//----------------------------------------------------------------------
// 캐릭터들의 얼굴 정보를 관리한다.
//
// 실제로 관리되는건.. 캐릭얼굴 화일이 있다/없다/요청해라.. 정도이고,
// 있을 경우.. 화일 이름을 읽어서 외부에서 잘~ 사용하면 된다.
//
// 프로그램이 실행될때 기존에건 다 지울 예정이므로.. 관리는 별로 필요없다.
//
//
// [화일 관리 방법]
//
// 사용자가 *.bmp를 profile에 넣어둔다고 하고..
// 어떤 캐릭의 profile이 필요하면 '캐릭터.bmp'가 있을때
// '캐릭터.spr'을 생성해서 profile에 넣어두고.. 사용하면 된다.
//
// 지울때는 spr만 다 지우면 된다.
//
// Profile.Get(name)에서 
//     내 client에 name.spr이 없다면..
//        다른 client에 요청을 해야한다.
//
//----------------------------------------------------------------------

#ifndef __PROFILE_MANAGER_H__
#define __PROFILE_MANAGER_H__


#pragma warning(disable:4786)

#include <map>
#include <string>

//----------------------------------------------------------------------
// ProfileManager
//----------------------------------------------------------------------
class ProfileManager {
	public :
		enum HAS_PROFILE
		{

		};

	public :
		typedef std::map<std::string, std::string>		PROFILE_MAP;

		typedef std::map<std::string, int>				REQUIRE_MAP;
		
	public :
		ProfileManager();
		~ProfileManager();

		//-------------------------------------------------------------
		// Delete / Init Profiles - Profile 디렉토리 관리
		//-------------------------------------------------------------
		static void		DeleteProfiles();
		static void		InitProfiles();		

		//-------------------------------------------------------------
		// Release
		//-------------------------------------------------------------
		void				Release();		

		//-------------------------------------------------------------
		// Add / Remove Profile
		//-------------------------------------------------------------
		bool			HasProfile(const char* pName) const;
		void			RequestProfile(const char* pName);
		void			AddProfile(const char* pName, const char* pFilename);		
		bool			RemoveProfile(const char* pName);

		//-------------------------------------------------------------
		// 상대방이 아예 Profile이 없는 경우
		//-------------------------------------------------------------
		bool			HasProfileNULL(const char* pName) const;
		void			AddProfileNULL(const char* pName);

		//-------------------------------------------------------------
		// Get
		//-------------------------------------------------------------
		const char*		GetFilename(const char* pName) const;


		//-------------------------------------------------------------
		// Require
		//-------------------------------------------------------------
		void			AddRequire(const char* pName);
		bool			HasRequire(const char* pName) const;
		bool			RemoveRequire(const char* pName);
		void			ReleaseRequire();

		void			Update();
		
	protected :
		//----------------------------------------------------------------------
		// Lock / Unlock
		//----------------------------------------------------------------------
		void		Lock()					{ EnterCriticalSection(&m_Lock); }
		void		Unlock()				{ LeaveCriticalSection(&m_Lock); }

	private :
		PROFILE_MAP		m_Profiles;

		REQUIRE_MAP		m_Requires;

		CRITICAL_SECTION		m_Lock;
};

extern ProfileManager*		g_pProfileManager;


#endif



