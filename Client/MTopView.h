//----------------------------------------------------------------------
// MTopView.h
//----------------------------------------------------------------------
//
// 한 Zone의 특정 위치를 출력해주는 class이다.
//
//----------------------------------------------------------------------
//
// DrawTile()     : 현재Surface의 Tile만 그린다.
//                  m_pTileSurface에 그려둔다.
//
// DrawZone()	  : 이전 Tile을 이용해서 현재Surface에 Tile을 그리고
//                  Zone의 모든 것들을 표현해준다.
//
// * 현재 Tile이 갑자기 변화할때(Zone이동 시, Teleport시)는 
//   SetFirstDraw()를 사용해서 m_bFirstTileDraw = true로 설정하면 
//   Draw()에서 한번은 DrawTile()을 선택하고 
//                     다음부터는 DrawZone()을 선택한다.
//
//---------------------------------------------------------------------- 
//
// - 이전의 Tile을 저장해두는 Surface는 실제 화면 크기보다 크다.
//   (800,600)이 실제화면이므로,
//   상하좌우로 3 Tile씩  (1152,768)으로 Surface를 생성한다.
//
// - 이렇게 해두면 이전의 Tile Surface에서 현재 필요한 영역만
//   읽어올때, 읽어오는 범위가 이전의 Tile Surface에 속하면 
//   다시 그려줘야 하는 부분이 없을 수 있다.
//   또한, 이런 경우 Tile Surface를 갱신할 필요도 없다.
// 
// - 단점이라면, 다시 그려줄 때 그려줘야하는 부분이 조금~ 더 크지만
//   평균적인 속도가 향상되므로 그리 문제는 없을 것이다.
//
// * 테스트후 크기를 조절해봐야지...
//
//---------------------------------------------------------------------- 
//
// * SpriteSet / FrameSet의 정보를 읽어와야 한다.
//
// [ MapView : Filename.MV ]   - View에서 Load한다.
//   Tile SpriteSet
//   Obstacle SpriteSet 
//   Obstacle FrameSet
//   ImageObject SpriteSet
//
//---------------------------------------------------------------------- 
//
// ImageObject Map...
//
// 최초에... 모두 Tile Scan
// 그리고.. 이전 Tile과의 변화값 만큼만 Tile Scan
//
//---------------------------------------------------------------------- 

#ifndef	__MTOPVIEW_H__
#define	__MTOPVIEW_H__

#pragma warning(disable:4786)

#ifdef PLATFORM_WINDOWS
#include "DX3D.h"
#endif
#include "MViewDef.h"
#include <fstream>
#include <map>
#include <queue>

using namespace std;

//----------------------------------------------------------------------
//
// using class..
//
//----------------------------------------------------------------------
#include "MZone.h"
class MObject;
#include "MImageObject.h"
#include "MCreature.h"
class MMissile;
class MItem;
class MEffect;
#include "SP.h"
#include "FR.h"
//#include "2D.h"
#include "CSpriteTexturePartManager.h"
#include "CTexturePartManager.h"
#include "CShadowPartManager.h"
#include "DrawTextNode.h"
#include "DrawItemNameNode.h"
#include "COrderedList.h"
#include "MRequestMode.h"
#include "CIndexSpritePack.h"
#include "CFilter.h"
#include "CFilterPack.h"
#include "CSpriteOutlineManager.h"

// TileRenderer for unified tile rendering
#include "TileRenderer.h"
#include "MZoneTileProvider.h"


//class MZoneInfo;
class TextComparison {
	public :
		// 시간이 오래된걸 선택해야 한다.		
		// true : right를 선택한다.
		// false : left를 선택한다.
		bool operator () (DRAWTEXT_NODE * left, DRAWTEXT_NODE * right) const;
};

typedef void (*DrawCreatureExceptionProc)( MCreature* pCreature, int& action, int& frame, int& direction );

//----------------------------------------------------------------------
// 한 Surface에 특정한 Zone에 대한 그림을 그려주는 class
//----------------------------------------------------------------------
class MTopView : public MRequestMode {
	public :	
		// 출력에 이용할 ImageObject map
		typedef std::map<QWORD, MImageObject*>			IMAGEOBJECT_OUTPUT_MAP;

		// 출력에 이용할 Creature map
		typedef std::map<QWORD, MCreature*>				CREATURE_OUTPUT_MAP;

		// 출력에 이용할 Item map
		typedef std::map<QWORD, MItem*>					ITEM_OUTPUT_MAP;

		// 출력할 string을 기억해 놓을 곳..
		//typedef	std::list<DRAWTEXT_NODE*>				DRAWTEXT_LIST;
		typedef std::priority_queue<DRAWTEXT_NODE*, std::vector<DRAWTEXT_NODE*>, TextComparison>	DRAWTEXT_PQ;

		// item이름 출력
		typedef	std::list<DRAWITEMNAME_NODE*>			DRAWITEMNAME_LIST;

		// int ordered list
		typedef	COrderedList<int>						INT_ORDERED_LIST;

	public :
		MTopView();
		~MTopView();

