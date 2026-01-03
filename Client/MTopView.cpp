//#define __3D_IMAGE_OBJECT__					// by sonee

//----------------------------------------------------------------------
// MTopView.cpp
//----------------------------------------------------------------------
// [ Effect Layer ]  - 免仿 鉴辑俊 蝶扼辑..
// 
//		-- Tile 免仿
//	* Tile俊 官肺 嘿篮 Effect
//
// 
//	* Sector狼 Effect			
//		-- 阿阿狼 Character 免仿
//	* Character俊 嘿篮 AttachEffect
// 
//----------------------------------------------------------------------
#include "Client_PCH.h"
#pragma warning(disable:4786)

#include "DX3D.h"
#include <math.h>
#include <list>
#include <stdio.h>
#include <fstream.h>
#include "MZone.h"
#include "MCreature.h"
#include "MFakeCreature.h"
#include "MPlayer.h"
#include "MItem.h"
#include "MImageObject.h"
#include "MAnimationObject.h"
#include "MShadowAnimationObject.h"
//#include "MInteractionObject.h"
#include "MMovingEffect.h"
#include "MScreenEffect.h"
#include "MTopView.h"
#include "MItemTable.h"
#include "MCreatureTable.h"
#include "MWeather.h"
#include "CMessageArray.h"
#include "DXLib.h"
#include "SP.h"
#include "FL2.h"
//#include "2D.h"
#include "ClientConfig.h"
#include "VS_UI_BASE.h"
#include "VS_UI_Mouse_Pointer.h"
#include "UIFunction.h"
#include "AddonDef.h"
#include "MZoneTable.h"
#include "MGameTime.h"
#include "MSlayerGear.h"
#include "MVampireGear.h"
#include "MOustersGear.h"
#include "UserOption.h"
#include "UserInformation.h"
#include "SpriteIDDef.h"
#include "MWorkThread.h"
#include "MLoadingSPKWorkNode.h"
#include "MGameStringTable.h"
//#include "MZoneInfo.h"
#include "MObjectSelector.h"
#include "MPortal.h"
#include "EffectSpriteTypeDef.h"
#include "MScreenEffectManager.h"
#include "MEffectSpriteTypeTable.h"
#include "RequestServerPlayerManager.h"
#include "RequestClientPlayerManager.h"
#include "MJusticeAttackManager.h"
#include "Profiler.h"
//#include "MFileDef.h"
#include "Properties.h"
#include "MGuildMarkManager.h"
#include "MEventManager.h"
#include "MAttachOrbitEffect.h"
#include "MTestDef.h"
#include "ServerInfo.h"
#include "DebugInfo.h"
#include "MTestDef.h"
#include "MItemOptionTable.h"
#include "MMonsterKillQuestInfo.h"
#include "Client.h"
#include "cmp3.h"
#include "CSprite555.h"
#include "CSprite565.h"
#include "SoundSetting.h"
#include "SystemAvailabilities.h"
#include "SkillDef.h"
#include "vs_ui_item.h"
#include "NicknameInfo.h"

#include "MTopViewDraw.inl"
#include "VS_UI_GameCommon2.h"
//#undef OUTPUT_DEBUG

//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE

#if defined(OUTPUT_DEBUG)//defined(_DEBUG) || 
//	#define OUTPUT_DEBUG_DRAW_3DBOX
	#ifdef __METROTECH_TEST__
//		#define OUTPUT_DEBUG_DRAW_PROCESS
//		#define OUTPUT_DEBUG_DRAW_PROCESS_INPUT
	#endif
//	#define OUTPUT_DEBUG_DRAW_SHADOW
#endif

void	StartTitleLoading();
void	DrawTitleLoading();
void	EndTitleLoading(bool SendLogin = false);

#ifdef __METROTECH_TEST__
	extern bool g_bLight;
#endif

extern void PlaySoundForce(TYPE_SOUNDID soundID);

extern HWND							g_hWnd;
extern POINT						g_SelectSector;
extern BOOL							g_bLButtonDown;
extern BOOL							g_bRButtonDown;
extern BOOL							g_bCButtonDown;
extern BOOL					g_bUIInput;
//extern MInput						g_Input;
//extern BOOL							g_WindowsNT;
extern BOOL			g_MyFull;
extern RECT			g_GameRect;
extern int			g_nZoneLarge;
extern int			g_nZoneSmall;
extern bool			g_bZonePlayerInLarge;
//extern HANDLE		g_hFileThread;

extern CMessageArray*	g_pSystemMessage;
extern CMessageArray*	g_pPlayerMessage;
extern CMessageArray*	g_pGameMessage;
extern CMessageArray*	g_pNoticeMessage;

extern MScreenEffectManager*	g_pInventoryEffectManager;

extern bool FileOpenBinary(const char* filename, class ifstream& file);
//GameNew Mode  add by sonc 2006.9.27
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
//end 
//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
MTopView*	g_pTopView = NULL;
extern bool g_bFrameChanged;		

bool g_bMouseInPortal  = false;

// 烙矫 东户.. 快庆庆
POINT g_MouseSector = { 0, 0 };		// mouse啊 啊府虐绰 sector谅钎

extern int	g_x;
extern int	g_y;

extern int	g_UpdateDelay;

extern MWorkThread*	g_pLoadingThread;

int g_ShowImageObjectID = 0;

// 齿付喉侩
//std::map<int, bool> g_mapPremiumZone;

#ifdef OUTPUT_DEBUG
	extern bool g_bSlideScreenShot;
	extern RECT g_rectScreenShot;

	extern int gtx_op;
	extern int gtx_src;
	extern int gtx_dest;

	MString g_ProfileInfoName("DrawZone");
#endif

//----------------------------------------------------------------------
// 茄 filter谅钎狼 拳搁厚啦 搬沥..
//----------------------------------------------------------------------
float MTopView::s_LightWidth	= (float)g_GameRect.right / SCREENLIGHT_WIDTH;
float MTopView::s_LightHeight	= (float)g_GameRect.bottom / SCREENLIGHT_HEIGHT;

//----------------------------------------------------------------------
//
// defines
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// 傈眉 Creature狼 弊覆 辆幅荐
//----------------------------------------------------------------------
#define	MAX_CREATURE_BODY		3
#define	MAX_CREATURE_ADDON		4
#define	MAX_ANIMATION			2
#define	MAX_ALPHAEFFECT			10
#define	MAX_NORMALEFFECT		1

//----------------------------------------------------------------------
// Light 3D狼 汲沥 蔼
//-----------------------------------------------------------------------
#define	MAX_LIGHT_SETSIZE		12

//-----------------------------------------------------------------------
// 胶唱捞俏俊辑 绢涤霸 免仿且锭狼 shift蔼
//-----------------------------------------------------------------------

//----------------------------------------------------------------------
//
// FONT 包访
//
//----------------------------------------------------------------------
enum FONTID
{
	FONTID_INFO,	
	FONTID_SLAYER_NAME,
	FONTID_VAMPIRE_NAME,
	FONTID_NPC_NAME,
	FONTID_SLAYER_CHAT,
	FONTID_VAMPIRE_CHAT,
	FONTID_NPC_CHAT,
	FONTID_ITEM,
	FONTID_LARGE_CHAT,
	FONTID_USER_ID,
	MAX_FONTID
};

PrintInfo*	g_ClientPrintInfo[MAX_FONTID];

//----------------------------------------------------------------------
//
//		Text Comparison
//
//----------------------------------------------------------------------
// 矫埃捞 坷贰等吧 急琶秦具 茄促.		
// left < right 俊 措茄 return蔼
//
// 泅 鉴辑甫 蜡瘤且妨搁 false甫
// 鉴辑甫 官曹妨搁 true甫 return秦具 茄促.
//
// 菊俊 巴捞 刚历 免仿瞪 巴捞骨肺.. 矫埃捞 狐弗 巴(利篮 巴)捞 菊俊 乐绢具 茄促.
// 
// PQ绰 累篮吧 第肺 焊辰促..
// 弊矾骨肺... 矫埃捞 蠢赴 巴(奴巴)捞 第肺 啊具茄促.
bool 
TextComparison::operator () (DRAWTEXT_NODE * left, DRAWTEXT_NODE * right) const
{ 
	// 刚历 汲沥等 臂磊牢 版快.. OK
	// 矫埃捞 鞍栏搁 老窜 第俊 甸绢埃促. 
	return left->GetTextTime() >= right->GetTextTime();
}

inline		bool			IsAffectFromDarkness()
{
	if( g_pPlayer->IsVampire() ||
		g_pPlayer->HasEffectStatus( EFFECTSTATUS_LIGHTNESS ) ||
		g_pZone->GetID() == 3001 )
		return false;

	return true;
}

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
MTopView::MTopView()
{
	m_bInit = false;

	m_pZone		= NULL;
	
	m_FirstSector.x = 0;
	m_FirstSector.y = 0;
	m_FirstZonePixel.x = 0;	
	m_FirstZonePixel.y = 0;

	m_SelectSector.x = SECTORPOSITION_NULL;
	m_SelectSector.y = SECTORPOSITION_NULL;

	// 急琶等 巴甸
	m_SelectCreatureID			= OBJECTID_NULL;
	m_SelectItemID				= OBJECTID_NULL;
//	m_SelectInteractionObjectID	= OBJECTID_NULL;
	m_SelectEffectID = OBJECTID_NULL;
	//m_bSelectModeAttack			= false;

	//m_ppSurface			= NULL;

	// Tile Surface
	m_pTileSurface			= NULL;
	m_bFirstTileDraw		= true;
	m_TileSurfaceFirstZonePixelX	= 0;
	m_TileSurfaceFirstZonePixelY	= 0;


	//m_SelectSector.x = 0;
	//m_SelectSector.y = 0;
	
	// 矫具
	m_DarkBits = 0;


	// imageobject texture manager
#ifdef __3D_IMAGE_OBJECT__
//	m_pImageObjectTextureManager = NULL;
#endif

	// Texture PartManager
//	m_pAlphaEffectTextureManager = NULL;
//	m_pScreenEffectTextureManager = NULL;

	// Shadow PartManager
//	m_pImageObjectShadowManager = NULL;
//	m_pCreatureShadowManager = NULL;
//	m_pAddonShadowManager = NULL;
//	m_pOustersShadowManager = NULL;
//	m_pEffectShadowManager = NULL;

	// LightBuffer Texture
	m_pLightBufferTexture = NULL;

	// 3D Box Texture Surface
//	m_p3DBoxSurface = NULL;	
//	m_p3DBoxBlackSurface = NULL;	
//	m_p3DBoxHPSurface = NULL;	
//	m_3DBoxCurrentPixel =  0x7000;
//
//	// 2001.7.14 林籍贸府
//	m_pMinimapSPR = NULL;	
//	m_pMinimapTexture = NULL;

	m_p2DLightPixelWidth = NULL;
	m_p2DLightPixelHeight = NULL;


	m_bDrawItemNameList = false;

	//
	m_pTileSFPArrayLargeZone = NULL;
	m_pImageObjectSFPArrayLargeZone = NULL;
	m_pTileSFPArraySmallZone = NULL;
	m_pImageObjectSFPArraySmallZone = NULL;

	m_bDrawRequest = false;

	//-----------------------------------------------
	// AlphBox甫 g_pTopView->Init()傈俊 荤侩窍扁 困秦辑.
	//-----------------------------------------------
//	if (CDirect3D::IsHAL())
//	{
//		m_p3DBoxSurface = new CSpriteSurface;
//		m_p3DBoxSurface->InitTextureSurface(1, 1, 0, CDirect3D::GetPixelFormat4444());
//
//		Set3DBoxColor( m_3DBoxCurrentPixel );
//
//		m_p3DBoxBlackSurface = new CSpriteSurface;
//		m_p3DBoxBlackSurface->InitTextureSurface(1, 1, 0, CDirect3D::GetPixelFormat4444());
//		m_p3DBoxBlackSurface->Restore();
//
//		m_p3DBoxHPSurface = new CSpriteSurface;
//		m_p3DBoxHPSurface->InitTextureSurface(1, 1, 0, CDirect3D::GetPixelFormat4444());
//		m_p3DBoxHPSurface->Restore();
//	}

//	m_bTileSearchForCreature = true;
	m_bTileSearchForCreature = false;		

	// 齿付喉侩
////	g_mapPremiumZone[31] = true;
////	g_mapPremiumZone[32] = true;
////	g_mapPremiumZone[33] = true;
////	g_mapPremiumZone[34] = true;
//	g_mapPremiumZone[51] = true;
//	g_mapPremiumZone[52] = true;
//	g_mapPremiumZone[53] = true;
//	g_mapPremiumZone[54] = true;
//	g_mapPremiumZone[61] = true;
//	g_mapPremiumZone[62] = true;
//	g_mapPremiumZone[63] = true;
//	g_mapPremiumZone[64] = true;
////	g_mapPremiumZone[1002] = true;
//	g_mapPremiumZone[1007] = true;
//	g_mapPremiumZone[1008] = true;
//	g_mapPremiumZone[1009] = true;
////	g_mapPremiumZone[1011] = true;
////	g_mapPremiumZone[1012] = true;
//	g_mapPremiumZone[1013] = true;
//	g_mapPremiumZone[1111] = true;
//	g_mapPremiumZone[1112] = true;
//	g_mapPremiumZone[1113] = true;
//	g_mapPremiumZone[1104] = true;
//	g_mapPremiumZone[1106] = true;
//	g_mapPremiumZone[1114] = true;
//	g_mapPremiumZone[1115] = true;
	InitMapingExceptionProc();
}


MTopView::~MTopView()
{
	Release();
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Init
//----------------------------------------------------------------------
// View俊 鞘夸茄 阿 何盒甸篮 檬扁拳 矫挪促.
//----------------------------------------------------------------------
bool
MTopView::Init()
{
	//------------------------------------------
	// 捞固 init灯促搁 init窍瘤 臼绰促.
	//------------------------------------------
	if (m_bInit)
	{
		return false;
	}

	//------------------------------------------
	// 利例茄 Texture Memory 拌魂
	//------------------------------------------
	DDSCAPS2 ddsCaps2;
	DWORD dwTotal;
	DWORD dwFree;
	ZeroMemory(&ddsCaps2, sizeof(ddsCaps2)); 
	ddsCaps2.dwCaps = DDSCAPS_TEXTURE; 
	HRESULT hr = CDirectDraw::GetDD()->GetAvailableVidMem(&ddsCaps2, &dwTotal, &dwFree);

	DEBUG_ADD_FORMAT("[TextureMemory] Before Init View = %d/%d", dwFree, dwTotal);
	
	//------------------------------------------
	// 利例茄 effect texture俺荐
	//------------------------------------------
	// 1500000沥档绰 促弗单 荤侩等促绊 夯促.
	// 乞闭利牢 茄 Texture狼 memory size = 65536 - -;;
	//------------------------------------------
	int freeMemory = dwFree-1500000;

	if (freeMemory < 0) freeMemory = 0;

	int num = freeMemory / 20000 / 12;   	
	
	num = max(num, 30);	// 30焊促绰 目具茄促.
	num = min(num, 60);	// 60捞 弥绊促.
	
	//num = 20;

	// 犁汲沥.. - -;
	//g_pClientConfig->MAX_TEXTUREPART_EFFECT				= num*3/2;
	//g_pClientConfig->MAX_TEXTUREPART_CREATURESHADOW		= num*3;
	//g_pClientConfig->MAX_TEXTUREPART_IMAGEOBJECTSHADOW	= num;
	//g_pClientConfig->MAX_TEXTUREPART_ADDON_SHADOW			= 240 + num*7;

	//------------------------------------------------------------------------
	// 某腐磐 弊覆磊甫 公炼扒 八霸 免仿茄促绊 沁阑 锭狼 拌魂
	//------------------------------------------------------------------------
	g_pClientConfig->MAX_TEXTUREPART_EFFECT				= num*2;	//num*10;
	g_pClientConfig->MAX_TEXTUREPART_SCREENEFFECT		= num*5;		//num*10;
	g_pClientConfig->MAX_TEXTUREPART_CREATURESHADOW		= 1;//num*3;
	g_pClientConfig->MAX_TEXTUREPART_IMAGEOBJECTSHADOW	= num*2;
	g_pClientConfig->MAX_TEXTUREPART_ADDON_SHADOW		= 1;//240 + num*7;

	DEBUG_ADD_FORMAT("[MTopView::TexturePart] AlphaEffect = %d", g_pClientConfig->MAX_TEXTUREPART_EFFECT);
	DEBUG_ADD_FORMAT("[MTopView::TexturePart] ScreenEffect = %d", g_pClientConfig->MAX_TEXTUREPART_SCREENEFFECT);
	DEBUG_ADD_FORMAT("[MTopView::TexturePart] CreatureShadow = %d", g_pClientConfig->MAX_TEXTUREPART_CREATURESHADOW);
	DEBUG_ADD_FORMAT("[MTopView::TexturePart] ImageObjectShadow = %d", g_pClientConfig->MAX_TEXTUREPART_IMAGEOBJECTSHADOW);
	DEBUG_ADD_FORMAT("[MTopView::TexturePart] AddonShadow = %d", g_pClientConfig->MAX_TEXTUREPART_ADDON_SHADOW);
		


	if (InitSurfaces() &&
		InitColors() &&
//		Init3DBoxSurface() &&
		InitSprites() &&
		InitFilters() &&
		InitCreatureFrames() &&
		InitImageFrames() &&
		InitAnimationFrames() &&
		InitEffectFrames() &&
		InitFonts())
	{
		//------------------------------------------
		// 利例茄 Texture Memory 拌魂
		//------------------------------------------
		#ifdef OUTPUT_DEBUG
			DDSCAPS2 ddsCaps2;
			DWORD dwTotal;
			DWORD dwFree;
			ZeroMemory(&ddsCaps2, sizeof(ddsCaps2)); 
			ddsCaps2.dwCaps = DDSCAPS_TEXTURE; 
			HRESULT hr = CDirectDraw::GetDD()->GetAvailableVidMem(&ddsCaps2, &dwTotal, &dwFree);

			DEBUG_ADD_FORMAT("[TextureMemory] After Init View = %d/%d", dwFree, dwTotal);			
		#endif

		m_bInit = true;
		
		PrecalculateAdvancementClassCreatureFrames();

		return true;
	}
	
	return false;
}

//----------------------------------------------------------------------
// Init Changes
//----------------------------------------------------------------------
// 2D <--> 3D 官拆锭 荤侩..
//----------------------------------------------------------------------
bool
MTopView::InitChanges()
{
	//------------------------------------------
	// 酒流 init登瘤 臼疽促搁 return
	//------------------------------------------
	if (!m_bInit)
	{
		return false;
	}

	if (InitSurfaces() &&
		InitColors() &&
//		Init3DBoxSurface() &&
		InitSprites() &&
		InitFilters() &&
		InitFonts())
	{
		//---------------------------------------------------------
		// 某腐磐 loading捞 窍唱档 救灯促绊 眉农秦敌促.
		// 弊覆磊 锭巩牢单.. 橙橙.. - -; 2001.10.6
		//---------------------------------------------------------
		if (g_pCreatureSpriteTable!=NULL)
		{
			int numSize = g_pCreatureSpriteTable->GetSize();

			for (int i=0; i<numSize; i++)
			{
				(*g_pCreatureSpriteTable)[i].bLoad = false;
			}
		}

//		if (g_pAddonSpriteTable!=NULL)
//		{
//			int numSize = g_pAddonSpriteTable->GetSize();
//
//			for (int i=0; i<numSize; i++)
//			{
//				(*g_pAddonSpriteTable)[i].bLoad = false;
//			}
//		}

//		if (g_pCreatureActionSpriteTable!=NULL)
//		{
//			int numSize = g_pCreatureActionSpriteTable->GetSize();
//
//			for (int i=0; i<numSize; i++)
//			{
//				(*g_pCreatureActionSpriteTable)[i].bLoad = false;
//			}
//		}

		return true;
	}
	
	return false;
}

//----------------------------------------------------------------------
// Init Fonts
//----------------------------------------------------------------------
bool
MTopView::InitFonts()
{	
	//------------------------------------------
	// UI狼 Font殿废
	//------------------------------------------	
	g_ClientPrintInfo[FONTID_INFO]			= &gpC_base->m_info_pi;			// info
	g_ClientPrintInfo[FONTID_SLAYER_NAME]	= &gpC_base->m_char_name_pi;	// slayer
	g_ClientPrintInfo[FONTID_VAMPIRE_NAME]	= &gpC_base->m_char_name_pi;	// vampire
	g_ClientPrintInfo[FONTID_NPC_NAME]		= &gpC_base->m_char_name_pi;	// npc
	g_ClientPrintInfo[FONTID_SLAYER_CHAT]	= &gpC_base->m_char_chat_pi;	// slayer
	g_ClientPrintInfo[FONTID_VAMPIRE_CHAT]	= &gpC_base->m_char_chat_pi;	// vampire
	g_ClientPrintInfo[FONTID_NPC_CHAT]		= &gpC_base->m_char_chat_pi;	// npc
	g_ClientPrintInfo[FONTID_ITEM]			= &gpC_base->m_item_pi;			// item
	g_ClientPrintInfo[FONTID_LARGE_CHAT]	= &gpC_base->m_char_chat_large_pi;
	g_ClientPrintInfo[FONTID_USER_ID]		= &gpC_base->m_user_id_pi;
	return true;
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void
MTopView::Release()
{
	//------------------------------------------------
	// File 摧扁
	//------------------------------------------------
//	if (m_TileSPKFile.is_open())
//	{
//		m_TileSPKFile.close();
//	}
//
//	if (m_ImageObjectSPKFile.is_open())
//	{
//		m_ImageObjectSPKFile.close();
//	}
//
//	if (m_EffectAlphaSPKFile.is_open())
//	{
//		m_EffectAlphaSPKFile.close();
//	}
//
//	if (m_EffectScreenSPKFile.is_open())
//	{
//		m_EffectScreenSPKFile.close();
//	}
//
//	if (m_AddonSPKFile.is_open())
//	{
//		m_AddonSPKFile.close();
//	}
//
//	if (m_AddonSSPKFile.is_open())
//	{
//		m_AddonSSPKFile.close();
//	}
//
//	if (m_CreatureSPKFile.is_open())
//	{
//		m_CreatureSPKFile.close();
//	}
//
//	if (m_CreatureSSPKFile.is_open())
//	{
//		m_CreatureSSPKFile.close();
//	}
//
	DEBUG_ADD("MTV-Rel-File closed");

	//----------------------------------------------------------------------
	// Tile Surface力芭
	//----------------------------------------------------------------------
	if (m_pTileSurface != NULL)
	{
		delete m_pTileSurface;
		m_pTileSurface = NULL;

		DEBUG_ADD("MTV-Rel-TileSur");
	}	
	
	// imageobject TextureManager
	//if (m_pImageObjectTextureManager!=NULL)
	//{
	//	delete m_pImageObjectTextureManager;
	//	m_pImageObjectTextureManager = NULL;
	//}
#ifdef __3D_IMAGE_OBJECT__
	SAFE_DELETE( m_pImageObjectTextureManager );
#endif

	DEBUG_ADD("MTV-Rel-StartRelTex");

	//----------------------------------------------------------------------
	// Texture Manager
	//----------------------------------------------------------------------
//	if (m_pAlphaEffectTextureManager!=NULL)
//	{
//		delete m_pAlphaEffectTextureManager;
//		m_pAlphaEffectTextureManager = NULL;
//		DEBUG_ADD("MTV-Rel-AlphaTex");
//	}	
//
//	if (m_pScreenEffectTextureManager!=NULL)
//	{
//		delete m_pScreenEffectTextureManager;
//		m_pScreenEffectTextureManager = NULL;
//		DEBUG_ADD("MTV-Rel-ScreenTex");
//	}	
//
//	//----------------------------------------------------------------------
//	// Shadow Manager
//	//----------------------------------------------------------------------
//	if (m_pEffectShadowManager!=NULL)
//	{
//		delete m_pEffectShadowManager;
//		m_pEffectShadowManager = NULL;
//		DEBUG_ADD("MTV-Rel-EffectShadow");
//	}	

//	if (m_pImageObjectShadowManager!=NULL)
//	{
//		delete m_pImageObjectShadowManager;
//		m_pImageObjectShadowManager = NULL;
//		DEBUG_ADD("MTV-Rel-IMShadow");
//	}	
//
//	if (m_pAddonShadowManager!=NULL)
//	{
//		delete m_pAddonShadowManager;
//		m_pAddonShadowManager = NULL;
//		DEBUG_ADD("MTV-Rel-AddonShadow");
//	}
//
//	if (m_pOustersShadowManager!=NULL)
//	{
//		delete m_pOustersShadowManager;
//		m_pOustersShadowManager = NULL;
//		DEBUG_ADD("MTV-Rel-OustersShadow");
//	}
//	
//	if (m_pCreatureShadowManager!=NULL)
//	{
//		delete m_pCreatureShadowManager;
//		m_pCreatureShadowManager = NULL;
//		DEBUG_ADD("MTV-Rel-CreatureShadow");
//	}

	//----------------------------------------------------------------------
	// LightBuffer Texture
	//----------------------------------------------------------------------
	if (m_pLightBufferTexture!=NULL)
	{
		delete m_pLightBufferTexture;
		m_pLightBufferTexture = NULL;
		DEBUG_ADD("MTV-Rel-LightBuffer");
	}	

	//----------------------------------------------------------------------
	// 3D Box Texture Surface
	//----------------------------------------------------------------------
//	if (m_p3DBoxSurface!=NULL)
//	{
//		delete m_p3DBoxSurface;
//		m_p3DBoxSurface = NULL;
//		DEBUG_ADD("MTV-Rel-3DBox");
//	}	
//
//	if (m_p3DBoxBlackSurface!=NULL)
//	{
//		delete m_p3DBoxBlackSurface;
//		m_p3DBoxBlackSurface = NULL;
//		DEBUG_ADD("MTV-Rel-3DBoxBl");
//	}	
//
//	if (m_p3DBoxHPSurface!=NULL)
//	{
//		delete m_p3DBoxHPSurface;
//		m_p3DBoxHPSurface = NULL;
//		DEBUG_ADD("MTV-Rel-3DBoxHP");
//	}	
//	
//	//----------------------------------------------------------------------
//	// Minimap
//	//----------------------------------------------------------------------
//	// 2001.7.14 林籍贸府
//	if (m_pMinimapSPR != NULL)
//	{
//		delete m_pMinimapSPR;
//		m_pMinimapSPR = NULL;
//	}
//
//	if (m_pMinimapTexture != NULL)
//	{
//		delete m_pMinimapTexture;
//		m_pMinimapTexture = NULL;
//	}
	

	//----------------------------------------------------------------------
	// 2D Light Pixel
	//----------------------------------------------------------------------
	if (m_p2DLightPixelWidth!=NULL)
	{
		delete [] m_p2DLightPixelWidth;
		m_p2DLightPixelWidth = NULL;
		DEBUG_ADD("MTV-Rel-2DLightPixelW");
	}

	if (m_p2DLightPixelHeight!=NULL)
	{
		delete [] m_p2DLightPixelHeight;
		m_p2DLightPixelHeight = NULL;
		DEBUG_ADD("MTV-Rel-2DLightPixelH");
	}

	DEBUG_ADD("MTV-Rel-ClearList");

	//----------------------------------------------------------------------
	// list , map 力芭
	//----------------------------------------------------------------------
	ClearItemNameList();
	ClearOutputCreature();	
	ClearTextList();

	DEBUG_ADD("MTV-Rel-Loaded");

	m_listLoadedCreatureSprite.Release();
	m_listLoadedMonsterSprite.Release();
	m_mapImageObject.clear();

	//----------------------------------------------------------------------
	// FPK 瘤快扁
	//----------------------------------------------------------------------
	DEBUG_ADD("MTV-Rel-FPK");

	m_CreatureFPK.Release();			// Creature frames
	m_AddonFPK.Release();				// Player addon frames
	m_OustersFPK.Release();				// Player addon frames
	m_ItemTileFPK.Release();			// Tile俊 乐绰 Item俊 措茄..
	m_ItemDropFPK.Release();			// 官蹿栏肺 冻绢瘤绰 Item俊 措茄..
	m_ImageObjectFPK.Release();		// ImageObject俊 措茄 frames
	m_ImageObjectShadowFPK.Release();

	// 铰柳 包访 fpk 瘤快扁
	m_AdvancementSlayerManFPK.Release();
	m_AdvancementSlayerWomanFPK.Release();
	m_AdvancementVampireManFPK.Release();
	m_AdvancementVampireWomanFPK.Release();
	m_AdvancementOustersFPK.Release();
	m_AdvancementSlayerManShadowFPK.Release();
	m_AdvancementSlayerWomanShadowFPK.Release();
	m_AdvancementVampireManShadowFPK.Release();
	m_AdvancementVampireWomanShadowFPK.Release();
	m_AdvancementOustersShadowFPK.Release();
//	m_InteractionObjectFPK.Release();		// ImageObject俊 措茄 frames

	DEBUG_ADD("MTV-Rel-EffectFPK");

	m_EffectAlphaFPK.Release();		// Effect俊 措茄 frames
	m_EffectScreenFPK.Release();
	m_EffectShadowFPK.Release();		// Effect俊 措茄 frames
	m_EffectNormalFPK.Release();		// Effect俊 措茄 frames
		
	DEBUG_ADD("MTV-Rel-CFPK");

	m_CreatureShadowFPK.Release();			// Creature frames
	m_AddonShadowFPK.Release();			// Player addon frames	
	m_OustersShadowFPK.Release();			// Player addon frames	
		
	//------------------------------------------------------
	// SpritePack 力芭
	//------------------------------------------------------		
	DEBUG_ADD("MTV-Rel-SPK");

	m_TileSPK.Release();				// Tile
	m_ImageObjectSPK.Release();		// ImageObject Sprite甸
//	m_InteractionObjectSPK.Release();		// ImageObject Sprite甸
	m_CreatureSPK.Release();			// Creature Sprite甸
	m_AddonSPK.Release();		// Sprite甸		
	m_OustersSPK.Release();
	
	DEBUG_ADD("MTV-Rel-ItemSPK");

	m_ItemTileISPK.Release();			// Sprite甸		
	m_ItemDropISPK.Release();			//
	m_ItemBrokenSPK.Release();			//
	m_ItemRealSPK.Release();			// Sprite甸				

	DEBUG_ADD("MTV-Rel-EffectSPk");
	m_EffectAlphaSPK.Release();		// AlphaSprite甸
	m_EffectScreenSPK.Release();		// AlphaSprite甸
	m_EffectShadowSPK.Release();		// ShadowSprite甸
	m_EffectNormalSPK.Release();		// NormalSprite甸
	m_EffectScreenPPK.Release();
	m_EffectAlphaPPK.Release();

	DEBUG_ADD("MTV-Rel-Other");

	m_WeatherSPK.Release();			// WeatherSprite甸
	m_GuildSPK.Release();

	m_EtcSPK.Release();				// 扁鸥...
	m_OustersFinSPK.Release();
	m_AdvacementQuestEnding.Release();

	// 铰流 包访 SPK 力芭
	m_AdvancementSlayerManSPK.Release();
	m_AdvancementSlayerWomanSPK.Release();
	m_AdvancementVampireManSPK.Release();
	m_AdvancementVampireWomanSPK.Release();
	m_AdvancementOustersSPK.Release();

	m_AdvancementSlayerManSSPK.Release();
	m_AdvancementSlayerWomanSSPK.Release();
	m_AdvancementVampireManSSPK.Release();
	m_AdvancementVampireWomanSSPK.Release();
	m_AdvancementOustersSSPK.Release();

	//------------------------------------------------------		
	// Shadow SpritePack
	//------------------------------------------------------
	DEBUG_ADD("MTV-Rel-SSPK");

	m_AddonSSPK.Release();			// Sprite甸		
	m_OustersSSPK.Release();			// Sprite甸		
	m_ImageObjectSSPK.Release();
//	m_InteractionObjectSSPK.Release();
	m_CreatureSSPK.Release();

	//----------------------------------------------------------------------
	// File Index Table 瘤快扁
	//----------------------------------------------------------------------
	DEBUG_ADD("MTV-Rel-SSPKI");

	m_TileSPKI.Release();
	m_ImageObjectSPKI.Release();
//	m_EffectAlphaSPKI.Release();
//	m_EffectScreenSPKI.Release();

	//----------------------------------------------------------------------
	// Filter
	//----------------------------------------------------------------------
	DEBUG_ADD("MTV-Rel-Filter");

	m_ImageObjectFilter.Release();
	m_LightFTP.Release();
	m_LightBufferFilter.Release();;

	//----------------------------------------------------------------------
	// SFP Array力芭
	//----------------------------------------------------------------------
	if (m_pTileSFPArrayLargeZone != NULL)
	{
		delete m_pTileSFPArrayLargeZone;
		m_pTileSFPArrayLargeZone = NULL;
		DEBUG_ADD("MTV-Rel-TileSFPAL");
	}

	if (m_pImageObjectSFPArrayLargeZone != NULL)
	{
		delete m_pImageObjectSFPArrayLargeZone;
		m_pImageObjectSFPArrayLargeZone = NULL;
		DEBUG_ADD("MTV-Rel-IMSFPAL");
	}

	if (m_pTileSFPArraySmallZone != NULL)
	{
		delete m_pTileSFPArraySmallZone;
		m_pTileSFPArraySmallZone = NULL;
		DEBUG_ADD("MTV-Rel-TileSFPS");
	}

	if (m_pImageObjectSFPArraySmallZone != NULL)
	{
		delete m_pImageObjectSFPArraySmallZone;
		m_pImageObjectSFPArraySmallZone = NULL;
		DEBUG_ADD("MTV-Rel-IMSFPS");
	}

	m_bInit = false;

	DEBUG_ADD("MTV-Rel-All OK");
}

//----------------------------------------------------------------------
// 免仿 措惑捞 登绰 Surface甫 搬沥茄促.
//----------------------------------------------------------------------
void	
MTopView::SetSurface(CSpriteSurface*& pSurface)
{
	m_pSurface = pSurface;
}

//----------------------------------------------------------------------
// Clear ShadowManager
//----------------------------------------------------------------------
void
MTopView::ClearShadowManager()
{
	if (CDirect3D::IsHAL())
	{
//		if (m_pImageObjectShadowManager!=NULL) m_pImageObjectShadowManager->Clear();
//		if (m_pAddonShadowManager!=NULL) m_pAddonShadowManager->Clear();
//		if (m_pOustersShadowManager!=NULL) m_pOustersShadowManager->Clear();
//		if (m_pCreatureShadowManager!=NULL) m_pCreatureShadowManager->Clear();
	}
}

//----------------------------------------------------------------------
// Set 3D Box Color
//----------------------------------------------------------------------
//void			
//MTopView::Set3DBoxColor(WORD pixel)
//{
//	#ifdef OUTPUT_DEBUG_DRAW_3DBOX
//		DEBUG_ADD_FORMAT("Set3DBoxColor = %x", pixel);
//	#endif
//
//	if (m_p3DBoxSurface->Lock())
//	{	
//		WORD *lpSurface = (WORD*)m_p3DBoxSurface->GetSurfacePointer();
//		WORD *lpSurfaceTemp = lpSurface;
//		*lpSurfaceTemp = pixel;
//		m_p3DBoxSurface->Unlock();
//		
//		m_3DBoxCurrentPixel = pixel;
//
//		#ifdef OUTPUT_DEBUG_DRAW_3DBOX
//			DEBUG_ADD("OK");
//		#endif
//	}
//}
//
////----------------------------------------------------------------------
//// Init 3D BoxSurface
////----------------------------------------------------------------------
//bool
//MTopView::Init3DBoxSurface()
//{
//	//-----------------------------------------------
//	// 3D Box Texture Surface
//	//-----------------------------------------------
//	if (CDirect3D::IsHAL())
//	{	
//		//-----------------------------------------------
//		// 八篮祸
//		//-----------------------------------------------
//		if (m_p3DBoxBlackSurface!=NULL)
//		{
//			if (m_p3DBoxBlackSurface->Lock())
//			{	
//				WORD *lpSurface = (WORD*)m_p3DBoxBlackSurface->GetSurfacePointer();
//				*lpSurface = m_ColorBlackHalf;
//				m_p3DBoxBlackSurface->Unlock();
//			}
//		}
//
//		//-----------------------------------------------
//		// HP祸彬
//		//-----------------------------------------------
//		if (m_p3DBoxHPSurface!=NULL)
//		{
//			if (m_p3DBoxHPSurface->Lock())
//			{	
//				WORD *lpSurface = (WORD*)m_p3DBoxHPSurface->GetSurfacePointer();
//				*lpSurface = m_ColorHPBar;
//				m_p3DBoxHPSurface->Unlock();
//			}
//		}
//	}
//
//	return true;
//}

//----------------------------------------------------------------------
// Restore Surface
//----------------------------------------------------------------------
void	
MTopView::RestoreSurface()
{
	m_bDrawRequest = false;

	//for (int i=0; i<4; i++)
	{
		CDirectDraw::RestoreAllSurfaces();

		if (CDirect3D::IsHAL())
		{
			CDirect3D::Restore();

			/*
			D3DRECT rect3d;
			rect3d.x1 = 0;
			rect3d.y1 = 0;
			rect3d.x2 = CLIPSURFACE_WIDTH;
			rect3d.y2 = CLIPSURFACE_HEIGHT;			

			CDirect3D::GetDevice()->Clear( 1UL, (D3DRECT *)&rect3d, 
										D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
										0x00000000, 1.0f, 0L );
			*/
		}

	//	CDirectDraw::Flip();		
	}

	if (m_pSurface!=NULL)
	{
		m_pSurface->Restore();
	}

	if (m_pTileSurface!=NULL)
	{
		m_pTileSurface->Restore();
	}
	
	// 葛滴 促矫 弊妨霖促.
	//m_bFirstTileDraw = true;
	
	ClearOutputCreature();

	ClearItemNameList();

	//------------------------------------------------------------
	// 啊加阑 荤侩窍绰 版快俊绰 
	// Texture啊 VideoMemory俊 棵扼啊 乐栏骨肺 Restore秦具 茄促.
	// 促矫 Load.. - -;;
	//------------------------------------------------------------
	if (CDirect3D::IsHAL())
	{	
		//------------------------------------------------------------
		// Part Manager
		//------------------------------------------------------------
		DEBUG_ADD("Restore Surfaces - m_pAlphaEffectTextureManager");
		
#ifdef __3D_IMAGE_OBJECT__
//		if( m_pImageObjectTextureManager != NULL )
//			m_pImageObjectTextureManager->Clear();
#endif

//		if (m_pAlphaEffectTextureManager!=NULL)
//		{
//			m_pAlphaEffectTextureManager->Clear();
//		}
//
//		if (m_pScreenEffectTextureManager!=NULL)
//		{
//			m_pScreenEffectTextureManager->Clear();
//		}

//		if (m_pImageObjectShadowManager!=NULL)
//		{
//			m_pImageObjectShadowManager->Clear();
//		}
		
//		if (m_pEffectShadowManager!=NULL)
//		{
//			m_pEffectShadowManager->Clear();
//		}
//
//		if (m_pAddonShadowManager!=NULL)
//		{
//			m_pAddonShadowManager->Clear();
//		}
//
//		if (m_pOustersShadowManager!=NULL)
//		{
//			m_pOustersShadowManager->Clear();
//		}
//		
//		if (m_pCreatureShadowManager!=NULL)
//		{
//			m_pCreatureShadowManager->Clear();
//		}

		//------------------------------------------------------------
		// Light Buffer Texture
		//------------------------------------------------------------
		DEBUG_ADD("Restore Surfaces - m_pLightBufferTexture");
		
		InitFilters();
			/*
		if (m_pLightBufferTexture!=NULL)
		{
			m_pLightBufferTexture->Restore();
			//delete m_pLightBufferTexture;
		}
		*/

		//m_pLightBufferTexture = new CSpriteSurface;
		//m_pLightBufferTexture->InitTextureSurface(SCREENLIGHT_WIDTH, SCREENLIGHT_HEIGHT, 0, CDirect3D::GetPixelFormat4444());
		//m_pLightBufferTexture->Restore();

//		if (m_p3DBoxSurface!=NULL)
//		{
//			//m_p3DBoxSurface->Restore();
//			delete m_p3DBoxSurface;	
//			m_p3DBoxSurface = NULL;
//		}
//
//		if (m_p3DBoxBlackSurface!=NULL)
//		{
//			//m_p3DBoxBlackSurface->Restore();
//			delete m_p3DBoxBlackSurface;	
//			m_p3DBoxBlackSurface = NULL;
//		}
//
//		if (m_p3DBoxHPSurface!=NULL)
//		{
//			//m_p3DBoxHPSurface->Restore();
//			delete m_p3DBoxHPSurface;	
//			m_p3DBoxHPSurface = NULL;
//		}


		//------------------------------------------------------------
		// 3DBox Surface
		//------------------------------------------------------------
		DEBUG_ADD("Restore Surfaces - m_p3DBoxSurface");
		
//		m_p3DBoxSurface = new CSpriteSurface;
//		m_p3DBoxSurface->InitTextureSurface(1, 1, 0, CDirect3D::GetPixelFormat4444());
//		m_p3DBoxSurface->Restore();
//
//		m_p3DBoxBlackSurface = new CSpriteSurface;
//		m_p3DBoxBlackSurface->InitTextureSurface(1, 1, 0, CDirect3D::GetPixelFormat4444());
//		m_p3DBoxBlackSurface->Restore();
//
//		m_p3DBoxHPSurface = new CSpriteSurface;
//		m_p3DBoxHPSurface->InitTextureSurface(1, 1, 0, CDirect3D::GetPixelFormat4444());
//		m_p3DBoxHPSurface->Restore();
//		
//		Set3DBoxColor( m_3DBoxCurrentPixel );
//
//		Init3DBoxSurface();

		//------------------------------------------------------------
		// minimap texture檬扁拳
		//------------------------------------------------------------
		// 2001.7.14 林籍贸府
//		if (m_pMinimapSPR!=NULL && m_pMinimapTexture!=NULL)
//		{
//			m_pMinimapTexture->Restore();
////			InitMinimapTexture();
//		}		
	}
	/*
	if (CDirect3D::IsHAL())
	{
		//------------------------------------------------------------
		//
		// Load  EffectPack
		//
		//------------------------------------------------------------
		class ifstream	effectFile2(FILE_ASPRITE_ALPHAEFFECT, ios::binary);
		m_EffectAlphaSPK.LoadFromFile(effectFile2);
		effectFile2.close();	

		//------------------------------------------------------------
		//
		// Init Effect TextureSurface
		//
		//------------------------------------------------------------		
		//--------------------------------------------
		// AlphaSPK阑 捞侩秦辑 TexturePack阑 积己茄促.
		//--------------------------------------------
		if (!m_EffectTPK.Init( m_EffectAlphaSPK ))
		{
			MessageBox(g_hWnd, "Can't Init EffectTexture!", NULL, MB_OK);
			return;
		}

		//------------------------------------------------------------	
		// EffectAlphaSprite甫 皋葛府俊辑 昏力茄促.
		// --> TextureSurface甫 荤侩且 巴捞骨肺.. 鞘夸啊 绝促.
		//------------------------------------------------------------		
		m_EffectAlphaSPK.Release();	
	}
	*/
}


//----------------------------------------------------------------------
// 荤侩且 Surface甸阑 檬扁拳 矫挪促.
//----------------------------------------------------------------------
bool
MTopView::InitSurfaces()
{
	//-----------------------------------------------
	// Tile Surface
	//-----------------------------------------------
	if (m_pTileSurface!=NULL)
	{
		delete m_pTileSurface;
	}

	m_pTileSurface = new CSpriteSurface;
		


	//
	// 3D啊加捞 登搁 VideoMemory俊.. (啊瓷窍搁. - -;)
	//
	// 救登搁.. system memory俊... 
	// (恐? 澜.. 绢凋啊俊 包访等 内靛啊 乐瘤 酵篮单 茫扁 蓖满促. 澜衬)
	//
	//----------------------------------------------------------------
	// 3D
	//----------------------------------------------------------------
	//-----------------------------------------------
	//
	//-----------------------------------------------
//	if (m_p3DBoxSurface!=NULL)
//	{
//		delete m_p3DBoxSurface;	
//		m_p3DBoxSurface = NULL;
//	}
//
//	//-----------------------------------------------
//	//
//	//-----------------------------------------------
//	if (m_p3DBoxBlackSurface!=NULL)
//	{
//		delete m_p3DBoxBlackSurface;	
//		m_p3DBoxBlackSurface = NULL;
//	}
//
//
//	//-----------------------------------------------
//	//
//	//-----------------------------------------------
//	if (m_p3DBoxHPSurface!=NULL)
//	{
//		delete m_p3DBoxHPSurface;	
//		m_p3DBoxHPSurface = NULL;
//	}


	if (CDirect3D::IsHAL())
	{
		// 老窜 video memory俊 积己捞 登档废 秦焊绊..
		// 救登搁 system memory俊 积己茄促.
//		if (!m_pTileSurface->InitOffsurface(TILESURFACE_WIDTH, TILESURFACE_HEIGHT))
		{
			DEBUG_ADD("[ InitGame ]  MTopView::InitSurface() - TileSurface vidmem failed");
			
			m_pTileSurface->InitOffsurface(g_TILESURFACE_WIDTH, 
											g_TILESURFACE_HEIGHT, 
											DDSCAPS_SYSTEMMEMORY);
			
			DEBUG_ADD("[ InitGame ]  MTopView::InitSurface() - TileSurface sysmem OK");			
		}
		
		//-----------------------------------------------
		// free
		//-----------------------------------------------
//		m_p3DBoxSurface = new CSpriteSurface;
//		m_p3DBoxSurface->InitTextureSurface(1, 1, 0, CDirect3D::GetPixelFormat4444());
//
//		Set3DBoxColor( m_3DBoxCurrentPixel );
//
//		m_p3DBoxBlackSurface = new CSpriteSurface;
//		m_p3DBoxBlackSurface->InitTextureSurface(1, 1, 0, CDirect3D::GetPixelFormat4444());
//		m_p3DBoxBlackSurface->Restore();
//
//		m_p3DBoxHPSurface = new CSpriteSurface;
//		m_p3DBoxHPSurface->InitTextureSurface(1, 1, 0, CDirect3D::GetPixelFormat4444());
//		m_p3DBoxHPSurface->Restore();


	}
	//----------------------------------------------------------------
	// 2D
	//----------------------------------------------------------------
	else 
	{
		m_pTileSurface->InitOffsurface(g_TILESURFACE_WIDTH, 
									g_TILESURFACE_HEIGHT,
									DDSCAPS_SYSTEMMEMORY);
	}

	//m_pTileSurface->InitOffsurface(TILESURFACE_WIDTH, 
	//								TILESURFACE_HEIGHT,
	//								DDSCAPS_SYSTEMMEMORY);
	//,(CDirect3D::IsHAL()? 0 : DDSCAPS_SYSTEMMEMORY));

	m_pTileSurface->SetTransparency( 0 );



	UI_DrawProgress(1);
	DrawTitleLoading();
	
	return true;
}

//----------------------------------------------------------------------
// Init Colors
//----------------------------------------------------------------------
// 祸彬甸阑 檬扁拳 茄促.
//----------------------------------------------------------------------
bool
MTopView::InitColors()
{
	//---------------------------------------------------
	// 疵 鞍篮 祸
	//---------------------------------------------------
	// item
	m_ColorNameItem					= g_pClientConfig->COLOR_NAME_ITEM;
	m_ColorNameItemOption			= g_pClientConfig->COLOR_NAME_ITEM_OPTION;

	// 辆练喊
	m_ColorNameVampire				= g_pClientConfig->COLOR_NAME_VAMPIRE;
	m_ColorNameSlayer				= g_pClientConfig->COLOR_NAME_SLAYER;
	m_ColorNameNPC					= g_pClientConfig->COLOR_NAME_NPC;
	
	m_ColorNameAlignment[0]			= g_pClientConfig->COLOR_NAME_EVIL_MORE;
	m_ColorNameAlignment[1]			= g_pClientConfig->COLOR_NAME_EVIL;
	m_ColorNameAlignment[2]			= g_pClientConfig->COLOR_NAME_NEUTRAL;
	m_ColorNameAlignment[3]			= g_pClientConfig->COLOR_NAME_GOOD;
	m_ColorNameAlignment[4]			= g_pClientConfig->COLOR_NAME_GOOD_MORE;	


	//---------------------------------------------------
	// 5:6:5牢 版快绰 官肺 蔼阑 佬栏搁 等促.
	//---------------------------------------------------
	if (CDirectDraw::Is565())
	{
		// interactionObject
//		m_ColorOutlineInteractionObject	= g_pClientConfig->COLOR_OUTLINE_INTERACTIONOBJECT;

		m_ColorOutlineItem				= g_pClientConfig->COLOR_OUTLINE_ITEM;

		// 傍拜 啊瓷?
		m_ColorOutlineNPC				= g_pClientConfig->COLOR_OUTLINE_NPC;
		m_ColorOutlineAttackPossible	= g_pClientConfig->COLOR_OUTLINE_ATTACK_POSSIBLE;
		m_ColorOutlineAttackImpossible	= g_pClientConfig->COLOR_OUTLINE_ATTACK_IMPOSSIBLE;	
	}
	//---------------------------------------------------
	// 5:5:5牢 版快绰 蔼阑 函券
	//---------------------------------------------------
	else
	{
		// interactionObject
//		m_ColorOutlineInteractionObject	= CDirectDraw::Convert565to555(g_pClientConfig->COLOR_OUTLINE_INTERACTIONOBJECT);

		// item
		m_ColorOutlineItem				= CDirectDraw::Convert565to555(g_pClientConfig->COLOR_OUTLINE_ITEM);

		// 傍拜 啊瓷?
		m_ColorOutlineNPC				= CDirectDraw::Convert565to555(g_pClientConfig->COLOR_OUTLINE_NPC);
		m_ColorOutlineAttackPossible	= CDirectDraw::Convert565to555(g_pClientConfig->COLOR_OUTLINE_ATTACK_POSSIBLE);
		m_ColorOutlineAttackImpossible	= CDirectDraw::Convert565to555(g_pClientConfig->COLOR_OUTLINE_ATTACK_IMPOSSIBLE);
	}


	//---------------------------------------------------
	// 3D 啊加苞 包访捞 乐绰 祸彬...
	//---------------------------------------------------
//	if (CDirect3D::IsHAL())
//	{	
//		// 4:4:4:4 format
//		m_ColorHPBar = 0xA000 
//						| ((g_pClientConfig->COLOR_HP_BAR_R >> 1) << 8)
//						| ((g_pClientConfig->COLOR_HP_BAR_G >> 1) << 4)
//						| (g_pClientConfig->COLOR_HP_BAR_B >> 1);			
//
//		m_ColorBlackHalf = 0xA000;
//	}
//	else
	{
		m_ColorHPBar = CDirectDraw::Color(g_pClientConfig->COLOR_HP_BAR_R,
											g_pClientConfig->COLOR_HP_BAR_G,
											g_pClientConfig->COLOR_HP_BAR_B);

		m_ColorBlackHalf = 0;
	}

	return true;
}
	

//----------------------------------------------------------------------
// SpritePack阑 佬绢辑  memory俊 Load茄促.
//----------------------------------------------------------------------
bool
MTopView::InitSprites()
{
	// Sprite
	/*
	WORD		spriteID = 0;
		
	WORD	*lpSurface, 
			*lpSurfaceTemp,
			*lpSurfaceTemp2;

	WORD	lPitch;

	int i,j;

	// Temp Surface	
	CSpriteSurface TempSurface;
	*/

	

	/*
	m_ImageObjectSSPK.Init( 42 );

	CShadowSprite::SetColorkey( 0x001F );
	//------------------------------------------------------------
	// Shadow
	//------------------------------------------------------------
	TempSurface.InitFromBMP("BuildingShadow0.bmp", 1024,768, DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);	
	m_ImageObjectSSPK[ 0 ].SetPixel(lpSurface, lPitch, 874, 360);
	TempSurface.Unlock();

	TempSurface.InitFromBMP("BuildingShadow1.bmp", 1024,768, DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);	
	m_ImageObjectSSPK[ 1 ].SetPixel(lpSurface, lPitch, 906, 494);
	TempSurface.Unlock();

	TempSurface.InitFromBMP("BuildingShadow2.bmp", 1024,768, DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);	
	m_ImageObjectSSPK[ 2 ].SetPixel(lpSurface, lPitch, 928, 436);
	TempSurface.Unlock();

	TempSurface.InitFromBMP("BuildingShadow3.bmp", 1024,768, DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);	
	m_ImageObjectSSPK[ 3 ].SetPixel(lpSurface, lPitch, 804, 502);	

	for (i=4; i<=31; i++)
	{
		m_ImageObjectSSPK[ i ].SetPixel(lpSurface, lPitch, 804, 502);	
	}

	TempSurface.Unlock();

	TempSurface.InitFromBMP("ImageObjectSomeShadow.bmp", 800,600, DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);	
	lpSurfaceTemp = lpSurface;
	m_ImageObjectSSPK[ 32 ].SetPixel(lpSurfaceTemp, lPitch, 87, 82);
	lpSurfaceTemp = lpSurface + 89;
	m_ImageObjectSSPK[ 33 ].SetPixel(lpSurfaceTemp, lPitch, 47, 50);
	lpSurfaceTemp = lpSurface + 157;
	m_ImageObjectSSPK[ 34 ].SetPixel(lpSurfaceTemp, lPitch, 58, 50);
	m_ImageObjectSSPK[ 35 ].SetPixel(lpSurfaceTemp, lPitch, 58, 50);

	lpSurfaceTemp = lpSurface + 223;
	m_ImageObjectSSPK[ 36 ].SetPixel(lpSurfaceTemp, lPitch, 49, 50);	
	m_ImageObjectSSPK[ 38 ].SetPixel(lpSurfaceTemp, lPitch, 49, 50);
	m_ImageObjectSSPK[ 40 ].SetPixel(lpSurfaceTemp, lPitch, 49, 50);
	
	lpSurfaceTemp = lpSurface + 286;
	m_ImageObjectSSPK[ 37 ].SetPixel(lpSurfaceTemp, lPitch, 51, 41);
	m_ImageObjectSSPK[ 39 ].SetPixel(lpSurfaceTemp, lPitch, 51, 41);	
	m_ImageObjectSSPK[ 41 ].SetPixel(lpSurfaceTemp, lPitch, 51, 41);
	TempSurface.Unlock();


	//------------------------------------------------------------	
	// Save  ImageObject Shadow SpritePack	
	//------------------------------------------------------------
	class ofstream	ImageObjectSFile(FILE_SSPRITE_IMAGEOBJECT, ios::binary);	
	class ofstream	ImageObjectSIndexFile(FILE_SSPRITEINDEX_IMAGEOBJECT, ios::binary);	

	m_ImageObjectSSPK.SaveToFile(ImageObjectSFile, ImageObjectSIndexFile);

	ImageObjectSFile.close();
	ImageObjectSIndexFile.close();
	*/

	//------------------------------------------------------------	
	// Load  ImageObject ShadowSpritePack	
	//------------------------------------------------------------
	//------------------------------------------------------------
	//
	// Load  EffectPack
	//
	//------------------------------------------------------------
	// 3d啊加捞 登搁 m_pImageObjectShadowManager甫 荤侩窍绊
	// 酒聪搁, m_ImageObjectSSPK甫 荤侩茄促.
	/*
	class ifstream	ImageObjectShadowFile2;//(FILE_SSPRITE_IMAGEOBJECT, ios::binary);
	if (!FileOpenBinary(FILE_SSPRITE_IMAGEOBJECT, ImageObjectShadowFile2))
		return false;
	m_ImageObjectSSPK.LoadFromFile(ImageObjectShadowFile2);
	ImageObjectShadowFile2.close();
	*/
//	if (m_pImageObjectShadowManager!=NULL)
//	{
//		delete m_pImageObjectShadowManager;
//		m_pImageObjectShadowManager = NULL;
//	}		
//
//	//m_ImageObjectSSPK.Release();
//
//	if (CDirect3D::IsHAL())
//	{	
//		//m_pAlphaEffectTextureManager = new CTexturePartManager( FILE_ASPRITE_ALPHAEFFECT, FILE_ASPRITEINDEX_ALPHAEFFECT, 70 );
//		//m_pImageObjectShadowManager = new CShadowPartManager( 
//		//										&m_ImageObjectSSPK, 
//		//										g_pClientConfig->MAX_TEXTUREPART_IMAGEOBJECTSHADOW );
//		m_pImageObjectShadowManager = new CShadowPartManager(g_pFileDef->getProperty("FILE_SSPRITE_IMAGEOBJECT").c_str(),
////															FILE_SSPRITEINDEX_IMAGEOBJECT, 
//															g_pClientConfig->MAX_TEXTUREPART_IMAGEOBJECTSHADOW );
//	}
//	else
	{
// 		class ifstream	ImageObjectShadowFile2;//(FILE_SSPRITE_IMAGEOBJECT, ios::binary);
//		if (!FileOpenBinary(FILE_SSPRITE_IMAGEOBJECT, ImageObjectShadowFile2))
//			return false;
		m_ImageObjectSSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SSPRITE_IMAGEOBJECT").c_str());
// 		ImageObjectShadowFile2.close();
	}

//	UI_DrawProgress(2);
//	DrawTitleLoading();
	
	//------------------------------------------------------------
	//
	//
	//                  Creature SpritePack
	//
	//
	//------------------------------------------------------------	
	//------------------------------------------------------------	
	// Init  Creature SpritePack	
	//------------------------------------------------------------
	// 老窜 傈眉 农扁父怒 memory绰 棱酒敌促.
	//------------------------------------------------------------
	//  Init Creature SpriteSet
	//------------------------------------------------------------	
	if (m_CreatureSPK.GetSize()==0)
	{
		/*
		WORD size;
		class ifstream CreaturePackIndexFile;//(FILE_ISPRITEINDEX_CREATURE, ios::binary);
		if (!FileOpenBinary(FILE_ISPRITEINDEX_CREATURE, CreaturePackIndexFile))
			return false;
		CreaturePackIndexFile.read((char*)&size, 2);	// Sprite狼 俺荐
		CreaturePackIndexFile.close();	

		m_CreatureSPK.Init( size, CDirectDraw::Is565() );
		*/
//		if (m_CreatureSPKFile.is_open())
//		{
//			m_CreatureSPKFile.close();
//		}
//		
//		// 拳老父 凯绢敌促.
//		//------------------------------------------------------------
//		// sprite load
//		//------------------------------------------------------------
//		if (!FileOpenBinary(FILE_ISPRITE_CREATURE, m_CreatureSPKFile))
//			return false;	
//
//		TYPE_SPRITEID	numCreatureSPK;
//		m_CreatureSPKFile.read((char*)&numCreatureSPK, SIZE_SPRITEID);
//		m_CreatureSPK.Init( numCreatureSPK, CDirectDraw::Is565() );			
		m_CreatureSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_ISPRITE_CREATURE").c_str() );

	}

//	UI_DrawProgress(3);
//	DrawTitleLoading();

	//m_CreatureSSPK.Init( size );
	//m_CreatureTPK.Init( size );
	
	//------------------------------------------------------------	
	//
	//			Creature Shadow SpritePack	
	//
	//------------------------------------------------------------	
	/*
	m_CreatureSSPK.Init( 1 );

	CShadowSprite::SetColorkey( 0x001F );
	//------------------------------------------------------------
	// Shadow
	//------------------------------------------------------------
	TempSurface.InitFromBMP("CreatureShadow.bmp", DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);	
	m_CreatureSSPK[ 0 ].SetPixel(lpSurface, lPitch, TempSurface.GetWidth(), TempSurface.GetHeight());
	TempSurface.Unlock();

	//------------------------------------------------------------	
	// Save  Creature SpritePack	
	//------------------------------------------------------------
	class ofstream	creatureShadowFile(FILE_SSPRITE_CREATURE, ios::binary);	
	class ofstream	creatureShadowIndexFile(FILE_SSPRITEINDEX_CREATURE, ios::binary);
	m_CreatureSSPK.SaveToFile(creatureShadowFile, creatureShadowIndexFile);
	creatureShadowFile.close();
	creatureShadowIndexFile.close();
	*/

	//------------------------------------------------------------
	// 抛胶飘 : 老馆SpritePack --> ShadowSpritePack
	//------------------------------------------------------------
	// 巢
	/*
	CSpritePack CreatureShadowSPK;
	class ifstream	CreatureShadowFile2;//(FILE_ISPRITE_ADDON, ios::binary);
	if (!FileOpenBinary("Data\\Image\\CreatureShadow.spk", CreatureShadowFile2))
		return false;
	CreatureShadowSPK.LoadFromFile(CreatureShadowFile2);
	CreatureShadowFile2.close();

	m_CreatureSSPK.Init( CreatureShadowSPK.GetSize() );

	for (int i=0; i<CreatureShadowSPK.GetSize(); i++)
	{
		m_CreatureSSPK[i].SetPixel( CreatureShadowSPK[i] );
	}

	//------------------------------------------------------------	
	// 巢 - Save  ShadowSpritePack	
	//------------------------------------------------------------
	class ofstream	CreatureShadowFile(FILE_SSPRITE_CREATURE, ios::binary);	
	class ofstream	CreatureShadowIndexFile(FILE_SSPRITEINDEX_CREATURE, ios::binary);	

	m_CreatureSSPK.SaveToFile(CreatureShadowFile, CreatureShadowIndexFile);

	CreatureShadowFile.close();
	CreatureShadowIndexFile.close();
	*/

	//------------------------------------------------------------
	// Load Creature Shadow SpritePack
	//------------------------------------------------------------
	// 3d啊加捞 登搁 m_pCreatureShadowManager甫 荤侩窍绊
	// 酒聪搁, m_CreatureSSPK甫 荤侩茄促.
	//------------------------------------------------------------
	// 3d啊加捞 登搁 m_pAlphaEffectTextureManager甫 荤侩窍绊
	// 酒聪搁, m_EffectAlphaSPK甫 荤侩茄促.
	
	// 傈眉 俺荐父 棱酒敌促.
	/*
	class ifstream CreatureShadowPackIndexFile;//(FILE_ISPRITEINDEX_CREATURE, ios::binary);
	if (!FileOpenBinary(FILE_SSPRITEINDEX_CREATURE, CreatureShadowPackIndexFile))
		return false;
	CreatureShadowPackIndexFile.read((char*)&size, 2);	// Sprite狼 俺荐
	CreatureShadowPackIndexFile.close();	

	m_CreatureSSPK.Init( size );
	*/
	/*
	// 烙矫肺 Load
	// 傈眉 loading秦滴绰 何盒
	class ifstream	CreatureShadowFile2;//(FILE_SSPRITE_Creature, ios::binary);
	if (!FileOpenBinary(FILE_SSPRITE_CREATURE, CreatureShadowFile2))
		return false;
	m_CreatureSSPK.LoadFromFile(CreatureShadowFile2);
	CreatureShadowFile2.close();
	*/
//	if (m_CreatureSSPKFile.is_open())
//	{
//		m_CreatureSSPKFile.close();
//	}		
//
//	if (m_pCreatureShadowManager!=NULL)
//	{
//		delete m_pCreatureShadowManager;
//		m_pCreatureShadowManager = NULL;
//	}
//	//m_CreatureSSPK.Release();
//	
//	if (CDirect3D::IsHAL())
//	{	
//		//m_pAlphaEffectTextureManager = new CTexturePartManager( FILE_ASPRITE_ALPHAEFFECT, FILE_ASPRITEINDEX_ALPHAEFFECT, 70 );
//		//m_pCreatureShadowManager = new CShadowPartManager( 
//		//									&m_CreatureSSPK,  
//		//									g_pClientConfig->MAX_TEXTUREPART_CREATURESHADOW);
//		m_pCreatureShadowManager = new CShadowPartManager( 
//											g_pFileDef->getProperty("FILE_SSPRITE_CREATURE").c_str(),
////											FILE_SSPRITEINDEX_CREATURE,  
//											g_pClientConfig->MAX_TEXTUREPART_CREATURESHADOW);
//	}
//	else
	{
		/*
		class ifstream	CreatureShadowFile2;//(FILE_SSPRITE_Creature, ios::binary);
		if (!FileOpenBinary(FILE_SSPRITE_CREATURE, CreatureShadowFile2))
			return false;
		m_CreatureSSPK.LoadFromFile(CreatureShadowFile2);
		CreatureShadowFile2.close();
		*/
		/*
		class ifstream indexFile;//(indexFilename, ios::binary);
		if (!FileOpenBinary(FILE_SSPRITEINDEX_CREATURE, indexFile))
			return false;

		int packSize;
		indexFile.read((char*)&packSize, 2);
		indexFile.close();

		m_CreatureSSPK.Init( packSize );
		*/
		//------------------------------------------------------------
		// 弊覆磊 load
		//------------------------------------------------------------
//		if (!FileOpenBinary(FILE_SSPRITE_CREATURE, m_CreatureSSPKFile))
//			return false;
//
//		TYPE_SPRITEID	numCreatureSSPK;
//
//		m_CreatureSSPKFile.read((char*)&numCreatureSSPK, SIZE_SPRITEID);
// 		m_CreatureSSPK.Init( numCreatureSSPK );			
		m_CreatureSSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SSPRITE_CREATURE").c_str());
	}

//	UI_DrawProgress(4);
//	DrawTitleLoading();

	//------------------------------------------------------------
	//
	//
	//                   Clothes SpritePack
	//
	//
	//------------------------------------------------------------
	/*
	CSpritePack	ClothesSPK;
	ClothesSPK.Init( 160, CDirectDraw::Is565() );

	//------------------------------------------------------------
	// Shirts
	//------------------------------------------------------------
	TempSurface.InitFromBMP("Shirts.bmp", 640,480, DDSCAPS_SYSTEMMEMORY);
	TempSurface.LockW(lpSurface, lPitch);

	spriteID = 0;
	for (i=0; i<4; i++)
	{
		lpSurfaceTemp = lpSurface;
		for (j=0; j<10; j++)
		{
			ClothesSPK[spriteID++].SetPixel(lpSurfaceTemp, lPitch, 64, 64);

			lpSurfaceTemp += 64;
		}
		lpSurface = (WORD*)((BYTE*)lpSurface + lPitch*64);
	}
	TempSurface.Unlock();

	//------------------------------------------------------------
	// Pants
	//------------------------------------------------------------
	TempSurface.InitFromBMP("Pants.bmp", 640,480, DDSCAPS_SYSTEMMEMORY);
	TempSurface.LockW(lpSurface, lPitch);

	for (i=0; i<4; i++)
	{
		lpSurfaceTemp = lpSurface;
		for (j=0; j<10; j++)
		{
			ClothesSPK[spriteID++].SetPixel(lpSurfaceTemp, lPitch, 64, 64);

			lpSurfaceTemp += 64;
		}
		lpSurface = (WORD*)((BYTE*)lpSurface + lPitch*64);
	}
	TempSurface.Unlock();
	

	//------------------------------------------------------------
	// Boots
	//------------------------------------------------------------
	TempSurface.InitFromBMP("Boots.bmp", 640,480, DDSCAPS_SYSTEMMEMORY);
	TempSurface.LockW(lpSurface, lPitch);

	for (i=0; i<4; i++)
	{
		lpSurfaceTemp = lpSurface;
		for (j=0; j<10; j++)
		{
			ClothesSPK[spriteID++].SetPixel(lpSurfaceTemp, lPitch, 64, 64);

			lpSurfaceTemp += 64;
		}
		lpSurface = (WORD*)((BYTE*)lpSurface + lPitch*64);
	}
	TempSurface.Unlock();

	//------------------------------------------------------------
	// Cloak
	//------------------------------------------------------------
	TempSurface.InitFromBMP("Cloak.bmp", 640,480, DDSCAPS_SYSTEMMEMORY);
	TempSurface.LockW(lpSurface, lPitch);

	for (i=0; i<4; i++)
	{
		lpSurfaceTemp = lpSurface;
		for (j=0; j<10; j++)
		{
			ClothesSPK[spriteID++].SetPixel(lpSurfaceTemp, lPitch, 64, 64);

			lpSurfaceTemp += 64;
		}
		lpSurface = (WORD*)((BYTE*)lpSurface + lPitch*64);
	}
	TempSurface.Unlock();

	//------------------------------------------------------------	
	// Save  Clothes SpritePack	
	//------------------------------------------------------------
	class ofstream	clothesFile("Clothes.spk", ios::binary);	
	class ofstream	clothesIndexFile("Clothes.spki", ios::binary);	

	ClothesSPK.SaveToFile(clothesFile, clothesIndexFile);

	clothesFile.close();
	clothesIndexFile.close();
	*/	
	
	//------------------------------------------------------------	
	// Load  Clothes SpritePack	
	//------------------------------------------------------------
	// slayer
/*	
	if (m_AddonSPK.GetSize()==0)
	{
		class ifstream	AddonFile2;//(FILE_ISPRITE_ADDON, ios::binary);
		if (!FileOpenBinary(FILE_ISPRITE_ADDON, AddonFile2))
			return false;
		
		// 2001.8.20 林籍贸府..
		//m_AddonSPK.LoadFromFile(AddonFile2);
		//m_AddonSPK.Init( 20000, CDirectDraw::Is565() );
		
		// 俺荐父 棱酒敌促.
		TYPE_SPRITEID addonSize;
		AddonFile2.read((char*)&addonSize, SIZE_SPRITEID);
		m_AddonSPK.Init( addonSize, CDirectDraw::Is565() );
		AddonFile2.close();
	}
*/	
//	if (m_AddonSPKFile.is_open())
//	{
//		m_AddonSPKFile.close();
//	}
//
//	if (m_AddonSSPKFile.is_open())
//	{
//		m_AddonSSPKFile.close();
//	}
//
//	// 拳老父 凯绢敌促.
//	//------------------------------------------------------------
//	// sprite load
//	//------------------------------------------------------------
//	if (!FileOpenBinary(FILE_ISPRITE_ADDON, m_AddonSPKFile))
//		return false;	
//
//	if (m_AddonSPK.GetSize()==0)
//	{
//		TYPE_SPRITEID	numAddonSPK;
//		m_AddonSPKFile.read((char*)&numAddonSPK, SIZE_SPRITEID);
//		m_AddonSPK.Init( numAddonSPK, CDirectDraw::Is565() );	
//	}
	m_AddonSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_ISPRITE_ADDON").c_str() );
	m_OustersSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_ISPRITE_OUSTERS").c_str() );

	m_AdvancementSlayerManSPK.LoadFromFileRunning( g_pFileDef->getProperty( "FILE_ISPRITE_ADVANCEMENT_CLASS_SLAYER_MAN" ).c_str() );
	m_AdvancementSlayerManSSPK.LoadFromFileRunning( g_pFileDef->getProperty( "FILE_SSPRITE_ADVANCEMENT_CLASS_SLAYER_MAN" ).c_str() );

	m_AdvancementSlayerWomanSPK.LoadFromFileRunning( g_pFileDef->getProperty( "FILE_ISPRITE_ADVANCEMENT_CLASS_SLAYER_WOMAN" ).c_str() );
	m_AdvancementSlayerWomanSSPK.LoadFromFileRunning( g_pFileDef->getProperty( "FILE_SSPRITE_ADVANCEMENT_CLASS_SLAYER_WOMAN" ).c_str() );
		
//	if (!CDirect3D::IsHAL())
	{
		//------------------------------------------------------------
		// 弊覆磊 load
		//------------------------------------------------------------
//		if (!FileOpenBinary(FILE_SSPRITE_ADDON, m_AddonSSPKFile))
//			return false;
//
//		if (m_AddonSSPK.GetSize()==0)
//		{
//			TYPE_SPRITEID	numAddonSSPK;
//			m_AddonSSPKFile.read((char*)&numAddonSSPK, SIZE_SPRITEID);
//			m_AddonSSPK.Init( numAddonSSPK );	
//		}
		m_AddonSSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SSPRITE_ADDON").c_str());
		m_OustersSSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SSPRITE_OUSTERS").c_str());
	}

	m_AdvancementOustersSPK.LoadFromFileRunning( g_pFileDef->getProperty( "FILE_ISPRITE_ADVANCEMENT_CLASS_OUSTERS" ).c_str() );
	m_AdvancementOustersSSPK.LoadFromFileRunning( g_pFileDef->getProperty( "FILE_SSPRITE_ADVANCEMENT_CLASS_OUSTERS" ).c_str() );// ***
	
	m_AdvancementVampireManSPK.LoadFromFileRunning( g_pFileDef->getProperty( "FILE_ISPRITE_ADVANCEMENT_CLASS_VAMPIRE_MAN" ).c_str() );
	m_AdvancementVampireManSSPK.LoadFromFileRunning( g_pFileDef->getProperty( "FILE_SSPRITE_ADVANCEMENT_CLASS_VAMPIRE_MAN" ).c_str() );
	
	m_AdvancementVampireWomanSPK.LoadFromFileRunning( g_pFileDef->getProperty( "FILE_ISPRITE_ADVANCEMENT_CLASS_VAMPIRE_WOMAN" ).c_str() );
	m_AdvancementVampireWomanSSPK.LoadFromFileRunning( g_pFileDef->getProperty( "FILE_SSPRITE_ADVANCEMENT_CLASS_VAMPIRE_WOMAN" ).c_str() );
	
	//add by viva
	/////////////////////////////////NewVampireSPK, NewVampireSSPK
	m_NewVampireSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_ISPRITE_NEW_CLASS_VAMPIRE").c_str());
	m_NewVampireSSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SSPRITE_NEW_CLASS_VAMPIRE").c_str());
	/////////////////////////////////NewSlayerManSPK, NewSlayerManSSPK
	m_NewSlayerManSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_ISPRITE_SLAYER_MAN").c_str());
	m_NewSlayerManSSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SSPRITE_SLAYER_MAN").c_str());
	/////////////////////////////////NewSlayerWomanSPK, NewSlayerWomanSSPK
	m_NewSlayerWomanSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_ISPRITE_SLAYER_WOMAN").c_str());
	m_NewSlayerWomanSSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SSPRITE_SLAYER_WOMAN").c_str());
	//end
/*
	m_AdvancementSlayerSPK.LoadFromFileRunning( g_pFileDef->getProperty( "FILE_ISPRITE_ADVANCEMENT_CLASS_SLAYER" ).c_str() );
	
	m_AdvancementSlayerSSPK.LoadFromFileRunning( g_pFileDef->getProperty( "FILE_SSPRITE_ADVANCEMENT_CLASS_SLAYER" ).c_str() );
	m_AdvancementVampireSSPK.LoadFromFileRunning( g_pFileDef->getProperty( "FILE_SSPRITE_ADVANCEMENT_CLASS_VAMPIRE" ).c_str() );
	
*/

	//------------------------------------------------------------	
	// 弊覆磊 - Load  Clothes SpritePack	
	//------------------------------------------------------------
	// 巢
	/*
	class ifstream	AddonMaleShadowFile2;//(FILE_ISPRITE_ADDON, ios::binary);
	if (!FileOpenBinary(FILE_SSPRITE_ADDON_MALE, AddonMaleShadowFile2))
		return false;
	m_AddonMaleShadowSPK.LoadFromFile(AddonMaleShadowFile2);
	AddonMaleShadowFile2.close();
	*/

	//------------------------------------------------------------
	// 抛胶飘 : 老馆SpritePack --> ShadowSpritePack
	//------------------------------------------------------------
	/*
	// 巢
	CSpritePack AddonMaleShadowSPK;
	class ifstream	AddonMaleShadowFile2;//(FILE_ISPRITE_ADDON, ios::binary);
	if (!FileOpenBinary("Data\\Image\\addonMaleShadow.spk", AddonMaleShadowFile2))
		return false;
	AddonMaleShadowSPK.LoadFromFile(AddonMaleShadowFile2);
	AddonMaleShadowFile2.close();

	m_AddonMaleSSPK.Init( AddonMaleShadowSPK.GetSize() );

	for (int i=0; i<AddonMaleShadowSPK.GetSize(); i++)
	{
		m_AddonMaleSSPK[i].SetPixel( AddonMaleShadowSPK[i] );
	}

	//------------------------------------------------------------	
	// 巢 - Save  ShadowSpritePack	
	//------------------------------------------------------------
	class ofstream	AddonMaleShadowFile(FILE_SSPRITE_ADDON_MALE, ios::binary);	
	class ofstream	AddonMaleShadowIndexFile(FILE_SSPRITEINDEX_ADDON_MALE, ios::binary);	

	m_AddonMaleSSPK.SaveToFile(AddonMaleShadowFile, AddonMaleShadowIndexFile);

	AddonMaleShadowFile.close();
	AddonMaleShadowIndexFile.close();
	*/

	//------------------------------------------------------------
	// Load Addon Shadow SpritePack
	//------------------------------------------------------------
	// Male
	/*
	class ifstream	AddonShadowFile2;//(FILE_ISPRITE_ADDON, ios::binary);
	if (!FileOpenBinary(FILE_SSPRITE_ADDON, AddonShadowFile2))
		return false;
	m_AddonSSPK.LoadFromFile(AddonShadowFile2);
	AddonShadowFile2.close();
	*/
//	if (m_pAddonShadowManager!=NULL)
//	{
//		delete m_pAddonShadowManager;
//		m_pAddonShadowManager = NULL;
//	}
//	if (m_pOustersShadowManager!=NULL)
//	{
//		delete m_pOustersShadowManager;
//		m_pOustersShadowManager = NULL;
//	}
	//m_AddonSSPK.Release();

//	if (CDirect3D::IsHAL())
//	{	
//		//m_pAlphaEffectTextureManager = new CTexturePartManager( FILE_ASPRITE_ALPHAEFFECT, FILE_ASPRITEINDEX_ALPHAEFFECT, 70 );
//		//m_pAddonShadowManager = new CShadowPartManager( 
//		//									&m_AddonSSPK, 
//		//									g_pClientConfig->MAX_TEXTUREPART_ADDON_SHADOW );
//		m_pAddonShadowManager = new CShadowPartManager( 
//											g_pFileDef->getProperty("FILE_SSPRITE_ADDON").c_str(), 
////											FILE_SSPRITEINDEX_ADDON,
//											g_pClientConfig->MAX_TEXTUREPART_ADDON_SHADOW );
//		m_pOustersShadowManager = new CShadowPartManager( 
//											g_pFileDef->getProperty("FILE_SSPRITE_OUSTERS").c_str(), 
//											//											FILE_SSPRITEINDEX_ADDON,
//											g_pClientConfig->MAX_TEXTUREPART_ADDON_SHADOW );
//	}
//	else
	{
		/*
		class ifstream	AddonShadowFile2;//(FILE_ISPRITE_ADDON, ios::binary);
		if (!FileOpenBinary(FILE_SSPRITE_ADDON, AddonShadowFile2))
			return false;
		m_AddonSSPK.LoadFromFile(AddonShadowFile2);
		AddonShadowFile2.close();
		*/
	}

//	UI_DrawProgress(7);
//	DrawTitleLoading();

	//------------------------------------------------------------
	//
	//
	//                  ItemTile SpritePack
	//
	//
	//------------------------------------------------------------	
	/*
	m_ItemTileSPK.Init( 5, CDirectDraw::Is565() );

	
	TempSurface.InitFromBMP("ItemTile.bmp", 800,600, DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);

	lpSurfaceTemp = (WORD*)((BYTE*)lpSurface + lPitch*96);

	CSprite::SetColorkey(0x07E0);

	spriteID = 0;
	for (i=0; i<1; i++)
	{
		lpSurfaceTemp = lpSurface;
		for (j=0; j<5; j++)
		{			
			m_ItemTileSPK[spriteID++].SetPixel(lpSurfaceTemp, lPitch, 64, 64);

			lpSurfaceTemp += 64;
		}
		lpSurface = (WORD*)((BYTE*)lpSurface + lPitch*64);
	}

	CSprite::SetColorkey(0);

	TempSurface.Unlock();

	//------------------------------------------------------------	
	// Save  ItemTile SpritePack	
	//------------------------------------------------------------
	class ofstream	itemTileFile(FILE_SPRITE_ITEMTILE, ios::binary);	
	class ofstream	itemTileIndexFile(FILE_SPRITEINDEX_ITEMTILE, ios::binary);	

	m_ItemTileSPK.SaveToFile(itemTileFile, itemTileIndexFile);

	itemTileFile.close();
	itemTileIndexFile.close();
	*/

	//------------------------------------------------------------	
	// Load  ItemTile SpritePack	
	//------------------------------------------------------------
	if (m_ItemTileISPK.GetSize()==0)
	{
//		class ifstream	itemTileFile2;//(FILE_SPRITE_ITEMTILE, ios::binary);
//		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_ISPRITE_ITEMTILE").c_str(), itemTileFile2))
//			return false;
//		m_ItemTileISPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_ISPRITE_ITEMTILE").c_str());
//		itemTileFile2.close();
		m_ItemTileISPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_ISPRITE_ITEMTILE").c_str());
	}

	//------------------------------------------------------------
	//
	//
	//                    ItemDrop SpritePack
	//
	//
	//------------------------------------------------------------		

	//------------------------------------------------------------	
	// Load  itemDrop SpritePack	
	//------------------------------------------------------------
	if (m_ItemDropISPK.GetSize()==0)
	{
//		class ifstream	itemDropFile2;//(FILE_SPRITE_itemDrop, ios::binary);
//		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_ISPRITE_ITEMDROP").c_str(), itemDropFile2))
//			return false;
//		m_ItemDropISPK.LoadFromFileRunning(itemDropFile2);
//		itemDropFile2.close();
		m_ItemDropISPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_ISPRITE_ITEMDROP").c_str());
	}

	//------------------------------------------------------------
	//
	//
	//                    ItemBroken SpritePack
	//
	//
	//------------------------------------------------------------		

	//------------------------------------------------------------	
	// Load  itemBroken SpritePack	
	//------------------------------------------------------------
	if (m_ItemBrokenSPK.GetSize()==0)
	{
//		class ifstream	itemBrokenFile2;//(FILE_SPRITE_itemBroken, ios::binary);
//		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_SPRITE_ITEMBROKEN").c_str(), itemBrokenFile2))
//			return false;
//		m_ItemBrokenSPK.LoadFromFileRunning(itemBrokenFile2);
//		itemBrokenFile2.close();
		m_ItemBrokenSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SPRITE_ITEMBROKEN").c_str());
	}

//	UI_DrawProgress(8);
//	DrawTitleLoading();

	//------------------------------------------------------------
	//
	//
	//                    Tile Sprite
	//
	//
	//------------------------------------------------------------		
	/*

	//------------------------------------------------------------		
	//
	//                 Tile SpritePack
	//
	//------------------------------------------------------------		
	m_TileSPK.Init( 20, CDirectDraw::Is565() );

	
	TempSurface.InitFromBMP("Tile.bmp", 800,600, DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);

	lpSurfaceTemp = (WORD*)((BYTE*)lpSurface + lPitch*96);

	spriteID = 0;
	for (i=0; i<2; i++)
	{
		lpSurfaceTemp = lpSurface;
		for (j=0; j<10; j++)
		{			
			m_TileSPK[spriteID++].SetPixel(lpSurfaceTemp, lPitch, 64, 32);

			lpSurfaceTemp += 64;
		}
		lpSurface = (WORD*)((BYTE*)lpSurface + lPitch*32);
	}

	TempSurface.Unlock();

	//------------------------------------------------------------	
	// Save  Tile SpritePack	
	//------------------------------------------------------------
	class ofstream	tileFile(FILE_SPRITE_TILE, ios::binary);	
	class ofstream	tileIndexFile(FILE_SPRITEINDEX_TILE, ios::binary);	

	m_TileSPK.SaveToFile(tileFile, tileIndexFile);

	tileFile.close();
	tileIndexFile.close();	

	//------------------------------------------------------------	
	// Load  Tile SpritePack	
	//------------------------------------------------------------	
	class ifstream	tileFile2(FILE_SPRITE_TILE, ios::binary);
	m_TileSPK.LoadFromFile(tileFile2);
	tileFile2.close();
	*/

	
	//------------------------------------------------------------	
	//
	//                 Tile SpriteSet
	//
	//------------------------------------------------------------
	/*
	CSpriteFilePositionArray	TileSFPArray;
	TileSFPArray.Init( 6 );

	// TileSPK Index甫 Load茄促.
	CFileIndexTable		TileIndex;

	class ifstream TilePackIndexFile(FILE_SPRITEINDEX_TILE, ios::binary);
	TileIndex.LoadFromFile( TilePackIndexFile );
	TilePackIndexFile.close();	

	// TileSPK俊辑 漂沥茄 SpriteID狼 Tile甸父栏肺 Set阑 积己茄促.
	TYPE_SPRITEID	TileSpriteID[6] = { 9, 10, 11, 12, 13, 19 };
	for (TYPE_SPRITEID t=0; t<6; t++)
	{
		TileSFPArray[t].SpriteID		= TileSpriteID[t];
		TileSFPArray[t].FilePosition	= TileIndex[ TileSpriteID[t] ];
	}		
	*/

	//------------------------------------------------------------
	//  Init Tile SpriteSet
	//------------------------------------------------------------
	//int size;
	///*
//	if (m_TileSPK.GetSize()==0)
//	{
//		class ifstream TilePackIndexFile;//(FILE_SPRITEINDEX_TILE, ios::binary);
//		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_SPRITEINDEX_TILE").c_str(), TilePackIndexFile))
//			return false;
//		
//		//-----------------------------------------------------------
//		// Index甫 Load茄促.
//		//-----------------------------------------------------------
//		m_TileSPKI.LoadFromFile( TilePackIndexFile );
//		TilePackIndexFile.close();	
//
//		m_TileSPK.Init( m_TileSPKI.GetSize() );
//
//		/*
//		for (int q=0; q<m_TileSPKI.GetSize(); q++)
//		{
//			DEBUG_ADD_FORMAT("[%d] %ld", q, m_TileSPKI[q]);
//		}
//		*/
//
//		//m_TileSPKFile.open(FILE_SPRITE_TILE, ios::binary);
////		if (!FileOpenBinary(FILE_SPRITE_TILE, m_TileSPKFile))
////			return false;
//	}

//	UI_DrawProgress(9);
//	DrawTitleLoading();

	//*/
	
	/* TILE INDEX 父甸扁
	CSpritePack spk;
	class ifstream TilePackIndexFile(FILE_SPRITE_TILE, ios::binary);
	spk.LoadFromFile(TilePackIndexFile);	
	TilePackIndexFile.close();	

	class ofstream TilePackIndexFile2(FILE_SPRITE_TILE, ios::binary);
	class ofstream TilePackIndexFile3(FILE_SPRITEINDEX_TILE, ios::binary);
	spk.SaveToFile(TilePackIndexFile2, TilePackIndexFile3);
	TilePackIndexFile2.close();
	TilePackIndexFile3.close();

	m_TileSPK.Init( spk.GetSize(), CDirectDraw::Is565() );
	*/


	/*
	class ifstream	TilePackFile(FILE_SPRITE_TILE, ios::binary);		
	m_TileSPK.LoadFromFilePart(TilePackFile, TileSFPArray);
	TilePackFile.close();
	*/
	m_TileSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SPRITE_TILE").c_str() );


	//------------------------------------------------------------	
	//
	//                 ImageObject SpriteSet
	//
	//------------------------------------------------------------	
	//
	// Index绝绰 SPK俊 Index积己窍扁
	/*
	CSpritePack tempSPK;
	class ifstream	ioFile2(FILE_SPRITE_IMAGEOBJECT, ios::binary);
	tempSPK.LoadFromFile(ioFile2);
	ioFile2.close();

	class ofstream	ioFile(FILE_SPRITE_IMAGEOBJECT, ios::binary);	
	class ofstream	ioIndexFile(FILE_SPRITEINDEX_IMAGEOBJECT, ios::binary);	

	tempSPK.SaveToFile(ioFile, ioIndexFile);

	ioFile.close();
	ioIndexFile.close();	
	*/

	/*
	CSpriteFilePositionArray	ImageObjectSFPArray;
	ImageObjectSFPArray.Init( 21 );

	// ImageObjectSPK Index甫 Load茄促.
	CFileIndexTable		ImageObjectIndex;

	class ifstream ImageObjectPackIndexFile(FILE_SPRITEINDEX_IMAGEOBJECT, ios::binary);
	ImageObjectIndex.LoadFromFile( ImageObjectPackIndexFile );
	ImageObjectPackIndexFile.close();	

	// ImageObjectSPK俊辑 漂沥茄 SpriteID狼 ImageObject甸父栏肺 Set阑 积己茄促.
	TYPE_SPRITEID	ImageObjectSpriteID[21] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	for (TYPE_SPRITEID io=0; io<21; io++)
	{
		ImageObjectSFPArray[io].SpriteID		= ImageObjectSpriteID[io];
		ImageObjectSFPArray[io].FilePosition	= ImageObjectIndex[ ImageObjectSpriteID[io] ];
	}		
	*/

	//------------------------------------------------------------
	//  Init ImageObject SpriteSet
	//------------------------------------------------------------	
	//int size;
	if (m_ImageObjectSPK.GetSize()==0)
	{
		
//		class ifstream ImageObjectPackIndexFile;//(FILE_SPRITEINDEX_IMAGEOBJECT, ios::binary);
//		if (!FileOpenBinary(FILE_SPRITEINDEX_IMAGEOBJECT, ImageObjectPackIndexFile))
//			return false;
//
//		//-----------------------------------------------------------
//		// Index甫 Load茄促.
//		//-----------------------------------------------------------
//		m_ImageObjectSPKI.LoadFromFile( ImageObjectPackIndexFile );
//		ImageObjectPackIndexFile.close();	
//
//		m_ImageObjectSPK.Init( m_ImageObjectSPKI.GetSize(), CDirectDraw::Is565() );
//
//		m_ImageObjectSPKFile.open(FILE_SPRITE_IMAGEOBJECT, ios::binary);
		m_ImageObjectSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SPRITE_IMAGEOBJECT").c_str() );
		// 睦.. 捞芭 恐 促 肺爹窍瘤. - -;;
		// 2001.8.20 林籍贸府
		//if (!FileOpenBinary(FILE_SPRITE_IMAGEOBJECT, m_ImageObjectSPKFile))
		//	return false;

		//m_ImageObjectSPK.LoadFromFile( m_ImageObjectSPKFile );

	}

	//-----------------------------------------------------------
	// ImageObject TextureManager
	//-----------------------------------------------------------
	
#ifdef __3D_IMAGE_OBJECT__
//	if (CDirect3D::IsHAL())
//	{	
//		//m_pAlphaEffectTextureManager = new CTexturePartManager( FILE_ASPRITE_ALPHAEFFECT, FILE_ASPRITEINDEX_ALPHAEFFECT, 70 );
//		SAFE_DELETE( m_pImageObjectTextureManager );
//		
//		m_pImageObjectTextureManager = new CNormalSpriteTexturePartManager(
//												&m_ImageObjectSPK, 
//												g_pClientConfig->MAX_TEXTUREPART_IMAGEOBJECTSPK );
//	}
#endif
	

//	UI_DrawProgress(10);
//	DrawTitleLoading();

	/*
	class ifstream	ImageObjectPackFile(FILE_SPRITE_IMAGEOBJECT, ios::binary);		
	m_ImageObjectSPK.LoadFromFilePart(ImageObjectPackFile, ImageObjectSFPArray);
	ImageObjectPackFile.close();	
	*/

	//------------------------------------------------------------
	//
	//
	//                   EffectSPK 檬扁拳
	//
	//
	//------------------------------------------------------------
	/*
	m_EffectAlphaSPK.Init( 67, CDirectDraw::Is565() );
	
	spriteID = 0;
	CAlphaSprite::SetColorkey( 0x001F );
	//------------------------------------------------------------
	// FireEffect
	//------------------------------------------------------------
	TempSurface.InitFromBMP("FireTest.bmp", DDSCAPS_SYSTEMMEMORY);	
	//TempSurface.InitFromBMP("FireEffect.bmp", DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);

	lpSurfaceTemp = (WORD*)((BYTE*)lpSurface + lPitch*154);

	for (i=0; i<8; i++)
	{		
		m_EffectAlphaSPK[spriteID++].SetPixel(lpSurface, lPitch, 
								lpSurfaceTemp, lPitch,
								110, 154);
		lpSurface += 110;
		lpSurfaceTemp += 110;
	}		

	TempSurface.Unlock();
	CAlphaSprite::SetColorkey( 0 );

	//------------------------------------------------------------
	// Lightning Effect
	//------------------------------------------------------------	
	TempSurface.InitFromBMP("Lightning.bmp", DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);

	lpSurfaceTemp = (WORD*)((BYTE*)lpSurface + lPitch*96);

	for (i=0; i<8; i++)
	{		
		m_EffectAlphaSPK[spriteID++].SetPixel(lpSurface, lPitch, 
								lpSurfaceTemp, lPitch,
								64, 96);
		lpSurface += 64;
		lpSurfaceTemp += 64;
	}		

	TempSurface.Unlock();

	//------------------------------------------------------------	
	// Light + Darkness Effect
	//------------------------------------------------------------	
	TempSurface.InitFromBMP("LightEffect.bmp", DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);

	lpSurfaceTemp = (WORD*)((BYTE*)lpSurface + lPitch*64);

	CAlphaSprite::SetColorkey( 0x001F );
	for (i=0; i<4; i++)
	{		
		m_EffectAlphaSPK[spriteID++].SetPixel(lpSurface, lPitch, 
								lpSurfaceTemp, lPitch,
								64, 64);
		lpSurface += 64;
		lpSurfaceTemp += 64;
	}		
	CAlphaSprite::SetColorkey( 0 );

	TempSurface.Unlock();

	//------------------------------------------------------------	
	// Aura Effect
	//------------------------------------------------------------	
	TempSurface.InitFromBMP("Aura.bmp", DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);
	
	lpSurfaceTemp2 = lpSurface;
	lpSurfaceTemp = (WORD*)((BYTE*)lpSurface + lPitch*32*4);

	CAlphaSprite::SetColorkey( 0xFFFF );
	for (i=0; i<3; i++)
	{		
		m_EffectAlphaSPK[spriteID++].SetPixel(lpSurface, lPitch, 
								lpSurfaceTemp, lPitch,
								64*3, 32*4);
		lpSurface += 64*3;
		lpSurfaceTemp += 64*3;
	}	
	
	
	lpSurface = (WORD*)((BYTE*)lpSurfaceTemp2 + lPitch*32*8);
	lpSurfaceTemp = (WORD*)((BYTE*)lpSurfaceTemp2 + lPitch*32*4);
	
	for (i=0; i<3; i++)
	{		
		m_EffectAlphaSPK[spriteID++].SetPixel(lpSurface, lPitch, 
								lpSurfaceTemp, lPitch,
								64*3, 32*4);
		lpSurface += 64*3;
		lpSurfaceTemp += 64*3;
	}	
	CAlphaSprite::SetColorkey( 0 );

	TempSurface.Unlock();

	//------------------------------------------------------------	
	// Sword Effect
	//------------------------------------------------------------	
	TempSurface.InitFromBMP("SwordEffect.bmp", DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);

	lpSurfaceTemp = (WORD*)((BYTE*)lpSurface + lPitch*64);

	CAlphaSprite::SetColorkey( 0xF800 );
	for (i=0; i<12; i++)
	{		
		m_EffectAlphaSPK[spriteID++].SetPixel(lpSurface, lPitch, 
								lpSurfaceTemp, lPitch,
								64, 64);
		lpSurface += 64;
		lpSurfaceTemp += 64;
	}		
	CAlphaSprite::SetColorkey( 0 );

	TempSurface.Unlock();

	//------------------------------------------------------------	
	// Comet Effect
	//------------------------------------------------------------	
	TempSurface.InitFromBMP("CometEffect.bmp", DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);

	lpSurfaceTemp = (WORD*)((BYTE*)lpSurface + lPitch*256);

	CAlphaSprite::SetColorkey( 0xFFFF );
	for (i=0; i<7; i++)
	{		
		m_EffectAlphaSPK[spriteID++].SetPixel(lpSurface, lPitch, 
								lpSurfaceTemp, lPitch,
								256, 256);
		lpSurface += 256;
		lpSurfaceTemp += 256;
	}		
	CAlphaSprite::SetColorkey( 0 );

	TempSurface.Unlock();

	
	//------------------------------------------------------------	
	// Star Effect
	//------------------------------------------------------------	
	TempSurface.InitFromBMP("StarEffect.bmp", DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);

	lpSurfaceTemp = (WORD*)((BYTE*)lpSurface + lPitch*120);

	CAlphaSprite::SetColorkey( 0x001F );
	for (i=0; i<12; i++)
	{		
		m_EffectAlphaSPK[spriteID++].SetPixel(lpSurface, lPitch, 
								lpSurfaceTemp, lPitch,
								160, 120);
		lpSurface += 160;
		lpSurfaceTemp += 160;
	}		
	TempSurface.Unlock();

	//------------------------------------------------------------	
	// Flare Effect
	//------------------------------------------------------------	
	TempSurface.InitFromBMP("FlareEffect.bmp", DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);

	lpSurfaceTemp = (WORD*)((BYTE*)lpSurface + lPitch*100);

	CAlphaSprite::SetColorkey( 0xF800 );
	for (i=0; i<10; i++)
	{		
		m_EffectAlphaSPK[spriteID++].SetPixel(lpSurface, lPitch, 
								lpSurfaceTemp, lPitch,
								100, 100);
		lpSurface += 100;
		lpSurfaceTemp += 100;
	}		
	TempSurface.Unlock();


	//------------------------------------------------------------
	//
	// Save  EffectSPK
	//
	//------------------------------------------------------------
	class ofstream	effectFile(FILE_ASPRITE_ALPHAEFFECT, ios::binary);	
	class ofstream	effectIndexFile(FILE_ASPRITEINDEX_ALPHAEFFECT, ios::binary);	

	m_EffectAlphaSPK.SaveToFile(effectFile, effectIndexFile);

	effectFile.close();
	effectIndexFile.close();
	*/

	//------------------------------------------------------------
	//
	// Load  EffectPack
	//
	//------------------------------------------------------------
	// 3d啊加捞 登搁 m_pAlphaEffectTextureManager甫 荤侩窍绊
	// 酒聪搁, m_EffectAlphaSPK甫 荤侩茄促.
/*
	class ifstream	effectFile2;//(FILE_ASPRITE_ALPHAEFFECT, ios::binary);
	if (!FileOpenBinary(FILE_ASPRITE_ALPHAEFFECT, effectFile2))
		return false;
	m_EffectAlphaSPK.LoadFromFile(effectFile2);
	effectFile2.close();	
	*/
	//------------------------------------------------------------
	// AlphaEffect
	//------------------------------------------------------------

//	SAFE_DELETE( m_pAlphaEffectTextureManager );
//	SAFE_DELETE( m_pImageObjectTextureManager );
//	m_EffectAlphaSPKI.Release();
//	m_EffectAlphaSPK.Release();
	
//	if (m_EffectAlphaSPKFile.is_open())
//	{
//		m_EffectAlphaSPKFile.close();
//	}

//	if (CDirect3D::IsHAL())
//	{	
//		//m_pAlphaEffectTextureManager = new CTexturePartManager( FILE_ASPRITE_ALPHAEFFECT, FILE_ASPRITEINDEX_ALPHAEFFECT, 70 );
//		//m_pAlphaEffectTextureManager = new CTexturePartManager( 
//		//								&m_EffectAlphaSPK, 
//		//								g_pClientConfig->MAX_TEXTUREPART_EFFECT );
//		m_pAlphaEffectTextureManager = new CTexturePartManager( 
//										g_pFileDef->getProperty("FILE_ASPRITE_ALPHAEFFECT").c_str(),
//// 										FILE_ASPRITEINDEX_ALPHAEFFECT, 
//										g_pClientConfig->MAX_TEXTUREPART_EFFECT );
////		m_EffectAlphaPPK.LoadFromFile( FILE_PALETTE_ALPHAEFFECT, CDirectDraw::Is565() );
//
//		
//	}
//	else
	{
		//class ifstream	effectFile2;//(FILE_ASPRITE_ALPHAEFFECT, ios::binary);
		//if (!FileOpenBinary(FILE_ASPRITE_ALPHAEFFECT, effectFile2))
		//	return false;
		//m_EffectAlphaSPK.LoadFromFile(effectFile2);
		//effectFile2.close();	
//		class ifstream effectFileIndex;//(FILE_ISPRITEINDEX_CREATURE, ios::binary);
//		if (!FileOpenBinary(FILE_ASPRITEINDEX_ALPHAEFFECT, effectFileIndex))
//			return false;
//		
//		// Index File父 loading窍绊 
//		m_EffectAlphaSPKI.LoadFromFile( effectFileIndex );
//
//		// 俺荐父怒 檬扁拳父 秦敌促.
//		m_EffectAlphaSPK.Init( m_EffectAlphaSPKI.GetSize(), CDirectDraw::Is565() );
//
//		effectFileIndex.close();	
//
//		//------------------------------------------------------------
//		// Run-time loading阑 困秦辑 file阑 凯绢敌促.
//		//------------------------------------------------------------
//		if (!FileOpenBinary(FILE_ASPRITE_ALPHAEFFECT, m_EffectAlphaSPKFile))
//			return false;
		m_EffectAlphaSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_ASPRITE_ALPHAEFFECT").c_str());
		m_EffectAlphaPPK.LoadFromFileRunning( g_pFileDef->getProperty("FILE_PALETTE_ALPHAEFFECT").c_str() );
	}

	//------------------------------------------------------------
	// ScreenEffect
	//------------------------------------------------------------
//	if (m_pScreenEffectTextureManager!=NULL)
//	{
//		delete m_pScreenEffectTextureManager;
//		m_pScreenEffectTextureManager = NULL;
//	}
//	m_EffectScreenSPKI.Release();
//	m_EffectScreenSPK.Release();
//	m_EffectScreenPPK.Release();
	
//	if (m_EffectScreenSPKFile.is_open())
//	{
//		m_EffectScreenSPKFile.close();
//	}

//	if (CDirect3D::IsHAL())
//	{	
//		//m_pAlphaEffectTextureManager = new CTexturePartManager( FILE_ASPRITE_ALPHAEFFECT, FILE_ASPRITEINDEX_ALPHAEFFECT, 70 );
//		//m_pAlphaEffectTextureManager = new CTexturePartManager( 
//		//								&m_EffectAlphaSPK, 
//		//								g_pClientConfig->MAX_TEXTUREPART_EFFECT );
//		m_pScreenEffectTextureManager = new CSpriteTexturePartManager( 
//										g_pFileDef->getProperty("FILE_SPRITE_SCREENEFFECT").c_str(),
////										FILE_SPRITEINDEX_SCREENEFFECT, 
//										g_pClientConfig->MAX_TEXTUREPART_SCREENEFFECT );
////		m_EffectScreenPPK.LoadFromFile( FILE_PALETTE_SCREENEFFECT, CDirectDraw::Is565() );
//	}
//	else
	{
		//class ifstream	effectFile2;//(FILE_ASPRITE_ALPHAEFFECT, ios::binary);
		//if (!FileOpenBinary(FILE_ASPRITE_ALPHAEFFECT, effectFile2))
		//	return false;
		//m_EffectAlphaSPK.LoadFromFile(effectFile2);
		//effectFile2.close();	
//		class ifstream effectFileIndex;//(FILE_ISPRITEINDEX_CREATURE, ios::binary);
//		if (!FileOpenBinary(FILE_SPRITEINDEX_SCREENEFFECT, effectFileIndex))
//			return false;
//		
//		// Index File父 loading窍绊 
//		m_EffectScreenSPKI.LoadFromFile( effectFileIndex );
//
//		// 俺荐父怒 檬扁拳父 秦敌促.
//		m_EffectScreenSPK.Init( m_EffectScreenSPKI.GetSize(), CDirectDraw::Is565() );
//
//		effectFileIndex.close();	
//
//		//------------------------------------------------------------
//		// Run-time loading阑 困秦辑 file阑 凯绢敌促.
//		//------------------------------------------------------------
//		if (!FileOpenBinary(FILE_SPRITE_SCREENEFFECT, m_EffectScreenSPKFile))
//			return false;
		m_EffectScreenSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SPRITE_SCREENEFFECT").c_str());
		//m_EffectScreenSPK.LoadFromFile(g_pFileDef->getProperty("FILE_SPRITE_SCREENEFFECT").c_str());
		m_EffectScreenPPK.LoadFromFile( g_pFileDef->getProperty("FILE_PALETTE_SCREENEFFECT").c_str() );
	}
	
	//------------------------------------------------------------
	//
	//		ShadowEffect
	//
	//------------------------------------------------------------
//	if (m_pEffectShadowManager!=NULL)
//	{
//		delete m_pEffectShadowManager;
//		m_pEffectShadowManager = NULL;
//	}
//	if (CDirect3D::IsHAL())
//	{	
//		m_pEffectShadowManager = new CShadowPartManager( 
//										g_pFileDef->getProperty("FILE_SSPRITE_SHADOWEFFECT").c_str(),
////										FILE_SSPRITEINDEX_SHADOWEFFECT, 
//										g_pClientConfig->MAX_TEXTUREPART_EFFECTSHADOW );
//	}
//	else
	{
//		class ifstream	effectFile2;
//		if (!FileOpenBinary(FILE_SSPRITE_SHADOWEFFECT, effectFile2))
//			return false;
//		m_EffectShadowSPK.LoadFromFile(effectFile2);
//		effectFile2.close();			
		m_EffectShadowSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SSPRITE_SHADOWEFFECT").c_str());
	}

//	UI_DrawProgress(12);
//	DrawTitleLoading();

	//class ifstream	effectFile2(FILE_ASPRITE_ALPHAEFFECT, ios::binary);
	//m_EffectAlphaSPK.LoadFromFile(effectFile2);
	//effectFile2.close();	
	
	//------------------------------------------------------------
	//
	// Init Effect TextureSurface
	//
	//------------------------------------------------------------
	/*
	if (CDirect3D::IsHAL())
	{	
		//--------------------------------------------
		// AlphaSPK阑 捞侩秦辑 TexturePack阑 积己茄促.
		//--------------------------------------------
		if (!m_EffectTPK.Init( m_EffectAlphaSPK ))
		{
			MessageBox(g_hWnd, "Can't Init EffectTexture!", NULL, MB_OK);
			return false;
		}

		//------------------------------------------------------------	
		// EffectAlphaSprite甫 皋葛府俊辑 昏力茄促.
		// --> TextureSurface甫 荤侩且 巴捞骨肺.. 鞘夸啊 绝促.
		//------------------------------------------------------------		
		m_EffectAlphaSPK.Release();
	}
	*/

	//------------------------------------------------------------		
	//
	//                 Etc SpritePack
	//
	//------------------------------------------------------------	
	/*
	m_EtcSPK.Init( 13, CDirectDraw::Is565() );

	
	TempSurface.InitFromBMP("Etc.bmp", DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);

	lpSurfaceTemp = (WORD*)((BYTE*)lpSurface + lPitch*96);

	spriteID = 0;
	CSprite::SetColorkey( 0x001F );
	for (i=0; i<2; i++)
	{
		lpSurfaceTemp = lpSurface;
		int endj = (i==0)? 5 : 8;					
		for (j=0; j<endj; j++)
		{	
			//if (spriteID==0)
				m_EtcSPK[spriteID++].SetPixel(lpSurfaceTemp, lPitch, 64, 32);
			//else
			//	m_EtcSPK[spriteID++].SetPixel(lpSurfaceTemp, lPitch, 64, 96);

			lpSurfaceTemp += 64;
		}
		lpSurface = (WORD*)((BYTE*)lpSurface + lPitch*32);
	}	
	CSprite::SetColorkey( 0 );

	TempSurface.Unlock();

	//------------------------------------------------------------	
	// Save  Etc SpritePack	
	//------------------------------------------------------------
	class ofstream	EtcFile(FILE_SPRITE_ETC, ios::binary);	
	class ofstream	EtcIndexFile(FILE_SPRITEINDEX_ETC, ios::binary);	

	m_EtcSPK.SaveToFile(EtcFile, EtcIndexFile);

	EtcFile.close();
	EtcIndexFile.close();	
	*/

	//------------------------------------------------------------	
	// Load  Etc SpritePack	
	//------------------------------------------------------------	
	if (m_EtcSPK.GetSize()==0)
	{
		class ifstream	EtcFile2;//(FILE_SPRITE_ETC, ios::binary);
		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_SPRITE_ETC").c_str(), EtcFile2))
			return false;
		m_EtcSPK.LoadFromFile(EtcFile2);
		EtcFile2.close();
	}

	//------------------------------------------------------------		
	//
	//                 InteractionObject SpritePack
	//
	//------------------------------------------------------------
//	if (m_InteractionObjectSPK.GetSize()==0)
//	{
//		class ifstream	ioFile2;//(FILE_SPRITE_ETC, ios::binary);
//		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_SPRITE_INTERACTIONOBJECT").c_str(), ioFile2))
//			return false;
//		m_InteractionObjectSPK.LoadFromFile(ioFile2);
//		ioFile2.close();
//	}

//	if (m_InteractionObjectSSPK.GetSize()==0)
//	{
//		/*
//		class ifstream	ioFile3;//(FILE_SPRITE_ETC, ios::binary);
//		if (!FileOpenBinary(FILE_SSPRITE_INTERACTIONOBJECT, ioFile3))
//			return false;
//		m_InteractionObjectSSPK.LoadFromFile(ioFile2);
//		ioFile3.close();
//		*/
//	}

	/*	
	//------------------------------------------------------------
	//
	//      Effect Normal
	//
	//------------------------------------------------------------
	m_EffectNormalSPK.Init( 3, CDirectDraw::Is565() );

	CSprite::SetColorkey( 0 );
	//------------------------------------------------------------
	// EffectNormalSPK
	//------------------------------------------------------------
	TempSurface.InitFromBMP("Blood1.bmp", DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);	
	m_EffectNormalSPK[ 0 ].SetPixel(lpSurface, lPitch, TempSurface.GetWidth(), TempSurface.GetHeight());
	TempSurface.Unlock();

	//------------------------------------------------------------
	// EffectNormalSPK
	//------------------------------------------------------------
	TempSurface.InitFromBMP("Blood2.bmp", DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);	
	m_EffectNormalSPK[ 1 ].SetPixel(lpSurface, lPitch, TempSurface.GetWidth(), TempSurface.GetHeight());
	TempSurface.Unlock();

	//------------------------------------------------------------
	// EffectNormalSPK
	//------------------------------------------------------------
	TempSurface.InitFromBMP("Blood3.bmp", DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);	
	m_EffectNormalSPK[ 2 ].SetPixel(lpSurface, lPitch, TempSurface.GetWidth(), TempSurface.GetHeight());
	TempSurface.Unlock();

	//------------------------------------------------------------	
	// Save  NormalEffect Shadow SpritePack	
	//------------------------------------------------------------
	
	class ofstream	NormalEffectFile(FILE_SPRITE_NORMALEFFECT, ios::binary);	
	class ofstream	NormalEffectIndexFile(FILE_SPRITEINDEX_NORMALEFFECT, ios::binary);	

	m_EffectNormalSPK.SaveToFile(NormalEffectFile, NormalEffectIndexFile);

	NormalEffectFile.close();
	NormalEffectIndexFile.close();
	*/

	//------------------------------------------------------------	
	// Load  Clothes SpritePack	
	//------------------------------------------------------------
	if (m_EffectNormalSPK.GetSize()==0)
	{
//		class ifstream	NormalEffectFile2;//(FILE_SPRITE_NORMALEFFECT, ios::binary);
//		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_SPRITE_NORMALEFFECT").c_str(), NormalEffectFile2))
//			return false;
//		m_EffectNormalSPK.LoadFromFile(NormalEffectFile2);
//		NormalEffectFile2.close();	
		m_EffectNormalSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SPRITE_NORMALEFFECT").c_str());
	}


	//------------------------------------------------------------
	//
	//
	//                   Weather SpritePack 檬扁拳
	//
	//
	//------------------------------------------------------------
	//------------------------------------------------------------	
	// Load  Clothes SpritePack	
	//------------------------------------------------------------
	if (m_WeatherSPK.GetSize()==0)
	{
		class ifstream	WeatherFile2;//(FILE_SPRITE_WEATHER, ios::binary);
		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_SPRITE_WEATHER").c_str(), WeatherFile2))
			return false;
		m_WeatherSPK.LoadFromFile(WeatherFile2);
		WeatherFile2.close();
	}

	//------------------------------------------------------------
	//
	//
	//                   Shadow Test 檬扁拳
	//
	//
	//------------------------------------------------------------
	/*
	CAlphaSpritePack	ShadowASPK;
	ShadowASPK.Init( 1, CDirectDraw::Is565() );
	
	spriteID = 0;
	CAlphaSprite::SetColorkey( 0xFFFF );
	//------------------------------------------------------------
	// FireEffect
	//------------------------------------------------------------
	TempSurface.InitFromBMP("Shadow.bmp", DDSCAPS_SYSTEMMEMORY);	
	//TempSurface.InitFromBMP("FireEffect.bmp", DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);

	lpSurfaceTemp = (WORD*)((BYTE*)lpSurface + lPitch*32);

	for (i=0; i<1; i++)
	{		
		ShadowASPK[spriteID++].SetPixel(lpSurface, lPitch, 
								lpSurfaceTemp, lPitch,
								32, 32);
		lpSurface += 32;
		lpSurfaceTemp += 32;
	}		

	TempSurface.Unlock();
	CAlphaSprite::SetColorkey( 0 );

	//--------------------------------------------
	// AlphaSPK阑 捞侩秦辑 TexturePack阑 积己茄促.
	//--------------------------------------------
	if (!m_ShadowTPK.Init( ShadowASPK ))
	{
		MessageBox(g_hWnd, "Can't Init ShadowTexture!", NULL, MB_OK);
		return false;
	}

	//------------------------------------------------------------	
	// EffectAlphaSprite甫 皋葛府俊辑 昏力茄促.
	// --> TextureSurface甫 荤侩且 巴捞骨肺.. 鞘夸啊 绝促.
	//------------------------------------------------------------		
	ShadowASPK.Release();
	*/
	
	//------------------------------------------------------------
	//
	//				Minimap	
	//
	//------------------------------------------------------------
	 // 2001.7.14 林籍贸府
//	if (m_pMinimapSPR!=NULL)
//	{
//		delete m_pMinimapSPR;
//		m_pMinimapSPR = NULL;
//	}
//
//	if (CDirectDraw::Is565())
//	{
//		m_pMinimapSPR = new CSprite565;
//	}
//	else
//	{
//		m_pMinimapSPR = new CSprite555;
//	}
	

	/*	
	CSprite::SetColorkey( 0x001F );
	
	
	//------------------------------------------------------------
	// MinimapSprite
	//------------------------------------------------------------
	TempSurface.InitFromBMP("a-minimap.bmp", DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);	
	m_pMinimapSPR->SetPixel(lpSurface, lPitch, TempSurface.GetWidth(), TempSurface.GetHeight());
	TempSurface.Unlock();

	//------------------------------------------------------------	
	// Save  Minimap Sprite
	//------------------------------------------------------------	
	class ofstream	MapTest1("Data\\Image\\map_a.spr", ios::binary);		
	m_pMinimapSPR->SaveToFile( MapTest1 );
	MapTest1.close();

	//------------------------------------------------------------
	// MinimapSprite
	//------------------------------------------------------------
	TempSurface.InitFromBMP("h-minimap.bmp", DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);	
	m_pMinimapSPR->SetPixel(lpSurface, lPitch, TempSurface.GetWidth(), TempSurface.GetHeight());
	TempSurface.Unlock();

	//------------------------------------------------------------	
	// Save  Minimap Sprite
	//------------------------------------------------------------	
	class ofstream	MapTest2("Data\\Image\\map_h.spr", ios::binary);		
	m_pMinimapSPR->SaveToFile( MapTest2 );
	MapTest2.close();

	//------------------------------------------------------------
	// MinimapSprite
	//------------------------------------------------------------
	TempSurface.InitFromBMP("e-minimap.bmp", DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);	
	m_pMinimapSPR->SetPixel(lpSurface, lPitch, TempSurface.GetWidth(), TempSurface.GetHeight());
	TempSurface.Unlock();

	//------------------------------------------------------------	
	// Save  Minimap Sprite
	//------------------------------------------------------------	
	class ofstream	MapTest3("Data\\Image\\map_e.spr", ios::binary);		
	m_pMinimapSPR->SaveToFile( MapTest3 );
	MapTest3.close();

	//------------------------------------------------------------
	// MinimapSprite
	//------------------------------------------------------------
	TempSurface.InitFromBMP("c-minimap.bmp", DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);	
	m_pMinimapSPR->SetPixel(lpSurface, lPitch, TempSurface.GetWidth(), TempSurface.GetHeight());
	TempSurface.Unlock();

	//------------------------------------------------------------	
	// Save  Minimap Sprite
	//------------------------------------------------------------	
	class ofstream	MapTest4("Data\\Image\\map_c.spr", ios::binary);		
	m_pMinimapSPR->SaveToFile( MapTest4 );
	MapTest4.close();

	//------------------------------------------------------------
	// MinimapSprite
	//------------------------------------------------------------
	TempSurface.InitFromBMP("d-minimap.bmp", DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);	
	m_pMinimapSPR->SetPixel(lpSurface, lPitch, TempSurface.GetWidth(), TempSurface.GetHeight());
	TempSurface.Unlock();

	//------------------------------------------------------------	
	// Save  Minimap Sprite
	//------------------------------------------------------------	
	class ofstream	MapTest5("Data\\Image\\map_d.spr", ios::binary);		
	m_pMinimapSPR->SaveToFile( MapTest5 );
	MapTest5.close();

	*/

	//------------------------------------------------------------
	//
	//
	//                   Guild SpritePack 檬扁拳
	//
	//
	//------------------------------------------------------------
	//------------------------------------------------------------	
	// Load  Guild SpritePack	
	//------------------------------------------------------------
	/*
	if (m_GuildSPK.GetSize()==0)
	{
		class ifstream	guildFile2;//(FILE_SPRITE_WEATHER, ios::binary);
		if (!FileOpenBinary(FILE_SPRITE_GUILD, guildFile2))
			return false;
		m_GuildSPK.LoadFromFile(guildFile2);
		guildFile2.close();
	}
	*/

//	UI_DrawProgress(13);
//	DrawTitleLoading();

	return true;
}

//----------------------------------------------------------------------
// Filter 檬扁拳
//----------------------------------------------------------------------
bool
MTopView::InitFilters()
{
	/*
	TYPE_FILTERID	filterID;

	CSpriteSurface	TempSurface;

	WORD	*lpSurface, 
			*lpSurfaceTemp;

	WORD	lPitch;

	int i,j;
	*/

	//------------------------------------------------------------		
	//
	//             Filter Tile SpritePack
	//
	//------------------------------------------------------------	
	/*
	m_Filter.Init( 105 );
	
	TempSurface.InitFromBMP("FogTile48.bmp", DDSCAPS_SYSTEMMEMORY);	
	TempSurface.LockW(lpSurface, lPitch);

	lpSurfaceTemp = (WORD*)((BYTE*)lpSurface + lPitch * TILE_Y*3);

	filterID = 0;

	for (i=0; i<12; i++)
	{
		lpSurfaceTemp = lpSurface;
		for (j=0; j<10; j++)
		{				
			m_Filter[filterID++].SetFilter(lpSurfaceTemp, lPitch, TILE_X, TILE_Y);

			lpSurfaceTemp += TILE_X;
			
			if (filterID%35==0) break;
		}
		lpSurface = (WORD*)((BYTE*)lpSurface + lPitch*TILE_Y);
	}

	TempSurface.Unlock();

	//------------------------------------------------------------	
	// Save  Light2D FilterPack
	//------------------------------------------------------------
	class ofstream	FilterLight2DFile(FILE_FILTER_LIGHT2D, ios::binary);		
	m_Filter.SaveToFile(FilterLight2DFile);
	FilterLight2DFile.close();	
	*/

	//------------------------------------------------------------	
	// Load  Light2D FilterPack
	//------------------------------------------------------------	
	//class ifstream	FilterLight2DFile2(FILE_FILTER_LIGHT2D, ios::binary);
	//m_Filter.LoadFromFile(FilterLight2DFile2);
	//FilterLight2DFile2.close();


	//------------------------------------------------------------	
	//
	//  3D 矫具贸府甫 困茄 Light Filter
	//
	//------------------------------------------------------------	
	//-----------------------------------------------
	// LightBuffer Texture 檬扁拳
	//-----------------------------------------------	
	if (m_pLightBufferTexture!=NULL)
	{
		//m_pLightBufferTexture->Restore();
		delete m_pLightBufferTexture;
		m_pLightBufferTexture = NULL;
	}
	//------------------------------------------------------------	
	// 2D light俊辑 茄 痢捞 瞒瘤窍绰 拳搁狼 pixel农扁
	//------------------------------------------------------------	
	if (m_p2DLightPixelWidth!=NULL)
	{
		delete [] m_p2DLightPixelWidth;
		m_p2DLightPixelWidth = NULL;
	}

	if (m_p2DLightPixelHeight!=NULL)
	{
		delete [] m_p2DLightPixelHeight;
		m_p2DLightPixelHeight = NULL;
	}

	
	if (CDirect3D::IsHAL())
	{
		/*
		//------------------------------------------------------
		// Light FilterPack 檬扁拳
		//------------------------------------------------------
		m_LightFTP.Init( MAX_LIGHT_SETSIZE );

		//------------------------------------------------------
		// LightFilter 积己
		//------------------------------------------------------		
		int maxLight = 14;
		int maxCenter = 9;
		int lightRange[MAX_LIGHT_SETSIZE] = { 20, 26, 36, 46, 56, 66, 76, 86, 96, 106, 116, 126};
		int l,i,j;

		// MAX_LIGHT_SETSIZE父怒 灌扁狼 农扁 辆幅啊 乐促.		
		for (l=0; l<MAX_LIGHT_SETSIZE; l++)
		{
			int lightRangeHalf = lightRange[l]>>1;	
			int maxK = maxCenter,maxLight;
			float gap = (float)(maxK,1)/(float)lightRangeHalf;	// k蔼捞 0~24鳖瘤..

			m_LightFTP[l].Init( lightRange[l], lightRange[l] );
			BYTE* pLight;
			for (i=0; i<lightRange[l]; i++)
			{
				pLight = m_LightFTP[l].GetFilter( i );
				for (j=0; j<lightRange[l]; j++)
				{
					int k = sqrt( 
								abs(i-lightRangeHalf)*abs(i-lightRangeHalf) 
								, abs(j-lightRangeHalf)*abs(j-lightRangeHalf) 
							);

					k = (float)k * gap;
				
					if (k <= maxCenter) k = 0;			// maxCenter鳖瘤绰 0
					else if (k >= maxK) k = maxLight;	// 逞栏搁 maxLight
					else k = k - maxCenter;				// 唱赣瘤绰 - maxCenter

					// 灌扁狼 SET窜拌甫 父电促.
					//k >>= shiftValue;

					//*pLight = k;
					*pLight = maxLight - k;
					pLight ++;
				}
			}
		}		
		
		//------------------------------------------------------------	
		// Save  Light3D FilterPack
		//------------------------------------------------------------
		class ofstream	LightFilter3DFile(FILE_FILTER_LIGHT3D, ios::binary);
		m_LightFTP.SaveToFile(LightFilter3DFile);
		LightFilter3DFile.close();	

		*/
		
		//------------------------------------------------------------	
		// Load  Light3D FilterPack
		//------------------------------------------------------------	
		class ifstream	LightFilter3DFile2;//(FILE_FILTER_LIGHT3D, ios::binary);
		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_FILTER_LIGHT3D").c_str(), LightFilter3DFile2))
			return false;
		m_LightFTP.LoadFromFile(LightFilter3DFile2);
		LightFilter3DFile2.close();

		//------------------------------------------------------
		// lightBuffer 檬扁拳
		//------------------------------------------------------		
		m_LightBufferFilter.Init( SCREENLIGHT_WIDTH, SCREENLIGHT_HEIGHT );		
  		
		m_pLightBufferTexture = new CSpriteSurface;
		m_pLightBufferTexture->InitTextureSurface(SCREENLIGHT_WIDTH, SCREENLIGHT_HEIGHT, 0, CDirect3D::GetPixelFormat4444());	
	
	}
	//------------------------------------------------------------	
	//
	//  2D 矫具贸府甫 困茄 Light Filter
	//
	//------------------------------------------------------------	
	else
	{
		/*
		//------------------------------------------------------
		// Light FilterPack 檬扁拳
		//------------------------------------------------------
		m_LightFTP.Init( MAX_LIGHT_SETSIZE );

		//------------------------------------------------------
		// LightFilter 积己
		//------------------------------------------------------		
		int maxLight = 14;
		int maxCenter = 9;
		int lightRange[MAX_LIGHT_SETSIZE] = { 20, 26, 36, 46, 56, 66, 76, 86, 96, 106, 116, 126 };
		int l,i,j;

		// MAX_LIGHT_SETSIZE父怒 灌扁狼 农扁 辆幅啊 乐促.		
		for (l=0; l<MAX_LIGHT_SETSIZE; l++)
		{
			int lightRangeHalf = lightRange[l]>>1;	
			int maxK = maxCenter,maxLight;
			float gap = (float)(maxK,1)/(float)lightRangeHalf;	// k蔼捞 0~24鳖瘤..

			m_LightFTP[l].Init( lightRange[l], lightRange[l] );
			BYTE* pLight;
			for (i=0; i<lightRange[l]; i++)
			{
				pLight = m_LightFTP[l].GetFilter( i );
				for (j=0; j<lightRange[l]; j++)
				{
					int k = sqrt( 
								abs(i-lightRangeHalf)*abs(i-lightRangeHalf) 
								, abs(j-lightRangeHalf)*abs(j-lightRangeHalf) 
							);

					k = (float)k * gap;
				
					if (k <= maxCenter) k = 0;			// maxCenter鳖瘤绰 0
					else if (k >= maxK) k = maxLight;	// 逞栏搁 maxLight
					else k = k - maxCenter;				// 唱赣瘤绰 - maxCenter

					// 灌扁狼 SET窜拌甫 父电促.
					//k >>= shiftValue;

					//*pLight = k;
					*pLight = (maxLight-k)<<1;//31 - ((maxLight - k)<<1);
					pLight ++;
				}
			}
		}	
		
		//------------------------------------------------------------	
		// Save  Light2D FilterPack
		//------------------------------------------------------------
		class ofstream	LightFilter2DFile(FILE_FILTER_LIGHT2D, ios::binary);
		m_LightFTP.SaveToFile(LightFilter2DFile);
		LightFilter2DFile.close();	
		*/

		//------------------------------------------------------
		// lightBuffer 檬扁拳
		//------------------------------------------------------		
		m_LightBufferFilter.Init( SCREENLIGHT_WIDTH, SCREENLIGHT_HEIGHT );	
		
		//------------------------------------------------------------	
		// Load  Light2D FilterPack
		//------------------------------------------------------------	
		class ifstream	LightFilter2DFile2;//(FILE_FILTER_LIGHT2D, ios::binary);
		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_FILTER_LIGHT2D").c_str(), LightFilter2DFile2))
			return false;
		m_LightFTP.LoadFromFile(LightFilter2DFile2);
		LightFilter2DFile2.close();

		m_p2DLightPixelWidth = new int [SCREENLIGHT_WIDTH];
		m_p2DLightPixelHeight = new int [SCREENLIGHT_HEIGHT];

		//----------------------------------------------------------------
		// LightBuffer狼 茄 痢捞 狼固窍绰 拳搁惑狼 pixel啊肺 辨捞
		//----------------------------------------------------------------
		// 啊肺狼 钦 = 100 * 8 = 800
		BYTE x1,y1=0;
		if(g_MyFull)
		{
			x1=16;
			y1=16;
		}
		else
		{
			x1=12;
			y1=13;
		}
		const int pPixelWidth[SCREENLIGHT_WIDTH] = 
		{
			x1, y1, x1, y1, x1,	y1, x1, y1, 
			x1, y1, x1, y1, x1, y1, x1, y1, 
			x1, y1, x1, y1, x1, y1, x1, y1, 
			x1, y1, x1, y1, x1, y1, x1, y1, 
			x1, y1, x1, y1, x1, y1, x1, y1, 
			x1, y1, x1, y1, x1, y1, x1, y1, 
			x1, y1, x1, y1, x1, y1, x1, y1, 
			x1, y1, x1, y1, x1, y1, x1, y1 
		};
/*
		const int pPixelWidth[SCREENLIGHT_WIDTH] = 
		{
			16, 16, 16, 16, 16,	16, 16, 16, 
			16, 16, 16, 16, 16, 16, 16, 16, 
			16, 16, 16, 16, 16, 16, 16, 16, 
			16, 16, 16, 16, 16, 16, 16, 16, 
			16, 16, 16, 16, 16, 16, 16, 16, 
			16, 16, 16, 16, 16, 16, 16, 16, 
			16, 16, 16, 16, 16, 16, 16, 16, 
			16, 16, 16, 16, 16, 16, 16, 16 
		};
*/

		//----------------------------------------------------------------
		// LightBuffer狼 茄 痢捞 狼固窍绰 拳搁惑狼 pixel技肺 辨捞
		//----------------------------------------------------------------
		// 俺荐绰 SCREENLIGHT_HEIGHT	
		// total蔼捞 CLIPSURFACE_HEIGHT客 鞍栏搁 等促.
		// 泅犁绰 600.  10*24 + 9*40	
		//----------------------------------------------------------------
		if(g_MyFull)
		{
			x1=12;
			y1=12;
		}
		else
		{
			x1=10;
			y1=9;
		}
		const int pPixelHeight[SCREENLIGHT_HEIGHT] = 
		{
			x1,y1,y1,x1, y1,y1,x1,y1,
			x1,y1,y1,x1, y1,y1,x1,y1,
			x1,y1,y1,x1, y1,y1,x1,y1,
			x1,y1,y1,x1, y1,y1,x1,y1,
			x1,y1,y1,x1, y1,y1,x1,y1,
			x1,y1,y1,x1, y1,y1,x1,y1,
			x1,y1,y1,x1, y1,y1,x1,y1,
			x1,y1,y1,x1, y1,y1,x1,y1,
		};
		/*
		//修改为768
		//----------------------------------------------------------------
		const int pPixelHeight[SCREENLIGHT_HEIGHT] = 
		{
			
			12,12,12,12, 12,12,12,12,
			12,12,12,12, 12,12,12,12,
			12,12,12,12, 12,12,12,12,
			12,12,12,12, 12,12,12,12,
			12,12,12,12, 12,12,12,12,
			12,12,12,12, 12,12,12,12,
			12,12,12,12, 12,12,12,12,
			12,12,12,12, 12,12,12,12,

		};
		*/

		for (int i=0; i<SCREENLIGHT_WIDTH; i++)
		{
			m_p2DLightPixelWidth[i] = pPixelWidth[i];
		}

		for (i=0; i<SCREENLIGHT_HEIGHT; i++)
		{
			m_p2DLightPixelHeight[i] = pPixelHeight[i];
		}

	}



	//------------------------------------------------------------
	//
	// Player甫 啊府绰 ImageObject甸阑 贸府且 Filter
	//
	//------------------------------------------------------------
	/*
	register int i;
	register int k;

	//
	m_ImageObjectFilter.Init(200,200);
	for (i=0; i<200; i++)
	{			
		for (int j=0; j<200; j++)
		{							
			k = sqrt(abs(i-100)*abs(i-100) ,
					abs(j-100)*abs(j-100));
			//k = sqrt(abs(i-100)*abs(i-100) ,
					//abs(j-200)/2*abs(j-200)/2);

			//k -= 50;	// 后 傍埃

			k >>= 2;	// 何靛矾款(?) 傍埃

			k += 5;
			

			if (k>32) k=32;
			else if (k<1) k=1;

			// [ TEST CODE ]
			//if (k>25 && k<31)
			//	k = (rand()%(k-22))? 32:26;
			//else if (k>20 && k<26)
			//	k = (rand()%(77 - k*3))? 26:32;
			//else if (k>15 && k<21)
			//	k = (rand()%(102 - k*5))? 26:32;
			
			// 0苞 1蔼父...
			if (k > 26) 
				k = 1;
			else
				k = 0;

			m_ImageObjectFilter.SetFilter(j,i, k);
		}				
	}

	//------------------------------------------------------------	
	// Save  Light3D FilterPack
	//------------------------------------------------------------
	class ofstream	ImageObjectFilterFile(FILE_FILTER_IMAGEOBJECT, ios::binary);
	m_ImageObjectFilter.SaveToFile(ImageObjectFilterFile);
	ImageObjectFilterFile.close();	
	*/
	
	//------------------------------------------------------------	
	// Load  Light3D FilterPack
	//------------------------------------------------------------	
	if (m_ImageObjectFilter.IsNotInit())
	{
		class ifstream	ImageObjectFilterFile2;//(FILE_FILTER_IMAGEOBJECT, ios::binary);
		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_FILTER_IMAGEOBJECT").c_str(), ImageObjectFilterFile2))
			return false;
		m_ImageObjectFilter.LoadFromFile(ImageObjectFilterFile2);
		ImageObjectFilterFile2.close();
	}

	return true;
}

//----------------------------------------------------------------------
// Character狼 沥焊甫 积己茄促.
//----------------------------------------------------------------------
// m_CreatureFrame[辆幅][Action][Direction][Frame]
//----------------------------------------------------------------------
bool
MTopView::InitCreatureFrames()
{		
	//------------------------------------------------------------
	//
	//  Creature Body狼 Frame 沥焊甫 积己茄促.
	//
	//------------------------------------------------------------
		 
	/*
	m_CreatureFPK.Init(MAX_CREATURE_BODY);	

	//-----------------------
	// 霉锅掳 Creature(Woman)
	//-----------------------
	// 悼累 俺荐 沥狼
	m_CreatureFPK[0].Init(ACTION_MAX);

	// n锅掳 Sprite
	WORD n = 0;
	int i,j,k;

	// 阿 悼累俊 8规氢狼 FrameArray啊 乐促.
	for (k=0; k<ACTION_MAX; k++)
		m_CreatureFPK[0][k].Init(8);

	// 0~8锅掳 规氢俊绰 阿阿 8俺狼 Frame捞 乐促.
	for (i=0; i<8; i++)
	{
		for (k=0; k<ACTION_MAX; k++)
			m_CreatureFPK[0][k][i].Init(8);		
		
		// 8俺狼 Frame俊 措茄 沥焊甫 Set茄促.
		for (j=0; j<8; j++)
		{
			for (k=0; k<ACTION_MAX; k++)
				m_CreatureFPK[0][k][i][j].Set(n, 0,-40);			

			n ++;
		}  		
	}	

	//-----------------------
	// 滴锅掳 Creature(Skeleton)
	//-----------------------
	// 悼累 俺荐 沥狼
	m_CreatureFPK[1].Init(ACTION_MAX);

	// 阿 悼累俊 8规氢狼 FrameArray啊 乐促.
	for (k=0; k<ACTION_MAX; k++)
		m_CreatureFPK[1][k].Init(8);	

	// sprite 鉴辑
	int step[] = { 2,3,4,3,2,1,0,1 };


	// n锅掳 Sprite
	n = 64;

	// 0~8锅掳 规氢俊绰 阿阿 8俺狼 Frame捞 乐促.
	for (i=0; i<8; i++)
	{
		for (k=0; k<ACTION_MAX; k++)
			m_CreatureFPK[1][k][i].Init(8);		
		
		// 8俺狼 Frame俊 措茄 沥焊甫 Set茄促.
		for (j=0; j<8; j++)
		{			
			for (k=0; k<ACTION_MAX; k++)			
				m_CreatureFPK[1][k][i][j].Set(n,step[j], 0,-40);
		}  

		n += 5;
	}	


	//-----------------------
	// 技锅掳 Creature(Vamp1)
	//-----------------------
	// 悼累 俺荐 沥狼
	m_CreatureFPK[2].Init(ACTION_MAX_VAMPIRE);

	// n锅掳 Sprite
	n = 104;
	
	// 阿 悼累俊 8规氢狼 FrameArray啊 乐促.
	for (k=0; k<ACTION_MAX_VAMPIRE; k++)
		m_CreatureFPK[2][k].Init(8);

	//------------------------------------------------
	// ACTION_STAND - 8规氢俊 措秦辑
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// 规氢寸 Frame荐 
		m_CreatureFPK[2][ACTION_STAND][i].Init(16);
		
		int index = 0;
		// 4俺狼 Frame俊 措茄 沥焊甫 Set茄促.
		for (j=0; j<4; j++)
		{			
			m_CreatureFPK[2][ACTION_STAND][i][index++].Set(n, 15,-60);
			m_CreatureFPK[2][ACTION_STAND][i][index++].Set(n, 15,-60);
			m_CreatureFPK[2][ACTION_STAND][i][index++].Set(n, 15,-60);
			m_CreatureFPK[2][ACTION_STAND][i][index++].Set(n, 15,-60);
			n ++;
		}
	}

	//------------------------------------------------
	// ACTION_MOVE - 8规氢俊 措秦辑
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// 规氢寸 Frame荐 
		m_CreatureFPK[2][ACTION_MOVE][i].Init(8);
		
		// 8俺狼 Frame俊 措茄 沥焊甫 Set茄促.
		for (j=0; j<7; j++)
		{
			// 0~7 frame
			m_CreatureFPK[2][ACTION_MOVE][i][j].Set(n, 15,-60);
			n ++;
		}
		// 8锅掳 frame
		m_CreatureFPK[2][ACTION_MOVE][i][7].Set(n-7, 15,-60);
	}

	//------------------------------------------------
	// ACTION_ATTACK - 8规氢俊 措秦辑
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// 规氢寸 Frame荐 
		m_CreatureFPK[2][ACTION_ATTACK][i].Init(8);
		
		// 8俺狼 Frame俊 措茄 沥焊甫 Set茄促.
		for (j=0; j<8; j++)
		{
			m_CreatureFPK[2][ACTION_ATTACK][i][j].Set(n, 15,-60);
			n ++;
		}
	}

	//------------------------------------------------
	// ACTION_MAGIC - 8规氢俊 措秦辑
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// 规氢寸 Frame荐 
		m_CreatureFPK[2][ACTION_MAGIC][i].Init(8);
		
		// 8俺狼 Frame俊 措茄 沥焊甫 Set茄促.
		k=0;
		int kk=6;
		//int index = 0;
		for (j=0; j<8; j++)
		{
			//m_CreatureFPK[2][ACTION_MAGIC][i][index++].Set(n, 15,-60-k);
			//m_CreatureFPK[2][ACTION_MAGIC][i][index++].Set(n, 15,-60-k);
			m_CreatureFPK[2][ACTION_MAGIC][i][j].Set(n, 15,-60-k);
			n ++;
		
			if (j==4) kk=-6;
			k += kk;
		}
	}

	//------------------------------------------------
	// ACTION_DAMAGED - 8规氢俊 措秦辑
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// 规氢寸 Frame荐 
		m_CreatureFPK[2][ACTION_DAMAGED][i].Init(6);
		
		// 6俺狼 Frame俊 措茄 沥焊甫 Set茄促.
		for (j=0; j<6; j++)
		{
			m_CreatureFPK[2][ACTION_DAMAGED][i][j].Set(n, 15,-60);
			n ++;
		}
	}

	//------------------------------------------------
	// ACTION_DRAINED - 8规氢俊 措秦辑
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// 规氢寸 Frame荐 
		m_CreatureFPK[2][ACTION_DRAINED][i].Init(7);
		
		// 7俺狼 Frame俊 措茄 沥焊甫 Set茄促.
		for (j=0; j<5; j++)
		{
			m_CreatureFPK[2][ACTION_DRAINED][i][j].Set(n, 15,-60);
			n ++;
		}

		m_CreatureFPK[2][ACTION_DRAINED][i][5].Set(n-3, 15,-60);
		m_CreatureFPK[2][ACTION_DRAINED][i][6].Set(n-4, 15,-60);
	}

	//------------------------------------------------
	// ACTION_DIE - 8规氢俊 措秦辑
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// 规氢寸 Frame荐 
		m_CreatureFPK[2][ACTION_DIE][i].Init(4);
		
		// 4俺狼 Frame俊 措茄 沥焊甫 Set茄促.
		for (j=0; j<4; j++)
		{
			m_CreatureFPK[2][ACTION_DIE][i][j].Set(n, 15,-60);
			//n ++;
		}
	}

	//------------------------------------------------
	// ACTION_VAMPIRE_DRAIN - 8规氢俊 措秦辑
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// 规氢寸 Frame荐 
		m_CreatureFPK[2][ACTION_VAMPIRE_DRAIN][i].Init(7);
		
		// 7俺狼 Frame俊 措茄 沥焊甫 Set茄促.
		for (j=0; j<5; j++)
		{
			m_CreatureFPK[2][ACTION_VAMPIRE_DRAIN][i][j].Set(n, 15,-60);
			n ++;
		}

		m_CreatureFPK[2][ACTION_VAMPIRE_DRAIN][i][5].Set(n-3, 15,-60);
		m_CreatureFPK[2][ACTION_VAMPIRE_DRAIN][i][6].Set(n-4, 15,-60);
	}

	
	class ofstream packFile(FILE_CFRAME_CREATURE, ios::binary);
	class ofstream indexFile(FILE_CFRAMEINDEX_CREATURE, ios::binary);
	m_CreatureFPK.SaveToFile(packFile, indexFile);	
	packFile.close();
	indexFile.close();	
	*/
	
	///*
	// Load from File
	class ifstream file;//(FILE_CFRAME_CREATURE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_CREATURE").c_str(), file))
		return false;
	m_CreatureFPK.LoadFromFile(file);
	file.close();
	//*/	
	DrawTitleLoading();
	//m_CreatureFPK.InfoToFile("log\\CreatureFPK.txt");

	// Frame2 (粱厚)狼 磷扁 悼累阑 y谅钎 ,40究 秦霖促.
	/*
	ACTION_FRAME_ARRAY &zombie = m_CreatureFPK[2];

	DIRECTION_FRAME_ARRAY &dead = zombie[ACTION_DIE];

	for (int d=0; d<8; d++)
	{
		FRAME_ARRAY &fr = dead[d];

		for (int f=0; f<fr.GetSize(); f++)
		{
			fr[f].Set( fr[f].GetSpriteID(), fr[f].GetCX(), fr[f].GetCY(),40 );
		}
	}
	
	class ofstream packFile(FILE_CFRAME_CREATURE, ios::binary);
	class ofstream indexFile(FILE_CFRAMEINDEX_CREATURE, ios::binary);
	m_CreatureFPK.SaveToFile(packFile, indexFile);	
	packFile.close();
	indexFile.close();
	*/


	/*
	// Damaged 0,1阑 1,0栏肺 官槽促.
	for (int ct=0; ct<m_CreatureFPK.GetSize(); ct++)
	{
		DIRECTION_FRAME_ARRAY& damaged = m_CreatureFPK[ct][ACTION_DAMAGED];

		for (int d=0; d<8; d++)
		{
			FRAME_ARRAY& dfr = damaged[d];

			// 0苞 1 framd阑 官层霖促.
			CFrame frame0 = dfr[0];
			CFrame frame1 = dfr[1];

			CFrame temp = frame0;			
			frame0 = frame1;
			frame1 = temp;
		}
	}
	*/

	//m_CreatureFPK.InfoToFile("creature.txt");

	
	// 沥瘤 4frame阑 6frame栏肺 官层辑 促矫 18frame栏肺..
	/*
	DIRECTION_FRAME_ARRAY& stand = m_CreatureFPK[1][ACTION_STAND];
	DIRECTION_FRAME_ARRAY standTemp;
	DIRECTION_FRAME_ARRAY standTemp2;
	standTemp = stand;
	standTemp2.Init( 8 );
	for (int d=0; d<8; d++)
	{		
		// 4frame阑  6frame栏肺..		
		standTemp2[d].Init( standTemp[d].GetSize(),2 );
		for (int f=0; f<standTemp[d].GetSize(); f++)
		{
			standTemp2[d][f] = standTemp[d][f];			
		}
		standTemp2[d][4] = standTemp[d][2];	
		standTemp2[d][5] = standTemp[d][1];	

		// 6frame阑 18frame栏肺..
		stand[d].Init( standTemp2[d].GetSize()*3 );
		int ff=0;
		for (f=0; f<standTemp2[d].GetSize(); f++)
		{
			stand[d][ff++] = standTemp2[d][f];			
			stand[d][ff++] = standTemp2[d][f];
			stand[d][ff++] = standTemp2[d][f];
		}
	}	

	// 叭扁 悼累 6frame阑 12frame栏肺...
	DIRECTION_FRAME_ARRAY& move = m_CreatureFPK[1][ACTION_MOVE];
	DIRECTION_FRAME_ARRAY moveTemp;
	moveTemp = move;
	for (d=0; d<8; d++)
	{
		move[d].Init( moveTemp[d].GetSize()*3 );
		
		int ff = 0;
		for (int f=0; f<moveTemp[d].GetSize(); f++)
		{
			move[d][ff++] = moveTemp[d][f];
			move[d][ff++] = moveTemp[d][f];
			move[d][ff++] = moveTemp[d][f];
		}
	}

	class ofstream packFile(FILE_CFRAME_CREATURE, ios::binary);
	class ofstream indexFile(FILE_CFRAMEINDEX_CREATURE, ios::binary);
	m_CreatureFPK.SaveToFile(packFile, indexFile);	
	packFile.close();
	indexFile.close();	
	*/

	//------------------------------------------------------------
	//
	// Creature Shadow FPK - Loading
	//
	//------------------------------------------------------------
	class ifstream fileShadow;//(FILE_CFRAME_CREATURE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_CREATURE_SHADOW").c_str(), fileShadow))
		return false;
	m_CreatureShadowFPK.LoadFromFile(fileShadow);
	fileShadow.close();
	DrawTitleLoading();
	//m_CreatureShadowFPK.InfoToFile("log\\CreatureShadowFPK.txt");
	
	//------------------------------------------------------------
	//
	//  addon狼 Frame 沥焊甫 积己茄促.
	//
	//------------------------------------------------------------
	//
	/*
	m_AddonFPK.Init(MAX_CREATURE_ADDON);
	

	//--------------
	// 霉锅掳 渴
	//--------------
	// 1啊瘤 悼累父 乐促.
	m_AddonFPK[0].Init(1);
	m_AddonFPK[1].Init(1);
	m_AddonFPK[2].Init(1);
	m_AddonFPK[3].Init(1);

	// 弊 1啊瘤 悼累俊 8规氢狼 FrameArray啊 乐促.
	m_AddonFPK[0][0].Init(8);
	m_AddonFPK[1][0].Init(8);
	m_AddonFPK[2][0].Init(8);
	m_AddonFPK[3][0].Init(8);

	// n锅掳 tile
	//n = n + 80;
	n = 0;

	// 0~8锅掳 规氢俊绰 阿阿 8俺狼 Frame捞 乐促.
	for (i=0; i<8; i++)
	{
		m_AddonFPK[0][0][i].Init(8);
		m_AddonFPK[1][0][i].Init(8);
		m_AddonFPK[2][0][i].Init(8);
		m_AddonFPK[3][0][i].Init(8);
		
		// 8俺狼 Frame俊 措茄 沥焊甫 Set茄促.
		for (int j=0; j<8; j++)
		{
			// 83~122 : Shirt 弊覆捞 乐促绊 沁阑 版快
			m_AddonFPK[0][0][i][j].Set(n,step[j], 0,-40);

			// 123~162 Pants 弊覆捞 乐促绊 沁阑 版快
			m_AddonFPK[1][0][i][j].Set(n,40,step[j], 0,-40);

			// Boots
			m_AddonFPK[2][0][i][j].Set(n,80,step[j], 0,-40);

			// Cloak
			m_AddonFPK[3][0][i][j].Set(n,120,step[j], 0,-40);
		}

		n+=5;
	}	

	packFile.open(FILE_CFRAME_ADDON, ios::binary);
	indexFile.open(FILE_CFRAMEINDEX_ADDON, ios::binary);
	m_AddonFPK.SaveToFile(packFile, indexFile);	
	packFile.close();
	indexFile.close();
	*/

	///*
	
	class ifstream AdvancementOustersFile;//(FILE_CFRAME_ADDON_MALE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_OUSTERS").c_str(), AdvancementOustersFile))
		return false;
	m_AdvancementOustersFPK.LoadFromFile(AdvancementOustersFile);
	AdvancementOustersFile.close();	

	class ifstream AdvancementOustersShadowFile;//(FILE_CFRAME_ADDON_MALE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_OUSTERS_SHADOW").c_str(), AdvancementOustersFile))
		return false;
	m_AdvancementOustersShadowFPK.LoadFromFile(AdvancementOustersFile);
	AdvancementOustersFile.close();	

	class ifstream AdvancementVampireManFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_VAMPIRE_MAN").c_str(), 
		AdvancementVampireManFile))
		return false;
	m_AdvancementVampireManFPK.LoadFromFile( AdvancementVampireManFile );
	AdvancementVampireManFile.close();

	//add by viva
	//--------------------------------vampire.cfpk
	class ifstream NewVampireFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_NEW_CLASS_VAMPIRE").c_str(), NewVampireFile))	return false;
	m_NewVampireFPK.LoadFromFile( NewVampireFile );
	NewVampireFile.close();
	//--------------------------------vampireShdow.cfpk
	class ifstream NewVampireShadowFile;
	if(!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_NEW_CLASS_VAMPIRE_SHADOW").c_str(), NewVampireShadowFile))	return false;
	m_NewVampireShadowFPK.LoadFromFile( NewVampireShadowFile );
	NewVampireShadowFile.close();

	//--------------------------------SlayerMan.cfpk(AddonMan.cfpk)
	class ifstream NewSlayerManFile;
	if(!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_SLAYER_MAN").c_str(), NewSlayerManFile))	return false;
	m_NewSlayerManFPK.LoadFromFile( NewSlayerManFile );
	NewSlayerManFile.close();
	//--------------------------------SlayerManShadow.cfpk(AddonManShadow.cfpk)
	class ifstream NewSlayerManShadowFile;
	if(!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_SLAYER_MAN_SHADOW").c_str(), NewSlayerManShadowFile))	return false;
	m_NewSlayerManShadowFPK.LoadFromFile( NewSlayerManShadowFile );
	NewSlayerManShadowFile.close();

	//---------------------------------SlayerWoman.cfpk(AddonWoman.cfpk)
	class ifstream NewSlayerWomanFile;
	if(!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_SLAYER_WOMAN").c_str(), NewSlayerWomanFile))	return false;
	m_NewSlayerWomanFPK.LoadFromFile( NewSlayerWomanFile );
	NewSlayerWomanFile.close();
	//---------------------------------SlayerWoman.cfpk(AddonManShadow.cfpk)
	class ifstream NewSlayerWomanShadowFile;
	if(!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_SLAYER_WOMAN_SHADOW").c_str(), NewSlayerWomanShadowFile))	return false;
	m_NewSlayerWomanShadowFPK.LoadFromFile( NewSlayerWomanShadowFile );
	NewSlayerWomanShadowFile.close();
	////end

	class ifstream AdvancementVampireManShadowFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_VAMPIRE_MAN_SHADOW").c_str(), 
		AdvancementVampireManShadowFile))
		return false;
	m_AdvancementVampireManShadowFPK.LoadFromFile( AdvancementVampireManShadowFile );
	AdvancementVampireManShadowFile.close();

	class ifstream AdvancementVampireWomanFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_VAMPIRE_WOMAN").c_str(), 
		AdvancementVampireWomanFile))
		return false;
	m_AdvancementVampireWomanFPK.LoadFromFile( AdvancementVampireWomanFile );
	AdvancementVampireWomanFile.close();

	class ifstream AdvancementVampireWomanShadowFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_VAMPIRE_WOMAN_SHADOW").c_str(), 
		AdvancementVampireWomanShadowFile))
		return false;
	m_AdvancementVampireWomanShadowFPK.LoadFromFile( AdvancementVampireWomanShadowFile );
	AdvancementVampireWomanShadowFile.close();


	class ifstream AdvancementSlayerManFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_SLAYER_MAN").c_str(), 
		AdvancementSlayerManFile))
		return false;
	m_AdvancementSlayerManFPK.LoadFromFile( AdvancementSlayerManFile );
	AdvancementSlayerManFile.close();

	class ifstream AdvancementSlayerManShadowFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_SLAYER_MAN_SHADOW").c_str(), 
		AdvancementSlayerManShadowFile))
		return false;
	m_AdvancementSlayerManShadowFPK.LoadFromFile( AdvancementSlayerManShadowFile );
	AdvancementSlayerManShadowFile.close();


	class ifstream AdvancementSlayerWomanFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_SLAYER_WOMAN").c_str(), 
		AdvancementSlayerWomanFile))
		return false;
	m_AdvancementSlayerWomanFPK.LoadFromFile( AdvancementSlayerWomanFile );
	AdvancementSlayerWomanFile.close();

	class ifstream AdvancementSlayerWomanShadowFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_SLAYER_WOMAN_SHADOW").c_str(), 
		AdvancementSlayerWomanShadowFile))
		return false;
	m_AdvancementSlayerWomanShadowFPK.LoadFromFile( AdvancementSlayerWomanShadowFile );
	AdvancementSlayerWomanShadowFile.close(); 

	/*
	//------------------------------------------------
	// Load from File
	//------------------------------------------------
	class ifstream AdvancementSlayerFile;//(FILE_CFRAME_ADDON_MALE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_SLAYER").c_str(), AdvancementSlayerFile))
		return false;
	m_AdvancementSlayerFPK.LoadFromFile(AdvancementSlayerFile);
	AdvancementSlayerFile.close();

	class ifstream AdvancementVampireFile;//(FILE_CFRAME_ADDON_MALE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_VAMPIRE").c_str(), AdvancementVampireFile))
		return false;
	m_AdvancementVampireFPK.LoadFromFile(AdvancementVampireFile);
	AdvancementVampireFile.close();
	
	class ifstream AdvancementSlayerShadowFile;//(FILE_CFRAME_ADDON_MALE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_SLAYER_SHADOW").c_str(), AdvancementSlayerFile))
		return false;
	m_AdvancementSlayerShadowFPK.LoadFromFile(AdvancementSlayerFile);
	AdvancementSlayerFile.close();

	class ifstream AdvancementVampireShadowFile;//(FILE_CFRAME_ADDON_MALE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_VAMPIRE_SHADOW").c_str(), AdvancementVampireFile))
		return false;
	m_AdvancementVampireShadowFPK.LoadFromFile(AdvancementVampireFile);
	AdvancementVampireFile.close();

*/
	//------------------------------------------------
	// Load from File
	//------------------------------------------------
	class ifstream AddonFile2;//(FILE_CFRAME_ADDON_MALE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADDON").c_str(), AddonFile2))
		return false;
	m_AddonFPK.LoadFromFile(AddonFile2);
	AddonFile2.close();
	class ifstream OustersFile2;//(FILE_CFRAME_ADDON_MALE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_OUSTERS").c_str(), OustersFile2))
		return false;
	m_OustersFPK.LoadFromFile(OustersFile2);
	OustersFile2.close();
	DrawTitleLoading();

	//------------------------------------------------
	// 弊覆磊 - Load from File
	//------------------------------------------------
	class ifstream AddonShadowFile2;//(FILE_CFRAME_ADDON_MALE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADDON_SHADOW").c_str(), AddonShadowFile2))
		return false;
	m_AddonShadowFPK.LoadFromFile(AddonShadowFile2);
	AddonShadowFile2.close();
	
	class ifstream OustersShadowFile2;//(FILE_CFRAME_ADDON_MALE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_OUSTERS_SHADOW").c_str(), OustersShadowFile2))
		return false;
	m_OustersShadowFPK.LoadFromFile(OustersShadowFile2);
	OustersShadowFile2.close();
	

	DrawTitleLoading();

	// 醚悼累俊 赣府墨遏 狐柳巴 眠啊
	// 捞贰档 购啊 救登匙!!
	/*
	int add;
	for (add=0; add<MAX_ADDONID_MALE; add++)
	{
		for (int i=0; i<8; i++)
		{
			if (add!=ADDONID_GUN_AR_MALE)		
				m_AddonMaleFPK[add][ACTION_SLAYER_GUN_AR][i] = m_AddonMaleFPK[add][ACTION_SLAYER_GUN_TR][i];

			if (add!=ADDONID_GUN_SMG_MALE)		
				m_AddonMaleFPK[add][ACTION_SLAYER_GUN_SMG][i] = m_AddonMaleFPK[add][ACTION_SLAYER_GUN_TR][i];
		}
	}
	
	for (add=0; add<MAX_ADDONID_FEMALE; add++)
	{
		for (int i=0; i<8; i++)
		{
			if (add!=ADDONID_GUN_AR_FEMALE)		
				m_AddonFemaleFPK[add][ACTION_SLAYER_GUN_AR][i] = m_AddonMaleFPK[add][ACTION_SLAYER_GUN_TR][i];

			if (add!=ADDONID_GUN_SMG_FEMALE)		
				m_AddonFemaleFPK[add][ACTION_SLAYER_GUN_SMG][i] = m_AddonMaleFPK[add][ACTION_SLAYER_GUN_TR][i];
		}
	}
	*/

	// 函版..
	/*
	int add, a, d, f;
	for (add=0; add<MAX_ADDONID_MALE; add++)
	{
		ACTION_FRAME_ARRAY& AFA = m_AddonMaleFPK[add];

		// motorcycle俊绰 action眠啊
		if (add==ADDONID_MOTORCYCLE_MALE)
		{			
			for (a=0; a<ACTION_MAX_SLAYER; a++)
			{
				if (a==1)
				{
					DIRECTION_FRAME_ARRAY& DFA = AFA[a];

					for (d=0; d<8; d++)
					{
						FRAME_ARRAY& FA = DFA[d];
						FRAME_ARRAY newFA;
						newFA.Init( 18 );
					
						newFA[0] = FA[0];
						newFA[1] = FA[1];
						newFA[2] = FA[2];
						newFA[3] = FA[1];
						for (f=4; f<18; f++)
						{
							newFA[f] = FA[f%4];
						}

						FA = newFA;
					}
				}
				else
				{
					DIRECTION_FRAME_ARRAY& DFA = AFA[a];

					for (d=0; d<8; d++)
					{
						FRAME_ARRAY& FA = DFA[d];
						FA.Init( 18 );
					
						FA[0] = AFA[1][d][0];
						FA[1] = AFA[1][d][1];
						FA[2] = AFA[1][d][2];
						FA[3] = AFA[1][d][1];
						for (f=4; f<18; f++)
						{
							FA[f] = FA[f%4];
						}
					}
				}
			}
		}
		else
		{
			for (a=0; a<AFA.GetSize(); a++)
			{
				DIRECTION_FRAME_ARRAY& DFA = AFA[a];

				for (d=0; d<DFA.GetSize(); d++)
				{
					FRAME_ARRAY& FA = DFA[d];
					
					// motor悼累阑 3 --> 4 frame栏肺 官槽促.
					if (a==ACTION_SLAYER_MOTOR_MOVE)
					{
						if (FA.GetSize()!=0)
						{
							FRAME_ARRAY newFA;
							newFA.Init( 4 );
							// 3俺楼府 FA甫 4俺肺 父甸绢具 茄促.
							for (f=0; f<FA.GetSize(); f++)
							{
								CFrame& frame = FA[f];
								
								newFA[f] = FA[f];
							}

							newFA[3] = FA[2];

							FA = newFA;
						}
					}
					
				}
			}
		}
	}
	*/

	//m_AddonMaleFPK.InfoToFile("log\\addonMale.txt");
	//m_AddonFemaleFPK.InfoToFile("log\\addonFemale.txt");
	//m_AddonMaleShadowFPK.InfoToFile("log\\addonMaleShadow.txt");
	//m_AddonFemaleShadowFPK.InfoToFile("log\\addonFemaleShadow.txt");

	/*
	// [巢磊] 漠悼累篮 6 frame栏肺 官操扁...
	for (int add=0; add<m_AddonMaleFPK.GetSize(); add++)
	{	
		ACTION_FRAME_ARRAY &addon = m_AddonMaleFPK[add];

		DIRECTION_FRAME_ARRAY &SWORD = addon[ACTION_SLAYER_SWORD];
		DIRECTION_FRAME_ARRAY &BLADE = addon[ACTION_SLAYER_BLADE];

		addon[ACTION_SLAYER_SWORD_2] = SWORD;
		addon[ACTION_SLAYER_BLADE_2] = BLADE;

		for (int d=0; d<8; d++)
		{
			FRAME_ARRAY &fr1 = SWORD[d];
			FRAME_ARRAY &fr2 = BLADE[d];

			if (fr1.GetSize()!=0)
			{			
				FRAME_ARRAY temp;
				temp.Init( 6 );

				temp[0] = fr1[0];
				temp[1] = fr1[1];
				temp[2] = fr1[2];
				temp[3] = fr1[3];
				temp[4] = fr1[4];
				temp[5] = fr1[fr1.GetSize()-1];

				fr1 = temp;
			}

			if (fr2.GetSize()!=0)
			{			
				FRAME_ARRAY temp;
				temp.Init( 6 );

				temp[0] = fr2[0];
				temp[1] = fr2[1];
				temp[2] = fr2[2];
				temp[3] = fr2[3];
				temp[4] = fr2[4];
				temp[5] = fr2[fr2.GetSize()-1];

				fr2 = temp;
			}
		}
	}
	
	// [咯磊] 漠悼累篮 6 frame栏肺 官操扁...
	for (add=0; add<m_AddonFemaleFPK.GetSize(); add++)
	{	
		ACTION_FRAME_ARRAY &addon = m_AddonFemaleFPK[add];

		DIRECTION_FRAME_ARRAY &SWORD = addon[ACTION_SLAYER_SWORD];
		DIRECTION_FRAME_ARRAY &BLADE = addon[ACTION_SLAYER_BLADE];

		addon[ACTION_SLAYER_SWORD_2] = SWORD;
		addon[ACTION_SLAYER_BLADE_2] = BLADE;

		for (int d=0; d<8; d++)
		{
			FRAME_ARRAY &fr1 = SWORD[d];
			FRAME_ARRAY &fr2 = BLADE[d];

			if (fr1.GetSize()!=0)
			{			
				FRAME_ARRAY temp;
				temp.Init( 6 );

				temp[0] = fr1[0];
				temp[1] = fr1[1];
				temp[2] = fr1[2];
				temp[3] = fr1[3];
				temp[4] = fr1[4];
				temp[5] = fr1[fr1.GetSize()-1];

				fr1 = temp;
			}

			if (fr2.GetSize()!=0)
			{			
				FRAME_ARRAY temp;
				temp.Init( 6 );

				temp[0] = fr2[0];
				temp[1] = fr2[1];
				temp[2] = fr2[2];
				temp[3] = fr2[3];
				temp[4] = fr2[4];
				temp[5] = fr2[fr2.GetSize()-1];

				fr2 = temp;
			}
		}
	}

	class ofstream packFileMale(FILE_CFRAME_ADDON_MALE, ios::binary);
	class ofstream indexFileMale(FILE_CFRAMEINDEX_ADDON_MALE, ios::binary);
	m_AddonMaleFPK.SaveToFile(packFileMale, indexFileMale);	
	packFileMale.close();
	indexFileMale.close();

	class ofstream packFileFemale(FILE_CFRAME_ADDON_FEMALE, ios::binary);
	class ofstream indexFileFemale(FILE_CFRAMEINDEX_ADDON_FEMALE, ios::binary);
	m_AddonFemaleFPK.SaveToFile(packFileFemale, indexFileFemale);	
	packFileFemale.close();
	indexFileFemale.close();

	*/

	
	// 咯磊 醚 悼累 官操扁
	/*
	m_AddonFemaleFPK[ADDONID_GUN_TR_FEMALE][14] = m_AddonFemaleFPK[ADDONID_GUN_TR_FEMALE][7];
	m_AddonFemaleFPK[ADDONID_GUN_TR_FEMALE][7].Release();

	m_AddonFemaleFPK[ADDONID_GUN_SG_FEMALE][13] = m_AddonFemaleFPK[ADDONID_GUN_SG_FEMALE][8];
	m_AddonFemaleFPK[ADDONID_GUN_SG_FEMALE][8].Release();

	m_AddonFemaleFPK[ADDONID_GUN_AR_FEMALE][8] = m_AddonFemaleFPK[ADDONID_GUN_AR_FEMALE][13];
	m_AddonFemaleFPK[ADDONID_GUN_AR_FEMALE][13].Release();

	m_AddonFemaleFPK[ADDONID_GUN_SMG_FEMALE][7] = m_AddonFemaleFPK[ADDONID_GUN_SMG_FEMALE][14];
	m_AddonFemaleFPK[ADDONID_GUN_SMG_FEMALE][14].Release();
	*/

	//*/

	//m_AddonMaleFPK.InfoToFile("log\\addonMale.txt");
	//m_AddonFemaleFPK.InfoToFile("log\\addonFemale.txt");


	return true;
}

//----------------------------------------------------------------------
// Init Image Frames
//----------------------------------------------------------------------
// CImageFramePack			m_ItemTileFPK;			// Tile俊 乐绰 Item俊 措茄..
// m_ImageFrame[辆幅]
//----------------------------------------------------------------------
bool
MTopView::InitImageFrames()
{
	//------------------------------------------------------------
	//
	//  Item on Tile 沥焊甫 积己茄促.
	//
	//------------------------------------------------------------
	/*
	m_ItemTileFPK.Init( 28 );
	
	// Item 0	
	for (int i=0; i<28; i++)
		m_ItemTileFPK[i].Set(i, 10, 10);

	class ofstream packFile(FILE_IFRAME_ITEMTILE, ios::binary);
	class ofstream indexFile(FILE_IFRAMEINDEX_ITEMTILE, ios::binary);

	m_ItemTileFPK.SaveToFile(packFile, indexFile);

	packFile.close();
	indexFile.close();
	*/

	///*
	// Load from File
	class ifstream file2;//(FILE_IFRAME_ITEMTILE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_IFRAME_ITEMTILE").c_str(), file2))
		return false;
	m_ItemTileFPK.LoadFromFile(file2);
	file2.close();
	//*/



	return true;
}

//----------------------------------------------------------------------
// Init Animation Frames
//----------------------------------------------------------------------
// m_ImageObjectFPK[辆幅][Frame]
//----------------------------------------------------------------------
bool
MTopView::InitAnimationFrames()
{		
	//------------------------------------------------------------
	//
	//  Animation Frame 沥焊甫 积己茄促.
	//
	//------------------------------------------------------------

	//------------------------------------------------------------
	//
	//  捞 沥焊绰 FrameSet捞促.
	//
	//  SpriteSet俊辑狼 SpriteID甫 历厘窍绊 乐绢具 茄促.
	//
	//------------------------------------------------------------
	
	class ifstream file2;//(FILE_AFRAME_ANIMATIONOBJECT, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_AFRAME_ANIMATIONOBJECT").c_str(), file2))
		return false;
	m_ImageObjectFPK.LoadFromFile(file2);
	file2.close();

	class ifstream file3;//(FILE_AFRAME_ANIMATIONOBJECT, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_AFRAME_ANIMATIONOBJECT_SHADOW").c_str(), file3))
		return false;
	m_ImageObjectShadowFPK.LoadFromFile(file3);
	file3.close();	

//#ifdef OUTPUT_DEBUG
//	m_InteractionObjectFPK.Init( 1 );
//	
//	//------------------------------------------
//	// 霉锅掳 Animation Object
//	//------------------------------------------
//	m_InteractionObjectFPK[0].Init(10);	
//	m_InteractionObjectFPK[0][0].Set(0, 0, -48);
//	m_InteractionObjectFPK[0][1].Set(1, 0, -48);
//	m_InteractionObjectFPK[0][2].Set(2, 0, -48);
//	m_InteractionObjectFPK[0][3].Set(3, 0, -48);
//	m_InteractionObjectFPK[0][4].Set(4, 0, -48);
//	m_InteractionObjectFPK[0][5].Set(4, 0, -48);
//	m_InteractionObjectFPK[0][6].Set(3, 0, -48);
//	m_InteractionObjectFPK[0][7].Set(2, 0, -48);
//	m_InteractionObjectFPK[0][8].Set(1, 0, -48);
//	m_InteractionObjectFPK[0][9].Set(0, 0, -48);
//	
//
//	class ofstream iopackFile(g_pFileDef->getProperty("FILE_AFRAME_INTERACTIONOBJECT").c_str(), ios::binary);
//	class ofstream ioindexFile(g_pFileDef->getProperty("FILE_AFRAMEINDEX_INTERACTIONOBJECT").c_str(), ios::binary);
//	m_InteractionObjectFPK.SaveToFile(iopackFile, ioindexFile);
//	iopackFile.close();
//	ioindexFile.close();	
//#endif

	///*
	// Load from File
//	class ifstream iofile2;//(FILE_AFRAME_ANIMATIONOBJECT, ios::binary);
//	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_AFRAME_INTERACTIONOBJECT").c_str(), iofile2))
//		return false;
//	m_InteractionObjectFPK.LoadFromFile(iofile2);
//	iofile2.close();
	//*/

	//------------------------------------------------------------
	//
	// Item Drop
	//
	//------------------------------------------------------------	
	class ifstream itemdropfile2;//(FILE_AFRAME_ANIMATIONOBJECT, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_AFRAME_ITEMDROP").c_str(), itemdropfile2))
		return false;
	m_ItemDropFPK.LoadFromFile(itemdropfile2);
	itemdropfile2.close();

//	for(int kkk = 803; kkk <= 804; kkk++)
//	{
//		for(int bbb = 0; bbb < 6; bbb++)
//		{
//			CFrame &Frame = m_ItemDropFPK[ kkk ][ bbb ];
//			int spriteID = Frame.GetSpriteID()+(kkk-802)*4;
//			Frame.Set(spriteID, Frame.GetCX(), Frame.GetCY());
//		}
//
//	}
//	class ofstream itemdropfile3(g_pFileDef->getProperty("FILE_AFRAME_ITEMDROP").c_str(), ios::binary);
//	class ofstream itemdropfile4(g_pFileDef->getProperty("FILE_AFRAMEINDEX_ITEMDROP").c_str(), ios::binary);
//	m_ItemDropFPK.SaveToFile(itemdropfile3, itemdropfile4);
//	itemdropfile4.close();
//	itemdropfile3.close();

	//------------------------------------------------------------	
	// 促矫 积己
	//------------------------------------------------------------	
	/*
	const int numItems	= 121;
	const int numFrames	= 6;

	// frame 函拳 蔼 
	// sprite绰 item付促 4俺究牢单 6 frame阑 父甸绢具 茄促.
	int nFrame[numFrames] = { 3, 0, 1, 2, 3, 0 };

	m_ItemDropFPK.Init( numItems );	// 傈眉 item 俺荐父怒狼 ani frame

	int spriteID = 0;
	for (int i=0; i<numItems; i++)
	{
		m_ItemDropFPK[i].Init( numFrames );		// 阿阿 6 frame究

		for (int f=0; f<numFrames; f++)
		{
			int sid = spriteID+nFrame[f];

			// 吝缴俊 坷档废 谅钎 焊沥..
			int cx = g_TILE_X_HALF - (m_ItemDropSPK[sid].GetWidth()>>1);
			int cy = g_TILE_Y_HALF - (m_ItemDropSPK[sid].GetHeight()>>1);

			m_ItemDropFPK[i][f].Set( sid, cx, cy );
		}
		
		spriteID += 4;
	}

	//------------------------------------------------------------	
	// 历厘
	//------------------------------------------------------------	
	class ofstream packFile(FILE_AFRAME_ITEMDROP, ios::binary);
	class ofstream indexFile(FILE_AFRAMEINDEX_ITEMDROP, ios::binary);
	m_ItemDropFPK.SaveToFile(packFile, indexFile);
	packFile.close();
	indexFile.close();
	*/

		//------------------------------------------------------------
	//
	//  Item Broken 沥焊甫 积己茄促.
	//
	//------------------------------------------------------------
	///*
#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
	const int maxSlayerItemBroken = (MSlayerGear::MAX_GEAR_SLAYER+2-12) * 3;
	const int maxVampireItemBroken = (MVampireGear::MAX_GEAR_VAMPIRE+2-12) * 3;
	const int maxOustersItemBroken = (MOustersGear::MAX_GEAR_OUSTERS+1-12) * 3;

	m_ItemBrokenFPK.Init( 5 );	// 浇饭捞绢 巢,咯 + 轨颇捞绢 巢,咯


	// { SpriteID, CX, CY }
	//------------------------------------------------------------
	// 浇饭捞绢 巢磊
	//------------------------------------------------------------
	int slayerMale[maxSlayerItemBroken][3] =
	{
		// 沥惑利牢芭
		{ SPRITEID_NULL,	33, 1 },	// 葛磊,
		{ SPRITEID_NULL,	 38, 18 },	// 格吧捞
		{ SPRITEID_NULL,	16, 19 },	// 惑狼
		{ 1,	62, 19 },	// 规菩	
		{ 2,	19, 11 },	// 漠		
		{ SPRITEID_NULL,	9, 49 },	// 厘癌
		{ SPRITEID_NULL,	27, 54 },	// 骇飘
		{ SPRITEID_NULL,	26, 59 },	// 窍狼
		{ 5,	59, 51 },	// 迫骂2		
		{ 5,	24, 51 },	// 迫骂1
		{ 6,	20, 60 },	// 馆瘤1
		{ 6,	28, 60 },	// 馆瘤2
		{ 6,	55, 60 },	// 馆瘤3
		{ 6,	63, 60 },	// 馆瘤4
		{ SPRITEID_NULL,	20, 122 },	// 脚惯
		{ 3,	18, 12 },	// 绞磊啊
		{ 4,	20, 12 },	// 醚

		// 距埃 何辑柳芭
		{ 9,	40, 10 },	// 葛磊,
		{ 10,	42, 19 },	// 格吧捞
		{ 7,	33, 18 },	// 惑狼
		{ 14,	62, 19 },	// 规菩	
		{ 15,	19, 11 },	// 漠		
		{ 11,	30, 35 },	// 厘癌
		{ 12,	37, 36 },	// 骇飘
		{ 8,	36, 35 },	// 窍狼
		{ 18,	59, 51 },	// 迫骂2		
		{ 18,	24, 51 },	// 迫骂1
		{ 19,	20, 60 },	// 馆瘤1
		{ 19,	28, 60 },	// 馆瘤2
		{ 19,	55, 60 },	// 馆瘤3
		{ 19,	63, 60 },	// 馆瘤4
		{ 13,	36, 72 },	// 脚惯
		{ 16,	18, 12 },	// 绞磊啊
		{ 17,	20, 12 },	// 醚

		// 促 何辑廉 啊绰芭
		{ 22,	40, 10 },	// 葛磊,
		{ 23,	42, 19 },	// 格吧捞
		{ 20,	33, 18 },	// 惑狼
		{ 27,	62, 19 },	// 规菩	
		{ 28,	19, 11 },	// 漠		
		{ 24,	30, 35 },	// 厘癌
		{ 25,	37, 36 },	// 骇飘
		{ 21,	36, 35 },	// 窍狼
		{ 31,	59, 51 },	// 迫骂2		
		{ 31,	24, 51 },	// 迫骂1
		{ 32,	20, 60 },	// 馆瘤1
		{ 32,	28, 60 },	// 馆瘤2
		{ 32,	55, 60 },	// 馆瘤3
		{ 32,	63, 60 },	// 馆瘤4
		{ 26,	36, 72 },	// 脚惯
		{ 29,	18, 12 },	// 绞磊啊
		{ 30,	20, 12 },	// 醚
	};

	//------------------------------------------------------------
	// 浇饭捞绢 咯磊
	//------------------------------------------------------------
	int slayerFemale[maxSlayerItemBroken][3] =
	{
		// 沥惑利牢芭
		{ SPRITEID_NULL,	33, 1 },	// 葛磊,
		{ SPRITEID_NULL,	 38, 18 },	// 格吧捞
		{ SPRITEID_NULL,	16, 19 },	// 惑狼
		{ 1,	62, 19 },	// 规菩	
		{ 2,	19, 11 },	// 漠		
		{ SPRITEID_NULL,	9, 49 },	// 厘癌
		{ SPRITEID_NULL,	27, 54 },	// 骇飘
		{ SPRITEID_NULL,	26, 59 },	// 窍狼
		{ 5,	59, 51 },	// 迫骂2		
		{ 5,	24, 51 },	// 迫骂1
		{ 6,	20, 60 },	// 馆瘤1
		{ 6,	28, 60 },	// 馆瘤2
		{ 6,	55, 60 },	// 馆瘤3
		{ 6,	63, 60 },	// 馆瘤4
		{ SPRITEID_NULL,	20, 122 },	// 脚惯
		{ 3,	18, 12 },	// 绞磊啊
		{ 4,	20, 12 },	// 醚

		// 距埃 何辑柳芭
		{ 36,	40, 10 },	// 葛磊,
		{ 37,	41, 20 },	// 格吧捞
		{ 34,	33, 20 },	// 惑狼
		{ 14,	62, 19 },	// 规菩	
		{ 15,	19, 11 },	// 漠		
		{ 38,	30, 37 },	// 厘癌
		{ 39,	40, 32 },	// 骇飘
		{ 35,	38, 35 },	// 窍狼
		{ 18,	59, 51 },	// 迫骂2		
		{ 18,	24, 51 },	// 迫骂1
		{ 19,	20, 60 },	// 馆瘤1
		{ 19,	28, 60 },	// 馆瘤2
		{ 19,	55, 60 },	// 馆瘤3
		{ 19,	63, 60 },	// 馆瘤4
		{ 40,	39, 55 },	// 脚惯
		{ 16,	18, 12 },	// 绞磊啊
		{ 17,	20, 12 },	// 醚

		// 促 何辑廉 啊绰芭
		{ 43,	40, 10 },	// 葛磊,
		{ 44,	41, 20 },	// 格吧捞
		{ 41,	33, 20 },	// 惑狼
		{ 14,	62, 19 },	// 规菩	
		{ 15,	19, 11 },	// 漠		
		{ 45,	30, 37 },	// 厘癌
		{ 46,	40, 32 },	// 骇飘
		{ 42,	38, 35 },	// 窍狼
		{ 18,	59, 51 },	// 迫骂2		
		{ 18,	24, 51 },	// 迫骂1
		{ 19,	20, 60 },	// 馆瘤1
		{ 19,	28, 60 },	// 馆瘤2
		{ 19,	55, 60 },	// 馆瘤3
		{ 19,	63, 60 },	// 馆瘤4
		{ 47,	39, 55 },	// 脚惯
		{ 16,	18, 12 },	// 绞磊啊
		{ 17,	20, 12 },	// 醚
	};

	//------------------------------------------------------------
	// 轨颇捞绢 巢磊
	//------------------------------------------------------------
	int vampireMale[maxVampireItemBroken][3] =
	{
		// 沥惑利牢芭
		{ SPRITEID_NULL,	40, 22 },	// 格吧捞
		{ SPRITEID_NULL,	19, 10 },	// 惑狼
		{ 49,	22, 45 },	// 迫骂1
		{ 49,	62, 45 },	// 迫骂2
		{ 50,	18, 54 },	// 馆瘤1
		{ 50,	26, 54 },	// 馆瘤2
		{ 50,	58, 54 },	// 馆瘤3
		{ 50,	66, 54 },	// 馆瘤4
		{ 62,	22, 18 },	// 蓖吧捞1
		{ 62,	62, 18 },	// 蓖吧捞2
		{ 65,	16, 22 },	// Weapon1
		{ 65,	64, 22 },	// Weapon2
		{ 68,	18, 63 },	// Amulet1
		{ 68,	26, 63 },	// Amulet2
		{ 68,	58, 63 },	// Amulet3
		{ 68,	66, 63 },	// Amulet4

		// 距埃 何辑柳芭
		{ 52,	43, 18 },	// 格吧捞
		{ 51,	30, 14 },	// 惑狼
		{ 53,	22, 45 },	// 迫骂1
		{ 53,	62, 45 },	// 迫骂2
		{ 54,	18, 54 },	// 馆瘤1
		{ 54,	26, 54 },	// 馆瘤2
		{ 54,	58, 54 },	// 馆瘤3
		{ 54,	66, 54 },	// 馆瘤4
		{ 63,	22, 18 },	// 蓖吧捞1
		{ 63,	62, 18 },	// 蓖吧捞2
		{ 66,	16, 22 },	// Weapon1
		{ 66,	64, 22 },	// Weapon2
		{ 69,	18, 63 },	// Amulet1
		{ 69,	26, 63 },	// Amulet2
		{ 69,	58, 63 },	// Amulet3
		{ 69,	66, 63 },	// Amulet4

		// 促 何辑柳芭
		{ 56,	43, 18 },	// 格吧捞
		{ 55,	30, 14 },	// 惑狼
		{ 57,	22, 45 },	// 迫骂1
		{ 57,	62, 45 },	// 迫骂2
		{ 58,	18, 54 },	// 馆瘤1
		{ 58,	26, 54 },	// 馆瘤2
		{ 58,	58, 54 },	// 馆瘤3
		{ 58,	66, 54 },	// 馆瘤4
		{ 64,	22, 18 },	// 蓖吧捞1
		{ 64,	62, 18 },	// 蓖吧捞2
		{ 67,	16, 22 },	// Weapon1
		{ 67,	64, 22 },	// Weapon2
		{ 70,	18, 63 },	// Amulet1
		{ 70,	26, 63 },	// Amulet2
		{ 70,	58, 63 },	// Amulet3
		{ 70,	66, 63 },	// Amulet4
	};

	//------------------------------------------------------------
	// 轨颇捞绢 咯磊
	//------------------------------------------------------------
	int vampireFemale[maxVampireItemBroken][3] =
	{
		// 沥惑利牢芭
		{ SPRITEID_NULL,	40, 22 },	// 格吧捞
		{ SPRITEID_NULL,	19, 10 },	// 惑狼
		{ 49,	22, 45 },	// 迫骂1
		{ 49,	62, 45 },	// 迫骂2
		{ 50,	18, 54 },	// 馆瘤1
		{ 50,	58, 54 },	// 馆瘤3
		{ 50,	26, 54 },	// 馆瘤2		
		{ 50,	66, 54 },	// 馆瘤4
		{ 62,	22, 18 },	// 蓖吧捞1
		{ 62,	62, 18 },	// 蓖吧捞2
		{ 65,	16, 22 },	// Weapon1
		{ 65,	64, 22 },	// Weapon2
		{ 68,	18, 63 },	// Amulet1
		{ 68,	26, 63 },	// Amulet2
		{ 68,	58, 63 },	// Amulet3
		{ 68,	66, 63 },	// Amulet4

		// 距埃 何辑柳芭
		{ 52,	42, 18 },	// 格吧捞
		{ 60,	31, 19 },	// 惑狼
		{ 53,	22, 45 },	// 迫骂1
		{ 53,	62, 45 },	// 迫骂2
		{ 54,	18, 54 },	// 馆瘤1
		{ 54,	58, 54 },	// 馆瘤3
		{ 54,	26, 54 },	// 馆瘤2		
		{ 54,	66, 54 },	// 馆瘤4
		{ 63,	22, 18 },	// 蓖吧捞1
		{ 63,	62, 18 },	// 蓖吧捞2
		{ 66,	16, 22 },	// Weapon1
		{ 66,	64, 22 },	// Weapon2
		{ 69,	18, 63 },	// Amulet1
		{ 69,	26, 63 },	// Amulet2
		{ 69,	58, 63 },	// Amulet3
		{ 69,	66, 63 },	// Amulet4

		// 促 何辑柳芭
		{ 56,	42, 18 },	// 格吧捞
		{ 61,	31, 19 },	// 惑狼
		{ 57,	22, 45 },	// 迫骂1
		{ 57,	62, 45 },	// 迫骂2
		{ 58,	18, 54 },	// 馆瘤1
		{ 58,	58, 54 },	// 馆瘤3
		{ 58,	26, 54 },	// 馆瘤2		
		{ 58,	66, 54 },	// 馆瘤4
		{ 64,	22, 18 },	// 蓖吧捞1
		{ 64,	62, 18 },	// 蓖吧捞2
		{ 67,	16, 22 },	// Weapon1
		{ 67,	64, 22 },	// Weapon2
		{ 70,	18, 63 },	// Amulet1
		{ 70,	26, 63 },	// Amulet2
		{ 70,	58, 63 },	// Amulet3
		{ 70,	66, 63 },	// Amulet4
	};		

	//------------------------------------------------------------
	// 酒快胶磐令
	//------------------------------------------------------------
	int Ousters[maxOustersItemBroken][3] =
	{
		// 沥惑利牢芭
		{ 77,	39, 8 },				// 辑努复
		{ SPRITEID_NULL ,	34, 18 },				// 渴
		{ 89,	17, 33 },				// 哭颊				// 府胶撇复篮 92
		{ 89,	61, 33 },				// 坷弗颊
		{ SPRITEID_NULL,	40, 69 },				// 脚惯
		{ 74,	19, 18},				// 鞠胶龟靛1
		{ 74,	57, 18},				// 鞠胶龟靛2
		{ 83,	19, 63 },				// 傅1
		{ 83,	65, 63 },				// 傅2
		{ 86,	35, 22 },				// 格吧捞1
		{ 86,	42, 22 },				// 格吧捞2
		{ 86,	49, 22 },				// 格吧捞3
		{ 80,	26, 63 },				// 沥飞籍1		
		{ 80,	58, 63 },				// 沥飞籍2
		{ 80,	26, 71 },				// 沥飞籍3
		{ 80,	58, 71 },				// 沥飞籍 4 
		{ 92,	61, 33 },				// 府胶撇复 

		// 吝埃
		{ 78,	40, 22 },				// 辑努复
		{ 72,	19, 10 },	// 渴
		{ 90,	22, 45 },				// 哭颊				// 府胶撇复篮 92
		{ 90,	62, 45 },				// 坷弗颊
		{ 95,	18, 54 },				// 脚惯
		{ 75,	26, 54 },				// 鞠胶龟靛1
		{ 75,	58, 54 },				// 鞠胶龟靛2
		{ 84,	66, 54 },				// 傅1
		{ 84,	22, 18 },				// 傅2
		{ 87,	62, 18 },				// 格吧捞1
		{ 87,	16, 22 },				// 格吧捞2
		{ 87,	16, 22 },				// 格吧捞3
		
		{ 81,	26, 63 },				// 沥飞籍1		
		{ 81,	58, 63 },				// 沥飞籍2
		{ 81,	26, 71 },				// 沥飞籍3
		{ 81,	58, 71 },				// 沥飞籍 4 

		{ 93,	61, 33 },				// 府胶撇复  

		// 唱慧芭
		{ 79,	40, 22 },				// 辑努复
		{ 73,	19, 10 },	// 渴
		{ 91,	22, 45 },				// 哭颊				// 府胶撇复篮 92
		{ 91,	62, 45 },				// 坷弗颊
		{ 96,	18, 54 },				// 脚惯
		{ 76,	26, 54 },				// 鞠胶龟靛1
		{ 76,	58, 54 },				// 鞠胶龟靛2
		{ 85,	66, 54 },				// 傅1
		{ 85,	22, 18 },				// 傅2
		{ 88,	62, 18 },				// 格吧捞1
		{ 88,	16, 22 },				// 格吧捞2
		{ 88,	16, 22 },				// 格吧捞3
		{ 82,	26, 63 },				// 沥飞籍1		
		{ 82,	58, 63 },				// 沥飞籍2
		{ 82,	26, 71 },				// 沥飞籍3
		{ 82,	58, 71 },				// 沥飞籍 4 
		{ 94,	61, 33 },				// 府胶撇复
	};		
	

	int j;	
		
	m_ItemBrokenFPK[0].Init( maxSlayerItemBroken );
	m_ItemBrokenFPK[1].Init( maxSlayerItemBroken );
	m_ItemBrokenFPK[2].Init( maxVampireItemBroken );
	m_ItemBrokenFPK[3].Init( maxVampireItemBroken );
	m_ItemBrokenFPK[4].Init( maxOustersItemBroken );

	for (j=0; j<maxSlayerItemBroken; j++)
	{		
		m_ItemBrokenFPK[0][j].Set(slayerMale[j][0], slayerMale[j][1]-30, slayerMale[j][2]-10);			
	}

	for (j=0; j<maxSlayerItemBroken; j++)
	{		
		m_ItemBrokenFPK[1][j].Set(slayerFemale[j][0], slayerFemale[j][1]-30, slayerFemale[j][2]-10);			
	}

	for (j=0; j<maxVampireItemBroken; j++)
	{		
		m_ItemBrokenFPK[2][j].Set(vampireMale[j][0], vampireMale[j][1]-30, vampireMale[j][2]-10);			
	}

	for (j=0; j<maxVampireItemBroken; j++)
	{		
		m_ItemBrokenFPK[3][j].Set(vampireFemale[j][0], vampireFemale[j][1]-30, vampireFemale[j][2]-10);			
	}
	
	for (j=0; j<maxOustersItemBroken; j++)
	{
		m_ItemBrokenFPK[4][j].Set(Ousters[j][0], Ousters[j%(maxOustersItemBroken/3)][1]-30, Ousters[j%(maxOustersItemBroken/3)][2]-10);
	}
	
	class ofstream packFile(g_pFileDef->getProperty("FILE_AFRAME_ITEMBROKEN").c_str(), ios::binary);
	class ofstream indexFile(g_pFileDef->getProperty("FILE_AFRAMEINDEX_ITEMBROKEN").c_str(), ios::binary);

	m_ItemBrokenFPK.SaveToFile(packFile, indexFile);

	packFile.close();
	indexFile.close();
#endif
	//*/

	///*
	// Load from File
	class ifstream fileItemBroken2;//(FILE_IFRAME_ITEMTILE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_AFRAME_ITEMBROKEN").c_str(), fileItemBroken2))
		return false;
	m_ItemBrokenFPK.LoadFromFile(fileItemBroken2);
	fileItemBroken2.close();
	//*/


	return true;
}

//----------------------------------------------------------------------
// Init Effect Frames
//----------------------------------------------------------------------
// m_EffectAlphaFPK[辆幅][Direction][Frame]
//----------------------------------------------------------------------
bool
MTopView::InitEffectFrames()
{
	//------------------------------------------------
	//
	//   Effect NormalSPK
	//
	//------------------------------------------------	
	// 0锅 : HolyWater 朝酒啊绰 effect
	// 1锅 : Bomb 朝酒啊绰 effect
	/*
	m_EffectNormalFPK.Init( 2 );

	for (int e=0; e<2; e++)
	{
		m_EffectNormalFPK[e].Init( 8 );

		for (int d=0; d<8; d++)
		{
			m_EffectNormalFPK[e][d].Init( 4 );	// 阿 4 frame

			for (int f=0; f<4; f++)
			{
				int spriteID = e*4 + f;
				int cx = -(m_EffectNormalSPK[spriteID].GetWidth()>>1);
				int cy = -(m_EffectNormalSPK[spriteID].GetHeight()>>1);

				m_EffectNormalFPK[e][d][f].Set(spriteID, cx, cy, 0);
			}
		}	
	}

	class ofstream packNormalFile(FILE_EFRAME_NORMALEFFECT, ios::binary);
	class ofstream indexNormalFile(FILE_EFRAMEINDEX_NORMALEFFECT, ios::binary);
	m_EffectNormalFPK.SaveToFile(packNormalFile, indexNormalFile);
	packNormalFile.close();
	indexNormalFile.close();
	*/
	/*
	m_EffectNormalFPK.Init(MAX_NORMALEFFECT);

	//-------------------
	// 霉锅掳 Effect
	//-------------------
	// 8规氢捞 乐促.
	m_EffectNormalFPK[0].Init(8);		// Bomb

	for (int d=0; d<8; d++)
	{		
		// Bomb
		m_EffectNormalFPK[0][d].Init(5);
		m_EffectNormalFPK[0][d][0].Set(0, 0, -48);
		m_EffectNormalFPK[0][d][1].Set(1, 0, -48);
		m_EffectNormalFPK[0][d][2].Set(2, 0, -48);
		m_EffectNormalFPK[0][d][3].Set(1, 0, -48);
		m_EffectNormalFPK[0][d][4].Set(0, 0, -48);
	}

	class ofstream packNormalFile(FILE_EFRAME_NORMALEFFECT, ios::binary);
	class ofstream indexNormalFile(FILE_EFRAMEINDEX_NORMALEFFECT, ios::binary);
	m_EffectNormalFPK.SaveToFile(packNormalFile, indexNormalFile);
	packNormalFile.close();
	indexNormalFile.close();
	*/

	//--------------------------------------------------
	// 窍唱狼 FRR阑 佬绢辑.. 
	// EFRR肺 函券矫难辑..
	// EFPK甫 积己茄促.
	//--------------------------------------------------
	// [ TEST CODE ]
	/*
	class ifstream fileFA;//(FILE_EFRAME_NORMALEFFECT, ios::binary);
	if (!FileOpenBinary("Data\\bomb.frr", fileFA))
		return false;

	FRAME_ARRAY FA;
	FA.LoadFromFile( fileFA );
	fileFA.close();

	m_EffectNormalFPK.Init( 1 );
	// FRR --> EFRR
	m_EffectNormalFPK[0].Init( 8 );
	for (int d=0; d<8; d++)
	{
		m_EffectNormalFPK[0][d].Init( FA.GetSize() );
		for (int f=0; f<FA.GetSize(); f++)
		{
			CFrame& frame = FA[f];
			CEffectFrame& eframe = m_EffectNormalFPK[0][d][f];

			eframe.Set( frame.GetSpriteID(), frame.GetCX(), frame.GetCY(), 0);
		}
	}

	class ofstream packNormalFile(FILE_EFRAME_NORMALEFFECT, ios::binary);
	class ofstream indexNormalFile(FILE_EFRAMEINDEX_NORMALEFFECT, ios::binary);
	m_EffectNormalFPK.SaveToFile(packNormalFile, indexNormalFile);
	packNormalFile.close();
	indexNormalFile.close();
	*/

	///*
	// Load from NormalFile
	class ifstream NormalFile2;//(FILE_EFRAME_NORMALEFFECT, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_EFRAME_NORMALEFFECT").c_str(), NormalFile2))
		return false;
	m_EffectNormalFPK.LoadFromFile(NormalFile2);
	NormalFile2.close();
	//*/

	// 技何沥焊 免仿窍扁
	/*
	class ofstream infoFile("Log\\EffectNormal.txt");	

	for (int type=0; type<m_EffectNormalFPK.GetSize(); type++)
	{
		for (int dir=0; dir<m_EffectNormalFPK[type].GetSize(); dir++)
		{
			for (int frame=0; frame<m_EffectNormalFPK[type][dir].GetSize(); frame++)
			{
				int spriteID = m_EffectNormalFPK[type][dir][frame].GetSpriteID();
				int cx = m_EffectNormalFPK[type][dir][frame].GetCX();
				int cy = m_EffectNormalFPK[type][dir][frame].GetCY();
				infoFile << "[" << type << "][" << dir << "][" << frame << "] = ( " << spriteID << ",  " << cx << ", " << cy << " ) " << endl;
			}
		}	
	}
	infoFile.close();
	*/

	//------------------------------------------------
	//
	//   Effect AlphaSPK
	//
	//------------------------------------------------
	/*
	m_EffectAlphaFPK.Init(MAX_ALPHAEFFECT);

	//-------------------
	// 霉锅掳 Effect
	//-------------------
	// 8规氢捞 乐促.
	m_EffectAlphaFPK[0].Init(8);		// Fire	
	m_EffectAlphaFPK[1].Init(8);		// Lightning
	m_EffectAlphaFPK[2].Init(8);		// Light
	m_EffectAlphaFPK[3].Init(8);		// Darkness
	m_EffectAlphaFPK[4].Init(8);		// Aura(Blue)
	m_EffectAlphaFPK[5].Init(8);		// Aura(Red)
	m_EffectAlphaFPK[6].Init(8);		// Sword	
	m_EffectAlphaFPK[7].Init(8);		// Comet
	m_EffectAlphaFPK[8].Init(8);		// Star
	m_EffectAlphaFPK[9].Init(8);		// Flare

	for (d=0; d<8; d++)
	{
		// 弊 8规氢究俊  8 Frames
		m_EffectAlphaFPK[0][d].Init(8);
		m_EffectAlphaFPK[1][d].Init(8);

		m_EffectAlphaFPK[0][d][0].Set(0, -35,-120, 1);		// Fire
		m_EffectAlphaFPK[0][d][1].Set(1, -35,-120, 1);		// Fire
		m_EffectAlphaFPK[0][d][2].Set(2, -35,-120, 1);		// Fire
		m_EffectAlphaFPK[0][d][3].Set(3, -35,-120, 2);		// Fire
		m_EffectAlphaFPK[0][d][4].Set(4, -35,-120, 2);		// Fire
		m_EffectAlphaFPK[0][d][5].Set(5, -35,-120, 1);		// Fire
		m_EffectAlphaFPK[0][d][6].Set(6, -35,-120, 1);		// Fire
		m_EffectAlphaFPK[0][d][7].Set(7, -35-120, 1);		// Fire


		for (int f=0; f<8; f++)
		{					
			m_EffectAlphaFPK[1][d][f].Set(f,8, 0,-64, 1);	// Lightning
		}

		// Light
		m_EffectAlphaFPK[2][d].Init(2);
		m_EffectAlphaFPK[2][d][0].Set(16, 0, -48, 1);
		m_EffectAlphaFPK[2][d][1].Set(17, 0, -48, 1);
		//m_EffectAlphaFPK[2][d][1].Set(17, 0, -50);
		//m_EffectAlphaFPK[2][d][2].Set(16, 0, -54);
		//m_EffectAlphaFPK[2][d][3].Set(17, 0, -60);
		//m_EffectAlphaFPK[2][d][4].Set(16, 0, -62);
		//m_EffectAlphaFPK[2][d][5].Set(17, 0, -58);
		//m_EffectAlphaFPK[2][d][6].Set(16, 0, -50);
		//m_EffectAlphaFPK[2][d][7].Set(17, 0, -48);
		
		// Darkness
		m_EffectAlphaFPK[3][d].Init(2);
		m_EffectAlphaFPK[3][d][0].Set(18, 0, -48, 0);
		m_EffectAlphaFPK[3][d][1].Set(19, 0, -48, 0);

		// Aura(Blue)
		m_EffectAlphaFPK[4][d].Init(5);
		m_EffectAlphaFPK[4][d][0].Set(20, -60, -80, 2);
		m_EffectAlphaFPK[4][d][1].Set(22, -60, -80, 2);
		m_EffectAlphaFPK[4][d][2].Set(21, -60, -80, 2);
		m_EffectAlphaFPK[4][d][3].Set(22, -60, -80, 2);
		m_EffectAlphaFPK[4][d][4].Set(20, -60, -80, 2);

		// Aura(Red)
		m_EffectAlphaFPK[5][d].Init(5);
		m_EffectAlphaFPK[5][d][0].Set(23, -60, -80, 2);
		m_EffectAlphaFPK[5][d][1].Set(25, -60, -80, 2);
		m_EffectAlphaFPK[5][d][2].Set(24, -60, -80, 2);
		m_EffectAlphaFPK[5][d][3].Set(25, -60, -80, 2);
		m_EffectAlphaFPK[5][d][4].Set(23, -60, -80, 2);

		// Sword
		m_EffectAlphaFPK[6][d].Init(12);
		for (f=0; f<12; f++)
		{
			m_EffectAlphaFPK[6][d][f].Set(26,f, 0, -48, 1);
		}

		// Comet
		m_EffectAlphaFPK[7][d].Init(7);
		for (f=0; f<7; f++)
		{
			m_EffectAlphaFPK[7][d][f].Set(38,f, -20, -160, 2);
		}

		// Star
		m_EffectAlphaFPK[8][d].Init(24);
		for (f=0; f<24; f++)
		{
			m_EffectAlphaFPK[8][d][f].Set(45,(f>>1), -50, -70, 1);
		}		

		// Flare
		m_EffectAlphaFPK[9][d].Init(10);		
		m_EffectAlphaFPK[9][d][0].Set(57, -21, -105, 4);
		m_EffectAlphaFPK[9][d][1].Set(58, -21, -106, 4);
		m_EffectAlphaFPK[9][d][2].Set(59, -21, -108, 4);
		m_EffectAlphaFPK[9][d][3].Set(60, -21, -112, 4);
		m_EffectAlphaFPK[9][d][4].Set(61, -21, -115, 5);
		m_EffectAlphaFPK[9][d][5].Set(62, -21, -112, 4);
		m_EffectAlphaFPK[9][d][6].Set(63, -21, -108, 4);
		m_EffectAlphaFPK[9][d][7].Set(64, -21, -106, 4);
		m_EffectAlphaFPK[9][d][8].Set(65, -21, -105, 4);
		m_EffectAlphaFPK[9][d][9].Set(66, -21, -104, 4);
	}

	class ofstream packAlphaFile(FILE_EFRAME_ALPHAEFFECT, ios::binary);
	class ofstream indexAlphaFile(FILE_EFRAMEINDEX_ALPHAEFFECT, ios::binary);
	m_EffectAlphaFPK.SaveToFile(packAlphaFile, indexAlphaFile);
	packAlphaFile.close();
	indexAlphaFile.close();
	*/

	///*
	// Load from AlphaFile
	class ifstream AlphaFile2;//(FILE_EFRAME_ALPHAEFFECT, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_EFRAME_ALPHAEFFECT").c_str(), AlphaFile2))
		return false;
	//class ifstream AlphaFile2("effect.efpk", ios::binary);
	m_EffectAlphaFPK.LoadFromFile(AlphaFile2);
	AlphaFile2.close();
	//*/

	/*
	// 技何 沥焊 免仿窍扁
	//m_EffectAlphaFPK.InfoToFile("Log\\Effect.txt");
	class ofstream infoFile("Log\\Effect.txt");	

	for (int type=0; type<m_EffectAlphaFPK.GetSize(); type++)
	{
		for (int dir=0; dir<m_EffectAlphaFPK[type].GetSize(); dir++)
		{
			for (int frame=0; frame<m_EffectAlphaFPK[type][dir].GetSize(); frame++)
			{
				int spriteID = m_EffectAlphaFPK[type][dir][frame].GetSpriteID();
				int cx = m_EffectAlphaFPK[type][dir][frame].GetCX();
				int cy = m_EffectAlphaFPK[type][dir][frame].GetCY();
				infoFile << "[" << type << "][" << dir << "][" << frame << "] = ( " << spriteID << ",  " << cx << ", " << cy << " ) " << endl;
			}
		}	
	}
	infoFile.close();
	*/
	/*
	//---------------------------------------------------------------
	// 阿 effect狼 frame荐 免仿窍扁
	//---------------------------------------------------------------
	class ofstream file("log\\EffectList.txt");
	for (int e=0; e<m_EffectAlphaFPK.GetSize(); e++)
	{
		DIRECTION_EFFECTFRAME_ARRAY& DEA = m_EffectAlphaFPK[e];
		
		file << "[" << e << "] " << (int)DEA[0].GetSize() << endl;
	}
	file.close();
	*/
	

	//---------------------------------------------------------------
	// 蝴 炼例窍扁
	//---------------------------------------------------------------
	/*
	int light;
	
	// effect 辆幅
	for (int e=0; e<m_EffectAlphaFPK.GetSize(); e++)
	{
		DIRECTION_EFFECTFRAME_ARRAY& effect = m_EffectAlphaFPK[e];

		// 规氢喊
		for (int d=0; d<8; d++)
		{
			EFFECTFRAME_ARRAY& EA = effect[d];
			
			// frame喊
			for (int f=0; f<EA.GetSize(); f++)
			{
				CEffectFrame &frame = EA[f];

				switch (e)
				{
					case EFFECTSPRITETYPE_LIGHT_1 :
						light = 5 + (f & 0x01);
					break;
					case EFFECTSPRITETYPE_LIGHT_2 :
						light = 6 + (f & 0x01);
					break;
					case EFFECTSPRITETYPE_LIGHT_3 :
						light = 4 + (f & 0x01);
					break;
					default : 
						light = 3;
				}
				
				frame.Set( frame.GetSpriteID(), frame.GetCX(), frame.GetCY(), light);
			}	
		}
	}

	class ofstream packAlphaFile(FILE_EFRAME_ALPHAEFFECT, ios::binary);
	class ofstream indexAlphaFile(FILE_EFRAMEINDEX_ALPHAEFFECT, ios::binary);
	m_EffectAlphaFPK.SaveToFile(packAlphaFile, indexAlphaFile);
	packAlphaFile.close();
	indexAlphaFile.close();
	*/

	/*
	//---------------------------------------------------------------
	// 乔 sprite 吝汗 frame持扁
	//---------------------------------------------------------------
	DIRECTION_EFFECTFRAME_ARRAY& effect = m_EffectAlphaFPK[EFFECTSPRITETYPE_SLAYER_DIE];

	for (int d=0; d<8; d++)
	{
		EFFECTFRAME_ARRAY& EA = effect[d];

		int frameMax = EA.GetSize()*3 + 30;

		EFFECTFRAME_ARRAY newEA;

		newEA.Init( frameMax );

		// 葛滴 场 悼累...
		for (int i=0; i<frameMax; i++)
		{
			newEA[i] = EA[EA.GetSize()-1];
		}
		
		// 滴frame究, 沥瘤 frame 拌加, 茄 frame芭操肺
		for (i=0; i<EA.GetSize(); i++)
		{
			newEA[i*2] = EA[i];
			newEA[i*2,1] = EA[i];

			newEA[frameMax-1 - i] = EA[i];
		}	

		EA = newEA;
	}

	class ofstream packAlphaFile(FILE_EFRAME_ALPHAEFFECT, ios::binary);
	class ofstream indexAlphaFile(FILE_EFRAMEINDEX_ALPHAEFFECT, ios::binary);
	m_EffectAlphaFPK.SaveToFile(packAlphaFile, indexAlphaFile);
	packAlphaFile.close();
	indexAlphaFile.close();
	*/

	
//	#ifdef OUTPUT_DEBUG
//
//		m_EffectShadowFPK.Init( 1 );
//
//		for (int se=0; se<1; se++)
//		{
//			m_EffectShadowFPK[se].Init( 8 );
//
//			for (int d=0; d<8; d++)
//			{
//				m_EffectShadowFPK[se][d].Init( 3 );
//
//				for (int f=0; f<3; f++)
//				{
//					m_EffectShadowFPK[se][d][f].Set( f, -110, -115, 6 );
//				}
//			}
//		}
//		
//
//		class ofstream packshadowEffectFile(g_pFileDef->getProperty("FILE_EFRAME_SHADOWEFFECT").c_str(), ios::binary);
//		class ofstream indexshadowEffectFile(g_pFileDef->getProperty("FILE_EFRAMEINDEX_SHADOWEFFECT").c_str(), ios::binary);
//		m_EffectShadowFPK.SaveToFile(packshadowEffectFile, indexshadowEffectFile);
//		packshadowEffectFile.close();
//		indexshadowEffectFile.close();
//	#endif

	///*
	// Load from shadowEffectFile
	class ifstream shadowEffectFile2;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_EFRAME_SHADOWEFFECT").c_str(), shadowEffectFile2))
		return false;
	m_EffectShadowFPK.LoadFromFile(shadowEffectFile2);
	shadowEffectFile2.close();

	// Load from ScreenFile
	class ifstream ScreenFile2;//(FILE_EFRAME_ScreenEFFECT, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_EFRAME_SCREENEFFECT").c_str(), ScreenFile2))
		return false;
	//class ifstream ScreenFile2("effect.efpk", ios::binary);
	m_EffectScreenFPK.LoadFromFile(ScreenFile2);
	ScreenFile2.close();

	return true;
}

//----------------------------------------------------------------------
// Load Minimap
//----------------------------------------------------------------------
void
MTopView::LoadMinimap(const char* filename)//, MZoneInfo* pZoneInfo)
{
	/* // 2001.7.14 林籍贸府
	//------------------------------------------------------------
	//
	//				Minimap	
	//
	//------------------------------------------------------------
	//------------------------------------------------------------	
	// Load  Clothes SpritePack	
	//------------------------------------------------------------
	class ifstream	MapTest1;
	if (!FileOpenBinary(filename, MapTest1))
		return;
	m_pMinimapSPR->LoadFromFile( MapTest1 );
	*/

	//------------------------------------------------------------	
	// 钎矫秦具且 ZoneInfo啊 乐绰 版快
	//------------------------------------------------------------	
	/*
	if (pZoneInfo!=NULL)
	{
		register int i;
		register int j;

		int spWidth = m_pMinimapSPR->GetWidth();
		int spHeight = m_pMinimapSPR->GetHeight();
		int zoneWidth = pZoneInfo->GetWidth();
		int zoneHeight = pZoneInfo->GetHeight();

		WORD* pPixel;
		int	count, transCount, colorCount;

		int spX, spY, zoneX, zoneY;

		// (256,128) --> (256,256)
		// (spX, spX) ---> (zoneX, zoneY)
		// zoneX = spX * zoneHeight / spHeight
		CSpriteSurface::s_Value1 = 16;
		CSpriteSurface::s_Value2 = 16;
		WORD green = CDirectDraw::Color(0, 20, 0);

		//----------------------------------------------------------------
		// Sprite磊眉甫 官层滚赴促. - -;
		//----------------------------------------------------------------
		for (spY=0; spY<spHeight; spY++)
		{
			zoneY = spY * zoneHeight / spHeight;

			spX = 0;
			pPixel = m_pMinimapSPR->GetPixelLine( spY );

			count = *pPixel++;		// 馆汗 雀荐
			
			//----------------------------------------------------------------
			// 茄 临俊 措茄 眉农..
			//----------------------------------------------------------------
			for (i=0; i<count; i++)
			{
				transCount = *pPixel++;
				colorCount = *pPixel++;

				spX += transCount;

				for (j=0; j<colorCount; j++)
				{
					zoneX = spX * zoneWidth / spWidth;
					
					//---------------------------------------------------------
					// 救傈瘤措捞搁 踌祸栏肺 馆捧疙..
					//---------------------------------------------------------
					if (pZoneInfo->IsSafeSector(zoneX, zoneY))
					{
						// 1痢 踌祸栏肺 alpha blending - -;
						CSpriteSurface::memcpyAlpha(pPixel, &green, 1);
					}

					// 促澜 痢
					pPixel++;
					spX++;
				}
			}
		}
	}
	*/

	/* // 2001.7.14 林籍贸府
	//------------------------------------------------------------
	// 3D 啊加捞搁...
	//------------------------------------------------------------
	if (CDirect3D::IsHAL())
	{
		InitMinimapTexture();
	}

	// m_SectorToMinimapWidth - 茄 sector绰 啊肺 割 pixel牢啊?
	// m_SectorToMinimapHeight - 茄 sector绰 技肺 割 pixel牢啊?
	if (g_pZone->GetWidth()==0 || g_pZone->GetHeight()==0)
	{
		m_SectorToMinimapWidth	= 0;
		m_SectorToMinimapHeight = 0;
	}
	else
	{
		m_SectorToMinimapWidth = (float)m_pMinimapSPR->GetWidth() / (float)g_pZone->GetWidth();
		m_SectorToMinimapHeight = (float)m_pMinimapSPR->GetHeight() / (float)g_pZone->GetHeight();
	}

	MapTest1.close();
	*/
}

//----------------------------------------------------------------------
// Init MinimapTexture
//----------------------------------------------------------------------
//void
//MTopView::InitMinimapTexture(CSpriteSurface *minimap_surface)
//{
//	// 胶橇扼捞飘甫 父甸磊 by 睛矮
//	if(minimap_surface != NULL)
//	{
//		if(minimap_surface->Lock())
//		{
//			WORD *pSurface = (WORD*)minimap_surface->GetSurfacePointer();
//			m_pMinimapSPR->SetPixelNoColorkey(pSurface, minimap_surface->GetSurfacePitch(), minimap_surface->GetWidth(), minimap_surface->GetHeight());
//			
//			minimap_surface->Unlock();
//		}
//	}
//
//	//------------------------------------------------------------
//	// 3D 啊加捞搁...
//	//------------------------------------------------------------
//	if (!CDirect3D::IsHAL())
//		return;
//
//	// 2001.7.14 林籍贸府
//	if (m_pMinimapTexture!=NULL)
//	{
//		delete m_pMinimapTexture;
//		m_pMinimapTexture = NULL;
//	}
//
//	m_pMinimapTexture = new CSpriteSurface;
//
//	if (m_pMinimapTexture==NULL)
//	{
//		DEBUG_ADD("[Error] m_pMinimapTexture is NULL");
//		
//		return;
//	}
//
//	// texture size 搬沥
//	int spWidth, spHeight;
//	int width, height;
//	BYTE smallShift = 0;
//
//	spWidth = m_pMinimapSPR->GetWidth();
//	spHeight = m_pMinimapSPR->GetHeight();
//	width = spWidth;
//	height = spHeight;
//	CDirect3D::GetTextureSize(width, height);		
//
//	//-----------------------------------------------------------
//	//
//	// 檬 鞘混 歹萍 抛胶飘 内靛.. 澜衬... - -;;
//	//
//	//-----------------------------------------------------------
//	//
//	// m_pMinimapTexture俊 minimap阑 免仿茄促.
//	//
//	//-----------------------------------------------------------
//	//-----------------------------------------------------------
//	// 2狼 铰荐父 瘤盔窍绰 版快..
//	//-----------------------------------------------------------
//	bool bDifferentSize;	// sprite客 texture狼 农扁啊 促弗啊?
//	if (CDirect3D::IsTexturePow2())
//	{
//		bDifferentSize = true;
//
//		// width客 height啊 促弗 版快...
//		// 累篮 率俊 嘎苗辑 度鞍捞 茄促.
//		// Square肺 嘎冕促..绊 且 荐 乐瘤.. 澜窍..
//		if (width < height)
//		{
//			height = width;
//		}
//		else if (width > height)
//		{
//			width = height;
//		}
//
//		//-----------------------------------------------------------
//		// Texture农扁焊促 Sprite 农扁啊 歹 奴 版快..
//		//-----------------------------------------------------------
//		// 溜, 窍靛傀绢俊辑 Sprite农扁父怒狼 Texture甫 瘤盔窍瘤 给窍绰 版快捞促.		
//		// shift甫 捞侩秦辑 农扁甫 临牢促.
//		while (spWidth > width || spHeight > height)
//		{
//			smallShift ++;
//
//			spWidth >>= 1;
//			spHeight >>= 1;
//		}
//	}
//	//-----------------------------------------------------------
//	// 酒公繁 size唱 包拌 绝绰 版快
//	//-----------------------------------------------------------
//	else
//	{
//		if (CDirect3D::IsTextureSquareOnly())
//		{
//			// width客 height啊 促弗 版快...
//			// 奴 率俊 嘎苗辑 度鞍捞 茄促.
//			// Square肺 嘎冕促..绊 且 荐 乐瘤.. 澜窍..
//			if (width > height)
//			{
//				height = width;
//				bDifferentSize = true;
//			}
//			else if (width < height)
//			{
//				width = height;
//				bDifferentSize = true;
//			}		
//			
//			m_MinimapTextureWidth = width;
//			m_MinimapTextureHeight = height;
//		}
//		else
//		{
//			bDifferentSize = false;
//		}			
//	}		
//
//	//---------------------------------------------------
//	// TextureSurface 积己
//	//---------------------------------------------------		
//	m_pMinimapTexture->InitTextureSurface(width, height, 0, CDirect3D::GetPixelFormat4444());
//	POINT point;
//	point.x = 0;
//	point.y = 0;
//	m_pMinimapTexture->Lock();
//
////	int i;
//	
//
//	//---------------------------------------------------
//	// 八霸 磨窍绰 何盒..
//	//---------------------------------------------------
//	//if (bDifferentSize)
//	{
//		/*
//		// Sprite啊 瞒瘤窍绰 康开阑 力寇茄 何盒阑 八霸~~
//		DWORD width2 = (width - spWidth) << 1;	// *2 
//		pSurface += spWidth;
//
//		// 坷弗率 糠何盒
//		if (width2 > 0)
//		{
//			i = spHeight;		
//			do
//			{			
//				memset(pSurface, 0, width2);
//				pSurface = (WORD*)((BYTE*)pSurface + pitch);
//			} while (--i);
//		}
//
//		// 酒贰率
//		pSurface -= spWidth;
//		width2 = width << 1;
//		i = height - spHeight;
//
//		if (i>0)
//		{
//			do
//			{			
//				memset(pSurface, 0, width2);
//				pSurface = (WORD*)((BYTE*)pSurface + pitch);
//			} while (--i);
//		}
//		*/
//		
//		//---------------------------------------------------
//		// Texture Surface 檬扁拳
//		//---------------------------------------------------
//		/*// 2001.7.14 林籍贸府
//		WORD *pSurface = (WORD*)m_pMinimapTexture->GetSurfacePointer();
//				//,	*pSurfaceTemp;
//		long pitch	= m_pMinimapTexture->GetSurfacePitch();
//
//		int width2 = width << 1;
//
//		for (i=0; i<height; i++)
//		{
//			memset(pSurface, 0, width2);
//			pSurface = (WORD*)((BYTE*)pSurface + pitch);
//		}
//		*/
//		
//	}
//	
//	//---------------------------------------------------
//	// Sprite免仿
//	//---------------------------------------------------
//	// 农扁甫 绊妨秦辑..
//
//	 // 2001.7.14 林籍贸府
//	// alpha蔼篮 16 == 馆捧疙
//	BYTE alpha = 16;
//
//	if (smallShift==0)
//	{
//		m_pMinimapTexture->BltSpriteAlpha4444NotTrans(&point, m_pMinimapSPR, alpha);
//	}
//	else
//	{
//		m_pMinimapTexture->BltSpriteAlpha4444SmallNotTrans(&point, m_pMinimapSPR, alpha, smallShift);
//	}
//
//	//---------------------------------------------------------------
//	// 免仿且锭狼 农扁
//	//---------------------------------------------------------------
//	spWidth = m_pMinimapSPR->GetWidth();
//	spHeight = m_pMinimapSPR->GetHeight();
//	if (CDirect3D::IsTexturePow2())	
//	{
//		width = 1;
//		height = 1;
//
//		// width客 height绰 spWidth客 spHeight焊促 目具 茄促.
//		// square肺 嘎苗霖促.
//		while (width < spWidth || height < spHeight)
//		{
//			width <<= 1;
//			height <<= 1;
//		}
//	}
//	else
//	{
//		width = spWidth;
//		height = spHeight;
//
//		// 奴 农扁肺 嘎眠绢辑 Square肺 父电促.
//		if (CDirect3D::IsTextureSquareOnly())
//		{				
//			if (width > height)
//			{
//				height = width;				
//			}
//			else if (width < height)
//			{
//				width = height;				
//			}	
//		}
//	}
//
//	m_MinimapTextureWidth = width;
//	m_MinimapTextureHeight = height;
//	
//	m_pMinimapTexture->Unlock();
//	
//}

//----------------------------------------------------------------------
// Use HalfFrame
//----------------------------------------------------------------------
/*
void
MTopView::UseHalfFrame(bool bUse)
{
	// Addon档 绢纳 秦具登绰单 - -;
	// Effect档... - -;

	//----------------------------------------------------------
	// 捞固 Loading等 巴甸 力芭
	//----------------------------------------------------------
	m_CreatureSPK.Release();
	m_CreatureSSPK.Release();

	if (CDirect3D::IsHAL())
	{
		m_pCreatureShadowManager->Release();
		
		m_pCreatureShadowManager->Init( FILE_SSPRITE_CREATURE,
							FILE_SSPRITEINDEX_CREATURE,  
							g_pClientConfig->MAX_TEXTUREPART_CREATURESHADOW);
	}

	//----------------------------------------------------------
	// Loading 等霸 绝促绊 钎矫
	//----------------------------------------------------------
	int num = g_pCreatureSpriteTable->GetSize();

	for (int i=0; i<num; i++)
	{
		(*g_pCreatureSpriteTable)[i].bLoad = false;
	}
	
	if (bUse)
	{
		// 捞固 Loading等 巴甸 力芭		
		class ifstream file(FILE_CFRAME_CREATURE2, ios::binary);
		m_CreatureFPK.LoadFromFile(file);
		file.close();		
	}
	else
	{
		// 捞固 Loading等 巴甸 力芭
		class ifstream file(FILE_CFRAME_CREATURE, ios::binary);
		m_CreatureFPK.LoadFromFile(file);
		file.close();
	}
}
*/

//----------------------------------------------------------------------
// Load From File CreatureSPK
//----------------------------------------------------------------------
// 泅犁 Zone俊辑 免泅且 Creature俊 措茄 Sprite甸阑 Load茄促.
//----------------------------------------------------------------------
void
MTopView::LoadFromFileCreatureSPK(int spriteType)
{
	if (spriteType<0 || spriteType >= (*g_pCreatureSpriteTable).GetSize())
	{
		DEBUG_ADD_FORMAT("[Error] Wrong SpriteType=%d", spriteType);
		
		return;
	}

	//int spriteType = (*g_pCreatureTable)[n].SpriteType;

	//----------------------------------------------------------------------
	// Load登菌绰瘤 眉农秦夯促.
	//----------------------------------------------------------------------
	if ((*g_pCreatureSpriteTable)[spriteType].bLoad)
	{
		// 捞固 Load等 版快
	}
	//----------------------------------------------------------------------
	// Load秦具登绰 版快
	//----------------------------------------------------------------------
	// CreatureAction俊 蝶扼辑 g_pCreatureActionSpriteTable阑 捞侩且 巴捞骨肺
	// 角力肺 loading窍柳 臼绰促.  2001.11.21
	//----------------------------------------------------------------------			
	else
	{
		if(g_pUserOption->IsPreLoadMonster)
		{
			
			// 泅犁 Zone俊辑 鞘夸茄 Sprite甸阑 Load窍搁 等促.
//			class ifstream	creatureFile;//(FILE_ISPRITE_CREATURE, ios::binary);
//			class ifstream	creatureShadowFile;
//
//			//------------------------------------------------------------
//			// sprite load
//			//------------------------------------------------------------
//			if (!FileOpenBinary(FILE_ISPRITE_CREATURE, creatureFile))
//				return;	
			

			//------------------------------------------------------------
			// Half Frame阑 荤侩且锭
			//------------------------------------------------------------
			if (0)//g_pUserOption->UseHalfFrame)
			{
				/*
				int frameID = (*g_pCreatureSpriteTable)[spriteType].FrameID;

				//------------------------------------------------------------	
				// 鞘夸茄 EFPK甫 积己窍搁辑 荤侩等 SpriteID档 备茄促.
				//------------------------------------------------------------
				COrderedList<int> intList;
				ACTION_FRAME_ARRAY& AFA = CreatureFPK[frameID];
				int numAction = AFA.GetSize();

				// orderedList狼 瓤啦阑 臭洒扁 困秦辑 芭操肺 ..
				for (int a=AFA.GetSize()-1; a>=0; a--)
				{
					DIRECTION_FRAME_ARRAY& DFA = AFA[a];

					for (int d=DFA.GetSize()-1; d>=0; d--)
					{
						FRAME_ARRAY& EA = DFA[d];
						
						for (int f=FA.GetSize()-1; f>=0; f--)
						{
							CFrame& Frame = FA[f];

							int spriteID = Frame.GetSpriteID();

							intList.Add( spriteID );
						}
					}
				}

				COrderedList<int>::DATA_LIST::const_iterator iID = intList.GetIterator();
				int numID = intList.GetSize();
				
				CSpriteFilePositionArray SFPA;	
				SFPA.Init( numID );

				class ifstream CreaturePackIndexFile(FILE_ISPRITEINDEX_CREATURE, ios::binary);			
				
				long fp;
				for (int i=0; i<numID; i++)
				{
					int id = *iID
					
					CreaturePackIndexFile.seekg( 2 + (id<<2));	// 2 + id*4
					CreaturePackIndexFile.read((char*)&fp, 4);


					SFPA[id].SpriteID		= id;
					SFPA[id].FilePosition	= fp;
				}			

				m_CreatureSPK.LoadFromFile(creatureFile, SFPA);

				CreaturePackIndexFile.close();
				*/
			}
			//------------------------------------------------------------
			// 傈眉 Frame 促 荤侩
			//------------------------------------------------------------
			else
			{
				
				TYPE_SPRITEID first = (*g_pCreatureSpriteTable)[spriteType].FirstSpriteID;
				TYPE_SPRITEID last	= (*g_pCreatureSpriteTable)[spriteType].LastSpriteID;
				long			fp	= (*g_pCreatureSpriteTable)[spriteType].SpriteFilePosition;

				m_CreatureSPK.LoadFromFilePart(first, last);
				// n锅掳 creature load	
//				m_CreatureSPK.LoadFromFilePart(creatureFile, 
//												fp,
//												first, 
//												last
//												);
				
			}

// 			creatureFile.close();

			//------------------------------------------------------------
			// 弊覆磊 load
			//------------------------------------------------------------
//			if (!FileOpenBinary(FILE_SSPRITE_CREATURE, creatureShadowFile))
//				return;	

		
//			if (!CDirect3D::IsHAL())
			{
				//------------------------------------------------------------
				// Half Frame 荤侩
				//------------------------------------------------------------
				if (g_pUserOption->UseHalfFrame)
				{
				}
				//------------------------------------------------------------
				// 傈眉 Frame 促 荤侩
				//------------------------------------------------------------
				else
				{
					TYPE_SPRITEID first = (*g_pCreatureSpriteTable)[spriteType].FirstShadowSpriteID;
					TYPE_SPRITEID last	= (*g_pCreatureSpriteTable)[spriteType].LastShadowSpriteID;

					m_CreatureSSPK.LoadFromFilePart(first, last);
//					m_CreatureSSPK.LoadFromFilePart(creatureShadowFile, 
//													(*g_pCreatureSpriteTable)[spriteType].SpriteShadowFilePosition,
//													first, 
//													last
//													);
				}
			}
		}	

		// Load沁促绊 钎矫茄促.
		(*g_pCreatureSpriteTable)[spriteType].bLoad = TRUE;

		//--------------------------------------------------------
		// Player弊覆老版快绰 眠啊窍瘤 臼绰促.
		//--------------------------------------------------------
		//if (spriteType!=(*g_pCreatureTable)[CREATURETYPE_SLAYER_MALE].SpriteType
		//	&& spriteType!=(*g_pCreatureTable)[CREATURETYPE_SLAYER_FEMALE].SpriteType
		//	&& spriteType!=(*g_pCreatureTable)[CREATURETYPE_VAMPIRE_MALE].SpriteType
		//	&& spriteType!=(*g_pCreatureTable)[CREATURETYPE_VAMPIRE_FEMALE].SpriteType
		//	&& spriteType!=(*g_pCreatureTable)[CREATURETYPE_WOLF].SpriteType
		//	&& spriteType!=(*g_pCreatureTable)[CREATURETYPE_BAT].SpriteType
		//	&& spriteType!=(*g_pCreatureTable)[CREATURETYPE_SLAYER_OPERATOR].SpriteType
		//	&& spriteType!=(*g_pCreatureTable)[CREATURETYPE_VAMPIRE_OPERATOR].SpriteType)
		if (!(*g_pCreatureSpriteTable)[spriteType].IsPlayerSprite())
		{
			m_listLoadedCreatureSprite.Add( spriteType );

			if ((*g_pCreatureSpriteTable)[spriteType].IsMonsterSprite())
			{
				m_listLoadedMonsterSprite.Add( spriteType );
			}
		}

		
		/*
		//------------------------------------------------------------
		// 弊覆磊 Pack积己
		//------------------------------------------------------------
		m_CreatureSSPK.InitPart(m_CreatureSPK, 
								(*g_pCreatureTable)[n].FirstSpriteID,
								(*g_pCreatureTable)[n].LastSpriteID);

		//------------------------------------------------------------
		//
		// Init 弊覆磊 TextureSurface
		//
		//------------------------------------------------------------
		if (CDirect3D::IsHAL())
		{	
			//--------------------------------------------
			// AlphaSPK阑 捞侩秦辑 TexturePack阑 积己茄促.
			//--------------------------------------------
			if (!m_CreatureTPK.InitPart( m_CreatureSSPK,
										(*g_pCreatureTable)[n].FirstSpriteID,
										(*g_pCreatureTable)[n].LastSpriteID))
			{
				MessageBox(g_hWnd, "Can't Init ShadowTexture!", NULL, MB_OK);
				return;
			}

			//------------------------------------------------------------	
			// EffectAlphaSprite甫 皋葛府俊辑 昏力茄促.
			// --> TextureSurface甫 荤侩且 巴捞骨肺.. 鞘夸啊 绝促.
			//------------------------------------------------------------		
			m_CreatureSSPK.ReleasePart((*g_pCreatureTable)[n].FirstSpriteID,
										(*g_pCreatureTable)[n].LastSpriteID);
		}
		*/
	}
}

//----------------------------------------------------------------------
// Load From File AddonSPK
//----------------------------------------------------------------------
// 泅犁 焊捞绰 Addon阑 Load茄促.
//----------------------------------------------------------------------
//void
//MTopView::LoadFromFileAddonSPK(int frame, int action)
//{
//	int frameID = frame*ACTION_MAX_SLAYER + action;
//
//	if (frameID<0 || frameID >= (*g_pAddonSpriteTable).GetSize())
//	{
//		DEBUG_ADD_FORMAT("[Error] Wrong AddonType=%d, %d", frame, action);
//		
//		return;
//	}
//
//	//int spriteType = (*g_pCreatureTable)[n].SpriteType;
//
//	//----------------------------------------------------------------------
//	// Load登菌绰瘤 眉农秦夯促.
//	//----------------------------------------------------------------------
//	if ((*g_pAddonSpriteTable)[frameID].bLoad)
//	{
//		// 捞固 Load等 版快
//	}
//	//----------------------------------------------------------------------
//	// Load秦具登绰 版快
//	//----------------------------------------------------------------------
//	else
//	{		
//		TYPE_SPRITEID first = (*g_pAddonSpriteTable)[frameID].FirstSpriteID;
//		TYPE_SPRITEID last	= (*g_pAddonSpriteTable)[frameID].LastSpriteID;
//		long			fp	= (*g_pAddonSpriteTable)[frameID].SpriteFilePosition;
//
//		DEBUG_ADD_FORMAT("[LoadAddonAction] frameID=%d, action=%d, spr=%d~%d", frameID, action, first, last);
//
//		__BEGIN_PROFILE("LoadAddonAction")
//
//		m_AddonSPK.LoadFromFilePart(first, last);
////		m_AddonSPK.LoadFromFilePart(m_AddonSPKFile, 
////										fp,
////										first, 
////										last
////										);
//
//		__END_PROFILE("LoadAddonAction")
//
//		if (!CDirect3D::IsHAL())
//		{			
//			TYPE_SPRITEID first = (*g_pAddonSpriteTable)[frameID].FirstShadowSpriteID;
//			TYPE_SPRITEID last	= (*g_pAddonSpriteTable)[frameID].LastShadowSpriteID;
//			long			fp	= (*g_pAddonSpriteTable)[frameID].SpriteShadowFilePosition;
//
//			DEBUG_ADD_FORMAT("[LoadAddonActionShadow] spr=%d~%d", frameID, action, first, last);
//
//			__BEGIN_PROFILE("LoadAddonActionShadow")
//
////			m_AddonSSPK.LoadFromFilePart(m_AddonSSPKFile, 
////											fp,
////											first, 
////											last
////											);			
//			m_AddonSSPK.LoadFromFilePart(first, last);
//
//			__END_PROFILE("LoadAddonActionShadow")
//		}
//
//		(*g_pAddonSpriteTable)[frameID].bLoad = TRUE;
//	}
//}

//----------------------------------------------------------------------
// Load From File CreatureActionSPK
//----------------------------------------------------------------------
// 泅犁 焊捞绰 CreatureAction阑 Load茄促.
//----------------------------------------------------------------------
//void
//MTopView::LoadFromFileCreatureActionSPK(int frame, int action)
//{
//	int frameID = frame*ACTION_MAX_VAMPIRE + action;
//
//	if (frameID<0 || frameID >= (*g_pCreatureActionSpriteTable).GetSize())
//	{
//		DEBUG_ADD_FORMAT("[Error] Wrong CreatureActionType=%d, %d", frame, action);
//		
//		return;
//	}
//
//	//int spriteType = (*g_pCreatureTable)[n].SpriteType;
//
//	//----------------------------------------------------------------------
//	// Load登菌绰瘤 眉农秦夯促.
//	//----------------------------------------------------------------------
//	if ((*g_pCreatureActionSpriteTable)[frameID].bLoad)
//	{
//		// 捞固 Load等 版快
//	}
//	//----------------------------------------------------------------------
//	// Load秦具登绰 版快
//	//----------------------------------------------------------------------
//	else
//	{		
//		TYPE_SPRITEID first = (*g_pCreatureActionSpriteTable)[frameID].FirstSpriteID;
//		TYPE_SPRITEID last	= (*g_pCreatureActionSpriteTable)[frameID].LastSpriteID;
//		long			fp	= (*g_pCreatureActionSpriteTable)[frameID].SpriteFilePosition;
//
//		DEBUG_ADD_FORMAT("[LoadCreatureAction] frameID=%d, action=%d, spr=%d~%d", frameID, action, first, last);
//
//		__BEGIN_PROFILE("LoadCreatureAction")
//
////		m_CreatureSPK.LoadFromFilePart(m_CreatureSPKFile, 
////										fp,
////										first, 
////										last
////										);
//		m_CreatureSPK.LoadFromFilePart(first, last);
//
//		__END_PROFILE("LoadCreatureAction")
//
//		if (!CDirect3D::IsHAL())
//		{			
//			TYPE_SPRITEID first = (*g_pCreatureActionSpriteTable)[frameID].FirstShadowSpriteID;
//			TYPE_SPRITEID last	= (*g_pCreatureActionSpriteTable)[frameID].LastShadowSpriteID;
//			long			fp	= (*g_pCreatureActionSpriteTable)[frameID].SpriteShadowFilePosition;
//
//			DEBUG_ADD_FORMAT("[LoadCreatureActionShadow] spr=%d~%d", first, last);
//
//			__BEGIN_PROFILE("LoadCreatureActionShadow")
//
////			m_CreatureSSPK.LoadFromFilePart(m_CreatureSSPKFile, 
////											fp,
////											first, 
////											last
////											);		
//			m_CreatureSSPK.LoadFromFilePart(first, last);
//			
//			__END_PROFILE("LoadCreatureActionShadow")
//		}
//
//		(*g_pCreatureActionSpriteTable)[frameID].bLoad = TRUE;
//
//		// frameID --> spriteType
//		//int spriteType = frameID + 2;	// mapping table阑 父甸绢具 窍绰单.
//										// slayer巢赤甫 力寇窍搁 鉴辑措肺促..
//
//		// loading灯促绊 钎矫秦抵具 粮 捞悼 饶 release且 荐 乐促.
//		// map栏肺 秦具茄促.
//		//m_listLoadedCreatureSprite.push_back( spriteType );
//	}
//}
//
////----------------------------------------------------------------------
//// Release CreatureActionSPK
////----------------------------------------------------------------------
//void
//MTopView::ReleaseCreatureActionSPK(int frame, int action)
//{
//	int frameID = frame*ACTION_MAX_VAMPIRE + action;
//
//	if (frameID<0 || frameID >= (*g_pCreatureActionSpriteTable).GetSize())
//	{
//		DEBUG_ADD_FORMAT("[Error] Wrong CreatureActionType=%d, %d", frame, action);
//		
//		return;
//	}
//
//	//int spriteType = (*g_pCreatureTable)[n].SpriteType;
//
//	//----------------------------------------------------------------------
//	// Load登菌绰瘤 眉农秦夯促.
//	//----------------------------------------------------------------------
//	if ((*g_pCreatureActionSpriteTable)[frameID].bLoad)
//	{
//		TYPE_SPRITEID first = (*g_pCreatureActionSpriteTable)[frameID].FirstSpriteID;
//		TYPE_SPRITEID last	= (*g_pCreatureActionSpriteTable)[frameID].LastSpriteID;
//		
//		m_CreatureSPK.ReleasePart(first, last);
//
//		if (!CDirect3D::IsHAL())
//		{			
//			TYPE_SPRITEID first = (*g_pCreatureActionSpriteTable)[frameID].FirstShadowSpriteID;
//			TYPE_SPRITEID last	= (*g_pCreatureActionSpriteTable)[frameID].LastShadowSpriteID;
//			
// 			m_CreatureSSPK.ReleasePart(first, last);			
//		}
//
//		(*g_pCreatureActionSpriteTable)[frameID].bLoad = FALSE;
//	}
//}

//----------------------------------------------------------------------
// Release From CreatureSPK
//----------------------------------------------------------------------
// 泅犁 Load等 Creature Sprite甫 皋葛府 秦力矫挪促.
//----------------------------------------------------------------------
void
MTopView::ReleaseCreatureSPK(int n)
{
//	//--------------------------------------------------------
//	// Player弊覆老版快绰 力芭窍瘤 臼绰促.
//	//--------------------------------------------------------
//	//if (n==CREATURETYPE_SLAYER_MALE
//	//	|| n==CREATURETYPE_SLAYER_FEMALE
//	//	|| n==CREATURETYPE_VAMPIRE_MALE
//	//	|| n==CREATURETYPE_VAMPIRE_FEMALE
//	//	|| n==CREATURETYPE_WOLF
//	//	|| n==CREATURETYPE_BAT
//	//	|| n==CREATURETYPE_SLAYER_OPERATOR
//	//	|| n==CREATURETYPE_VAMPIRE_OPERATOR)
//	//{
//	//	return;
//	//{
//
//	int spriteType = (*g_pCreatureTable)[n].SpriteType;
//
//	// Player弊覆老版快绰 力芭窍瘤 臼绰促.
//	if ((*g_pCreatureSpriteTable)[spriteType].IsPlayerSprite())
//	{
//		return;
//	}
//
//	//----------------------------------------------------------------------
//	// Load登菌绰瘤 眉农秦夯促.
//	//----------------------------------------------------------------------
//	if ((*g_pCreatureSpriteTable)[spriteType].bLoad)
//	{
//		/*
//		// 捞固 Load等 版快
//		// n锅掳 creature狼 sprite甫 皋葛府 秦力
//		m_CreatureSPK.ReleasePart((*g_pCreatureSpriteTable)[spriteType].FirstSpriteID,
//									(*g_pCreatureSpriteTable)[spriteType].LastSpriteID);
//
//		m_CreatureSSPK.ReleasePart((*g_pCreatureSpriteTable)[spriteType].FirstSpriteID,
//									(*g_pCreatureSpriteTable)[spriteType].LastSpriteID);
//
//		*/
////		int frameID = (*g_pCreatureSpriteTable)[spriteType].FrameID;
////
////		if (frameID!=FRAMEID_NULL)
////		{
////			for (int a=0; a<ACTION_MAX_VAMPIRE; a++)
////			{
////				ReleaseCreatureActionSPK( frameID, a );
////			}
//			TYPE_SPRITEID first = (*g_pCreatureSpriteTable)[spriteType].FirstSpriteID;
//			TYPE_SPRITEID last	= (*g_pCreatureSpriteTable)[spriteType].LastSpriteID;
//			long			fp	= (*g_pCreatureSpriteTable)[spriteType].SpriteFilePosition;
//			
//			m_CreatureSPK.ReleasePart(first, last);
//// 		}
//
//		// load窍瘤 臼疽促绊 钎矫茄促.
//		(*g_pCreatureSpriteTable)[spriteType].bLoad = FALSE;
//
//		m_listLoadedCreatureSprite.Remove( spriteType );
//
//		if ((*g_pCreatureSpriteTable)[spriteType].IsMonsterSprite())
//		{
//			m_listLoadedMonsterSprite.Remove( spriteType );
//		}
//	}
//	else
//	{
//		// Load登瘤 臼篮 版快
//	}
	//--------------------------------------------------------
	// Player弊覆老版快绰 力芭窍瘤 臼绰促.
	//--------------------------------------------------------
	//if (n==CREATURETYPE_SLAYER_MALE
	//	|| n==CREATURETYPE_SLAYER_FEMALE
	//	|| n==CREATURETYPE_VAMPIRE_MALE
	//	|| n==CREATURETYPE_VAMPIRE_FEMALE
	//	|| n==CREATURETYPE_WOLF
	//	|| n==CREATURETYPE_BAT
	//	|| n==CREATURETYPE_SLAYER_OPERATOR
	//	|| n==CREATURETYPE_VAMPIRE_OPERATOR)
	//{
	//	return;
	//{

	//int spriteType = (*g_pCreatureTable)[n].SpriteTypes[0];
	int spriteTypeCount = (*g_pCreatureTable)[n].SpriteTypes.GetSize();
	
	for(int FrameIndex = 0;FrameIndex < spriteTypeCount; FrameIndex++ )
	{
		
		int spriteType = (*g_pCreatureTable)[n].SpriteTypes[FrameIndex];
		// Player弊覆老版快绰 力芭窍瘤 臼绰促.
		if ((*g_pCreatureSpriteTable)[spriteType].IsPlayerSprite())
		{
			return;
		}
		
		//----------------------------------------------------------------------
		// Load登菌绰瘤 眉农秦夯促.
		//----------------------------------------------------------------------
		if ((*g_pCreatureSpriteTable)[spriteType].bLoad)
		{
		/*
		// 捞固 Load等 版快
		// n锅掳 creature狼 sprite甫 皋葛府 秦力
		m_CreatureSPK.ReleasePart((*g_pCreatureSpriteTable)[spriteType].FirstSpriteID,
		(*g_pCreatureSpriteTable)[spriteType].LastSpriteID);
		
		  m_CreatureSSPK.ReleasePart((*g_pCreatureSpriteTable)[spriteType].FirstSpriteID,
		  (*g_pCreatureSpriteTable)[spriteType].LastSpriteID);
		  
			*/
			//		int frameID = (*g_pCreatureSpriteTable)[spriteType].FrameID;
			//
			//		if (frameID!=FRAMEID_NULL)
			//		{
			//			for (int a=0; a<ACTION_MAX_VAMPIRE; a++)
			//			{
			//				ReleaseCreatureActionSPK( frameID, a );
			//			}
			TYPE_SPRITEID first = (*g_pCreatureSpriteTable)[spriteType].FirstSpriteID;
			TYPE_SPRITEID last	= (*g_pCreatureSpriteTable)[spriteType].LastSpriteID;
			long			fp	= (*g_pCreatureSpriteTable)[spriteType].SpriteFilePosition;
			
			m_CreatureSPK.ReleasePart(first, last);
			// 		}
			
			// load窍瘤 臼疽促绊 钎矫茄促.
			(*g_pCreatureSpriteTable)[spriteType].bLoad = FALSE;
			
			m_listLoadedCreatureSprite.Remove( spriteType );
			
			if ((*g_pCreatureSpriteTable)[spriteType].IsMonsterSprite())
			{
				m_listLoadedMonsterSprite.Remove( spriteType );
			}
		}
		else
		{
			// Load登瘤 臼篮 版快
		}
		



	}
}

//----------------------------------------------------------------------
// Add MonsterSpriteTypes
//----------------------------------------------------------------------
// Hallu俊辑 曼炼窍霸 瞪 巴捞寸...
//----------------------------------------------------------------------
void		
MTopView::AddMonsterSpriteTypes(const INT_ORDERED_LIST& listUse)
{		
	INT_ORDERED_LIST::DATA_LIST::const_iterator iUse = listUse.GetIterator();

	for (int i=0; i<listUse.GetSize(); i++)
	{
		int useSpriteType = *iUse;

		if ((*g_pCreatureSpriteTable)[useSpriteType].IsMonsterSprite())
		{
			m_listLoadedMonsterSprite.Add( useSpriteType );
		}

		iUse++;
	}
}

//----------------------------------------------------------------------
// Release Useless CreatureSPK Except
//----------------------------------------------------------------------
// list绰 荤侩窍绰 creature甸狼 sprite type 捞促.
//----------------------------------------------------------------------
void
MTopView::ReleaseUselessCreatureSPKExcept(const INT_ORDERED_LIST& listUse)
{
	// 瘤陛篮 各+NPC 胶橇扼捞飘啊 倔付 绝栏骨肺 release窍瘤 臼绰促.
	//return;

	//--------------------------------------------------------
	// load等 巴甸 吝俊辑 荤侩吝牢芭绰 哗绊..
	//--------------------------------------------------------
	m_listLoadedCreatureSprite -= listUse;

	//--------------------------------------------------------
	// 唱赣瘤绰 力芭茄促.
	//--------------------------------------------------------
	INT_ORDERED_LIST::DATA_LIST::const_iterator iSpriteType = m_listLoadedCreatureSprite.GetIterator();

	for (int i=0; i<m_listLoadedCreatureSprite.GetSize(); i++)
	{
		int spriteType = *iSpriteType;
		
		//--------------------------------------------------------
		// Player弊覆老版快绰 力芭窍瘤 臼绰促.
		//--------------------------------------------------------
		//if (spriteType==(*g_pCreatureTable)[CREATURETYPE_SLAYER_MALE].SpriteType
		//	|| spriteType==(*g_pCreatureTable)[CREATURETYPE_SLAYER_FEMALE].SpriteType
		//	|| spriteType==(*g_pCreatureTable)[CREATURETYPE_VAMPIRE_MALE].SpriteType
		//	|| spriteType==(*g_pCreatureTable)[CREATURETYPE_VAMPIRE_FEMALE].SpriteType
		//	|| spriteType==(*g_pCreatureTable)[CREATURETYPE_WOLF].SpriteType
		//	|| spriteType==(*g_pCreatureTable)[CREATURETYPE_BAT].SpriteType
		//	|| spriteType==(*g_pCreatureTable)[CREATURETYPE_SLAYER_OPERATOR].SpriteType
		//	|| spriteType==(*g_pCreatureTable)[CREATURETYPE_VAMPIRE_OPERATOR].SpriteType)
		if ((*g_pCreatureSpriteTable)[spriteType].IsPlayerSprite())
		{
			DEBUG_ADD_FORMAT("[ReleaseUselessCreatureSPK] Skip spriteType=%d", spriteType);
			
			iSpriteType++;
			continue;
		}

		DEBUG_ADD_FORMAT("[ReleaseUselessCreatureSPK] Release spriteType=%d", spriteType);
		

		if ((*g_pCreatureSpriteTable)[spriteType].bLoad)
		{
			/*
			int first, last;
			// 捞固 Load等 版快
			// n锅掳 creature狼 sprite甫 皋葛府 秦力

			//--------------------------------------------------------
			// Creature SPK 力芭
			//--------------------------------------------------------
			first = (*g_pCreatureSpriteTable)[spriteType].FirstSpriteID;
			last = (*g_pCreatureSpriteTable)[spriteType].LastSpriteID;

			if (last > first && first!=SPRITEID_NULL && last!=SPRITEID_NULL)
			{
				m_CreatureSPK.ReleasePart(first, last);
			}

			//--------------------------------------------------------
			// Creature SSPK 力芭
			//--------------------------------------------------------
			first = (*g_pCreatureSpriteTable)[spriteType].FirstSpriteID;
			last = (*g_pCreatureSpriteTable)[spriteType].LastSpriteID;
			
			if (last > first && first!=SPRITEID_NULL && last!=SPRITEID_NULL)
			{			
				m_CreatureSSPK.ReleasePart( first, last );
			}
			*/
			// loading茄 action父 力芭
			// 2001.11.21
			int frameID = (*g_pCreatureSpriteTable)[spriteType].FrameID;

			if (frameID!=FRAMEID_NULL)
			{
				ReleaseCreatureSPK( frameID );
			}

			// load窍瘤 臼疽促绊 钎矫茄促.
			(*g_pCreatureSpriteTable)[spriteType].bLoad = FALSE;
		}

		iSpriteType++;
	}

	//--------------------------------------------------------
	// load等 巴 促矫 汲沥
	//--------------------------------------------------------	
	m_listLoadedCreatureSprite.Release();
	m_listLoadedMonsterSprite.Release();
	
	INT_ORDERED_LIST::DATA_LIST::const_iterator iUse = listUse.GetIterator();

	for (i=0; i<listUse.GetSize(); i++)
	{
		int useSpriteType = *iUse;

		m_listLoadedCreatureSprite.Add( useSpriteType );

		if ((*g_pCreatureSpriteTable)[useSpriteType].IsMonsterSprite())
		{
			m_listLoadedMonsterSprite.Add( useSpriteType );
		}

		iUse++;
	}
}

//----------------------------------------------------------------------
// Load From File  Tile & ImageObject Set
//----------------------------------------------------------------------
// 老何狼 Tile苞 ImageObject甫 Load茄促.
//----------------------------------------------------------------------
bool		
MTopView::LoadFromFileTileAndImageObjectSet(const CSpriteSetManager &TileSSM, const CSpriteSetManager &ImageObjectSSM)
{
	//--------------------------------------------------------
	//
	// Tile 老何 Load
	//
	//--------------------------------------------------------
//	class ifstream	TileSPKFile;//(FILE_SPRITE_TILE, ios::binary);	
//	if (!FileOpenBinary(FILE_SPRITE_TILE, TileSPKFile))
//		return false;
//	bool bLoad = m_TileSPK.LoadFromFilePart(TileSPKFile, m_TileSPKI, TileSSM);
//	TileSPKFile.close();
//
//	if (!bLoad)
//		return false;
//
	bool bLoad = m_TileSPK.LoadFromFilePart(TileSSM);
	
	//--------------------------------------------------------
	//
	// ImageObject 老何 Load
	//
	//--------------------------------------------------------
//	class ifstream	ImageObjectSPKFile;//(FILE_SPRITE_IMAGEOBJECT, ios::binary);	
//	if (!FileOpenBinary(FILE_SPRITE_IMAGEOBJECT, ImageObjectSPKFile))
//		return false;
//	bLoad = m_ImageObjectSPK.LoadFromFilePart(ImageObjectSPKFile, m_ImageObjectSPKI, ImageObjectSSM);
//	ImageObjectSPKFile.close();	
	m_ImageObjectSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SPRITE_IMAGEOBJECT").c_str());

	return bLoad;
}

//----------------------------------------------------------------------
// Load From File Tile LargeZone
//----------------------------------------------------------------------
// File俊辑 LargeZone俊辑父 荤侩且 TileSprite阑 Load茄促.
//----------------------------------------------------------------------
bool
MTopView::LoadFromFileTileSPKLargeZone(class ifstream & file)
{
	//------------------------------------------------------------
	// File俊辑 TileSprite俊 措茄 沥焊甫 Load茄促.
	//------------------------------------------------------------
	CSpriteFilePositionArray* pOldTileSFPA = m_pTileSFPArrayLargeZone;

	m_pTileSFPArrayLargeZone = new CSpriteFilePositionArray;
	
	m_pTileSFPArrayLargeZone->LoadFromFile( file );

	//------------------------------------------------------------
	// 鞘夸 绝绰芭 力芭
	//------------------------------------------------------------
	if (pOldTileSFPA!=NULL)
	{
		DEBUG_ADD( "Remove Old-Tile SFPArray LargeZone");
		
		COrderedList<TYPE_SPRITEID>	oldTileID;
		COrderedList<TYPE_SPRITEID>	newTileID;

		register int i;

		DEBUG_ADD( "Sort Part1");
		
		// Sort
		for (i=pOldTileSFPA->GetSize()-1; i>=0; i--)
		{
			oldTileID.Add( (*pOldTileSFPA)[i].SpriteID );
		}

		DEBUG_ADD( "Sort Part2");
		
		// Sort
		for (i=m_pTileSFPArrayLargeZone->GetSize()-1; i>=0; i--)
		{
			newTileID.Add( (*m_pTileSFPArrayLargeZone)[i].SpriteID );
		}

		DEBUG_ADD( "Subtract");
		
		// oldTileID俊辑 newTileID 力芭茄促.
		oldTileID -= newTileID;
		
		DEBUG_ADD( "Release Part");
		
		// 抗傈狼 zone俊父 粮犁窍绰 TileID甸阑 力芭茄促.
		m_TileSPK.ReleasePart( oldTileID );

		DEBUG_ADD( "Delete pOldTilSFPA");
		
		delete pOldTileSFPA;
		pOldTileSFPA = NULL;
	}
	
	DEBUG_ADD_FORMAT( "Load Tile SFPArray LargeZone : size=%d", m_pTileSFPArrayLargeZone->GetSize() );
	
//#ifdef OUTPUT_DEBUG
	//for (int i=0; i<m_pTileSFPArrayLargeZone->GetSize(); i++)
	//{
	//	DEBUG_ADD_FORMAT("SFP[%d] %ld", m_pTileSFPArrayLargeZone[i].SpriteID, m_pTileSFPArrayLargeZone[i].FilePosition);
	//}
//#endif
	//------------------------------------------------------------
	//  Load Tile SpriteSet
	//------------------------------------------------------------
	/*
	class ifstream	TilePackFile;//(FILE_SPRITE_TILE, ios::binary);	
	if (!FileOpenBinary(FILE_SPRITE_TILE, TilePackFile))
		return false;
	
	bool bLoad = m_TileSPK.LoadFromFilePart(TilePackFile, m_pTileSFPArrayLargeZone);

	TilePackFile.close();

	return bLoad;
	*/

	/*
	// 2001.8.20 林籍贸府
	CSpriteFilePositionArray* pSFPA = new CSpriteFilePositionArray;
	*pSFPA = *m_pTileSFPArrayLargeZone;

	MLoadingSPKWorkNode2* pNode = new MLoadingSPKWorkNode2( pSFPA );
	pNode->SetSPK( &m_TileSPK, FILE_SPRITE_TILE );
	pNode->SetType( 1 );	// zone ID

	DEBUG_ADD_FORMAT("[Thread]Before AddLast: size=%d, working=%d, finish=%d, stop=%d", 
											g_pLoadingThread->GetSize(),
											(int)g_pLoadingThread->IsWorking(),
											(int)g_pLoadingThread->IsFinishCurrentWork(),
											(int)g_pLoadingThread->IsStopWork());
	
	g_pLoadingThread->AddLast( pNode );

	DEBUG_ADD("[Thread] Add Last OK");
	*/

	return true;
}

//----------------------------------------------------------------------
// Load From File ImageObject LargeZone
//----------------------------------------------------------------------
// File俊辑 LargeZone俊辑父 荤侩且 ImageObjectSprite甸阑 Load茄促.
//----------------------------------------------------------------------
bool
MTopView::LoadFromFileImageObjectSPKLargeZone(class ifstream & file)
{
	//------------------------------------------------------------
	// File俊辑 ImageObjectSprite俊 措茄 沥焊甫 Load茄促.
	//------------------------------------------------------------
	CSpriteFilePositionArray* pOldImageObjectSFPA = m_pImageObjectSFPArrayLargeZone;

	m_pImageObjectSFPArrayLargeZone = new CSpriteFilePositionArray;
	
	m_pImageObjectSFPArrayLargeZone->LoadFromFile( file );

	//------------------------------------------------------------
	// 鞘夸 绝绰芭 力芭
	//------------------------------------------------------------
	if (pOldImageObjectSFPA!=NULL)
	{
		DEBUG_ADD( "Remove Old-ImageObject SFPArray LargeZone");
		
		COrderedList<TYPE_SPRITEID>	oldImageObjectID;
		COrderedList<TYPE_SPRITEID>	newImageObjectID;

		register int i;

		// Sort
		for (i=pOldImageObjectSFPA->GetSize()-1; i>=0; i--)
		{
			oldImageObjectID.Add( (*pOldImageObjectSFPA)[i].SpriteID );

			//DEBUG_ADD_FORMAT( "Old: %d", (*pOldImageObjectSFPA)[i].SpriteID );			
		}

		// Sort
		for (i=m_pImageObjectSFPArrayLargeZone->GetSize()-1; i>=0; i--)
		{
			newImageObjectID.Add( (*m_pImageObjectSFPArrayLargeZone)[i].SpriteID );

			//	DEBUG_ADD_FORMAT( "New: %d", (*m_pImageObjectSFPArrayLargeZone)[i].SpriteID );
			
		}

		// oldImageObjectID俊辑 newImageObjectID 力芭茄促.
		oldImageObjectID -= newImageObjectID;
		
		/*
		#ifdef OUTPUT_DEBUG
			COrderedList<TYPE_SPRITEID>::DATA_LIST::const_iterator iData = oldImageObjectID.GetIterator();

			for (i=0; i<oldImageObjectID.GetSize(); i++)
			{
				TYPE_SPRITEID spriteID = *iData;

				DEBUG_ADD_FORMAT( "Release: %d", spriteID );

				iData++;				
			}							
		#endif
		*/

		// 抗傈狼 zone俊父 粮犁窍绰 ImageObjectID甸阑 力芭茄促.
		m_ImageObjectSPK.ReleasePart( oldImageObjectID );

		delete pOldImageObjectSFPA;
		pOldImageObjectSFPA = NULL;
	}

	DEBUG_ADD_FORMAT( "Load ImageObject SFPArray LargeZone : size=%d", m_pImageObjectSFPArrayLargeZone->GetSize() );
	
	//------------------------------------------------------------
	//  Load ImageObject SpriteSet
	//------------------------------------------------------------
	/*
	class ifstream	ImageObjectPackFile;//(FILE_SPRITE_IMAGEOBJECT, ios::binary);	
	if (!FileOpenBinary(FILE_SPRITE_IMAGEOBJECT, ImageObjectPackFile))
		return false;
	
	bool bLoadOK = m_ImageObjectSPK.LoadFromFilePart(ImageObjectPackFile, m_pImageObjectSFPArrayLargeZone);

	ImageObjectPackFile.close();

	return bLoadOK;
	*/

	/*
	// 2001.8.20 林籍贸府
	// 货肺款 SFPA 积己.
	CSpriteFilePositionArray* pSFPA = new CSpriteFilePositionArray;
	*pSFPA = *m_pImageObjectSFPArrayLargeZone;

	MLoadingSPKWorkNode2* pNode = new MLoadingSPKWorkNode2( pSFPA );
	pNode->SetSPK( &m_ImageObjectSPK, FILE_SPRITE_IMAGEOBJECT );
	pNode->SetType( 1 );	// zone ID

	DEBUG_ADD_FORMAT("[Thread]Before AddLast: size=%d, working=%d, finish=%d, stop=%d", 
											g_pLoadingThread->GetSize(),
											(int)g_pLoadingThread->IsWorking(),
											(int)g_pLoadingThread->IsFinishCurrentWork(),
											(int)g_pLoadingThread->IsStopWork());
	
	g_pLoadingThread->AddLast( pNode );

	DEBUG_ADD("[Thread] Add Last OK");
	*/
	return true;
}

//----------------------------------------------------------------------
// Load From File Tile SmallZone
//----------------------------------------------------------------------
// File俊辑 SmallZone俊辑父 荤侩且 TileSprite阑 Load茄促.
//----------------------------------------------------------------------
bool
MTopView::LoadFromFileTileSPKSmallZone(class ifstream & file)
{
	//------------------------------------------------------------
	// File俊辑 TileSprite俊 措茄 沥焊甫 Load茄促.
	//------------------------------------------------------------
	CSpriteFilePositionArray* pOldTileSFPA = m_pTileSFPArraySmallZone;

	m_pTileSFPArraySmallZone = new CSpriteFilePositionArray;
	
	m_pTileSFPArraySmallZone->LoadFromFile( file );

	//------------------------------------------------------------
	// 鞘夸 绝绰芭 力芭
	//------------------------------------------------------------
	if (pOldTileSFPA!=NULL)
	{
		DEBUG_ADD( "Remove Old-Tile SFPArray SmallZone");
		
		COrderedList<TYPE_SPRITEID>	oldTileID;
		COrderedList<TYPE_SPRITEID>	newTileID;

		register int i;

		DEBUG_ADD( "Sort Part1");
		
		// Sort
		for (i=pOldTileSFPA->GetSize()-1; i>=0; i--)
		{
			oldTileID.Add( (*pOldTileSFPA)[i].SpriteID );
		}

		DEBUG_ADD( "Sort Part2");
		
		// Sort
		for (i=m_pTileSFPArraySmallZone->GetSize()-1; i>=0; i--)
		{
			newTileID.Add( (*m_pTileSFPArraySmallZone)[i].SpriteID );
		}

		DEBUG_ADD( "Subtract");
		
		// oldTileID俊辑 newTileID 力芭茄促.
		oldTileID -= newTileID;
		
		DEBUG_ADD( "Release");
		
		// 抗傈狼 zone俊父 粮犁窍绰 TileID甸阑 力芭茄促.
		m_TileSPK.ReleasePart( oldTileID );

		DEBUG_ADD( "Delete pOldTilSFPA");
		
		delete pOldTileSFPA;
		pOldTileSFPA = NULL;
	}
		

	DEBUG_ADD_FORMAT( "Load Tile SFPArray SmallZone : size=%d", m_pTileSFPArraySmallZone->GetSize() );
	
	//------------------------------------------------------------
	//  Load Tile SpriteSet
	//------------------------------------------------------------
	/*
	class ifstream	TilePackFile;//(FILE_SPRITE_TILE, ios::binary);	
	if (!FileOpenBinary(FILE_SPRITE_TILE, TilePackFile))
		return false;

	bool bLoad = m_TileSPK.LoadFromFilePart(TilePackFile, m_pTileSFPArraySmallZone);

	TilePackFile.close();

	return bLoad;
	*/

	/*
	// 2001.8.20 林籍贸府
	CSpriteFilePositionArray* pSFPA = new CSpriteFilePositionArray;
	*pSFPA = *m_pTileSFPArraySmallZone;

	MLoadingSPKWorkNode2* pNode = new MLoadingSPKWorkNode2( pSFPA );
	pNode->SetSPK( &m_TileSPK, FILE_SPRITE_TILE );
	pNode->SetType( 1 );	// zone ID
	
	DEBUG_ADD_FORMAT("[Thread]Before AddLast: size=%d, working=%d, finish=%d, stop=%d", 
											g_pLoadingThread->GetSize(),
											(int)g_pLoadingThread->IsWorking(),
											(int)g_pLoadingThread->IsFinishCurrentWork(),
											(int)g_pLoadingThread->IsStopWork());
	
	g_pLoadingThread->AddLast( pNode );

	DEBUG_ADD("[Thread] Add Last OK");
	*/

	return true;
}

//----------------------------------------------------------------------
// Load From File ImageObject SmallZone
//----------------------------------------------------------------------
// File俊辑 SmallZone俊辑父 荤侩且 ImageObjectSprite甸阑 Load茄促.
//----------------------------------------------------------------------
bool
MTopView::LoadFromFileImageObjectSPKSmallZone(class ifstream & file)
{
	//------------------------------------------------------------
	// File俊辑 ImageObjectSprite俊 措茄 沥焊甫 Load茄促.
	//------------------------------------------------------------
	CSpriteFilePositionArray* pOldImageObjectSFPA = m_pImageObjectSFPArraySmallZone;

	m_pImageObjectSFPArraySmallZone = new CSpriteFilePositionArray;
	
	m_pImageObjectSFPArraySmallZone->LoadFromFile( file );

	//------------------------------------------------------------
	// 鞘夸 绝绰芭 力芭
	//------------------------------------------------------------
	if (pOldImageObjectSFPA!=NULL)
	{
		DEBUG_ADD( "Remove Old-ImageObject SFPArray SmallZone");
		
		COrderedList<TYPE_SPRITEID>	oldImageObjectID;
		COrderedList<TYPE_SPRITEID>	newImageObjectID;

		register int i;

		// Sort
		for (i=pOldImageObjectSFPA->GetSize()-1; i>=0; i--)
		{
			oldImageObjectID.Add( (*pOldImageObjectSFPA)[i].SpriteID );
		}

		// Sort
		for (i=m_pImageObjectSFPArraySmallZone->GetSize()-1; i>=0; i--)
		{
			newImageObjectID.Add( (*m_pImageObjectSFPArraySmallZone)[i].SpriteID );
		}

		// oldImageObjectID俊辑 newImageObjectID 力芭茄促.
		oldImageObjectID -= newImageObjectID;
		
		// 抗傈狼 zone俊父 粮犁窍绰 ImageObjectID甸阑 力芭茄促.
		m_ImageObjectSPK.ReleasePart( oldImageObjectID );

		delete pOldImageObjectSFPA;
		pOldImageObjectSFPA = NULL;
	}
	

	DEBUG_ADD_FORMAT( "Load ImageObject SFPArray SmallZone : size=%d", m_pImageObjectSFPArraySmallZone->GetSize() );
	
	//------------------------------------------------------------
	//  Load ImageObject SpriteSet
	//------------------------------------------------------------
	/*
	class ifstream	ImageObjectPackFile;//(FILE_SPRITE_IMAGEOBJECT, ios::binary);	
	if (!FileOpenBinary(FILE_SPRITE_IMAGEOBJECT, ImageObjectPackFile))
		return false;
	
	bool bLoadOK = m_ImageObjectSPK.LoadFromFilePart(ImageObjectPackFile, m_pImageObjectSFPArraySmallZone);

	ImageObjectPackFile.close();
	
	return bLoadOK;
	*/
	/*
	// 2001.8.20 林籍贸府
	CSpriteFilePositionArray* pSFPA = new CSpriteFilePositionArray;
	*pSFPA = *m_pImageObjectSFPArraySmallZone;

	MLoadingSPKWorkNode2* pNode = new MLoadingSPKWorkNode2( pSFPA );
	pNode->SetSPK( &m_ImageObjectSPK, FILE_SPRITE_IMAGEOBJECT );
	pNode->SetType( 1 );	// zone ID
	g_pLoadingThread->AddLast( pNode );
	*/
	return true;
}

//----------------------------------------------------------------------
// Release TileSPK LargeZone
//----------------------------------------------------------------------
void	
MTopView::ReleaseTileSPKLargeZone()
{
	/*
	if (m_pTileSFPArrayLargeZone!=NULL)
	{		
		DEBUG_ADD_FORMAT( "Release TileSPK LargeZone : size=%d", m_pTileSFPArrayLargeZone->GetSize() );
		
		m_TileSPK.ReleasePart( m_pTileSFPArrayLargeZone );	

		DEBUG_ADD( "ReleasePart OK" );
		
		delete m_pTileSFPArrayLargeZone;		

		m_pTileSFPArrayLargeZone = NULL;
	}
	*/
	DEBUG_ADD( "Release TileSPK LargeZone OK" );
}

//----------------------------------------------------------------------
// Release ImageObjectSPK LargeZone
//----------------------------------------------------------------------
void	
MTopView::ReleaseImageObjectSPKLargeZone()
{
	/*
	if (m_pImageObjectSFPArrayLargeZone!=NULL)
	{
		DEBUG_ADD_FORMAT( "ReleaseImageObjectSPKLargeZone size=%d", m_pImageObjectSFPArrayLargeZone->GetSize() );
		
		m_ImageObjectSPK.ReleasePart( m_pImageObjectSFPArrayLargeZone );

		DEBUG_ADD( "ReleasePart OK" );
		
		delete m_pImageObjectSFPArrayLargeZone;

		m_pImageObjectSFPArrayLargeZone = NULL;
	}
	*/
	DEBUG_ADD( "ReleaseImageObjectSPKLargeZone OK" );
}

//----------------------------------------------------------------------
// Release TileSPK SmallZone
//----------------------------------------------------------------------
// SmallZone俊父 粮犁窍绰 Tile甸阑 力芭茄促.
//----------------------------------------------------------------------
void	
MTopView::ReleaseTileSPKSmallZone()
{
	/*
	DEBUG_ADD( "ReleaseTileSPKSmallZone" );
	
	// m_pTileSFPArraySmallZone客 m_pTileSFPArrayLargeZone甫 sort窍绊
	// SmallZone - LargeZone秦辑 巢篮 巴阑 力芭茄促.
	COrderedList<TYPE_SPRITEID>	SmallZoneTileID;
	COrderedList<TYPE_SPRITEID>	LargeZoneTileID;

	register int i;

	// Sort
	for (i=m_pTileSFPArraySmallZone->GetSize()-1; i>=0; i--)
	{
		SmallZoneTileID.Add( m_pTileSFPArraySmallZone[i].SpriteID );
	}

	// Sort
	for (i=m_pTileSFPArrayLargeZone->GetSize()-1; i>=0; i--)
	{
		LargeZoneTileID.Add( m_pTileSFPArrayLargeZone[i].SpriteID );
	}

	// SmallZoneTileID俊辑 LargeZoneTileID甫 力芭茄促.
	SmallZoneTileID -= LargeZoneTileID;
	
	// SmallZone俊父 粮犁窍绰 TileID甸阑 力芭茄促.
	m_TileSPK.ReleasePart( SmallZoneTileID );

	m_pTileSFPArraySmallZone->Release();
	*/
	DEBUG_ADD( "ReleaseTileSPKSmallZone OK" );
}

//----------------------------------------------------------------------
// Release ImageObjectSPK SmallZone
//----------------------------------------------------------------------
// SmallZone俊父 粮犁窍绰 ImageObject甸阑 力芭茄促.
//----------------------------------------------------------------------
void	
MTopView::ReleaseImageObjectSPKSmallZone()
{
	/*
	DEBUG_ADD( "ReleaseImageObjectSPKSmallZone" );
	
	// m_pImageObjectSFPArraySmallZone客 m_pImageObjectSFPArrayLargeZone甫 sort窍绊
	// SmallZone - LargeZone秦辑 巢篮 巴阑 力芭茄促.
	COrderedList<TYPE_SPRITEID>	SmallZoneImageObjectID;
	COrderedList<TYPE_SPRITEID>	LargeZoneImageObjectID;

	register int i;

	// Sort
	for (i=m_pImageObjectSFPArraySmallZone->GetSize()-1; i>=0; i--)
	{
		SmallZoneImageObjectID.Add( m_pImageObjectSFPArraySmallZone[i].SpriteID );
	}

	// Sort
	for (i=m_pImageObjectSFPArrayLargeZone->GetSize()-1; i>=0; i--)
	{
		LargeZoneImageObjectID.Add( m_pImageObjectSFPArrayLargeZone[i].SpriteID );
	}

	// SmallZoneImageObjectID俊辑 LargeZoneImageObjectID甫 力芭茄促.
	SmallZoneImageObjectID -= LargeZoneImageObjectID;
	
	// SmallZone俊父 粮犁窍绰 ImageObjectID甸阑 力芭茄促.
	m_ImageObjectSPK.ReleasePart( SmallZoneImageObjectID );

	m_pImageObjectSFPArraySmallZone->Release();
	*/
	DEBUG_ADD( "ReleaseImageObjectSPKSmallZone OK" );
}

//----------------------------------------------------------------------
// 焊咯临 Zone阑 汲沥茄促.
//----------------------------------------------------------------------
void
MTopView::SetZone(MZone* pZone)
{
	m_pZone		= pZone;
	
	// 葛滴 促矫 弊妨霖促.
	m_bFirstTileDraw = true;
	
	ClearOutputCreature();

	ClearItemNameList();

	// 谅钎 汲沥
	//----------------------------------------
	// 矫具 包访..
	//----------------------------------------
//	int x, y;
//	for (y=0; y<m_pZone->GetHeight(); y++)
//	{
//		for (x=0; x<m_pZone->GetWidth(); x++)
//		{			
//			m_PreviousFogSpriteID[y][x] = SPRITEID_NULL;
//		}
//	}
//	m_bFogChanged = true;
}

//----------------------------------------------------------------------
// Map to Pixel  :  (Xm, Ym) ---> (Xp, Yp)
//----------------------------------------------------------------------
// inline

//----------------------------------------------------------------------
// Pixel to Map  :  (Xp, Yp) ---> (Xm, Ym)
//----------------------------------------------------------------------
// inline


//----------------------------------------------------------------------
// Get Screen Point
//----------------------------------------------------------------------
// 拳搁 惑狼 谅钎 (x,y)绰 Zone俊辑狼 pixel谅钎肺绰 绢蠢 痢牢啊?
//----------------------------------------------------------------------
POINT	
MTopView::ScreenToPixel(int x, int y)
{
	// 泅犁 拳搁狼 (x,y)狼 谅钎甫 备茄促.
	POINT zonePixel;	// 拳搁惑狼 贸澜 Sector啊 唱鸥郴绰 Zone狼 pixel谅钎

	// 拳搁狼 霉 Sector啊 唱鸥郴绰 Map俊辑狼 Pixel谅钎甫 备窍绊
	// 拳搁谅钎 (x,y)甫 歹秦林搁 
	// 拳搁 惑狼 谅钎啊 唱鸥郴绰 Map俊辑狼 Pixel谅钎甫 备窍绰 巴捞促
	
	//zonePixel = MapToPixel(m_FirstSector.x, m_FirstSector.y);
	//zonePixel.x += m_PlusPoint.x  + x;
	//zonePixel.y += m_PlusPoint.y  + y;

	zonePixel.x = m_FirstZonePixel.x + x;
	zonePixel.y = m_FirstZonePixel.y + y;

	return zonePixel;
}

//----------------------------------------------------------------------
// Pixel To Screen
//----------------------------------------------------------------------
// Zone狼 pixel谅钎甫 拳搁狼 谅钎肺 官槽促.
//----------------------------------------------------------------------
POINT
MTopView::PixelToScreen(int x, int y)
{
	POINT screenPixel;

	// zone狼 pixel谅钎俊辑 
	// 拳搁狼 霉痢捞 啊府虐绰 zone狼 谅钎甫 哗搁 等促.
	screenPixel.x = x - m_FirstZonePixel.x;
	screenPixel.y = y - m_FirstZonePixel.y;

	return screenPixel;
}

//----------------------------------------------------------------------
// Map To Screen
//----------------------------------------------------------------------
// Map狼 sector谅钎甫 拳搁狼 谅钎肺 官槽促.
//----------------------------------------------------------------------
POINT
MTopView::MapToScreen(int sX, int sY)
{
	POINT screenPixel;

	// Map狼 Sector谅钎甫 Pixel谅钎肺 官槽促.
	screenPixel = MapToPixel(sX, sY);

	// Pixel谅钎甫 Screen谅钎肺 官槽促.
	screenPixel.x -= m_FirstZonePixel.x;
	screenPixel.y -= m_FirstZonePixel.y;

	return screenPixel;
}

//----------------------------------------------------------------------
// 拳搁 谅钎 (x,y)啊 啊府虐绰 困摹绰 
// Zone俊辑狼 绢恫(sX,sY) Sector老鳖?
//----------------------------------------------------------------------
POINT
MTopView::GetSelectedSector(int x, int y)
{
	POINT point;

	//-------------------------------------------------
	// 霖厚等 蔼甸
	//-------------------------------------------------
	// 拳搁 惑狼 霉 Sector : m_FirstSector
	// 谅钎焊沥蔼          : m_PlusPoint
	//-------------------------------------------------

	//-------------------------------------------------
	// 规过
	//-------------------------------------------------
	// 1. 拳搁狼 (0,0)捞 Zone俊辑 唱鸥郴绰 
	//    pixel窜困谅钎甫 舅酒辰促.(pX,pY) - MapToPixel
	//
	// 2. (pX,pY) + (x,y)甫 备茄促.
	//
	// 3. (pX,x, pY,y)啊 Zone俊辑 唱鸥郴绰
	//    Sector谅钎甫 舅酒辰促.           - PixelToMap
	//-------------------------------------------------

	point = MapToPixel(m_FirstSector.x, m_FirstSector.y);
	point.x += m_PlusPoint.x;
	point.y += m_PlusPoint.y;

	point.x += x;
	point.y += y;

	point = PixelToMap(point.x, point.y);

	if (g_pZone->IsPositionInZone( point.x, point.y ))
	{
		return point;
	}

	point.x = SECTORPOSITION_NULL;
	point.y = SECTORPOSITION_NULL;

	return point;
}

//----------------------------------------------------------------------
// Set Direction To Creature
//----------------------------------------------------------------------
// 促弗 Creature甫 氢秦辑 官扼夯促.
//----------------------------------------------------------------------
BYTE
MTopView::GetDirectionToPosition(int originX, int originY, int destX, int destY)
{
	int	stepX = destX - originX,
		stepY = destY - originY;

	// 0老 锭 check
	float	k	= (stepX==0)? 0 : (float)(stepY) / stepX;	// 扁匡扁
									

	//--------------------------------------------------
	// 规氢阑 沥秦具 茄促.	
	//--------------------------------------------------
	if (stepY == 0)
	{
		// X绵
		// - -;;
		if (stepX == 0)
			return DIRECTION_DOWN;
		else if (stepX > 0)
			return DIRECTION_RIGHT;
		else 
			return DIRECTION_LEFT;
	}
	else
	if (stepY < 0)	// UP率栏肺
	{
		// y绵 困
		if (stepX == 0)
		{
			return DIRECTION_UP;
		}
		// 1荤盒搁
		else if (stepX > 0)
		{
			if (k < -BASIS_DIRECTION_HIGH)
				return DIRECTION_UP;
			else if (k <= -BASIS_DIRECTION_LOW)
				return DIRECTION_RIGHTUP;
			else
				return DIRECTION_RIGHT;
		}
		// 2荤盒搁
		else
		{
			if (k > BASIS_DIRECTION_HIGH)
				return DIRECTION_UP;
			else if (k >= BASIS_DIRECTION_LOW)
				return DIRECTION_LEFTUP;
			else
				return DIRECTION_LEFT;
		}
	}
	// 酒贰率
	else
	{		
		// y绵 酒贰
		if (stepX == 0)
		{
			return DIRECTION_DOWN;
		}
		// 4荤盒搁
		else if (stepX > 0)
		{
			if (k > BASIS_DIRECTION_HIGH)
				return DIRECTION_DOWN;
			else if (k >= BASIS_DIRECTION_LOW)
				return DIRECTION_RIGHTDOWN;
			else
				return DIRECTION_RIGHT;
		}
		// 3荤盒搁
		else
		{
			if (k < -BASIS_DIRECTION_HIGH)
				return DIRECTION_DOWN;
			else if (k <= -BASIS_DIRECTION_LOW)
				return DIRECTION_LEFTDOWN;
			else
				return DIRECTION_LEFT;
		}
	}
}

//----------------------------------------------------------------------
// Set Fade Start
//----------------------------------------------------------------------
// start何磐 end鳖瘤 step究..
// (r,g,b)祸彬肺 Fade in 矫挪促.
// 0(盔祸)~31(绝澜)
//----------------------------------------------------------------------
void			
MTopView::SetFadeStart(char start, char end, char step, BYTE r, BYTE g, BYTE b, WORD delay)
{	
	step = abs(step);

	m_FadeValue	= start; 
	m_FadeEnd	= end;
	m_FadeInc	= (start<end)? step : -step; 
	m_bFade		= true;
	m_delayFrame = delay;
	if (CDirect3D::IsHAL())
	{
		// 3D 啊加牢 版快浚 4:4:4捞促.
		m_FadeColor = (((WORD)r>>1) << 8)
						| (((WORD)g>>1) << 4)
						| ((WORD)b>>1);
	}
	else
	{
		m_FadeColor	= CDirectDraw::Color(r,g,b);
	}
}


//----------------------------------------------------------------------
// Draw Fade
//----------------------------------------------------------------------
// Fade In/Out 贸府
//----------------------------------------------------------------------	
void 
MTopView::DrawFade()
{	
	if(g_pEventManager->GetEventByFlag(EVENTFLAG_NOT_FADE_SCREEN))
		return;

	//--------------------------------------------------------
	// Fade In/Out 矫虐绰 吝捞搁 免仿..
	//--------------------------------------------------------
	if (m_bFade)// || bEvent)
	{
		static DWORD TempFadeFrame = g_CurrentFrame;
//		if(bEvent)
//		{
//			const MEvent *event = g_pEventManager->GetEventByFlag(EVENTFLAG_FADE_SCREEN);
//
//			m_FadeValue = event->parameter1 >> 24;
//			
//			m_bFade = true;
//			m_FadeInc = 1;
//			m_FadeEnd = 31;
//
//			int r = (event->parameter1 >> 16)&0xff, g = (event->parameter1 >> 8)&0xff, b = (event->parameter1)&0xff;
//			
//			if (CDirect3D::IsHAL())
//			{
//				// 3D 啊加牢 版快浚 4:4:4捞促.
//				m_FadeColor = (((WORD)r>>1) << 8)
//					| (((WORD)g>>1) << 4)
//					| ((WORD)b>>1);
//			}
//			else
//			{
//				m_FadeColor	= CDirectDraw::Color(r,g,b);
//			}
//		}

		RECT rect;
		rect.left =0;
		rect.top = 0;
		rect.right = g_GameRect.right;
		rect.bottom = g_GameRect.bottom;	

//		//--------------------------------------------------------
//		// 3D 啊加狼 版快
//		//--------------------------------------------------------
//		if (CDirect3D::IsHAL())
//		{
//			WORD alpha = (32-m_FadeValue)>>1;			
//			WORD pixel = alpha << 12;
//
//			// m_FadeColor肺 拳搁阑 丹绰促.
//			pixel |= m_FadeColor;
//
//			DrawBox3D(&rect, pixel);
//		}
//		//--------------------------------------------------------
//		// 2D
//		//--------------------------------------------------------
//		else
		{
			m_pSurface->Lock();

			//-------------------------------------------------
			// 八沥祸捞搁.. 奖霸 等促~
			//-------------------------------------------------
			if (m_FadeColor==0)
			{
				// 2D 5:6:5
				if (CDirectDraw::Is565())
				{
					m_pSurface->GammaBox565(&rect, m_FadeValue);
				}
				// 2D 5:5:5
				else
				{
					m_pSurface->GammaBox555(&rect, m_FadeValue);
				}
			}
			//-------------------------------------------------
			// 酒聪搁...
			//-------------------------------------------------
			else
			{
				m_pSurface->BltColorAlpha(&rect, m_FadeColor, m_FadeValue);
			}

			m_pSurface->Unlock();
		}

		//------------------------------------------------
		// 促澜 fade蔼
		//------------------------------------------------
		// 2004, 6, 21, sobeit add start - 龙靛饭 楷免 东俊 眠啊..
		if(m_delayFrame)
		{
			if(g_CurrentFrame - TempFadeFrame >= m_delayFrame)
			{
				if( m_FadeEnd == -1 && 1 == m_FadeValue ) // 龙靛饭 楷免东俊 绢滴况 柳淬俊 泪矫 蜡瘤..^^;
				{
					if(g_CurrentFrame - TempFadeFrame> 16*5) // 5檬埃 蜡瘤
						m_bFade = false;
				}
				else
				{
					TempFadeFrame = g_CurrentFrame;
					m_FadeValue += m_FadeInc;
				}
			}
		}
		else
		// 2004, 6, 21, sobeit add end - 龙靛饭 楷免 东俊 眠啊..
			m_FadeValue += m_FadeInc;

		//------------------------------------------------
		// 场牢啊?
		//------------------------------------------------
		// 刘啊窍绊 乐绰 版快
		if (m_FadeInc > 0)
		{
			if (m_FadeValue > m_FadeEnd || m_FadeValue > 31)
			{
				m_bFade = false;
			}
		}
		else
		{
			if (m_FadeValue < m_FadeEnd || m_FadeValue < 1)
			{
				m_bFade = false;
			}
		}	
	}	
}
//----------------------------------------------------------------------
// 拳搁 谅钎 (x,y)啊 啊府虐绰 困摹甫 急琶窍搁 
// Zone狼 绢恫(sX,sY) Object啊 急琶瞪鳖?
//----------------------------------------------------------------------
MObject*
MTopView::GetSelectedObject(int x, int y)
{
	//--------------------------------------------------------------	
	// 1. 拳搁谅钎 (x,y)狼 Zone俊辑狼 Sector(sX,sY)谅钎甫 备茄促.
	// 
	// 2. (sX,sY) 辟贸俊 Object啊 乐栏搁 
	//    弊 Object狼 拳搁 惑狼 谅钎甫 备秦辑
	//    (x,y)啊 加窍搁 "急琶茄促"
	//    酒聪搁, 弊 促澜狼 辟贸 Sector甫 check秦毫具 茄促.
	//
	// (!) Player绰 力寇促. 绝促绊 积阿窍绊 check茄促.
	//--------------------------------------------------------------

	//--------------------------------------------------------------
	// 辟贸 Sector甫 茫绰 鉴辑
	//--------------------------------------------------------------	
	// - 弊覆篮 酒贰率 Sector俊 乐绰 巴老荐废 词霸 免仿登骨肺
	//   急琶瞪 快急鉴困啊 臭促.
	// 
	//      .  .  .  
	//      .  X  . 
	//      .  .  .  
	//      .  .  .  
	//      .  .  .     X啊 急琶灯阑 版快, Ground,Item 殿... check裹困
	//
	//
	//               
	//         X    
	//               
	//      .  .  .  
	//      .  .  .  
	//      .  .  .  
	//      .  .  .     Flying捞 64 pixel(滴 鸥老) 臭捞 躲 版快..
	//      .  .  .     X啊 急琶灯阑 版快, Flying... check裹困
	//
	//--------------------------------------------------------------

	//--------------------------------------------------------------
	// 阿 Sector俊 措秦辑 check秦毫具 窍绰 巴
	//--------------------------------------------------------------
	/*
	 
	   while ( ExistNextSector )
	   {			
			if ( IsExistObject )
			{
				if ( IsObjectCreature && ClickPoint In TheCreatureRect)
					Select Creature
	    
				else if ( IsObjectItem && ClickPoint In TheItemRect )
					Select Item
			}			

			CurrentSector = NextSector
		}

		if ( Not Selected )
			return NULL;

		return Object;
	*/
	//--------------------------------------------------------------

	//--------------------------------------------------------------
	// 酒流 霸烙 拳搁捞 救 弊妨柳 版快捞搁..
	// Player啊 促农聪胶 加俊 乐促搁..
	// 弊成 return
	//--------------------------------------------------------------
//	bool bSlayerPlayer = g_pPlayer->IsSlayer();

	if ((m_bFirstTileDraw
		|| !g_pPlayer->IsVampire()&& g_pPlayer->IsInDarkness() && (!g_pPlayer->HasEffectStatus( EFFECTSTATUS_LIGHTNESS )||g_pZone->GetID() == 3001)
		&& !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
		&& !g_bLight
#endif
		)
		)		
	{
		return NULL;
	}

	POINT firstZonePixel;	// 拳搁惑狼 贸澜 Sector啊 唱鸥郴绰 Zone狼 pixel谅钎
	firstZonePixel = MapToPixel(m_FirstSector.x, m_FirstSector.y);
	firstZonePixel.x += m_PlusPoint.x;
	firstZonePixel.y += m_PlusPoint.y;


	POINT	pixelPoint;			// Pixel Position In Zone
	POINT	sectorPoint;		// Sector Position In Zone

	//--------------------------------------------------------------
	// (x,y)狼 Zone俊辑狼 pixel谅钎甫 备茄促.
	//--------------------------------------------------------------
	pixelPoint = MapToPixel(m_FirstSector.x, m_FirstSector.y);
	pixelPoint.x += m_PlusPoint.x + x;
	pixelPoint.y += m_PlusPoint.y + y;

	//--------------------------------------------------------------
	// (x,y)狼 Zone俊辑狼 sector谅钎甫 备茄促.
	//--------------------------------------------------------------						
	sectorPoint = PixelToMap(pixelPoint.x, pixelPoint.y);

	g_MouseSector = sectorPoint;

	//--------------------------------------------------------------
	// 器呕 眉农
	//--------------------------------------------------------------
	// 咯扁辑 窍绰扒 眶扁瘤父.. -_-;;
	// 贸府狼 祈狼甫 困秦辑.. 烙矫肺... 且且... ぱ.ぱ;;
	//--------------------------------------------------------------
	g_bMouseInPortal = false;

	if (sectorPoint.x >= 0 
		&& sectorPoint.y >= 0
		&& sectorPoint.x < g_pZone->GetWidth() 
		&& sectorPoint.y < g_pZone->GetHeight())
	{
		#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
			DEBUG_ADD("ptChk");
		#endif

		const MSector& sector = m_pZone->GetSector(sectorPoint.x, sectorPoint.y);

		if (sector.IsPortal())
		{
			#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
				DEBUG_ADD("hasPt");
			#endif

			int numPortal = sector.GetPortalSize();
			
			
			/*				
			for (int i=0; i<numPortal; i++)
			{
				// 捞悼 啊瓷茄 zone狼 ID
				PORTAL_INFO portalInfo = *iPortal;			

				//-------------------------------------------------------
				// 烙矫 免仿
				//-------------------------------------------------------
				DEBUG_ADD_FORMAT( "[Portal] Type=%d, ZoneID=%d", portalInfo.Type, portalInfo.ZoneID );
			
				iPortal++;
			}
			*/

			if (numPortal != 0)
			{
				PORTAL_LIST::const_iterator	iPortal = sector.GetPortalBegin();

				PORTAL_INFO portalInfo = *iPortal;
				
				bool canMove = false;
				bool bSiegePotal = false;
				//-------------------------------------------------------
				// portal狼 辆幅俊 蝶扼 哎荐 乐绰镑牢瘤 眉农
				//-------------------------------------------------------			
				switch (portalInfo.Type)				
				{
					case MPortal::TYPE_BATTLE_PORTAL :
					case MPortal::TYPE_NOMAL :
					case MPortal::TYPE_CLIENT_ONLY :
						canMove = true;
					break;

					case MPortal::TYPE_SLAYER_ONLY :
						canMove = g_pPlayer->IsSlayer();
					break;

					case MPortal::TYPE_VAMPIRE_ONLY :
						canMove = g_pPlayer->IsVampire();
					break;

					case MPortal::TYPE_OUSTERS_ONLY :
						canMove = g_pPlayer->IsOusters();
						break;
						
					case MPortal::TYPE_MULTI_PORTAL :
						canMove = g_pPlayer->IsSlayer();
					break;
					
					case MPortal::TYPE_GUILD_PORTAL:
						canMove = (g_pUserInformation->GuildName.GetLength()>0);
					break;
					case MPortal::TYPE_CLIENT_SIEGE_POTAL_1:
					case MPortal::TYPE_CLIENT_SIEGE_POTAL_2:
					case MPortal::TYPE_CLIENT_SIEGE_POTAL_3:
					case MPortal::TYPE_CLIENT_SIEGE_POTAL_4:
						canMove = true;
						bSiegePotal = true;
						break;

				}

				//-------------------------------------------------------			
				// 哎 荐 乐栏搁 器呕 目辑 免仿
				//-------------------------------------------------------			
				if (canMove)
				{
					ZONETABLE_INFO* pZoneInfo = g_pZoneTable->Get( portalInfo.ZoneID );

					if (pZoneInfo!=NULL)
					{
						//char pZoneName[80];
						int zoneID = portalInfo.ZoneID;

						if(bSiegePotal)
						{
							if(g_pSystemAvailableManager->ZoneFiltering( zoneID )&& zoneID == 1500)
								gpC_mouse_pointer->SetCursorPortal( zoneID , portalInfo.Type);
						}
						else
						{
							if (numPortal <= 1)
							{
								//strcpy(pZoneName, pZoneInfo->Name.GetString());
							}
							else
							{
								// 鞘混 窍靛内爹~~
								zoneID = 60001;
								//strcpy(pZoneName, "郡府海捞磐");
							}

							//if (!g_bMouseInPortal)
							//{
							//	gpC_mouse_pointer->SetCursorPortal( pZoneName );
							//}
							
							if(// (!g_pUserInformation->IsNetmarble || g_mapPremiumZone.find(zoneID) == g_mapPremiumZone.end())
								/*&&*/ g_pSystemAvailableManager->ZoneFiltering( zoneID ))
								gpC_mouse_pointer->SetCursorPortal( zoneID );
						}
					}
				}
				else
				{
					
					if (!g_bMouseInPortal)
					{
						//char pZoneName[80];
						//strcpy(pZoneName, "哎 荐 绝绰 镑");
						//gpC_mouse_pointer->SetCursorPortal( -1 );
					}
				}
			}

			g_bMouseInPortal = true;

			#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
				DEBUG_ADD("ptK");
			#endif
		}		

		#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
			DEBUG_ADD("ptK2");
		#endif
	}
	
	
	//--------------------------------------------------------------
	//
	// check
	//
	//--------------------------------------------------------------
	
	MObject*	pObject = NULL;	
	MCreature*	pCreature = NULL;
	MItem*		pItem	= NULL;
	CFrame			frame;
//	TYPE_SPRITEID	sprite;

	// Object狼 康开
//	POINT	objectPixelPoint;
	//RECT	rect;

	// Player Creature's ID
	TYPE_OBJECTID	pid = g_pPlayer->GetID();

	//class ofstream file("log.txt");


	// 泅犁 check窍绰 谅钎
	register int currentX;
	register int currentY;

	int numCreature = 0;

	//--------------------------------------------------------------
	//
	// Flying Creature Check 
	//
	// : 鞍篮 臭捞俊 乐绰 版快 
	//   老馆利牢 Object焊促 酒贰率 Sector俊 粮犁且 荐 乐促.
	//
	//--------------------------------------------------------------
	int	sX1 = sectorPoint.x - 3,		// 茄 临究 歹 刘啊矫淖促. 澜衬府~
			sX2 = sectorPoint.x + 3;

	// 冠零 臭捞 锭巩俊.. +甫 歹沁促.
	int	sY1 = sectorPoint.y + 4,
			sY2 = sectorPoint.y + 8;

	//------------------------------------------------------
	// Zone狼 康开捞 酒囱 版快俊 Skip...
	//------------------------------------------------------
	if (sX1 < 0) 
	{			
		sX1 = 0;	
	}

	if (sX2 >= m_pZone->GetWidth())
	{
		sX2 = m_pZone->GetWidth()-1;
	}

	if (sY1 < 0)
	{	
		sY1 = 0;	
	}

	if (sY2 >= m_pZone->GetHeight())
	{
		sY2 = m_pZone->GetHeight()-1;
	}


	//------------------------------------------------------	
	// 乐阑过茄 Sector甫 check茄促.
	//------------------------------------------------------	
	for (currentY=sY2; currentY>=sY1; currentY--)	
	{	
		for (currentX=sX1; currentX<=sX2; currentX++)	
		{			
			#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
				DEBUG_ADD_FORMAT("ckF1(%d,%d)", currentX, currentY);
			#endif

			const MSector& sector = m_pZone->GetSector(currentX, currentY);

			// 绢恫 Object啊 乐阑 版快
			if (sector.IsExistObject() )			
			{		
				
				//file << "Exist Object!" << endl;

				// Object狼 辆幅俊 蝶扼辑 崔府 check茄促.
				// 急琶且 荐 乐绰 巴篮 Creature客 Item挥捞促.

				//------------------------------------------------
				//
				// Flying Creature啊 乐绰 版快
				//
				//------------------------------------------------
				numCreature = sector.GetFlyingCreatureSize();

				if (numCreature > 0)
				{
					#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
						DEBUG_ADD("hasFC");
					#endif

					OBJECT_MAP::const_iterator iCreature = sector.GetFlyingCreatureIterator();

					for (int c=0; c<numCreature; c++, iCreature++)
					{
						if (iCreature==sector.GetObjectEnd() 
							|| iCreature->first > MSector::POSITION_FLYINGCREATURE_MAX)
							break;

						pCreature = (MCreature*)iCreature->second;
				
						// Player捞搁 急琶窍瘤 臼绰促.
						if (pCreature->GetID()!=pid)	
						{
							// FlyingCreature啊 瞒瘤窍绰 康开阑 备茄促.
							// FRAME_PACK俊辑 沥焊甫 佬绢具 且 淀!!
							// FlyingCreature狼 Sector谅钎甫 Pixel谅钎肺 官层具 茄促.

							// FlyingCreature狼 Zone狼 谅钎甫 拌魂茄促.
							/*
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// Zone狼 谅钎甫 拳搁狼 谅钎肺 官槽促.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature啊 拳搁俊 粮犁窍绰 康开阑 拌魂秦辰促.	
							frame = m_CreatureFPK[(*g_pCreatureTable)[pCreature->GetCreatureType()].FrameID][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// 泅犁 Sprite啊 拳搁俊 免仿登绰 困摹
							objectPixelPoint.x += frame.GetCX();
							objectPixelPoint.y += frame.GetCY() - FLYINGCREATURE_HEIGHT;
												// FramePack俊辑 臭捞甫 汲沥窍骨肺,
												// Flying牢 版快档 度鞍促.

							// Creature啊 拳搁俊 粮犁窍绰 康开阑 拌魂秦辰促.
							rect.left	= objectPixelPoint.x;
							rect.top	= objectPixelPoint.y;
							rect.right	= rect.left + m_CreatureSPK[ frame.GetSpriteID() ].GetWidth();
							rect.bottom	= rect.top + m_CreatureSPK[ frame.GetSpriteID() ].GetHeight();
							*/

							// 促农聪胶俊 乐栏搁 
							// 急琶茄 困摹啊 Creature狼 康开俊 加窍搁
							// 傍拜 mode牢 版快绰 傍拜啊瓷茄 某腐磐父 急琶
							// 酒聪搁 酒公唱.
							if (
								(g_pPlayer->IsVampire()&&g_pZone->GetID() != 3001 ||
								!g_pPlayer->IsVampire()&& !(!pCreature->IsNPC() && pCreature->IsInDarkness()) ||
								!g_pPlayer->IsVampire()&& g_pPlayer->HasEffectStatus( EFFECTSTATUS_LIGHTNESS ) 
								|| g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
								|| g_bLight
#endif
								)
								&& !pCreature->IsCutHeight()	// 荤扼瘤绰 惑怕搁
								&& g_pObjectSelector->CanSelect(pCreature)
								&& pCreature->IsPointInScreenRect(x,y)
								&& !(pCreature->IsOusters() && pCreature->IsInGroundElemental() && !g_pPlayer->IsOusters()))
							//if (x > rect.left && x < rect.right
								//&& y > rect.top && y < rect.bottom)
							{											
								return pCreature;
							}				
						}
					}
				}	
			}
		}
	}

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
		DEBUG_ADD("chk2");
	#endif

	//--------------------------------------------------------------
	//
	// Creature / Item Check data
	//
	//--------------------------------------------------------------
	sY1 = sectorPoint.y - 3,
	sY2 = sectorPoint.y + 4;		// 茄 临究 歹 刘啊矫淖促.

	//------------------------------------------------------
	// Zone狼 康开捞 酒囱 版快俊 Skip...
	//------------------------------------------------------	
	if (sY1 < 0)
	{	
		sY1 = 0;	
	}

	if (sY2 >= m_pZone->GetHeight())
	{
		sY2 = m_pZone->GetHeight()-1;
	}

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
		DEBUG_ADD("ckIO9");
	#endif

	//------------------------------------------------------
	// Check措惑捞 登绰 ImageObject Iterator
	//------------------------------------------------------
	IMAGEOBJECT_OUTPUT_MAP::reverse_iterator iImageObjectCheck
		= m_mapImageObject.rbegin();

	//------------------------------------------------------
	// 免仿 矫痢捞 sY1焊促 利篮 版快狼 
	// ImageObject甸阑 Check茄促.
	// m_mapImageObject俊绰 Viewpoint啊 key蔼捞 登绢辑 Sort登绢 乐促.
	//------------------------------------------------------
	while (iImageObjectCheck != m_mapImageObject.rend())
	{		
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectCheck).second);

		// InteractionObject牢 版快父 check茄促.
//		if (pImageObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
//		{
//			MInteractionObject* const pInteractionObject = (MInteractionObject* const)pImageObject;
//
//			// 免仿 矫痢捞 sY1焊促 利篮 版快..绰 Check
//			if (pInteractionObject->GetViewpoint() > sY2)
//			{
//				/*
//				// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
//				objectPixelPoint.x = pInteractionObject->GetPixelX() - m_FirstZonePixel.x;
//				objectPixelPoint.y = pInteractionObject->GetPixelY() - m_FirstZonePixel.y;
//
//				// frame俊 包拌等 sprite甫 备茄促.
//				frame = m_ImageObjectFPK[ pInteractionObject->GetFrameID() ][ pInteractionObject->GetFrame() ];
//				sprite = frame.GetSpriteID();
//
//				// 檬扁拳 登瘤 臼篮 版快搁..
//				if (m_ImageObjectSPK[ sprite ].IsNotInit())
//				{
//					iImageObjectCheck ++;
//					continue;
//				}
//						
//				// 谅钎 焊沥
//				objectPixelPoint.x += frame.GetCX();
//				objectPixelPoint.y += frame.GetCY();
//
//				// 拳搁俊 粮犁窍绰 康开阑 拌魂秦辰促.
//				rect.left	= objectPixelPoint.x;
//				rect.top	= objectPixelPoint.y;
//				rect.right	= rect.left + m_ImageObjectSPK[ sprite ].GetWidth();
//				rect.bottom	= rect.top + m_ImageObjectSPK[ sprite ].GetHeight();
//				*/		
//				// 急琶茄 困摹啊 ImageObject狼 康开俊 加窍搁
//				if (pInteractionObject->IsPointInScreenRect(x,y))
//				//if (x > rect.left && x < rect.right
//				//	&& y > rect.top && y < rect.bottom)
//				{
//					#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
//						DEBUG_ADD("reIO9");
//					#endif
//
//					//pInteractionObject->SetAction( 1 );
//					return pInteractionObject;
//				}
//			}
//			// 酒聪搁.. Object check且锭 Check秦具登绰 版快捞促.
//			else
//			{
//				break;
//			}
//		}

		iImageObjectCheck ++;
	}

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
		DEBUG_ADD("ckSe");
	#endif

	//------------------------------------------------------	
	// 乐阑过茄 Sector甫 check茄促.
	//------------------------------------------------------	
	for (currentY=sY2; currentY>=sY1; currentY--)	
	{	
		for (currentX=sX1; currentX<=sX2; currentX++)	
		{				
			#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
				DEBUG_ADD_FORMAT("gS(%d,%d)", currentX, currentY);
			#endif

			const MSector& sector = m_pZone->GetSector(currentX, currentY);

			// 绢恫 Object啊 乐阑 版快
			if (sector.IsExistObject() )			
			{		
				
				//file << "Exist Object!" << endl;

				// Object狼 辆幅俊 蝶扼辑 崔府 check茄促.
				// 急琶且 荐 乐绰 巴篮 Creature客 Item挥捞促.

				//------------------------------------------------
				//
				// Flying Creature啊 乐绰 版快
				//
				//------------------------------------------------
				#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
					DEBUG_ADD("ckFc");
				#endif

				numCreature = sector.GetFlyingCreatureSize();

				if (numCreature > 0)
				{
					OBJECT_MAP::const_iterator iCreature = sector.GetFlyingCreatureIterator();

					for (int c=0; c<numCreature; c++, iCreature++)
					{
						if (iCreature==sector.GetObjectEnd() 
							|| iCreature->first > MSector::POSITION_FLYINGCREATURE_MAX)
							break;

						pCreature = (MCreature*)iCreature->second;

						// Player捞搁 急琶窍瘤 臼绰促.
						if (pCreature->GetID()!=pid)	
						{
							/*
							// FlyingCreature啊 瞒瘤窍绰 康开阑 备茄促.
							// FRAME_PACK俊辑 沥焊甫 佬绢具 且 淀!!
							// FlyingCreature狼 Sector谅钎甫 Pixel谅钎肺 官层具 茄促.

							// FlyingCreature狼 Zone狼 谅钎甫 拌魂茄促.
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// Zone狼 谅钎甫 拳搁狼 谅钎肺 官槽促.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature啊 拳搁俊 粮犁窍绰 康开阑 拌魂秦辰促.	
							frame = m_CreatureFPK[(*g_pCreatureTable)[pCreature->GetCreatureType()].FrameID][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// 泅犁 Sprite啊 拳搁俊 免仿登绰 困摹
							objectPixelPoint.x += frame.GetCX();
							objectPixelPoint.y += frame.GetCY() - FLYINGCREATURE_HEIGHT;
												// FramePack俊辑 臭捞甫 汲沥窍骨肺,
												// Flying牢 版快档 度鞍促.

							// 拳搁俊 粮犁窍绰 康开阑 拌魂秦辰促.
							rect.left	= objectPixelPoint.x;
							rect.top	= objectPixelPoint.y;
							rect.right	= rect.left + m_CreatureSPK[ frame.GetSpriteID() ].GetWidth();
							rect.bottom	= rect.top + m_CreatureSPK[ frame.GetSpriteID() ].GetHeight();
							*/
							
							// 急琶茄 困摹啊 Creature狼 康开俊 加窍搁
							if ((
								g_pPlayer->IsVampire()&&g_pZone->GetID() != 3001|| 
								!g_pPlayer->IsVampire()&& !(!pCreature->IsNPC() && pCreature->IsInDarkness()) ||
								!g_pPlayer->IsVampire()&& g_pPlayer->HasEffectStatus( EFFECTSTATUS_LIGHTNESS ) 
								|| g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
								|| g_bLight
#endif
								)
								&& !pCreature->IsCutHeight()	// 荤扼瘤绰 惑怕搁
								&& g_pObjectSelector->CanSelect(pCreature)
								&& pCreature->IsPointInScreenRect(x,y)
								&& !(pCreature->IsOusters() && pCreature->IsInGroundElemental() && !g_pPlayer->IsOusters()))
							//if (x > rect.left && x < rect.right
							//	&& y > rect.top && y < rect.bottom)
							{
								#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
									DEBUG_ADD("reFc");
								#endif

								return pCreature;
							}
						}
					}
				}				
				
				//------------------------------------------------
				//
				// Creature啊 乐绰 版快
				//
				//------------------------------------------------
				#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
					DEBUG_ADD("ckGc");
				#endif

				numCreature = sector.GetGroundCreatureSize();

				if (numCreature > 0)
				{
					OBJECT_MAP::const_iterator iCreature = sector.GetGroundCreatureIterator();

					for (int c=0; c<numCreature; c++, iCreature++)
					{
						if (iCreature==sector.GetObjectEnd() 
							|| iCreature->first > MSector::POSITION_GROUNDCREATURE_MAX)
							break;

						pCreature = (MCreature*)iCreature->second;

						// Player捞搁 急琶窍瘤 臼绰促.
						if (pCreature->GetID()!=pid)	
						{
							/*
							// Creature啊 瞒瘤窍绰 康开阑 备茄促.
							// FRAME_PACK俊辑 沥焊甫 佬绢具 且 淀!!
							// Creature狼 Sector谅钎甫 Pixel谅钎肺 官层具 茄促.

							// Creature狼 Zone狼 谅钎甫 拌魂茄促.
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// Zone狼 谅钎甫 拳搁狼 谅钎肺 官槽促.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature啊 拳搁俊 粮犁窍绰 康开阑 拌魂秦辰促.	
							frame = m_CreatureFPK[(*g_pCreatureTable)[pCreature->GetCreatureType()].FrameID][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// 泅犁 Sprite啊 拳搁俊 免仿登绰 困摹
							objectPixelPoint.x += frame.GetCX();
							objectPixelPoint.y += frame.GetCY();

							// 拳搁俊 粮犁窍绰 康开阑 拌魂秦辰促.
							rect.left	= objectPixelPoint.x;
							rect.top	= objectPixelPoint.y;
							rect.right	= rect.left + m_CreatureSPK[ frame.GetSpriteID() ].GetWidth();
							rect.bottom	= rect.top + m_CreatureSPK[ frame.GetSpriteID() ].GetHeight();
							*/			
							// 急琶茄 困摹啊 Creature狼 康开俊 加窍搁
							if ((g_pPlayer->IsVampire()&&g_pZone->GetID() != 3001 ||
								!g_pPlayer->IsVampire()&& !(!pCreature->IsNPC() && pCreature->IsInDarkness()) ||
								!g_pPlayer->IsVampire()&& g_pPlayer->HasEffectStatus( EFFECTSTATUS_LIGHTNESS ) 
								|| g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
								|| g_bLight
#endif
								)
								&& !pCreature->IsCutHeight()	// 荤扼瘤绰 惑怕搁
								&& g_pObjectSelector->CanSelect(pCreature)
								&& pCreature->IsPointInScreenRect(x,y)
								&& !(pCreature->IsOusters() && pCreature->IsInGroundElemental() && !g_pPlayer->IsOusters()))
							//if (x > rect.left && x < rect.right
							//	&& y > rect.top && y < rect.bottom)
							{
								#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
									DEBUG_ADD("reGc");
								#endif

								return pCreature;
							}
						}
					}
				}

				//------------------------------------------------
				//
				// Effect啊 乐绰 版快
				//
				//------------------------------------------------				
				#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
					DEBUG_ADD("ckE");
				#endif

				if (sector.IsExistEffect())
				{	
					int	effectNum = sector.GetEffectSize();

					EFFECT_LIST::const_iterator iEffect = sector.GetEffectIterator();

					for (int e=0; e<effectNum; e++)
					{
						MEffect* pEffect = *iEffect;

						if (pEffect->IsSelectable())
						{
							if (pEffect->IsPointInScreenRect(x,y))							
							{
								#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
									DEBUG_ADD("reE");
								#endif

								return pEffect;
							}							
						}

						iEffect++;
					}					
				}

				//------------------------------------------------
				//
				// UndergroundCreature啊 乐绰 版快
				//
				//------------------------------------------------
				#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
					DEBUG_ADD("ckUc");
				#endif

				numCreature = sector.GetUndergroundCreatureSize();

				if (numCreature > 0)
				{
					OBJECT_MAP::const_iterator iCreature = sector.GetUndergroundCreatureIterator();

					for (int c=0; c<numCreature; c++, iCreature++)
					{
						if (iCreature==sector.GetObjectEnd() 
							|| iCreature->first > MSector::POSITION_UNDERGROUNDCREATURE_MAX)
							break;

						pCreature = (MCreature*)iCreature->second;

						// Player捞搁 急琶窍瘤 臼绰促.
						if (pCreature->GetID()!=pid)	
						{							
							// 急琶茄 困摹啊 Creature狼 康开俊 加窍搁
							if ((g_pPlayer->IsVampire() &&g_pZone->GetID() != 3001||
								!g_pPlayer->IsVampire()&& !(!pCreature->IsNPC() && pCreature->IsInDarkness())
								|| !g_pPlayer->IsVampire() && g_pPlayer->HasEffectStatus( EFFECTSTATUS_LIGHTNESS ) 								 
								||g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
								|| g_bLight
#endif
								)
								&& g_pObjectSelector->CanSelect(pCreature)
								&& pCreature->IsPointInScreenRect(x,y)
								&& !(pCreature->IsOusters() && pCreature->IsInGroundElemental() && !g_pPlayer->IsOusters())
								)
							{
								#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
									DEBUG_ADD("uC");
								#endif

								return pCreature;
							}
						}
					}
				}
						
				//------------------------------------------------
				//
				// Item捞 乐绰 版快
				//
				//------------------------------------------------
				#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
					DEBUG_ADD("ckIt");
				#endif

				MItem* const pItem = sector.GetItem();

				if (pItem != NULL)
				{	
					#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
						DEBUG_ADD("IT");
					#endif

					//file << "Exist Item!" << endl;

					// Item啊 瞒瘤窍绰 康开阑 备茄促.
					// FRAME_PACK俊辑 沥焊甫 佬绢具 且 淀!!
					// Item狼 Sector谅钎甫 Pixel谅钎肺 官层具 茄促.
					/*
					// Item狼 Zone狼 谅钎甫 拌魂茄促.
					objectPixelPoint = MapToPixel(pItem->GetX(), pItem->GetY());
					objectPixelPoint.x += m_PlusPoint.x;// + pItem->GetSX();
					objectPixelPoint.y += m_PlusPoint.y;// + pItem->GetSY();
						
					// Zone狼 谅钎甫 拳搁狼 谅钎肺 官槽促.
					objectPixelPoint.x -= firstZonePixel.x;
					objectPixelPoint.y -= firstZonePixel.y;

					// Item啊 拳搁俊 粮犁窍绰 康开阑 拌魂秦辰促.				
					frame = m_ItemTileFPK[ pItem->GetTileFrameID() ];

					// 泅犁 Sprite啊 拳搁俊 免仿登绰 困摹
					objectPixelPoint.x += frame.GetCX();
					objectPixelPoint.y += frame.GetCY();

					// 拳搁俊 粮犁窍绰 康开阑 拌魂秦辰促.
					rect.left	= objectPixelPoint.x;
					rect.top	= objectPixelPoint.y;
					rect.right	= rect.left + m_ItemTileSPK[ frame.GetSpriteID() ].GetWidth();
					rect.bottom	= rect.top + m_ItemTileSPK[ frame.GetSpriteID() ].GetHeight();
					*/			

					//------------------------------------------------
					// 轨颇捞绢芭唱
					// 浇饭捞绢牢 版快绰 杭 荐 乐绰 惑炔捞扼搁.. 
					//------------------------------------------------
					if (g_pPlayer->IsVampire()&&g_pZone->GetID() != 3001
						|| !g_pPlayer->IsVampire() && g_pPlayer->ShowInDarkness(currentX, currentY) 
						|| g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
						|| g_bLight
#endif
						)
					{
						//------------------------------------------------
						// 矫眉牢 版快绰 
						// 矫眉 Creature狼 康开阑 毫具 茄促.
						//------------------------------------------------
						if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
						{
							#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
								DEBUG_ADD("corp");
							#endif

							MCorpse* pCorpse = (MCorpse*)pItem;

							MCreature* pCreature = pCorpse->GetCreature();

							if (pCreature!=NULL)
							{
								if (pCreature->IsPointInScreenRect(x,y))
								{
									#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
										DEBUG_ADD("reIT0");
									#endif

									return pItem;
								}
							}
						}
						else
						{
							// 急琶茄 困摹啊 Item狼 康开俊 加窍搁
							if (pItem->IsPointInScreenRect(x,y))
							//if (x > rect.left && x < rect.right
								//&& y > rect.top && y < rect.bottom)
							{
								#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
									DEBUG_ADD("reIT");
								#endif

								return pItem;
							}					
						}
					}
				}				

			}
		}

		#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
			DEBUG_ADD("ckIO0");
		#endif

		//------------------------------------------------------
		// 免仿 矫痢捞 currentY焊促 利篮 版快狼 
		// ImageObject甸阑 Check茄促.
		// m_mapImageObject俊绰 Viewpoint啊 key蔼捞 登绢辑 Sort登绢 乐促.
		//------------------------------------------------------
//		while (iImageObjectCheck != m_mapImageObject.rend())
//		{
//			MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectCheck).second);
//
			// InteractionObject牢 版快父 check茄促.
//			if (pImageObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
//			{
//				MInteractionObject* const pInteractionObject = (MInteractionObject* const)pImageObject;
//
//				// 免仿 矫痢捞 sY1焊促 利篮 版快..绰 Check
//				if (pInteractionObject->GetViewpoint() == currentY)
//				{
//					/*
//					// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
//					objectPixelPoint.x = pInteractionObject->GetPixelX() - m_FirstZonePixel.x;
//					objectPixelPoint.y = pInteractionObject->GetPixelY() - m_FirstZonePixel.y;
//
//					// frame俊 包拌等 sprite甫 备茄促.
//					frame = m_ImageObjectFPK[ pInteractionObject->GetFrameID() ][ pInteractionObject->GetFrame() ];
//					sprite = frame.GetSpriteID();
//
//					// 檬扁拳 登瘤 臼篮 版快搁..
//					if (m_ImageObjectSPK[ sprite ].IsNotInit())
//					{
//						iImageObjectCheck ++;
//						continue;
//					}
//							
//					// 谅钎 焊沥
//					objectPixelPoint.x += frame.GetCX();
//					objectPixelPoint.y += frame.GetCY();
//
//					// 拳搁俊 粮犁窍绰 康开阑 拌魂秦辰促.
//					rect.left	= objectPixelPoint.x;
//					rect.top	= objectPixelPoint.y;
//					rect.right	= rect.left + m_ImageObjectSPK[ sprite ].GetWidth();
//					rect.bottom	= rect.top + m_ImageObjectSPK[ sprite ].GetHeight();
//					*/			
//					// 急琶茄 困摹啊 ImageObject狼 康开俊 加窍搁
//					if (pInteractionObject->IsPointInScreenRect(x,y))
//					//if (x > rect.left && x < rect.right
//					//	&& y > rect.top && y < rect.bottom)
//					{			
//						#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
//							DEBUG_ADD("reIO");
//						#endif
//
//						return pInteractionObject;
//					}					
//				}
//				// 酒聪搁.. Object check且锭 Check秦具登绰 版快捞促.
//				else
//				{
//					break;
//				}
//			}
//
//			iImageObjectCheck ++;
//		}
	}	

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
		DEBUG_ADD("ckIO");
	#endif

	//------------------------------------------------------
	// 免仿 矫痢捞 currentY焊促 利篮 版快狼 
	// ImageObject甸阑 Check茄促.
	// m_mapImageObject俊绰 Viewpoint啊 key蔼捞 登绢辑 Sort登绢 乐促.
	//------------------------------------------------------
//	while (iImageObjectCheck != m_mapImageObject.rend())
//	{
//		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectCheck).second);
//
		// InteractionObject牢 版快父 check茄促.
//		if (pImageObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
//		{
//			MInteractionObject* const pInteractionObject = (MInteractionObject* const)pImageObject;
//		
//			// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
//			/*
//			objectPixelPoint.x = pInteractionObject->GetPixelX() - m_FirstZonePixel.x;
//			objectPixelPoint.y = pInteractionObject->GetPixelY() - m_FirstZonePixel.y;
//
//			// frame俊 包拌等 sprite甫 备茄促.
//			frame = m_ImageObjectFPK[ pInteractionObject->GetFrameID() ][ pInteractionObject->GetFrame() ];
//			sprite = frame.GetSpriteID();
//
//			// 檬扁拳 登瘤 臼篮 版快搁..
//			if (m_ImageObjectSPK[ sprite ].IsNotInit())
//			{
//				iImageObjectCheck ++;
//				continue;
//			}
//							
//			// 谅钎 焊沥
//			objectPixelPoint.x += frame.GetCX();
//			objectPixelPoint.y += frame.GetCY();
//
//			// 拳搁俊 粮犁窍绰 康开阑 拌魂秦辰促.
//			rect.left	= objectPixelPoint.x;
//			rect.top	= objectPixelPoint.y;
//			rect.right	= rect.left + m_ImageObjectSPK[ sprite ].GetWidth();
//			rect.bottom	= rect.top + m_ImageObjectSPK[ sprite ].GetHeight();
//			*/				
//			// 急琶茄 困摹啊 ImageObject狼 康开俊 加窍搁
//			if (pInteractionObject->IsPointInScreenRect(x,y))
//			//if (x > rect.left && x < rect.right
//			//	&& y > rect.top && y < rect.bottom)
//			{
//				#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
//					DEBUG_ADD("reIO");
//				#endif
//
//				return pInteractionObject;
//			}
//		}
//
//		iImageObjectCheck ++;
//	}

	//------------------------------------------------------
	// 酒公巴档 绝促搁.. GroundEffect档 眉农茄促.
	//------------------------------------------------------
	#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
		DEBUG_ADD("chkGE");
	#endif

	int effectNum = g_pZone->GetGroundEffectCount();
	MZone::EFFECT_MAP::const_iterator iEffect = g_pZone->GetGroundEffects();
//	MZone::EFFECT_MAP::const_iterator iEffectEnd = g_pZone->GetGroundEffectsEnd();
//
//	while(iEffect != iEffectEnd)
//	{
//		MEffect* pEffect = iEffect->second;
//		if(pEffect != NULL)
//		{
//			if (pEffect->IsSelectable()
//				&& pEffect->IsPointInScreenRect(x, y))
//			{
//				#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
//					DEBUG_ADD("reGE");
//				#endif
//
//				return pEffect;			
//			}
//		}
//		iEffect++;
//	}
	for (int e=0; e<effectNum; e++)
	{
		MEffect* pEffect = iEffect->second;
		if(pEffect != NULL)
		{
			if (pEffect->IsSelectable()
				&& pEffect->IsPointInScreenRect(x, y))
			{
				#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
					DEBUG_ADD("reGE");
				#endif

				return pEffect;			
			}
		}
		iEffect++;
	}

	if(IsRequestInfo())
	{
		//------------------------------------------------------
		// 酒公巴档 绝促搁.. FakeCreature(Pet)档 眉农茄促.
		//------------------------------------------------------
		int fakeCreatureNum = g_pZone->GetFakeCreatureNumber();
		MZone::CREATURE_MAP::const_iterator iFakeCreature = g_pZone->GetFakeCreatureBegin();
		for(int fc = 0; fc < fakeCreatureNum; fc++)
		{
			MFakeCreature *pFakeCreature = (MFakeCreature *)iFakeCreature->second;

			// 脐牢版快父 八祸
			if(pFakeCreature->GetOwnerID() != OBJECTID_NULL && pFakeCreature->IsPointInScreenRect(x, y))
			{
				return pFakeCreature;
			}

			iFakeCreature++;
		}
	}

	return NULL;
}


//----------------------------------------------------------------------
// 拳搁 谅钎 (x,y)啊 啊府虐绰 困摹甫 急琶窍搁 
// Zone狼 绢恫(sX,sY) Object啊 急琶瞪鳖?
//
// Sprite俊辑 pixel沥焊甫 佬绢辑 厚背茄促.
//
//----------------------------------------------------------------------
MObject*
MTopView::GetSelectedObjectSprite(int x, int y)
{
	//--------------------------------------------------------------	
	// 1. 拳搁谅钎 (x,y)狼 Zone俊辑狼 Sector(sX,sY)谅钎甫 备茄促.
	// 
	// 2. (sX,sY) 辟贸俊 Object啊 乐栏搁 
	//    弊 Object狼 拳搁 惑狼 谅钎甫 备秦辑
	//    (x,y)啊 加窍搁 "急琶茄促"
	//    酒聪搁, 弊 促澜狼 辟贸 Sector甫 check秦毫具 茄促.
	//
	// (!) Player绰 力寇促. 绝促绊 积阿窍绊 check茄促.
	//--------------------------------------------------------------

	//--------------------------------------------------------------
	// 辟贸 Sector甫 茫绰 鉴辑
	//--------------------------------------------------------------	
	// - 弊覆篮 酒贰率 Sector俊 乐绰 巴老荐废 词霸 免仿登骨肺
	//   急琶瞪 快急鉴困啊 臭促.
	// 
	//      .  .  .  
	//      .  X  . 
	//      .  .  .  
	//      .  .  .  
	//      .  .  .     X啊 急琶灯阑 版快, Ground,Item 殿... check裹困
	//
	//
	//               
	//         X    
	//               
	//      .  .  .  
	//      .  .  .  
	//      .  .  .  
	//      .  .  .     Flying捞 64 pixel(滴 鸥老) 臭捞 躲 版快..
	//      .  .  .     X啊 急琶灯阑 版快, Flying... check裹困
	//
	//--------------------------------------------------------------

	//--------------------------------------------------------------
	// 阿 Sector俊 措秦辑 check秦毫具 窍绰 巴
	//--------------------------------------------------------------
	/*
	 
	   while ( ExistNextSector )
	   {			
			if ( IsExistObject )
			{
				if ( IsObjectCreature && ClickPoint In TheCreatureRect)
					Select Creature
	    
				else if ( IsObjectItem && ClickPoint In TheItemRect )
					Select Item
			}			

			CurrentSector = NextSector
		}

		if ( Not Selected )
			return NULL;

		return Object;
	*/
	//--------------------------------------------------------------

	POINT firstZonePixel;	// 拳搁惑狼 贸澜 Sector啊 唱鸥郴绰 Zone狼 pixel谅钎
	firstZonePixel = MapToPixel(m_FirstSector.x, m_FirstSector.y);
	firstZonePixel.x += m_PlusPoint.x;
	firstZonePixel.y += m_PlusPoint.y;


	POINT	pixelPoint;			// Pixel Position In Zone
	POINT	sectorPoint;		// Sector Position In Zone

	//--------------------------------------------------------------
	// (x,y)狼 Zone俊辑狼 pixel谅钎甫 备茄促.
	//--------------------------------------------------------------
	pixelPoint = MapToPixel(m_FirstSector.x, m_FirstSector.y);
	pixelPoint.x += m_PlusPoint.x + x;
	pixelPoint.y += m_PlusPoint.y + y;

	//--------------------------------------------------------------
	// (x,y)狼 Zone俊辑狼 sector谅钎甫 备茄促.
	//--------------------------------------------------------------						
	sectorPoint = PixelToMap(pixelPoint.x, pixelPoint.y);
	
	//--------------------------------------------------------------
	//
	// check
	//
	//--------------------------------------------------------------
	
	MObject*	pObject = NULL;	
	MCreature*	pCreature = NULL;
	MItem*		pItem	= NULL;
	CFrame			frame;
//	TYPE_SPRITEID	sprite;

	// Object狼 康开
	POINT	objectPixelPoint;

	// Player Creature's ID
	TYPE_OBJECTID	pid = g_pPlayer->GetID();

	//class ofstream file("log.txt");


	// 泅犁 check窍绰 谅钎
	register int currentX;
	register int currentY;

	int numCreature = 0;

	//--------------------------------------------------------------
	//
	// Flying Creature Check 
	//
	// : 鞍篮 臭捞俊 乐绰 版快 
	//   老馆利牢 Object焊促 酒贰率 Sector俊 粮犁且 荐 乐促.
	//
	//--------------------------------------------------------------
	int	sX1 = sectorPoint.x - 1,
			sX2 = sectorPoint.x + 1;
	int	sY1 = sectorPoint.y + 4,
			sY2 = sectorPoint.y + 5;

	//------------------------------------------------------
	// Zone狼 康开捞 酒囱 版快俊 Skip...
	//------------------------------------------------------
	if (sX1 < 0) 
	{			
		sX1 = 0;	
	}

	if (sX2 >= m_pZone->GetWidth())
	{
		sX2 = m_pZone->GetWidth()-1;
	}

	if (sY1 < 0)
	{	
		sY1 = 0;	
	}

	if (sY2 >= m_pZone->GetHeight())
	{
		sY2 = m_pZone->GetHeight()-1;
	}


	//------------------------------------------------------	
	// 乐阑过茄 Sector甫 check茄促.
	//------------------------------------------------------	
	for (currentY=sY2; currentY>=sY1; currentY--)	
	{	
		for (currentX=sX1; currentX<=sX2; currentX++)	
		{				
			const MSector& sector = m_pZone->GetSector(currentX, currentY);

			// 绢恫 Object啊 乐阑 版快
			if (sector.IsExistObject() )			
			{		
				
				//file << "Exist Object!" << endl;

				// Object狼 辆幅俊 蝶扼辑 崔府 check茄促.
				// 急琶且 荐 乐绰 巴篮 Creature客 Item挥捞促.

				//------------------------------------------------
				//
				// Flying Creature啊 乐绰 版快
				//
				//------------------------------------------------
				numCreature = sector.GetFlyingCreatureSize();

				if (numCreature > 0)
				{
					OBJECT_MAP::const_iterator iCreature = sector.GetFlyingCreatureIterator();

					for (int c=0; c<numCreature; c++, iCreature++)
					{
						if (iCreature==sector.GetObjectEnd() 
							|| iCreature->first > MSector::POSITION_FLYINGCREATURE_MAX)
							break;

						pCreature = (MCreature*)iCreature->second;

						// Player捞搁 急琶窍瘤 臼绰促.
						if (pCreature->GetID()!=pid)	
						{
							// FlyingCreature啊 瞒瘤窍绰 康开阑 备茄促.
							// FRAME_PACK俊辑 沥焊甫 佬绢具 且 淀!!
							// FlyingCreature狼 Sector谅钎甫 Pixel谅钎肺 官层具 茄促.

							// FlyingCreature狼 Zone狼 谅钎甫 拌魂茄促.
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// Zone狼 谅钎甫 拳搁狼 谅钎肺 官槽促.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature啊 拳搁俊 粮犁窍绰 康开阑 拌魂秦辰促.	
							frame = m_CreatureFPK[pCreature->GetCreatureFrameID(0)][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// 泅犁 Sprite啊 拳搁俊 免仿登绰 困摹
							objectPixelPoint.x += frame.GetCX();
							objectPixelPoint.y += frame.GetCY() - pCreature->GetZ();//FLYINGCREATURE_HEIGHT;
												// FramePack俊辑 臭捞甫 汲沥窍骨肺,
												// Flying牢 版快档 度鞍促.

							// mouse谅钎啊 Sprite俊 加窍绰瘤 舅酒焊扁
							// Sprite俊辑狼 谅钎啊 祸彬捞 乐绰 版快老锭
							if (m_CreatureSPK[ frame.GetSpriteID() ].IsColorPixel( 
									x - objectPixelPoint.x, 
									y - objectPixelPoint.y 
								))
							{
								return pCreature;
							}				
						}
					}
				}	
			}
		}
	}


	//--------------------------------------------------------------
	//
	// Creature / Item Check data
	//
	//--------------------------------------------------------------
	sY1 = sectorPoint.y - 1,
	sY2 = sectorPoint.y + 3;

	//------------------------------------------------------
	// Zone狼 康开捞 酒囱 版快俊 Skip...
	//------------------------------------------------------	
	if (sY1 < 0)
	{	
		sY1 = 0;	
	}

	if (sY2 >= m_pZone->GetHeight())
	{
		sY2 = m_pZone->GetHeight()-1;
	}

	//------------------------------------------------------
	// Check措惑捞 登绰 ImageObject Iterator
	//------------------------------------------------------
	IMAGEOBJECT_OUTPUT_MAP::reverse_iterator iImageObjectCheck
		= m_mapImageObject.rbegin();

	//------------------------------------------------------
	// 免仿 矫痢捞 sY1焊促 利篮 版快狼 
	// ImageObject甸阑 Check茄促.
	// m_mapImageObject俊绰 Viewpoint啊 key蔼捞 登绢辑 Sort登绢 乐促.
	//------------------------------------------------------
	while (iImageObjectCheck != m_mapImageObject.rend())
	{		
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectCheck).second);

		// InteractionObject牢 版快父 check茄促.
//		if (pImageObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
//		{
//			MInteractionObject* const pInteractionObject = (MInteractionObject* const)pImageObject;
//
//			// 免仿 矫痢捞 sY1焊促 利篮 版快..绰 Check
//			if (pInteractionObject->GetViewpoint() > sY2)
//			{
//				// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
//				objectPixelPoint.x = pInteractionObject->GetPixelX() - m_FirstZonePixel.x;
//				objectPixelPoint.y = pInteractionObject->GetPixelY() - m_FirstZonePixel.y;
//
//				// frame俊 包拌等 sprite甫 备茄促.
//				frame = m_ImageObjectFPK[ pInteractionObject->GetFrameID() ][ pInteractionObject->GetFrame() ];
//				sprite = frame.GetSpriteID();
//						
//				// 谅钎 焊沥
//				objectPixelPoint.x += frame.GetCX();
//				objectPixelPoint.y += frame.GetCY();
//									
//				// InteractionObject绰 (point.x, point.y)俊 免仿登绢 乐阑 巴捞促.
//				// mouse谅钎啊 Sprite俊 加窍绰瘤 舅酒焊扁
//				// Sprite俊辑狼 谅钎啊 祸彬捞 乐绰 版快老锭
//				if (m_ImageObjectSPK[ sprite ].IsColorPixel( 
//						x - objectPixelPoint.x, 
//						y - objectPixelPoint.y 
//					))
//				{	
//					pInteractionObject->SetAction( 1 );
//					return pInteractionObject;
//				}
//			}
//			// 酒聪搁.. Object check且锭 Check秦具登绰 版快捞促.
//			else
//			{
//				break;
//			}
//		}

		iImageObjectCheck ++;
	}

	//------------------------------------------------------	
	// 乐阑过茄 Sector甫 check茄促.
	//------------------------------------------------------	
	for (currentY=sY2; currentY>=sY1; currentY--)	
	{	
		for (currentX=sX1; currentX<=sX2; currentX++)	
		{				
			const MSector& sector = m_pZone->GetSector(currentX, currentY);

			// 绢恫 Object啊 乐阑 版快
			if (sector.IsExistObject() )			
			{		
				
				//file << "Exist Object!" << endl;

				// Object狼 辆幅俊 蝶扼辑 崔府 check茄促.
				// 急琶且 荐 乐绰 巴篮 Creature客 Item挥捞促.

				//------------------------------------------------
				//
				// Flying Creature啊 乐绰 版快
				//
				//------------------------------------------------
				numCreature = sector.GetFlyingCreatureSize();

				if (numCreature > 0)
				{
					OBJECT_MAP::const_iterator iCreature = sector.GetFlyingCreatureIterator();

					for (int c=0; c<numCreature; c++, iCreature++)
					{
						if (iCreature==sector.GetObjectEnd() 
							|| iCreature->first > MSector::POSITION_FLYINGCREATURE_MAX)
							break;

						pCreature = (MCreature*)iCreature->second;

						// Player捞搁 急琶窍瘤 臼绰促.
						if (pCreature->GetID()!=pid)	
						{

							// FlyingCreature啊 瞒瘤窍绰 康开阑 备茄促.
							// FRAME_PACK俊辑 沥焊甫 佬绢具 且 淀!!
							// FlyingCreature狼 Sector谅钎甫 Pixel谅钎肺 官层具 茄促.

							// FlyingCreature狼 Zone狼 谅钎甫 拌魂茄促.
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// Zone狼 谅钎甫 拳搁狼 谅钎肺 官槽促.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature啊 拳搁俊 粮犁窍绰 康开阑 拌魂秦辰促.	
							frame = m_CreatureFPK[pCreature->GetCreatureFrameID(0)][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// 泅犁 Sprite啊 拳搁俊 免仿登绰 困摹
							objectPixelPoint.x += frame.GetCX();
							objectPixelPoint.y += frame.GetCY() - pCreature->GetZ();//FLYINGCREATURE_HEIGHT;
												// FramePack俊辑 臭捞甫 汲沥窍骨肺,
												// Flying牢 版快档 度鞍促.

							// mouse谅钎啊 Sprite俊 加窍绰瘤 舅酒焊扁
							// Sprite俊辑狼 谅钎啊 祸彬捞 乐绰 版快老锭
							if (m_CreatureSPK[ frame.GetSpriteID() ].IsColorPixel( 
									x - objectPixelPoint.x, 
									y - objectPixelPoint.y 
								))
							{
								return pCreature;
							}
						}
					}
				}				
				
				//------------------------------------------------
				//
				// Creature啊 乐绰 版快
				//
				//------------------------------------------------
				numCreature = sector.GetGroundCreatureSize();

				if (numCreature > 0)
				{
					OBJECT_MAP::const_iterator iCreature = sector.GetGroundCreatureIterator();

					for (int c=0; c<numCreature; c++, iCreature++)
					{
						if (iCreature==sector.GetObjectEnd() 
							|| iCreature->first > MSector::POSITION_GROUNDCREATURE_MAX)
							break;

						pCreature = (MCreature*)iCreature->second;

						// Player捞搁 急琶窍瘤 臼绰促.
						if (pCreature->GetID()!=pid)	
						{

							// Creature啊 瞒瘤窍绰 康开阑 备茄促.
							// FRAME_PACK俊辑 沥焊甫 佬绢具 且 淀!!
							// Creature狼 Sector谅钎甫 Pixel谅钎肺 官层具 茄促.

							// Creature狼 Zone狼 谅钎甫 拌魂茄促.
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// Zone狼 谅钎甫 拳搁狼 谅钎肺 官槽促.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature啊 拳搁俊 粮犁窍绰 康开阑 拌魂秦辰促.	
							frame = m_CreatureFPK[pCreature->GetCreatureFrameID(0)][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// 泅犁 Sprite啊 拳搁俊 免仿登绰 困摹
							objectPixelPoint.x += frame.GetCX();
							objectPixelPoint.y += frame.GetCY();

							// mouse谅钎啊 Sprite俊 加窍绰瘤 舅酒焊扁
							// Sprite俊辑狼 谅钎啊 祸彬捞 乐绰 版快老锭
							if (m_CreatureSPK[ frame.GetSpriteID() ].IsColorPixel( 
									x - objectPixelPoint.x, 
									y - objectPixelPoint.y 
								))
							{
								return pCreature;
							}
						}
					}
				}

				//------------------------------------------------
				//
				// Underground Creature啊 乐绰 版快
				//
				//------------------------------------------------
				numCreature = sector.GetUndergroundCreatureSize();

				if (numCreature > 0)
				{
					OBJECT_MAP::const_iterator iCreature = sector.GetUndergroundCreatureIterator();

					for (int c=0; c<numCreature; c++, iCreature++)
					{
						if (iCreature==sector.GetObjectEnd() 
							|| iCreature->first > MSector::POSITION_UNDERGROUNDCREATURE_MAX)
							break;

						pCreature = (MCreature*)iCreature->second;

						// Player捞搁 急琶窍瘤 臼绰促.
						if (pCreature->GetID()!=pid)	
						{

							// Creature啊 瞒瘤窍绰 康开阑 备茄促.
							// FRAME_PACK俊辑 沥焊甫 佬绢具 且 淀!!
							// Creature狼 Sector谅钎甫 Pixel谅钎肺 官层具 茄促.

							// Creature狼 Zone狼 谅钎甫 拌魂茄促.
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// Zone狼 谅钎甫 拳搁狼 谅钎肺 官槽促.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature啊 拳搁俊 粮犁窍绰 康开阑 拌魂秦辰促.	
							// 谅钎 焊沥
							//frame = m_CreatureFPK[pCreature->GetCreatureFrameID()][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// 泅犁 Sprite啊 拳搁俊 免仿登绰 困摹
							//objectPixelPoint.x += frame.GetCX();
							//objectPixelPoint.y += frame.GetCY();

							// mouse谅钎啊 Sprite俊 加窍绰瘤 舅酒焊扁
							// Sprite俊辑狼 谅钎啊 祸彬捞 乐绰 版快老锭
							if (m_EtcSPK[ SPRITEID_CREATURE_BURROW ].IsColorPixel( 
									x - objectPixelPoint.x, 
									y - objectPixelPoint.y 
								))
							{
								return pCreature;
							}
						}
					}
				}
						
				//------------------------------------------------
				//
				// Item捞 乐绰 版快
				//
				//------------------------------------------------				
				pObject = (MObject*)sector.GetItem();

				if (pObject != NULL)
				{	
					//file << "Exist Item!" << endl;

					pItem = (MItem*)pObject;

					// Item啊 瞒瘤窍绰 康开阑 备茄促.
					// FRAME_PACK俊辑 沥焊甫 佬绢具 且 淀!!
					// Item狼 Sector谅钎甫 Pixel谅钎肺 官层具 茄促.

					// Item狼 Zone狼 谅钎甫 拌魂茄促.
					objectPixelPoint = MapToPixel(pItem->GetX(), pItem->GetY());
					objectPixelPoint.x += m_PlusPoint.x;// + pItem->GetSX();
					objectPixelPoint.y += m_PlusPoint.y;// + pItem->GetSY();
						
					// Zone狼 谅钎甫 拳搁狼 谅钎肺 官槽促.
					objectPixelPoint.x -= firstZonePixel.x;
					objectPixelPoint.y -= firstZonePixel.y;

					// Item啊 拳搁俊 粮犁窍绰 康开阑 拌魂秦辰促.				
					frame = m_ItemTileFPK[ pItem->GetTileFrameID() ];

					// 泅犁 Sprite啊 拳搁俊 免仿登绰 困摹
					objectPixelPoint.x += frame.GetCX();
					objectPixelPoint.y += frame.GetCY();

					// mouse谅钎啊 Sprite俊 加窍绰瘤 舅酒焊扁
					// Sprite俊辑狼 谅钎啊 祸彬捞 乐绰 版快老锭
					if (m_ItemTileISPK[ frame.GetSpriteID() ].IsColorPixel( 
							x - objectPixelPoint.x, 
							y - objectPixelPoint.y 
						))
					{
						//file << "Select Item!" << endl;					
						//#ifdef	OUTPUT_DEBUG					
							//sprintf(g_pDebugMessage->GetCurrent(), "SelectItem(%d,%d)", x - objectPixelPoint.x, y - objectPixelPoint.y );
							//g_pDebugMessage->Next()
						//#endif

						return pItem;
					}					
				}				

			}
		}

		//------------------------------------------------------
		// 免仿 矫痢捞 currentY焊促 利篮 版快狼 
		// ImageObject甸阑 Check茄促.
		// m_mapImageObject俊绰 Viewpoint啊 key蔼捞 登绢辑 Sort登绢 乐促.
		//------------------------------------------------------
		while (iImageObjectCheck != m_mapImageObject.rend())
		{
			MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectCheck).second);

			// InteractionObject牢 版快父 check茄促.
//			if (pImageObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
//			{
//				MInteractionObject* const pInteractionObject = (MInteractionObject* const)pImageObject;
//
//				// 免仿 矫痢捞 sY1焊促 利篮 版快..绰 Check
//				if (pInteractionObject->GetViewpoint() == currentY)
//				{
//					// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
//					objectPixelPoint.x = pInteractionObject->GetPixelX() - m_FirstZonePixel.x;
//					objectPixelPoint.y = pInteractionObject->GetPixelY() - m_FirstZonePixel.y;
//
//					// frame俊 包拌等 sprite甫 备茄促.
//					frame = m_ImageObjectFPK[ pInteractionObject->GetFrameID() ][ pInteractionObject->GetFrame() ];
//					sprite = frame.GetSpriteID();
//							
//					// 谅钎 焊沥
//					objectPixelPoint.x += frame.GetCX();
//					objectPixelPoint.y += frame.GetCY();
//										
//					// InteractionObject绰 (point.x, point.y)俊 免仿登绢 乐阑 巴捞促.
//					// mouse谅钎啊 Sprite俊 加窍绰瘤 舅酒焊扁
//					// Sprite俊辑狼 谅钎啊 祸彬捞 乐绰 版快老锭
//					if (m_ImageObjectSPK[ sprite ].IsColorPixel( 
//							x - objectPixelPoint.x, 
//							y - objectPixelPoint.y 
//						))
//					{							
//						return pInteractionObject;
//					}
//				}
//				// 酒聪搁.. Object check且锭 Check秦具登绰 版快捞促.
//				else
//				{
//					break;
//				}
//			}

			iImageObjectCheck ++;
		}
	}	

	//------------------------------------------------------
	// 免仿 矫痢捞 currentY焊促 利篮 版快狼 
	// ImageObject甸阑 Check茄促.
	// m_mapImageObject俊绰 Viewpoint啊 key蔼捞 登绢辑 Sort登绢 乐促.
	//------------------------------------------------------
	while (iImageObjectCheck != m_mapImageObject.rend())
	{
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectCheck).second);

		// InteractionObject牢 版快父 check茄促.
//		if (pImageObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
//		{
//			MInteractionObject* const pInteractionObject = (MInteractionObject* const)pImageObject;
//		
//			// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
//			objectPixelPoint.x = pInteractionObject->GetPixelX() - m_FirstZonePixel.x;
//			objectPixelPoint.y = pInteractionObject->GetPixelY() - m_FirstZonePixel.y;
//
//			// frame俊 包拌等 sprite甫 备茄促.
//			frame = m_ImageObjectFPK[ pInteractionObject->GetFrameID() ][ pInteractionObject->GetFrame() ];
//			sprite = frame.GetSpriteID();
//							
//			// 谅钎 焊沥
//			objectPixelPoint.x += frame.GetCX();
//			objectPixelPoint.y += frame.GetCY();
//									
//			// InteractionObject绰 (point.x, point.y)俊 免仿登绢 乐阑 巴捞促.
//			// mouse谅钎啊 Sprite俊 加窍绰瘤 舅酒焊扁
//			// Sprite俊辑狼 谅钎啊 祸彬捞 乐绰 版快老锭
//			if (m_ImageObjectSPK[ sprite ].IsColorPixel( 
//					x - objectPixelPoint.x, 
//					y - objectPixelPoint.y 
//				))
//			{					
//				return pInteractionObject;
//			}
//		}

		iImageObjectCheck ++;
	}

	return NULL;
}

//----------------------------------------------------------------------
// Clear ItemNameList
//----------------------------------------------------------------------
// 促 瘤况拎具 茄促.
//----------------------------------------------------------------------
void			
MTopView::ClearItemNameList()
{
	DRAWITEMNAME_LIST::iterator iItemName = m_listDrawItemName.begin();

	// 葛电 node甫 瘤款促.
	while (iItemName != m_listDrawItemName.end())
	{
		DRAWITEMNAME_NODE* pNode = *iItemName;

		// 皋葛府俊辑 node力芭
		delete pNode;
		
		iItemName ++;
	}

	m_listDrawItemName.clear();

}

//----------------------------------------------------------------------
// Add ItemName
//----------------------------------------------------------------------
// 寇何俊辑 new甫 秦拎具 茄促.
//
// pNode->GetDistance肺 sort秦辑 眠啊秦具 茄促.
// 坷抚瞒鉴..
//----------------------------------------------------------------------
// item捞 腹篮 版快绰 第俊辑何磐 眉农窍绰霸 狐福促.
// MAX蔼捞 沥秦廉 乐扁 锭巩俊...
// 弊矾唱.. 绊摹扁 蓖满焙.. --;
// 捞芭 摹绰 矫埃俊 绊闷摆促.. 澜衬..
//----------------------------------------------------------------------
void		 	
MTopView::AddItemName(DRAWITEMNAME_NODE* pNode)
{		
	DRAWITEMNAME_LIST::iterator iItemName = m_listDrawItemName.begin();

	BOOL bInsertEnd = TRUE;

	//-------------------------------------------------------
	// 芭府甫 厚背窍搁辑 ItemName阑 眠啊茄促.
	//-------------------------------------------------------
	while (iItemName != m_listDrawItemName.end())
	{
		DRAWITEMNAME_NODE* pOldNode = *iItemName;

		//------------------------------------------
		// 厚背 厚背~
		//------------------------------------------
		if (pNode->GetDistance() < pOldNode->GetDistance())
		{			
			m_listDrawItemName.insert( iItemName, pNode );

			bInsertEnd = FALSE;

			break;
		}
		
		iItemName ++;
	}

	if (bInsertEnd)
	{
		// 盖 场俊 眠啊
		m_listDrawItemName.push_back( pNode );
	}
	
	//-------------------------------------------------------
	// MAX甫 逞绢埃 版快
	//-------------------------------------------------------
	if (m_listDrawItemName.size() > g_pClientConfig->MAX_DRAWITEMNAME)
	{
		// 付瘤阜 吧 力芭茄促.
		DRAWITEMNAME_NODE* pDeleteNode = m_listDrawItemName.back();

		delete pDeleteNode;

		m_listDrawItemName.pop_back();
	}
}

//----------------------------------------------------------------------
// DrawItemNameList
//----------------------------------------------------------------------
// 阿阿狼 Item付促 AddText甫 茄促.
//----------------------------------------------------------------------
void			
MTopView::DrawItemNameList()
{
	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "Start DrawItemNameList" );
	#endif
	

	DRAWITEMNAME_LIST::iterator iItemName = m_listDrawItemName.begin();

	int x, x2;
	int y = 0, y2;

	// minimap阑 弊府绰 版快.. 
	// 谅钎 东矫...
	// 酒贰率何磐 弊赴促..

	// 2001.7.14 林籍贸府
	//if (g_pUserOption->DrawMinimap)// && g_pPlayer->IsSlayer())
	{
		// 快庆庆 窍靛 内爹~~
		y = 160;
	}

	while (iItemName != m_listDrawItemName.end())
	{
		DRAWITEMNAME_NODE* pNode = *iItemName;

		MItem* pItem = pNode->GetItem();
	
		
		const char* pItemName = pItem->GetName();

		// option捞 嘿篮芭绰 颇鄂祸..
		COLORREF nameColor;
		if (pItem->IsSpecialColorItem())
		{
			WORD temp_color;
//			if(pItem->IsQuestItem() )
//				temp_color = CIndexSprite::ColorSet[MItem::GetQuestItemColorset()][16];
//			else
//				temp_color = CIndexSprite::ColorSet[MItem::GetUniqueItemColorset()][16];				
			
			temp_color = CIndexSprite::ColorSet[pItem->GetSpecialColorItemColorset()][16];
			nameColor = RGB(CDirectDraw::Red(temp_color)<<3, CDirectDraw::Green(temp_color)<<3, CDirectDraw::Blue(temp_color)<<3);
		}
		else
		if (pItem->IsEmptyItemOptionList())
		{
			nameColor = m_ColorNameItem;
		}
		else if(pItem->GetItemOptionListCount() ==2)
		{
			nameColor = g_pClientConfig->COLOR_NAME_ITEM_RARE_OPTION;
		}
		// add by Sonic 2006.10.28 增加显示三属性装备为红色
		else if(pItem->GetItemOptionListCount() > 2)
		{
			nameColor = g_pClientConfig->COLOR_NAME_VAMPIRE; //Red
		}
		// end by Sonic 2006.10.28 增加显示三属性装备为红色
		else
		{
			nameColor = m_ColorNameItemOption;
		}

		if (pItemName != NULL)
		{
			// 拳搁 救俊 臂磊啊 甸绢坷档废..
			x = g_GameRect.right - g_GetStringWidth( pItemName, g_ClientPrintInfo[FONTID_ITEM]->hfont );
			x2 = g_GameRect.right;
			
			y2 = y + g_pClientConfig->FONT_ITEM_HEIGHT;

			// 八篮祸 冠胶 康开
			RECT rect = {
				x, 
				y,
				x2,
				y2
			};

			// mouse肺 急琶等 Item牢 版快
			if (m_SelectItemID == pItem->GetID())		
			{
				DRAWTEXT_NODE* pTextNodeBK = new DRAWTEXT_NODE(
										x,
										y+1,
										pItemName,										
										0,		// black
										FONTID_ITEM
				);

				AddText( pTextNodeBK );
		
				DRAWTEXT_NODE* pTextNode = new DRAWTEXT_NODE(
										x+1,
										y+1+1,
										pItemName,										
										nameColor,
										FONTID_ITEM
				);

				AddText( pTextNode );
			}
			else
			{
				DRAWTEXT_NODE* pTextNode = new DRAWTEXT_NODE(
											x,
											y+1,
											pItemName,										
											nameColor,
											FONTID_ITEM
				);

				pTextNode->SetBox( rect, m_ColorBlackHalf );		

				AddText( pTextNode );
			}	

			// item Name狼 急琶 康开
			pNode->SetRect( rect );
		}

		// 促澜..
		iItemName ++;
		y += g_pClientConfig->FONT_ITEM_HEIGHT;
	}

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "End DrawItemNameList" );
	#endif	
	
}

//----------------------------------------------------------------------
// Select Item Name
//----------------------------------------------------------------------
// 拳搁 谅钎(x,y)甫 急琶茄 版快
// 弊 谅钎俊 ItemName捞 乐绰啊?
//----------------------------------------------------------------------
MItem*
MTopView::SelectItemName(int x, int y)
{
	DRAWITEMNAME_LIST::iterator iItemName = m_listDrawItemName.begin();

	while (iItemName != m_listDrawItemName.end())
	{
		DRAWITEMNAME_NODE* pNode = *iItemName;

		//--------------------------------------------
		// ItemName狼 急琶康开俊 加窍搁?
		//--------------------------------------------
		if (pNode->IsPointInRect(x,y))
		{			
			// item pointer甫 逞败霖促.
			// zone俊 绝绰 版快档 乐阑 荐 乐栏骨肺...
			// zone俊辑 佬绢客具 茄促.
			return m_pZone->GetItem( pNode->GetID() );		
		}
	
		// 促澜 item name
		iItemName ++;
	}

	return NULL;
}

//----------------------------------------------------------------------
// Clear TextList
//----------------------------------------------------------------------
// 促 瘤况拎具 茄促.
//----------------------------------------------------------------------
void			
MTopView::ClearTextList()
{
	while (!m_pqDrawText.empty())
	{
		DRAWTEXT_NODE* pNode = m_pqDrawText.top();

		// 皋葛府俊辑 node力芭
		delete pNode;
		
		m_pqDrawText.pop();
	}
}

//----------------------------------------------------------------------
// Add Text
//----------------------------------------------------------------------
// DrawTextList俊 免仿且 string狼 沥焊甫 眠啊茄促.
// 寇何俊辑 new甫 秦拎具 茄促.
//----------------------------------------------------------------------
void			
MTopView::AddText(DRAWTEXT_NODE* pNode)
{
	/*
	const char* str = pNode->GetString();

	if (str==NULL)
	{
		delete pNode;
		return;
	}
	else if (str[0]=='\0')
	{
		delete pNode;
		return;
	}
	*/

	// 唱吝俊 嘛洒霸 窍扁
	if (pNode->GetTextTime()==0)
	{
		QWORD time = ((QWORD)(g_CurrentFrame+160) << 41) 
							+ m_pqDrawText.size();

		pNode->SetTextTime( time );
	}

	m_pqDrawText.push( pNode );
}

//----------------------------------------------------------------------
// DrawTextList
//----------------------------------------------------------------------
// Unlock惑怕俊辑 免仿茄促绊 啊沥茄促.
//----------------------------------------------------------------------
void			
MTopView::DrawTextList()
{
	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "Start DrawTextList" );
	#endif
	
	//DRAWTEXT_PQ::iterator iText = m_pqDrawText.begin();

	//--------------------------------------------------------
	// 3D 啊加 荤侩捞搁...
	// 老窜 肌~~ 绢涤霸.. --;
	// Lock/Unlock阑 代 静扁 困秦辑..
	//
	// 弊繁单! 
	// 臂磊 嘛阑 锭, 官肺 流傈俊 box甫 弊妨拎具 茄促. 氖...
	//--------------------------------------------------------
	/*
	if (CDirect3D::IsHAL())
	{
		m_pSurface->Unlock();

		// 葛电 node甫 瘤款促.
		while (iText != m_pqDrawText.end())
		{
			DRAWTEXT_NODE* pNode = *iText;

			//--------------------------------------------------------
			// 臂磊免仿登绰 何盒俊 八篮祸 惑磊 免仿
			//--------------------------------------------------------
			RECT rect = { pNode->GetX(), pNode->GetY(), 
						pNode->GetXPlusWidth(), pNode->GetY(),15 };
			
			// 八篮 冠胶 免仿..
			DrawBox3D(&rect, 0x7000);			

			iText++;
		}

		m_pSurface->Lock();	
		
		// 促矫 贸澜栏肺..
		iText = m_pqDrawText.begin();
	}
	*/

	//m_pSurface->Unlock();
	//--------------------------------------------------------
	// 葛电 node甫 免仿窍绊 瘤款促.
	//--------------------------------------------------------
	RECT rect2;

	while (!m_pqDrawText.empty())
	{
		DRAWTEXT_NODE* pNode = m_pqDrawText.top();

		//--------------------------------------------------------
		// Box甫 免仿且鳖?
		//--------------------------------------------------------
		if (pNode->IsExistBox())
		{
			//--------------------------------------------------------
			// 臂磊免仿登绰 何盒俊 八篮祸 惑磊 免仿
			//--------------------------------------------------------
			RECT rect = pNode->GetBox();
			
			//--------------------------------------------------------
			// 捧疙茄 冠胶牢 版快
			//--------------------------------------------------------
			if (pNode->IsTransBox())
			{
				//--------------------------------------------------------
				// 3D 啊加狼 版快
				//--------------------------------------------------------
//				if (CDirect3D::IsHAL())
//				{
//					// Unlock / lock... 栏厩~~ 捞芭 矫埃 腹捞 棱酒冈阑扒单..
//					//m_pSurface->Unlock();
//
//					// 冠胶 免仿..
//					#ifdef OUTPUT_DEBUG_DRAW_3DBOX
//						DEBUG_ADD_FORMAT("TextList-Box3D( %d, %d, %d, %d ), color=%x",
//														rect.left, rect.top, rect.right, rect.bottom, pNode->GetBoxColor());
//					#endif
//
//					// 官蹿 彬扁
//					WORD boxColor = pNode->GetBoxColor();
//
//					rect2 = rect;
//					DrawBox3D(&rect2, boxColor);
//
//					if (pNode->IsExistBoxOutline())
//					{
//						// 寇胞 冠胶 弊府扁
//						rect2 = rect;
//						DRAW_CHAT_BOX_OUTLINE( rect2, pNode->GetBoxOutlineColor() )
//
//						
//						// XXXXXX
//						// XXXXXX
//						// .XXXX.
//						// .XXXX.
//						// ..XX..
//						// ..XX..
//
//						rect2.left	= rect.left + CHAT_BOX_TAIL_X;
//						rect2.top	= rect.bottom;
//						rect2.right = rect.left + CHAT_BOX_TAIL_X + 6;
//						rect2.bottom = rect.bottom + 2;		
//						DrawBox3D(&rect2, boxColor);
//
//						rect2.left	= rect.left + CHAT_BOX_TAIL_X+1;
//						rect2.top	= rect.bottom + 2;
//						rect2.right = rect.left + CHAT_BOX_TAIL_X+1 + 4;
//						rect2.bottom = rect.bottom + 2 + 2;
//						DrawBox3D(&rect2, boxColor);
//
//						rect2.left	= rect.left + CHAT_BOX_TAIL_X+2;
//						rect2.top	= rect.bottom + 4;
//						rect2.right = rect.left + CHAT_BOX_TAIL_X+2 + 2;
//						rect2.bottom = rect.bottom + 4 + 2;
//						DrawBox3D(&rect2, boxColor);
//
//						// 急
//						m_pSurface->Lock();
//						DRAW_CHAT_BOX_TAIL_LOCKED( rect, pNode->GetBoxOutlineColor() );
//						m_pSurface->Unlock();
//					}
//
//					//m_pSurface->Lock();
//				}
//				else
				{	
					WORD boxColor = pNode->GetBoxColor();
						
					if (pNode->IsExistBoxOutline())
					{
						WORD color = pNode->GetBoxOutlineColor();

						// 寇胞 冠胶 弊府扁
						rect2 = rect;
						DRAW_CHAT_BOX_OUTLINE( rect2, color )

						// Lock
						m_pSurface->Lock();

						WORD boxColor = pNode->GetBoxColor();

						// 官蹿 弊府扁
						rect2 = rect;
						DRAW_ALPHA_BOX_2D( rect2, boxColor );
						
						// 八沥祸牢 版快
						if (boxColor==0)
						{
							// XXXXXX
							// XXXXXX
							// .XXXX.
							// .XXXX.
							// ..XX..
							// ..XX..

							// 2D 5:6:5
							if (CDirectDraw::Is565())
							{
								rect2 = rect;
								m_pSurface->GammaBox565(&rect2, 15);
								
								rect2.left	= rect.left + CHAT_BOX_TAIL_X;
								rect2.top	= rect.bottom;
								rect2.right = rect.left + CHAT_BOX_TAIL_X + 6;
								rect2.bottom = rect.bottom + 2;								
								m_pSurface->GammaBox565(&rect2, 15);

								rect2.left	= rect.left + CHAT_BOX_TAIL_X+1;
								rect2.top	= rect.bottom + 2;
								rect2.right = rect.left + CHAT_BOX_TAIL_X+1 + 4;
								rect2.bottom = rect.bottom + 2 + 2;
								m_pSurface->GammaBox565(&rect2, 15);

								rect2.left	= rect.left + CHAT_BOX_TAIL_X+2;
								rect2.top	= rect.bottom + 4;
								rect2.right = rect.left + CHAT_BOX_TAIL_X+2 + 2;
								rect2.bottom = rect.bottom + 4 + 2;
								m_pSurface->GammaBox565(&rect2, 15);								
							}
							// 2D 5:5:5
							else
							{
								rect2 = rect;
								m_pSurface->GammaBox555(&rect2, 15);
								
								rect2.left	= rect.left + CHAT_BOX_TAIL_X;
								rect2.top	= rect.bottom;
								rect2.right = rect.left + CHAT_BOX_TAIL_X + 6;
								rect2.bottom = rect.bottom + 2;								
								m_pSurface->GammaBox555(&rect2, 15);

								rect2.left	= rect.left + CHAT_BOX_TAIL_X+1;
								rect2.top	= rect.bottom + 2;
								rect2.right = rect.left + CHAT_BOX_TAIL_X+1 + 4;
								rect2.bottom = rect.bottom + 2 + 2;
								m_pSurface->GammaBox555(&rect2, 15);

								rect2.left	= rect.left + CHAT_BOX_TAIL_X+2;
								rect2.top	= rect.bottom + 4;
								rect2.right = rect.left + CHAT_BOX_TAIL_X+2 + 2;
								rect2.bottom = rect.bottom + 4 + 2;
								m_pSurface->GammaBox555(&rect2, 15);
							}					

							
						}
						// 促弗 祸牢 版快
						else
						{
							m_pSurface->BltColorAlpha(&rect, boxColor, 15);
						}

						// 急
						DRAW_CHAT_BOX_TAIL_LOCKED( rect, color );

						m_pSurface->Unlock();						
					}
					else
					{
						m_pSurface->Lock();

						DRAW_ALPHA_BOX_2D( rect, boxColor );
						
						m_pSurface->Unlock();
					}					
				}
			}
			//--------------------------------------------------------
			// 捧疙茄 冠胶啊 酒囱 版快俊
			//--------------------------------------------------------
			else
			{
				m_pSurface->FillRect(&rect, pNode->GetBoxColor());				
			}
		}

		//--------------------------------------------------------
		// 免仿且 臂磊啊 乐绰 版快..
		//--------------------------------------------------------
		const char* pString = pNode->GetString();

		if (pString!=NULL && pString[0]!='\0')
		{
			//--------------------------------------------------------
			// 免仿 谅钎
			//--------------------------------------------------------
			int x = pNode->GetX();
			int y = pNode->GetY();

			//--------------------------------------------------------
			// font 汲沥
			//--------------------------------------------------------	
			PrintInfo* pPrintInfo = g_ClientPrintInfo[pNode->GetFont()];				

			//--------------------------------------------------------
			// flag俊 蝶弗 贸府档 秦拎具 茄促....				
			//--------------------------------------------------------
			if (pNode->IsOutline())
			{
				// 开惑 color汲沥
				pPrintInfo->text_color = (~pNode->GetColor()) & 0x00FFFFFF;
					
				// 免仿
				g_Print(x-1, y-1, pString, pPrintInfo);
				g_Print(x+1, y+1, pString, pPrintInfo);
			}

			// text color汲沥
			pPrintInfo->text_color = pNode->GetColor();
				
			//--------------------------------------------------------
			// string 免仿
			//--------------------------------------------------------
			g_Print(x, y, pString, pPrintInfo);
		}

		// 皋葛府俊辑 力芭茄促.
		delete pNode;
		
		// 促澜..
		m_pqDrawText.pop();
	}

	// 葛电 node甫 力芭茄促.
	//m_pqDrawText.clear();

	// Lock惑怕
	//m_pSurface->Lock();

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "End DrawTextList" );
	#endif
}

//----------------------------------------------------------------------
// Clear LightBufferFilter
//----------------------------------------------------------------------
// LightBuffer甫 檬扁拳茄促.
// 扁夯利栏肺 Player狼 矫具甫 钎泅秦拎档 等促.
//----------------------------------------------------------------------
void
MTopView::ClearLightBufferFilter3D()
{
	//if (CDirect3D::IsHAL() && 
	//if	(m_DarkBits)
	{
		//--------------------------------------------------
		// 蝴狼 蔼 汲沥
		//--------------------------------------------------
		BYTE DarkColor;// = 0;
		///*
		//int maxLight[MAX_LIGHT3D_SET]	= { 7, 11, 14 };
		//int maxCenter[MAX_LIGHT3D_SET]	= { 3, 6, 9 };
		/*
		switch (m_DarkBits)
		{
			case 0 : DarkColor = 0; break;
			case 1 : DarkColor = 4; break;
			case 2 : DarkColor = 8; break;
			case 3 : DarkColor = 14; break;
			case 4 : DarkColor = 14; break;
		}
		*/

		//DarkColor = m_DarkBits;
		int darkBits = m_DarkBits;
		
		if (g_pPlayer->IsInDarkness() && (!g_pPlayer->HasEffectStatus( EFFECTSTATUS_LIGHTNESS )||g_pZone->GetID() == 3001 ))
		{
			darkBits = 15;
		}

		DarkColor = max(0, darkBits - (g_pPlayer->GetLightSight() - g_pPlayer->GetTimeLightSight()));
	
		//--------------------------------------------------
		// 檬扁拳 - 拳搁 傈眉甫 盲匡 蝴..
		//--------------------------------------------------
		BYTE* pBuffer;

		register int i = SCREENLIGHT_HEIGHT-1;
		do {		
			pBuffer = m_LightBufferFilter.GetFilter( i );
			memset(pBuffer, DarkColor, SCREENLIGHT_WIDTH);				
		} while (i--);

		m_nLight = 0;
	}
}

//----------------------------------------------------------------------
// Clear LightBufferFilter
//----------------------------------------------------------------------
// LightBuffer甫 檬扁拳茄促.
// 扁夯利栏肺 Player狼 矫具甫 钎泅秦拎档 等促.
//----------------------------------------------------------------------
void
MTopView::ClearLightBufferFilter2D()
{
	//if (1)//CDirect3D::IsHAL() && m_b3DLight && m_DarkBits)
	{
		//--------------------------------------------------
		// 蝴狼 蔼 汲沥
		//--------------------------------------------------
		BYTE DarkColor;// = 0;
		///*
		//int maxLight[MAX_LIGHT3D_SET]	= { 7, 11, 14 };
		//int maxCenter[MAX_LIGHT3D_SET]	= { 3, 6, 9 };
		/*
		switch (m_DarkBits)
		{
			case 0 : DarkColor = 31; break;		// 31-0
			case 1 : DarkColor = 23; break;	// 31-4*2
			case 2 : DarkColor = 15; break;	// 31-8*2
			case 3 : DarkColor = 5; break;	// 31-14*2
			case 4 : DarkColor = 5; break;	// 31-14*2
		}
		*/
		int darkBits = m_DarkBits;
		
		if (g_pPlayer->IsInDarkness() && (!g_pPlayer->HasEffectStatus( EFFECTSTATUS_LIGHTNESS) ||g_pZone->GetID() == 3001))
		{
			darkBits = 15;
		}
	
		DarkColor = 31 - (darkBits<<1);

		DarkColor = max(0, DarkColor - ((g_pPlayer->GetLightSight() - g_pPlayer->GetTimeLightSight())<<1));

		//--------------------------------------------------
		// 檬扁拳 - 拳搁 傈眉甫 盲匡 蝴..
		//--------------------------------------------------
		BYTE* pBuffer;

		register int i = SCREENLIGHT_HEIGHT-1;
		do {		
			pBuffer = m_LightBufferFilter.GetFilter( i );
			memset(pBuffer, DarkColor, SCREENLIGHT_WIDTH);				
		} while (i--);

		m_nLight = 0;
	}
}

//----------------------------------------------------------------------
// Add LightFilter To Screen
//----------------------------------------------------------------------
// LightFilter甫 眠啊茄促.
// Screen谅钎甫 LightBuffer俊 嘎档废 官层具茄促.
//
// (CLIPSURFACE_WIDTH, CLIPSURFACE_WIDTH) 
//  --> (SCREENLIGHT_WIDTH, SCREENLIGHT_HEIGHT)肺..
//----------------------------------------------------------------------
void			
MTopView::AddLightFilter2D(int x, int y, BYTE range, bool bMapPixel, bool bForceLight)
{
	if (!bForceLight)
	{
		range = min( max(g_pPlayer->GetSight()-3, 0), range );
	}

	if (//CDirect3D::IsHAL() && m_b3DLight && 
		!g_pPlayer->IsInDarkness()
		&& range!=0 
		&& (m_DarkBits || g_pPlayer->IsInDarkness()) 
		&& m_nLight < g_pClientConfig->MAX_LIGHT_DRAW
		|| bForceLight) // 公炼扒 免仿秦具窍绰 蝴
	{
		// tile 谅钎 焊沥 
		 //x += g_TILE_X_HALF;
		 //y += g_TILE_Y_HALF;

		// Light Filter狼 裹困甫 逞瘤 臼档废 茄促.
		if (range >= m_LightFTP.GetSize())
		{
			range = m_LightFTP.GetSize()-1;
		}		

		//-----------------------------------------------------
		// range俊 蝶扼辑 免仿且 lightFilter甫 搬沥秦具茄促.
		//-----------------------------------------------------		
		int filterID = range;

		//-----------------------------------------------------
		// LightFilter 免仿 矫痢 搬沥
		/// 弊府绊, filter谅钎 (fx,fy)肺 官层具 窍绊..
		// filter狼 吝缴阑 (x,y)俊 坷档废 嘎苗具 茄促.
		//-----------------------------------------------------
		int fx, fy;

		// (x,y)啊 绢恫 谅钎牢瘤俊 蝶扼辑 拌魂阑 促福霸 秦霖促.
		if (bMapPixel)
		{
			// mapPixel --> screen
			POINT screenPoint = PixelToScreen(x,y);	
			
			// screen -> filter
			fx = (float)screenPoint.x / s_LightWidth;
			fy = (float)screenPoint.y / s_LightHeight;
		}
		else
		{
			// screen -> filter
			fx = (float)x / s_LightWidth;
			fy = (float)y / s_LightHeight;
		}

		// filter 吝缴 嘎眠扁
		fx = fx - (m_LightFTP[filterID].GetWidth() >> 1);
		fy = fy - (m_LightFTP[filterID].GetHeight() >> 1);
			
		//-----------------------------------------------------
		// BufferFilter俊 Light filter甫 眠啊茄促.
		//-----------------------------------------------------
		m_LightBufferFilter.BltFilterAdd( fx, fy, m_LightFTP[filterID] );		

		m_nLight++;
	}
}

//----------------------------------------------------------------------
// Add LightFilter To Screen
//----------------------------------------------------------------------
// LightFilter甫 眠啊茄促.
// Screen谅钎甫 LightBuffer俊 嘎档废 官层具茄促.
//
// (CLIPSURFACE_WIDTH, CLIPSURFACE_WIDTH) 
//  --> (SCREENLIGHT_WIDTH, SCREENLIGHT_HEIGHT)肺..
//----------------------------------------------------------------------
void			
MTopView::AddLightFilter3D(int x, int y, BYTE range, bool bMapPixel, bool bForceLight)
{
	if (!bForceLight)
	{
		range = min( max(g_pPlayer->GetSight()-3, 0), range );
	}
		
	if (//CDirect3D::IsHAL() && 
		!g_pPlayer->IsInDarkness()
		&& range!=0
		&&	(m_DarkBits	|| g_pPlayer->IsInDarkness()) 
		&& m_nLight < g_pClientConfig->MAX_LIGHT_DRAW
		|| bForceLight)	// 公炼扒 免仿秦具窍绰 蝴
	{
		// tile 谅钎 焊沥 
		//x += g_TILE_X_HALF;
		//y += g_TILE_Y_HALF;

		// Light Filter狼 裹困甫 逞瘤 臼档废 茄促.
		if (range >= m_LightFTP.GetSize())
		{
			range = m_LightFTP.GetSize()-1;
		}		

		//-----------------------------------------------------
		// range俊 蝶扼辑 免仿且 lightFilter甫 搬沥秦具茄促.
		//-----------------------------------------------------		
		int filterID = range;

		//-----------------------------------------------------
		// LightFilter 免仿 矫痢 搬沥
		/// 弊府绊, filter谅钎 (fx,fy)肺 官层具 窍绊..
		// filter狼 吝缴阑 (x,y)俊 坷档废 嘎苗具 茄促.
		//-----------------------------------------------------
		int fx, fy;

		// (x,y)啊 绢恫 谅钎牢瘤俊 蝶扼辑 拌魂阑 促福霸 秦霖促.
		if (bMapPixel)
		{
			// mapPixel --> screen
			POINT screenPoint = PixelToScreen(x,y);	
			
			// screen -> filter
			fx = (float)screenPoint.x / s_LightWidth;
			fy = (float)screenPoint.y / s_LightHeight;
		}
		else
		{
			// screen -> filter
			fx = (float)x / s_LightWidth;
			fy = (float)y / s_LightHeight;
		}

		// filter 吝缴 嘎眠扁
		fx = fx - (m_LightFTP[filterID].GetWidth() >> 1);
		fy = fy - (m_LightFTP[filterID].GetHeight() >> 1);
			
		//-----------------------------------------------------
		// BufferFilter俊 Light filter甫 眠啊茄促.
		//-----------------------------------------------------
		m_LightBufferFilter.BltFilterSub( fx, fy, m_LightFTP[filterID] );

		m_nLight++;
	}
}

//----------------------------------------------------------------------
// Add CreatureOutput All
//----------------------------------------------------------------------
// m_pZone狼 Creature map阑 烹掳肺~ 眠啊
//----------------------------------------------------------------------
// OutputMap俊辑狼 ID绰 Creature狼..
//
//    2 bytes + 2 bytes    = 4 bytes 肺 备己登绢 乐促.
// [pixel y谅钎][object id]
//
// 老馆 creature绰 眠啊且瘤 富瘤甫 箭磊肺 搬沥窍绊..
// FakeCreature绰 tile俊绰 眠啊救登骨肺.. 公炼扒 咯扁辑 眠啊秦具茄促.
//----------------------------------------------------------------------
void	
MTopView::AddOutputCreatureAll()
{
	// 檬扁拳
	m_mapCreature.clear();

	// [货扁贱3] 包 加俊 乐阑锭绰 磊扁父 免仿窍霸 茄促.
	if (g_pPlayer->IsInCasket())
	{
		m_bTileSearchForCreature = false;	

		QWORD key = GetOutputCreatureID(g_pPlayer);

		// (key蔼, pCreature)甫 眠啊茄促.
		m_mapCreature.insert(
			CREATURE_OUTPUT_MAP::value_type( key, g_pPlayer )
		);

		return;
	}


	MZone::CREATURE_MAP::const_iterator iCreature;

	MCreature* pCreature;

	int num = m_pZone->GetCreatureNumber();

	// 20疙 捞惑 乐栏搁 ... 
	// 弊成 八祸窍绰霸 炒促绊 魄窜.. 沥富老鳖? - -;
	if (num >= 20)
	{
//		m_bTileSearchForCreature = true;		
		m_bTileSearchForCreature = false;		
	}
	else
	{
		m_bTileSearchForCreature = false;
	}
	
	//---------------------------------------------------------
	// Tile阑 八祸秦辑 免仿窍绰 版快啊 酒聪搁..
	// 葛电 Creature甫 OutputMap俊 眠啊秦具 茄促.
	//---------------------------------------------------------
	if (!m_bTileSearchForCreature)
	{
		iCreature = m_pZone->GetCreatureBegin();

		for (int i=0; i<num; i++)
		{
			pCreature = iCreature->second;

			// key 蔼 = [pixel Y谅钎][object id]
			QWORD key = GetOutputCreatureID(pCreature);

			// (key蔼, pCreature)甫 眠啊茄促.
			m_mapCreature.insert(
				CREATURE_OUTPUT_MAP::value_type( key, pCreature )
			);
		
			// 促澜 Creature
			iCreature++;
		}
	}

	//---------------------------------------------------------
	// Fake Creature
	//---------------------------------------------------------
	iCreature = m_pZone->GetFakeCreatureBegin();
	num = m_pZone->GetFakeCreatureNumber();
	
	//---------------------------------------------------------
	// 葛电 Creature甫 OutputMap俊 眠啊秦具 茄促.
	//---------------------------------------------------------
	for (int i=0; i<num; i++)
	{
		pCreature = iCreature->second;

		// key 蔼 = [pixel Y谅钎][object id]
		QWORD key = GetOutputCreatureID(pCreature);

		// (key蔼, pCreature)甫 眠啊茄促.
		m_mapCreature.insert(
			CREATURE_OUTPUT_MAP::value_type( key, pCreature )
		);
	
		// 促澜 Creature
		iCreature++;
	}
}

//----------------------------------------------------------------------
// Add OutputCreature
//----------------------------------------------------------------------
// pCreature甫 眠啊茄促.
//----------------------------------------------------------------------
bool	
MTopView::AddOutputCreature(MCreature* pCreature)
{	
	// key蔼 积己
	QWORD key = GetOutputCreatureID(pCreature);

	// 捞固 乐绰瘤 犬牢秦焊绊 绝栏搁 眠啊.
	if (m_mapCreature.find( key ) == m_mapCreature.end())
	{			
		// (key蔼, pCreature)甫 眠啊茄促.
		m_mapCreature.insert(
			CREATURE_OUTPUT_MAP::value_type( key, pCreature )
		);

		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// Remove OutputCreature
//----------------------------------------------------------------------
// id客 包访乐绰 creature甫 力芭茄促.
//----------------------------------------------------------------------
bool	
MTopView::RemoveOutputCreature(MCreature* pCreature)
{
	// 捞固 乐绰瘤 犬牢秦焊绊 乐栏搁 力芭
	CREATURE_OUTPUT_MAP::iterator iCreature 
		= m_mapCreature.find( GetOutputCreatureID(pCreature) );

	if (iCreature	!= m_mapCreature.end())
	{			
		m_mapCreature.erase( iCreature );

		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// ID啊 id牢 Creature狼 谅钎啊 y0俊辑 y1栏肺 官诧吧 update茄促.
//----------------------------------------------------------------------
// id客 包访乐绰 creature狼 沥焊(免仿谅钎包访id)甫 Update茄促.
// 乐栏搁.. 力芭窍绊.. 促矫 眠啊秦具 茄促.
//----------------------------------------------------------------------
bool			
MTopView::UpdateOutputCreature(TYPE_OBJECTID id, int y0, int y1)
{
	// 捞固 乐绰瘤 犬牢秦焊绊 乐栏搁 update
	CREATURE_OUTPUT_MAP::iterator iCreature 
		= m_mapCreature.find( GetOutputCreatureID(id, y0) );

	if (iCreature != m_mapCreature.end())
	{			
		MCreature* pCreature = (*iCreature).second;

		// 力芭
		m_mapCreature.erase( iCreature );

		// (key蔼, pCreature)甫 促矫 眠啊茄促.
		m_mapCreature.insert(
			CREATURE_OUTPUT_MAP::value_type( 
					GetOutputCreatureID(id, y1),	// key蔼
					pCreature 
			)
		);

		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// DrawBox3D
//----------------------------------------------------------------------
// 八篮祸 馆捧疙 冠胶甫 弊赴促. 3D
//----------------------------------------------------------------------
//void
//MTopView::DrawBox3D(RECT* pRect, WORD pixel)
//{
//	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
//		DEBUG_ADD( "Start DrawBox3D" );
//	#endif
//
//
//	CSpriteSurface* pUsingSurface = NULL;
//
//	//-------------------------------------------------
//	// Color Black Bar
//	//-------------------------------------------------
//	if (pixel==m_ColorBlackHalf)
//	{
//		pUsingSurface = m_p3DBoxBlackSurface;
//	}
//	//-------------------------------------------------
//	// Color HP Bar
//	//-------------------------------------------------
//	else if (pixel==m_ColorHPBar)
//	{
//		pUsingSurface = m_p3DBoxHPSurface;
//	}
//	//-------------------------------------------------
//	// 促弗 祸彬老 版快.. 促矫 汲沥..
//	//------------------------------------------------		
//	else
//	{		
//		pUsingSurface = m_p3DBoxSurface;
//
//		if (pixel != m_3DBoxCurrentPixel)
//		{
//			Set3DBoxColor( pixel );
//
//			//m_3DBoxCurrentPixel = pixel;
//		}
//	}
//	
//	DRAW_TEXTURE_SURFACE( pUsingSurface, pRect )
//	else
//	{
//		#ifdef OUTPUT_DEBUG_DRAW_PROCESS
//			DEBUG_ADD( "Failed DrawBox3D" );
//		#endif	
//	}
//
//	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
//		DEBUG_ADD( "End DrawBox3D" );
//	#endif
//}

//----------------------------------------------------------------------
// Draw Alpha Box
//----------------------------------------------------------------------
// 2D/3D啊加俊 包拌绝捞...
// 泅犁狼 Lock/Unlock 惑怕俊 包拌绝捞... 累悼茄促.
//
// m_pSurface俊 
// (r,g,b)客 alpha蔼(1~31)俊 蝶扼辑 pRect康开俊 捧疙茄 box甫 弊赴促.
//----------------------------------------------------------------------
void
MTopView::DrawAlphaBox(RECT* pRect, BYTE r, BYTE g, BYTE b, BYTE alpha)
{
	WORD color;

	//--------------------------------------------------------
	// Win2000牢 版快.. 咯扁辑 决没抄 加档历窍啊 积变促.. 恐 弊凡鳖?
	// 老窜篮.. AlphaBox甫 荤侩窍瘤 臼绰促.
	// 抛胶飘 搬苞.. 捞霸 巩力啊 酒聪菌促... 构啊 巩力老鳖. - -;
	//--------------------------------------------------------
	/*
	if (g_WindowsNT)
	{
		//------------------------------------------------
		// Unlock 惑怕肺 父电促.
		//------------------------------------------------
		BOOL bLock = m_pSurface->IsLock();
		if (bLock)
		{
			m_pSurface->Unlock();
		}
		
		color = CDirectDraw::Color(r,g,b);

		m_pSurface->FillRect(pRect, color);

		//------------------------------------------------
		// 盔贰 lock 等 惑怕搁 促矫 登倒赴促.
		//------------------------------------------------
		if (bLock)
		{
			m_pSurface->Lock();
		}
	}
	*/
	//--------------------------------------------------------
	// 98牢 版快..
	//--------------------------------------------------------
	//else
	{
		//--------------------------------------------------------
		// 3D 啊加狼 版快
		//--------------------------------------------------------
//		if (CDirect3D::IsHAL())
//		{
//			//------------------------------------------------
//			// Unlock 惑怕肺 父电促.
//			//------------------------------------------------
//			BOOL bLock = m_pSurface->IsLock();
//			if (bLock)
//			{
//				m_pSurface->Unlock();
//			}
//
//			//------------------------------------------------
//			// 4:4:4:4
//			//------------------------------------------------
//			//						1 1110b
//			WORD color = ((32-alpha) & 0x1E) << 11;		// ((32-alpha)>>1) << 12
//
//			color = color 
//					| (((WORD)r>>1) << 8)
//					| (((WORD)g>>1) << 4)
//					| ((WORD)b>>1);
//
//			
//			CSpriteSurface* pUsingSurface = NULL;
//
//			//-------------------------------------------------
//			// Color Black Bar
//			//-------------------------------------------------
//			if (color==m_ColorBlackHalf)
//			{
//				pUsingSurface = m_p3DBoxBlackSurface;
//			}
//			//-------------------------------------------------
//			// Color HP Bar
//			//-------------------------------------------------
//			else if (color==m_ColorHPBar)
//			{
//				pUsingSurface = m_p3DBoxHPSurface;
//			}
//			//-------------------------------------------------
//			// 促弗 祸彬老 版快.. 促矫 汲沥..
//			//------------------------------------------------		
//			else
//			{		
//				pUsingSurface = m_p3DBoxSurface;
//
//				if (color != m_3DBoxCurrentPixel)
//				{
//					Set3DBoxColor( color );
//
//					//m_3DBoxCurrentPixel = pixel;
//				}
//			}
//
//			DRAW_TEXTURE_SURFACE( pUsingSurface, pRect )			
//
//			//------------------------------------------------
//			// 盔贰 lock 等 惑怕搁 促矫 登倒赴促.
//			//------------------------------------------------
//			if (bLock)
//			{
//				m_pSurface->Lock();
//			}
//		}
//		//--------------------------------------------------------
//		// 2D
//		//--------------------------------------------------------
//		else
		{
			//------------------------------------------------
			// Lock 惑怕肺 父电促.
			//------------------------------------------------
			BOOL bUnlock = !m_pSurface->IsLock();
			if (bUnlock)
			{
				m_pSurface->Lock();
			}

			color = CDirectDraw::Color(r,g,b);

			//-------------------------------------------------
			// 八沥祸捞搁.. 奖霸 等促~
			//-------------------------------------------------
			if (color==0)
			{
				// 2D 5:6:5
				if (CDirectDraw::Is565())
				{
					m_pSurface->GammaBox565(pRect, alpha);
				}
				// 2D 5:5:5
				else
				{
					m_pSurface->GammaBox555(pRect, alpha);
				}
			}
			//-------------------------------------------------
			// 酒聪搁...
			//-------------------------------------------------
			else
			{
				m_pSurface->BltColorAlpha(pRect, color, alpha);
			}

			//------------------------------------------------
			// 盔贰狼 Lock 惑怕肺 登倒赴促.
			//------------------------------------------------
			if (bUnlock)
			{
				m_pSurface->Unlock();
			}
		}
	}
}

//----------------------------------------------------------------------
// Draw Information
//----------------------------------------------------------------------
// Debug俊 鞘夸茄 沥焊甫 免仿茄促.
//
// 臂磊 免仿且锭绰 Unlock惑怕..
//----------------------------------------------------------------------
void
MTopView::DrawInformation()
{
	if(g_pEventManager->GetEventByFlag(EVENTFLAG_NOT_DRAW_INFORMATION))
		return;

	char str[128];

	//-----------------------------------------------------------------
	//							Lock
	//-----------------------------------------------------------------	
	//if (!m_pSurface->Lock()) return;
	// Surface狼 沥焊甫 历厘秦敌促.
	//S_SURFACEINFO		SurfaceInfo;
	//SetSurfaceInfo(&SurfaceInfo, m_pSurface->GetDDSD());

	//-----------------------------------------------------------------
	// Font 急琶
	//-----------------------------------------------------------------
	PrintInfo* pPrintInfo = g_ClientPrintInfo[FONTID_INFO];	
	
	int y;//2;

	if (g_pPlayer->IsSlayer())
	{
		y = 45;
	}
	else
	{
		y = 48;
	}


	//-----------------------------------------------------------------
	// [ TEST CODE ]
	//-----------------------------------------------------------------
	// Zone捞抚 免仿
	//-----------------------------------------------------------------
	/* // 2001.7.14 林籍贸府
	if (g_pUserOption->DrawZoneName)
	{
		if (g_bZonePlayerInLarge)
		{
			sprintf(str, (*g_pGameStringTable)[STRING_DRAW_ZONE_NAME].GetString(),
							g_pZoneTable->Get( g_nZoneLarge )->Name.GetString(),
							g_pPlayer->GetX(), g_pPlayer->GetY());		
		}
		else
		{
			sprintf(str, (*g_pGameStringTable)[STRING_DRAW_ZONE_NAME].GetString(),
							g_pZoneTable->Get( g_nZoneSmall )->Name.GetString(),
							g_pPlayer->GetX(), g_pPlayer->GetY());
		}
		const COLORREF color	= RGB(18<<3, 14<<3, 30<<3);	//CDirectDraw::Color(18,14,30);
		const COLORREF bkcolor = RGB(0, 0, 0);				//CDirectDraw::Color(0,0,0);

		pPrintInfo->text_color	= bkcolor;
		g_Print(6, y+1, str, pPrintInfo);
		
		pPrintInfo->text_color	= color;
		g_Print(5, y, str, pPrintInfo);

		y += 18;
	}
	*/
	

	//-----------------------------------------------------------------
	// 霸烙 矫埃 免仿
	//-----------------------------------------------------------------
	/*
	static DWORD lastDisplayGameTime = g_CurrentTime;

	if (g_pUserOption->DrawGameTime)
	{
		//static WORD dayColor = CDirectDraw::Color(5,2,24);
		const COLORREF timeColor	= RGB(12<<3, 8<<3, 25<<3);	//CDirectDraw::Color(12,8,25);
		const COLORREF timeBkColor = RGB(0, 0, 0);				//CDirectDraw::Color(0,0,0);

		g_pGameTime->SetCurrentTime( g_CurrentTime );
	

		sprintf(str, (*g_pGameStringTable)[STRING_DRAW_GAME_TIME].GetString(),
						g_pGameTime->GetHour(),
						g_pGameTime->GetMinute(),
						g_pGameTime->GetSecond()
				);

		pPrintInfo->text_color = timeBkColor;
		g_Print(6, y+1, str, pPrintInfo);
		
		pPrintInfo->text_color = timeColor;
		g_Print(5, y, str, pPrintInfo);

		lastDisplayGameTime = g_CurrentTime;

		y += 18;
	}
	else
	{
		// 30檬俊 茄锅究篮 矫埃阑 官层霖促.
		if (g_CurrentTime - lastDisplayGameTime > 30000)
		{
			g_pGameTime->SetCurrentTime( g_CurrentTime );

			lastDisplayGameTime = g_CurrentTime;
		}
	}
	*/

	//-----------------------------------------------------------------
	//
	// Item 何辑柳芭 免仿
	//
	//-----------------------------------------------------------------
	y += 10;
	DrawItemBroken(5, y);

	y += 70;	// ItemBroken狼 农扁.. 蓖满酒辑 扁成.. せせ

	//-----------------------------------------------------------------
	//
	// Event Message 免仿
	//
	//-----------------------------------------------------------------
	int strY = 30;
	int strX = g_GameRect.right /2;

	// Event String 免仿
	DrawEventString( strX, strY );

	//-----------------------------------------------------------------
	//
	// System Message 免仿
	//
	//-----------------------------------------------------------------

	strY = y+20;
	for (int c=0; c<g_pSystemMessage->GetSize(); c++)
	{
		if ((*g_pSystemMessage)[c][0] != NULL)
		{
			const COLORREF color = RGB(29<<3, 8<<3, 12<<3);	//CDirectDraw::Color(29,8,12);

			pPrintInfo->text_color = 0;
			
			g_Print(11, strY+1, (*g_pSystemMessage)[c], pPrintInfo);

			pPrintInfo->text_color = color;
			g_Print(10, strY, (*g_pSystemMessage)[c], pPrintInfo);

			strY+=20;
		}		
	}
	
	//-----------------------------------------------------------------
	//
	// Player Message
	//
	//-----------------------------------------------------------------
	// new version

	int x = 10;//((g_GameRect.right /2) /2) - 50;
	strY = 40;
	int iColorType = 0;
	char* pColorType=NULL;
	char message[300];
	for (c=0; c<g_pPlayerMessage->GetSize(); c++)
	{
		if ((*g_pPlayerMessage)[c][0] != NULL)
		{
			// 取出信息
			strcpy(message,(*g_pPlayerMessage)[c]);
			// 取出颜色类型
			pColorType = &message[strlen(message)-1];
			iColorType = atoi(pColorType);
			pColorType[0]=NULL;
			COLORREF color = RGB(20<<3,31<<3,12<<3);
			switch(iColorType)
			{
			case 0: // 绿色
				color = RGB_GREEN;
				break;
			case 1: // 蓝色
				color = RGB(50,50,200);//RGB_BLUE;
				break;
			case 2: // 黄色
				color = RGB_YELLOW;
				break;
			}
			
			pPrintInfo->text_color = 0;
			g_Print(x+1, strY+1, message, pPrintInfo);

			pPrintInfo->text_color = color;
			g_Print(x, strY, message, pPrintInfo);

			strY+=20;
		}		
	}

	PrintInfo* pNoticeInfo = g_ClientPrintInfo[FONTID_LARGE_CHAT];	
	
	
	for( c = 0; c< g_pNoticeMessage->GetSize(); c++ )
	{
		if( (*g_pNoticeMessage)[c][0] != NULL )
		{
			const COLORREF color = RGB(20<<3,31<<3,12<<3);

			pNoticeInfo->text_color = 0;
			g_Print(11, strY+1, (*g_pNoticeMessage)[c], pNoticeInfo );
			pNoticeInfo->text_color = color;
			g_Print(10, strY, (*g_pNoticeMessage)[c], pNoticeInfo );
			strY += 16;
		}
	}

	// 5檬付促 茄锅究.. scroll
	static DWORD lastTime = g_CurrentTime;
	if (g_CurrentTime - lastTime >= g_pClientConfig->DELAY_SYSTEMMESSAGE)
	{
		g_pSystemMessage->Add("\0");		
		lastTime = g_CurrentTime;
	}

	// 7檬付促 茄锅究.. scroll
	static DWORD lastNoticeTime = g_CurrentTime;
	if (g_CurrentTime - lastNoticeTime >= (g_pClientConfig->DELAY_SYSTEMMESSAGE + 2000) )
	{
		g_pNoticeMessage->Add("\0");
		lastNoticeTime = g_CurrentTime;
	}

	// 7檬付促 茄锅究.. scroll
	static DWORD lastPlayerTimer = g_CurrentTime;
	if (g_CurrentTime - lastPlayerTimer >= (g_pClientConfig->DELAY_SYSTEMMESSAGE + 10000) )
	{
		g_pPlayerMessage->Add("\0");		
		lastPlayerTimer = g_CurrentTime;
	}
	//-----------------------------------------------------------------
	//
	// Game Message 免仿
	//
	//-----------------------------------------------------------------
	switch(g_pPlayer->GetRace())
	{
	case RACE_SLAYER:
		strX = 10;
		strY = 410;
		if (g_pPlayer->GetBonusPoint()!=0)
		{
			// bonus point 棵副妨绊 穿福绰 滚瓢捞 乐绰 困摹
			strX = 85;
		}	
		if (g_MyFull)
		{
			strY = 578;
		}
		break;

	case RACE_VAMPIRE:
		strX = 10;
		strY = 440;
		if (g_pPlayer->GetBonusPoint()!=0)
		{
			// bonus point 棵副妨绊 穿福绰 滚瓢捞 乐绰 困摹
			strX = 85;
		}
		if (g_MyFull)
		{
			strY = 608;
		}	
		break;

	case RACE_OUSTERS:
		strX = 10;
		strY = 410;
		if (g_pPlayer->GetBonusPoint()!=0)
		{
			// bonus point 棵副妨绊 穿福绰 滚瓢捞 乐绰 困摹
			strX = 85;
		}
		if (g_MyFull)
		{
			strY = 578;
		}	
		break;
	}

	for (c=g_pGameMessage->GetSize()-1; c>=0; c--)
	{
		if ((*g_pGameMessage)[c][0] != NULL)
		{
			const COLORREF color = RGB(8<<3, 28<<3, 8<<3);	//CDirectDraw::Color(29,8,12);

			pPrintInfo->text_color = 0;
			g_Print(strX+1, strY+1, (*g_pGameMessage)[c], pPrintInfo);

			pPrintInfo->text_color = color;
			g_Print(strX, strY, (*g_pGameMessage)[c], pPrintInfo);

			strY-=20;
		}		
	}

	// 5檬付促 茄锅究.. scroll
	static DWORD gamelastTime = g_CurrentTime;
	if (g_CurrentTime - gamelastTime >= g_pClientConfig->DELAY_GAMEMESSAGE)
	{
		g_pGameMessage->Add("\0");
		gamelastTime = g_CurrentTime;
	}

	//-----------------------------------------------------------------
	//
	// 醚舅 巢篮芭 免仿
	//
	//-----------------------------------------------------------------
	if (g_pCurrentMagazine!=NULL)
	{
		POINT point = { 225, 9 };

		m_pSurface->Lock();
		m_pSurface->BltSprite(&point, &m_EtcSPK[SPRITEID_BULLET]);
		m_pSurface->Unlock();

		point.x += m_EtcSPK[SPRITEID_BULLET].GetWidth() + 2;
		point.y += m_EtcSPK[SPRITEID_BULLET].GetHeight() - 18;

		sprintf(str, "%d", g_pCurrentMagazine->GetNumber());

		const COLORREF bulletColor = RGB(30<<3, 28<<3, 30<<3);

		pPrintInfo->text_color = 0;		
		g_Print(point.x+1, point.y+1, str, pPrintInfo);

		pPrintInfo->text_color = bulletColor;		
		g_Print(point.x, point.y, str, pPrintInfo);
	}

	//-----------------------------------------------------------------
	// 某腐磐 惑怕 免仿
	//-----------------------------------------------------------------
	#ifdef OUTPUT_DEBUG
		if (g_pDXInput->KeyDown(DIK_P) && 
			(g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL)))
		{
			const int statusNum = 3;
			char statusName[statusNum][5] =
			{
				"STR",
				"DEX",
				"INT"
			};

			int statusValue[statusNum];

			statusValue[0] = g_pPlayer->GetSTR();
			statusValue[1] = g_pPlayer->GetDEX();
			statusValue[2] = g_pPlayer->GetINT();

			const COLORREF color = RGB(8<<3, 28<<3, 8<<3);	//CDirectDraw::Color(29,8,12);


			strY = 50;

			for (int status=0; status<statusNum; status++)
			{
				sprintf(str, "%s = %d", statusName[status], statusValue[status]);

				pPrintInfo->text_color = 0;
				g_Print(701, strY+1, str, pPrintInfo);

				pPrintInfo->text_color = color;
				g_Print(700, strY, str, pPrintInfo);

				strY+=20;
			}
		}

	#endif

	//-----------------------------------------------------------------
	//							UnLock
	//-----------------------------------------------------------------
	//m_pSurface->Unlock();
}

#if defined(OUTPUT_DEBUG) && defined(_LIB)
	extern int g_SurfaceCount;
	extern int g_EffectCount;
#endif
	
void
MTopView::DrawEventString(int& strX, int& strY)
{
	char str[256] = {0,};
	char str2[256] = {0,};
	
	// Delay String
	PrintInfo* pPrintInfo = g_ClientPrintInfo[FONTID_INFO];	

	const MEvent *event = g_pEventManager->GetEventByFlag(EVENTFLAG_SHOW_DELAY_STRING, 0);
	
	if(event != NULL)
	{
		for(int eventMessageCount = 0; eventMessageCount < (event->m_StringsID.empty()?1:event->m_StringsID.size()); eventMessageCount++)
		{
			str[0] = NULL;
			
			switch(event->eventFlag & EVENTFLAG_SHOW_DELAY_STRING)
			{
			case EVENTFLAG_SHOW_STRING:
				if(!event->m_StringsID.empty())
				{
					if(event->m_StringsID[eventMessageCount] < MAX_GAME_STRING && (*g_pGameStringTable)[event->m_StringsID[eventMessageCount]].GetString() != NULL)
						strcpy(str, (*g_pGameStringTable)[event->m_StringsID[eventMessageCount]].GetString());
				}
				break;
				
			case EVENTFLAG_SHOW_DELAY:
				sprintf(str, "%d", (event->eventDelay - (GetTickCount() - event->eventStartTickCount)+999)/1000);
				break;
				
			case EVENTFLAG_SHOW_DELAY_STRING:
				if(!event->m_StringsID.empty())
				{
					if(event->m_StringsID[eventMessageCount] < MAX_GAME_STRING && (*g_pGameStringTable)[event->m_StringsID[eventMessageCount]].GetString() != NULL)
						sprintf(str, (*g_pGameStringTable)[event->m_StringsID[eventMessageCount]].GetString(), (event->eventDelay - (GetTickCount() - event->eventStartTickCount)+999)/1000);
				}
				break;
			}			
			if (str[0] != NULL)
			{
				const COLORREF color = RGB(29<<3, 29<<3, 12<<3);	//CDirectDraw::Color(29,8,12);
				
				const int strWidth = g_GetStringWidth(str, pPrintInfo->hfont)/2;
				pPrintInfo->text_color = 0;
				g_Print(strX-strWidth+1, strY+1, str, pPrintInfo);
				
				pPrintInfo->text_color = color;
				g_Print(strX-strWidth, strY, str, pPrintInfo);
				
				strY+=20;
			}		
		}
	}

	ZeroMemory( str, 256 );
	ZeroMemory( str2, 256 );
	

	// 涅胶飘 何盒篮 涅胶飘 牢磐其捞胶肺 猾促. 酒贰何盒篮 荤侩登瘤 臼阑淀..
	const MEvent *QuestEvent = g_pEventManager->GetEventByFlag(EVENTFLAG_QUEST_INFO, 0);
	
	if(QuestEvent != NULL)
	{
		COLORREF color = RGB(116,243,73);
		COLORREF color2 = RGB(80,150,40);
		
		str[0] = NULL;
		str2[0] = NULL;
		
		bool bColor;
		if(QuestEvent->parameter1 > timeGetTime())
		{
			if( ((QuestEvent->parameter1 - timeGetTime()) / 300 ) & 0x1 )
				bColor = true;
			else
				bColor = false;
		}
		else
		{	
			bColor = false;
		}
		
		//if( QuestEvent->parameter1 == 0 )
		{
			QUEST_INFO* mkq = g_pQuestInfoManager->GetInfo( QuestEvent->parameter2 );
			
			if( NULL != mkq)
			{
				switch(mkq->GetType() )
				{
				case QUEST_INFO_GATHER_ITEM :
					{
						// 酒捞袍 葛栏扁
						sprintf(str,"瘤陛篮 酒捞袍 葛栏绰吝 せせせ %d/%d %d",QuestEvent->parameter3, mkq->GetGoal(), QuestEvent->parameter4 - (timeGetTime() / 1000));
					}
					break;
				case QUEST_INFO_MONSTER_KILL :
					if( QuestEvent->parameter4 <= (timeGetTime() / 1000) )
					{
						sprintf(str,"%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_EXPIRED_TIME_MONSTER_KILL_QUEST].GetString() );
						color = RGB(255,181,181);
						color2 = RGB(255,181,181);
					} else
						if( QuestEvent->parameter3 >= mkq->GetGoal() )
						{
							sprintf(str,"%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_FINISH_MONSTER_KILL_QUEST].GetString() );
							memset(str2, 0, sizeof(str2) );
							DWORD time = QuestEvent->parameter4 - (timeGetTime() / 1000);				
							DWORD sec = time % 60;
							DWORD minute = (time / 60) % 60;
							DWORD hour = (time / 60 / 60);
							bool bContinue = false;				
							if(hour > 0 || bContinue)
							{
								bContinue = true;
								char temp[100];
								sprintf(temp,(*g_pGameStringTable)[UI_STRING_MESSAGE_HOUR].GetString(), hour );
								strcat(str2, temp);
								strcat(str2, " ");
							}
							if(minute > 0 || bContinue)
							{
								bContinue = true;
								char temp[100];
								sprintf(temp,(*g_pGameStringTable)[UI_STRING_MESSAGE_MINUTE].GetString(), minute );
								strcat(str2, temp);
								strcat(str2, " ");
							}
							if(sec> 0 && minute <= 0 && hour <= 0)
							{
								char temp[100];
								sprintf(temp,(*g_pGameStringTable)[UI_STRING_MESSAGE_SECOND].GetString(), sec );
								strcat(str2, temp);					
							}
							//sprintf(str2,"%s",(*g_pGameStringTable)[UI_STRING_MESSAGE_CAN_REQUITAL_FROM_NPC].GetString() );
							strcat( str2, (*g_pGameStringTable)[UI_STRING_MESSAGE_CAN_REQUITAL_FROM_NPC].GetString() );
							color = RGB(142,200,200);
							color2 = RGB(180,220,220);
						}
						
						else
						{				
							sprintf(str, "%s %d/%d ", mkq->GetName(), QuestEvent->parameter3, mkq->GetGoal() );
							DWORD time = QuestEvent->parameter4 - (timeGetTime() / 1000);
							
							
							DWORD sec = time % 60;
							DWORD minute = (time / 60) % 60;
							DWORD hour = (time / 60 / 60);
							bool bContinue = false;
							
							if(hour > 0 || bContinue)
							{
								bContinue = true;
								char temp[100];
								sprintf(temp,(*g_pGameStringTable)[UI_STRING_MESSAGE_HOUR].GetString(), hour );
								strcat(str, temp);
								strcat(str, " ");
							}
							if(minute > 0 || bContinue)
							{
								bContinue = true;
								char temp[100];
								sprintf(temp,(*g_pGameStringTable)[UI_STRING_MESSAGE_MINUTE].GetString(), minute );
								strcat(str, temp);
								strcat(str, " ");
							}
							if(sec> 0 || bContinue)
							{
								bContinue = true;
								char temp[100];
								sprintf(temp,(*g_pGameStringTable)[UI_STRING_MESSAGE_SECOND].GetString(), sec );
								strcat(str, temp);
								strcat(str, " ");
							}
						}
						break;
				case QUEST_INFO_MEET_NPC :
					break;
				}
			}
		}
		
		if (str[0] != NULL)
		{
			
			const int strWidth = g_GetStringWidth(str, pPrintInfo->hfont)/2;
			pPrintInfo->text_color = 0;
			g_Print(strX-strWidth+1, strY+1, str, pPrintInfo);
			
			pPrintInfo->text_color = bColor ? color2 : color;
			g_Print(strX-strWidth, strY, str, pPrintInfo);
			
			strY+=20;
		}		
		if (str2[0] != NULL )
		{
			const int strWidth = g_GetStringWidth(str2, pPrintInfo->hfont)/2;
			pPrintInfo->text_color = 0;
			g_Print(strX-strWidth+1, strY+1, str2, pPrintInfo);
			
			pPrintInfo->text_color = bColor ? color2 : color;
			g_Print(strX-strWidth, strY, str2, pPrintInfo);
			
			strY+=20;		
		}
	}
}

//----------------------------------------------------------------------
// Draw Information
//----------------------------------------------------------------------
// Debug俊 鞘夸茄 沥焊甫 免仿茄促.
//----------------------------------------------------------------------
void
MTopView::DrawDebugInfo(CDirectDrawSurface* pSurface)
{ 
	#ifdef	OUTPUT_DEBUG		
		//----------------------------------------------------------------
		// debug侩 code
		//----------------------------------------------------------------
		///*
		char str[128];
		char attackMode[20];

		if (g_pPlayer->IsAttackModeNormal())
		{
			strcpy(attackMode, "Normal");
		}
		else if (g_pPlayer->IsAttackModeAggress())
		{
			strcpy(attackMode, "Aggress");
		}
		else if (g_pPlayer->IsAttackModePeace())
		{
			strcpy(attackMode, "Peace");
		}		

		switch (g_pPlayer->GetWeaponSpeed())
		{
			case MCreature::WEAPON_SPEED_SLOW : sprintf(attackMode, "Slow(%d)", g_pPlayer->GetAttackSpeed()); break;
			case MCreature::WEAPON_SPEED_NORMAL : sprintf(attackMode, "Normal(%d)", g_pPlayer->GetAttackSpeed()); break;
			case MCreature::WEAPON_SPEED_FAST : sprintf(attackMode, "Fast(%d)", g_pPlayer->GetAttackSpeed()); break;		
		}

		sprintf(attackMode, "gtx(%d. %d %d)", gtx_op, gtx_src, gtx_dest);
		
		int zoneID = (g_bZonePlayerInLarge)? g_nZoneLarge : g_nZoneSmall;

		sprintf(str, "ID = %d / %d疙 [Weapon=%s] [align=%d]", g_pPlayer->GetID(), m_pZone->GetCreatureNumber(), attackMode, g_pPlayer->GetAlignment());	
		pSurface->GDI_Text(10,35, str, RGB(220,220,220));

		sprintf(str, " [ZoneID=%d] XY=(%d, %d), sXY=(%d, %d), SerXY=(%d, %d)", zoneID, g_pPlayer->GetX(), g_pPlayer->GetY(), g_pPlayer->GetSX(), g_pPlayer->GetSY(), g_pPlayer->GetServerX(), g_pPlayer->GetServerY());	
		pSurface->GDI_Text(10,56, str, RGB(220,220,220));

		sprintf(str, "SX = %d, SY = %d, HP=%d, MP=%d", g_pPlayer->GetSX(), g_pPlayer->GetSY(), g_pPlayer->GetHP(), g_pPlayer->GetMP());
		//sprintf(str, "[Wait Move = %d]", g_pPlayer->GetSendMove());
		pSurface->GDI_Text(10,72, str, RGB(220,220,220));

		sprintf(str, "ActionCount = [%d] %d/%d", g_pPlayer->GetAction(), g_pPlayer->GetActionCount(), g_pPlayer->GetActionCountMax());
		pSurface->GDI_Text(10,88, str, RGB(220,220,220));

		sprintf(str, "MoveCount = %d/%d", g_pPlayer->GetMoveCount(), g_pPlayer->GetMoveCountMax());
		pSurface->GDI_Text(10,104, str, RGB(220,220,220));

		sprintf(str, "Dark/Light/Sight = %d/%d/%d", m_DarkBits, g_pPlayer->GetLightSight() + g_pPlayer->GetItemLightSight(), g_pPlayer->GetSight());
		pSurface->GDI_Text(10,120, str, RGB(220,220,220));

		if (g_pProfiler!=NULL && g_pProfiler->HasProfileInfo(g_ProfileInfoName.GetString()))
		{
			DWORD totalTime = g_pProfiler->GetTotalTime(g_ProfileInfoName.GetString());
			DWORD times = g_pProfiler->GetTimes(g_ProfileInfoName.GetString());
			float avgTime = g_pProfiler->GetAverageTime(g_ProfileInfoName.GetString());

			sprintf(str, "%s = %d / %d = %7.3f", g_ProfileInfoName.GetString(), totalTime, times, avgTime);
			pSurface->GDI_Text(10,136, str, RGB(220,220,220));		
		}		
		

//		if (m_pAlphaEffectTextureManager!=NULL)
//		{
//			sprintf(str, "Effect Used/Part/Max = %d/%d/%d", m_pAlphaEffectTextureManager->GetUsed(), m_pAlphaEffectTextureManager->GetMaxPartIndex(), m_pAlphaEffectTextureManager->GetMaxIndex());
//			pSurface->GDI_Text(10,150, str, RGB(220,220,220));	
//		}
//
//		if (m_pScreenEffectTextureManager!=NULL)
//		{
//			sprintf(str, "ScreenEffect Used/Part/Max = %d/%d/%d", m_pScreenEffectTextureManager->GetUsed(), m_pScreenEffectTextureManager->GetMaxPartIndex(), m_pScreenEffectTextureManager->GetMaxIndex());
//			pSurface->GDI_Text(10,165, str, RGB(220,220,220));	
//		}
//
//		if (m_pImageObjectShadowManager!=NULL)
//		{
//			sprintf(str, "ImageObjectShadow Used/Part/Max = %d/%d/%d", m_pImageObjectShadowManager->GetUsed(), m_pImageObjectShadowManager->GetMaxPartIndex(), m_pImageObjectShadowManager->GetMaxIndex());
//			pSurface->GDI_Text(10,180, str, RGB(220,220,220));	
//		}
//
//		if (m_pAddonShadowManager!=NULL)
//		{
//			sprintf(str, "AddonShadow Used/Part/Max = %d/%d/%d", m_pAddonShadowManager->GetUsed(), m_pAddonShadowManager->GetMaxPartIndex(), m_pAddonShadowManager->GetMaxIndex());
//			pSurface->GDI_Text(10,195, str, RGB(220,220,220));	
//		}
//
//		if (m_pEffectShadowManager!=NULL)
//		{
//			sprintf(str, "EffectShadow Used/Part/Max = %d/%d/%d", m_pEffectShadowManager->GetUsed(), m_pEffectShadowManager->GetMaxPartIndex(), m_pEffectShadowManager->GetMaxIndex());
//			pSurface->GDI_Text(10,210, str, RGB(220,220,220));	
//		}

		if (g_pRequestServerPlayerManager!=NULL
			&& g_pRequestClientPlayerManager!=NULL)
		{
			sprintf(str, "Request Other/My = %d/%d", g_pRequestServerPlayerManager->GetSize(), g_pRequestClientPlayerManager->GetSize());
			pSurface->GDI_Text(10,225, str, RGB(220,220,220));	
		}

		//#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
		//	sprintf(str, "Repeat = %d, UsedActionInfo = %d", (g_pPlayer->IsRepeatAction()?"True":"False"), g_pPlayer->GetUsedActionInfo());
			//pSurface->GDI_Text(10,195, str, RGB(220,220,220));	
		//#endif

		
		//if (g_pCurrentMagazine!=NULL)
		//{
		//	sprintf(str, "Bullet = %d", g_pCurrentMagazine->GetNumber());
		//	pSurface->GDI_Text(10,136, str, RGB(220,220,220));
		//}

		//sprintf(str, "FirstPoint : X = %d, Y = %d", firstPointX, firstPointY);
		//pSurface->GDI_Text(9,109, str, RGB(220,220,220));
		//*/	

		// Timer 免仿
		//sprintf(str, "Timer = %ld", g_CurrentTime);	
		//g_pBack->GDI_Text(11,11, str, RGB(20,20,20));
		//g_pBack->GDI_Text(10,20, str, 0xFFFFFF);
		

		// Mouse谅钎 嘛扁	
		//sprintf(str, "%d, %d", g_x, g_y);	
		//g_pBack->GDI_Text(11,101, str, RGB(20,20,20));
		//g_pBack->GDI_Text(10,100, str, RGB(255,255,255));

		//sprintf(str, "ImageObject=%d俺", m_mapImageObject.size());
		//pSurface->GDI_Text(100,580, str, 0xFFFFFF);

		char infoStr[4][2][20] =
		{
			// FALSE, TRUE
			{ "Mode=555", "Mode=565" },
			{ "WinMM", "SWSynth" },
			{ "Game", "UI" },
			{ "One", "Slide" }
		};

		sprintf(str, "%s, %s, InputFocus=%s, SCShot=%s",
						infoStr[0][CDirectDraw::Is565()],
						infoStr[1][g_DXMusic.IsSoftwareSynth()],
						infoStr[2][g_bUIInput],
						infoStr[3][g_bSlideScreenShot]
				);

		pSurface->GDI_Text(150,580, str, 0xFFFFFF);		

		// 巢篮 Texture皋葛府 test
		DDSCAPS2 ddsCaps2;
		DWORD dwTotal;
		DWORD dwFree;
		ZeroMemory(&ddsCaps2, sizeof(ddsCaps2)); 
		ddsCaps2.dwCaps = DDSCAPS_TEXTURE; 
		HRESULT hr = CDirectDraw::GetDD()->GetAvailableVidMem(&ddsCaps2, &dwTotal, &dwFree);

		sprintf(str, "Texture = %ld/%ld", dwFree, dwTotal);
		pSurface->GDI_Text(500,580, str, 0xFFFFFF);		

	
	#endif
}


//----------------------------------------------------------------------
// Draw Test Help
//----------------------------------------------------------------------
// ClientTest侩 Help
//----------------------------------------------------------------------
void
MTopView::DrawTestHelp()
{
	#ifdef OUTPUT_DEBUG
		//--------------------------------------------------------------------
		//
		// F1阑 穿弗 惑怕 --> 档框富 免仿
		//
		//--------------------------------------------------------------------	
		if (g_pDXInput->KeyDown(DIK_F11)
			&& (g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL)))
		{
			RECT rect = { 250, 25, 530, 410 };

			if (CDirect3D::IsHAL())
			{
				//DrawBox3D(&rect, 0x7000);
				DrawAlphaBox(&rect, 0, 0, 0, 20);
			}
			else
			{
				m_pSurface->Lock();
					
				if (CDirectDraw::Is565())
				{
					m_pSurface->GammaBox565(&rect, 15);
				}
				else
				{
					m_pSurface->GammaBox555(&rect, 15);
				}

				m_pSurface->Unlock();
			}

			const int maxHelp = 19;
			const char helpStr[maxHelp][128] = 
			{
				"---------------------------------------------------------------------",
				"         ALPHA-TEST CLIENT 档框富",
				"---------------------------------------------------------------------",
				"  [ESC] Menu",
				"  [F1~F8] 骇飘 酒捞袍 荤侩",
				"  [Scroll Lock] 拳搁 母媚(--> \\ScreenShot)",
				"  [Ctrl + M/T/Z] 瘤档/霸烙矫埃/粮 捞抚",
				"  [Tab] 酒捞袍芒 on/off",			
				"  [Ctrl + Tab] 厘馒芒 on/off",			
				"  [L-Ctrl + ~] 骇飘 on/off",			
				"  [L-Ctrl + S] 扁贱 酒捞能 急琶",			
				"  [L-Ctrl + Space] 酒捞袍芒, 厘馒芒 on/off",
				"---------------------------------------------------------------------",
				"  [L-Button] 捞悼, 眠利, 傍拜",
				"  [L-Shift + L-Button] 碍力 傍拜",
				"  [L-Ctrl + L-Button] 利父 急琶 傍拜",
				"  [R-Button] 付过, 扁贱",
				"---------------------------------------------------------------------",
				"  [盲泼且锭] ! 甫 荤侩窍搁 粮 盲泼"
			};

			POINT point;
			point.x = 250;		
			point.y = 25;
			for (int i=0; i<maxHelp; i++)
			{
				//pSurface->GDI_Text(point.x,1, point.y,1, helpStr[i], RGB(0,0,0));
				m_pSurface->GDI_Text(point.x, point.y, helpStr[i], RGB(220,220,220));
				point.y += 20;
			}
		}
	#endif
}

//----------------------------------------------------------------------
// Draw
//----------------------------------------------------------------------
// 傈何 促矫 弊副扒瘤   
// 捞傈狼 Tile阑 捞侩秦辑 老何父 弊副扒瘤甫 急琶茄促.
//----------------------------------------------------------------------
void
MTopView::Draw(int firstPointX,int firstPointY)
{ 
	if (g_pZone==NULL)
	{
		#ifdef OUTPUT_DEBUG_DRAW_PROCESS
			DEBUG_ADD("g_pZone is NULL");
		#endif

		return;
	}

	#ifdef OUTPUT_DEBUG
/*		if (g_pDXInput->KeyDown(DIK_RCONTROL) && g_pDXInput->KeyDown(DIK_RSHIFT))
		{
			if (g_WipeValue==0 || g_WipeValue==64)
			{
				if (g_WipeStep==0)	
				{
					if (g_WipeValue==0)
						g_WipeStep = 1;
					else
						g_WipeStep = -1;
				}
				else g_WipeStep *= -1;
			}
		}	
		else
		{
			g_WipeValue += g_WipeStep;

			if (g_WipeValue<0) g_WipeValue = 0;
			else if (g_WipeValue>64) g_WipeValue=64;
		}
*/
		// 鸥老俊 器呕 沥焊 免仿窍扁
		if (g_pDXInput->KeyDown(DIK_T) && g_pDXInput->KeyDown(DIK_LCONTROL))
		{
			SetFirstDraw();
		}
	#endif	

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD("Draw");
	#endif

	/*
	// Work Thread甫 捞侩茄 loading test
	static int first = 1;
	if (first)
	{
		//18336
		MLoadingSPKWorkNode4* pNode = new MLoadingSPKWorkNode4(0, 10000, 2);
		pNode->SetSPK( &m_AddonSPK, FILE_ISPRITE_ADDON );
		pNode->SetType( 1 );
		g_pLoadingThread->SetPriority( THREAD_PRIORITY_BELOW_NORMAL );
		g_pLoadingThread->AddLast( pNode );

		first = 0;
	}
	*/

	//------------------------------------------------------------
	// 厚傍侥(-_-;) 康绢 迄飘 荤侩救窃 - -;
	//------------------------------------------------------------
	//gC_font.NotUseNoPublicEng();		

	//#ifdef OUTPUT_DEBUG
		//	DEBUG_ADD("[TempDebug] Before Draw");
	//#endif	

	//------------------------------------------------------------
	// Clip康开 救俊辑父 免仿茄促.
	//------------------------------------------------------------
	int clipRight = m_pSurface->GetClipRight();
	int clipBottom	= m_pSurface->GetClipBottom();
	m_pSurface->SetClipRightBottom(g_GameRect.right, g_GameRect.bottom);		

	//------------------------------------------------------------
	// Tile阑 贸澜 弊副 锭, 促~ 弊赴促.
	//------------------------------------------------------------
	if (m_bFirstTileDraw)
	{
		DEBUG_ADD("Draw m_bFirstTileDraw: Restore");
		
		CDirectDraw::RestoreAllSurfaces();
		CDirect3D::Restore();

		DEBUG_ADD("Draw m_bFirstTileDraw: Clear");
		
		m_mapImageObject.clear();
		m_mapCreature.clear();
		
		DEBUG_ADD("Draw m_bFirstTileDraw: DrawTileSurface");
		
		__BEGIN_PROFILE("DrawTileSurface")

		DrawTileSurface();

		__END_PROFILE("DrawTileSurface")

		DEBUG_ADD("Draw m_bFirstTileDraw: DetermineImageObject");
		
		DetermineImageObject();
		m_bFirstTileDraw = false;

		DEBUG_ADD("Draw m_bFirstTileDraw: OK");
		
		/*
		CDirect3D::GetDevice()->LightEnable( 0, false );
		CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_LIGHTING, false );
		
	
		D3DLIGHT7 light;
	    D3DUtil_InitLight( light, D3DLIGHT_POINT, 0.0f, 0.0f, -10.0f );

		light.dltType      = D3DLIGHT_SPOT;
		light.dcvDiffuse.r   = 1.0f;
		light.dcvDiffuse.g   = 1.0f;
		light.dcvDiffuse.b   = 1.0f;
		light.dcvSpecular = light.dcvDiffuse;
		light.dvPosition.x = 0.0f;
		light.dvPosition.y = 0.0f;
		light.dvPosition.z = -10.0f;
		light.dvDirection.x = 0.0f;	
		light.dvDirection.y = 0.0f;
		light.dvDirection.z = 1.0f;
		light.dvTheta =       0.5f; //盔辉狼 吝居 农扁
		light.dvPhi =         0.8f; //盔辉狼 寇胞 农扁
		light.dvAttenuation0 = 1.0f;
		light.dvFalloff		= 1.0f;
		
		CDirect3D::GetDevice()->SetLight( 0, &light );	
		*/
		//*/
	}
	//------------------------------------------------------------
	// 捞傈俊 弊赴 Tile阑 促矫 捞侩茄促.
	//------------------------------------------------------------
	//else
	{			
		if (CDirect3D::IsHAL())// && m_b3DLight)
		{
			#ifdef OUTPUT_DEBUG_DRAW_PROCESS
				DEBUG_ADD( "IsLost?" );
			#endif

			// 档框捞 瞪鳖..
			if (CDirect3D::IsLost())
			{
				#ifdef OUTPUT_DEBUG_DRAW_PROCESS
					DEBUG_ADD( "CDirect3D::Restore" );
				#endif

				CDirect3D::Restore();

				#ifdef OUTPUT_DEBUG_DRAW_PROCESS
					DEBUG_ADD( "RestoreSurface" );
				#endif

				RestoreSurface();
			}

			//------------------------------------------------
			// BeginScene
			//------------------------------------------------
			/*
			if (CDirect3D::GetDevice()->BeginScene()!=D3D_OK)
			{
				CDirectDraw::RestoreAllSurfaces();
				CDirect3D::Restore();

				RestoreSurface();

				m_pSurface->SetClipNULL();
				return;
			}			
			*/
			

			///*
			///*
			/*
			D3DRECT rect3d;
			rect3d.x1 = 0;
			rect3d.y1 = 0;
			rect3d.x2 = CLIPSURFACE_WIDTH;
			rect3d.y2 = CLIPSURFACE_HEIGHT;
			CDirect3D::GetDevice()->Clear( 1UL, (D3DRECT *)&rect3d, 
										D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
										0x00000000, 1.0f, 0L );
			*/
			#ifdef OUTPUT_DEBUG_DRAW_PROCESS
				DEBUG_ADD( "Clear 3D" );
			#endif

			CDirect3D::GetDevice()->Clear( 0, NULL, D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );
			//*/
			

			//DrawZone(firstPointX, firstPointY);

			//*/
	

			/*
			if (g_pDXInput->KeyDown(DIK_Z))
			{
				D3DLIGHT7 light;
				CDirect3D::GetDevice()->GetLight( 0, &light );

				light.dvTheta -=       0.01f; //盔辉狼 吝居 农扁
				light.dvPhi -=         0.01f; //盔辉狼 寇胞 农扁
				
				CDirect3D::GetDevice()->SetLight( 0, &light );
			}

			if (g_pDXInput->KeyDown(DIK_A))
			{
				D3DLIGHT7 light;
				CDirect3D::GetDevice()->GetLight( 0, &light );

				light.dvTheta +=       0.01f; //盔辉狼 吝居 农扁
				light.dvPhi +=         0.01f; //盔辉狼 寇胞 农扁
				
				CDirect3D::GetDevice()->SetLight( 0, &light );
			}
			*/
			
			///*
			//test.Rotate( 0.1f, 0.1f, 0 );

			//------------------------------------------------
			// LightBuffer 檬扁拳
			//------------------------------------------------
			#ifdef OUTPUT_DEBUG_DRAW_PROCESS
				DEBUG_ADD( "Clear LightBufferFilter3D" );
			#endif

			__BEGIN_PROFILE("ClearLBF3D")

			ClearLightBufferFilter3D();

			__END_PROFILE("ClearLBF3D")
			
			//------------------------------------------------
			// Draw Zone
			//------------------------------------------------
			#ifdef OUTPUT_DEBUG_DRAW_PROCESS
				DEBUG_ADD( "DrawZone" );
			#endif

			__BEGIN_PROFILE("DrawZone")

			DrawZone(firstPointX, firstPointY);	

			__END_PROFILE("DrawZone")


			// mouse谅钎狼 蝴			
			//AddLightFilter( m_FirstZonePixel.x,g_x, m_FirstZonePixel.y,g_y, 4 );
			//------------------------------------------------
			// Fade
			//------------------------------------------------
			#ifdef OUTPUT_DEBUG_DRAW_PROCESS
				DEBUG_ADD( "DrawFade" );
			#endif

			__BEGIN_PROFILE("DrawFade")

			DrawFade();

			__END_PROFILE("DrawFade")

			//------------------------------------------------
			// Minimap
			//------------------------------------------------
			/* // 2001.7.14 林籍贸府
			if (g_pUserOption->DrawMinimap)
			{
				DrawMinimap( CLIPSURFACE_WIDTH-m_pMinimapSPR->GetWidth(),
							0,//440-m_pMinimapSPR->GetHeight(),
							1);	// scale
			}
			*/

			//------------------------------------------------
			// test侩 档框富
			//------------------------------------------------
			if (g_pDXInput->KeyDown(DIK_F11))
			{
				DrawTestHelp();
			}

		
			//------------------------------------------------
			// EndScene
			//------------------------------------------------
			//CDirect3D::GetDevice()->EndScene();

			//------------------------------------------------
			// 力芭等 Surface甸阑 瘤况拎具 茄促.
			// 馆靛矫, EndScene()捞 场唱绊 瘤况具 茄促.
			//------------------------------------------------
			#ifdef OUTPUT_DEBUG_DRAW_PROCESS
				DEBUG_ADD( "Remove RemovedTexture" );
			#endif

			__BEGIN_PROFILE("ClearTexture")

//			if (m_pAlphaEffectTextureManager)				m_pAlphaEffectTextureManager->DeleteRemoved();
//			if (m_pScreenEffectTextureManager)	m_pScreenEffectTextureManager->DeleteRemoved();
//			if (m_pImageObjectShadowManager)	m_pImageObjectShadowManager->DeleteRemoved();
//			if (m_pAddonShadowManager)			m_pAddonShadowManager->DeleteRemoved();
//			if (m_pOustersShadowManager)		m_pOustersShadowManager->DeleteRemoved();
//			if (m_pEffectShadowManager)			m_pEffectShadowManager->DeleteRemoved();
//			if (m_pCreatureShadowManager)		m_pCreatureShadowManager->DeleteRemoved();
			
			__END_PROFILE("ClearTexture")
		}
		else
		{
			//------------------------------------------------
			// LightBuffer 檬扁拳
			//------------------------------------------------
			#ifdef OUTPUT_DEBUG_DRAW_PROCESS
				DEBUG_ADD( "Clear LightBufferFilter2D");
			#endif

			__BEGIN_PROFILE("ClearLBF2D")
			
			ClearLightBufferFilter2D();

			__END_PROFILE("ClearLBF2D")
			
			
			//------------------------------------------------
			// Draw Zone
			//------------------------------------------------
			#ifdef OUTPUT_DEBUG_DRAW_PROCESS
				DEBUG_ADD( "DrawZone" );
			#endif

			__BEGIN_PROFILE("DrawZone")

			DrawZone(firstPointX, firstPointY);		

			__END_PROFILE("DrawZone")

			//------------------------------------------------
			// Fade
			//------------------------------------------------
			#ifdef OUTPUT_DEBUG_DRAW_PROCESS
				DEBUG_ADD( "DrawFade" );
			#endif

			__BEGIN_PROFILE("DrawFade")

			DrawFade();	

			__BEGIN_PROFILE("DrawFade")

			//------------------------------------------------
			// Minimap
			//------------------------------------------------
			/* // 2001.7.14 林籍贸府
			if (g_pUserOption->DrawMinimap)
			{
				DrawMinimap( CLIPSURFACE_WIDTH-m_pMinimapSPR->GetWidth(),
							0, //440-m_pMinimapSPR->GetHeight(),
							1);	// scale
			}
			*/

			//------------------------------------------------
			// test侩 档框富
			//------------------------------------------------
//			if (g_pDXInput->KeyDown(DIK_F11))
//			{
//				DrawTestHelp();				
//			}
		}	
	}

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "Draw Information" );
	#endif

	//------------------------------------------------
	// 沥焊 免仿
	//------------------------------------------------
	DrawInformation();

	//#ifdef OUTPUT_DEBUG
		//	DEBUG_ADD("[TempDebug] After Draw");
	//#endif	
	

	//char str[80];
	//sprintf(str, "ImageObject=%d俺", m_mapImageObject.size());
	//m_pSurface->GDI_Text(100,580, str, 0xFFFFFF);

	//------------------------------------------------------------
	// 沥惑利牢 免仿
	//------------------------------------------------------------
	//m_pSurface->SetClipNULL();	
	m_pSurface->SetClipRightBottom(clipRight, clipBottom);

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD("Draw OK");		
	#endif
}

//----------------------------------------------------------------------
// Draw LightBuffer
//----------------------------------------------------------------------
// m_LightBufferFilter甫 m_pLightBufferTexture肺 blt窍绊
// m_pLightBufferTexture甫 texture肺 汲沥秦辑
// 拳搁俊 LightBufferFilter甫 免仿窍霸 等促.
//----------------------------------------------------------------------
void
MTopView::DrawLightBuffer3D()
{
	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "Start DrawLightBuffer3D" );
	#endif

	if (//CDirect3D::IsHAL() && 
		m_DarkBits || g_pPlayer->IsInDarkness())
	{
		//------------------------------------------------
		// LightBufferFilter --> Texture
		//------------------------------------------------
		WORD *lpSurface, pitch;
		m_pLightBufferTexture->Lock();
		lpSurface = (WORD*)m_pLightBufferTexture->GetSurfacePointer();
		pitch = m_pLightBufferTexture->GetSurfacePitch();

		m_LightBufferFilter.Blt4444(lpSurface, pitch);
		//m_LightBufferFilter.Blt4444Color(lpSurface, pitch, 0x0200);

		m_pLightBufferTexture->Unlock();
		//*/
		
		//------------------------------------------------
		// Texture汲沥窍绊 免仿
		//------------------------------------------------
		RECT rect = { 0, 0, g_GameRect.right, g_GameRect.bottom };
			
		DRAW_TEXTURE_SURFACE( m_pLightBufferTexture, &rect )		
		else
		{
			#ifdef OUTPUT_DEBUG_DRAW_PROCESS
				DEBUG_ADD( "Failed DrawLightBuffer3D" );
			#endif
		}


		#ifdef OUTPUT_DEBUG_DRAW_PROCESS
			DEBUG_ADD( "End DrawLightBuffer3D" );
		#endif
	}

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "End DrawLightBuffer3D" );
	#endif

	
}

//----------------------------------------------------------------------
// Draw LightBuffer
//----------------------------------------------------------------------
// m_LightBufferFilter甫 m_pLightBufferTexture肺 blt窍绊
// m_pLightBufferTexture甫 texture肺 汲沥秦辑
// 拳搁俊 LightBufferFilter甫 免仿窍霸 等促.
//----------------------------------------------------------------------
void
MTopView::DrawLightBuffer2D()
{
	//if (CDirect3D::IsHAL() && m_b3DLight && 
	if (m_DarkBits || g_pPlayer->IsInDarkness())
	{
		//------------------------------------------------
		// LightBufferFilter --> m_pSurface
		//------------------------------------------------
		WORD *lpSurface, pitch;

		WORD	*lpSurfaceTemp1,
				*lpSurfaceTemp2,
				*lpSurfaceTemp3,
				*lpSurfaceTemp4,
				*lpSurfaceTemp5,
				*lpSurfaceTemp6,
				*lpSurfaceTemp7,
				*lpSurfaceTemp8,
				*lpSurfaceTemp9,
				*lpSurfaceTemp10,
				*lpSurfaceTemp11,
				*lpSurfaceTemp12;
		//m_pSurface->Lock();
		lpSurface = (WORD*)m_pSurface->GetSurfacePointer();
		pitch = m_pSurface->GetSurfacePitch();		

		//--------------------------------------------
		// 5:6:5
		//--------------------------------------------
		if (CDirectDraw::Is565())
		{
			int*	pPH = m_p2DLightPixelHeight;
			for (int y=0; y<m_LightBufferFilter.GetHeight(); y++)
			{			
				BYTE*	pFilter = m_LightBufferFilter.GetFilter(y);
				int*	pPW	= m_p2DLightPixelWidth;			
				
				
				//--------------------------------------------
				// 9临究 免仿且锭...
				//--------------------------------------------
				if (*pPH==9)
				{
					lpSurfaceTemp1 = lpSurface;
					lpSurfaceTemp2 = (WORD*)((BYTE*)lpSurfaceTemp1 + pitch);
					lpSurfaceTemp3 = (WORD*)((BYTE*)lpSurfaceTemp2 + pitch);
					lpSurfaceTemp4 = (WORD*)((BYTE*)lpSurfaceTemp3 + pitch);
					lpSurfaceTemp5 = (WORD*)((BYTE*)lpSurfaceTemp4 + pitch);
					lpSurfaceTemp6 = (WORD*)((BYTE*)lpSurfaceTemp5 + pitch);
					lpSurfaceTemp7 = (WORD*)((BYTE*)lpSurfaceTemp6 + pitch);
					lpSurfaceTemp8 = (WORD*)((BYTE*)lpSurfaceTemp7 + pitch);
					lpSurfaceTemp9 = (WORD*)((BYTE*)lpSurfaceTemp8 + pitch);
					// add by sonic 2006.9.29
					if(g_MyFull)
					{
						lpSurfaceTemp10 = (WORD*)((BYTE*)lpSurfaceTemp9 + pitch);
						lpSurfaceTemp11 = (WORD*)((BYTE*)lpSurfaceTemp10 + pitch);
						lpSurface = (WORD*)((BYTE*)lpSurfaceTemp11 + pitch);
					}
					else
					{
						lpSurface = (WORD*)((BYTE*)lpSurfaceTemp9 + pitch);
					}
					//end by sonic
					// 促澜..
					

					for (int x=0; x<m_LightBufferFilter.GetWidth(); x++)
					{				
						int light	= *pFilter;
						int	len		= *pPW;
						
						//if (light!=31)
						//{
							// len痢 嘛扁
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp1, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp2, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp3, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp4, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp5, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp6, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp7, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp8, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp9, len, light);
							// add by sonic 2006.9.29
							if(g_MyFull)
							{
								m_pSurface->Gamma4Pixel565(lpSurfaceTemp10, len, light);
								m_pSurface->Gamma4Pixel565(lpSurfaceTemp11, len, light);
							}
							// end by sonic
						//}
						

						// 促澜 filter蔼
						pFilter++;
						pPW++;

						// 促澜 免仿 困摹 --> 8 pixel 第
						lpSurfaceTemp1 += len;
						lpSurfaceTemp2 += len;
						lpSurfaceTemp3 += len;
						lpSurfaceTemp4 += len;
						lpSurfaceTemp5 += len;
						lpSurfaceTemp6 += len;
						lpSurfaceTemp7 += len;
						lpSurfaceTemp8 += len;
						lpSurfaceTemp9 += len;
						// add by sonic 2006.9.29
						if(g_MyFull)
						{
							lpSurfaceTemp10 += len;
							lpSurfaceTemp11 += len;							
						}
						// end by sonic
					}	
				}
				//--------------------------------------------
				// 10临究 免仿
				//--------------------------------------------
				else //if (*pPH==8)
				{
					lpSurfaceTemp1 = lpSurface;
					lpSurfaceTemp2 = (WORD*)((BYTE*)lpSurfaceTemp1 + pitch);
					lpSurfaceTemp3 = (WORD*)((BYTE*)lpSurfaceTemp2 + pitch);
					lpSurfaceTemp4 = (WORD*)((BYTE*)lpSurfaceTemp3 + pitch);
					lpSurfaceTemp5 = (WORD*)((BYTE*)lpSurfaceTemp4 + pitch);
					lpSurfaceTemp6 = (WORD*)((BYTE*)lpSurfaceTemp5 + pitch);
					lpSurfaceTemp7 = (WORD*)((BYTE*)lpSurfaceTemp6 + pitch);
					lpSurfaceTemp8 = (WORD*)((BYTE*)lpSurfaceTemp7 + pitch);
					lpSurfaceTemp9 = (WORD*)((BYTE*)lpSurfaceTemp8 + pitch);
					lpSurfaceTemp10 = (WORD*)((BYTE*)lpSurfaceTemp9 + pitch);
					// add by sonic 2006.9.29
					if(g_MyFull)
					{
						lpSurfaceTemp11 = (WORD*)((BYTE*)lpSurfaceTemp10 + pitch);
						lpSurfaceTemp12 = (WORD*)((BYTE*)lpSurfaceTemp11 + pitch);
						lpSurface = (WORD*)((BYTE*)lpSurfaceTemp12 + pitch);
					}
					else
					{
						lpSurface = (WORD*)((BYTE*)lpSurfaceTemp10 + pitch);
					}
					// end by sonic

					// 促澜..
					//lpSurface = (WORD*)((BYTE*)lpSurfaceTemp10 + pitch);

					for (int x=0; x<m_LightBufferFilter.GetWidth(); x++)
					{				
						int light	= *pFilter;
						int	len		= *pPW;
						
						//if (light!=31)
						//{
							// len痢 嘛扁
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp1, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp2, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp3, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp4, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp5, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp6, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp7, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp8, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp9, len, light);
							m_pSurface->Gamma4Pixel565(lpSurfaceTemp10, len, light);
							// add by sonic 2006.9.29
							if(g_MyFull)
							{
								m_pSurface->Gamma4Pixel565(lpSurfaceTemp11, len, light);
								m_pSurface->Gamma4Pixel565(lpSurfaceTemp12, len, light);
							}
							// edn by sonic
						//}
						

						// 促澜 filter蔼
						pFilter++;
						pPW++;

						// 促澜 免仿 困摹 --> 8 pixel 第
						lpSurfaceTemp1 += len;
						lpSurfaceTemp2 += len;
						lpSurfaceTemp3 += len;
						lpSurfaceTemp4 += len;
						lpSurfaceTemp5 += len;
						lpSurfaceTemp6 += len;
						lpSurfaceTemp7 += len;
						lpSurfaceTemp8 += len;
						lpSurfaceTemp9 += len;
						lpSurfaceTemp10 += len;
						// add by sonic 2006.9.29
						if(g_MyFull)
						{
							lpSurfaceTemp11 += len;
							lpSurfaceTemp12 += len;
						}
						// end by sonic
					}	
				}
				
				pPH++;
			}
		}
		//--------------------------------------------
		// 5:5:5
		//--------------------------------------------
		else
		{
			int*	pPH = m_p2DLightPixelHeight;
			for (int y=0; y<m_LightBufferFilter.GetHeight(); y++)
			{			
				BYTE*	pFilter = m_LightBufferFilter.GetFilter(y);
				int*	pPW	= m_p2DLightPixelWidth;			
				
				
				//--------------------------------------------
				// 9临究 免仿且锭...
				//--------------------------------------------
				if (*pPH==9)
				{
					lpSurfaceTemp1 = lpSurface;
					lpSurfaceTemp2 = (WORD*)((BYTE*)lpSurfaceTemp1 + pitch);
					lpSurfaceTemp3 = (WORD*)((BYTE*)lpSurfaceTemp2 + pitch);
					lpSurfaceTemp4 = (WORD*)((BYTE*)lpSurfaceTemp3 + pitch);
					lpSurfaceTemp5 = (WORD*)((BYTE*)lpSurfaceTemp4 + pitch);
					lpSurfaceTemp6 = (WORD*)((BYTE*)lpSurfaceTemp5 + pitch);
					lpSurfaceTemp7 = (WORD*)((BYTE*)lpSurfaceTemp6 + pitch);
					lpSurfaceTemp8 = (WORD*)((BYTE*)lpSurfaceTemp7 + pitch);
					lpSurfaceTemp9 = (WORD*)((BYTE*)lpSurfaceTemp8 + pitch);
					// add by sonic 2006.9.29
					if(g_MyFull)
					{
						lpSurfaceTemp10 = (WORD*)((BYTE*)lpSurfaceTemp9 + pitch);
						lpSurfaceTemp11 = (WORD*)((BYTE*)lpSurfaceTemp10 + pitch);
						lpSurface = (WORD*)((BYTE*)lpSurfaceTemp11 + pitch);
					}
					else
					{
						lpSurface = (WORD*)((BYTE*)lpSurfaceTemp9 + pitch);
					}
					// end by sonic
					// 促澜..
					

					for (int x=0; x<m_LightBufferFilter.GetWidth(); x++)
					{				
						int light	= *pFilter;
						int	len		= *pPW;
						
						//if (light!=31)
						//{
							// len痢 嘛扁
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp1, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp2, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp3, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp4, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp5, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp6, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp7, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp8, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp9, len, light);
							// add by sonic 2006.9.29
							if(g_MyFull)
							{
								m_pSurface->Gamma4Pixel555(lpSurfaceTemp10, len, light);
								m_pSurface->Gamma4Pixel555(lpSurfaceTemp11, len, light);
							}
							// end by sonic
						//}
						

						// 促澜 filter蔼
						pFilter++;
						pPW++;

						// 促澜 免仿 困摹 --> 8 pixel 第
						lpSurfaceTemp1 += len;
						lpSurfaceTemp2 += len;
						lpSurfaceTemp3 += len;
						lpSurfaceTemp4 += len;
						lpSurfaceTemp5 += len;
						lpSurfaceTemp6 += len;
						lpSurfaceTemp7 += len;
						lpSurfaceTemp8 += len;
						lpSurfaceTemp9 += len;
						// add by sonic 2006.9.29
						if(g_MyFull)
						{
							lpSurfaceTemp10 += len;
							lpSurfaceTemp11 += len;
						}
						// end by sonic
					}	
				}
				//--------------------------------------------
				// 10临究 免仿
				//--------------------------------------------
				else //if (*pPH==8)
				{
					lpSurfaceTemp1 = lpSurface;
					lpSurfaceTemp2 = (WORD*)((BYTE*)lpSurfaceTemp1 + pitch);
					lpSurfaceTemp3 = (WORD*)((BYTE*)lpSurfaceTemp2 + pitch);
					lpSurfaceTemp4 = (WORD*)((BYTE*)lpSurfaceTemp3 + pitch);
					lpSurfaceTemp5 = (WORD*)((BYTE*)lpSurfaceTemp4 + pitch);
					lpSurfaceTemp6 = (WORD*)((BYTE*)lpSurfaceTemp5 + pitch);
					lpSurfaceTemp7 = (WORD*)((BYTE*)lpSurfaceTemp6 + pitch);
					lpSurfaceTemp8 = (WORD*)((BYTE*)lpSurfaceTemp7 + pitch);
					lpSurfaceTemp9 = (WORD*)((BYTE*)lpSurfaceTemp8 + pitch);
					lpSurfaceTemp10 = (WORD*)((BYTE*)lpSurfaceTemp9 + pitch);
						// add by sonic 2006.9.29
						if(g_MyFull)
						{
							lpSurfaceTemp11 = (WORD*)((BYTE*)lpSurfaceTemp10 + pitch);
							lpSurfaceTemp12 = (WORD*)((BYTE*)lpSurfaceTemp11 + pitch);
							lpSurface = (WORD*)((BYTE*)lpSurfaceTemp12 + pitch);
						}else 
						{
							// 促澜..
							lpSurface = (WORD*)((BYTE*)lpSurfaceTemp10 + pitch);
						}
						// end by sonic


					for (int x=0; x<m_LightBufferFilter.GetWidth(); x++)
					{				
						int light	= *pFilter;
						int	len		= *pPW;
						
						//if (light!=31)
						//{
							// len痢 嘛扁
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp1, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp2, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp3, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp4, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp5, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp6, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp7, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp8, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp9, len, light);
							m_pSurface->Gamma4Pixel555(lpSurfaceTemp10, len, light);
							// add by sonic 2006.9.29
							if(g_MyFull)
							{
								m_pSurface->Gamma4Pixel555(lpSurfaceTemp11, len, light);
								m_pSurface->Gamma4Pixel555(lpSurfaceTemp12, len, light);
							}
							// end by sonic
						//}
						

						// 促澜 filter蔼
						pFilter++;
						pPW++;

						// 促澜 免仿 困摹 --> 8 pixel 第
						lpSurfaceTemp1 += len;
						lpSurfaceTemp2 += len;
						lpSurfaceTemp3 += len;
						lpSurfaceTemp4 += len;
						lpSurfaceTemp5 += len;
						lpSurfaceTemp6 += len;
						lpSurfaceTemp7 += len;
						lpSurfaceTemp8 += len;
						lpSurfaceTemp9 += len;
						lpSurfaceTemp10 += len;
						// add by sonic 2006.9.29
						if(g_MyFull)
						{
							lpSurfaceTemp11 += len;
							lpSurfaceTemp12 += len;
						}
						// end by sonic
					}	
				}
				
				pPH++;
			}
		}
		//m_pSurface->Unlock();
		//*/	
	}
}


//----------------------------------------------------------------------
// Determine ImageObject
//----------------------------------------------------------------------
// 拳搁俊 乐绰 ImageObject甸阑 葛滴 八祸茄促.
//----------------------------------------------------------------------
void
MTopView::DetermineImageObject()
{	
	//---------------------------------------------------------------	
	// firstPoint俊 免仿瞪 Zone俊辑狼 霉锅掳 Sector
	//---------------------------------------------------------------	
	// Player甫 吝缴栏肺 秦辑 免仿秦具窍扁 锭巩俊
	// 哭率~~~~ 困~ 何磐 免仿茄促.
	//---------------------------------------------------------------	
	POINT	firstSector;
	firstSector.x = g_pPlayer->GetX() + g_SECTOR_SKIP_PLAYER_LEFT;
	firstSector.y = g_pPlayer->GetY() + g_SECTOR_SKIP_PLAYER_UP;

	//---------------------------------------------------------------	
	// Zone狼 版拌俊 艾阑 锭,
	// 八篮 何盒 救 焊捞霸 窍扁..
	//---------------------------------------------------------------	
	// X 谅钎 嘎眠扁..
	//---------------------------------------------------------------	
	if (firstSector.x <= 0)
	{	
		firstSector.x = 0;			
	}
	else if (firstSector.x+g_SECTOR_WIDTH+1 >= m_pZone->GetWidth())
	{
		firstSector.x = m_pZone->GetWidth()-g_SECTOR_WIDTH-1;
	}

	//---------------------------------------------------------------	
	// Y 谅钎 嘎眠扁
	//---------------------------------------------------------------	
	if (firstSector.y <= 0)
	{
		firstSector.y = 0;
	}
	else if (firstSector.y+g_SECTOR_HEIGHT+1 >= m_pZone->GetHeight())
	{
		firstSector.y = m_pZone->GetHeight()-g_SECTOR_HEIGHT-1;	
	}

	//----------------------------------------------------------------------
	//
	//                         ImageObject 八祸
	//
	//----------------------------------------------------------------------	
	//POINT firstSector = PixelToMap(m_FirstZonePixel.x, m_FirstZonePixel.y);
	int sX1 = firstSector.x + SECTOR_SKIP_LEFT;
	int sY1 = firstSector.y + SECTOR_SKIP_UP;
	int sX2 = firstSector.x + g_SECTOR_WIDTH+1;
	int sY2 = firstSector.y + g_SECTOR_HEIGHT+1;
	
	//------------------------------------------------------
	// Zone狼 康开捞 酒囱 版快俊 Skip...
	//------------------------------------------------------
	if (sX1 < 0) 
	{					
		//tilePoint.x += -sX1 * TILE_X;
		sX1 = 0;	
	}

	if (sX2 >= m_pZone->GetWidth())
	{
		sX2 = m_pZone->GetWidth()-1;
	}

	if (sY1 < 0)
	{
		//tilePoint.y += -sY1 * TILE_Y;
		sY1 = 0;	
	}

	if (sY2 >= m_pZone->GetHeight())
	{
		sY2 = m_pZone->GetHeight()-1;
	}
		
	//------------------------------------------------------
	// 捞傈俊 乐带 ImageObject甫 葛滴 瘤款促.
	//------------------------------------------------------
	m_mapImageObject.clear();	

	//------------------------------------------------------
	// 阿 Sector狼 ImageObject八祸
	//------------------------------------------------------
	int y,x,i;
	for (y=sY1; y<=sY2; y++)
	{				
		for (x=sX1; x<=sX2; x++)
		{				
			const MSector& sector = m_pZone->GetSector(x,y);

			// ImageObject啊 乐促搁.. 葛滴~ 扁撅秦敌促.
			// 窜, 吝汗阑 倾侩窍瘤 臼绰促.
			if (sector.IsExistImageObject())
			{
				OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

				// Sector俊 乐绰 葛电 ImageObject甸阑 八祸茄促.
				for (i=0; i<sector.GetImageObjectSize(); i++)
				{
					MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);

					//----------------------------------------
					// 免仿 矫痢阑 眉农秦具 登瘤父,
					// 绢恫 ImageObject甸篮
					// 磊脚狼 ViewSector啊 酒囱 viewpoint甫
					// 爱绰 版快档 乐促. -_-;
					// 弊贰辑.. 公炼扒 眠啊茄促.
					//----------------------------------------
					// 免仿 矫痢捞 灯栏搁.. 眠啊茄促.
					//if (pImageObject->GetViewpoint()==y				// 沥秦柳 临
					//	|| y==sY2-1									// 场临
					//	|| pImageObject->GetViewpoint() < sY1)		// 霉临
					{						
						// 捞固 乐绰瘤 犬牢秦焊绊 绝栏搁 眠啊.
						QWORD key = GetOutputImageObjectID( pImageObject );

						if (m_mapImageObject.find( key )
							== m_mapImageObject.end())
						{			
							//----------------------------------------
							// Key蔼 = (Viewpoint << 32) | ID
							//----------------------------------------
							// Viewpoint俊 狼秦辑 sort等促.
							// Viewpoint客 ID俊 狼秦辑 力芭且 荐 乐促.
							//----------------------------------------
							m_mapImageObject.insert(
								IMAGEOBJECT_OUTPUT_MAP::value_type(
									key,	// key蔼
									pImageObject
								)
							);
						}
					}
					
					iImageObject++;
				}
			}
		}
	}
}

//----------------------------------------------------------------------
// Update ImageObject
//----------------------------------------------------------------------
// m_FirstPoint绰 官肺 傈狼~ 拳搁 谅钎捞绊..
// parameter狼 firstSector啊 泅犁狼 拳搁 谅钎捞促.
//
// 拳搁俊辑 绝绢瘤绰 ImageObject绰 m_mapImageObject俊辑 绝局绊
// 拳搁俊 唱鸥唱绰 ImageObject绰 m_mapImageObject俊 眠啊茄促.
//----------------------------------------------------------------------
void
MTopView::UpdateImageObject(const POINT &newFirstSector)
{
	//#ifdef OUTPUT_DEBUG
		//	DEBUG_ADD("[TempDebug] Before Update ImageObject");
	//#endif	



	int x,y,i;

	//---------------------------------------------------------------
	// 
	//               ImageObject 盎脚
	//
	//---------------------------------------------------------------
	// newFirstSector客 m_FirstSector(官肺 捞傈狼 谅钎)甫 厚背秦辑
	// 促弗 版快... check~~秦具茄促.
	//---------------------------------------------------------------
	// 惑窍肺 谅钎啊 崔扼柳 版快
	//---------------------------------------------------------------
	if (newFirstSector.y != m_FirstSector.y)
	{		
		int	sX1=newFirstSector.x + SECTOR_SKIP_LEFT, 
				sX2=newFirstSector.x + g_SECTOR_WIDTH+1, 
				sX01=m_FirstSector.x + SECTOR_SKIP_LEFT, 
				sX02=m_FirstSector.x + g_SECTOR_WIDTH+1, 
				eraseY1, eraseY2,		// 瘤况具瞪 巴甸捞 乐绰 临
				lastY,					// 拳搁狼 付瘤阜 临(瘤快搁 救登绰 巴甸)
				newY1, newY2;			// 货肺 唱鸥唱绰 巴甸捞 乐绰 临
		
		// 眉农且 鞘夸啊 乐绰啊?
		bool bCheckLast		= true;
		bool bCheckErase	= true;
		bool bCheckNew		= true;

		//-----------------------------------
		// Zone康开阑 哈绢唱绰 版快
		//-----------------------------------
		if (sX2 >= m_pZone->GetWidth())
		{
			sX2 = m_pZone->GetWidth()-1;
		}

		if (sX1 < 0) 
		{				
			sX1 = 0;	
		}

		if (sX02 >= m_pZone->GetWidth())
		{
			sX02 = m_pZone->GetWidth()-1;
		}

		if (sX01 < 0) 
		{				
			sX01 = 0;	
		}

		//-----------------------------------
		// 困率栏肺 捞悼茄 版快
		//-----------------------------------
		if (newFirstSector.y - m_FirstSector.y < 0)
		{
			// 酒贰率俊辑 荤扼瘤绰 扒拱捞 乐绰瘤 犬牢秦毫具茄促.
			// --> 捞傈 拳搁狼 付瘤阜 临俊绰 乐瘤父,
			//     泅犁 拳搁狼 付瘤阜 临俊绰 绝绰 巴甸篮 荤扼瘤绰 巴捞促.
			
			// 促矫 混副 巴
			lastY	= newFirstSector.y + g_SECTOR_HEIGHT+1;	// 泅犁 拳搁狼 付瘤阜 临

			// 瘤况具 瞪 巴 : lasyY+1 ~ eraseY
			eraseY1	= lastY+1;							
			eraseY2	= m_FirstSector.y + g_SECTOR_HEIGHT+1;	// 捞傈 拳搁狼 付瘤阜 临			

			// 货肺 唱鸥唱绰 巴
			newY1	= newFirstSector.y + SECTOR_SKIP_UP;
			newY2	= m_FirstSector.y + SECTOR_SKIP_UP - 1;

			//------------------------------------------------------
			// Zone狼 康开捞 酒囱 版快俊 Skip...
			//------------------------------------------------------
			if (lastY >= m_pZone->GetHeight())
			{
				//lastY = m_pZone->GetHeight()-1;				
				// 混副 鞘夸 绝促.
				bCheckLast = false;
			}

			if (eraseY1 >= m_pZone->GetHeight())
			{
				//eraseY1 = m_pZone->GetHeight()-1;
				// erase且 鞘夸 绝促. 
				bCheckErase = false;
			}
			else if (eraseY2 >= m_pZone->GetHeight())
			{
				eraseY2 = m_pZone->GetHeight()-1;								
			}		

			if (newY2 < 0) 
			{				
				//newY2 = 0;
				// new且 鞘夸 绝促.
				bCheckNew = false;
			}		
			else if (newY1 < 0) 
			{				
				newY1 = 0;	
			}
			
		}
		//-----------------------------------
		// 酒贰率栏肺 捞悼茄 版快
		//-----------------------------------
		else
		{	
			// 促矫 混副 巴
			lastY	= newFirstSector.y + SECTOR_SKIP_UP;	// 泅犁 拳搁狼 付瘤阜 临

			// 瘤况具瞪 巴 : eraseY ~ lastY-1
			eraseY1	= m_FirstSector.y + SECTOR_SKIP_UP;	// 捞傈 拳搁狼 付瘤阜 临
			eraseY2	= lastY-1;		

			// 货肺 唱鸥唱绰 巴
			newY1	= m_FirstSector.y + g_SECTOR_HEIGHT+1 + 1;
			newY2	= newFirstSector.y + g_SECTOR_HEIGHT+1;

			//------------------------------------------------------
			// Zone狼 康开捞 酒囱 版快俊 Skip...
			//------------------------------------------------------				
			if (lastY < 0) 
			{				
				//lastY = 0;	
				bCheckLast = false;
			}
			
			if (eraseY2 < 0) 
			{				
				//eraseY2 = 0;	
				bCheckErase = false;
			}
			else if (eraseY1 < 0) 
			{				
				eraseY1 = 0;	
			}			

			if (newY1 >= m_pZone->GetHeight())
			{
				//newY1 = m_pZone->GetHeight()-1;				
				bCheckNew = false;
			}
			else if (newY2 >= m_pZone->GetHeight())
			{
				newY2 = m_pZone->GetHeight()-1;				
			}		

		}
		

		//----------------------------------------------
		// erase1 ~ erase2鳖瘤绰 瘤匡父茄(?) 巴甸.
		//----------------------------------------------
		// 老窜 m_mapImageObject俊辑 瘤款促.
		//----------------------------------------------
		if (bCheckErase)
		{
			for (y=eraseY1; y<=eraseY2; y++)
			{
				for (x=sX01; x<=sX02; x++)
				{
					const MSector& sector = m_pZone->GetSector(x,y);

					// ImageObject啊 乐促搁.. 葛滴~ 扁撅秦敌促.
					// 窜, 吝汗阑 倾侩窍瘤 臼绰促.
					if (sector.IsExistImageObject())
					{
						OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

						// Sector俊 乐绰 葛电 ImageObject甸阑 八祸茄促.
						for (int i=0; i<sector.GetImageObjectSize(); i++)
						{
							MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);
							
							//----------------------------------------
							// Key蔼 = (Viewpoint << 32) | ID
							//----------------------------------------
							// 捞固 乐绰瘤 犬牢秦焊绊 乐栏搁 力芭.
							IMAGEOBJECT_OUTPUT_MAP::iterator iDelete = m_mapImageObject.find( GetOutputImageObjectID( pImageObject ) );
							if (iDelete	!= m_mapImageObject.end())
							{			
								m_mapImageObject.erase( iDelete );
							}
								
							iImageObject++;
						}
					}
				}
			}
		}

		//----------------------------------------------
		// lastY绰 促矫 眠啊茄促.
		//----------------------------------------------
		if (bCheckLast)
		{
			for (x=sX1; x<=sX2; x++)
			{
				const MSector& sector = m_pZone->GetSector(x, lastY);

				// ImageObject啊 乐栏搁..
				if (sector.IsExistImageObject())
				{
					OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

					// Sector俊 乐绰 葛电 ImageObject甸阑 八祸茄促.
					for (i=0; i<sector.GetImageObjectSize(); i++)
					{
						MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);

						// 免仿 矫痢捞 灯栏搁.. 眠啊茄促.
						//if (pImageObject->GetViewpoint()==y				// 沥秦柳 临
						//	|| y==sY2-1									// 场临
						//	|| pImageObject->GetViewpoint() < sY1)		// 霉临
						{		
							QWORD key = GetOutputImageObjectID(pImageObject);
							// 捞固 乐绰瘤 犬牢秦焊绊 绝栏搁 眠啊.
							if (m_mapImageObject.find( key )
								== m_mapImageObject.end())
							{			
								//----------------------------------------
								// Key蔼 = (Viewpoint << 32) | ID
								//----------------------------------------
								// Viewpoint俊 狼秦辑 sort等促.
								// Viewpoint客 ID俊 狼秦辑 力芭且 荐 乐促.
								//----------------------------------------
								m_mapImageObject.insert(
									IMAGEOBJECT_OUTPUT_MAP::value_type(
										key,	// key蔼
										pImageObject
									)
								);
							}
						}
							
						iImageObject++;
					}
				}
			}
		}

		//----------------------------------------------
		// 货肺 唱鸥绰 巴甸 : newY1 ~ newY2
		//----------------------------------------------
		if (bCheckNew)
		{
			for (y=newY1; y<=newY2; y++)
			{
				for (x=sX1; x<=sX2; x++)
				{
					const MSector& sector = m_pZone->GetSector(x, y);

					// ImageObject啊 乐栏搁..
					if (sector.IsExistImageObject())
					{
						OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

						// Sector俊 乐绰 葛电 ImageObject甸阑 八祸茄促.
						for (i=0; i<sector.GetImageObjectSize(); i++)
						{
							MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);
							
							// 免仿 矫痢捞 灯栏搁.. 眠啊茄促.						
							//if (viewPoint==y				// 沥秦柳 临
							//	|| viewPoint > newY2									// 场临
							//	|| viewPoint < newY1)		// 霉临
							{						
								// 捞固 乐绰瘤 犬牢秦焊绊 绝栏搁 眠啊.
								QWORD key = GetOutputImageObjectID( pImageObject );

								if (m_mapImageObject.find( key ) == m_mapImageObject.end())
								{			
									//----------------------------------------
									// Key蔼 = (Viewpoint << 32) | ID
									//----------------------------------------
									// Viewpoint俊 狼秦辑 sort等促.
									// Viewpoint客 ID俊 狼秦辑 力芭且 荐 乐促.
									//----------------------------------------
									m_mapImageObject.insert(
										IMAGEOBJECT_OUTPUT_MAP::value_type(
											key,	// key蔼
											pImageObject
										)
									);
								}
							}
								
							iImageObject++;
						}
					}
				}
			}
		}
		
	}

	//---------------------------------------------------------------
	// 谅快肺 谅钎啊 崔扼柳 版快
	//---------------------------------------------------------------
	if (newFirstSector.x != m_FirstSector.x)
	{
		int	sY1=newFirstSector.y + SECTOR_SKIP_UP, 
				sY2=newFirstSector.y + g_SECTOR_HEIGHT+1, 
				sY01=m_FirstSector.y + SECTOR_SKIP_UP, 
				sY02=m_FirstSector.y + g_SECTOR_HEIGHT+1, 
				eraseX1, eraseX2,		// 瘤况具瞪 巴甸捞 乐绰 临
				lastX,					// 拳搁狼 付瘤阜 临(瘤快搁 救登绰 巴甸)
				newX1, newX2;			// 货肺 唱鸥唱绰 巴甸捞 乐绰 临

		// 眉农且 鞘夸啊 乐绰啊?
		bool bCheckLast		= true;
		bool bCheckErase	= true;
		bool bCheckNew		= true;

		//-----------------------------------
		// Zone康开阑 哈绢唱绰 版快
		//-----------------------------------
		if (sY2 >= m_pZone->GetHeight())
		{
			sY2 = m_pZone->GetHeight()-1;
		}

		if (sY1 < 0) 
		{				
			sY1 = 0;	
		}

		if (sY02 >= m_pZone->GetHeight())
		{
			sY02 = m_pZone->GetHeight()-1;
		}

		if (sY01 < 0) 
		{				
			sY01 = 0;	
		}

		//-----------------------------------
		// 哭率栏肺 捞悼茄 版快
		//-----------------------------------
		if (newFirstSector.x - m_FirstSector.x < 0)
		{
			// 坷弗率俊辑 荤扼瘤绰 扒拱捞 乐绰瘤 犬牢秦毫具茄促.
			// --> 捞傈 拳搁狼 付瘤阜 临俊绰 乐瘤父,
			//     泅犁 拳搁狼 付瘤阜 临俊绰 绝绰 巴甸篮 荤扼瘤绰 巴捞促.
			
			// 促矫 混副 巴
			lastX	= newFirstSector.x + g_SECTOR_WIDTH+1;	// 泅犁 拳搁狼 付瘤阜 临

			// 瘤况具 瞪 巴 : lasyX+1 ~ eraseX
			eraseX1	= lastX+1;							
			eraseX2	= m_FirstSector.x + g_SECTOR_WIDTH+1;	// 捞傈 拳搁狼 付瘤阜 临			

			// 货肺 唱鸥唱绰 巴
			newX1	= newFirstSector.x + SECTOR_SKIP_LEFT;
			newX2	= m_FirstSector.x + SECTOR_SKIP_LEFT - 1;

			//------------------------------------------------------
			// Zone狼 康开捞 酒囱 版快俊 Skip...
			//------------------------------------------------------
			if (lastX >= m_pZone->GetWidth())
			{
				//lastX = m_pZone->GetWidth()-1;
				bCheckLast = false;
			}

			if (eraseX1 >= m_pZone->GetWidth())
			{
				//eraseX1 = m_pZone->GetWidth()-1;
				bCheckErase = false;
			}
			else if (eraseX2 >= m_pZone->GetWidth())
			{
				eraseX2 = m_pZone->GetWidth()-1;
			}		

			if (newX2 < 0) 
			{				
				//newX2 = 0;	
				bCheckNew = false;
			}	
			else if (newX1 < 0) 
			{				
				newX1 = 0;	
			}				
			
		}
		//-----------------------------------
		// 坷弗率栏肺 捞悼茄 版快
		//-----------------------------------
		else
		{	
			// 力芭             眠啊      眠啊
			// eraseX1~eraseX2~lastX ~~~~ newX1~newX2
			// 促矫 混副 巴
			lastX	= newFirstSector.x + SECTOR_SKIP_LEFT;	// 泅犁 拳搁狼 付瘤阜 临

			// 瘤况具瞪 巴 : eraseX ~ lastX-1
			eraseX1	= m_FirstSector.x + SECTOR_SKIP_LEFT;	// 捞傈 拳搁狼 付瘤阜 临
			eraseX2	= lastX-1;		

			// 货肺 唱鸥唱绰 巴
			newX1	= m_FirstSector.x + g_SECTOR_WIDTH+1 + 1;
			newX2	= newFirstSector.x + g_SECTOR_WIDTH+1;

			//------------------------------------------------------
			// Zone狼 康开捞 酒囱 版快俊 Skip...
			//------------------------------------------------------				
			if (lastX < 0) 
			{				
				//lastX = 0;
				bCheckLast = false;
			}
			
			if (eraseX2 < 0) 
			{				
				//eraseX2 = 0;
				bCheckErase = false;
			}
			else if (eraseX1 < 0) 
			{				
				eraseX1 = 0;	
			}
			
			if (newX1 >= m_pZone->GetWidth())
			{
				//newX1 = m_pZone->GetWidth()-1;
				bCheckNew = false;
			}
			else if (newX2 >= m_pZone->GetWidth())
			{
				newX2 = m_pZone->GetWidth()-1;
			}	
		}
		

		//----------------------------------------------
		// erase1 ~ erase2鳖瘤绰 瘤匡父茄(?) 巴甸.
		//----------------------------------------------
		// 老窜 m_mapImageObject俊辑 瘤款促.
		//----------------------------------------------
		if (bCheckErase)
		{
			for (x=eraseX1; x<=eraseX2; x++)
			{
				for (y=sY01; y<=sY02; y++)
				{
					const MSector& sector = m_pZone->GetSector(x,y);

					// ImageObject啊 乐促搁.. 葛滴~ 扁撅秦敌促.
					// 窜, 吝汗阑 倾侩窍瘤 臼绰促.
					if (sector.IsExistImageObject())
					{
						OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

						// Sector俊 乐绰 葛电 ImageObject甸阑 八祸茄促.
						for (i=0; i<sector.GetImageObjectSize(); i++)
						{
							MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);
							
							//----------------------------------------
							// Key蔼 = (Viewpoint << 32) | ID
							//----------------------------------------
							// 捞固 乐绰瘤 犬牢秦焊绊 乐栏搁 力芭.
							IMAGEOBJECT_OUTPUT_MAP::iterator iDelete = m_mapImageObject.find( GetOutputImageObjectID( pImageObject ) );
							if (iDelete	!= m_mapImageObject.end())
							{			
								m_mapImageObject.erase( iDelete );
							}
								
							iImageObject++;
						}
					}
				}
			}
		}

		//----------------------------------------------
		// lastX绰 促矫 眠啊茄促.
		//----------------------------------------------
		if (bCheckLast)
		{
			for (y=sY1; y<=sY2; y++)
			{
				const MSector& sector = m_pZone->GetSector(lastX, y);

				// ImageObject啊 乐栏搁..
				if (sector.IsExistImageObject())
				{
					OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

					// Sector俊 乐绰 葛电 ImageObject甸阑 八祸茄促.
					for (i=0; i<sector.GetImageObjectSize(); i++)
					{
						MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);
						
						// 免仿 矫痢捞 灯栏搁.. 眠啊茄促.
						//if (pImageObject->GetViewpoint()==y				// 沥秦柳 临
							//|| y==sY2-1									// 场临
							//|| pImageObject->GetViewpoint() < sY1)		// 霉临
						{						
							// 捞固 乐绰瘤 犬牢秦焊绊 绝栏搁 眠啊.
							QWORD key = GetOutputImageObjectID( pImageObject );

							if (m_mapImageObject.find( key )
								== m_mapImageObject.end())
							{			
								//----------------------------------------
								// Key蔼 = (Viewpoint << 32) | ID
								//----------------------------------------
								// Viewpoint俊 狼秦辑 sort等促.
								// Viewpoint客 ID俊 狼秦辑 力芭且 荐 乐促.
								//----------------------------------------
								m_mapImageObject.insert(
									IMAGEOBJECT_OUTPUT_MAP::value_type(
										key,	// key蔼
										pImageObject
									)
								);
							}
						}
							
						iImageObject++;
					}
				}
			}
		}

		//----------------------------------------------
		// 货肺 唱鸥绰 巴甸 : newX1 ~ newX2
		//----------------------------------------------
		if (bCheckNew)
		{
			for (x=newX1; x<=newX2; x++)
			{
				for (y=sY1; y<=sY2; y++)
				{
					const MSector& sector = m_pZone->GetSector(x, y);

					// ImageObject啊 乐栏搁..
					if (sector.IsExistImageObject())
					{
						OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

						// Sector俊 乐绰 葛电 ImageObject甸阑 八祸茄促.
						for (i=0; i<sector.GetImageObjectSize(); i++)
						{
							MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);
						
							// 免仿 矫痢捞 灯栏搁.. 眠啊茄促.
							//if (viewPoint<=y			// 困俊 波
							//	|| viewPoint > sY2)		// 场临
								//|| viewPoint < newX1)		// 霉临
							{								
								// 捞固 乐绰瘤 犬牢秦焊绊 绝栏搁 眠啊.
								QWORD key = GetOutputImageObjectID( pImageObject );

								if (m_mapImageObject.find( key )
									== m_mapImageObject.end())
								{			
									//----------------------------------------
									// Key蔼 = (Viewpoint << 32) | ID
									//----------------------------------------
									// Viewpoint俊 狼秦辑 sort等促.
									// Viewpoint客 ID俊 狼秦辑 力芭且 荐 乐促.
									//----------------------------------------
									m_mapImageObject.insert(
										IMAGEOBJECT_OUTPUT_MAP::value_type(
											key,	// key蔼
											pImageObject
										)
									);
								}
							}
								
							iImageObject++;
						}
					}
				}
			}
		}
	}

	//#ifdef OUTPUT_DEBUG
		//	DEBUG_ADD("[TempDebug] After Determine ImageObject");
	//#endif	
}

//----------------------------------------------------------------------
// Draw Zone
//----------------------------------------------------------------------
// Pixel 谅钎牢 (Xp, Yp)啊 
//       拳搁狼 (0, 0)俊 坷档废 窍咯 唱赣瘤 何盒阑 免仿茄促.
//
// pSurface狼 (X,Y)俊 Zone狼 (Xp,Yp)何盒何磐 免仿茄促.
//----------------------------------------------------------------------
//
// 捞傈俊 免仿茄 Tile阑 捞侩秦辑 泅犁 Tile阑 备己秦霖 第,
// 促弗 Object甫 免仿茄促.
//
//----------------------------------------------------------------------
void	
MTopView::DrawZone(int firstPointX,int firstPointY)
{
	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "Start DrawZone" );
	#endif

	//-------------------------------------------------
	// ItemName list甫 瘤况霖促.
	//-------------------------------------------------
	ClearItemNameList();	


	// mouse肺 急琶等 某腐磐..
	m_pSelectedCreature = NULL;


	//-------------------------------------------------
	//
	// Object甫 免仿窍扁 困茄 沥焊
	//
	//-------------------------------------------------
	// pointer variable
	//MCreature*		pCreature;
	MItem*			pItem;

	// sprite point
	POINT	tilePoint, point;
	RECT	rect;

	register int x;
	register int y;
	register int i;

	//-------------------------------------------------
	// Player甫 啊府绰 ImageObject甸俊 措茄 沥焊
	//-------------------------------------------------
	//if (m_pZone->GetSector(g_pPlayer->GetX(), g_pPlayer->GetY()).IsImageObjectAndNotBlock())
	//{
		//m_BehindImageObjectID = m_pZone->GetSector(g_pPlayer->GetX(), g_pPlayer->GetY()).GetImageObject()->GetImageObjectID();
	//}
	//else
	//{
		//m_BehindImageObjectID = IMAGEOBJECTID_NULL;
	//}
	

	//-------------------------------------------------
	// 霉 Sector啊 免仿瞪 谅钎 焊沥(smooth scroll阑 困秦辑)
	// Player狼 谅钎甫 吝缴栏肺 秦辑 沥茄促.
	//-------------------------------------------------
	m_PlusPoint.x = g_pPlayer->GetSX() - firstPointX;
	m_PlusPoint.y = g_pPlayer->GetSY() - firstPointY;
	firstPointX -= g_pPlayer->GetSX();
	firstPointY -= g_pPlayer->GetSY();


	//---------------------------------------------------------------	
	// firstPoint俊 免仿瞪 Zone俊辑狼 霉锅掳 Sector
	//---------------------------------------------------------------	
	// Player甫 吝缴栏肺 秦辑 免仿秦具窍扁 锭巩俊
	// 哭率~~~~ 困~ 何磐 免仿茄促.
	//---------------------------------------------------------------	
	POINT	firstSector;
	firstSector.x = g_pPlayer->GetX() + g_SECTOR_SKIP_PLAYER_LEFT;
	firstSector.y = g_pPlayer->GetY() + g_SECTOR_SKIP_PLAYER_UP;

	//---------------------------------------------------------------	
	// Zone狼 版拌俊 艾阑 锭,
	// 八篮 何盒 救 焊捞霸 窍扁..
	//---------------------------------------------------------------	
	// X 谅钎 嘎眠扁..
	//---------------------------------------------------------------	
	if (firstSector.x <= 0)
	{
		if (firstSector.x==0)
		{
			if (g_pPlayer->GetSX() < 0)
			{
				m_PlusPoint.x = 0;
				firstPointX = 0;				
			}			
		}
		else
		{
			m_PlusPoint.x = 0;		
			firstPointX = 0;			
		}

		firstSector.x = 0;			
	}
	if (firstSector.x+g_SECTOR_WIDTH+1 >= m_pZone->GetWidth())
	{
		if (firstSector.x+g_SECTOR_WIDTH+1==m_pZone->GetWidth())
		{
			if (g_pPlayer->GetSX() > 0)
			{
				m_PlusPoint.x = 0;
				firstPointX = 0;				
			}			
		}
		else
		{
			m_PlusPoint.x = 0;	
			firstPointX = 0;			
		}

		firstSector.x = m_pZone->GetWidth()-g_SECTOR_WIDTH-1;
	}

	//---------------------------------------------------------------	
	// Y 谅钎 嘎眠扁
	//---------------------------------------------------------------	
	if (firstSector.y <= 0)
	{
		if (firstSector.y==0)
		{
			if (g_pPlayer->GetSY() < 0)
			{
				m_PlusPoint.y = 0;
				firstPointY = 0;
			}			
		}
		else
		{
			m_PlusPoint.y = 0;
			firstPointY = 0;
		}

		firstSector.y = 0;
	}
	else if (firstSector.y+g_SECTOR_HEIGHT >= m_pZone->GetHeight())
	{
		if (firstSector.y+g_SECTOR_HEIGHT == m_pZone->GetHeight())
		{
			if (g_pPlayer->GetSY() > 0)
			{
				m_PlusPoint.y = 0;
				firstPointY = 0;
			}			
		}
		else
		{
			m_PlusPoint.y = 0;	
			firstPointY = 0;
		}

		firstSector.y = m_pZone->GetHeight()-g_SECTOR_HEIGHT;	
	}

	//---------------------------------------------------------------
	// 货肺 唱鸥唱芭唱 拳搁俊辑 绝绢瘤绰 ImageObject八祸
	//---------------------------------------------------------------
	// 2001.8.22

	__BEGIN_PROFILE("UpdateImageObject")

	UpdateImageObject(firstSector);	

	__END_PROFILE("UpdateImageObject")

	
	//---------------------------------------------------------------
	// 寇何俊辑 鞘夸茄 沥焊甫 积己..
	// 拳搁 惑狼 霉 困摹俊 免仿登绰 Sector狼 谅钎
	//---------------------------------------------------------------
	m_FirstSector	= firstSector;	

	//---------------------------------------------------------------
	// 拳搁狼 (0,0)捞 唱鸥郴绰 Zone狼 Pixel谅钎
	//---------------------------------------------------------------
	m_FirstZonePixel = MapToPixel(firstSector.x, firstSector.y);
	m_FirstZonePixel.x += m_PlusPoint.x;
	m_FirstZonePixel.y += m_PlusPoint.y;

	#ifdef OUTPUT_DEBUG		
		if (g_pPlayer->GetX() < 0 
			|| g_pPlayer->GetY() < 0 
			|| g_pPlayer->GetX() >= g_pZone->GetWidth()
			|| g_pPlayer->GetY() >= g_pZone->GetHeight()
			|| m_FirstZonePixel.x > 12000 || m_FirstZonePixel.y > 6144
			|| m_FirstZonePixel.x < 0 || m_FirstZonePixel.y < 0)
		{
			if (g_pDebugMessage)
			{
				DEBUG_ADD_FORMAT("[Error]SX,SY-DrawZone:Player=(%d, %d), s(%d,%d)", g_pPlayer->GetX(), g_pPlayer->GetY(), g_pPlayer->GetSX(), g_pPlayer->GetSY());
			}
		}
	#endif
	

	//---------------------------------------------------------------
	// Player甫 啊府绰 ImageObject甸阑 贸府且 Filter狼 免仿 谅钎
	//---------------------------------------------------------------
	m_FilterPosition.x = g_pPlayer->GetPixelX() - m_FirstZonePixel.x - (m_ImageObjectFilter.GetWidth()>>1) + g_TILE_X_HALF;
	m_FilterPosition.y = g_pPlayer->GetPixelY() - m_FirstZonePixel.y - (m_ImageObjectFilter.GetHeight()>>1) 
						- (g_pPlayer->IsFlyingCreature()? 3:1 )*TILE_Y;
	//filterPoint.x = 310;	// 410 - 100;
	//filterPoint.y = 116;	// 204 - 100;


	//short	sX,
	//		sY = (short)firstSector.y;

	// 免仿且 Surface惑狼 困摹	
	tilePoint.y = firstPointY;


	//--------------------------------------------------
	// TileSurface狼 康开 : m_TileSurfaceFirstPixelXY 
	//                      + (TILESURFACE_WIDTH+ TILESURFACE_HEIGHT)
	//
	// 泅犁 拳搁狼 康开   : m_FirstZonePixel + (800,600)
	//--------------------------------------------------
	//
	// 泅犁 拳搁 康开捞 TileSurface俊 加窍搁 
	// TileSurface狼 老何甫 弊措肺 捞侩窍搁 等促.
	//
	//
	//--------------------------------------------------
	__BEGIN_PROFILE("ReuseTileSurface")

	RECT rectTileSurface;
	RECT rectScreen;
	RECT	rectReuse;

	// TileSurface狼 Zone俊辑狼 康开
	rectTileSurface.left	= m_TileSurfaceFirstZonePixelX;
	rectTileSurface.top		= m_TileSurfaceFirstZonePixelY;
	rectTileSurface.right	= m_TileSurfaceFirstZonePixelX + g_TILESURFACE_WIDTH;
	rectTileSurface.bottom	= m_TileSurfaceFirstZonePixelY + g_TILESURFACE_HEIGHT;

	// 泅犁 拳搁狼 康开
	rectScreen.left		= m_FirstZonePixel.x;
	rectScreen.top		= m_FirstZonePixel.y;
	rectScreen.right	= m_FirstZonePixel.x + g_GameRect.right;
	rectScreen.bottom	= m_FirstZonePixel.y + g_GameRect.bottom;

	// 泅犁 拳搁 免仿俊 捞侩且 TileSurface狼 康开阑 沥茄促.
	// 拳搁狼 (0,0)捞 瞪 TileSurface俊辑狼 霉 痢
	rectReuse.left		= rectScreen.left - rectTileSurface.left;
	rectReuse.top		= rectScreen.top - rectTileSurface.top;
	rectReuse.right		= rectReuse.left + g_GameRect.right;
	rectReuse.bottom	= rectReuse.top + g_GameRect.bottom;

	//----------------------------------------------------------------	
	// rectReuse甫 力寇茄 何盒篮 八篮祸栏肺 磨秦拎具 茄促.
	//----------------------------------------------------------------
	/*
	// 困率
	if (rectReuse.top != 0)
	{
		rect.left = 0;
		rect.right = SURFACE_WIDTH;
		rect.top = 0;
		rect.bottom = rectReuse.top;

		m_pSurface->FillRect(&rect, 0);
	}

	// 酒贰率
	if (rectReuse.bottom != SURFACE_HEIGHT)
	{
		rect.left = 0;
		rect.right = SURFACE_WIDTH;
		rect.top = rectReuse.bottom;
		rect.bottom = SURFACE_HEIGHT;

		m_pSurface->FillRect(&rect, 0);
	}

	// 哭率
	if (rectReuse.left != 0)
	{
		rect.left = 0;
		rect.right = rectReuse.left;
		rect.top = rectReuse.top;
		rect.bottom = rectReuse.bottom;	

		m_pSurface->FillRect(&rect, 0);
	}

	// 坷弗率
	if (rectReuse.right != SURFACE_WIDTH)
	{
		rect.left = rectReuse.right;
		rect.right = SURFACE_WIDTH;
		rect.top = rectReuse.top;
		rect.bottom = rectReuse.bottom;	

		m_pSurface->FillRect(&rect, 0);
	}
	*/



	//--------------------------------------------------
	// TileSurface甫 弊措肺 镜 荐 乐绰瘤 眉农茄促.
	//--------------------------------------------------
	// 何练茄 何盒 钎矫	
	BYTE bLack = 0;
	 
	//bLack |= (rectScreen.left <= rectTileSurface.left);				// 1
	//bLack |= (rectScreen.right >= rectTileSurface.right) << 1;		// 2
	//bLack |= (rectScreen.top <= rectTileSurface.top) << 2;			// 4
	//bLack |= (rectScreen.bottom >= rectTileSurface.bottom) << 3;	// 8
	int leftGap = rectScreen.left - rectTileSurface.left;
	int rightGap = rectScreen.right - rectTileSurface.right;
	int topGap = rectScreen.top - rectTileSurface.top;
	int bottomGap = rectScreen.bottom - rectTileSurface.bottom;

	if (leftGap <= 0) bLack |= FLAG_TILESURFACE_LACK_LEFT;
	if (rightGap >= 0) bLack |= FLAG_TILESURFACE_LACK_RIGHT;
	if (topGap <= 0) bLack |= FLAG_TILESURFACE_LACK_UP;
	if (bottomGap >= 0) bLack |= FLAG_TILESURFACE_LACK_DOWN;	

	//DEBUG_ADD_FORMAT("[DRAW LACK]  (%d, %d)  %s %s %s %s", 
	//	g_pPlayer->GetX(), g_pPlayer->GetY(),
	//	((bLack & FLAG_TILESURFACE_LACK_LEFT)? "Left":" "),
	//	((bLack & FLAG_TILESURFACE_LACK_RIGHT)? "Right":" "),
	//	((bLack & FLAG_TILESURFACE_LACK_UP)? "Up":" "),
	//	((bLack & FLAG_TILESURFACE_LACK_DOWN)? "Down":" "));

	//---------------------------------------------------------------------
	//
	// 泅犁 拳搁捞 TileSurface俊 加窍绰 版快
	//
	//---------------------------------------------------------------------
	if (bLack==0)
	{
		//....
		//---------------------------------------------------------------
		// 货肺 唱鸥唱芭唱 拳搁俊辑 绝绢瘤绰 ImageObject八祸
		//---------------------------------------------------------------
		// 2001.8.22
		//UpdateImageObject(firstSector);	

	}
	/*
	//---------------------------------------------------------------------
	//
	// 呈公 腹捞 盎脚秦具窍绰 版快
	//
	//---------------------------------------------------------------------
	else if (abs(leftGap) >= (TILE_X<<2)
			|| abs(rightGap) >= (TILE_X<<2)
			|| abs(topGap) >= (TILE_Y<<2)
			|| abs(bottomGap) >= (TILE_Y<<2)
			)
	{
		DEBUG_ADD("[DrawZone] Too Far Move");
		
		m_mapImageObject.clear();
		m_mapCreature.clear();
		
		// Tile傈眉甫 促矫 弊妨霖促.
		DrawTileSurface();

		DetermineImageObject();		

		m_bFirstTileDraw = false;

		DEBUG_ADD("[DrawZone] Too Far Move - DrawTileSurface OK");		
	}
	*/
	//---------------------------------------------------------------------
	//
	// 泅犁 拳搁捞 TileSurface俊 加窍瘤 臼绰 版快
	//
	//---------------------------------------------------------------------
	else
	{
		//---------------------------------------------------------------
		// 货肺 唱鸥唱芭唱 拳搁俊辑 绝绢瘤绰 ImageObject八祸
		//---------------------------------------------------------------
		// 2001.8.22
		//UpdateImageObject(firstSector);	

		//----------------------------------------
		// 何盒利栏肺父 盎脚窍绰单 鞘夸茄 沥焊
		//----------------------------------------
		BYTE	changeCount = 0;	// 1 or 2
		int		sX1[2], sX2[2], sY1[2], sY2[2];
		POINT	firstTilePoint[2];			


		//--------------------------
		// LEFT....
		//--------------------------
		if (bLack & FLAG_TILESURFACE_LACK_LEFT)
		{
			//--------------------------
			// LEFT + UP 何练
			//--------------------------
			if (bLack & FLAG_TILESURFACE_LACK_UP)
			{
				//--------------------------------------------------
				// 捞傈 Tile Surface俊辑
				// 促矫 备己且妨绰 Tile Surface俊辑 荤侩且 荐 
				// 乐绰 何盒阑 巢败敌促.			
				//--------------------------------------------------
				point.x = TILESURFACE_OUTLINE_LEFT;
				point.y = TILESURFACE_OUTLINE_UP;
				rect.left	= 0;
				rect.top	= 0;
				rect.right	= g_TILESURFACE_OUTLINE_RIGHT;
				rect.bottom = g_TILESURFACE_OUTLINE_DOWN;
				m_pTileSurface->BltNoColorkey(&point, m_pTileSurface, &rect);

				//--------------------------------------------------
				//
				// 何练茄 何盒阑 弊妨霖促.
				//
				//--------------------------------------------------				

				changeCount = 2;

				//--------------------------------------------------
				// Left何盒
				//--------------------------------------------------
				// 免仿且 sector (sX1,sY) ~ (sX2, sY2)鳖瘤 免仿
				//--------------------------------------------------			
				sX1[0] = m_TileSurfaceFirstSectorX - TILESURFACE_SECTOR_EDGE;
				sY1[0] = m_TileSurfaceFirstSectorY - TILESURFACE_SECTOR_EDGE;
				sX2[0] = sX1[0]	+ TILESURFACE_SECTOR_EDGE;
				sY2[0] = sY1[0] + g_TILESURFACE_SECTOR_HEIGHT;
					
				//--------------------------------------------------
				// 霉锅掳 免仿 谅钎
				//--------------------------------------------------
				firstTilePoint[0].x = 0;
				firstTilePoint[0].y = 0;

				//--------------------------------------------------
				// Up何盒
				//--------------------------------------------------
				// 免仿且 sector (sX1,sY) ~ (sX2, sY2)鳖瘤 免仿
				//--------------------------------------------------			
				sX1[1] = m_TileSurfaceFirstSectorX - TILESURFACE_SECTOR_EDGE;
				sY1[1] = m_TileSurfaceFirstSectorY - TILESURFACE_SECTOR_EDGE;
				sX2[1] = sX1[1]	+ g_TILESURFACE_SECTOR_WIDTH;
				sY2[1] = sY1[1] + TILESURFACE_SECTOR_EDGE;
					
				//--------------------------------------------------
				// 霉锅掳 免仿 谅钎
				//--------------------------------------------------
				firstTilePoint[1].x = 0;
				firstTilePoint[1].y = 0;


				//--------------------------------------------------
				// 免仿俊 荤侩且 TileSurface 康开 促矫 汲沥
				//--------------------------------------------------
				rectReuse.left += TILESURFACE_OUTLINE_LEFT;
				rectReuse.right += TILESURFACE_OUTLINE_LEFT;
				rectReuse.top += TILESURFACE_OUTLINE_UP;
				rectReuse.bottom += TILESURFACE_OUTLINE_UP;

				//--------------------------------------------------
				// 盎脚等 TileSurface俊 措茄 谅钎 汲沥
				//--------------------------------------------------
				m_TileSurfaceFirstSectorX -= TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelX -= TILESURFACE_OUTLINE_LEFT;
				m_TileSurfaceFirstSectorY -= TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelY -= TILESURFACE_OUTLINE_UP;
			}			
			//--------------------------
			// LEFT + DOWN 何练
			//--------------------------
			else if (bLack & FLAG_TILESURFACE_LACK_DOWN)
			{
				//--------------------------------------------------
				// 捞傈 Tile Surface俊辑
				// 促矫 备己且妨绰 Tile Surface俊辑 荤侩且 荐 
				// 乐绰 何盒阑 巢败敌促.			
				//--------------------------------------------------
				point.x = TILESURFACE_OUTLINE_LEFT;
				point.y = 0;
				rect.left	= 0;
				rect.top	= TILESURFACE_OUTLINE_UP;
				rect.right	= g_TILESURFACE_OUTLINE_RIGHT;
				rect.bottom = g_TILESURFACE_HEIGHT;			
				m_pTileSurface->BltNoColorkey(&point, m_pTileSurface, &rect);

				//--------------------------------------------------
				//
				// 何练茄 何盒阑 弊妨霖促.
				//
				//--------------------------------------------------

				changeCount = 2;

				//--------------------------------------------------
				// Left何盒
				//--------------------------------------------------
				// 免仿且 sector (sX1,sY) ~ (sX2, sY2)鳖瘤 免仿
				//--------------------------------------------------			
				sX1[0] = m_TileSurfaceFirstSectorX - TILESURFACE_SECTOR_EDGE;
				sY1[0] = m_TileSurfaceFirstSectorY + TILESURFACE_SECTOR_EDGE;
				sX2[0] = sX1[0]	+ TILESURFACE_SECTOR_EDGE;
				sY2[0] = sY1[0] + g_TILESURFACE_SECTOR_HEIGHT;
					
				//--------------------------------------------------
				// 霉锅掳 免仿 谅钎
				//--------------------------------------------------
				firstTilePoint[0].x = 0;
				firstTilePoint[0].y = 0;

				//--------------------------------------------------
				// Down何盒
				//--------------------------------------------------
				// 免仿且 sector (sX1,sY) ~ (sX2, sY2)鳖瘤 免仿
				//--------------------------------------------------			
				sX1[1] = m_TileSurfaceFirstSectorX - TILESURFACE_SECTOR_EDGE;
				sY1[1] = m_TileSurfaceFirstSectorY + g_TILESURFACE_SECTOR_HEIGHT;
				sX2[1] = sX1[1]	+ g_TILESURFACE_SECTOR_WIDTH;
				sY2[1] = sY1[1] + TILESURFACE_SECTOR_EDGE;
					
				//--------------------------------------------------
				// 霉锅掳 免仿 谅钎
				//--------------------------------------------------
				firstTilePoint[1].x = 0;
				firstTilePoint[1].y = g_TILESURFACE_OUTLINE_DOWN;

				//--------------------------------------------------
				// 免仿俊 荤侩且 TileSurface 康开 促矫 汲沥
				//--------------------------------------------------
				rectReuse.left += TILESURFACE_OUTLINE_LEFT;
				rectReuse.right += TILESURFACE_OUTLINE_LEFT;
				rectReuse.top -= TILESURFACE_OUTLINE_UP;
				rectReuse.bottom -= TILESURFACE_OUTLINE_UP;

				//--------------------------------------------------
				// 盎脚等 TileSurface俊 措茄 谅钎 汲沥
				//--------------------------------------------------
				m_TileSurfaceFirstSectorX -= TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelX -= TILESURFACE_OUTLINE_LEFT;
				m_TileSurfaceFirstSectorY += TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelY += TILESURFACE_OUTLINE_UP;
			}
			//--------------------------
			// LEFT 何练
			//--------------------------
			else
			{
				//--------------------------------------------------
				// 捞傈 Tile Surface俊辑
				// 促矫 备己且妨绰 Tile Surface俊辑 荤侩且 荐 
				// 乐绰 何盒阑 巢败敌促.			
				//--------------------------------------------------
				point.x = TILESURFACE_OUTLINE_LEFT;
				point.y = 0;
				rect.left	= 0;
				rect.top	= 0;
				rect.right	= g_TILESURFACE_OUTLINE_RIGHT;
				rect.bottom = g_TILESURFACE_HEIGHT;			
				m_pTileSurface->BltNoColorkey(&point, m_pTileSurface, &rect);

				//--------------------------------------------------
				//
				// 何练茄 何盒阑 弊妨霖促.
				//
				//--------------------------------------------------

				changeCount = 1;

				//--------------------------------------------------
				// 免仿且 sector (sX1,sY) ~ (sX2, sY2)鳖瘤 免仿
				//--------------------------------------------------			
				sX1[0] = m_TileSurfaceFirstSectorX - TILESURFACE_SECTOR_EDGE;
				sY1[0] = m_TileSurfaceFirstSectorY;
				sX2[0] = sX1[0]	+ TILESURFACE_SECTOR_EDGE;
				sY2[0] = sY1[0] + g_TILESURFACE_SECTOR_HEIGHT;
					
				//--------------------------------------------------
				// 霉锅掳 免仿 谅钎
				//--------------------------------------------------
				firstTilePoint[0].x = 0;
				firstTilePoint[0].y = 0;

				//--------------------------------------------------
				// 免仿俊 荤侩且 TileSurface 康开 促矫 汲沥
				//--------------------------------------------------
				rectReuse.left += TILESURFACE_OUTLINE_LEFT;
				rectReuse.right += TILESURFACE_OUTLINE_LEFT;

				//--------------------------------------------------
				// 盎脚等 TileSurface俊 措茄 谅钎 汲沥
				//--------------------------------------------------
				m_TileSurfaceFirstSectorX -= TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelX -= TILESURFACE_OUTLINE_LEFT;
			}
		}		
		//--------------------------
		// RIGHT....
		//--------------------------
		else if (bLack & FLAG_TILESURFACE_LACK_RIGHT)
		{
			//--------------------------
			// RIGHT + UP 何练
			//--------------------------
			if (bLack & FLAG_TILESURFACE_LACK_UP)
			{
				//--------------------------------------------------
				// 捞傈 Tile Surface俊辑
				// 促矫 备己且妨绰 Tile Surface俊辑 荤侩且 荐 
				// 乐绰 何盒阑 巢败敌促.			
				//--------------------------------------------------
				point.x = 0;
				point.y = TILESURFACE_OUTLINE_UP;
				rect.left	= TILESURFACE_OUTLINE_LEFT;
				rect.top	= 0;
				rect.right	= g_TILESURFACE_WIDTH;
				rect.bottom = g_TILESURFACE_OUTLINE_DOWN;
				m_pTileSurface->BltNoColorkey(&point, m_pTileSurface, &rect);

				//--------------------------------------------------
				//
				// 何练茄 何盒阑 弊妨霖促.
				//
				//--------------------------------------------------

				changeCount = 2;

				//--------------------------------------------------
				// Right何盒
				//--------------------------------------------------
				// 免仿且 sector (sX1,sY) ~ (sX2, sY2)鳖瘤 免仿
				//--------------------------------------------------			
				sX1[0] = m_TileSurfaceFirstSectorX + g_TILESURFACE_SECTOR_WIDTH;
				sY1[0] = m_TileSurfaceFirstSectorY - TILESURFACE_SECTOR_OUTLINE_UP;
				sX2[0] = sX1[0]	+ TILESURFACE_SECTOR_EDGE;
				sY2[0] = sY1[0] + g_TILESURFACE_SECTOR_HEIGHT;
					
				//--------------------------------------------------
				// 霉锅掳 免仿 谅钎
				//--------------------------------------------------
				firstTilePoint[0].x = g_TILESURFACE_OUTLINE_RIGHT;
				firstTilePoint[0].y = 0;

				//--------------------------------------------------
				// UP何盒
				//--------------------------------------------------
				// 免仿且 sector (sX1,sY) ~ (sX2, sY2)鳖瘤 免仿
				//--------------------------------------------------			
				sX1[1] = m_TileSurfaceFirstSectorX + TILESURFACE_SECTOR_OUTLINE_LEFT;
				sY1[1] = m_TileSurfaceFirstSectorY - TILESURFACE_SECTOR_EDGE;
				sX2[1] = sX1[1]	+ g_TILESURFACE_SECTOR_WIDTH;
				sY2[1] = sY1[1] + TILESURFACE_SECTOR_EDGE;
					
				//--------------------------------------------------
				// 霉锅掳 免仿 谅钎
				//--------------------------------------------------
				firstTilePoint[1].x = 0;
				firstTilePoint[1].y = 0;

				//--------------------------------------------------
				// 免仿俊 荤侩且 TileSurface 康开 促矫 汲沥
				//--------------------------------------------------
				rectReuse.left -= TILESURFACE_OUTLINE_LEFT;
				rectReuse.right -= TILESURFACE_OUTLINE_LEFT;
				rectReuse.top += TILESURFACE_OUTLINE_UP;
				rectReuse.bottom += TILESURFACE_OUTLINE_UP;

				//--------------------------------------------------
				// 盎脚等 TileSurface俊 措茄 谅钎 汲沥
				//--------------------------------------------------
				m_TileSurfaceFirstSectorX += TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelX += TILESURFACE_OUTLINE_LEFT;
				m_TileSurfaceFirstSectorY -= TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelY -= TILESURFACE_OUTLINE_UP;
			}
			//--------------------------
			// RIGHT + DOWN 何练
			//--------------------------
			else if (bLack & FLAG_TILESURFACE_LACK_DOWN)
			{
				//--------------------------------------------------
				// 捞傈 Tile Surface俊辑
				// 促矫 备己且妨绰 Tile Surface俊辑 荤侩且 荐 
				// 乐绰 何盒阑 巢败敌促.			
				//--------------------------------------------------
				point.x = 0;
				point.y = 0;
				rect.left	= TILESURFACE_OUTLINE_LEFT;
				rect.top	= TILESURFACE_OUTLINE_UP;
				rect.right	= g_TILESURFACE_WIDTH;
				rect.bottom = g_TILESURFACE_HEIGHT;			
				m_pTileSurface->BltNoColorkey(&point, m_pTileSurface, &rect);

				//--------------------------------------------------
				//
				// 何练茄 何盒阑 弊妨霖促.
				//
				//--------------------------------------------------

				changeCount = 2;

				//--------------------------------------------------
				// Right何盒
				//--------------------------------------------------
				// 免仿且 sector (sX1,sY) ~ (sX2, sY2)鳖瘤 免仿
				//--------------------------------------------------			
				sX1[0] = m_TileSurfaceFirstSectorX + g_TILESURFACE_SECTOR_WIDTH;
				sY1[0] = m_TileSurfaceFirstSectorY + TILESURFACE_SECTOR_OUTLINE_UP;
				sX2[0] = sX1[0]	+ TILESURFACE_SECTOR_EDGE;
				sY2[0] = sY1[0] + g_TILESURFACE_SECTOR_HEIGHT;
					
				//--------------------------------------------------
				// 霉锅掳 免仿 谅钎
				//--------------------------------------------------
				firstTilePoint[0].x = g_TILESURFACE_OUTLINE_RIGHT;
				firstTilePoint[0].y = 0;

				//--------------------------------------------------
				// Down何盒
				//--------------------------------------------------
				// 免仿且 sector (sX1,sY) ~ (sX2, sY2)鳖瘤 免仿
				//--------------------------------------------------			
				sX1[1] = m_TileSurfaceFirstSectorX + TILESURFACE_SECTOR_OUTLINE_LEFT;
				sY1[1] = m_TileSurfaceFirstSectorY + g_TILESURFACE_SECTOR_HEIGHT;
				sX2[1] = sX1[1]	+ g_TILESURFACE_SECTOR_WIDTH;
				sY2[1] = sY1[1] + TILESURFACE_SECTOR_EDGE;
					
				//--------------------------------------------------
				// 霉锅掳 免仿 谅钎
				//--------------------------------------------------
				firstTilePoint[1].x = 0;
				firstTilePoint[1].y = g_TILESURFACE_OUTLINE_DOWN;

				//--------------------------------------------------
				// 免仿俊 荤侩且 TileSurface 康开 促矫 汲沥
				//--------------------------------------------------
				rectReuse.left -= TILESURFACE_OUTLINE_LEFT;
				rectReuse.right -= TILESURFACE_OUTLINE_LEFT;
				rectReuse.top -= TILESURFACE_OUTLINE_UP;
				rectReuse.bottom -= TILESURFACE_OUTLINE_UP;

				//--------------------------------------------------
				// 盎脚等 TileSurface俊 措茄 谅钎 汲沥
				//--------------------------------------------------
				m_TileSurfaceFirstSectorX += TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelX += TILESURFACE_OUTLINE_LEFT;
				m_TileSurfaceFirstSectorY += TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelY += TILESURFACE_OUTLINE_UP;
			}
			//--------------------------
			// RIGHT 何练
			//--------------------------
			else
			{
				//--------------------------------------------------
				// 捞傈 Tile Surface俊辑
				// 促矫 备己且妨绰 Tile Surface俊辑 荤侩且 荐 
				// 乐绰 何盒阑 巢败敌促.			
				//--------------------------------------------------
				point.x = 0;
				point.y = 0;
				rect.left	= TILESURFACE_OUTLINE_LEFT;
				rect.top	= 0;
				rect.right	= g_TILESURFACE_WIDTH;
				rect.bottom = g_TILESURFACE_HEIGHT;			
				m_pTileSurface->BltNoColorkey(&point, m_pTileSurface, &rect);

				//--------------------------------------------------
				//
				// 何练茄 何盒阑 弊妨霖促.
				//
				//--------------------------------------------------

				changeCount = 1;

				//--------------------------------------------------
				// 免仿且 sector (sX1,sY) ~ (sX2, sY2)鳖瘤 免仿
				//--------------------------------------------------			
				sX1[0] = m_TileSurfaceFirstSectorX + g_TILESURFACE_SECTOR_WIDTH;
				sY1[0] = m_TileSurfaceFirstSectorY;
				sX2[0] = sX1[0]	+ TILESURFACE_SECTOR_EDGE;
				sY2[0] = sY1[0] + g_TILESURFACE_SECTOR_HEIGHT;
					
				//--------------------------------------------------
				// 霉锅掳 免仿 谅钎
				//--------------------------------------------------
				firstTilePoint[0].x = g_TILESURFACE_OUTLINE_RIGHT;
				firstTilePoint[0].y = 0;

				//--------------------------------------------------
				// 免仿俊 荤侩且 TileSurface 康开 促矫 汲沥
				//--------------------------------------------------
				rectReuse.left -= TILESURFACE_OUTLINE_LEFT;
				rectReuse.right -= TILESURFACE_OUTLINE_LEFT;

				//--------------------------------------------------
				// 盎脚等 TileSurface俊 措茄 谅钎 汲沥
				//--------------------------------------------------
				m_TileSurfaceFirstSectorX += TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelX += TILESURFACE_OUTLINE_LEFT;
			}
		}
		//--------------------------
		// UP 何练
		//--------------------------
		else if (bLack & FLAG_TILESURFACE_LACK_UP)
		{
			//--------------------------------------------------
			// 捞傈 Tile Surface俊辑
			// 促矫 备己且妨绰 Tile Surface俊辑 荤侩且 荐 
			// 乐绰 何盒阑 巢败敌促.			
			//--------------------------------------------------
			point.x = 0;
			point.y = TILESURFACE_OUTLINE_UP;
			rect.left	= 0;
			rect.top	= 0;
			rect.right	= g_TILESURFACE_WIDTH;
			rect.bottom = g_TILESURFACE_OUTLINE_DOWN;
			m_pTileSurface->BltNoColorkey(&point, m_pTileSurface, &rect);

			//--------------------------------------------------
			//
			// 何练茄 何盒阑 弊妨霖促.
			//
			//--------------------------------------------------

			changeCount = 1;

			//--------------------------------------------------
			// 免仿且 sector (sX1,sY) ~ (sX2, sY2)鳖瘤 免仿
			//--------------------------------------------------			
			sX1[0] = m_TileSurfaceFirstSectorX;
			sY1[0] = m_TileSurfaceFirstSectorY - TILESURFACE_SECTOR_EDGE;
			sX2[0] = sX1[0]	+ g_TILESURFACE_SECTOR_WIDTH;
			sY2[0] = sY1[0] + TILESURFACE_SECTOR_EDGE;
				
			//--------------------------------------------------
			// 霉锅掳 免仿 谅钎
			//--------------------------------------------------
			firstTilePoint[0].x = 0;
			firstTilePoint[0].y = 0;

			//--------------------------------------------------
			// 免仿俊 荤侩且 TileSurface 康开 促矫 汲沥
			//--------------------------------------------------
			rectReuse.top += TILESURFACE_OUTLINE_UP;
			rectReuse.bottom += TILESURFACE_OUTLINE_UP;

			//--------------------------------------------------
			// 盎脚等 TileSurface俊 措茄 谅钎 汲沥
			//--------------------------------------------------
			m_TileSurfaceFirstSectorY -= TILESURFACE_SECTOR_EDGE;
			m_TileSurfaceFirstZonePixelY -= TILESURFACE_OUTLINE_UP;
		}
		//--------------------------
		// DOWN 何练
		//--------------------------
		else if (bLack & FLAG_TILESURFACE_LACK_DOWN)
		{
			//--------------------------------------------------
			// 捞傈 Tile Surface俊辑
			// 促矫 备己且妨绰 Tile Surface俊辑 荤侩且 荐 
			// 乐绰 何盒阑 巢败敌促.			
			//--------------------------------------------------
			point.x = 0;
			point.y = 0;
			rect.left	= 0;
			rect.top	= TILESURFACE_OUTLINE_UP;
			rect.right	= g_TILESURFACE_WIDTH;
			rect.bottom = g_TILESURFACE_HEIGHT;
			m_pTileSurface->BltNoColorkey(&point, m_pTileSurface, &rect);

			//--------------------------------------------------
			//
			// 何练茄 何盒阑 弊妨霖促.
			//
			//--------------------------------------------------

			changeCount = 1;

			//--------------------------------------------------
			// 免仿且 sector (sX1,sY) ~ (sX2, sY2)鳖瘤 免仿
			//--------------------------------------------------			
			sX1[0] = m_TileSurfaceFirstSectorX;
			sY1[0] = m_TileSurfaceFirstSectorY + g_TILESURFACE_SECTOR_HEIGHT;
			sX2[0] = sX1[0]	+ g_TILESURFACE_SECTOR_WIDTH;
			sY2[0] = sY1[0] + TILESURFACE_SECTOR_EDGE;
				
			//--------------------------------------------------
			// 霉锅掳 免仿 谅钎
			//--------------------------------------------------
			firstTilePoint[0].x = 0;
			firstTilePoint[0].y = g_TILESURFACE_OUTLINE_DOWN;

			//--------------------------------------------------
			// 免仿俊 荤侩且 TileSurface 康开 促矫 汲沥
			//--------------------------------------------------
			rectReuse.top -= TILESURFACE_OUTLINE_UP;
			rectReuse.bottom -= TILESURFACE_OUTLINE_UP;

			//--------------------------------------------------
			// 盎脚等 TileSurface俊 措茄 谅钎 汲沥
			//--------------------------------------------------
			m_TileSurfaceFirstSectorY += TILESURFACE_SECTOR_EDGE;
			m_TileSurfaceFirstZonePixelY += TILESURFACE_OUTLINE_UP;
		}


		//------------------------------------------------------
		//
		//   何盒利栏肺 Tile甸阑 盎脚窍扁..
		//
		//------------------------------------------------------
		register int n;
		for (n=0; n<changeCount; n++)
		{			
			POINT tilePointTemp;

			//------------------------------------------------------
			// Zone狼 康开捞 酒囱 版快俊 Skip...
			//------------------------------------------------------
			if (sX1[n] < 0) 
			{	
				// Zone寇胞 何盒篮 八篮祸栏肺 磨茄促.
				rect.left = firstTilePoint[n].x;
				rect.top = 0;	
				
				firstTilePoint[n].x += -sX1[n] * TILE_X;

				rect.right = firstTilePoint[n].x;
				rect.bottom = g_TILESURFACE_HEIGHT;				 

				m_pTileSurface->FillRect(&rect, 0);
				
				sX1[n] = 0;	
			}

			if (sX2[n] > m_pZone->GetWidth())
			{
				sX2[n] = m_pZone->GetWidth();//-1;				

				// Zone寇胞 何盒篮 八篮祸栏肺 磨茄促.
				rect.left = firstTilePoint[n].x + (sX2[n]-sX1[n])*TILE_X;
				rect.top = 0;					
				rect.right = g_TILESURFACE_WIDTH;
				rect.bottom = g_TILESURFACE_HEIGHT;

				m_pTileSurface->FillRect(&rect, 0);				
			}

			if (sY1[n] < 0)
			{
				// Zone寇胞 何盒篮 八篮祸栏肺 磨茄促.
				rect.left = firstTilePoint[n].x;
				rect.top = firstTilePoint[n].y;	
				
				firstTilePoint[n].y += -sY1[n] * TILE_Y;

				rect.right = firstTilePoint[n].x + (sX2[n]-sX1[n])*TILE_X;
				rect.bottom = firstTilePoint[n].y;
				m_pTileSurface->FillRect(&rect, 0);
				
				sY1[n] = 0;	
			}

			if (sY2[n] > m_pZone->GetHeight())
			{
				// Zone寇胞 何盒篮 八篮祸栏肺 磨茄促.
				rect.left = firstTilePoint[n].x;
				rect.top = firstTilePoint[n].y;					
				rect.right = firstTilePoint[n].x + (sX2[n]-sX1[n])*TILE_X;
				rect.bottom = g_TILESURFACE_HEIGHT;
				m_pTileSurface->FillRect(&rect, 0);

				sY2[n] = m_pZone->GetHeight();//-1;
			}
					
			// 霉锅掳 临			
			tilePointTemp.y = firstTilePoint[n].y;

			//---------------------------------------
			// LOCK
			//---------------------------------------
			if (!m_pTileSurface->Lock()) return;

			//char str[80];
			int endY = sY2[n];
			int endX = sX2[n];
			for (y=sY1[n]; y<endY; y++)
			{				
				// 茄 临狼 霉锅掳 Sector					
				tilePointTemp.x = firstTilePoint[n].x;

				for (x=sX1[n]; x<endX; x++)
				{				
					//m_PreviousFogSpriteID[y][x] = SPRITEID_NULL;

					point = tilePointTemp;					

					int spriteID = m_pZone->GetSector(x,y).GetSpriteID();

					if (spriteID==SPRITEID_NULL)
					{
#ifdef __DEBUG_OUTPUT__
						if( m_pZone->GetID() == 3001 && m_pZone->GetSector(x,y).IsBlockAny() )
							m_pTileSurface->BltSprite(&point, &m_EtcSPK[1]);				
						else
							m_pTileSurface->BltSprite(&point, &m_EtcSPK[SPRITEID_TILE_NULL]);				
#else
						m_pTileSurface->BltSprite(&point, &m_EtcSPK[SPRITEID_TILE_NULL]);
#endif
					}
					else
					{
						CSprite& sprite = m_TileSPK[ spriteID ];

						//---------------------------------------
						// ID啊 spriteID牢 Tile阑 Load茄促.
						//---------------------------------------
//						if (sprite.IsNotInit())
//						{
//							#ifdef	OUTPUT_DEBUG
//								if (g_pDebugMessage!=NULL)
//									sprintf(g_pDebugMessage->GetCurrent(), "[RunTimeLoading] Tile(%d,%d) - Draw : sprite=%d", x, y, spriteID);
//							#endif
//
//							m_TileSPKFile.seekg(m_TileSPKI[spriteID], ios::beg);
//							
//							//--------------------------------------------------
//							// Loading俊 己傍茄 版快
//							//--------------------------------------------------
//							if (m_TileSPK[spriteID].LoadFromFile( m_TileSPKFile ))
//							{
//								#ifdef	OUTPUT_DEBUG
//									if (g_pDebugMessage!=NULL)
//									{
//										sprintf(g_pDebugMessage->GetCurrent(), "%s ...OK", g_pDebugMessage->GetCurrent());
//										g_pDebugMessage->Next();
//									}
//								#endif
//							}
//							//--------------------------------------------------
//							// 角菩茄 版快 --> 捞固 Loading窍绊 乐绰 版快捞促.				
//							//--------------------------------------------------
//							/*
//							// 2001.8.20 林籍贸府
//							else
//							{
//								#ifdef	OUTPUT_DEBUG
//									if (g_pDebugMessage!=NULL)
//									{
//										sprintf(g_pDebugMessage->GetCurrent(), "%s ...Fail & Wait Loading", g_pDebugMessage->GetCurrent());
//										g_pDebugMessage->Next();
//									}
//								#endif
//
//		
//								// file thread 鉴困甫 臭腮促.
//								//SetThreadPriority(g_hFileThread, THREAD_PRIORITY_HIGHEST);
//
//								MLoadingSPKWorkNode3* pNode = new MLoadingSPKWorkNode3(spriteID, m_TileSPKI[spriteID]);
//								pNode->SetSPK( &m_TileSPK, FILE_SPRITE_TILE );
//								pNode->SetType( 1 );
//								g_pLoadingThread->SetPriority( THREAD_PRIORITY_HIGHEST );
//								g_pLoadingThread->AddFirst( pNode );
//
//								// Thread俊辑 Loading捞 场朝锭鳖瘤 扁促赴促.
//								int waitCount = 0;
//								while (1)
//								{								
//									DEBUG_ADD_FORMAT( "Check Load id=%d", spriteID );
//
//									if (m_TileSPK[spriteID].IsInit())
//									{
//										DEBUG_ADD( "Is Init" );
//
//										break;
//									}
//									else
//									{
//										DEBUG_ADD( "Is Not Init" );
//									}
//
//									
//									//if (++waitCount == 1000)
//									//{
//									//	break;
//									//}											
//								}
//								//while (!m_TileSPK[spriteID].LoadFromFile( m_TileSPKFile ));
//
//								// file thread 鉴困甫 撤冕促.
//								//SetThreadPriority(g_hFileThread, THREAD_PRIORITY_BELOW_NORMAL);	
//								g_pLoadingThread->SetPriority( THREAD_PRIORITY_LOWEST );
//
//							}
//							*/
//							
//							//if (sprite.IsInit())
//							//{
//							//	m_pTileSurface->BltSprite(&point, &sprite);
//							//}
//						}
						//else 
						//{
							// 哆厩~~!!!!!!! 加档 棱酒 冈绰促~!!!
//							POINT pointTempTemp = point;
//							m_pTileSurface->BltSprite(&pointTempTemp, &m_EtcSPK[SPRITEID_TILE_NULL]);

							m_pTileSurface->BltSprite(&point, &sprite);
						//}
						//m_pTileSurface->BltSprite(&point, &m_SpritePack[ 3 ]);
						
						//sprintf(str, "(%d,%d)", x, y);			
						//m_pTileSurface->GDI_Text(point.x, point.y, str, 0);
					}
					

					// 免仿窍妨绰 谅钎 捞悼
					tilePointTemp.x += TILE_X;
				}		
						
				// 促澜 临
				tilePointTemp.y += TILE_Y;					
			}	

			//---------------------------------------
			// UNLOCK
			//---------------------------------------
			m_pTileSurface->Unlock();
		}

//		m_bFogChanged = true;
	}

	//----------------------------------------------------------------
	//
	// Tile Image to Current Surface
	//
	//----------------------------------------------------------------	
	point.x = 0;
	point.y = 0;

	
	////m_pSurface->BltDarkness(&point, m_pTileSurface, &rectReuse, DARK_VALUE);
	
	//----------------------------------------------------------------	
	//
	// [ TEST CODE ]  Perspective
	//
	//----------------------------------------------------------------	
	/*
#ifdef OUTPUT_DEBUG

	if (!g_pDXInput->KeyDown(DIK_LSHIFT))
	{
		m_pSurface->BltNoColorkey(&point, m_pTileSurface, &rectReuse);
	}
	else
	{
		if (CDirect3D::IsHAL())
		{	
			
			// TileTexture 汲沥
			CSpriteSurface* m_pTileTexture = new CSpriteSurface;
			m_pTileTexture->InitTextureSurface(256, 256);
			
			RECT destRect = { 0, 0, 256, 256 };

			m_pTileTexture->Blt(&destRect, m_pTileSurface, &rectReuse);

			CDirect3D::GetDevice()->SetTexture(0, m_pTileTexture->GetSurface());	
			//CDirect3D::GetDevice()->SetTexture(0, NULL);	
			
			static CD3DObject object;
			static bool first = true;

			if (first)
			{
				//--------------------------------------------------------
				// Light 汲沥
				//--------------------------------------------------------
				D3DLIGHT7 light;
				ZeroMemory( &light, sizeof(D3DLIGHT7) );
				light.dltType        = D3DLIGHT_POINT;
				light.dcvDiffuse.r   = 1.0f;
				light.dcvDiffuse.g   = 1.0f;
				light.dcvDiffuse.b   = 1.0f;
				light.dcvSpecular    = light.dcvDiffuse;
				light.dvPosition.x   = light.dvDirection.x = 0.0f;
				light.dvPosition.y   = light.dvDirection.y = 8.5f;
				light.dvPosition.z   = light.dvDirection.z = -5.0f;
				light.dvAttenuation0 = 1.0f;
				light.dvRange        = D3DLIGHT_RANGE_MAX;

				CDirect3D::GetDevice()->SetLight( 0, &light );
				CDirect3D::GetDevice()->LightEnable( 0, TRUE );
				CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );


				// world matrix甫 扁匡烙..
				D3DMATRIX matWorld;
				CDirect3D::GetDevice()->GetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
				CD3DMath::MultiplyRotateXMatrix(matWorld, 25*g_DEGTORAD);
				CDirect3D::GetDevice()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

				////object.CreatePlane(2.0f);
				object.CreatePlane(6.5f);
				object.Move(0, 0.5f, 0);		


				D3DMATERIAL7 mtrl;
				mtrl.dcvDiffuse.r = mtrl.dcvAmbient.r = 1.0f;
				mtrl.dcvDiffuse.g = mtrl.dcvAmbient.g = 1.0f;
				mtrl.dcvDiffuse.b = mtrl.dcvAmbient.b = 1.0f;
				mtrl.dcvDiffuse.a = mtrl.dcvAmbient.a = 1.0f;		
				
				//g_Background.Rotate( 90.0f*g_DEGTORAD, 0, 0 );
				mtrl.dcvEmissive.r = 0;
				mtrl.dcvEmissive.g = 0;
				mtrl.dcvEmissive.b = 0;
				object.SetMaterial( mtrl );

				first = false;
			}
			
			object.Draw();

			delete m_pTileTexture;
		}	
	}

#else
*/


	bool bDrawBackGround = DrawEvent();
	
	__BEGIN_PROFILE("ReuseBltTileSurface")
	// 2004, 9, 3, sobeit add start - 鸥老 缔率俊 备抚-_-;
//	event = g_pEventManager->GetEventByFlag(EVENTFLAG_CLOUD_BACKGROUND);
	if(g_pPlayer->IsShowAdamCloud())
	{
		CDirectDrawSurface *pCloudSurface = g_pEventManager->GetEventBackground(EVENTBACKGROUNDID_CLOUD);
		if(pCloudSurface != NULL)
		{
			int CloudPos = g_CurrentFrame % g_GameRect.right;
			POINT CloudPoint = {0,0};
			RECT CloudRect = { CloudPos, 0, g_GameRect.left, g_GameRect.top };
			if(CloudPos != g_GameRect.left)
				m_pSurface->BltNoColorkey(&CloudPoint, pCloudSurface, &CloudRect);
			if(CloudPos != 0)
			{
				CloudPoint.x	= g_GameRect.left-CloudPos;
				CloudRect.left	=  0;
				CloudRect.right	=  CloudPos;
				m_pSurface->BltNoColorkey(&CloudPoint, pCloudSurface, &CloudRect);
			}
			if(bDrawBackGround)
				m_pSurface->Blt(&point, m_pTileSurface, &rectReuse);
		}
		else
		if(bDrawBackGround)
			m_pSurface->BltNoColorkey(&point, m_pTileSurface, &rectReuse);
	}
	else
	// 2004, 9, 3, sobeit add end - 鸥老 缔率俊 备抚-_-;
	if(bDrawBackGround)
		m_pSurface->BltNoColorkey(&point, m_pTileSurface, &rectReuse);
	

	__END_PROFILE("ReuseBltTileSurface")

	__END_PROFILE("ReuseTileSurface")
//#endif
		
	POINT tilePointTemp;
	int sX1, sX2, sY1, sY2;	

	//------------------------------------------------------
	// Object甫 免仿窍扁 困茄 拳搁 谅钎 汲沥
	//------------------------------------------------------
	sX1 = firstSector.x + SECTOR_SKIP_LEFT;
	sY1 = firstSector.y + SECTOR_SKIP_UP;
	sX2 = firstSector.x + g_SECTOR_WIDTH;
	sY2 = firstSector.y + g_SECTOR_HEIGHT;

	// 免仿且 Surface惑狼 困摹
	tilePoint.x = firstPointX + TILE_X*SECTOR_SKIP_LEFT;
	tilePoint.y = firstPointY + TILE_Y*SECTOR_SKIP_UP;

	//------------------------------------------------------
	// Zone狼 康开捞 酒囱 版快俊 Skip...
	//------------------------------------------------------
	if (sX1 < 0) 
	{					
		tilePoint.x += -sX1 * TILE_X;
		sX1 = 0;	
	}

	if (sX2 >= m_pZone->GetWidth())
	{
		sX2 = m_pZone->GetWidth()-1;
	}

	if (sY1 < 0)
	{
		tilePoint.y += -sY1 * TILE_Y;
		sY1 = 0;	
	}

	if (sY2 >= m_pZone->GetHeight())
	{
		sY2 = m_pZone->GetHeight()-1;
	}
					
				

	//------------------------------------------------------
	//
	//                  Object 免仿
	//
	//------------------------------------------------------

	//BYTE DarkBits;

	//---------------------------------------
	// LOCK
	//---------------------------------------
	if (!m_pSurface->Lock()) return;

// 2004, 03, 24, sobeit start
	//------------------------------------------------------
	// Creature OutputMap阑 积己茄促.
	//------------------------------------------------------
	__BEGIN_PROFILE("AddOutputCreature")

	AddOutputCreatureAll();

	__END_PROFILE("AddOutputCreature")

	BOOL bUnlockStatus = CDirect3D::IsHAL() && g_pUserOption->BlendingShadow;
	if (bUnlockStatus)
	{	
		m_pSurface->Unlock();
	}


	CREATURE_OUTPUT_MAP::const_iterator iCreatureOutput = m_mapCreature.begin();
		
	//------------------------------------------------------
	// Creature - ghost draw.
	//------------------------------------------------------
	while (iCreatureOutput != m_mapCreature.end())
	{
		MCreature* const pCreature = (MCreature* const)((*iCreatureOutput).second);

		if(pCreature->GetCreatureType() == CREATURETYPE_GHOST)
		{
			// 免仿 矫痢捞 sY1焊促 利篮 版快..绰 免仿
			// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
			point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
			point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
									
			// 灌扁甫 搬沥茄促.
			//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;
			
			DrawCreature(&point, pCreature);//, DarkBits);
		}
	
		iCreatureOutput ++;
	}
// 2004, 03, 24, sobeit end
	/*
	//------------------------------------------------------
	// Sprite 窍唱 焊扁
	//------------------------------------------------------
			static int s_id = 0;
			static DWORD lastFrame = g_CurrentFrame;

			if (g_pDXInput->KeyDown(DIK_N) && g_CurrentFrame-lastFrame>2)
			{
				if (g_pDXInput->KeyDown(DIK_LSHIFT))
				{
					s_id -= 9;
				}

				if (--s_id < 0)
				{
					s_id = 0;
				}

				lastFrame = g_CurrentFrame;
			}

			if (g_pDXInput->KeyDown(DIK_M) && g_CurrentFrame-lastFrame>2)
			{
				if (g_pDXInput->KeyDown(DIK_LSHIFT))
				{
					s_id += 9;
				}

				if (++s_id >= m_CreatureSPK.GetSize())
				{
					s_id = m_CreatureSPK.GetSize()-1;
				}
				lastFrame = g_CurrentFrame;
			}

			POINT pointS = { 400, 20 };
			CIndexSprite::SetUsingColorSet( 10, 10 );
			char str[80];
			sprintf(str, "id = %d", s_id);
			
			m_pSurface->BltIndexSprite(&pointS, &m_CreatureSPK[s_id]);
m_pSurface->Unlock();
			m_pSurface->GDI_Text(400, 2, str, 0xFFFFFF);
if (!m_pSurface->Lock()) return;
	*/


	// Surface狼 沥焊甫 历厘秦敌促.
	//SetSurfaceInfo(&m_SurfaceInfo, m_pSurface->GetDDSD());

	//------------------------------------------------------
	//
	//			官蹿 坷宏璃飘 免仿
	//
	//------------------------------------------------------	
	//------------------------------------------------------
	// 免仿措惑捞 登绰 ImageObject Iterator
	//------------------------------------------------------
	IMAGEOBJECT_OUTPUT_MAP::const_iterator iImageObjectOutput0 = m_mapImageObject.begin();
	
	//------------------------------------------------------
	// 免仿 矫痢捞 0 溜 官蹿 坷宏璃飘牢
	// ImageObject甸阑 免仿茄促.
	// m_mapImageObject俊绰 Viewpoint啊 key蔼捞 登绢辑 Sort登绢 乐促.
	//------------------------------------------------------
	while (bDrawBackGround && iImageObjectOutput0 != m_mapImageObject.end())
	{
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectOutput0).second);

		// 免仿 矫痢捞 0牢版快 版快..绰 免仿
		if (pImageObject->GetViewpoint() == 0)
		{
			// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
			point.x = pImageObject->GetPixelX() - m_FirstZonePixel.x;
			point.y = pImageObject->GetPixelY() - m_FirstZonePixel.y;
									
			DrawImageObject(&point, pImageObject);
		}
		// 酒聪搁.. Object 免仿且锭 免仿登绰 版快捞促.
//		else
//		{
//			break;
//		}

		iImageObjectOutput0 ++;
	}


	//------------------------------------------------------
	//
	//			Ground Effect 免仿
	//
	//------------------------------------------------------	
	// player啊 包加俊 乐绰 版快 [货扁贱3]
	//------------------------------------------------------
	bool bPlayerInCasket = g_pPlayer->IsInCasket();
		
	if (bPlayerInCasket)
	{
		// [货扁贱3]
		// 拳搁 傈眉甫 绢涤霸 钎矫茄促. Tile父 - -;;
		rect.left = 0;
		rect.right = g_GameRect.right;
		rect.top = 0;
		rect.bottom = g_GameRect.bottom;

		DrawAlphaBox(&rect, 0, 0, 0, 20);	
	}
	else
	{	
		__BEGIN_PROFILE("DrawGroundEffect")

		DrawGroundEffect();

		__END_PROFILE("DrawGroundEffect")
	}


	//------------------------------------------------------
	//
	// 急琶等 Sector甫 钎矫茄促.
	//
	//------------------------------------------------------
	// 急琶等 困摹啊 乐绊.. 急琶等 Object啊 绝阑 锭...
	//if (g_pUserInformation->Invisible)
	{
	}
	//else


//	if (!g_pUserInformation->Invisible)
	{
		bool bPutSelectedSector = false;
		if (m_SelectSector.x!=SECTORPOSITION_NULL 
			&& m_SelectSector.y!=SECTORPOSITION_NULL)		
		{
			POINT selectedPoint = MapToScreen( m_SelectSector.x, m_SelectSector.y );

			// [ TEST CODE ]
			// 0,1,2,3,4,5
			// 2,2,3,3,4,4		
			const int MaxClickFrame = 6;
			const int frameID[MaxClickFrame] =
			{
				4, 4, 5, 5, 6, 6
			};
			
			static int clickFrame = 0;
			
			if(g_pEventManager->GetEventByFlag(EVENTFLAG_NOT_DRAW_CREATURE) == NULL)
			{
				// 吝缴 谅钎 焊沥
				CSprite* pSprite = &m_EtcSPK[ frameID[clickFrame] ];
				
				selectedPoint.x += g_TILE_X_HALF - (pSprite->GetWidth()>>1);
				selectedPoint.y += g_TILE_Y_HALF - (pSprite->GetHeight()>>1);
				
				
				m_pSurface->BltSprite(&selectedPoint, pSprite);
			}
			
			static DWORD lastTime = g_CurrentTime;
			if (g_CurrentTime - lastTime >= g_UpdateDelay)
			{
				if (++clickFrame==MaxClickFrame) clickFrame = 0;
				lastTime = g_CurrentTime;
			}
			
			
			// Player啊 急琶等 困摹俊 吭栏搁..
			if (g_pPlayer->GetX()==m_SelectSector.x && g_pPlayer->GetY()==m_SelectSector.y)
			{
				m_SelectSector.x = SECTORPOSITION_NULL;
				m_SelectSector.y = SECTORPOSITION_NULL;
			}

			bPutSelectedSector = true;
		}	
	}


	//------------------------------------------------------
	// 
	//       ImageObject狼 弊覆磊 免仿
	//
	//------------------------------------------------------
	//------------------------------------------------------
	// 免仿措惑捞 登绰 ImageObject Iterator
	//------------------------------------------------------
	__BEGIN_PROFILE("DrawImageObjectShadow")

	IMAGEOBJECT_OUTPUT_MAP::const_iterator iImageObjectShadowOutput = m_mapImageObject.begin();

	TYPE_SPRITEID	sprite;
	while (bDrawBackGround && iImageObjectShadowOutput != m_mapImageObject.end())
	{
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectShadowOutput).second);
		
		// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
		point.x = pImageObject->GetPixelX() - m_FirstZonePixel.x;
		point.y = pImageObject->GetPixelY() - m_FirstZonePixel.y;
											
		if (pImageObject->GetSpriteID()==SPRITEID_NULL)
		{
			/*
			if (pImageObject->IsBltTypeNormal())
			{
				sprite = m_ImageObjectFPK[ pImageObject->GetFrameID() ][ pImageObject->GetFrame() ].GetSpriteID();
			}
			else
			{
				sprite = m_EffectAlphaFPK[ pImageObject->GetFrameID() ][ 0 ][ pImageObject->GetFrame() ].GetSpriteID();
			}
			*/
			iImageObjectShadowOutput ++;
			continue;
		}
		else
		{
			sprite = pImageObject->GetSpriteID();
		}

		//point.y += m_ImageObjectSPK[sprite].GetHeight();
		//point.y -= 200;
		//m_pSurface->BltShadowSpriteDarkness(&point, &m_ImageObjectSSPK[ sprite ], 3-m_DarkBits);
		//m_pSurface->BltShadowSprite(&point, &m_ImageObjectSSPK[ 0 ]);

		iImageObjectShadowOutput ++;
	}

	__END_PROFILE("DrawImageObjectShadow")

	//------------------------------------------------------
	//
	// 免仿 矫痢焊促 困率俊 乐绰 ImageObject甸阑 免仿茄促.
	//
	//------------------------------------------------------
	//------------------------------------------------------
	// 免仿措惑捞 登绰 ImageObject Iterator
	//------------------------------------------------------
	IMAGEOBJECT_OUTPUT_MAP::const_iterator iImageObjectOutput = m_mapImageObject.begin();
	
	//------------------------------------------------------
	// 免仿 矫痢捞 sY1焊促 利篮 版快狼 
	// ImageObject甸阑 免仿茄促.
	// m_mapImageObject俊绰 Viewpoint啊 key蔼捞 登绢辑 Sort登绢 乐促.
	//------------------------------------------------------
	while (bDrawBackGround && iImageObjectOutput != m_mapImageObject.end())
	{
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectOutput).second);

		// 免仿 矫痢捞 sY1焊促 利篮 版快..绰 免仿
		if (pImageObject->GetViewpoint() < sY1)
		{
			if(pImageObject->GetViewpoint() != 0)
			{
				// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
				point.x = pImageObject->GetPixelX() - m_FirstZonePixel.x;
				point.y = pImageObject->GetPixelY() - m_FirstZonePixel.y;
										
				DrawImageObject(&point, pImageObject);
			}
		}
		// 酒聪搁.. Object 免仿且锭 免仿登绰 版快捞促.
		else
		{
			break;
		}

		iImageObjectOutput ++;
	}

	// 局聪皋捞记 坷宏璃飘俊辑 Loop啊 登绰 俊聪皋捞记 坷宏璃飘狼 Frame阑 嘎眠扁 困秦辑
	if(g_bFrameChanged)
		MAnimationObject::NextLoopFrame();

/*	- 2004, 4, 24 sobeit block - ghost 锭巩俊 困俊辑 贸府 窃
	//------------------------------------------------------
	// player啊 包 加俊 乐绰 版快啊 酒聪搁.. [货扁贱3]
	//------------------------------------------------------
	//------------------------------------------------------
	// Creature OutputMap阑 积己茄促.
	//------------------------------------------------------
	__BEGIN_PROFILE("AddOutputCreature")

	AddOutputCreatureAll();

	__END_PROFILE("AddOutputCreature")

	//------------------------------------------------------
	//
	//			葛电 creature狼 弊覆磊甫 免仿茄促.
	//
	//------------------------------------------------------
	// 葛电 creature狼 shadow甫 免仿茄促.
	// (!) AddOutputCreatureAll()甫 茄 促澜俊 角青秦具 茄促.
	//
	// Lock等 惑怕俊辑 角青茄促绊 啊沥茄促.
	//----------------------------------------------------
	// 3D啊加且锭绰 Unlock栏肺 父电促.
	//----------------------------------------------------
	BOOL bUnlockStatus = CDirect3D::IsHAL() && g_pUserOption->BlendingShadow;
	if (bUnlockStatus)
	{	
		m_pSurface->Unlock();
	}
*/
	//------------------------------------------------------
	// 免仿措惑捞 登绰 Creature Iterator
	//------------------------------------------------------
	__BEGIN_PROFILE("DrawCreatureShadowAll")

	//CREATURE_OUTPUT_MAP::const_iterator iCreatureOutput = m_mapCreature.begin();
	iCreatureOutput = m_mapCreature.begin();

	//------------------------------------------------------
	// 某腐磐 弊覆磊甫 葛滴 免仿茄促.
	//------------------------------------------------------
	// 鸥老阑 眉农秦辑 免仿窍绰 版快
	//------------------------------------------------------
	if (m_bTileSearchForCreature)
	{
		for (y=sY1; y<=sY2; y++)
		{
			for (x=sX1; x<=sX2; x++)
			{	
				int darknessCount = g_pPlayer->GetDarknessCount();
				
				// darkness尔 包拌绝捞 焊捞绰 版快. effect绰 +2
				if (darknessCount < 0
					|| max(abs(g_pPlayer->GetX()-x), abs(g_pPlayer->GetY()-y)) <= darknessCount+2)
				//g_pPlayer->ShowInDarkness(x, y))
				{			
					const MSector& sector = m_pZone->GetSector(x,y);

					if (sector.IsExistObject())
					{
						//------------------------------------------------------
						// Underground Creature
						//------------------------------------------------------
						MCreature* pCreature;

						/*
						pCreature = sector.GetUndergroundCreature();

						if (pCreature!=NULL)
						{
							// player捞芭唱 darkness尔 包拌绝捞 焊捞绰 版快
							if (pCreature==g_pPlayer
								|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
							{				
								// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
								point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
								point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
														
								DrawCreatureShadow(&point, pCreature);//, DarkBits);
							}
						}
						*/

						//------------------------------------------------------
						// Ground Creature
						// Flying Creature
						//------------------------------------------------------
						int numCreature = sector.GetCreatureSize();

						if (numCreature > 0)
						{
							OBJECT_MAP::const_iterator iCreature = sector.GetCreatureIterator();

							for (int c=0; c<numCreature; c++, iCreature++)
							{
								if (iCreature==sector.GetObjectEnd() 
									|| iCreature->first > MSector::POSITION_FLYINGCREATURE_MAX)
									break;

								// underground绰 弊覆磊啊 绝寸.
								if (iCreature->first >= MSector::POSITION_GROUNDCREATURE)
								{
									pCreature = (MCreature*)iCreature->second;

									// player捞芭唱 darkness尔 包拌绝捞 焊捞绰 版快
									if (
										(pCreature==g_pPlayer
										|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY())) &&
										!pCreature->IsFakeCreature()										
										)
									{				
										// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
										point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
										point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
																
										DrawCreatureShadow(&point, pCreature);//, DarkBits);
									}
								}							
							}
						}
					}					
				}
			}
		}
		if( m_pZone->GetFakeCreatureNumber() > 0 )
		{
			int numFakeCreature = m_pZone->GetFakeCreatureNumber();
			
			MZone::CREATURE_MAP::const_iterator itr = m_pZone->GetFakeCreatureBegin();
			
			for(int i = 0; i < numFakeCreature; i++,itr++)
			{
				if( itr->first < MSector::POSITION_GROUNDCREATURE ||
					itr->first > MSector::POSITION_FLYINGCREATURE_MAX )
					continue;

				MCreature *pCreature = itr->second;
				
				point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
				point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
				
				DrawCreatureShadow(&point, pCreature);
			}
		}

		// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
		point.x = g_pPlayer->GetPixelX() - m_FirstZonePixel.x;
		point.y = g_pPlayer->GetPixelY() - m_FirstZonePixel.y;
								
		DrawCreatureShadow(&point, g_pPlayer);//, DarkBits);		
	}
	//------------------------------------------------------
	// 某腐磐 OutputMap阑 捞侩窍绰 版快
	//------------------------------------------------------
	else
	{
		while (iCreatureOutput != m_mapCreature.end())
		{
			MCreature* const pCreature = (MCreature* const)((*iCreatureOutput).second);

			if (pCreature==g_pPlayer
				|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
			{
				// 免仿 矫痢捞 sY1焊促 利篮 版快..绰 免仿
				// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
				point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
				point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
										
				// 灌扁甫 搬沥茄促.
				//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;
				
				DrawCreatureShadow(&point, pCreature);//, DarkBits);
			}
		
			iCreatureOutput ++;
		}
	}

	__END_PROFILE("DrawCreatureShadowAll")

	//----------------------------------------------------
	// 盔贰狼 Lock等 惑怕甫 蜡瘤矫难 霖促.
	//----------------------------------------------------
	if (bUnlockStatus)
	{	
		m_pSurface->Lock();
	}

	// Item 弊覆磊 免仿侩
	//------------------------------------------------------
	// 拳搁狼 Sector甸阑 葛滴 八祸窍搁辑
	// Sector俊 粮犁窍绰 Object甸阑 免仿茄促.
	//------------------------------------------------------
	// 霉锅掳 临	
	tilePointTemp.y = tilePoint.y;

	//----------------------------------------------------------------
	//
	// tile阑 八祸秦辑 某腐磐甫 免仿窍绰 版快
	//
	//----------------------------------------------------------------
	if (m_bTileSearchForCreature)
	{
		for (y=sY1; y<=sY2; y++)
		{				
			// 茄 临狼 霉锅掳 Sector					
			tilePointTemp.x = tilePoint.x;			
			
			for (x=sX1; x<=sX2; x++)
			{	
				const MSector& sector = m_pZone->GetSector(x,y);			

				//------------------------------------------------
				//
				//              Object 免仿
				//
				//------------------------------------------------
					
				if (sector.IsExistObject())
				{				
			
					//----------------------------------------
					// Item老 版快
					//----------------------------------------					
					pItem = sector.GetItem();
					if (pItem != NULL
						&& g_pPlayer->ShowInDarkness(x, y))
					{		
						//----------------------------------------
						// 矫眉牢 版快
						// Effect甫 免仿窍绊 唱辑 免仿茄促.
						//
						// 2002.1.23
						// 攫力 Effect免仿内靛啊 狐柳芭瘤..
						// 鞠瓢 矫眉 包访 内靛绰 猾促. 
						//----------------------------------------
						//if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
						{
						//	pCorpseItem = pItem;
						}
						//else
						{
							point.x = tilePointTemp.x;
							point.y = tilePointTemp.y;


							// 灌扁甫 搬沥茄促.
							//DarkBits = (sector.GetLight()==0)?m_DarkBits:0;
							
							// 唱吝俊 Frame沥焊俊辑 cx,cy甫 背沥秦具 茄促.									
							DrawItemShadow(&point, pItem);//, DarkBits);
						}
					}
				}


				//------------------------------------			
				// 免仿窍妨绰 谅钎 捞悼
				//------------------------------------
				tilePointTemp.x += TILE_X;
			}
			
					
			// 促澜 临
			tilePointTemp.y += TILE_Y;					
		}		
	}
	//----------------------------------------------------------------
	//
	// tile阑 八祸窍瘤 臼绊 OutputCreatureMap阑 捞侩秦辑 某腐磐甫 免仿窍绰 版快
	//
	//----------------------------------------------------------------
	else
	{	
		for (y=sY1; y<=sY2; y++)
		{
			// 包 加俊 乐阑 锭绰 弊成 八祸秦辑 免仿窍绰 object绰 绝促.
			// AddOutputCreatureAll()俊辑 m_bTileSearchForCreature啊 false肺
			// 汲沥登骨肺... OutputCreatureMap阑 捞侩窍搁 等促.
			if (!bPlayerInCasket)	// [货扁贱3]
			{			
				// 茄 临狼 霉锅掳 Sector					
				tilePointTemp.x = tilePoint.x;			
				
				for (x=sX1; x<=sX2; x++)
				{	
					const MSector& sector = m_pZone->GetSector(x,y);
					
					//------------------------------------------------
					//
					//              Object 免仿
					//
					//------------------------------------------------
					//MItem* pCorpseItem = NULL;
						
					if (sector.IsExistObject())
					{				
						//----------------------------------------
						// Item老 版快
						//----------------------------------------					
						pItem = sector.GetItem();
						if (pItem != NULL
							&& g_pPlayer->ShowInDarkness(x, y))
						{		
							//----------------------------------------
							// 矫眉牢 版快
							// Effect甫 免仿窍绊 唱辑 免仿茄促.
							//
							// 2002.1.23
							// 攫力 Effect免仿内靛啊 狐柳芭瘤..
							// 鞠瓢 矫眉 包访 内靛绰 猾促. 
							//----------------------------------------
							//if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
							{
							//	pCorpseItem = pItem;
							}
							//else
							{
								point.x = tilePointTemp.x;
								point.y = tilePointTemp.y;

								// 唱吝俊 Frame沥焊俊辑 cx,cy甫 背沥秦具 茄促.									
								DrawItemShadow(&point, pItem);//, DarkBits);
							}
						}
					}


					//------------------------------------			
					// 免仿窍妨绰 谅钎 捞悼
					//------------------------------------
					tilePointTemp.x += TILE_X;
				}
			}	// bPlayerInCasket
					
			// 促澜 临
			tilePointTemp.y += TILE_Y;
		}
	}


	//------------------------------------------------------
	//
	//				角力 sprite 免仿
	//
	//------------------------------------------------------
	//------------------------------------------------------
	// 免仿措惑捞 登绰 Creature Iterator
	//------------------------------------------------------
	iCreatureOutput = m_mapCreature.begin();

	//------------------------------------------------------
	// 免仿 矫痢捞 sY1焊促 利篮 版快狼 
	// Creature甸阑 免仿茄促.
	// m_mapCreature俊绰 Viewpoint啊 key蔼捞 登绢辑 Sort登绢 乐促.
	//------------------------------------------------------
	while (iCreatureOutput != m_mapCreature.end())
	{
		MCreature* const pCreature = (MCreature* const)((*iCreatureOutput).second);

		// 2004, 04, 24 sobeit add start -ghost 老锭 
		if(pCreature->GetCreatureType() == CREATURETYPE_GHOST)
		{
			iCreatureOutput ++;
			continue;
		}
		// 2004, 04, 24 sobeit add end -ghost 老锭 

		// 免仿 矫痢捞 sY1焊促 利篮 版快..绰 免仿
		if (pCreature->GetY() < sY1)
		{
			// player捞芭唱 darkness尔 包拌绝捞 焊捞绰 版快
			if (pCreature==g_pPlayer
				|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
			{					
				// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
				point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
				point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
										
				// 灌扁甫 搬沥茄促.
				//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;

				DrawCreature(&point, pCreature);//, DarkBits);
			}
		}
		// 酒聪搁.. 茄 临 免仿捞 场唱绊 免仿且锭 免仿登绰 版快捞促.
		else
		{
			break;
		}

		iCreatureOutput ++;
	}

	//------------------------------------------------------
	// 拳搁狼 Sector甸阑 葛滴 八祸窍搁辑
	// Sector俊 粮犁窍绰 Object甸阑 免仿茄促.
	//------------------------------------------------------
	// 霉锅掳 临	
	tilePointTemp.y = tilePoint.y;

	//----------------------------------------------------------------
	//
	// tile阑 八祸秦辑 某腐磐甫 免仿窍绰 版快
	//
	//----------------------------------------------------------------
	if (m_bTileSearchForCreature)
	{
		for (y=sY1; y<=sY2; y++)
		{				
			// 茄 临狼 霉锅掳 Sector					
			tilePointTemp.x = tilePoint.x;			
			
			for (x=sX1; x<=sX2; x++)
			{	
				const MSector& sector = m_pZone->GetSector(x,y);			

				//------------------------------------------------------
				// 辑滚 block谅钎俊 荤阿屈
				//------------------------------------------------------
				#ifdef OUTPUT_DEBUG
					if (g_pDXInput->KeyDown(DIK_LCONTROL) && g_pDXInput->KeyDown(DIK_V))
					{		
						if (sector.IsBlockServerGround())
						{
							m_pSurface->Unlock();
							RECT rect2 = { tilePointTemp.x, tilePointTemp.y, tilePointTemp.x + TILE_X, tilePointTemp.y + TILE_Y };
							m_pSurface->DrawRect(&rect2, CDirectDraw::Color(31,20,20));
							m_pSurface->Lock();
						}

						if (sector.IsExistAnyCreature())
						{
							m_pSurface->Unlock();
							RECT rect2 = { tilePointTemp.x+2, tilePointTemp.y+2, tilePointTemp.x + TILE_X - 2, tilePointTemp.y + TILE_Y - 2 };
							m_pSurface->DrawRect(&rect2, CDirectDraw::Color(20,20,31));
							m_pSurface->Lock();
						}
					}
				#endif
				
				//------------------------------------------------------
				// 漂沥茄 ImageObject狼 ViewSector?俊 "X"钎窍扁
				//------------------------------------------------------
				#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
					if (g_pDXInput->KeyDown(DIK_A) && 
						(g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL)))
					{
						if (sector.IsExistImageObject())
						{
							OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

							// Sector俊 乐绰 葛电 ImageObject甸阑 八祸茄促.
							for (i=0; i<sector.GetImageObjectSize(); i++)
							{
								MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);

								if (pImageObject->GetObjectType()==MObject::TYPE_IMAGEOBJECT
									&& pImageObject->GetImageObjectID()==g_ShowImageObjectID)
								{
									m_pSurface->Unlock();
									m_pSurface->GDI_Text(tilePointTemp.x, 
														tilePointTemp.y, 
														"X", 														
														0xFFFFFF);	
									m_pSurface->Lock();
								}
								
								iImageObject++;
							}
						}
					}
				#endif				

				//------------------------------------------------
				// 蝴狼 碍档甫 免仿茄促.
				//------------------------------------------------
				//if (x==g_SelectSector.x && y==g_SelectSector.y)
				//{
				//	char str[80];
				//	sprintf(str, "%d", sector.GetLight());
				//	gC_font.PrintStringNoConvert(&m_SurfaceInfo, str, tilePointTemp.x+25,tilePointTemp.y+12, CDirectDraw::Color(28,28,28));				
				//}

				//------------------------------------------------
				//
				//              Object 免仿
				//
				//------------------------------------------------
				//MItem* pCorpseItem = NULL;
					
				if (sector.IsExistObject())
				{				
					//----------------------------------------
					// test code : Object 困摹 钎矫
					//----------------------------------------
					//point = tilePointTemp;
					//m_pSurface->Copy(&point, m_pm_pSurface[0], 
					//				&m_SpriteSurfacePack[0][105]);
					//
					//----------------------------------------
			
					//----------------------------------------
					// Item老 版快
					//----------------------------------------					
					pItem = sector.GetItem();
					if (pItem != NULL
						&& g_pPlayer->ShowInDarkness(x, y))
					{		
						//----------------------------------------
						// 矫眉牢 版快
						// Effect甫 免仿窍绊 唱辑 免仿茄促.
						//
						// 2002.1.23
						// 攫力 Effect免仿内靛啊 狐柳芭瘤..
						// 鞠瓢 矫眉 包访 内靛绰 猾促. 
						//----------------------------------------
						//if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
						{
						//	pCorpseItem = pItem;
						}
						//else
						{
							point.x = tilePointTemp.x;
							point.y = tilePointTemp.y;

							/*
							// 匡贩匡贩~~
							int size = g_CurrentFrame & 0x00000007;

							int x0 = point.x + g_TILE_X_HALF;
							int y0 = point.y + g_TILE_Y_HALF;
							int size4 = size << 4;
							int size2 = size << 3;
							RECT rect = 
							{ 
								x0 - size4,
								y0 - size2,
								x0 + size4,
								y0 + size2
							};
							m_pSurface->ChangeBrightnessBit( &rect, g_CurrentFrame );
							*/

							// 灌扁甫 搬沥茄促.
							//DarkBits = (sector.GetLight()==0)?m_DarkBits:0;
							
							// 唱吝俊 Frame沥焊俊辑 cx,cy甫 背沥秦具 茄促.									
							DrawItem(&point, pItem);//, DarkBits);
						}
					}
				}

				//------------------------------------------------
				//
				//              Effect 免仿
				//
				//------------------------------------------------
				//if (sector.IsExistEffect())
				//{				
				//	point = tilePointTemp;				
	//
	//				DrawEffect(&point, sector.GetEffectIterator(), sector.GetEffectSize());
	//			}

				//------------------------------------------------
				// 矫眉
				//------------------------------------------------
				// 澜!! 操瘤茄 内靛狼 瘤抚辨.. 澜衬.. 唱档 隔扼
				//------------------------------------------------
				/*
				if (pCorpseItem)
				{
					point.x = tilePointTemp.x;
					point.y = tilePointTemp.y;

					// 灌扁甫 搬沥茄促.
					//DarkBits = (sector.GetLight()==0)?m_DarkBits:0;
					
					// 唱吝俊 Frame沥焊俊辑 cx,cy甫 背沥秦具 茄促.									
					DrawItem(&point, pItem);//, DarkBits);
				}
				*/

				//------------------------------------			
				// 免仿窍妨绰 谅钎 捞悼
				//------------------------------------
				tilePointTemp.x += TILE_X;
			}
			
			//-----------------------------------------------------
			// 茄 临捞 场朝锭付促 
			// 免仿秦具且 弊 临狼 ImageObject甸阑 免仿茄促.
			//-----------------------------------------------------	
			while (bDrawBackGround && iImageObjectOutput != m_mapImageObject.end())
			{
				MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectOutput).second);

				// 免仿 矫痢捞 y客 鞍篮 版快 免仿
				if (pImageObject->GetViewpoint() <= y)
				{
					if(pImageObject->GetViewpoint() != 0)
					{
						// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
						point.x = pImageObject->GetPixelX() - m_FirstZonePixel.x;
						point.y = pImageObject->GetPixelY() - m_FirstZonePixel.y;				

						DrawImageObject(&point, pImageObject);

						//char str[80];
						//sprintf(str, "%d", pImageObject->GetViewpoint());
						//m_pSurface->GDI_Text(point.x, point.y, str, 0xFFFFFF);
					}
				}
				// 酒聪搁.. 促弗 临捞促.
				else
				{				
					break;
				}

				iImageObjectOutput ++;
			}

			//------------------------------------------------------
			// 茄 临捞 场朝锭付促 
			// 免仿秦具且 弊 临狼 Creature甸阑 免仿茄促.
			//------------------------------------------------------
			while (iCreatureOutput != m_mapCreature.end())
			{
				MCreature* const pCreature = (MCreature* const)((*iCreatureOutput).second);

				// 免仿 矫痢捞 sY1焊促 利篮 版快..绰 免仿
				if (pCreature->GetY() <= y)
				{
					// 2004, 04, 24 sobeit add start -ghost 老锭 
					if(pCreature->GetCreatureType() == CREATURETYPE_GHOST)
					{
						iCreatureOutput ++;
						continue;
					}
					// 2004, 04, 24 sobeit add end -ghost 老锭 

					// player捞芭唱 darkness尔 包拌绝捞 焊捞绰 版快
					if (pCreature==g_pPlayer
						|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
					{				
						// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
						point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
						point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
												
						// 灌扁甫 搬沥茄促.
						//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;

						DrawCreature(&point, pCreature);//, DarkBits);
					}
				}
				// 酒聪搁.. 茄 临 免仿捞 场唱绊 免仿且锭 免仿登绰 版快捞促.
				else
				{
					break;
				}

				iCreatureOutput ++;
			}

			//------------------------------------------------------
			// 免仿鉴辑锭巩俊.. 咯扁辑 某腐磐甫 眉农茄促.
			// Sector狼 Effect 免仿
			//------------------------------------------------------
			// 某腐磐焊促 词霸 免仿登绢具 窍骨肺...
			//------------------------------------------------------
			//------------------------------------------------------
			// Player绰 Tile俊 加窍瘤 臼扁 锭巩俊 蝶肺 眉农茄促.
			//------------------------------------------------------
			if (g_pPlayer->GetY() == y)
			{
				// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
				point.x = g_pPlayer->GetPixelX() - m_FirstZonePixel.x;
				point.y = g_pPlayer->GetPixelY() - m_FirstZonePixel.y;
										
				// 灌扁甫 搬沥茄促.
				//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;

				DrawCreature(&point, g_pPlayer);//, DarkBits);
			}

			tilePointTemp.x = tilePoint.x;
			for (x=sX1; x<=sX2; x++)
			{	
				int darknessCount = g_pPlayer->GetDarknessCount();
				
				// darkness尔 包拌绝捞 焊捞绰 版快. effect绰 +2
				if (darknessCount < 0
					|| max(abs(g_pPlayer->GetX()-x), abs(g_pPlayer->GetY()-y)) <= darknessCount+2)
				//g_pPlayer->ShowInDarkness(x, y))
				{			
					const MSector& sector = m_pZone->GetSector(x,y);

					if (sector.IsExistObject())
					{							
						//------------------------------------------------------
						// Underground Creature
						// Ground Creature
						// Flying Creature
						//------------------------------------------------------
						int numCreature = sector.GetCreatureSize();

						if (numCreature > 0)
						{
							OBJECT_MAP::const_iterator iCreature = sector.GetCreatureIterator();

							for (int c=0; c<numCreature; c++, iCreature++)
							{
								if (iCreature==sector.GetObjectEnd() 
									|| iCreature->first > MSector::POSITION_FLYINGCREATURE_MAX)
									break;

								MCreature* pCreature = (MCreature*)iCreature->second;
						
								// 2004, 04, 24 sobeit add start -ghost 老锭 
								if(pCreature->GetCreatureType() == CREATURETYPE_GHOST)
									continue;
								// 2004, 04, 24 sobeit add end -ghost 老锭 

								// player捞芭唱 darkness尔 包拌绝捞 焊捞绰 版快
								if (pCreature==g_pPlayer
									|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
								{				
									// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
									point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
									point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
															
									DrawCreature(&point, pCreature);//, DarkBits);
								}
							}
						}
					}

					//------------------------------------------------
					//
					//              Effect 免仿
					//
					//------------------------------------------------
					if (sector.IsExistEffect())
					{				
						point = tilePointTemp;				

	 					DrawEffect(&point, sector.GetEffectIterator(), sector.GetEffectSize());
					}
				}

				//------------------------------------			
				// 免仿窍妨绰 谅钎 捞悼
				//------------------------------------
				tilePointTemp.x += TILE_X;
			}
					
			// 促澜 临
			tilePointTemp.y += TILE_Y;					
		}		
	}
	//----------------------------------------------------------------
	//
	// tile阑 八祸窍瘤 臼绊 OutputCreatureMap阑 捞侩秦辑 某腐磐甫 免仿窍绰 版快
	//
	//----------------------------------------------------------------
	else
	{	
		for (y=sY1; y<=sY2; y++)
		{
			// 包 加俊 乐阑 锭绰 弊成 八祸秦辑 免仿窍绰 object绰 绝促.
			// AddOutputCreatureAll()俊辑 m_bTileSearchForCreature啊 false肺
			// 汲沥登骨肺... OutputCreatureMap阑 捞侩窍搁 等促.
			if (!bPlayerInCasket)	// [货扁贱3]
			{			
				// 茄 临狼 霉锅掳 Sector					
				tilePointTemp.x = tilePoint.x;			
				
				for (x=sX1; x<=sX2; x++)
				{	
					const MSector& sector = m_pZone->GetSector(x,y);
					
					//------------------------------------------------------
					// 辑滚 block谅钎俊 荤阿屈
					//------------------------------------------------------
					#ifdef OUTPUT_DEBUG
						if (g_pDXInput->KeyDown(DIK_LCONTROL) && g_pDXInput->KeyDown(DIK_V))
						{		
							if (sector.IsBlockServerGround())
							{
								m_pSurface->Unlock();
								RECT rect2 = { tilePointTemp.x, tilePointTemp.y, tilePointTemp.x + TILE_X, tilePointTemp.y + TILE_Y };
								m_pSurface->DrawRect(&rect2, CDirectDraw::Color(31,20,20));
								m_pSurface->Lock();
							}
						}
					#endif
					
					//------------------------------------------------------
					// 漂沥茄 ImageObject狼 ViewSector?俊 "X"钎窍扁
					//------------------------------------------------------
					#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
						if (g_pDXInput->KeyDown(DIK_A) && 
							(g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL)))
						{
							if (sector.IsExistImageObject())
							{
								OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

								// Sector俊 乐绰 葛电 ImageObject甸阑 八祸茄促.
								for (i=0; i<sector.GetImageObjectSize(); i++)
								{
									MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);

									if (pImageObject->GetObjectType()==MObject::TYPE_IMAGEOBJECT
										&& pImageObject->GetImageObjectID()==g_ShowImageObjectID)
									{
										m_pSurface->Unlock();
										m_pSurface->GDI_Text(tilePointTemp.x, 
															tilePointTemp.y, 
															"X", 														
															0xFFFFFF);	
										m_pSurface->Lock();
									}
									
									iImageObject++;
								}
							}
						}
					#endif				

					//------------------------------------------------
					// 蝴狼 碍档甫 免仿茄促.
					//------------------------------------------------
					//if (x==g_SelectSector.x && y==g_SelectSector.y)
					//{
					//	char str[80];
					//	sprintf(str, "%d", sector.GetLight());
					//	gC_font.PrintStringNoConvert(&m_SurfaceInfo, str, tilePointTemp.x+25,tilePointTemp.y+12, CDirectDraw::Color(28,28,28));				
					//}

					//------------------------------------------------
					//
					//              Object 免仿
					//
					//------------------------------------------------
					//MItem* pCorpseItem = NULL;
						
					if (sector.IsExistObject())
					{				
						//----------------------------------------
						// test code : Object 困摹 钎矫
						//----------------------------------------
						//point = tilePointTemp;
						//m_pSurface->Copy(&point, m_pm_pSurface[0], 
						//				&m_SpriteSurfacePack[0][105]);
						//
						//----------------------------------------
				
						//----------------------------------------
						// Item老 版快
						//----------------------------------------					
						pItem = sector.GetItem();
						if (pItem != NULL
							&& g_pPlayer->ShowInDarkness(x, y))
						{		
							//----------------------------------------
							// 矫眉牢 版快
							// Effect甫 免仿窍绊 唱辑 免仿茄促.
							//
							// 2002.1.23
							// 攫力 Effect免仿内靛啊 狐柳芭瘤..
							// 鞠瓢 矫眉 包访 内靛绰 猾促. 
							//----------------------------------------
							//if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
							{
							//	pCorpseItem = pItem;
							}
							//else
							{
								point.x = tilePointTemp.x;
								point.y = tilePointTemp.y;

								/*
								// 匡贩匡贩~~
								int size = g_CurrentFrame & 0x00000007;

								int x0 = point.x + g_TILE_X_HALF;
								int y0 = point.y + g_TILE_Y_HALF;
								int size4 = size << 4;
								int size2 = size << 3;
								RECT rect = 
								{ 
									x0 - size4,
									y0 - size2,
									x0 + size4,
									y0 + size2
								};
								m_pSurface->ChangeBrightnessBit( &rect, g_CurrentFrame );
								*/

								// 灌扁甫 搬沥茄促.
								//DarkBits = (sector.GetLight()==0)?m_DarkBits:0;
								
								// 唱吝俊 Frame沥焊俊辑 cx,cy甫 背沥秦具 茄促.									
								DrawItem(&point, pItem);//, DarkBits);
							}
						}
					}

					//------------------------------------------------
					//
					//              Effect 免仿
					//
					//------------------------------------------------
					//if (sector.IsExistEffect())
					//{				
					//	point = tilePointTemp;				
		//
		//				DrawEffect(&point, sector.GetEffectIterator(), sector.GetEffectSize());
		//			}

					//------------------------------------------------
					// 矫眉
					//------------------------------------------------
					// 澜!! 操瘤茄 内靛狼 瘤抚辨.. 澜衬.. 唱档 隔扼
					//------------------------------------------------
					/*
					if (pCorpseItem)
					{
						point.x = tilePointTemp.x;
						point.y = tilePointTemp.y;

						// 灌扁甫 搬沥茄促.
						//DarkBits = (sector.GetLight()==0)?m_DarkBits:0;
						
						// 唱吝俊 Frame沥焊俊辑 cx,cy甫 背沥秦具 茄促.									
						DrawItem(&point, pItem);//, DarkBits);
					}
					*/

					//------------------------------------			
					// 免仿窍妨绰 谅钎 捞悼
					//------------------------------------
					tilePointTemp.x += TILE_X;
				}
			}	// bPlayerInCasket
			
			//-----------------------------------------------------
			// 茄 临捞 场朝锭付促 
			// 免仿秦具且 弊 临狼 ImageObject甸阑 免仿茄促.
			//-----------------------------------------------------	
			while (bDrawBackGround && iImageObjectOutput != m_mapImageObject.end())
			{
				MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectOutput).second);

				// 免仿 矫痢捞 y客 鞍篮 版快 免仿
				if (pImageObject->GetViewpoint() <= y)
				{
					if(pImageObject->GetViewpoint() != 0)
					{
						// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
						point.x = pImageObject->GetPixelX() - m_FirstZonePixel.x;
						point.y = pImageObject->GetPixelY() - m_FirstZonePixel.y;				

						DrawImageObject(&point, pImageObject);

						//char str[80];
						//sprintf(str, "%d", pImageObject->GetViewpoint());
						//m_pSurface->GDI_Text(point.x, point.y, str, 0xFFFFFF);
					}
				}
				// 酒聪搁.. 促弗 临捞促.
				else
				{				
					break;
				}

				iImageObjectOutput ++;
			}

			//------------------------------------------------------
			// 茄 临捞 场朝锭付促 
			// 免仿秦具且 弊 临狼 Creature甸阑 免仿茄促.
			//------------------------------------------------------
			while (iCreatureOutput != m_mapCreature.end())
			{
				MCreature* const pCreature = (MCreature* const)((*iCreatureOutput).second);

				// 2004, 04, 24 sobeit add start -ghost 老锭 
				if(pCreature->GetCreatureType() == CREATURETYPE_GHOST)
				{
					iCreatureOutput ++;
					continue;
				}
				// 2004, 04, 24 sobeit end start -ghost 老锭 

				// 免仿 矫痢捞 sY1焊促 利篮 版快..绰 免仿
				if (pCreature->GetY() <= y)
				{
					// player捞芭唱 darkness尔 包拌绝捞 焊捞绰 版快
					if (pCreature==g_pPlayer
						|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
					{				
						// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
						point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
						point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
												
						// 灌扁甫 搬沥茄促.
						//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;

						DrawCreature(&point, pCreature);//, DarkBits);
					}
				}
				// 酒聪搁.. 茄 临 免仿捞 场唱绊 免仿且锭 免仿登绰 版快捞促.
				else
				{
					break;
				}

				iCreatureOutput ++;
			}

			//------------------------------------------------------
			// Sector狼 Effect 免仿
			//------------------------------------------------------
			// 某腐磐焊促 词霸 免仿登绢具 窍骨肺...
			//------------------------------------------------------
			if (!bPlayerInCasket)	// [货扁贱3] 包 加俊 乐阑锭 Effect绰 救 嘛绰促.
			{			
				tilePointTemp.x = tilePoint.x;
				for (x=sX1; x<=sX2; x++)
				{	
					int darknessCount = g_pPlayer->GetDarknessCount();
					
					// darkness尔 包拌绝捞 焊捞绰 版快. effect绰 +2
					if (darknessCount < 0
						|| max(abs(g_pPlayer->GetX()-x), abs(g_pPlayer->GetY()-y)) <= darknessCount+2)
					//g_pPlayer->ShowInDarkness(x, y))
					{			
						const MSector& sector = m_pZone->GetSector(x,y);

						//------------------------------------------------
						//
						//              Effect 免仿
						//
						//------------------------------------------------
						if (sector.IsExistEffect())
						{				
							point = tilePointTemp;				

	 						DrawEffect(&point, sector.GetEffectIterator(), sector.GetEffectSize());
						}
					}

					//------------------------------------			
					// 免仿窍妨绰 谅钎 捞悼
					//------------------------------------
					tilePointTemp.x += TILE_X;
				}
			}
					
			// 促澜 临
			tilePointTemp.y += TILE_Y;
		}
	}

	
	//------------------------------------------------------
	// 免仿 矫痢捞 sY2焊促 奴 版快狼 
	// Creature甸阑 免仿茄促.
	// m_mapCreature俊绰 Viewpoint啊 key蔼捞 登绢辑 Sort登绢 乐促.
	//------------------------------------------------------
	while (iCreatureOutput != m_mapCreature.end())
	{
		MCreature* const pCreature = (MCreature* const)((*iCreatureOutput).second);

		// 2004, 04, 24 sobeit add start -ghost 老锭 
		if(pCreature->GetCreatureType() == CREATURETYPE_GHOST)
		{
			iCreatureOutput ++;
			continue;
		}
		// 2004, 04, 24 sobeit add end -ghost 老锭 

		// player捞芭唱 darkness尔 包拌绝捞 焊捞绰 版快
		if (pCreature==g_pPlayer
			|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
		{
			// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
			point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
			point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
									
			// 灌扁甫 搬沥茄促.
			//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;

			DrawCreature(&point, pCreature);//, DarkBits);		
		}

		iCreatureOutput ++;
	}


	//------------------------------------------------------
	// 免仿 矫痢捞 sY2焊促 奴 版快狼 
	// ImageObject甸阑 免仿茄促.
	// m_mapImageObject俊绰 Viewpoint啊 key蔼捞 登绢辑 Sort登绢 乐促.
	//------------------------------------------------------
	while (bDrawBackGround && iImageObjectOutput != m_mapImageObject.end())
	{
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectOutput).second);

		// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
		point.x = pImageObject->GetPixelX() - m_FirstZonePixel.x;
		point.y = pImageObject->GetPixelY() - m_FirstZonePixel.y;
									
		DrawImageObject(&point, pImageObject);
		
		iImageObjectOutput ++;
	}

	//------------------------------------------------
	// mouse急琶茄 巴狼 寇胞急父 促矫 茄 锅 免仿秦霖促.
	//------------------------------------------------
	if (m_SOM.IsInit())
	{
		#ifdef OUTPUT_DEBUG_DRAW_PROCESS
			DEBUG_ADD( "m_SOM.Clear()" );
		#endif
		//m_pSurface->BltSpriteOutlineOnly( &m_SOM,  m_SOMOutlineColor );

		// 力芭
		m_SOM.Clear();
	}
	
	//----------------------------------------------------------------
	// player啊 包 加俊 乐绰 版快啊 酒聪搁.. [货扁贱3]
	//----------------------------------------------------------------
	if (!bPlayerInCasket && bDrawBackGround)
	{			
		//----------------------------------------------------------------
		//
		//          Weather - 朝揪 瓤苞 免仿
		//
		//----------------------------------------------------------------
		
		#ifdef OUTPUT_DEBUG_DRAW_PROCESS
			DEBUG_ADD( "Start DrawWeather" );
		#endif

		__BEGIN_PROFILE("DrawWeather")

		if (g_pWeather->IsActive())
		{
			// 啊肺 谅钎 焊沥..
			// 泅犁 Player狼 谅钎客 厚背茄促.
			//int gapX = ((g_pPlayer->GetX() - g_pWeather->GetStartX()) * TILE_X + g_pPlayer->GetSX()) % SURFACE_WIDTH;
			//int gapY = ((g_pPlayer->GetY() - g_pWeather->GetStartY()) * TILE_Y + g_pPlayer->GetSY()) % SURFACE_HEIGHT;
			int gapX = (m_FirstZonePixel.x - g_pWeather->GetStartX()) % g_GameRect.right;
			int gapY = (m_FirstZonePixel.y - g_pWeather->GetStartY()) % g_GameRect.bottom;

			for (i=0; i<g_pWeather->GetSize(); i++)
			{
				const MAP_EFFECT& MapEffect = (*g_pWeather)[i];

				// 肛眠瘤 臼篮 巴捞搁...
				if (MapEffect.IsActive())
				{
					point.x = MapEffect.GetX() - gapX;
					point.y = MapEffect.GetY() - gapY;

					if (point.x < 0) point.x += g_GameRect.right;
					else if (point.x > g_GameRect.right) point.x -= g_GameRect.right;

					if (point.y < 0) point.y += g_GameRect.bottom;
					else if (point.y > g_GameRect.bottom) point.y -= g_GameRect.bottom;

					if(g_pWeather->GetWeatherType() == MWeather::WEATHER_SPOT)
					{
						CSpriteSurface::SetEffect(CSpriteSurface::EFFECT_SCREEN_ALPHA);
						CSpriteSurface::s_Value1 = MapEffect.GetCount()*32/MapEffect.GetMaxCount();
						m_pSurface->BltSpriteEffect(&point, &m_WeatherSPK[ MapEffect.GetSpriteID() ]);
					}
//						m_pSurface->BltSpriteAlpha(&point, &m_WeatherSPK[ MapEffect.GetSpriteID() ], MapEffect.GetCount()*10/60);
					else
						m_pSurface->BltSprite(&point, &m_WeatherSPK[ MapEffect.GetSpriteID() ]);
					//m_pSurface->BltSpriteAlpha(&point, &m_WeatherSPK[ MapEffect.GetSpriteID() ], 24);

					/*
					#ifdef OUTPUT_DEBUG
						if (MapEffect.GetSpriteID()>20)
						{
							DEBUG_ADD_FORMAT("Weather[%d] : spriteID=%d", i, MapEffect.GetSpriteID());
						}
					#endif
					*/
				}
			}
		}

		__END_PROFILE("DrawWeather")

		#ifdef OUTPUT_DEBUG_DRAW_PROCESS
			DEBUG_ADD( "End DrawWeather" );
		#endif

#ifdef __METROTECH_TEST__
		if(!g_bLight)
		{
#endif
		if((g_pZone->GetID() != 1006 || !g_pPlayer->IsInSafeSector()) && !g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST ))
		{
			//----------------------------------------------------------------
			//
			// Draw LightFilter to Screen
			//
			//----------------------------------------------------------------
			__BEGIN_PROFILE("DrawLightFilter")
				
				//----------------------------------------------------------------
				// player狼 light 谅钎 汲沥
				//----------------------------------------------------------------
				int pX = g_pPlayer->GetPixelX() - m_FirstZonePixel.x + g_TILE_X_HALF;
			int pY = g_pPlayer->GetPixelY() - m_FirstZonePixel.y - TILE_Y;
			char sh[255]={0};
			// add by Sonic 2006.9.30 修正人物视野焦点
			if(g_MyFull)
			{
				pX-=110;
				pY-=75;
			}
			// End by Sonic 2006.9.30
			//MessageBox(0,sh,"",MB_OK);
			//----------------------------------------------------------------
			// 3D 
			//----------------------------------------------------------------
			// 冠零牢 版快 矫具 +3	
			//int addLight = (g_pPlayer->GetCreatureType()==CREATURETYPE_BAT ? 3 : 0);
//			int addLight = (g_pPlayer->IsFlyingCreature() ? 3 : 0);
			g_pPlayer->CalculateLightSight();
			int playerLight = g_pPlayer->GetLightSight();// + g_pPlayer->GetItemLightSight() + addLight);
			
			if (g_pPlayer->IsInDarkness())
			{
				playerLight = min(playerLight, g_pPlayer->GetDarknessCount());
			}
//			if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_BLINDNESS))
//				playerLight = 1;

		//	DEBUG_ADD_FORMAT("######## Sight  (%d)", playerLight); 
			if (CDirect3D::IsHAL())
			{ 
				AddLightFilter3D( pX, 
					pY - (g_pPlayer->IsFlyingCreature()? 72:0 ),	//g_pPlayer->GetZ(), 
					playerLight, 
					false,	// screenPixel谅钎			
					true);	// 公炼扒 免仿秦具窍绰 蝴		
				
				// 坷配官捞 阂蝴
				ADD_MOTORCYCLE_LIGHT_XY_3D( g_pPlayer, pX, pY, true );		
				
				//---------------------------------------
				// UNLOCK
				//---------------------------------------
				m_pSurface->Unlock();
				
				//---------------------------------------
				// 3D 矫具贸府 免仿
				//---------------------------------------
				point.x = 0;
				point.y = 0;
				rect.left = 0;
				rect.top = 0;
				rect.right = g_GameRect.right;
				rect.bottom = g_GameRect.bottom;
				g_pBack->BltNoColorkey( &point, g_pLast, &rect );		

				g_pTopView->DrawLightBuffer3D();

				g_pLast->BltNoColorkey( &point, g_pBack, &rect );		

//				g_pLast->FillSurface(0x001F);

//				DrawLightBuffer3D();
				
				// Surface狼 沥焊甫 历厘秦敌促.
				//SetSurfaceInfo(&m_SurfaceInfo, m_pSurface->GetDDSD());
			}
			//----------------------------------------------------------------
			// 2D
			//----------------------------------------------------------------
			else
			{
				// 冠零牢 版快 矫具 +3		
				AddLightFilter2D( pX, 
					pY - g_pPlayer->GetZ(), 
					playerLight,  
					false,	// screenPixel谅钎			
					true);	// 公炼扒 免仿秦具窍绰 蝴
				
				// 坷配官捞 阂蝴
				ADD_MOTORCYCLE_LIGHT_XY_2D( g_pPlayer, pX, pY, true );	
				
				DrawLightBuffer2D();
				
				//---------------------------------------
				// UNLOCK
				//---------------------------------------
				m_pSurface->Unlock();
			}
			
			__END_PROFILE("DrawLightFilter")
#ifdef __METROTECH_TEST__
		}
#endif
		}
		m_pSurface->Unlock();


		//----------------------------------------------------------------	
		// Unlock 惑怕捞促..
		//----------------------------------------------------------------		

		//----------------------------------------------------------------
		// Mouse肺 急琶等 某腐磐捞搁.. 灌篮 臂磊 免仿
		//----------------------------------------------------------------			
		if(IsRequestInfo())
			DrawCreatureMyName();
		if (m_pSelectedCreature != NULL)
		{
			__BEGIN_PROFILE("DrawSelectedCreature")

			DrawCreatureName( m_pSelectedCreature );

			__END_PROFILE("DrawSelectedCreature")
		}
		else
		{
			m_bDrawRequest = false;
		}


		//----------------------------------------------------------------
		// Item捞抚甸阑 免仿窍霸 茄促.
		//----------------------------------------------------------------
		if (m_bDrawItemNameList && !g_pPlayer->IsInDarkness())
		{
			__BEGIN_PROFILE("DrawItemNameList")

			// unlock惑怕俊辑 龋免..
			DrawItemNameList();		

			__END_PROFILE("DrawItemNameList")
		}
	} // bPlayerInCasket
	else 
	{
		// [货扁贱3]
		m_pSurface->Unlock();
	}

	//----------------------------------------------------------------	
	// Lock
	//----------------------------------------------------------------	
	//m_pSurface->Lock();

	//----------------------------------------------------------------
	// 拳搁俊 唱坷绰 臂磊甫 免仿茄促.
	//----------------------------------------------------------------
	__BEGIN_PROFILE("DrawTextList")

	DrawTextList();

	__END_PROFILE("DrawTextList")

	//-------------------------------------------------
	// 免仿且 string俊 扁撅等吧 促 绝局霖促.
	//-------------------------------------------------
	ClearTextList();

	//-------------------------------------------------
	// guild mark 免仿 抛胶飘
	//-------------------------------------------------
//	#ifdef _DEBUG
//
//		if (g_pDXInput->KeyDown(DIK_LCONTROL) && g_pDXInput->KeyDown(DIK_G))
//		{		
//			int guildID = g_pPlayer->GetX()%10;
//
//			//-------------------------------------------------
//			// load登绢 乐绰瘤 夯促.
//			//-------------------------------------------------
//			CSprite* pSprite = g_pGuildMarkManager->GetGuildMark(guildID);
//
//			if (pSprite==NULL)
//			{		
//				//-------------------------------------------------
//				// file俊 乐绰瘤 夯促.
//				//-------------------------------------------------
//				g_pGuildMarkManager->LoadGuildMark(guildID);
//
//				//-------------------------------------------------
//				// file俊辑 load登菌绰瘤 促矫 眉农
//				//-------------------------------------------------
//				pSprite = g_pGuildMarkManager->GetGuildMark(guildID);
//
//				//-------------------------------------------------
//				// file俊档 绝绰 版快..
//				// guildMark包府辑滚?俊辑 罐酒柯促.
//				//-------------------------------------------------
//				if (pSprite==NULL)
//				{
//					// 咯扁辑绰.. 烙矫肺 拳老俊辑 积己茄促.
//					char str[256];
//					sprintf(str, "Data\\Guild\\Guild%d.bmp", guildID);
////					g_pGuildMarkManager->CreateGuildMark( guildID, str );
//				}
//			}
//			else
//			{			
//				// 累篮芭档 傲 窃 嘛绢夯促.
//				CSprite* pSpriteSmall = g_pGuildMarkManager->GetGuildMarkSmall(guildID);
//
//				m_pSurface->Lock();
//
//				POINT point = { 100, 100 };
//
//				m_pSurface->BltSprite(&point, pSprite);
//
//				if (pSpriteSmall!=NULL)
//				{
//					point.x = 150;
//					point.y = 100;
//					m_pSurface->BltSprite(&point, pSpriteSmall);
//				}
//
//				m_pSurface->Unlock();
//			}		
//		}
//	#endif


	//----------------------------------------------------------------
	// Light 免仿
	//----------------------------------------------------------------
	
	/*
	point.x = 210;
	point.y = 200;
	rect.left = 0;
	rect.top = 0;
	rect.right = 400;
	rect.bottom = 200;
	m_pSurface->CopyTransAlpha(&point, m_pBuffer, &rect, 8);
	*/
	

	//----------------------------------------------------------------
	// Object 免仿	
	//----------------------------------------------------------------
	// Y谅钎肺 sort甫 秦辑 免仿秦具 茄促.
	//----------------------------------------------------------------
	// list措脚俊 priority queue甫 荤侩窍绰霸 炒瘤 酵促! sort磊悼~~
	// 弊繁单! list俊 眠啊瞪锭 鉴辑啊 乐栏聪鳖... 包拌啊 绝阑 芭 鞍促!
	//----------------------------------------------------------------

	


	//----------------------------------------------------------------
	// Effect Test
	//----------------------------------------------------------------
	// sword
	/*
	static int swordFrameID = 5;
	static int swordD = 0;
	static int swordFrame = 0;

	point.x = 380;
	point.y = 260;;
	m_pSurface->BltAlphaSprite(&point, &m_EffectAlphaSPK[ m_EffectAlphaFPK[swordFrameID][swordD][swordFrame].GetSpriteID() ]);
		
	if (++swordFrame == 12) swordFrame = 0;

	// Aura
	static int auraFrameID = 3;
	static int auraD = 0;
	static int auraFrame = 0;

	point.x = 320;
	point.y = 220;;	
	m_pSurface->BltAlphaSprite(&point, &m_EffectAlphaSPK[ m_EffectAlphaFPK[auraFrameID][auraD][auraFrame].GetSpriteID() ]);

	if (++auraFrame == 5) auraFrame = 0;
	*/

	//----------------------------------------------------------------
	//
	// TEST CODE  - SpriteOutlineManager Test
	//
	//----------------------------------------------------------------
	/*
	static	CSpriteOutlineManager	SOM;
	static int f = 0;

	if (f==0)
	{
		SOM.Add( 100,70, &m_ImageObjectSPK[38] );
		SOM.Add( 80,100, &m_ImageObjectSPK[38] );
		SOM.Add( 130,100, &m_ImageObjectSPK[39] );
		SOM.Add( 100,130, &m_ImageObjectSPK[39] );

		SOM.Generate();

		f=1;
	}

	m_pSurface->BltSpriteOutline( &SOM, 0xFFFF );
	*/

	/*
	// 弊覆磊 Test		
	if (g_pDXInput->KeyDown(DIK_SPACE))
	{
		point.x = g_x;
		point.y = g_y;

		if (CDirect3D::IsHAL())
		{
			sprite = 0;//rand()%m_pImageObjectShadowManager->GetMaxIndex();

			m_pSurface->Unlock();
			// EffectTexture 汲沥
			//CDirect3D::GetDevice()->SetTexture(0, m_EffectTPK[sprite].GetSurface());
			CSpriteSurface* pSurface = m_pImageObjectShadowManager->GetTexture(sprite);		
			CDirect3D::GetDevice()->SetTexture(0, pSurface->GetSurface());
			
			// 谅钎 汲沥
			m_SpriteVertices[0].sx = point.x;	
			m_SpriteVertices[0].sy = point.y;
			//m_SpriteVertices[1].sx = point.x,m_EffectTPK[sprite].GetWidth();
			m_SpriteVertices[1].sx = point.x + m_pImageObjectShadowManager->GetWidth(sprite);
			m_SpriteVertices[1].sy = point.y;
			m_SpriteVertices[2].sx = point.x;
			//m_SpriteVertices[2].sy = point.y,m_EffectTPK[sprite].GetHeight();
			m_SpriteVertices[2].sy = point.y + m_pImageObjectShadowManager->GetHeight(sprite);
			m_SpriteVertices[3].sx = m_SpriteVertices[1].sx;
			m_SpriteVertices[3].sy = m_SpriteVertices[2].sy;		
			
			// 免仿
			CDirect3D::GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 
									D3DFVF_TLVERTEX, 
									m_SpriteVertices, 
									4, 0);

			m_pSurface->Lock();			
		}
		else
		{
			sprite = 0;//rand()%m_ImageObjectSSPK.GetSize();

			m_pSurface->BltShadowSpriteDarkness( &point, &m_ImageObjectSSPK[sprite], 1);
		}
	}	
	*/	

	//------------------------------------------------------
	// (目辑)
	// 框流捞绰 规氢捞 拌加 官差绊 乐绰 版快 --> 目辑 钎泅	
	//
	//------------------------------------------------------
	/*
	if (!bPutSelectedSector && g_bLButtonDown)			
	{
		POINT cursorPoint;
		GetCursorPos(&cursorPoint);
		// 弊府绊, 泅犁 Player乐绰 困摹甫 急琶窍瘤 臼篮 版快捞促.
		point = GetSelectedSector(cursorPoint.x, cursorPoint.y);
		if (g_pPlayer->GetX()!=point.x || g_pPlayer->GetY()!=point.y)
		{
			// player啊 框流捞绰 规氢栏肺 目辑 葛剧 钎矫
			int direction = g_pPlayer->GetDirection();

			// [ TEST CODE ]
			point.x = cursorPoint.x - 32;
			point.y = cursorPoint.y - 16;
			m_pSurface->BltSprite(&point, &m_EtcSPK[5,direction]);
		}
	}
	*/


	//---------------------------------------
	// UNLOCK
	//---------------------------------------
	//m_pSurface->Unlock();



	//---------------------------------------
	// Filter康开 荤阿屈
	//---------------------------------------
	//rect.left = 410 - 100;
	//rect.right = 410 + 100;
	//rect.top = 270 - 100;
	//rect.bottom = 270 + 100;
	//m_pSurface->DrawRect(&rect, 0xFFFF);

	/*
	// Creature Sprite 焊咯林扁
	static int s = 3;

	if (++s == 43) s=3;

	point.x = 500;
	point.y = 400;
	m_pSurface->BltSprite(&point, m_SpritePack.GetSprite( s ));
	*/

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "End DrawZone" );
	#endif
}

//----------------------------------------------------------------------
// Draw TileSurface
//----------------------------------------------------------------------
// - Player谅钎甫 吝缴栏肺 秦辑...
//   窜瘤 Tile阑 m_pTileSurface俊 弊赴促.
//
// - 泅犁 拳搁狼 Object甸阑 八祸秦辑 免仿俊 捞侩且 荐 乐霸 扁撅秦敌促.
//----------------------------------------------------------------------
void	
MTopView::DrawTileSurface()
{
	//-------------------------------------------------
	//
	// Object甫 免仿窍扁 困茄 沥焊
	//
	//-------------------------------------------------
	// sprite point
	POINT	tilePoint, point;
	RECT	rect;
	
	//---------------------------------------------------------------
	// Player甫 吝缴俊 滴扁 困茄 霉锅掳 Sector谅钎甫 备茄促.
	// 拳搁狼 (0,0)捞 唱鸥郴绰 Zone狼 Pixel谅钎
	//---------------------------------------------------------------
	m_FirstZonePixel = MapToPixel(g_pPlayer->GetX()+g_SECTOR_SKIP_PLAYER_LEFT, 
									g_pPlayer->GetY()+g_SECTOR_SKIP_PLAYER_UP);
	m_FirstZonePixel.x += g_pPlayer->GetSX();
	m_FirstZonePixel.y += g_pPlayer->GetSY();

	#ifdef OUTPUT_DEBUG
		if (g_pPlayer->GetX() < 0 
			|| g_pPlayer->GetY() < 0 
			|| g_pPlayer->GetX() >= g_pZone->GetWidth()
			|| g_pPlayer->GetY() >= g_pZone->GetHeight()
			|| m_FirstZonePixel.x > 12000 || m_FirstZonePixel.y > 6144
			|| m_FirstZonePixel.x < 0 || m_FirstZonePixel.y < 0)
		{
			if (g_pDebugMessage)
			{
				DEBUG_ADD_FORMAT("[Error]SX,SY-DrawTileSurface:Player=(%d, %d), s(%d,%d)", g_pPlayer->GetX(), g_pPlayer->GetY(), g_pPlayer->GetSX(), g_pPlayer->GetSY());
			}		
		}
	#endif
	
	//---------------------------------------------------------------	
	// 拳搁狼 (0,0)俊 免仿登绰 Sector
	//---------------------------------------------------------------		
	POINT	firstSector;
	firstSector = PixelToMap(m_FirstZonePixel.x, m_FirstZonePixel.y);
	m_FirstSector = firstSector;

	//---------------------------------------------------------------
	// TileSurface (0,0)俊 免仿登绰 Sector谅钎
	//---------------------------------------------------------------
	firstSector.x -= TILESURFACE_SECTOR_EDGE;
	firstSector.y -= TILESURFACE_SECTOR_EDGE;

	//---------------------------------------------------------------
	// TileSurface (0,0)狼 Zone俊辑狼 pixel谅钎甫 扁撅秦敌促.
	//---------------------------------------------------------------
	m_TileSurfaceFirstSectorX		= firstSector.x;
	m_TileSurfaceFirstSectorY		= firstSector.y;
	
	point = MapToPixel(m_TileSurfaceFirstSectorX, m_TileSurfaceFirstSectorY);
	m_TileSurfaceFirstZonePixelX	= point.x;
	m_TileSurfaceFirstZonePixelY	= point.y;
	

	//----------------------------------------------------------------------
	//
	//                         Tile 免仿
	//
	//----------------------------------------------------------------------
	//----------------------------------------------------------------------
	// 公炼扒 弥檬 Sector啊 TileSurface狼 (0,0)俊 坷档废 秦具茄促.
	//----------------------------------------------------------------------
	int	sX1 = firstSector.x, 
			sX2 = firstSector.x + g_TILESURFACE_SECTOR_WIDTH,		// sX1 ~ sX2
			sY1 = firstSector.y, 
			sY2 = firstSector.y + g_TILESURFACE_SECTOR_HEIGHT;	// sY1 ~ sY2

	// 免仿且 Surface惑狼 困摹	
	tilePoint.x = 0;
	tilePoint.y = 0;
		
	//------------------------------------------------------
	// Zone狼 康开捞 酒囱 版快俊 Skip...
	//------------------------------------------------------					
	if (sX1 < 0) 
	{			
		// Zone寇胞 何盒篮 八篮祸栏肺 磨茄促.
		rect.left = tilePoint.x;
		rect.top = 0;	
		
		tilePoint.x += -sX1 * TILE_X;

		rect.right = tilePoint.x;
		rect.bottom = g_TILESURFACE_HEIGHT;				 

		m_pTileSurface->FillRect(&rect, 0);
		
		sX1 = 0;	
	}

	if (sX2 > m_pZone->GetWidth())
	{
		sX2 = m_pZone->GetWidth();//-1;

		// Zone寇胞 何盒篮 八篮祸栏肺 磨茄促.
		rect.left = tilePoint.x + (sX2-sX1)*TILE_X;
		rect.top = 0;					
		rect.right = g_TILESURFACE_WIDTH;
		rect.bottom = g_TILESURFACE_HEIGHT;

		m_pTileSurface->FillRect(&rect, 0);
	}

	if (sY1 < 0)
	{
		// Zone寇胞 何盒篮 八篮祸栏肺 磨茄促.
		rect.left = tilePoint.x;
		rect.top = tilePoint.y;	
		
		tilePoint.y += -sY1 * TILE_Y;

		rect.right = tilePoint.x + (sX2-sX1)*TILE_X;
		rect.bottom = tilePoint.y;
		m_pTileSurface->FillRect(&rect, 0);
		
		sY1 = 0;	
	}

	if (sY2 > m_pZone->GetHeight())
	{
		// Zone寇胞 何盒篮 八篮祸栏肺 磨茄促.
		rect.left = tilePoint.x;
		rect.top = tilePoint.y;					
		rect.right = tilePoint.x + (sX2-sX1)*TILE_X;
		rect.bottom = g_TILESURFACE_HEIGHT;
		m_pTileSurface->FillRect(&rect, 0);

		sY2 = m_pZone->GetHeight();//-1;
	}			
			
	//------------------------------------------------------
	// 老窜 八篮祸栏肺 傈眉甫 磨茄促.
	//------------------------------------------------------
	/*
	rect.left = 0;
	rect.top = 0;
	rect.right = TILESURFACE_WIDTH;	
	rect.bottom = TILESURFACE_HEIGHT;

	m_pTileSurface->FillRect(&rect, 0);
	*/

	//------------------------------------------------------
	// 阿 Sector免仿
	//------------------------------------------------------	
	// 霉锅掳 临
	POINT tilePointTemp;
	tilePointTemp.y = tilePoint.y;

	register int x;
	register int y;

	DEBUG_ADD_FORMAT("[DrawTileSurface] (%d, %d) ~ (%d, %d)", sX1, sX2, sY1, sY2);
	
	//---------------------------------------
	// LOCK
	//---------------------------------------
	if (!m_pTileSurface->Lock()) return;

	//char str[80];
	for (y=sY1; y<sY2; y++)
	{				
		// 茄 临狼 霉锅掳 Sector					
		tilePointTemp.x = tilePoint.x;

		for (x=sX1; x<sX2; x++)
		{				
			point = tilePointTemp;

			// (sX,sY) Sector狼 SpriteID甫 佬绢辑 免仿
			//m_pTileSurface->Lock();			
		
			int spriteID = m_pZone->GetSector(x,y).GetSpriteID();

			if (spriteID==SPRITEID_NULL)
			{
#ifdef __DEBUG_OUTPUT__ 
				if( g_pZone->GetID() == 3001 && m_pZone->GetSector(x,y).IsBlockAny() )
					m_pTileSurface->BltSprite(&point, &m_EtcSPK[1]);				
				else
					m_pTileSurface->BltSprite(&point, &m_EtcSPK[SPRITEID_TILE_NULL]);				
#else
				m_pTileSurface->BltSprite(&point, &m_EtcSPK[SPRITEID_TILE_NULL]);				
#endif
			}
			else
			{
				#ifdef OUTPUT_DEBUG
					//if (g_pDebugMessage)
					//	DEBUG_ADD_FORMAT("Draw Tile (%d, %d) id=%d", x, y, spriteID);
				#endif

				CSprite& sprite = m_TileSPK[ spriteID ];

				//---------------------------------------
				// ID啊 spriteID牢 Tile阑 Load茄促.
				//---------------------------------------
//				#ifdef	OUTPUT_DEBUG
//					char str[256];
//				#endif
//				if (sprite.IsNotInit())
//				{
//					#ifdef	OUTPUT_DEBUG
//						sprintf(str, "[RunTimeLoading] Tile(%d,%d) - DrawTileSurface : sprite=%d", x, y, spriteID);
//					#endif
//
//					m_TileSPKFile.seekg(m_TileSPKI[spriteID], ios::beg);
//					//--------------------------------------------------
//					// 己傍茄 版快.. 
//					//--------------------------------------------------
//					if (m_TileSPK[spriteID].LoadFromFile( m_TileSPKFile ))
//					{
//						#ifdef	OUTPUT_DEBUG
//							strcat(str, "...OK");
//						#endif
//					}
//					//--------------------------------------------------
//					// 角菩茄 版快 --> 捞固 Loading窍绊 乐绰 版快捞促.				
//					//--------------------------------------------------
//					/*
//					// 2001.8.20 林籍贸府
//					else
//					{
//						#ifdef	OUTPUT_DEBUG
//							strcat(str, "...Fail & Wait Loading");
//						#endif
//
//						// file thread 鉴困甫 臭腮促.
//						//SetThreadPriority(g_hFileThread, THREAD_PRIORITY_HIGHEST);
//
//						// Thread俊辑 Loading捞 场朝锭鳖瘤 扁促赴促.
//						//while (m_TileSPK[spriteID].IsNotInit());
//						//while (!m_TileSPK[spriteID].LoadFromFile( m_TileSPKFile ));
//						MLoadingSPKWorkNode3* pNode = new MLoadingSPKWorkNode3(spriteID, m_TileSPKI[spriteID]);
//						pNode->SetSPK( &m_TileSPK, FILE_SPRITE_TILE );
//						pNode->SetType( 1 );
//						g_pLoadingThread->SetPriority( THREAD_PRIORITY_HIGHEST );
//						g_pLoadingThread->AddFirst( pNode );
//
//						while (1)
//						{
//							#ifdef	OUTPUT_DEBUG
//							//	DEBUG_ADD_FORMAT( "Check Load id=%d", spriteID );
//							#endif	
//
//							if (m_TileSPK[spriteID].IsInit())
//							{
//								#ifdef	OUTPUT_DEBUG
//								//	DEBUG_ADD( "Is Init" );
//								#endif	
//
//								break;
//							}
//							else
//							{
//								#ifdef	OUTPUT_DEBUG
//								//	DEBUG_ADD( "Is Not Init" );
//								#endif	
//							}
//						}
//
//						// file thread 鉴困甫 撤冕促.
//						//SetThreadPriority(g_hFileThread, THREAD_PRIORITY_BELOW_NORMAL);					
//						g_pLoadingThread->SetPriority( THREAD_PRIORITY_LOWEST );
//					}
//					*/
//
//					DEBUG_ADD( str );
//				}

				// 哆厩~~!!!!!!! 加档 棱酒 冈绰促~!!!
//				POINT pointTempTemp = point;
//				m_pTileSurface->BltSprite(&pointTempTemp, &m_EtcSPK[SPRITEID_TILE_NULL]);

				m_pTileSurface->BltSprite(&point, &sprite);

				#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
					if (m_pZone->GetSector(x,y).IsBlockGround())
					{
						if (g_pDXInput->KeyDown(DIK_T) && g_pDXInput->KeyDown(DIK_LCONTROL))
						{	
							m_pTileSurface->BltSpriteColor(&point, &sprite, 0);
						}
					}
				#endif				
			}
		
//			m_PreviousFogSpriteID[y][x] = SPRITEID_NULL;//sector.GetFilterSpriteID();
			//m_pTileSurface->Unlock();		
			

			// 免仿窍妨绰 谅钎 捞悼
			tilePointTemp.x += TILE_X;
		}		
				
		// 促澜 临
		tilePointTemp.y += TILE_Y;					
	}		

	//---------------------------------------
	// UNLOCK
	//---------------------------------------
	m_pTileSurface->Unlock();
}

//----------------------------------------------------------------------
// Draw PartyHP
//----------------------------------------------------------------------
void
MTopView::DrawPartyHP(POINT* pPoint, MCreature* pCreature)
{
	pPoint->x += 10;

	//-----------------------------------------------------
	// HP拌魂
	//-----------------------------------------------------
	int currentHP		= pCreature->GetHP();
	int maxHP			= pCreature->GetMAX_HP();

	//-----------------------------------------------------
	// 顶加俊 乐绰 局绰 HP啊 菜 蛮巴贸烦 焊咯霖促.
	//-----------------------------------------------------
	if (pCreature->IsUndergroundCreature())
	{
		currentHP = 1;
		maxHP = 1;
	}

	if (currentHP > maxHP)
	{
		currentHP = maxHP;
	}

	//-----------------------------------------------------
	// 角力肺 焊咯龙 pixel拌魂
	//-----------------------------------------------------
	CSprite* pHPBackSprite	= &m_EtcSPK[SPRITEID_PARTY_HP_BACKGROUND];
	CSprite* pHPSprite		= &m_EtcSPK[SPRITEID_PARTY_HP];

	int maxPixels		= pHPBackSprite->GetWidth();
	int currentPixels	= (maxHP==0)? 0 : maxPixels * currentHP / maxHP;

	//-----------------------------------------------------
	// 免仿
	//-----------------------------------------------------
	POINT pointTemp;

	pointTemp.x = pPoint->x;
	pointTemp.y = pPoint->y;
	m_pSurface->BltSprite(&pointTemp, pHPBackSprite);

	pointTemp.x = pPoint->x;
	pointTemp.y = pPoint->y;

	int clipRight = pointTemp.x + currentPixels;

	if (clipRight > 0)
	{
		if (clipRight < m_pSurface->GetWidth())
		{
			int oldClipRight = m_pSurface->GetClipRight();
			m_pSurface->SetClipRight( clipRight );

			m_pSurface->BltSprite(&pointTemp, pHPSprite);

			m_pSurface->SetClipRight( oldClipRight );
		}
		else
		{
			m_pSurface->BltSprite(&pointTemp, pHPSprite);
		}
	}
}


//----------------------------------------------------------------------
// Draw InventoryEffect
//----------------------------------------------------------------------
void
MTopView::DrawInventoryEffect(POINT* pPoint)
{
	if (g_pInventoryEffectManager!=NULL)
	{			
		//------------------------------------------------
		// Unlock 惑怕肺 父电促. 
		//------------------------------------------------
		BOOL bLock = m_pSurface->IsLock();
		if (!bLock)
		{
			m_pSurface->Lock();
		}

		MScreenEffect::SetScreenBasis( pPoint->x, pPoint->y );
		
		g_pTopView->DrawEffect(pPoint, g_pInventoryEffectManager->GetEffects(), g_pInventoryEffectManager->GetSize());

		//------------------------------------------------
		// 盔贰 lock 等 惑怕搁 促矫 登倒赴促.
		//------------------------------------------------
		if (!bLock)
		{
			m_pSurface->Unlock();
		}
	}
}


//----------------------------------------------------------------------
// Draw InventoryEffect
//----------------------------------------------------------------------
void
MTopView::DrawTitleEffect(POINT* pPoint)
{
	int _FrameID = EFFECTSPRITETYPE_UI_TITLE_FIRE;
	int _Direction = 0;
	int _Frame = (g_FrameCount % 80)>>1;
	CEffectFrame& Frame = m_EffectAlphaFPK[_FrameID][_Direction][_Frame];
	int spriteID = Frame.GetSpriteID();
	POINT point = *pPoint;
	if (spriteID != SPRITEID_NULL)
	{
		point.x += Frame.GetCX();
		point.y += Frame.GetCY();

		DRAW_ALPHASPRITEPAL(&point, spriteID, m_EffectAlphaSPK, m_EffectAlphaPPK[_FrameID])//, m_EffectAlphaSPKI, m_EffectAlphaSPKFile)
	}
}
// 2004, 11, 22, sobeit add start
void
MTopView::DrawBloodBibleEffect_InGear(POINT* pPoint)
{
	int _FrameID = EFFECTSPRITETYPE_BLOODY_BIBLE_IN_GEAR;
	int _Direction = 0;
	int _Frame = (g_FrameCount % 112)>>3; 
	CEffectFrame& Frame = m_EffectAlphaFPK[_FrameID][_Direction][_Frame];
	int spriteID = Frame.GetSpriteID();
	POINT point = *pPoint;
	if (spriteID != SPRITEID_NULL)
	{
		point.x += Frame.GetCX();
		point.y += Frame.GetCY();

		DRAW_ALPHASPRITEPAL(&point, spriteID, m_EffectAlphaSPK, m_EffectAlphaPPK[_FrameID])//, m_EffectAlphaSPKI, m_EffectAlphaSPKFile)
	}
}
// 2004, 11, 22, sobeit add end
//----------------------------------------------------------------------
// Draw Chat String
//----------------------------------------------------------------------
int
	MTopView::DrawChatString(POINT* pPoint, MCreature* pCreature, COLORREF color, BYTE flag)
{
	
	int y2 = pPoint->y;
	// 距埃 哭率俊辑 免仿...
	//pPoint->x -= 50;

	// 2005, 1, 20, sobeit add start
	if(g_pUserOption->DoNotShowPersnalShopMsg && pCreature->CurPernalShop())
	{
		//str = "...";
		return y2;
	}
	// 2005, 1, 20, sobeit add end
	//---------------------------------------------------------
	//
	//			Font汲沥
	//
	//---------------------------------------------------------	
	FONTID	font;
	//---------------------------------------------------------
	// Vampire客 Slayer俊 蝶扼辑 Font甫 促福霸 秦霖促.
	//---------------------------------------------------------
	if (pCreature->IsVampire())
	{
		//gC_font.NotUseNoPublicEng();

		font	= FONTID_VAMPIRE_CHAT;
	}
	else //if (pCreature->IsSlayer() || pCreature->IsNPC())	
	{
		font	= FONTID_SLAYER_CHAT;
	}	

	bool	bLargeFont = pCreature->HasEffectStatus( EFFECTSTATUS_LOUD );
	if( bLargeFont )
		font = FONTID_LARGE_CHAT;

	const int MinWidthChatBox = bLargeFont ? MIN_CHAT_LARGE_BOX_WIDTH : MIN_CHAT_BOX_WIDTH;
	const int ChatFontHeight = bLargeFont ? g_pClientConfig->FONT_HEIGHT + 5 : g_pClientConfig->FONT_HEIGHT ;
	//---------------------------------------------------------
	//
	//		力老 变 string狼 辨捞甫 掘绰促.
	//
	//---------------------------------------------------------
	int maxLen = 0;
	int numString = g_pClientConfig->MAX_CHATSTRING;
	int maxWidth = 0;
	for (register int i=g_pClientConfig->MAX_CHATSTRING_MINUS_1; i>=0; i--)
	{
		const char *str = pCreature->GetChatString(i);		

		//---------------------------------------------------------
		// 购啊 乐栏搁.. 免仿秦具茄促.
		//---------------------------------------------------------
		if (str[0] != NULL)
		{
			int width = max( MinWidthChatBox, g_GetStringWidth(str, g_ClientPrintInfo[font]->hfont) );

			if (width > maxWidth)
			{
				maxWidth = width;
			}			
		}
		else
		{	
			numString = g_pClientConfig->MAX_CHATSTRING_MINUS_1 - i;

			//---------------------------------------------------------
			// 免仿且霸 绝绰 版快	
			//---------------------------------------------------------
			if (numString==0)
			{
				return y2;
			}

			break;
		}
	}

	//---------------------------------------------------------
	//
	//			免仿 沥焊 汲沥
	//
	//---------------------------------------------------------	
	int firstY = y2 - numString * ChatFontHeight;

	int x = pPoint->x;
	int y = firstY;



	//---------------------------------------------------------
	// 拳搁 救俊 臂磊啊 甸绢坷档废..
	//---------------------------------------------------------
	if (x<0) x=0;

	if (m_SelectCreatureID == pCreature->GetID())
	{
		const int FontHeight = ChatFontHeight;
			
		if (pCreature->HasLevelName())
		{
			const int FontHeight2 = FontHeight<<1;

			if (y<FontHeight2)
			{
				firstY = FontHeight2;

				y2 = (numString+2) * FontHeight;
				y = FontHeight2;
			}
		}
		else if (y<FontHeight)
		{
			firstY = FontHeight;

			y2 = (numString+1) * FontHeight;
			y = FontHeight;		
		}
	}
	else	
	{
		if (y<0)
		{
			firstY = 0;
			
			y2 = numString * ChatFontHeight;		
			y=0;
		}
	}

	int x2 = x + maxWidth;//gC_font.GetStringmaxWidth( str ),
	
	if (x2 >= g_GameRect.right)
	{
		x -= x2-g_GameRect.right;
		x2 = g_GameRect.right;
	}

	QWORD timeBase;
	
	if (pCreature==m_pSelectedCreature)
	{
		timeBase = ((QWORD)(g_CurrentFrame+160) << 41);
	}
	else
	{
		if( pCreature->CurPernalShop() == 1 )
			timeBase = ((QWORD)(g_CurrentFrame+pCreature->GetPersonalShopMsgTime()) << 41);
		else		
			timeBase = ((QWORD)pCreature->GetChatTime() << 41) 
						| ((QWORD)pCreature->GetID() << 9);
	}

	//---------------------------------------------------------
	//  阿 string 免仿...
	//---------------------------------------------------------
	int start = g_pClientConfig->MAX_CHATSTRING-numString; 
	for (i=start; i<g_pClientConfig->MAX_CHATSTRING; i++)
	{
		const char *str = pCreature->GetChatString(i);
		
		// 购啊 乐栏搁.. 免仿秦具茄促.
		//if (str[0] != NULL)
		{
			//m_pSurface->GDI_Text(pointTemp.x, pointTemp.y, str, 0xFFFF);
			//gC_font.PrintStringNoConvert(&m_SurfaceInfo, str, pPoint->x, pPoint->y, color);
			DRAWTEXT_NODE*	pNode = new DRAWTEXT_NODE (
				x,
				y+3,
				str,
				color,
				font,
				flag// | FLAG_DRAWTEXT_HANGUL_JOHAP
			);	

			//---------------------------------------------------------
			// 33栏肺 茄芭绰.. DWORD time俊辑 霉 bit啊 鞘夸绝阑 淀 窍绊
			// Creature Object ID肺 某腐磐喊肺 鉴辑俊 嘎苗辑 免仿窍绊 
			// 免仿鉴辑 i甫 眠啊秦辑 鉴辑措肺 免仿登霸 茄 巴捞促.
			//
			// 坷贰等芭 < 某腐磐喊肺 < 临鉴辑
			//---------------------------------------------------------
			QWORD time = timeBase + m_pqDrawText.size();
			pNode->SetTextTime( time );
			
			//---------------------------------------------------------
			// 八篮祸 冠胶
			//---------------------------------------------------------
			if (i==start)
			{
				RECT rect = {
					x - 4, 
					firstY - 2,
					x2 + 4,
					y2 +2
				};

				pNode->SetBox( rect, m_ColorBlackHalf );

				//---------------------------------------------------------
				// 盲泼俊 寇胞 冠胶 弊府扁
				//---------------------------------------------------------
				if (g_pUserOption->DrawChatBoxOutline)
				{
					DWORD color_degree = pCreature->GetOriChatColor();
					BYTE	r, 
							//g, 
							b = (color_degree&0xFF000000)>>24;//GetBValue(  ) >> 3;

					b = min(max(b-8, 0), 20);
					r = min(24, b+4);
					//g = b;
					if(pCreature->CurPernalShop()==1)
						pNode->SetBoxOutline( CDirectDraw::Color(255, 0, 200) );
					else
						pNode->SetBoxOutline( CDirectDraw::Color(r, b, b) );
						//CDirectDraw::Color(24, 20, 20) );
				}
			}

			AddText( pNode );
		}
		//else
		//{	
			// y临 谅钎 return
		//	return firstY;
		//}

		// 茄临究 酒贰肺..
		y += ChatFontHeight;
	}

	return firstY;
}

//----------------------------------------------------------------------
// Draw Item : Item 免仿窍扁	
//----------------------------------------------------------------------
// pSurface狼 pPoint俊 pItem阑 免仿茄促. 
//----------------------------------------------------------------------
void	
MTopView::DrawItem(POINT* pPoint, MItem* pItem)
{
	if(g_pEventManager->GetEventByFlag(EVENTFLAG_NOT_DRAW_ITEM))
		return;

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "Start DrawItem" );
	#endif

	__BEGIN_PROFILE("DrawItem")

	//------------------------------------------------
	// Slayer牢 版快 Darkness 救阑 杭 荐 绝促.
	//------------------------------------------------
	if (g_pZone->GetSector(pItem->GetX(), pItem->GetY()).HasDarkness() &&
		IsAffectFromDarkness() &&
		!(
			pItem->GetItemClass() == ITEM_CLASS_CORPSE && 
			((MCorpse *)pItem)->GetCreature() != NULL &&			
			(
				((MCorpse *)pItem)->GetCreature()->GetCreatureType() >= 526 && ((MCorpse *)pItem)->GetCreature()->GetCreatureType() <= 549 ||
				((MCorpse *)pItem)->GetCreature()->GetCreatureType() >= 371 && ((MCorpse *)pItem)->GetCreature()->GetCreatureType() <= 376 ||
				((MCorpse *)pItem)->GetCreature()->GetCreatureType() >= 560 && ((MCorpse *)pItem)->GetCreature()->GetCreatureType() <= 563 ||
				((MCorpse *)pItem)->GetCreature()->GetCreatureType() == 670 ||
				((MCorpse *)pItem)->GetCreature()->GetCreatureType() == 672 ||
				((MCorpse *)pItem)->GetCreature()->GetCreatureType() == 673
			)
		)
		)
	{
		#ifdef OUTPUT_DEBUG_DRAW_PROCESS
			DEBUG_ADD( "End DrawItem(D)" );
		#endif

		return;
	}
	
	//---------------------------------------------------------
	// 矫眉牢 版快绰 Creature甫 免仿茄促.
	//---------------------------------------------------------
	if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
	{
		MCorpse* pCorpse = (MCorpse*)pItem;

		MCreature* pCreature = pCorpse->GetCreature();

		POINT orPoint = *pPoint;

		if (pCreature!=NULL)
		{
			//---------------------------------------------------------
			// PC Vampire牢 版快绰 '刚瘤'肺 钎泅茄促.
			// 
			// 凭滚傈牢版快绰 葛滴 '刚瘤'肺 钎泅茄促.
			//---------------------------------------------------------
//			if (//pCreature->GetCreatureType()==CREATURETYPE_VAMPIRE_MALE 
//				//|| pCreature->GetCreatureType()==CREATURETYPE_VAMPIRE_FEMALE)
//				(*g_pCreatureSpriteTable)[(*g_pCreatureTable)[pCreature->GetCreatureType()].SpriteType].IsPlayerVampireOnlySprite()
//				|| g_pUserInformation->GoreLevel == false)
//			{
//				CSprite* pSprite = &m_EtcSPK[ SPRITEID_VAMPIRE_DUST ];
//
//				POINT pointTemp;
//
//				// 谅钎 焊沥
//				pointTemp.x = pPoint->x - 5;
//				pointTemp.y = pPoint->y;
//					
//
//				// 急琶等 巴牢 版快
//				// Creature甫 急琶茄 巴贸烦秦辑 免仿.. 荤扁寸. - -;
///				if (m_SelectItemID == pItem->GetID())
//				{
//					m_SOMOutlineColor = m_ColorOutlineItem;
//					
//					m_SOM.Add( pointTemp.x, pointTemp.y, pSprite );
//					
//					m_SOM.Generate();
//
//					m_pSurface->BltSpriteOutline( &m_SOM, m_SOMOutlineColor );
//				}
//				else
//				{
//					m_pSurface->BltSprite(&pointTemp, pSprite);	
//				}
//			}
			//---------------------------------------------------------
			// 老馆 某腐磐 磷篮 葛嚼..
			//---------------------------------------------------------
//			else
			{
				// 急琶等 巴牢 版快
				// Creature甫 急琶茄 巴贸烦秦辑 免仿.. 荤扁寸. - -;
				if (m_SelectItemID == pItem->GetID())
				{
					TYPE_OBJECTID temp = m_SelectCreatureID;
					m_SelectCreatureID = pCreature->GetID();

//					if (
//							(
//							pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376	// 己拱
//							|| pCreature->GetCreatureType() == 482	// 农府胶付胶 飘府
//							)
//						||
//							(
//							pCreature->GetActionCount() < pCreature->GetActionCountMax()
//							// 凭滚傈老 版快俊绰 浇饭捞绢矫眉 弊覆磊 免仿秦拎具窃
//							|| g_pUserInformation->GoreLevel == false
//							&& pCreature->GetActionCount()>=pCreature->GetActionCountMax()/2
//							&& pCreature->IsDead()
//							)
//						)
//					{
//						POINT pointTemp = *pPoint;
//						DrawCreatureShadow(&pointTemp, pCreature);
//					}

					DrawCreature(pPoint, pCreature);

					m_SelectCreatureID = OBJECTID_NULL;			
				}
				else
				{
//					if (
//							(
//							pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376	// 己拱
//							|| pCreature->GetCreatureType() == 482	// 农府胶付胶 飘府
//							)
//						||
//							(
//							pCreature->GetActionCount() < pCreature->GetActionCountMax()
//							// 凭滚傈老 版快俊绰 浇饭捞绢矫眉 弊覆磊 免仿秦拎具窃
//							|| g_pUserInformation->GoreLevel == false
//							&& pCreature->GetActionCount()>=pCreature->GetActionCountMax()/2
//							&& pCreature->IsDead()
//							)
//						)
//					{
//						POINT pointTemp = *pPoint;
//						DrawCreatureShadow(&pointTemp, pCreature);
//					}

					DrawCreature(pPoint, pCreature);
				}
			}

			//------------------------------------------------
			// 磷篮 某腐磐捞搁 急琶 荤阿屈阑
			// 茄 Tile 农扁肺 力茄茄促.
			//------------------------------------------------
			if(!(pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376 || pCreature->GetCreatureType() >= 560 && pCreature->GetCreatureType() <= 563 || pCreature->GetCreatureType() >= 526 && pCreature->GetCreatureType() <= 549 || pCreature->GetCreatureType() == 672 || pCreature->GetCreatureType() == 673 ))
			{
				if(pCreature->GetCreatureType() == 482 || pCreature->GetCreatureType() == 650 )	// 飘府绰 select救登霸
				{
					RECT rect;
					rect.left	= orPoint.x;
					rect.top	= orPoint.y;
					rect.right	= orPoint.x;
					rect.bottom = orPoint.y;
					pCreature->SetScreenRect( &rect );			
				}
				else
				{
					RECT rect;
					rect.left	= orPoint.x;
					rect.top	= orPoint.y;
					rect.right	= orPoint.x + TILE_X;
					rect.bottom = orPoint.y + TILE_Y;
					pCreature->SetScreenRect( &rect );			
				}
			}
		}

		#ifdef OUTPUT_DEBUG_DRAW_PROCESS
			DEBUG_ADD( "End DrawItem" );
		#endif

		if(pCreature != NULL)
			pCreature->FadeChatString();	

		return;
	}

	//---------------------------------------------------------
	// 酒流 冻绢瘤绊 乐绰 吝捞搁..
	//---------------------------------------------------------
	// m_ItemDropFPK客 m_ItemDropSPK甫 捞侩..
	//---------------------------------------------------------
	if (pItem->IsDropping())
	{
		int dropFrameID = pItem->GetDropFrameID();
		int frame = pItem->GetFrame();

		//#ifdef OUTPUT_DEBUG
			if (dropFrameID >= m_ItemDropFPK.GetSize())
			{
				DEBUG_ADD_FORMAT("[Error] DrawDropItem:item exceed Max : id=%d, itemFrame=%d", pItem->GetID(), dropFrameID);
				return;
			}
			else if (frame >= m_ItemDropFPK[dropFrameID].GetSize())
			{
				DEBUG_ADD_FORMAT("[Error] DrawDropItem:frame exceed Max : id=%d, itemFrame=%d, Frame=%d", pItem->GetID(), dropFrameID, frame);
				return;
			}
		//#endif

		//---------------------------------------------------------
		// 捞芭 绊摹绊唱辑.. MZone::AddItem( ...) 俊辑
		// if (bDropping) 第狼 林籍何盒阑 混妨具 茄促..
		//---------------------------------------------------------
		CFrame &Frame = m_ItemDropFPK[ dropFrameID ][ frame ];
		//CFrame &Frame = m_ItemTileFPK[ pItem->GetTileFrameID() ];

		POINT pointTemp;
		pointTemp.x = pPoint->x + Frame.GetCX();
		pointTemp.y = pPoint->y + Frame.GetCY() - pItem->GetDropHeight();

		#ifdef OUTPUT_DEBUG
			if (Frame.GetSpriteID() >= m_ItemDropISPK.GetSize())
			{
				DEBUG_ADD_FORMAT("[Error] DrawDropItem:sprite exceed Max : id=%d, spriteid=%d", pItem->GetID(), Frame.GetSpriteID());
				return;
			}			
		#endif

		CIndexSprite* pSprite = &m_ItemDropISPK[ Frame.GetSpriteID() ];
		//CSprite* pSprite = &m_ItemTileSPK[ Frame.GetSpriteID() ];

		// 免仿
		int colorSet = pItem->GetItemOptionColorSet();

//		if(pItem->IsQuestItem())
//			colorSet = MItem::GetQuestItemColorset();
//		else
//		if(pItem->IsUniqueItem())
//			colorSet = MItem::GetUniqueItemColorset();

		if(pItem->IsSpecialColorItem() )
			colorSet = pItem->GetSpecialColorItemColorset();
		
		CIndexSprite::SetUsingColorSet( colorSet, colorSet );

		m_pSurface->BltIndexSprite(&pointTemp, pSprite);							
	}
	//---------------------------------------------------------
	// 官蹿俊 乐绰 item
	//---------------------------------------------------------
	// m_ItemTileFPK客 m_ItemTileSPK甫 捞侩
	//---------------------------------------------------------
	else
	{
		//#ifdef OUTPUT_DEBUG
			//	DEBUG_ADD_FORMAT("[TempDebug] Before Draw Item : %d", pItem->GetID());
		//#endif	

		int tileFrameID = pItem->GetTileFrameID();
		//#ifdef OUTPUT_DEBUG
			if (tileFrameID >= m_ItemTileFPK.GetSize())
			{
				DEBUG_ADD_FORMAT("[Error] DrawItem:frame exceed Max : id=%d, frame=%d", pItem->GetID(), tileFrameID);
				return;
			}			
		//#endif

		CFrame &Frame = m_ItemTileFPK[ tileFrameID ];
		 
		/*
		DEBUG_ADD_FORMAT("class=%d, type=%d, frame=%d, id=%d", pItem->GetItemClass(),
			pItem->GetItemType(),
			pItem->GetTileFrameID(),
			Frame.GetSpriteID());
		*/

		// 谅钎 焊沥
		POINT pointTemp;
		pointTemp.x = pPoint->x + Frame.GetCX();
		pointTemp.y = pPoint->y + Frame.GetCY();

		#ifdef OUTPUT_DEBUG
			if (Frame.GetSpriteID() >= m_ItemTileISPK.GetSize())
			{
				DEBUG_ADD_FORMAT("[Error] DrawItem:sprite exceed Max : id=%d, spriteid=%d", pItem->GetID(), Frame.GetSpriteID());
				return;
			}			
		#endif

		CIndexSprite* pSprite = &m_ItemTileISPK[ Frame.GetSpriteID() ];

		int colorSet = pItem->GetItemOptionColorSet();

//		if(pItem->IsQuestItem())
//			colorSet = MItem::GetQuestItemColorset();
//		else
//		if(pItem->IsUniqueItem())
//			colorSet = MItem::GetUniqueItemColorset();
		
		if(pItem->IsSpecialColorItem() )
			colorSet = pItem->GetSpecialColorItemColorset();

		CIndexSprite::SetUsingColorSet( colorSet, colorSet );

		//---------------------------------------- 		
		// Item 急琶 荤阿屈 康开 汲沥
		// 酒捞袍狼 急琶 康开 农扁甫 力茄茄促.
		//---------------------------------------- 	
		RECT rect;
		rect.left	= max(pointTemp.x, pPoint->x);
		rect.top	= max(pointTemp.y, pPoint->y);
		rect.right	= min(pointTemp.x + pSprite->GetWidth(), pPoint->x + TILE_X);
		rect.bottom = min(pointTemp.y + pSprite->GetHeight(), pPoint->y + TILE_Y);		

		pItem->SetScreenRect( &rect );

		// m_SpritePack俊辑 利例茄 sprite甫 榜扼辑 免仿秦霖促.

		//-------------------------------------------------------
		// Mouse啊 啊府虐绊 乐绰 Item
		//-------------------------------------------------------
		if (m_SelectItemID == pItem->GetID())		
		{		
			m_SOMOutlineColor = m_ColorOutlineItem;

			m_SOM.Add( pointTemp.x, pointTemp.y, pSprite );
			
			m_SOM.Generate();

			//if (CDirect3D::IsHAL() || DarkBits==0)
			//{
				m_pSurface->BltSpriteOutline( &m_SOM, m_SOMOutlineColor );
			//}
			//else
			//{
				//m_pSurface->BltSpriteOutlineDarkness( &m_SOM, m_SOMOutlineColor, DarkBits );
			//}

			//-------------------------------------
			// mouse啊 啊府虐绰 Item狼 沥焊 钎矫
			//-------------------------------------
			/*
			const char* pItemName = pItem->GetName();

			// option捞 嘿篮芭绰 颇鄂祸..
			COLORREF nameColor;
			if (pItem->GetItemOption()==0)
			{
				nameColor = m_ColorNameItem;
			}
			else
			{
				nameColor = m_ColorNameItemOption;
			}

			if (pItemName != NULL)
			{
				FONTID	font = FONTID_ITEM;

				int x = pointTemp.x + (pSprite->GetWidth()>>1);
				int y = pointTemp.y - g_pClientConfig->FONT_ITEM_HEIGHT;			

				// 拳搁 救俊 臂磊啊 甸绢坷档废..
				if (x<0) x=0;
				if (y<0) y=0;

				//--------------------------------------------------
				// GetStringWidth俊辑绰 unlock秦具茄促.
				//--------------------------------------------------
				m_pSurface->Unlock();
				int x2 = x + g_GetStringWidth(pItemName, g_ClientPrintInfo[font]->hfont);
				m_pSurface->Lock();

				int y2 = y + g_pClientConfig->FONT_ITEM_HEIGHT;

				if (x2 >= CLIPSURFACE_WIDTH)
				{
					x -= x2-CLIPSURFACE_WIDTH;
					x2 = CLIPSURFACE_WIDTH;
				}

				DRAWTEXT_NODE* pNode = new DRAWTEXT_NODE(
											x,
											y+1,
											pItemName,										
											nameColor,
											font
				);


				// 八篮祸 冠胶			
				RECT rect = {
					x, 
					y,
					x2,
					y2
				};

				pNode->SetBox( rect, m_ColorBlackHalf );

				AddText( pNode );
			}	
			*/
			//gC_font.PrintStringNoConvert(&m_SurfaceInfo, 
			//							g_ItemTable[pItem->GetItemClass()][pItem->GetItemType()].GetName(), 
			//							pointTemp.x, pointTemp.y, 
			//							0xFFFF
			//);
			
		}
		else
		{	
			//-------------------------------------------------------
			// 眠利 吝牢 Item钎矫
			//-------------------------------------------------------
			//if (g_pPlayer->IsTraceItem() && g_pPlayer->GetTraceID()==pItem->GetID())
			{	
				// RED
			//	m_pSurface->BltSpriteColor(&pointTemp, &m_ItemTileSPK[ Frame.GetSpriteID() ], 0);
			}	
			//-------------------------------------------------------
			// 沥惑 免仿
			//-------------------------------------------------------
			//else 
			{
				//if (CDirect3D::IsHAL() || DarkBits==0)
				//{			
					
				//-------------------------------------------------------
				// 汲摹等 瘤汾捞搁..
				//-------------------------------------------------------
				if (pItem->GetItemClass()==ITEM_CLASS_MINE
					&& ((MMine*)pItem)->IsInstalled())
				{
					// 弧埃祸栏肺 免仿茄促.
					m_pSurface->BltIndexSpriteColor(&pointTemp, pSprite, 0);
				}
				//-------------------------------------------------------	
				// 老馆利牢 酒捞袍
				//-------------------------------------------------------
				else
				{					
					m_pSurface->BltIndexSprite(&pointTemp, pSprite);
				}
				//}
				//else
				//{
					//m_pSurface->BltSpriteDarkness(&pointTemp, &m_ItemTileSPK[ Frame.GetSpriteID() ], DarkBits);
				//}
			}
		}	
		
		// [ TEST CODE ]
		//-------------------------------------
		// mouse啊 啊府虐绰 Item狼 沥焊 钎矫
		//-------------------------------------
		/*
		DRAWTEXT_NODE* pNode = new DRAWTEXT_NODE(
									pointTemp.x + (m_ItemTileSPK[ Frame.GetSpriteID() ].GetWidth()>>1),
									pointTemp.y - 20,
									pItem->GetName(),
									0xFFFF					
		);

		AddText( pNode );
		*/
	}

	//--------------------------------------------------------------------
	// ItemNameList俊 眠啊
	//--------------------------------------------------------------------
	if (m_bDrawItemNameList)
	{
		DWORD distance = max( abs(g_pPlayer->GetX()-pItem->GetX()),
								abs(g_pPlayer->GetY()-pItem->GetY()) );

		AddItemName( new DRAWITEMNAME_NODE(distance, pItem) );
	}
	
	//#ifdef OUTPUT_DEBUG
		//	DEBUG_ADD("[TempDebug] After Draw Item");
	//#endif	

	__END_PROFILE("DrawItem")

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "End DrawItem" );
	#endif

	
}


//----------------------------------------------------------------------
// Draw ItemShadow : Item 弊覆磊 免仿窍扁	
//----------------------------------------------------------------------
// pSurface狼 pPoint俊 pItem阑 免仿茄促.
//----------------------------------------------------------------------
void	
MTopView::DrawItemShadow(POINT* pPoint, MItem* pItem)
{
	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "Start DrawItemShadow" );
	#endif

	__BEGIN_PROFILE("DrawItemShadow")

	//------------------------------------------------
	// Slayer牢 版快 Darkness 救阑 杭 荐 绝促.
	//------------------------------------------------
	if (g_pZone->GetSector(pItem->GetX(), pItem->GetY()).HasDarkness() 
		&&IsAffectFromDarkness())
	{
		#ifdef OUTPUT_DEBUG_DRAW_PROCESS
			DEBUG_ADD( "End DrawItemShadow(D)" );
		#endif

		return;
	}
	
	//---------------------------------------------------------
	// 矫眉牢 版快绰 Creature甫 免仿茄促.
	//---------------------------------------------------------
	if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
	{
		MCorpse* pCorpse = (MCorpse*)pItem;

		MCreature* pCreature = pCorpse->GetCreature();

		if (pCreature!=NULL)
		{
			//---------------------------------------------------------
			// 老馆 某腐磐 磷篮 葛嚼..
			//---------------------------------------------------------
			{
				// 急琶等 巴牢 版快
				// Creature甫 急琶茄 巴贸烦秦辑 免仿.. 荤扁寸. - -;
				if (m_SelectItemID == pItem->GetID())
				{
					TYPE_OBJECTID temp = m_SelectCreatureID;
					m_SelectCreatureID = pCreature->GetID();

					if (
							(
							pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376	// 己拱
							|| pCreature->GetCreatureType() >= 560 && pCreature->GetCreatureType() <= 563	// 己拱
							|| pCreature->GetCreatureType() >= 526 && pCreature->GetCreatureType() <= 549	// 己窜
							|| pCreature->GetCreatureType() == 482	// 农府胶付胶 飘府
							|| pCreature->GetCreatureType() == 650	// 霸矫魄
							|| pCreature->GetCreatureType() == 672
							|| pCreature->GetCreatureType() == 673
							|| pCreature->GetCreatureType() == 730 // 傀捞 器牢飘1
							|| pCreature->GetCreatureType() == 731 // 傀捞 器牢飘2
							|| pCreature->GetCreatureType() == 732 // 傀捞 器牢飘3
							)
						||
							(
							pCreature->GetActionCount() < pCreature->GetActionCountMax()
							// 凭滚傈老 版快俊绰 浇饭捞绢矫眉 弊覆磊 免仿秦拎具窃
							|| g_pUserInformation->GoreLevel == false
							&& pCreature->GetActionCount()>=pCreature->GetActionCountMax()/2
							&& pCreature->IsDead()
							)
						)
					{
						POINT pointTemp = *pPoint;
						DrawCreatureShadow(&pointTemp, pCreature);
					}

					m_SelectCreatureID = OBJECTID_NULL;			
				}
				else
				{
					if (
							(
							pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376	// 己拱
							|| pCreature->GetCreatureType() >= 560 && pCreature->GetCreatureType() <= 563	// 己拱
							|| pCreature->GetCreatureType() >= 526 && pCreature->GetCreatureType() <= 549	// 己拱
							|| pCreature->GetCreatureType() == 482	// 农府胶付胶 飘府
							|| pCreature->GetCreatureType() == 650  // 霸矫魄
							|| pCreature->GetCreatureType() == 672
							|| pCreature->GetCreatureType() == 673
							|| pCreature->GetCreatureType() == 730 // 傀捞 器牢飘1
							|| pCreature->GetCreatureType() == 731 // 傀捞 器牢飘2
							|| pCreature->GetCreatureType() == 732 // 傀捞 器牢飘3
							)
						||
							(
							pCreature->GetActionCount() < pCreature->GetActionCountMax()
							// 凭滚傈老 版快俊绰 浇饭捞绢矫眉 弊覆磊 免仿秦拎具窃
							|| g_pUserInformation->GoreLevel == false
							&& pCreature->GetActionCount()>=pCreature->GetActionCountMax()/2
							&& pCreature->IsDead()
							)
						)
					{
						POINT pointTemp = *pPoint;
						DrawCreatureShadow(&pointTemp, pCreature);
					}
				}
			}

		}

		#ifdef OUTPUT_DEBUG_DRAW_PROCESS
			DEBUG_ADD( "End DrawItemShadow" );
		#endif

	

		return;
	}


	__END_PROFILE("DrawItemShadow")

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "End DrawItemShadow" );
	#endif

	
}

//----------------------------------------------------------------------
// Draw ImageObject : 弊覆 免仿窍扁	
//----------------------------------------------------------------------
// pSurface狼 pPoint俊 pImageObject阑 免仿茄促.
//----------------------------------------------------------------
void	
MTopView::DrawImageObject(POINT* pPoint, MImageObject* pImageObject)
{		
	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "Start DrawImageObject" );
	#endif

	__BEGIN_PROFILE("DrawImageObject")

	#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
		if (g_pDXInput->KeyDown(DIK_T) && g_pDXInput->KeyDown(DIK_LCONTROL))
		{
			return;
		}
	#endif
	
	//#ifdef OUTPUT_DEBUG
		//	DEBUG_ADD_FORMAT("[TempDebug] Before Draw ImageObject : %d", pImageObject->GetID());
	//#endif	

	// m_SpritePack俊辑 利例茄 sprite甫 榜扼辑 免仿秦霖促.
	// Player甫 啊府绰 弊覆老 版快 馆捧疙 贸府
	
	//if (pImageObject->GetImageObjectID() == m_BehindImageObjectID)

	//m_pSurface->HLine(410,310, 1, 0xFFFF);
	// (410,310)篮 Player狼 拳搁 惑狼 惯困摹..捞促..
	// (g_x,g_y)绰 Mouse谅钎甫 扁霖栏肺....

	//------------------------------------------------------
	// 1. ImageObject啊 捧疙捞 登绰 巴牢啊?
	// 2. Viewpoint啊 Player困摹焊促 酒贰率俊 乐绰啊?
	// 3. Sprite啊 Player甫 啊府绰啊?
	//------------------------------------------------------
	// 1,2,3捞 葛滴 父练茄 版快.. 馆捧疙 贸府茄促.
	//------------------------------------------------------
	bool bTrans;

	// player谅钎
	//POINT m_FilterPosition;// = MapToPixel(g_pPlayer->GetX(), g_pPlayer->GetY());
	//m_FilterPosition.x -= m_FirstZonePixel.x-m_PlusPoint.x;
	//m_FilterPosition.y -= m_FirstZonePixel.y-m_PlusPoint.y;

	// 弊覆狼 谅钎甫 泅犁 拳搁狼 谅钎俊 嘎眠扁								
	//m_FilterPosition.x = g_pPlayer->GetPixelX() - m_FirstZonePixel.x - filter.GetWidth();
	//m_FilterPosition.y = g_pPlayer->GetPixelY() - m_FirstZonePixel.y - filter.GetHeight();
	//m_FilterPosition.x = 310;	// 410 - 100;
	//m_FilterPosition.y = 116;	// 204 - 100;
	
	//------------------------------------------------------
	// 傈眉利栏肺 绢滴款 蔼阑 瘤沥茄促.
	// 窜, ImageObject狼 谅钎(Zone俊辑狼 Sector谅钎)啊 
	// 灌篮 Tile捞搁(Light啊 0捞 酒囱 版快) DarkBits=0栏肺 茄促.
	//------------------------------------------------------
	//BYTE DarkBits;
	
	//------------------------------------------------------
	// [ TEST CODE ]
	//------------------------------------------------------
		// 捞芭绰 鞘夸绝绰 code捞促.
		// MapEditor俊辑 肋 父甸绢林搁 等促.
	/*
	if (pImageObject->GetX()!=SECTORPOSITION_NULL && pImageObject->GetY()!=SECTORPOSITION_NULL
		&&
		m_pZone->GetSector(pImageObject->GetX(), pImageObject->GetY()).GetLight()!=0)
	{
		DarkBits = 0;
	}
	else
	{
		DarkBits = m_DarkBits;
	}
	
	// 3D 啊加捞 登绰 版快.. 公炼扒 盔贰 祸彬阑 弊措肺 免仿茄促.
	if (CDirect3D::IsHAL())
	{
		DarkBits = 0;
	}
	*/

	//----------------------------------------------------				
	// 扒拱篮 公炼扒 馆捧疙
	//----------------------------------------------------				
	BOOL bBlendingShadow = 1;//g_pUserOption->BlendingShadow;
	
	//------------------------------------------------------
	//
	//  ShadowObject 免仿
	//
	//------------------------------------------------------
	if (pImageObject->GetObjectType()==MObject::TYPE_SHADOWOBJECT)
	{
		int spriteID = pImageObject->GetSpriteID();

		if (spriteID != SPRITEID_NULL)	
		{			
//			if (CDirect3D::IsHAL())
//			{			
//				if (bBlendingShadow)
//				{
//					DRAW_TEXTURE_SPRITE_LOCKED(pPoint->x, pPoint->y, 
//												spriteID, m_pImageObjectShadowManager)
//
//
//					/*
//					#ifdef OUTPUT_DEBUG
//						if (g_pDXInput->KeyDown(DIK_S) && 
//							(g_pDXInput->KeyDown(DIK_RCONTROL)))
//						{
//							char str[128];
//							sprintf(str, "iid=%d, view=%d", (int)pImageObject->GetImageObjectID(), (int)pImageObject->GetViewpoint());
//							m_pSurface->GDI_Text(pPoint->x, pPoint->y, str, 0xFFFF);
//						}
//					#endif
//					*/
//				}
//				else
//				{
//					m_pSurface->BltShadowSprite( pPoint, &(m_pImageObjectShadowManager->GetSprite(spriteID)) );
//
//					#ifdef OUTPUT_DEBUG
//						if (g_pDXInput->KeyDown(DIK_S) && 
//							(g_pDXInput->KeyDown(DIK_RCONTROL)))
//						{
//							m_pSurface->Unlock();
//					
//							char str[128];
//							sprintf(str, "iid=%d, view=%d", (int)pImageObject->GetImageObjectID(), (int)pImageObject->GetViewpoint());
//							m_pSurface->GDI_Text(pPoint->x, pPoint->y, str, 0xFFFF);
//							
//							m_pSurface->Lock();
//						}
//					#endif
//				}
//			}
//			else
			{	
				if (bBlendingShadow)
				{
					m_pSurface->BltShadowSpriteDarkness( pPoint, &m_ImageObjectSSPK[spriteID], 1 );
				}
				else
				{
					m_pSurface->BltShadowSprite( pPoint, &m_ImageObjectSSPK[spriteID] );
				}

				#ifdef OUTPUT_DEBUG
					if (g_pDXInput->KeyDown(DIK_S) && 
						(g_pDXInput->KeyDown(DIK_RCONTROL)))
					{
						m_pSurface->Unlock();
				
						char str[128];
						sprintf(str, "iid=%d, view=%d", (int)pImageObject->GetImageObjectID(), (int)pImageObject->GetViewpoint());
						m_pSurface->GDI_Text(pPoint->x, pPoint->y, str, 0xFFFF);
						
						m_pSurface->Lock();
					}
				#endif
			}	
		}
	}
	//------------------------------------------------------
	//
	//          ShadowAnimationObject 免仿
	//
	//------------------------------------------------------
	else if (pImageObject->GetObjectType()==MObject::TYPE_SHADOWANIMATIONOBJECT)
	{
	}
	//------------------------------------------------------
	//
	//          InteractionObject 免仿
	//
	//------------------------------------------------------
//	else if (pImageObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
//	{
//		MInteractionObject* pInteractionObject = (MInteractionObject*)pImageObject;
//
//		TYPE_SPRITEID sprite;
//	
//		switch (pInteractionObject->GetBltType())
//		{
//			//--------------------------------
//			// Blt Normal
//			//--------------------------------
//			case BLT_NORMAL :
//			{
//				CFrame& Frame = m_InteractionObjectFPK[ pInteractionObject->GetFrameID() ][ pInteractionObject->GetFrame() ];
//				sprite = Frame.GetSpriteID();
//				
//				/*
//				#ifdef	OUTPUT_DEBUG
//					sprintf(g_pDebugMessage->GetCurrent(), 
//							"[%d] FrameID=%d, Frame=%d, sprite=%d, xy=(%d,%d)", 
//							pInteractionObject->GetID(), pInteractionObject->GetFrameID(), pInteractionObject->GetFrame(), sprite, pInteractionObject->GetX(), pInteractionObject->GetY()
//					);								
//					g_pDebugMessage->Next();					
//				#endif
//				*/
//
//				// 谅钎 焊沥
//				pPoint->x += Frame.GetCX();
//				pPoint->y += Frame.GetCY();
//
//				CSprite* pSprite = &m_InteractionObjectSPK[ sprite ];		
//				
//				//---------------------------------------- 		
//				// InteractionObject狼 急琶 荤阿屈 康开 汲沥
//				//---------------------------------------- 	
//				RECT rect;
//				rect.left	= pPoint->x;
//				rect.top	= pPoint->y;
//				rect.right	= rect.left + pSprite->GetWidth();
//				rect.bottom = rect.top + pSprite->GetHeight();
//				pInteractionObject->SetScreenRect( &rect );
//
//
//				// 馆捧疙 check				
//				bTrans = pImageObject->IsTrans();
//
//				//--------------------------------
//				// 急琶等 InteractionObject老 版快...
//				//--------------------------------
//				if (m_SelectInteractionObjectID==pInteractionObject->GetID())
//				{			
//					//if (m_ImageObjectSPK[ sprite ].IsInit())
//					{
//						m_SOMOutlineColor = m_ColorOutlineInteractionObject;						
//
//						m_SOM.Add( pPoint->x, pPoint->y, pSprite );
//						
//						m_SOM.Generate();
//
//						//if (DarkBits==0)
//						//{
//							m_pSurface->BltSpriteOutline( &m_SOM, m_SOMOutlineColor );
//						//}
//						//else
//						//{
//							//m_pSurface->BltSpriteOutlineDarkness( &m_SOM, m_SOMOutlineColor, DarkBits );
//						//}				
//					}
//				}
//				//--------------------------------
//				// 捧疙 贸府
//				//--------------------------------
//				else if (bTrans)
//				{
//					// 馆捧疙
//					//m_pSurface->BltSpriteHalf(pPoint, &m_ImageObjectSPK[ sprite ]);
//					// 弊覆俊 嘎冕 filter 谅钎 焊沥
//					CSprite::SetFilter(m_FilterPosition.x - pPoint->x,
//										m_FilterPosition.y - pPoint->y,
//										&m_ImageObjectFilter);
//
//					//if (m_ImageObjectSPK[ sprite ].IsIntersectFilter() 
//					//	|| DarkBits==0)
//					//{
//						m_pSurface->BltSpriteAlphaFilter(pPoint, pSprite);
//					//}
//					//else
//					//{
//					//	m_pSurface->BltSpriteAlphaFilterDarkness(pPoint, 
//														//&m_ImageObjectSPK[ sprite ], DarkBits);
//					//}					
//				}
//				//--------------------------------
//				// 弊成 免仿
//				//--------------------------------
//				else
//				{
//					//if (m_ImageObjectSPK[ sprite ].IsInit())
//					{
//						///*
//						// Player狼 矫具狼 农扁甫 馆康秦辑
//						// 矫具 救俊 乐绰 ImageObject甸父 焊捞霸 茄促.
//						//CSprite& spriteRef = m_ImageObjectSPK[ sprite ];
//
//						//int sight = g_pPlayer->GetMaxLightSight();
//						//int pX = sight * TILE_X;
//						//int pY = sight * TILE_Y;
//						//int sX = 410-(pX>>1) - pPoint->x;
//						//int sY = 270-(pY>>1) - pPoint->y;			
//
//						//if (DarkBits==0 //|| m_ImageObjectSPK[ pImageObject->GetSpriteID() ].IsIntersectFilter())
//							// 免仿 矫累困摹(s_X,s_Y)啊 Sprite狼 场痢焊促绰 利阑 版快
//						//	|| (sX < spriteRef.GetWidth() && sY < spriteRef.GetHeight()
//							// Filter狼 场痢捞 Sprite 霉痢 焊促 奴 版快
//						//	 &&	sX,pX > 0 && sY,pY > 0))
//						//{
//							m_pSurface->BltSprite(pPoint, pSprite);
//						//}
//						//else
//						//{
//						//	m_pSurface->BltSpriteDarkness(pPoint, &m_ImageObjectSPK[ sprite ], DarkBits);
//						//}
//					}
//				}
//			}
//			break;
//
//			//--------------------------------
//			// Blt Channel 
//			//--------------------------------
//			case BLT_EFFECT :	
//				/*
//				//--------------------------------
//				// 急琶等 InteractionObject老 版快...
//				//--------------------------------
//				if (m_SelectInteractionObjectID==pInteractionObject->GetID())
//				{
//					//DrawSpriteOutline(pPoint, &m_ImageObjectSPK[ sprite ], 0xFFFF);
//				}
//				else
//				{				
//					m_pSurface->BltAlphaSprite(pPoint, &m_EffectAlphaSPK[ sprite ]);
//				}
//				*/
//			break;
//		}
//
//		if (g_bFrameChanged)
//		{
//			pInteractionObject->NextFrame();
//		}
//	}	
	else
	{
		//------------------------------------------------------
		//
		//          ImageObject : Sprite 免仿
		//
		//------------------------------------------------------
		int spriteID = pImageObject->GetSpriteID();
		if (spriteID != SPRITEID_NULL)	
		{			
			//---------------------------------------
			// ID啊 sprite牢 ImageObject甫 Load茄促.
			//---------------------------------------
//			#ifdef	OUTPUT_DEBUG
//				char str[256];
//			#endif
//
//			if (m_ImageObjectSPK[ spriteID ].IsNotInit())
//			{
//				#ifdef	OUTPUT_DEBUG
//					sprintf(str, "[RunTimeLoading] ImageObject : sprite=%d. fp=%ld. (%d x %d)", spriteID, m_ImageObjectSPKI[spriteID], m_ImageObjectSPK[spriteID].GetWidth(), m_ImageObjectSPK[spriteID].GetHeight());
//				#endif				
//
//				m_ImageObjectSPKFile.seekg(m_ImageObjectSPKI[spriteID], ios::beg);				
//				//--------------------------------------------------
//				// Loading俊 己傍茄 版快
//				//--------------------------------------------------
//				if (m_ImageObjectSPK[ spriteID ].LoadFromFile( m_ImageObjectSPKFile ))
//				{
//					#ifdef	OUTPUT_DEBUG
//						sprintf(str, "%s ...OK (%d x %d)", str, m_ImageObjectSPK[spriteID].GetWidth(), m_ImageObjectSPK[spriteID].GetHeight());
//
//						DEBUG_ADD( str );
//					#endif
//				}
//				//--------------------------------------------------
//				// 角菩茄 版快 --> 捞固 Loading窍绊 乐绰 版快捞促.				
//				//--------------------------------------------------
//				/*
//				// 2001.8.20 林籍贸府
//				else
//				{
//					#ifdef	OUTPUT_DEBUG
//						strcat(str, " ...Fail & Wait Loading");
//	
//						DEBUG_ADD( str );
//					#endif
//
//					// file thread 鉴困甫 臭腮促.
//					//SetThreadPriority(g_hFileThread, THREAD_PRIORITY_HIGHEST);
//					MLoadingSPKWorkNode3* pNode = new MLoadingSPKWorkNode3(spriteID, m_ImageObjectSPKI[spriteID]);
//					pNode->SetSPK( &m_ImageObjectSPK, FILE_SPRITE_IMAGEOBJECT );
//					pNode->SetType( 1 );
//					g_pLoadingThread->SetPriority( THREAD_PRIORITY_HIGHEST );
//					g_pLoadingThread->AddFirst( pNode );
//
//					// Thread俊辑 Loading捞 场朝锭鳖瘤 扁促赴促.
//					//while (m_ImageObjectSPK[ spriteID ].IsNotInit());
//					while (1)
//					{
//						DEBUG_ADD_FORMAT( "Check Load id=%d", spriteID );
//
//						if (m_ImageObjectSPK[spriteID].IsInit())
//						{
//							DEBUG_ADD( "Is Init" );	
//						
//							break;
//						}
//						else
//						{
//							DEBUG_ADD( "Is Not Init" );
//						}
//					}
//					//while (!m_ImageObjectSPK[spriteID].LoadFromFile( m_ImageObjectSPKFile ));
//
//					// file thread 鉴困甫 撤冕促.
//					//SetThreadPriority(g_hFileThread, THREAD_PRIORITY_BELOW_NORMAL);
//					g_pLoadingThread->SetPriority( THREAD_PRIORITY_LOWEST );
//
//				}
//				*/
//			}

			bTrans = pImageObject->IsTrans()
						&& pImageObject->GetViewpoint() >= g_pPlayer->GetY()
						// 2001.9.5 眠啊
						&& pImageObject->IsWallTransPosition(g_pPlayer->GetX(), g_pPlayer->GetY());

						
						//&& m_ImageObjectSPK[ spriteID ].IsColorPixel(
						//	410 - pPoint->x,
						//	310 - pPoint->y);
		
			//--------------------------------
			// 捧疙 贸府
			//--------------------------------
			if (bTrans)
			{	
				///*
				//m_pSurface->BltSpriteHalf(pPoint, &m_ImageObjectSPK[ spriteID ]);
			

				// 弊覆俊 嘎冕 filter 谅钎 焊沥
				CSprite::SetFilter(m_FilterPosition.x - pPoint->x,
									m_FilterPosition.y - pPoint->y,
									&m_ImageObjectFilter);

				if (m_ImageObjectSPK[ spriteID ].IsIntersectFilter())
				{
					//if (DarkBits==0)
					//{
#ifdef __3D_IMAGE_OBJECT__
					if (CDirect3D::IsHAL())
					{
						m_pSurface->Unlock();

						// Texture 汲沥
						CSpriteSurface* pSurface = m_pImageObjectTextureManager->GetTexture(spriteID);

						if( pSurface != NULL )
						{
							CDirect3D::GetDevice()->SetTexture(0, pSurface->GetSurface());

							CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);					
							CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_ONE );
							CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );   
							
							// 谅钎 汲沥
							//						m_SpriteVertices[0].sx = pPoint->x;
							//						m_SpriteVertices[0].sy = pPoint->y;
							//						m_SpriteVertices[1].sx = pPoint->x,m_pImageObjectTextureManager->GetWidth(spriteID);
							//						m_SpriteVertices[1].sy = pPoint->y;
							//						m_SpriteVertices[2].sx = pPoint->x;
							//						m_SpriteVertices[2].sy = pPoint->y,m_pImageObjectTextureManager->GetHeight(spriteID);
							//						m_SpriteVertices[3].sx = m_SpriteVertices[1].sx;
							//						m_SpriteVertices[3].sy = m_SpriteVertices[2].sy;		
							
							CDirect3D::GetDevice()->BeginScene();						
							
							RECT rect;
							
							rect.left = pPoint->x;
							rect.top = pPoint->y;
							rect.right = rect.left + m_pImageObjectTextureManager->GetWidth( spriteID );
							rect.bottom = rect.top + m_pImageObjectTextureManager->GetHeight( spriteID );
							
							m_TextureEffect.DrawEffect2D( &rect );
							
							CDirect3D::GetDevice()->EndScene();
							CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);					
							CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
							CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
							m_pSurface->Lock();	
						}
					}					
#else
					#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
						if (g_pDXInput->KeyDown(DIK_A) && 
							(g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL)))
						{
							// CTRL+A穿福搁 鞠芭档 救 嘛绰促.
						}
						else
					#endif
						{
							m_pSurface->BltSpriteAlphaFilter(pPoint, 
														&m_ImageObjectSPK[ spriteID ]);							
						}						
#endif
					//}
					//else
					//{
					//	m_pSurface->BltSpriteAlphaFilterDarkness(pPoint, 
					//								&m_ImageObjectSPK[ spriteID ], DarkBits);
					//}	

					// Texture父甸绢辑 馆捧疙 窍扁... 
					/*
					if (CDirect3D::IsHAL())
					{
						m_pSurface->Unlock();

						// Texture 汲沥
						CSpriteSurface* pSurface = m_pImageObjectTextureManager->GetTexture(spriteID);
						CDirect3D::GetDevice()->SetTexture(0, pSurface->GetSurface());
				
						// 谅钎 汲沥
						m_SpriteVertices[0].sx = pPoint->x;
						m_SpriteVertices[0].sy = pPoint->y;
						m_SpriteVertices[1].sx = pPoint->x,m_pImageObjectTextureManager->GetWidth(spriteID);
						m_SpriteVertices[1].sy = pPoint->y;
						m_SpriteVertices[2].sx = pPoint->x;
						m_SpriteVertices[2].sy = pPoint->y,m_pImageObjectTextureManager->GetHeight(spriteID);
						m_SpriteVertices[3].sx = m_SpriteVertices[1].sx;
						m_SpriteVertices[3].sy = m_SpriteVertices[2].sy;		
						
						// 免仿
						CDirect3D::GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 
												D3DFVF_TLVERTEX, 
												m_SpriteVertices, 
												4, 0);

						//CDirect3D::GetDevice()->EndScene();
						m_pSurface->Lock();	
					}
					else
					{
						m_pSurface->BltSpriteAlphaFilter(pPoint, 
													&m_ImageObjectSPK[ spriteID ]);
					}
					*/
				}
				else
				{
					//if (DarkBits==0)
					//{
					#ifdef __3D_IMAGE_OBJECT__
					if (CDirect3D::IsHAL())
					{
						m_pSurface->Unlock();

						// Texture 汲沥
						CSpriteSurface* pSurface = m_pImageObjectTextureManager->GetTexture(spriteID);

						if( pSurface != NULL )
						{
							CDirect3D::GetDevice()->SetTexture(0, pSurface->GetSurface());
							
							// 谅钎 汲沥
							//						m_SpriteVertices[0].sx = pPoint->x;
							//						m_SpriteVertices[0].sy = pPoint->y;
							//						m_SpriteVertices[1].sx = pPoint->x,m_pImageObjectTextureManager->GetWidth(spriteID);
							//						m_SpriteVertices[1].sy = pPoint->y;
							//						m_SpriteVertices[2].sx = pPoint->x;
							//						m_SpriteVertices[2].sy = pPoint->y,m_pImageObjectTextureManager->GetHeight(spriteID);
							//						m_SpriteVertices[3].sx = m_SpriteVertices[1].sx;
							//						m_SpriteVertices[3].sy = m_SpriteVertices[2].sy;		
							
							CDirect3D::GetDevice()->BeginScene();						
							
							RECT rect;
							
							rect.left = pPoint->x;
							rect.top = pPoint->y;
							rect.right = rect.left + m_pImageObjectTextureManager->GetWidth( spriteID );
							rect.bottom = rect.top + m_pImageObjectTextureManager->GetHeight( spriteID );
							
							m_TextureEffect.DrawEffect2D( &rect );
							
							CDirect3D::GetDevice()->EndScene();
							m_pSurface->Lock();	
						}
					}					
#else
					if( pImageObject->IsHalf() )
						m_pSurface->BltSpriteHalf(pPoint, &m_ImageObjectSPK[ spriteID ]);
					else
						m_pSurface->BltSprite(pPoint, &m_ImageObjectSPK[ spriteID ]);
#endif
					//}
					//else 
					//{
						//m_pSurface->BltSpriteDarkness(pPoint, &m_ImageObjectSPK[ spriteID ], DarkBits);		
					//}
				}
				//*/

				//m_pSurface->BltSpriteHalf(pPoint, &m_ImageObjectSPK[ spriteID ]);
				//m_pSurface->BltSprite(pPoint, &m_ImageObjectOutlineSPK[ 0 ]);
			}
			//--------------------------------
			// 弊成 免仿
			//--------------------------------
			else
			{					
				//if (m_ImageObjectSPK[ spriteID ].IsInit())
				{
					///*
					// Player狼 矫具狼 农扁甫 馆康秦辑
					// 矫具 救俊 乐绰 ImageObject甸父 焊捞霸 茄促.
					//CSprite& spriteRef = m_ImageObjectSPK[ spriteID ];

					//int sight = g_pPlayer->GetMaxLightSight();
					//int pX = sight * TILE_X;
					//int pY = sight * TILE_Y;
					//int sX = 410-(pX>>1) - pPoint->x;
					//int sY = 270-(pY>>1) - pPoint->y;			

					//if (DarkBits==0 //|| m_ImageObjectSPK[ spriteID ].IsIntersectFilter())
						// 免仿 矫累困摹(s_X,s_Y)啊 Sprite狼 场痢焊促绰 利阑 版快
					//	|| (sX < spriteRef.GetWidth() && sY < spriteRef.GetHeight()
						// Filter狼 场痢捞 Sprite 霉痢 焊促 奴 版快
					//	 &&	sX,pX > 0 && sY,pY > 0))
						
					//{
#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
					if (g_pDXInput->KeyDown(DIK_A) && 
						(g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL)))
					{
						// CTRL+A穿福搁 鞠芭档 救 嘛绰促.
					}
					else
#endif
					{
						if( pImageObject->IsHalf() )
							m_pSurface->BltSpriteHalf(pPoint, &m_ImageObjectSPK[ spriteID ]);
						else
							m_pSurface->BltSprite(pPoint, &m_ImageObjectSPK[ spriteID ]);
						/*							
						static int gray = 0;
						static int g = 1;

						CSpriteSurface::s_Value1 = gray;

						CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_GRAY_SCALE_VARIOUS );
					
						m_pSurface->BltSpriteEffect(pPoint, &m_ImageObjectSPK[ spriteID ]);

						static DWORD lastFrame = g_CurrentFrame;

						if (g_CurrentFrame - lastFrame > 2)
						{
							gray += g;

							if (gray == 32 || gray == 0)
							{
								g = -g;
							}						

							lastFrame = g_CurrentFrame;
						}
						*/
					}

/*
#ifdef OUTPUT_DEBUG
	if (g_pDXInput->KeyDown(DIK_RCONTROL))
	{
		static int value = 0;
		static int step = 0;
		
		if (value==0)
		{
			m_pSurface->BltSprite(pPoint, &m_ImageObjectSPK[ spriteID ]);
		}
		else if (value==64)
		{
			CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_SIMPLE_OUTLINE );

			m_pSurface->BltSpriteEffect(pPoint, &m_ImageObjectSPK[ spriteID ]);
		}
		else
		{
			CSpriteSurface::s_Value1 = value;

			CSpriteSurface::SetEffect( CSpriteSurface::EFFECT_WIPE_OUT );
		
			m_pSurface->BltSpriteEffect(pPoint, &m_ImageObjectSPK[ spriteID ]);
		}

		if (g_pDXInput->KeyDown(DIK_RSHIFT))
		{
			if (value==0 || value==64)
			{
				if (step==0)	
				{
					if (value==0)
						step = 1;
					else
						step = -1;
				}
				else step *= -1;
			}
		}	
		else
		{
			value += step;

			if (value<0) value = 0;
			else if (value>64) value=64;
		}
	}
	else
#endif
	{
		m_pSurface->BltSprite(pPoint, &m_ImageObjectSPK[ spriteID ]);
	}
	*/

					//}
					//else 
					//{
						//m_pSurface->BltSpriteDarkness(pPoint, &m_ImageObjectSPK[ spriteID ], DarkBits);		
					//}
				}
			}

			#ifdef OUTPUT_DEBUG
				if (g_pDXInput->KeyDown(DIK_I) && 
					(g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL)))
				{
					char str[128];
					sprintf(str, "iid=%d, sid=%d, vp=%d", (int)pImageObject->GetImageObjectID(), (int)spriteID, (int)pImageObject->GetViewpoint());

					m_pSurface->Unlock();
					m_pSurface->GDI_Text(pPoint->x, pPoint->y, str, 0xFFFF);
					m_pSurface->Lock();
				}
			#endif
		}


		//------------------------------------------------------------
		//
		//     AnimationObject牢 版快浚 Frame档 免仿秦拎具 茄促.
		//
		//------------------------------------------------------------
		if (pImageObject->IsAnimation())
		{
			MAnimationObject* pAnimationObject = (MAnimationObject*)pImageObject;

			TYPE_SPRITEID sprite;

			POINT pointTemp = *pPoint;

			/*******************弊覆磊 免仿 矫累******************/ 
			if (pAnimationObject->GetFrameID() < m_ImageObjectShadowFPK.GetSize()
				&& pAnimationObject->GetFrame() < m_ImageObjectShadowFPK[pAnimationObject->GetFrameID()].GetSize())
			{
				CFrame &FrameShadow = m_ImageObjectShadowFPK[ pAnimationObject->GetFrameID() ][ pAnimationObject->GetFrame() ];
				sprite = FrameShadow.GetSpriteID();

				if (sprite != SPRITEID_NULL)	
				{			
					pointTemp.x += (FrameShadow.GetCX());
					pointTemp.y += (FrameShadow.GetCY());
					if (bBlendingShadow)
					{
						m_pSurface->BltShadowSpriteDarkness( &pointTemp, &m_ImageObjectSSPK[sprite], 1 );
					}
					else
					{
						m_pSurface->BltShadowSprite( &pointTemp, &m_ImageObjectSSPK[sprite] );
					}
				}
			}
			/*******************弊覆磊 免仿 场******************/ 

			switch (pAnimationObject->GetBltType())
			{
				//--------------------------------
				// Blt Normal
				//--------------------------------
				case BLT_NORMAL :
				{
					int currentFrame = g_CurrentFrame % m_ImageObjectFPK[ pAnimationObject->GetFrameID() ].GetSize();
					
					CFrame &Frame = m_ImageObjectFPK[ pAnimationObject->GetFrameID() ][ currentFrame ];
					sprite = Frame.GetSpriteID();
					if(sprite >= m_ImageObjectSPK.GetSize())
						break;
					//---------------------------------------
					// ID啊 sprite牢 ImageObject甫 Load茄促.
					//---------------------------------------
//					if (m_ImageObjectSPK[ sprite ].IsNotInit())
//					{
//						#ifdef	OUTPUT_DEBUG
//							if (g_pDebugMessage!=NULL)
//								sprintf(g_pDebugMessage->GetCurrent(), "[RunTimeLoading] AnimationObject : sprite=%d", sprite);
//						#endif
//
//						m_ImageObjectSPKFile.seekg(m_ImageObjectSPKI[sprite], ios::beg);						
//						
//						//--------------------------------------------------
//						// Loading俊 己傍茄 版快
//						//--------------------------------------------------
//						if (m_ImageObjectSPK[ sprite ].LoadFromFile( m_ImageObjectSPKFile ))
//						{
//							#ifdef	OUTPUT_DEBUG
//								if (g_pDebugMessage!=NULL)
//								{
//									sprintf(g_pDebugMessage->GetCurrent(), "%s ...OK", g_pDebugMessage->GetCurrent());
//									g_pDebugMessage->Next();
//								}
//							#endif
//						}
//						//--------------------------------------------------
//						// 角菩茄 版快 --> 捞固 Loading窍绊 乐绰 版快捞促.				
//						//--------------------------------------------------
//						
////						// 2001.8.20 林籍贸府
////						else
////						{
////							#ifdef	OUTPUT_DEBUG
////								if (g_pDebugMessage!=NULL)
////								{
////									sprintf(g_pDebugMessage->GetCurrent(), "%s ...Fail & Wait Loading", g_pDebugMessage->GetCurrent());
////									g_pDebugMessage->Next();
////								}
////							#endif
////
////							// file thread 鉴困甫 臭腮促.
////							//SetThreadPriority(g_hFileThread, THREAD_PRIORITY_HIGHEST);
////							MLoadingSPKWorkNode3* pNode = new MLoadingSPKWorkNode3(sprite, m_ImageObjectSPKI[sprite]);
////							pNode->SetSPK( &m_ImageObjectSPK, FILE_SPRITE_IMAGEOBJECT );
////							pNode->SetType( 1 );
////							g_pLoadingThread->SetPriority( THREAD_PRIORITY_HIGHEST );
////							g_pLoadingThread->AddFirst( pNode );
////
////							// Thread俊辑 Loading捞 场朝锭鳖瘤 扁促赴促.
////							//while (m_ImageObjectSPK[sprite].IsNotInit());
////							while (1)
////							{
////								DEBUG_ADD_FORMAT( "Check Load id=%d", sprite );
////
////								if (m_ImageObjectSPK[sprite].IsInit())
////								{
////									DEBUG_ADD( "Is Init" );
////								
////									break;
////								}
////								else
////								{
////									DEBUG_ADD( "Is Not Init" );
////								}
////							}
////							//while (!m_ImageObjectSPK[ sprite ].LoadFromFile( m_ImageObjectSPKFile ));
////
////							// file thread 鉴困甫 撤冕促.
////							//SetThreadPriority(g_hFileThread, THREAD_PRIORITY_BELOW_NORMAL);
////							g_pLoadingThread->SetPriority( THREAD_PRIORITY_LOWEST );
////
////						}
//						
//					}
					// 谅钎 焊沥
					pPoint->x += Frame.GetCX();
					pPoint->y += Frame.GetCY();

					// spriteID啊 绝绰 版快绰 馆捧疙 促矫 check				
					if (spriteID==SPRITEID_NULL)
					{
						bTrans = true;
					//	bTrans = pImageObject->IsTrans();
								//&& pImageObject->GetViewpoint() >= g_pPlayer->GetY();
								//&& m_ImageObjectSPK[ sprite ].IsColorPixel(
								//	410 - pPoint->x,
								//	310 - pPoint->y);
					}				

					//--------------------------------
					// 捧疙 贸府
					//--------------------------------
					if (bTrans)
					{
						// 馆捧疙
						//m_pSurface->BltSpriteHalf(pPoint, &m_ImageObjectSPK[ sprite ]);
						// 弊覆俊 嘎冕 filter 谅钎 焊沥
						CSprite::SetFilter(m_FilterPosition.x - pPoint->x,
											m_FilterPosition.y - pPoint->y,
											&m_ImageObjectFilter);

						//if (m_ImageObjectSPK[ sprite ].IsIntersectFilter()
						//	|| DarkBits==0)
						//{
							m_pSurface->BltSpriteAlphaFilter(pPoint, 
															&m_ImageObjectSPK[ sprite ]);
						//}
						//else
						//{
							//m_pSurface->BltSpriteAlphaFilterDarkness(pPoint, 
															//&m_ImageObjectSPK[ sprite ], DarkBits);
						//}
						
					}
					//--------------------------------
					// 弊成 免仿
					//--------------------------------
					else
					{
						///*
						// Player狼 矫具狼 农扁甫 馆康秦辑
						// 矫具 救俊 乐绰 ImageObject甸父 焊捞霸 茄促.
						//if (m_ImageObjectSPK[ sprite ].IsInit())
						{
							//CSprite& spriteRef = m_ImageObjectSPK[ sprite ];

							//int sight = g_pPlayer->GetMaxLightSight();
							//int pX = sight * TILE_X;
							//int pY = sight * TILE_Y;
							//int sX = 410-(pX>>1) - pPoint->x;
							//int sY = 270-(pY>>1) - pPoint->y;			

							//if (DarkBits==0 //|| m_ImageObjectSPK[ spriteID ].IsIntersectFilter())
								// 免仿 矫累困摹(s_X,s_Y)啊 Sprite狼 场痢焊促绰 利阑 版快
							//	|| (sX < spriteRef.GetWidth() && sY < spriteRef.GetHeight()
								// Filter狼 场痢捞 Sprite 霉痢 焊促 奴 版快
							//	 &&	sX,pX > 0 && sY,pY > 0))
							//{
							//if( pImageObject->IsHalf() )
							//	m_pSurface->BltSpriteHalf(pPoint, &m_ImageObjectSPK[ spriteID ]);
							//else
								m_pSurface->BltSprite(pPoint, &m_ImageObjectSPK[ spriteID ]);
							//}
							//else
							//{
								//m_pSurface->BltSpriteDarkness(pPoint, &m_ImageObjectSPK[ sprite ], DarkBits);
							//}
						}
					}
				}
				break;

				//--------------------------------
				// Blt Channel 
				//--------------------------------
				case BLT_EFFECT :
				{
					int fid = pAnimationObject->GetFrameID();
					if( fid < m_EffectAlphaFPK.GetSize() )
					{
						CEffectFrame &Frame = m_EffectAlphaFPK[ fid ][ 0 ][ pAnimationObject->GetFrame() ];

						sprite = Frame.GetSpriteID();
						
						// 谅钎 焊沥
						pPoint->x += Frame.GetCX();
						pPoint->y += Frame.GetCY();
						
						DRAW_ALPHASPRITEPAL(pPoint, sprite, m_EffectAlphaSPK, m_EffectAlphaPPK[fid])//, m_EffectAlphaSPKI, m_EffectAlphaSPKFile)					
								
						//-------------------------------------------------------
						// H/W 啊加捞 登绰 版快捞搁...
						//-------------------------------------------------------
						if (CDirect3D::IsHAL())
						{
//							#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
//							CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, gtx_op );
//							CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  gtx_src );
//							CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, gtx_dest );	
//							#endif
//							
//							DRAW_TEXTURE_SPRITEPAL_LOCKED(pPoint->x, pPoint->y, 
//								sprite, m_pAlphaEffectTextureManager, fid )//m_EffectAlphaPPK[fid])
//								
//							#ifdef OUTPUT_DEBUG
//								// 盔贰措肺
//							CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);					
//							CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//							CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
//							#endif
							
							
							//------------------------------------------------
							// LightFilter甫 眠啊茄促.
							//------------------------------------------------
							AddLightFilter3D( pPoint->x + g_TILE_X_HALF, 
								pPoint->y + g_TILE_Y_HALF, 
								Frame.GetLight(),			// 蝴狼 灌扁
								false);		// false = screen谅钎
							
						}
						//-------------------------------------------------------
						// 2D 免仿
						//-------------------------------------------------------
						else
						{
//							DRAW_ALPHASPRITEPAL(pPoint, sprite, m_EffectAlphaSPK, m_EffectAlphaPPK[fid])//, m_EffectAlphaSPKI, m_EffectAlphaSPKFile)					
								
								AddLightFilter2D( pPoint->x + g_TILE_X_HALF, 
								pPoint->y + g_TILE_Y_HALF, 
								Frame.GetLight(),			// 蝴狼 灌扁
								false);		// false = screen谅钎
						}
					}
					if (g_bFrameChanged)
					{
						pAnimationObject->NextFrame();
					}

				}
				break;

				//--------------------------------
				// Blt Channel 
				//--------------------------------
				case BLT_SCREEN :
				{
					int fid = pAnimationObject->GetFrameID();
					if(fid < m_EffectScreenFPK.GetSize())
					{
						CEffectFrame &Frame = m_EffectScreenFPK[fid][0][pAnimationObject->GetFrame()];					
						sprite = Frame.GetSpriteID();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetSpriteID();
						
						if (sprite!=SPRITEID_NULL)
						{
							// Frame 谅钎 焊沥
							pPoint->x += Frame.GetCX();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetCX();
							pPoint->y += Frame.GetCY();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetCY();
							
							DRAW_NORMALSPRITEPAL_EFFECT(pPoint, 
								sprite, 
								m_EffectScreenSPK, 
//									m_EffectScreenSPKI, 
//									m_EffectScreenSPKFile,
								m_EffectScreenPPK[fid],
								CSpriteSurface::EFFECT_SCREEN)
							//-------------------------------------------------------
							// H/W 啊加捞 登绰 版快捞搁...
							//-------------------------------------------------------
							if (CDirect3D::IsHAL())
							{
								// screen test
//								CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_ONE );
//								CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );									
//								
//								#ifdef OUTPUT_DEBUG
//								CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  gtx_src );
//								CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, gtx_dest );									
//								#endif
//								
//								DRAW_TEXTURE_SPRITEPAL_LOCKED(pPoint->x, pPoint->y, 
//									sprite, m_pScreenEffectTextureManager, fid ) //m_EffectScreenPPK[fid])
//									
//									// 盔贰措肺
//									CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//								CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
								
								//------------------------------------------------
								// LightFilter甫 眠啊茄促.
								//------------------------------------------------
								AddLightFilter3D( pPoint->x + g_TILE_X_HALF, 
									pPoint->y + g_TILE_Y_HALF, 
									Frame.GetLight(),			// 蝴狼 灌扁
									false);		// false = screen谅钎
								
							}
							//-------------------------------------------------------
							// H/W 啊加捞 救登绰 版快捞搁...
							//-------------------------------------------------------
							else
							{
//								DRAW_NORMALSPRITEPAL_EFFECT(pPoint, 
//									sprite, 
//									m_EffectScreenSPK, 
////									m_EffectScreenSPKI, 
////									m_EffectScreenSPKFile,
//									m_EffectScreenPPK[fid],
//									CSpriteSurface::EFFECT_SCREEN)
									
									AddLightFilter2D( pPoint->x + g_TILE_X_HALF, 
									pPoint->y + g_TILE_Y_HALF, 
									Frame.GetLight(),			// 蝴狼 灌扁
									false);		// false = screen谅钎
							}
						}
						if (g_bFrameChanged)
						{
							pAnimationObject->NextFrame();
						}
					}
					

				}
				break;
			}

			//---------------------------------------
			// Test Code : Next Frame
			//---------------------------------------
			//if (rand()%5) 
//			if (g_bFrameChanged)
//			{
//				pAnimationObject->NextFrame();
//			}
		}

	}

	/*
	// ImageObject ID 免仿
	char str[80];
	sprintf(str, "%d", pImageObject->GetID());
	gC_font.PrintStringNoConvert(&m_SurfaceInfo, 
								str, 
								pPoint->x, 
								pPoint->y, 
								0xFFFFFF);
	*/

	//#ifdef OUTPUT_DEBUG
		//	DEBUG_ADD("[TempDebug] After Draw ImageObject");
	//#endif

	__END_PROFILE("DrawImageObject")

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "End DrawImageObject" );
	#endif

	
}

//----------------------------------------------------------------
// SurfaceLock For AlphaEffect
//----------------------------------------------------------------
// 3D捞绊 lock蹬 乐栏搁 Unlock栏肺...
// 3D酒聪绊 lock救 蹬 乐栏搁 lock栏肺..
//
// return蔼篮 盔贰 Lock惑怕
//----------------------------------------------------------------
/*
BOOL
MTopView::SurfaceLockForAlphaEffect()
{
	BOOL bLock = m_pSurface->IsLock();

	if (CDirect3D::IsHAL())
	{
		if (bLock)
		{
			// 3D捞绊 lock蹬 乐栏搁 Unlock栏肺...
			m_pSurface->Unlock();
		}
	}
	else
	{	
		if (!bLock)
		{
			// 3D酒聪绊 lock救 蹬 乐栏搁 lock栏肺..
			m_pSurface->Lock();
		}
	}

	return bLock;
}
*/

//----------------------------------------------------------------
// SurfaceLock
//----------------------------------------------------------------
// return蔼篮 盔贰狼 lock惑怕
//----------------------------------------------------------------
/*
BOOL
MTopView::SurfaceLock()
{
	BOOL bLock = m_pSurface->IsLock();

	if (!bLock)
	{
		m_pSurface->Lock();
	}
	
	return bLock;
}
*/

//----------------------------------------------------------------
// SurfaceLock Restore
//----------------------------------------------------------------
// 盔贰狼 Lock惑怕肺 倒妨霖促.
//----------------------------------------------------------------
/*
void
MTopView::SurfaceLockRestore( BOOL bOldLock )
{
	if (bOldLock)
	{
		if (!m_pSurface->IsLock())
		{
			// 盔贰 Lock捞菌绰单 Lock捞 酒聪搁
			m_pSurface->Lock();
		}
	}
	else
	{
		if (m_pSurface->IsLock())
		{
			// 盔贰 Lock捞 酒聪菌绰单 Lock捞搁..
			m_pSurface->Unlock();
		}
	}
}
*/

//----------------------------------------------------------------
// Effect 免仿
//----------------------------------------------------------------
void	
MTopView::DrawEffect(POINT* pPoint, EFFECT_LIST::const_iterator iEffect, BYTE size)
{	
	if(g_pEventManager->GetEventByFlag(EVENTFLAG_NOT_DRAW_EFFECT))
		return;

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "Start DrawEffect" );
	#endif

	
	POINT point;

 	for (int i=0; i<size; i++)
	{
		MEffect* pEffect = *iEffect;

		if(pEffect->IsDelayFrame() == true)
		{
			iEffect++;
			continue;
		}
		//------------------------------------------------------------------
		// Tile窜困狼 免仿 谅钎
		//------------------------------------------------------------------
		//if (pEffect->GetEffectType()==MEffect::EFFECT_SECTOR)
		//{			
		//	point = *pPoint;
		//}
		//------------------------------------------------------------------
		// Screen 免仿 谅钎
		//------------------------------------------------------------------
		//else 
		if (pEffect->GetEffectType()==MEffect::EFFECT_SCREEN)
		{			
			MScreenEffect* pScreenEffect = (MScreenEffect*)pEffect;
			//MScreenEffect::SetScreenBasis( pPoint->x, pPoint->y );	// 茄锅父 秦林搁 登绰单.. 澜衬.
			point.x = pScreenEffect->GetScreenX();
			point.y = pScreenEffect->GetScreenY();
		}
		//------------------------------------------------------------------
		// Pixel窜困狼 免仿 谅钎
		//------------------------------------------------------------------
		else
		{
			//MMovingEffect* pMovingEffect = (MMovingEffect* const)pEffect;
			
			// Pixel 窜困狼 谅钎甫 拳搁狼 谅钎肺 官层霖促.
			point.x = pEffect->GetPixelX() - m_FirstZonePixel.x;
			point.y = pEffect->GetPixelY() - pEffect->GetPixelZ() - m_FirstZonePixel.y;
		}

		//------------------------------------------------------------------
		// 免仿
		//------------------------------------------------------------------
		DrawEffect( &point, pEffect, pEffect->IsSelectable() );		

		// 促澜 Effect
		iEffect++;
	}	
	
	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "End DrawEffect" );
	#endif

	
}


//----------------------------------------------------------------
// Draw GroundEffect
//----------------------------------------------------------------
// Tile俊 官肺 嘿绢 乐绰 GroundEffect甫 免仿
//----------------------------------------------------------------
void	
MTopView::DrawGroundEffect()
{	
	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "Start DrawGroundEffect" );
	#endif
	
	POINT point;
	POINT pointTemp;

	int size = g_pZone->GetGroundEffectCount();
	MZone::EFFECT_MAP::const_iterator iEffect = g_pZone->GetGroundEffects();

	for (int i=0; i<size; i++)
	{
		MEffect* pEffect = iEffect->second;
		
		if(pEffect->IsSkipDraw())
		{
			iEffect++;
			continue;
		}
		// Pixel 窜困狼 谅钎甫 拳搁狼 谅钎肺 官层霖促.
		// 吝缴痢
		point.x = pEffect->GetPixelX() - m_FirstZonePixel.x;
		point.y = pEffect->GetPixelY() - pEffect->GetPixelZ() - m_FirstZonePixel.y;			
		
		pointTemp = point;

		//------------------------------------------------------------------
		// 免仿
		//------------------------------------------------------------------
		DrawEffect( &pointTemp, pEffect, pEffect->IsSelectable());
		
		//------------------------------------------------------------------
		// 急琶登绰 Effect牢 版快
		//------------------------------------------------------------------
		if (pEffect->IsSelectable()
			&& m_SelectEffectID==pEffect->GetID())
		{
			MEffectTarget* pEffectTarget = pEffect->GetEffectTarget();

			if (pEffectTarget!=NULL)
			{
				//------------------------------------------------------------------
				// Portal牢 版快
				//------------------------------------------------------------------
				if (pEffectTarget->GetEffectTargetType()==MEffectTarget::EFFECT_TARGET_PORTAL
					&& g_pPlayer->IsVampire())
				{
					MPortalEffectTarget* pPortalEffectTarget = (MPortalEffectTarget*)pEffectTarget;

					char ownerName[20];
					strcpy(ownerName, pPortalEffectTarget->GetOwnerName());					
					int zoneID = pPortalEffectTarget->GetZoneID();


					int zoneX = pPortalEffectTarget->GetZoneX();
					int zoneY = pPortalEffectTarget->GetZoneY();

					if(//( !g_pUserInformation->IsNetmarble || g_mapPremiumZone.find(zoneID) == g_mapPremiumZone.end() )
						/*&&*/ g_pSystemAvailableManager->ZoneFiltering( zoneID ))
						gpC_mouse_pointer->SetCursorPortal( zoneID, zoneX, zoneY, ownerName );

				}
			}
		}
		// 促澜 Effect
		iEffect++;
	}	

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "End DrawGroundEffect" );
	#endif

	
}

//----------------------------------------------------------------
// Draw AttachEffect
//----------------------------------------------------------------
// Effect 免仿
// [窍靛内爹狼 柳荐] - -;;
// type - 0 - 公炼扒 免仿
//        1 - fore ground父 免仿
//        2 - back ground父 免仿
//----------------------------------------------------------------
// 老窜..
// PairFrame篮 AttachEffect俊辑父 免仿茄促.. 
// ActionFrame档 AttachEffect俊辑父 免仿茄促..
// NormalEffect俊绰 ActionFrame阑 公矫沁促. 酒流篮 嘛阑老档 绝绊 蓖满绊 秦辑 --;; 
//----------------------------------------------------------------
void	
MTopView::DrawAttachEffect(POINT* pPoint, ATTACHEFFECT_LIST::const_iterator iEffect, BYTE size, MCreature* pCreature, int type)
{	
	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "Start DrawAttachEffect" );
	#endif

	if(pCreature == NULL)	
		return;
	POINT pointTemp;
	for (int i=0; i<size; i++)	
	{
		MEffect* pEffect = *iEffect;
		
		//----------------------------------------------------------------
		// Sprite肺 钎泅窍绰 Effect牢 版快父 免仿茄促.
		// Attach啊 酒聪芭唱.. Attach捞歹扼档 Sprite肺 免仿窍绰 版快
		//----------------------------------------------------------------		
		if (pEffect->GetEffectType()!=MEffect::EFFECT_ATTACH ||
			pEffect->GetEffectType()==MEffect::EFFECT_ATTACH && ((MAttachEffect*)pEffect)->IsEffectSprite()
			
		
			)
		{
			int direction = pEffect->GetDirection();
			int frameID = pEffect->GetFrameID();
			int frame = pEffect->GetFrame();

			// 2004, 9, 14, sobeit add start - 牢胶喷 磐房老锭 捞棋飘 救焊咯淋
			int TempSecreenEffect = GET_EFFECTSPRITETYPE_SCREEN( frameID );

			if(pCreature->HasEffectStatus(EFFECTSTATUS_INSTALL_TURRET))
			{
				if(frameID != EFFECTSPRITETYPE_INSTALL_TURRET_FIRE1 &&
					frameID != EFFECTSPRITETYPE_INSTALL_TURRET_FIRE2 &&
					TempSecreenEffect != EFFECTSPRITETYPE_INSTALL_TURRET_BULLET)
				{
					iEffect++;
					continue;
				}
			}
			// 2004, 9, 14, sobeit add end - 牢胶喷 磐房老锭 捞棋飘 救焊咯淋
//			if( pEffect->GetBltType() == BLT_EFFECT &&
//				frameID >= EFFECTSPRITETYPE_PROTECTION_FROM_BLOOD_ING &&
//				frameID <= EFFECTSPRITETYPE_PROTECTION_FROM_CURSE_ING			
//				)
//			{
//				// 秦寸 捞棋飘绰 唱 肚绰 快府 颇萍盔父 焊咯具 茄促.
//				if( ! (
//					pCreature == g_pPlayer || pCreature->IsPlayerParty() )
//					)
//				{
//					iEffect++;					
//					continue;
//				}
//			}

			switch ((*iEffect)->GetBltType())
			{
				//--------------------------------------------------------
				// NORMAL
				//--------------------------------------------------------
				case BLT_NORMAL :
				{
					POINT pointTemp2 = *pPoint;

					AFFECT_ORBIT_EFFECT_POSITION( pEffect, pointTemp )

					DRAW_SPRITE_WITH_EFFECTFRAME( 
											&pointTemp2, 
											m_EffectNormalSPK, 
											m_EffectNormalFPK, 
											frameID, direction, frame )

					int est = GET_EFFECTSPRITETYPE_NORMAL( frameID );

					DRAW_SPRITE_WITH_EFFECTFRAME_PAIR( 
											est,
											&pointTemp2, 
											m_EffectNormalSPK, 
											m_EffectNormalFPK, 
											direction, frame )
				}
				break;

				//--------------------------------------------------------
				// EFFECT
				//--------------------------------------------------------
				case BLT_EFFECT :
				{
					int est = GET_EFFECTSPRITETYPE_EFFECT( frameID );

					// action俊 嘎绰 effectFrameID甫 榜扼林绰 芭寸.
					int aest = GET_ACTION_EFFECTSPRITETYPE(est);
					const bool bBack = (*g_pEffectSpriteTypeTable)[est].bPairFrameBack;
					int sest = GET_EFFECTSPRITETYPE_EFFECT( frameID );
					
					if (aest!=FRAMEID_NULL)
					{
						// FRAMEID_NULL牢吧 眉农茄促. min( , ) -_-;
						int action = pCreature->GetAction();
						if(action == ACTION_ADVANCEMENT_SLAYER_MAGIC_ATTACK || 
							action == ACTION_ADVANCEMENT_OUSTERS_MAGIC_ATTACK || 
							action == ACTION_ADVANCEMENT_MAGIC_ATTACK)
							action = ACTION_MAGIC;

						if( aest == ACTIONEFFECTSPRITETYPE_EXTREME_MALE ||
							aest == ACTIONEFFECTSPRITETYPE_EXTREME_FEMALE )
						{
							if( GET_ACTION_EFFECTSPRITETYPE_FRAMEID( aest, action ) == EFFECTSPRITETYPE_NULL ||
								pCreature->GetCreatureType() == CREATURETYPE_BAT ||
								pCreature->GetCreatureType() == CREATURETYPE_WOLF ||
								pCreature->GetCreatureType() == CREATURETYPE_WER_WOLF||
								pCreature->IsAdvancementClass())
							{
								iEffect++;
								continue;
							}
						}

						int actionFrameID = GET_ACTION_EFFECTSPRITETYPE_FRAMEID( aest, action );
						
						direction = pCreature->GetDirection();

						if (actionFrameID!=FRAMEID_NULL
							)//&& actionFrameID!=frameID)
						{
							frameID = actionFrameID;						
						
							int actionCount = ((action==ACTION_MOVE || action==ACTION_SLAYER_MOTOR_MOVE)? pCreature->GetMoveCount() : pCreature->GetActionCount());
							frame = max( 0, min( actionCount, pEffect->GetMaxFrame()-1 ) );

							sest = GET_EFFECTSPRITETYPE_EFFECT( actionFrameID );
						}
					}

					// 瘤陛篮 窍靛内爹 登绢 乐瘤父, 蝶肺 捞棋飘 胶橇扼捞飘鸥涝苞 悼累 橇饭烙埃狼 橇饭烙 悼扁拳
					// 包访 扁瓷捞 眠啊登搁, 窍靛内爹阑 阜阑 荐 乐阑 巴捞促. 
					// 悼扁拳 扁瓷捞 眠啊等促搁, 某腐磐俊 秦寸 捞棋飘啊 嘿绰 悼累阑 技泼且 荐 乐绢具 摆瘤..
					
					bool		bLarSlash = est >= EFFECTSPRITETYPE_LAR_SLASH_MALE_FAST && est <= EFFECTSPRITETYPE_LAR_SLASH_FEMALE_SLOW;
					bool		bRediance = est >= EFFECTSPRITETYPE_REDIANCE_MALE_FAST && est <= EFFECTSPRITETYPE_REDIANCE_FEMALE_NORMAL_ATTACK_SLOW;
					
					if( bRediance || bLarSlash )
					{						
						if( bRediance && (pCreature->GetAction() == ACTION_SLAYER_SWORD ||  pCreature->GetAction() == ACTION_SLAYER_SWORD_FAST || pCreature->GetAction() == ACTION_SLAYER_SWORD_SLOW) ||
							pCreature->GetAction() == ACTION_SLAYER_SWORD_2 || pCreature->GetAction() == ACTION_SLAYER_SWORD_2_SLOW || pCreature->GetAction() == ACTION_SLAYER_SWORD_2_FAST )
						{
							frame = max(0,min(pCreature->GetActionCount(),pEffect->GetMaxFrame()-1) );
						}
						else
						{
							// 促弗 悼累捞搁 肋 舅酒辑 逞扁磊.
							iEffect++;
							continue;
						}
					}
					
					EFFECTFRAME_ARRAY &EFA = m_EffectAlphaFPK[frameID][direction];
					
					frame = min( frame, EFA.GetSize()-1 );					

					CEffectFrame &Frame = EFA[frame];

					bool bFrameBackground = Frame.IsBackground();
					AFFECT_ORBIT_EFFECT_BACKGROUND(pEffect, bFrameBackground);
				
					// 攫力 免仿登绰扒瘤 眉农窃 秦霖促.
					if (HAS_PAIR_EFFECTSPRITETYPE(sest) && bBack == true )
					{
						EFFECTSPRITETYPETABLE_INFO::FRAMEID_LIST& idList = (*g_pEffectSpriteTypeTable)[sest].PairFrameIDList;

						EFFECTSPRITETYPETABLE_INFO::FRAMEID_LIST::iterator iID = idList.begin();

						while (iID != idList.end())
						{
							int pairFrameID = *iID;	//(*g_pEffectSpriteTypeTable)[sest].PairFrameID;
							EFFECTFRAME_ARRAY& EA = m_EffectAlphaFPK[pairFrameID][direction];
							
							int tempFrame = frame % EA.GetSize();

							CEffectFrame &Frame = EA[tempFrame];

							bool bFrameBackground = Frame.IsBackground();
							AFFECT_ORBIT_EFFECT_BACKGROUND(pEffect, bFrameBackground);

							// 攫力 免仿登绰扒瘤 眉农窃 秦霖促.
							if (type==0 
								|| type==1 && !bFrameBackground
								|| type==2 && bFrameBackground)
							{
								int sprite = Frame.GetSpriteID();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetSpriteID();

								if (sprite!=SPRITEID_NULL)
								{
									// Frame 谅钎 焊沥
									pointTemp = *pPoint;
									pointTemp.x += Frame.GetCX();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetCX();
									pointTemp.y += Frame.GetCY();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetCY();

									AFFECT_ORBIT_EFFECT_POSITION( pEffect, pointTemp )

									DRAW_ALPHASPRITEPAL(&pointTemp, 
														sprite, 
														m_EffectAlphaSPK, 
														m_EffectAlphaPPK[pairFrameID])
//															m_EffectAlphaSPKI, 
//															m_EffectAlphaSPKFile)
									//-------------------------------------------------------
									// H/W 啊加捞 登绰 版快捞搁...
									//-------------------------------------------------------
									if (CDirect3D::IsHAL())
									{
//										#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
//											CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, gtx_op );
//											CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  gtx_src );
//											CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, gtx_dest );	
//										#endif
//							
//										DRAW_TEXTURE_SPRITEPAL_LOCKED(pointTemp.x, pointTemp.y, 
//																	sprite, m_pAlphaEffectTextureManager, pairFrameID ) //m_EffectAlphaPPK[pairFrameID])
//
//										#ifdef OUTPUT_DEBUG
//											// 盔贰措肺
//											CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);					
//											CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//											CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
//										#endif
							

										//------------------------------------------------
										// LightFilter甫 眠啊茄促.
										//------------------------------------------------
										AddLightFilter3D( pPoint->x + g_TILE_X_HALF, 
														pPoint->y + g_TILE_Y_HALF, 
														Frame.GetLight(),			// 蝴狼 灌扁
														false);		// false = screen谅钎
									
									}
									//-------------------------------------------------------
									// H/W 啊加捞 救登绰 版快捞搁...
									//-------------------------------------------------------
									else
									{
//										DRAW_ALPHASPRITEPAL(&pointTemp, 
//															sprite, 
//															m_EffectAlphaSPK, 
//															m_EffectAlphaPPK[pairFrameID])
////															m_EffectAlphaSPKI, 
////															m_EffectAlphaSPKFile)

										AddLightFilter2D( pPoint->x + g_TILE_X_HALF, 
													pPoint->y + g_TILE_Y_HALF, 
													Frame.GetLight(),			// 蝴狼 灌扁
													false);		// false = screen谅钎
									}
								}
							}

							iID++;
						}						
					}

					if (type==0 
						|| type==1 && !bFrameBackground
						|| type==2 && bFrameBackground)
					{
						int sprite = Frame.GetSpriteID();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetSpriteID();

						if (sprite!=SPRITEID_NULL)
						{
							// Frame 谅钎 焊沥
							pointTemp = *pPoint;
							pointTemp.x += Frame.GetCX();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetCX();
							pointTemp.y += Frame.GetCY();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetCY();

							AFFECT_ORBIT_EFFECT_POSITION( pEffect, pointTemp )

							DRAW_ALPHASPRITEPAL(&pointTemp, 
												sprite, 
												m_EffectAlphaSPK, 
												m_EffectAlphaPPK[frameID])
//													m_EffectAlphaSPKI, 
//													m_EffectAlphaSPKFile)
							//-------------------------------------------------------
							// H/W 啊加捞 登绰 版快捞搁...
							//-------------------------------------------------------
							if (CDirect3D::IsHAL())
							{
//								#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
//									CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, gtx_op );
//									CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  gtx_src );
//									CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, gtx_dest );	
//								#endif
//					
//								DRAW_TEXTURE_SPRITEPAL_LOCKED(pointTemp.x, pointTemp.y, 
//															sprite, m_pAlphaEffectTextureManager, frameID ) //m_EffectAlphaPPK[frameID])
//
//								#ifdef OUTPUT_DEBUG
//									// 盔贰措肺
//									CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);					
//									CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//									CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
//								#endif
					

								//------------------------------------------------
								// LightFilter甫 眠啊茄促.
								//------------------------------------------------
								AddLightFilter3D( pPoint->x + g_TILE_X_HALF, 
												pPoint->y + g_TILE_Y_HALF, 
												Frame.GetLight(),			// 蝴狼 灌扁
												false);		// false = screen谅钎
							
							}
							//-------------------------------------------------------
							// H/W 啊加捞 救登绰 版快捞搁...
							//-------------------------------------------------------
							else
							{
								DRAW_ALPHASPRITEPAL(&pointTemp, 
													sprite, 
													m_EffectAlphaSPK, 
													m_EffectAlphaPPK[frameID])
//													m_EffectAlphaSPKI, 
//													m_EffectAlphaSPKFile)

								AddLightFilter2D( pPoint->x + g_TILE_X_HALF, 
											pPoint->y + g_TILE_Y_HALF, 
											Frame.GetLight(),			// 蝴狼 灌扁
											false);		// false = screen谅钎
							}
						}
					}

					//--------------------------------------------------------
					// Pair Frame 免仿
					//--------------------------------------------------------
//					sest = GET_EFFECTSPRITETYPE_EFFECT( frameID );

					if (HAS_PAIR_EFFECTSPRITETYPE(sest) && bBack == false )
					{
						EFFECTSPRITETYPETABLE_INFO::FRAMEID_LIST& idList = (*g_pEffectSpriteTypeTable)[sest].PairFrameIDList;

						EFFECTSPRITETYPETABLE_INFO::FRAMEID_LIST::iterator iID = idList.begin();

						while (iID != idList.end())
						{
							int pairFrameID = *iID;	//(*g_pEffectSpriteTypeTable)[sest].PairFrameID;
							EFFECTFRAME_ARRAY& EA = m_EffectAlphaFPK[pairFrameID][direction];
							
							int tempFrame = frame % EA.GetSize();

							CEffectFrame &Frame = EA[tempFrame];

							bool bFrameBackground = Frame.IsBackground();
							AFFECT_ORBIT_EFFECT_BACKGROUND(pEffect, bFrameBackground);

							// 攫力 免仿登绰扒瘤 眉农窃 秦霖促.
							if (type==0 
								|| type==1 && !bFrameBackground
								|| type==2 && bFrameBackground)
							{
								int sprite = Frame.GetSpriteID();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetSpriteID();

								if (sprite!=SPRITEID_NULL)
								{
									// Frame 谅钎 焊沥
									pointTemp = *pPoint;
									pointTemp.x += Frame.GetCX();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetCX();
									pointTemp.y += Frame.GetCY();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetCY();

									AFFECT_ORBIT_EFFECT_POSITION( pEffect, pointTemp )

									DRAW_ALPHASPRITEPAL(&pointTemp, 
														sprite, 
														m_EffectAlphaSPK, 
														m_EffectAlphaPPK[pairFrameID])
//															m_EffectAlphaSPKI, 
//															m_EffectAlphaSPKFile)
									//-------------------------------------------------------
									// H/W 啊加捞 登绰 版快捞搁...
									//-------------------------------------------------------
									if (CDirect3D::IsHAL())
									{
//										#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
//											CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, gtx_op );
//											CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  gtx_src );
//											CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, gtx_dest );	
//										#endif
//							
//										DRAW_TEXTURE_SPRITEPAL_LOCKED(pointTemp.x, pointTemp.y, 
//																	sprite, m_pAlphaEffectTextureManager, pairFrameID ) //m_EffectAlphaPPK[pairFrameID])
//
//										#ifdef OUTPUT_DEBUG
//											// 盔贰措肺
//											CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);					
//											CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//											CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
//										#endif
							

										//------------------------------------------------
										// LightFilter甫 眠啊茄促.
										//------------------------------------------------
										AddLightFilter3D( pPoint->x + g_TILE_X_HALF, 
														pPoint->y + g_TILE_Y_HALF, 
														Frame.GetLight(),			// 蝴狼 灌扁
														false);		// false = screen谅钎
									
									}
									//-------------------------------------------------------
									// H/W 啊加捞 救登绰 版快捞搁...
									//-------------------------------------------------------
									else
									{
//										DRAW_ALPHASPRITEPAL(&pointTemp, 
//															sprite, 
//															m_EffectAlphaSPK, 
//															m_EffectAlphaPPK[pairFrameID])
////															m_EffectAlphaSPKI, 
////															m_EffectAlphaSPKFile)

										AddLightFilter2D( pPoint->x + g_TILE_X_HALF, 
													pPoint->y + g_TILE_Y_HALF, 
													Frame.GetLight(),			// 蝴狼 灌扁
													false);		// false = screen谅钎
									}
								}
							}

							iID++;
						}						
					}
				}
				break;			

				//--------------------------------------------------------
				// SCREEN
				//--------------------------------------------------------
				case BLT_SCREEN :
				{
					int est = GET_EFFECTSPRITETYPE_SCREEN( frameID );
					bool bBack = (*g_pEffectSpriteTypeTable)[est].bPairFrameBack;

					// action俊 嘎绰 effectFrameID甫 榜扼林绰 芭寸.
					int aest = GET_ACTION_EFFECTSPRITETYPE(est);
					if (aest!=FRAMEID_NULL)
					{
						int action = pCreature->GetAction();
						if(action == ACTION_ADVANCEMENT_SLAYER_MAGIC_ATTACK || 
							action == ACTION_ADVANCEMENT_OUSTERS_MAGIC_ATTACK || 
							action == ACTION_ADVANCEMENT_MAGIC_ATTACK)
							action = ACTION_MAGIC;

						int actionFrameID = GET_ACTION_EFFECTSPRITETYPE_FRAMEID( aest, action );
						
						direction = pCreature->GetDirection();
						
						if (actionFrameID!=FRAMEID_NULL
							&& actionFrameID!=frameID)
						{
							frameID = actionFrameID;
							
							int actionCount = ((action==ACTION_MOVE || action==ACTION_SLAYER_MOTOR_MOVE)? pCreature->GetMoveCount() : pCreature->GetActionCount());
							frame = max( 0, min( actionCount, pEffect->GetMaxFrame()-1 ) );
						}
					}

					EFFECTFRAME_ARRAY &EFA = m_EffectScreenFPK[frameID][direction];
					frame = min( frame, EFA.GetSize()-1 );

					CEffectFrame &Frame = EFA[frame];

					bool bFrameBackground = Frame.IsBackground();
					AFFECT_ORBIT_EFFECT_BACKGROUND(pEffect, bFrameBackground);

					// 攫力 免仿登绰扒瘤 眉农窃 秦霖促.
					if (type==0 
						|| type==1 && !bFrameBackground
						|| type==2 && bFrameBackground)
					{
						int sprite = Frame.GetSpriteID();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetSpriteID();

						if (sprite!=SPRITEID_NULL)
						{
							// Frame 谅钎 焊沥
							pointTemp = *pPoint;
							pointTemp.x += Frame.GetCX();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetCX();
							pointTemp.y += Frame.GetCY();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetCY();
							
							AFFECT_ORBIT_EFFECT_POSITION( pEffect, pointTemp )							

							DRAW_NORMALSPRITEPAL_EFFECT(&pointTemp, 
												sprite, 
												m_EffectScreenSPK, 
												m_EffectScreenPPK[frameID],
//													m_EffectScreenSPKI, 
//													m_EffectScreenSPKFile,
												CSpriteSurface::EFFECT_SCREEN)
							//-------------------------------------------------------
							// H/W 啊加捞 登绰 版快捞搁...
							//-------------------------------------------------------
							if (CDirect3D::IsHAL())
							{
//								// screen test
//								CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_ONE );
//								CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );									
//
//							#ifdef OUTPUT_DEBUG
//								CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  gtx_src );
//								CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, gtx_dest );									
//							#endif
//					
//								DRAW_TEXTURE_SPRITEPAL_LOCKED(pointTemp.x, pointTemp.y, 
//															sprite, m_pScreenEffectTextureManager, frameID ) // m_EffectScreenPPK[frameID])
//
//								// 盔贰措肺
//								CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//								CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
					
								//------------------------------------------------
								// LightFilter甫 眠啊茄促.
								//------------------------------------------------
								AddLightFilter3D( pPoint->x + g_TILE_X_HALF, 
												pPoint->y + g_TILE_Y_HALF, 
												Frame.GetLight(),			// 蝴狼 灌扁
												false);		// false = screen谅钎
							
							}
							//-------------------------------------------------------
							// H/W 啊加捞 救登绰 版快捞搁...
							//-------------------------------------------------------
							else
							{
								DRAW_NORMALSPRITEPAL_EFFECT(&pointTemp, 
													sprite, 
													m_EffectScreenSPK, 
													m_EffectScreenPPK[frameID],
//													m_EffectScreenSPKI, 
//													m_EffectScreenSPKFile,
													CSpriteSurface::EFFECT_SCREEN)

								AddLightFilter2D( pPoint->x + g_TILE_X_HALF, 
											pPoint->y + g_TILE_Y_HALF, 
											Frame.GetLight(),			// 蝴狼 灌扁
											false);		// false = screen谅钎
							}
						}
					}

					//--------------------------------------------------------
					// Pair Frame 免仿
					//--------------------------------------------------------
					est = GET_EFFECTSPRITETYPE_SCREEN( frameID );

					if (HAS_PAIR_EFFECTSPRITETYPE(est))
					{
						EFFECTSPRITETYPETABLE_INFO::FRAMEID_LIST& idList = (*g_pEffectSpriteTypeTable)[est].PairFrameIDList;

						EFFECTSPRITETYPETABLE_INFO::FRAMEID_LIST::iterator iID = idList.begin();

						while (iID != idList.end())
						{
							int pairFrameID = *iID;//(*g_pEffectSpriteTypeTable)[*iID].PairFrameID;
							
							EFFECTFRAME_ARRAY& EA = m_EffectScreenFPK[pairFrameID][direction];
							
							int tempFrame = frame % EA.GetSize();

							CEffectFrame &Frame = EA[tempFrame];

							bool bFrameBackground = Frame.IsBackground();
							AFFECT_ORBIT_EFFECT_BACKGROUND(pEffect, bFrameBackground);

							// 攫力 免仿登绰扒瘤 眉农窃 秦霖促.
							if (type==0 
								|| type==1 && !bFrameBackground
								|| type==2 && bFrameBackground)
							{
								int sprite = Frame.GetSpriteID();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetSpriteID();

								if (sprite!=SPRITEID_NULL)
								{
									// Frame 谅钎 焊沥
									pointTemp = *pPoint;
									pointTemp.x += Frame.GetCX();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetCX();
									pointTemp.y += Frame.GetCY();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetCY();

									AFFECT_ORBIT_EFFECT_POSITION( pEffect, pointTemp )

									DRAW_NORMALSPRITEPAL_EFFECT(&pointTemp, 
														sprite, 
														m_EffectScreenSPK, 
														m_EffectScreenPPK[pairFrameID],
//															m_EffectScreenSPKI, 
//															m_EffectScreenSPKFile,
														CSpriteSurface::EFFECT_SCREEN)
									//-------------------------------------------------------
									// H/W 啊加捞 登绰 版快捞搁...
									//-------------------------------------------------------
									if (CDirect3D::IsHAL())
									{
//										// screen test
//										CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_ONE );
//										CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );									
//
//									#ifdef OUTPUT_DEBUG
//										CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  gtx_src );
//										CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, gtx_dest );									
//									#endif
//							
//										DRAW_TEXTURE_SPRITEPAL_LOCKED(pointTemp.x, pointTemp.y, 
//																	sprite, m_pScreenEffectTextureManager, pairFrameID )//m_EffectScreenPPK[pairFrameID])
//
//										// 盔贰措肺
//										CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//										CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
							
										//------------------------------------------------
										// LightFilter甫 眠啊茄促.
										//------------------------------------------------
										AddLightFilter3D( pPoint->x + g_TILE_X_HALF, 
														pPoint->y + g_TILE_Y_HALF, 
														Frame.GetLight(),			// 蝴狼 灌扁
														false);		// false = screen谅钎
									
									}
									//-------------------------------------------------------
									// H/W 啊加捞 救登绰 版快捞搁...
									//-------------------------------------------------------
									else
									{
										DRAW_NORMALSPRITEPAL_EFFECT(&pointTemp, 
															sprite, 
															m_EffectScreenSPK, 
															m_EffectScreenPPK[pairFrameID],
//															m_EffectScreenSPKI, 
//															m_EffectScreenSPKFile,
															CSpriteSurface::EFFECT_SCREEN)

										AddLightFilter2D( pPoint->x + g_TILE_X_HALF, 
													pPoint->y + g_TILE_Y_HALF, 
													Frame.GetLight(),			// 蝴狼 灌扁
													false);		// false = screen谅钎
									}
								}
							}

							iID++;
						}
					}
				}
				break;
			}
		}

		// 促澜 Effect
		iEffect++;
	}	

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "End DrawAttachEffect" );
	#endif

	
}

//----------------------------------------------------------------
// (x,y)俊 m_pZone狼 Minimap阑 弊赴促.
//----------------------------------------------------------------
// Test Function
//----------------------------------------------------------------
void
MTopView::DrawMinimap(int x, int y, BYTE scale)
{
	
	//------------------------------------------------
	// vampire牢 版快绰 免仿救茄促.
	//------------------------------------------------
	//if (g_pPlayer->IsVampire())
	//{
	//	return;
	//}

	//------------------------------------------------
	// 3D 啊加
	//------------------------------------------------
//	if (CDirect3D::IsHAL())
//	{
//		if (m_pMinimapTexture!=NULL)
//		{
//			if (CDirect3D::GetDevice()->BeginScene()!=D3D_OK)
//			{
//				return;
//			}			
//	
//			// Texture 汲沥
//			CDirect3D::GetDevice()->SetTexture(0, m_pMinimapTexture->GetSurface());
//
//			RECT rect = { x, 
//							y,
//							x + m_MinimapTextureWidth,
//							y + m_MinimapTextureHeight
//			};
//
//			m_TextureEffect.DrawEffect2D( &rect );		
//			
//			CDirect3D::GetDevice()->EndScene();		
//
//			CDirect3D::GetDevice()->SetTexture(0, NULL);
//
//
//			//------------------------------------------------
//			// Player困摹俊 痢嘛扁
//			//------------------------------------------------
////			if (!m_pSurface->Lock())
////				return;
////
////			POINT point = { x, y };
////
////			WORD*	pSurface	= (WORD*)m_pSurface->GetSurfacePointer();
////			long	lPitch		= m_pSurface->GetSurfacePitch();
////
////			// Player狼 Minimap惑狼 谅钎
////			point.x = x + g_pPlayer->GetX()*m_SectorToMinimapWidth;
////			point.y = y + g_pPlayer->GetY()*m_SectorToMinimapHeight;
////
////			// 痢 嘛扁
////			WORD*	pSurfaceTemp;	
////			pSurfaceTemp = (WORD*)((BYTE*)pSurface + lPitch*point.y + (point.x<<1));
////			*pSurfaceTemp++ = 0xFFFF;
////			*pSurfaceTemp = 0xFFFF;
////			pSurfaceTemp = (WORD*)((BYTE*)pSurface + lPitch*(point.y+1) + (point.x<<1));
////			*pSurfaceTemp++ = 0xFFFF;
////			*pSurfaceTemp = 0xFFFF;
////
////			m_pSurface->Unlock();
//		}
//	}
//	//------------------------------------------------
//	// 2D
//	//------------------------------------------------
//	else
//	{
//		if (m_pMinimapSPR!=NULL)
//		{
//			if (!m_pSurface->Lock())
//				return;
//
//			POINT point = { x, y };
//			m_pSurface->BltSpriteHalf(&point, m_pMinimapSPR);
//
//			//------------------------------------------------
//			// Player困摹俊 痢嘛扁
//			//------------------------------------------------	
////			WORD*	pSurface	= (WORD*)m_pSurface->GetSurfacePointer();
////			long	lPitch		= m_pSurface->GetSurfacePitch();
////
////			// Player狼 Minimap惑狼 谅钎
////			point.x = x + g_pPlayer->GetX()*m_SectorToMinimapWidth;
////			point.y = y + g_pPlayer->GetY()*m_SectorToMinimapHeight;
////
////			// 痢 嘛扁
////			WORD*	pSurfaceTemp;	
////			pSurfaceTemp = (WORD*)((BYTE*)pSurface + lPitch*point.y + (point.x<<1));
////			*pSurfaceTemp++ = 0xFFFF;
////			*pSurfaceTemp = 0xFFFF;
////			pSurfaceTemp = (WORD*)((BYTE*)pSurface + lPitch*(point.y+1) + (point.x<<1));
////			*pSurfaceTemp++ = 0xFFFF;
////			*pSurfaceTemp = 0xFFFF;
////
//			m_pSurface->Unlock();
//		}
//	}
	

	/*
	// 弊覆 免仿
	m_pSurface->Lock();

	POINT point = { x, y };

	// Minimap 免仿
	m_pSurface->BltSprite( &point, m_pMinimapSPR );

	int width = m_pMinimapSPR->GetWidth();
	int height = m_pMinimapSPR->GetHeight();

	WORD*	pSurface	= (WORD*)m_pSurface->GetSurfacePointer();
	long	lPitch		= m_pSurface->GetSurfacePitch();

	// Player狼 Minimap惑狼 谅钎
	point.x = x + g_pPlayer->GetX()*m_SectorToMinimapWidth;
	point.y = y + g_pPlayer->GetY()*m_SectorToMinimapHeight;

	// 痢 嘛扁
	WORD*	pSurfaceTemp;	
	pSurfaceTemp = (WORD*)((BYTE*)pSurface + lPitch*point.y + (point.x<<1));
	*pSurfaceTemp++ = 0xFFFF;
	*pSurfaceTemp = 0xFFFF;
	pSurfaceTemp = (WORD*)((BYTE*)pSurface + lPitch*(point.y,1) + (point.x<<1));
	*pSurfaceTemp++ = 0xFFFF;
	*pSurfaceTemp = 0xFFFF;

	m_pSurface->Unlock();
	*/

	/*
	WORD	*lpSurface, 
			*lpSurfaceTemp;
		
	WORD	pitch;

	WORD	color = 0;

	m_pSurface->LockW(lpSurface, pitch);

	lpSurface = (WORD*)((BYTE*)lpSurface + y*pitch + (x<<1));
	//lpSurface += (m_pZone->GetHeight()-1)<<scale;
	
	TYPE_SECTORPOSITION		pX = g_pPlayer->GetX(),
							pY = g_pPlayer->GetY();

	register int i,j;

	// Draw Minimap
	for (i=0; i<m_pZone->GetHeight(); i++)
	{
		lpSurfaceTemp = lpSurface;

		for (j=0; j<m_pZone->GetWidth(); j++)
		{			
			// player困摹
			if (pX==j && pY==i)
			{
				color = 0xFFFF;
			}			
			else
			{				
				// tile祸彬喊肺..
				
				//switch (m_pZone->GetSector(j,i).GetSpriteID())
				//{
				//	case 0 :	color = 0x07E0;	break;	// G
				//	case 1 :	color = 0xC2E5;	break;	// ?
				//	case 2 :	color = 0x001F;	break;	// B
				//	default :	color = 0x18E3;	break;	// ,black
				//}
				
				 const MSector &sector = m_pZone->GetSector(j,i);
			
				// 弊覆
				if (sector.IsExistImageObject() && sector.IsBlockAny())
				{
					color = 0x07E0;	// G
				}
				// Creature
				else if (sector.IsExistAnyCreature())
				{
					color = 0xF000;	// R
				}
				// Item
				else if (sector.IsExistItem())
				{
					color = 0x001F;	// B
				}				
				else 
				{
					color = 0;
					//lpSurfaceTemp = (WORD*)((BYTE*)lpSurfaceTemp + pitch*scale + scale*4);
					//continue;
				}
			}
			
			*lpSurfaceTemp = color;
			
			// 促澜 痢
			lpSurfaceTemp = (WORD*)((BYTE*)lpSurfaceTemp + scale*4);
		}

		// 促澜 临
		lpSurface = (WORD*)((BYTE*)lpSurface + pitch*scale);
	}

	m_pSurface->Unlock();
	*/
}

//----------------------------------------------------------------
// Draw Item Broken
//----------------------------------------------------------------
void	
MTopView::DrawItemBroken(int x, int y)
{	
//	return;
	/*
		MAX_GEAR俊 -10 茄扒 core zap+bloodbible 肮荐 父怒 猾芭..^^;
	*/
	// 何辑龙妨绰 item免仿
	MPlayerGear* pGear;
	int spriteID;
	int frameType;
	int toSomewhatBroken;
	int toAlmostBroken;
	
	//----------------------------------------------------------------
	// Slayer
	//----------------------------------------------------------------
	switch(g_pPlayer->GetRace())
	{
	case RACE_SLAYER:
	{
		if (g_pPlayer->IsMale())
		{
			frameType = 0;
			spriteID = SPRITEID_ITEM_BROKEN_SLAYER_MALE;		
		}
		else
		{
			frameType = 1;
			spriteID = SPRITEID_ITEM_BROKEN_SLAYER_FEMALE;		
		}

		pGear = g_pSlayerGear;
		toSomewhatBroken = MSlayerGear::MAX_GEAR_SLAYER + 2-12;		
		toAlmostBroken = toSomewhatBroken << 1;
	}
	break;

	case RACE_VAMPIRE:
	{
		if (g_pPlayer->IsMale())
		{
			frameType = 2;
			spriteID = SPRITEID_ITEM_BROKEN_VAMPIRE_MALE;		
		}
		else
		{
			frameType = 3;
			spriteID = SPRITEID_ITEM_BROKEN_VAMPIRE_FEMALE;		
		}

		pGear = g_pVampireGear;
		toSomewhatBroken = MVampireGear::MAX_GEAR_VAMPIRE-12;
		toAlmostBroken = toSomewhatBroken << 1;
	}
	break;

	case RACE_OUSTERS:
		{
			spriteID = SPRITEID_ITEM_BROKEN_OUSTERS;
			frameType = 4;
			pGear = g_pOustersGear;
			toSomewhatBroken = MOustersGear::MAX_GEAR_OUSTERS + 1-12;
			toAlmostBroken = toSomewhatBroken << 1;
		}
		break;
	}

	//----------------------------------------------------------------
	// 何辑龙妨绰 item捞 乐促搁 免仿茄促.
	//----------------------------------------------------------------
	if (pGear->HasBrokenItem() )
	{
		//---------------------------------------------
		// Lock
		//---------------------------------------------
		m_pSurface->Lock();

		//---------------------------------------------
		// 扁夯 伯单扁(-_-;) 免仿
		//---------------------------------------------
		POINT pointBasis = { x+15, y };
		POINT point = { pointBasis.x, pointBasis.y };

		m_pSurface->BltSprite( &point, &m_ItemBrokenSPK[spriteID] );
		

		int size = pGear->GetSize();

		//---------------------------------------------
		// Gear狼 葛电 slot俊 措秦辑 眉农
		//---------------------------------------------
		for (int i=0; i<size-4-6; i++)
		{			
			const MItem* pItem = pGear->GetItem( (BYTE)i );	
			
			//---------------------------------------------
			// 购啊 馒侩窍绊 乐阑锭父 免仿茄促.
			//---------------------------------------------
			if (pItem!=NULL)
			{			
				MPlayerGear::ITEM_STATUS itemStatus = pGear->GetItemStatus( i );

				//---------------------------------------------
				// 公扁牢 版快.. 辆幅俊 蝶扼辑 弊覆捞 促福促.
				//---------------------------------------------
				int frameID;

				if (g_pPlayer->IsSlayer())
				{
					//---------------------------------------------
					// 哭颊牢单.
					//---------------------------------------------
					if (i==MSlayerGear::GEAR_SLAYER_LEFTHAND)
					{
						// 规菩捞搁 免仿
						if (pItem->GetItemClass()==ITEM_CLASS_SHIELD)
						{
							frameID = i;
						}
						// 规菩啊 酒聪搁.. 免仿 救茄促.
						else
						{
							continue;
						}
					}
					//---------------------------------------------
					// 坷弗颊篮 公扁俊 蝶扼辑 免仿
					//---------------------------------------------
					else if (i==MSlayerGear::GEAR_SLAYER_RIGHTHAND)
					{				
						// 醚?				
						if (pItem->IsGunItem())
						{
							frameID = MSlayerGear::MAX_GEAR_SLAYER + 1 - 12;
						}
						// 绞磊啊?
						else if (pItem->GetItemClass()==ITEM_CLASS_CROSS
								|| pItem->GetItemClass()==ITEM_CLASS_MACE)
						{
							frameID = MSlayerGear::MAX_GEAR_SLAYER -12;
						}
						// 酒聪谷.. 漠捞唱 档
						else
						{
							frameID = i;
						}
					}
					else
					{
						frameID = i;
					}
				}
				else if (g_pPlayer->IsOusters() )
				{
					if( i == MOustersGear::GEAR_OUSTERS_RIGHTHAND )
					{
						if( pItem->GetItemClass() == ITEM_CLASS_OUSTERS_WRISTLET )
							frameID = MOustersGear::MAX_GEAR_OUSTERS -12;
						else
							frameID = i;
					} else
					{
						frameID = i;
					}
				} else
				{
					frameID = i;
				}

				//---------------------------------------------
				// 沥惑利捞搁 沥惑利牢芭 免仿..
				//---------------------------------------------
				if (itemStatus==MPlayerGear::ITEM_STATUS_OK || pItem->GetItemClass() == ITEM_CLASS_COUPLE_RING ||
					pItem->GetItemClass() == ITEM_CLASS_VAMPIRE_COUPLE_RING)
				{
					CFrame& frame = m_ItemBrokenFPK[frameType][frameID];
				
					spriteID = frame.GetSpriteID();

					if (spriteID!=SPRITEID_NULL)
					{
						point.x = pointBasis.x + frame.GetCX();
						point.y = pointBasis.y + frame.GetCY();

						m_pSurface->BltSprite( &point, &m_ItemBrokenSPK[spriteID] );
					}
				}
				//---------------------------------------------
				// 何辑廉 啊绰 巴捞搁.. 祸彬~乐霸 免仿..
				//---------------------------------------------
				else
				{
					int plusFrameID = ((itemStatus==MPlayerGear::ITEM_STATUS_SOMEWHAT_BROKEN)? toSomewhatBroken : toAlmostBroken);

					CFrame& frame = m_ItemBrokenFPK[frameType][frameID + plusFrameID];
				
					spriteID = frame.GetSpriteID();

					if (spriteID!=SPRITEID_NULL)
					{
						point.x = pointBasis.x + frame.GetCX();
						point.y = pointBasis.y + frame.GetCY();

						m_pSurface->BltSprite( &point, &m_ItemBrokenSPK[spriteID] );
					}
				}
			}
		}

		//---------------------------------------------
		// Unlock
		//---------------------------------------------
		m_pSurface->Unlock();
	}
}

//----------------------------------------------------------------
// Get MaxEffectFrame
//----------------------------------------------------------------
int				
MTopView::GetMaxEffectFrame(BLT_TYPE bltType, TYPE_FRAMEID frameID) const
{
	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD_FORMAT("GetMaxEffectFrame(%d, %d)", bltType, frameID);
	#endif

	int numFrame = 0;

	switch (bltType)
	{
		case BLT_SCREEN :
			numFrame = m_EffectScreenFPK[frameID][0].GetSize();
		break;

		case BLT_EFFECT :
			numFrame = m_EffectAlphaFPK[frameID][0].GetSize();
		break;

		case BLT_NORMAL :
			numFrame = m_EffectNormalFPK[frameID][0].GetSize();
		break;
		
		case BLT_SHADOW :
			numFrame = m_EffectShadowFPK[frameID][0].GetSize();
		break;
	}

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD_FORMAT("numFrame = %d", numFrame);
	#endif

	return numFrame;
}

//----------------------------------------------------------------
// Get EffectLight
//----------------------------------------------------------------
int				
MTopView::GetEffectLight(BLT_TYPE bltType, TYPE_FRAMEID frameID, int dir, int frame) const
{
	switch (bltType)
	{
		case BLT_SCREEN :
			return m_EffectScreenFPK[frameID][dir][frame].GetLight();

		case BLT_EFFECT :
			return m_EffectAlphaFPK[frameID][dir][frame].GetLight();
		
		case BLT_NORMAL :
			return m_EffectNormalFPK[frameID][dir][frame].GetLight();		

		case BLT_SHADOW :
			return m_EffectShadowFPK[frameID][dir][frame].GetLight();
	}

	return 0;
}

//----------------------------------------------------------------
// Get EffectSpriteType
//----------------------------------------------------------------
int				
MTopView::GetEffectSpriteType(BLT_TYPE bltType, TYPE_FRAMEID frameID) const
{
	switch (bltType)
	{
		case BLT_SCREEN :
			return frameID + MAX_EFFECTSPRITETYPE_ALPHAEFFECT;

		case BLT_EFFECT :
			return frameID;
		
		case BLT_NORMAL :
			return frameID + MAX_EFFECTSPRITETYPE_SCREENEFFECT;

		case BLT_SHADOW :
			return frameID + MAX_EFFECTSPRITETYPE_NORMALEFFECT;
	}

	return 0;
}

//----------------------------------------------------------------
// DrawEffect ( point, MEffect* )
//----------------------------------------------------------------
// m_pSurface->IsLock() 惑怕俊辑 龋免等促绊 啊沥茄促.
//----------------------------------------------------------------
void
MTopView::DrawEffect(POINT* pPoint, MEffect* pEffect, bool bSelectable)
{
	if(g_pEventManager->GetEventByFlag(EVENTFLAG_NOT_DRAW_EFFECT))
		return;

	POINT point = *pPoint;

	switch (pEffect->GetBltType())
	{
		//------------------------------------------------------------------
		//
		//						Normal Effect
		//
		//------------------------------------------------------------------
		case BLT_NORMAL :
		{				
			CFrame &Frame = m_EffectNormalFPK[pEffect->GetFrameID()][pEffect->GetDirection()][pEffect->GetFrame()];
			int spriteID = Frame.GetSpriteID();

			if (spriteID != SPRITEID_NULL)	//< m_EffectNormalSPK.GetSize())
			{			
				CSprite* pSprite = &m_EffectNormalSPK[ spriteID ];

				point.x += Frame.GetCX();
				point.y += Frame.GetCY();

				m_pSurface->BltSprite(&point, pSprite);				

				//---------------------------------------- 		
				// 捞棋飘 急琶 荤阿屈 康开 汲沥
				//---------------------------------------- 	
				if (bSelectable)
				{
					RECT rect;
					rect.left	= point.x;
					rect.top	= point.y;
					rect.right	= rect.left + pSprite->GetWidth();
					rect.bottom = rect.top + pSprite->GetHeight();

					pEffect->SetScreenRect( &rect );					
				}
			}
			else
			{
				pEffect->ClearScreenRect();
			}
		}
		break;

		//------------------------------------------------------------------
		//
		//						Alpha Effect
		//
		//------------------------------------------------------------------
		case BLT_EFFECT :
		{
			CEffectFrame& Frame = m_EffectAlphaFPK[pEffect->GetFrameID()][pEffect->GetDirection()][pEffect->GetFrame()];
			int spriteID = Frame.GetSpriteID();
			
			if (spriteID != SPRITEID_NULL)
			{
				point.x += Frame.GetCX();
				point.y += Frame.GetCY();

				DRAW_ALPHASPRITEPAL(&point, spriteID, m_EffectAlphaSPK, m_EffectAlphaPPK[pEffect->GetFrameID()])//, m_EffectAlphaSPKI, m_EffectAlphaSPKFile)

				//---------------------------------------- 		
				// 捞棋飘 急琶 荤阿屈 康开 汲沥
				//---------------------------------------- 	
				if (bSelectable)
				{
					CAlphaSpritePal* pSprite = &m_EffectAlphaSPK[ spriteID ];

					RECT rect;
					rect.left	= point.x;
					rect.top	= point.y;
					rect.right	= rect.left + pSprite->GetWidth();
					rect.bottom = rect.top + pSprite->GetHeight();

					pEffect->SetScreenRect( &rect );
				}

				//-------------------------------------------------------
				// H/W 啊加捞 登绰 版快捞搁...
				//-------------------------------------------------------
				if (CDirect3D::IsHAL())
				{
//					#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
//						CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, gtx_op );
//						CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  gtx_src );
//						CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, gtx_dest );	
//					#endif
//					
//					DRAW_TEXTURE_SPRITEPAL_LOCKED(point.x, point.y, spriteID, m_pAlphaEffectTextureManager, pEffect->GetFrameID() ) //m_EffectAlphaPPK[pEffect->GetFrameID()])
//
//					#ifdef OUTPUT_DEBUG
//						// 盔贰措肺
//						CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);					
//						CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//						CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
//					#endif
					
					//---------------------------------------- 		
					// 捞棋飘 急琶 荤阿屈 康开 汲沥
					//---------------------------------------- 	
//					if (bSelectable)
//					{
//						RECT rect;
//						rect.left	= point.x;
//						rect.top	= point.y;
//						rect.right	= rect.left + m_pAlphaEffectTextureManager->GetSpriteWidth(spriteID);
//						rect.bottom = rect.top + m_pAlphaEffectTextureManager->GetSpriteHeight(spriteID);
//
//						pEffect->SetScreenRect( &rect );							
//					}

					//------------------------------------------------
					// LightFilter甫 眠啊茄促.
					//------------------------------------------------
					AddLightFilter3D( pPoint->x + g_TILE_X_HALF, 
									pPoint->y + g_TILE_Y_HALF, 
									Frame.GetLight(),			// 蝴狼 灌扁
									false);	// false = screen谅钎					
				
				}
				//-------------------------------------------------------
				// H/W 啊加捞 救登绰 版快捞搁...
				//-------------------------------------------------------
				else
				{
//					DRAW_ALPHASPRITEPAL(&point, spriteID, m_EffectAlphaSPK, m_EffectAlphaPPK[pEffect->GetFrameID()])//, m_EffectAlphaSPKI, m_EffectAlphaSPKFile)

//					//---------------------------------------- 		
//					// 捞棋飘 急琶 荤阿屈 康开 汲沥
//					//---------------------------------------- 	
//					if (bSelectable)
//					{
//						CAlphaSpritePal* pSprite = &m_EffectAlphaSPK[ spriteID ];
//
//						RECT rect;
//						rect.left	= point.x;
//						rect.top	= point.y;
//						rect.right	= rect.left + pSprite->GetWidth();
//						rect.bottom = rect.top + pSprite->GetHeight();
//
//						pEffect->SetScreenRect( &rect );
//					}


					AddLightFilter2D( pPoint->x + g_TILE_X_HALF, 
									pPoint->y + g_TILE_Y_HALF, 
									Frame.GetLight(),			// 蝴狼 灌扁
									false);		// false = screen谅钎
				}
			}
		}
		break;

		//------------------------------------------------------------------
		//
		//						Screen Effect
		//
		//------------------------------------------------------------------
		case BLT_SCREEN :
		{
			CEffectFrame& Frame = m_EffectScreenFPK[pEffect->GetFrameID()][pEffect->GetDirection()][pEffect->GetFrame()];
			int spriteID = Frame.GetSpriteID();
			
			if (spriteID != SPRITEID_NULL)
			{
				point.x += Frame.GetCX();
				point.y += Frame.GetCY();

				DRAW_NORMALSPRITEPAL_EFFECT(&point, 
									spriteID, 
									m_EffectScreenSPK, 
									m_EffectScreenPPK[pEffect->GetFrameID()],
//										m_EffectScreenSPKI, 
//										m_EffectScreenSPKFile,
									CSpriteSurface::EFFECT_SCREEN)

				//---------------------------------------- 		
				// 捞棋飘 急琶 荤阿屈 康开 汲沥
				//---------------------------------------- 	
				if (bSelectable)
				{
					CSpritePal* pSprite = &m_EffectScreenSPK[ spriteID ];

					RECT rect;
					rect.left	= point.x;
					rect.top	= point.y;
					rect.right	= rect.left + pSprite->GetWidth();
					rect.bottom = rect.top + pSprite->GetHeight();

					pEffect->SetScreenRect( &rect );
				}
				//-------------------------------------------------------
				// H/W 啊加捞 登绰 版快捞搁...
				//-------------------------------------------------------
				if (CDirect3D::IsHAL())
				{
					// screen test
//					CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_ONE );
//					CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );	
//
//				#ifdef OUTPUT_DEBUG
//					CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  gtx_src );
//					CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, gtx_dest );									
//				#endif
//					
//					DRAW_TEXTURE_SPRITEPAL_LOCKED(point.x, point.y, spriteID, m_pScreenEffectTextureManager, pEffect->GetFrameID() ) //m_EffectScreenPPK[pEffect->GetFrameID()])
//
//					// 盔贰措肺
//					CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//					CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
					
					//---------------------------------------- 		
					// 捞棋飘 急琶 荤阿屈 康开 汲沥
					//---------------------------------------- 	
//					if (bSelectable)
//					{
//						RECT rect;
//						rect.left	= point.x;
//						rect.top	= point.y;
//						rect.right	= rect.left + m_pScreenEffectTextureManager->GetSpriteWidth(spriteID);
//						rect.bottom = rect.top + m_pScreenEffectTextureManager->GetSpriteHeight(spriteID);
//
//						pEffect->SetScreenRect( &rect );							
//					}

					//------------------------------------------------
					// LightFilter甫 眠啊茄促.
					//------------------------------------------------
					AddLightFilter3D( pPoint->x + g_TILE_X_HALF, 
									pPoint->y + g_TILE_Y_HALF, 
									Frame.GetLight(),			// 蝴狼 灌扁
									false);	// false = screen谅钎					
				
				}
				//-------------------------------------------------------
				// H/W 啊加捞 救登绰 版快捞搁...
				//-------------------------------------------------------
				else
				{
//					DRAW_NORMALSPRITEPAL_EFFECT(&point, 
//										spriteID, 
//										m_EffectScreenSPK, 
//										m_EffectScreenPPK[pEffect->GetFrameID()],
////										m_EffectScreenSPKI, 
////										m_EffectScreenSPKFile,
//										CSpriteSurface::EFFECT_SCREEN)

//					//---------------------------------------- 		
//					// 捞棋飘 急琶 荤阿屈 康开 汲沥
//					//---------------------------------------- 	
//					if (bSelectable)
//					{
//						CSpritePal* pSprite = &m_EffectScreenSPK[ spriteID ];
//
//						RECT rect;
//						rect.left	= point.x;
//						rect.top	= point.y;
//						rect.right	= rect.left + pSprite->GetWidth();
//						rect.bottom = rect.top + pSprite->GetHeight();
//
//						pEffect->SetScreenRect( &rect );
//					}


					AddLightFilter2D( pPoint->x + g_TILE_X_HALF, 
									pPoint->y + g_TILE_Y_HALF, 
									Frame.GetLight(),			// 蝴狼 灌扁
									false);		// false = screen谅钎
				}

			}
		}
		break;
		
		//------------------------------------------------------------------
		//
		//						Shadow Effect
		//
		//------------------------------------------------------------------
		case BLT_SHADOW :
		{
			CEffectFrame& Frame = m_EffectShadowFPK[pEffect->GetFrameID()][pEffect->GetDirection()][pEffect->GetFrame()];
			int spriteID = Frame.GetSpriteID();
			
			if (spriteID != SPRITEID_NULL)
			{
				point.x += Frame.GetCX();
				point.y += Frame.GetCY();

				CShadowSprite* pSprite = &m_EffectShadowSPK[spriteID];

				if (pSprite->IsInit())
				{
					m_pSurface->BltShadowSpriteDarkness(&point, pSprite, 1 );
				}
				//-------------------------------------------------------
				// H/W 啊加捞 登绰 版快捞搁...
				//-------------------------------------------------------
				if (CDirect3D::IsHAL())
				{
//					DRAW_TEXTURE_SPRITE_LOCKED(point.x, point.y, spriteID, m_pEffectShadowManager)		

					//------------------------------------------------
					// LightFilter甫 眠啊茄促.
					//------------------------------------------------
					AddLightFilter3D( pPoint->x + g_TILE_X_HALF, 
									pPoint->y + g_TILE_Y_HALF, 
									Frame.GetLight(),			// 蝴狼 灌扁
									false);	// false = screen谅钎					
				
				}
				//-------------------------------------------------------
				// H/W 啊加捞 救登绰 版快捞搁...
				//-------------------------------------------------------
				else
				{
//					CShadowSprite* pSprite = &m_EffectShadowSPK[spriteID];
//
//					if (pSprite->IsInit())
//					{
//						m_pSurface->BltShadowSpriteDarkness(&point, pSprite, 1 );
//						
						AddLightFilter2D( pPoint->x + g_TILE_X_HALF, 
										pPoint->y + g_TILE_Y_HALF, 
										Frame.GetLight(),			// 蝴狼 灌扁
										false);		// false = screen谅钎
//					}
				}
			}
		}
		break;
	}
}

//----------------------------------------------------------------------
// Get Random MonsterType In Zone
//----------------------------------------------------------------------
int				
MTopView::GetRandomMonsterTypeInZone() const
{
	int numSprites = m_listLoadedMonsterSprite.GetSize();

	int spriteType;

	if (numSprites==0)
	{
		// 喉矾靛 况废 弊覆.. 
		spriteType = 27;
	}
	else
	{
		int nth = rand()%numSprites;

		INT_ORDERED_LIST::DATA_LIST::const_iterator	iID = m_listLoadedMonsterSprite.GetIterator();

		for (int i=0; i<nth; i++)
		{			
			iID ++;
		}

		spriteType = *iID;
	}

	// spriteID啊 sprite牢 阁胶磐甸 吝俊辑 窍唱甫 急琶茄促.
	return g_pCreatureSpriteTypeMapper->GetRandomCreatureType( spriteType );
}

void		
MTopView::DrawCreatureHPModify(POINT *point, MCreature* pCreature)
{	
//	return;
	
	if(!g_pPlayer->HasEffectStatus( EFFECTSTATUS_VIEW_HP ) || pCreature->IsEmptyHPModifyList())
		return;

	MCreature::HPMODIFYLIST *pList = (MCreature::HPMODIFYLIST *)pCreature->GetHPModifyList();

	MCreature::HPMODIFYLIST::iterator itr = pList->begin();

	int py = point->y - (pList->size()-1)*15;

	g_FL2_GetDC();

	while(itr != pList->end())
	{
		char str[128];
		COLORREF color;
		
		const int modifyValue = itr->modify;
		if(itr->modify < 0 )
		{
			sprintf(str, "%d", modifyValue);
			color = RGB(255, 150, 150);
		}
		else
		{
			sprintf(str, "+%d", modifyValue);
			RGB(150, 255, 150);
		}
		
		g_PrintColorStrOut(point->x + g_TILE_X_HALF - g_GetStringWidth(str, gpC_base->m_chatting_pi.hfont)/2, py, str, gpC_base->m_chatting_pi, color, RGB_BLACK);

		py += 15;

		if(GetTickCount() - itr->TickCount > g_pClientConfig->HPModifyListTime)
		{
			pList->pop_front();
			itr = pList->begin();
		}
		else
			itr++;
	}

	g_FL2_ReleaseDC();
}
// 2004, 08, 18, sobeit add start 
//----------------------------------------------------------------------
// DrawGuildMark
/*----------------------------------------------------------------------
	- 傍己傈 矫 某腐磐 困俊 辨靛 付农甫 焊咯霖促.
	- 急琶 咯何客 惑包绝捞 亲惑 焊牢促.
	- g_pPlayer啊 傍己傈 曼啊吝老 锭父 傍己傈 曼啊吝牢 促弗 农府媚狼 辨靛付农 钎矫
//----------------------------------------------------------------------*/
void		
MTopView::DrawGuildMarkInSiegeWar(MCreature* pCreature, int YPos)
{	
	// EFFECTSTATUS_GHOST 俊 吧妨乐栏搁 巢档 给焊绊 唱档 给夯促
	if(pCreature == NULL) return;
	if(pCreature->HasEffectStatus(EFFECTSTATUS_GHOST))return;

	//-----------------------------------------------------
	// Hallu俊 吧府搁 辨靛付农唱 拌鞭付农 救嘛澜
	//-----------------------------------------------------
	if(!g_pPlayer->HasEffectStatus(EFFECTSTATUS_HALLUCINATION)
		&& pCreature->GetCreatureType() != CREATURETYPE_VAMPIRE_OPERATOR
		&& pCreature->GetCreatureType() != CREATURETYPE_SLAYER_OPERATOR
		&& pCreature->GetCreatureType() != CREATURETYPE_OUSTERS_OPERATOR
		&& g_pPlayer->GetIsInSiegeWar()
		&& pCreature->GetIsInSiegeWar()
		)
	{
		//-----------------------------------------------------
		// Guild啊 乐绰 版快 - Guild Mark 免仿
		//-----------------------------------------------------
		int guildID = pCreature->GetGuildNumber();
			
		if (guildID > 0)
		{
			//-------------------------------------------------
			// load登绢 乐绰瘤 夯促.
			//-------------------------------------------------
			CSprite* pSprite = g_pGuildMarkManager->GetGuildMarkSmall(guildID);
			
			if (pSprite!=NULL)
			{							
				POINT pointTemp = { pCreature->GetPixelX() - m_FirstZonePixel.x  + 15, YPos - 23};
				
				m_pSurface->BltSprite(&pointTemp, pSprite);				
				
			}
			else
			{
				if (!g_pGuildMarkManager->HasGuildMark(guildID))
				{
					//-------------------------------------------------
					// file俊 乐绰瘤 夯促.
					//-------------------------------------------------
					g_pGuildMarkManager->LoadGuildMark(guildID);
					
					//-------------------------------------------------
					// file俊辑 load登菌绰瘤 促矫 眉农
					//-------------------------------------------------
					pSprite = g_pGuildMarkManager->GetGuildMark(guildID);
					
					//-------------------------------------------------
					// file俊档 绝绰 版快..
					// guildMark包府辑滚?俊辑 罐酒柯促.
					//-------------------------------------------------
					//if (pSprite==NULL) {}
				}				
			}
		}
	}
}
// 2004, 08, 18, sobeit add end
//----------------------------------------------------------------------
// Draw CreatureName
//----------------------------------------------------------------------
// Unlock 惑怕俊辑 阂妨瘤绰 窃荐捞促.
//
// 急琶等 某腐磐 捞抚阑 免仿茄促.
// 捞抚俊绰 HP, 己氢, 辨靛.. 殿殿狼 沥焊啊 器窃等促.
// 辑厚胶肺(-_-;) RequestMode俊 蝶弗 icon档 免仿茄促.
//----------------------------------------------------------------------
void		
MTopView::DrawCreatureName(MCreature* pCreature)
{	
	// EFFECTSTATUS_GHOST 俊 吧妨乐栏搁 巢档 给焊绊 唱档 给夯促
	if(pCreature == NULL) return;
	if(pCreature->HasEffectStatus(EFFECTSTATUS_GHOST))return;

	// 2004, 08, 05 sobeit add start - 己巩篮 焊咯瘤搁 捞惑窍促?
	if( pCreature->GetCreatureType() >= 726 &&
		pCreature->GetCreatureType() <= 729)
		return;
	// 2004, 08, 05 sobeit add end - 己巩 
	if(
		(g_pPlayer->HasEffectStatus(EFFECTSTATUS_YELLOW_POISON_TO_CREATURE) || 
		g_pPlayer->HasEffectStatus(EFFECTSTATUS_FLARE )||
		g_pPlayer->HasEffectStatus(EFFECTSTATUS_BLINDNESS))
		&& pCreature != g_pPlayer 
		)
	{
		// 芭府俊 蝶扼辑 免仿 咯何甫 搬沥茄促.
		int sx,sy,ex,ey;

		sx = g_pPlayer->GetX() - 1;
		ex = g_pPlayer->GetX() + 1;

		sy = g_pPlayer->GetY() - 1;
		ey = g_pPlayer->GetY() + 1;

		if( pCreature->GetX() < sx || pCreature->GetX() > ex ||
			pCreature->GetY() < sy || pCreature->GetY() > ey )
			return;
	}

	//---------------------------------------------------------------
	// 顶 加俊 乐绰 局绰 捞抚档 绝寸. - -;
	//---------------------------------------------------------------
	if (pCreature->IsUndergroundCreature())
	{
		return;
	}

	POINT point = m_pointChatString;
	int yPoint = DrawChatString(&point, 
								pCreature, 
								RGB_WHITE,	//CDirectDraw::Color(31,31,31),
								FLAG_DRAWTEXT_OUTLINE);
	point.y = yPoint;
	DrawCreatureHPModify(&point, pCreature);

	m_bDrawRequest = false;
	//------------------------------------------------
	// item 背券茄促绰 钎矫 免仿
	//------------------------------------------------
	if (IsRequestMode())	
	{
		bool bRequest = false;

		if(pCreature->GetClassType() == MCreature::CLASS_FAKE)
		{
			MFakeCreature *pFakeCreature = (MFakeCreature *)pCreature;
			if(pFakeCreature->GetOwnerID() != OBJECTID_NULL)	// 脐捞促
			{
				bRequest = true;
			}
		}
		//------------------------------------------------
		// 酒流篮 免仿窍搁 救等促.
		//------------------------------------------------			
		else if (// NPC啊 酒聪绊
			!pCreature->IsNPC()
			// Player牢 版快父
			//&& pCreature->GetCreatureType()<=CREATURETYPE_VAMPIRE_FEMALE
			//&& (*g_pCreatureSpriteTable)[(*g_pCreatureTable)[pCreature->GetCreatureType()].SpriteType].IsPlayerOnlySprite()
			&& (*g_pCreatureSpriteTable)[(*g_pCreatureTable)[pCreature->GetCreatureType()].SpriteTypes[0]].IsPlayerOnlySprite()
			// 包 加俊 乐绰 版快
			&& !pCreature->IsInCasket()
			// 顶俊 汞囚乐阑订 芭贰 茄促绰 钎矫 救 唱坷霸...
			//&& !pCreature->IsUndergroundCreature()
			// 混酒乐绰.. - -;
			&& pCreature->IsAlive()
			&& pCreature->GetCreatureType() != CREATURETYPE_SLAYER_OPERATOR
			&& pCreature->GetCreatureType() != CREATURETYPE_VAMPIRE_OPERATOR
			&& pCreature->GetCreatureType() != CREATURETYPE_OUSTERS_OPERATOR
			)
		{
			bRequest = true;
		}

		if(bRequest == true)
		{
			POINT pointTemp;
			TYPE_SPRITEID	RequestSpriteID;
				
			//---------------------------------------------------------------
			// trade 脚没窍绰 酒捞能
			//---------------------------------------------------------------
			if (IsRequestTrade())
			{
				const maxRequestFrames = SPRITEID_ITEM_TRADE_LAST - SPRITEID_ITEM_TRADE + 1;
				RequestSpriteID = SPRITEID_ITEM_TRADE + ((g_CurrentFrame>>1) % (maxRequestFrames<<1));

				if (RequestSpriteID > SPRITEID_ITEM_TRADE_LAST)
				{
					RequestSpriteID = SPRITEID_ITEM_TRADE_LAST - (RequestSpriteID - SPRITEID_ITEM_TRADE_LAST);

					if (RequestSpriteID < SPRITEID_ITEM_TRADE)
					{
						RequestSpriteID = SPRITEID_ITEM_TRADE;
					}
				}					
			}
			//---------------------------------------------------------------
			// party 脚没窍绰 酒捞能
			//---------------------------------------------------------------
			else if(IsRequestParty())
			{
				const maxRequestFrames = SPRITEID_PARTY_REQUEST_LAST - SPRITEID_PARTY_REQUEST + 1;
				RequestSpriteID = SPRITEID_PARTY_REQUEST + ((g_CurrentFrame>>1) % (maxRequestFrames+4));

				if (RequestSpriteID > SPRITEID_PARTY_REQUEST_LAST)
				{
					RequestSpriteID = SPRITEID_PARTY_REQUEST_LAST;					
				}
			}
			//---------------------------------------------------------------
			// Info 脚没窍绰 酒捞能
			//---------------------------------------------------------------
			else if(IsRequestInfo())
			{
				const maxRequestFrames = SPRITEID_INFO_REQUEST_LAST - SPRITEID_INFO_REQUEST + 1;
				RequestSpriteID = SPRITEID_INFO_REQUEST + ((g_CurrentFrame>>1) % (maxRequestFrames+4));

				if (RequestSpriteID > SPRITEID_INFO_REQUEST_LAST)
				{
					RequestSpriteID = SPRITEID_INFO_REQUEST_LAST;					
				}
			}

			pointTemp.x = g_x + 27 - m_EtcSPK[RequestSpriteID].GetWidth();
			pointTemp.y = g_y + 15 - m_EtcSPK[RequestSpriteID].GetHeight();

			m_pSurface->Lock();
			m_pSurface->BltSprite( &pointTemp, &m_EtcSPK[RequestSpriteID] );
			m_pSurface->Unlock();

			m_bDrawRequest = true;
		}			
	}

		
	//------------------------------------------------
	// 某腐磐 捞抚 免仿
	//------------------------------------------------
	//gC_font.PrintStringNoConvert(&m_SurfaceInfo, pCreature->GetName(), pointTemp.x, pointTemp.y, 0xFFFF);	
	bool bMasterWords = pCreature->GetCreatureType()==CREATURETYPE_SLAYER_OPERATOR
						|| pCreature->GetCreatureType()==CREATURETYPE_VAMPIRE_OPERATOR
						|| pCreature->GetCreatureType() == CREATURETYPE_OUSTERS_OPERATOR
						|| strncmp( pCreature->GetName(), (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetString(), (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetLength() ) == 0;
//						|| (strstr(pCreature->GetName(), "款康磊")!=NULL);

	bool bHalluName = (g_pPlayer->HasEffectStatus(EFFECTSTATUS_HALLUCINATION) || 
		!bMasterWords && 
		!g_pUserInformation->IsMaster && 
		(pCreature->GetClassType() == MCreature::CLASS_PLAYER || 
		pCreature->GetClassType() == MCreature::CLASS_CREATUREWEAR) && 
		g_pPlayer->GetRace() != pCreature->GetRace()) &&
		g_pZoneTable->Get( g_pZone->GetID() )->ChatMaskByRace &&
		g_pPlayer->GetCreatureType() != CREATURETYPE_SLAYER_OPERATOR &&
		g_pPlayer->GetCreatureType() != CREATURETYPE_VAMPIRE_OPERATOR &&
		g_pPlayer->GetCreatureType() != CREATURETYPE_OUSTERS_OPERATOR;						// 锚 付胶农甫 竞匡 版快俊父 
	if(g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST ))
		bHalluName = false;
#ifdef __METROTECH_TEST__
	if(g_bLight)
		bHalluName = false;
#endif
	const char* pName = (bHalluName?pCreature->GetHalluName() : pCreature->GetName());

	COLORREF	color;
	FONTID		font;

	/*
	if (pCreature->IsVampire())
	{
		color	= m_ColorNameVampire;
		font	= FONTID_VAMPIRE_NAME;
	}
	else if (pCreature->IsSlayer())
	{
		color = m_ColorNameSlayer;
		font	= FONTID_SLAYER_NAME;
	}
	else if (pCreature->IsNPC())
	{
		color = m_ColorNameNPC;
		font	= FONTID_NPC_NAME;
	}
	*/

	//-----------------------------------------------------
	// NPC牢 版快
	//-----------------------------------------------------
	if (pCreature->IsNPC())
	{
		// 官配府牢 版快.. 窍靛内爹 - -;;
		if (pCreature->GetCreatureType()==217)
		{
			color	= m_ColorNameAlignment[1];
			font	= FONTID_VAMPIRE_NAME;
		}
		else
		{
			color = m_ColorNameNPC;
			font	= FONTID_NPC_NAME;			
		}
	}
	//-----------------------------------------------------
	// 款康磊牢 版快
	//-----------------------------------------------------
	else if (pCreature->GetCompetence()==0)
	{
		color = CDirectDraw::Color( 31, 23, 3 );	// 陛祸?
		font	= FONTID_NPC_NAME;
	}
	//-----------------------------------------------------
	// Hallu俊 吧赴 版快
	//-----------------------------------------------------
	else if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_HALLUCINATION))
	{
		color	= m_ColorNameAlignment[1];
		font	= FONTID_VAMPIRE_NAME;
	}
	//-----------------------------------------------------
	// Slayer牢 版快 
	//-----------------------------------------------------
	else if (g_pPlayer->IsSlayer())
	{
		//-----------------------------------------------------
		// 辆练俊 蝶扼辑 
		//-----------------------------------------------------
		if (g_pPlayer->CanAttackTribe( pCreature )
			|| g_pJusticeAttackManager->HasCreature( pCreature->GetName() ))
//		if (g_pObjectSelector->CanAttack( pCreature ) )
		{
			color	= m_ColorNameAlignment[1];
			font	= FONTID_VAMPIRE_NAME;
		}
		//-----------------------------------------------------
		// 酒聪搁.. 己氢俊 蝶弗 钎矫
		//-----------------------------------------------------
		else
		{
			int alignment = pCreature->GetAlignment();

			//alignment = min( 4, max(0, alignment) );

			color = m_ColorNameAlignment[alignment];
			font	= FONTID_SLAYER_NAME;				
		}
	}
	else if (g_pPlayer->IsOusters() )
	{
		if (g_pPlayer->CanAttackTribe( pCreature )
			|| g_pJusticeAttackManager->HasCreature( pCreature->GetName() ))
		{
			color	= m_ColorNameAlignment[1];
			font	= FONTID_VAMPIRE_NAME;
		}
		//-----------------------------------------------------
		// 酒聪搁.. 己氢俊 蝶弗 钎矫
		//-----------------------------------------------------
		else
		{
			int alignment = pCreature->GetAlignment();
			
			color = m_ColorNameAlignment[alignment];
			font	= FONTID_VAMPIRE_NAME;				
		}
	}
	//-----------------------------------------------------
	// 轨颇捞绢牢 版快
	//-----------------------------------------------------
	else
	{
		//-----------------------------------------------------
		// 辨靛俊 蝶扼辑
		//-----------------------------------------------------
		if (g_pPlayer->CanAttackGuild( pCreature )
			|| g_pJusticeAttackManager->HasCreature( pCreature->GetName() ))
//		if(g_pObjectSelector->CanAttack( pCreature ) )
		{
			color	= m_ColorNameAlignment[1];
			font	= FONTID_VAMPIRE_NAME;
		}
		//-----------------------------------------------------
		// 酒聪搁.. 己氢俊 蝶弗 钎矫
		//-----------------------------------------------------
		else
		{
			int alignment = pCreature->GetAlignment();

			color = m_ColorNameAlignment[alignment];
			font	= FONTID_VAMPIRE_NAME;					
		}			
	}

	if (pName!=NULL)
	{
		//-----------------------------------------------------
		// Level Name捞 乐绰 版快
		//-----------------------------------------------------
		if (pCreature->HasLevelName())
		{
			const COLORREF levelNameColor = RGB(200, 250, 200);

			int yPoint_40 = yPoint - (g_pClientConfig->FONT_HEIGHT << 1)-3;

			DRAWTEXT_NODE* pLevelNameNode = new DRAWTEXT_NODE (
													m_pointChatString.x+1,
													yPoint_40+3,//2+1,
													pCreature->GetLevelName(),
													levelNameColor,
													font
													);			

			AddText( pLevelNameNode );
		}
		
		int yPoint_20 = yPoint - 23;	// g_pClientConfig->FONT_HEIGHT
		
		//int MAX_HP_BAR = pCreature->GetHPBarWidth(); //g_pClientConfig->MAX_HP_BAR_PIXEL;
		int POSITION_HP_BAR = g_pClientConfig->POSITION_HP_BAR;

		int namePixel = g_GetStringWidth(pName, g_ClientPrintInfo[font]->hfont);
		int MAX_HP_BAR = max(100, namePixel + 20);
		
		int guildID = 0;
		//-----------------------------------------------------
		// 捞抚 关俊 彬副 傈眉(捞抚) 农扁狼 冠胶
		//-----------------------------------------------------
		int rectLeft	= m_pointChatString.x + POSITION_HP_BAR -14 ;
		int rectRight	= rectLeft + MAX_HP_BAR;//m_pointChatString.x+5 + namePixel;
		int rectTop		= yPoint_20;
		int rectBottom	= yPoint_20 + g_pClientConfig->FONT_HEIGHT;

		//-----------------------------------------------------
		// Hallu俊 吧府搁 辨靛付农唱 拌鞭付农 救嘛澜
		//-----------------------------------------------------
		if(!g_pPlayer->HasEffectStatus(EFFECTSTATUS_HALLUCINATION)
			&& pCreature->GetCreatureType() != CREATURETYPE_VAMPIRE_OPERATOR
			&& pCreature->GetCreatureType() != CREATURETYPE_SLAYER_OPERATOR
			&& pCreature->GetCreatureType() != CREATURETYPE_OUSTERS_OPERATOR
			&& pCreature->GetCreatureType() != 672
			&& pCreature->GetCreatureType() != 673
			)
		{
			//-----------------------------------------------------
			// Guild啊 乐绰 版快 - Guild Mark 免仿
			//-----------------------------------------------------
			guildID = pCreature->GetGuildNumber();
			if(	g_pPlayer->GetRace() != pCreature->GetRace() && pCreature->IsPlayer() &&
				g_pZoneTable->Get( g_pZone->GetID() )->ChatMaskByRace == true
				)
				guildID = 0;
			
			// grade免仿
			int gradeID = pCreature->GetGrade()-1;	// 1~50 栏肺 登乐栏骨肺 -1秦辑 0~49肺 嘎冕促
			// 敲饭捞绢父 拌鞭 免仿, 冠零唱 戳措绰 救窃
			if(!pCreature->IsPlayerOnly())
				gradeID = -1;
			
				/*
				if (guildID>=0)
				{
				// run-time loading档 宝满阑 淀..
				if (guildID < m_GuildSPK.GetSize())
				{
				m_pSurface->Lock();
				
				  POINT pointTemp = { rectLeft-20, yPoint_20 };
				  
					m_pSurface->BltSprite( &pointTemp, &m_GuildSPK[guildID] );
					
					  m_pSurface->Unlock();
					  }
					  }
			*/
			if (guildID > 0)
			{
				//-------------------------------------------------
				// load登绢 乐绰瘤 夯促.
				//-------------------------------------------------
				CSprite* pSprite = g_pGuildMarkManager->GetGuildMarkSmall(guildID);
				
				if (pSprite!=NULL)
				{			
					m_pSurface->Lock();
					
					POINT pointTemp = { rectLeft-20, yPoint_20 };
					
					m_pSurface->BltSprite(&pointTemp, pSprite);				
					
					m_pSurface->Unlock();
				}
				else
				{
					if (!g_pGuildMarkManager->HasGuildMark(guildID))
					{
						//-------------------------------------------------
						// file俊 乐绰瘤 夯促.
						//-------------------------------------------------
						g_pGuildMarkManager->LoadGuildMark(guildID);
						
						//-------------------------------------------------
						// file俊辑 load登菌绰瘤 促矫 眉农
						//-------------------------------------------------
						pSprite = g_pGuildMarkManager->GetGuildMark(guildID);
						
						//-------------------------------------------------
						// file俊档 绝绰 版快..
						// guildMark包府辑滚?俊辑 罐酒柯促.
						//-------------------------------------------------
						//if (pSprite==NULL) {}
					}				
				}
			}
			
			// 拌鞭捞 乐绰版快 拌鞭付农 免仿
			if(gradeID > -1 && gradeID <= GRADE_MARK_MAX)
			{
				CSprite* pSprite = g_pGuildMarkManager->GetGradeMarkSmall(gradeID, pCreature->GetRace());
				
				if (pSprite!=NULL)
				{			
					m_pSurface->Lock();
					
					POINT pointTemp = { rectRight, yPoint_20 };
					
					m_pSurface->BltSprite(&pointTemp, pSprite);				
					
					m_pSurface->Unlock();
				}
			}
			
		}
		// 脐牢版快 饭骇 付农 免仿
		if(pCreature->GetClassType() == MCreature::CLASS_FAKE)
		{
			MFakeCreature *pFakeCreature = (MFakeCreature *)pCreature;

			if(pFakeCreature->GetOwnerID() != OBJECTID_NULL)
			{
				MPetItem *pPetItem = pFakeCreature->GetPetItem();
				
				if(pPetItem != NULL)
				{
					int petLevel = pPetItem->GetNumber();
					if(petLevel > -1 && petLevel <= 50)
					{
						CSprite* pSprite = g_pGuildMarkManager->GetLevelMarkSmall(petLevel);
						
						if (pSprite!=NULL)
						{			
							m_pSurface->Lock();
							
							POINT pointTemp = { rectLeft-20, yPoint_20 };
							
							m_pSurface->BltSprite(&pointTemp, pSprite);				
							
							m_pSurface->Unlock();
						}
					}
				}
			}
		}

		//-----------------------------------------------------
		// HP拌魂
		//-----------------------------------------------------
		int currentHP		= max(0,int(pCreature->GetHP()));
		int maxHP			= pCreature->GetMAX_HP();

		#ifdef OUTPUT_DEBUG
			if (g_pDXInput->KeyDown(DIK_H) && 
				(g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL)))
			{
				char str[128];
				sprintf(str, "HP=%d/%d", currentHP, maxHP);
				m_pSurface->GDI_Text(rectLeft, rectTop-20, str, 0xFFFF);
			}					
		#endif

		//-----------------------------------------------------
		// 顶加俊 乐绰 局绰 HP啊 菜 蛮巴贸烦 焊咯霖促.
		//-----------------------------------------------------
		//if (pCreature->IsUndergroundCreature())
		//{
		//	currentHP = 1;
			//maxHP = 1;
		//}

		if (currentHP > maxHP)
		{
			currentHP = maxHP;
		}

		//-----------------------------------------------------
		// 角力肺 焊咯龙 pixel拌魂
		//-----------------------------------------------------
		int maxPixels		= rectRight - rectLeft;
		int currentPixels	= max(0,int((maxHP==0)? 0 : maxPixels * currentHP / maxHP));

		int nameX = //rectLeft+(MAX_HP_BAR>>1) - (namePixel>>1);
					rectLeft + ((MAX_HP_BAR - namePixel)>>1);

		
		//-----------------------------------------------------
		// HP啊 菜 瞒霸 钎矫登绰 版快
		//-----------------------------------------------------
		if (currentPixels==maxPixels)
		{
			// 官帕祸 彬 鞘夸 绝瘤父.. 捞抚 肋 焊捞霸 且妨绊.. --
			DRAWTEXT_NODE* pNodeBase = new DRAWTEXT_NODE (
										nameX+1,
										yPoint_20+4+1,
										NULL,//pName,
										0,
										font);

			AddText( pNodeBase );
		}
		//-----------------------------------------------------
		// HP 粹篮 何盒 钎矫
		//-----------------------------------------------------
		else
		{
			//-----------------------------------------------------
			//
			//			Max HP 父怒狼 Box父 钎矫
			//
			//-----------------------------------------------------
			DRAWTEXT_NODE* pNodeBase = new DRAWTEXT_NODE (
										nameX+1,
										yPoint_20+4+1,
										NULL,//pName,
										0,
										font);

			//-----------------------------------------------------
			// HP 粹篮 何盒
			//-----------------------------------------------------
			RECT rect = {	rectLeft + currentPixels, 
							rectTop,
							rectRight,
							rectBottom 
			};


			pNodeBase->SetBox( rect, m_ColorBlackHalf );
			AddText( pNodeBase );
		}

		//-----------------------------------------------------
		//
		//		泅犁 HP父怒狼 Box客 某腐磐 捞抚 免仿
		//
		//-----------------------------------------------------
		// 泅犁 HP父怒狼 Box客 某腐磐 捞抚 免仿(鳖改霸)
		//-----------------------------------------------------	
		DRAWTEXT_NODE* pNode = new DRAWTEXT_NODE (
									nameX + 1,
									yPoint_20+4 + 1,
									pName,
									0,		//color,
									font);	//FLAG_DRAWTEXT_HANGUL_JOHAP);

		//-----------------------------------------------------
		// 0捞 酒聪搁 box免仿
		//-----------------------------------------------------							
		if (currentPixels!=0)				
		{
			//-----------------------------------------------------
			// 泅犁 HP俊 措茄 冠胶
			//-----------------------------------------------------
			RECT rectHP = {	rectLeft, 
							rectTop,
							rectLeft + currentPixels,
							rectBottom
			};

			//-----------------------------------------------------
			// Current HP
			//-----------------------------------------------------				
			if (g_pUserOption->DrawTransHPBar)
			{
				pNode->SetBox( rectHP, m_ColorHPBar, true );
			}
			else
			{
				// 阂捧疙茄 HP bar甫 免仿且锭
				static WORD notTransHPBar 
					= CDirectDraw::Color(g_pClientConfig->COLOR_HP_BAR_R>>1,
											g_pClientConfig->COLOR_HP_BAR_G>>1,
											g_pClientConfig->COLOR_HP_BAR_B>>1);

				pNode->SetBox( rectHP, notTransHPBar, false );
			}			
		}

		//-----------------------------------------------------
		// 免仿窍霸 汲沥..
		//-----------------------------------------------------
		AddText( pNode );			

		// 2004, 10, 28, sobeit add start -阁胶磐 懦 涅胶飘 秦寸 阁胶磐俊 钎矫.
//		if(pCreature->IsAlive() && UI_IsMonsterKillQuest_Monster((*g_pCreatureTable)[pCreature->GetCreatureType()].SpriteTypes[0]))
//		{
//			WORD temp_color;
//			temp_color = CIndexSprite::ColorSet[MItem::GetQuestItemColorset()][15];
//			color  = RGB(CDirectDraw::Red(temp_color)<<3, CDirectDraw::Green(temp_color)<<3, CDirectDraw::Blue(temp_color)<<3);
//		}
		// 2004, 10, 28, sobeit add end
		//-----------------------------------------------------
		// 捞抚 促矫 嘛扁
		//-----------------------------------------------------
		DRAWTEXT_NODE* pNode2 = new DRAWTEXT_NODE (
									nameX,
									yPoint_20+4,
									pName,
									color,
									font);	//FLAG_DRAWTEXT_HANGUL_JOHAP);

		AddText( pNode2 );
		// 2004, 6, 17, sobeit add start - about nick name - 鞍篮 辆练 老锭父 葱匙烙 焊捞霸..荐沥.
		if(pCreature->GetNickNameType() != NicknameInfo::NICK_NONE)// && (/*g_pPlayer->GetRace() == pCreature->GetRace()|| */pCreature->IsFakeCreature()))
		{
			if(yPoint-42<0) return; // 傲 嘛瘤 富磊..ぞぞ
			BYTE bType = pCreature->GetNickNameType();
			const char* szNickName = pCreature->GetNickName().c_str();
			if(strlen(szNickName)>0)
			{
				font = FONTID_USER_ID;

				namePixel = g_GetStringWidth(szNickName, g_ClientPrintInfo[font]->hfont);
				WORD bgColor = 0;
				
				switch(bType)
				{
				case NicknameInfo::NICK_BUILT_IN:
					color = RGB_YELLOW;
					bgColor = CDirectDraw::Color(255,0,0);

					break;
				case NicknameInfo::NICK_QUEST:
					color = RGB(150,150,150);
					bgColor = CDirectDraw::Color(0,0,255);
					break;
				case NicknameInfo::NICK_FORCED:
				case NicknameInfo::NICK_CUSTOM_FORCED:
					color = RGB_RED;
					break;
				case NicknameInfo::NICK_CUSTOM:
					color = RGB(50,170,230);
				//	bgColor = CDirectDraw::Color(255,0,0);
					break;
				default:
					color = RGB_YELLOW;
					break;
				}
			//	if(guildID >0) // 辨靛付农啊 救嘛鳃订 葱匙烙阑 hpbar+拌鞭付农 吝居俊 嘛绊
			//		nameX = rectLeft + ((MAX_HP_BAR - namePixel+20)>>1);
			//	else // 弊寇浚 hpbar 吝居俊 嘛绰促
					nameX = rectLeft + ((MAX_HP_BAR - namePixel)>>1);

				DRAWTEXT_NODE* pNode = new DRAWTEXT_NODE (
												nameX ,
												yPoint-38,
												szNickName,
												color,		//color,
												font);	//FLAG_DRAWTEXT_HANGUL_JOHAP);
				if(bgColor != 0)
				{
//					RECT rectHP = {	nameX-2, 
//									yPoint-42,
//									nameX + namePixel+4,
//									yPoint-23	};
					
					RECT rectHP = {	rectLeft -20, 
									yPoint-42,
									rectRight + 20 ,
									yPoint-23	};

					if (g_pUserOption->DrawTransHPBar)
					{
						pNode->SetBox( rectHP, bgColor, true );
					}
					else
					{
						pNode->SetBox( rectHP, bgColor, false );
					}			
				}	
				AddText( pNode );			
			}
		}
		// 2004, 6, 17, sobeit add end - about nick name
	}
}


//----------------------------------------------------------------------
// GetChangeValueToDirection
//----------------------------------------------------------------------
// 咯扁 乐绢具且 窃荐绰 酒聪瘤父.. - -;
//----------------------------------------------------------------------
POINT
MTopView::GetChangeValueToDirection(int direction)
{
	POINT pt = { 0, 0 };

	switch (direction)
	{
		case DIRECTION_LEFTDOWN		: pt.x=-1;	pt.y=1;	break;
		case DIRECTION_RIGHTUP		: pt.x=1;	pt.y=-1;	break;
		case DIRECTION_LEFTUP		: pt.x=-1;	pt.y=-1;	break;
		case DIRECTION_RIGHTDOWN	: pt.x=1;	pt.y=1;	break;
		case DIRECTION_LEFT			: pt.x=-1;			break;
		case DIRECTION_DOWN			: pt.y=1;	break;
		case DIRECTION_UP			: pt.y=-1;	break;
		case DIRECTION_RIGHT		: pt.x=1;			break;	
	}

	return pt;
}


void
MTopView::DrawUndergroundCreature(POINT *pPoint, MCreature *pCreature)
{
	//----------------------------------------
	// 绊沥等 burrow弊覆  -_-;; 
	//----------------------------------------
	POINT pointTemp;
	RECT rect;

	CSprite* pSprite = &m_EtcSPK[ SPRITEID_CREATURE_BURROW ];	
	
	//----------------------------------------
	// 谅钎 焊沥
	//----------------------------------------
	int cx = 4;
	int cy = 4;
	pointTemp.x = pPoint->x + cx;
	pointTemp.y = pPoint->y + cy;
	
	//---------------------------------------- 		
	// 某腐磐 急琶 荤阿屈 康开 汲沥
	//---------------------------------------- 	
	rect.left	= pointTemp.x;
	rect.top	= pointTemp.y;
	rect.right	= rect.left + pSprite->GetWidth();
	rect.bottom = rect.top + pSprite->GetHeight();
	pCreature->SetScreenRect( &rect );				
	
	//---------------------------------------- 	
	// 急琶等 版快
	//---------------------------------------- 	
	if (m_SelectCreatureID == pCreature->GetID() )
	{
		// SpriteOutlineManager俊 眠啊
		
		// 顶加俊 乐绰 局甸篮 公炼扒 vampire捞促.
		if (g_pObjectSelector->CanAttack(pCreature))
		{
			m_SOMOutlineColor = m_ColorOutlineAttackPossible;
		}
		else
		{
			m_SOMOutlineColor = m_ColorOutlineAttackImpossible;
		}			
		
		m_SOM.Add( pointTemp.x, pointTemp.y, pSprite );
		m_SOM.Generate();
		
		m_pSurface->BltSpriteOutline( &m_SOM,  m_SOMOutlineColor );
		
		//---------------------------------------- 	
		// 捞抚 免仿且 谅钎 瘤沥
		//---------------------------------------- 	
		const int FontHeight = g_pClientConfig->FONT_HEIGHT;
		const int FontHeight2 = FontHeight << 1;
		
		pointTemp.x = pPoint->x;			
		if (pointTemp.x<0) pointTemp.x=0;
		
		//---------------------------------------- 	
		// Level Name 嘛阑 困摹档 拌魂
		//---------------------------------------- 	
		if (pCreature->HasLevelName())
		{
			pointTemp.y = pPoint->y - FontHeight2;
			
			if (pointTemp.y < FontHeight2)
			{
				pointTemp.y = FontHeight2;
			}
		}
		//---------------------------------------- 	
		// 弊成 捞抚父 嘛阑 锭
		//---------------------------------------- 	
		else
		{
			pointTemp.y = pPoint->y - FontHeight;
			
			if (pointTemp.y < FontHeight) 
			{
				pointTemp.y = FontHeight;
			}
		}
		
		m_pointChatString	= pointTemp;
		m_pSelectedCreature = pCreature;
	}
	//----------------------------------------
	// 弊成 免仿
	//----------------------------------------
	else
	{
		m_pSurface->BltSprite(&pointTemp, pSprite);
	}
	
	//----------------------------------------
	// 个俊 嘿篮 Effect 免仿
	//----------------------------------------
	/*
	if (pCreature->IsExistAttachEffect())
	{				
	pointTemp = *pPoint;
	//pointTemp.x += pCreature->GetSX();
	//pointTemp.y += pCreature->GetSY();
	
	  DrawAttachEffect(&pointTemp, pCreature->GetAttachEffectIterator(), pCreature->GetAttachEffectSize());
	  }
	*/	
}

void		
MTopView::DrawCreatureMyName()
{	
	// EFFECTSTATUS_GHOST 俊 吧妨乐栏搁 巢档 给焊绊 唱档 给夯促
	if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_GHOST))return;

	const char* pCreatureName;
	pCreatureName = g_pUserInformation->CharacterID.GetString();
	bool bMasterWords = false;	
	COLORREF	color;
	FONTID		font = FONTID_USER_ID;

	if (pCreatureName!=NULL)
	{
		int alignment = g_char_slot_ingame.alignment;
		int POSITION_HP_BAR = g_pClientConfig->POSITION_HP_BAR;

		color = m_ColorNameAlignment[alignment];
		POINT TempPoint = {g_pPlayer->GetPixelX() - m_FirstZonePixel.x , g_pPlayer->GetPixelY() - g_pPlayer->GetHeight() - m_FirstZonePixel.y};
		const int firstY = DrawChatString(&TempPoint, g_pPlayer, g_pPlayer->GetChatColor());//CDirectDraw::Color(26,26,26));		

		int yPoint = firstY/*g_pPlayer->GetPixelY() - g_pPlayer->GetHeight() - m_FirstZonePixel.y*/;
		int rectLeft, rectRight, rectTop, rectBottom;
		int gradeID = 0;
		int guildID = 0;

		int yPoint_20 = yPoint - 23;	// g_pClientConfig->FONT_HEIGHT
		
		int namePixel = g_GetStringWidth(pCreatureName, g_ClientPrintInfo[font]->hfont);
		int MAX_HP_BAR = max(100, namePixel + 20);
		

		//-----------------------------------------------------
		// 捞抚 关俊 彬副 傈眉(捞抚) 农扁狼 冠胶
		//-----------------------------------------------------
		{
			rectLeft	= g_pPlayer->GetPixelX() - m_FirstZonePixel.x + POSITION_HP_BAR - 14;
			rectRight	= rectLeft + MAX_HP_BAR;//m_pointChatString.x+5 + namePixel;
			rectTop		= yPoint_20;
			rectBottom	= yPoint_20+ g_pClientConfig->FONT_HEIGHT;
		}
		//-----------------------------------------------------
		// Hallu俊 吧府搁 辨靛付农唱 拌鞭付农 救嘛澜
		//-----------------------------------------------------
		if(!g_pPlayer->HasEffectStatus(EFFECTSTATUS_HALLUCINATION))
		{
			//-----------------------------------------------------
			// Guild啊 乐绰 版快 - Guild Mark 免仿
			//-----------------------------------------------------
			guildID = g_pPlayer->GetGuildNumber();

			gradeID = g_pPlayer->GetGrade()-1;	// 1~50 栏肺 登乐栏骨肺 -1秦辑 0~49肺 嘎冕促
			if(!g_pPlayer->IsPlayerOnly())
				gradeID = -1;
			
			if (guildID > 0)
			{
				//-------------------------------------------------
				// load登绢 乐绰瘤 夯促.
				//-------------------------------------------------
				CSprite* pSprite = g_pGuildMarkManager->GetGuildMarkSmall(guildID);
				
				if (pSprite!=NULL)
				{			
					m_pSurface->Lock();
					
					POINT pointTemp = { rectLeft-20, yPoint_20 };
					
					m_pSurface->BltSprite(&pointTemp, pSprite);				
					
					m_pSurface->Unlock();
				}
				else
				{
					if (!g_pGuildMarkManager->HasGuildMark(guildID))
					{
						g_pGuildMarkManager->LoadGuildMark(guildID);
						pSprite = g_pGuildMarkManager->GetGuildMark(guildID);
						
					}				
				}
			}
			
			// 拌鞭捞 乐绰版快 拌鞭付农 免仿
			if(gradeID > -1 && gradeID <= GRADE_MARK_MAX)
			{
				CSprite* pSprite = g_pGuildMarkManager->GetGradeMarkSmall(gradeID, g_pPlayer->GetRace());
				
				if (pSprite!=NULL)
				{			
					m_pSurface->Lock();
					
					POINT pointTemp = { rectRight, yPoint_20 };
					
					m_pSurface->BltSprite(&pointTemp, pSprite);				
					
					m_pSurface->Unlock();
				}
			}
			
		}
		//-----------------------------------------------------
		// HP拌魂
		//-----------------------------------------------------
		int currentHP		= max(0,int(g_pPlayer->GetHP()));
		int maxHP			= g_pPlayer->GetMAX_HP();

		#ifdef OUTPUT_DEBUG
			if (g_pDXInput->KeyDown(DIK_H) && 
				(g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL)))
			{
				char str[128];
				sprintf(str, "HP=%d/%d", currentHP, maxHP);
				m_pSurface->GDI_Text(rectLeft, rectTop-20, str, 0xFFFF);
			}					
		#endif

		if (currentHP > maxHP)
		{
			currentHP = maxHP;
		}

		//-----------------------------------------------------
		// 角力肺 焊咯龙 pixel拌魂
		//-----------------------------------------------------
		int maxPixels		= rectRight - rectLeft;
		int currentPixels	= max(0,int((maxHP==0)? 0 : maxPixels * currentHP / maxHP));

		int nameX = //rectLeft+(MAX_HP_BAR>>1) - (namePixel>>1);
					rectLeft + ((MAX_HP_BAR - namePixel)>>1);

		
		//-----------------------------------------------------
		// HP啊 菜 瞒霸 钎矫登绰 版快
		//-----------------------------------------------------
		if (currentPixels==maxPixels)
		{
			// 官帕祸 彬 鞘夸 绝瘤父.. 捞抚 肋 焊捞霸 且妨绊.. --
			DRAWTEXT_NODE* pNodeBase = new DRAWTEXT_NODE (
										nameX+1,
										yPoint_20+4+1,
										NULL,//pName,
										0,
										font);

			AddText( pNodeBase );
		}
		//-----------------------------------------------------
		// HP 粹篮 何盒 钎矫
		//-----------------------------------------------------
		else
		{
			//-----------------------------------------------------
			//
			//			Max HP 父怒狼 Box父 钎矫
			//
			//-----------------------------------------------------
			DRAWTEXT_NODE* pNodeBase = new DRAWTEXT_NODE (
										nameX+1,
										yPoint_20+4+1,
										NULL,//pName,
										0,
										font);

			//-----------------------------------------------------
			// HP 粹篮 何盒
			//-----------------------------------------------------
			RECT rect = {	rectLeft + currentPixels, 
							rectTop,
							rectRight,
							rectBottom 
			};


			pNodeBase->SetBox( rect, m_ColorBlackHalf );
			AddText( pNodeBase );
		}

		//-----------------------------------------------------
		//
		//		泅犁 HP父怒狼 Box客 某腐磐 捞抚 免仿
		//
		//-----------------------------------------------------
		// 泅犁 HP父怒狼 Box客 某腐磐 捞抚 免仿(鳖改霸)
		//-----------------------------------------------------			
		DRAWTEXT_NODE* pNode = new DRAWTEXT_NODE (
									nameX + 1,
									yPoint_20+4 + 1,
									pCreatureName,
									0,		//color,
									font);	//FLAG_DRAWTEXT_HANGUL_JOHAP);

		//-----------------------------------------------------
		// 0捞 酒聪搁 box免仿
		//-----------------------------------------------------							
		if (currentPixels!=0)				
		{
			//-----------------------------------------------------
			// 泅犁 HP俊 措茄 冠胶
			//-----------------------------------------------------
			RECT rectHP = {	rectLeft, 
							rectTop,
							rectLeft + currentPixels,
							rectBottom
			};

			//-----------------------------------------------------
			// Current HP
			//-----------------------------------------------------				
			if (g_pUserOption->DrawTransHPBar)
			{
				pNode->SetBox( rectHP, m_ColorHPBar, true );
			}
			else
			{
				// 阂捧疙茄 HP bar甫 免仿且锭
				static WORD notTransHPBar 
					= CDirectDraw::Color(g_pClientConfig->COLOR_HP_BAR_R>>1,
											g_pClientConfig->COLOR_HP_BAR_G>>1,
											g_pClientConfig->COLOR_HP_BAR_B>>1);

				pNode->SetBox( rectHP, notTransHPBar, false );
			}			
		}

		//-----------------------------------------------------
		// 免仿窍霸 汲沥..
		//-----------------------------------------------------
		AddText( pNode );			


		//-----------------------------------------------------
		// 捞抚 促矫 嘛扁
		//-----------------------------------------------------
		DRAWTEXT_NODE* pNode2 = new DRAWTEXT_NODE (
									nameX,
									yPoint_20+4,
									pCreatureName,
									color,
									font);	//FLAG_DRAWTEXT_HANGUL_JOHAP);

		AddText( pNode2 );
		// 2004, 6, 17, sobeit add start - about nick name
		if(g_pPlayer->GetNickNameType() != NicknameInfo::NICK_NONE)
		{
			BYTE bType = g_pPlayer->GetNickNameType();
			const char* szNickName = g_pPlayer->GetNickName().c_str();
			if(strlen(szNickName)>0)
			{
				namePixel = g_GetStringWidth(szNickName, g_ClientPrintInfo[font]->hfont);
				WORD bgColor = 0;
				
				switch(bType)
				{
				case NicknameInfo::NICK_BUILT_IN:
					color = RGB_YELLOW;
					bgColor = CDirectDraw::Color(255,0,0);

					break;
				case NicknameInfo::NICK_QUEST:
					color = RGB(150,150,150);
					bgColor = CDirectDraw::Color(0,0,255);
					break;
				case NicknameInfo::NICK_FORCED:
				case NicknameInfo::NICK_CUSTOM_FORCED:
					color = RGB_RED;
					break;
				case NicknameInfo::NICK_CUSTOM:
					color = RGB(50,170,230);
				//	bgColor = CDirectDraw::Color(255,0,0);
					break;
				default:
					color = RGB_YELLOW;
					break;
				}
			//	if(guildID >0) // 辨靛付农啊 救嘛鳃订 葱匙烙阑 hpbar+拌鞭付农 吝居俊 嘛绊
			//		nameX = rectLeft + ((MAX_HP_BAR - namePixel+20)>>1);
			//	else // 弊寇浚 hpbar 吝居俊 嘛绰促
					nameX = rectLeft + ((MAX_HP_BAR - namePixel)>>1);

				DRAWTEXT_NODE* pNode = new DRAWTEXT_NODE (
												nameX ,
												yPoint-38,
												szNickName,
												color,		//color,
												font);	//FLAG_DRAWTEXT_HANGUL_JOHAP);
				if(bgColor != 0)
				{
//					RECT rectHP = {	nameX-2, 
//									yPoint-42,
//									nameX + namePixel+4,
//									yPoint-23	};

					RECT rectHP = {	rectLeft -20, 
									yPoint-42,
									rectRight + 20 ,
									yPoint-23	};

					if (g_pUserOption->DrawTransHPBar)
					{
						pNode->SetBox( rectHP, bgColor, true );
					}
					else
					{
						pNode->SetBox( rectHP, bgColor, false );
					}			
				}	
				AddText( pNode );			
			}

		}
		// 2004, 6, 17, sobeit add end - about nick name
	}
	
}
bool
MTopView::DrawEvent()
{
	bool bDrawBackGround = true;
	MEvent *AdvancementQuestEndingEvent = const_cast<MEvent *>(g_pEventManager->GetEvent(EVENTID_ADVANCEMENT_QUEST_ENDING));
	if(AdvancementQuestEndingEvent!= NULL)
	{
		bDrawBackGround = ExcuteAdvancementQuestEnding((void *)AdvancementQuestEndingEvent);
	}
	else
		bDrawBackGround = ExcuteOustersFinEvent();
	
	// 2005, 2, 18, sobeit add start - WebBrowser啊 栋乐阑 锭 贸府
	if(UI_IsRunning_WebBrowser())
	{
		//bDrawBackGround = false; // 硅版阑 嘛瘤 臼绰促. UI绰 嘛绰促..
		int TempValue = UI_GetMouseCursorInfo_WebBrowser();

		if(TempValue == 1) // 目辑啊 昆 拳搁 救栏肺 甸绢咳
		{
			UI_ShowWindowCursor(); // 目辑 焊咯林厘..
		}
		else if( TempValue == -1) // 目辑啊 昆 拳搁 观栏肺 唱艾促.
		{
			UI_HiddenWindowCursor(); // 目辑 救焊咯霖促.
		}
//		if(UI_IsMouseIn_WebBrowser())
//		{
//			
//			CURSORINFO _CursorInfo;
//			GetCursorInfo(&_CursorInfo);
//			if(_CursorInfo.flags != CURSOR_SHOWING)
//				ShowCursor( TRUE );
//		}
//		else
//		{
//			CURSORINFO _CursorInfo;
//			GetCursorInfo(&_CursorInfo);
//			if(_CursorInfo.flags == CURSOR_SHOWING)
//				ShowCursor( FALSE );
//		}
			
	// 2005, 2, 18, sobeit add end	
	}
	return bDrawBackGround;
}

bool
MTopView::ExcuteAdvancementQuestEnding(void *pVoid)
{
	MEvent *AdvancementQuestEndingEvent = (MEvent*)pVoid;

	if(AdvancementQuestEndingEvent!= NULL)
	{
		bool bFinEnd = false;

		int SpkIndex = AdvancementQuestEndingEvent->parameter4;
		if(m_AdvacementQuestEnding.GetSize() == 0)
		{
			class ifstream	FinFile;
			if (!FileOpenBinary(g_pFileDef->getProperty("FILE_SPRITE_ADVANCEMENT_QUEST").c_str(), FinFile))
				return false;
			m_AdvacementQuestEnding.LoadFromFile(FinFile);
			FinFile.close();
		}
		const TYPE_SOUNDID soundID = SOUND_SLAYER_ENCHANT_B2;

		static BYTE fadeColor[3] = {255, 255, 255};
		static int fadeSpeed = 10;
		static int fadeDirect = 0;
		POINT pointZero = { 0, 0 };
		
		if(AdvancementQuestEndingEvent->parameter1 == 0)
		{
			fadeColor[0] = 255;
			fadeColor[1] = 255;
			fadeColor[2] = 255;
			fadeSpeed = 10;
			fadeDirect = 0;
			
			//AdvancementQuestEndingEvent->parameter1 = g_pPlayer->IsSlayer()?1000:2000;
			AdvancementQuestEndingEvent->parameter1 = 9998;
			AdvancementQuestEndingEvent->parameter2 = 0;
			AdvancementQuestEndingEvent->parameter3 = 1000;
//				g_pPlayer->SetChatString((*g_pGameStringTable)[g_pPlayer->IsSlayer()?STRING_MESSAGE_RIPATY_SCRIPT_1:STRING_MESSAGE_AMATA_SCRIPT_1].GetString());
//
//				g_pMP3->Stop();
//				g_pMP3->Open( "Data\\Music\\blood.wav" );


			if( g_DXSound.IsInit() )
			{
				if( g_pOGG != NULL )
					g_pOGG->streamClose();
				
				if( g_pSoundBufferForOGG == NULL )
					g_pSoundBufferForOGG = new CDirectSoundBuffer(g_hWnd, SOUND_STEREO, SOUND_44K, SOUND_16BIT);
				
				if( g_pOGG == NULL )
#ifdef _MT
					g_pOGG = new COGGSTREAM(g_hWnd, g_pSoundBufferForOGG, 44100, 11025, 8800);
#else
					g_pOGG = new COGGSTREAM(g_hWnd, g_pSoundBufferForOGG, 44100, 11025, 8800,1);
#endif
				
				if( g_oggfile != NULL)
					fclose(g_oggfile );
				g_oggfile = fopen("data\\music\\Silence_of_Battlefield.ogg","rb");
				g_pOGG->streamLoad(g_oggfile, NULL);
				g_pOGG->streamPlay(SOUND_PLAY_REPEAT);
				int volume = (g_pUserOption->VolumeMusic - 15) * 250;
				g_pOGG->streamVolume( max( -10000, min( -1, volume ) ) );
			}
			PlaySoundForce(soundID);
			AdvancementQuestEndingEvent->eventStartTickCount = GetTickCount();

			
//			g_pBack->Lock();
//			g_pBack->BltSprite( &pointZero, &m_AdvacementQuestEnding[SpkIndex] );
//			g_pBack->Unlock();
		}
//
		m_pSurface->Lock();
		if(AdvancementQuestEndingEvent->parameter3 > 32)
		{
			m_pSurface->BltSprite( &pointZero, &m_AdvacementQuestEnding[SpkIndex+1] );
		}
		else if(AdvancementQuestEndingEvent->parameter3 == 0)
		{
			m_pSurface->BltSprite( &pointZero, &m_AdvacementQuestEnding[SpkIndex] );
		}
		else
		{
			m_pSurface->BltSprite( &pointZero, &m_AdvacementQuestEnding[SpkIndex] );
			m_pSurface->BltSpriteAlpha( &pointZero, &m_AdvacementQuestEnding[SpkIndex+1], AdvancementQuestEndingEvent->parameter3 );
		}
		m_pSurface->Unlock();
		if(AdvancementQuestEndingEvent->parameter3>0 && g_FrameCount & 0x01)
			AdvancementQuestEndingEvent->parameter3 --;
		switch(AdvancementQuestEndingEvent->parameter1)
		{
		case 9998:
			if(m_pSurface->Lock())
			{
				int scroll_progress = -500+(GetTickCount()-AdvancementQuestEndingEvent->eventStartTickCount)/66;
				
				const int scroll_x = g_GameRect.right/2-m_AdvacementQuestEnding[0].GetWidth()/2, scroll_y = 50;
				
				int scroll = ( (scroll_progress < 0)? 0 : scroll_progress );
				int scroll2 = ( (scroll_progress < 0)? scroll_progress : scroll_progress );
				int scroll3 = ( (scroll_progress < 0)? 500+scroll_progress : 500 );
				
				Rect rect(0, scroll, m_AdvacementQuestEnding[0].GetWidth(), min(scroll3, m_AdvacementQuestEnding[0].GetHeight() - scroll));
				S_SURFACEINFO surface_info;
				SetSurfaceInfo(&surface_info, gpC_base->m_p_DDSurface_back->GetDDSD());
				
				int x = scroll_x, y = scroll_y-scroll2;
				RECT rt;
				rt.left = max(-x, rect.x);
				rt.top = max(-y, rect.y);
				rt.right = min(rect.x+rect.w, g_GameRect.right-x);
				rt.bottom = min(rect.y+rect.h, g_GameRect.bottom-y);
				
				if(rt.bottom < rt.top)
				{
					AdvancementQuestEndingEvent->parameter1 = 9999;
				}
				
				if(rt.left < rt.right && rt.top < rt.bottom)
				{
					WORD * p_dest = (WORD *)surface_info.p_surface+x+rt.left;
					p_dest = (WORD *)((BYTE *)p_dest+(y+rt.top)*surface_info.pitch);
					
					//void BltClip(WORD *pDest, WORD pitch, RECT* pRect); // in CSprite.h
					m_AdvacementQuestEnding[0].BltClipWidth(p_dest, surface_info.pitch, &rt);
				}
				m_pSurface->Unlock();
			}
			break;

		case 9999:
			bFinEnd = true;
			break;
		}
		
		RECT	rect;
		rect.left = 0;
		rect.right = g_GameRect.right;
		rect.top = 0;
		rect.bottom = g_GameRect.bottom;
		
		DrawAlphaBox(&rect, fadeColor[0], fadeColor[1], fadeColor[2], min(31, (GetTickCount() - AdvancementQuestEndingEvent->eventStartTickCount) /fadeSpeed)^fadeDirect);	

		if(bFinEnd)
		{
//				char szTemp[512];
//				sprintf(szTemp, "%d", (GetTickCount()-AdvancementQuestEndingEvent->eventStartTickCount)/1000);
//				UI_AddChatToHistory( szTemp, szTemp, 0, RGB(255, 255, 255) );
			UI_REQUEST_DIE_TIMER_RESET();
			g_pEventManager->RemoveEvent(EVENTID_ADVANCEMENT_QUEST_ENDING);
			m_AdvacementQuestEnding.Release();
		}
		return false;
	}
	return true;
}
bool 
MTopView::ExcuteOustersFinEvent()
{
	bool bDrawBackGround = true;
	const MEvent *event = g_pEventManager->GetEventByFlag(EVENTFLAG_ONLY_EVENT_BACKGROUND);
	if(event != NULL)
	{
		if((event->eventFlag & EVENTFLAG_ONLY_EVENT_BACKGROUND) == EVENTFLAG_NOT_DRAW_BACKGROUND)
		{
			m_pSurface->FillSurface(0);	//	拳搁瘤快扁
		}
		else if(event->parameter4 < EVENTBACKGROUNDID_MAX)
		{
//			AssertEventBackground(event->parameter4);
			
			POINT p = { 0, 0 };
			RECT r = {0, 0, g_GameRect.left, g_GameRect.top };
//			if(!m_pSurface->Lock()) return;

			CDirectDrawSurface *pSurface = g_pEventManager->GetEventBackground((EVENTBACKGROUND_ID)event->parameter4);

			m_pSurface->BltNoColorkey(&p, pSurface, &r);
				
//			m_pSurface->BltSprite(&p, g_pEventManager->GetEventBackground(event->parameter4));

//			m_pSurface->Unlock();
		}

		if(event->eventFlag & EVENTFLAG_ONLY_EVENT_BACKGROUND)
			bDrawBackGround = false;
	}

	MEvent *OustersFinEvent = const_cast<MEvent *>(g_pEventManager->GetEvent(EVENTID_OUSTERS_FIN));

	if(OustersFinEvent != NULL)
	{
		bool bFinEnd = false;

		if(m_OustersFinSPK.GetSize() == 0)
		{
			class ifstream	FinFile;
			if (!FileOpenBinary(g_pFileDef->getProperty("FILE_SPRITE_OUSTERS_FIN").c_str(), FinFile))
				return bDrawBackGround;
			m_OustersFinSPK.LoadFromFile(FinFile);
			FinFile.close();
		}

		const TYPE_SOUNDID soundID = SOUND_SLAYER_ENCHANT_B2;

		static BYTE fadeColor[3] = {255, 255, 255};
		static int fadeSpeed = 10;
		static int fadeDirect = 0;
		
		if(OustersFinEvent->parameter1 == 0)
		{
			fadeColor[0] = 255;
			fadeColor[1] = 255;
			fadeColor[2] = 255;
			fadeSpeed = 10;
			fadeDirect = 0;
			
			//OustersFinEvent->parameter1 = g_pPlayer->IsSlayer()?1000:2000;
			OustersFinEvent->parameter1 = 9998;
			OustersFinEvent->parameter2 = 0;
//				g_pPlayer->SetChatString((*g_pGameStringTable)[g_pPlayer->IsSlayer()?STRING_MESSAGE_RIPATY_SCRIPT_1:STRING_MESSAGE_AMATA_SCRIPT_1].GetString());
//
//				g_pMP3->Stop();
//				g_pMP3->Open( "Data\\Music\\blood.wav" );


			if( g_DXSound.IsInit() )
			{
				if( g_pOGG != NULL )
					g_pOGG->streamClose();
				
				if( g_pSoundBufferForOGG == NULL )
					g_pSoundBufferForOGG = new CDirectSoundBuffer(g_hWnd, SOUND_STEREO, SOUND_44K, SOUND_16BIT);
				
				if( g_pOGG == NULL )
#ifdef _MT
					g_pOGG = new COGGSTREAM(g_hWnd, g_pSoundBufferForOGG, 44100, 11025, 8800);
#else
					g_pOGG = new COGGSTREAM(g_hWnd, g_pSoundBufferForOGG, 44100, 11025, 8800,1);
#endif
				
				if( g_oggfile != NULL)
					fclose(g_oggfile );
				g_oggfile = fopen("data\\music\\chaos.ogg","rb");
				g_pOGG->streamLoad(g_oggfile, NULL);
				g_pOGG->streamPlay(SOUND_PLAY_REPEAT);
				int volume = (g_pUserOption->VolumeMusic - 15) * 250;
				g_pOGG->streamVolume( max( -10000, min( -1, volume ) ) );
			}
			PlaySoundForce(soundID);
			OustersFinEvent->eventStartTickCount = GetTickCount();
		}

		switch(OustersFinEvent->parameter1)
		{
		case 1000:
			{
				if((GetTickCount() - OustersFinEvent->eventStartTickCount)/4000 > 0)
				{
					OustersFinEvent->parameter2++;
					if(OustersFinEvent->parameter2 > 8)
					{
						g_pPlayer->ClearChatString();

						if(OustersFinEvent->parameter2 == 9)
						{
							g_pPlayer->SetDead();
							PlaySoundForce(g_pPlayer->IsFemale()?SOUND_SLAYER_DIE_FEMALE:SOUND_SLAYER_DIE_MALE);
							fadeSpeed = 32*4;
							fadeColor[0] = 255;
							fadeColor[1] = 0;
							fadeColor[2] = 0;
						}
						
						if(OustersFinEvent->parameter2 > 9)
						{
							fadeColor[0] = 0;
							fadeColor[1] = 0;
							fadeColor[2] = 0;
							fadeDirect = 31;

							if(OustersFinEvent->parameter2 > 10)
							{
								OustersFinEvent->eventStartTickCount = GetTickCount();
								OustersFinEvent->parameter1 = 9998;
								OustersFinEvent->parameter4 = EVENTBACKGROUNDID_OUSTERS_SLAYER;
								g_pMP3->Play( false );
								fadeDirect = 0;
							}
						}
					}
					else
					{
						PlaySoundForce(soundID);
						g_pPlayer->SetChatString((*g_pGameStringTable)[STRING_MESSAGE_RIPATY_SCRIPT_1+OustersFinEvent->parameter2].GetString());
					}

					if(OustersFinEvent->parameter2 < 11)
						OustersFinEvent->eventStartTickCount = GetTickCount();
				}
				if(m_pSurface->Lock())
				{
					const int char_plus_x = 10, char_plus_y = 24;
					
					POINT p[8] = 
					{
						{ 400 - m_OustersFinSPK[3].GetWidth()/2+char_plus_x, 160-m_OustersFinSPK[3].GetHeight()+char_plus_y},
						{ 300 - m_OustersFinSPK[4].GetWidth()/2+char_plus_x, 210-m_OustersFinSPK[4].GetHeight()+char_plus_y},
						{ 240 - m_OustersFinSPK[5].GetWidth()/2+char_plus_x, 300-m_OustersFinSPK[5].GetHeight()+char_plus_y},
						{ 300 - m_OustersFinSPK[6].GetWidth()/2+char_plus_x, 390-m_OustersFinSPK[6].GetHeight()+char_plus_y},
						{ 400 - m_OustersFinSPK[7].GetWidth()/2+char_plus_x, 440-m_OustersFinSPK[7].GetHeight()+char_plus_y},
						{ 500 - m_OustersFinSPK[8].GetWidth()/2+char_plus_x, 390-m_OustersFinSPK[8].GetHeight()+char_plus_y},
						{ 560 - m_OustersFinSPK[9].GetWidth()/2+char_plus_x, 300-m_OustersFinSPK[9].GetHeight()+char_plus_y},
						{ 500 - m_OustersFinSPK[10].GetWidth()/2+char_plus_x, 210-m_OustersFinSPK[10].GetHeight()+char_plus_y},
					};
					
					m_pSurface->BltSprite(&p[0], &m_OustersFinSPK[3]);
					POINT strPoint = p[0];
					strPoint.x -= 15;
					strPoint.y -= 15;
					DrawChatString(&strPoint, g_pPlayer, RGB(255, 255, 255));
					
					if(OustersFinEvent->parameter2 > 0)
					{
						for(int i = 0; i < 7; i++)
						{
							m_pSurface->BltSprite(&p[i+1], &m_OustersFinSPK[4+i]);
						}
					}
					
					MCreature *pCreature = g_pPlayer;
					
					int action = pCreature->GetAction(), direction = 6, frame = pCreature->GetFrame();
					
					MCreatureWear*	pCreatureWear = (MCreatureWear*)pCreature;
					
					// Creature狼 Action俊 嘎绰 add-on阑 免仿茄促.
					//action = pCreature->GetAction();
					
					WORD clothes;
					BYTE clothesType;

					for (int i=0; i<ADDON_MAX; i++)
					{
						// Creature狼 泅犁 规氢俊 蝶扼辑...
						// 渴阑 免仿秦林绰 鉴辑啊 促甫 荐 乐促.
						clothesType = MCreatureWear::s_AddonOrder[direction][i];
						
						// i锅掳 辆幅狼 渴阑 涝绊 乐促搁 免仿秦 霖促.
						const MCreatureWear::ADDON_INFO& addonInfo = pCreatureWear->GetAddonInfo(clothesType);
						
						if (addonInfo.bAddon)
						{
							clothes = addonInfo.FrameID;
							
							FRAME_ARRAY &FA = m_AddonFPK[clothes][action][direction];
							
							// 乐绰 悼累牢 版快
							if (FA.GetSize() > frame)
							{
								CFrame &Frame = FA[frame];					
								int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
								int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
								int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
								
								
								CIndexSprite* pSprite = &m_AddonSPK[ sprite ];					
								
								// 汗厘Sprite啊 檬扁拳 登瘤 臼篮 版快
//									if (pSprite->IsNotInit())
//									{
//										LoadFromFileAddonSPK( clothes, action );
//									}
								
								
								POINT pointTemp;
								
								// 谅钎 焊沥
								pointTemp.x = 384+cx;
								pointTemp.y = 312+cy;
								
								{
									int colorSet1 = addonInfo.ColorSet1, colorSet2 = addonInfo.ColorSet2;
									if(colorSet2 == UNIQUE_ITEM_COLOR || colorSet2 == QUEST_ITEM_COLOR)
									{
										// 蜡聪农 酒捞袍捞搁
										//									if(colorSet2 == QUEST_ITEM_COLOR)
										//										colorSet2 = MItem::GetQuestItemColorset();
										//									else
										//										colorSet2 = MItem::GetUniqueItemColorset();										
										
										colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
										if(addonInfo.ItemClass != ITEM_CLASS_COAT && addonInfo.ItemClass != ITEM_CLASS_TROUSER)
											colorSet1 = colorSet2;
									}
									
									CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
									
									// 绢涤霸 嘛扁
									if (pCreature->IsFade())
									{
										m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 1);
									}
									else
									{
										m_pSurface->BltIndexSprite(&pointTemp, pSprite);
									}
								}						
								
								// 沥惑利牢 免仿
								//CIndexSprite::SetUsingColorSet( addonInfo.ColorSet1, addonInfo.ColorSet2 );
								//m_pSurface->BltIndexSprite(&pointTemp, pSprite);
								
							}
						}
						
					}
					
					m_pSurface->Unlock();
				}
			}
			break;
			
		case 2000:
			{
				if((GetTickCount() - OustersFinEvent->eventStartTickCount)/4000 > 0)
				{
					OustersFinEvent->parameter2++;

					if(OustersFinEvent->parameter2 > 8)
					{
						g_pPlayer->ClearChatString();

						if(OustersFinEvent->parameter2 == 9)
						{
							g_pPlayer->SetDead();
							PlaySoundForce(g_pPlayer->IsFemale()?SOUND_VAMPIRE_DIE_FEMALE:SOUND_VAMPIRE_DIE_MALE);
							fadeSpeed = 32*4;
							fadeColor[0] = 255;
							fadeColor[1] = 0;
							fadeColor[2] = 0;
						}

						if(OustersFinEvent->parameter2 > 9)
						{
							fadeColor[0] = 0;
							fadeColor[1] = 0;
							fadeColor[2] = 0;
							fadeDirect = 31;
							
							if(OustersFinEvent->parameter2 > 10)
							{
								OustersFinEvent->eventStartTickCount = GetTickCount();
								OustersFinEvent->parameter1 = 9998;
								OustersFinEvent->parameter4 = EVENTBACKGROUNDID_OUSTERS_VAMPIRE;
								g_pMP3->Play( false );
								fadeDirect = 0;
							}
						}
					}
					else
					{
						PlaySoundForce(soundID);
						g_pPlayer->SetChatString((*g_pGameStringTable)[STRING_MESSAGE_AMATA_SCRIPT_1+OustersFinEvent->parameter2].GetString());
					}

					if(OustersFinEvent->parameter2 < 11)
						OustersFinEvent->eventStartTickCount = GetTickCount();
				}

				if(m_pSurface->Lock())
				{
					const int char_plus_x = 10, char_plus_y = 24;

					POINT p[8] = 
					{
						{ 400 - m_OustersFinSPK[2].GetWidth()/2+char_plus_x, 160-m_OustersFinSPK[2].GetHeight()+char_plus_y},
						{ 300 - m_OustersFinSPK[4].GetWidth()/2+char_plus_x, 210-m_OustersFinSPK[4].GetHeight()+char_plus_y},
						{ 240 - m_OustersFinSPK[5].GetWidth()/2+char_plus_x, 300-m_OustersFinSPK[5].GetHeight()+char_plus_y},
						{ 300 - m_OustersFinSPK[6].GetWidth()/2+char_plus_x, 390-m_OustersFinSPK[6].GetHeight()+char_plus_y},
						{ 400 - m_OustersFinSPK[7].GetWidth()/2+char_plus_x, 440-m_OustersFinSPK[7].GetHeight()+char_plus_y},
						{ 500 - m_OustersFinSPK[8].GetWidth()/2+char_plus_x, 390-m_OustersFinSPK[8].GetHeight()+char_plus_y},
						{ 560 - m_OustersFinSPK[9].GetWidth()/2+char_plus_x, 300-m_OustersFinSPK[9].GetHeight()+char_plus_y},
						{ 500 - m_OustersFinSPK[10].GetWidth()/2+char_plus_x, 210-m_OustersFinSPK[10].GetHeight()+char_plus_y},
					};

					m_pSurface->BltSprite(&p[0], &m_OustersFinSPK[2]);
					POINT strPoint = p[0];
					strPoint.x -= 15;
					strPoint.y -= 15;
					DrawChatString(&strPoint, g_pPlayer, RGB(255, 255, 255));

					if(OustersFinEvent->parameter2 > 0)
					{
						for(int i = 0; i < 7; i++)
						{
							m_pSurface->BltSprite(&p[i+1], &m_OustersFinSPK[4+i]);
						}
					}

					int body, action, direction, frame, creature_type;
				
					MCreature *pCreature = g_pPlayer;


					body		= pCreature->GetCreatureFrameID(0);
					action		= pCreature->GetAction();
					direction	= 6;//pCreature->GetDirection();
					frame		= pCreature->GetFrame();//%m_CreatureFPK[body][action][direction].GetCount();
					creature_type = pCreature->GetCreatureType();

					// vampire
					FRAME_ARRAY& FA = m_CreatureFPK[body][action][direction];
					
					if (FA.GetSize() > frame)
					{
						CFrame& Frame =	FA[frame];
						int sprite = Frame.GetSpriteID(),	//m_CreatureFPK[body][action][direction][frame].GetSpriteID(),
							cx		= Frame.GetCX(),	//m_CreatureFPK[body][action][direction][frame].GetCX(),
							cy		= Frame.GetCY();	//m_CreatureFPK[body][action][direction][frame].GetCY();
						
						CIndexSprite* pSprite = &m_CreatureSPK[ sprite ];
						
//							if (pSprite->IsNotInit())
//							{
//								LoadFromFileCreatureActionSPK( body, action );
//							}

						int colorSet1 = pCreature->GetBodyColor1();
						int colorSet2 = pCreature->GetBodyColor2();
						if( colorSet1 == QUEST_ITEM_COLOR || colorSet1 == UNIQUE_ITEM_COLOR )
							colorSet1 = MItem::GetSpecialColorItemColorset( colorSet1 );
						
						if( colorSet2 == QUEST_ITEM_COLOR || colorSet2 == UNIQUE_ITEM_COLOR )
							colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );

						POINT pointTemp;
						
						// 谅钎 焊沥
						pointTemp.x = 384+cx;
						pointTemp.y = 312+cy;

						CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
						m_pSurface->BltIndexSprite(&pointTemp, pSprite);
					}

					m_pSurface->Unlock();
				}
			}
			break;

		case 9998:
			if(m_pSurface->Lock())
			{
				int scroll_progress = -500+(GetTickCount()-OustersFinEvent->eventStartTickCount)/66;
				
				const int spriteID = 0;
				const int scroll_x = g_GameRect.right/2-m_OustersFinSPK[spriteID].GetWidth()/2, scroll_y = 50;
				
				int scroll = ( (scroll_progress < 0)? 0 : scroll_progress );
				int scroll2 = ( (scroll_progress < 0)? scroll_progress : scroll_progress );
				int scroll3 = ( (scroll_progress < 0)? 500+scroll_progress : 500 );
				
				Rect rect(0, scroll, m_OustersFinSPK[spriteID].GetWidth(), min(scroll3, m_OustersFinSPK[spriteID].GetHeight() - scroll));
				S_SURFACEINFO surface_info;
				SetSurfaceInfo(&surface_info, gpC_base->m_p_DDSurface_back->GetDDSD());
				
				int x = scroll_x, y = scroll_y-scroll2;
				RECT rt;
				rt.left = max(-x, rect.x);
				rt.top = max(-y, rect.y);
				rt.right = min(rect.x+rect.w, g_GameRect.right-x);
				rt.bottom = min(rect.y+rect.h, g_GameRect.bottom-y);
				
				if(rt.bottom < rt.top)
				{
					OustersFinEvent->parameter1 = 9999;
				}
				
				if(rt.left < rt.right && rt.top < rt.bottom)
				{
					WORD * p_dest = (WORD *)surface_info.p_surface+x+rt.left;
					p_dest = (WORD *)((BYTE *)p_dest+(y+rt.top)*surface_info.pitch);
					
					//void BltClip(WORD *pDest, WORD pitch, RECT* pRect); // in CSprite.h
					m_OustersFinSPK[spriteID].BltClipWidth(p_dest, surface_info.pitch, &rt);
				}
				
				Rect rect2(0, 0, m_OustersFinSPK[1].GetWidth(), min(scroll3 - (m_OustersFinSPK[spriteID].GetHeight() - scroll) -50, m_OustersFinSPK[1].GetHeight()));
				
				x = scroll_x +m_OustersFinSPK[spriteID].GetWidth()/2-m_OustersFinSPK[1].GetWidth()/2;
				y = scroll_y + max(200, (m_OustersFinSPK[spriteID].GetHeight() - scroll + 50) );
				rt.left = max(-x, rect2.x);
				rt.top = max(-y, rect2.y);
				rt.right = min(rect2.x+rect2.w, g_GameRect.right-x);
				rt.bottom = min(rect2.y+rect2.h, g_GameRect.bottom-y);
				
				if(rt.left < rt.right && rt.top < rt.bottom)
				{
					WORD *p_dest = (WORD *)surface_info.p_surface+x+rt.left;
					p_dest = (WORD *)((BYTE *)p_dest+(y+rt.top)*surface_info.pitch);
					
					//void BltClip(WORD *pDest, WORD pitch, rect2* prect2); // in CSprite.h
					m_OustersFinSPK[1].BltClipWidth(p_dest, surface_info.pitch, &rt);
				}
				
				m_pSurface->Unlock();
			}
			break;

		case 9999:
			bFinEnd = true;
			break;
		}

		RECT	rect;
		rect.left = 0;
		rect.right = g_GameRect.right;
		rect.top = 0;
		rect.bottom = g_GameRect.bottom;
		
		DrawAlphaBox(&rect, fadeColor[0], fadeColor[1], fadeColor[2], min(31, (GetTickCount() - OustersFinEvent->eventStartTickCount) /fadeSpeed)^fadeDirect);	

		if(bFinEnd)
		{
//				char szTemp[512];
//				sprintf(szTemp, "%d", (GetTickCount()-OustersFinEvent->eventStartTickCount)/1000);
//				UI_AddChatToHistory( szTemp, szTemp, 0, RGB(255, 255, 255) );
			UI_REQUEST_DIE_TIMER_RESET();
			g_pEventManager->RemoveEvent(EVENTID_OUSTERS_FIN);
			m_OustersFinSPK.Release();
		}

	}
	return bDrawBackGround;
}