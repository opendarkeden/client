////////////////////////////////////////////////////////////////////////////////
//	created:	2004/12/22
//	file base:	client_pch.h
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable:4290)
#pragma warning(disable:4018)
#pragma warning(disable:4244)
#pragma warning(disable:4018)
#pragma warning(disable:4786)


#pragma warning(push)

#include <string>
#include <assert.h>
#include <vector>
#include <map>
#include <list>
#include <deque>
#include <bitset>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <MMSystem.h>
#include <Digitalv.h>
#include <DDraw.h>
#include <stdio.h>
#include <stdarg.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h> 
//#include "GAME1024.h"
#pragma warning(pop)

using std::string;
using std::vector;
using std::map;
using std::list;
using std::deque;
using std::bitset;

extern BOOL g_MyFull;
extern RECT g_GameRect;
//extern GAME1024 g_NewMode;
extern	LONG g_SECTOR_WIDTH;
extern	LONG g_SECTOR_HEIGHT;
extern	LONG g_SECTOR_WIDTH_HALF;
extern	LONG g_SECTOR_HEIGHT_HALF;
extern	LONG g_SECTOR_SKIP_PLAYER_LEFT;
extern	LONG g_SECTOR_SKIP_PLAYER_UP;

extern	LONG g_TILESURFACE_SECTOR_WIDTH;
extern	LONG g_TILESURFACE_SECTOR_HEIGHT;
extern	LONG g_TILESURFACE_SECTOR_OUTLINE_RIGHT;
extern	LONG g_TILESURFACE_SECTOR_OUTLINE_DOWN;
extern	LONG g_TILESURFACE_WIDTH;
extern	LONG g_TILESURFACE_HEIGHT;
extern	LONG g_TILESURFACE_OUTLINE_RIGHT;
extern	LONG g_TILESURFACE_OUTLINE_DOWN;
extern	LONG g_TILE_X_HALF;
extern	LONG g_TILE_Y_HALF;