		//------------------------------------------------------
		//
		// init
		//
		//------------------------------------------------------
		bool		Init();
		bool		InitChanges();	// 2D <--> 3D 바뀔때.. 음냐 - -;
		bool		IsInit() const		{ return m_bInit; }
		void		Release();
		void		SetSurface(CSpriteSurface*& pSurface);
		void		SetZone(MZone* pZone);
		void		RestoreSurface();
		void		ClearShadowManager();
		const FRAME_ARRAY&	GetCreatureFrameArray(int body, int action, int direction) const	{ return m_CreatureFPK[body][action][direction]; }
		const CIndexSprite*	GetCreatureSprite(int spriteID)		{ return &m_CreatureSPK[ spriteID ]; }

		//------------------------------------------------------
		//
		// Zone마다 Load해야 하는 것
		//
		//------------------------------------------------------		
		void		LoadMinimap(const char* filename);//, MZoneInfo* pZoneInfo=NULL);
		bool		LoadFromFileTileSPKLargeZone(ifstream & file);	// zone관련 spk load
		bool		LoadFromFileImageObjectSPKLargeZone(ifstream & file);	// zone관련 spk load
		bool		LoadFromFileTileSPKSmallZone(ifstream & file);	// zone관련 spk load
		bool		LoadFromFileImageObjectSPKSmallZone(ifstream & file);	// zone관련 spk load
		void		LoadFromFileCreatureSPK(int n);			// creature load		
// 		void		LoadFromFileAddonSPK(int frame, int action);		// addon load		
//		void		LoadFromFileCreatureActionSPK(int frame, int action);		// addon load		
		bool		LoadFromFileTileAndImageObjectSet(const CSpriteSetManager &TileSSM, const CSpriteSetManager &ImageObjectSSM);
		void		ReleaseCreatureSPK(int n);			// creature release
		void		ReleaseTileSPKLargeZone();
		void		ReleaseImageObjectSPKLargeZone();
		void		ReleaseTileSPKSmallZone();
		void		ReleaseImageObjectSPKSmallZone();
		//void		ReleaseAddonSPK(int frame, int action);
//		void		ReleaseCreatureActionSPK(int frame, int action);

		void		ReleaseUselessCreatureSPKExcept(const INT_ORDERED_LIST& listUse);
		void		AddMonsterSpriteTypes(const INT_ORDERED_LIST& liseUse);

//		void		StopLoadImageObjectSPK()	{ m_ImageObjectSPK.SetLoadingStop(); }
//		void		StopLoadTileSPK()			{ m_TileSPK.SetLoadingStop(); }

		// Tile을 모두 다시 그려줘야할때
		void		SetFirstDraw()			{ m_bFirstTileDraw = true; }
		
		//------------------------------------------------------
		//
		// Draw Functions
		//
		//------------------------------------------------------
		void		Draw(int firstPointX, int firstPointY);

		// Test용 도움말
		void		DrawDebugInfo(void* pSurface);

		void		DrawTitleEffect(POINT* pPoint);

		void		DrawBloodBibleEffect_InGear(POINT* pPoint);
		
		//------------------------------------------------------
		//
		// Fade In/out
		//
		//------------------------------------------------------
		bool			IsFade() const			{ return m_bFade; }
		void			SetFadeStart(char start, char end, char step, BYTE r=0, BYTE g=0, BYTE b=0, WORD delay = 0);
		void			SetFadeEnd() 			{ m_bFade = false; }
		
		//------------------------------------------------------
		//
		// 좌표 변환		
		//
		//------------------------------------------------------
		static POINT	PixelToMap(const int& Xp, const int& Yp)	{ POINT p; p.x=Xp/TILE_X; p.y=Yp/TILE_Y; return p; }//{ POINT p; p.x=Xp>>6; p.y=Yp>>5; return p; }
		static POINT	MapToPixel(const int& Xm, const int& Ym)	{ POINT p; p.x=Xm*TILE_X; p.y=Ym*TILE_Y; return p; }//{ POINT p; p.x=Xm<<6; p.y=Ym<<5; return p; }

		static int		PixelToMapX(const int& Xp)		{ return (Xp/TILE_X); }
		static int		PixelToMapY(const int& Yp)		{ return (Yp/TILE_Y); }
		static int		MapToPixelX(const int& Xm)		{ return (Xm*TILE_X); }	
		static int		MapToPixelY(const int& Ym)		{ return (Ym*TILE_Y); }

		static BYTE		GetDirectionToPosition(int originX, int originY, int destX, int destY);
		static POINT	GetChangeValueToDirection(int direction);

		//------------------------------------------------------
		// TileSize가 64*32일 경우.. 최적화된 좌표 계산(Shift이용)
		//------------------------------------------------------
		//static POINT	PixelToMap(const int& Xp, const int& Yp)	{ POINT p; p.x=Xp>>6; p.y=Yp>>5; return p; }
		//static POINT	MapToPixel(const int& Xm, const int& Ym)	{ POINT p; p.x=Xm<<6; p.y=Ym<<5; return p; }

		//static int		PixelToMapX(const int& Xp)		{ return (Xp >> 6); }
		//static int		PixelToMapY(const int& Yp)		{ return (Yp >> 5); }
		//static int		MapToPixelX(const int& Xm)		{ return (Xm << 6); }	
		//static int		MapToPixelY(const int& Ym)		{ return (Ym << 5); }


		//------------------------------------------------------
		//
		// View와 화면과의 연관
		//
		//------------------------------------------------------
		// 화면 상의 좌표 (x,y)는 Zone에서의 pixel좌표로는 어느 점인가?
		POINT			ScreenToPixel(int x, int y);

