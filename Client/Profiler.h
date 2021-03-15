//----------------------------------------------------------------------
// Profiler.h
//----------------------------------------------------------------------
// 프로파일링... 냠..
//
//
// < 필요한 정보 >
//
// 이름						: 내용이기도 하고, ID이다.
// StartTime				: 이번 loop의 시작 시간
// TotalTime				: 전체 진행 시간
// Times					: 몇 번 반복을 했는가?
// 
//----------------------------------------------------------------------

#ifndef __PROFILER_H__
#define __PROFILER_H__

#pragma warning(disable:4786)

#include <Windows.h>
#include <map>
#include <string>
#include <fstream>
#include "DebugInfo.h"

//----------------------------------------------------------------------
// ProfilerInfo
//----------------------------------------------------------------------
class ProfilerInfo
{
	public :
		ProfilerInfo();
		~ProfilerInfo();

		void		Clear();

		//---------------------------------------------------------------
		// Begin / End
		//---------------------------------------------------------------
		void		Begin();
		void		End();

		//---------------------------------------------------------------
		// Get
		//---------------------------------------------------------------
		DWORD		GetTimes() const		{ return m_Times; }
		DWORD		GetTotalTime() const	{ return m_TotalTime; }
		float		GetAverageTime() const	{ return (float)m_TotalTime / m_Times; }

		//---------------------------------------------------------------
		// File Out
		//---------------------------------------------------------------
		void		WriteToFile(ofstream& file) const;

	private :
		DWORD		m_StartTime;		// 이번 loop 시작 시간
		DWORD		m_TotalTime;		// 전체 진행 시간
		DWORD		m_Times;			// 몇 번 반복 했는가?
};

//----------------------------------------------------------------------
// Profiler
//----------------------------------------------------------------------
class Profiler {
	public :
		typedef std::map< std::string, ProfilerInfo* >	PROFILE_MAP;

	public :
		Profiler();
		~Profiler();

		void			Release();

		//---------------------------------------------------------------
		// Begin / End
		//---------------------------------------------------------------
		void			Begin(const char* pName);
		void			End(const char* pName);

		//---------------------------------------------------------------
		// Get
		//---------------------------------------------------------------
		bool			HasProfileInfo(const char* pName) const;
		DWORD			GetTimes(const char* pName) const;
		DWORD			GetTotalTime(const char* pName) const;
		float			GetAverageTime(const char* pName) const;

		//---------------------------------------------------------------
		// Iterating
		//---------------------------------------------------------------
		int							GetNumber() const	{ return m_mapProfile.size(); }
		PROFILE_MAP::const_iterator GetBegin() const	{ return m_mapProfile.begin(); }

		//---------------------------------------------------------------
		// File Out
		//---------------------------------------------------------------
		void		WriteToFile(const char* pFilename, bool bAppend=false) const;

	protected :
		PROFILE_MAP		m_mapProfile;
};

extern Profiler* g_pProfiler;

#ifdef OUTPUT_DEBUG
	#define __BEGIN_PROFILE(name)	if (g_pProfiler!=NULL) g_pProfiler->Begin(name);
	#define __END_PROFILE(name)		if (g_pProfiler!=NULL) g_pProfiler->End(name);	
#else
	#define __BEGIN_PROFILE(name)	((void)0);
	#define __END_PROFILE(name)		((void)0);
#endif

#endif

