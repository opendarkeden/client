//----------------------------------------------------------------------
// Profiler.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "Profiler.h"
#include <MMSystem.h>


//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
Profiler*	g_pProfiler = NULL;

//----------------------------------------------------------------------
//
//							ProfilerInfo
//
//----------------------------------------------------------------------
ProfilerInfo::ProfilerInfo()
{
	Clear();
}

ProfilerInfo::~ProfilerInfo()
{
}

//----------------------------------------------------------------------
// Clear
//----------------------------------------------------------------------
void		
ProfilerInfo::Clear()
{
	m_StartTime = 0;
	m_TotalTime = 0;
	m_Times = 0;
}

//----------------------------------------------------------------------
// Begin
//----------------------------------------------------------------------
void		
ProfilerInfo::Begin()
{
	//if (m_StartTime!=0)
	{
		//End();

	}

	m_StartTime = timeGetTime();
}

//----------------------------------------------------------------------
// End
//----------------------------------------------------------------------
void		
ProfilerInfo::End()
{
	if (m_StartTime!=0)
	{
		DWORD endTime = timeGetTime();

		m_TotalTime += endTime - m_StartTime;
		m_Times ++;

		m_StartTime = 0;
	}
}

//----------------------------------------------------------------------
// Write To File
//----------------------------------------------------------------------
void		
ProfilerInfo::WriteToFile(ofstream& file) const
{	
	char str[256];
	sprintf(str, "%10d  %10d  %7.3f", m_Times, m_TotalTime, GetAverageTime());

	file << str;
}

//----------------------------------------------------------------------
//
//							Profiler
//
//----------------------------------------------------------------------
Profiler::Profiler()
{
}

Profiler::~Profiler()
{
	Release();
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void			
Profiler::Release()
{
	PROFILE_MAP::iterator iInfo = m_mapProfile.begin();

	while (iInfo != m_mapProfile.end())
	{
		delete iInfo->second;

		iInfo ++;
	}

	m_mapProfile.clear();
}

//----------------------------------------------------------------------
// Begin
//----------------------------------------------------------------------
void			
Profiler::Begin(const char* pName)
{
	std::string name = pName;

	PROFILE_MAP::iterator iInfo = m_mapProfile.find( name );

	if (iInfo != m_mapProfile.end())
	{
		iInfo->second->Begin();
	}
	else
	{
		// 없으면 생성해서 추가해준다.
		ProfilerInfo* pInfo = new ProfilerInfo;

		m_mapProfile[name] = pInfo;

		pInfo->Begin();
	}
}

//----------------------------------------------------------------------
// End
//----------------------------------------------------------------------
void			
Profiler::End(const char* pName)
{
	PROFILE_MAP::iterator iInfo = m_mapProfile.find( std::string(pName) );

	if (iInfo != m_mapProfile.end())
	{
		iInfo->second->End();
	}
}

//----------------------------------------------------------------------
// Has ProfileInfo
//----------------------------------------------------------------------
bool			
Profiler::HasProfileInfo(const char* pName) const
{
	PROFILE_MAP::const_iterator iInfo = m_mapProfile.find( std::string(pName) );

	if (iInfo != m_mapProfile.end())
	{
		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// Get Times
//----------------------------------------------------------------------
DWORD			
Profiler::GetTimes(const char* pName) const
{
	PROFILE_MAP::const_iterator iInfo = m_mapProfile.find( std::string(pName) );

	if (iInfo != m_mapProfile.end())
	{
		return iInfo->second->GetTimes();
	}

	return 0;
}

//----------------------------------------------------------------------
// Get TotalTime
//----------------------------------------------------------------------
DWORD			
Profiler::GetTotalTime(const char* pName) const
{
	PROFILE_MAP::const_iterator iInfo = m_mapProfile.find( std::string(pName) );

	if (iInfo != m_mapProfile.end())
	{
		return iInfo->second->GetTotalTime();
	}

	return 0;
}

//----------------------------------------------------------------------
// Get AverageTime
//----------------------------------------------------------------------
float
Profiler::GetAverageTime(const char* pName) const
{
	PROFILE_MAP::const_iterator iInfo = m_mapProfile.find( std::string(pName) );

	if (iInfo != m_mapProfile.end())
	{
		return iInfo->second->GetAverageTime();
	}

	return 0;
}

//----------------------------------------------------------------------
// Write To File
//----------------------------------------------------------------------
void		
Profiler::WriteToFile(const char* pFilename, bool bAppend) const
{
	DWORD flag = (bAppend? ios::app : 0);

	ofstream file(pFilename, flag);

	PROFILE_MAP::const_iterator iInfo = m_mapProfile.begin();

	char str[256];

	sprintf(str, "%-20s ", "ProfilerName");
	file << "\n\n"
		<< str
		<< " LoopTimes   TotalTime   AverageTime"
		<< "\n\n";

	while (iInfo != m_mapProfile.end())
	{
		sprintf(str, "%-20s ", (const char*)iInfo->first.c_str());

		file << str;

		iInfo->second->WriteToFile( file );

		file << "\n";

		iInfo ++;
	}

	file.close();
}