		// Zone의 pixel좌표는 화면 상의 어느 점인가?
		POINT			MapToScreen(int sX, int sY);
		POINT			PixelToScreen(int x, int y);
		POINT			GetFirstZonePixel()	const		{ return m_FirstZonePixel; }		
		POINT			GetFirstSector() const			{ return m_FirstSector; }

		//------------------------------------------------------
		//
		// Mouse로 선택하는 Object
		//
		//------------------------------------------------------
		// Selected Sector
		inline void		SetSelectedSector(int sX, int sY)	{ m_SelectSector.x = sX; m_SelectSector.y = sY; }	
		inline void		SetSelectedSector(const POINT& s)	{ m_SelectSector.x = s.x; m_SelectSector.y = s.y; }
		void			SetSelectedSectorNULL()		{ m_SelectSector.x = m_SelectSector.y = SECTORPOSITION_NULL; }
		POINT			GetSelectedSector(int x, int y);// const;

		// 공격할 때의 선택 모드
		//void			SetSelectModeAttack()				{ m_bSelectModeAttack = true; }
		//void			SetSelectModeNormal()				{ m_bSelectModeAttack = false; }


		// 화면상의 (x,y)를 선택했을 경우 선택된 Object를 return한다.
		MObject*		GetSelectedObject(int x, int y);
		MObject*		GetSelectedObjectSprite(int x, int y);

		// Creature 선택
//		void			SetSelectedCreature(TYPE_OBJECTID id)	{ m_SelectCreatureID = id; m_SelectItemID = m_SelectInteractionObjectID = m_SelectEffectID = OBJECTID_NULL; }		
		void			SetSelectedCreature(TYPE_OBJECTID id)	{ m_SelectCreatureID = id; m_SelectItemID = m_SelectEffectID = OBJECTID_NULL; }		
		TYPE_OBJECTID	GetSelectedCreature() const				{ return m_SelectCreatureID; }

		// Item 선택
//		void			SetSelectedItem(TYPE_OBJECTID id)	{ m_SelectItemID = id;  m_SelectCreatureID = m_SelectInteractionObjectID = m_SelectEffectID = OBJECTID_NULL; }
		void			SetSelectedItem(TYPE_OBJECTID id)	{ m_SelectItemID = id;  m_SelectCreatureID = m_SelectEffectID = OBJECTID_NULL; }
		TYPE_OBJECTID	GetSelectedItemID()					{ return m_SelectItemID; }
		
		// InteractionObject 선택
//		void			SetSelectedInteractionObject(TYPE_OBJECTID id)	{ m_SelectInteractionObjectID = id; m_SelectItemID = m_SelectCreatureID = m_SelectEffectID = OBJECTID_NULL; }

		// Effect 선택
//		void			SetSelectedEffect(TYPE_OBJECTID id)		{ m_SelectEffectID = id; m_SelectItemID = m_SelectCreatureID = m_SelectInteractionObjectID = OBJECTID_NULL; }
		void			SetSelectedEffect(TYPE_OBJECTID id)		{ m_SelectEffectID = id; m_SelectItemID = m_SelectCreatureID =  OBJECTID_NULL; }
		
		// 선택된 것이 없게 한다.
//		void			SetSelectedNULL()				{ m_SelectCreatureID = m_SelectItemID = m_SelectInteractionObjectID = m_SelectEffectID = OBJECTID_NULL;}
		void			SetSelectedNULL()				{ m_SelectCreatureID = m_SelectItemID = m_SelectEffectID = OBJECTID_NULL;}


		//------------------------------------------------------
		//
		// 시야 관련..
		//
		//------------------------------------------------------		
		BYTE			GetDarkBits() const				{ return m_DarkBits; }
		void			SetDarkBits(BYTE DarkBits)		{ if (DarkBits<16) m_DarkBits = DarkBits; }

		// 3D 시야관련
		void			ClearLightBufferFilter3D();
		void			AddLightFilter3D(int x, int y, BYTE range, bool bMapPixel=true, bool bForceLight=false);
		void			DrawLightBuffer3D();

		// 2D 시야관련
		void			ClearLightBufferFilter2D();
		void			AddLightFilter2D(int x, int y, BYTE range, bool bMapPixel=true, bool bForceLight=false);
		void			DrawLightBuffer2D();		

		//------------------------------------------------------
		//
		// Creature Output에 관련된 것들
		//
		//------------------------------------------------------
		QWORD			GetOutputImageObjectID(const MImageObject* pImageObject) const
											{ return ((QWORD)pImageObject->GetViewpoint() << 32) | pImageObject->GetID(); }

		//------------------------------------------------------
		//
		// Creature Output에 관련된 것들
		//
		//------------------------------------------------------		
		// [pixel y좌표][object id] ,  4 bytes  +  4 bytes  = 8 bytes 로 구성되어 있다.
		QWORD			GetOutputCreatureID(const MCreature* pCreature) const	
											{ return ((QWORD)pCreature->GetPixelY() << 32) | pCreature->GetID(); }

		QWORD			GetOutputCreatureID(TYPE_OBJECTID id, int y) const	
											{ return ((QWORD)y << 32) | id; }

		// m_pZone의 Creature map을 통째로~ 추가
		void			AddOutputCreatureAll();

