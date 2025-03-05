//----------------------------------------------------------------------
// MViewDef.h
//----------------------------------------------------------------------
//
// View에 관련된 정의.
//
//----------------------------------------------------------------------

#ifndef	__MVIEWDEF_H__
#define	__MVIEWDEF_H__


// 전시회용은 FullScreen이다.
#ifdef __EXPO_CLIENT__
	#define	__FULLSCREEN_MODE__
#endif

//----------------------------------------------------------------------
//
// defines
//
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// 화면 크기에 대한 정보
//----------------------------------------------------------------------
#define	SURFACE_WIDTH			800
#define	SURFACE_HEIGHT			600


//----------------------------------------------------------------------
// 기본 바닥 Tile의 크기
//----------------------------------------------------------------------
#define	TILE_X					48	//64
#define	TILE_Y					24	//32
#define	TILE_X_HALF				24	//32
#define	TILE_Y_HALF				12	//16

//----------------------------------------------------------------------
// 8방향에 따른 기준이 되는 기울기 : 가로/세로 비율과 관련
//----------------------------------------------------------------------
#define	BASIS_DIRECTION_LOW			0.35
#define	BASIS_DIRECTION_HIGH		3.0


//----------------------------------------------------------------------
// ScreenLight 크기 - 3D시야처리에서 사용하는 값
//----------------------------------------------------------------------
#define	SCREENLIGHT_WIDTH		64
#define	SCREENLIGHT_HEIGHT		64


#ifdef __FULLSCREEN_MODE__

	//----------------------------------------------------------------------
	// 한 화면에 출력되는 Sector의 개수?
	//----------------------------------------------------------------------
	#define	SECTOR_SKIP_LEFT		-1
	#define	SECTOR_SKIP_UP			-1
	#define	SECTOR_WIDTH			16	//16
	#define	SECTOR_HEIGHT			25

	//----------------------------------------------------------------------
	// Player의 시야 최대 범위 == 화면 상의 Tile범위
	//
	// 가로 : 9 + 1 + 9
	// 세로 : 13 + 1 + 13
	//
	// 1은 캐릭터의 좌표 중심이다.
	//----------------------------------------------------------------------
	#define	SECTOR_WIDTH_HALF			9
	#define	SECTOR_HEIGHT_HALF			13

	// 캐릭터를 화면 중심에 세우기 위한 좌표
	#define	SECTOR_SKIP_PLAYER_LEFT		-8
	#define	SECTOR_SKIP_PLAYER_UP		-12	// -SECTOR_HEIGHT/2

	//----------------------------------------------------------------------
	// 실제 게임 화면
	//----------------------------------------------------------------------
	#define	CLIPSURFACE_WIDTH		800
	#define	CLIPSURFACE_HEIGHT		600

	//----------------------------------------------------------------------
	//
	// Tile Surface에 대한 정보
	//
	//----------------------------------------------------------------------
	#define	TILESURFACE_SECTOR_EDGE				3		// 여유 공간 Sector개수

	#define	TILESURFACE_SECTOR_WIDTH			22		// 3+16+3가로 여백
	#define	TILESURFACE_SECTOR_HEIGHT			31		// 3+25+3세로 여백
	#define	TILESURFACE_SECTOR_OUTLINE_LEFT		3		// 왼쪽 경계
	#define	TILESURFACE_SECTOR_OUTLINE_RIGHT	19		// 3+16오른쪽 경계
	#define	TILESURFACE_SECTOR_OUTLINE_UP		3		// 위쪽 경계
	#define	TILESURFACE_SECTOR_OUTLINE_DOWN		28		// 3+25아래쪽 경계

	#define	TILESURFACE_WIDTH					1056	//1152	// TileSurface의 가로 크기
	#define	TILESURFACE_HEIGHT					744		//768		// TileSurface의 세로 크기
	#define	TILESURFACE_OUTLINE_LEFT			144		// 왼쪽 경계
	#define	TILESURFACE_OUTLINE_RIGHT			912		// 오른쪽 경계
	#define	TILESURFACE_OUTLINE_UP				72		// 위쪽 경계
	#define	TILESURFACE_OUTLINE_DOWN			672		//672		// 아래쪽 경계

#else

	//----------------------------------------------------------------------
	// 한 화면에 출력되는 Sector의 개수?
	//----------------------------------------------------------------------
	#define	SECTOR_SKIP_LEFT		-1
	#define	SECTOR_SKIP_UP			-1
	#define	SECTOR_WIDTH			16	//16
	#define	SECTOR_HEIGHT			25	//18 - interface때문에 500만 넘으면 된다.

	//----------------------------------------------------------------------
	// Player의 시야 최대 범위 == 화면 상의 Tile범위
	//
	// 가로 : 9 + 1 + 9
	// 세로 : 10 + 1 + 10
	//
	// 1은 캐릭터의 좌표 중심이다.
	//----------------------------------------------------------------------
	#define	SECTOR_WIDTH_HALF			9
	#define	SECTOR_HEIGHT_HALF			12

	// 캐릭터를 화면 중심에 세우기 위한 좌표
	#define	SECTOR_SKIP_PLAYER_LEFT		-8
	#define	SECTOR_SKIP_PLAYER_UP		-13

	//----------------------------------------------------------------------
	// 실제 게임 화면
	//----------------------------------------------------------------------
	#define	CLIPSURFACE_WIDTH		800
	#define	CLIPSURFACE_HEIGHT		600

	//----------------------------------------------------------------------
	//
	// Tile Surface에 대한 정보
	//
	//----------------------------------------------------------------------
	#define	TILESURFACE_SECTOR_EDGE				3		// 여유 공간 Sector개수

	#define	TILESURFACE_SECTOR_WIDTH			22		// 3+16+3가로 여백
	#define	TILESURFACE_SECTOR_HEIGHT			31		// 3+25+3세로 여백
	#define	TILESURFACE_SECTOR_OUTLINE_LEFT		3		// 왼쪽 경계
	#define	TILESURFACE_SECTOR_OUTLINE_RIGHT	19		// 3+16오른쪽 경계
	#define	TILESURFACE_SECTOR_OUTLINE_UP		3		// 위쪽 경계
	#define	TILESURFACE_SECTOR_OUTLINE_DOWN		28		// 3+25아래쪽 경계

	#define	TILESURFACE_WIDTH					1056	//1152	// TileSurface의 가로 크기
	#define	TILESURFACE_HEIGHT					744		//768		// TileSurface의 세로 크기
	#define	TILESURFACE_OUTLINE_LEFT			144		// 왼쪽 경계
	#define	TILESURFACE_OUTLINE_RIGHT			912		// 오른쪽 경계
	#define	TILESURFACE_OUTLINE_UP				72		// 위쪽 경계
	#define	TILESURFACE_OUTLINE_DOWN			672		//672		// 아래쪽 경계
#endif

//----------------------------------------------------------------------
// Tile Surface를 다시 그릴때 Flag
//----------------------------------------------------------------------
#define	FLAG_TILESURFACE_LACK_LEFT			0x01	// 왼쪽 부족
#define	FLAG_TILESURFACE_LACK_RIGHT			0x02	// 오른쪽 부족
#define	FLAG_TILESURFACE_LACK_UP			0x04	// 위쪽 부족
#define	FLAG_TILESURFACE_LACK_DOWN			0x08	// 아래쪽 부족


#endif