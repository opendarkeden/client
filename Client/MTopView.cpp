//#define __3D_IMAGE_OBJECT__					// by sonee

//----------------------------------------------------------------------
// MTopView.cpp
//----------------------------------------------------------------------
// [ Effect Layer ]  - Ãâ·Â ¼ø¼­¿¡ µû¶ó¼­..
// 
//		-- Tile Ãâ·Â
//	* Tile¿¡ ¹Ù·Î ºÙÀº Effect
//
// 
//	* SectorÀÇ Effect			
//		-- °¢°¢ÀÇ Character Ãâ·Â
//	* Character¿¡ ºÙÀº AttachEffect
// 
//----------------------------------------------------------------------
#include "Client_PCH.h"
#pragma warning(disable:4786)

#ifdef PLATFORM_WINDOWS
#include "DX3D.h"
#else
#include "DX3D.h"  // Also include on non-Windows platforms for stub definitions
#include "TextSystem/TextService.h"
#endif
#include <math.h>
#include <list>
#include <stdio.h>
#include <fstream>
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
#include "DebugLog.h"
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

extern bool FileOpenBinary(const char* filename, std::ifstream& file);
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

// ÀÓ½Ã ¶«»§.. ¿ìÇìÇì
POINT g_MouseSector = { 0, 0 };		// mouse°¡ °¡¸®Å°´Â sectorÁÂÇ¥

extern int	g_x;
extern int	g_y;

extern int	g_UpdateDelay;

extern MWorkThread*	g_pLoadingThread;

int g_ShowImageObjectID = 0;

// ³Ý¸¶ºí¿ë
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
// ÇÑ filterÁÂÇ¥ÀÇ È­¸éºñÀ² °áÁ¤..
//----------------------------------------------------------------------
float MTopView::s_LightWidth	= (float)g_GameRect.right / SCREENLIGHT_WIDTH;
float MTopView::s_LightHeight	= (float)g_GameRect.bottom / SCREENLIGHT_HEIGHT;

//----------------------------------------------------------------------
//
// defines
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// ÀüÃ¼ CreatureÀÇ ±×¸² Á¾·ù¼ö
//----------------------------------------------------------------------
#define	MAX_CREATURE_BODY		3
#define	MAX_CREATURE_ADDON		4
#define	MAX_ANIMATION			2
#define	MAX_ALPHAEFFECT			10
#define	MAX_NORMALEFFECT		1

//----------------------------------------------------------------------
// Light 3DÀÇ ¼³Á¤ °ª
//-----------------------------------------------------------------------
#define	MAX_LIGHT_SETSIZE		12

//-----------------------------------------------------------------------
// ½º³ªÀÌÇÎ¿¡¼­ ¾îµÓ°Ô Ãâ·ÂÇÒ¶§ÀÇ shift°ª
//-----------------------------------------------------------------------

//----------------------------------------------------------------------
//
// FONT °ü·Ã
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
// ½Ã°£ÀÌ ¿À·¡µÈ°É ¼±ÅÃÇØ¾ß ÇÑ´Ù.		
// left < right ¿¡ ´ëÇÑ return°ª
//
// Çö ¼ø¼­¸¦ À¯ÁöÇÒ·Á¸é false¸¦
// ¼ø¼­¸¦ ¹Ù²Ü·Á¸é true¸¦ returnÇØ¾ß ÇÑ´Ù.
//
// ¾Õ¿¡ °ÍÀÌ ¸ÕÀú Ãâ·ÂµÉ °ÍÀÌ¹Ç·Î.. ½Ã°£ÀÌ ºü¸¥ °Í(ÀûÀº °Í)ÀÌ ¾Õ¿¡ ÀÖ¾î¾ß ÇÑ´Ù.
// 
// PQ´Â ÀÛÀº°É µÚ·Î º¸³½´Ù..
// ±×·¯¹Ç·Î... ½Ã°£ÀÌ ´À¸° °Í(Å«°Í)ÀÌ µÚ·Î °¡¾ßÇÑ´Ù.
bool 
TextComparison::operator () (DRAWTEXT_NODE * left, DRAWTEXT_NODE * right) const
{ 
	// ¸ÕÀú ¼³Á¤µÈ ±ÛÀÚÀÎ °æ¿ì.. OK
	// ½Ã°£ÀÌ °°À¸¸é ÀÏ´Ü µÚ¿¡ µé¾î°£´Ù. 
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

	// ¼±ÅÃµÈ °Íµé
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

	// TileRenderer (Phase 4 integration)
	m_pTileRenderer			= NULL;


	//m_SelectSector.x = 0;
	//m_SelectSector.y = 0;
	
	// ½Ã¾ß
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
//	// 2001.7.14 ÁÖ¼®Ã³¸®
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
	// AlphBox¸¦ g_pTopView->Init()Àü¿¡ »ç¿ëÇÏ±â À§ÇØ¼­.
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

	// ³Ý¸¶ºí¿ë
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
// View¿¡ ÇÊ¿äÇÑ °¢ ºÎºÐµéÀº ÃÊ±âÈ­ ½ÃÅ²´Ù.
//----------------------------------------------------------------------
bool
MTopView::Init()
{
	//------------------------------------------
	// ÀÌ¹Ì initµÆ´Ù¸é initÇÏÁö ¾Ê´Â´Ù.
	//------------------------------------------
	if (m_bInit)
	{
		return false;
	}

	//------------------------------------------
	// ÀûÀýÇÑ Texture Memory °è»ê
	//------------------------------------------
#ifdef PLATFORM_WINDOWS
	DDSCAPS2 ddsCaps2;
	DWORD dwTotal;
	DWORD dwFree;
	ZeroMemory(&ddsCaps2, sizeof(ddsCaps2)); 
	ddsCaps2.dwCaps = DDSCAPS_TEXTURE; 
	HRESULT hr = CDirectDraw::GetDD()->GetAvailableVidMem(&ddsCaps2, &dwTotal, &dwFree);

	DEBUG_ADD_FORMAT("[TextureMemory] Before Init View = %d/%d", dwFree, dwTotal);
	
	//------------------------------------------
	// ÀûÀýÇÑ effect texture°³¼ö
	//------------------------------------------
	// 1500000Á¤µµ´Â ´Ù¸¥µ¥ »ç¿ëµÈ´Ù°í º»´Ù.
	// Æò±ÕÀûÀÎ ÇÑ TextureÀÇ memory size = 65536 - -;;
	//------------------------------------------
	int freeMemory = dwFree-1500000;

	if (freeMemory < 0) freeMemory = 0;

	int num = freeMemory / 20000 / 12;   	
	
	num = max(num, 30);	// 30º¸´Ù´Â Ä¿¾ßÇÑ´Ù.
	num = min(num, 60);	// 60ÀÌ ÃÖ°í´Ù.
#else
	// Stub for macOS - use default values
	int num = 30;  // Default number of effect textures
	DEBUG_ADD("[TextureMemory] Using default value for macOS");
#endif
	
	//num = 20;

	// Àç¼³Á¤.. - -;
	//g_pClientConfig->MAX_TEXTUREPART_EFFECT				= num*3/2;
	//g_pClientConfig->MAX_TEXTUREPART_CREATURESHADOW		= num*3;
	//g_pClientConfig->MAX_TEXTUREPART_IMAGEOBJECTSHADOW	= num;
	//g_pClientConfig->MAX_TEXTUREPART_ADDON_SHADOW			= 240 + num*7;

	//------------------------------------------------------------------------
	// Ä³¸¯ÅÍ ±×¸²ÀÚ¸¦ ¹«Á¶°Ç °Ë°Ô Ãâ·ÂÇÑ´Ù°í ÇßÀ» ¶§ÀÇ °è»ê
	//------------------------------------------------------------------------
	g_pClientConfig->MAX_TEXTUREPART_EFFECT				= num*2;	//num*10;
	g_pClientConfig->MAX_TEXTUREPART_SCREENEFFECT		= num*5;		//num*10;
	g_pClientConfig->MAX_TEXTUREPART_CREATURESHADOW		= 1;//num*3;
	g_pClientConfig->MAX_TEXTUREPART_IMAGEOBJECTSHADOW	= num*2;
	g_pClientConfig->MAX_TEXTUREPART_ADDON_SHADOW		= 1;//240 + num*7;

	LOG_INFO("[MTopView::TexturePart] AlphaEffect = %d", g_pClientConfig->MAX_TEXTUREPART_EFFECT);
	LOG_INFO("[MTopView::TexturePart] ScreenEffect = %d", g_pClientConfig->MAX_TEXTUREPART_SCREENEFFECT);
	LOG_INFO("[MTopView::TexturePart] CreatureShadow = %d", g_pClientConfig->MAX_TEXTUREPART_CREATURESHADOW);
	LOG_INFO("[MTopView::TexturePart] ImageObjectShadow = %d", g_pClientConfig->MAX_TEXTUREPART_IMAGEOBJECTSHADOW);
	LOG_INFO("[MTopView::TexturePart] AddonShadow = %d", g_pClientConfig->MAX_TEXTUREPART_ADDON_SHADOW);	

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
		// ÀûÀýÇÑ Texture Memory °è»ê
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
// 2D <--> 3D ¹Ù²ð¶§ »ç¿ë..
//----------------------------------------------------------------------
bool
MTopView::InitChanges()
{
	//------------------------------------------
	// ¾ÆÁ÷ initµÇÁö ¾Ê¾Ò´Ù¸é return
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
		// Ä³¸¯ÅÍ loadingÀÌ ÇÏ³ªµµ ¾ÈµÆ´Ù°í Ã¼Å©ÇØµÐ´Ù.
		// ±×¸²ÀÚ ¶§¹®ÀÎµ¥.. ³È³È.. - -; 2001.10.6
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
	// UIÀÇ Fontµî·Ï
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
	// File ´Ý±â
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
	// Tile SurfaceÁ¦°Å
	//----------------------------------------------------------------------
	if (m_pTileSurface != NULL)
	{
		delete m_pTileSurface;
		m_pTileSurface = NULL;

		DEBUG_ADD("MTV-Rel-TileSur");
	}

	//----------------------------------------------------------------------
	// TileRenderer Á¦°Å (Phase 4 integration)
	//----------------------------------------------------------------------
	if (m_pTileRenderer != NULL)
	{
		delete m_pTileRenderer;
		m_pTileRenderer = NULL;

		DEBUG_ADD("MTV-Rel-TileRenderer");
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
//	// 2001.7.14 ÁÖ¼®Ã³¸®
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
	// list , map Á¦°Å
	//----------------------------------------------------------------------
	ClearItemNameList();
	ClearOutputCreature();	
	ClearTextList();

	DEBUG_ADD("MTV-Rel-Loaded");

	m_listLoadedCreatureSprite.Release();
	m_listLoadedMonsterSprite.Release();
	m_mapImageObject.clear();

	//----------------------------------------------------------------------
	// FPK Áö¿ì±â
	//----------------------------------------------------------------------
	DEBUG_ADD("MTV-Rel-FPK");

	m_CreatureFPK.Release();			// Creature frames
	m_AddonFPK.Release();				// Player addon frames
	m_OustersFPK.Release();				// Player addon frames
	m_ItemTileFPK.Release();			// Tile¿¡ ÀÖ´Â Item¿¡ ´ëÇÑ..
	m_ItemDropFPK.Release();			// ¹Ù´ÚÀ¸·Î ¶³¾îÁö´Â Item¿¡ ´ëÇÑ..
	m_ImageObjectFPK.Release();		// ImageObject¿¡ ´ëÇÑ frames
	m_ImageObjectShadowFPK.Release();

	// ½ÂÁø °ü·Ã fpk Áö¿ì±â
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
//	m_InteractionObjectFPK.Release();		// ImageObject¿¡ ´ëÇÑ frames

	DEBUG_ADD("MTV-Rel-EffectFPK");

	m_EffectAlphaFPK.Release();		// Effect¿¡ ´ëÇÑ frames
	m_EffectScreenFPK.Release();
	m_EffectShadowFPK.Release();		// Effect¿¡ ´ëÇÑ frames
	m_EffectNormalFPK.Release();		// Effect¿¡ ´ëÇÑ frames
		
	DEBUG_ADD("MTV-Rel-CFPK");

	m_CreatureShadowFPK.Release();			// Creature frames
	m_AddonShadowFPK.Release();			// Player addon frames	
	m_OustersShadowFPK.Release();			// Player addon frames	
		
	//------------------------------------------------------
	// SpritePack Á¦°Å
	//------------------------------------------------------		
	DEBUG_ADD("MTV-Rel-SPK");

	m_TileSPK.Release();				// Tile
	m_ImageObjectSPK.Release();		// ImageObject Spriteµé
//	m_InteractionObjectSPK.Release();		// ImageObject Spriteµé
	m_CreatureSPK.Release();			// Creature Spriteµé
	m_AddonSPK.Release();		// Spriteµé		
	m_OustersSPK.Release();
	
	DEBUG_ADD("MTV-Rel-ItemSPK");

	m_ItemTileISPK.Release();			// Spriteµé		
	m_ItemDropISPK.Release();			//
	m_ItemBrokenSPK.Release();			//
	m_ItemRealSPK.Release();			// Spriteµé				

	DEBUG_ADD("MTV-Rel-EffectSPk");
	m_EffectAlphaSPK.Release();		// AlphaSpriteµé
	m_EffectScreenSPK.Release();		// AlphaSpriteµé
	m_EffectShadowSPK.Release();		// ShadowSpriteµé
	m_EffectNormalSPK.Release();		// NormalSpriteµé
	m_EffectScreenPPK.Release();
	m_EffectAlphaPPK.Release();

	DEBUG_ADD("MTV-Rel-Other");

	m_WeatherSPK.Release();			// WeatherSpriteµé
	m_GuildSPK.Release();

	m_EtcSPK.Release();				// ±âÅ¸...
	m_OustersFinSPK.Release();
	m_AdvacementQuestEnding.Release();

	// ½ÂÁ÷ °ü·Ã SPK Á¦°Å
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

	m_AddonSSPK.Release();			// Spriteµé		
	m_OustersSSPK.Release();			// Spriteµé		
	m_ImageObjectSSPK.Release();
//	m_InteractionObjectSSPK.Release();
	m_CreatureSSPK.Release();

	//----------------------------------------------------------------------
	// File Index Table Áö¿ì±â
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
	// SFP ArrayÁ¦°Å
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
// Ãâ·Â ´ë»óÀÌ µÇ´Â Surface¸¦ °áÁ¤ÇÑ´Ù.
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
#ifdef PLATFORM_WINDOWS
	if (CDirect3D::IsHAL())
	{
//		if (m_pImageObjectShadowManager!=NULL) m_pImageObjectShadowManager->Clear();
//		if (m_pAddonShadowManager!=NULL) m_pAddonShadowManager->Clear();
//		if (m_pOustersShadowManager!=NULL) m_pOustersShadowManager->Clear();
//		if (m_pCreatureShadowManager!=NULL) m_pCreatureShadowManager->Clear();
	}
#endif
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
//		// °ËÀº»ö
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
//		// HP»ö±ò
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

#ifdef PLATFORM_WINDOWS
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
#endif

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
	
	// ¸ðµÎ ´Ù½Ã ±×·ÁÁØ´Ù.
	//m_bFirstTileDraw = true;
	
	ClearOutputCreature();

	ClearItemNameList();

	//------------------------------------------------------------
	// °¡¼ÓÀ» »ç¿ëÇÏ´Â °æ¿ì¿¡´Â
	// Texture°¡ VideoMemory¿¡ ¿Ã¶ó°¡ ÀÖÀ¸¹Ç·Î RestoreÇØ¾ß ÇÑ´Ù.
	// ´Ù½Ã Load.. - -;;
	//------------------------------------------------------------
#ifdef PLATFORM_WINDOWS
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
		// minimap textureÃÊ±âÈ­
		//------------------------------------------------------------
		// 2001.7.14 ÁÖ¼®Ã³¸®
//		if (m_pMinimapSPR!=NULL && m_pMinimapTexture!=NULL)
//		{
//			m_pMinimapTexture->Restore();
////			InitMinimapTexture();
//		}		
	}
#endif // PLATFORM_WINDOWS
	/*
	if (CDirect3D::IsHAL())
	{
		//------------------------------------------------------------
		//
		// Load  EffectPack
		//
		//------------------------------------------------------------
		std::ifstream	effectFile2(FILE_ASPRITE_ALPHAEFFECT, ios::binary);
		m_EffectAlphaSPK.LoadFromFile(effectFile2);
		effectFile2.close();	

		//------------------------------------------------------------
		//
		// Init Effect TextureSurface
		//
		//------------------------------------------------------------		
		//--------------------------------------------
		// AlphaSPKÀ» ÀÌ¿ëÇØ¼­ TexturePackÀ» »ý¼ºÇÑ´Ù.
		//--------------------------------------------
		if (!m_EffectTPK.Init( m_EffectAlphaSPK ))
		{
			MessageBox(g_hWnd, "Can't Init EffectTexture!", NULL, MB_OK);
			return;
		}

		//------------------------------------------------------------	
		// EffectAlphaSprite¸¦ ¸Þ¸ð¸®¿¡¼­ »èÁ¦ÇÑ´Ù.
		// --> TextureSurface¸¦ »ç¿ëÇÒ °ÍÀÌ¹Ç·Î.. ÇÊ¿ä°¡ ¾ø´Ù.
		//------------------------------------------------------------		
		m_EffectAlphaSPK.Release();	
	}
	*/
}


//----------------------------------------------------------------------
// »ç¿ëÇÒ SurfaceµéÀ» ÃÊ±âÈ­ ½ÃÅ²´Ù.
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
	// 3D°¡¼ÓÀÌ µÇ¸é VideoMemory¿¡.. (°¡´ÉÇÏ¸é. - -;)
	//
	// ¾ÈµÇ¸é.. system memory¿¡... 
	// (¿Ö? À½.. ¾îµò°¡¿¡ °ü·ÃµÈ ÄÚµå°¡ ÀÖÁö ½ÍÀºµ¥ Ã£±â ±ÍÂú´Ù. À½³Ä)
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


#ifdef PLATFORM_WINDOWS
	if (CDirect3D::IsHAL())
	{
		// ÀÏ´Ü video memory¿¡ »ý¼ºÀÌ µÇµµ·Ï ÇØº¸°í..
		// ¾ÈµÇ¸é system memory¿¡ »ý¼ºÇÑ´Ù.
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
#endif
	//----------------------------------------------------------------
	// 2D
	//----------------------------------------------------------------
#ifdef PLATFORM_WINDOWS
	else
	{
		m_pTileSurface->InitOffsurface(g_TILESURFACE_WIDTH,
									g_TILESURFACE_HEIGHT,
									DDSCAPS_SYSTEMMEMORY);
	}
#else
	{
		m_pTileSurface->InitOffsurface(g_TILESURFACE_WIDTH,
									g_TILESURFACE_HEIGHT);
	}
#endif

	//m_pTileSurface->InitOffsurface(TILESURFACE_WIDTH, 
	//								TILESURFACE_HEIGHT,
	//								DDSCAPS_SYSTEMMEMORY);
	//,(CDirect3D::IsHAL()? 0 : DDSCAPS_SYSTEMMEMORY));

	m_pTileSurface->SetTransparency( 0 );

	//----------------------------------------------------------------------
	// Note: TileRenderer initialization moved to InitSprites()
	// to fix initialization order issue (InitSprites is called after InitSurfaces)
	//----------------------------------------------------------------------



	UI_DrawProgress(1);
	DrawTitleLoading();

	return true;
}

//----------------------------------------------------------------------
// Init Colors
//----------------------------------------------------------------------
// »ö±òµéÀ» ÃÊ±âÈ­ ÇÑ´Ù.
//----------------------------------------------------------------------
bool
MTopView::InitColors()
{
	//---------------------------------------------------
	// ´Ã °°Àº »ö
	//---------------------------------------------------
	// item
	m_ColorNameItem					= g_pClientConfig->COLOR_NAME_ITEM;
	m_ColorNameItemOption			= g_pClientConfig->COLOR_NAME_ITEM_OPTION;

	// Á¾Á·º°
	m_ColorNameVampire				= g_pClientConfig->COLOR_NAME_VAMPIRE;
	m_ColorNameSlayer				= g_pClientConfig->COLOR_NAME_SLAYER;
	m_ColorNameNPC					= g_pClientConfig->COLOR_NAME_NPC;
	
	m_ColorNameAlignment[0]			= g_pClientConfig->COLOR_NAME_EVIL_MORE;
	m_ColorNameAlignment[1]			= g_pClientConfig->COLOR_NAME_EVIL;
	m_ColorNameAlignment[2]			= g_pClientConfig->COLOR_NAME_NEUTRAL;
	m_ColorNameAlignment[3]			= g_pClientConfig->COLOR_NAME_GOOD;
	m_ColorNameAlignment[4]			= g_pClientConfig->COLOR_NAME_GOOD_MORE;	


	//---------------------------------------------------
	// 5:6:5ÀÎ °æ¿ì´Â ¹Ù·Î °ªÀ» ÀÐÀ¸¸é µÈ´Ù.
	//---------------------------------------------------
	if (CDirectDraw::Is565())
	{
		// interactionObject
//		m_ColorOutlineInteractionObject	= g_pClientConfig->COLOR_OUTLINE_INTERACTIONOBJECT;

		m_ColorOutlineItem				= g_pClientConfig->COLOR_OUTLINE_ITEM;

		// °ø°Ý °¡´É?
		m_ColorOutlineNPC				= g_pClientConfig->COLOR_OUTLINE_NPC;
		m_ColorOutlineAttackPossible	= g_pClientConfig->COLOR_OUTLINE_ATTACK_POSSIBLE;
		m_ColorOutlineAttackImpossible	= g_pClientConfig->COLOR_OUTLINE_ATTACK_IMPOSSIBLE;	
	}
	//---------------------------------------------------
	// 5:5:5ÀÎ °æ¿ì´Â °ªÀ» º¯È¯
	//---------------------------------------------------
	else
	{
		// interactionObject
//		m_ColorOutlineInteractionObject	= CDirectDraw::Convert565to555(g_pClientConfig->COLOR_OUTLINE_INTERACTIONOBJECT);

		// item
		m_ColorOutlineItem				= CDirectDraw::Convert565to555(g_pClientConfig->COLOR_OUTLINE_ITEM);

		// °ø°Ý °¡´É?
		m_ColorOutlineNPC				= CDirectDraw::Convert565to555(g_pClientConfig->COLOR_OUTLINE_NPC);
		m_ColorOutlineAttackPossible	= CDirectDraw::Convert565to555(g_pClientConfig->COLOR_OUTLINE_ATTACK_POSSIBLE);
		m_ColorOutlineAttackImpossible	= CDirectDraw::Convert565to555(g_pClientConfig->COLOR_OUTLINE_ATTACK_IMPOSSIBLE);
	}


	//---------------------------------------------------
	// 3D °¡¼Ó°ú °ü·ÃÀÌ ÀÖ´Â »ö±ò...
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

		m_ColorHPBarBg = CDirectDraw::Color(g_pClientConfig->COLOR_HP_BAR_BG_R,
											 g_pClientConfig->COLOR_HP_BAR_BG_G,
											 g_pClientConfig->COLOR_HP_BAR_BG_B);

		// UI 박스 배경색: RGB565의 G는 6비트이므로 2배 필요
		// 어두운 회색: R=12, G=24, B=12
		m_ColorUIBoxBg = CDirectDraw::Color(12, 24, 12);

		m_ColorBlackHalf = 0;
	}

	return true;
}
	

//----------------------------------------------------------------------
// SpritePackÀ» ÀÐ¾î¼­  memory¿¡ LoadÇÑ´Ù.
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
	std::ofstream	ImageObjectSFile(FILE_SSPRITE_IMAGEOBJECT, ios::binary);	
	std::ofstream	ImageObjectSIndexFile(FILE_SSPRITEINDEX_IMAGEOBJECT, ios::binary);	

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
	// 3d°¡¼ÓÀÌ µÇ¸é m_pImageObjectShadowManager¸¦ »ç¿ëÇÏ°í
	// ¾Æ´Ï¸é, m_ImageObjectSSPK¸¦ »ç¿ëÇÑ´Ù.
	/*
	std::ifstream	ImageObjectShadowFile2;//(FILE_SSPRITE_IMAGEOBJECT, ios::binary);
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
// 		std::ifstream	ImageObjectShadowFile2;//(FILE_SSPRITE_IMAGEOBJECT, ios::binary);
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
	// ÀÏ´Ü ÀüÃ¼ Å©±â¸¸Å­ memory´Â Àâ¾ÆµÐ´Ù.
	//------------------------------------------------------------
	//  Init Creature SpriteSet
	//------------------------------------------------------------	
	if (m_CreatureSPK.GetSize()==0)
	{
		/*
		WORD size;
		std::ifstream CreaturePackIndexFile;//(FILE_ISPRITEINDEX_CREATURE, ios::binary);
		if (!FileOpenBinary(FILE_ISPRITEINDEX_CREATURE, CreaturePackIndexFile))
			return false;
		CreaturePackIndexFile.read((char*)&size, 2);	// SpriteÀÇ °³¼ö
		CreaturePackIndexFile.close();	

		m_CreatureSPK.Init( size, CDirectDraw::Is565() );
		*/
//		if (m_CreatureSPKFile.is_open())
//		{
//			m_CreatureSPKFile.close();
//		}
//		
//		// È­ÀÏ¸¸ ¿­¾îµÐ´Ù.
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
	std::ofstream	creatureShadowFile(FILE_SSPRITE_CREATURE, ios::binary);	
	std::ofstream	creatureShadowIndexFile(FILE_SSPRITEINDEX_CREATURE, ios::binary);
	m_CreatureSSPK.SaveToFile(creatureShadowFile, creatureShadowIndexFile);
	creatureShadowFile.close();
	creatureShadowIndexFile.close();
	*/

	//------------------------------------------------------------
	// Å×½ºÆ® : ÀÏ¹ÝSpritePack --> ShadowSpritePack
	//------------------------------------------------------------
	// ³²
	/*
	CSpritePack CreatureShadowSPK;
	std::ifstream	CreatureShadowFile2;//(FILE_ISPRITE_ADDON, ios::binary);
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
	// ³² - Save  ShadowSpritePack	
	//------------------------------------------------------------
	std::ofstream	CreatureShadowFile(FILE_SSPRITE_CREATURE, ios::binary);	
	std::ofstream	CreatureShadowIndexFile(FILE_SSPRITEINDEX_CREATURE, ios::binary);	

	m_CreatureSSPK.SaveToFile(CreatureShadowFile, CreatureShadowIndexFile);

	CreatureShadowFile.close();
	CreatureShadowIndexFile.close();
	*/

	//------------------------------------------------------------
	// Load Creature Shadow SpritePack
	//------------------------------------------------------------
	// 3d°¡¼ÓÀÌ µÇ¸é m_pCreatureShadowManager¸¦ »ç¿ëÇÏ°í
	// ¾Æ´Ï¸é, m_CreatureSSPK¸¦ »ç¿ëÇÑ´Ù.
	//------------------------------------------------------------
	// 3d°¡¼ÓÀÌ µÇ¸é m_pAlphaEffectTextureManager¸¦ »ç¿ëÇÏ°í
	// ¾Æ´Ï¸é, m_EffectAlphaSPK¸¦ »ç¿ëÇÑ´Ù.
	
	// ÀüÃ¼ °³¼ö¸¸ Àâ¾ÆµÐ´Ù.
	/*
	std::ifstream CreatureShadowPackIndexFile;//(FILE_ISPRITEINDEX_CREATURE, ios::binary);
	if (!FileOpenBinary(FILE_SSPRITEINDEX_CREATURE, CreatureShadowPackIndexFile))
		return false;
	CreatureShadowPackIndexFile.read((char*)&size, 2);	// SpriteÀÇ °³¼ö
	CreatureShadowPackIndexFile.close();	

	m_CreatureSSPK.Init( size );
	*/
	/*
	// ÀÓ½Ã·Î Load
	// ÀüÃ¼ loadingÇØµÎ´Â ºÎºÐ
	std::ifstream	CreatureShadowFile2;//(FILE_SSPRITE_Creature, ios::binary);
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
		std::ifstream	CreatureShadowFile2;//(FILE_SSPRITE_Creature, ios::binary);
		if (!FileOpenBinary(FILE_SSPRITE_CREATURE, CreatureShadowFile2))
			return false;
		m_CreatureSSPK.LoadFromFile(CreatureShadowFile2);
		CreatureShadowFile2.close();
		*/
		/*
		std::ifstream indexFile;//(indexFilename, ios::binary);
		if (!FileOpenBinary(FILE_SSPRITEINDEX_CREATURE, indexFile))
			return false;

		int packSize;
		indexFile.read((char*)&packSize, 2);
		indexFile.close();

		m_CreatureSSPK.Init( packSize );
		*/
		//------------------------------------------------------------
		// ±×¸²ÀÚ load
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
	std::ofstream	clothesFile("Clothes.spk", ios::binary);	
	std::ofstream	clothesIndexFile("Clothes.spki", ios::binary);	

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
		std::ifstream	AddonFile2;//(FILE_ISPRITE_ADDON, ios::binary);
		if (!FileOpenBinary(FILE_ISPRITE_ADDON, AddonFile2))
			return false;
		
		// 2001.8.20 ÁÖ¼®Ã³¸®..
		//m_AddonSPK.LoadFromFile(AddonFile2);
		//m_AddonSPK.Init( 20000, CDirectDraw::Is565() );
		
		// °³¼ö¸¸ Àâ¾ÆµÐ´Ù.
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
//	// È­ÀÏ¸¸ ¿­¾îµÐ´Ù.
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
		// ±×¸²ÀÚ load
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

/*
	m_AdvancementSlayerSPK.LoadFromFileRunning( g_pFileDef->getProperty( "FILE_ISPRITE_ADVANCEMENT_CLASS_SLAYER" ).c_str() );
	
	m_AdvancementSlayerSSPK.LoadFromFileRunning( g_pFileDef->getProperty( "FILE_SSPRITE_ADVANCEMENT_CLASS_SLAYER" ).c_str() );
	m_AdvancementVampireSSPK.LoadFromFileRunning( g_pFileDef->getProperty( "FILE_SSPRITE_ADVANCEMENT_CLASS_VAMPIRE" ).c_str() );
	
*/

	//------------------------------------------------------------	
	// ±×¸²ÀÚ - Load  Clothes SpritePack	
	//------------------------------------------------------------
	// ³²
	/*
	std::ifstream	AddonMaleShadowFile2;//(FILE_ISPRITE_ADDON, ios::binary);
	if (!FileOpenBinary(FILE_SSPRITE_ADDON_MALE, AddonMaleShadowFile2))
		return false;
	m_AddonMaleShadowSPK.LoadFromFile(AddonMaleShadowFile2);
	AddonMaleShadowFile2.close();
	*/

	//------------------------------------------------------------
	// Å×½ºÆ® : ÀÏ¹ÝSpritePack --> ShadowSpritePack
	//------------------------------------------------------------
	/*
	// ³²
	CSpritePack AddonMaleShadowSPK;
	std::ifstream	AddonMaleShadowFile2;//(FILE_ISPRITE_ADDON, ios::binary);
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
	// ³² - Save  ShadowSpritePack	
	//------------------------------------------------------------
	std::ofstream	AddonMaleShadowFile(FILE_SSPRITE_ADDON_MALE, ios::binary);	
	std::ofstream	AddonMaleShadowIndexFile(FILE_SSPRITEINDEX_ADDON_MALE, ios::binary);	

	m_AddonMaleSSPK.SaveToFile(AddonMaleShadowFile, AddonMaleShadowIndexFile);

	AddonMaleShadowFile.close();
	AddonMaleShadowIndexFile.close();
	*/

	//------------------------------------------------------------
	// Load Addon Shadow SpritePack
	//------------------------------------------------------------
	// Male
	/*
	std::ifstream	AddonShadowFile2;//(FILE_ISPRITE_ADDON, ios::binary);
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
		std::ifstream	AddonShadowFile2;//(FILE_ISPRITE_ADDON, ios::binary);
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
	std::ofstream	itemTileFile(FILE_SPRITE_ITEMTILE, ios::binary);	
	std::ofstream	itemTileIndexFile(FILE_SPRITEINDEX_ITEMTILE, ios::binary);	

	m_ItemTileSPK.SaveToFile(itemTileFile, itemTileIndexFile);

	itemTileFile.close();
	itemTileIndexFile.close();
	*/

	//------------------------------------------------------------	
	// Load  ItemTile SpritePack	
	//------------------------------------------------------------
	if (m_ItemTileISPK.GetSize()==0)
	{
//		std::ifstream	itemTileFile2;//(FILE_SPRITE_ITEMTILE, ios::binary);
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
//		std::ifstream	itemDropFile2;//(FILE_SPRITE_itemDrop, ios::binary);
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
//		std::ifstream	itemBrokenFile2;//(FILE_SPRITE_itemBroken, ios::binary);
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
	std::ofstream	tileFile(FILE_SPRITE_TILE, ios::binary);	
	std::ofstream	tileIndexFile(FILE_SPRITEINDEX_TILE, ios::binary);	

	m_TileSPK.SaveToFile(tileFile, tileIndexFile);

	tileFile.close();
	tileIndexFile.close();	

	//------------------------------------------------------------	
	// Load  Tile SpritePack	
	//------------------------------------------------------------	
	std::ifstream	tileFile2(FILE_SPRITE_TILE, ios::binary);
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

	// TileSPK Index¸¦ LoadÇÑ´Ù.
	CFileIndexTable		TileIndex;

	std::ifstream TilePackIndexFile(FILE_SPRITEINDEX_TILE, ios::binary);
	TileIndex.LoadFromFile( TilePackIndexFile );
	TilePackIndexFile.close();	

	// TileSPK¿¡¼­ Æ¯Á¤ÇÑ SpriteIDÀÇ Tileµé¸¸À¸·Î SetÀ» »ý¼ºÇÑ´Ù.
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
//		std::ifstream TilePackIndexFile;//(FILE_SPRITEINDEX_TILE, ios::binary);
//		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_SPRITEINDEX_TILE").c_str(), TilePackIndexFile))
//			return false;
//		
//		//-----------------------------------------------------------
//		// Index¸¦ LoadÇÑ´Ù.
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
	
	/* TILE INDEX ¸¸µé±â
	CSpritePack spk;
	std::ifstream TilePackIndexFile(FILE_SPRITE_TILE, ios::binary);
	spk.LoadFromFile(TilePackIndexFile);	
	TilePackIndexFile.close();	

	std::ofstream TilePackIndexFile2(FILE_SPRITE_TILE, ios::binary);
	std::ofstream TilePackIndexFile3(FILE_SPRITEINDEX_TILE, ios::binary);
	spk.SaveToFile(TilePackIndexFile2, TilePackIndexFile3);
	TilePackIndexFile2.close();
	TilePackIndexFile3.close();

	m_TileSPK.Init( spk.GetSize(), CDirectDraw::Is565() );
	*/


	/*
	std::ifstream	TilePackFile(FILE_SPRITE_TILE, ios::binary);		
	m_TileSPK.LoadFromFilePart(TilePackFile, TileSFPArray);
	TilePackFile.close();
	*/
	m_TileSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SPRITE_TILE").c_str() );

	//----------------------------------------------------------------------
	// Create TileRenderer instance (Phase 4 integration)
	//----------------------------------------------------------------------
	m_pTileRenderer = new TileRenderer();
	if (m_pTileRenderer == NULL)
	{
		DEBUG_ADD("ERROR: Failed to create TileRenderer!");
		return false;
	}
	DEBUG_ADD("TileRenderer instance created");

	//----------------------------------------------------------------------
	// Initialize TileRenderer (moved from InitSurfaces to fix initialization order)
	//----------------------------------------------------------------------
	// Note: m_pTileSurface should already be created by InitSurfaces() at this point
	if (m_pTileSurface != NULL)
	{
		printf("[InitSprites] Calling TileRenderer::Init with m_pTileSurface=%p, &m_TileSPK=%p\n",
			m_pTileSurface, &m_TileSPK);

		if (!m_pTileRenderer->Init(m_pTileSurface, &m_TileSPK))
		{
			printf("[InitSprites] ERROR: Failed to initialize TileRenderer!\n");
			DEBUG_ADD("ERROR: Failed to initialize TileRenderer!");
			delete m_pTileRenderer;
			m_pTileRenderer = NULL;
			return false;
		}
		else
		{
			// Set tile dimensions
			m_pTileRenderer->SetTileDimensions(TILE_X, TILE_Y);

			// Set null tile sprite pack (m_EtcSPK for SPRITEID_TILE_NULL)
			m_pTileRenderer->SetNullTileSpritePack(&m_EtcSPK, SPRITEID_TILE_NULL);

			printf("[InitSprites] TileRenderer initialized successfully\n");
			DEBUG_ADD("TileRenderer initialized successfully");
		}
	}
	else
	{
		printf("[InitSprites] WARNING: m_pTileSurface is NULL, TileRenderer will be initialized later\n");
		// TileRenderer will be initialized in InitSurfaces after m_pTileSurface is created
	}



	//------------------------------------------------------------
	//
	//
	//                 ImageObject SpriteSet
	//
	//------------------------------------------------------------
	//
	// Index¾ø´Â SPK¿¡ Index»ý¼ºÇÏ±â
	/*
	CSpritePack tempSPK;
	std::ifstream	ioFile2(FILE_SPRITE_IMAGEOBJECT, ios::binary);
	tempSPK.LoadFromFile(ioFile2);
	ioFile2.close();

	std::ofstream	ioFile(FILE_SPRITE_IMAGEOBJECT, ios::binary);	
	std::ofstream	ioIndexFile(FILE_SPRITEINDEX_IMAGEOBJECT, ios::binary);	

	tempSPK.SaveToFile(ioFile, ioIndexFile);

	ioFile.close();
	ioIndexFile.close();	
	*/

	/*
	CSpriteFilePositionArray	ImageObjectSFPArray;
	ImageObjectSFPArray.Init( 21 );

	// ImageObjectSPK Index¸¦ LoadÇÑ´Ù.
	CFileIndexTable		ImageObjectIndex;

	std::ifstream ImageObjectPackIndexFile(FILE_SPRITEINDEX_IMAGEOBJECT, ios::binary);
	ImageObjectIndex.LoadFromFile( ImageObjectPackIndexFile );
	ImageObjectPackIndexFile.close();	

	// ImageObjectSPK¿¡¼­ Æ¯Á¤ÇÑ SpriteIDÀÇ ImageObjectµé¸¸À¸·Î SetÀ» »ý¼ºÇÑ´Ù.
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
		
//		std::ifstream ImageObjectPackIndexFile;//(FILE_SPRITEINDEX_IMAGEOBJECT, ios::binary);
//		if (!FileOpenBinary(FILE_SPRITEINDEX_IMAGEOBJECT, ImageObjectPackIndexFile))
//			return false;
//
//		//-----------------------------------------------------------
//		// Index¸¦ LoadÇÑ´Ù.
//		//-----------------------------------------------------------
//		m_ImageObjectSPKI.LoadFromFile( ImageObjectPackIndexFile );
//		ImageObjectPackIndexFile.close();	
//
//		m_ImageObjectSPK.Init( m_ImageObjectSPKI.GetSize(), CDirectDraw::Is565() );
//
//		m_ImageObjectSPKFile.open(FILE_SPRITE_IMAGEOBJECT, ios::binary);
		m_ImageObjectSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SPRITE_IMAGEOBJECT").c_str() );
		// ÄÀ.. ÀÌ°Å ¿Ö ´Ù ·ÎµùÇÏÁö. - -;;
		// 2001.8.20 ÁÖ¼®Ã³¸®
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
	std::ifstream	ImageObjectPackFile(FILE_SPRITE_IMAGEOBJECT, ios::binary);		
	m_ImageObjectSPK.LoadFromFilePart(ImageObjectPackFile, ImageObjectSFPArray);
	ImageObjectPackFile.close();	
	*/

	//------------------------------------------------------------
	//
	//
	//                   EffectSPK ÃÊ±âÈ­
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
	std::ofstream	effectFile(FILE_ASPRITE_ALPHAEFFECT, ios::binary);	
	std::ofstream	effectIndexFile(FILE_ASPRITEINDEX_ALPHAEFFECT, ios::binary);	

	m_EffectAlphaSPK.SaveToFile(effectFile, effectIndexFile);

	effectFile.close();
	effectIndexFile.close();
	*/

	//------------------------------------------------------------
	//
	// Load  EffectPack
	//
	//------------------------------------------------------------
	// 3d°¡¼ÓÀÌ µÇ¸é m_pAlphaEffectTextureManager¸¦ »ç¿ëÇÏ°í
	// ¾Æ´Ï¸é, m_EffectAlphaSPK¸¦ »ç¿ëÇÑ´Ù.
/*
	std::ifstream	effectFile2;//(FILE_ASPRITE_ALPHAEFFECT, ios::binary);
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
		//std::ifstream	effectFile2;//(FILE_ASPRITE_ALPHAEFFECT, ios::binary);
		//if (!FileOpenBinary(FILE_ASPRITE_ALPHAEFFECT, effectFile2))
		//	return false;
		//m_EffectAlphaSPK.LoadFromFile(effectFile2);
		//effectFile2.close();	
//		std::ifstream effectFileIndex;//(FILE_ISPRITEINDEX_CREATURE, ios::binary);
//		if (!FileOpenBinary(FILE_ASPRITEINDEX_ALPHAEFFECT, effectFileIndex))
//			return false;
//		
//		// Index File¸¸ loadingÇÏ°í 
//		m_EffectAlphaSPKI.LoadFromFile( effectFileIndex );
//
//		// °³¼ö¸¸Å­ ÃÊ±âÈ­¸¸ ÇØµÐ´Ù.
//		m_EffectAlphaSPK.Init( m_EffectAlphaSPKI.GetSize(), CDirectDraw::Is565() );
//
//		effectFileIndex.close();	
//
//		//------------------------------------------------------------
//		// Run-time loadingÀ» À§ÇØ¼­ fileÀ» ¿­¾îµÐ´Ù.
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
		//std::ifstream	effectFile2;//(FILE_ASPRITE_ALPHAEFFECT, ios::binary);
		//if (!FileOpenBinary(FILE_ASPRITE_ALPHAEFFECT, effectFile2))
		//	return false;
		//m_EffectAlphaSPK.LoadFromFile(effectFile2);
		//effectFile2.close();	
//		std::ifstream effectFileIndex;//(FILE_ISPRITEINDEX_CREATURE, ios::binary);
//		if (!FileOpenBinary(FILE_SPRITEINDEX_SCREENEFFECT, effectFileIndex))
//			return false;
//		
//		// Index File¸¸ loadingÇÏ°í 
//		m_EffectScreenSPKI.LoadFromFile( effectFileIndex );
//
//		// °³¼ö¸¸Å­ ÃÊ±âÈ­¸¸ ÇØµÐ´Ù.
//		m_EffectScreenSPK.Init( m_EffectScreenSPKI.GetSize(), CDirectDraw::Is565() );
//
//		effectFileIndex.close();	
//
//		//------------------------------------------------------------
//		// Run-time loadingÀ» À§ÇØ¼­ fileÀ» ¿­¾îµÐ´Ù.
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
//		std::ifstream	effectFile2;
//		if (!FileOpenBinary(FILE_SSPRITE_SHADOWEFFECT, effectFile2))
//			return false;
//		m_EffectShadowSPK.LoadFromFile(effectFile2);
//		effectFile2.close();			
		m_EffectShadowSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SSPRITE_SHADOWEFFECT").c_str());
	}

//	UI_DrawProgress(12);
//	DrawTitleLoading();

	//std::ifstream	effectFile2(FILE_ASPRITE_ALPHAEFFECT, ios::binary);
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
		// AlphaSPKÀ» ÀÌ¿ëÇØ¼­ TexturePackÀ» »ý¼ºÇÑ´Ù.
		//--------------------------------------------
		if (!m_EffectTPK.Init( m_EffectAlphaSPK ))
		{
			MessageBox(g_hWnd, "Can't Init EffectTexture!", NULL, MB_OK);
			return false;
		}

		//------------------------------------------------------------	
		// EffectAlphaSprite¸¦ ¸Þ¸ð¸®¿¡¼­ »èÁ¦ÇÑ´Ù.
		// --> TextureSurface¸¦ »ç¿ëÇÒ °ÍÀÌ¹Ç·Î.. ÇÊ¿ä°¡ ¾ø´Ù.
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
	std::ofstream	EtcFile(FILE_SPRITE_ETC, ios::binary);	
	std::ofstream	EtcIndexFile(FILE_SPRITEINDEX_ETC, ios::binary);	

	m_EtcSPK.SaveToFile(EtcFile, EtcIndexFile);

	EtcFile.close();
	EtcIndexFile.close();	
	*/

	//------------------------------------------------------------	
	// Load  Etc SpritePack	
	//------------------------------------------------------------	
	if (m_EtcSPK.GetSize()==0)
	{
		std::ifstream	EtcFile2;//(FILE_SPRITE_ETC, ios::binary);
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
//		std::ifstream	ioFile2;//(FILE_SPRITE_ETC, ios::binary);
//		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_SPRITE_INTERACTIONOBJECT").c_str(), ioFile2))
//			return false;
//		m_InteractionObjectSPK.LoadFromFile(ioFile2);
//		ioFile2.close();
//	}

//	if (m_InteractionObjectSSPK.GetSize()==0)
//	{
//		/*
//		std::ifstream	ioFile3;//(FILE_SPRITE_ETC, ios::binary);
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
	
	std::ofstream	NormalEffectFile(FILE_SPRITE_NORMALEFFECT, ios::binary);	
	std::ofstream	NormalEffectIndexFile(FILE_SPRITEINDEX_NORMALEFFECT, ios::binary);	

	m_EffectNormalSPK.SaveToFile(NormalEffectFile, NormalEffectIndexFile);

	NormalEffectFile.close();
	NormalEffectIndexFile.close();
	*/

	//------------------------------------------------------------	
	// Load  Clothes SpritePack	
	//------------------------------------------------------------
	if (m_EffectNormalSPK.GetSize()==0)
	{
//		std::ifstream	NormalEffectFile2;//(FILE_SPRITE_NORMALEFFECT, ios::binary);
//		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_SPRITE_NORMALEFFECT").c_str(), NormalEffectFile2))
//			return false;
//		m_EffectNormalSPK.LoadFromFile(NormalEffectFile2);
//		NormalEffectFile2.close();	
		m_EffectNormalSPK.LoadFromFileRunning(g_pFileDef->getProperty("FILE_SPRITE_NORMALEFFECT").c_str());
	}


	//------------------------------------------------------------
	//
	//
	//                   Weather SpritePack ÃÊ±âÈ­
	//
	//
	//------------------------------------------------------------
	//------------------------------------------------------------	
	// Load  Clothes SpritePack	
	//------------------------------------------------------------
	if (m_WeatherSPK.GetSize()==0)
	{
		std::ifstream	WeatherFile2;//(FILE_SPRITE_WEATHER, ios::binary);
		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_SPRITE_WEATHER").c_str(), WeatherFile2))
			return false;
		m_WeatherSPK.LoadFromFile(WeatherFile2);
		WeatherFile2.close();
	}

	//------------------------------------------------------------
	//
	//
	//                   Shadow Test ÃÊ±âÈ­
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
	// AlphaSPKÀ» ÀÌ¿ëÇØ¼­ TexturePackÀ» »ý¼ºÇÑ´Ù.
	//--------------------------------------------
	if (!m_ShadowTPK.Init( ShadowASPK ))
	{
		MessageBox(g_hWnd, "Can't Init ShadowTexture!", NULL, MB_OK);
		return false;
	}

	//------------------------------------------------------------	
	// EffectAlphaSprite¸¦ ¸Þ¸ð¸®¿¡¼­ »èÁ¦ÇÑ´Ù.
	// --> TextureSurface¸¦ »ç¿ëÇÒ °ÍÀÌ¹Ç·Î.. ÇÊ¿ä°¡ ¾ø´Ù.
	//------------------------------------------------------------		
	ShadowASPK.Release();
	*/
	
	//------------------------------------------------------------
	//
	//				Minimap	
	//
	//------------------------------------------------------------
	 // 2001.7.14 ÁÖ¼®Ã³¸®
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
	std::ofstream	MapTest1("Data\\Image\\map_a.spr", ios::binary);		
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
	std::ofstream	MapTest2("Data\\Image\\map_h.spr", ios::binary);		
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
	std::ofstream	MapTest3("Data\\Image\\map_e.spr", ios::binary);		
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
	std::ofstream	MapTest4("Data\\Image\\map_c.spr", ios::binary);		
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
	std::ofstream	MapTest5("Data\\Image\\map_d.spr", ios::binary);		
	m_pMinimapSPR->SaveToFile( MapTest5 );
	MapTest5.close();

	*/

	//------------------------------------------------------------
	//
	//
	//                   Guild SpritePack ÃÊ±âÈ­
	//
	//
	//------------------------------------------------------------
	//------------------------------------------------------------	
	// Load  Guild SpritePack	
	//------------------------------------------------------------
	/*
	if (m_GuildSPK.GetSize()==0)
	{
		std::ifstream	guildFile2;//(FILE_SPRITE_WEATHER, ios::binary);
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
// Filter ÃÊ±âÈ­
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
	std::ofstream	FilterLight2DFile(FILE_FILTER_LIGHT2D, ios::binary);		
	m_Filter.SaveToFile(FilterLight2DFile);
	FilterLight2DFile.close();	
	*/

	//------------------------------------------------------------	
	// Load  Light2D FilterPack
	//------------------------------------------------------------	
	//std::ifstream	FilterLight2DFile2(FILE_FILTER_LIGHT2D, ios::binary);
	//m_Filter.LoadFromFile(FilterLight2DFile2);
	//FilterLight2DFile2.close();


	//------------------------------------------------------------	
	//
	//  3D ½Ã¾ßÃ³¸®¸¦ À§ÇÑ Light Filter
	//
	//------------------------------------------------------------	
	//-----------------------------------------------
	// LightBuffer Texture ÃÊ±âÈ­
	//-----------------------------------------------	
	if (m_pLightBufferTexture!=NULL)
	{
		//m_pLightBufferTexture->Restore();
		delete m_pLightBufferTexture;
		m_pLightBufferTexture = NULL;
	}
	//------------------------------------------------------------	
	// 2D light¿¡¼­ ÇÑ Á¡ÀÌ Â÷ÁöÇÏ´Â È­¸éÀÇ pixelÅ©±â
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


#ifdef PLATFORM_WINDOWS
	if (CDirect3D::IsHAL())
	{
		/*
		//------------------------------------------------------
		// Light FilterPack ÃÊ±âÈ­
		//------------------------------------------------------
		m_LightFTP.Init( MAX_LIGHT_SETSIZE );

		//------------------------------------------------------
		// LightFilter »ý¼º
		//------------------------------------------------------		
		int maxLight = 14;
		int maxCenter = 9;
		int lightRange[MAX_LIGHT_SETSIZE] = { 20, 26, 36, 46, 56, 66, 76, 86, 96, 106, 116, 126};
		int l,i,j;

		// MAX_LIGHT_SETSIZE¸¸Å­ ¹à±âÀÇ Å©±â Á¾·ù°¡ ÀÖ´Ù.		
		for (l=0; l<MAX_LIGHT_SETSIZE; l++)
		{
			int lightRangeHalf = lightRange[l]>>1;	
			int maxK = maxCenter,maxLight;
			float gap = (float)(maxK,1)/(float)lightRangeHalf;	// k°ªÀÌ 0~24±îÁö..

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
				
					if (k <= maxCenter) k = 0;			// maxCenter±îÁö´Â 0
					else if (k >= maxK) k = maxLight;	// ³ÑÀ¸¸é maxLight
					else k = k - maxCenter;				// ³ª¸ÓÁö´Â - maxCenter

					// ¹à±âÀÇ SET´Ü°è¸¦ ¸¸µç´Ù.
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
		std::ofstream	LightFilter3DFile(FILE_FILTER_LIGHT3D, ios::binary);
		m_LightFTP.SaveToFile(LightFilter3DFile);
		LightFilter3DFile.close();	

		*/
		
		//------------------------------------------------------------	
		// Load  Light3D FilterPack
		//------------------------------------------------------------	
		std::ifstream	LightFilter3DFile2;//(FILE_FILTER_LIGHT3D, ios::binary);
		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_FILTER_LIGHT3D").c_str(), LightFilter3DFile2))
			return false;
		m_LightFTP.LoadFromFile(LightFilter3DFile2);
		LightFilter3DFile2.close();

		//------------------------------------------------------
		// lightBuffer ÃÊ±âÈ­
		//------------------------------------------------------		
		m_LightBufferFilter.Init( SCREENLIGHT_WIDTH, SCREENLIGHT_HEIGHT );		
  		
		m_pLightBufferTexture = new CSpriteSurface;
		m_pLightBufferTexture->InitTextureSurface(SCREENLIGHT_WIDTH, SCREENLIGHT_HEIGHT, 0, CDirect3D::GetPixelFormat4444());	
	
	}
#endif

	//------------------------------------------------------------
	//
	//  2D Rendering path for non-HAL platforms (SDL, macOS, Linux)
	//  This is the PRIMARY path for SDL backend!
	//
	//------------------------------------------------------------
#ifndef PLATFORM_WINDOWS
	// SDL platform always uses 2D rendering path
	if (true)  // Force execute on SDL platforms
#else
	// Windows platform: use 2D path when 3D HAL is not available
	if (!CDirect3D::IsHAL())
#endif
	{
		/*
		//------------------------------------------------------
		// Light FilterPack ÃÊ±âÈ­
		//------------------------------------------------------
		m_LightFTP.Init( MAX_LIGHT_SETSIZE );

		//------------------------------------------------------
		// LightFilter »ý¼º
		//------------------------------------------------------		
		int maxLight = 14;
		int maxCenter = 9;
		int lightRange[MAX_LIGHT_SETSIZE] = { 20, 26, 36, 46, 56, 66, 76, 86, 96, 106, 116, 126 };
		int l,i,j;

		// MAX_LIGHT_SETSIZE¸¸Å­ ¹à±âÀÇ Å©±â Á¾·ù°¡ ÀÖ´Ù.		
		for (l=0; l<MAX_LIGHT_SETSIZE; l++)
		{
			int lightRangeHalf = lightRange[l]>>1;	
			int maxK = maxCenter,maxLight;
			float gap = (float)(maxK,1)/(float)lightRangeHalf;	// k°ªÀÌ 0~24±îÁö..

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
				
					if (k <= maxCenter) k = 0;			// maxCenter±îÁö´Â 0
					else if (k >= maxK) k = maxLight;	// ³ÑÀ¸¸é maxLight
					else k = k - maxCenter;				// ³ª¸ÓÁö´Â - maxCenter

					// ¹à±âÀÇ SET´Ü°è¸¦ ¸¸µç´Ù.
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
		std::ofstream	LightFilter2DFile(FILE_FILTER_LIGHT2D, ios::binary);
		m_LightFTP.SaveToFile(LightFilter2DFile);
		LightFilter2DFile.close();	
		*/

		//------------------------------------------------------
		// lightBuffer ÃÊ±âÈ­
		//------------------------------------------------------		
		m_LightBufferFilter.Init( SCREENLIGHT_WIDTH, SCREENLIGHT_HEIGHT );	
		
		//------------------------------------------------------------	
		// Load  Light2D FilterPack
		//------------------------------------------------------------	
		std::ifstream	LightFilter2DFile2;//(FILE_FILTER_LIGHT2D, ios::binary);
		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_FILTER_LIGHT2D").c_str(), LightFilter2DFile2))
			return false;
		m_LightFTP.LoadFromFile(LightFilter2DFile2);
		LightFilter2DFile2.close();

		m_p2DLightPixelWidth = new int [SCREENLIGHT_WIDTH];
		m_p2DLightPixelHeight = new int [SCREENLIGHT_HEIGHT];

		//----------------------------------------------------------------
		// LightBufferÀÇ ÇÑ Á¡ÀÌ ÀÇ¹ÌÇÏ´Â È­¸é»óÀÇ pixel°¡·Î ±æÀÌ
		//----------------------------------------------------------------
		// °¡·ÎÀÇ ÇÕ = 100 * 8 = 800
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
		// LightBufferÀÇ ÇÑ Á¡ÀÌ ÀÇ¹ÌÇÏ´Â È­¸é»óÀÇ pixel¼¼·Î ±æÀÌ
		//----------------------------------------------------------------
		// °³¼ö´Â SCREENLIGHT_HEIGHT	
		// total°ªÀÌ CLIPSURFACE_HEIGHT¿Í °°À¸¸é µÈ´Ù.
		// ÇöÀç´Â 600.  10*24 + 9*40	
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
		//ÐÞ¸ÄÎª768
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

		int i;
		for (i=0; i<SCREENLIGHT_WIDTH; i++)
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
	// Player¸¦ °¡¸®´Â ImageObjectµéÀ» Ã³¸®ÇÒ Filter
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

			//k -= 50;	// ºó °ø°£

			k >>= 2;	// ºÎµå·¯¿î(?) °ø°£

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
			
			// 0°ú 1°ª¸¸...
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
	std::ofstream	ImageObjectFilterFile(FILE_FILTER_IMAGEOBJECT, ios::binary);
	m_ImageObjectFilter.SaveToFile(ImageObjectFilterFile);
	ImageObjectFilterFile.close();	
	*/
	
	//------------------------------------------------------------	
	// Load  Light3D FilterPack
	//------------------------------------------------------------	
	if (m_ImageObjectFilter.IsNotInit())
	{
		std::ifstream	ImageObjectFilterFile2;//(FILE_FILTER_IMAGEOBJECT, ios::binary);
		if (!FileOpenBinary(g_pFileDef->getProperty("FILE_FILTER_IMAGEOBJECT").c_str(), ImageObjectFilterFile2))
			return false;
		m_ImageObjectFilter.LoadFromFile(ImageObjectFilterFile2);
		ImageObjectFilterFile2.close();
	}

	return true;
}

//----------------------------------------------------------------------
// CharacterÀÇ Á¤º¸¸¦ »ý¼ºÇÑ´Ù.
//----------------------------------------------------------------------
// m_CreatureFrame[Á¾·ù][Action][Direction][Frame]
//----------------------------------------------------------------------
bool
MTopView::InitCreatureFrames()
{		
	//------------------------------------------------------------
	//
	//  Creature BodyÀÇ Frame Á¤º¸¸¦ »ý¼ºÇÑ´Ù.
	//
	//------------------------------------------------------------
		 
	/*
	m_CreatureFPK.Init(MAX_CREATURE_BODY);	

	//-----------------------
	// Ã¹¹øÂ° Creature(Woman)
	//-----------------------
	// µ¿ÀÛ °³¼ö Á¤ÀÇ
	m_CreatureFPK[0].Init(ACTION_MAX);

	// n¹øÂ° Sprite
	WORD n = 0;
	int i,j,k;

	// °¢ µ¿ÀÛ¿¡ 8¹æÇâÀÇ FrameArray°¡ ÀÖ´Ù.
	for (k=0; k<ACTION_MAX; k++)
		m_CreatureFPK[0][k].Init(8);

	// 0~8¹øÂ° ¹æÇâ¿¡´Â °¢°¢ 8°³ÀÇ FrameÀÌ ÀÖ´Ù.
	for (i=0; i<8; i++)
	{
		for (k=0; k<ACTION_MAX; k++)
			m_CreatureFPK[0][k][i].Init(8);		
		
		// 8°³ÀÇ Frame¿¡ ´ëÇÑ Á¤º¸¸¦ SetÇÑ´Ù.
		for (j=0; j<8; j++)
		{
			for (k=0; k<ACTION_MAX; k++)
				m_CreatureFPK[0][k][i][j].Set(n, 0,-40);			

			n ++;
		}  		
	}	

	//-----------------------
	// µÎ¹øÂ° Creature(Skeleton)
	//-----------------------
	// µ¿ÀÛ °³¼ö Á¤ÀÇ
	m_CreatureFPK[1].Init(ACTION_MAX);

	// °¢ µ¿ÀÛ¿¡ 8¹æÇâÀÇ FrameArray°¡ ÀÖ´Ù.
	for (k=0; k<ACTION_MAX; k++)
		m_CreatureFPK[1][k].Init(8);	

	// sprite ¼ø¼­
	int step[] = { 2,3,4,3,2,1,0,1 };


	// n¹øÂ° Sprite
	n = 64;

	// 0~8¹øÂ° ¹æÇâ¿¡´Â °¢°¢ 8°³ÀÇ FrameÀÌ ÀÖ´Ù.
	for (i=0; i<8; i++)
	{
		for (k=0; k<ACTION_MAX; k++)
			m_CreatureFPK[1][k][i].Init(8);		
		
		// 8°³ÀÇ Frame¿¡ ´ëÇÑ Á¤º¸¸¦ SetÇÑ´Ù.
		for (j=0; j<8; j++)
		{			
			for (k=0; k<ACTION_MAX; k++)			
				m_CreatureFPK[1][k][i][j].Set(n,step[j], 0,-40);
		}  

		n += 5;
	}	


	//-----------------------
	// ¼¼¹øÂ° Creature(Vamp1)
	//-----------------------
	// µ¿ÀÛ °³¼ö Á¤ÀÇ
	m_CreatureFPK[2].Init(ACTION_MAX_VAMPIRE);

	// n¹øÂ° Sprite
	n = 104;
	
	// °¢ µ¿ÀÛ¿¡ 8¹æÇâÀÇ FrameArray°¡ ÀÖ´Ù.
	for (k=0; k<ACTION_MAX_VAMPIRE; k++)
		m_CreatureFPK[2][k].Init(8);

	//------------------------------------------------
	// ACTION_STAND - 8¹æÇâ¿¡ ´ëÇØ¼­
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// ¹æÇâ´ç Frame¼ö 
		m_CreatureFPK[2][ACTION_STAND][i].Init(16);
		
		int index = 0;
		// 4°³ÀÇ Frame¿¡ ´ëÇÑ Á¤º¸¸¦ SetÇÑ´Ù.
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
	// ACTION_MOVE - 8¹æÇâ¿¡ ´ëÇØ¼­
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// ¹æÇâ´ç Frame¼ö 
		m_CreatureFPK[2][ACTION_MOVE][i].Init(8);
		
		// 8°³ÀÇ Frame¿¡ ´ëÇÑ Á¤º¸¸¦ SetÇÑ´Ù.
		for (j=0; j<7; j++)
		{
			// 0~7 frame
			m_CreatureFPK[2][ACTION_MOVE][i][j].Set(n, 15,-60);
			n ++;
		}
		// 8¹øÂ° frame
		m_CreatureFPK[2][ACTION_MOVE][i][7].Set(n-7, 15,-60);
	}

	//------------------------------------------------
	// ACTION_ATTACK - 8¹æÇâ¿¡ ´ëÇØ¼­
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// ¹æÇâ´ç Frame¼ö 
		m_CreatureFPK[2][ACTION_ATTACK][i].Init(8);
		
		// 8°³ÀÇ Frame¿¡ ´ëÇÑ Á¤º¸¸¦ SetÇÑ´Ù.
		for (j=0; j<8; j++)
		{
			m_CreatureFPK[2][ACTION_ATTACK][i][j].Set(n, 15,-60);
			n ++;
		}
	}

	//------------------------------------------------
	// ACTION_MAGIC - 8¹æÇâ¿¡ ´ëÇØ¼­
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// ¹æÇâ´ç Frame¼ö 
		m_CreatureFPK[2][ACTION_MAGIC][i].Init(8);
		
		// 8°³ÀÇ Frame¿¡ ´ëÇÑ Á¤º¸¸¦ SetÇÑ´Ù.
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
	// ACTION_DAMAGED - 8¹æÇâ¿¡ ´ëÇØ¼­
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// ¹æÇâ´ç Frame¼ö 
		m_CreatureFPK[2][ACTION_DAMAGED][i].Init(6);
		
		// 6°³ÀÇ Frame¿¡ ´ëÇÑ Á¤º¸¸¦ SetÇÑ´Ù.
		for (j=0; j<6; j++)
		{
			m_CreatureFPK[2][ACTION_DAMAGED][i][j].Set(n, 15,-60);
			n ++;
		}
	}

	//------------------------------------------------
	// ACTION_DRAINED - 8¹æÇâ¿¡ ´ëÇØ¼­
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// ¹æÇâ´ç Frame¼ö 
		m_CreatureFPK[2][ACTION_DRAINED][i].Init(7);
		
		// 7°³ÀÇ Frame¿¡ ´ëÇÑ Á¤º¸¸¦ SetÇÑ´Ù.
		for (j=0; j<5; j++)
		{
			m_CreatureFPK[2][ACTION_DRAINED][i][j].Set(n, 15,-60);
			n ++;
		}

		m_CreatureFPK[2][ACTION_DRAINED][i][5].Set(n-3, 15,-60);
		m_CreatureFPK[2][ACTION_DRAINED][i][6].Set(n-4, 15,-60);
	}

	//------------------------------------------------
	// ACTION_DIE - 8¹æÇâ¿¡ ´ëÇØ¼­
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// ¹æÇâ´ç Frame¼ö 
		m_CreatureFPK[2][ACTION_DIE][i].Init(4);
		
		// 4°³ÀÇ Frame¿¡ ´ëÇÑ Á¤º¸¸¦ SetÇÑ´Ù.
		for (j=0; j<4; j++)
		{
			m_CreatureFPK[2][ACTION_DIE][i][j].Set(n, 15,-60);
			//n ++;
		}
	}

	//------------------------------------------------
	// ACTION_VAMPIRE_DRAIN - 8¹æÇâ¿¡ ´ëÇØ¼­
	//------------------------------------------------
	for (i=0; i<8; i++)
	{
		// ¹æÇâ´ç Frame¼ö 
		m_CreatureFPK[2][ACTION_VAMPIRE_DRAIN][i].Init(7);
		
		// 7°³ÀÇ Frame¿¡ ´ëÇÑ Á¤º¸¸¦ SetÇÑ´Ù.
		for (j=0; j<5; j++)
		{
			m_CreatureFPK[2][ACTION_VAMPIRE_DRAIN][i][j].Set(n, 15,-60);
			n ++;
		}

		m_CreatureFPK[2][ACTION_VAMPIRE_DRAIN][i][5].Set(n-3, 15,-60);
		m_CreatureFPK[2][ACTION_VAMPIRE_DRAIN][i][6].Set(n-4, 15,-60);
	}

	
	std::ofstream packFile(FILE_CFRAME_CREATURE, ios::binary);
	std::ofstream indexFile(FILE_CFRAMEINDEX_CREATURE, ios::binary);
	m_CreatureFPK.SaveToFile(packFile, indexFile);	
	packFile.close();
	indexFile.close();	
	*/
	
	///*
	// Load from File
	std::ifstream file;//(FILE_CFRAME_CREATURE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_CREATURE").c_str(), file))
		return false;
	m_CreatureFPK.LoadFromFile(file);
	file.close();
	//*/	
	DrawTitleLoading();
	//m_CreatureFPK.InfoToFile("log\\CreatureFPK.txt");

	// Frame2 (Á»ºñ)ÀÇ Á×±â µ¿ÀÛÀ» yÁÂÇ¥ ,40¾¿ ÇØÁØ´Ù.
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
	
	std::ofstream packFile(FILE_CFRAME_CREATURE, ios::binary);
	std::ofstream indexFile(FILE_CFRAMEINDEX_CREATURE, ios::binary);
	m_CreatureFPK.SaveToFile(packFile, indexFile);	
	packFile.close();
	indexFile.close();
	*/


	/*
	// Damaged 0,1À» 1,0À¸·Î ¹Ù²Û´Ù.
	for (int ct=0; ct<m_CreatureFPK.GetSize(); ct++)
	{
		DIRECTION_FRAME_ARRAY& damaged = m_CreatureFPK[ct][ACTION_DAMAGED];

		for (int d=0; d<8; d++)
		{
			FRAME_ARRAY& dfr = damaged[d];

			// 0°ú 1 framdÀ» ¹Ù²ãÁØ´Ù.
			CFrame frame0 = dfr[0];
			CFrame frame1 = dfr[1];

			CFrame temp = frame0;			
			frame0 = frame1;
			frame1 = temp;
		}
	}
	*/

	//m_CreatureFPK.InfoToFile("creature.txt");

	
	// Á¤Áö 4frameÀ» 6frameÀ¸·Î ¹Ù²ã¼­ ´Ù½Ã 18frameÀ¸·Î..
	/*
	DIRECTION_FRAME_ARRAY& stand = m_CreatureFPK[1][ACTION_STAND];
	DIRECTION_FRAME_ARRAY standTemp;
	DIRECTION_FRAME_ARRAY standTemp2;
	standTemp = stand;
	standTemp2.Init( 8 );
	for (int d=0; d<8; d++)
	{		
		// 4frameÀ»  6frameÀ¸·Î..		
		standTemp2[d].Init( standTemp[d].GetSize(),2 );
		for (int f=0; f<standTemp[d].GetSize(); f++)
		{
			standTemp2[d][f] = standTemp[d][f];			
		}
		standTemp2[d][4] = standTemp[d][2];	
		standTemp2[d][5] = standTemp[d][1];	

		// 6frameÀ» 18frameÀ¸·Î..
		stand[d].Init( standTemp2[d].GetSize()*3 );
		int ff=0;
		for (f=0; f<standTemp2[d].GetSize(); f++)
		{
			stand[d][ff++] = standTemp2[d][f];			
			stand[d][ff++] = standTemp2[d][f];
			stand[d][ff++] = standTemp2[d][f];
		}
	}	

	// °È±â µ¿ÀÛ 6frameÀ» 12frameÀ¸·Î...
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

	std::ofstream packFile(FILE_CFRAME_CREATURE, ios::binary);
	std::ofstream indexFile(FILE_CFRAMEINDEX_CREATURE, ios::binary);
	m_CreatureFPK.SaveToFile(packFile, indexFile);	
	packFile.close();
	indexFile.close();	
	*/

	//------------------------------------------------------------
	//
	// Creature Shadow FPK - Loading
	//
	//------------------------------------------------------------
	std::ifstream fileShadow;//(FILE_CFRAME_CREATURE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_CREATURE_SHADOW").c_str(), fileShadow))
		return false;
	m_CreatureShadowFPK.LoadFromFile(fileShadow);
	fileShadow.close();
	DrawTitleLoading();
	//m_CreatureShadowFPK.InfoToFile("log\\CreatureShadowFPK.txt");
	
	//------------------------------------------------------------
	//
	//  addonÀÇ Frame Á¤º¸¸¦ »ý¼ºÇÑ´Ù.
	//
	//------------------------------------------------------------
	//
	/*
	m_AddonFPK.Init(MAX_CREATURE_ADDON);
	

	//--------------
	// Ã¹¹øÂ° ¿Ê
	//--------------
	// 1°¡Áö µ¿ÀÛ¸¸ ÀÖ´Ù.
	m_AddonFPK[0].Init(1);
	m_AddonFPK[1].Init(1);
	m_AddonFPK[2].Init(1);
	m_AddonFPK[3].Init(1);

	// ±× 1°¡Áö µ¿ÀÛ¿¡ 8¹æÇâÀÇ FrameArray°¡ ÀÖ´Ù.
	m_AddonFPK[0][0].Init(8);
	m_AddonFPK[1][0].Init(8);
	m_AddonFPK[2][0].Init(8);
	m_AddonFPK[3][0].Init(8);

	// n¹øÂ° tile
	//n = n + 80;
	n = 0;

	// 0~8¹øÂ° ¹æÇâ¿¡´Â °¢°¢ 8°³ÀÇ FrameÀÌ ÀÖ´Ù.
	for (i=0; i<8; i++)
	{
		m_AddonFPK[0][0][i].Init(8);
		m_AddonFPK[1][0][i].Init(8);
		m_AddonFPK[2][0][i].Init(8);
		m_AddonFPK[3][0][i].Init(8);
		
		// 8°³ÀÇ Frame¿¡ ´ëÇÑ Á¤º¸¸¦ SetÇÑ´Ù.
		for (int j=0; j<8; j++)
		{
			// 83~122 : Shirt ±×¸²ÀÌ ÀÖ´Ù°í ÇßÀ» °æ¿ì
			m_AddonFPK[0][0][i][j].Set(n,step[j], 0,-40);

			// 123~162 Pants ±×¸²ÀÌ ÀÖ´Ù°í ÇßÀ» °æ¿ì
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
	
	std::ifstream AdvancementOustersFile;//(FILE_CFRAME_ADDON_MALE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_OUSTERS").c_str(), AdvancementOustersFile))
		return false;
	m_AdvancementOustersFPK.LoadFromFile(AdvancementOustersFile);
	AdvancementOustersFile.close();	

	std::ifstream AdvancementOustersShadowFile;//(FILE_CFRAME_ADDON_MALE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_OUSTERS_SHADOW").c_str(), AdvancementOustersFile))
		return false;
	m_AdvancementOustersShadowFPK.LoadFromFile(AdvancementOustersFile);
	AdvancementOustersFile.close();	

	std::ifstream AdvancementVampireManFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_VAMPIRE_MAN").c_str(), 
		AdvancementVampireManFile))
		return false;
	m_AdvancementVampireManFPK.LoadFromFile( AdvancementVampireManFile );
	AdvancementVampireManFile.close();


	std::ifstream AdvancementVampireManShadowFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_VAMPIRE_MAN_SHADOW").c_str(), 
		AdvancementVampireManShadowFile))
		return false;
	m_AdvancementVampireManShadowFPK.LoadFromFile( AdvancementVampireManShadowFile );
	AdvancementVampireManShadowFile.close();

	std::ifstream AdvancementVampireWomanFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_VAMPIRE_WOMAN").c_str(), 
		AdvancementVampireWomanFile))
		return false;
	m_AdvancementVampireWomanFPK.LoadFromFile( AdvancementVampireWomanFile );
	AdvancementVampireWomanFile.close();

	std::ifstream AdvancementVampireWomanShadowFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_VAMPIRE_WOMAN_SHADOW").c_str(), 
		AdvancementVampireWomanShadowFile))
		return false;
	m_AdvancementVampireWomanShadowFPK.LoadFromFile( AdvancementVampireWomanShadowFile );
	AdvancementVampireWomanShadowFile.close();


	std::ifstream AdvancementSlayerManFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_SLAYER_MAN").c_str(), 
		AdvancementSlayerManFile))
		return false;
	m_AdvancementSlayerManFPK.LoadFromFile( AdvancementSlayerManFile );
	AdvancementSlayerManFile.close();

	std::ifstream AdvancementSlayerManShadowFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_SLAYER_MAN_SHADOW").c_str(), 
		AdvancementSlayerManShadowFile))
		return false;
	m_AdvancementSlayerManShadowFPK.LoadFromFile( AdvancementSlayerManShadowFile );
	AdvancementSlayerManShadowFile.close();


	std::ifstream AdvancementSlayerWomanFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_SLAYER_WOMAN").c_str(), 
		AdvancementSlayerWomanFile))
		return false;
	m_AdvancementSlayerWomanFPK.LoadFromFile( AdvancementSlayerWomanFile );
	AdvancementSlayerWomanFile.close();

	std::ifstream AdvancementSlayerWomanShadowFile;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_SLAYER_WOMAN_SHADOW").c_str(), 
		AdvancementSlayerWomanShadowFile))
		return false;
	m_AdvancementSlayerWomanShadowFPK.LoadFromFile( AdvancementSlayerWomanShadowFile );
	AdvancementSlayerWomanShadowFile.close(); 

	/*
	//------------------------------------------------
	// Load from File
	//------------------------------------------------
	std::ifstream AdvancementSlayerFile;//(FILE_CFRAME_ADDON_MALE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_SLAYER").c_str(), AdvancementSlayerFile))
		return false;
	m_AdvancementSlayerFPK.LoadFromFile(AdvancementSlayerFile);
	AdvancementSlayerFile.close();

	std::ifstream AdvancementVampireFile;//(FILE_CFRAME_ADDON_MALE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_VAMPIRE").c_str(), AdvancementVampireFile))
		return false;
	m_AdvancementVampireFPK.LoadFromFile(AdvancementVampireFile);
	AdvancementVampireFile.close();
	
	std::ifstream AdvancementSlayerShadowFile;//(FILE_CFRAME_ADDON_MALE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_SLAYER_SHADOW").c_str(), AdvancementSlayerFile))
		return false;
	m_AdvancementSlayerShadowFPK.LoadFromFile(AdvancementSlayerFile);
	AdvancementSlayerFile.close();

	std::ifstream AdvancementVampireShadowFile;//(FILE_CFRAME_ADDON_MALE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADVANCEMENT_CLASS_VAMPIRE_SHADOW").c_str(), AdvancementVampireFile))
		return false;
	m_AdvancementVampireShadowFPK.LoadFromFile(AdvancementVampireFile);
	AdvancementVampireFile.close();

*/
	//------------------------------------------------
	// Load from File
	//------------------------------------------------
	std::ifstream AddonFile2;//(FILE_CFRAME_ADDON_MALE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADDON").c_str(), AddonFile2))
		return false;
	m_AddonFPK.LoadFromFile(AddonFile2);
	AddonFile2.close();
	std::ifstream OustersFile2;//(FILE_CFRAME_ADDON_MALE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_OUSTERS").c_str(), OustersFile2))
		return false;
	m_OustersFPK.LoadFromFile(OustersFile2);
	OustersFile2.close();
	DrawTitleLoading();

	//------------------------------------------------
	// ±×¸²ÀÚ - Load from File
	//------------------------------------------------
	std::ifstream AddonShadowFile2;//(FILE_CFRAME_ADDON_MALE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_ADDON_SHADOW").c_str(), AddonShadowFile2))
		return false;
	m_AddonShadowFPK.LoadFromFile(AddonShadowFile2);
	AddonShadowFile2.close();
	
	std::ifstream OustersShadowFile2;//(FILE_CFRAME_ADDON_MALE, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_CFRAME_OUSTERS_SHADOW").c_str(), OustersShadowFile2))
		return false;
	m_OustersShadowFPK.LoadFromFile(OustersShadowFile2);
	OustersShadowFile2.close();
	

	DrawTitleLoading();

	// ÃÑµ¿ÀÛ¿¡ ¸Ó¸®Ä«¶ô ºüÁø°Í Ãß°¡
	// ÀÌ·¡µµ ¹º°¡ ¾ÈµÇ³×!!
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

	// º¯°æ..
	/*
	int add, a, d, f;
	for (add=0; add<MAX_ADDONID_MALE; add++)
	{
		ACTION_FRAME_ARRAY& AFA = m_AddonMaleFPK[add];

		// motorcycle¿¡´Â actionÃß°¡
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
					
					// motorµ¿ÀÛÀ» 3 --> 4 frameÀ¸·Î ¹Ù²Û´Ù.
					if (a==ACTION_SLAYER_MOTOR_MOVE)
					{
						if (FA.GetSize()!=0)
						{
							FRAME_ARRAY newFA;
							newFA.Init( 4 );
							// 3°³Â¥¸® FA¸¦ 4°³·Î ¸¸µé¾î¾ß ÇÑ´Ù.
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
	// [³²ÀÚ] Ä®µ¿ÀÛÀº 6 frameÀ¸·Î ¹Ù²Ù±â...
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
	
	// [¿©ÀÚ] Ä®µ¿ÀÛÀº 6 frameÀ¸·Î ¹Ù²Ù±â...
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

	std::ofstream packFileMale(FILE_CFRAME_ADDON_MALE, ios::binary);
	std::ofstream indexFileMale(FILE_CFRAMEINDEX_ADDON_MALE, ios::binary);
	m_AddonMaleFPK.SaveToFile(packFileMale, indexFileMale);	
	packFileMale.close();
	indexFileMale.close();

	std::ofstream packFileFemale(FILE_CFRAME_ADDON_FEMALE, ios::binary);
	std::ofstream indexFileFemale(FILE_CFRAMEINDEX_ADDON_FEMALE, ios::binary);
	m_AddonFemaleFPK.SaveToFile(packFileFemale, indexFileFemale);	
	packFileFemale.close();
	indexFileFemale.close();

	*/

	
	// ¿©ÀÚ ÃÑ µ¿ÀÛ ¹Ù²Ù±â
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
// CImageFramePack			m_ItemTileFPK;			// Tile¿¡ ÀÖ´Â Item¿¡ ´ëÇÑ..
// m_ImageFrame[Á¾·ù]
//----------------------------------------------------------------------
bool
MTopView::InitImageFrames()
{
	//------------------------------------------------------------
	//
	//  Item on Tile Á¤º¸¸¦ »ý¼ºÇÑ´Ù.
	//
	//------------------------------------------------------------
	/*
	m_ItemTileFPK.Init( 28 );
	
	// Item 0	
	for (int i=0; i<28; i++)
		m_ItemTileFPK[i].Set(i, 10, 10);

	std::ofstream packFile(FILE_IFRAME_ITEMTILE, ios::binary);
	std::ofstream indexFile(FILE_IFRAMEINDEX_ITEMTILE, ios::binary);

	m_ItemTileFPK.SaveToFile(packFile, indexFile);

	packFile.close();
	indexFile.close();
	*/

	///*
	// Load from File
	std::ifstream file2;//(FILE_IFRAME_ITEMTILE, ios::binary);
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
// m_ImageObjectFPK[Á¾·ù][Frame]
//----------------------------------------------------------------------
bool
MTopView::InitAnimationFrames()
{		
	//------------------------------------------------------------
	//
	//  Animation Frame Á¤º¸¸¦ »ý¼ºÇÑ´Ù.
	//
	//------------------------------------------------------------

	//------------------------------------------------------------
	//
	//  ÀÌ Á¤º¸´Â FrameSetÀÌ´Ù.
	//
	//  SpriteSet¿¡¼­ÀÇ SpriteID¸¦ ÀúÀåÇÏ°í ÀÖ¾î¾ß ÇÑ´Ù.
	//
	//------------------------------------------------------------
	
	std::ifstream file2;//(FILE_AFRAME_ANIMATIONOBJECT, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_AFRAME_ANIMATIONOBJECT").c_str(), file2))
		return false;
	m_ImageObjectFPK.LoadFromFile(file2);
	file2.close();

	std::ifstream file3;//(FILE_AFRAME_ANIMATIONOBJECT, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_AFRAME_ANIMATIONOBJECT_SHADOW").c_str(), file3))
		return false;
	m_ImageObjectShadowFPK.LoadFromFile(file3);
	file3.close();	

//#ifdef OUTPUT_DEBUG
//	m_InteractionObjectFPK.Init( 1 );
//	
//	//------------------------------------------
//	// Ã¹¹øÂ° Animation Object
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
//	std::ofstream iopackFile(g_pFileDef->getProperty("FILE_AFRAME_INTERACTIONOBJECT").c_str(), ios::binary);
//	std::ofstream ioindexFile(g_pFileDef->getProperty("FILE_AFRAMEINDEX_INTERACTIONOBJECT").c_str(), ios::binary);
//	m_InteractionObjectFPK.SaveToFile(iopackFile, ioindexFile);
//	iopackFile.close();
//	ioindexFile.close();	
//#endif

	///*
	// Load from File
//	std::ifstream iofile2;//(FILE_AFRAME_ANIMATIONOBJECT, ios::binary);
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
	std::ifstream itemdropfile2;//(FILE_AFRAME_ANIMATIONOBJECT, ios::binary);
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
//	std::ofstream itemdropfile3(g_pFileDef->getProperty("FILE_AFRAME_ITEMDROP").c_str(), ios::binary);
//	std::ofstream itemdropfile4(g_pFileDef->getProperty("FILE_AFRAMEINDEX_ITEMDROP").c_str(), ios::binary);
//	m_ItemDropFPK.SaveToFile(itemdropfile3, itemdropfile4);
//	itemdropfile4.close();
//	itemdropfile3.close();

	//------------------------------------------------------------	
	// ´Ù½Ã »ý¼º
	//------------------------------------------------------------	
	/*
	const int numItems	= 121;
	const int numFrames	= 6;

	// frame º¯È­ °ª 
	// sprite´Â item¸¶´Ù 4°³¾¿ÀÎµ¥ 6 frameÀ» ¸¸µé¾î¾ß ÇÑ´Ù.
	int nFrame[numFrames] = { 3, 0, 1, 2, 3, 0 };

	m_ItemDropFPK.Init( numItems );	// ÀüÃ¼ item °³¼ö¸¸Å­ÀÇ ani frame

	int spriteID = 0;
	for (int i=0; i<numItems; i++)
	{
		m_ItemDropFPK[i].Init( numFrames );		// °¢°¢ 6 frame¾¿

		for (int f=0; f<numFrames; f++)
		{
			int sid = spriteID+nFrame[f];

			// Áß½É¿¡ ¿Àµµ·Ï ÁÂÇ¥ º¸Á¤..
			int cx = 24 - (m_ItemDropSPK[sid].GetWidth()>>1);
			int cy = 24 - (m_ItemDropSPK[sid].GetHeight()>>1);

			m_ItemDropFPK[i][f].Set( sid, cx, cy );
		}
		
		spriteID += 4;
	}

	//------------------------------------------------------------	
	// ÀúÀå
	//------------------------------------------------------------	
	std::ofstream packFile(FILE_AFRAME_ITEMDROP, ios::binary);
	std::ofstream indexFile(FILE_AFRAMEINDEX_ITEMDROP, ios::binary);
	m_ItemDropFPK.SaveToFile(packFile, indexFile);
	packFile.close();
	indexFile.close();
	*/

		//------------------------------------------------------------
	//
	//  Item Broken Á¤º¸¸¦ »ý¼ºÇÑ´Ù.
	//
	//------------------------------------------------------------
	///*
#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
	const int maxSlayerItemBroken = (MSlayerGear::MAX_GEAR_SLAYER+2-12) * 3;
	const int maxVampireItemBroken = (MVampireGear::MAX_GEAR_VAMPIRE+2-12) * 3;
	const int maxOustersItemBroken = (MOustersGear::MAX_GEAR_OUSTERS+1-12) * 3;

	m_ItemBrokenFPK.Init( 5 );	// ½½·¹ÀÌ¾î ³²,¿© + ¹ìÆÄÀÌ¾î ³²,¿©


	// { SpriteID, CX, CY }
	//------------------------------------------------------------
	// ½½·¹ÀÌ¾î ³²ÀÚ
	//------------------------------------------------------------
	int slayerMale[maxSlayerItemBroken][3] =
	{
		// Á¤»óÀûÀÎ°Å
		{ SPRITEID_NULL,	33, 1 },	// ¸ðÀÚ,
		{ SPRITEID_NULL,	 38, 18 },	// ¸ñ°ÉÀÌ
		{ SPRITEID_NULL,	16, 19 },	// »óÀÇ
		{ 1,	62, 19 },	// ¹æÆÐ	
		{ 2,	19, 11 },	// Ä®		
		{ SPRITEID_NULL,	9, 49 },	// Àå°©
		{ SPRITEID_NULL,	27, 54 },	// º§Æ®
		{ SPRITEID_NULL,	26, 59 },	// ÇÏÀÇ
		{ 5,	59, 51 },	// ÆÈÂî2		
		{ 5,	24, 51 },	// ÆÈÂî1
		{ 6,	20, 60 },	// ¹ÝÁö1
		{ 6,	28, 60 },	// ¹ÝÁö2
		{ 6,	55, 60 },	// ¹ÝÁö3
		{ 6,	63, 60 },	// ¹ÝÁö4
		{ SPRITEID_NULL,	20, 122 },	// ½Å¹ß
		{ 3,	18, 12 },	// ½ÊÀÚ°¡
		{ 4,	20, 12 },	// ÃÑ

		// ¾à°£ ºÎ¼­Áø°Å
		{ 9,	40, 10 },	// ¸ðÀÚ,
		{ 10,	42, 19 },	// ¸ñ°ÉÀÌ
		{ 7,	33, 18 },	// »óÀÇ
		{ 14,	62, 19 },	// ¹æÆÐ	
		{ 15,	19, 11 },	// Ä®		
		{ 11,	30, 35 },	// Àå°©
		{ 12,	37, 36 },	// º§Æ®
		{ 8,	36, 35 },	// ÇÏÀÇ
		{ 18,	59, 51 },	// ÆÈÂî2		
		{ 18,	24, 51 },	// ÆÈÂî1
		{ 19,	20, 60 },	// ¹ÝÁö1
		{ 19,	28, 60 },	// ¹ÝÁö2
		{ 19,	55, 60 },	// ¹ÝÁö3
		{ 19,	63, 60 },	// ¹ÝÁö4
		{ 13,	36, 72 },	// ½Å¹ß
		{ 16,	18, 12 },	// ½ÊÀÚ°¡
		{ 17,	20, 12 },	// ÃÑ

		// ´Ù ºÎ¼­Á® °¡´Â°Å
		{ 22,	40, 10 },	// ¸ðÀÚ,
		{ 23,	42, 19 },	// ¸ñ°ÉÀÌ
		{ 20,	33, 18 },	// »óÀÇ
		{ 27,	62, 19 },	// ¹æÆÐ	
		{ 28,	19, 11 },	// Ä®		
		{ 24,	30, 35 },	// Àå°©
		{ 25,	37, 36 },	// º§Æ®
		{ 21,	36, 35 },	// ÇÏÀÇ
		{ 31,	59, 51 },	// ÆÈÂî2		
		{ 31,	24, 51 },	// ÆÈÂî1
		{ 32,	20, 60 },	// ¹ÝÁö1
		{ 32,	28, 60 },	// ¹ÝÁö2
		{ 32,	55, 60 },	// ¹ÝÁö3
		{ 32,	63, 60 },	// ¹ÝÁö4
		{ 26,	36, 72 },	// ½Å¹ß
		{ 29,	18, 12 },	// ½ÊÀÚ°¡
		{ 30,	20, 12 },	// ÃÑ
	};

	//------------------------------------------------------------
	// ½½·¹ÀÌ¾î ¿©ÀÚ
	//------------------------------------------------------------
	int slayerFemale[maxSlayerItemBroken][3] =
	{
		// Á¤»óÀûÀÎ°Å
		{ SPRITEID_NULL,	33, 1 },	// ¸ðÀÚ,
		{ SPRITEID_NULL,	 38, 18 },	// ¸ñ°ÉÀÌ
		{ SPRITEID_NULL,	16, 19 },	// »óÀÇ
		{ 1,	62, 19 },	// ¹æÆÐ	
		{ 2,	19, 11 },	// Ä®		
		{ SPRITEID_NULL,	9, 49 },	// Àå°©
		{ SPRITEID_NULL,	27, 54 },	// º§Æ®
		{ SPRITEID_NULL,	26, 59 },	// ÇÏÀÇ
		{ 5,	59, 51 },	// ÆÈÂî2		
		{ 5,	24, 51 },	// ÆÈÂî1
		{ 6,	20, 60 },	// ¹ÝÁö1
		{ 6,	28, 60 },	// ¹ÝÁö2
		{ 6,	55, 60 },	// ¹ÝÁö3
		{ 6,	63, 60 },	// ¹ÝÁö4
		{ SPRITEID_NULL,	20, 122 },	// ½Å¹ß
		{ 3,	18, 12 },	// ½ÊÀÚ°¡
		{ 4,	20, 12 },	// ÃÑ

		// ¾à°£ ºÎ¼­Áø°Å
		{ 36,	40, 10 },	// ¸ðÀÚ,
		{ 37,	41, 20 },	// ¸ñ°ÉÀÌ
		{ 34,	33, 20 },	// »óÀÇ
		{ 14,	62, 19 },	// ¹æÆÐ	
		{ 15,	19, 11 },	// Ä®		
		{ 38,	30, 37 },	// Àå°©
		{ 39,	40, 32 },	// º§Æ®
		{ 35,	38, 35 },	// ÇÏÀÇ
		{ 18,	59, 51 },	// ÆÈÂî2		
		{ 18,	24, 51 },	// ÆÈÂî1
		{ 19,	20, 60 },	// ¹ÝÁö1
		{ 19,	28, 60 },	// ¹ÝÁö2
		{ 19,	55, 60 },	// ¹ÝÁö3
		{ 19,	63, 60 },	// ¹ÝÁö4
		{ 40,	39, 55 },	// ½Å¹ß
		{ 16,	18, 12 },	// ½ÊÀÚ°¡
		{ 17,	20, 12 },	// ÃÑ

		// ´Ù ºÎ¼­Á® °¡´Â°Å
		{ 43,	40, 10 },	// ¸ðÀÚ,
		{ 44,	41, 20 },	// ¸ñ°ÉÀÌ
		{ 41,	33, 20 },	// »óÀÇ
		{ 14,	62, 19 },	// ¹æÆÐ	
		{ 15,	19, 11 },	// Ä®		
		{ 45,	30, 37 },	// Àå°©
		{ 46,	40, 32 },	// º§Æ®
		{ 42,	38, 35 },	// ÇÏÀÇ
		{ 18,	59, 51 },	// ÆÈÂî2		
		{ 18,	24, 51 },	// ÆÈÂî1
		{ 19,	20, 60 },	// ¹ÝÁö1
		{ 19,	28, 60 },	// ¹ÝÁö2
		{ 19,	55, 60 },	// ¹ÝÁö3
		{ 19,	63, 60 },	// ¹ÝÁö4
		{ 47,	39, 55 },	// ½Å¹ß
		{ 16,	18, 12 },	// ½ÊÀÚ°¡
		{ 17,	20, 12 },	// ÃÑ
	};

	//------------------------------------------------------------
	// ¹ìÆÄÀÌ¾î ³²ÀÚ
	//------------------------------------------------------------
	int vampireMale[maxVampireItemBroken][3] =
	{
		// Á¤»óÀûÀÎ°Å
		{ SPRITEID_NULL,	40, 22 },	// ¸ñ°ÉÀÌ
		{ SPRITEID_NULL,	19, 10 },	// »óÀÇ
		{ 49,	22, 45 },	// ÆÈÂî1
		{ 49,	62, 45 },	// ÆÈÂî2
		{ 50,	18, 54 },	// ¹ÝÁö1
		{ 50,	26, 54 },	// ¹ÝÁö2
		{ 50,	58, 54 },	// ¹ÝÁö3
		{ 50,	66, 54 },	// ¹ÝÁö4
		{ 62,	22, 18 },	// ±Í°ÉÀÌ1
		{ 62,	62, 18 },	// ±Í°ÉÀÌ2
		{ 65,	16, 22 },	// Weapon1
		{ 65,	64, 22 },	// Weapon2
		{ 68,	18, 63 },	// Amulet1
		{ 68,	26, 63 },	// Amulet2
		{ 68,	58, 63 },	// Amulet3
		{ 68,	66, 63 },	// Amulet4

		// ¾à°£ ºÎ¼­Áø°Å
		{ 52,	43, 18 },	// ¸ñ°ÉÀÌ
		{ 51,	30, 14 },	// »óÀÇ
		{ 53,	22, 45 },	// ÆÈÂî1
		{ 53,	62, 45 },	// ÆÈÂî2
		{ 54,	18, 54 },	// ¹ÝÁö1
		{ 54,	26, 54 },	// ¹ÝÁö2
		{ 54,	58, 54 },	// ¹ÝÁö3
		{ 54,	66, 54 },	// ¹ÝÁö4
		{ 63,	22, 18 },	// ±Í°ÉÀÌ1
		{ 63,	62, 18 },	// ±Í°ÉÀÌ2
		{ 66,	16, 22 },	// Weapon1
		{ 66,	64, 22 },	// Weapon2
		{ 69,	18, 63 },	// Amulet1
		{ 69,	26, 63 },	// Amulet2
		{ 69,	58, 63 },	// Amulet3
		{ 69,	66, 63 },	// Amulet4

		// ´Ù ºÎ¼­Áø°Å
		{ 56,	43, 18 },	// ¸ñ°ÉÀÌ
		{ 55,	30, 14 },	// »óÀÇ
		{ 57,	22, 45 },	// ÆÈÂî1
		{ 57,	62, 45 },	// ÆÈÂî2
		{ 58,	18, 54 },	// ¹ÝÁö1
		{ 58,	26, 54 },	// ¹ÝÁö2
		{ 58,	58, 54 },	// ¹ÝÁö3
		{ 58,	66, 54 },	// ¹ÝÁö4
		{ 64,	22, 18 },	// ±Í°ÉÀÌ1
		{ 64,	62, 18 },	// ±Í°ÉÀÌ2
		{ 67,	16, 22 },	// Weapon1
		{ 67,	64, 22 },	// Weapon2
		{ 70,	18, 63 },	// Amulet1
		{ 70,	26, 63 },	// Amulet2
		{ 70,	58, 63 },	// Amulet3
		{ 70,	66, 63 },	// Amulet4
	};

	//------------------------------------------------------------
	// ¹ìÆÄÀÌ¾î ¿©ÀÚ
	//------------------------------------------------------------
	int vampireFemale[maxVampireItemBroken][3] =
	{
		// Á¤»óÀûÀÎ°Å
		{ SPRITEID_NULL,	40, 22 },	// ¸ñ°ÉÀÌ
		{ SPRITEID_NULL,	19, 10 },	// »óÀÇ
		{ 49,	22, 45 },	// ÆÈÂî1
		{ 49,	62, 45 },	// ÆÈÂî2
		{ 50,	18, 54 },	// ¹ÝÁö1
		{ 50,	58, 54 },	// ¹ÝÁö3
		{ 50,	26, 54 },	// ¹ÝÁö2		
		{ 50,	66, 54 },	// ¹ÝÁö4
		{ 62,	22, 18 },	// ±Í°ÉÀÌ1
		{ 62,	62, 18 },	// ±Í°ÉÀÌ2
		{ 65,	16, 22 },	// Weapon1
		{ 65,	64, 22 },	// Weapon2
		{ 68,	18, 63 },	// Amulet1
		{ 68,	26, 63 },	// Amulet2
		{ 68,	58, 63 },	// Amulet3
		{ 68,	66, 63 },	// Amulet4

		// ¾à°£ ºÎ¼­Áø°Å
		{ 52,	42, 18 },	// ¸ñ°ÉÀÌ
		{ 60,	31, 19 },	// »óÀÇ
		{ 53,	22, 45 },	// ÆÈÂî1
		{ 53,	62, 45 },	// ÆÈÂî2
		{ 54,	18, 54 },	// ¹ÝÁö1
		{ 54,	58, 54 },	// ¹ÝÁö3
		{ 54,	26, 54 },	// ¹ÝÁö2		
		{ 54,	66, 54 },	// ¹ÝÁö4
		{ 63,	22, 18 },	// ±Í°ÉÀÌ1
		{ 63,	62, 18 },	// ±Í°ÉÀÌ2
		{ 66,	16, 22 },	// Weapon1
		{ 66,	64, 22 },	// Weapon2
		{ 69,	18, 63 },	// Amulet1
		{ 69,	26, 63 },	// Amulet2
		{ 69,	58, 63 },	// Amulet3
		{ 69,	66, 63 },	// Amulet4

		// ´Ù ºÎ¼­Áø°Å
		{ 56,	42, 18 },	// ¸ñ°ÉÀÌ
		{ 61,	31, 19 },	// »óÀÇ
		{ 57,	22, 45 },	// ÆÈÂî1
		{ 57,	62, 45 },	// ÆÈÂî2
		{ 58,	18, 54 },	// ¹ÝÁö1
		{ 58,	58, 54 },	// ¹ÝÁö3
		{ 58,	26, 54 },	// ¹ÝÁö2		
		{ 58,	66, 54 },	// ¹ÝÁö4
		{ 64,	22, 18 },	// ±Í°ÉÀÌ1
		{ 64,	62, 18 },	// ±Í°ÉÀÌ2
		{ 67,	16, 22 },	// Weapon1
		{ 67,	64, 22 },	// Weapon2
		{ 70,	18, 63 },	// Amulet1
		{ 70,	26, 63 },	// Amulet2
		{ 70,	58, 63 },	// Amulet3
		{ 70,	66, 63 },	// Amulet4
	};		

	//------------------------------------------------------------
	// ¾Æ¿ì½ºÅÍÁî
	//------------------------------------------------------------
	int Ousters[maxOustersItemBroken][3] =
	{
		// Á¤»óÀûÀÎ°Å
		{ 77,	39, 8 },				// ¼­Å¬¸´
		{ SPRITEID_NULL ,	34, 18 },				// ¿Ê
		{ 89,	17, 33 },				// ¿Þ¼Õ				// ¸®½ºÆ²¸´Àº 92
		{ 89,	61, 33 },				// ¿À¸¥¼Õ
		{ SPRITEID_NULL,	40, 69 },				// ½Å¹ß
		{ 74,	19, 18},				// ¾Ï½º¹êµå1
		{ 74,	57, 18},				// ¾Ï½º¹êµå2
		{ 83,	19, 63 },				// ¸µ1
		{ 83,	65, 63 },				// ¸µ2
		{ 86,	35, 22 },				// ¸ñ°ÉÀÌ1
		{ 86,	42, 22 },				// ¸ñ°ÉÀÌ2
		{ 86,	49, 22 },				// ¸ñ°ÉÀÌ3
		{ 80,	26, 63 },				// Á¤·É¼®1		
		{ 80,	58, 63 },				// Á¤·É¼®2
		{ 80,	26, 71 },				// Á¤·É¼®3
		{ 80,	58, 71 },				// Á¤·É¼® 4 
		{ 92,	61, 33 },				// ¸®½ºÆ²¸´ 

		// Áß°£
		{ 78,	40, 22 },				// ¼­Å¬¸´
		{ 72,	19, 10 },	// ¿Ê
		{ 90,	22, 45 },				// ¿Þ¼Õ				// ¸®½ºÆ²¸´Àº 92
		{ 90,	62, 45 },				// ¿À¸¥¼Õ
		{ 95,	18, 54 },				// ½Å¹ß
		{ 75,	26, 54 },				// ¾Ï½º¹êµå1
		{ 75,	58, 54 },				// ¾Ï½º¹êµå2
		{ 84,	66, 54 },				// ¸µ1
		{ 84,	22, 18 },				// ¸µ2
		{ 87,	62, 18 },				// ¸ñ°ÉÀÌ1
		{ 87,	16, 22 },				// ¸ñ°ÉÀÌ2
		{ 87,	16, 22 },				// ¸ñ°ÉÀÌ3
		
		{ 81,	26, 63 },				// Á¤·É¼®1		
		{ 81,	58, 63 },				// Á¤·É¼®2
		{ 81,	26, 71 },				// Á¤·É¼®3
		{ 81,	58, 71 },				// Á¤·É¼® 4 

		{ 93,	61, 33 },				// ¸®½ºÆ²¸´  

		// ³ª»Û°Å
		{ 79,	40, 22 },				// ¼­Å¬¸´
		{ 73,	19, 10 },	// ¿Ê
		{ 91,	22, 45 },				// ¿Þ¼Õ				// ¸®½ºÆ²¸´Àº 92
		{ 91,	62, 45 },				// ¿À¸¥¼Õ
		{ 96,	18, 54 },				// ½Å¹ß
		{ 76,	26, 54 },				// ¾Ï½º¹êµå1
		{ 76,	58, 54 },				// ¾Ï½º¹êµå2
		{ 85,	66, 54 },				// ¸µ1
		{ 85,	22, 18 },				// ¸µ2
		{ 88,	62, 18 },				// ¸ñ°ÉÀÌ1
		{ 88,	16, 22 },				// ¸ñ°ÉÀÌ2
		{ 88,	16, 22 },				// ¸ñ°ÉÀÌ3
		{ 82,	26, 63 },				// Á¤·É¼®1		
		{ 82,	58, 63 },				// Á¤·É¼®2
		{ 82,	26, 71 },				// Á¤·É¼®3
		{ 82,	58, 71 },				// Á¤·É¼® 4 
		{ 94,	61, 33 },				// ¸®½ºÆ²¸´
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
	
	std::ofstream packFile(g_pFileDef->getProperty("FILE_AFRAME_ITEMBROKEN").c_str(), ios::binary);
	std::ofstream indexFile(g_pFileDef->getProperty("FILE_AFRAMEINDEX_ITEMBROKEN").c_str(), ios::binary);

	m_ItemBrokenFPK.SaveToFile(packFile, indexFile);

	packFile.close();
	indexFile.close();
#endif
	//*/

	///*
	// Load from File
	std::ifstream fileItemBroken2;//(FILE_IFRAME_ITEMTILE, ios::binary);
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
// m_EffectAlphaFPK[Á¾·ù][Direction][Frame]
//----------------------------------------------------------------------
bool
MTopView::InitEffectFrames()
{
	//------------------------------------------------
	//
	//   Effect NormalSPK
	//
	//------------------------------------------------	
	// 0¹ø : HolyWater ³¯¾Æ°¡´Â effect
	// 1¹ø : Bomb ³¯¾Æ°¡´Â effect
	/*
	m_EffectNormalFPK.Init( 2 );

	for (int e=0; e<2; e++)
	{
		m_EffectNormalFPK[e].Init( 8 );

		for (int d=0; d<8; d++)
		{
			m_EffectNormalFPK[e][d].Init( 4 );	// °¢ 4 frame

			for (int f=0; f<4; f++)
			{
				int spriteID = e*4 + f;
				int cx = -(m_EffectNormalSPK[spriteID].GetWidth()>>1);
				int cy = -(m_EffectNormalSPK[spriteID].GetHeight()>>1);

				m_EffectNormalFPK[e][d][f].Set(spriteID, cx, cy, 0);
			}
		}	
	}

	std::ofstream packNormalFile(FILE_EFRAME_NORMALEFFECT, ios::binary);
	std::ofstream indexNormalFile(FILE_EFRAMEINDEX_NORMALEFFECT, ios::binary);
	m_EffectNormalFPK.SaveToFile(packNormalFile, indexNormalFile);
	packNormalFile.close();
	indexNormalFile.close();
	*/
	/*
	m_EffectNormalFPK.Init(MAX_NORMALEFFECT);

	//-------------------
	// Ã¹¹øÂ° Effect
	//-------------------
	// 8¹æÇâÀÌ ÀÖ´Ù.
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

	std::ofstream packNormalFile(FILE_EFRAME_NORMALEFFECT, ios::binary);
	std::ofstream indexNormalFile(FILE_EFRAMEINDEX_NORMALEFFECT, ios::binary);
	m_EffectNormalFPK.SaveToFile(packNormalFile, indexNormalFile);
	packNormalFile.close();
	indexNormalFile.close();
	*/

	//--------------------------------------------------
	// ÇÏ³ªÀÇ FRRÀ» ÀÐ¾î¼­.. 
	// EFRR·Î º¯È¯½ÃÄÑ¼­..
	// EFPK¸¦ »ý¼ºÇÑ´Ù.
	//--------------------------------------------------
	// [ TEST CODE ]
	/*
	std::ifstream fileFA;//(FILE_EFRAME_NORMALEFFECT, ios::binary);
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

	std::ofstream packNormalFile(FILE_EFRAME_NORMALEFFECT, ios::binary);
	std::ofstream indexNormalFile(FILE_EFRAMEINDEX_NORMALEFFECT, ios::binary);
	m_EffectNormalFPK.SaveToFile(packNormalFile, indexNormalFile);
	packNormalFile.close();
	indexNormalFile.close();
	*/

	///*
	// Load from NormalFile
	std::ifstream NormalFile2;//(FILE_EFRAME_NORMALEFFECT, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_EFRAME_NORMALEFFECT").c_str(), NormalFile2))
		return false;
	m_EffectNormalFPK.LoadFromFile(NormalFile2);
	NormalFile2.close();
	//*/

	// ¼¼ºÎÁ¤º¸ Ãâ·ÂÇÏ±â
	/*
	std::ofstream infoFile("Log\\EffectNormal.txt");	

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
	// Ã¹¹øÂ° Effect
	//-------------------
	// 8¹æÇâÀÌ ÀÖ´Ù.
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
		// ±× 8¹æÇâ¾¿¿¡  8 Frames
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

	std::ofstream packAlphaFile(FILE_EFRAME_ALPHAEFFECT, ios::binary);
	std::ofstream indexAlphaFile(FILE_EFRAMEINDEX_ALPHAEFFECT, ios::binary);
	m_EffectAlphaFPK.SaveToFile(packAlphaFile, indexAlphaFile);
	packAlphaFile.close();
	indexAlphaFile.close();
	*/

	///*
	// Load from AlphaFile
	std::ifstream AlphaFile2;//(FILE_EFRAME_ALPHAEFFECT, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_EFRAME_ALPHAEFFECT").c_str(), AlphaFile2))
		return false;
	//std::ifstream AlphaFile2("effect.efpk", ios::binary);
	m_EffectAlphaFPK.LoadFromFile(AlphaFile2);
	AlphaFile2.close();
	//*/

	/*
	// ¼¼ºÎ Á¤º¸ Ãâ·ÂÇÏ±â
	//m_EffectAlphaFPK.InfoToFile("Log\\Effect.txt");
	std::ofstream infoFile("Log\\Effect.txt");	

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
	// °¢ effectÀÇ frame¼ö Ãâ·ÂÇÏ±â
	//---------------------------------------------------------------
	std::ofstream file("log\\EffectList.txt");
	for (int e=0; e<m_EffectAlphaFPK.GetSize(); e++)
	{
		DIRECTION_EFFECTFRAME_ARRAY& DEA = m_EffectAlphaFPK[e];
		
		file << "[" << e << "] " << (int)DEA[0].GetSize() << endl;
	}
	file.close();
	*/
	

	//---------------------------------------------------------------
	// ºû Á¶ÀýÇÏ±â
	//---------------------------------------------------------------
	/*
	int light;
	
	// effect Á¾·ù
	for (int e=0; e<m_EffectAlphaFPK.GetSize(); e++)
	{
		DIRECTION_EFFECTFRAME_ARRAY& effect = m_EffectAlphaFPK[e];

		// ¹æÇâº°
		for (int d=0; d<8; d++)
		{
			EFFECTFRAME_ARRAY& EA = effect[d];
			
			// frameº°
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

	std::ofstream packAlphaFile(FILE_EFRAME_ALPHAEFFECT, ios::binary);
	std::ofstream indexAlphaFile(FILE_EFRAMEINDEX_ALPHAEFFECT, ios::binary);
	m_EffectAlphaFPK.SaveToFile(packAlphaFile, indexAlphaFile);
	packAlphaFile.close();
	indexAlphaFile.close();
	*/

	/*
	//---------------------------------------------------------------
	// ÇÇ sprite Áßº¹ frame³Ö±â
	//---------------------------------------------------------------
	DIRECTION_EFFECTFRAME_ARRAY& effect = m_EffectAlphaFPK[EFFECTSPRITETYPE_SLAYER_DIE];

	for (int d=0; d<8; d++)
	{
		EFFECTFRAME_ARRAY& EA = effect[d];

		int frameMax = EA.GetSize()*3 + 30;

		EFFECTFRAME_ARRAY newEA;

		newEA.Init( frameMax );

		// ¸ðµÎ ³¡ µ¿ÀÛ...
		for (int i=0; i<frameMax; i++)
		{
			newEA[i] = EA[EA.GetSize()-1];
		}
		
		// µÎframe¾¿, Á¤Áö frame °è¼Ó, ÇÑ frame°Å²Ù·Î
		for (i=0; i<EA.GetSize(); i++)
		{
			newEA[i*2] = EA[i];
			newEA[i*2,1] = EA[i];

			newEA[frameMax-1 - i] = EA[i];
		}	

		EA = newEA;
	}

	std::ofstream packAlphaFile(FILE_EFRAME_ALPHAEFFECT, ios::binary);
	std::ofstream indexAlphaFile(FILE_EFRAMEINDEX_ALPHAEFFECT, ios::binary);
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
//		std::ofstream packshadowEffectFile(g_pFileDef->getProperty("FILE_EFRAME_SHADOWEFFECT").c_str(), ios::binary);
//		std::ofstream indexshadowEffectFile(g_pFileDef->getProperty("FILE_EFRAMEINDEX_SHADOWEFFECT").c_str(), ios::binary);
//		m_EffectShadowFPK.SaveToFile(packshadowEffectFile, indexshadowEffectFile);
//		packshadowEffectFile.close();
//		indexshadowEffectFile.close();
//	#endif

	///*
	// Load from shadowEffectFile
	std::ifstream shadowEffectFile2;
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_EFRAME_SHADOWEFFECT").c_str(), shadowEffectFile2))
		return false;
	m_EffectShadowFPK.LoadFromFile(shadowEffectFile2);
	shadowEffectFile2.close();

	// Load from ScreenFile
	std::ifstream ScreenFile2;//(FILE_EFRAME_ScreenEFFECT, ios::binary);
	if (!FileOpenBinary(g_pFileDef->getProperty("FILE_EFRAME_SCREENEFFECT").c_str(), ScreenFile2))
		return false;
	//std::ifstream ScreenFile2("effect.efpk", ios::binary);
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
	/* // 2001.7.14 ÁÖ¼®Ã³¸®
	//------------------------------------------------------------
	//
	//				Minimap	
	//
	//------------------------------------------------------------
	//------------------------------------------------------------	
	// Load  Clothes SpritePack	
	//------------------------------------------------------------
	std::ifstream	MapTest1;
	if (!FileOpenBinary(filename, MapTest1))
		return;
	m_pMinimapSPR->LoadFromFile( MapTest1 );
	*/

	//------------------------------------------------------------	
	// Ç¥½ÃÇØ¾ßÇÒ ZoneInfo°¡ ÀÖ´Â °æ¿ì
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
		// SpriteÀÚÃ¼¸¦ ¹Ù²ã¹ö¸°´Ù. - -;
		//----------------------------------------------------------------
		for (spY=0; spY<spHeight; spY++)
		{
			zoneY = spY * zoneHeight / spHeight;

			spX = 0;
			pPixel = m_pMinimapSPR->GetPixelLine( spY );

			count = *pPixel++;		// ¹Ýº¹ È¸¼ö
			
			//----------------------------------------------------------------
			// ÇÑ ÁÙ¿¡ ´ëÇÑ Ã¼Å©..
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
					// ¾ÈÀüÁö´ëÀÌ¸é ³ì»öÀ¸·Î ¹ÝÅõ¸í..
					//---------------------------------------------------------
					if (pZoneInfo->IsSafeSector(zoneX, zoneY))
					{
						// 1Á¡ ³ì»öÀ¸·Î alpha blending - -;
						CSpriteSurface::memcpyAlpha(pPixel, &green, 1);
					}

					// ´ÙÀ½ Á¡
					pPixel++;
					spX++;
				}
			}
		}
	}
	*/

	/* // 2001.7.14 ÁÖ¼®Ã³¸®
	//------------------------------------------------------------
	// 3D °¡¼ÓÀÌ¸é...
	//------------------------------------------------------------
	if (CDirect3D::IsHAL())
	{
		InitMinimapTexture();
	}

	// m_SectorToMinimapWidth - ÇÑ sector´Â °¡·Î ¸î pixelÀÎ°¡?
	// m_SectorToMinimapHeight - ÇÑ sector´Â ¼¼·Î ¸î pixelÀÎ°¡?
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
//	// ½ºÇÁ¶óÀÌÆ®¸¦ ¸¸µéÀÚ by ¾¦°«
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
//	// 3D °¡¼ÓÀÌ¸é...
//	//------------------------------------------------------------
//	if (!CDirect3D::IsHAL())
//		return;
//
//	// 2001.7.14 ÁÖ¼®Ã³¸®
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
//	// texture size °áÁ¤
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
//	// ÃÊ ÇÊ»ì ´õÆ¼ Å×½ºÆ® ÄÚµå.. À½³Ä... - -;;
//	//
//	//-----------------------------------------------------------
//	//
//	// m_pMinimapTexture¿¡ minimapÀ» Ãâ·ÂÇÑ´Ù.
//	//
//	//-----------------------------------------------------------
//	//-----------------------------------------------------------
//	// 2ÀÇ ½Â¼ö¸¸ Áö¿øÇÏ´Â °æ¿ì..
//	//-----------------------------------------------------------
//	bool bDifferentSize;	// sprite¿Í textureÀÇ Å©±â°¡ ´Ù¸¥°¡?
//	if (CDirect3D::IsTexturePow2())
//	{
//		bDifferentSize = true;
//
//		// width¿Í height°¡ ´Ù¸¥ °æ¿ì...
//		// ÀÛÀº ÂÊ¿¡ ¸ÂÃç¼­ ¶È°°ÀÌ ÇÑ´Ù.
//		// Square·Î ¸ÂÃá´Ù..°í ÇÒ ¼ö ÀÖÁö.. À½ÇÏ..
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
//		// TextureÅ©±âº¸´Ù Sprite Å©±â°¡ ´õ Å« °æ¿ì..
//		//-----------------------------------------------------------
//		// Áï, ÇÏµå¿þ¾î¿¡¼­ SpriteÅ©±â¸¸Å­ÀÇ Texture¸¦ Áö¿øÇÏÁö ¸øÇÏ´Â °æ¿ìÀÌ´Ù.		
//		// shift¸¦ ÀÌ¿ëÇØ¼­ Å©±â¸¦ ÁÙÀÎ´Ù.
//		while (spWidth > width || spHeight > height)
//		{
//			smallShift ++;
//
//			spWidth >>= 1;
//			spHeight >>= 1;
//		}
//	}
//	//-----------------------------------------------------------
//	// ¾Æ¹«·± size³ª °ü°è ¾ø´Â °æ¿ì
//	//-----------------------------------------------------------
//	else
//	{
//		if (CDirect3D::IsTextureSquareOnly())
//		{
//			// width¿Í height°¡ ´Ù¸¥ °æ¿ì...
//			// Å« ÂÊ¿¡ ¸ÂÃç¼­ ¶È°°ÀÌ ÇÑ´Ù.
//			// Square·Î ¸ÂÃá´Ù..°í ÇÒ ¼ö ÀÖÁö.. À½ÇÏ..
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
//	// TextureSurface »ý¼º
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
//	// °Ë°Ô Ä¥ÇÏ´Â ºÎºÐ..
//	//---------------------------------------------------
//	//if (bDifferentSize)
//	{
//		/*
//		// Sprite°¡ Â÷ÁöÇÏ´Â ¿µ¿ªÀ» Á¦¿ÜÇÑ ºÎºÐÀ» °Ë°Ô~~
//		DWORD width2 = (width - spWidth) << 1;	// *2 
//		pSurface += spWidth;
//
//		// ¿À¸¥ÂÊ ¿·ºÎºÐ
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
//		// ¾Æ·¡ÂÊ
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
//		// Texture Surface ÃÊ±âÈ­
//		//---------------------------------------------------
//		/*// 2001.7.14 ÁÖ¼®Ã³¸®
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
//	// SpriteÃâ·Â
//	//---------------------------------------------------
//	// Å©±â¸¦ °í·ÁÇØ¼­..
//
//	 // 2001.7.14 ÁÖ¼®Ã³¸®
//	// alpha°ªÀº 16 == ¹ÝÅõ¸í
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
//	// Ãâ·ÂÇÒ¶§ÀÇ Å©±â
//	//---------------------------------------------------------------
//	spWidth = m_pMinimapSPR->GetWidth();
//	spHeight = m_pMinimapSPR->GetHeight();
//	if (CDirect3D::IsTexturePow2())	
//	{
//		width = 1;
//		height = 1;
//
//		// width¿Í height´Â spWidth¿Í spHeightº¸´Ù Ä¿¾ß ÇÑ´Ù.
//		// square·Î ¸ÂÃçÁØ´Ù.
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
//		// Å« Å©±â·Î ¸ÂÃß¾î¼­ Square·Î ¸¸µç´Ù.
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
	// Addonµµ ¾îÄÉ ÇØ¾ßµÇ´Âµ¥ - -;
	// Effectµµ... - -;

	//----------------------------------------------------------
	// ÀÌ¹Ì LoadingµÈ °Íµé Á¦°Å
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
	// Loading µÈ°Ô ¾ø´Ù°í Ç¥½Ã
	//----------------------------------------------------------
	int num = g_pCreatureSpriteTable->GetSize();

	for (int i=0; i<num; i++)
	{
		(*g_pCreatureSpriteTable)[i].bLoad = false;
	}
	
	if (bUse)
	{
		// ÀÌ¹Ì LoadingµÈ °Íµé Á¦°Å		
		std::ifstream file(FILE_CFRAME_CREATURE2, ios::binary);
		m_CreatureFPK.LoadFromFile(file);
		file.close();		
	}
	else
	{
		// ÀÌ¹Ì LoadingµÈ °Íµé Á¦°Å
		std::ifstream file(FILE_CFRAME_CREATURE, ios::binary);
		m_CreatureFPK.LoadFromFile(file);
		file.close();
	}
}
*/

//----------------------------------------------------------------------
// Load From File CreatureSPK
//----------------------------------------------------------------------
// ÇöÀç Zone¿¡¼­ ÃâÇöÇÒ Creature¿¡ ´ëÇÑ SpriteµéÀ» LoadÇÑ´Ù.
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
	// LoadµÇ¾ú´ÂÁö Ã¼Å©ÇØº»´Ù.
	//----------------------------------------------------------------------
	if ((*g_pCreatureSpriteTable)[spriteType].bLoad)
	{
		// ÀÌ¹Ì LoadµÈ °æ¿ì
	}
	//----------------------------------------------------------------------
	// LoadÇØ¾ßµÇ´Â °æ¿ì
	//----------------------------------------------------------------------
	// CreatureAction¿¡ µû¶ó¼­ g_pCreatureActionSpriteTableÀ» ÀÌ¿ëÇÒ °ÍÀÌ¹Ç·Î
	// ½ÇÁ¦·Î loadingÇÏÁø ¾Ê´Â´Ù.  2001.11.21
	//----------------------------------------------------------------------			
	else
	{
		if(g_pUserOption->IsPreLoadMonster)
		{
			
			// ÇöÀç Zone¿¡¼­ ÇÊ¿äÇÑ SpriteµéÀ» LoadÇÏ¸é µÈ´Ù.
//			std::ifstream	creatureFile;//(FILE_ISPRITE_CREATURE, ios::binary);
//			std::ifstream	creatureShadowFile;
//
//			//------------------------------------------------------------
//			// sprite load
//			//------------------------------------------------------------
//			if (!FileOpenBinary(FILE_ISPRITE_CREATURE, creatureFile))
//				return;	
			

			//------------------------------------------------------------
			// Half FrameÀ» »ç¿ëÇÒ¶§
			//------------------------------------------------------------
			if (0)//g_pUserOption->UseHalfFrame)
			{
				/*
				int frameID = (*g_pCreatureSpriteTable)[spriteType].FrameID;

				//------------------------------------------------------------	
				// ÇÊ¿äÇÑ EFPK¸¦ »ý¼ºÇÏ¸é¼­ »ç¿ëµÈ SpriteIDµµ ±¸ÇÑ´Ù.
				//------------------------------------------------------------
				COrderedList<int> intList;
				ACTION_FRAME_ARRAY& AFA = CreatureFPK[frameID];
				int numAction = AFA.GetSize();

				// orderedListÀÇ È¿À²À» ³ôÈ÷±â À§ÇØ¼­ °Å²Ù·Î ..
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

				std::ifstream CreaturePackIndexFile(FILE_ISPRITEINDEX_CREATURE, ios::binary);			
				
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
			// ÀüÃ¼ Frame ´Ù »ç¿ë
			//------------------------------------------------------------
			else
			{
				
				TYPE_SPRITEID first = (*g_pCreatureSpriteTable)[spriteType].FirstSpriteID;
				TYPE_SPRITEID last	= (*g_pCreatureSpriteTable)[spriteType].LastSpriteID;
				long			fp	= (*g_pCreatureSpriteTable)[spriteType].SpriteFilePosition;

				m_CreatureSPK.LoadFromFilePart(first, last);
				// n¹øÂ° creature load	
//				m_CreatureSPK.LoadFromFilePart(creatureFile, 
//												fp,
//												first, 
//												last
//												);
				
			}

// 			creatureFile.close();

			//------------------------------------------------------------
			// ±×¸²ÀÚ load
			//------------------------------------------------------------
//			if (!FileOpenBinary(FILE_SSPRITE_CREATURE, creatureShadowFile))
//				return;	

		
//			if (!CDirect3D::IsHAL())
			{
				//------------------------------------------------------------
				// Half Frame »ç¿ë
				//------------------------------------------------------------
				if (g_pUserOption->UseHalfFrame)
				{
				}
				//------------------------------------------------------------
				// ÀüÃ¼ Frame ´Ù »ç¿ë
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

		// LoadÇß´Ù°í Ç¥½ÃÇÑ´Ù.
		(*g_pCreatureSpriteTable)[spriteType].bLoad = TRUE;

		//--------------------------------------------------------
		// Player±×¸²ÀÏ°æ¿ì´Â Ãß°¡ÇÏÁö ¾Ê´Â´Ù.
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
		// ±×¸²ÀÚ Pack»ý¼º
		//------------------------------------------------------------
		m_CreatureSSPK.InitPart(m_CreatureSPK, 
								(*g_pCreatureTable)[n].FirstSpriteID,
								(*g_pCreatureTable)[n].LastSpriteID);

		//------------------------------------------------------------
		//
		// Init ±×¸²ÀÚ TextureSurface
		//
		//------------------------------------------------------------
		if (CDirect3D::IsHAL())
		{	
			//--------------------------------------------
			// AlphaSPKÀ» ÀÌ¿ëÇØ¼­ TexturePackÀ» »ý¼ºÇÑ´Ù.
			//--------------------------------------------
			if (!m_CreatureTPK.InitPart( m_CreatureSSPK,
										(*g_pCreatureTable)[n].FirstSpriteID,
										(*g_pCreatureTable)[n].LastSpriteID))
			{
				MessageBox(g_hWnd, "Can't Init ShadowTexture!", NULL, MB_OK);
				return;
			}

			//------------------------------------------------------------	
			// EffectAlphaSprite¸¦ ¸Þ¸ð¸®¿¡¼­ »èÁ¦ÇÑ´Ù.
			// --> TextureSurface¸¦ »ç¿ëÇÒ °ÍÀÌ¹Ç·Î.. ÇÊ¿ä°¡ ¾ø´Ù.
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
// ÇöÀç º¸ÀÌ´Â AddonÀ» LoadÇÑ´Ù.
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
//	// LoadµÇ¾ú´ÂÁö Ã¼Å©ÇØº»´Ù.
//	//----------------------------------------------------------------------
//	if ((*g_pAddonSpriteTable)[frameID].bLoad)
//	{
//		// ÀÌ¹Ì LoadµÈ °æ¿ì
//	}
//	//----------------------------------------------------------------------
//	// LoadÇØ¾ßµÇ´Â °æ¿ì
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
// ÇöÀç º¸ÀÌ´Â CreatureActionÀ» LoadÇÑ´Ù.
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
//	// LoadµÇ¾ú´ÂÁö Ã¼Å©ÇØº»´Ù.
//	//----------------------------------------------------------------------
//	if ((*g_pCreatureActionSpriteTable)[frameID].bLoad)
//	{
//		// ÀÌ¹Ì LoadµÈ °æ¿ì
//	}
//	//----------------------------------------------------------------------
//	// LoadÇØ¾ßµÇ´Â °æ¿ì
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
//		//int spriteType = frameID + 2;	// mapping tableÀ» ¸¸µé¾î¾ß ÇÏ´Âµ¥.
//										// slayer³²³à¸¦ Á¦¿ÜÇÏ¸é ¼ø¼­´ë·Î´Ù..
//
//		// loadingµÆ´Ù°í Ç¥½ÃÇØµÖ¾ß Á¸ ÀÌµ¿ ÈÄ releaseÇÒ ¼ö ÀÖ´Ù.
//		// mapÀ¸·Î ÇØ¾ßÇÑ´Ù.
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
//	// LoadµÇ¾ú´ÂÁö Ã¼Å©ÇØº»´Ù.
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
// ÇöÀç LoadµÈ Creature Sprite¸¦ ¸Þ¸ð¸® ÇØÁ¦½ÃÅ²´Ù.
//----------------------------------------------------------------------
void
MTopView::ReleaseCreatureSPK(int n)
{
//	//--------------------------------------------------------
//	// Player±×¸²ÀÏ°æ¿ì´Â Á¦°ÅÇÏÁö ¾Ê´Â´Ù.
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
//	// Player±×¸²ÀÏ°æ¿ì´Â Á¦°ÅÇÏÁö ¾Ê´Â´Ù.
//	if ((*g_pCreatureSpriteTable)[spriteType].IsPlayerSprite())
//	{
//		return;
//	}
//
//	//----------------------------------------------------------------------
//	// LoadµÇ¾ú´ÂÁö Ã¼Å©ÇØº»´Ù.
//	//----------------------------------------------------------------------
//	if ((*g_pCreatureSpriteTable)[spriteType].bLoad)
//	{
//		/*
//		// ÀÌ¹Ì LoadµÈ °æ¿ì
//		// n¹øÂ° creatureÀÇ sprite¸¦ ¸Þ¸ð¸® ÇØÁ¦
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
//		// loadÇÏÁö ¾Ê¾Ò´Ù°í Ç¥½ÃÇÑ´Ù.
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
//		// LoadµÇÁö ¾ÊÀº °æ¿ì
//	}
	//--------------------------------------------------------
	// Player±×¸²ÀÏ°æ¿ì´Â Á¦°ÅÇÏÁö ¾Ê´Â´Ù.
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
		// Player±×¸²ÀÏ°æ¿ì´Â Á¦°ÅÇÏÁö ¾Ê´Â´Ù.
		if ((*g_pCreatureSpriteTable)[spriteType].IsPlayerSprite())
		{
			return;
		}
		
		//----------------------------------------------------------------------
		// LoadµÇ¾ú´ÂÁö Ã¼Å©ÇØº»´Ù.
		//----------------------------------------------------------------------
		if ((*g_pCreatureSpriteTable)[spriteType].bLoad)
		{
		/*
		// ÀÌ¹Ì LoadµÈ °æ¿ì
		// n¹øÂ° creatureÀÇ sprite¸¦ ¸Þ¸ð¸® ÇØÁ¦
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
			
			// loadÇÏÁö ¾Ê¾Ò´Ù°í Ç¥½ÃÇÑ´Ù.
			(*g_pCreatureSpriteTable)[spriteType].bLoad = FALSE;
			
			m_listLoadedCreatureSprite.Remove( spriteType );
			
			if ((*g_pCreatureSpriteTable)[spriteType].IsMonsterSprite())
			{
				m_listLoadedMonsterSprite.Remove( spriteType );
			}
		}
		else
		{
			// LoadµÇÁö ¾ÊÀº °æ¿ì
		}
		



	}
}

//----------------------------------------------------------------------
// Add MonsterSpriteTypes
//----------------------------------------------------------------------
// Hallu¿¡¼­ ÂüÁ¶ÇÏ°Ô µÉ °ÍÀÌ´ç...
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
// list´Â »ç¿ëÇÏ´Â creatureµéÀÇ sprite type ÀÌ´Ù.
//----------------------------------------------------------------------
void
MTopView::ReleaseUselessCreatureSPKExcept(const INT_ORDERED_LIST& listUse)
{
	// Áö±ÝÀº ¸÷+NPC ½ºÇÁ¶óÀÌÆ®°¡ ¾ó¸¶ ¾øÀ¸¹Ç·Î releaseÇÏÁö ¾Ê´Â´Ù.
	//return;

	//--------------------------------------------------------
	// loadµÈ °Íµé Áß¿¡¼­ »ç¿ëÁßÀÎ°Å´Â »©°í..
	//--------------------------------------------------------
	m_listLoadedCreatureSprite -= listUse;

	//--------------------------------------------------------
	// ³ª¸ÓÁö´Â Á¦°ÅÇÑ´Ù.
	//--------------------------------------------------------
	INT_ORDERED_LIST::DATA_LIST::const_iterator iSpriteType = m_listLoadedCreatureSprite.GetIterator();

	for (int i=0; i<m_listLoadedCreatureSprite.GetSize(); i++)
	{
		int spriteType = *iSpriteType;
		
		//--------------------------------------------------------
		// Player±×¸²ÀÏ°æ¿ì´Â Á¦°ÅÇÏÁö ¾Ê´Â´Ù.
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
			// ÀÌ¹Ì LoadµÈ °æ¿ì
			// n¹øÂ° creatureÀÇ sprite¸¦ ¸Þ¸ð¸® ÇØÁ¦

			//--------------------------------------------------------
			// Creature SPK Á¦°Å
			//--------------------------------------------------------
			first = (*g_pCreatureSpriteTable)[spriteType].FirstSpriteID;
			last = (*g_pCreatureSpriteTable)[spriteType].LastSpriteID;

			if (last > first && first!=SPRITEID_NULL && last!=SPRITEID_NULL)
			{
				m_CreatureSPK.ReleasePart(first, last);
			}

			//--------------------------------------------------------
			// Creature SSPK Á¦°Å
			//--------------------------------------------------------
			first = (*g_pCreatureSpriteTable)[spriteType].FirstSpriteID;
			last = (*g_pCreatureSpriteTable)[spriteType].LastSpriteID;
			
			if (last > first && first!=SPRITEID_NULL && last!=SPRITEID_NULL)
			{			
				m_CreatureSSPK.ReleasePart( first, last );
			}
			*/
			// loadingÇÑ action¸¸ Á¦°Å
			// 2001.11.21
			int frameID = (*g_pCreatureSpriteTable)[spriteType].FrameID;

			if (frameID!=FRAMEID_NULL)
			{
				ReleaseCreatureSPK( frameID );
			}

			// loadÇÏÁö ¾Ê¾Ò´Ù°í Ç¥½ÃÇÑ´Ù.
			(*g_pCreatureSpriteTable)[spriteType].bLoad = FALSE;
		}

		iSpriteType++;
	}

	//--------------------------------------------------------
	// loadµÈ °Í ´Ù½Ã ¼³Á¤
	//--------------------------------------------------------	
	m_listLoadedCreatureSprite.Release();
	m_listLoadedMonsterSprite.Release();

	INT_ORDERED_LIST::DATA_LIST::const_iterator iUse = listUse.GetIterator();

	int i;
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
// ÀÏºÎÀÇ Tile°ú ImageObject¸¦ LoadÇÑ´Ù.
//----------------------------------------------------------------------
bool		
MTopView::LoadFromFileTileAndImageObjectSet(const CSpriteSetManager &TileSSM, const CSpriteSetManager &ImageObjectSSM)
{
	//--------------------------------------------------------
	//
	// Tile ÀÏºÎ Load
	//
	//--------------------------------------------------------
//	std::ifstream	TileSPKFile;//(FILE_SPRITE_TILE, ios::binary);	
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
	// ImageObject ÀÏºÎ Load
	//
	//--------------------------------------------------------
//	std::ifstream	ImageObjectSPKFile;//(FILE_SPRITE_IMAGEOBJECT, ios::binary);	
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
// File¿¡¼­ LargeZone¿¡¼­¸¸ »ç¿ëÇÒ TileSpriteÀ» LoadÇÑ´Ù.
//----------------------------------------------------------------------
bool
MTopView::LoadFromFileTileSPKLargeZone(std::ifstream & file)
{
	//------------------------------------------------------------
	// File¿¡¼­ TileSprite¿¡ ´ëÇÑ Á¤º¸¸¦ LoadÇÑ´Ù.
	//------------------------------------------------------------
	CSpriteFilePositionArray* pOldTileSFPA = m_pTileSFPArrayLargeZone;

	m_pTileSFPArrayLargeZone = new CSpriteFilePositionArray;
	
	m_pTileSFPArrayLargeZone->LoadFromFile( file );

	//------------------------------------------------------------
	// ÇÊ¿ä ¾ø´Â°Å Á¦°Å
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
		
		// oldTileID¿¡¼­ newTileID Á¦°ÅÇÑ´Ù.
		oldTileID -= newTileID;
		
		DEBUG_ADD( "Release Part");
		
		// ¿¹ÀüÀÇ zone¿¡¸¸ Á¸ÀçÇÏ´Â TileIDµéÀ» Á¦°ÅÇÑ´Ù.
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
	std::ifstream	TilePackFile;//(FILE_SPRITE_TILE, ios::binary);	
	if (!FileOpenBinary(FILE_SPRITE_TILE, TilePackFile))
		return false;
	
	bool bLoad = m_TileSPK.LoadFromFilePart(TilePackFile, m_pTileSFPArrayLargeZone);

	TilePackFile.close();

	return bLoad;
	*/

	/*
	// 2001.8.20 ÁÖ¼®Ã³¸®
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
// File¿¡¼­ LargeZone¿¡¼­¸¸ »ç¿ëÇÒ ImageObjectSpriteµéÀ» LoadÇÑ´Ù.
//----------------------------------------------------------------------
bool
MTopView::LoadFromFileImageObjectSPKLargeZone(std::ifstream & file)
{
	//------------------------------------------------------------
	// File¿¡¼­ ImageObjectSprite¿¡ ´ëÇÑ Á¤º¸¸¦ LoadÇÑ´Ù.
	//------------------------------------------------------------
	CSpriteFilePositionArray* pOldImageObjectSFPA = m_pImageObjectSFPArrayLargeZone;

	m_pImageObjectSFPArrayLargeZone = new CSpriteFilePositionArray;
	
	m_pImageObjectSFPArrayLargeZone->LoadFromFile( file );

	//------------------------------------------------------------
	// ÇÊ¿ä ¾ø´Â°Å Á¦°Å
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

		// oldImageObjectID¿¡¼­ newImageObjectID Á¦°ÅÇÑ´Ù.
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

		// ¿¹ÀüÀÇ zone¿¡¸¸ Á¸ÀçÇÏ´Â ImageObjectIDµéÀ» Á¦°ÅÇÑ´Ù.
		m_ImageObjectSPK.ReleasePart( oldImageObjectID );

		delete pOldImageObjectSFPA;
		pOldImageObjectSFPA = NULL;
	}

	DEBUG_ADD_FORMAT( "Load ImageObject SFPArray LargeZone : size=%d", m_pImageObjectSFPArrayLargeZone->GetSize() );
	
	//------------------------------------------------------------
	//  Load ImageObject SpriteSet
	//------------------------------------------------------------
	/*
	std::ifstream	ImageObjectPackFile;//(FILE_SPRITE_IMAGEOBJECT, ios::binary);	
	if (!FileOpenBinary(FILE_SPRITE_IMAGEOBJECT, ImageObjectPackFile))
		return false;
	
	bool bLoadOK = m_ImageObjectSPK.LoadFromFilePart(ImageObjectPackFile, m_pImageObjectSFPArrayLargeZone);

	ImageObjectPackFile.close();

	return bLoadOK;
	*/

	/*
	// 2001.8.20 ÁÖ¼®Ã³¸®
	// »õ·Î¿î SFPA »ý¼º.
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
// File¿¡¼­ SmallZone¿¡¼­¸¸ »ç¿ëÇÒ TileSpriteÀ» LoadÇÑ´Ù.
//----------------------------------------------------------------------
bool
MTopView::LoadFromFileTileSPKSmallZone(std::ifstream & file)
{
	//------------------------------------------------------------
	// File¿¡¼­ TileSprite¿¡ ´ëÇÑ Á¤º¸¸¦ LoadÇÑ´Ù.
	//------------------------------------------------------------
	CSpriteFilePositionArray* pOldTileSFPA = m_pTileSFPArraySmallZone;

	m_pTileSFPArraySmallZone = new CSpriteFilePositionArray;
	
	m_pTileSFPArraySmallZone->LoadFromFile( file );

	//------------------------------------------------------------
	// ÇÊ¿ä ¾ø´Â°Å Á¦°Å
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
		
		// oldTileID¿¡¼­ newTileID Á¦°ÅÇÑ´Ù.
		oldTileID -= newTileID;
		
		DEBUG_ADD( "Release");
		
		// ¿¹ÀüÀÇ zone¿¡¸¸ Á¸ÀçÇÏ´Â TileIDµéÀ» Á¦°ÅÇÑ´Ù.
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
	std::ifstream	TilePackFile;//(FILE_SPRITE_TILE, ios::binary);	
	if (!FileOpenBinary(FILE_SPRITE_TILE, TilePackFile))
		return false;

	bool bLoad = m_TileSPK.LoadFromFilePart(TilePackFile, m_pTileSFPArraySmallZone);

	TilePackFile.close();

	return bLoad;
	*/

	/*
	// 2001.8.20 ÁÖ¼®Ã³¸®
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
// File¿¡¼­ SmallZone¿¡¼­¸¸ »ç¿ëÇÒ ImageObjectSpriteµéÀ» LoadÇÑ´Ù.
//----------------------------------------------------------------------
bool
MTopView::LoadFromFileImageObjectSPKSmallZone(std::ifstream & file)
{
	//------------------------------------------------------------
	// File¿¡¼­ ImageObjectSprite¿¡ ´ëÇÑ Á¤º¸¸¦ LoadÇÑ´Ù.
	//------------------------------------------------------------
	CSpriteFilePositionArray* pOldImageObjectSFPA = m_pImageObjectSFPArraySmallZone;

	m_pImageObjectSFPArraySmallZone = new CSpriteFilePositionArray;
	
	m_pImageObjectSFPArraySmallZone->LoadFromFile( file );

	//------------------------------------------------------------
	// ÇÊ¿ä ¾ø´Â°Å Á¦°Å
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

		// oldImageObjectID¿¡¼­ newImageObjectID Á¦°ÅÇÑ´Ù.
		oldImageObjectID -= newImageObjectID;
		
		// ¿¹ÀüÀÇ zone¿¡¸¸ Á¸ÀçÇÏ´Â ImageObjectIDµéÀ» Á¦°ÅÇÑ´Ù.
		m_ImageObjectSPK.ReleasePart( oldImageObjectID );

		delete pOldImageObjectSFPA;
		pOldImageObjectSFPA = NULL;
	}
	

	DEBUG_ADD_FORMAT( "Load ImageObject SFPArray SmallZone : size=%d", m_pImageObjectSFPArraySmallZone->GetSize() );
	
	//------------------------------------------------------------
	//  Load ImageObject SpriteSet
	//------------------------------------------------------------
	/*
	std::ifstream	ImageObjectPackFile;//(FILE_SPRITE_IMAGEOBJECT, ios::binary);	
	if (!FileOpenBinary(FILE_SPRITE_IMAGEOBJECT, ImageObjectPackFile))
		return false;
	
	bool bLoadOK = m_ImageObjectSPK.LoadFromFilePart(ImageObjectPackFile, m_pImageObjectSFPArraySmallZone);

	ImageObjectPackFile.close();
	
	return bLoadOK;
	*/
	/*
	// 2001.8.20 ÁÖ¼®Ã³¸®
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
// SmallZone¿¡¸¸ Á¸ÀçÇÏ´Â TileµéÀ» Á¦°ÅÇÑ´Ù.
//----------------------------------------------------------------------
void	
MTopView::ReleaseTileSPKSmallZone()
{
	/*
	DEBUG_ADD( "ReleaseTileSPKSmallZone" );
	
	// m_pTileSFPArraySmallZone¿Í m_pTileSFPArrayLargeZone¸¦ sortÇÏ°í
	// SmallZone - LargeZoneÇØ¼­ ³²Àº °ÍÀ» Á¦°ÅÇÑ´Ù.
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

	// SmallZoneTileID¿¡¼­ LargeZoneTileID¸¦ Á¦°ÅÇÑ´Ù.
	SmallZoneTileID -= LargeZoneTileID;
	
	// SmallZone¿¡¸¸ Á¸ÀçÇÏ´Â TileIDµéÀ» Á¦°ÅÇÑ´Ù.
	m_TileSPK.ReleasePart( SmallZoneTileID );

	m_pTileSFPArraySmallZone->Release();
	*/
	DEBUG_ADD( "ReleaseTileSPKSmallZone OK" );
}

//----------------------------------------------------------------------
// Release ImageObjectSPK SmallZone
//----------------------------------------------------------------------
// SmallZone¿¡¸¸ Á¸ÀçÇÏ´Â ImageObjectµéÀ» Á¦°ÅÇÑ´Ù.
//----------------------------------------------------------------------
void	
MTopView::ReleaseImageObjectSPKSmallZone()
{
	/*
	DEBUG_ADD( "ReleaseImageObjectSPKSmallZone" );
	
	// m_pImageObjectSFPArraySmallZone¿Í m_pImageObjectSFPArrayLargeZone¸¦ sortÇÏ°í
	// SmallZone - LargeZoneÇØ¼­ ³²Àº °ÍÀ» Á¦°ÅÇÑ´Ù.
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

	// SmallZoneImageObjectID¿¡¼­ LargeZoneImageObjectID¸¦ Á¦°ÅÇÑ´Ù.
	SmallZoneImageObjectID -= LargeZoneImageObjectID;
	
	// SmallZone¿¡¸¸ Á¸ÀçÇÏ´Â ImageObjectIDµéÀ» Á¦°ÅÇÑ´Ù.
	m_ImageObjectSPK.ReleasePart( SmallZoneImageObjectID );

	m_pImageObjectSFPArraySmallZone->Release();
	*/
	DEBUG_ADD( "ReleaseImageObjectSPKSmallZone OK" );
}

//----------------------------------------------------------------------
// º¸¿©ÁÙ ZoneÀ» ¼³Á¤ÇÑ´Ù.
//----------------------------------------------------------------------
void
MTopView::SetZone(MZone* pZone)
{
	m_pZone		= pZone;
	
	// ¸ðµÎ ´Ù½Ã ±×·ÁÁØ´Ù.
	m_bFirstTileDraw = true;
	
	ClearOutputCreature();

	ClearItemNameList();

	// ÁÂÇ¥ ¼³Á¤
	//----------------------------------------
	// ½Ã¾ß °ü·Ã..
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
// È­¸é »óÀÇ ÁÂÇ¥ (x,y)´Â Zone¿¡¼­ÀÇ pixelÁÂÇ¥·Î´Â ¾î´À Á¡ÀÎ°¡?
//----------------------------------------------------------------------
POINT	
MTopView::ScreenToPixel(int x, int y)
{
	// ÇöÀç È­¸éÀÇ (x,y)ÀÇ ÁÂÇ¥¸¦ ±¸ÇÑ´Ù.
	POINT zonePixel;	// È­¸é»óÀÇ Ã³À½ Sector°¡ ³ªÅ¸³»´Â ZoneÀÇ pixelÁÂÇ¥

	// È­¸éÀÇ Ã¹ Sector°¡ ³ªÅ¸³»´Â Map¿¡¼­ÀÇ PixelÁÂÇ¥¸¦ ±¸ÇÏ°í
	// È­¸éÁÂÇ¥ (x,y)¸¦ ´õÇØÁÖ¸é 
	// È­¸é »óÀÇ ÁÂÇ¥°¡ ³ªÅ¸³»´Â Map¿¡¼­ÀÇ PixelÁÂÇ¥¸¦ ±¸ÇÏ´Â °ÍÀÌ´Ù
	
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
// ZoneÀÇ pixelÁÂÇ¥¸¦ È­¸éÀÇ ÁÂÇ¥·Î ¹Ù²Û´Ù.
//----------------------------------------------------------------------
POINT
MTopView::PixelToScreen(int x, int y)
{
	POINT screenPixel;

	// zoneÀÇ pixelÁÂÇ¥¿¡¼­ 
	// È­¸éÀÇ Ã¹Á¡ÀÌ °¡¸®Å°´Â zoneÀÇ ÁÂÇ¥¸¦ »©¸é µÈ´Ù.
	screenPixel.x = x - m_FirstZonePixel.x;
	screenPixel.y = y - m_FirstZonePixel.y;

	return screenPixel;
}

//----------------------------------------------------------------------
// Map To Screen
//----------------------------------------------------------------------
// MapÀÇ sectorÁÂÇ¥¸¦ È­¸éÀÇ ÁÂÇ¥·Î ¹Ù²Û´Ù.
//----------------------------------------------------------------------
POINT
MTopView::MapToScreen(int sX, int sY)
{
	POINT screenPixel;

	// MapÀÇ SectorÁÂÇ¥¸¦ PixelÁÂÇ¥·Î ¹Ù²Û´Ù.
	screenPixel = MapToPixel(sX, sY);

	// PixelÁÂÇ¥¸¦ ScreenÁÂÇ¥·Î ¹Ù²Û´Ù.
	screenPixel.x -= m_FirstZonePixel.x;
	screenPixel.y -= m_FirstZonePixel.y;

	return screenPixel;
}

//----------------------------------------------------------------------
// È­¸é ÁÂÇ¥ (x,y)°¡ °¡¸®Å°´Â À§Ä¡´Â 
// Zone¿¡¼­ÀÇ ¾î¶²(sX,sY) SectorÀÏ±î?
//----------------------------------------------------------------------
POINT
MTopView::GetSelectedSector(int x, int y)
{
	POINT point;

	//-------------------------------------------------
	// ÁØºñµÈ °ªµé
	//-------------------------------------------------
	// È­¸é »óÀÇ Ã¹ Sector : m_FirstSector
	// ÁÂÇ¥º¸Á¤°ª          : m_PlusPoint
	//-------------------------------------------------

	//-------------------------------------------------
	// ¹æ¹ý
	//-------------------------------------------------
	// 1. È­¸éÀÇ (0,0)ÀÌ Zone¿¡¼­ ³ªÅ¸³»´Â 
	//    pixel´ÜÀ§ÁÂÇ¥¸¦ ¾Ë¾Æ³½´Ù.(pX,pY) - MapToPixel
	//
	// 2. (pX,pY) + (x,y)¸¦ ±¸ÇÑ´Ù.
	//
	// 3. (pX,x, pY,y)°¡ Zone¿¡¼­ ³ªÅ¸³»´Â
	//    SectorÁÂÇ¥¸¦ ¾Ë¾Æ³½´Ù.           - PixelToMap
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
// ´Ù¸¥ Creature¸¦ ÇâÇØ¼­ ¹Ù¶óº»´Ù.
//----------------------------------------------------------------------
BYTE
MTopView::GetDirectionToPosition(int originX, int originY, int destX, int destY)
{
	int	stepX = destX - originX,
		stepY = destY - originY;

	// 0ÀÏ ¶§ check
	float	k	= (stepX==0)? 0 : (float)(stepY) / stepX;	// ±â¿ï±â
									

	//--------------------------------------------------
	// ¹æÇâÀ» Á¤ÇØ¾ß ÇÑ´Ù.	
	//--------------------------------------------------
	if (stepY == 0)
	{
		// XÃà
		// - -;;
		if (stepX == 0)
			return DIRECTION_DOWN;
		else if (stepX > 0)
			return DIRECTION_RIGHT;
		else 
			return DIRECTION_LEFT;
	}
	else
	if (stepY < 0)	// UPÂÊÀ¸·Î
	{
		// yÃà À§
		if (stepX == 0)
		{
			return DIRECTION_UP;
		}
		// 1»çºÐ¸é
		else if (stepX > 0)
		{
			if (k < -BASIS_DIRECTION_HIGH)
				return DIRECTION_UP;
			else if (k <= -BASIS_DIRECTION_LOW)
				return DIRECTION_RIGHTUP;
			else
				return DIRECTION_RIGHT;
		}
		// 2»çºÐ¸é
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
	// ¾Æ·¡ÂÊ
	else
	{		
		// yÃà ¾Æ·¡
		if (stepX == 0)
		{
			return DIRECTION_DOWN;
		}
		// 4»çºÐ¸é
		else if (stepX > 0)
		{
			if (k > BASIS_DIRECTION_HIGH)
				return DIRECTION_DOWN;
			else if (k >= BASIS_DIRECTION_LOW)
				return DIRECTION_RIGHTDOWN;
			else
				return DIRECTION_RIGHT;
		}
		// 3»çºÐ¸é
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
// startºÎÅÍ end±îÁö step¾¿..
// (r,g,b)»ö±ò·Î Fade in ½ÃÅ²´Ù.
// 0(¿ø»ö)~31(¾øÀ½)
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
#ifdef PLATFORM_WINDOWS
	if (CDirect3D::IsHAL())
	{
		// 3D °¡¼ÓÀÎ °æ¿ì¿£ 4:4:4ÀÌ´Ù.
		m_FadeColor = (((WORD)r>>1) << 8)
						| (((WORD)g>>1) << 4)
						| ((WORD)b>>1);
	}
	else
#endif
	{
		m_FadeColor	= CDirectDraw::Color(r,g,b);
	}
}


//----------------------------------------------------------------------
// Draw Fade
//----------------------------------------------------------------------
// Fade In/Out Ã³¸®
//----------------------------------------------------------------------	
void 
MTopView::DrawFade()
{	
	if(g_pEventManager->GetEventByFlag(EVENTFLAG_NOT_FADE_SCREEN))
		return;

	//--------------------------------------------------------
	// Fade In/Out ½ÃÅ°´Â ÁßÀÌ¸é Ãâ·Â..
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
//				// 3D °¡¼ÓÀÎ °æ¿ì¿£ 4:4:4ÀÌ´Ù.
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
//		// 3D °¡¼ÓÀÇ °æ¿ì
//		//--------------------------------------------------------
//		if (CDirect3D::IsHAL())
//		{
//			WORD alpha = (32-m_FadeValue)>>1;			
//			WORD pixel = alpha << 12;
//
//			// m_FadeColor·Î È­¸éÀ» µ¤´Â´Ù.
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
			// °ËÁ¤»öÀÌ¸é.. ½±°Ô µÈ´Ù~
			//-------------------------------------------------
			if (m_FadeColor==0)
			{
#ifdef PLATFORM_WINDOWS
				// 2D 5:6:5
				if (CDirectDraw::Is565())
				{
					//m_pSurface->GammaBox565(&rect, m_FadeValue);
				}
				// 2D 5:5:5
				else
				{
					//m_pSurface->GammaBox555(&rect, m_FadeValue);
				}
#endif
			}
			//-------------------------------------------------
			// ¾Æ´Ï¸é...
			//-------------------------------------------------
			else
			{
				m_pSurface->BltColorAlpha(&rect, m_FadeColor, m_FadeValue);
			}

			m_pSurface->Unlock();
		}

		//------------------------------------------------
		// ´ÙÀ½ fade°ª
		//------------------------------------------------
		// 2004, 6, 21, sobeit add start - Áúµå·¹ ¿¬Ãâ ¶«¿¡ Ãß°¡..
		if(m_delayFrame)
		{
			if(g_CurrentFrame - TempFadeFrame >= m_delayFrame)
			{
				if( m_FadeEnd == -1 && 1 == m_FadeValue ) // Áúµå·¹ ¿¬Ãâ¶«¿¡ ¾îµÎ¿ö Áø´ã¿¡ Àá½Ã À¯Áö..^^;
				{
					if(g_CurrentFrame - TempFadeFrame> 16*5) // 5ÃÊ°£ À¯Áö
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
		// 2004, 6, 21, sobeit add end - Áúµå·¹ ¿¬Ãâ ¶«¿¡ Ãß°¡..
			m_FadeValue += m_FadeInc;

		//------------------------------------------------
		// ³¡ÀÎ°¡?
		//------------------------------------------------
		// Áõ°¡ÇÏ°í ÀÖ´Â °æ¿ì
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
// È­¸é ÁÂÇ¥ (x,y)°¡ °¡¸®Å°´Â À§Ä¡¸¦ ¼±ÅÃÇÏ¸é 
// ZoneÀÇ ¾î¶²(sX,sY) Object°¡ ¼±ÅÃµÉ±î?
//----------------------------------------------------------------------
MObject*
MTopView::GetSelectedObject(int x, int y)
{
	//--------------------------------------------------------------	
	// 1. È­¸éÁÂÇ¥ (x,y)ÀÇ Zone¿¡¼­ÀÇ Sector(sX,sY)ÁÂÇ¥¸¦ ±¸ÇÑ´Ù.
	// 
	// 2. (sX,sY) ±ÙÃ³¿¡ Object°¡ ÀÖÀ¸¸é 
	//    ±× ObjectÀÇ È­¸é »óÀÇ ÁÂÇ¥¸¦ ±¸ÇØ¼­
	//    (x,y)°¡ ¼ÓÇÏ¸é "¼±ÅÃÇÑ´Ù"
	//    ¾Æ´Ï¸é, ±× ´ÙÀ½ÀÇ ±ÙÃ³ Sector¸¦ checkÇØºÁ¾ß ÇÑ´Ù.
	//
	// (!) Player´Â Á¦¿Ü´Ù. ¾ø´Ù°í »ý°¢ÇÏ°í checkÇÑ´Ù.
	//--------------------------------------------------------------

	//--------------------------------------------------------------
	// ±ÙÃ³ Sector¸¦ Ã£´Â ¼ø¼­
	//--------------------------------------------------------------	
	// - ±×¸²Àº ¾Æ·¡ÂÊ Sector¿¡ ÀÖ´Â °ÍÀÏ¼ö·Ï ´Ê°Ô Ãâ·ÂµÇ¹Ç·Î
	//   ¼±ÅÃµÉ ¿ì¼±¼øÀ§°¡ ³ô´Ù.
	// 
	//      .  .  .  
	//      .  X  . 
	//      .  .  .  
	//      .  .  .  
	//      .  .  .     X°¡ ¼±ÅÃµÆÀ» °æ¿ì, Ground,Item µî... check¹üÀ§
	//
	//
	//               
	//         X    
	//               
	//      .  .  .  
	//      .  .  .  
	//      .  .  .  
	//      .  .  .     FlyingÀÌ 64 pixel(µÎ Å¸ÀÏ) ³ôÀÌ ¶ã °æ¿ì..
	//      .  .  .     X°¡ ¼±ÅÃµÆÀ» °æ¿ì, Flying... check¹üÀ§
	//
	//--------------------------------------------------------------

	//--------------------------------------------------------------
	// °¢ Sector¿¡ ´ëÇØ¼­ checkÇØºÁ¾ß ÇÏ´Â °Í
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
	// ¾ÆÁ÷ °ÔÀÓ È­¸éÀÌ ¾È ±×·ÁÁø °æ¿ìÀÌ¸é..
	// Player°¡ ´ÙÅ©´Ï½º ¼Ó¿¡ ÀÖ´Ù¸é..
	// ±×³É return
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

	POINT firstZonePixel;	// È­¸é»óÀÇ Ã³À½ Sector°¡ ³ªÅ¸³»´Â ZoneÀÇ pixelÁÂÇ¥
	firstZonePixel = MapToPixel(m_FirstSector.x, m_FirstSector.y);
	firstZonePixel.x += m_PlusPoint.x;
	firstZonePixel.y += m_PlusPoint.y;


	POINT	pixelPoint;			// Pixel Position In Zone
	POINT	sectorPoint;		// Sector Position In Zone

	//--------------------------------------------------------------
	// (x,y)ÀÇ Zone¿¡¼­ÀÇ pixelÁÂÇ¥¸¦ ±¸ÇÑ´Ù.
	//--------------------------------------------------------------
	pixelPoint = MapToPixel(m_FirstSector.x, m_FirstSector.y);
	pixelPoint.x += m_PlusPoint.x + x;
	pixelPoint.y += m_PlusPoint.y + y;

	//--------------------------------------------------------------
	// (x,y)ÀÇ Zone¿¡¼­ÀÇ sectorÁÂÇ¥¸¦ ±¸ÇÑ´Ù.
	//--------------------------------------------------------------						
	sectorPoint = PixelToMap(pixelPoint.x, pixelPoint.y);

	g_MouseSector = sectorPoint;

	//--------------------------------------------------------------
	// Æ÷Å» Ã¼Å©
	//--------------------------------------------------------------
	// ¿©±â¼­ ÇÏ´Â°Ç ¿ô±âÁö¸¸.. -_-;;
	// Ã³¸®ÀÇ ÆíÀÇ¸¦ À§ÇØ¼­.. ÀÓ½Ã·Î... ÇÒÇÒ... ¤Ñ.¤Ñ;;
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
				// ÀÌµ¿ °¡´ÉÇÑ zoneÀÇ ID
				PORTAL_INFO portalInfo = *iPortal;			

				//-------------------------------------------------------
				// ÀÓ½Ã Ãâ·Â
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
				// portalÀÇ Á¾·ù¿¡ µû¶ó °¥¼ö ÀÖ´Â°÷ÀÎÁö Ã¼Å©
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
				// °¥ ¼ö ÀÖÀ¸¸é Æ÷Å» Ä¿¼­ Ãâ·Â
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
								// ÇÊ»ì ÇÏµåÄÚµù~~
								zoneID = 60001;
								//strcpy(pZoneName, "¿¤¸®º£ÀÌÅÍ");
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
						//strcpy(pZoneName, "°¥ ¼ö ¾ø´Â °÷");
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

	// ObjectÀÇ ¿µ¿ª
//	POINT	objectPixelPoint;
	//RECT	rect;

	// Player Creature's ID
	TYPE_OBJECTID	pid = g_pPlayer->GetID();

	//std::ofstream file("log.txt");


	// ÇöÀç checkÇÏ´Â ÁÂÇ¥
	register int currentX;
	register int currentY;

	int numCreature = 0;

	//--------------------------------------------------------------
	//
	// Flying Creature Check 
	//
	// : °°Àº ³ôÀÌ¿¡ ÀÖ´Â °æ¿ì 
	//   ÀÏ¹ÝÀûÀÎ Objectº¸´Ù ¾Æ·¡ÂÊ Sector¿¡ Á¸ÀçÇÒ ¼ö ÀÖ´Ù.
	//
	//--------------------------------------------------------------
	int	sX1 = sectorPoint.x - 3,		// ÇÑ ÁÙ¾¿ ´õ Áõ°¡½ÃÄ×´Ù. À½³Ä¸®~
			sX2 = sectorPoint.x + 3;

	// ¹ÚÁã ³ôÀÌ ¶§¹®¿¡.. +¸¦ ´õÇß´Ù.
	int	sY1 = sectorPoint.y + 4,
			sY2 = sectorPoint.y + 8;

	//------------------------------------------------------
	// ZoneÀÇ ¿µ¿ªÀÌ ¾Æ´Ñ °æ¿ì¿¡ Skip...
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
	// ÀÖÀ»¹ýÇÑ Sector¸¦ checkÇÑ´Ù.
	//------------------------------------------------------	
	for (currentY=sY2; currentY>=sY1; currentY--)	
	{	
		for (currentX=sX1; currentX<=sX2; currentX++)	
		{			
			#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
				DEBUG_ADD_FORMAT("ckF1(%d,%d)", currentX, currentY);
			#endif

			const MSector& sector = m_pZone->GetSector(currentX, currentY);

			// ¾î¶² Object°¡ ÀÖÀ» °æ¿ì
			if (sector.IsExistObject() )			
			{		
				
				//file << "Exist Object!" << endl;

				// ObjectÀÇ Á¾·ù¿¡ µû¶ó¼­ ´Þ¸® checkÇÑ´Ù.
				// ¼±ÅÃÇÒ ¼ö ÀÖ´Â °ÍÀº Creature¿Í Item»ÓÀÌ´Ù.

				//------------------------------------------------
				//
				// Flying Creature°¡ ÀÖ´Â °æ¿ì
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
				
						// PlayerÀÌ¸é ¼±ÅÃÇÏÁö ¾Ê´Â´Ù.
						if (pCreature->GetID()!=pid)	
						{
							// FlyingCreature°¡ Â÷ÁöÇÏ´Â ¿µ¿ªÀ» ±¸ÇÑ´Ù.
							// FRAME_PACK¿¡¼­ Á¤º¸¸¦ ÀÐ¾î¾ß ÇÒ µí!!
							// FlyingCreatureÀÇ SectorÁÂÇ¥¸¦ PixelÁÂÇ¥·Î ¹Ù²ã¾ß ÇÑ´Ù.

							// FlyingCreatureÀÇ ZoneÀÇ ÁÂÇ¥¸¦ °è»êÇÑ´Ù.
							/*
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// ZoneÀÇ ÁÂÇ¥¸¦ È­¸éÀÇ ÁÂÇ¥·Î ¹Ù²Û´Ù.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature°¡ È­¸é¿¡ Á¸ÀçÇÏ´Â ¿µ¿ªÀ» °è»êÇØ³½´Ù.	
							frame = m_CreatureFPK[(*g_pCreatureTable)[pCreature->GetCreatureType()].FrameID][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// ÇöÀç Sprite°¡ È­¸é¿¡ Ãâ·ÂµÇ´Â À§Ä¡
							objectPixelPoint.x += frame.GetCX();
							objectPixelPoint.y += frame.GetCY() - FLYINGCREATURE_HEIGHT;
												// FramePack¿¡¼­ ³ôÀÌ¸¦ ¼³Á¤ÇÏ¹Ç·Î,
												// FlyingÀÎ °æ¿ìµµ ¶È°°´Ù.

							// Creature°¡ È­¸é¿¡ Á¸ÀçÇÏ´Â ¿µ¿ªÀ» °è»êÇØ³½´Ù.
							rect.left	= objectPixelPoint.x;
							rect.top	= objectPixelPoint.y;
							rect.right	= rect.left + m_CreatureSPK[ frame.GetSpriteID() ].GetWidth();
							rect.bottom	= rect.top + m_CreatureSPK[ frame.GetSpriteID() ].GetHeight();
							*/

							// ´ÙÅ©´Ï½º¿¡ ÀÖÀ¸¸é 
							// ¼±ÅÃÇÑ À§Ä¡°¡ CreatureÀÇ ¿µ¿ª¿¡ ¼ÓÇÏ¸é
							// °ø°Ý modeÀÎ °æ¿ì´Â °ø°Ý°¡´ÉÇÑ Ä³¸¯ÅÍ¸¸ ¼±ÅÃ
							// ¾Æ´Ï¸é ¾Æ¹«³ª.
							if (
								(g_pPlayer->IsVampire()&&g_pZone->GetID() != 3001 ||
								!g_pPlayer->IsVampire()&& !(!pCreature->IsNPC() && pCreature->IsInDarkness()) ||
								!g_pPlayer->IsVampire()&& g_pPlayer->HasEffectStatus( EFFECTSTATUS_LIGHTNESS ) 
								|| g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
								|| g_bLight
#endif
								)
								&& !pCreature->IsCutHeight()	// »ç¶óÁö´Â »óÅÂ¸é
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
	sY2 = sectorPoint.y + 4;		// ÇÑ ÁÙ¾¿ ´õ Áõ°¡½ÃÄ×´Ù.

	//------------------------------------------------------
	// ZoneÀÇ ¿µ¿ªÀÌ ¾Æ´Ñ °æ¿ì¿¡ Skip...
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
	// Check´ë»óÀÌ µÇ´Â ImageObject Iterator
	//------------------------------------------------------
	IMAGEOBJECT_OUTPUT_MAP::reverse_iterator iImageObjectCheck
		= m_mapImageObject.rbegin();

	//------------------------------------------------------
	// Ãâ·Â ½ÃÁ¡ÀÌ sY1º¸´Ù ÀûÀº °æ¿ìÀÇ 
	// ImageObjectµéÀ» CheckÇÑ´Ù.
	// m_mapImageObject¿¡´Â Viewpoint°¡ key°ªÀÌ µÇ¾î¼­ SortµÇ¾î ÀÖ´Ù.
	//------------------------------------------------------
	while (iImageObjectCheck != m_mapImageObject.rend())
	{		
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectCheck).second);

		// InteractionObjectÀÎ °æ¿ì¸¸ checkÇÑ´Ù.
//		if (pImageObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
//		{
//			MInteractionObject* const pInteractionObject = (MInteractionObject* const)pImageObject;
//
//			// Ãâ·Â ½ÃÁ¡ÀÌ sY1º¸´Ù ÀûÀº °æ¿ì..´Â Check
//			if (pInteractionObject->GetViewpoint() > sY2)
//			{
//				/*
//				// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
//				objectPixelPoint.x = pInteractionObject->GetPixelX() - m_FirstZonePixel.x;
//				objectPixelPoint.y = pInteractionObject->GetPixelY() - m_FirstZonePixel.y;
//
//				// frame¿¡ °ü°èµÈ sprite¸¦ ±¸ÇÑ´Ù.
//				frame = m_ImageObjectFPK[ pInteractionObject->GetFrameID() ][ pInteractionObject->GetFrame() ];
//				sprite = frame.GetSpriteID();
//
//				// ÃÊ±âÈ­ µÇÁö ¾ÊÀº °æ¿ì¸é..
//				if (m_ImageObjectSPK[ sprite ].IsNotInit())
//				{
//					iImageObjectCheck ++;
//					continue;
//				}
//						
//				// ÁÂÇ¥ º¸Á¤
//				objectPixelPoint.x += frame.GetCX();
//				objectPixelPoint.y += frame.GetCY();
//
//				// È­¸é¿¡ Á¸ÀçÇÏ´Â ¿µ¿ªÀ» °è»êÇØ³½´Ù.
//				rect.left	= objectPixelPoint.x;
//				rect.top	= objectPixelPoint.y;
//				rect.right	= rect.left + m_ImageObjectSPK[ sprite ].GetWidth();
//				rect.bottom	= rect.top + m_ImageObjectSPK[ sprite ].GetHeight();
//				*/		
//				// ¼±ÅÃÇÑ À§Ä¡°¡ ImageObjectÀÇ ¿µ¿ª¿¡ ¼ÓÇÏ¸é
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
//			// ¾Æ´Ï¸é.. Object checkÇÒ¶§ CheckÇØ¾ßµÇ´Â °æ¿ìÀÌ´Ù.
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
	// ÀÖÀ»¹ýÇÑ Sector¸¦ checkÇÑ´Ù.
	//------------------------------------------------------	
	for (currentY=sY2; currentY>=sY1; currentY--)	
	{	
		for (currentX=sX1; currentX<=sX2; currentX++)	
		{				
			#ifdef OUTPUT_DEBUG_DRAW_PROCESS_INPUT
				DEBUG_ADD_FORMAT("gS(%d,%d)", currentX, currentY);
			#endif

			const MSector& sector = m_pZone->GetSector(currentX, currentY);

			// ¾î¶² Object°¡ ÀÖÀ» °æ¿ì
			if (sector.IsExistObject() )			
			{		
				
				//file << "Exist Object!" << endl;

				// ObjectÀÇ Á¾·ù¿¡ µû¶ó¼­ ´Þ¸® checkÇÑ´Ù.
				// ¼±ÅÃÇÒ ¼ö ÀÖ´Â °ÍÀº Creature¿Í Item»ÓÀÌ´Ù.

				//------------------------------------------------
				//
				// Flying Creature°¡ ÀÖ´Â °æ¿ì
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

						// PlayerÀÌ¸é ¼±ÅÃÇÏÁö ¾Ê´Â´Ù.
						if (pCreature->GetID()!=pid)	
						{
							/*
							// FlyingCreature°¡ Â÷ÁöÇÏ´Â ¿µ¿ªÀ» ±¸ÇÑ´Ù.
							// FRAME_PACK¿¡¼­ Á¤º¸¸¦ ÀÐ¾î¾ß ÇÒ µí!!
							// FlyingCreatureÀÇ SectorÁÂÇ¥¸¦ PixelÁÂÇ¥·Î ¹Ù²ã¾ß ÇÑ´Ù.

							// FlyingCreatureÀÇ ZoneÀÇ ÁÂÇ¥¸¦ °è»êÇÑ´Ù.
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// ZoneÀÇ ÁÂÇ¥¸¦ È­¸éÀÇ ÁÂÇ¥·Î ¹Ù²Û´Ù.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature°¡ È­¸é¿¡ Á¸ÀçÇÏ´Â ¿µ¿ªÀ» °è»êÇØ³½´Ù.	
							frame = m_CreatureFPK[(*g_pCreatureTable)[pCreature->GetCreatureType()].FrameID][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// ÇöÀç Sprite°¡ È­¸é¿¡ Ãâ·ÂµÇ´Â À§Ä¡
							objectPixelPoint.x += frame.GetCX();
							objectPixelPoint.y += frame.GetCY() - FLYINGCREATURE_HEIGHT;
												// FramePack¿¡¼­ ³ôÀÌ¸¦ ¼³Á¤ÇÏ¹Ç·Î,
												// FlyingÀÎ °æ¿ìµµ ¶È°°´Ù.

							// È­¸é¿¡ Á¸ÀçÇÏ´Â ¿µ¿ªÀ» °è»êÇØ³½´Ù.
							rect.left	= objectPixelPoint.x;
							rect.top	= objectPixelPoint.y;
							rect.right	= rect.left + m_CreatureSPK[ frame.GetSpriteID() ].GetWidth();
							rect.bottom	= rect.top + m_CreatureSPK[ frame.GetSpriteID() ].GetHeight();
							*/
							
							// ¼±ÅÃÇÑ À§Ä¡°¡ CreatureÀÇ ¿µ¿ª¿¡ ¼ÓÇÏ¸é
							if ((
								g_pPlayer->IsVampire()&&g_pZone->GetID() != 3001|| 
								!g_pPlayer->IsVampire()&& !(!pCreature->IsNPC() && pCreature->IsInDarkness()) ||
								!g_pPlayer->IsVampire()&& g_pPlayer->HasEffectStatus( EFFECTSTATUS_LIGHTNESS ) 
								|| g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
								|| g_bLight
#endif
								)
								&& !pCreature->IsCutHeight()	// »ç¶óÁö´Â »óÅÂ¸é
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
				// Creature°¡ ÀÖ´Â °æ¿ì
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

						// PlayerÀÌ¸é ¼±ÅÃÇÏÁö ¾Ê´Â´Ù.
						if (pCreature->GetID()!=pid)	
						{
							/*
							// Creature°¡ Â÷ÁöÇÏ´Â ¿µ¿ªÀ» ±¸ÇÑ´Ù.
							// FRAME_PACK¿¡¼­ Á¤º¸¸¦ ÀÐ¾î¾ß ÇÒ µí!!
							// CreatureÀÇ SectorÁÂÇ¥¸¦ PixelÁÂÇ¥·Î ¹Ù²ã¾ß ÇÑ´Ù.

							// CreatureÀÇ ZoneÀÇ ÁÂÇ¥¸¦ °è»êÇÑ´Ù.
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// ZoneÀÇ ÁÂÇ¥¸¦ È­¸éÀÇ ÁÂÇ¥·Î ¹Ù²Û´Ù.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature°¡ È­¸é¿¡ Á¸ÀçÇÏ´Â ¿µ¿ªÀ» °è»êÇØ³½´Ù.	
							frame = m_CreatureFPK[(*g_pCreatureTable)[pCreature->GetCreatureType()].FrameID][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// ÇöÀç Sprite°¡ È­¸é¿¡ Ãâ·ÂµÇ´Â À§Ä¡
							objectPixelPoint.x += frame.GetCX();
							objectPixelPoint.y += frame.GetCY();

							// È­¸é¿¡ Á¸ÀçÇÏ´Â ¿µ¿ªÀ» °è»êÇØ³½´Ù.
							rect.left	= objectPixelPoint.x;
							rect.top	= objectPixelPoint.y;
							rect.right	= rect.left + m_CreatureSPK[ frame.GetSpriteID() ].GetWidth();
							rect.bottom	= rect.top + m_CreatureSPK[ frame.GetSpriteID() ].GetHeight();
							*/			
							// ¼±ÅÃÇÑ À§Ä¡°¡ CreatureÀÇ ¿µ¿ª¿¡ ¼ÓÇÏ¸é
							if ((g_pPlayer->IsVampire()&&g_pZone->GetID() != 3001 ||
								!g_pPlayer->IsVampire()&& !(!pCreature->IsNPC() && pCreature->IsInDarkness()) ||
								!g_pPlayer->IsVampire()&& g_pPlayer->HasEffectStatus( EFFECTSTATUS_LIGHTNESS ) 
								|| g_pPlayer->HasEffectStatus( EFFECTSTATUS_GHOST )
#ifdef __METROTECH_TEST__
								|| g_bLight
#endif
								)
								&& !pCreature->IsCutHeight()	// »ç¶óÁö´Â »óÅÂ¸é
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
				// Effect°¡ ÀÖ´Â °æ¿ì
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
				// UndergroundCreature°¡ ÀÖ´Â °æ¿ì
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

						// PlayerÀÌ¸é ¼±ÅÃÇÏÁö ¾Ê´Â´Ù.
						if (pCreature->GetID()!=pid)	
						{							
							// ¼±ÅÃÇÑ À§Ä¡°¡ CreatureÀÇ ¿µ¿ª¿¡ ¼ÓÇÏ¸é
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
				// ItemÀÌ ÀÖ´Â °æ¿ì
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

					// Item°¡ Â÷ÁöÇÏ´Â ¿µ¿ªÀ» ±¸ÇÑ´Ù.
					// FRAME_PACK¿¡¼­ Á¤º¸¸¦ ÀÐ¾î¾ß ÇÒ µí!!
					// ItemÀÇ SectorÁÂÇ¥¸¦ PixelÁÂÇ¥·Î ¹Ù²ã¾ß ÇÑ´Ù.
					/*
					// ItemÀÇ ZoneÀÇ ÁÂÇ¥¸¦ °è»êÇÑ´Ù.
					objectPixelPoint = MapToPixel(pItem->GetX(), pItem->GetY());
					objectPixelPoint.x += m_PlusPoint.x;// + pItem->GetSX();
					objectPixelPoint.y += m_PlusPoint.y;// + pItem->GetSY();
						
					// ZoneÀÇ ÁÂÇ¥¸¦ È­¸éÀÇ ÁÂÇ¥·Î ¹Ù²Û´Ù.
					objectPixelPoint.x -= firstZonePixel.x;
					objectPixelPoint.y -= firstZonePixel.y;

					// Item°¡ È­¸é¿¡ Á¸ÀçÇÏ´Â ¿µ¿ªÀ» °è»êÇØ³½´Ù.				
					frame = m_ItemTileFPK[ pItem->GetTileFrameID() ];

					// ÇöÀç Sprite°¡ È­¸é¿¡ Ãâ·ÂµÇ´Â À§Ä¡
					objectPixelPoint.x += frame.GetCX();
					objectPixelPoint.y += frame.GetCY();

					// È­¸é¿¡ Á¸ÀçÇÏ´Â ¿µ¿ªÀ» °è»êÇØ³½´Ù.
					rect.left	= objectPixelPoint.x;
					rect.top	= objectPixelPoint.y;
					rect.right	= rect.left + m_ItemTileSPK[ frame.GetSpriteID() ].GetWidth();
					rect.bottom	= rect.top + m_ItemTileSPK[ frame.GetSpriteID() ].GetHeight();
					*/			

					//------------------------------------------------
					// ¹ìÆÄÀÌ¾î°Å³ª
					// ½½·¹ÀÌ¾îÀÎ °æ¿ì´Â º¼ ¼ö ÀÖ´Â »óÈ²ÀÌ¶ó¸é.. 
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
						// ½ÃÃ¼ÀÎ °æ¿ì´Â 
						// ½ÃÃ¼ CreatureÀÇ ¿µ¿ªÀ» ºÁ¾ß ÇÑ´Ù.
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
							// ¼±ÅÃÇÑ À§Ä¡°¡ ItemÀÇ ¿µ¿ª¿¡ ¼ÓÇÏ¸é
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
		// Ãâ·Â ½ÃÁ¡ÀÌ currentYº¸´Ù ÀûÀº °æ¿ìÀÇ 
		// ImageObjectµéÀ» CheckÇÑ´Ù.
		// m_mapImageObject¿¡´Â Viewpoint°¡ key°ªÀÌ µÇ¾î¼­ SortµÇ¾î ÀÖ´Ù.
		//------------------------------------------------------
//		while (iImageObjectCheck != m_mapImageObject.rend())
//		{
//			MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectCheck).second);
//
			// InteractionObjectÀÎ °æ¿ì¸¸ checkÇÑ´Ù.
//			if (pImageObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
//			{
//				MInteractionObject* const pInteractionObject = (MInteractionObject* const)pImageObject;
//
//				// Ãâ·Â ½ÃÁ¡ÀÌ sY1º¸´Ù ÀûÀº °æ¿ì..´Â Check
//				if (pInteractionObject->GetViewpoint() == currentY)
//				{
//					/*
//					// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
//					objectPixelPoint.x = pInteractionObject->GetPixelX() - m_FirstZonePixel.x;
//					objectPixelPoint.y = pInteractionObject->GetPixelY() - m_FirstZonePixel.y;
//
//					// frame¿¡ °ü°èµÈ sprite¸¦ ±¸ÇÑ´Ù.
//					frame = m_ImageObjectFPK[ pInteractionObject->GetFrameID() ][ pInteractionObject->GetFrame() ];
//					sprite = frame.GetSpriteID();
//
//					// ÃÊ±âÈ­ µÇÁö ¾ÊÀº °æ¿ì¸é..
//					if (m_ImageObjectSPK[ sprite ].IsNotInit())
//					{
//						iImageObjectCheck ++;
//						continue;
//					}
//							
//					// ÁÂÇ¥ º¸Á¤
//					objectPixelPoint.x += frame.GetCX();
//					objectPixelPoint.y += frame.GetCY();
//
//					// È­¸é¿¡ Á¸ÀçÇÏ´Â ¿µ¿ªÀ» °è»êÇØ³½´Ù.
//					rect.left	= objectPixelPoint.x;
//					rect.top	= objectPixelPoint.y;
//					rect.right	= rect.left + m_ImageObjectSPK[ sprite ].GetWidth();
//					rect.bottom	= rect.top + m_ImageObjectSPK[ sprite ].GetHeight();
//					*/			
//					// ¼±ÅÃÇÑ À§Ä¡°¡ ImageObjectÀÇ ¿µ¿ª¿¡ ¼ÓÇÏ¸é
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
//				// ¾Æ´Ï¸é.. Object checkÇÒ¶§ CheckÇØ¾ßµÇ´Â °æ¿ìÀÌ´Ù.
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
	// Ãâ·Â ½ÃÁ¡ÀÌ currentYº¸´Ù ÀûÀº °æ¿ìÀÇ 
	// ImageObjectµéÀ» CheckÇÑ´Ù.
	// m_mapImageObject¿¡´Â Viewpoint°¡ key°ªÀÌ µÇ¾î¼­ SortµÇ¾î ÀÖ´Ù.
	//------------------------------------------------------
//	while (iImageObjectCheck != m_mapImageObject.rend())
//	{
//		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectCheck).second);
//
		// InteractionObjectÀÎ °æ¿ì¸¸ checkÇÑ´Ù.
//		if (pImageObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
//		{
//			MInteractionObject* const pInteractionObject = (MInteractionObject* const)pImageObject;
//		
//			// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
//			/*
//			objectPixelPoint.x = pInteractionObject->GetPixelX() - m_FirstZonePixel.x;
//			objectPixelPoint.y = pInteractionObject->GetPixelY() - m_FirstZonePixel.y;
//
//			// frame¿¡ °ü°èµÈ sprite¸¦ ±¸ÇÑ´Ù.
//			frame = m_ImageObjectFPK[ pInteractionObject->GetFrameID() ][ pInteractionObject->GetFrame() ];
//			sprite = frame.GetSpriteID();
//
//			// ÃÊ±âÈ­ µÇÁö ¾ÊÀº °æ¿ì¸é..
//			if (m_ImageObjectSPK[ sprite ].IsNotInit())
//			{
//				iImageObjectCheck ++;
//				continue;
//			}
//							
//			// ÁÂÇ¥ º¸Á¤
//			objectPixelPoint.x += frame.GetCX();
//			objectPixelPoint.y += frame.GetCY();
//
//			// È­¸é¿¡ Á¸ÀçÇÏ´Â ¿µ¿ªÀ» °è»êÇØ³½´Ù.
//			rect.left	= objectPixelPoint.x;
//			rect.top	= objectPixelPoint.y;
//			rect.right	= rect.left + m_ImageObjectSPK[ sprite ].GetWidth();
//			rect.bottom	= rect.top + m_ImageObjectSPK[ sprite ].GetHeight();
//			*/				
//			// ¼±ÅÃÇÑ À§Ä¡°¡ ImageObjectÀÇ ¿µ¿ª¿¡ ¼ÓÇÏ¸é
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
	// ¾Æ¹«°Íµµ ¾ø´Ù¸é.. GroundEffectµµ Ã¼Å©ÇÑ´Ù.
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
		// ¾Æ¹«°Íµµ ¾ø´Ù¸é.. FakeCreature(Pet)µµ Ã¼Å©ÇÑ´Ù.
		//------------------------------------------------------
		int fakeCreatureNum = g_pZone->GetFakeCreatureNumber();
		MZone::CREATURE_MAP::const_iterator iFakeCreature = g_pZone->GetFakeCreatureBegin();
		for(int fc = 0; fc < fakeCreatureNum; fc++)
		{
			MFakeCreature *pFakeCreature = (MFakeCreature *)iFakeCreature->second;

			// ÆêÀÎ°æ¿ì¸¸ °Ë»ö
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
// È­¸é ÁÂÇ¥ (x,y)°¡ °¡¸®Å°´Â À§Ä¡¸¦ ¼±ÅÃÇÏ¸é 
// ZoneÀÇ ¾î¶²(sX,sY) Object°¡ ¼±ÅÃµÉ±î?
//
// Sprite¿¡¼­ pixelÁ¤º¸¸¦ ÀÐ¾î¼­ ºñ±³ÇÑ´Ù.
//
//----------------------------------------------------------------------
MObject*
MTopView::GetSelectedObjectSprite(int x, int y)
{
	//--------------------------------------------------------------	
	// 1. È­¸éÁÂÇ¥ (x,y)ÀÇ Zone¿¡¼­ÀÇ Sector(sX,sY)ÁÂÇ¥¸¦ ±¸ÇÑ´Ù.
	// 
	// 2. (sX,sY) ±ÙÃ³¿¡ Object°¡ ÀÖÀ¸¸é 
	//    ±× ObjectÀÇ È­¸é »óÀÇ ÁÂÇ¥¸¦ ±¸ÇØ¼­
	//    (x,y)°¡ ¼ÓÇÏ¸é "¼±ÅÃÇÑ´Ù"
	//    ¾Æ´Ï¸é, ±× ´ÙÀ½ÀÇ ±ÙÃ³ Sector¸¦ checkÇØºÁ¾ß ÇÑ´Ù.
	//
	// (!) Player´Â Á¦¿Ü´Ù. ¾ø´Ù°í »ý°¢ÇÏ°í checkÇÑ´Ù.
	//--------------------------------------------------------------

	//--------------------------------------------------------------
	// ±ÙÃ³ Sector¸¦ Ã£´Â ¼ø¼­
	//--------------------------------------------------------------	
	// - ±×¸²Àº ¾Æ·¡ÂÊ Sector¿¡ ÀÖ´Â °ÍÀÏ¼ö·Ï ´Ê°Ô Ãâ·ÂµÇ¹Ç·Î
	//   ¼±ÅÃµÉ ¿ì¼±¼øÀ§°¡ ³ô´Ù.
	// 
	//      .  .  .  
	//      .  X  . 
	//      .  .  .  
	//      .  .  .  
	//      .  .  .     X°¡ ¼±ÅÃµÆÀ» °æ¿ì, Ground,Item µî... check¹üÀ§
	//
	//
	//               
	//         X    
	//               
	//      .  .  .  
	//      .  .  .  
	//      .  .  .  
	//      .  .  .     FlyingÀÌ 64 pixel(µÎ Å¸ÀÏ) ³ôÀÌ ¶ã °æ¿ì..
	//      .  .  .     X°¡ ¼±ÅÃµÆÀ» °æ¿ì, Flying... check¹üÀ§
	//
	//--------------------------------------------------------------

	//--------------------------------------------------------------
	// °¢ Sector¿¡ ´ëÇØ¼­ checkÇØºÁ¾ß ÇÏ´Â °Í
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

	POINT firstZonePixel;	// È­¸é»óÀÇ Ã³À½ Sector°¡ ³ªÅ¸³»´Â ZoneÀÇ pixelÁÂÇ¥
	firstZonePixel = MapToPixel(m_FirstSector.x, m_FirstSector.y);
	firstZonePixel.x += m_PlusPoint.x;
	firstZonePixel.y += m_PlusPoint.y;


	POINT	pixelPoint;			// Pixel Position In Zone
	POINT	sectorPoint;		// Sector Position In Zone

	//--------------------------------------------------------------
	// (x,y)ÀÇ Zone¿¡¼­ÀÇ pixelÁÂÇ¥¸¦ ±¸ÇÑ´Ù.
	//--------------------------------------------------------------
	pixelPoint = MapToPixel(m_FirstSector.x, m_FirstSector.y);
	pixelPoint.x += m_PlusPoint.x + x;
	pixelPoint.y += m_PlusPoint.y + y;

	//--------------------------------------------------------------
	// (x,y)ÀÇ Zone¿¡¼­ÀÇ sectorÁÂÇ¥¸¦ ±¸ÇÑ´Ù.
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

	// ObjectÀÇ ¿µ¿ª
	POINT	objectPixelPoint;

	// Player Creature's ID
	TYPE_OBJECTID	pid = g_pPlayer->GetID();

	//std::ofstream file("log.txt");


	// ÇöÀç checkÇÏ´Â ÁÂÇ¥
	register int currentX;
	register int currentY;

	int numCreature = 0;

	//--------------------------------------------------------------
	//
	// Flying Creature Check 
	//
	// : °°Àº ³ôÀÌ¿¡ ÀÖ´Â °æ¿ì 
	//   ÀÏ¹ÝÀûÀÎ Objectº¸´Ù ¾Æ·¡ÂÊ Sector¿¡ Á¸ÀçÇÒ ¼ö ÀÖ´Ù.
	//
	//--------------------------------------------------------------
	int	sX1 = sectorPoint.x - 1,
			sX2 = sectorPoint.x + 1;
	int	sY1 = sectorPoint.y + 4,
			sY2 = sectorPoint.y + 5;

	//------------------------------------------------------
	// ZoneÀÇ ¿µ¿ªÀÌ ¾Æ´Ñ °æ¿ì¿¡ Skip...
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
	// ÀÖÀ»¹ýÇÑ Sector¸¦ checkÇÑ´Ù.
	//------------------------------------------------------	
	for (currentY=sY2; currentY>=sY1; currentY--)	
	{	
		for (currentX=sX1; currentX<=sX2; currentX++)	
		{				
			const MSector& sector = m_pZone->GetSector(currentX, currentY);

			// ¾î¶² Object°¡ ÀÖÀ» °æ¿ì
			if (sector.IsExistObject() )			
			{		
				
				//file << "Exist Object!" << endl;

				// ObjectÀÇ Á¾·ù¿¡ µû¶ó¼­ ´Þ¸® checkÇÑ´Ù.
				// ¼±ÅÃÇÒ ¼ö ÀÖ´Â °ÍÀº Creature¿Í Item»ÓÀÌ´Ù.

				//------------------------------------------------
				//
				// Flying Creature°¡ ÀÖ´Â °æ¿ì
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

						// PlayerÀÌ¸é ¼±ÅÃÇÏÁö ¾Ê´Â´Ù.
						if (pCreature->GetID()!=pid)	
						{
							// FlyingCreature°¡ Â÷ÁöÇÏ´Â ¿µ¿ªÀ» ±¸ÇÑ´Ù.
							// FRAME_PACK¿¡¼­ Á¤º¸¸¦ ÀÐ¾î¾ß ÇÒ µí!!
							// FlyingCreatureÀÇ SectorÁÂÇ¥¸¦ PixelÁÂÇ¥·Î ¹Ù²ã¾ß ÇÑ´Ù.

							// FlyingCreatureÀÇ ZoneÀÇ ÁÂÇ¥¸¦ °è»êÇÑ´Ù.
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// ZoneÀÇ ÁÂÇ¥¸¦ È­¸éÀÇ ÁÂÇ¥·Î ¹Ù²Û´Ù.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature°¡ È­¸é¿¡ Á¸ÀçÇÏ´Â ¿µ¿ªÀ» °è»êÇØ³½´Ù.	
							frame = m_CreatureFPK[pCreature->GetCreatureFrameID(0)][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// ÇöÀç Sprite°¡ È­¸é¿¡ Ãâ·ÂµÇ´Â À§Ä¡
							objectPixelPoint.x += frame.GetCX();
							objectPixelPoint.y += frame.GetCY() - pCreature->GetZ();//FLYINGCREATURE_HEIGHT;
												// FramePack¿¡¼­ ³ôÀÌ¸¦ ¼³Á¤ÇÏ¹Ç·Î,
												// FlyingÀÎ °æ¿ìµµ ¶È°°´Ù.

							// mouseÁÂÇ¥°¡ Sprite¿¡ ¼ÓÇÏ´ÂÁö ¾Ë¾Æº¸±â
							// Sprite¿¡¼­ÀÇ ÁÂÇ¥°¡ »ö±òÀÌ ÀÖ´Â °æ¿ìÀÏ¶§
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
	// ZoneÀÇ ¿µ¿ªÀÌ ¾Æ´Ñ °æ¿ì¿¡ Skip...
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
	// Check´ë»óÀÌ µÇ´Â ImageObject Iterator
	//------------------------------------------------------
	IMAGEOBJECT_OUTPUT_MAP::reverse_iterator iImageObjectCheck
		= m_mapImageObject.rbegin();

	//------------------------------------------------------
	// Ãâ·Â ½ÃÁ¡ÀÌ sY1º¸´Ù ÀûÀº °æ¿ìÀÇ 
	// ImageObjectµéÀ» CheckÇÑ´Ù.
	// m_mapImageObject¿¡´Â Viewpoint°¡ key°ªÀÌ µÇ¾î¼­ SortµÇ¾î ÀÖ´Ù.
	//------------------------------------------------------
	while (iImageObjectCheck != m_mapImageObject.rend())
	{		
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectCheck).second);

		// InteractionObjectÀÎ °æ¿ì¸¸ checkÇÑ´Ù.
//		if (pImageObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
//		{
//			MInteractionObject* const pInteractionObject = (MInteractionObject* const)pImageObject;
//
//			// Ãâ·Â ½ÃÁ¡ÀÌ sY1º¸´Ù ÀûÀº °æ¿ì..´Â Check
//			if (pInteractionObject->GetViewpoint() > sY2)
//			{
//				// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
//				objectPixelPoint.x = pInteractionObject->GetPixelX() - m_FirstZonePixel.x;
//				objectPixelPoint.y = pInteractionObject->GetPixelY() - m_FirstZonePixel.y;
//
//				// frame¿¡ °ü°èµÈ sprite¸¦ ±¸ÇÑ´Ù.
//				frame = m_ImageObjectFPK[ pInteractionObject->GetFrameID() ][ pInteractionObject->GetFrame() ];
//				sprite = frame.GetSpriteID();
//						
//				// ÁÂÇ¥ º¸Á¤
//				objectPixelPoint.x += frame.GetCX();
//				objectPixelPoint.y += frame.GetCY();
//									
//				// InteractionObject´Â (point.x, point.y)¿¡ Ãâ·ÂµÇ¾î ÀÖÀ» °ÍÀÌ´Ù.
//				// mouseÁÂÇ¥°¡ Sprite¿¡ ¼ÓÇÏ´ÂÁö ¾Ë¾Æº¸±â
//				// Sprite¿¡¼­ÀÇ ÁÂÇ¥°¡ »ö±òÀÌ ÀÖ´Â °æ¿ìÀÏ¶§
//				if (m_ImageObjectSPK[ sprite ].IsColorPixel( 
//						x - objectPixelPoint.x, 
//						y - objectPixelPoint.y 
//					))
//				{	
//					pInteractionObject->SetAction( 1 );
//					return pInteractionObject;
//				}
//			}
//			// ¾Æ´Ï¸é.. Object checkÇÒ¶§ CheckÇØ¾ßµÇ´Â °æ¿ìÀÌ´Ù.
//			else
//			{
//				break;
//			}
//		}

		iImageObjectCheck ++;
	}

	//------------------------------------------------------	
	// ÀÖÀ»¹ýÇÑ Sector¸¦ checkÇÑ´Ù.
	//------------------------------------------------------	
	for (currentY=sY2; currentY>=sY1; currentY--)	
	{	
		for (currentX=sX1; currentX<=sX2; currentX++)	
		{				
			const MSector& sector = m_pZone->GetSector(currentX, currentY);

			// ¾î¶² Object°¡ ÀÖÀ» °æ¿ì
			if (sector.IsExistObject() )			
			{		
				
				//file << "Exist Object!" << endl;

				// ObjectÀÇ Á¾·ù¿¡ µû¶ó¼­ ´Þ¸® checkÇÑ´Ù.
				// ¼±ÅÃÇÒ ¼ö ÀÖ´Â °ÍÀº Creature¿Í Item»ÓÀÌ´Ù.

				//------------------------------------------------
				//
				// Flying Creature°¡ ÀÖ´Â °æ¿ì
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

						// PlayerÀÌ¸é ¼±ÅÃÇÏÁö ¾Ê´Â´Ù.
						if (pCreature->GetID()!=pid)	
						{

							// FlyingCreature°¡ Â÷ÁöÇÏ´Â ¿µ¿ªÀ» ±¸ÇÑ´Ù.
							// FRAME_PACK¿¡¼­ Á¤º¸¸¦ ÀÐ¾î¾ß ÇÒ µí!!
							// FlyingCreatureÀÇ SectorÁÂÇ¥¸¦ PixelÁÂÇ¥·Î ¹Ù²ã¾ß ÇÑ´Ù.

							// FlyingCreatureÀÇ ZoneÀÇ ÁÂÇ¥¸¦ °è»êÇÑ´Ù.
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// ZoneÀÇ ÁÂÇ¥¸¦ È­¸éÀÇ ÁÂÇ¥·Î ¹Ù²Û´Ù.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature°¡ È­¸é¿¡ Á¸ÀçÇÏ´Â ¿µ¿ªÀ» °è»êÇØ³½´Ù.	
							frame = m_CreatureFPK[pCreature->GetCreatureFrameID(0)][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// ÇöÀç Sprite°¡ È­¸é¿¡ Ãâ·ÂµÇ´Â À§Ä¡
							objectPixelPoint.x += frame.GetCX();
							objectPixelPoint.y += frame.GetCY() - pCreature->GetZ();//FLYINGCREATURE_HEIGHT;
												// FramePack¿¡¼­ ³ôÀÌ¸¦ ¼³Á¤ÇÏ¹Ç·Î,
												// FlyingÀÎ °æ¿ìµµ ¶È°°´Ù.

							// mouseÁÂÇ¥°¡ Sprite¿¡ ¼ÓÇÏ´ÂÁö ¾Ë¾Æº¸±â
							// Sprite¿¡¼­ÀÇ ÁÂÇ¥°¡ »ö±òÀÌ ÀÖ´Â °æ¿ìÀÏ¶§
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
				// Creature°¡ ÀÖ´Â °æ¿ì
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

						// PlayerÀÌ¸é ¼±ÅÃÇÏÁö ¾Ê´Â´Ù.
						if (pCreature->GetID()!=pid)	
						{

							// Creature°¡ Â÷ÁöÇÏ´Â ¿µ¿ªÀ» ±¸ÇÑ´Ù.
							// FRAME_PACK¿¡¼­ Á¤º¸¸¦ ÀÐ¾î¾ß ÇÒ µí!!
							// CreatureÀÇ SectorÁÂÇ¥¸¦ PixelÁÂÇ¥·Î ¹Ù²ã¾ß ÇÑ´Ù.

							// CreatureÀÇ ZoneÀÇ ÁÂÇ¥¸¦ °è»êÇÑ´Ù.
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// ZoneÀÇ ÁÂÇ¥¸¦ È­¸éÀÇ ÁÂÇ¥·Î ¹Ù²Û´Ù.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature°¡ È­¸é¿¡ Á¸ÀçÇÏ´Â ¿µ¿ªÀ» °è»êÇØ³½´Ù.	
							frame = m_CreatureFPK[pCreature->GetCreatureFrameID(0)][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// ÇöÀç Sprite°¡ È­¸é¿¡ Ãâ·ÂµÇ´Â À§Ä¡
							objectPixelPoint.x += frame.GetCX();
							objectPixelPoint.y += frame.GetCY();

							// mouseÁÂÇ¥°¡ Sprite¿¡ ¼ÓÇÏ´ÂÁö ¾Ë¾Æº¸±â
							// Sprite¿¡¼­ÀÇ ÁÂÇ¥°¡ »ö±òÀÌ ÀÖ´Â °æ¿ìÀÏ¶§
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
				// Underground Creature°¡ ÀÖ´Â °æ¿ì
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

						// PlayerÀÌ¸é ¼±ÅÃÇÏÁö ¾Ê´Â´Ù.
						if (pCreature->GetID()!=pid)	
						{

							// Creature°¡ Â÷ÁöÇÏ´Â ¿µ¿ªÀ» ±¸ÇÑ´Ù.
							// FRAME_PACK¿¡¼­ Á¤º¸¸¦ ÀÐ¾î¾ß ÇÒ µí!!
							// CreatureÀÇ SectorÁÂÇ¥¸¦ PixelÁÂÇ¥·Î ¹Ù²ã¾ß ÇÑ´Ù.

							// CreatureÀÇ ZoneÀÇ ÁÂÇ¥¸¦ °è»êÇÑ´Ù.
							objectPixelPoint = MapToPixel(pCreature->GetX(), pCreature->GetY());
							objectPixelPoint.x += m_PlusPoint.x + pCreature->GetSX();
							objectPixelPoint.y += m_PlusPoint.y + pCreature->GetSY();
								
							// ZoneÀÇ ÁÂÇ¥¸¦ È­¸éÀÇ ÁÂÇ¥·Î ¹Ù²Û´Ù.
							objectPixelPoint.x -= firstZonePixel.x;
							objectPixelPoint.y -= firstZonePixel.y;

							// Creature°¡ È­¸é¿¡ Á¸ÀçÇÏ´Â ¿µ¿ªÀ» °è»êÇØ³½´Ù.	
							// ÁÂÇ¥ º¸Á¤
							//frame = m_CreatureFPK[pCreature->GetCreatureFrameID()][pCreature->GetAction()][pCreature->GetDirection()][pCreature->GetFrame()];
							
							// ÇöÀç Sprite°¡ È­¸é¿¡ Ãâ·ÂµÇ´Â À§Ä¡
							//objectPixelPoint.x += frame.GetCX();
							//objectPixelPoint.y += frame.GetCY();

							// mouseÁÂÇ¥°¡ Sprite¿¡ ¼ÓÇÏ´ÂÁö ¾Ë¾Æº¸±â
							// Sprite¿¡¼­ÀÇ ÁÂÇ¥°¡ »ö±òÀÌ ÀÖ´Â °æ¿ìÀÏ¶§
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
				// ItemÀÌ ÀÖ´Â °æ¿ì
				//
				//------------------------------------------------				
				pObject = (MObject*)sector.GetItem();

				if (pObject != NULL)
				{	
					//file << "Exist Item!" << endl;

					pItem = (MItem*)pObject;

					// Item°¡ Â÷ÁöÇÏ´Â ¿µ¿ªÀ» ±¸ÇÑ´Ù.
					// FRAME_PACK¿¡¼­ Á¤º¸¸¦ ÀÐ¾î¾ß ÇÒ µí!!
					// ItemÀÇ SectorÁÂÇ¥¸¦ PixelÁÂÇ¥·Î ¹Ù²ã¾ß ÇÑ´Ù.

					// ItemÀÇ ZoneÀÇ ÁÂÇ¥¸¦ °è»êÇÑ´Ù.
					objectPixelPoint = MapToPixel(pItem->GetX(), pItem->GetY());
					objectPixelPoint.x += m_PlusPoint.x;// + pItem->GetSX();
					objectPixelPoint.y += m_PlusPoint.y;// + pItem->GetSY();
						
					// ZoneÀÇ ÁÂÇ¥¸¦ È­¸éÀÇ ÁÂÇ¥·Î ¹Ù²Û´Ù.
					objectPixelPoint.x -= firstZonePixel.x;
					objectPixelPoint.y -= firstZonePixel.y;

					// Item°¡ È­¸é¿¡ Á¸ÀçÇÏ´Â ¿µ¿ªÀ» °è»êÇØ³½´Ù.				
					frame = m_ItemTileFPK[ pItem->GetTileFrameID() ];

					// ÇöÀç Sprite°¡ È­¸é¿¡ Ãâ·ÂµÇ´Â À§Ä¡
					objectPixelPoint.x += frame.GetCX();
					objectPixelPoint.y += frame.GetCY();

					// mouseÁÂÇ¥°¡ Sprite¿¡ ¼ÓÇÏ´ÂÁö ¾Ë¾Æº¸±â
					// Sprite¿¡¼­ÀÇ ÁÂÇ¥°¡ »ö±òÀÌ ÀÖ´Â °æ¿ìÀÏ¶§
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
		// Ãâ·Â ½ÃÁ¡ÀÌ currentYº¸´Ù ÀûÀº °æ¿ìÀÇ 
		// ImageObjectµéÀ» CheckÇÑ´Ù.
		// m_mapImageObject¿¡´Â Viewpoint°¡ key°ªÀÌ µÇ¾î¼­ SortµÇ¾î ÀÖ´Ù.
		//------------------------------------------------------
		while (iImageObjectCheck != m_mapImageObject.rend())
		{
			MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectCheck).second);

			// InteractionObjectÀÎ °æ¿ì¸¸ checkÇÑ´Ù.
//			if (pImageObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
//			{
//				MInteractionObject* const pInteractionObject = (MInteractionObject* const)pImageObject;
//
//				// Ãâ·Â ½ÃÁ¡ÀÌ sY1º¸´Ù ÀûÀº °æ¿ì..´Â Check
//				if (pInteractionObject->GetViewpoint() == currentY)
//				{
//					// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
//					objectPixelPoint.x = pInteractionObject->GetPixelX() - m_FirstZonePixel.x;
//					objectPixelPoint.y = pInteractionObject->GetPixelY() - m_FirstZonePixel.y;
//
//					// frame¿¡ °ü°èµÈ sprite¸¦ ±¸ÇÑ´Ù.
//					frame = m_ImageObjectFPK[ pInteractionObject->GetFrameID() ][ pInteractionObject->GetFrame() ];
//					sprite = frame.GetSpriteID();
//							
//					// ÁÂÇ¥ º¸Á¤
//					objectPixelPoint.x += frame.GetCX();
//					objectPixelPoint.y += frame.GetCY();
//										
//					// InteractionObject´Â (point.x, point.y)¿¡ Ãâ·ÂµÇ¾î ÀÖÀ» °ÍÀÌ´Ù.
//					// mouseÁÂÇ¥°¡ Sprite¿¡ ¼ÓÇÏ´ÂÁö ¾Ë¾Æº¸±â
//					// Sprite¿¡¼­ÀÇ ÁÂÇ¥°¡ »ö±òÀÌ ÀÖ´Â °æ¿ìÀÏ¶§
//					if (m_ImageObjectSPK[ sprite ].IsColorPixel( 
//							x - objectPixelPoint.x, 
//							y - objectPixelPoint.y 
//						))
//					{							
//						return pInteractionObject;
//					}
//				}
//				// ¾Æ´Ï¸é.. Object checkÇÒ¶§ CheckÇØ¾ßµÇ´Â °æ¿ìÀÌ´Ù.
//				else
//				{
//					break;
//				}
//			}

			iImageObjectCheck ++;
		}
	}	

	//------------------------------------------------------
	// Ãâ·Â ½ÃÁ¡ÀÌ currentYº¸´Ù ÀûÀº °æ¿ìÀÇ 
	// ImageObjectµéÀ» CheckÇÑ´Ù.
	// m_mapImageObject¿¡´Â Viewpoint°¡ key°ªÀÌ µÇ¾î¼­ SortµÇ¾î ÀÖ´Ù.
	//------------------------------------------------------
	while (iImageObjectCheck != m_mapImageObject.rend())
	{
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectCheck).second);

		// InteractionObjectÀÎ °æ¿ì¸¸ checkÇÑ´Ù.
//		if (pImageObject->GetObjectType()==MObject::TYPE_INTERACTIONOBJECT)
//		{
//			MInteractionObject* const pInteractionObject = (MInteractionObject* const)pImageObject;
//		
//			// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
//			objectPixelPoint.x = pInteractionObject->GetPixelX() - m_FirstZonePixel.x;
//			objectPixelPoint.y = pInteractionObject->GetPixelY() - m_FirstZonePixel.y;
//
//			// frame¿¡ °ü°èµÈ sprite¸¦ ±¸ÇÑ´Ù.
//			frame = m_ImageObjectFPK[ pInteractionObject->GetFrameID() ][ pInteractionObject->GetFrame() ];
//			sprite = frame.GetSpriteID();
//							
//			// ÁÂÇ¥ º¸Á¤
//			objectPixelPoint.x += frame.GetCX();
//			objectPixelPoint.y += frame.GetCY();
//									
//			// InteractionObject´Â (point.x, point.y)¿¡ Ãâ·ÂµÇ¾î ÀÖÀ» °ÍÀÌ´Ù.
//			// mouseÁÂÇ¥°¡ Sprite¿¡ ¼ÓÇÏ´ÂÁö ¾Ë¾Æº¸±â
//			// Sprite¿¡¼­ÀÇ ÁÂÇ¥°¡ »ö±òÀÌ ÀÖ´Â °æ¿ìÀÏ¶§
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
// ´Ù Áö¿öÁà¾ß ÇÑ´Ù.
//----------------------------------------------------------------------
void			
MTopView::ClearItemNameList()
{
	DRAWITEMNAME_LIST::iterator iItemName = m_listDrawItemName.begin();

	// ¸ðµç node¸¦ Áö¿î´Ù.
	while (iItemName != m_listDrawItemName.end())
	{
		DRAWITEMNAME_NODE* pNode = *iItemName;

		// ¸Þ¸ð¸®¿¡¼­ nodeÁ¦°Å
		delete pNode;
		
		iItemName ++;
	}

	m_listDrawItemName.clear();

}

//----------------------------------------------------------------------
// Add ItemName
//----------------------------------------------------------------------
// ¿ÜºÎ¿¡¼­ new¸¦ ÇØÁà¾ß ÇÑ´Ù.
//
// pNode->GetDistance·Î sortÇØ¼­ Ãß°¡ÇØ¾ß ÇÑ´Ù.
// ¿À¸§Â÷¼ø..
//----------------------------------------------------------------------
// itemÀÌ ¸¹Àº °æ¿ì´Â µÚ¿¡¼­ºÎÅÍ Ã¼Å©ÇÏ´Â°Ô ºü¸£´Ù.
// MAX°ªÀÌ Á¤ÇØÁ® ÀÖ±â ¶§¹®¿¡...
// ±×·¯³ª.. °íÄ¡±â ±ÍÂú±º.. --;
// ÀÌ°Å Ä¡´Â ½Ã°£¿¡ °íÃÆ°Ú´Ù.. À½³Ä..
//----------------------------------------------------------------------
void		 	
MTopView::AddItemName(DRAWITEMNAME_NODE* pNode)
{		
	DRAWITEMNAME_LIST::iterator iItemName = m_listDrawItemName.begin();

	BOOL bInsertEnd = TRUE;

	//-------------------------------------------------------
	// °Å¸®¸¦ ºñ±³ÇÏ¸é¼­ ItemNameÀ» Ãß°¡ÇÑ´Ù.
	//-------------------------------------------------------
	while (iItemName != m_listDrawItemName.end())
	{
		DRAWITEMNAME_NODE* pOldNode = *iItemName;

		//------------------------------------------
		// ºñ±³ ºñ±³~
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
		// ¸Ç ³¡¿¡ Ãß°¡
		m_listDrawItemName.push_back( pNode );
	}
	
	//-------------------------------------------------------
	// MAX¸¦ ³Ñ¾î°£ °æ¿ì
	//-------------------------------------------------------
	if (m_listDrawItemName.size() > g_pClientConfig->MAX_DRAWITEMNAME)
	{
		// ¸¶Áö¸· °É Á¦°ÅÇÑ´Ù.
		DRAWITEMNAME_NODE* pDeleteNode = m_listDrawItemName.back();

		delete pDeleteNode;

		m_listDrawItemName.pop_back();
	}
}

//----------------------------------------------------------------------
// DrawItemNameList
//----------------------------------------------------------------------
// °¢°¢ÀÇ Item¸¶´Ù AddText¸¦ ÇÑ´Ù.
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

	// minimapÀ» ±×¸®´Â °æ¿ì.. 
	// ÁÂÇ¥ ¶«½Ã...
	// ¾Æ·¡ÂÊºÎÅÍ ±×¸°´Ù..

	// 2001.7.14 ÁÖ¼®Ã³¸®
	//if (g_pUserOption->DrawMinimap)// && g_pPlayer->IsSlayer())
	{
		// ¿ìÇìÇì ÇÏµå ÄÚµù~~
		y = 160;
	}

	while (iItemName != m_listDrawItemName.end())
	{
		DRAWITEMNAME_NODE* pNode = *iItemName;

		MItem* pItem = pNode->GetItem();


		const char* pItemName = pItem->GetName();

		// optionÀÌ ºÙÀº°Å´Â ÆÄ¶õ»ö..
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
		// add by Sonic 2006.10.28 Ôö¼ÓÏÔÊ¾ÈýÊôÐÔ×°±¸ÎªºìÉ«
		else if(pItem->GetItemOptionListCount() > 2)
		{
			nameColor = g_pClientConfig->COLOR_NAME_VAMPIRE; //Red
		}
		// end by Sonic 2006.10.28 Ôö¼ÓÏÔÊ¾ÈýÊôÐÔ×°±¸ÎªºìÉ«
		else
		{
			nameColor = m_ColorNameItemOption;
		}

		if (pItemName != NULL)
		{
			// È­¸é ¾È¿¡ ±ÛÀÚ°¡ µé¾î¿Àµµ·Ï..
			x = g_GameRect.right - g_GetStringWidth( pItemName, g_ClientPrintInfo[FONTID_ITEM]->hfont );
			x2 = g_GameRect.right;
			
			y2 = y + g_pClientConfig->FONT_ITEM_HEIGHT;

			// °ËÀº»ö ¹Ú½º ¿µ¿ª
			RECT rect = {
				x, 
				y,
				x2,
				y2
			};

			// mouse·Î ¼±ÅÃµÈ ItemÀÎ °æ¿ì
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

				pTextNode->SetBox( rect, m_ColorUIBoxBg );		

				AddText( pTextNode );
			}	

			// item NameÀÇ ¼±ÅÃ ¿µ¿ª
			pNode->SetRect( rect );
		}

		// ´ÙÀ½..
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
// È­¸é ÁÂÇ¥(x,y)¸¦ ¼±ÅÃÇÑ °æ¿ì
// ±× ÁÂÇ¥¿¡ ItemNameÀÌ ÀÖ´Â°¡?
//----------------------------------------------------------------------
MItem*
MTopView::SelectItemName(int x, int y)
{
	DRAWITEMNAME_LIST::iterator iItemName = m_listDrawItemName.begin();

	while (iItemName != m_listDrawItemName.end())
	{
		DRAWITEMNAME_NODE* pNode = *iItemName;

		//--------------------------------------------
		// ItemNameÀÇ ¼±ÅÃ¿µ¿ª¿¡ ¼ÓÇÏ¸é?
		//--------------------------------------------
		if (pNode->IsPointInRect(x,y))
		{			
			// item pointer¸¦ ³Ñ°ÜÁØ´Ù.
			// zone¿¡ ¾ø´Â °æ¿ìµµ ÀÖÀ» ¼ö ÀÖÀ¸¹Ç·Î...
			// zone¿¡¼­ ÀÐ¾î¿Í¾ß ÇÑ´Ù.
			return m_pZone->GetItem( pNode->GetID() );		
		}
	
		// ´ÙÀ½ item name
		iItemName ++;
	}

	return NULL;
}

//----------------------------------------------------------------------
// Clear TextList
//----------------------------------------------------------------------
// ´Ù Áö¿öÁà¾ß ÇÑ´Ù.
//----------------------------------------------------------------------
void			
MTopView::ClearTextList()
{
	while (!m_pqDrawText.empty())
	{
		DRAWTEXT_NODE* pNode = m_pqDrawText.top();

		// ¸Þ¸ð¸®¿¡¼­ nodeÁ¦°Å
		delete pNode;
		
		m_pqDrawText.pop();
	}
}

//----------------------------------------------------------------------
// Add Text
//----------------------------------------------------------------------
// DrawTextList¿¡ Ãâ·ÂÇÒ stringÀÇ Á¤º¸¸¦ Ãß°¡ÇÑ´Ù.
// ¿ÜºÎ¿¡¼­ new¸¦ ÇØÁà¾ß ÇÑ´Ù.
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

	// ³ªÁß¿¡ ÂïÈ÷°Ô ÇÏ±â
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
// Unlock»óÅÂ¿¡¼­ Ãâ·ÂÇÑ´Ù°í °¡Á¤ÇÑ´Ù.
//----------------------------------------------------------------------
void			
MTopView::DrawTextList()
{
	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "Start DrawTextList" );
	#endif
	
	//DRAWTEXT_PQ::iterator iText = m_pqDrawText.begin();

	//--------------------------------------------------------
	// 3D °¡¼Ó »ç¿ëÀÌ¸é...
	// ÀÏ´Ü ¼¡~~ ¾îµÓ°Ô.. --;
	// Lock/UnlockÀ» ´ú ¾²±â À§ÇØ¼­..
	//
	// ±×·±µ¥! 
	// ±ÛÀÚ ÂïÀ» ¶§, ¹Ù·Î Á÷Àü¿¡ box¸¦ ±×·ÁÁà¾ß ÇÑ´Ù. ÄÊ...
	//--------------------------------------------------------
	/*
	if (CDirect3D::IsHAL())
	{
		m_pSurface->Unlock();

		// ¸ðµç node¸¦ Áö¿î´Ù.
		while (iText != m_pqDrawText.end())
		{
			DRAWTEXT_NODE* pNode = *iText;

			//--------------------------------------------------------
			// ±ÛÀÚÃâ·ÂµÇ´Â ºÎºÐ¿¡ °ËÀº»ö »óÀÚ Ãâ·Â
			//--------------------------------------------------------
			RECT rect = { pNode->GetX(), pNode->GetY(), 
						pNode->GetXPlusWidth(), pNode->GetY(),15 };
			
			// °ËÀº ¹Ú½º Ãâ·Â..
			DrawBox3D(&rect, 0x7000);			

			iText++;
		}

		m_pSurface->Lock();	
		
		// ´Ù½Ã Ã³À½À¸·Î..
		iText = m_pqDrawText.begin();
	}
	*/

	//m_pSurface->Unlock();
	//--------------------------------------------------------
	// ¸ðµç node¸¦ Ãâ·ÂÇÏ°í Áö¿î´Ù.
	//--------------------------------------------------------
	RECT rect2;

	while (!m_pqDrawText.empty())
	{
		DRAWTEXT_NODE* pNode = m_pqDrawText.top();

		//--------------------------------------------------------
		// Box¸¦ Ãâ·ÂÇÒ±î?
		//--------------------------------------------------------
		if (pNode->IsExistBox())
		{
			//--------------------------------------------------------
			// ±ÛÀÚÃâ·ÂµÇ´Â ºÎºÐ¿¡ °ËÀº»ö »óÀÚ Ãâ·Â
			//--------------------------------------------------------
			RECT rect = pNode->GetBox();
			
			//--------------------------------------------------------
			// Åõ¸íÇÑ ¹Ú½ºÀÎ °æ¿ì
			//--------------------------------------------------------
			if (pNode->IsTransBox())
			{
				//--------------------------------------------------------
				// 3D °¡¼ÓÀÇ °æ¿ì
				//--------------------------------------------------------
//				if (CDirect3D::IsHAL())
//				{
//					// Unlock / lock... À¸¾Ç~~ ÀÌ°Å ½Ã°£ ¸¹ÀÌ Àâ¾Æ¸ÔÀ»°Çµ¥..
//					//m_pSurface->Unlock();
//
//					// ¹Ú½º Ãâ·Â..
//					#ifdef OUTPUT_DEBUG_DRAW_3DBOX
//						DEBUG_ADD_FORMAT("TextList-Box3D( %d, %d, %d, %d ), color=%x",
//														rect.left, rect.top, rect.right, rect.bottom, pNode->GetBoxColor());
//					#endif
//
//					// ¹Ù´Ú ±ò±â
//					WORD boxColor = pNode->GetBoxColor();
//
//					rect2 = rect;
//					DrawBox3D(&rect2, boxColor);
//
//					if (pNode->IsExistBoxOutline())
//					{
//						// ¿Ü°û ¹Ú½º ±×¸®±â
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
//						// ¼±
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

						// ¿Ü°û ¹Ú½º ±×¸®±â
						rect2 = rect;
						DRAW_CHAT_BOX_OUTLINE( rect2, color )

						// Lock
						m_pSurface->Lock();

						WORD boxColor = pNode->GetBoxColor();

						// ¹Ù´Ú ±×¸®±â
						rect2 = rect;
						DRAW_ALPHA_BOX_2D( rect2, boxColor );
						
						// °ËÁ¤»öÀÎ °æ¿ì
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
								//m_pSurface->GammaBox565(&rect2, 15);
								
								rect2.left	= rect.left + CHAT_BOX_TAIL_X;
								rect2.top	= rect.bottom;
								rect2.right = rect.left + CHAT_BOX_TAIL_X + 6;
								rect2.bottom = rect.bottom + 2;								
								//m_pSurface->GammaBox565(&rect2, 15);

								rect2.left	= rect.left + CHAT_BOX_TAIL_X+1;
								rect2.top	= rect.bottom + 2;
								rect2.right = rect.left + CHAT_BOX_TAIL_X+1 + 4;
								rect2.bottom = rect.bottom + 2 + 2;
								//m_pSurface->GammaBox565(&rect2, 15);

								rect2.left	= rect.left + CHAT_BOX_TAIL_X+2;
								rect2.top	= rect.bottom + 4;
								rect2.right = rect.left + CHAT_BOX_TAIL_X+2 + 2;
								rect2.bottom = rect.bottom + 4 + 2;
								//m_pSurface->GammaBox565(&rect2, 15);								
							}
							// 2D 5:5:5
							else
							{
								rect2 = rect;
								//m_pSurface->GammaBox555(&rect2, 15);
								
								rect2.left	= rect.left + CHAT_BOX_TAIL_X;
								rect2.top	= rect.bottom;
								rect2.right = rect.left + CHAT_BOX_TAIL_X + 6;
								rect2.bottom = rect.bottom + 2;								
								//m_pSurface->GammaBox555(&rect2, 15);

								rect2.left	= rect.left + CHAT_BOX_TAIL_X+1;
								rect2.top	= rect.bottom + 2;
								rect2.right = rect.left + CHAT_BOX_TAIL_X+1 + 4;
								rect2.bottom = rect.bottom + 2 + 2;
								//m_pSurface->GammaBox555(&rect2, 15);

								rect2.left	= rect.left + CHAT_BOX_TAIL_X+2;
								rect2.top	= rect.bottom + 4;
								rect2.right = rect.left + CHAT_BOX_TAIL_X+2 + 2;
								rect2.bottom = rect.bottom + 4 + 2;
								//m_pSurface->GammaBox555(&rect2, 15);
							}					

							
						}
						// ´Ù¸¥ »öÀÎ °æ¿ì
						else
						{
							m_pSurface->BltColorAlpha(&rect, boxColor, 15);
						}

						// ¼±
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
			// Åõ¸íÇÑ ¹Ú½º°¡ ¾Æ´Ñ °æ¿ì¿¡
			//--------------------------------------------------------
			else
			{
				m_pSurface->FillRect(&rect, pNode->GetBoxColor());				
			}
		}

		//--------------------------------------------------------
		// Ãâ·ÂÇÒ ±ÛÀÚ°¡ ÀÖ´Â °æ¿ì..
		//--------------------------------------------------------
		const char* pString = pNode->GetString();

		if (pString!=NULL && pString[0]!='\0')
		{
			//--------------------------------------------------------
			// Ãâ·Â ÁÂÇ¥
			//--------------------------------------------------------
			int x = pNode->GetX();
			int y = pNode->GetY();

			//--------------------------------------------------------
			// font ¼³Á¤
			//--------------------------------------------------------	
			PrintInfo* pPrintInfo = g_ClientPrintInfo[pNode->GetFont()];				

			//--------------------------------------------------------
			// flag¿¡ µû¸¥ Ã³¸®µµ ÇØÁà¾ß ÇÑ´Ù....				
			//--------------------------------------------------------
			if (pNode->IsOutline())
			{
				// ¿ª»ó color¼³Á¤
				pPrintInfo->text_color = (~pNode->GetColor()) & 0x00FFFFFF;
					
				// Ãâ·Â
				g_Print(x-1, y-1, pString, pPrintInfo);
				g_Print(x+1, y+1, pString, pPrintInfo);
			}

			// text color¼³Á¤
			pPrintInfo->text_color = pNode->GetColor();
				
			//--------------------------------------------------------
			// string Ãâ·Â
			//--------------------------------------------------------
			g_Print(x, y, pString, pPrintInfo);
		}

		// ¸Þ¸ð¸®¿¡¼­ Á¦°ÅÇÑ´Ù.
		delete pNode;
		
		// ´ÙÀ½..
		m_pqDrawText.pop();
	}

	// ¸ðµç node¸¦ Á¦°ÅÇÑ´Ù.
	//m_pqDrawText.clear();

	// Lock»óÅÂ
	//m_pSurface->Lock();

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "End DrawTextList" );
	#endif
}

//----------------------------------------------------------------------
// Clear LightBufferFilter
//----------------------------------------------------------------------
// LightBuffer¸¦ ÃÊ±âÈ­ÇÑ´Ù.
// ±âº»ÀûÀ¸·Î PlayerÀÇ ½Ã¾ß¸¦ Ç¥ÇöÇØÁàµµ µÈ´Ù.
//----------------------------------------------------------------------
void
MTopView::ClearLightBufferFilter3D()
{
	//if (CDirect3D::IsHAL() && 
	//if	(m_DarkBits)
	{
		//--------------------------------------------------
		// ºûÀÇ °ª ¼³Á¤
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
		// ÃÊ±âÈ­ - È­¸é ÀüÃ¼¸¦ Ã¤¿ï ºû..
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
// LightBuffer¸¦ ÃÊ±âÈ­ÇÑ´Ù.
// ±âº»ÀûÀ¸·Î PlayerÀÇ ½Ã¾ß¸¦ Ç¥ÇöÇØÁàµµ µÈ´Ù.
//----------------------------------------------------------------------
void
MTopView::ClearLightBufferFilter2D()
{
	//if (1)//CDirect3D::IsHAL() && m_b3DLight && m_DarkBits)
	{
		//--------------------------------------------------
		// ºûÀÇ °ª ¼³Á¤
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
		// Check if filter is initialized (SDL backend defensive check)
		//--------------------------------------------------
		if (m_LightBufferFilter.IsNotInit()) {
			m_nLight = 0;
			return;
		}

		//--------------------------------------------------
		// ÃÊ±âÈ­ - È­¸é ÀüÃ¼¸¦ Ã¤¿ï ºû..
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
// LightFilter¸¦ Ãß°¡ÇÑ´Ù.
// ScreenÁÂÇ¥¸¦ LightBuffer¿¡ ¸Âµµ·Ï ¹Ù²ã¾ßÇÑ´Ù.
//
// (CLIPSURFACE_WIDTH, CLIPSURFACE_WIDTH) 
//  --> (SCREENLIGHT_WIDTH, SCREENLIGHT_HEIGHT)·Î..
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
		|| bForceLight) // ¹«Á¶°Ç Ãâ·ÂÇØ¾ßÇÏ´Â ºû
	{
		// tile ÁÂÇ¥ º¸Á¤ 
		 //x += 24;
		 //y += 24;

		// Light FilterÀÇ ¹üÀ§¸¦ ³ÑÁö ¾Êµµ·Ï ÇÑ´Ù.
		if (range >= m_LightFTP.GetSize())
		{
			range = m_LightFTP.GetSize()-1;
		}		

		//-----------------------------------------------------
		// range¿¡ µû¶ó¼­ Ãâ·ÂÇÒ lightFilter¸¦ °áÁ¤ÇØ¾ßÇÑ´Ù.
		//-----------------------------------------------------		
		int filterID = range;

		//-----------------------------------------------------
		// LightFilter Ãâ·Â ½ÃÁ¡ °áÁ¤
		/// ±×¸®°í, filterÁÂÇ¥ (fx,fy)·Î ¹Ù²ã¾ß ÇÏ°í..
		// filterÀÇ Áß½ÉÀ» (x,y)¿¡ ¿Àµµ·Ï ¸ÂÃç¾ß ÇÑ´Ù.
		//-----------------------------------------------------
		int fx, fy;

		// (x,y)°¡ ¾î¶² ÁÂÇ¥ÀÎÁö¿¡ µû¶ó¼­ °è»êÀ» ´Ù¸£°Ô ÇØÁØ´Ù.
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

		// filter Áß½É ¸ÂÃß±â
		fx = fx - (m_LightFTP[filterID].GetWidth() >> 1);
		fy = fy - (m_LightFTP[filterID].GetHeight() >> 1);
			
		//-----------------------------------------------------
		// BufferFilter¿¡ Light filter¸¦ Ãß°¡ÇÑ´Ù.
		//-----------------------------------------------------
		m_LightBufferFilter.BltFilterAdd( fx, fy, m_LightFTP[filterID] );		

		m_nLight++;
	}
}

//----------------------------------------------------------------------
// Add LightFilter To Screen
//----------------------------------------------------------------------
// LightFilter¸¦ Ãß°¡ÇÑ´Ù.
// ScreenÁÂÇ¥¸¦ LightBuffer¿¡ ¸Âµµ·Ï ¹Ù²ã¾ßÇÑ´Ù.
//
// (CLIPSURFACE_WIDTH, CLIPSURFACE_WIDTH) 
//  --> (SCREENLIGHT_WIDTH, SCREENLIGHT_HEIGHT)·Î..
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
		|| bForceLight)	// ¹«Á¶°Ç Ãâ·ÂÇØ¾ßÇÏ´Â ºû
	{
		// tile ÁÂÇ¥ º¸Á¤ 
		//x += 24;
		//y += 24;

		// Light FilterÀÇ ¹üÀ§¸¦ ³ÑÁö ¾Êµµ·Ï ÇÑ´Ù.
		if (range >= m_LightFTP.GetSize())
		{
			range = m_LightFTP.GetSize()-1;
		}		

		//-----------------------------------------------------
		// range¿¡ µû¶ó¼­ Ãâ·ÂÇÒ lightFilter¸¦ °áÁ¤ÇØ¾ßÇÑ´Ù.
		//-----------------------------------------------------		
		int filterID = range;

		//-----------------------------------------------------
		// LightFilter Ãâ·Â ½ÃÁ¡ °áÁ¤
		/// ±×¸®°í, filterÁÂÇ¥ (fx,fy)·Î ¹Ù²ã¾ß ÇÏ°í..
		// filterÀÇ Áß½ÉÀ» (x,y)¿¡ ¿Àµµ·Ï ¸ÂÃç¾ß ÇÑ´Ù.
		//-----------------------------------------------------
		int fx, fy;

		// (x,y)°¡ ¾î¶² ÁÂÇ¥ÀÎÁö¿¡ µû¶ó¼­ °è»êÀ» ´Ù¸£°Ô ÇØÁØ´Ù.
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

		// filter Áß½É ¸ÂÃß±â
		fx = fx - (m_LightFTP[filterID].GetWidth() >> 1);
		fy = fy - (m_LightFTP[filterID].GetHeight() >> 1);
			
		//-----------------------------------------------------
		// BufferFilter¿¡ Light filter¸¦ Ãß°¡ÇÑ´Ù.
		//-----------------------------------------------------
		m_LightBufferFilter.BltFilterSub( fx, fy, m_LightFTP[filterID] );

		m_nLight++;
	}
}

//----------------------------------------------------------------------
// Add CreatureOutput All
//----------------------------------------------------------------------
// m_pZoneÀÇ Creature mapÀ» ÅëÂ°·Î~ Ãß°¡
//----------------------------------------------------------------------
// OutputMap¿¡¼­ÀÇ ID´Â CreatureÀÇ..
//
//    2 bytes + 2 bytes    = 4 bytes ·Î ±¸¼ºµÇ¾î ÀÖ´Ù.
// [pixel yÁÂÇ¥][object id]
//
// ÀÏ¹Ý creature´Â Ãß°¡ÇÒÁö ¸»Áö¸¦ ¼ýÀÚ·Î °áÁ¤ÇÏ°í..
// FakeCreature´Â tile¿¡´Â Ãß°¡¾ÈµÇ¹Ç·Î.. ¹«Á¶°Ç ¿©±â¼­ Ãß°¡ÇØ¾ßÇÑ´Ù.
//----------------------------------------------------------------------
void	
MTopView::AddOutputCreatureAll()
{
	// ÃÊ±âÈ­
	m_mapCreature.clear();

	// [»õ±â¼ú3] °ü ¼Ó¿¡ ÀÖÀ»¶§´Â ÀÚ±â¸¸ Ãâ·ÂÇÏ°Ô ÇÑ´Ù.
	if (g_pPlayer->IsInCasket())
	{
		m_bTileSearchForCreature = false;	

		QWORD key = GetOutputCreatureID(g_pPlayer);

		// (key°ª, pCreature)¸¦ Ãß°¡ÇÑ´Ù.
		m_mapCreature.insert(
			CREATURE_OUTPUT_MAP::value_type( key, g_pPlayer )
		);

		return;
	}


	MZone::CREATURE_MAP::const_iterator iCreature;

	MCreature* pCreature;

	int num = m_pZone->GetCreatureNumber();

	// 20¸í ÀÌ»ó ÀÖÀ¸¸é ... 
	// ±×³É °Ë»öÇÏ´Â°Ô ³´´Ù°í ÆÇ´Ü.. Á¤¸»ÀÏ±î? - -;
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
	// TileÀ» °Ë»öÇØ¼­ Ãâ·ÂÇÏ´Â °æ¿ì°¡ ¾Æ´Ï¸é..
	// ¸ðµç Creature¸¦ OutputMap¿¡ Ãß°¡ÇØ¾ß ÇÑ´Ù.
	//---------------------------------------------------------
	if (!m_bTileSearchForCreature)
	{
		iCreature = m_pZone->GetCreatureBegin();

		for (int i=0; i<num; i++)
		{
			pCreature = iCreature->second;

			// key °ª = [pixel YÁÂÇ¥][object id]
			QWORD key = GetOutputCreatureID(pCreature);

			// (key°ª, pCreature)¸¦ Ãß°¡ÇÑ´Ù.
			m_mapCreature.insert(
				CREATURE_OUTPUT_MAP::value_type( key, pCreature )
			);
		
			// ´ÙÀ½ Creature
			iCreature++;
		}
	}

	//---------------------------------------------------------
	// Fake Creature
	//---------------------------------------------------------
	iCreature = m_pZone->GetFakeCreatureBegin();
	num = m_pZone->GetFakeCreatureNumber();
	
	//---------------------------------------------------------
	// ¸ðµç Creature¸¦ OutputMap¿¡ Ãß°¡ÇØ¾ß ÇÑ´Ù.
	//---------------------------------------------------------
	for (int i=0; i<num; i++)
	{
		pCreature = iCreature->second;

		// key °ª = [pixel YÁÂÇ¥][object id]
		QWORD key = GetOutputCreatureID(pCreature);

		// (key°ª, pCreature)¸¦ Ãß°¡ÇÑ´Ù.
		m_mapCreature.insert(
			CREATURE_OUTPUT_MAP::value_type( key, pCreature )
		);
	
		// ´ÙÀ½ Creature
		iCreature++;
	}
}

//----------------------------------------------------------------------
// Add OutputCreature
//----------------------------------------------------------------------
// pCreature¸¦ Ãß°¡ÇÑ´Ù.
//----------------------------------------------------------------------
bool	
MTopView::AddOutputCreature(MCreature* pCreature)
{	
	// key°ª »ý¼º
	QWORD key = GetOutputCreatureID(pCreature);

	// ÀÌ¹Ì ÀÖ´ÂÁö È®ÀÎÇØº¸°í ¾øÀ¸¸é Ãß°¡.
	if (m_mapCreature.find( key ) == m_mapCreature.end())
	{			
		// (key°ª, pCreature)¸¦ Ãß°¡ÇÑ´Ù.
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
// id¿Í °ü·ÃÀÖ´Â creature¸¦ Á¦°ÅÇÑ´Ù.
//----------------------------------------------------------------------
bool	
MTopView::RemoveOutputCreature(MCreature* pCreature)
{
	// ÀÌ¹Ì ÀÖ´ÂÁö È®ÀÎÇØº¸°í ÀÖÀ¸¸é Á¦°Å
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
// ID°¡ idÀÎ CreatureÀÇ ÁÂÇ¥°¡ y0¿¡¼­ y1À¸·Î ¹Ù²ï°É updateÇÑ´Ù.
//----------------------------------------------------------------------
// id¿Í °ü·ÃÀÖ´Â creatureÀÇ Á¤º¸(Ãâ·ÂÁÂÇ¥°ü·Ãid)¸¦ UpdateÇÑ´Ù.
// ÀÖÀ¸¸é.. Á¦°ÅÇÏ°í.. ´Ù½Ã Ãß°¡ÇØ¾ß ÇÑ´Ù.
//----------------------------------------------------------------------
bool			
MTopView::UpdateOutputCreature(TYPE_OBJECTID id, int y0, int y1)
{
	// ÀÌ¹Ì ÀÖ´ÂÁö È®ÀÎÇØº¸°í ÀÖÀ¸¸é update
	CREATURE_OUTPUT_MAP::iterator iCreature 
		= m_mapCreature.find( GetOutputCreatureID(id, y0) );

	if (iCreature != m_mapCreature.end())
	{			
		MCreature* pCreature = (*iCreature).second;

		// Á¦°Å
		m_mapCreature.erase( iCreature );

		// (key°ª, pCreature)¸¦ ´Ù½Ã Ãß°¡ÇÑ´Ù.
		m_mapCreature.insert(
			CREATURE_OUTPUT_MAP::value_type( 
					GetOutputCreatureID(id, y1),	// key°ª
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
// °ËÀº»ö ¹ÝÅõ¸í ¹Ú½º¸¦ ±×¸°´Ù. 3D
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
//	// ´Ù¸¥ »ö±òÀÏ °æ¿ì.. ´Ù½Ã ¼³Á¤..
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
// 2D/3D°¡¼Ó¿¡ °ü°è¾øÀÌ...
// ÇöÀçÀÇ Lock/Unlock »óÅÂ¿¡ °ü°è¾øÀÌ... ÀÛµ¿ÇÑ´Ù.
//
// m_pSurface¿¡ 
// (r,g,b)¿Í alpha°ª(1~31)¿¡ µû¶ó¼­ pRect¿µ¿ª¿¡ Åõ¸íÇÑ box¸¦ ±×¸°´Ù.
//----------------------------------------------------------------------
void
MTopView::DrawAlphaBox(RECT* pRect, BYTE r, BYTE g, BYTE b, BYTE alpha)
{
	WORD color;

	//--------------------------------------------------------
	// Win2000ÀÎ °æ¿ì.. ¿©±â¼­ ¾öÃ»³­ ¼ÓµµÀúÇÏ°¡ »ý±ä´Ù.. ¿Ö ±×·²±î?
	// ÀÏ´ÜÀº.. AlphaBox¸¦ »ç¿ëÇÏÁö ¾Ê´Â´Ù.
	// Å×½ºÆ® °á°ú.. ÀÌ°Ô ¹®Á¦°¡ ¾Æ´Ï¾ú´Ù... ¹¹°¡ ¹®Á¦ÀÏ±î. - -;
	//--------------------------------------------------------
	/*
	if (g_WindowsNT)
	{
		//------------------------------------------------
		// Unlock »óÅÂ·Î ¸¸µç´Ù.
		//------------------------------------------------
		BOOL bLock = m_pSurface->IsLock();
		if (bLock)
		{
			m_pSurface->Unlock();
		}
		
		color = CDirectDraw::Color(r,g,b);

		m_pSurface->FillRect(pRect, color);

		//------------------------------------------------
		// ¿ø·¡ lock µÈ »óÅÂ¸é ´Ù½Ã µÇµ¹¸°´Ù.
		//------------------------------------------------
		if (bLock)
		{
			m_pSurface->Lock();
		}
	}
	*/
	//--------------------------------------------------------
	// 98ÀÎ °æ¿ì..
	//--------------------------------------------------------
	//else
	{
		//--------------------------------------------------------
		// 3D °¡¼ÓÀÇ °æ¿ì
		//--------------------------------------------------------
//		if (CDirect3D::IsHAL())
//		{
//			//------------------------------------------------
//			// Unlock »óÅÂ·Î ¸¸µç´Ù.
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
//			// ´Ù¸¥ »ö±òÀÏ °æ¿ì.. ´Ù½Ã ¼³Á¤..
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
//			// ¿ø·¡ lock µÈ »óÅÂ¸é ´Ù½Ã µÇµ¹¸°´Ù.
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
			// Lock »óÅÂ·Î ¸¸µç´Ù.
			//------------------------------------------------
			BOOL bUnlock = !m_pSurface->IsLock();
			if (bUnlock)
			{
				m_pSurface->Lock();
			}

			color = CDirectDraw::Color(r,g,b);

			//-------------------------------------------------
			// °ËÁ¤»öÀÌ¸é.. ½±°Ô µÈ´Ù~
			//-------------------------------------------------
			if (color==0)
			{
				// 2D 5:6:5
				if (CDirectDraw::Is565())
				{
					//m_pSurface->GammaBox565(pRect, alpha);
				}
				// 2D 5:5:5
				else
				{
					//m_pSurface->GammaBox555(pRect, alpha);
				}
			}
			//-------------------------------------------------
			// ¾Æ´Ï¸é...
			//-------------------------------------------------
			else
			{
				m_pSurface->BltColorAlpha(pRect, color, alpha);
			}

			//------------------------------------------------
			// ¿ø·¡ÀÇ Lock »óÅÂ·Î µÇµ¹¸°´Ù.
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
// Debug¿¡ ÇÊ¿äÇÑ Á¤º¸¸¦ Ãâ·ÂÇÑ´Ù.
//
// ±ÛÀÚ Ãâ·ÂÇÒ¶§´Â Unlock»óÅÂ..
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
	// SurfaceÀÇ Á¤º¸¸¦ ÀúÀåÇØµÐ´Ù.
	//S_SURFACEINFO		SurfaceInfo;
	//SetSurfaceInfo(&SurfaceInfo, m_pSurface->GetDDSD());

	//-----------------------------------------------------------------
	// Font ¼±ÅÃ
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
	// ZoneÀÌ¸§ Ãâ·Â
	//-----------------------------------------------------------------
	/* // 2001.7.14 ÁÖ¼®Ã³¸®
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
	// °ÔÀÓ ½Ã°£ Ãâ·Â
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
		// 30ÃÊ¿¡ ÇÑ¹ø¾¿Àº ½Ã°£À» ¹Ù²ãÁØ´Ù.
		if (g_CurrentTime - lastDisplayGameTime > 30000)
		{
			g_pGameTime->SetCurrentTime( g_CurrentTime );

			lastDisplayGameTime = g_CurrentTime;
		}
	}
	*/

	//-----------------------------------------------------------------
	//
	// Item ºÎ¼­Áø°Å Ãâ·Â
	//
	//-----------------------------------------------------------------
	y += 10;
	DrawItemBroken(5, y);

	y += 70;	// ItemBrokenÀÇ Å©±â.. ±ÍÂú¾Æ¼­ ±â³É.. ¤»¤»

	//-----------------------------------------------------------------
	//
	// Event Message Ãâ·Â
	//
	//-----------------------------------------------------------------
	int strY = 30;
	int strX = g_GameRect.right /2;

	// Event String Ãâ·Â
	DrawEventString( strX, strY );

	//-----------------------------------------------------------------
	//
	// System Message Ãâ·Â
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
	int c;
	for (c=0; c<g_pPlayerMessage->GetSize(); c++)
	{
		if ((*g_pPlayerMessage)[c][0] != NULL)
		{
			// È¡³öÐÅÏ¢
			strcpy(message,(*g_pPlayerMessage)[c]);
			// È¡³öÑÕÉ«ÀàÐÍ
			pColorType = &message[strlen(message)-1];
			iColorType = atoi(pColorType);
			pColorType[0]=NULL;
			COLORREF color = RGB(20<<3,31<<3,12<<3);
			switch(iColorType)
			{
			case 0: // ÂÌÉ«
				color = RGB_GREEN;
				break;
			case 1: // À¶É«
				color = RGB(50,50,200);//RGB_BLUE;
				break;
			case 2: // »ÆÉ«
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

	// 5ÃÊ¸¶´Ù ÇÑ¹ø¾¿.. scroll
	static DWORD lastTime = g_CurrentTime;
	if (g_CurrentTime - lastTime >= g_pClientConfig->DELAY_SYSTEMMESSAGE)
	{
		g_pSystemMessage->Add("\0");		
		lastTime = g_CurrentTime;
	}

	// 7ÃÊ¸¶´Ù ÇÑ¹ø¾¿.. scroll
	static DWORD lastNoticeTime = g_CurrentTime;
	if (g_CurrentTime - lastNoticeTime >= (g_pClientConfig->DELAY_SYSTEMMESSAGE + 2000) )
	{
		g_pNoticeMessage->Add("\0");
		lastNoticeTime = g_CurrentTime;
	}

	// 7ÃÊ¸¶´Ù ÇÑ¹ø¾¿.. scroll
	static DWORD lastPlayerTimer = g_CurrentTime;
	if (g_CurrentTime - lastPlayerTimer >= (g_pClientConfig->DELAY_SYSTEMMESSAGE + 10000) )
	{
		g_pPlayerMessage->Add("\0");		
		lastPlayerTimer = g_CurrentTime;
	}
	//-----------------------------------------------------------------
	//
	// Game Message Ãâ·Â
	//
	//-----------------------------------------------------------------
	switch(g_pPlayer->GetRace())
	{
	case RACE_SLAYER:
		strX = 10;
		strY = 410;
		if (g_pPlayer->GetBonusPoint()!=0)
		{
			// bonus point ¿Ã¸±·Á°í ´©¸£´Â ¹öÆ°ÀÌ ÀÖ´Â À§Ä¡
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
			// bonus point ¿Ã¸±·Á°í ´©¸£´Â ¹öÆ°ÀÌ ÀÖ´Â À§Ä¡
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
			// bonus point ¿Ã¸±·Á°í ´©¸£´Â ¹öÆ°ÀÌ ÀÖ´Â À§Ä¡
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

	// 5ÃÊ¸¶´Ù ÇÑ¹ø¾¿.. scroll
	static DWORD gamelastTime = g_CurrentTime;
	if (g_CurrentTime - gamelastTime >= g_pClientConfig->DELAY_GAMEMESSAGE)
	{
		g_pGameMessage->Add("\0");
		gamelastTime = g_CurrentTime;
	}

	//-----------------------------------------------------------------
	//
	// ÃÑ¾Ë ³²Àº°Å Ãâ·Â
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
	// Ä³¸¯ÅÍ »óÅÂ Ãâ·Â
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
	

	// Äù½ºÆ® ºÎºÐÀº Äù½ºÆ® ÀÎÅÍÆäÀÌ½º·Î »«´Ù. ¾Æ·¡ºÎºÐÀº »ç¿ëµÇÁö ¾ÊÀ»µí..
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
						// ¾ÆÀÌÅÛ ¸ðÀ¸±â
						sprintf(str,"Áö±ÝÀº ¾ÆÀÌÅÛ ¸ðÀ¸´ÂÁß ¤»¤»¤» %d/%d %d",QuestEvent->parameter3, mkq->GetGoal(), QuestEvent->parameter4 - (timeGetTime() / 1000));
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
// Debug¿¡ ÇÊ¿äÇÑ Á¤º¸¸¦ Ãâ·ÂÇÑ´Ù.
//----------------------------------------------------------------------
void MTopView::DrawDebugInfo(void* pSurface)
{
	#ifdef PLATFORM_WINDOWS
		CDirectDrawSurface* pSurfaceCast = (CDirectDrawSurface*)pSurface;
	#else
		CSpriteSurface* pSurfaceCast = (CSpriteSurface*)pSurface;
	#endif
	#ifdef	OUTPUT_DEBUG		
		//----------------------------------------------------------------
		// debug¿ë code
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

		sprintf(str, "ID = %d / %d¸í [Weapon=%s] [align=%d]", g_pPlayer->GetID(), m_pZone->GetCreatureNumber(), attackMode, g_pPlayer->GetAlignment());	
		pSurfaceCast->GDI_Text(10,35, str, RGB(220,220,220));

		sprintf(str, " [ZoneID=%d] XY=(%d, %d), sXY=(%d, %d), SerXY=(%d, %d)", zoneID, g_pPlayer->GetX(), g_pPlayer->GetY(), g_pPlayer->GetSX(), g_pPlayer->GetSY(), g_pPlayer->GetServerX(), g_pPlayer->GetServerY());	
		pSurfaceCast->GDI_Text(10,56, str, RGB(220,220,220));

		sprintf(str, "SX = %d, SY = %d, HP=%d, MP=%d", g_pPlayer->GetSX(), g_pPlayer->GetSY(), g_pPlayer->GetHP(), g_pPlayer->GetMP());
		//sprintf(str, "[Wait Move = %d]", g_pPlayer->GetSendMove());
		pSurfaceCast->GDI_Text(10,72, str, RGB(220,220,220));

		sprintf(str, "ActionCount = [%d] %d/%d", g_pPlayer->GetAction(), g_pPlayer->GetActionCount(), g_pPlayer->GetActionCountMax());
		pSurfaceCast->GDI_Text(10,88, str, RGB(220,220,220));

		sprintf(str, "MoveCount = %d/%d", g_pPlayer->GetMoveCount(), g_pPlayer->GetMoveCountMax());
		pSurfaceCast->GDI_Text(10,104, str, RGB(220,220,220));

		sprintf(str, "Dark/Light/Sight = %d/%d/%d", m_DarkBits, g_pPlayer->GetLightSight() + g_pPlayer->GetItemLightSight(), g_pPlayer->GetSight());
		pSurfaceCast->GDI_Text(10,120, str, RGB(220,220,220));

		if (g_pProfiler!=NULL && g_pProfiler->HasProfileInfo(g_ProfileInfoName.GetString()))
		{
			DWORD totalTime = g_pProfiler->GetTotalTime(g_ProfileInfoName.GetString());
			DWORD times = g_pProfiler->GetTimes(g_ProfileInfoName.GetString());
			float avgTime = g_pProfiler->GetAverageTime(g_ProfileInfoName.GetString());

			sprintf(str, "%s = %d / %d = %7.3f", g_ProfileInfoName.GetString(), totalTime, times, avgTime);
			pSurfaceCast->GDI_Text(10,136, str, RGB(220,220,220));		
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
			pSurfaceCast->GDI_Text(10,225, str, RGB(220,220,220));	
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

		// Timer Ãâ·Â
		//sprintf(str, "Timer = %ld", g_CurrentTime);	
		//g_pBack->GDI_Text(11,11, str, RGB(20,20,20));
		//g_pBack->GDI_Text(10,20, str, 0xFFFFFF);
		

		// MouseÁÂÇ¥ Âï±â	
		//sprintf(str, "%d, %d", g_x, g_y);	
		//g_pBack->GDI_Text(11,101, str, RGB(20,20,20));
		//g_pBack->GDI_Text(10,100, str, RGB(255,255,255));

		//sprintf(str, "ImageObject=%d°³", m_mapImageObject.size());
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

		pSurfaceCast->GDI_Text(150,580, str, 0xFFFFFF);		

		// ³²Àº Texture¸Þ¸ð¸® test
		DDSCAPS2 ddsCaps2;
		DWORD dwTotal;
		DWORD dwFree;
		ZeroMemory(&ddsCaps2, sizeof(ddsCaps2)); 
		ddsCaps2.dwCaps = DDSCAPS_TEXTURE; 
		HRESULT hr = CDirectDraw::GetDD()->GetAvailableVidMem(&ddsCaps2, &dwTotal, &dwFree);

		sprintf(str, "Texture = %ld/%ld", dwFree, dwTotal);
		pSurfaceCast->GDI_Text(500,580, str, 0xFFFFFF);		

	
	#endif
}


//----------------------------------------------------------------------
// Draw Test Help
//----------------------------------------------------------------------
// ClientTest¿ë Help
//----------------------------------------------------------------------
void
MTopView::DrawTestHelp()
{
	#ifdef OUTPUT_DEBUG
		//--------------------------------------------------------------------
		//
		// F1À» ´©¸¥ »óÅÂ --> µµ¿ò¸» Ãâ·Â
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
					//m_pSurface->GammaBox565(&rect, 15);
				}
				else
				{
					//m_pSurface->GammaBox555(&rect, 15);
				}

				m_pSurface->Unlock();
			}

			const int maxHelp = 19;
			const char helpStr[maxHelp][128] = 
			{
				"---------------------------------------------------------------------",
				"         ALPHA-TEST CLIENT µµ¿ò¸»",
				"---------------------------------------------------------------------",
				"  [ESC] Menu",
				"  [F1~F8] º§Æ® ¾ÆÀÌÅÛ »ç¿ë",
				"  [Scroll Lock] È­¸é Ä¸ÃÄ(--> \\ScreenShot)",
				"  [Ctrl + M/T/Z] Áöµµ/°ÔÀÓ½Ã°£/Á¸ ÀÌ¸§",
				"  [Tab] ¾ÆÀÌÅÛÃ¢ on/off",			
				"  [Ctrl + Tab] ÀåÂøÃ¢ on/off",			
				"  [L-Ctrl + ~] º§Æ® on/off",			
				"  [L-Ctrl + S] ±â¼ú ¾ÆÀÌÄÜ ¼±ÅÃ",			
				"  [L-Ctrl + Space] ¾ÆÀÌÅÛÃ¢, ÀåÂøÃ¢ on/off",
				"---------------------------------------------------------------------",
				"  [L-Button] ÀÌµ¿, ÃßÀû, °ø°Ý",
				"  [L-Shift + L-Button] °­Á¦ °ø°Ý",
				"  [L-Ctrl + L-Button] Àû¸¸ ¼±ÅÃ °ø°Ý",
				"  [R-Button] ¸¶¹ý, ±â¼ú",
				"---------------------------------------------------------------------",
				"  [Ã¤ÆÃÇÒ¶§] ! ¸¦ »ç¿ëÇÏ¸é Á¸ Ã¤ÆÃ"
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
// ÀüºÎ ´Ù½Ã ±×¸±°ÇÁö   
// ÀÌÀüÀÇ TileÀ» ÀÌ¿ëÇØ¼­ ÀÏºÎ¸¸ ±×¸±°ÇÁö¸¦ ¼±ÅÃÇÑ´Ù.
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
		// Å¸ÀÏ¿¡ Æ÷Å» Á¤º¸ Ãâ·ÂÇÏ±â
		if (g_pDXInput->KeyDown(DIK_T) && g_pDXInput->KeyDown(DIK_LCONTROL))
		{
			SetFirstDraw();
		}
	#endif	

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD("Draw");
	#endif

	/*
	// Work Thread¸¦ ÀÌ¿ëÇÑ loading test
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
	// ºñ°ø½Ä(-_-;) ¿µ¾î ÆùÆ® »ç¿ë¾ÈÇÔ - -;
	//------------------------------------------------------------
	//gC_font.NotUseNoPublicEng();		

	//#ifdef OUTPUT_DEBUG
		//	DEBUG_ADD("[TempDebug] Before Draw");
	//#endif	

	//------------------------------------------------------------
	// Clip¿µ¿ª ¾È¿¡¼­¸¸ Ãâ·ÂÇÑ´Ù.
	//------------------------------------------------------------
// 	int clipRight = m_pSurface->GetClipRight();
// 	int clipBottom	= m_pSurface->GetClipBottom();
// 	m_pSurface->SetClipRightBottom(g_GameRect.right, g_GameRect.bottom);		

	//------------------------------------------------------------
	// TileÀ» Ã³À½ ±×¸± ¶§, ´Ù~ ±×¸°´Ù.
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
		light.dvTheta =       0.5f; //¿ø»ÔÀÇ Áß¾Ó Å©±â
		light.dvPhi =         0.8f; //¿ø»ÔÀÇ ¿Ü°û Å©±â
		light.dvAttenuation0 = 1.0f;
		light.dvFalloff		= 1.0f;
		
		CDirect3D::GetDevice()->SetLight( 0, &light );	
		*/
		//*/
	}
	//------------------------------------------------------------
	// ÀÌÀü¿¡ ±×¸° TileÀ» ´Ù½Ã ÀÌ¿ëÇÑ´Ù.
	//------------------------------------------------------------
	//else
	{			
		if (CDirect3D::IsHAL())// && m_b3DLight)
		{
			#ifdef OUTPUT_DEBUG_DRAW_PROCESS
				DEBUG_ADD( "IsLost?" );
			#endif

			// µµ¿òÀÌ µÉ±î..
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

			// SDL: Not needed - removed D3D device clear call
			// CDirect3D::GetDevice()->Clear( 0, NULL, D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );
			//*/
			

			//DrawZone(firstPointX, firstPointY);

			//*/
	

			/*
			if (g_pDXInput->KeyDown(DIK_Z))
			{
				D3DLIGHT7 light;
				CDirect3D::GetDevice()->GetLight( 0, &light );

				light.dvTheta -=       0.01f; //¿ø»ÔÀÇ Áß¾Ó Å©±â
				light.dvPhi -=         0.01f; //¿ø»ÔÀÇ ¿Ü°û Å©±â
				
				CDirect3D::GetDevice()->SetLight( 0, &light );
			}

			if (g_pDXInput->KeyDown(DIK_A))
			{
				D3DLIGHT7 light;
				CDirect3D::GetDevice()->GetLight( 0, &light );

				light.dvTheta +=       0.01f; //¿ø»ÔÀÇ Áß¾Ó Å©±â
				light.dvPhi +=         0.01f; //¿ø»ÔÀÇ ¿Ü°û Å©±â
				
				CDirect3D::GetDevice()->SetLight( 0, &light );
			}
			*/
			
			///*
			//test.Rotate( 0.1f, 0.1f, 0 );

			//------------------------------------------------
			// LightBuffer ÃÊ±âÈ­
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


			// mouseÁÂÇ¥ÀÇ ºû			
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
			/* // 2001.7.14 ÁÖ¼®Ã³¸®
			if (g_pUserOption->DrawMinimap)
			{
				DrawMinimap( CLIPSURFACE_WIDTH-m_pMinimapSPR->GetWidth(),
							0,//440-m_pMinimapSPR->GetHeight(),
							1);	// scale
			}
			*/

			//------------------------------------------------
			// test¿ë µµ¿ò¸»
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
			// Á¦°ÅµÈ SurfaceµéÀ» Áö¿öÁà¾ß ÇÑ´Ù.
			// ¹Ýµå½Ã, EndScene()ÀÌ ³¡³ª°í Áö¿ö¾ß ÇÑ´Ù.
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
			// LightBuffer ÃÊ±âÈ­
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
			/* // 2001.7.14 ÁÖ¼®Ã³¸®
			if (g_pUserOption->DrawMinimap)
			{
				DrawMinimap( CLIPSURFACE_WIDTH-m_pMinimapSPR->GetWidth(),
							0, //440-m_pMinimapSPR->GetHeight(),
							1);	// scale
			}
			*/

			//------------------------------------------------
			// test¿ë µµ¿ò¸»
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
	// Á¤º¸ Ãâ·Â
	//------------------------------------------------
	DrawInformation();

	//#ifdef OUTPUT_DEBUG
		//	DEBUG_ADD("[TempDebug] After Draw");
	//#endif	
	

	//char str[80];
	//sprintf(str, "ImageObject=%d°³", m_mapImageObject.size());
	//m_pSurface->GDI_Text(100,580, str, 0xFFFFFF);

	//------------------------------------------------------------
	// Á¤»óÀûÀÎ Ãâ·Â
	//------------------------------------------------------------
	//m_pSurface->SetClipNULL();
#ifdef PLATFORM_WINDOWS
	m_pSurface->SetClipRightBottom(clipRight, clipBottom);
#endif

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD("Draw OK");
	#endif
}

//----------------------------------------------------------------------
// Draw LightBuffer
//----------------------------------------------------------------------
// m_LightBufferFilter¸¦ m_pLightBufferTexture·Î bltÇÏ°í
// m_pLightBufferTexture¸¦ texture·Î ¼³Á¤ÇØ¼­
// È­¸é¿¡ LightBufferFilter¸¦ Ãâ·ÂÇÏ°Ô µÈ´Ù.
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
#ifdef PLATFORM_WINDOWS
		WORD *lpSurface, pitch;
		m_pLightBufferTexture->Lock();
		lpSurface = (WORD*)m_pLightBufferTexture->GetSurfacePointer();
		pitch = m_pLightBufferTexture->GetSurfacePitch();

		m_LightBufferFilter.Blt4444(lpSurface, pitch);
		//m_LightBufferFilter.Blt4444Color(lpSurface, pitch, 0x0200);

		m_pLightBufferTexture->Unlock();
		//*/

		//------------------------------------------------
		// Texture¼³Á¤ÇÏ°í Ãâ·Â
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
#else
		// SDL/macOS: 3D light buffer rendering not implemented
		(void)m_pLightBufferTexture;
		(void)m_DarkBits;
#endif
	}

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "End DrawLightBuffer3D" );
	#endif

	
}

//----------------------------------------------------------------------
// Draw LightBuffer
//----------------------------------------------------------------------
// m_LightBufferFilter¸¦ m_pLightBufferTexture·Î bltÇÏ°í
// m_pLightBufferTexture¸¦ texture·Î ¼³Á¤ÇØ¼­
// È­¸é¿¡ LightBufferFilter¸¦ Ãâ·ÂÇÏ°Ô µÈ´Ù.
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
				// 9ÁÙ¾¿ Ãâ·ÂÇÒ¶§...
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
					// ´ÙÀ½..
					

					for (int x=0; x<m_LightBufferFilter.GetWidth(); x++)
					{				
						int light	= *pFilter;
						int	len		= *pPW;
						
						//if (light!=31)
						//{
							// lenÁ¡ Âï±â
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
						

						// ´ÙÀ½ filter°ª
						pFilter++;
						pPW++;

						// ´ÙÀ½ Ãâ·Â À§Ä¡ --> 8 pixel µÚ
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
				// 10ÁÙ¾¿ Ãâ·Â
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

					// ´ÙÀ½..
					//lpSurface = (WORD*)((BYTE*)lpSurfaceTemp10 + pitch);

					for (int x=0; x<m_LightBufferFilter.GetWidth(); x++)
					{				
						int light	= *pFilter;
						int	len		= *pPW;
						
						//if (light!=31)
						//{
							// lenÁ¡ Âï±â
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
						

						// ´ÙÀ½ filter°ª
						pFilter++;
						pPW++;

						// ´ÙÀ½ Ãâ·Â À§Ä¡ --> 8 pixel µÚ
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
				// 9ÁÙ¾¿ Ãâ·ÂÇÒ¶§...
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
					// ´ÙÀ½..
					

					for (int x=0; x<m_LightBufferFilter.GetWidth(); x++)
					{				
						int light	= *pFilter;
						int	len		= *pPW;
						
						//if (light!=31)
						//{
							// lenÁ¡ Âï±â
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
						

						// ´ÙÀ½ filter°ª
						pFilter++;
						pPW++;

						// ´ÙÀ½ Ãâ·Â À§Ä¡ --> 8 pixel µÚ
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
				// 10ÁÙ¾¿ Ãâ·Â
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
							// ´ÙÀ½..
							lpSurface = (WORD*)((BYTE*)lpSurfaceTemp10 + pitch);
						}
						// end by sonic


					for (int x=0; x<m_LightBufferFilter.GetWidth(); x++)
					{				
						int light	= *pFilter;
						int	len		= *pPW;
						
						//if (light!=31)
						//{
							// lenÁ¡ Âï±â
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
						

						// ´ÙÀ½ filter°ª
						pFilter++;
						pPW++;

						// ´ÙÀ½ Ãâ·Â À§Ä¡ --> 8 pixel µÚ
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
// È­¸é¿¡ ÀÖ´Â ImageObjectµéÀ» ¸ðµÎ °Ë»öÇÑ´Ù.
//----------------------------------------------------------------------
void
MTopView::DetermineImageObject()
{	
	//---------------------------------------------------------------	
	// firstPoint¿¡ Ãâ·ÂµÉ Zone¿¡¼­ÀÇ Ã¹¹øÂ° Sector
	//---------------------------------------------------------------	
	// Player¸¦ Áß½ÉÀ¸·Î ÇØ¼­ Ãâ·ÂÇØ¾ßÇÏ±â ¶§¹®¿¡
	// ¿ÞÂÊ~~~~ À§~ ºÎÅÍ Ãâ·ÂÇÑ´Ù.
	//---------------------------------------------------------------	
	POINT	firstSector;
	firstSector.x = g_pPlayer->GetX() + g_SECTOR_SKIP_PLAYER_LEFT;
	firstSector.y = g_pPlayer->GetY() + g_SECTOR_SKIP_PLAYER_UP;

	//---------------------------------------------------------------	
	// ZoneÀÇ °æ°è¿¡ °¬À» ¶§,
	// °ËÀº ºÎºÐ ¾È º¸ÀÌ°Ô ÇÏ±â..
	//---------------------------------------------------------------	
	// X ÁÂÇ¥ ¸ÂÃß±â..
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
	// Y ÁÂÇ¥ ¸ÂÃß±â
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
	//                         ImageObject °Ë»ö
	//
	//----------------------------------------------------------------------	
	//POINT firstSector = PixelToMap(m_FirstZonePixel.x, m_FirstZonePixel.y);
	int sX1 = firstSector.x + SECTOR_SKIP_LEFT;
	int sY1 = firstSector.y + SECTOR_SKIP_UP;
	int sX2 = firstSector.x + g_SECTOR_WIDTH+1;
	int sY2 = firstSector.y + g_SECTOR_HEIGHT+1;
	
	//------------------------------------------------------
	// ZoneÀÇ ¿µ¿ªÀÌ ¾Æ´Ñ °æ¿ì¿¡ Skip...
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
	// ÀÌÀü¿¡ ÀÖ´ø ImageObject¸¦ ¸ðµÎ Áö¿î´Ù.
	//------------------------------------------------------
	m_mapImageObject.clear();	

	//------------------------------------------------------
	// °¢ SectorÀÇ ImageObject°Ë»ö
	//------------------------------------------------------
	int y,x,i;
	for (y=sY1; y<=sY2; y++)
	{				
		for (x=sX1; x<=sX2; x++)
		{				
			const MSector& sector = m_pZone->GetSector(x,y);

			// ImageObject°¡ ÀÖ´Ù¸é.. ¸ðµÎ~ ±â¾ïÇØµÐ´Ù.
			// ´Ü, Áßº¹À» Çã¿ëÇÏÁö ¾Ê´Â´Ù.
			if (sector.IsExistImageObject())
			{
				OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

				// Sector¿¡ ÀÖ´Â ¸ðµç ImageObjectµéÀ» °Ë»öÇÑ´Ù.
				for (i=0; i<sector.GetImageObjectSize(); i++)
				{
					MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);

					//----------------------------------------
					// Ãâ·Â ½ÃÁ¡À» Ã¼Å©ÇØ¾ß µÇÁö¸¸,
					// ¾î¶² ImageObjectµéÀº
					// ÀÚ½ÅÀÇ ViewSector°¡ ¾Æ´Ñ viewpoint¸¦
					// °®´Â °æ¿ìµµ ÀÖ´Ù. -_-;
					// ±×·¡¼­.. ¹«Á¶°Ç Ãß°¡ÇÑ´Ù.
					//----------------------------------------
					// Ãâ·Â ½ÃÁ¡ÀÌ µÆÀ¸¸é.. Ãß°¡ÇÑ´Ù.
					//if (pImageObject->GetViewpoint()==y				// Á¤ÇØÁø ÁÙ
					//	|| y==sY2-1									// ³¡ÁÙ
					//	|| pImageObject->GetViewpoint() < sY1)		// Ã¹ÁÙ
					{						
						// ÀÌ¹Ì ÀÖ´ÂÁö È®ÀÎÇØº¸°í ¾øÀ¸¸é Ãß°¡.
						QWORD key = GetOutputImageObjectID( pImageObject );

						if (m_mapImageObject.find( key )
							== m_mapImageObject.end())
						{			
							//----------------------------------------
							// Key°ª = (Viewpoint << 32) | ID
							//----------------------------------------
							// Viewpoint¿¡ ÀÇÇØ¼­ sortµÈ´Ù.
							// Viewpoint¿Í ID¿¡ ÀÇÇØ¼­ Á¦°ÅÇÒ ¼ö ÀÖ´Ù.
							//----------------------------------------
							m_mapImageObject.insert(
								IMAGEOBJECT_OUTPUT_MAP::value_type(
									key,	// key°ª
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
// m_FirstPoint´Â ¹Ù·Î ÀüÀÇ~ È­¸é ÁÂÇ¥ÀÌ°í..
// parameterÀÇ firstSector°¡ ÇöÀçÀÇ È­¸é ÁÂÇ¥ÀÌ´Ù.
//
// È­¸é¿¡¼­ ¾ø¾îÁö´Â ImageObject´Â m_mapImageObject¿¡¼­ ¾ø¾Ö°í
// È­¸é¿¡ ³ªÅ¸³ª´Â ImageObject´Â m_mapImageObject¿¡ Ãß°¡ÇÑ´Ù.
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
	//               ImageObject °»½Å
	//
	//---------------------------------------------------------------
	// newFirstSector¿Í m_FirstSector(¹Ù·Î ÀÌÀüÀÇ ÁÂÇ¥)¸¦ ºñ±³ÇØ¼­
	// ´Ù¸¥ °æ¿ì... check~~ÇØ¾ßÇÑ´Ù.
	//---------------------------------------------------------------
	// »óÇÏ·Î ÁÂÇ¥°¡ ´Þ¶óÁø °æ¿ì
	//---------------------------------------------------------------
	if (newFirstSector.y != m_FirstSector.y)
	{		
		int	sX1=newFirstSector.x + SECTOR_SKIP_LEFT, 
				sX2=newFirstSector.x + g_SECTOR_WIDTH+1, 
				sX01=m_FirstSector.x + SECTOR_SKIP_LEFT, 
				sX02=m_FirstSector.x + g_SECTOR_WIDTH+1, 
				eraseY1, eraseY2,		// Áö¿ö¾ßµÉ °ÍµéÀÌ ÀÖ´Â ÁÙ
				lastY,					// È­¸éÀÇ ¸¶Áö¸· ÁÙ(Áö¿ì¸é ¾ÈµÇ´Â °Íµé)
				newY1, newY2;			// »õ·Î ³ªÅ¸³ª´Â °ÍµéÀÌ ÀÖ´Â ÁÙ
		
		// Ã¼Å©ÇÒ ÇÊ¿ä°¡ ÀÖ´Â°¡?
		bool bCheckLast		= true;
		bool bCheckErase	= true;
		bool bCheckNew		= true;

		//-----------------------------------
		// Zone¿µ¿ªÀ» ¹þ¾î³ª´Â °æ¿ì
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
		// À§ÂÊÀ¸·Î ÀÌµ¿ÇÑ °æ¿ì
		//-----------------------------------
		if (newFirstSector.y - m_FirstSector.y < 0)
		{
			// ¾Æ·¡ÂÊ¿¡¼­ »ç¶óÁö´Â °Ç¹°ÀÌ ÀÖ´ÂÁö È®ÀÎÇØºÁ¾ßÇÑ´Ù.
			// --> ÀÌÀü È­¸éÀÇ ¸¶Áö¸· ÁÙ¿¡´Â ÀÖÁö¸¸,
			//     ÇöÀç È­¸éÀÇ ¸¶Áö¸· ÁÙ¿¡´Â ¾ø´Â °ÍµéÀº »ç¶óÁö´Â °ÍÀÌ´Ù.
			
			// ´Ù½Ã »ì¸± °Í
			lastY	= newFirstSector.y + g_SECTOR_HEIGHT+1;	// ÇöÀç È­¸éÀÇ ¸¶Áö¸· ÁÙ

			// Áö¿ö¾ß µÉ °Í : lasyY+1 ~ eraseY
			eraseY1	= lastY+1;							
			eraseY2	= m_FirstSector.y + g_SECTOR_HEIGHT+1;	// ÀÌÀü È­¸éÀÇ ¸¶Áö¸· ÁÙ			

			// »õ·Î ³ªÅ¸³ª´Â °Í
			newY1	= newFirstSector.y + SECTOR_SKIP_UP;
			newY2	= m_FirstSector.y + SECTOR_SKIP_UP - 1;

			//------------------------------------------------------
			// ZoneÀÇ ¿µ¿ªÀÌ ¾Æ´Ñ °æ¿ì¿¡ Skip...
			//------------------------------------------------------
			if (lastY >= m_pZone->GetHeight())
			{
				//lastY = m_pZone->GetHeight()-1;				
				// »ì¸± ÇÊ¿ä ¾ø´Ù.
				bCheckLast = false;
			}

			if (eraseY1 >= m_pZone->GetHeight())
			{
				//eraseY1 = m_pZone->GetHeight()-1;
				// eraseÇÒ ÇÊ¿ä ¾ø´Ù. 
				bCheckErase = false;
			}
			else if (eraseY2 >= m_pZone->GetHeight())
			{
				eraseY2 = m_pZone->GetHeight()-1;								
			}		

			if (newY2 < 0) 
			{				
				//newY2 = 0;
				// newÇÒ ÇÊ¿ä ¾ø´Ù.
				bCheckNew = false;
			}		
			else if (newY1 < 0) 
			{				
				newY1 = 0;	
			}
			
		}
		//-----------------------------------
		// ¾Æ·¡ÂÊÀ¸·Î ÀÌµ¿ÇÑ °æ¿ì
		//-----------------------------------
		else
		{	
			// ´Ù½Ã »ì¸± °Í
			lastY	= newFirstSector.y + SECTOR_SKIP_UP;	// ÇöÀç È­¸éÀÇ ¸¶Áö¸· ÁÙ

			// Áö¿ö¾ßµÉ °Í : eraseY ~ lastY-1
			eraseY1	= m_FirstSector.y + SECTOR_SKIP_UP;	// ÀÌÀü È­¸éÀÇ ¸¶Áö¸· ÁÙ
			eraseY2	= lastY-1;		

			// »õ·Î ³ªÅ¸³ª´Â °Í
			newY1	= m_FirstSector.y + g_SECTOR_HEIGHT+1 + 1;
			newY2	= newFirstSector.y + g_SECTOR_HEIGHT+1;

			//------------------------------------------------------
			// ZoneÀÇ ¿µ¿ªÀÌ ¾Æ´Ñ °æ¿ì¿¡ Skip...
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
		// erase1 ~ erase2±îÁö´Â Áö¿ï¸¸ÇÑ(?) °Íµé.
		//----------------------------------------------
		// ÀÏ´Ü m_mapImageObject¿¡¼­ Áö¿î´Ù.
		//----------------------------------------------
		if (bCheckErase)
		{
			for (y=eraseY1; y<=eraseY2; y++)
			{
				for (x=sX01; x<=sX02; x++)
				{
					const MSector& sector = m_pZone->GetSector(x,y);

					// ImageObject°¡ ÀÖ´Ù¸é.. ¸ðµÎ~ ±â¾ïÇØµÐ´Ù.
					// ´Ü, Áßº¹À» Çã¿ëÇÏÁö ¾Ê´Â´Ù.
					if (sector.IsExistImageObject())
					{
						OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

						// Sector¿¡ ÀÖ´Â ¸ðµç ImageObjectµéÀ» °Ë»öÇÑ´Ù.
						for (int i=0; i<sector.GetImageObjectSize(); i++)
						{
							MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);
							
							//----------------------------------------
							// Key°ª = (Viewpoint << 32) | ID
							//----------------------------------------
							// ÀÌ¹Ì ÀÖ´ÂÁö È®ÀÎÇØº¸°í ÀÖÀ¸¸é Á¦°Å.
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
		// lastY´Â ´Ù½Ã Ãß°¡ÇÑ´Ù.
		//----------------------------------------------
		if (bCheckLast)
		{
			for (x=sX1; x<=sX2; x++)
			{
				const MSector& sector = m_pZone->GetSector(x, lastY);

				// ImageObject°¡ ÀÖÀ¸¸é..
				if (sector.IsExistImageObject())
				{
					OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

					// Sector¿¡ ÀÖ´Â ¸ðµç ImageObjectµéÀ» °Ë»öÇÑ´Ù.
					for (i=0; i<sector.GetImageObjectSize(); i++)
					{
						MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);

						// Ãâ·Â ½ÃÁ¡ÀÌ µÆÀ¸¸é.. Ãß°¡ÇÑ´Ù.
						//if (pImageObject->GetViewpoint()==y				// Á¤ÇØÁø ÁÙ
						//	|| y==sY2-1									// ³¡ÁÙ
						//	|| pImageObject->GetViewpoint() < sY1)		// Ã¹ÁÙ
						{		
							QWORD key = GetOutputImageObjectID(pImageObject);
							// ÀÌ¹Ì ÀÖ´ÂÁö È®ÀÎÇØº¸°í ¾øÀ¸¸é Ãß°¡.
							if (m_mapImageObject.find( key )
								== m_mapImageObject.end())
							{			
								//----------------------------------------
								// Key°ª = (Viewpoint << 32) | ID
								//----------------------------------------
								// Viewpoint¿¡ ÀÇÇØ¼­ sortµÈ´Ù.
								// Viewpoint¿Í ID¿¡ ÀÇÇØ¼­ Á¦°ÅÇÒ ¼ö ÀÖ´Ù.
								//----------------------------------------
								m_mapImageObject.insert(
									IMAGEOBJECT_OUTPUT_MAP::value_type(
										key,	// key°ª
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
		// »õ·Î ³ªÅ¸´Â °Íµé : newY1 ~ newY2
		//----------------------------------------------
		if (bCheckNew)
		{
			for (y=newY1; y<=newY2; y++)
			{
				for (x=sX1; x<=sX2; x++)
				{
					const MSector& sector = m_pZone->GetSector(x, y);

					// ImageObject°¡ ÀÖÀ¸¸é..
					if (sector.IsExistImageObject())
					{
						OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

						// Sector¿¡ ÀÖ´Â ¸ðµç ImageObjectµéÀ» °Ë»öÇÑ´Ù.
						for (i=0; i<sector.GetImageObjectSize(); i++)
						{
							MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);
							
							// Ãâ·Â ½ÃÁ¡ÀÌ µÆÀ¸¸é.. Ãß°¡ÇÑ´Ù.						
							//if (viewPoint==y				// Á¤ÇØÁø ÁÙ
							//	|| viewPoint > newY2									// ³¡ÁÙ
							//	|| viewPoint < newY1)		// Ã¹ÁÙ
							{						
								// ÀÌ¹Ì ÀÖ´ÂÁö È®ÀÎÇØº¸°í ¾øÀ¸¸é Ãß°¡.
								QWORD key = GetOutputImageObjectID( pImageObject );

								if (m_mapImageObject.find( key ) == m_mapImageObject.end())
								{			
									//----------------------------------------
									// Key°ª = (Viewpoint << 32) | ID
									//----------------------------------------
									// Viewpoint¿¡ ÀÇÇØ¼­ sortµÈ´Ù.
									// Viewpoint¿Í ID¿¡ ÀÇÇØ¼­ Á¦°ÅÇÒ ¼ö ÀÖ´Ù.
									//----------------------------------------
									m_mapImageObject.insert(
										IMAGEOBJECT_OUTPUT_MAP::value_type(
											key,	// key°ª
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
	// ÁÂ¿ì·Î ÁÂÇ¥°¡ ´Þ¶óÁø °æ¿ì
	//---------------------------------------------------------------
	if (newFirstSector.x != m_FirstSector.x)
	{
		int	sY1=newFirstSector.y + SECTOR_SKIP_UP, 
				sY2=newFirstSector.y + g_SECTOR_HEIGHT+1, 
				sY01=m_FirstSector.y + SECTOR_SKIP_UP, 
				sY02=m_FirstSector.y + g_SECTOR_HEIGHT+1, 
				eraseX1, eraseX2,		// Áö¿ö¾ßµÉ °ÍµéÀÌ ÀÖ´Â ÁÙ
				lastX,					// È­¸éÀÇ ¸¶Áö¸· ÁÙ(Áö¿ì¸é ¾ÈµÇ´Â °Íµé)
				newX1, newX2;			// »õ·Î ³ªÅ¸³ª´Â °ÍµéÀÌ ÀÖ´Â ÁÙ

		// Ã¼Å©ÇÒ ÇÊ¿ä°¡ ÀÖ´Â°¡?
		bool bCheckLast		= true;
		bool bCheckErase	= true;
		bool bCheckNew		= true;

		//-----------------------------------
		// Zone¿µ¿ªÀ» ¹þ¾î³ª´Â °æ¿ì
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
		// ¿ÞÂÊÀ¸·Î ÀÌµ¿ÇÑ °æ¿ì
		//-----------------------------------
		if (newFirstSector.x - m_FirstSector.x < 0)
		{
			// ¿À¸¥ÂÊ¿¡¼­ »ç¶óÁö´Â °Ç¹°ÀÌ ÀÖ´ÂÁö È®ÀÎÇØºÁ¾ßÇÑ´Ù.
			// --> ÀÌÀü È­¸éÀÇ ¸¶Áö¸· ÁÙ¿¡´Â ÀÖÁö¸¸,
			//     ÇöÀç È­¸éÀÇ ¸¶Áö¸· ÁÙ¿¡´Â ¾ø´Â °ÍµéÀº »ç¶óÁö´Â °ÍÀÌ´Ù.
			
			// ´Ù½Ã »ì¸± °Í
			lastX	= newFirstSector.x + g_SECTOR_WIDTH+1;	// ÇöÀç È­¸éÀÇ ¸¶Áö¸· ÁÙ

			// Áö¿ö¾ß µÉ °Í : lasyX+1 ~ eraseX
			eraseX1	= lastX+1;							
			eraseX2	= m_FirstSector.x + g_SECTOR_WIDTH+1;	// ÀÌÀü È­¸éÀÇ ¸¶Áö¸· ÁÙ			

			// »õ·Î ³ªÅ¸³ª´Â °Í
			newX1	= newFirstSector.x + SECTOR_SKIP_LEFT;
			newX2	= m_FirstSector.x + SECTOR_SKIP_LEFT - 1;

			//------------------------------------------------------
			// ZoneÀÇ ¿µ¿ªÀÌ ¾Æ´Ñ °æ¿ì¿¡ Skip...
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
		// ¿À¸¥ÂÊÀ¸·Î ÀÌµ¿ÇÑ °æ¿ì
		//-----------------------------------
		else
		{	
			// Á¦°Å             Ãß°¡      Ãß°¡
			// eraseX1~eraseX2~lastX ~~~~ newX1~newX2
			// ´Ù½Ã »ì¸± °Í
			lastX	= newFirstSector.x + SECTOR_SKIP_LEFT;	// ÇöÀç È­¸éÀÇ ¸¶Áö¸· ÁÙ

			// Áö¿ö¾ßµÉ °Í : eraseX ~ lastX-1
			eraseX1	= m_FirstSector.x + SECTOR_SKIP_LEFT;	// ÀÌÀü È­¸éÀÇ ¸¶Áö¸· ÁÙ
			eraseX2	= lastX-1;		

			// »õ·Î ³ªÅ¸³ª´Â °Í
			newX1	= m_FirstSector.x + g_SECTOR_WIDTH+1 + 1;
			newX2	= newFirstSector.x + g_SECTOR_WIDTH+1;

			//------------------------------------------------------
			// ZoneÀÇ ¿µ¿ªÀÌ ¾Æ´Ñ °æ¿ì¿¡ Skip...
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
		// erase1 ~ erase2±îÁö´Â Áö¿ï¸¸ÇÑ(?) °Íµé.
		//----------------------------------------------
		// ÀÏ´Ü m_mapImageObject¿¡¼­ Áö¿î´Ù.
		//----------------------------------------------
		if (bCheckErase)
		{
			for (x=eraseX1; x<=eraseX2; x++)
			{
				for (y=sY01; y<=sY02; y++)
				{
					const MSector& sector = m_pZone->GetSector(x,y);

					// ImageObject°¡ ÀÖ´Ù¸é.. ¸ðµÎ~ ±â¾ïÇØµÐ´Ù.
					// ´Ü, Áßº¹À» Çã¿ëÇÏÁö ¾Ê´Â´Ù.
					if (sector.IsExistImageObject())
					{
						OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

						// Sector¿¡ ÀÖ´Â ¸ðµç ImageObjectµéÀ» °Ë»öÇÑ´Ù.
						for (i=0; i<sector.GetImageObjectSize(); i++)
						{
							MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);
							
							//----------------------------------------
							// Key°ª = (Viewpoint << 32) | ID
							//----------------------------------------
							// ÀÌ¹Ì ÀÖ´ÂÁö È®ÀÎÇØº¸°í ÀÖÀ¸¸é Á¦°Å.
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
		// lastX´Â ´Ù½Ã Ãß°¡ÇÑ´Ù.
		//----------------------------------------------
		if (bCheckLast)
		{
			for (y=sY1; y<=sY2; y++)
			{
				const MSector& sector = m_pZone->GetSector(lastX, y);

				// ImageObject°¡ ÀÖÀ¸¸é..
				if (sector.IsExistImageObject())
				{
					OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

					// Sector¿¡ ÀÖ´Â ¸ðµç ImageObjectµéÀ» °Ë»öÇÑ´Ù.
					for (i=0; i<sector.GetImageObjectSize(); i++)
					{
						MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);
						
						// Ãâ·Â ½ÃÁ¡ÀÌ µÆÀ¸¸é.. Ãß°¡ÇÑ´Ù.
						//if (pImageObject->GetViewpoint()==y				// Á¤ÇØÁø ÁÙ
							//|| y==sY2-1									// ³¡ÁÙ
							//|| pImageObject->GetViewpoint() < sY1)		// Ã¹ÁÙ
						{						
							// ÀÌ¹Ì ÀÖ´ÂÁö È®ÀÎÇØº¸°í ¾øÀ¸¸é Ãß°¡.
							QWORD key = GetOutputImageObjectID( pImageObject );

							if (m_mapImageObject.find( key )
								== m_mapImageObject.end())
							{			
								//----------------------------------------
								// Key°ª = (Viewpoint << 32) | ID
								//----------------------------------------
								// Viewpoint¿¡ ÀÇÇØ¼­ sortµÈ´Ù.
								// Viewpoint¿Í ID¿¡ ÀÇÇØ¼­ Á¦°ÅÇÒ ¼ö ÀÖ´Ù.
								//----------------------------------------
								m_mapImageObject.insert(
									IMAGEOBJECT_OUTPUT_MAP::value_type(
										key,	// key°ª
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
		// »õ·Î ³ªÅ¸´Â °Íµé : newX1 ~ newX2
		//----------------------------------------------
		if (bCheckNew)
		{
			for (x=newX1; x<=newX2; x++)
			{
				for (y=sY1; y<=sY2; y++)
				{
					const MSector& sector = m_pZone->GetSector(x, y);

					// ImageObject°¡ ÀÖÀ¸¸é..
					if (sector.IsExistImageObject())
					{
						OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

						// Sector¿¡ ÀÖ´Â ¸ðµç ImageObjectµéÀ» °Ë»öÇÑ´Ù.
						for (i=0; i<sector.GetImageObjectSize(); i++)
						{
							MImageObject* const pImageObject = (MImageObject* const)((*iImageObject).second);
						
							// Ãâ·Â ½ÃÁ¡ÀÌ µÆÀ¸¸é.. Ãß°¡ÇÑ´Ù.
							//if (viewPoint<=y			// À§¿¡ ²¨
							//	|| viewPoint > sY2)		// ³¡ÁÙ
								//|| viewPoint < newX1)		// Ã¹ÁÙ
							{								
								// ÀÌ¹Ì ÀÖ´ÂÁö È®ÀÎÇØº¸°í ¾øÀ¸¸é Ãß°¡.
								QWORD key = GetOutputImageObjectID( pImageObject );

								if (m_mapImageObject.find( key )
									== m_mapImageObject.end())
								{			
									//----------------------------------------
									// Key°ª = (Viewpoint << 32) | ID
									//----------------------------------------
									// Viewpoint¿¡ ÀÇÇØ¼­ sortµÈ´Ù.
									// Viewpoint¿Í ID¿¡ ÀÇÇØ¼­ Á¦°ÅÇÒ ¼ö ÀÖ´Ù.
									//----------------------------------------
									m_mapImageObject.insert(
										IMAGEOBJECT_OUTPUT_MAP::value_type(
											key,	// key°ª
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
// Pixel ÁÂÇ¥ÀÎ (Xp, Yp)°¡ 
//       È­¸éÀÇ (0, 0)¿¡ ¿Àµµ·Ï ÇÏ¿© ³ª¸ÓÁö ºÎºÐÀ» Ãâ·ÂÇÑ´Ù.
//
// pSurfaceÀÇ (X,Y)¿¡ ZoneÀÇ (Xp,Yp)ºÎºÐºÎÅÍ Ãâ·ÂÇÑ´Ù.
//----------------------------------------------------------------------
//
// ÀÌÀü¿¡ Ãâ·ÂÇÑ TileÀ» ÀÌ¿ëÇØ¼­ ÇöÀç TileÀ» ±¸¼ºÇØÁØ µÚ,
// ´Ù¸¥ Object¸¦ Ãâ·ÂÇÑ´Ù.
//
//----------------------------------------------------------------------
void	
MTopView::DrawZone(int firstPointX,int firstPointY)
{
	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "Start DrawZone" );
	#endif

	//-------------------------------------------------
	// ItemName list¸¦ Áö¿öÁØ´Ù.
	//-------------------------------------------------
	ClearItemNameList();	


	// mouse·Î ¼±ÅÃµÈ Ä³¸¯ÅÍ..
	m_pSelectedCreature = NULL;


	//-------------------------------------------------
	//
	// Object¸¦ Ãâ·ÂÇÏ±â À§ÇÑ Á¤º¸
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
	// Player¸¦ °¡¸®´Â ImageObjectµé¿¡ ´ëÇÑ Á¤º¸
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
	// Ã¹ Sector°¡ Ãâ·ÂµÉ ÁÂÇ¥ º¸Á¤(smooth scrollÀ» À§ÇØ¼­)
	// PlayerÀÇ ÁÂÇ¥¸¦ Áß½ÉÀ¸·Î ÇØ¼­ Á¤ÇÑ´Ù.
	//-------------------------------------------------
	m_PlusPoint.x = g_pPlayer->GetSX() - firstPointX;
	m_PlusPoint.y = g_pPlayer->GetSY() - firstPointY;
	firstPointX -= g_pPlayer->GetSX();
	firstPointY -= g_pPlayer->GetSY();


	//---------------------------------------------------------------	
	// firstPoint¿¡ Ãâ·ÂµÉ Zone¿¡¼­ÀÇ Ã¹¹øÂ° Sector
	//---------------------------------------------------------------	
	// Player¸¦ Áß½ÉÀ¸·Î ÇØ¼­ Ãâ·ÂÇØ¾ßÇÏ±â ¶§¹®¿¡
	// ¿ÞÂÊ~~~~ À§~ ºÎÅÍ Ãâ·ÂÇÑ´Ù.
	//---------------------------------------------------------------	
	POINT	firstSector;
	firstSector.x = g_pPlayer->GetX() + g_SECTOR_SKIP_PLAYER_LEFT;
	firstSector.y = g_pPlayer->GetY() + g_SECTOR_SKIP_PLAYER_UP;

	//---------------------------------------------------------------	
	// ZoneÀÇ °æ°è¿¡ °¬À» ¶§,
	// °ËÀº ºÎºÐ ¾È º¸ÀÌ°Ô ÇÏ±â..
	//---------------------------------------------------------------	
	// X ÁÂÇ¥ ¸ÂÃß±â..
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
	// Y ÁÂÇ¥ ¸ÂÃß±â
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
	// »õ·Î ³ªÅ¸³ª°Å³ª È­¸é¿¡¼­ ¾ø¾îÁö´Â ImageObject°Ë»ö
	//---------------------------------------------------------------
	// 2001.8.22

	__BEGIN_PROFILE("UpdateImageObject")

	UpdateImageObject(firstSector);	

	__END_PROFILE("UpdateImageObject")

	
	//---------------------------------------------------------------
	// ¿ÜºÎ¿¡¼­ ÇÊ¿äÇÑ Á¤º¸¸¦ »ý¼º..
	// È­¸é »óÀÇ Ã¹ À§Ä¡¿¡ Ãâ·ÂµÇ´Â SectorÀÇ ÁÂÇ¥
	//---------------------------------------------------------------
	m_FirstSector	= firstSector;	

	//---------------------------------------------------------------
	// È­¸éÀÇ (0,0)ÀÌ ³ªÅ¸³»´Â ZoneÀÇ PixelÁÂÇ¥
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
	// Player¸¦ °¡¸®´Â ImageObjectµéÀ» Ã³¸®ÇÒ FilterÀÇ Ãâ·Â ÁÂÇ¥
	//---------------------------------------------------------------
	m_FilterPosition.x = g_pPlayer->GetPixelX() - m_FirstZonePixel.x - (m_ImageObjectFilter.GetWidth()>>1) + 24;
	m_FilterPosition.y = g_pPlayer->GetPixelY() - m_FirstZonePixel.y - (m_ImageObjectFilter.GetHeight()>>1) 
						- (g_pPlayer->IsFlyingCreature()? 3:1 )*TILE_Y;
	//filterPoint.x = 310;	// 410 - 100;
	//filterPoint.y = 116;	// 204 - 100;


	//short	sX,
	//		sY = (short)firstSector.y;

	// Ãâ·ÂÇÒ Surface»óÀÇ À§Ä¡	
	tilePoint.y = firstPointY;


	//--------------------------------------------------
	// TileSurfaceÀÇ ¿µ¿ª : m_TileSurfaceFirstPixelXY 
	//                      + (TILESURFACE_WIDTH+ TILESURFACE_HEIGHT)
	//
	// ÇöÀç È­¸éÀÇ ¿µ¿ª   : m_FirstZonePixel + (800,600)
	//--------------------------------------------------
	//
	// ÇöÀç È­¸é ¿µ¿ªÀÌ TileSurface¿¡ ¼ÓÇÏ¸é 
	// TileSurfaceÀÇ ÀÏºÎ¸¦ ±×´ë·Î ÀÌ¿ëÇÏ¸é µÈ´Ù.
	//
	//
	//--------------------------------------------------
	__BEGIN_PROFILE("ReuseTileSurface")

	RECT rectTileSurface;
	RECT rectScreen;
	RECT	rectReuse;

	// TileSurfaceÀÇ Zone¿¡¼­ÀÇ ¿µ¿ª
	rectTileSurface.left	= m_TileSurfaceFirstZonePixelX;
	rectTileSurface.top		= m_TileSurfaceFirstZonePixelY;
	rectTileSurface.right	= m_TileSurfaceFirstZonePixelX + g_TILESURFACE_WIDTH;
	rectTileSurface.bottom	= m_TileSurfaceFirstZonePixelY + g_TILESURFACE_HEIGHT;

	// ÇöÀç È­¸éÀÇ ¿µ¿ª
	rectScreen.left		= m_FirstZonePixel.x;
	rectScreen.top		= m_FirstZonePixel.y;
	rectScreen.right	= m_FirstZonePixel.x + g_GameRect.right;
	rectScreen.bottom	= m_FirstZonePixel.y + g_GameRect.bottom;

	// ÇöÀç È­¸é Ãâ·Â¿¡ ÀÌ¿ëÇÒ TileSurfaceÀÇ ¿µ¿ªÀ» Á¤ÇÑ´Ù.
	// È­¸éÀÇ (0,0)ÀÌ µÉ TileSurface¿¡¼­ÀÇ Ã¹ Á¡
	rectReuse.left		= rectScreen.left - rectTileSurface.left;
	rectReuse.top		= rectScreen.top - rectTileSurface.top;
	rectReuse.right		= rectReuse.left + g_GameRect.right;
	rectReuse.bottom	= rectReuse.top + g_GameRect.bottom;

	//----------------------------------------------------------------	
	// rectReuse¸¦ Á¦¿ÜÇÑ ºÎºÐÀº °ËÀº»öÀ¸·Î Ä¥ÇØÁà¾ß ÇÑ´Ù.
	//----------------------------------------------------------------
	/*
	// À§ÂÊ
	if (rectReuse.top != 0)
	{
		rect.left = 0;
		rect.right = SURFACE_WIDTH;
		rect.top = 0;
		rect.bottom = rectReuse.top;

		m_pSurface->FillRect(&rect, 0);
	}

	// ¾Æ·¡ÂÊ
	if (rectReuse.bottom != SURFACE_HEIGHT)
	{
		rect.left = 0;
		rect.right = SURFACE_WIDTH;
		rect.top = rectReuse.bottom;
		rect.bottom = SURFACE_HEIGHT;

		m_pSurface->FillRect(&rect, 0);
	}

	// ¿ÞÂÊ
	if (rectReuse.left != 0)
	{
		rect.left = 0;
		rect.right = rectReuse.left;
		rect.top = rectReuse.top;
		rect.bottom = rectReuse.bottom;	

		m_pSurface->FillRect(&rect, 0);
	}

	// ¿À¸¥ÂÊ
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
	// TileSurface¸¦ ±×´ë·Î ¾µ ¼ö ÀÖ´ÂÁö Ã¼Å©ÇÑ´Ù.
	//--------------------------------------------------
	// ºÎÁ·ÇÑ ºÎºÐ Ç¥½Ã	
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
	// ÇöÀç È­¸éÀÌ TileSurface¿¡ ¼ÓÇÏ´Â °æ¿ì
	//
	//---------------------------------------------------------------------
	if (bLack==0)
	{
		//....
		//---------------------------------------------------------------
		// »õ·Î ³ªÅ¸³ª°Å³ª È­¸é¿¡¼­ ¾ø¾îÁö´Â ImageObject°Ë»ö
		//---------------------------------------------------------------
		// 2001.8.22
		//UpdateImageObject(firstSector);	

	}
	/*
	//---------------------------------------------------------------------
	//
	// ³Ê¹« ¸¹ÀÌ °»½ÅÇØ¾ßÇÏ´Â °æ¿ì
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
		
		// TileÀüÃ¼¸¦ ´Ù½Ã ±×·ÁÁØ´Ù.
		DrawTileSurface();

		DetermineImageObject();		

		m_bFirstTileDraw = false;

		DEBUG_ADD("[DrawZone] Too Far Move - DrawTileSurface OK");		
	}
	*/
	//---------------------------------------------------------------------
	//
	// ÇöÀç È­¸éÀÌ TileSurface¿¡ ¼ÓÇÏÁö ¾Ê´Â °æ¿ì
	//
	//---------------------------------------------------------------------
	else
	{
		//---------------------------------------------------------------
		// »õ·Î ³ªÅ¸³ª°Å³ª È­¸é¿¡¼­ ¾ø¾îÁö´Â ImageObject°Ë»ö
		//---------------------------------------------------------------
		// 2001.8.22
		//UpdateImageObject(firstSector);	

		//----------------------------------------
		// ºÎºÐÀûÀ¸·Î¸¸ °»½ÅÇÏ´Âµ¥ ÇÊ¿äÇÑ Á¤º¸
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
			// LEFT + UP ºÎÁ·
			//--------------------------
			if (bLack & FLAG_TILESURFACE_LACK_UP)
			{
				//--------------------------------------------------
				// ÀÌÀü Tile Surface¿¡¼­
				// ´Ù½Ã ±¸¼ºÇÒ·Á´Â Tile Surface¿¡¼­ »ç¿ëÇÒ ¼ö 
				// ÀÖ´Â ºÎºÐÀ» ³²°ÜµÐ´Ù.			
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
				// ºÎÁ·ÇÑ ºÎºÐÀ» ±×·ÁÁØ´Ù.
				//
				//--------------------------------------------------				

				changeCount = 2;

				//--------------------------------------------------
				// LeftºÎºÐ
				//--------------------------------------------------
				// Ãâ·ÂÇÒ sector (sX1,sY) ~ (sX2, sY2)±îÁö Ãâ·Â
				//--------------------------------------------------			
				sX1[0] = m_TileSurfaceFirstSectorX - TILESURFACE_SECTOR_EDGE;
				sY1[0] = m_TileSurfaceFirstSectorY - TILESURFACE_SECTOR_EDGE;
				sX2[0] = sX1[0]	+ TILESURFACE_SECTOR_EDGE;
				sY2[0] = sY1[0] + g_TILESURFACE_SECTOR_HEIGHT;
					
				//--------------------------------------------------
				// Ã¹¹øÂ° Ãâ·Â ÁÂÇ¥
				//--------------------------------------------------
				firstTilePoint[0].x = 0;
				firstTilePoint[0].y = 0;

				//--------------------------------------------------
				// UpºÎºÐ
				//--------------------------------------------------
				// Ãâ·ÂÇÒ sector (sX1,sY) ~ (sX2, sY2)±îÁö Ãâ·Â
				//--------------------------------------------------			
				sX1[1] = m_TileSurfaceFirstSectorX - TILESURFACE_SECTOR_EDGE;
				sY1[1] = m_TileSurfaceFirstSectorY - TILESURFACE_SECTOR_EDGE;
				sX2[1] = sX1[1]	+ g_TILESURFACE_SECTOR_WIDTH;
				sY2[1] = sY1[1] + TILESURFACE_SECTOR_EDGE;
					
				//--------------------------------------------------
				// Ã¹¹øÂ° Ãâ·Â ÁÂÇ¥
				//--------------------------------------------------
				firstTilePoint[1].x = 0;
				firstTilePoint[1].y = 0;


				//--------------------------------------------------
				// Ãâ·Â¿¡ »ç¿ëÇÒ TileSurface ¿µ¿ª ´Ù½Ã ¼³Á¤
				//--------------------------------------------------
				rectReuse.left += TILESURFACE_OUTLINE_LEFT;
				rectReuse.right += TILESURFACE_OUTLINE_LEFT;
				rectReuse.top += TILESURFACE_OUTLINE_UP;
				rectReuse.bottom += TILESURFACE_OUTLINE_UP;

				//--------------------------------------------------
				// °»½ÅµÈ TileSurface¿¡ ´ëÇÑ ÁÂÇ¥ ¼³Á¤
				//--------------------------------------------------
				m_TileSurfaceFirstSectorX -= TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelX -= TILESURFACE_OUTLINE_LEFT;
				m_TileSurfaceFirstSectorY -= TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelY -= TILESURFACE_OUTLINE_UP;
			}			
			//--------------------------
			// LEFT + DOWN ºÎÁ·
			//--------------------------
			else if (bLack & FLAG_TILESURFACE_LACK_DOWN)
			{
				//--------------------------------------------------
				// ÀÌÀü Tile Surface¿¡¼­
				// ´Ù½Ã ±¸¼ºÇÒ·Á´Â Tile Surface¿¡¼­ »ç¿ëÇÒ ¼ö 
				// ÀÖ´Â ºÎºÐÀ» ³²°ÜµÐ´Ù.			
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
				// ºÎÁ·ÇÑ ºÎºÐÀ» ±×·ÁÁØ´Ù.
				//
				//--------------------------------------------------

				changeCount = 2;

				//--------------------------------------------------
				// LeftºÎºÐ
				//--------------------------------------------------
				// Ãâ·ÂÇÒ sector (sX1,sY) ~ (sX2, sY2)±îÁö Ãâ·Â
				//--------------------------------------------------			
				sX1[0] = m_TileSurfaceFirstSectorX - TILESURFACE_SECTOR_EDGE;
				sY1[0] = m_TileSurfaceFirstSectorY + TILESURFACE_SECTOR_EDGE;
				sX2[0] = sX1[0]	+ TILESURFACE_SECTOR_EDGE;
				sY2[0] = sY1[0] + g_TILESURFACE_SECTOR_HEIGHT;
					
				//--------------------------------------------------
				// Ã¹¹øÂ° Ãâ·Â ÁÂÇ¥
				//--------------------------------------------------
				firstTilePoint[0].x = 0;
				firstTilePoint[0].y = 0;

				//--------------------------------------------------
				// DownºÎºÐ
				//--------------------------------------------------
				// Ãâ·ÂÇÒ sector (sX1,sY) ~ (sX2, sY2)±îÁö Ãâ·Â
				//--------------------------------------------------			
				sX1[1] = m_TileSurfaceFirstSectorX - TILESURFACE_SECTOR_EDGE;
				sY1[1] = m_TileSurfaceFirstSectorY + g_TILESURFACE_SECTOR_HEIGHT;
				sX2[1] = sX1[1]	+ g_TILESURFACE_SECTOR_WIDTH;
				sY2[1] = sY1[1] + TILESURFACE_SECTOR_EDGE;
					
				//--------------------------------------------------
				// Ã¹¹øÂ° Ãâ·Â ÁÂÇ¥
				//--------------------------------------------------
				firstTilePoint[1].x = 0;
				firstTilePoint[1].y = g_TILESURFACE_OUTLINE_DOWN;

				//--------------------------------------------------
				// Ãâ·Â¿¡ »ç¿ëÇÒ TileSurface ¿µ¿ª ´Ù½Ã ¼³Á¤
				//--------------------------------------------------
				rectReuse.left += TILESURFACE_OUTLINE_LEFT;
				rectReuse.right += TILESURFACE_OUTLINE_LEFT;
				rectReuse.top -= TILESURFACE_OUTLINE_UP;
				rectReuse.bottom -= TILESURFACE_OUTLINE_UP;

				//--------------------------------------------------
				// °»½ÅµÈ TileSurface¿¡ ´ëÇÑ ÁÂÇ¥ ¼³Á¤
				//--------------------------------------------------
				m_TileSurfaceFirstSectorX -= TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelX -= TILESURFACE_OUTLINE_LEFT;
				m_TileSurfaceFirstSectorY += TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelY += TILESURFACE_OUTLINE_UP;
			}
			//--------------------------
			// LEFT ºÎÁ·
			//--------------------------
			else
			{
				//--------------------------------------------------
				// ÀÌÀü Tile Surface¿¡¼­
				// ´Ù½Ã ±¸¼ºÇÒ·Á´Â Tile Surface¿¡¼­ »ç¿ëÇÒ ¼ö 
				// ÀÖ´Â ºÎºÐÀ» ³²°ÜµÐ´Ù.			
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
				// ºÎÁ·ÇÑ ºÎºÐÀ» ±×·ÁÁØ´Ù.
				//
				//--------------------------------------------------

				changeCount = 1;

				//--------------------------------------------------
				// Ãâ·ÂÇÒ sector (sX1,sY) ~ (sX2, sY2)±îÁö Ãâ·Â
				//--------------------------------------------------			
				sX1[0] = m_TileSurfaceFirstSectorX - TILESURFACE_SECTOR_EDGE;
				sY1[0] = m_TileSurfaceFirstSectorY;
				sX2[0] = sX1[0]	+ TILESURFACE_SECTOR_EDGE;
				sY2[0] = sY1[0] + g_TILESURFACE_SECTOR_HEIGHT;
					
				//--------------------------------------------------
				// Ã¹¹øÂ° Ãâ·Â ÁÂÇ¥
				//--------------------------------------------------
				firstTilePoint[0].x = 0;
				firstTilePoint[0].y = 0;

				//--------------------------------------------------
				// Ãâ·Â¿¡ »ç¿ëÇÒ TileSurface ¿µ¿ª ´Ù½Ã ¼³Á¤
				//--------------------------------------------------
				rectReuse.left += TILESURFACE_OUTLINE_LEFT;
				rectReuse.right += TILESURFACE_OUTLINE_LEFT;

				//--------------------------------------------------
				// °»½ÅµÈ TileSurface¿¡ ´ëÇÑ ÁÂÇ¥ ¼³Á¤
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
			// RIGHT + UP ºÎÁ·
			//--------------------------
			if (bLack & FLAG_TILESURFACE_LACK_UP)
			{
				//--------------------------------------------------
				// ÀÌÀü Tile Surface¿¡¼­
				// ´Ù½Ã ±¸¼ºÇÒ·Á´Â Tile Surface¿¡¼­ »ç¿ëÇÒ ¼ö 
				// ÀÖ´Â ºÎºÐÀ» ³²°ÜµÐ´Ù.			
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
				// ºÎÁ·ÇÑ ºÎºÐÀ» ±×·ÁÁØ´Ù.
				//
				//--------------------------------------------------

				changeCount = 2;

				//--------------------------------------------------
				// RightºÎºÐ
				//--------------------------------------------------
				// Ãâ·ÂÇÒ sector (sX1,sY) ~ (sX2, sY2)±îÁö Ãâ·Â
				//--------------------------------------------------			
				sX1[0] = m_TileSurfaceFirstSectorX + g_TILESURFACE_SECTOR_WIDTH;
				sY1[0] = m_TileSurfaceFirstSectorY - TILESURFACE_SECTOR_OUTLINE_UP;
				sX2[0] = sX1[0]	+ TILESURFACE_SECTOR_EDGE;
				sY2[0] = sY1[0] + g_TILESURFACE_SECTOR_HEIGHT;
					
				//--------------------------------------------------
				// Ã¹¹øÂ° Ãâ·Â ÁÂÇ¥
				//--------------------------------------------------
				firstTilePoint[0].x = g_TILESURFACE_OUTLINE_RIGHT;
				firstTilePoint[0].y = 0;

				//--------------------------------------------------
				// UPºÎºÐ
				//--------------------------------------------------
				// Ãâ·ÂÇÒ sector (sX1,sY) ~ (sX2, sY2)±îÁö Ãâ·Â
				//--------------------------------------------------			
				sX1[1] = m_TileSurfaceFirstSectorX + TILESURFACE_SECTOR_OUTLINE_LEFT;
				sY1[1] = m_TileSurfaceFirstSectorY - TILESURFACE_SECTOR_EDGE;
				sX2[1] = sX1[1]	+ g_TILESURFACE_SECTOR_WIDTH;
				sY2[1] = sY1[1] + TILESURFACE_SECTOR_EDGE;
					
				//--------------------------------------------------
				// Ã¹¹øÂ° Ãâ·Â ÁÂÇ¥
				//--------------------------------------------------
				firstTilePoint[1].x = 0;
				firstTilePoint[1].y = 0;

				//--------------------------------------------------
				// Ãâ·Â¿¡ »ç¿ëÇÒ TileSurface ¿µ¿ª ´Ù½Ã ¼³Á¤
				//--------------------------------------------------
				rectReuse.left -= TILESURFACE_OUTLINE_LEFT;
				rectReuse.right -= TILESURFACE_OUTLINE_LEFT;
				rectReuse.top += TILESURFACE_OUTLINE_UP;
				rectReuse.bottom += TILESURFACE_OUTLINE_UP;

				//--------------------------------------------------
				// °»½ÅµÈ TileSurface¿¡ ´ëÇÑ ÁÂÇ¥ ¼³Á¤
				//--------------------------------------------------
				m_TileSurfaceFirstSectorX += TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelX += TILESURFACE_OUTLINE_LEFT;
				m_TileSurfaceFirstSectorY -= TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelY -= TILESURFACE_OUTLINE_UP;
			}
			//--------------------------
			// RIGHT + DOWN ºÎÁ·
			//--------------------------
			else if (bLack & FLAG_TILESURFACE_LACK_DOWN)
			{
				//--------------------------------------------------
				// ÀÌÀü Tile Surface¿¡¼­
				// ´Ù½Ã ±¸¼ºÇÒ·Á´Â Tile Surface¿¡¼­ »ç¿ëÇÒ ¼ö 
				// ÀÖ´Â ºÎºÐÀ» ³²°ÜµÐ´Ù.			
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
				// ºÎÁ·ÇÑ ºÎºÐÀ» ±×·ÁÁØ´Ù.
				//
				//--------------------------------------------------

				changeCount = 2;

				//--------------------------------------------------
				// RightºÎºÐ
				//--------------------------------------------------
				// Ãâ·ÂÇÒ sector (sX1,sY) ~ (sX2, sY2)±îÁö Ãâ·Â
				//--------------------------------------------------			
				sX1[0] = m_TileSurfaceFirstSectorX + g_TILESURFACE_SECTOR_WIDTH;
				sY1[0] = m_TileSurfaceFirstSectorY + TILESURFACE_SECTOR_OUTLINE_UP;
				sX2[0] = sX1[0]	+ TILESURFACE_SECTOR_EDGE;
				sY2[0] = sY1[0] + g_TILESURFACE_SECTOR_HEIGHT;
					
				//--------------------------------------------------
				// Ã¹¹øÂ° Ãâ·Â ÁÂÇ¥
				//--------------------------------------------------
				firstTilePoint[0].x = g_TILESURFACE_OUTLINE_RIGHT;
				firstTilePoint[0].y = 0;

				//--------------------------------------------------
				// DownºÎºÐ
				//--------------------------------------------------
				// Ãâ·ÂÇÒ sector (sX1,sY) ~ (sX2, sY2)±îÁö Ãâ·Â
				//--------------------------------------------------			
				sX1[1] = m_TileSurfaceFirstSectorX + TILESURFACE_SECTOR_OUTLINE_LEFT;
				sY1[1] = m_TileSurfaceFirstSectorY + g_TILESURFACE_SECTOR_HEIGHT;
				sX2[1] = sX1[1]	+ g_TILESURFACE_SECTOR_WIDTH;
				sY2[1] = sY1[1] + TILESURFACE_SECTOR_EDGE;
					
				//--------------------------------------------------
				// Ã¹¹øÂ° Ãâ·Â ÁÂÇ¥
				//--------------------------------------------------
				firstTilePoint[1].x = 0;
				firstTilePoint[1].y = g_TILESURFACE_OUTLINE_DOWN;

				//--------------------------------------------------
				// Ãâ·Â¿¡ »ç¿ëÇÒ TileSurface ¿µ¿ª ´Ù½Ã ¼³Á¤
				//--------------------------------------------------
				rectReuse.left -= TILESURFACE_OUTLINE_LEFT;
				rectReuse.right -= TILESURFACE_OUTLINE_LEFT;
				rectReuse.top -= TILESURFACE_OUTLINE_UP;
				rectReuse.bottom -= TILESURFACE_OUTLINE_UP;

				//--------------------------------------------------
				// °»½ÅµÈ TileSurface¿¡ ´ëÇÑ ÁÂÇ¥ ¼³Á¤
				//--------------------------------------------------
				m_TileSurfaceFirstSectorX += TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelX += TILESURFACE_OUTLINE_LEFT;
				m_TileSurfaceFirstSectorY += TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelY += TILESURFACE_OUTLINE_UP;
			}
			//--------------------------
			// RIGHT ºÎÁ·
			//--------------------------
			else
			{
				//--------------------------------------------------
				// ÀÌÀü Tile Surface¿¡¼­
				// ´Ù½Ã ±¸¼ºÇÒ·Á´Â Tile Surface¿¡¼­ »ç¿ëÇÒ ¼ö 
				// ÀÖ´Â ºÎºÐÀ» ³²°ÜµÐ´Ù.			
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
				// ºÎÁ·ÇÑ ºÎºÐÀ» ±×·ÁÁØ´Ù.
				//
				//--------------------------------------------------

				changeCount = 1;

				//--------------------------------------------------
				// Ãâ·ÂÇÒ sector (sX1,sY) ~ (sX2, sY2)±îÁö Ãâ·Â
				//--------------------------------------------------			
				sX1[0] = m_TileSurfaceFirstSectorX + g_TILESURFACE_SECTOR_WIDTH;
				sY1[0] = m_TileSurfaceFirstSectorY;
				sX2[0] = sX1[0]	+ TILESURFACE_SECTOR_EDGE;
				sY2[0] = sY1[0] + g_TILESURFACE_SECTOR_HEIGHT;
					
				//--------------------------------------------------
				// Ã¹¹øÂ° Ãâ·Â ÁÂÇ¥
				//--------------------------------------------------
				firstTilePoint[0].x = g_TILESURFACE_OUTLINE_RIGHT;
				firstTilePoint[0].y = 0;

				//--------------------------------------------------
				// Ãâ·Â¿¡ »ç¿ëÇÒ TileSurface ¿µ¿ª ´Ù½Ã ¼³Á¤
				//--------------------------------------------------
				rectReuse.left -= TILESURFACE_OUTLINE_LEFT;
				rectReuse.right -= TILESURFACE_OUTLINE_LEFT;

				//--------------------------------------------------
				// °»½ÅµÈ TileSurface¿¡ ´ëÇÑ ÁÂÇ¥ ¼³Á¤
				//--------------------------------------------------
				m_TileSurfaceFirstSectorX += TILESURFACE_SECTOR_EDGE;
				m_TileSurfaceFirstZonePixelX += TILESURFACE_OUTLINE_LEFT;
			}
		}
		//--------------------------
		// UP ºÎÁ·
		//--------------------------
		else if (bLack & FLAG_TILESURFACE_LACK_UP)
		{
			//--------------------------------------------------
			// ÀÌÀü Tile Surface¿¡¼­
			// ´Ù½Ã ±¸¼ºÇÒ·Á´Â Tile Surface¿¡¼­ »ç¿ëÇÒ ¼ö 
			// ÀÖ´Â ºÎºÐÀ» ³²°ÜµÐ´Ù.			
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
			// ºÎÁ·ÇÑ ºÎºÐÀ» ±×·ÁÁØ´Ù.
			//
			//--------------------------------------------------

			changeCount = 1;

			//--------------------------------------------------
			// Ãâ·ÂÇÒ sector (sX1,sY) ~ (sX2, sY2)±îÁö Ãâ·Â
			//--------------------------------------------------			
			sX1[0] = m_TileSurfaceFirstSectorX;
			sY1[0] = m_TileSurfaceFirstSectorY - TILESURFACE_SECTOR_EDGE;
			sX2[0] = sX1[0]	+ g_TILESURFACE_SECTOR_WIDTH;
			sY2[0] = sY1[0] + TILESURFACE_SECTOR_EDGE;
				
			//--------------------------------------------------
			// Ã¹¹øÂ° Ãâ·Â ÁÂÇ¥
			//--------------------------------------------------
			firstTilePoint[0].x = 0;
			firstTilePoint[0].y = 0;

			//--------------------------------------------------
			// Ãâ·Â¿¡ »ç¿ëÇÒ TileSurface ¿µ¿ª ´Ù½Ã ¼³Á¤
			//--------------------------------------------------
			rectReuse.top += TILESURFACE_OUTLINE_UP;
			rectReuse.bottom += TILESURFACE_OUTLINE_UP;

			//--------------------------------------------------
			// °»½ÅµÈ TileSurface¿¡ ´ëÇÑ ÁÂÇ¥ ¼³Á¤
			//--------------------------------------------------
			m_TileSurfaceFirstSectorY -= TILESURFACE_SECTOR_EDGE;
			m_TileSurfaceFirstZonePixelY -= TILESURFACE_OUTLINE_UP;
		}
		//--------------------------
		// DOWN ºÎÁ·
		//--------------------------
		else if (bLack & FLAG_TILESURFACE_LACK_DOWN)
		{
			//--------------------------------------------------
			// ÀÌÀü Tile Surface¿¡¼­
			// ´Ù½Ã ±¸¼ºÇÒ·Á´Â Tile Surface¿¡¼­ »ç¿ëÇÒ ¼ö 
			// ÀÖ´Â ºÎºÐÀ» ³²°ÜµÐ´Ù.			
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
			// ºÎÁ·ÇÑ ºÎºÐÀ» ±×·ÁÁØ´Ù.
			//
			//--------------------------------------------------

			changeCount = 1;

			//--------------------------------------------------
			// Ãâ·ÂÇÒ sector (sX1,sY) ~ (sX2, sY2)±îÁö Ãâ·Â
			//--------------------------------------------------			
			sX1[0] = m_TileSurfaceFirstSectorX;
			sY1[0] = m_TileSurfaceFirstSectorY + g_TILESURFACE_SECTOR_HEIGHT;
			sX2[0] = sX1[0]	+ g_TILESURFACE_SECTOR_WIDTH;
			sY2[0] = sY1[0] + TILESURFACE_SECTOR_EDGE;
				
			//--------------------------------------------------
			// Ã¹¹øÂ° Ãâ·Â ÁÂÇ¥
			//--------------------------------------------------
			firstTilePoint[0].x = 0;
			firstTilePoint[0].y = g_TILESURFACE_OUTLINE_DOWN;

			//--------------------------------------------------
			// Ãâ·Â¿¡ »ç¿ëÇÒ TileSurface ¿µ¿ª ´Ù½Ã ¼³Á¤
			//--------------------------------------------------
			rectReuse.top -= TILESURFACE_OUTLINE_UP;
			rectReuse.bottom -= TILESURFACE_OUTLINE_UP;

			//--------------------------------------------------
			// °»½ÅµÈ TileSurface¿¡ ´ëÇÑ ÁÂÇ¥ ¼³Á¤
			//--------------------------------------------------
			m_TileSurfaceFirstSectorY += TILESURFACE_SECTOR_EDGE;
			m_TileSurfaceFirstZonePixelY += TILESURFACE_OUTLINE_UP;
		}


		//------------------------------------------------------
		//
		//   ºÎºÐÀûÀ¸·Î TileµéÀ» °»½ÅÇÏ±â..
		//
		//------------------------------------------------------
		register int n;
		for (n=0; n<changeCount; n++)
		{			
			POINT tilePointTemp;

			//------------------------------------------------------
			// ZoneÀÇ ¿µ¿ªÀÌ ¾Æ´Ñ °æ¿ì¿¡ Skip...
			//------------------------------------------------------
			if (sX1[n] < 0) 
			{	
				// Zone¿Ü°û ºÎºÐÀº °ËÀº»öÀ¸·Î Ä¥ÇÑ´Ù.
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

				// Zone¿Ü°û ºÎºÐÀº °ËÀº»öÀ¸·Î Ä¥ÇÑ´Ù.
				rect.left = firstTilePoint[n].x + (sX2[n]-sX1[n])*TILE_X;
				rect.top = 0;					
				rect.right = g_TILESURFACE_WIDTH;
				rect.bottom = g_TILESURFACE_HEIGHT;

				m_pTileSurface->FillRect(&rect, 0);				
			}

			if (sY1[n] < 0)
			{
				// Zone¿Ü°û ºÎºÐÀº °ËÀº»öÀ¸·Î Ä¥ÇÑ´Ù.
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
				// Zone¿Ü°û ºÎºÐÀº °ËÀº»öÀ¸·Î Ä¥ÇÑ´Ù.
				rect.left = firstTilePoint[n].x;
				rect.top = firstTilePoint[n].y;					
				rect.right = firstTilePoint[n].x + (sX2[n]-sX1[n])*TILE_X;
				rect.bottom = g_TILESURFACE_HEIGHT;
				m_pTileSurface->FillRect(&rect, 0);

				sY2[n] = m_pZone->GetHeight();//-1;
			}
					
			// Ã¹¹øÂ° ÁÙ			
			tilePointTemp.y = firstTilePoint[n].y;

			//---------------------------------------
			// LOCK
			//---------------------------------------
			if (!m_pTileSurface->Lock()) return;

			//----------------------------------------------------------------------
			// Use TileRenderer for incremental updates
			// Ensures consistency with DrawTileSurface()
			//----------------------------------------------------------------------
			if (m_pTileRenderer != NULL && m_pTileRenderer->IsInit())
			{
				// Set the zone provider
				m_zoneTileProvider.SetZone(m_pZone);

				// Draw new edge tiles using TileRenderer
				m_pTileRenderer->DrawTilesNoLock(
					&m_zoneTileProvider,
					sX1[n], sY1[n],
					sX2[n] - sX1[n],
					sY2[n] - sY1[n],
					firstTilePoint[n].x,
					firstTilePoint[n].y
				);
			}
			else
			{
				printf("[IncrementalUpdate] ERROR: TileRenderer not initialized! Cannot render tiles.\n");
			}

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


	bool bDrawBackGround = DrawEvent();
	
	__BEGIN_PROFILE("ReuseBltTileSurface")
	// 2004, 9, 3, sobeit add start - Å¸ÀÏ µÞÂÊ¿¡ ±¸¸§-_-;
//	event = g_pEventManager->GetEventByFlag(EVENTFLAG_CLOUD_BACKGROUND);
	if(g_pPlayer->IsShowAdamCloud())
	{
		CDirectDrawSurface *pCloudSurface = g_pEventManager->GetEventBackground(EVENTBACKGROUNDID_CLOUD);
		if(pCloudSurface != NULL)
		{
			int CloudPos = g_CurrentFrame % g_GameRect.right;
			POINT CloudPoint = {0,0};
			RECT CloudRect = { CloudPos, 0, g_GameRect.left, g_GameRect.top };
#ifdef PLATFORM_WINDOWS
			if(CloudPos != g_GameRect.left)
				m_pSurface->BltNoColorkey(&CloudPoint, pCloudSurface, &CloudRect);
			if(CloudPos != 0)
			{
				CloudPoint.x	= g_GameRect.left-CloudPos;
				CloudRect.left	=  0;
				CloudRect.right	=  CloudPos;
				m_pSurface->BltNoColorkey(&CloudPoint, pCloudSurface, &CloudRect);
			}
#else
			// SDL backend: Cast CDirectDrawSurface* to CSpriteSurface* for compatibility
			CSpriteSurface* pCloudSprite = reinterpret_cast<CSpriteSurface*>(pCloudSurface);
			if(CloudPos != g_GameRect.left)
				m_pSurface->BltNoColorkey(&CloudPoint, pCloudSprite, &CloudRect);
			if(CloudPos != 0)
			{
				CloudPoint.x	= g_GameRect.left-CloudPos;
				CloudRect.left	=  0;
				CloudRect.right	=  CloudPos;
				m_pSurface->BltNoColorkey(&CloudPoint, pCloudSprite, &CloudRect);
			}
#endif
			if(bDrawBackGround)
				m_pSurface->Blt(&point, m_pTileSurface, &rectReuse);
		}
		else
		if(bDrawBackGround)
			m_pSurface->BltNoColorkey(&point, m_pTileSurface, &rectReuse);
	}
	else
	// 2004, 9, 3, sobeit add end - Å¸ÀÏ µÞÂÊ¿¡ ±¸¸§-_-;
	if(bDrawBackGround)
	{
		m_pSurface->BltNoColorkey(&point, m_pTileSurface, &rectReuse);
	}
	

	__END_PROFILE("ReuseBltTileSurface")

	__END_PROFILE("ReuseTileSurface")

	POINT tilePointTemp;
	int sX1, sX2, sY1, sY2;	

	//------------------------------------------------------
	// Object¸¦ Ãâ·ÂÇÏ±â À§ÇÑ È­¸é ÁÂÇ¥ ¼³Á¤
	//------------------------------------------------------
	sX1 = firstSector.x + SECTOR_SKIP_LEFT;
	sY1 = firstSector.y + SECTOR_SKIP_UP;
	sX2 = firstSector.x + g_SECTOR_WIDTH;
	sY2 = firstSector.y + g_SECTOR_HEIGHT;

	// Ãâ·ÂÇÒ Surface»óÀÇ À§Ä¡
	tilePoint.x = firstPointX + TILE_X*SECTOR_SKIP_LEFT;
	tilePoint.y = firstPointY + TILE_Y*SECTOR_SKIP_UP;

	//------------------------------------------------------
	// ZoneÀÇ ¿µ¿ªÀÌ ¾Æ´Ñ °æ¿ì¿¡ Skip...
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
	//                  Object Ãâ·Â
	//
	//------------------------------------------------------

	//BYTE DarkBits;

	//---------------------------------------
	// LOCK
	//---------------------------------------
	if (!m_pSurface->Lock()) return;

// 2004, 03, 24, sobeit start
	//------------------------------------------------------
	// Creature OutputMapÀ» »ý¼ºÇÑ´Ù.
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
			// Ãâ·Â ½ÃÁ¡ÀÌ sY1º¸´Ù ÀûÀº °æ¿ì..´Â Ãâ·Â
			// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
			point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
			point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
									
			// ¹à±â¸¦ °áÁ¤ÇÑ´Ù.
			//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;
			
			DrawCreature(&point, pCreature);//, DarkBits);
		}
	
		iCreatureOutput ++;
	}
// 2004, 03, 24, sobeit end
	/*
	//------------------------------------------------------
	// Sprite ÇÏ³ª º¸±â
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


	// SurfaceÀÇ Á¤º¸¸¦ ÀúÀåÇØµÐ´Ù.
	//SetSurfaceInfo(&m_SurfaceInfo, m_pSurface->GetDDSD());

	//------------------------------------------------------
	//
	//			¹Ù´Ú ¿ÀºêÁ§Æ® Ãâ·Â
	//
	//------------------------------------------------------	
	//------------------------------------------------------
	// Ãâ·Â´ë»óÀÌ µÇ´Â ImageObject Iterator
	//------------------------------------------------------
	IMAGEOBJECT_OUTPUT_MAP::const_iterator iImageObjectOutput0 = m_mapImageObject.begin();
	
	//------------------------------------------------------
	// Ãâ·Â ½ÃÁ¡ÀÌ 0 Áï ¹Ù´Ú ¿ÀºêÁ§Æ®ÀÎ
	// ImageObjectµéÀ» Ãâ·ÂÇÑ´Ù.
	// m_mapImageObject¿¡´Â Viewpoint°¡ key°ªÀÌ µÇ¾î¼­ SortµÇ¾î ÀÖ´Ù.
	//------------------------------------------------------
	while (bDrawBackGround && iImageObjectOutput0 != m_mapImageObject.end())
	{
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectOutput0).second);

		// Ãâ·Â ½ÃÁ¡ÀÌ 0ÀÎ°æ¿ì °æ¿ì..´Â Ãâ·Â
		if (pImageObject->GetViewpoint() == 0)
		{
			// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
			point.x = pImageObject->GetPixelX() - m_FirstZonePixel.x;
			point.y = pImageObject->GetPixelY() - m_FirstZonePixel.y;
									
			DrawImageObject(&point, pImageObject);
		}
		// ¾Æ´Ï¸é.. Object Ãâ·ÂÇÒ¶§ Ãâ·ÂµÇ´Â °æ¿ìÀÌ´Ù.
//		else
//		{
//			break;
//		}

		iImageObjectOutput0 ++;
	}


	//------------------------------------------------------
	//
	//			Ground Effect Ãâ·Â
	//
	//------------------------------------------------------	
	// player°¡ °ü¼Ó¿¡ ÀÖ´Â °æ¿ì [»õ±â¼ú3]
	//------------------------------------------------------
	bool bPlayerInCasket = g_pPlayer->IsInCasket();
		
	if (bPlayerInCasket)
	{
		// [»õ±â¼ú3]
		// È­¸é ÀüÃ¼¸¦ ¾îµÓ°Ô Ç¥½ÃÇÑ´Ù. Tile¸¸ - -;;
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
	// ¼±ÅÃµÈ Sector¸¦ Ç¥½ÃÇÑ´Ù.
	//
	//------------------------------------------------------
	// ¼±ÅÃµÈ À§Ä¡°¡ ÀÖ°í.. ¼±ÅÃµÈ Object°¡ ¾øÀ» ¶§...
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
				// Áß½É ÁÂÇ¥ º¸Á¤
				CSprite* pSprite = &m_EtcSPK[ frameID[clickFrame] ];
				
				selectedPoint.x += 24 - (pSprite->GetWidth()>>1);
				selectedPoint.y += 24 - (pSprite->GetHeight()>>1);
				
				
				m_pSurface->BltSprite(&selectedPoint, pSprite);
			}
			
			static DWORD lastTime = g_CurrentTime;
			if (g_CurrentTime - lastTime >= g_UpdateDelay)
			{
				if (++clickFrame==MaxClickFrame) clickFrame = 0;
				lastTime = g_CurrentTime;
			}
			
			
			// Player°¡ ¼±ÅÃµÈ À§Ä¡¿¡ ¿ÔÀ¸¸é..
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
	//       ImageObjectÀÇ ±×¸²ÀÚ Ãâ·Â
	//
	//------------------------------------------------------
	//------------------------------------------------------
	// Ãâ·Â´ë»óÀÌ µÇ´Â ImageObject Iterator
	//------------------------------------------------------
	__BEGIN_PROFILE("DrawImageObjectShadow")

	IMAGEOBJECT_OUTPUT_MAP::const_iterator iImageObjectShadowOutput = m_mapImageObject.begin();

	TYPE_SPRITEID	sprite;
	while (bDrawBackGround && iImageObjectShadowOutput != m_mapImageObject.end())
	{
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectShadowOutput).second);
		
		// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
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
	// Ãâ·Â ½ÃÁ¡º¸´Ù À§ÂÊ¿¡ ÀÖ´Â ImageObjectµéÀ» Ãâ·ÂÇÑ´Ù.
	//
	//------------------------------------------------------
	//------------------------------------------------------
	// Ãâ·Â´ë»óÀÌ µÇ´Â ImageObject Iterator
	//------------------------------------------------------
	IMAGEOBJECT_OUTPUT_MAP::const_iterator iImageObjectOutput = m_mapImageObject.begin();
	
	//------------------------------------------------------
	// Ãâ·Â ½ÃÁ¡ÀÌ sY1º¸´Ù ÀûÀº °æ¿ìÀÇ 
	// ImageObjectµéÀ» Ãâ·ÂÇÑ´Ù.
	// m_mapImageObject¿¡´Â Viewpoint°¡ key°ªÀÌ µÇ¾î¼­ SortµÇ¾î ÀÖ´Ù.
	//------------------------------------------------------
	while (bDrawBackGround && iImageObjectOutput != m_mapImageObject.end())
	{
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectOutput).second);

		// Ãâ·Â ½ÃÁ¡ÀÌ sY1º¸´Ù ÀûÀº °æ¿ì..´Â Ãâ·Â
		if (pImageObject->GetViewpoint() < sY1)
		{
			if(pImageObject->GetViewpoint() != 0)
			{
				// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
				point.x = pImageObject->GetPixelX() - m_FirstZonePixel.x;
				point.y = pImageObject->GetPixelY() - m_FirstZonePixel.y;
										
				DrawImageObject(&point, pImageObject);
			}
		}
		// ¾Æ´Ï¸é.. Object Ãâ·ÂÇÒ¶§ Ãâ·ÂµÇ´Â °æ¿ìÀÌ´Ù.
		else
		{
			break;
		}

		iImageObjectOutput ++;
	}

	// ¾Ö´Ï¸ÞÀÌ¼Ç ¿ÀºêÁ§Æ®¿¡¼­ Loop°¡ µÇ´Â ¿¡´Ï¸ÞÀÌ¼Ç ¿ÀºêÁ§Æ®ÀÇ FrameÀ» ¸ÂÃß±â À§ÇØ¼­
	if(g_bFrameChanged)
		MAnimationObject::NextLoopFrame();

/*	- 2004, 4, 24 sobeit block - ghost ¶§¹®¿¡ À§¿¡¼­ Ã³¸® ÇÔ
	//------------------------------------------------------
	// player°¡ °ü ¼Ó¿¡ ÀÖ´Â °æ¿ì°¡ ¾Æ´Ï¸é.. [»õ±â¼ú3]
	//------------------------------------------------------
	//------------------------------------------------------
	// Creature OutputMapÀ» »ý¼ºÇÑ´Ù.
	//------------------------------------------------------
	__BEGIN_PROFILE("AddOutputCreature")

	AddOutputCreatureAll();

	__END_PROFILE("AddOutputCreature")

	//------------------------------------------------------
	//
	//			¸ðµç creatureÀÇ ±×¸²ÀÚ¸¦ Ãâ·ÂÇÑ´Ù.
	//
	//------------------------------------------------------
	// ¸ðµç creatureÀÇ shadow¸¦ Ãâ·ÂÇÑ´Ù.
	// (!) AddOutputCreatureAll()¸¦ ÇÑ ´ÙÀ½¿¡ ½ÇÇàÇØ¾ß ÇÑ´Ù.
	//
	// LockµÈ »óÅÂ¿¡¼­ ½ÇÇàÇÑ´Ù°í °¡Á¤ÇÑ´Ù.
	//----------------------------------------------------
	// 3D°¡¼ÓÇÒ¶§´Â UnlockÀ¸·Î ¸¸µç´Ù.
	//----------------------------------------------------
	BOOL bUnlockStatus = CDirect3D::IsHAL() && g_pUserOption->BlendingShadow;
	if (bUnlockStatus)
	{	
		m_pSurface->Unlock();
	}
*/
	//------------------------------------------------------
	// Ãâ·Â´ë»óÀÌ µÇ´Â Creature Iterator
	//------------------------------------------------------
	__BEGIN_PROFILE("DrawCreatureShadowAll")

	//CREATURE_OUTPUT_MAP::const_iterator iCreatureOutput = m_mapCreature.begin();
	iCreatureOutput = m_mapCreature.begin();

	//------------------------------------------------------
	// Ä³¸¯ÅÍ ±×¸²ÀÚ¸¦ ¸ðµÎ Ãâ·ÂÇÑ´Ù.
	//------------------------------------------------------
	// Å¸ÀÏÀ» Ã¼Å©ÇØ¼­ Ãâ·ÂÇÏ´Â °æ¿ì
	//------------------------------------------------------
	if (m_bTileSearchForCreature)
	{
		for (y=sY1; y<=sY2; y++)
		{
			for (x=sX1; x<=sX2; x++)
			{	
				int darknessCount = g_pPlayer->GetDarknessCount();
				
				// darkness¶û °ü°è¾øÀÌ º¸ÀÌ´Â °æ¿ì. effect´Â +2
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
							// playerÀÌ°Å³ª darkness¶û °ü°è¾øÀÌ º¸ÀÌ´Â °æ¿ì
							if (pCreature==g_pPlayer
								|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
							{				
								// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
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

								// underground´Â ±×¸²ÀÚ°¡ ¾ø´ç.
								if (iCreature->first >= MSector::POSITION_GROUNDCREATURE)
								{
									pCreature = (MCreature*)iCreature->second;

									// playerÀÌ°Å³ª darkness¶û °ü°è¾øÀÌ º¸ÀÌ´Â °æ¿ì
									if (
										(pCreature==g_pPlayer
										|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY())) &&
										!pCreature->IsFakeCreature()										
										)
									{				
										// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
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

		// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
		point.x = g_pPlayer->GetPixelX() - m_FirstZonePixel.x;
		point.y = g_pPlayer->GetPixelY() - m_FirstZonePixel.y;
								
		DrawCreatureShadow(&point, g_pPlayer);//, DarkBits);		
	}
	//------------------------------------------------------
	// Ä³¸¯ÅÍ OutputMapÀ» ÀÌ¿ëÇÏ´Â °æ¿ì
	//------------------------------------------------------
	else
	{
		while (iCreatureOutput != m_mapCreature.end())
		{
			MCreature* const pCreature = (MCreature* const)((*iCreatureOutput).second);

			if (pCreature==g_pPlayer
				|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
			{
				// Ãâ·Â ½ÃÁ¡ÀÌ sY1º¸´Ù ÀûÀº °æ¿ì..´Â Ãâ·Â
				// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
				point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
				point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
										
				// ¹à±â¸¦ °áÁ¤ÇÑ´Ù.
				//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;
				
				DrawCreatureShadow(&point, pCreature);//, DarkBits);
			}
		
			iCreatureOutput ++;
		}
	}

	__END_PROFILE("DrawCreatureShadowAll")

	//----------------------------------------------------
	// ¿ø·¡ÀÇ LockµÈ »óÅÂ¸¦ À¯Áö½ÃÄÑ ÁØ´Ù.
	//----------------------------------------------------
	if (bUnlockStatus)
	{	
		m_pSurface->Lock();
	}

	// Item ±×¸²ÀÚ Ãâ·Â¿ë
	//------------------------------------------------------
	// È­¸éÀÇ SectorµéÀ» ¸ðµÎ °Ë»öÇÏ¸é¼­
	// Sector¿¡ Á¸ÀçÇÏ´Â ObjectµéÀ» Ãâ·ÂÇÑ´Ù.
	//------------------------------------------------------
	// Ã¹¹øÂ° ÁÙ	
	tilePointTemp.y = tilePoint.y;

	//----------------------------------------------------------------
	//
	// tileÀ» °Ë»öÇØ¼­ Ä³¸¯ÅÍ¸¦ Ãâ·ÂÇÏ´Â °æ¿ì
	//
	//----------------------------------------------------------------
	if (m_bTileSearchForCreature)
	{
		for (y=sY1; y<=sY2; y++)
		{				
			// ÇÑ ÁÙÀÇ Ã¹¹øÂ° Sector					
			tilePointTemp.x = tilePoint.x;			
			
			for (x=sX1; x<=sX2; x++)
			{	
				const MSector& sector = m_pZone->GetSector(x,y);			

				//------------------------------------------------
				//
				//              Object Ãâ·Â
				//
				//------------------------------------------------
					
				if (sector.IsExistObject())
				{				
			
					//----------------------------------------
					// ItemÀÏ °æ¿ì
					//----------------------------------------					
					pItem = sector.GetItem();
					if (pItem != NULL
						&& g_pPlayer->ShowInDarkness(x, y))
					{		
						//----------------------------------------
						// ½ÃÃ¼ÀÎ °æ¿ì
						// Effect¸¦ Ãâ·ÂÇÏ°í ³ª¼­ Ãâ·ÂÇÑ´Ù.
						//
						// 2002.1.23
						// ¾ðÁ¦ EffectÃâ·ÂÄÚµå°¡ ºüÁø°ÅÁö..
						// ¾ÏÆ° ½ÃÃ¼ °ü·Ã ÄÚµå´Â »«´Ù. 
						//----------------------------------------
						//if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
						{
						//	pCorpseItem = pItem;
						}
						//else
						{
							point.x = tilePointTemp.x;
							point.y = tilePointTemp.y;


							// ¹à±â¸¦ °áÁ¤ÇÑ´Ù.
							//DarkBits = (sector.GetLight()==0)?m_DarkBits:0;
							
							// ³ªÁß¿¡ FrameÁ¤º¸¿¡¼­ cx,cy¸¦ ±³Á¤ÇØ¾ß ÇÑ´Ù.									
							DrawItemShadow(&point, pItem);//, DarkBits);
						}
					}
				}


				//------------------------------------			
				// Ãâ·ÂÇÏ·Á´Â ÁÂÇ¥ ÀÌµ¿
				//------------------------------------
				tilePointTemp.x += TILE_X;
			}
			
					
			// ´ÙÀ½ ÁÙ
			tilePointTemp.y += TILE_Y;					
		}		
	}
	//----------------------------------------------------------------
	//
	// tileÀ» °Ë»öÇÏÁö ¾Ê°í OutputCreatureMapÀ» ÀÌ¿ëÇØ¼­ Ä³¸¯ÅÍ¸¦ Ãâ·ÂÇÏ´Â °æ¿ì
	//
	//----------------------------------------------------------------
	else
	{	
		for (y=sY1; y<=sY2; y++)
		{
			// °ü ¼Ó¿¡ ÀÖÀ» ¶§´Â ±×³É °Ë»öÇØ¼­ Ãâ·ÂÇÏ´Â object´Â ¾ø´Ù.
			// AddOutputCreatureAll()¿¡¼­ m_bTileSearchForCreature°¡ false·Î
			// ¼³Á¤µÇ¹Ç·Î... OutputCreatureMapÀ» ÀÌ¿ëÇÏ¸é µÈ´Ù.
			if (!bPlayerInCasket)	// [»õ±â¼ú3]
			{			
				// ÇÑ ÁÙÀÇ Ã¹¹øÂ° Sector					
				tilePointTemp.x = tilePoint.x;			
				
				for (x=sX1; x<=sX2; x++)
				{	
					const MSector& sector = m_pZone->GetSector(x,y);
					
					//------------------------------------------------
					//
					//              Object Ãâ·Â
					//
					//------------------------------------------------
					//MItem* pCorpseItem = NULL;
						
					if (sector.IsExistObject())
					{				
						//----------------------------------------
						// ItemÀÏ °æ¿ì
						//----------------------------------------					
						pItem = sector.GetItem();
						if (pItem != NULL
							&& g_pPlayer->ShowInDarkness(x, y))
						{		
							//----------------------------------------
							// ½ÃÃ¼ÀÎ °æ¿ì
							// Effect¸¦ Ãâ·ÂÇÏ°í ³ª¼­ Ãâ·ÂÇÑ´Ù.
							//
							// 2002.1.23
							// ¾ðÁ¦ EffectÃâ·ÂÄÚµå°¡ ºüÁø°ÅÁö..
							// ¾ÏÆ° ½ÃÃ¼ °ü·Ã ÄÚµå´Â »«´Ù. 
							//----------------------------------------
							//if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
							{
							//	pCorpseItem = pItem;
							}
							//else
							{
								point.x = tilePointTemp.x;
								point.y = tilePointTemp.y;

								// ³ªÁß¿¡ FrameÁ¤º¸¿¡¼­ cx,cy¸¦ ±³Á¤ÇØ¾ß ÇÑ´Ù.									
								DrawItemShadow(&point, pItem);//, DarkBits);
							}
						}
					}


					//------------------------------------			
					// Ãâ·ÂÇÏ·Á´Â ÁÂÇ¥ ÀÌµ¿
					//------------------------------------
					tilePointTemp.x += TILE_X;
				}
			}	// bPlayerInCasket
					
			// ´ÙÀ½ ÁÙ
			tilePointTemp.y += TILE_Y;
		}
	}


	//------------------------------------------------------
	//
	//				½ÇÁ¦ sprite Ãâ·Â
	//
	//------------------------------------------------------
	//------------------------------------------------------
	// Ãâ·Â´ë»óÀÌ µÇ´Â Creature Iterator
	//------------------------------------------------------
	iCreatureOutput = m_mapCreature.begin();

	//------------------------------------------------------
	// Ãâ·Â ½ÃÁ¡ÀÌ sY1º¸´Ù ÀûÀº °æ¿ìÀÇ 
	// CreatureµéÀ» Ãâ·ÂÇÑ´Ù.
	// m_mapCreature¿¡´Â Viewpoint°¡ key°ªÀÌ µÇ¾î¼­ SortµÇ¾î ÀÖ´Ù.
	//------------------------------------------------------
	while (iCreatureOutput != m_mapCreature.end())
	{
		MCreature* const pCreature = (MCreature* const)((*iCreatureOutput).second);

		// 2004, 04, 24 sobeit add start -ghost ÀÏ¶§ 
		if(pCreature->GetCreatureType() == CREATURETYPE_GHOST)
		{
			iCreatureOutput ++;
			continue;
		}
		// 2004, 04, 24 sobeit add end -ghost ÀÏ¶§ 

		// Ãâ·Â ½ÃÁ¡ÀÌ sY1º¸´Ù ÀûÀº °æ¿ì..´Â Ãâ·Â
		if (pCreature->GetY() < sY1)
		{
			// playerÀÌ°Å³ª darkness¶û °ü°è¾øÀÌ º¸ÀÌ´Â °æ¿ì
			if (pCreature==g_pPlayer
				|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
			{					
				// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
				point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
				point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
										
				// ¹à±â¸¦ °áÁ¤ÇÑ´Ù.
				//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;

				DrawCreature(&point, pCreature);//, DarkBits);
			}
		}
		// ¾Æ´Ï¸é.. ÇÑ ÁÙ Ãâ·ÂÀÌ ³¡³ª°í Ãâ·ÂÇÒ¶§ Ãâ·ÂµÇ´Â °æ¿ìÀÌ´Ù.
		else
		{
			break;
		}

		iCreatureOutput ++;
	}

	//------------------------------------------------------
	// È­¸éÀÇ SectorµéÀ» ¸ðµÎ °Ë»öÇÏ¸é¼­
	// Sector¿¡ Á¸ÀçÇÏ´Â ObjectµéÀ» Ãâ·ÂÇÑ´Ù.
	//------------------------------------------------------
	// Ã¹¹øÂ° ÁÙ	
	tilePointTemp.y = tilePoint.y;

	//----------------------------------------------------------------
	//
	// tileÀ» °Ë»öÇØ¼­ Ä³¸¯ÅÍ¸¦ Ãâ·ÂÇÏ´Â °æ¿ì
	//
	//----------------------------------------------------------------
	if (m_bTileSearchForCreature)
	{
		for (y=sY1; y<=sY2; y++)
		{				
			// ÇÑ ÁÙÀÇ Ã¹¹øÂ° Sector					
			tilePointTemp.x = tilePoint.x;			
			
			for (x=sX1; x<=sX2; x++)
			{	
				const MSector& sector = m_pZone->GetSector(x,y);			

				//------------------------------------------------------
				// ¼­¹ö blockÁÂÇ¥¿¡ »ç°¢Çü
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
				// Æ¯Á¤ÇÑ ImageObjectÀÇ ViewSector?¿¡ "X"Ç¥ÇÏ±â
				//------------------------------------------------------
				#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
					if (g_pDXInput->KeyDown(DIK_A) && 
						(g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL)))
					{
						if (sector.IsExistImageObject())
						{
							OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

							// Sector¿¡ ÀÖ´Â ¸ðµç ImageObjectµéÀ» °Ë»öÇÑ´Ù.
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
				// ºûÀÇ °­µµ¸¦ Ãâ·ÂÇÑ´Ù.
				//------------------------------------------------
				//if (x==g_SelectSector.x && y==g_SelectSector.y)
				//{
				//	char str[80];
				//	sprintf(str, "%d", sector.GetLight());
				//	gC_font.PrintStringNoConvert(&m_SurfaceInfo, str, tilePointTemp.x+25,tilePointTemp.y+12, CDirectDraw::Color(28,28,28));				
				//}

				//------------------------------------------------
				//
				//              Object Ãâ·Â
				//
				//------------------------------------------------
				//MItem* pCorpseItem = NULL;
					
				if (sector.IsExistObject())
				{				
					//----------------------------------------
					// test code : Object À§Ä¡ Ç¥½Ã
					//----------------------------------------
					//point = tilePointTemp;
					//m_pSurface->Copy(&point, m_pm_pSurface[0], 
					//				&m_SpriteSurfacePack[0][105]);
					//
					//----------------------------------------
			
					//----------------------------------------
					// ItemÀÏ °æ¿ì
					//----------------------------------------					
					pItem = sector.GetItem();
					if (pItem != NULL
						&& g_pPlayer->ShowInDarkness(x, y))
					{		
						//----------------------------------------
						// ½ÃÃ¼ÀÎ °æ¿ì
						// Effect¸¦ Ãâ·ÂÇÏ°í ³ª¼­ Ãâ·ÂÇÑ´Ù.
						//
						// 2002.1.23
						// ¾ðÁ¦ EffectÃâ·ÂÄÚµå°¡ ºüÁø°ÅÁö..
						// ¾ÏÆ° ½ÃÃ¼ °ü·Ã ÄÚµå´Â »«´Ù. 
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
							// ¿ï··¿ï··~~
							int size = g_CurrentFrame & 0x00000007;

							int x0 = point.x + 24;
							int y0 = point.y + 24;
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

							// ¹à±â¸¦ °áÁ¤ÇÑ´Ù.
							//DarkBits = (sector.GetLight()==0)?m_DarkBits:0;
							
							// ³ªÁß¿¡ FrameÁ¤º¸¿¡¼­ cx,cy¸¦ ±³Á¤ÇØ¾ß ÇÑ´Ù.									
							DrawItem(&point, pItem);//, DarkBits);
						}
					}
				}

				//------------------------------------------------
				//
				//              Effect Ãâ·Â
				//
				//------------------------------------------------
				//if (sector.IsExistEffect())
				//{				
				//	point = tilePointTemp;				
	//
	//				DrawEffect(&point, sector.GetEffectIterator(), sector.GetEffectSize());
	//			}

				//------------------------------------------------
				// ½ÃÃ¼
				//------------------------------------------------
				// À½!! ²ÙÁöÇÑ ÄÚµåÀÇ Áö¸§±æ.. À½³Ä.. ³ªµµ ¸ô¶ó
				//------------------------------------------------
				/*
				if (pCorpseItem)
				{
					point.x = tilePointTemp.x;
					point.y = tilePointTemp.y;

					// ¹à±â¸¦ °áÁ¤ÇÑ´Ù.
					//DarkBits = (sector.GetLight()==0)?m_DarkBits:0;
					
					// ³ªÁß¿¡ FrameÁ¤º¸¿¡¼­ cx,cy¸¦ ±³Á¤ÇØ¾ß ÇÑ´Ù.									
					DrawItem(&point, pItem);//, DarkBits);
				}
				*/

				//------------------------------------			
				// Ãâ·ÂÇÏ·Á´Â ÁÂÇ¥ ÀÌµ¿
				//------------------------------------
				tilePointTemp.x += TILE_X;
			}
			
			//-----------------------------------------------------
			// ÇÑ ÁÙÀÌ ³¡³¯¶§¸¶´Ù 
			// Ãâ·ÂÇØ¾ßÇÒ ±× ÁÙÀÇ ImageObjectµéÀ» Ãâ·ÂÇÑ´Ù.
			//-----------------------------------------------------	
			while (bDrawBackGround && iImageObjectOutput != m_mapImageObject.end())
			{
				MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectOutput).second);

				// Ãâ·Â ½ÃÁ¡ÀÌ y¿Í °°Àº °æ¿ì Ãâ·Â
				if (pImageObject->GetViewpoint() <= y)
				{
					if(pImageObject->GetViewpoint() != 0)
					{
						// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
						point.x = pImageObject->GetPixelX() - m_FirstZonePixel.x;
						point.y = pImageObject->GetPixelY() - m_FirstZonePixel.y;				

						DrawImageObject(&point, pImageObject);

						//char str[80];
						//sprintf(str, "%d", pImageObject->GetViewpoint());
						//m_pSurface->GDI_Text(point.x, point.y, str, 0xFFFFFF);
					}
				}
				// ¾Æ´Ï¸é.. ´Ù¸¥ ÁÙÀÌ´Ù.
				else
				{				
					break;
				}

				iImageObjectOutput ++;
			}

			//------------------------------------------------------
			// ÇÑ ÁÙÀÌ ³¡³¯¶§¸¶´Ù 
			// Ãâ·ÂÇØ¾ßÇÒ ±× ÁÙÀÇ CreatureµéÀ» Ãâ·ÂÇÑ´Ù.
			//------------------------------------------------------
			while (iCreatureOutput != m_mapCreature.end())
			{
				MCreature* const pCreature = (MCreature* const)((*iCreatureOutput).second);

				// Ãâ·Â ½ÃÁ¡ÀÌ sY1º¸´Ù ÀûÀº °æ¿ì..´Â Ãâ·Â
				if (pCreature->GetY() <= y)
				{
					// 2004, 04, 24 sobeit add start -ghost ÀÏ¶§ 
					if(pCreature->GetCreatureType() == CREATURETYPE_GHOST)
					{
						iCreatureOutput ++;
						continue;
					}
					// 2004, 04, 24 sobeit add end -ghost ÀÏ¶§ 

					// playerÀÌ°Å³ª darkness¶û °ü°è¾øÀÌ º¸ÀÌ´Â °æ¿ì
					if (pCreature==g_pPlayer
						|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
					{				
						// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
						point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
						point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
												
						// ¹à±â¸¦ °áÁ¤ÇÑ´Ù.
						//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;

						DrawCreature(&point, pCreature);//, DarkBits);
					}
				}
				// ¾Æ´Ï¸é.. ÇÑ ÁÙ Ãâ·ÂÀÌ ³¡³ª°í Ãâ·ÂÇÒ¶§ Ãâ·ÂµÇ´Â °æ¿ìÀÌ´Ù.
				else
				{
					break;
				}

				iCreatureOutput ++;
			}

			//------------------------------------------------------
			// Ãâ·Â¼ø¼­¶§¹®¿¡.. ¿©±â¼­ Ä³¸¯ÅÍ¸¦ Ã¼Å©ÇÑ´Ù.
			// SectorÀÇ Effect Ãâ·Â
			//------------------------------------------------------
			// Ä³¸¯ÅÍº¸´Ù ´Ê°Ô Ãâ·ÂµÇ¾î¾ß ÇÏ¹Ç·Î...
			//------------------------------------------------------
			//------------------------------------------------------
			// Player´Â Tile¿¡ ¼ÓÇÏÁö ¾Ê±â ¶§¹®¿¡ µû·Î Ã¼Å©ÇÑ´Ù.
			//------------------------------------------------------
			if (g_pPlayer->GetY() == y)
			{
				// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
				point.x = g_pPlayer->GetPixelX() - m_FirstZonePixel.x;
				point.y = g_pPlayer->GetPixelY() - m_FirstZonePixel.y;
										
				// ¹à±â¸¦ °áÁ¤ÇÑ´Ù.
				//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;

				DrawCreature(&point, g_pPlayer);//, DarkBits);
			}

			tilePointTemp.x = tilePoint.x;
			for (x=sX1; x<=sX2; x++)
			{	
				int darknessCount = g_pPlayer->GetDarknessCount();
				
				// darkness¶û °ü°è¾øÀÌ º¸ÀÌ´Â °æ¿ì. effect´Â +2
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
						
								// 2004, 04, 24 sobeit add start -ghost ÀÏ¶§ 
								if(pCreature->GetCreatureType() == CREATURETYPE_GHOST)
									continue;
								// 2004, 04, 24 sobeit add end -ghost ÀÏ¶§ 

								// playerÀÌ°Å³ª darkness¶û °ü°è¾øÀÌ º¸ÀÌ´Â °æ¿ì
								if (pCreature==g_pPlayer
									|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
								{				
									// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
									point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
									point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
															
									DrawCreature(&point, pCreature);//, DarkBits);
								}
							}
						}
					}

					//------------------------------------------------
					//
					//              Effect Ãâ·Â
					//
					//------------------------------------------------
					if (sector.IsExistEffect())
					{				
						point = tilePointTemp;				

	 					DrawEffect(&point, sector.GetEffectIterator(), sector.GetEffectSize());
					}
				}

				//------------------------------------			
				// Ãâ·ÂÇÏ·Á´Â ÁÂÇ¥ ÀÌµ¿
				//------------------------------------
				tilePointTemp.x += TILE_X;
			}
					
			// ´ÙÀ½ ÁÙ
			tilePointTemp.y += TILE_Y;					
		}		
	}
	//----------------------------------------------------------------
	//
	// tileÀ» °Ë»öÇÏÁö ¾Ê°í OutputCreatureMapÀ» ÀÌ¿ëÇØ¼­ Ä³¸¯ÅÍ¸¦ Ãâ·ÂÇÏ´Â °æ¿ì
	//
	//----------------------------------------------------------------
	else
	{	
		for (y=sY1; y<=sY2; y++)
		{
			// °ü ¼Ó¿¡ ÀÖÀ» ¶§´Â ±×³É °Ë»öÇØ¼­ Ãâ·ÂÇÏ´Â object´Â ¾ø´Ù.
			// AddOutputCreatureAll()¿¡¼­ m_bTileSearchForCreature°¡ false·Î
			// ¼³Á¤µÇ¹Ç·Î... OutputCreatureMapÀ» ÀÌ¿ëÇÏ¸é µÈ´Ù.
			if (!bPlayerInCasket)	// [»õ±â¼ú3]
			{			
				// ÇÑ ÁÙÀÇ Ã¹¹øÂ° Sector					
				tilePointTemp.x = tilePoint.x;			
				
				for (x=sX1; x<=sX2; x++)
				{	
					const MSector& sector = m_pZone->GetSector(x,y);
					
					//------------------------------------------------------
					// ¼­¹ö blockÁÂÇ¥¿¡ »ç°¢Çü
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
					// Æ¯Á¤ÇÑ ImageObjectÀÇ ViewSector?¿¡ "X"Ç¥ÇÏ±â
					//------------------------------------------------------
					#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
						if (g_pDXInput->KeyDown(DIK_A) && 
							(g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL)))
						{
							if (sector.IsExistImageObject())
							{
								OBJECT_MAP::const_iterator iImageObject = sector.GetImageObjectIterator();

								// Sector¿¡ ÀÖ´Â ¸ðµç ImageObjectµéÀ» °Ë»öÇÑ´Ù.
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
					// ºûÀÇ °­µµ¸¦ Ãâ·ÂÇÑ´Ù.
					//------------------------------------------------
					//if (x==g_SelectSector.x && y==g_SelectSector.y)
					//{
					//	char str[80];
					//	sprintf(str, "%d", sector.GetLight());
					//	gC_font.PrintStringNoConvert(&m_SurfaceInfo, str, tilePointTemp.x+25,tilePointTemp.y+12, CDirectDraw::Color(28,28,28));				
					//}

					//------------------------------------------------
					//
					//              Object Ãâ·Â
					//
					//------------------------------------------------
					//MItem* pCorpseItem = NULL;
						
					if (sector.IsExistObject())
					{				
						//----------------------------------------
						// test code : Object À§Ä¡ Ç¥½Ã
						//----------------------------------------
						//point = tilePointTemp;
						//m_pSurface->Copy(&point, m_pm_pSurface[0], 
						//				&m_SpriteSurfacePack[0][105]);
						//
						//----------------------------------------
				
						//----------------------------------------
						// ItemÀÏ °æ¿ì
						//----------------------------------------					
						pItem = sector.GetItem();
						if (pItem != NULL
							&& g_pPlayer->ShowInDarkness(x, y))
						{		
							//----------------------------------------
							// ½ÃÃ¼ÀÎ °æ¿ì
							// Effect¸¦ Ãâ·ÂÇÏ°í ³ª¼­ Ãâ·ÂÇÑ´Ù.
							//
							// 2002.1.23
							// ¾ðÁ¦ EffectÃâ·ÂÄÚµå°¡ ºüÁø°ÅÁö..
							// ¾ÏÆ° ½ÃÃ¼ °ü·Ã ÄÚµå´Â »«´Ù. 
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
								// ¿ï··¿ï··~~
								int size = g_CurrentFrame & 0x00000007;

								int x0 = point.x + 24;
								int y0 = point.y + 24;
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

								// ¹à±â¸¦ °áÁ¤ÇÑ´Ù.
								//DarkBits = (sector.GetLight()==0)?m_DarkBits:0;
								
								// ³ªÁß¿¡ FrameÁ¤º¸¿¡¼­ cx,cy¸¦ ±³Á¤ÇØ¾ß ÇÑ´Ù.									
								DrawItem(&point, pItem);//, DarkBits);
							}
						}
					}

					//------------------------------------------------
					//
					//              Effect Ãâ·Â
					//
					//------------------------------------------------
					//if (sector.IsExistEffect())
					//{				
					//	point = tilePointTemp;				
		//
		//				DrawEffect(&point, sector.GetEffectIterator(), sector.GetEffectSize());
		//			}

					//------------------------------------------------
					// ½ÃÃ¼
					//------------------------------------------------
					// À½!! ²ÙÁöÇÑ ÄÚµåÀÇ Áö¸§±æ.. À½³Ä.. ³ªµµ ¸ô¶ó
					//------------------------------------------------
					/*
					if (pCorpseItem)
					{
						point.x = tilePointTemp.x;
						point.y = tilePointTemp.y;

						// ¹à±â¸¦ °áÁ¤ÇÑ´Ù.
						//DarkBits = (sector.GetLight()==0)?m_DarkBits:0;
						
						// ³ªÁß¿¡ FrameÁ¤º¸¿¡¼­ cx,cy¸¦ ±³Á¤ÇØ¾ß ÇÑ´Ù.									
						DrawItem(&point, pItem);//, DarkBits);
					}
					*/

					//------------------------------------			
					// Ãâ·ÂÇÏ·Á´Â ÁÂÇ¥ ÀÌµ¿
					//------------------------------------
					tilePointTemp.x += TILE_X;
				}
			}	// bPlayerInCasket
			
			//-----------------------------------------------------
			// ÇÑ ÁÙÀÌ ³¡³¯¶§¸¶´Ù 
			// Ãâ·ÂÇØ¾ßÇÒ ±× ÁÙÀÇ ImageObjectµéÀ» Ãâ·ÂÇÑ´Ù.
			//-----------------------------------------------------	
			while (bDrawBackGround && iImageObjectOutput != m_mapImageObject.end())
			{
				MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectOutput).second);

				// Ãâ·Â ½ÃÁ¡ÀÌ y¿Í °°Àº °æ¿ì Ãâ·Â
				if (pImageObject->GetViewpoint() <= y)
				{
					if(pImageObject->GetViewpoint() != 0)
					{
						// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
						point.x = pImageObject->GetPixelX() - m_FirstZonePixel.x;
						point.y = pImageObject->GetPixelY() - m_FirstZonePixel.y;				

						DrawImageObject(&point, pImageObject);

						//char str[80];
						//sprintf(str, "%d", pImageObject->GetViewpoint());
						//m_pSurface->GDI_Text(point.x, point.y, str, 0xFFFFFF);
					}
				}
				// ¾Æ´Ï¸é.. ´Ù¸¥ ÁÙÀÌ´Ù.
				else
				{				
					break;
				}

				iImageObjectOutput ++;
			}

			//------------------------------------------------------
			// ÇÑ ÁÙÀÌ ³¡³¯¶§¸¶´Ù 
			// Ãâ·ÂÇØ¾ßÇÒ ±× ÁÙÀÇ CreatureµéÀ» Ãâ·ÂÇÑ´Ù.
			//------------------------------------------------------
			while (iCreatureOutput != m_mapCreature.end())
			{
				MCreature* const pCreature = (MCreature* const)((*iCreatureOutput).second);

				// 2004, 04, 24 sobeit add start -ghost ÀÏ¶§ 
				if(pCreature->GetCreatureType() == CREATURETYPE_GHOST)
				{
					iCreatureOutput ++;
					continue;
				}
				// 2004, 04, 24 sobeit end start -ghost ÀÏ¶§ 

				// Ãâ·Â ½ÃÁ¡ÀÌ sY1º¸´Ù ÀûÀº °æ¿ì..´Â Ãâ·Â
				if (pCreature->GetY() <= y)
				{
					// playerÀÌ°Å³ª darkness¶û °ü°è¾øÀÌ º¸ÀÌ´Â °æ¿ì
					if (pCreature==g_pPlayer
						|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
					{				
						// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
						point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
						point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
												
						// ¹à±â¸¦ °áÁ¤ÇÑ´Ù.
						//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;

						DrawCreature(&point, pCreature);//, DarkBits);
					}
				}
				// ¾Æ´Ï¸é.. ÇÑ ÁÙ Ãâ·ÂÀÌ ³¡³ª°í Ãâ·ÂÇÒ¶§ Ãâ·ÂµÇ´Â °æ¿ìÀÌ´Ù.
				else
				{
					break;
				}

				iCreatureOutput ++;
			}

			//------------------------------------------------------
			// SectorÀÇ Effect Ãâ·Â
			//------------------------------------------------------
			// Ä³¸¯ÅÍº¸´Ù ´Ê°Ô Ãâ·ÂµÇ¾î¾ß ÇÏ¹Ç·Î...
			//------------------------------------------------------
			if (!bPlayerInCasket)	// [»õ±â¼ú3] °ü ¼Ó¿¡ ÀÖÀ»¶§ Effect´Â ¾È Âï´Â´Ù.
			{			
				tilePointTemp.x = tilePoint.x;
				for (x=sX1; x<=sX2; x++)
				{	
					int darknessCount = g_pPlayer->GetDarknessCount();
					
					// darkness¶û °ü°è¾øÀÌ º¸ÀÌ´Â °æ¿ì. effect´Â +2
					if (darknessCount < 0
						|| max(abs(g_pPlayer->GetX()-x), abs(g_pPlayer->GetY()-y)) <= darknessCount+2)
					//g_pPlayer->ShowInDarkness(x, y))
					{			
						const MSector& sector = m_pZone->GetSector(x,y);

						//------------------------------------------------
						//
						//              Effect Ãâ·Â
						//
						//------------------------------------------------
						if (sector.IsExistEffect())
						{				
							point = tilePointTemp;				

	 						DrawEffect(&point, sector.GetEffectIterator(), sector.GetEffectSize());
						}
					}

					//------------------------------------			
					// Ãâ·ÂÇÏ·Á´Â ÁÂÇ¥ ÀÌµ¿
					//------------------------------------
					tilePointTemp.x += TILE_X;
				}
			}
					
			// ´ÙÀ½ ÁÙ
			tilePointTemp.y += TILE_Y;
		}
	}

	
	//------------------------------------------------------
	// Ãâ·Â ½ÃÁ¡ÀÌ sY2º¸´Ù Å« °æ¿ìÀÇ 
	// CreatureµéÀ» Ãâ·ÂÇÑ´Ù.
	// m_mapCreature¿¡´Â Viewpoint°¡ key°ªÀÌ µÇ¾î¼­ SortµÇ¾î ÀÖ´Ù.
	//------------------------------------------------------
	while (iCreatureOutput != m_mapCreature.end())
	{
		MCreature* const pCreature = (MCreature* const)((*iCreatureOutput).second);

		// 2004, 04, 24 sobeit add start -ghost ÀÏ¶§ 
		if(pCreature->GetCreatureType() == CREATURETYPE_GHOST)
		{
			iCreatureOutput ++;
			continue;
		}
		// 2004, 04, 24 sobeit add end -ghost ÀÏ¶§ 

		// playerÀÌ°Å³ª darkness¶û °ü°è¾øÀÌ º¸ÀÌ´Â °æ¿ì
		if (pCreature==g_pPlayer
			|| g_pPlayer->ShowInDarkness(pCreature->GetX(), pCreature->GetY()))
		{
			// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
			point.x = pCreature->GetPixelX() - m_FirstZonePixel.x;
			point.y = pCreature->GetPixelY() - m_FirstZonePixel.y;
									
			// ¹à±â¸¦ °áÁ¤ÇÑ´Ù.
			//int DarkBits = (m_pZone->GetSector(pCreature->GetX(),pCreature->GetY()).GetLight()==0)?m_DarkBits:0;

			DrawCreature(&point, pCreature);//, DarkBits);		
		}

		iCreatureOutput ++;
	}


	//------------------------------------------------------
	// Ãâ·Â ½ÃÁ¡ÀÌ sY2º¸´Ù Å« °æ¿ìÀÇ 
	// ImageObjectµéÀ» Ãâ·ÂÇÑ´Ù.
	// m_mapImageObject¿¡´Â Viewpoint°¡ key°ªÀÌ µÇ¾î¼­ SortµÇ¾î ÀÖ´Ù.
	//------------------------------------------------------
	while (bDrawBackGround && iImageObjectOutput != m_mapImageObject.end())
	{
		MImageObject* const pImageObject = (MImageObject* const)((*iImageObjectOutput).second);

		// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
		point.x = pImageObject->GetPixelX() - m_FirstZonePixel.x;
		point.y = pImageObject->GetPixelY() - m_FirstZonePixel.y;
									
		DrawImageObject(&point, pImageObject);
		
		iImageObjectOutput ++;
	}

	//------------------------------------------------
	// mouse¼±ÅÃÇÑ °ÍÀÇ ¿Ü°û¼±¸¸ ´Ù½Ã ÇÑ ¹ø Ãâ·ÂÇØÁØ´Ù.
	//------------------------------------------------
	if (m_SOM.IsInit())
	{
		#ifdef OUTPUT_DEBUG_DRAW_PROCESS
			DEBUG_ADD( "m_SOM.Clear()" );
		#endif
		//m_pSurface->BltSpriteOutlineOnly( &m_SOM,  m_SOMOutlineColor );

		// Á¦°Å
		m_SOM.Clear();
	}
	
	//----------------------------------------------------------------
	// player°¡ °ü ¼Ó¿¡ ÀÖ´Â °æ¿ì°¡ ¾Æ´Ï¸é.. [»õ±â¼ú3]
	//----------------------------------------------------------------
	if (!bPlayerInCasket && bDrawBackGround)
	{			
		//----------------------------------------------------------------
		//
		//          Weather - ³¯¾¾ È¿°ú Ãâ·Â
		//
		//----------------------------------------------------------------
		
		#ifdef OUTPUT_DEBUG_DRAW_PROCESS
			DEBUG_ADD( "Start DrawWeather" );
		#endif

		__BEGIN_PROFILE("DrawWeather")

		if (g_pWeather->IsActive())
		{
			// °¡·Î ÁÂÇ¥ º¸Á¤..
			// ÇöÀç PlayerÀÇ ÁÂÇ¥¿Í ºñ±³ÇÑ´Ù.
			//int gapX = ((g_pPlayer->GetX() - g_pWeather->GetStartX()) * TILE_X + g_pPlayer->GetSX()) % SURFACE_WIDTH;
			//int gapY = ((g_pPlayer->GetY() - g_pWeather->GetStartY()) * TILE_Y + g_pPlayer->GetSY()) % SURFACE_HEIGHT;
			int gapX = (m_FirstZonePixel.x - g_pWeather->GetStartX()) % g_GameRect.right;
			int gapY = (m_FirstZonePixel.y - g_pWeather->GetStartY()) % g_GameRect.bottom;

			for (i=0; i<g_pWeather->GetSize(); i++)
			{
				const MAP_EFFECT& MapEffect = (*g_pWeather)[i];

				// ¸ØÃßÁö ¾ÊÀº °ÍÀÌ¸é...
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
				// playerÀÇ light ÁÂÇ¥ ¼³Á¤
				//----------------------------------------------------------------
				int pX = g_pPlayer->GetPixelX() - m_FirstZonePixel.x + 24;
			int pY = g_pPlayer->GetPixelY() - m_FirstZonePixel.y - TILE_Y;
			char sh[255]={0};
			// add by Sonic 2006.9.30 ÐÞÕýÈËÎïÊÓÒ°½¹µã
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
			// ¹ÚÁãÀÎ °æ¿ì ½Ã¾ß +3	
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
					false,	// screenPixelÁÂÇ¥			
					true);	// ¹«Á¶°Ç Ãâ·ÂÇØ¾ßÇÏ´Â ºû		
				
				// ¿ÀÅä¹ÙÀÌ ºÒºû
				ADD_MOTORCYCLE_LIGHT_XY_3D( g_pPlayer, pX, pY, true );		
				
				//---------------------------------------
				// UNLOCK
				//---------------------------------------
				m_pSurface->Unlock();
				
				//---------------------------------------
				// 3D ½Ã¾ßÃ³¸® Ãâ·Â
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
				
				// SurfaceÀÇ Á¤º¸¸¦ ÀúÀåÇØµÐ´Ù.
				//SetSurfaceInfo(&m_SurfaceInfo, m_pSurface->GetDDSD());
			}
			//----------------------------------------------------------------
			// 2D
			//----------------------------------------------------------------
			else
			{
				// ¹ÚÁãÀÎ °æ¿ì ½Ã¾ß +3		
				AddLightFilter2D( pX, 
					pY - g_pPlayer->GetZ(), 
					playerLight,  
					false,	// screenPixelÁÂÇ¥			
					true);	// ¹«Á¶°Ç Ãâ·ÂÇØ¾ßÇÏ´Â ºû
				
				// ¿ÀÅä¹ÙÀÌ ºÒºû
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
		// Unlock »óÅÂÀÌ´Ù..
		//----------------------------------------------------------------		

		//----------------------------------------------------------------
		// Mouse·Î ¼±ÅÃµÈ Ä³¸¯ÅÍÀÌ¸é.. ¹àÀº ±ÛÀÚ Ãâ·Â
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
		// ItemÀÌ¸§µéÀ» Ãâ·ÂÇÏ°Ô ÇÑ´Ù.
		//----------------------------------------------------------------
		if (m_bDrawItemNameList && !g_pPlayer->IsInDarkness())
		{
			__BEGIN_PROFILE("DrawItemNameList")

			// unlock»óÅÂ¿¡¼­ È£Ãâ..
			DrawItemNameList();		

			__END_PROFILE("DrawItemNameList")
		}
	} // bPlayerInCasket
	else 
	{
		// [»õ±â¼ú3]
		m_pSurface->Unlock();
	}

	//----------------------------------------------------------------	
	// Lock
	//----------------------------------------------------------------	
	//m_pSurface->Lock();

	//----------------------------------------------------------------
	// È­¸é¿¡ ³ª¿À´Â ±ÛÀÚ¸¦ Ãâ·ÂÇÑ´Ù.
	//----------------------------------------------------------------
	__BEGIN_PROFILE("DrawTextList")

	DrawTextList();

	__END_PROFILE("DrawTextList")

	//-------------------------------------------------
	// Ãâ·ÂÇÒ string¿¡ ±â¾ïµÈ°É ´Ù ¾ø¾ÖÁØ´Ù.
	//-------------------------------------------------
	ClearTextList();

	//-------------------------------------------------
	// guild mark Ãâ·Â Å×½ºÆ®
	//-------------------------------------------------
//	#ifdef _DEBUG
//
//		if (g_pDXInput->KeyDown(DIK_LCONTROL) && g_pDXInput->KeyDown(DIK_G))
//		{		
//			int guildID = g_pPlayer->GetX()%10;
//
//			//-------------------------------------------------
//			// loadµÇ¾î ÀÖ´ÂÁö º»´Ù.
//			//-------------------------------------------------
//			CSprite* pSprite = g_pGuildMarkManager->GetGuildMark(guildID);
//
//			if (pSprite==NULL)
//			{		
//				//-------------------------------------------------
//				// file¿¡ ÀÖ´ÂÁö º»´Ù.
//				//-------------------------------------------------
//				g_pGuildMarkManager->LoadGuildMark(guildID);
//
//				//-------------------------------------------------
//				// file¿¡¼­ loadµÇ¾ú´ÂÁö ´Ù½Ã Ã¼Å©
//				//-------------------------------------------------
//				pSprite = g_pGuildMarkManager->GetGuildMark(guildID);
//
//				//-------------------------------------------------
//				// file¿¡µµ ¾ø´Â °æ¿ì..
//				// guildMark°ü¸®¼­¹ö?¿¡¼­ ¹Þ¾Æ¿Â´Ù.
//				//-------------------------------------------------
//				if (pSprite==NULL)
//				{
//					// ¿©±â¼­´Â.. ÀÓ½Ã·Î È­ÀÏ¿¡¼­ »ý¼ºÇÑ´Ù.
//					char str[256];
//					sprintf(str, "Data\\Guild\\Guild%d.bmp", guildID);
////					g_pGuildMarkManager->CreateGuildMark( guildID, str );
//				}
//			}
//			else
//			{			
//				// ÀÛÀº°Åµµ °Á ÇÔ Âï¾îº»´Ù.
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
	// Light Ãâ·Â
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
	// Object Ãâ·Â	
	//----------------------------------------------------------------
	// YÁÂÇ¥·Î sort¸¦ ÇØ¼­ Ãâ·ÂÇØ¾ß ÇÑ´Ù.
	//----------------------------------------------------------------
	// list´ë½Å¿¡ priority queue¸¦ »ç¿ëÇÏ´Â°Ô ³´Áö ½Í´Ù! sortÀÚµ¿~~
	// ±×·±µ¥! list¿¡ Ãß°¡µÉ¶§ ¼ø¼­°¡ ÀÖÀ¸´Ï±î... °ü°è°¡ ¾øÀ» °Å °°´Ù!
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
	// ±×¸²ÀÚ Test		
	if (g_pDXInput->KeyDown(DIK_SPACE))
	{
		point.x = g_x;
		point.y = g_y;

		if (CDirect3D::IsHAL())
		{
			sprite = 0;//rand()%m_pImageObjectShadowManager->GetMaxIndex();

			m_pSurface->Unlock();
			// EffectTexture ¼³Á¤
			//CDirect3D::GetDevice()->SetTexture(0, m_EffectTPK[sprite].GetSurface());
			CSpriteSurface* pSurface = m_pImageObjectShadowManager->GetTexture(sprite);		
			CDirect3D::GetDevice()->SetTexture(0, pSurface->GetSurface());
			
			// ÁÂÇ¥ ¼³Á¤
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
			
			// Ãâ·Â
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
	// (Ä¿¼­)
	// ¿òÁ÷ÀÌ´Â ¹æÇâÀÌ °è¼Ó ¹Ù²î°í ÀÖ´Â °æ¿ì --> Ä¿¼­ Ç¥Çö	
	//
	//------------------------------------------------------
	/*
	if (!bPutSelectedSector && g_bLButtonDown)			
	{
		POINT cursorPoint;
		GetCursorPos(&cursorPoint);
		// ±×¸®°í, ÇöÀç PlayerÀÖ´Â À§Ä¡¸¦ ¼±ÅÃÇÏÁö ¾ÊÀº °æ¿ìÀÌ´Ù.
		point = GetSelectedSector(cursorPoint.x, cursorPoint.y);
		if (g_pPlayer->GetX()!=point.x || g_pPlayer->GetY()!=point.y)
		{
			// player°¡ ¿òÁ÷ÀÌ´Â ¹æÇâÀ¸·Î Ä¿¼­ ¸ð¾ç Ç¥½Ã
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
	// Filter¿µ¿ª »ç°¢Çü
	//---------------------------------------
	//rect.left = 410 - 100;
	//rect.right = 410 + 100;
	//rect.top = 270 - 100;
	//rect.bottom = 270 + 100;
	//m_pSurface->DrawRect(&rect, 0xFFFF);

	/*
	// Creature Sprite º¸¿©ÁÖ±â
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
// - PlayerÁÂÇ¥¸¦ Áß½ÉÀ¸·Î ÇØ¼­...
//   ´ÜÁö TileÀ» m_pTileSurface¿¡ ±×¸°´Ù.
//
// - ÇöÀç È­¸éÀÇ ObjectµéÀ» °Ë»öÇØ¼­ Ãâ·Â¿¡ ÀÌ¿ëÇÒ ¼ö ÀÖ°Ô ±â¾ïÇØµÐ´Ù.
//----------------------------------------------------------------------
void	
MTopView::DrawTileSurface()
{
	//-------------------------------------------------
	//
	// Object¸¦ Ãâ·ÂÇÏ±â À§ÇÑ Á¤º¸
	//
	//-------------------------------------------------
	// sprite point
	POINT	tilePoint, point;
	RECT	rect;
	
	//---------------------------------------------------------------
	// Player¸¦ Áß½É¿¡ µÎ±â À§ÇÑ Ã¹¹øÂ° SectorÁÂÇ¥¸¦ ±¸ÇÑ´Ù.
	// È­¸éÀÇ (0,0)ÀÌ ³ªÅ¸³»´Â ZoneÀÇ PixelÁÂÇ¥
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
	// È­¸éÀÇ (0,0)¿¡ Ãâ·ÂµÇ´Â Sector
	//---------------------------------------------------------------		
	POINT	firstSector;
	firstSector = PixelToMap(m_FirstZonePixel.x, m_FirstZonePixel.y);
	m_FirstSector = firstSector;

	//---------------------------------------------------------------
	// TileSurface (0,0)¿¡ Ãâ·ÂµÇ´Â SectorÁÂÇ¥
	//---------------------------------------------------------------
	firstSector.x -= TILESURFACE_SECTOR_EDGE;
	firstSector.y -= TILESURFACE_SECTOR_EDGE;

	//---------------------------------------------------------------
	// TileSurface (0,0)ÀÇ Zone¿¡¼­ÀÇ pixelÁÂÇ¥¸¦ ±â¾ïÇØµÐ´Ù.
	//---------------------------------------------------------------
	m_TileSurfaceFirstSectorX		= firstSector.x;
	m_TileSurfaceFirstSectorY		= firstSector.y;
	
	point = MapToPixel(m_TileSurfaceFirstSectorX, m_TileSurfaceFirstSectorY);
	m_TileSurfaceFirstZonePixelX	= point.x;
	m_TileSurfaceFirstZonePixelY	= point.y;
	

	//----------------------------------------------------------------------
	//
	//                         Tile Ãâ·Â
	//
	//----------------------------------------------------------------------
	//----------------------------------------------------------------------
	// ¹«Á¶°Ç ÃÖÃÊ Sector°¡ TileSurfaceÀÇ (0,0)¿¡ ¿Àµµ·Ï ÇØ¾ßÇÑ´Ù.
	//----------------------------------------------------------------------
	int	sX1 = firstSector.x, 
			sX2 = firstSector.x + g_TILESURFACE_SECTOR_WIDTH,		// sX1 ~ sX2
			sY1 = firstSector.y, 
			sY2 = firstSector.y + g_TILESURFACE_SECTOR_HEIGHT;	// sY1 ~ sY2

	// Ãâ·ÂÇÒ Surface»óÀÇ À§Ä¡	
	tilePoint.x = 0;
	tilePoint.y = 0;
		
	//------------------------------------------------------
	// ZoneÀÇ ¿µ¿ªÀÌ ¾Æ´Ñ °æ¿ì¿¡ Skip...
	//------------------------------------------------------					
	if (sX1 < 0) 
	{			
		// Zone¿Ü°û ºÎºÐÀº °ËÀº»öÀ¸·Î Ä¥ÇÑ´Ù.
		rect.left = tilePoint.x;
		rect.top = 0;	
		
		tilePoint.x += -sX1 * TILE_X;

		rect.right = tilePoint.x;
		rect.bottom = g_TILESURFACE_HEIGHT;				 

		m_pTileSurface->FillRect(&rect, 0);
		
		// CRITICAL FIX: Sync m_TileSurfaceFirstSectorX with adjusted sX1
		m_TileSurfaceFirstSectorX += (-sX1);
		sX1 = 0;	
	}

	if (sX2 > m_pZone->GetWidth())
	{
		sX2 = m_pZone->GetWidth();//-1;

		// Zone¿Ü°û ºÎºÐÀº °ËÀº»öÀ¸·Î Ä¥ÇÑ´Ù.
		rect.left = tilePoint.x + (sX2-sX1)*TILE_X;
		rect.top = 0;					
		rect.right = g_TILESURFACE_WIDTH;
		rect.bottom = g_TILESURFACE_HEIGHT;

		m_pTileSurface->FillRect(&rect, 0);
	}

	if (sY1 < 0)
	{
		// Zone¿Ü°û ºÎºÐÀº °ËÀº»öÀ¸·Î Ä¥ÇÑ´Ù.
		rect.left = tilePoint.x;
		rect.top = tilePoint.y;	
		
		tilePoint.y += -sY1 * TILE_Y;

		rect.right = tilePoint.x + (sX2-sX1)*TILE_X;
		rect.bottom = tilePoint.y;
		m_pTileSurface->FillRect(&rect, 0);
		
		// CRITICAL FIX: Sync m_TileSurfaceFirstSectorY with adjusted sY1
		m_TileSurfaceFirstSectorY += (-sY1);
		sY1 = 0;	
	}

	if (sY2 > m_pZone->GetHeight())
	{
		// Zone¿Ü°û ºÎºÐÀº °ËÀº»öÀ¸·Î Ä¥ÇÑ´Ù.
		rect.left = tilePoint.x;
		rect.top = tilePoint.y;					
		rect.right = tilePoint.x + (sX2-sX1)*TILE_X;
		rect.bottom = g_TILESURFACE_HEIGHT;
		m_pTileSurface->FillRect(&rect, 0);

		sY2 = m_pZone->GetHeight();//-1;
	}			
			
	//------------------------------------------------------
	// ÀÏ´Ü °ËÀº»öÀ¸·Î ÀüÃ¼¸¦ Ä¥ÇÑ´Ù.
	//------------------------------------------------------
	/*
	rect.left = 0;
	rect.top = 0;
	rect.right = TILESURFACE_WIDTH;	
	rect.bottom = TILESURFACE_HEIGHT;

	m_pTileSurface->FillRect(&rect, 0);
	*/

	//------------------------------------------------------
	// °¢ SectorÃâ·Â
	//------------------------------------------------------	
	// Ã¹¹øÂ° ÁÙ
	POINT tilePointTemp;
	tilePointTemp.y = tilePoint.y;

	register int x;
	register int y;

	DEBUG_ADD_FORMAT("[DrawTileSurface] (%d, %d) ~ (%d, %d)", sX1, sX2, sY1, sY2);

	//---------------------------------------
	// LOCK
	//---------------------------------------
	if (!m_pTileSurface->Lock()) return;

	//----------------------------------------------------------------------
	// Use TileRenderer for unified tile rendering (Phase 4 integration)
	//----------------------------------------------------------------------
	if (m_pTileRenderer != NULL && m_pTileRenderer->IsInit())
	{
		// Set the zone provider
		m_zoneTileProvider.SetZone(m_pZone);

		// Draw tiles using TileRenderer
		// Note: DrawTilesNoLock is used because surface is already locked
		m_pTileRenderer->DrawTilesNoLock(
			&m_zoneTileProvider,
			sX1, sY1,
			sX2 - sX1,
			sY2 - sY1,
			tilePoint.x,
			tilePoint.y
		);
	}
	else
	{
		printf("[DrawTileSurface] ERROR: TileRenderer not initialized! Cannot render tiles.\n");
	}

	//---------------------------------------
	// UNLOCK
	//---------------------------------------
	m_pTileSurface->Unlock();

	//----------------------------------------------------------------------
	// NOTE: Tile surface to main screen blit is handled in DrawZone()
	// via the bDrawBackGround condition. Do NOT duplicate here.
	//----------------------------------------------------------------------
}

//----------------------------------------------------------------------
// Draw PartyHP
//----------------------------------------------------------------------
void
MTopView::DrawPartyHP(POINT* pPoint, MCreature* pCreature)
{
	pPoint->x += 10;

	//-----------------------------------------------------
	// HP°è»ê
	//-----------------------------------------------------
	int currentHP		= pCreature->GetHP();
	int maxHP			= pCreature->GetMAX_HP();

	//-----------------------------------------------------
	// ¶¥¼Ó¿¡ ÀÖ´Â ¾Ö´Â HP°¡ ²Ë Âù°ÍÃ³·³ º¸¿©ÁØ´Ù.
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
	// ½ÇÁ¦·Î º¸¿©Áú pixel°è»ê
	//-----------------------------------------------------
	CSprite* pHPBackSprite	= &m_EtcSPK[SPRITEID_PARTY_HP_BACKGROUND];
	CSprite* pHPSprite		= &m_EtcSPK[SPRITEID_PARTY_HP];

	int maxPixels		= pHPBackSprite->GetWidth();
	int currentPixels	= (maxHP==0)? 0 : maxPixels * currentHP / maxHP;

	//-----------------------------------------------------
	// Ãâ·Â
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
		// Unlock »óÅÂ·Î ¸¸µç´Ù. 
		//------------------------------------------------
		BOOL bLock = m_pSurface->IsLock();
		if (!bLock)
		{
			m_pSurface->Lock();
		}

		MScreenEffect::SetScreenBasis( pPoint->x, pPoint->y );
		
		g_pTopView->DrawEffect(pPoint, g_pInventoryEffectManager->GetEffects(), g_pInventoryEffectManager->GetSize());

		//------------------------------------------------
		// ¿ø·¡ lock µÈ »óÅÂ¸é ´Ù½Ã µÇµ¹¸°´Ù.
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
	// ¾à°£ ¿ÞÂÊ¿¡¼­ Ãâ·Â...
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
	//			Font¼³Á¤
	//
	//---------------------------------------------------------	
	FONTID	font;
	//---------------------------------------------------------
	// Vampire¿Í Slayer¿¡ µû¶ó¼­ Font¸¦ ´Ù¸£°Ô ÇØÁØ´Ù.
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
	//		Á¦ÀÏ ±ä stringÀÇ ±æÀÌ¸¦ ¾ò´Â´Ù.
	//
	//---------------------------------------------------------
	int maxLen = 0;
	int numString = g_pClientConfig->MAX_CHATSTRING;
	int maxWidth = 0;
	for (register int i=g_pClientConfig->MAX_CHATSTRING_MINUS_1; i>=0; i--)
	{
		const char *str = pCreature->GetChatString(i);		

		//---------------------------------------------------------
		// ¹º°¡ ÀÖÀ¸¸é.. Ãâ·ÂÇØ¾ßÇÑ´Ù.
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
			// Ãâ·ÂÇÒ°Ô ¾ø´Â °æ¿ì	
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
	//			Ãâ·Â Á¤º¸ ¼³Á¤
	//
	//---------------------------------------------------------	
	int firstY = y2 - numString * ChatFontHeight;

	int x = pPoint->x;
	int y = firstY;



	//---------------------------------------------------------
	// È­¸é ¾È¿¡ ±ÛÀÚ°¡ µé¾î¿Àµµ·Ï..
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
	//  °¢ string Ãâ·Â...
	//---------------------------------------------------------
	int start = g_pClientConfig->MAX_CHATSTRING-numString;
	for (int i=start; i<g_pClientConfig->MAX_CHATSTRING; i++)
	{
		const char *str = pCreature->GetChatString(i);
		
		// ¹º°¡ ÀÖÀ¸¸é.. Ãâ·ÂÇØ¾ßÇÑ´Ù.
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
			// 33À¸·Î ÇÑ°Å´Â.. DWORD time¿¡¼­ Ã¹ bit°¡ ÇÊ¿ä¾øÀ» µí ÇÏ°í
			// Creature Object ID·Î Ä³¸¯ÅÍº°·Î ¼ø¼­¿¡ ¸ÂÃç¼­ Ãâ·ÂÇÏ°í 
			// Ãâ·Â¼ø¼­ i¸¦ Ãß°¡ÇØ¼­ ¼ø¼­´ë·Î Ãâ·ÂµÇ°Ô ÇÑ °ÍÀÌ´Ù.
			//
			// ¿À·¡µÈ°Å < Ä³¸¯ÅÍº°·Î < ÁÙ¼ø¼­
			//---------------------------------------------------------
			QWORD time = timeBase + m_pqDrawText.size();
			pNode->SetTextTime( time );
			
			//---------------------------------------------------------
			// °ËÀº»ö ¹Ú½º (黑色背景框 - 聊天气泡)
			//---------------------------------------------------------
			if (i==start)
			{
				RECT rect = {
					x - 4,
					firstY - 2,
					x2 + 4,
					y2 +2
				};

				pNode->SetBox( rect, m_ColorUIBoxBg );  // RGB565 格式的灰色

				//---------------------------------------------------------
				// Ã¤ÆÃ¿¡ ¿Ü°û ¹Ú½º ±×¸®±â
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
			// yÁÙ ÁÂÇ¥ return
		//	return firstY;
		//}

		// ÇÑÁÙ¾¿ ¾Æ·¡·Î..
		y += ChatFontHeight;
	}

	return firstY;
}

//----------------------------------------------------------------------
// Draw Item : Item Ãâ·ÂÇÏ±â	
//----------------------------------------------------------------------
// pSurfaceÀÇ pPoint¿¡ pItemÀ» Ãâ·ÂÇÑ´Ù. 
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
	// SlayerÀÎ °æ¿ì Darkness ¾ÈÀ» º¼ ¼ö ¾ø´Ù.
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
	// ½ÃÃ¼ÀÎ °æ¿ì´Â Creature¸¦ Ãâ·ÂÇÑ´Ù.
	//---------------------------------------------------------
	if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
	{
		MCorpse* pCorpse = (MCorpse*)pItem;

		MCreature* pCreature = pCorpse->GetCreature();

		POINT orPoint = *pPoint;

		if (pCreature!=NULL)
		{
			//---------------------------------------------------------
			// PC VampireÀÎ °æ¿ì´Â '¸ÕÁö'·Î Ç¥ÇöÇÑ´Ù.
			// 
			// Æ¾¹öÀüÀÎ°æ¿ì´Â ¸ðµÎ '¸ÕÁö'·Î Ç¥ÇöÇÑ´Ù.
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
//				// ÁÂÇ¥ º¸Á¤
//				pointTemp.x = pPoint->x - 5;
//				pointTemp.y = pPoint->y;
//					
//
//				// ¼±ÅÃµÈ °ÍÀÎ °æ¿ì
//				// Creature¸¦ ¼±ÅÃÇÑ °ÍÃ³·³ÇØ¼­ Ãâ·Â.. »ç±â´ç. - -;
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
			// ÀÏ¹Ý Ä³¸¯ÅÍ Á×Àº ¸ð½À..
			//---------------------------------------------------------
//			else
			{
				// ¼±ÅÃµÈ °ÍÀÎ °æ¿ì
				// Creature¸¦ ¼±ÅÃÇÑ °ÍÃ³·³ÇØ¼­ Ãâ·Â.. »ç±â´ç. - -;
				if (m_SelectItemID == pItem->GetID())
				{
					TYPE_OBJECTID temp = m_SelectCreatureID;
					m_SelectCreatureID = pCreature->GetID();

//					if (
//							(
//							pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376	// ¼º¹°
//							|| pCreature->GetCreatureType() == 482	// Å©¸®½º¸¶½º Æ®¸®
//							)
//						||
//							(
//							pCreature->GetActionCount() < pCreature->GetActionCountMax()
//							// Æ¾¹öÀüÀÏ °æ¿ì¿¡´Â ½½·¹ÀÌ¾î½ÃÃ¼ ±×¸²ÀÚ Ãâ·ÂÇØÁà¾ßÇÔ
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
//							pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376	// ¼º¹°
//							|| pCreature->GetCreatureType() == 482	// Å©¸®½º¸¶½º Æ®¸®
//							)
//						||
//							(
//							pCreature->GetActionCount() < pCreature->GetActionCountMax()
//							// Æ¾¹öÀüÀÏ °æ¿ì¿¡´Â ½½·¹ÀÌ¾î½ÃÃ¼ ±×¸²ÀÚ Ãâ·ÂÇØÁà¾ßÇÔ
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
			// Á×Àº Ä³¸¯ÅÍÀÌ¸é ¼±ÅÃ »ç°¢ÇüÀ»
			// ÇÑ Tile Å©±â·Î Á¦ÇÑÇÑ´Ù.
			//------------------------------------------------
			if(!(pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376 || pCreature->GetCreatureType() >= 560 && pCreature->GetCreatureType() <= 563 || pCreature->GetCreatureType() >= 526 && pCreature->GetCreatureType() <= 549 || pCreature->GetCreatureType() == 672 || pCreature->GetCreatureType() == 673 ))
			{
				if(pCreature->GetCreatureType() == 482 || pCreature->GetCreatureType() == 650 )	// Æ®¸®´Â select¾ÈµÇ°Ô
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
	// ¾ÆÁ÷ ¶³¾îÁö°í ÀÖ´Â ÁßÀÌ¸é..
	//---------------------------------------------------------
	// m_ItemDropFPK¿Í m_ItemDropSPK¸¦ ÀÌ¿ë..
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
		// ÀÌ°Å °íÄ¡°í³ª¼­.. MZone::AddItem( ...) ¿¡¼­
		// if (bDropping) µÚÀÇ ÁÖ¼®ºÎºÐÀ» »ì·Á¾ß ÇÑ´Ù..
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

		// Ãâ·Â
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
	// ¹Ù´Ú¿¡ ÀÖ´Â item
	//---------------------------------------------------------
	// m_ItemTileFPK¿Í m_ItemTileSPK¸¦ ÀÌ¿ë
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

		// ÁÂÇ¥ º¸Á¤
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
		// Item ¼±ÅÃ »ç°¢Çü ¿µ¿ª ¼³Á¤
		// ¾ÆÀÌÅÛÀÇ ¼±ÅÃ ¿µ¿ª Å©±â¸¦ Á¦ÇÑÇÑ´Ù.
		//---------------------------------------- 	
		RECT rect;
		rect.left	= max(pointTemp.x, pPoint->x);
		rect.top	= max(pointTemp.y, pPoint->y);
		rect.right	= min(pointTemp.x + pSprite->GetWidth(), pPoint->x + TILE_X);
		rect.bottom = min(pointTemp.y + pSprite->GetHeight(), pPoint->y + TILE_Y);		

		pItem->SetScreenRect( &rect );

		// m_SpritePack¿¡¼­ ÀûÀýÇÑ sprite¸¦ °ñ¶ó¼­ Ãâ·ÂÇØÁØ´Ù.

		//-------------------------------------------------------
		// Mouse°¡ °¡¸®Å°°í ÀÖ´Â Item
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
			// mouse°¡ °¡¸®Å°´Â ItemÀÇ Á¤º¸ Ç¥½Ã
			//-------------------------------------
			/*
			const char* pItemName = pItem->GetName();

			// optionÀÌ ºÙÀº°Å´Â ÆÄ¶õ»ö..
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

				// È­¸é ¾È¿¡ ±ÛÀÚ°¡ µé¾î¿Àµµ·Ï..
				if (x<0) x=0;
				if (y<0) y=0;

				//--------------------------------------------------
				// GetStringWidth¿¡¼­´Â unlockÇØ¾ßÇÑ´Ù.
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


				// °ËÀº»ö ¹Ú½º			
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
			// ÃßÀû ÁßÀÎ ItemÇ¥½Ã
			//-------------------------------------------------------
			//if (g_pPlayer->IsTraceItem() && g_pPlayer->GetTraceID()==pItem->GetID())
			{	
				// RED
			//	m_pSurface->BltSpriteColor(&pointTemp, &m_ItemTileSPK[ Frame.GetSpriteID() ], 0);
			}	
			//-------------------------------------------------------
			// Á¤»ó Ãâ·Â
			//-------------------------------------------------------
			//else 
			{
				//if (CDirect3D::IsHAL() || DarkBits==0)
				//{			
					
				//-------------------------------------------------------
				// ¼³Ä¡µÈ Áö·ÚÀÌ¸é..
				//-------------------------------------------------------
				if (pItem->GetItemClass()==ITEM_CLASS_MINE
					&& ((MMine*)pItem)->IsInstalled())
				{
					// »¡°£»öÀ¸·Î Ãâ·ÂÇÑ´Ù.
					m_pSurface->BltIndexSpriteColor(&pointTemp, pSprite, 0);
				}
				//-------------------------------------------------------	
				// ÀÏ¹ÝÀûÀÎ ¾ÆÀÌÅÛ
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
		// mouse°¡ °¡¸®Å°´Â ItemÀÇ Á¤º¸ Ç¥½Ã
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
	// ItemNameList¿¡ Ãß°¡
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
// Draw ItemShadow : Item ±×¸²ÀÚ Ãâ·ÂÇÏ±â	
//----------------------------------------------------------------------
// pSurfaceÀÇ pPoint¿¡ pItemÀ» Ãâ·ÂÇÑ´Ù.
//----------------------------------------------------------------------
void	
MTopView::DrawItemShadow(POINT* pPoint, MItem* pItem)
{
	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "Start DrawItemShadow" );
	#endif

	__BEGIN_PROFILE("DrawItemShadow")

	//------------------------------------------------
	// SlayerÀÎ °æ¿ì Darkness ¾ÈÀ» º¼ ¼ö ¾ø´Ù.
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
	// ½ÃÃ¼ÀÎ °æ¿ì´Â Creature¸¦ Ãâ·ÂÇÑ´Ù.
	//---------------------------------------------------------
	if (pItem->GetItemClass()==ITEM_CLASS_CORPSE)
	{
		MCorpse* pCorpse = (MCorpse*)pItem;

		MCreature* pCreature = pCorpse->GetCreature();

		if (pCreature!=NULL)
		{
			//---------------------------------------------------------
			// ÀÏ¹Ý Ä³¸¯ÅÍ Á×Àº ¸ð½À..
			//---------------------------------------------------------
			{
				// ¼±ÅÃµÈ °ÍÀÎ °æ¿ì
				// Creature¸¦ ¼±ÅÃÇÑ °ÍÃ³·³ÇØ¼­ Ãâ·Â.. »ç±â´ç. - -;
				if (m_SelectItemID == pItem->GetID())
				{
					TYPE_OBJECTID temp = m_SelectCreatureID;
					m_SelectCreatureID = pCreature->GetID();

					if (
							(
							pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376	// ¼º¹°
							|| pCreature->GetCreatureType() >= 560 && pCreature->GetCreatureType() <= 563	// ¼º¹°
							|| pCreature->GetCreatureType() >= 526 && pCreature->GetCreatureType() <= 549	// ¼º´Ü
							|| pCreature->GetCreatureType() == 482	// Å©¸®½º¸¶½º Æ®¸®
							|| pCreature->GetCreatureType() == 650	// °Ô½ÃÆÇ
							|| pCreature->GetCreatureType() == 672
							|| pCreature->GetCreatureType() == 673
							|| pCreature->GetCreatureType() == 730 // ¿þÀÌ Æ÷ÀÎÆ®1
							|| pCreature->GetCreatureType() == 731 // ¿þÀÌ Æ÷ÀÎÆ®2
							|| pCreature->GetCreatureType() == 732 // ¿þÀÌ Æ÷ÀÎÆ®3
							)
						||
							(
							pCreature->GetActionCount() < pCreature->GetActionCountMax()
							// Æ¾¹öÀüÀÏ °æ¿ì¿¡´Â ½½·¹ÀÌ¾î½ÃÃ¼ ±×¸²ÀÚ Ãâ·ÂÇØÁà¾ßÇÔ
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
							pCreature->GetCreatureType() >= 371 && pCreature->GetCreatureType() <= 376	// ¼º¹°
							|| pCreature->GetCreatureType() >= 560 && pCreature->GetCreatureType() <= 563	// ¼º¹°
							|| pCreature->GetCreatureType() >= 526 && pCreature->GetCreatureType() <= 549	// ¼º¹°
							|| pCreature->GetCreatureType() == 482	// Å©¸®½º¸¶½º Æ®¸®
							|| pCreature->GetCreatureType() == 650  // °Ô½ÃÆÇ
							|| pCreature->GetCreatureType() == 672
							|| pCreature->GetCreatureType() == 673
							|| pCreature->GetCreatureType() == 730 // ¿þÀÌ Æ÷ÀÎÆ®1
							|| pCreature->GetCreatureType() == 731 // ¿þÀÌ Æ÷ÀÎÆ®2
							|| pCreature->GetCreatureType() == 732 // ¿þÀÌ Æ÷ÀÎÆ®3
							)
						||
							(
							pCreature->GetActionCount() < pCreature->GetActionCountMax()
							// Æ¾¹öÀüÀÏ °æ¿ì¿¡´Â ½½·¹ÀÌ¾î½ÃÃ¼ ±×¸²ÀÚ Ãâ·ÂÇØÁà¾ßÇÔ
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
// Draw ImageObject : ±×¸² Ãâ·ÂÇÏ±â	
//----------------------------------------------------------------------
// pSurfaceÀÇ pPoint¿¡ pImageObjectÀ» Ãâ·ÂÇÑ´Ù.
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

	// m_SpritePack¿¡¼­ ÀûÀýÇÑ sprite¸¦ °ñ¶ó¼­ Ãâ·ÂÇØÁØ´Ù.
	// Player¸¦ °¡¸®´Â ±×¸²ÀÏ °æ¿ì ¹ÝÅõ¸í Ã³¸®
	
	//if (pImageObject->GetImageObjectID() == m_BehindImageObjectID)

	//m_pSurface->HLine(410,310, 1, 0xFFFF);
	// (410,310)Àº PlayerÀÇ È­¸é »óÀÇ ¹ßÀ§Ä¡..ÀÌ´Ù..
	// (g_x,g_y)´Â MouseÁÂÇ¥¸¦ ±âÁØÀ¸·Î....

	//------------------------------------------------------
	// 1. ImageObject°¡ Åõ¸íÀÌ µÇ´Â °ÍÀÎ°¡?
	// 2. Viewpoint°¡ PlayerÀ§Ä¡º¸´Ù ¾Æ·¡ÂÊ¿¡ ÀÖ´Â°¡?
	// 3. Sprite°¡ Player¸¦ °¡¸®´Â°¡?
	//------------------------------------------------------
	// 1,2,3ÀÌ ¸ðµÎ ¸¸Á·ÇÑ °æ¿ì.. ¹ÝÅõ¸í Ã³¸®ÇÑ´Ù.
	//------------------------------------------------------
	bool bTrans;

	// playerÁÂÇ¥
	//POINT m_FilterPosition;// = MapToPixel(g_pPlayer->GetX(), g_pPlayer->GetY());
	//m_FilterPosition.x -= m_FirstZonePixel.x-m_PlusPoint.x;
	//m_FilterPosition.y -= m_FirstZonePixel.y-m_PlusPoint.y;

	// ±×¸²ÀÇ ÁÂÇ¥¸¦ ÇöÀç È­¸éÀÇ ÁÂÇ¥¿¡ ¸ÂÃß±â								
	//m_FilterPosition.x = g_pPlayer->GetPixelX() - m_FirstZonePixel.x - filter.GetWidth();
	//m_FilterPosition.y = g_pPlayer->GetPixelY() - m_FirstZonePixel.y - filter.GetHeight();
	//m_FilterPosition.x = 310;	// 410 - 100;
	//m_FilterPosition.y = 116;	// 204 - 100;
	
	//------------------------------------------------------
	// ÀüÃ¼ÀûÀ¸·Î ¾îµÎ¿î °ªÀ» ÁöÁ¤ÇÑ´Ù.
	// ´Ü, ImageObjectÀÇ ÁÂÇ¥(Zone¿¡¼­ÀÇ SectorÁÂÇ¥)°¡ 
	// ¹àÀº TileÀÌ¸é(Light°¡ 0ÀÌ ¾Æ´Ñ °æ¿ì) DarkBits=0À¸·Î ÇÑ´Ù.
	//------------------------------------------------------
	//BYTE DarkBits;
	
	//------------------------------------------------------
	// [ TEST CODE ]
	//------------------------------------------------------
		// ÀÌ°Å´Â ÇÊ¿ä¾ø´Â codeÀÌ´Ù.
		// MapEditor¿¡¼­ Àß ¸¸µé¾îÁÖ¸é µÈ´Ù.
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
	
	// 3D °¡¼ÓÀÌ µÇ´Â °æ¿ì.. ¹«Á¶°Ç ¿ø·¡ »ö±òÀ» ±×´ë·Î Ãâ·ÂÇÑ´Ù.
	if (CDirect3D::IsHAL())
	{
		DarkBits = 0;
	}
	*/

	//----------------------------------------------------				
	// °Ç¹°Àº ¹«Á¶°Ç ¹ÝÅõ¸í
	//----------------------------------------------------				
	BOOL bBlendingShadow = 1;//g_pUserOption->BlendingShadow;
	
	//------------------------------------------------------
	//
	//  ShadowObject Ãâ·Â
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
	//          ShadowAnimationObject Ãâ·Â
	//
	//------------------------------------------------------
	else if (pImageObject->GetObjectType()==MObject::TYPE_SHADOWANIMATIONOBJECT)
	{
	}
	//------------------------------------------------------
	//
	//          InteractionObject Ãâ·Â
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
//				// ÁÂÇ¥ º¸Á¤
//				pPoint->x += Frame.GetCX();
//				pPoint->y += Frame.GetCY();
//
//				CSprite* pSprite = &m_InteractionObjectSPK[ sprite ];		
//				
//				//---------------------------------------- 		
//				// InteractionObjectÀÇ ¼±ÅÃ »ç°¢Çü ¿µ¿ª ¼³Á¤
//				//---------------------------------------- 	
//				RECT rect;
//				rect.left	= pPoint->x;
//				rect.top	= pPoint->y;
//				rect.right	= rect.left + pSprite->GetWidth();
//				rect.bottom = rect.top + pSprite->GetHeight();
//				pInteractionObject->SetScreenRect( &rect );
//
//
//				// ¹ÝÅõ¸í check				
//				bTrans = pImageObject->IsTrans();
//
//				//--------------------------------
//				// ¼±ÅÃµÈ InteractionObjectÀÏ °æ¿ì...
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
//				// Åõ¸í Ã³¸®
//				//--------------------------------
//				else if (bTrans)
//				{
//					// ¹ÝÅõ¸í
//					//m_pSurface->BltSpriteHalf(pPoint, &m_ImageObjectSPK[ sprite ]);
//					// ±×¸²¿¡ ¸ÂÃá filter ÁÂÇ¥ º¸Á¤
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
//				// ±×³É Ãâ·Â
//				//--------------------------------
//				else
//				{
//					//if (m_ImageObjectSPK[ sprite ].IsInit())
//					{
//						///*
//						// PlayerÀÇ ½Ã¾ßÀÇ Å©±â¸¦ ¹Ý¿µÇØ¼­
//						// ½Ã¾ß ¾È¿¡ ÀÖ´Â ImageObjectµé¸¸ º¸ÀÌ°Ô ÇÑ´Ù.
//						//CSprite& spriteRef = m_ImageObjectSPK[ sprite ];
//
//						//int sight = g_pPlayer->GetMaxLightSight();
//						//int pX = sight * TILE_X;
//						//int pY = sight * TILE_Y;
//						//int sX = 410-(pX>>1) - pPoint->x;
//						//int sY = 270-(pY>>1) - pPoint->y;			
//
//						//if (DarkBits==0 //|| m_ImageObjectSPK[ pImageObject->GetSpriteID() ].IsIntersectFilter())
//							// Ãâ·Â ½ÃÀÛÀ§Ä¡(s_X,s_Y)°¡ SpriteÀÇ ³¡Á¡º¸´Ù´Â ÀûÀ» °æ¿ì
//						//	|| (sX < spriteRef.GetWidth() && sY < spriteRef.GetHeight()
//							// FilterÀÇ ³¡Á¡ÀÌ Sprite Ã¹Á¡ º¸´Ù Å« °æ¿ì
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
//				// ¼±ÅÃµÈ InteractionObjectÀÏ °æ¿ì...
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
		//          ImageObject : Sprite Ãâ·Â
		//
		//------------------------------------------------------
		int spriteID = pImageObject->GetSpriteID();
		if (spriteID != SPRITEID_NULL)	
		{			
			//---------------------------------------
			// ID°¡ spriteÀÎ ImageObject¸¦ LoadÇÑ´Ù.
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
//				// Loading¿¡ ¼º°øÇÑ °æ¿ì
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
//				// ½ÇÆÐÇÑ °æ¿ì --> ÀÌ¹Ì LoadingÇÏ°í ÀÖ´Â °æ¿ìÀÌ´Ù.				
//				//--------------------------------------------------
//				/*
//				// 2001.8.20 ÁÖ¼®Ã³¸®
//				else
//				{
//					#ifdef	OUTPUT_DEBUG
//						strcat(str, " ...Fail & Wait Loading");
//	
//						DEBUG_ADD( str );
//					#endif
//
//					// file thread ¼øÀ§¸¦ ³ôÈù´Ù.
//					//SetThreadPriority(g_hFileThread, THREAD_PRIORITY_HIGHEST);
//					MLoadingSPKWorkNode3* pNode = new MLoadingSPKWorkNode3(spriteID, m_ImageObjectSPKI[spriteID]);
//					pNode->SetSPK( &m_ImageObjectSPK, FILE_SPRITE_IMAGEOBJECT );
//					pNode->SetType( 1 );
//					g_pLoadingThread->SetPriority( THREAD_PRIORITY_HIGHEST );
//					g_pLoadingThread->AddFirst( pNode );
//
//					// Thread¿¡¼­ LoadingÀÌ ³¡³¯¶§±îÁö ±â´Ù¸°´Ù.
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
//					// file thread ¼øÀ§¸¦ ³·Ãá´Ù.
//					//SetThreadPriority(g_hFileThread, THREAD_PRIORITY_BELOW_NORMAL);
//					g_pLoadingThread->SetPriority( THREAD_PRIORITY_LOWEST );
//
//				}
//				*/
//			}

			bTrans = pImageObject->IsTrans()
						&& pImageObject->GetViewpoint() >= g_pPlayer->GetY()
						// 2001.9.5 Ãß°¡
						&& pImageObject->IsWallTransPosition(g_pPlayer->GetX(), g_pPlayer->GetY());

						
						//&& m_ImageObjectSPK[ spriteID ].IsColorPixel(
						//	410 - pPoint->x,
						//	310 - pPoint->y);
		
			//--------------------------------
			// Åõ¸í Ã³¸®
			//--------------------------------
			if (bTrans)
			{	
				///*
				//m_pSurface->BltSpriteHalf(pPoint, &m_ImageObjectSPK[ spriteID ]);
			

				// ±×¸²¿¡ ¸ÂÃá filter ÁÂÇ¥ º¸Á¤
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

						// Texture ¼³Á¤
						CSpriteSurface* pSurface = m_pImageObjectTextureManager->GetTexture(spriteID);

						if( pSurface != NULL )
						{
							CDirect3D::GetDevice()->SetTexture(0, pSurface->GetSurface());

							CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);					
							CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_ONE );
							CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );   
							
							// ÁÂÇ¥ ¼³Á¤
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
							// CTRL+A´©¸£¸é ¾Ï°Åµµ ¾È Âï´Â´Ù.
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

					// Texture¸¸µé¾î¼­ ¹ÝÅõ¸í ÇÏ±â... 
					/*
					if (CDirect3D::IsHAL())
					{
						m_pSurface->Unlock();

						// Texture ¼³Á¤
						CSpriteSurface* pSurface = m_pImageObjectTextureManager->GetTexture(spriteID);
						CDirect3D::GetDevice()->SetTexture(0, pSurface->GetSurface());
				
						// ÁÂÇ¥ ¼³Á¤
						m_SpriteVertices[0].sx = pPoint->x;
						m_SpriteVertices[0].sy = pPoint->y;
						m_SpriteVertices[1].sx = pPoint->x,m_pImageObjectTextureManager->GetWidth(spriteID);
						m_SpriteVertices[1].sy = pPoint->y;
						m_SpriteVertices[2].sx = pPoint->x;
						m_SpriteVertices[2].sy = pPoint->y,m_pImageObjectTextureManager->GetHeight(spriteID);
						m_SpriteVertices[3].sx = m_SpriteVertices[1].sx;
						m_SpriteVertices[3].sy = m_SpriteVertices[2].sy;		
						
						// Ãâ·Â
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

						// Texture ¼³Á¤
						CSpriteSurface* pSurface = m_pImageObjectTextureManager->GetTexture(spriteID);

						if( pSurface != NULL )
						{
							CDirect3D::GetDevice()->SetTexture(0, pSurface->GetSurface());
							
							// ÁÂÇ¥ ¼³Á¤
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
			// ±×³É Ãâ·Â
			//--------------------------------
			else
			{					
				//if (m_ImageObjectSPK[ spriteID ].IsInit())
				{
					///*
					// PlayerÀÇ ½Ã¾ßÀÇ Å©±â¸¦ ¹Ý¿µÇØ¼­
					// ½Ã¾ß ¾È¿¡ ÀÖ´Â ImageObjectµé¸¸ º¸ÀÌ°Ô ÇÑ´Ù.
					//CSprite& spriteRef = m_ImageObjectSPK[ spriteID ];

					//int sight = g_pPlayer->GetMaxLightSight();
					//int pX = sight * TILE_X;
					//int pY = sight * TILE_Y;
					//int sX = 410-(pX>>1) - pPoint->x;
					//int sY = 270-(pY>>1) - pPoint->y;			

					//if (DarkBits==0 //|| m_ImageObjectSPK[ spriteID ].IsIntersectFilter())
						// Ãâ·Â ½ÃÀÛÀ§Ä¡(s_X,s_Y)°¡ SpriteÀÇ ³¡Á¡º¸´Ù´Â ÀûÀ» °æ¿ì
					//	|| (sX < spriteRef.GetWidth() && sY < spriteRef.GetHeight()
						// FilterÀÇ ³¡Á¡ÀÌ Sprite Ã¹Á¡ º¸´Ù Å« °æ¿ì
					//	 &&	sX,pX > 0 && sY,pY > 0))
						
					//{
#if defined(OUTPUT_DEBUG) && defined(_DEBUG)
					if (g_pDXInput->KeyDown(DIK_A) && 
						(g_pDXInput->KeyDown(DIK_LCONTROL) || g_pDXInput->KeyDown(DIK_RCONTROL)))
					{
						// CTRL+A´©¸£¸é ¾Ï°Åµµ ¾È Âï´Â´Ù.
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
		//     AnimationObjectÀÎ °æ¿ì¿£ Frameµµ Ãâ·ÂÇØÁà¾ß ÇÑ´Ù.
		//
		//------------------------------------------------------------
		if (pImageObject->IsAnimation())
		{
			MAnimationObject* pAnimationObject = (MAnimationObject*)pImageObject;

			TYPE_SPRITEID sprite;

			POINT pointTemp = *pPoint;

			/*******************±×¸²ÀÚ Ãâ·Â ½ÃÀÛ******************/ 
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
			/*******************±×¸²ÀÚ Ãâ·Â ³¡******************/ 

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
					// ID°¡ spriteÀÎ ImageObject¸¦ LoadÇÑ´Ù.
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
//						// Loading¿¡ ¼º°øÇÑ °æ¿ì
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
//						// ½ÇÆÐÇÑ °æ¿ì --> ÀÌ¹Ì LoadingÇÏ°í ÀÖ´Â °æ¿ìÀÌ´Ù.				
//						//--------------------------------------------------
//						
////						// 2001.8.20 ÁÖ¼®Ã³¸®
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
////							// file thread ¼øÀ§¸¦ ³ôÈù´Ù.
////							//SetThreadPriority(g_hFileThread, THREAD_PRIORITY_HIGHEST);
////							MLoadingSPKWorkNode3* pNode = new MLoadingSPKWorkNode3(sprite, m_ImageObjectSPKI[sprite]);
////							pNode->SetSPK( &m_ImageObjectSPK, FILE_SPRITE_IMAGEOBJECT );
////							pNode->SetType( 1 );
////							g_pLoadingThread->SetPriority( THREAD_PRIORITY_HIGHEST );
////							g_pLoadingThread->AddFirst( pNode );
////
////							// Thread¿¡¼­ LoadingÀÌ ³¡³¯¶§±îÁö ±â´Ù¸°´Ù.
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
////							// file thread ¼øÀ§¸¦ ³·Ãá´Ù.
////							//SetThreadPriority(g_hFileThread, THREAD_PRIORITY_BELOW_NORMAL);
////							g_pLoadingThread->SetPriority( THREAD_PRIORITY_LOWEST );
////
////						}
//						
//					}
					// ÁÂÇ¥ º¸Á¤
					pPoint->x += Frame.GetCX();
					pPoint->y += Frame.GetCY();

					// spriteID°¡ ¾ø´Â °æ¿ì´Â ¹ÝÅõ¸í ´Ù½Ã check				
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
					// Åõ¸í Ã³¸®
					//--------------------------------
					if (bTrans)
					{
						// ¹ÝÅõ¸í
						//m_pSurface->BltSpriteHalf(pPoint, &m_ImageObjectSPK[ sprite ]);
						// ±×¸²¿¡ ¸ÂÃá filter ÁÂÇ¥ º¸Á¤
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
					// ±×³É Ãâ·Â
					//--------------------------------
					else
					{
						///*
						// PlayerÀÇ ½Ã¾ßÀÇ Å©±â¸¦ ¹Ý¿µÇØ¼­
						// ½Ã¾ß ¾È¿¡ ÀÖ´Â ImageObjectµé¸¸ º¸ÀÌ°Ô ÇÑ´Ù.
						//if (m_ImageObjectSPK[ sprite ].IsInit())
						{
							//CSprite& spriteRef = m_ImageObjectSPK[ sprite ];

							//int sight = g_pPlayer->GetMaxLightSight();
							//int pX = sight * TILE_X;
							//int pY = sight * TILE_Y;
							//int sX = 410-(pX>>1) - pPoint->x;
							//int sY = 270-(pY>>1) - pPoint->y;			

							//if (DarkBits==0 //|| m_ImageObjectSPK[ spriteID ].IsIntersectFilter())
								// Ãâ·Â ½ÃÀÛÀ§Ä¡(s_X,s_Y)°¡ SpriteÀÇ ³¡Á¡º¸´Ù´Â ÀûÀ» °æ¿ì
							//	|| (sX < spriteRef.GetWidth() && sY < spriteRef.GetHeight()
								// FilterÀÇ ³¡Á¡ÀÌ Sprite Ã¹Á¡ º¸´Ù Å« °æ¿ì
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
						
						// ÁÂÇ¥ º¸Á¤
						pPoint->x += Frame.GetCX();
						pPoint->y += Frame.GetCY();
						
						DRAW_ALPHASPRITEPAL(pPoint, sprite, m_EffectAlphaSPK, m_EffectAlphaPPK[fid])//, m_EffectAlphaSPKI, m_EffectAlphaSPKFile)					
								
						//-------------------------------------------------------
						// H/W °¡¼ÓÀÌ µÇ´Â °æ¿ìÀÌ¸é...
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
//								// ¿ø·¡´ë·Î
//							CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);					
//							CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//							CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
//							#endif
							
							
							//------------------------------------------------
							// LightFilter¸¦ Ãß°¡ÇÑ´Ù.
							//------------------------------------------------
							AddLightFilter3D( pPoint->x + 24, 
								pPoint->y + 24, 
								Frame.GetLight(),			// ºûÀÇ ¹à±â
								false);		// false = screenÁÂÇ¥
							
						}
						//-------------------------------------------------------
						// 2D Ãâ·Â
						//-------------------------------------------------------
						else
						{
//							DRAW_ALPHASPRITEPAL(pPoint, sprite, m_EffectAlphaSPK, m_EffectAlphaPPK[fid])//, m_EffectAlphaSPKI, m_EffectAlphaSPKFile)					
								
								AddLightFilter2D( pPoint->x + 24, 
								pPoint->y + 24, 
								Frame.GetLight(),			// ºûÀÇ ¹à±â
								false);		// false = screenÁÂÇ¥
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
							// Frame ÁÂÇ¥ º¸Á¤
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
							// H/W °¡¼ÓÀÌ µÇ´Â °æ¿ìÀÌ¸é...
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
//									// ¿ø·¡´ë·Î
//									CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//								CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
								
								//------------------------------------------------
								// LightFilter¸¦ Ãß°¡ÇÑ´Ù.
								//------------------------------------------------
								AddLightFilter3D( pPoint->x + 24, 
									pPoint->y + 24, 
									Frame.GetLight(),			// ºûÀÇ ¹à±â
									false);		// false = screenÁÂÇ¥
								
							}
							//-------------------------------------------------------
							// H/W °¡¼ÓÀÌ ¾ÈµÇ´Â °æ¿ìÀÌ¸é...
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
									
									AddLightFilter2D( pPoint->x + 24, 
									pPoint->y + 24, 
									Frame.GetLight(),			// ºûÀÇ ¹à±â
									false);		// false = screenÁÂÇ¥
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
	// ImageObject ID Ãâ·Â
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
// 3DÀÌ°í lockµÅ ÀÖÀ¸¸é UnlockÀ¸·Î...
// 3D¾Æ´Ï°í lock¾È µÅ ÀÖÀ¸¸é lockÀ¸·Î..
//
// return°ªÀº ¿ø·¡ Lock»óÅÂ
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
			// 3DÀÌ°í lockµÅ ÀÖÀ¸¸é UnlockÀ¸·Î...
			m_pSurface->Unlock();
		}
	}
	else
	{	
		if (!bLock)
		{
			// 3D¾Æ´Ï°í lock¾È µÅ ÀÖÀ¸¸é lockÀ¸·Î..
			m_pSurface->Lock();
		}
	}

	return bLock;
}
*/

//----------------------------------------------------------------
// SurfaceLock
//----------------------------------------------------------------
// return°ªÀº ¿ø·¡ÀÇ lock»óÅÂ
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
// ¿ø·¡ÀÇ Lock»óÅÂ·Î µ¹·ÁÁØ´Ù.
//----------------------------------------------------------------
/*
void
MTopView::SurfaceLockRestore( BOOL bOldLock )
{
	if (bOldLock)
	{
		if (!m_pSurface->IsLock())
		{
			// ¿ø·¡ LockÀÌ¾ú´Âµ¥ LockÀÌ ¾Æ´Ï¸é
			m_pSurface->Lock();
		}
	}
	else
	{
		if (m_pSurface->IsLock())
		{
			// ¿ø·¡ LockÀÌ ¾Æ´Ï¾ú´Âµ¥ LockÀÌ¸é..
			m_pSurface->Unlock();
		}
	}
}
*/

//----------------------------------------------------------------
// Effect Ãâ·Â
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
		// Tile´ÜÀ§ÀÇ Ãâ·Â ÁÂÇ¥
		//------------------------------------------------------------------
		//if (pEffect->GetEffectType()==MEffect::EFFECT_SECTOR)
		//{			
		//	point = *pPoint;
		//}
		//------------------------------------------------------------------
		// Screen Ãâ·Â ÁÂÇ¥
		//------------------------------------------------------------------
		//else 
		if (pEffect->GetEffectType()==MEffect::EFFECT_SCREEN)
		{			
			MScreenEffect* pScreenEffect = (MScreenEffect*)pEffect;
			//MScreenEffect::SetScreenBasis( pPoint->x, pPoint->y );	// ÇÑ¹ø¸¸ ÇØÁÖ¸é µÇ´Âµ¥.. À½³Ä.
			point.x = pScreenEffect->GetScreenX();
			point.y = pScreenEffect->GetScreenY();
		}
		//------------------------------------------------------------------
		// Pixel´ÜÀ§ÀÇ Ãâ·Â ÁÂÇ¥
		//------------------------------------------------------------------
		else
		{
			//MMovingEffect* pMovingEffect = (MMovingEffect* const)pEffect;
			
			// Pixel ´ÜÀ§ÀÇ ÁÂÇ¥¸¦ È­¸éÀÇ ÁÂÇ¥·Î ¹Ù²ãÁØ´Ù.
			point.x = pEffect->GetPixelX() - m_FirstZonePixel.x;
			point.y = pEffect->GetPixelY() - pEffect->GetPixelZ() - m_FirstZonePixel.y;
		}

		//------------------------------------------------------------------
		// Ãâ·Â
		//------------------------------------------------------------------
		DrawEffect( &point, pEffect, pEffect->IsSelectable() );		

		// ´ÙÀ½ Effect
		iEffect++;
	}	
	
	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "End DrawEffect" );
	#endif

	
}


//----------------------------------------------------------------
// Draw GroundEffect
//----------------------------------------------------------------
// Tile¿¡ ¹Ù·Î ºÙ¾î ÀÖ´Â GroundEffect¸¦ Ãâ·Â
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
		// Pixel ´ÜÀ§ÀÇ ÁÂÇ¥¸¦ È­¸éÀÇ ÁÂÇ¥·Î ¹Ù²ãÁØ´Ù.
		// Áß½ÉÁ¡
		point.x = pEffect->GetPixelX() - m_FirstZonePixel.x;
		point.y = pEffect->GetPixelY() - pEffect->GetPixelZ() - m_FirstZonePixel.y;			
		
		pointTemp = point;

		//------------------------------------------------------------------
		// Ãâ·Â
		//------------------------------------------------------------------
		DrawEffect( &pointTemp, pEffect, pEffect->IsSelectable());
		
		//------------------------------------------------------------------
		// ¼±ÅÃµÇ´Â EffectÀÎ °æ¿ì
		//------------------------------------------------------------------
		if (pEffect->IsSelectable()
			&& m_SelectEffectID==pEffect->GetID())
		{
			MEffectTarget* pEffectTarget = pEffect->GetEffectTarget();

			if (pEffectTarget!=NULL)
			{
				//------------------------------------------------------------------
				// PortalÀÎ °æ¿ì
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
		// ´ÙÀ½ Effect
		iEffect++;
	}	

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "End DrawGroundEffect" );
	#endif

	
}

//----------------------------------------------------------------
// Draw AttachEffect
//----------------------------------------------------------------
// Effect Ãâ·Â
// [ÇÏµåÄÚµùÀÇ Áø¼ö] - -;;
// type - 0 - ¹«Á¶°Ç Ãâ·Â
//        1 - fore ground¸¸ Ãâ·Â
//        2 - back ground¸¸ Ãâ·Â
//----------------------------------------------------------------
// ÀÏ´Ü..
// PairFrameÀº AttachEffect¿¡¼­¸¸ Ãâ·ÂÇÑ´Ù.. 
// ActionFrameµµ AttachEffect¿¡¼­¸¸ Ãâ·ÂÇÑ´Ù..
// NormalEffect¿¡´Â ActionFrameÀ» ¹«½ÃÇß´Ù. ¾ÆÁ÷Àº ÂïÀ»ÀÏµµ ¾ø°í ±ÍÂú°í ÇØ¼­ --;; 
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
		// Sprite·Î Ç¥ÇöÇÏ´Â EffectÀÎ °æ¿ì¸¸ Ãâ·ÂÇÑ´Ù.
		// Attach°¡ ¾Æ´Ï°Å³ª.. AttachÀÌ´õ¶óµµ Sprite·Î Ãâ·ÂÇÏ´Â °æ¿ì
		//----------------------------------------------------------------		
		if (pEffect->GetEffectType()!=MEffect::EFFECT_ATTACH ||
			pEffect->GetEffectType()==MEffect::EFFECT_ATTACH && ((MAttachEffect*)pEffect)->IsEffectSprite()
			
		
			)
		{
			int direction = pEffect->GetDirection();
			int frameID = pEffect->GetFrameID();
			int frame = pEffect->GetFrame();
			BYTE bltType = pEffect->GetBltType();

			// 2004, 9, 14, sobeit add start - ÀÎ½ºÅç ÅÍ·¿ÀÏ¶§ ÀÌÆåÆ® ¾Èº¸¿©ÁÜ
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
			// 2004, 9, 14, sobeit add end - ÀÎ½ºÅç ÅÍ·¿ÀÏ¶§ ÀÌÆåÆ® ¾Èº¸¿©ÁÜ
//			if( pEffect->GetBltType() == BLT_EFFECT &&
//				frameID >= EFFECTSPRITETYPE_PROTECTION_FROM_BLOOD_ING &&
//				frameID <= EFFECTSPRITETYPE_PROTECTION_FROM_CURSE_ING			
//				)
//			{
//				// ÇØ´ç ÀÌÆåÆ®´Â ³ª ¶Ç´Â ¿ì¸® ÆÄÆ¼¿ø¸¸ º¸¿©¾ß ÇÑ´Ù.
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

					// FIX: Use the saved table index from effect creation time
					// instead of recalculating from FrameID using macro
					int est;
					if (pEffect->GetEffectType() == MEffect::EFFECT_ATTACH) {
						// MAttachEffect saves the table index in m_EffectSpriteType
						est = ((MAttachEffect*)pEffect)->GetEffectSpriteType();
					} else {
						// For other effect types, fall back to macro (may need fixing too)
						est = GET_EFFECTSPRITETYPE_NORMAL(frameID);
					}

					// Bounds check
					if (est < 0 || est >= g_pEffectSpriteTypeTable->GetSize()) {
						LOG_ERROR("[Effect BLT_NORMAL] est=%d out of bounds (tableSize=%d), frameID=%d, frame=%d, dir=%d",
						         est, g_pEffectSpriteTypeTable->GetSize(), frameID, frame, direction);
						break;  // Skip this effect to avoid crash
					}

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
					// FIX: Use the saved table index from effect creation time
					// instead of recalculating from FrameID using macro
					int est;
					if (pEffect->GetEffectType() == MEffect::EFFECT_ATTACH) {
						// MAttachEffect saves the table index in m_EffectSpriteType
						est = ((MAttachEffect*)pEffect)->GetEffectSpriteType();
					} else {
						// For other effect types, fall back to macro (may need fixing too)
						est = GET_EFFECTSPRITETYPE_EFFECT(frameID);
					}

					// Bounds check
					if (g_pEffectSpriteTypeTable != NULL) {
						int tableSize = g_pEffectSpriteTypeTable->GetSize();
						if (est < 0 || est >= tableSize) {
							LOG_ERROR("[Effect BLT_EFFECT] est=%d out of bounds (tableSize=%d), frameID=%d, frame=%d, dir=%d",
							         est, tableSize, frameID, frame, direction);
							break;
						}
					}

					// action¿¡ ¸Â´Â effectFrameID¸¦ °ñ¶óÁÖ´Â °Å´ç.
					int aest = GET_ACTION_EFFECTSPRITETYPE(est);
					const bool bBack = (*g_pEffectSpriteTypeTable)[est].bPairFrameBack;

					// FIX: sest should use the same table index as est
					int sest = est;

					if (aest!=FRAMEID_NULL)
					{
						// FRAMEID_NULLÀÎ°É Ã¼Å©ÇÑ´Ù. min( , ) -_-;
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

							// FIX: Don't recalculate sest from actionFrameID
							// sest should remain as the saved table index (est)
							// sest = GET_EFFECTSPRITETYPE_EFFECT( actionFrameID );
						}
					}

					// Áö±ÝÀº ÇÏµåÄÚµù µÇ¾î ÀÖÁö¸¸, µû·Î ÀÌÆåÆ® ½ºÇÁ¶óÀÌÆ®Å¸ÀÔ°ú µ¿ÀÛ ÇÁ·¹ÀÓ°£ÀÇ ÇÁ·¹ÀÓ µ¿±âÈ­
					// °ü·Ã ±â´ÉÀÌ Ãß°¡µÇ¸é, ÇÏµåÄÚµùÀ» ¸·À» ¼ö ÀÖÀ» °ÍÀÌ´Ù. 
					// µ¿±âÈ­ ±â´ÉÀÌ Ãß°¡µÈ´Ù¸é, Ä³¸¯ÅÍ¿¡ ÇØ´ç ÀÌÆåÆ®°¡ ºÙ´Â µ¿ÀÛÀ» ¼¼ÆÃÇÒ ¼ö ÀÖ¾î¾ß °ÚÁö..
					
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
							// ´Ù¸¥ µ¿ÀÛÀÌ¸é Àß ¾Ë¾Æ¼­ ³Ñ±âÀÚ.
							iEffect++;
							continue;
						}
					}
					
					EFFECTFRAME_ARRAY &EFA = m_EffectAlphaFPK[frameID][direction];
					
					frame = min( frame, EFA.GetSize()-1 );

					CEffectFrame &Frame = EFA[frame];

					bool bFrameBackground = Frame.IsBackground();
					AFFECT_ORBIT_EFFECT_BACKGROUND(pEffect, bFrameBackground);

					// ¾ðÁ¦ Ãâ·ÂµÇ´Â°ÇÁö Ã¼Å©ÇÔ ÇØÁØ´Ù.
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

							// ¾ðÁ¦ Ãâ·ÂµÇ´Â°ÇÁö Ã¼Å©ÇÔ ÇØÁØ´Ù.
							if (type==0 
								|| type==1 && !bFrameBackground
								|| type==2 && bFrameBackground)
							{
								int sprite = Frame.GetSpriteID();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetSpriteID();

								if (sprite!=SPRITEID_NULL)
								{
									// Frame ÁÂÇ¥ º¸Á¤
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
									// H/W °¡¼ÓÀÌ µÇ´Â °æ¿ìÀÌ¸é...
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
//											// ¿ø·¡´ë·Î
//											CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);					
//											CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//											CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
//										#endif
							

										//------------------------------------------------
										// LightFilter¸¦ Ãß°¡ÇÑ´Ù.
										//------------------------------------------------
										AddLightFilter3D( pPoint->x + 24, 
														pPoint->y + 24, 
														Frame.GetLight(),			// ºûÀÇ ¹à±â
														false);		// false = screenÁÂÇ¥
									
									}
									//-------------------------------------------------------
									// H/W °¡¼ÓÀÌ ¾ÈµÇ´Â °æ¿ìÀÌ¸é...
									//-------------------------------------------------------
									else
									{
//										DRAW_ALPHASPRITEPAL(&pointTemp, 
//															sprite, 
//															m_EffectAlphaSPK, 
//															m_EffectAlphaPPK[pairFrameID])
////															m_EffectAlphaSPKI, 
////															m_EffectAlphaSPKFile)

										AddLightFilter2D( pPoint->x + 24, 
													pPoint->y + 24, 
													Frame.GetLight(),			// ºûÀÇ ¹à±â
													false);		// false = screenÁÂÇ¥
									}
								}
							}

							iID++;
						}						
					}

					// DEBUG: Log condition result before checking
					bool conditionMet = (type==0 || type==1 && !bFrameBackground || type==2 && bFrameBackground);
					if (conditionMet)
					{
						int sprite = Frame.GetSpriteID();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetSpriteID();

						if (sprite!=SPRITEID_NULL)
						{
							// Frame ÁÂÇ¥ º¸Á¤
							pointTemp = *pPoint;
							pointTemp.x += Frame.GetCX();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetCX();
							pointTemp.y += Frame.GetCY();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetCY();

							AFFECT_ORBIT_EFFECT_POSITION( pEffect, pointTemp )

							// Call the actual draw function (expanded from macro for logging)
							{
								CAlphaSpritePal* pSprite = &m_EffectAlphaSPK[sprite];
								m_pSurface->BltAlphaSpritePal(&pointTemp, pSprite, m_EffectAlphaPPK[frameID]);
							}
//													m_EffectAlphaSPKI, 
//													m_EffectAlphaSPKFile)
							//-------------------------------------------------------
							// H/W °¡¼ÓÀÌ µÇ´Â °æ¿ìÀÌ¸é...
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
//									// ¿ø·¡´ë·Î
//									CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);					
//									CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//									CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
//								#endif
					

								//------------------------------------------------
								// LightFilter¸¦ Ãß°¡ÇÑ´Ù.
								//------------------------------------------------
								AddLightFilter3D( pPoint->x + 24, 
												pPoint->y + 24, 
												Frame.GetLight(),			// ºûÀÇ ¹à±â
												false);		// false = screenÁÂÇ¥
							
							}
							//-------------------------------------------------------
							// H/W °¡¼ÓÀÌ ¾ÈµÇ´Â °æ¿ìÀÌ¸é...
							//-------------------------------------------------------
							else
							{
								DRAW_ALPHASPRITEPAL(&pointTemp, 
													sprite, 
													m_EffectAlphaSPK, 
													m_EffectAlphaPPK[frameID])
//													m_EffectAlphaSPKI, 
//													m_EffectAlphaSPKFile)

								AddLightFilter2D( pPoint->x + 24, 
											pPoint->y + 24, 
											Frame.GetLight(),			// ºûÀÇ ¹à±â
											false);		// false = screenÁÂÇ¥
							}
						}
					}

					//--------------------------------------------------------
					// Pair Frame Ãâ·Â
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

							// ¾ðÁ¦ Ãâ·ÂµÇ´Â°ÇÁö Ã¼Å©ÇÔ ÇØÁØ´Ù.
							if (type==0 
								|| type==1 && !bFrameBackground
								|| type==2 && bFrameBackground)
							{
								int sprite = Frame.GetSpriteID();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetSpriteID();

								if (sprite!=SPRITEID_NULL)
								{
									// Frame ÁÂÇ¥ º¸Á¤
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
									// H/W °¡¼ÓÀÌ µÇ´Â °æ¿ìÀÌ¸é...
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
//											// ¿ø·¡´ë·Î
//											CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);					
//											CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//											CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
//										#endif
							

										//------------------------------------------------
										// LightFilter¸¦ Ãß°¡ÇÑ´Ù.
										//------------------------------------------------
										AddLightFilter3D( pPoint->x + 24, 
														pPoint->y + 24, 
														Frame.GetLight(),			// ºûÀÇ ¹à±â
														false);		// false = screenÁÂÇ¥
									
									}
									//-------------------------------------------------------
									// H/W °¡¼ÓÀÌ ¾ÈµÇ´Â °æ¿ìÀÌ¸é...
									//-------------------------------------------------------
									else
									{
//										DRAW_ALPHASPRITEPAL(&pointTemp, 
//															sprite, 
//															m_EffectAlphaSPK, 
//															m_EffectAlphaPPK[pairFrameID])
////															m_EffectAlphaSPKI, 
////															m_EffectAlphaSPKFile)

										AddLightFilter2D( pPoint->x + 24, 
													pPoint->y + 24, 
													Frame.GetLight(),			// ºûÀÇ ¹à±â
													false);		// false = screenÁÂÇ¥
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
					// FIX: Use the saved table index from effect creation time
					// instead of recalculating from FrameID using macro
					int est;
					if (pEffect->GetEffectType() == MEffect::EFFECT_ATTACH) {
						// MAttachEffect saves the table index in m_EffectSpriteType
						est = ((MAttachEffect*)pEffect)->GetEffectSpriteType();
					} else {
						// For other effect types, fall back to macro (may need fixing too)
						est = GET_EFFECTSPRITETYPE_SCREEN(frameID);
					}

					// Bounds check
					if (g_pEffectSpriteTypeTable != NULL) {
						int tableSize = g_pEffectSpriteTypeTable->GetSize();
						if (est < 0 || est >= tableSize) {
							LOG_ERROR("[Effect BLT_SCREEN] est=%d out of bounds (tableSize=%d), frameID=%d, frame=%d, dir=%d",
							         est, tableSize, frameID, frame, direction);
							break;
						}
					}

					bool bBack = (*g_pEffectSpriteTypeTable)[est].bPairFrameBack;

					// action¿¡ ¸Â´Â effectFrameID¸¦ °ñ¶óÁÖ´Â °Å´ç.
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

					// ¾ðÁ¦ Ãâ·ÂµÇ´Â°ÇÁö Ã¼Å©ÇÔ ÇØÁØ´Ù.
					if (type==0 
						|| type==1 && !bFrameBackground
						|| type==2 && bFrameBackground)
					{
						int sprite = Frame.GetSpriteID();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetSpriteID();

						if (sprite!=SPRITEID_NULL)
						{
							// Frame ÁÂÇ¥ º¸Á¤
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
							// H/W °¡¼ÓÀÌ µÇ´Â °æ¿ìÀÌ¸é...
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
//								// ¿ø·¡´ë·Î
//								CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//								CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
					
								//------------------------------------------------
								// LightFilter¸¦ Ãß°¡ÇÑ´Ù.
								//------------------------------------------------
								AddLightFilter3D( pPoint->x + 24, 
												pPoint->y + 24, 
												Frame.GetLight(),			// ºûÀÇ ¹à±â
												false);		// false = screenÁÂÇ¥
							
							}
							//-------------------------------------------------------
							// H/W °¡¼ÓÀÌ ¾ÈµÇ´Â °æ¿ìÀÌ¸é...
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

								AddLightFilter2D( pPoint->x + 24, 
											pPoint->y + 24, 
											Frame.GetLight(),			// ºûÀÇ ¹à±â
											false);		// false = screenÁÂÇ¥
							}
						}
					}

					//--------------------------------------------------------
					// Pair Frame Ãâ·Â
					//--------------------------------------------------------
					// FIX: est already contains the saved table index from above
					// No need to recalculate with GET_EFFECTSPRITETYPE_SCREEN(frameID)
					// est = GET_EFFECTSPRITETYPE_SCREEN( frameID );

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

							// ¾ðÁ¦ Ãâ·ÂµÇ´Â°ÇÁö Ã¼Å©ÇÔ ÇØÁØ´Ù.
							if (type==0 
								|| type==1 && !bFrameBackground
								|| type==2 && bFrameBackground)
							{
								int sprite = Frame.GetSpriteID();	//m_EffectAlphaFPK[(*iEffect)->GetFrameID()][direction][(*iEffect)->GetFrame()].GetSpriteID();

								if (sprite!=SPRITEID_NULL)
								{
									// Frame ÁÂÇ¥ º¸Á¤
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
									// H/W °¡¼ÓÀÌ µÇ´Â °æ¿ìÀÌ¸é...
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
//										// ¿ø·¡´ë·Î
//										CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//										CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
							
										//------------------------------------------------
										// LightFilter¸¦ Ãß°¡ÇÑ´Ù.
										//------------------------------------------------
										AddLightFilter3D( pPoint->x + 24, 
														pPoint->y + 24, 
														Frame.GetLight(),			// ºûÀÇ ¹à±â
														false);		// false = screenÁÂÇ¥
									
									}
									//-------------------------------------------------------
									// H/W °¡¼ÓÀÌ ¾ÈµÇ´Â °æ¿ìÀÌ¸é...
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

										AddLightFilter2D( pPoint->x + 24, 
													pPoint->y + 24, 
													Frame.GetLight(),			// ºûÀÇ ¹à±â
													false);		// false = screenÁÂÇ¥
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

		// ´ÙÀ½ Effect
		iEffect++;
	}	

	#ifdef OUTPUT_DEBUG_DRAW_PROCESS
		DEBUG_ADD( "End DrawAttachEffect" );
	#endif

	
}

//----------------------------------------------------------------
// (x,y)¿¡ m_pZoneÀÇ MinimapÀ» ±×¸°´Ù.
//----------------------------------------------------------------
// Test Function
//----------------------------------------------------------------
void
MTopView::DrawMinimap(int x, int y, BYTE scale)
{
	
	//------------------------------------------------
	// vampireÀÎ °æ¿ì´Â Ãâ·Â¾ÈÇÑ´Ù.
	//------------------------------------------------
	//if (g_pPlayer->IsVampire())
	//{
	//	return;
	//}

	//------------------------------------------------
	// 3D °¡¼Ó
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
//			// Texture ¼³Á¤
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
//			// PlayerÀ§Ä¡¿¡ Á¡Âï±â
//			//------------------------------------------------
////			if (!m_pSurface->Lock())
////				return;
////
////			POINT point = { x, y };
////
////			WORD*	pSurface	= (WORD*)m_pSurface->GetSurfacePointer();
////			long	lPitch		= m_pSurface->GetSurfacePitch();
////
////			// PlayerÀÇ Minimap»óÀÇ ÁÂÇ¥
////			point.x = x + g_pPlayer->GetX()*m_SectorToMinimapWidth;
////			point.y = y + g_pPlayer->GetY()*m_SectorToMinimapHeight;
////
////			// Á¡ Âï±â
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
//			// PlayerÀ§Ä¡¿¡ Á¡Âï±â
//			//------------------------------------------------	
////			WORD*	pSurface	= (WORD*)m_pSurface->GetSurfacePointer();
////			long	lPitch		= m_pSurface->GetSurfacePitch();
////
////			// PlayerÀÇ Minimap»óÀÇ ÁÂÇ¥
////			point.x = x + g_pPlayer->GetX()*m_SectorToMinimapWidth;
////			point.y = y + g_pPlayer->GetY()*m_SectorToMinimapHeight;
////
////			// Á¡ Âï±â
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
	// ±×¸² Ãâ·Â
	m_pSurface->Lock();

	POINT point = { x, y };

	// Minimap Ãâ·Â
	m_pSurface->BltSprite( &point, m_pMinimapSPR );

	int width = m_pMinimapSPR->GetWidth();
	int height = m_pMinimapSPR->GetHeight();

	WORD*	pSurface	= (WORD*)m_pSurface->GetSurfacePointer();
	long	lPitch		= m_pSurface->GetSurfacePitch();

	// PlayerÀÇ Minimap»óÀÇ ÁÂÇ¥
	point.x = x + g_pPlayer->GetX()*m_SectorToMinimapWidth;
	point.y = y + g_pPlayer->GetY()*m_SectorToMinimapHeight;

	// Á¡ Âï±â
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
			// playerÀ§Ä¡
			if (pX==j && pY==i)
			{
				color = 0xFFFF;
			}			
			else
			{				
				// tile»ö±òº°·Î..
				
				//switch (m_pZone->GetSector(j,i).GetSpriteID())
				//{
				//	case 0 :	color = 0x07E0;	break;	// G
				//	case 1 :	color = 0xC2E5;	break;	// ?
				//	case 2 :	color = 0x001F;	break;	// B
				//	default :	color = 0x18E3;	break;	// ,black
				//}
				
				 const MSector &sector = m_pZone->GetSector(j,i);
			
				// ±×¸²
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
			
			// ´ÙÀ½ Á¡
			lpSurfaceTemp = (WORD*)((BYTE*)lpSurfaceTemp + scale*4);
		}

		// ´ÙÀ½ ÁÙ
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
		MAX_GEAR¿¡ -10 ÇÑ°Ç core zap+bloodbible °¹¼ö ¸¸Å­ »«°Å..^^;
	*/
	// ºÎ¼­Áú·Á´Â itemÃâ·Â
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
	// ºÎ¼­Áú·Á´Â itemÀÌ ÀÖ´Ù¸é Ãâ·ÂÇÑ´Ù.
	//----------------------------------------------------------------
	if (pGear->HasBrokenItem() )
	{
		//---------------------------------------------
		// Lock
		//---------------------------------------------
		m_pSurface->Lock();

		//---------------------------------------------
		// ±âº» ²®µ¥±â(-_-;) Ãâ·Â
		//---------------------------------------------
		POINT pointBasis = { x+15, y };
		POINT point = { pointBasis.x, pointBasis.y };

		m_pSurface->BltSprite( &point, &m_ItemBrokenSPK[spriteID] );
		

		int size = pGear->GetSize();

		//---------------------------------------------
		// GearÀÇ ¸ðµç slot¿¡ ´ëÇØ¼­ Ã¼Å©
		//---------------------------------------------
		for (int i=0; i<size-4-6; i++)
		{			
			const MItem* pItem = pGear->GetItem( (BYTE)i );	
			
			//---------------------------------------------
			// ¹º°¡ Âø¿ëÇÏ°í ÀÖÀ»¶§¸¸ Ãâ·ÂÇÑ´Ù.
			//---------------------------------------------
			if (pItem!=NULL)
			{			
				MPlayerGear::ITEM_STATUS itemStatus = pGear->GetItemStatus( i );

				//---------------------------------------------
				// ¹«±âÀÎ °æ¿ì.. Á¾·ù¿¡ µû¶ó¼­ ±×¸²ÀÌ ´Ù¸£´Ù.
				//---------------------------------------------
				int frameID;

				if (g_pPlayer->IsSlayer())
				{
					//---------------------------------------------
					// ¿Þ¼ÕÀÎµ¥.
					//---------------------------------------------
					if (i==MSlayerGear::GEAR_SLAYER_LEFTHAND)
					{
						// ¹æÆÐÀÌ¸é Ãâ·Â
						if (pItem->GetItemClass()==ITEM_CLASS_SHIELD)
						{
							frameID = i;
						}
						// ¹æÆÐ°¡ ¾Æ´Ï¸é.. Ãâ·Â ¾ÈÇÑ´Ù.
						else
						{
							continue;
						}
					}
					//---------------------------------------------
					// ¿À¸¥¼ÕÀº ¹«±â¿¡ µû¶ó¼­ Ãâ·Â
					//---------------------------------------------
					else if (i==MSlayerGear::GEAR_SLAYER_RIGHTHAND)
					{				
						// ÃÑ?				
						if (pItem->IsGunItem())
						{
							frameID = MSlayerGear::MAX_GEAR_SLAYER + 1 - 12;
						}
						// ½ÊÀÚ°¡?
						else if (pItem->GetItemClass()==ITEM_CLASS_CROSS
								|| pItem->GetItemClass()==ITEM_CLASS_MACE)
						{
							frameID = MSlayerGear::MAX_GEAR_SLAYER -12;
						}
						// ¾Æ´Ï¹È.. Ä®ÀÌ³ª µµ
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
				// Á¤»óÀûÀÌ¸é Á¤»óÀûÀÎ°Å Ãâ·Â..
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
				// ºÎ¼­Á® °¡´Â °ÍÀÌ¸é.. »ö±ò~ÀÖ°Ô Ãâ·Â..
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

	// Check bounds to prevent crash (SDL backend defensive fix)
	if (frameID < 0) {
		return 0;
	}

	switch (bltType)
	{
		case BLT_SCREEN :
			if (frameID >= m_EffectScreenFPK.GetSize()) return 0;
			numFrame = m_EffectScreenFPK[frameID][0].GetSize();
		break;

		case BLT_EFFECT :
			if (frameID >= m_EffectAlphaFPK.GetSize()) return 0;
			numFrame = m_EffectAlphaFPK[frameID][0].GetSize();
		break;

		case BLT_NORMAL :
			if (frameID >= m_EffectNormalFPK.GetSize()) return 0;
			numFrame = m_EffectNormalFPK[frameID][0].GetSize();
		break;

		case BLT_SHADOW :
			if (frameID >= m_EffectShadowFPK.GetSize()) return 0;
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
	// Defensive check for SDL backend
	switch (bltType)
	{
		case BLT_SCREEN :
			if (frameID >= m_EffectScreenFPK.GetSize()) return 0;
			if (dir >= m_EffectScreenFPK[frameID].GetSize()) return 0;
			if (frame >= m_EffectScreenFPK[frameID][dir].GetSize()) return 0;
			return m_EffectScreenFPK[frameID][dir][frame].GetLight();

		case BLT_EFFECT :
			if (frameID >= m_EffectAlphaFPK.GetSize()) return 0;
			if (dir >= m_EffectAlphaFPK[frameID].GetSize()) return 0;
			if (frame >= m_EffectAlphaFPK[frameID][dir].GetSize()) return 0;
			return m_EffectAlphaFPK[frameID][dir][frame].GetLight();

		case BLT_NORMAL :
			if (frameID >= m_EffectNormalFPK.GetSize()) return 0;
			if (dir >= m_EffectNormalFPK[frameID].GetSize()) return 0;
			if (frame >= m_EffectNormalFPK[frameID][dir].GetSize()) return 0;
			return m_EffectNormalFPK[frameID][dir][frame].GetLight();

		case BLT_SHADOW :
			if (frameID >= m_EffectShadowFPK.GetSize()) return 0;
			if (dir >= m_EffectShadowFPK[frameID].GetSize()) return 0;
			if (frame >= m_EffectShadowFPK[frameID][dir].GetSize()) return 0;
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
// m_pSurface->IsLock() »óÅÂ¿¡¼­ È£ÃâµÈ´Ù°í °¡Á¤ÇÑ´Ù.
//----------------------------------------------------------------
void
MTopView::DrawEffect(POINT* pPoint, MEffect* pEffect, bool bSelectable)
{
	if(g_pEventManager->GetEventByFlag(EVENTFLAG_NOT_DRAW_EFFECT))
		return;

	if (!pEffect || !pPoint) {
		return;
	}

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
			TYPE_FRAMEID frameID = pEffect->GetFrameID();
			BYTE direction = pEffect->GetDirection();
			BYTE frame = pEffect->GetFrame();

			// Boundary check
			if (frameID >= m_EffectNormalFPK.GetSize()) {
#ifdef OUTPUT_DEBUG
				DEBUG_ADD_FORMAT("DrawEffect(BLT_NORMAL): Invalid frameID=%d", frameID);
#endif
				pEffect->ClearScreenRect();
				break;
			}

			CFrame& Frame = m_EffectNormalFPK[frameID][direction][frame];
			int spriteID = Frame.GetSpriteID();

			if (spriteID != SPRITEID_NULL)
			{
				// Boundary check for sprite
				if (spriteID < 0 || spriteID >= m_EffectNormalSPK.GetSize()) {
#ifdef OUTPUT_DEBUG
					DEBUG_ADD_FORMAT("DrawEffect(BLT_NORMAL): Invalid spriteID=%d", spriteID);
#endif
					pEffect->ClearScreenRect();
					break;
				}

				CSprite* pSprite = &m_EffectNormalSPK[spriteID];

				point.x += Frame.GetCX();
				point.y += Frame.GetCY();

				m_pSurface->BltSprite(&point, pSprite);

				//----------------------------------------
				// Set selectable area
				//----------------------------------------
				if (bSelectable)
				{
					RECT rect;
					rect.left	= point.x;
					rect.top	= point.y;
					rect.right	= rect.left + pSprite->GetWidth();
					rect.bottom = rect.top + pSprite->GetHeight();

					pEffect->SetScreenRect(&rect);
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
			TYPE_FRAMEID frameID = pEffect->GetFrameID();
			BYTE direction = pEffect->GetDirection();
			BYTE frame = pEffect->GetFrame();

			// Boundary check
			if (frameID >= m_EffectAlphaFPK.GetSize()) {
#ifdef OUTPUT_DEBUG
				DEBUG_ADD_FORMAT("DrawEffect(BLT_EFFECT): Invalid frameID=%d", frameID);
#endif
				break;
			}

			CEffectFrame& Frame = m_EffectAlphaFPK[frameID][direction][frame];
			int spriteID = Frame.GetSpriteID();

			if (spriteID != SPRITEID_NULL)
			{
				// Boundary check for palette
				if (frameID >= m_EffectAlphaPPK.GetSize()) {
#ifdef OUTPUT_DEBUG
					DEBUG_ADD_FORMAT("DrawEffect(BLT_EFFECT): Invalid palette frameID=%d", frameID);
#endif
					break;
				}

				// Boundary check for sprite
				if (spriteID < 0 || spriteID >= m_EffectAlphaSPK.GetSize()) {
#ifdef OUTPUT_DEBUG
					DEBUG_ADD_FORMAT("DrawEffect(BLT_EFFECT): Invalid spriteID=%d", spriteID);
#endif
					break;
				}

				point.x += Frame.GetCX();
				point.y += Frame.GetCY();

				DRAW_ALPHASPRITEPAL(&point, spriteID, m_EffectAlphaSPK, m_EffectAlphaPPK[frameID])

				//----------------------------------------
				// Set selectable area
				//----------------------------------------
				if (bSelectable)
				{
					CAlphaSpritePal* pSprite = &m_EffectAlphaSPK[spriteID];

					RECT rect;
					rect.left	= point.x;
					rect.top	= point.y;
					rect.right	= rect.left + pSprite->GetWidth();
					rect.bottom = rect.top + pSprite->GetHeight();

					pEffect->SetScreenRect(&rect);
				}

				//-------------------------------------------------------
				// Hardware acceleration enabled
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
//						// ¿ø·¡´ë·Î
//						CDirect3D::GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);					
//						CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//						CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
//					#endif
					
					//---------------------------------------- 		
					// ÀÌÆåÆ® ¼±ÅÃ »ç°¢Çü ¿µ¿ª ¼³Á¤
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
					// LightFilter¸¦ Ãß°¡ÇÑ´Ù.
					//------------------------------------------------
					AddLightFilter3D( pPoint->x + 24, 
									pPoint->y + 24, 
									Frame.GetLight(),			// ºûÀÇ ¹à±â
									false);	// false = screenÁÂÇ¥					
				
				}
				//-------------------------------------------------------
				// H/W °¡¼ÓÀÌ ¾ÈµÇ´Â °æ¿ìÀÌ¸é...
				//-------------------------------------------------------
				else
				{
//					DRAW_ALPHASPRITEPAL(&point, spriteID, m_EffectAlphaSPK, m_EffectAlphaPPK[pEffect->GetFrameID()])//, m_EffectAlphaSPKI, m_EffectAlphaSPKFile)

//					//---------------------------------------- 		
//					// ÀÌÆåÆ® ¼±ÅÃ »ç°¢Çü ¿µ¿ª ¼³Á¤
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


					AddLightFilter2D( pPoint->x + 24, 
									pPoint->y + 24, 
									Frame.GetLight(),			// ºûÀÇ ¹à±â
									false);		// false = screenÁÂÇ¥
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
			TYPE_FRAMEID frameID = pEffect->GetFrameID();
			BYTE direction = pEffect->GetDirection();
			BYTE frame = pEffect->GetFrame();

			// Boundary check
			if (frameID >= m_EffectScreenFPK.GetSize()) {
#ifdef OUTPUT_DEBUG
				DEBUG_ADD_FORMAT("DrawEffect(BLT_SCREEN): Invalid frameID=%d", frameID);
#endif
				break;
			}

			CEffectFrame& Frame = m_EffectScreenFPK[frameID][direction][frame];
			int spriteID = Frame.GetSpriteID();

			if (spriteID != SPRITEID_NULL)
			{
				// Boundary check for palette
				if (frameID >= m_EffectScreenPPK.GetSize()) {
#ifdef OUTPUT_DEBUG
					DEBUG_ADD_FORMAT("DrawEffect(BLT_SCREEN): Invalid palette frameID=%d", frameID);
#endif
					break;
				}

				// Boundary check for sprite
				if (spriteID < 0 || spriteID >= m_EffectScreenSPK.GetSize()) {
#ifdef OUTPUT_DEBUG
					DEBUG_ADD_FORMAT("DrawEffect(BLT_SCREEN): Invalid spriteID=%d", spriteID);
#endif
					break;
				}

				point.x += Frame.GetCX();
				point.y += Frame.GetCY();

				DRAW_NORMALSPRITEPAL_EFFECT(&point,
									spriteID,
									m_EffectScreenSPK,
									m_EffectScreenPPK[frameID],
									CSpriteSurface::EFFECT_SCREEN)

				//----------------------------------------
				// Set selectable area
				//----------------------------------------
				if (bSelectable)
				{
					CSpritePal* pSprite = &m_EffectScreenSPK[spriteID];

					RECT rect;
					rect.left	= point.x;
					rect.top	= point.y;
					rect.right	= rect.left + pSprite->GetWidth();
					rect.bottom = rect.top + pSprite->GetHeight();

					pEffect->SetScreenRect(&rect);
				}

				//-------------------------------------------------------
				// Hardware acceleration enabled
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
//					// ¿ø·¡´ë·Î
//					CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
//					CDirect3D::GetDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
					
					//---------------------------------------- 		
					// ÀÌÆåÆ® ¼±ÅÃ »ç°¢Çü ¿µ¿ª ¼³Á¤
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
					// LightFilter¸¦ Ãß°¡ÇÑ´Ù.
					//------------------------------------------------
					AddLightFilter3D( pPoint->x + 24, 
									pPoint->y + 24, 
									Frame.GetLight(),			// ºûÀÇ ¹à±â
									false);	// false = screenÁÂÇ¥					
				
				}
				//-------------------------------------------------------
				// H/W °¡¼ÓÀÌ ¾ÈµÇ´Â °æ¿ìÀÌ¸é...
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
//					// ÀÌÆåÆ® ¼±ÅÃ »ç°¢Çü ¿µ¿ª ¼³Á¤
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


					AddLightFilter2D( pPoint->x + 24, 
									pPoint->y + 24, 
									Frame.GetLight(),			// ºûÀÇ ¹à±â
									false);		// false = screenÁÂÇ¥
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
			TYPE_FRAMEID frameID = pEffect->GetFrameID();
			BYTE direction = pEffect->GetDirection();
			BYTE frame = pEffect->GetFrame();

			// Boundary check
			if (frameID >= m_EffectShadowFPK.GetSize()) {
#ifdef OUTPUT_DEBUG
				DEBUG_ADD_FORMAT("DrawEffect(BLT_SHADOW): Invalid frameID=%d", frameID);
#endif
				break;
			}

			CEffectFrame& Frame = m_EffectShadowFPK[frameID][direction][frame];
			int spriteID = Frame.GetSpriteID();

			if (spriteID != SPRITEID_NULL)
			{
				// Boundary check for sprite
				if (spriteID < 0 || spriteID >= m_EffectShadowSPK.GetSize()) {
#ifdef OUTPUT_DEBUG
					DEBUG_ADD_FORMAT("DrawEffect(BLT_SHADOW): Invalid spriteID=%d", spriteID);
#endif
					break;
				}

				point.x += Frame.GetCX();
				point.y += Frame.GetCY();

				CShadowSprite* pSprite = &m_EffectShadowSPK[spriteID];

				if (pSprite->IsInit())
				{
					m_pSurface->BltShadowSpriteDarkness(&point, pSprite, 1);
				}

				//-------------------------------------------------------
				// Hardware acceleration enabled
				//-------------------------------------------------------
				if (CDirect3D::IsHAL())
				{
//					DRAW_TEXTURE_SPRITE_LOCKED(point.x, point.y, spriteID, m_pEffectShadowManager)		

					//------------------------------------------------
					// LightFilter¸¦ Ãß°¡ÇÑ´Ù.
					//------------------------------------------------
					AddLightFilter3D( pPoint->x + 24, 
									pPoint->y + 24, 
									Frame.GetLight(),			// ºûÀÇ ¹à±â
									false);	// false = screenÁÂÇ¥					
				
				}
				//-------------------------------------------------------
				// H/W °¡¼ÓÀÌ ¾ÈµÇ´Â °æ¿ìÀÌ¸é...
				//-------------------------------------------------------
				else
				{
//					CShadowSprite* pSprite = &m_EffectShadowSPK[spriteID];
//
//					if (pSprite->IsInit())
//					{
//						m_pSurface->BltShadowSpriteDarkness(&point, pSprite, 1 );
//						
						AddLightFilter2D( pPoint->x + 24, 
										pPoint->y + 24, 
										Frame.GetLight(),			// ºûÀÇ ¹à±â
										false);		// false = screenÁÂÇ¥
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
		// ºí·¯µå ¿ö·Ï ±×¸².. 
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

	// spriteID°¡ spriteÀÎ ¸ó½ºÅÍµé Áß¿¡¼­ ÇÏ³ª¸¦ ¼±ÅÃÇÑ´Ù.
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
		
		g_PrintColorStrOut(point->x + 24 - g_GetStringWidth(str, gpC_base->m_chatting_pi.hfont)/2, py, str, gpC_base->m_chatting_pi, color, RGB_BLACK);

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
	- °ø¼ºÀü ½Ã Ä³¸¯ÅÍ À§¿¡ ±æµå ¸¶Å©¸¦ º¸¿©ÁØ´Ù.
	- ¼±ÅÃ ¿©ºÎ¿Í »ó°ü¾øÀÌ Ç×»ó º¸ÀÎ´Ù.
	- g_pPlayer°¡ °ø¼ºÀü Âü°¡ÁßÀÏ ¶§¸¸ °ø¼ºÀü Âü°¡ÁßÀÎ ´Ù¸¥ Å©¸®ÃÄÀÇ ±æµå¸¶Å© Ç¥½Ã
//----------------------------------------------------------------------*/
void		
MTopView::DrawGuildMarkInSiegeWar(MCreature* pCreature, int YPos)
{	
	// EFFECTSTATUS_GHOST ¿¡ °É·ÁÀÖÀ¸¸é ³²µµ ¸øº¸°í ³ªµµ ¸øº»´Ù
	if(pCreature == NULL) return;
	if(pCreature->HasEffectStatus(EFFECTSTATUS_GHOST))return;

	//-----------------------------------------------------
	// Hallu¿¡ °É¸®¸é ±æµå¸¶Å©³ª °è±Þ¸¶Å© ¾ÈÂïÀ½
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
		// Guild°¡ ÀÖ´Â °æ¿ì - Guild Mark Ãâ·Â
		//-----------------------------------------------------
		int guildID = pCreature->GetGuildNumber();
			
		if (guildID > 0)
		{
			//-------------------------------------------------
			// loadµÇ¾î ÀÖ´ÂÁö º»´Ù.
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
					// file¿¡ ÀÖ´ÂÁö º»´Ù.
					//-------------------------------------------------
					g_pGuildMarkManager->LoadGuildMark(guildID);
					
					//-------------------------------------------------
					// file¿¡¼­ loadµÇ¾ú´ÂÁö ´Ù½Ã Ã¼Å©
					//-------------------------------------------------
					pSprite = g_pGuildMarkManager->GetGuildMark(guildID);
					
					//-------------------------------------------------
					// file¿¡µµ ¾ø´Â °æ¿ì..
					// guildMark°ü¸®¼­¹ö?¿¡¼­ ¹Þ¾Æ¿Â´Ù.
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
// Unlock »óÅÂ¿¡¼­ ºÒ·ÁÁö´Â ÇÔ¼öÀÌ´Ù.
//
// ¼±ÅÃµÈ Ä³¸¯ÅÍ ÀÌ¸§À» Ãâ·ÂÇÑ´Ù.
// ÀÌ¸§¿¡´Â HP, ¼ºÇâ, ±æµå.. µîµîÀÇ Á¤º¸°¡ Æ÷ÇÔµÈ´Ù.
// ¼­ºñ½º·Î(-_-;) RequestMode¿¡ µû¸¥ iconµµ Ãâ·ÂÇÑ´Ù.
//----------------------------------------------------------------------
void		
MTopView::DrawCreatureName(MCreature* pCreature)
{	
	// EFFECTSTATUS_GHOST ¿¡ °É·ÁÀÖÀ¸¸é ³²µµ ¸øº¸°í ³ªµµ ¸øº»´Ù
	if(pCreature == NULL) return;
	if(pCreature->HasEffectStatus(EFFECTSTATUS_GHOST))return;

	// 2004, 08, 05 sobeit add start - ¼º¹®Àº º¸¿©Áö¸é ÀÌ»óÇÏ´Ù?
	if( pCreature->GetCreatureType() >= 726 &&
		pCreature->GetCreatureType() <= 729)
		return;
	// 2004, 08, 05 sobeit add end - ¼º¹® 
	if(
		(g_pPlayer->HasEffectStatus(EFFECTSTATUS_YELLOW_POISON_TO_CREATURE) || 
		g_pPlayer->HasEffectStatus(EFFECTSTATUS_FLARE )||
		g_pPlayer->HasEffectStatus(EFFECTSTATUS_BLINDNESS))
		&& pCreature != g_pPlayer 
		)
	{
		// °Å¸®¿¡ µû¶ó¼­ Ãâ·Â ¿©ºÎ¸¦ °áÁ¤ÇÑ´Ù.
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
	// ¶¥ ¼Ó¿¡ ÀÖ´Â ¾Ö´Â ÀÌ¸§µµ ¾ø´ç. - -;
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
	// item ±³È¯ÇÑ´Ù´Â Ç¥½Ã Ãâ·Â
	//------------------------------------------------
	if (IsRequestMode())	
	{
		bool bRequest = false;

		if(pCreature->GetClassType() == MCreature::CLASS_FAKE)
		{
			MFakeCreature *pFakeCreature = (MFakeCreature *)pCreature;
			if(pFakeCreature->GetOwnerID() != OBJECTID_NULL)	// ÆêÀÌ´Ù
			{
				bRequest = true;
			}
		}
		//------------------------------------------------
		// ¾ÆÁ÷Àº Ãâ·ÂÇÏ¸é ¾ÈµÈ´Ù.
		//------------------------------------------------			
		else if (// NPC°¡ ¾Æ´Ï°í
			!pCreature->IsNPC()
			// PlayerÀÎ °æ¿ì¸¸
			//&& pCreature->GetCreatureType()<=CREATURETYPE_VAMPIRE_FEMALE
			//&& (*g_pCreatureSpriteTable)[(*g_pCreatureTable)[pCreature->GetCreatureType()].SpriteType].IsPlayerOnlySprite()
			&& (*g_pCreatureSpriteTable)[(*g_pCreatureTable)[pCreature->GetCreatureType()].SpriteTypes[0]].IsPlayerOnlySprite()
			// °ü ¼Ó¿¡ ÀÖ´Â °æ¿ì
			&& !pCreature->IsInCasket()
			// ¶¥¿¡ ¹¯ÇôÀÖÀ»¶© °Å·¡ ÇÑ´Ù´Â Ç¥½Ã ¾È ³ª¿À°Ô...
			//&& !pCreature->IsUndergroundCreature()
			// »ì¾ÆÀÖ´Â.. - -;
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
			// trade ½ÅÃ»ÇÏ´Â ¾ÆÀÌÄÜ
			//---------------------------------------------------------------
			if (IsRequestTrade())
			{
				const int maxRequestFrames = SPRITEID_ITEM_TRADE_LAST - SPRITEID_ITEM_TRADE + 1;
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
			// party ½ÅÃ»ÇÏ´Â ¾ÆÀÌÄÜ
			//---------------------------------------------------------------
			else if(IsRequestParty())
			{
				const int maxRequestFrames = SPRITEID_PARTY_REQUEST_LAST - SPRITEID_PARTY_REQUEST + 1;
				RequestSpriteID = SPRITEID_PARTY_REQUEST + ((g_CurrentFrame>>1) % (maxRequestFrames+4));

				if (RequestSpriteID > SPRITEID_PARTY_REQUEST_LAST)
				{
					RequestSpriteID = SPRITEID_PARTY_REQUEST_LAST;					
				}
			}
			//---------------------------------------------------------------
			// Info ½ÅÃ»ÇÏ´Â ¾ÆÀÌÄÜ
			//---------------------------------------------------------------
			else if(IsRequestInfo())
			{
				const int maxRequestFrames = SPRITEID_INFO_REQUEST_LAST - SPRITEID_INFO_REQUEST + 1;
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
	// Ä³¸¯ÅÍ ÀÌ¸§ Ãâ·Â
	//------------------------------------------------
	//gC_font.PrintStringNoConvert(&m_SurfaceInfo, pCreature->GetName(), pointTemp.x, pointTemp.y, 0xFFFF);	
	bool bMasterWords = pCreature->GetCreatureType()==CREATURETYPE_SLAYER_OPERATOR
						|| pCreature->GetCreatureType()==CREATURETYPE_VAMPIRE_OPERATOR
						|| pCreature->GetCreatureType() == CREATURETYPE_OUSTERS_OPERATOR
						|| strncmp( pCreature->GetName(), (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetString(), (*g_pGameStringTable)[UI_STRING_MESSAGE_MASTER_NAME].GetLength() ) == 0;
//						|| (strstr(pCreature->GetName(), "¿î¿µÀÚ")!=NULL);

	bool bHalluName = (g_pPlayer->HasEffectStatus(EFFECTSTATUS_HALLUCINATION) || 
		!bMasterWords && 
		!g_pUserInformation->IsMaster && 
		(pCreature->GetClassType() == MCreature::CLASS_PLAYER || 
		pCreature->GetClassType() == MCreature::CLASS_CREATUREWEAR) && 
		g_pPlayer->GetRace() != pCreature->GetRace()) &&
		g_pZoneTable->Get( g_pZone->GetID() )->ChatMaskByRace &&
		g_pPlayer->GetCreatureType() != CREATURETYPE_SLAYER_OPERATOR &&
		g_pPlayer->GetCreatureType() != CREATURETYPE_VAMPIRE_OPERATOR &&
		g_pPlayer->GetCreatureType() != CREATURETYPE_OUSTERS_OPERATOR;						// Ãª ¸¶½ºÅ©¸¦ ¾º¿ï °æ¿ì¿¡¸¸ 
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
	// NPCÀÎ °æ¿ì
	//-----------------------------------------------------
	if (pCreature->IsNPC())
	{
		// ¹ÙÅä¸®ÀÎ °æ¿ì.. ÇÏµåÄÚµù - -;;
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
	// ¿î¿µÀÚÀÎ °æ¿ì
	//-----------------------------------------------------
	else if (pCreature->GetCompetence()==0)
	{
		color = CDirectDraw::Color( 31, 23, 3 );	// ±Ý»ö?
		font	= FONTID_NPC_NAME;
	}
	//-----------------------------------------------------
	// Hallu¿¡ °É¸° °æ¿ì
	//-----------------------------------------------------
	else if(g_pPlayer->HasEffectStatus(EFFECTSTATUS_HALLUCINATION))
	{
		color	= m_ColorNameAlignment[1];
		font	= FONTID_VAMPIRE_NAME;
	}
	//-----------------------------------------------------
	// SlayerÀÎ °æ¿ì 
	//-----------------------------------------------------
	else if (g_pPlayer->IsSlayer())
	{
		//-----------------------------------------------------
		// Á¾Á·¿¡ µû¶ó¼­ 
		//-----------------------------------------------------
		if (g_pPlayer->CanAttackTribe( pCreature )
			|| g_pJusticeAttackManager->HasCreature( pCreature->GetName() ))
//		if (g_pObjectSelector->CanAttack( pCreature ) )
		{
			color	= m_ColorNameAlignment[1];
			font	= FONTID_VAMPIRE_NAME;
		}
		//-----------------------------------------------------
		// ¾Æ´Ï¸é.. ¼ºÇâ¿¡ µû¸¥ Ç¥½Ã
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
		// ¾Æ´Ï¸é.. ¼ºÇâ¿¡ µû¸¥ Ç¥½Ã
		//-----------------------------------------------------
		else
		{
			int alignment = pCreature->GetAlignment();
			
			color = m_ColorNameAlignment[alignment];
			font	= FONTID_VAMPIRE_NAME;				
		}
	}
	//-----------------------------------------------------
	// ¹ìÆÄÀÌ¾îÀÎ °æ¿ì
	//-----------------------------------------------------
	else
	{
		//-----------------------------------------------------
		// ±æµå¿¡ µû¶ó¼­
		//-----------------------------------------------------
		if (g_pPlayer->CanAttackGuild( pCreature )
			|| g_pJusticeAttackManager->HasCreature( pCreature->GetName() ))
//		if(g_pObjectSelector->CanAttack( pCreature ) )
		{
			color	= m_ColorNameAlignment[1];
			font	= FONTID_VAMPIRE_NAME;
		}
		//-----------------------------------------------------
		// ¾Æ´Ï¸é.. ¼ºÇâ¿¡ µû¸¥ Ç¥½Ã
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
		// Level NameÀÌ ÀÖ´Â °æ¿ì
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
		// ÀÌ¸§ ¹Ø¿¡ ±ò¸± ÀüÃ¼(ÀÌ¸§) Å©±âÀÇ ¹Ú½º
		//-----------------------------------------------------
		int rectLeft	= m_pointChatString.x + POSITION_HP_BAR -14 ;
		int rectRight	= rectLeft + MAX_HP_BAR;//m_pointChatString.x+5 + namePixel;
		int rectTop		= yPoint_20;
		int rectBottom	= yPoint_20 + g_pClientConfig->FONT_HEIGHT;

		//-----------------------------------------------------
		// Hallu¿¡ °É¸®¸é ±æµå¸¶Å©³ª °è±Þ¸¶Å© ¾ÈÂïÀ½
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
			// Guild°¡ ÀÖ´Â °æ¿ì - Guild Mark Ãâ·Â
			//-----------------------------------------------------
			guildID = pCreature->GetGuildNumber();
			if(	g_pPlayer->GetRace() != pCreature->GetRace() && pCreature->IsPlayer() &&
				g_pZoneTable->Get( g_pZone->GetID() )->ChatMaskByRace == true
				)
				guildID = 0;
			
			// gradeÃâ·Â
			int gradeID = pCreature->GetGrade()-1;	// 1~50 À¸·Î µÇÀÖÀ¸¹Ç·Î -1ÇØ¼­ 0~49·Î ¸ÂÃá´Ù
			// ÇÃ·¹ÀÌ¾î¸¸ °è±Þ Ãâ·Â, ¹ÚÁã³ª ´Á´ë´Â ¾ÈÇÔ
			if(!pCreature->IsPlayerOnly())
				gradeID = -1;
			
				/*
				if (guildID>=0)
				{
				// run-time loadingµµ ±¦ÂúÀ» µí..
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
				// loadµÇ¾î ÀÖ´ÂÁö º»´Ù.
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
						// file¿¡ ÀÖ´ÂÁö º»´Ù.
						//-------------------------------------------------
						g_pGuildMarkManager->LoadGuildMark(guildID);
						
						//-------------------------------------------------
						// file¿¡¼­ loadµÇ¾ú´ÂÁö ´Ù½Ã Ã¼Å©
						//-------------------------------------------------
						pSprite = g_pGuildMarkManager->GetGuildMark(guildID);
						
						//-------------------------------------------------
						// file¿¡µµ ¾ø´Â °æ¿ì..
						// guildMark°ü¸®¼­¹ö?¿¡¼­ ¹Þ¾Æ¿Â´Ù.
						//-------------------------------------------------
						//if (pSprite==NULL) {}
					}				
				}
			}
			
			// °è±ÞÀÌ ÀÖ´Â°æ¿ì °è±Þ¸¶Å© Ãâ·Â
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
		// ÆêÀÎ°æ¿ì ·¹º§ ¸¶Å© Ãâ·Â
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
		// HP°è»ê
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
		// FIX: Handle maxHP being 0 - set to currentHP to show full bar
		//-----------------------------------------------------
		if (maxHP == 0 && currentHP > 0)
		{
			maxHP = currentHP;
		}
		else if (maxHP == 0 && currentHP == 0)
		{
			// Both are 0, show empty bar
			maxHP = 1;
			currentHP = 0;
		}

		//-----------------------------------------------------
		// ¶¥¼Ó¿¡ ÀÖ´Â ¾Ö´Â HP°¡ ²Ë Âù°ÍÃ³·³ º¸¿©ÁØ´Ù.
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
		// ½ÇÁ¦·Î º¸¿©Áú pixel°è»ê
		//-----------------------------------------------------
		int maxPixels		= rectRight - rectLeft;
		int currentPixels	= max(0,int((maxHP==0)? 0 : maxPixels * currentHP / maxHP));

		int nameX = //rectLeft+(MAX_HP_BAR>>1) - (namePixel>>1);
					rectLeft + ((MAX_HP_BAR - namePixel)>>1);

		
		//-----------------------------------------------------
		// HP°¡ ²Ë Â÷°Ô Ç¥½ÃµÇ´Â °æ¿ì
		//-----------------------------------------------------
		if (currentPixels==maxPixels)
		{
			// ¹ÙÅÁ»ö ±ò ÇÊ¿ä ¾øÁö¸¸.. ÀÌ¸§ Àß º¸ÀÌ°Ô ÇÒ·Á°í.. --
			DRAWTEXT_NODE* pNodeBase = new DRAWTEXT_NODE (
										nameX+1,
										yPoint_20+4+1,
										NULL,//pName,
										0,
										font);

			AddText( pNodeBase );
		}
		//-----------------------------------------------------
		// HP ´âÀº ºÎºÐ Ç¥½Ã
		//-----------------------------------------------------
		else
		{
			//-----------------------------------------------------
			//
			//			Max HP ¸¸Å­ÀÇ Box¸¸ Ç¥½Ã
			//
			//-----------------------------------------------------
			DRAWTEXT_NODE* pNodeBase = new DRAWTEXT_NODE (
										nameX+1,
										yPoint_20+4+1,
										NULL,//pName,
										0,
										font);

			//-----------------------------------------------------
			// HP ´âÀº ºÎºÐ
			//-----------------------------------------------------
			RECT rect = {	rectLeft + currentPixels, 
							rectTop,
							rectRight,
							rectBottom 
			};


			pNodeBase->SetBox( rect, m_ColorHPBarBg, false );  // 不透明模式
			AddText( pNodeBase );
		}

		//-----------------------------------------------------
		//
		//		ÇöÀç HP¸¸Å­ÀÇ Box¿Í Ä³¸¯ÅÍ ÀÌ¸§ Ãâ·Â
		//
		//-----------------------------------------------------
		// ÇöÀç HP¸¸Å­ÀÇ Box¿Í Ä³¸¯ÅÍ ÀÌ¸§ Ãâ·Â(±î¸Ä°Ô)
		//-----------------------------------------------------	
		DRAWTEXT_NODE* pNode = new DRAWTEXT_NODE (
									nameX + 1,
									yPoint_20+4 + 1,
									pName,
									0,		//color,
									font);	//FLAG_DRAWTEXT_HANGUL_JOHAP);

		//-----------------------------------------------------
		// 0ÀÌ ¾Æ´Ï¸é boxÃâ·Â
		//-----------------------------------------------------							
		if (currentPixels!=0)				
		{
			//-----------------------------------------------------
			// ÇöÀç HP¿¡ ´ëÇÑ ¹Ú½º
			//-----------------------------------------------------
			RECT rectHP = {	rectLeft,
							rectTop,
							rectLeft + currentPixels,
							rectBottom
			};

			//-----------------------------------------------------
			// FIX: Force non-transparent HP bar for better visibility
			// Force non-transparent mode regardless of user option
			//-----------------------------------------------------
			pNode->SetBox( rectHP, m_ColorHPBar, false );
		}

		//-----------------------------------------------------
		// Ãâ·ÂÇÏ°Ô ¼³Á¤..
		//-----------------------------------------------------
		AddText( pNode );			

		// 2004, 10, 28, sobeit add start -¸ó½ºÅÍ Å³ Äù½ºÆ® ÇØ´ç ¸ó½ºÅÍ¿¡ Ç¥½Ã.
//		if(pCreature->IsAlive() && UI_IsMonsterKillQuest_Monster((*g_pCreatureTable)[pCreature->GetCreatureType()].SpriteTypes[0]))
//		{
//			WORD temp_color;
//			temp_color = CIndexSprite::ColorSet[MItem::GetQuestItemColorset()][15];
//			color  = RGB(CDirectDraw::Red(temp_color)<<3, CDirectDraw::Green(temp_color)<<3, CDirectDraw::Blue(temp_color)<<3);
//		}
		// 2004, 10, 28, sobeit add end
		//-----------------------------------------------------
		// ÀÌ¸§ ´Ù½Ã Âï±â
		//-----------------------------------------------------
		DRAWTEXT_NODE* pNode2 = new DRAWTEXT_NODE (
									nameX,
									yPoint_20+4,
									pName,
									color,
									font);	//FLAG_DRAWTEXT_HANGUL_JOHAP);

		AddText( pNode2 );
		// 2004, 6, 17, sobeit add start - about nick name - °°Àº Á¾Á· ÀÏ¶§¸¸ ´Ð³×ÀÓ º¸ÀÌ°Ô..¼öÁ¤.
		if(pCreature->GetNickNameType() != NicknameInfo::NICK_NONE)// && (/*g_pPlayer->GetRace() == pCreature->GetRace()|| */pCreature->IsFakeCreature()))
		{
			if(yPoint-42<0) return; // °Á ÂïÁö ¸»ÀÚ..¤¾¤¾
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
			//	if(guildID >0) // ±æµå¸¶Å©°¡ ¾ÈÂïÈú¶© ´Ð³×ÀÓÀ» hpbar+°è±Þ¸¶Å© Áß¾Ó¿¡ Âï°í
			//		nameX = rectLeft + ((MAX_HP_BAR - namePixel+20)>>1);
			//	else // ±×¿Ü¿£ hpbar Áß¾Ó¿¡ Âï´Â´Ù
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

					// FIX: Force non-transparent box for better visibility
					pNode->SetBox( rectHP, bgColor, false );
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
// ¿©±â ÀÖ¾î¾ßÇÒ ÇÔ¼ö´Â ¾Æ´ÏÁö¸¸.. - -;
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
	// °íÁ¤µÈ burrow±×¸²  -_-;; 
	//----------------------------------------
	POINT pointTemp;
	RECT rect;

	CSprite* pSprite = &m_EtcSPK[ SPRITEID_CREATURE_BURROW ];	
	
	//----------------------------------------
	// ÁÂÇ¥ º¸Á¤
	//----------------------------------------
	int cx = 4;
	int cy = 4;
	pointTemp.x = pPoint->x + cx;
	pointTemp.y = pPoint->y + cy;
	
	//---------------------------------------- 		
	// Ä³¸¯ÅÍ ¼±ÅÃ »ç°¢Çü ¿µ¿ª ¼³Á¤
	//---------------------------------------- 	
	rect.left	= pointTemp.x;
	rect.top	= pointTemp.y;
	rect.right	= rect.left + pSprite->GetWidth();
	rect.bottom = rect.top + pSprite->GetHeight();
	pCreature->SetScreenRect( &rect );				
	
	//---------------------------------------- 	
	// ¼±ÅÃµÈ °æ¿ì
	//---------------------------------------- 	
	if (m_SelectCreatureID == pCreature->GetID() )
	{
		// SpriteOutlineManager¿¡ Ãß°¡
		
		// ¶¥¼Ó¿¡ ÀÖ´Â ¾ÖµéÀº ¹«Á¶°Ç vampireÀÌ´Ù.
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
		// ÀÌ¸§ Ãâ·ÂÇÒ ÁÂÇ¥ ÁöÁ¤
		//---------------------------------------- 	
		const int FontHeight = g_pClientConfig->FONT_HEIGHT;
		const int FontHeight2 = FontHeight << 1;
		
		pointTemp.x = pPoint->x;			
		if (pointTemp.x<0) pointTemp.x=0;
		
		//---------------------------------------- 	
		// Level Name ÂïÀ» À§Ä¡µµ °è»ê
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
		// ±×³É ÀÌ¸§¸¸ ÂïÀ» ¶§
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
	// ±×³É Ãâ·Â
	//----------------------------------------
	else
	{
		m_pSurface->BltSprite(&pointTemp, pSprite);
	}
	
	//----------------------------------------
	// ¸ö¿¡ ºÙÀº Effect Ãâ·Â
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
	// EFFECTSTATUS_GHOST ¿¡ °É·ÁÀÖÀ¸¸é ³²µµ ¸øº¸°í ³ªµµ ¸øº»´Ù
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
		// ÀÌ¸§ ¹Ø¿¡ ±ò¸± ÀüÃ¼(ÀÌ¸§) Å©±âÀÇ ¹Ú½º
		//-----------------------------------------------------
		{
			rectLeft	= g_pPlayer->GetPixelX() - m_FirstZonePixel.x + POSITION_HP_BAR - 14;
			rectRight	= rectLeft + MAX_HP_BAR;//m_pointChatString.x+5 + namePixel;
			rectTop		= yPoint_20;
			rectBottom	= yPoint_20+ g_pClientConfig->FONT_HEIGHT;
		}
		//-----------------------------------------------------
		// Hallu¿¡ °É¸®¸é ±æµå¸¶Å©³ª °è±Þ¸¶Å© ¾ÈÂïÀ½
		//-----------------------------------------------------
		if(!g_pPlayer->HasEffectStatus(EFFECTSTATUS_HALLUCINATION))
		{
			//-----------------------------------------------------
			// Guild°¡ ÀÖ´Â °æ¿ì - Guild Mark Ãâ·Â
			//-----------------------------------------------------
			guildID = g_pPlayer->GetGuildNumber();

			gradeID = g_pPlayer->GetGrade()-1;	// 1~50 À¸·Î µÇÀÖÀ¸¹Ç·Î -1ÇØ¼­ 0~49·Î ¸ÂÃá´Ù
			if(!g_pPlayer->IsPlayerOnly())
				gradeID = -1;
			
			if (guildID > 0)
			{
				//-------------------------------------------------
				// loadµÇ¾î ÀÖ´ÂÁö º»´Ù.
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
			
			// °è±ÞÀÌ ÀÖ´Â°æ¿ì °è±Þ¸¶Å© Ãâ·Â
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
		// HP°è»ê
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
		// ½ÇÁ¦·Î º¸¿©Áú pixel°è»ê
		//-----------------------------------------------------
		int maxPixels		= rectRight - rectLeft;
		int currentPixels	= max(0,int((maxHP==0)? 0 : maxPixels * currentHP / maxHP));

		int nameX = //rectLeft+(MAX_HP_BAR>>1) - (namePixel>>1);
					rectLeft + ((MAX_HP_BAR - namePixel)>>1);

		
		//-----------------------------------------------------
		// HP°¡ ²Ë Â÷°Ô Ç¥½ÃµÇ´Â °æ¿ì
		//-----------------------------------------------------
		if (currentPixels==maxPixels)
		{
			// ¹ÙÅÁ»ö ±ò ÇÊ¿ä ¾øÁö¸¸.. ÀÌ¸§ Àß º¸ÀÌ°Ô ÇÒ·Á°í.. --
			DRAWTEXT_NODE* pNodeBase = new DRAWTEXT_NODE (
										nameX+1,
										yPoint_20+4+1,
										NULL,//pName,
										0,
										font);

			AddText( pNodeBase );
		}
		//-----------------------------------------------------
		// HP ´âÀº ºÎºÐ Ç¥½Ã
		//-----------------------------------------------------
		else
		{
			//-----------------------------------------------------
			//
			//			Max HP ¸¸Å­ÀÇ Box¸¸ Ç¥½Ã
			//
			//-----------------------------------------------------
			DRAWTEXT_NODE* pNodeBase = new DRAWTEXT_NODE (
										nameX+1,
										yPoint_20+4+1,
										NULL,//pName,
										0,
										font);

			//-----------------------------------------------------
			// HP ´âÀº ºÎºÐ
			//-----------------------------------------------------
			RECT rect = {	rectLeft + currentPixels, 
							rectTop,
							rectRight,
							rectBottom 
			};


			pNodeBase->SetBox( rect, m_ColorHPBarBg, false );  // 不透明模式
			AddText( pNodeBase );
		}

		//-----------------------------------------------------
		//
		//		ÇöÀç HP¸¸Å­ÀÇ Box¿Í Ä³¸¯ÅÍ ÀÌ¸§ Ãâ·Â
		//
		//-----------------------------------------------------
		// ÇöÀç HP¸¸Å­ÀÇ Box¿Í Ä³¸¯ÅÍ ÀÌ¸§ Ãâ·Â(±î¸Ä°Ô)
		//-----------------------------------------------------			
		DRAWTEXT_NODE* pNode = new DRAWTEXT_NODE (
									nameX + 1,
									yPoint_20+4 + 1,
									pCreatureName,
									0,		//color,
									font);	//FLAG_DRAWTEXT_HANGUL_JOHAP);

		//-----------------------------------------------------
		// 0ÀÌ ¾Æ´Ï¸é boxÃâ·Â
		//-----------------------------------------------------							
		if (currentPixels!=0)				
		{
			//-----------------------------------------------------
			// ÇöÀç HP¿¡ ´ëÇÑ ¹Ú½º
			//-----------------------------------------------------
			RECT rectHP = {	rectLeft,
							rectTop,
							rectLeft + currentPixels,
							rectBottom
			};

			//-----------------------------------------------------
			// FIX: Force non-transparent HP bar for better visibility
			// Force non-transparent mode regardless of user option
			//-----------------------------------------------------
			pNode->SetBox( rectHP, m_ColorHPBar, false );
		}

		//-----------------------------------------------------
		// Ãâ·ÂÇÏ°Ô ¼³Á¤..
		//-----------------------------------------------------
		AddText( pNode );			


		//-----------------------------------------------------
		// ÀÌ¸§ ´Ù½Ã Âï±â
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
			//	if(guildID >0) // ±æµå¸¶Å©°¡ ¾ÈÂïÈú¶© ´Ð³×ÀÓÀ» hpbar+°è±Þ¸¶Å© Áß¾Ó¿¡ Âï°í
			//		nameX = rectLeft + ((MAX_HP_BAR - namePixel+20)>>1);
			//	else // ±×¿Ü¿£ hpbar Áß¾Ó¿¡ Âï´Â´Ù
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

					// FIX: Force non-transparent box for better visibility
					pNode->SetBox( rectHP, bgColor, false );
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
	
	// 2005, 2, 18, sobeit add start - WebBrowser°¡ ¶°ÀÖÀ» ¶§ Ã³¸®
	if(UI_IsRunning_WebBrowser())
	{
		//bDrawBackGround = false; // ¹è°æÀ» ÂïÁö ¾Ê´Â´Ù. UI´Â Âï´Â´Ù..
		int TempValue = UI_GetMouseCursorInfo_WebBrowser();

		if(TempValue == 1) // Ä¿¼­°¡ À¥ È­¸é ¾ÈÀ¸·Î µé¾î¿È
		{
			UI_ShowWindowCursor(); // Ä¿¼­ º¸¿©ÁÖÀå..
		}
		else if( TempValue == -1) // Ä¿¼­°¡ À¥ È­¸é ¹ÛÀ¸·Î ³ª°¬´Ù.
		{
			UI_HiddenWindowCursor(); // Ä¿¼­ ¾Èº¸¿©ÁØ´Ù.
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
			std::ifstream	FinFile;
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
					g_pSoundBufferForOGG = NULL; // SDL backend: CDirectSoundBuffer not implemented
				
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
				g_pOGG->streamPlay(0);
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
			m_pSurface->FillSurface(0);	//	È­¸éÁö¿ì±â
		}
		else if(event->parameter4 < EVENTBACKGROUNDID_MAX)
		{
//			AssertEventBackground(event->parameter4);
			
			POINT p = { 0, 0 };
			RECT r = {0, 0, g_GameRect.left, g_GameRect.top };
//			if(!m_pSurface->Lock()) return;

			CDirectDrawSurface *pSurface = g_pEventManager->GetEventBackground((EVENTBACKGROUND_ID)event->parameter4);

#ifdef PLATFORM_WINDOWS
			m_pSurface->BltNoColorkey(&p, pSurface, &r);
#else
			// SDL backend: Cast CDirectDrawSurface* to CSpriteSurface* for compatibility
			CSpriteSurface* pSpriteSurface = reinterpret_cast<CSpriteSurface*>(pSurface);
			m_pSurface->BltNoColorkey(&p, pSpriteSurface, &r);
#endif
				
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
			std::ifstream	FinFile;
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
					g_pSoundBufferForOGG = NULL; // SDL backend: CDirectSoundBuffer not implemented
				
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
				g_pOGG->streamPlay(0);
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
					
					// CreatureÀÇ Action¿¡ ¸Â´Â add-onÀ» Ãâ·ÂÇÑ´Ù.
					//action = pCreature->GetAction();
					
					WORD clothes;
					BYTE clothesType;

					for (int i=0; i<ADDON_MAX; i++)
					{
						// CreatureÀÇ ÇöÀç ¹æÇâ¿¡ µû¶ó¼­...
						// ¿ÊÀ» Ãâ·ÂÇØÁÖ´Â ¼ø¼­°¡ ´Ù¸¦ ¼ö ÀÖ´Ù.
						clothesType = MCreatureWear::s_AddonOrder[direction][i];
						
						// i¹øÂ° Á¾·ùÀÇ ¿ÊÀ» ÀÔ°í ÀÖ´Ù¸é Ãâ·ÂÇØ ÁØ´Ù.
						const MCreatureWear::ADDON_INFO& addonInfo = pCreatureWear->GetAddonInfo(clothesType);
						
						if (addonInfo.bAddon)
						{
							clothes = addonInfo.FrameID;
							
							FRAME_ARRAY &FA = m_AddonFPK[clothes][action][direction];
							
							// ÀÖ´Â µ¿ÀÛÀÎ °æ¿ì
							if (FA.GetSize() > frame)
							{
								CFrame &Frame = FA[frame];					
								int sprite	= Frame.GetSpriteID();	//m_AddonFPK[clothes][action][direction][frame].GetSpriteID();
								int cx		= Frame.GetCX();	//m_AddonFPK[clothes][action][direction][frame].GetCX();
								int cy		= Frame.GetCY();	//m_AddonFPK[clothes][action][direction][frame].GetCY();
								
								
								CIndexSprite* pSprite = &m_AddonSPK[ sprite ];					
								
								// º¹ÀåSprite°¡ ÃÊ±âÈ­ µÇÁö ¾ÊÀº °æ¿ì
//									if (pSprite->IsNotInit())
//									{
//										LoadFromFileAddonSPK( clothes, action );
//									}
								
								
								POINT pointTemp;
								
								// ÁÂÇ¥ º¸Á¤
								pointTemp.x = 384+cx;
								pointTemp.y = 312+cy;
								
								{
									int colorSet1 = addonInfo.ColorSet1, colorSet2 = addonInfo.ColorSet2;
									if(colorSet2 == UNIQUE_ITEM_COLOR || colorSet2 == QUEST_ITEM_COLOR)
									{
										// À¯´ÏÅ© ¾ÆÀÌÅÛÀÌ¸é
										//									if(colorSet2 == QUEST_ITEM_COLOR)
										//										colorSet2 = MItem::GetQuestItemColorset();
										//									else
										//										colorSet2 = MItem::GetUniqueItemColorset();										
										
										colorSet2 = MItem::GetSpecialColorItemColorset( colorSet2 );
										if(addonInfo.ItemClass != ITEM_CLASS_COAT && addonInfo.ItemClass != ITEM_CLASS_TROUSER)
											colorSet1 = colorSet2;
									}
									
									CIndexSprite::SetUsingColorSet( colorSet1, colorSet2 );
									
									// ¾îµÓ°Ô Âï±â
									if (pCreature->IsFade())
									{
										m_pSurface->BltIndexSpriteDarkness(&pointTemp, pSprite, 1);
									}
									else
									{
										m_pSurface->BltIndexSprite(&pointTemp, pSprite);
									}
								}						
								
								// Á¤»óÀûÀÎ Ãâ·Â
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
						
						// ÁÂÇ¥ º¸Á¤
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