		// pCreature를 추가한다.
		bool			AddOutputCreature(MCreature* pCreature);

		// Creature OutputMap을 clear한다.
		void			ClearOutputCreature()	{ m_mapCreature.clear(); }

		// id와 관련있는 creature를 제거한다.
		bool			RemoveOutputCreature(MCreature* pCreature);

		// id와 관련있는 creature의 정보(출력좌표관련id)를 Update한다.
		bool			UpdateOutputCreature(TYPE_OBJECTID id, int y0, int y1);


		//------------------------------------------------------		
		//
		// 채팅 string 등 출력..
		//
		//------------------------------------------------------		
		void			ClearTextList();
		void			AddText(DRAWTEXT_NODE* pNode);
		void			DrawTextList();

		//------------------------------------------------------		
		//
		// ItemName  출력..
		//
		//------------------------------------------------------		
		bool			IsDrawItemNameList() const		{ return m_bDrawItemNameList; }
		void			SetDrawItemNameList()			{ m_bDrawItemNameList = true; }
		void			UnSetDrawItemNameList()			{ m_bDrawItemNameList = false; }
		void			ClearItemNameList();
		void			AddItemName(DRAWITEMNAME_NODE* pNode);
		void			DrawItemNameList();
		MItem*			SelectItemName(int x, int y);

		//------------------------------------------------------		
		//
		// Request
		//
		//------------------------------------------------------		
		bool			IsDrawRequest() const			{ return m_bDrawRequest; }
		
		//------------------------------------------------------		
		// Get MaxEffectFrame
		//------------------------------------------------------		
		int				GetMaxEffectFrame(BLT_TYPE bltType, TYPE_FRAMEID frameID) const;
		int				GetEffectLight(BLT_TYPE bltType, TYPE_FRAMEID frameID, int dir, int frame) const;
		int				GetEffectSpriteType(BLT_TYPE bltType, TYPE_FRAMEID frameID) const;

		//------------------------------------------------------		
		// 기타..
		//------------------------------------------------------		
		int				GetRandomMonsterTypeInZone() const;

//		void			InitMinimapTexture(CSpriteSurface *minimap_surface);			// minimap texture초기화

	protected :
		//------------------------------------------------------
		//
		// 내부적으로 초기화하는 함수들..
		//
		//------------------------------------------------------
		bool			InitCreatureFrames();		// character 정보 초기화
		bool			InitImageFrames();			// ImageFrames 정보 초기화
		bool			InitAnimationFrames();		// AnimationFrames 초기화
		bool			InitEffectFrames();			// EffectFrames 초기화
		bool			InitSprites();				// Sprites 초기화
		bool			InitSurfaces();				// Surfaces 초기화
		bool			InitFilters();				// Filter초기화
		//bool	InitSpriteSurfaces();		// SpriteSurfaces 초기화 
		bool			InitColors();				// 색깔들 초기화
// 		bool			Init3DBoxSurface();
		bool			InitFonts();				// font정보 초기화

		// box surface의 색깔 설정
// 		void			Set3DBoxColor(WORD pixel);

		//------------------------------------------------------
		// 화면에 보이는 ImageObject 변경
		//------------------------------------------------------
		void			DetermineImageObject();
		void			UpdateImageObject(const POINT &newFirstSector);

		//------------------------------------------------------
		// SurfaceLock
		//------------------------------------------------------
		//BOOL			SurfaceLock();
		//BOOL			SurfaceLockForEffect();
		//void			SurfaceLockRestore( BOOL bOldLock );

		//------------------------------------------------------
		// Draw 함수들..
		//------------------------------------------------------
		void		DrawTileSurface();
		void		DrawZone(int X,int Y);
		void		DrawInformation();		
		void		DrawEventString(int &strX, int &strY);			// 이벤트 메니저에서 스트링으로 찍히는 부분을 가져와 화면에 찍어주는 함수
		int			DrawChatString(POINT* pPoint, MCreature* pCreature, COLORREF color, BYTE flag=0);
		void		DrawItem(POINT* pPoint, MItem* pItem);
		void		DrawItemShadow(POINT* pPoint, MItem* pItem);
		void		DrawImageObject(POINT* pPoint, MImageObject* pImageObject);
		void		DrawAttachEffect(POINT* pPoint, ATTACHEFFECT_LIST::const_iterator iEffect, BYTE size, MCreature* pCreature, int type=0);
		void		DrawGroundEffect();
		void		DrawEffect(POINT* pPoint, MEffect* pEffect, bool bSeletable);
		void		DrawEffect(POINT* pPoint, EFFECT_LIST::const_iterator iEffect, BYTE size);
		void		DrawAlphaBox(RECT* pRect, BYTE r, BYTE g, BYTE b, BYTE alpha);
		void		DrawTestHelp();
		void		DrawInventoryEffect(POINT* pPoint);
		void		DrawMinimap(int x, int y, BYTE scale);
		void		DrawItemBroken(int x, int y);
		void		DrawPartyHP(POINT* pPoint, MCreature* pCreature);
// 		void		DrawBox3D(RECT* pRect, WORD pixel);			
		void		DrawFade();

