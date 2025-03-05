// GAME1024.cpp: implementation of the GAME1024 class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "Client_PCH.h"
#include "GAME1024.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GAME1024::GAME1024()
{

}

GAME1024::~GAME1024()
{

}

void GAME1024::init1024()
{
	SECTOR_WIDTH           = 21;
	SECTOR_HEIGHT          = 32;
	SECTOR_WIDTH_HALF      = 11;
	SECTOR_HEIGHT_HALF     = 17;
	SECTOR_SKIP_PLAYER_LEFT= -10;
	SECTOR_SKIP_PLAYER_UP  = -16;

	TILESURFACE_SECTOR_WIDTH         =27;
	TILESURFACE_SECTOR_HEIGHT        =37;
	TILESURFACE_SECTOR_OUTLINE_RIGHT =24;
	TILESURFACE_SECTOR_OUTLINE_DOWN  =34;
	TILESURFACE_WIDTH                =1296;
	TILESURFACE_HEIGHT               =888;
	TILESURFACE_OUTLINE_RIGHT        =1152;
	TILESURFACE_OUTLINE_DOWN         =816;
}

void GAME1024::inti800()
{
	SECTOR_WIDTH           =16 ;
	SECTOR_HEIGHT          =25 ;
	SECTOR_WIDTH_HALF      =9 ;
	SECTOR_HEIGHT_HALF     =13 ;
	SECTOR_SKIP_PLAYER_LEFT=-8 ;
	SECTOR_SKIP_PLAYER_UP  =-12 ;

	TILESURFACE_SECTOR_WIDTH         =22;
	TILESURFACE_SECTOR_HEIGHT        =31;
	TILESURFACE_SECTOR_OUTLINE_RIGHT =19;
	TILESURFACE_SECTOR_OUTLINE_DOWN  =28;
	TILESURFACE_WIDTH                =1056;
	TILESURFACE_HEIGHT               =744;
	TILESURFACE_OUTLINE_RIGHT        =912;
	TILESURFACE_OUTLINE_DOWN         =672;
}
