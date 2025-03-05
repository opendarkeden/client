//////////////////////////////////////////////////////////////////////////////
// Filename    : SystemTypes.h
// Written By  : Reiot
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SYSTEM_TYPES_H__
#define __SYSTEM_TYPES_H__
//#define __WINDOWS__
	#pragma warning ( disable : 4786 )
	#include <Windows.h>
#if defined(__WINDOWS__)
	// 4786 - STL의 심볼명의 길이가 255를 넘어갈 때
	#pragma warning ( disable : 4786 )
	#include <Windows.h>
#elif defined(__LINUX__)
	#include <sys/types.h>
#endif
// #endif
 
#include <iostream>
#include <fstream>
#include <string>

//using namespace std;
using std::string;

//////////////////////////////////////////////////////////////////////////////
// built-in type redefinition
//////////////////////////////////////////////////////////////////////////////
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

#if defined(__LINUX__) || defined(__WIN_CONSOLE__)
	typedef unsigned char  BYTE;
	typedef unsigned short WORD;
	typedef unsigned long DWORD;
	typedef unsigned long long ulonglong;
#else
	typedef unsigned __int64 ulonglong;
#endif

#if defined(__LINUX__)
	const char separatorChar = '/';
	const std::string separator = "/";
#elif defined(__WINDOWS__)
	const char separatorChar = '\\';
	const std::string separator = "\\";
#endif


//////////////////////////////////////////////////////////////////////////////
// built-in type size
//////////////////////////////////////////////////////////////////////////////
const unsigned int szbool   = sizeof(bool);
const unsigned int szchar   = sizeof(char);
const unsigned int szshort  = sizeof(short);
const unsigned int szint    = sizeof(int);
const unsigned int szlong   = sizeof(long);
const unsigned int szuchar  = sizeof(unsigned char);
const unsigned int szushort = sizeof(unsigned short);
const unsigned int szuint   = sizeof(unsigned int);
const unsigned int szulong  = sizeof(unsigned long);
const unsigned int szBYTE   = sizeof(BYTE);
const unsigned int szWORD   = sizeof(WORD);
const unsigned int szDWORD  = sizeof(DWORD);


//////////////////////////////////////////////////////////////////////////////
// ServerGroupInfo
//////////////////////////////////////////////////////////////////////////////
typedef BYTE ServerGroupID_t;
const uint szServerGroupID = sizeof(ServerGroupID_t);


//////////////////////////////////////////////////////////////////////////////
// SubServerInfo
//////////////////////////////////////////////////////////////////////////////
typedef WORD ServerID_t;
const uint szServerID = sizeof(ServerID_t);

typedef WORD UserNum_t;
const uint szUserNum = sizeof(UserNum_t);

//////////////////////////////////////////////////////////////////////////////
// SubServerInfo
//////////////////////////////////////////////////////////////////////////////
enum ServerStatus 
{
	SERVER_FREE,
	SERVER_NORMAL,
	SERVER_BUSY,
	SERVER_VERY_BUSY,
	SERVER_FULL,
	SERVER_DOWN
};

enum WorldStatus
{
	WORLD_OPEN,
	WORLD_CLOSE	
};

typedef unsigned long IP_t;
const uint szIP = sizeof(IP_t);

typedef BYTE WorldID_t;
const uint szWorldID = sizeof(WorldID_t);

#endif