		//------------------------------------------------------
		// Character Draw 함수들... by sonee   2003.6
		//------------------------------------------------------
		void		DrawCreature(POINT* pPoint, MCreature* pCreature);		
		void		DrawCreatureShadow(POINT* pPoint, MCreature* pCreature);
		void		DrawCreatureName(MCreature* pCreature);
		void		DrawCreatureMyName();
		void		DrawCreatureHPModify(POINT *point, MCreature* pCreature);
		void		DrawUndergroundCreature(POINT *pPoint, MCreature *pCreature);

		void		DrawGuildMarkInSiegeWar(MCreature* pCreature, int YPos); // 길드마크 큰놈으로 뿌려주자..ㅋ

	protected :		
		bool				m_bInit;

		//------------------------------------------------------
		// 출력 대상이 되는 Surface
		//------------------------------------------------------
		CSpriteSurface*		m_pSurface;
		//S_SURFACEINFO		m_SurfaceInfo;
	

		// SpriteSurfacePack
		//CSpriteSurfacePack	m_SpriteSurfacePack;	// 그림들		
		//CSpriteSurface**			m_ppSurface;			// SpriteSurfaces

		//------------------------------------------------------
		//
		//                   Frame
		//
		//------------------------------------------------------
	public :
		//------------------------------------------------------
		// FramePacks
		//------------------------------------------------------		
		CCreatureFramePack		m_CreatureFPK;			// Creature frames
		CCreatureFramePack		m_AddonFPK;				// Player addon frames
		CCreatureFramePack		m_OustersFPK;			// Ousters frames

		CCreatureFramePack		m_AdvancementSlayerManFPK;			// Creature frames
		CCreatureFramePack		m_AdvancementSlayerWomanFPK;			// Creature frames
		CCreatureFramePack		m_AdvancementVampireManFPK;				// Player addon frames
		CCreatureFramePack		m_AdvancementVampireWomanFPK;				// Player addon frames
		CCreatureFramePack		m_AdvancementOustersFPK;			// Ousters frames

		CImageFramePack			m_ItemTileFPK;			// Tile에 있는 Item에 대한..
		CAnimationFramePack		m_ItemDropFPK;			// 바닥으로 떨어지는 Item에 대한..
		CAnimationFramePack		m_ItemBrokenFPK;		// 부서지는 Item에 대한..
		CAnimationFramePack		m_ImageObjectFPK;		// ImageObject에 대한 frames
		CAnimationFramePack		m_ImageObjectShadowFPK;		// ImageObject에 대한 frames
//		CAnimationFramePack		m_InteractionObjectFPK;		// ImageObject에 대한 frames
		CEffectFramePack		m_EffectAlphaFPK;		// Effect에 대한 frames
		CEffectFramePack		m_EffectNormalFPK;		// Effect에 대한 frames
		CEffectFramePack		m_EffectShadowFPK;		// Effect에 대한 frames
		CEffectFramePack		m_EffectScreenFPK;
		
		CCreatureFramePack		m_CreatureShadowFPK;			// Creature frames
		CCreatureFramePack		m_AddonShadowFPK;			// Player addon frames
		CCreatureFramePack		m_OustersShadowFPK;			// Ousters addon frames

		CCreatureFramePack		m_AdvancementSlayerManShadowFPK;			// Creature frames
		CCreatureFramePack		m_AdvancementSlayerWomanShadowFPK;			// Creature frames
		CCreatureFramePack		m_AdvancementVampireManShadowFPK;			// Player addon frames
		CCreatureFramePack		m_AdvancementVampireWomanShadowFPK;			// Player addon frames
		CCreatureFramePack		m_AdvancementOustersShadowFPK;			// Ousters addon frames

		//add by viva
		//                   Sprite
		//
		//------------------------------------------------------
		//------------------------------------------------------
		// SpritePack
		//------------------------------------------------------		
		CSpritePack				m_TileSPK;				// Tile
		CSpritePack				m_ImageObjectSPK;		// ImageObject Sprite들
//		CSpritePack				m_InteractionObjectSPK;		// ImageObject Sprite들
		CIndexSpritePack		m_CreatureSPK;			// Creature Sprite들
		CIndexSpritePack		m_AddonSPK;		// Sprite들		
		CIndexSpritePack		m_OustersSPK;		// Sprite들		

		CIndexSpritePack		m_AdvancementSlayerManSPK;			// Creature Sprite들
		CIndexSpritePack		m_AdvancementSlayerWomanSPK;			// Creature Sprite들
		CIndexSpritePack		m_AdvancementVampireManSPK;		// Sprite들		
		CIndexSpritePack		m_AdvancementVampireWomanSPK;		// Sprite들		
		CIndexSpritePack		m_AdvancementOustersSPK;		// Sprite들	
		
		//end
		
		//CSpritePack				m_ItemTileSPK;			// Sprite들		
		CIndexSpritePack		m_ItemTileISPK;			// Sprite들		
		CIndexSpritePack		m_ItemDropISPK;			//
		CSpritePack				m_ItemBrokenSPK;		// 부서지는 아이템
		CSpritePack				m_ItemRealSPK;			// Sprite들				
		CAlphaSpritePalPack		m_EffectAlphaSPK;		// AlphaSprite들
		CSpritePalPack			m_EffectScreenSPK;		// ScreenEffect
		MPalettePack			m_EffectAlphaPPK;		// AlphaEffect
		MPalettePack			m_EffectScreenPPK;		// ScreenEffect
		CSpritePack				m_EffectNormalSPK;		// NormalSprite들
		CShadowSpritePack		m_EffectShadowSPK;		// ShadowSprite들
		CSpritePack				m_WeatherSPK;			// WeatherSprite들
		CSpritePack				m_GuildSPK;				// GuildMark

		

		CSpritePack				m_EtcSPK;				// 기타...
		CSpritePack				m_OustersFinSPK;				// 기타...
		CSpritePack				m_AdvacementQuestEnding;				// 기타...


		//------------------------------------------------------		
		// Shadow SpritePack
		//------------------------------------------------------
		CShadowSpritePack		m_AddonSSPK;			// Sprite들		
		CShadowSpritePack		m_CreatureSSPK;
		CShadowSpritePack		m_OustersSSPK;
		CShadowSpritePack		m_ImageObjectSSPK;

		CShadowSpritePack		m_AdvancementSlayerManSSPK;			// Sprite들		
		CShadowSpritePack		m_AdvancementSlayerWomanSSPK;			// Sprite들		
		CShadowSpritePack		m_AdvancementVampireManSSPK;
		CShadowSpritePack		m_AdvancementVampireWomanSSPK;
		CShadowSpritePack		m_AdvancementOustersSSPK;
		//add by viva
//		CShadowSpritePack		m_InteractionObjectSSPK;

		//CShadowSpritePack		m_CreatureSSPK;

//		float					m_SectorToMinimapHeight;
//		CSpriteSurface*			m_pMinimapTexture;
//		int						m_MinimapTextureWidth;
//		int						m_MinimapTextureHeight;


		//------------------------------------------------------
		//
		//                  Sprite Index
		//
		//------------------------------------------------------
		CFileIndexTable			m_TileSPKI;
		CFileIndexTable			m_ImageObjectSPKI;
//		CFileIndexTable			m_EffectAlphaSPKI;
//		CFileIndexTable			m_EffectScreenSPKI;

		
		//------------------------------------------------------
		// 
		//                  Sprite File
		//
		//------------------------------------------------------
//		std::ifstream			m_TileSPKFile;
//		std::ifstream			m_ImageObjectSPKFile;
//		std::ifstream			m_EffectAlphaSPKFile;
//		std::ifstream			m_EffectScreenSPKFile;
//		std::ifstream			m_AddonSPKFile;
//		std::ifstream			m_AddonSSPKFile;
//		std::ifstream			m_CreatureSPKFile;
//		std::ifstream			m_CreatureSSPKFile;

		//------------------------------------------------------
		//
		//                   Filter
		//
		//------------------------------------------------------
		//CFilterPack				m_Filter;
		CFilter					m_ImageObjectFilter;

		//------------------------------------------------------
		//
		//               Texture PartManager
		//
		//------------------------------------------------------
		// Texture PartManager
//		CSpriteTexturePartManager*	m_pScreenEffectTextureManager;
//		CNormalSpriteTexturePartManager*	m_pImageObjectTextureManager;
//		CTexturePartManager*		m_pAlphaEffectTextureManager;
//		CShadowPartManager*			m_pImageObjectShadowManager;
//		CShadowPartManager*			m_pAddonShadowManager;
//		CShadowPartManager*			m_pCreatureShadowManager;
//		CShadowPartManager*			m_pOustersShadowManager;
//		CShadowPartManager*			m_pEffectShadowManager;


		//------------------------------------------------------
		// Zone마다 독립적으로 Load된 것들에 대한 정보
		//------------------------------------------------------
		CSpriteFilePositionArray*	m_pTileSFPArrayLargeZone;
		CSpriteFilePositionArray*	m_pImageObjectSFPArrayLargeZone;
		CSpriteFilePositionArray*	m_pTileSFPArraySmallZone;
		CSpriteFilePositionArray*	m_pImageObjectSFPArraySmallZone;


		//------------------------------------------------------
		//
		//                Other Data members
		//
		//------------------------------------------------------

		//------------------------------------------------------
		// Zone
		//------------------------------------------------------
		MZone*				m_pZone;				// 현재 출력하려는 Zone

		static int			m_MiddleX;				// zone의 중간(pixel X 좌표)
		POINT				m_FirstSector;			// 화면 상의 첫 점의 Zone에서의 좌표(sector)
		POINT				m_PlusPoint;			// FirstSector의 pixel단위의 좌표보정값
		POINT				m_FirstZonePixel;		// 화면(0,0)이 나타내는 Zone상의 Pixel좌표

		//------------------------------------------------------
		// Mouse로 선택된 화면 상의 것들..
		//------------------------------------------------------
		POINT				m_SelectSector;		
		TYPE_OBJECTID		m_SelectCreatureID;
		TYPE_OBJECTID		m_SelectItemID;
//		TYPE_OBJECTID		m_SelectInteractionObjectID;
		TYPE_OBJECTID		m_SelectEffectID;
		//bool				m_bSelectModeAttack;

		WORD					m_SOMOutlineColor;
		CSpriteOutlineManager	m_SOM;	// 선택된 것의 외곽선 출력

		//------------------------------------------------------
		// 시야 처리
		//------------------------------------------------------		
		POINT					m_FilterPosition;	// Player를 가리는 시야 처리		
		BYTE					m_DarkBits;
		

		// 시야처리 관련 - 빛이 있는 시야부분
		CFilterPack				m_LightFTP;
		CFilter					m_LightBufferFilter;
		CSpriteSurface*			m_pLightBufferTexture;
		int						m_nLight;


		//------------------------------------------------------
		// Fade In/Out 관련
		//------------------------------------------------------		
		char				m_FadeValue;	// 현재 fade값
		char				m_FadeEnd;		// 끝 값
		char				m_FadeInc;		// 변화값
		bool				m_bFade;		// fade 중인가?
		WORD				m_FadeColor;	// fade 색
		WORD				m_delayFrame;	// 변화값을 바꿀 시점에 대한 프레임
		
		//------------------------------------------------------
		// 현재 화면의 Tile을 저장해서 다음에 출력할때 이용한다.
		//------------------------------------------------------
		CSpriteSurface*				m_pTileSurface;
		bool						m_bFirstTileDraw;
		int							m_TileSurfaceFirstZonePixelX,
									m_TileSurfaceFirstZonePixelY;
		int							m_TileSurfaceFirstSectorX,
									m_TileSurfaceFirstSectorY;

		//------------------------------------------------------
		// TileRenderer for unified tile rendering (Phase 4 integration)
		//------------------------------------------------------
		TileRenderer*				m_pTileRenderer;
		MZoneTileProvider			m_zoneTileProvider;

		//------------------------------------------------------
		// 현재 화면에서 존재하는 ImageObject들의 정보
		//------------------------------------------------------
		IMAGEOBJECT_OUTPUT_MAP				m_mapImageObject;

		//------------------------------------------------------
		// Creature OutputMap - 출력 순서에 관계된다.
		//------------------------------------------------------
		CREATURE_OUTPUT_MAP					m_mapCreature;
		bool								m_bTileSearchForCreature;

		//------------------------------------------------------
		// Mouse로 선택된 Creature의 채팅을 출력하기 위한 정보
		//------------------------------------------------------
		POINT						m_pointChatString;
		MCreature*					m_pSelectedCreature;

		//------------------------------------------------------
		// Texture Surface
		//------------------------------------------------------
		// Effect
		//CTexturePack				m_EffectTPK;
		//CTexturePack				m_CreatureTPK;
		//CTexturePack				m_ShadowTPK;	// for test

#ifdef PLATFORM_WINDOWS
		CD3DTextureEffect			m_TextureEffect;
#endif
		
		//------------------------------------------------------
		// 출력할 String을 기억..
		//------------------------------------------------------
		DRAWTEXT_PQ					m_pqDrawText;

		//------------------------------------------------------
		// ItemName 출력
		//------------------------------------------------------
		bool						m_bDrawItemNameList;
		DRAWITEMNAME_LIST			m_listDrawItemName;

		//------------------------------------------------------
		// Request mode
		//------------------------------------------------------
		bool						m_bDrawRequest;

		//------------------------------------------------------
		// 3D 관련..
		//------------------------------------------------------
//		CSpriteSurface*				m_p3DBoxSurface;
//		WORD						m_3DBoxCurrentPixel;

//		CSpriteSurface*				m_p3DBoxBlackSurface;
//		CSpriteSurface*				m_p3DBoxHPSurface;
		

		//------------------------------------------------------
		// 색깔들..
		//------------------------------------------------------
		// interactionObject
//		WORD		m_ColorOutlineInteractionObject;

		// item
		COLORREF	m_ColorNameItem;
		COLORREF	m_ColorNameItemOption;
		WORD		m_ColorOutlineItem;

		// 종족별
		COLORREF	m_ColorNameVampire;
		COLORREF	m_ColorNameSlayer;
		COLORREF	m_ColorNameNPC;

		COLORREF	m_ColorNameAlignment[5];	// 음냐..

		// 공격 가능?
		WORD		m_ColorOutlineNPC;
		WORD		m_ColorOutlineAttackPossible;
		WORD		m_ColorOutlineAttackImpossible;

		// hp bar의 색깔
		WORD		m_ColorHPBar;

		// 반투명 검정색
		WORD		m_ColorBlackHalf;


		//------------------------------------------------------
		// 2D light에서 한 점이 차지하는 화면의 pixel크기
		//------------------------------------------------------
		int*		m_p2DLightPixelWidth;
		int*		m_p2DLightPixelHeight;

		//------------------------------------------------------
		// Load된 것 정보
		//------------------------------------------------------
		INT_ORDERED_LIST	m_listLoadedCreatureSprite;
		INT_ORDERED_LIST	m_listLoadedMonsterSprite;
		
	public :
		// 한Light좌표의 화면에서의 크기
		static float				s_LightWidth;
		static float				s_LightHeight;
		
		// 2004, 8, 18 sobeit add start - 공성전 성문 타겟팅때문에 여차여차 추가..
		enum GENERATE_OPTION{
			GENERATE_ALL = 0,
			GENERATE_EXCEPT_LEFT,	// 왼쪽 아웃라인은 제외
			GENERATE_EXCEPT_RIGHT,  // 오른쪽 아웃라인은 제외
			GENERATE_EXCEPT_SIDE,	// 양 사이드 아웃라인은 제외
			// -_- 위,아래도 만들어 놔야 하나..-_-;
		};
		// 2004, 8, 18 sobeit add start

	private :
		// -_- 예외처리 함수 포인터로 뺌.
		//DrawCreatureExecptionProc*	m_pExceptionProc;
		void	InitMapingExceptionProc();
		void	RegistExceptionProcFunction( DWORD objectID, DrawCreatureExceptionProc proc );

		// DrawSelectedCharacter
		void	DrawSelectedVampireCreature( POINT* pPoint, MCreature* pCreature, int action, int direction, int frame, int body, int frameindex );
		void	DrawSelectedSlayerCreature( POINT* pPoint, MCreature* pCreature, int action, int direction, int frame );
		void	DrawSelectedOustersCreature( POINT* pPoint, MCreature* pCreature, int action, int direction, int frame );

		void	DrawSelectedAdvancementVampireCreature( POINT* pPoint, MCreature* pCreature, int action, int direction, int frame, int body, int frameindex );
		void	DrawSelectedAdvancementSlayerCreature( POINT* pPoint, MCreature* pCreature, int action, int direction, int frame );
		void	DrawSelectedAdvancementOustersCreature( POINT* pPoint, MCreature* pCreature, int action, int direction, int frame );

		// DrawEffect
		void	DrawInstallTurret( POINT *pPoint, MCreature* pCreature, int action, int direction, int frame, int frameindex );
		void	DrawFadeOut( POINT* pPoint, MCreature* pCreature, int action, int direction, int frame );
		void	DrawFastMove(POINT* pPoint, MCreature* pCreature, int action, int direction, int frame );
		void	DrawInvisible(POINT* pPoint, MCreature* pCreature, int action, int direction, int frame );
		void	DrawWeaponFadeOut(POINT* pPoint, MCreature* pCreature, int action, int direction, int frame );
		
		void	DrawFadeOutForACSlayer( POINT *pPoint, MCreature* pCreature, int action, int direction, int frame );
		void	DrawFastMoveForACSlayer(POINT* pPoint, MCreature* pCreature, int action, int direction, int frame );
		void	DrawInvisibleForACSlayer(POINT* pPoint, MCreature* pCreature, int action, int direction, int frame );
		void	DrawWeaponFadeOutForACSlayer(POINT* pPoint, MCreature* pCreature, int action, int direction, int frame );
		
		// DrawDivineGuidance
		void	DrawDivineGuidanceSlayerCharacter( POINT *pPoint, MCreature* pCreature, int direction );
		void	DrawDivineGuidanceVampireCharacter( POINT *pPoint, MCreature* pCreature, int direction, int body );
		void	DrawDivineGuidanceOustersCharacter( POINT *pPoint, MCreature* pCreature, int direction );
		
		void	DrawDivineGuidanceAdvancementClassSlayerCharacter( POINT *pPoint, MCreature* pCreature, int direction );
		void	DrawDivineGuidanceAdvancementClassVampireCharacter( POINT *pPoint, MCreature* pCreature, int direction, int body );
		void	DrawDivineGuidanceAdvancementClassOustersCharacter( POINT *pPoint, MCreature* pCreature, int direction );
		
		// DrawNormalCharacter
		void	DrawSlayerCharacter( POINT *pPoint, MCreature* pCreature, int action, int direction, int frame, int FrameIndex );		
		void	DrawVampireCharacter( POINT* pPoint, MCreature* pCreature, int action, int direction, int frame, int body, int frameindex );
		void	DrawOustersCharacter( POINT* pPoint, MCreature* pCreature, int action, int direction, int frame );
		
		void	DrawShadowSlayerCharacter( POINT *pPoint, MCreature* pCreature, int action, int direction, int frame );
		void	DrawShadowVampireCharacter( POINT *pPoint, MCreature* pCreature, int action, int direction, int frame, int body, bool bBlendingShadow , bool bSlayerPet_ShowTurret );
		void	DrawShadowOustersCharacter( POINT *pPoint, MCreature* pCreature, int action, int direction, int frame );

		// DrawAdvancementClassCharacter
		void	DrawAdvancementClassSlayerCharacter( POINT *pPoint, MCreature* pCreature, int action, int direction, int frame, int FrameIndex );
		void	DrawAdvancementClassOustersCharacter( POINT *pPoint, MCreature* pCreature, int action, int direction, int frame );
		void	DrawAdvancementClassVampireCharacter( POINT* pPoint, MCreature* pCreature, int action, int direction, int frame, int body, int frameindex );

		void	DrawShadowAdvancementClassSlayerCharacter( POINT *pPoint, MCreature* pCreature, int action, int direction, int frame );
		void	DrawShadowAdvancementClassVampireCharacter( POINT *pPoint, MCreature* pCreature, int action, int direction, int frame, int body, bool bBlendingShadow , bool bSlayerPet_ShowTurret );
		void	DrawShadowAdvancementClassOustersCharacter( POINT *pPoint, MCreature* pCreature, int action, int direction, int frame );

		// 2004, 12, 11, sobeit add start
		void	DrawCentauroTurret( POINT* pPoint, MCreature* pCreature, int action, int direction, int frame , int body);
		// 2004, 12, 11, sobeit add end
		
		// 2005, 1, 20, sobeit add start
		bool	DrawEvent();
		bool	ExcuteOustersFinEvent();
		bool	ExcuteAdvancementQuestEnding(void *pVoid);
		// 2005, 1, 20, sobeit add end
		
		std::map< DWORD, DrawCreatureExceptionProc >	m_ExceptionProcMap;
};

extern MTopView*	g_pTopView;

#endif
